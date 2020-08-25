// nsnetstore.cpp
//
// Client support for Akamai NetStore 4
//
//   (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#include "nsnetstore.h"

//
// CURL Callbacks
//
size_t __NSNetStore_DownloadCB(void *ptr,size_t size,size_t nmemb,void *userdata)
{
  QByteArray *data=(QByteArray *)userdata;
  *data+=QByteArray((const char *)ptr,size*nmemb);

  printf("DATA: %s\n",data->constData());

  return size*nmemb;
}

NSNetStore::NSNetStore(const QString &hostname,const QString &cp_code,
		       const QString &keyname,const QString &secret,
		       QObject *parent)
  : QObject(parent)
{
  c_host_name=hostname;
  c_cp_code=cp_code;
  c_key_name=keyname;
  c_secret=secret;
}


QString NSNetStore::hostName() const
{
  return c_host_name;
}


void NSNetStore::setHostName(const QString &str)
{
  c_host_name=str;
}


QString NSNetStore::cpCode() const
{
  return c_cp_code;
}


void NSNetStore::setCpCode(const QString &str)
{
  c_cp_code=str;
}


QString NSNetStore::keyName() const
{
  return c_key_name;
}


void NSNetStore::setKeyName(const QString &str)
{
  c_key_name=str;
}


QString NSNetStore::secret() const
{
  return c_secret;
}


void NSNetStore::setSecret(const QString &str)
{
  c_secret=str;
}


bool NSNetStore::dir(QList<NSNetStoreObject> &objs,const QString &path)
{
  QString url;
  long response_code;
  CURL *curl=NULL;
  CURLcode curl_err;
  struct curl_slist *cheaders=NULL;

  if((curl=curl_easy_init())==NULL) {
    return false;
  }
  c_cb_data=QByteArray();

  //
  // Add Custom Headers
  //
  cheaders=
    curl_slist_append(cheaders,
		      (QString("X-Akamai-ACS-Action: ")+
		       "version=1"+
		       "&action=dir"+
		       "&format=xml").toUtf8().constData());
  cheaders=AddAuthHeaders(cheaders,NSNetStore::Dir,path);

  url=QString("http://")+c_host_name+"-nsu.akamaihd.net/"+c_cp_code+path;
  curl_easy_setopt(curl,CURLOPT_URL,url.toUtf8().constData());
  printf("URL: %s\n",url.toUtf8().constData());
  curl_easy_setopt(curl,CURLOPT_HTTPHEADER,cheaders);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,__NSNetStore_DownloadCB);
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,&c_cb_data);
  curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
  //  curl_easy_setopt(curl,CURLOPT_USERAGENT,
  //		   conv_config->userAgent().toUtf8().constData());
  //curl_easy_setopt(curl,CURLOPT_TIMEOUT,RD_CURL_TIMEOUT);

  if((curl_err=curl_easy_perform(curl))==CURLE_OK) {
    printf("DOWNLOADED: %s\n",c_cb_data.constData());
  }
  else {
    printf("CURL ERROR\n");
    curl_easy_cleanup(curl);
    return false;
  }

  curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&response_code);
  curl_easy_cleanup(curl);
  curl_slist_free_all(cheaders);

  printf("RESPONSE_CODE: %ld\n",response_code);

  switch(response_code) {
  case 200:
    break;

  case 404:
    return false;

  default:
    return false;
  }

  return true;
}


QString NSNetStore::sigHeader1(const QDateTime &dt,unsigned unique_id,
			       const QString &keyname)
{
  QString ret="";

  ret+=QString().sprintf("%u, ",NSNETSTORE_AUTH_ENCRYPTION_FORMAT);
  ret+="0.0.0.0, ";
  ret+="0.0.0.0, ";
  ret+=QString().sprintf("%u, ",dt.toTime_t());
  ret+=QString().sprintf("%u, ",unique_id);
  ret+=keyname;

  return ret;
}


QString NSNetStore::sigHeader2(const QString &hdr1,NSNetStore::Action act,
			       const QString &path,const QString &secret,
			       const QStringList &hdrs)
{
  QString ret="";
  QString raw="";
  HMAC_CTX ctx;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned md_len=0;

  //
  // Build raw data
  //
  raw+=hdr1;
  raw+=path+"\n";
  raw+="x-akamai-acs-action:version=1&action="+NSNetStore::actionText(act);
  for(int i=0;i<hdrs.size();i++) {
    raw+="&"+hdrs.at(i);
  }
  raw+="\n";

  //
  // Genrate the HMAC-SHA256 hash
  //
  ENGINE_load_builtin_engines();
  ENGINE_register_all_complete();

  HMAC_CTX_init(&ctx);
  if(HMAC_Init_ex(&ctx,secret.toUtf8(),secret.toUtf8().size(),EVP_sha256(),
		  NULL)==0) {
    fprintf(stderr,
	    "NSNetStore::sigHeader2(): HMAC_Init_ex() returned an error [%s]\n",
	    GetLastSslErrorString().toUtf8().constData());
  }
  if(HMAC_Update(&ctx,(const unsigned char *)raw.toUtf8().constData(),
		 raw.toUtf8().size())==0) {
    fprintf(stderr,
	    "NSNetStore::sigHeader2(): HMAC_Update() returned an error [%s]\n",
	    GetLastSslErrorString().toUtf8().constData());
  }
  if(HMAC_Final(&ctx,md_value,&md_len)==0) {
    fprintf(stderr,
	    "NSNetStore::sigHeader2(): HMAC_Final() returned an error [%s]\n",
	    GetLastSslErrorString().toUtf8().constData());
  }
  HMAC_CTX_cleanup(&ctx);

  return QByteArray((const char *)md_value,md_len).toBase64();
}


QString NSNetStore::actionText(NSNetStore::Action act)
{
  switch(act) {
  case NSNetStore::Delete:
    return QString("delete");

  case NSNetStore::Dir:
    return QString("dir");

  case NSNetStore::Download:
    return QString("download");

  case NSNetStore::Du:
    return QString("du");

  case NSNetStore::List:
    return QString("list");

  case NSNetStore::Mkdir:
    return QString("mkdir");

  case NSNetStore::Mtime:
    return QString("mtime");

  case NSNetStore::QuickDelete:
    return QString("quick-delete");

  case NSNetStore::Rename:
    return QString("rename");

  case NSNetStore::Rmdir:
    return QString("rmdir");

  case NSNetStore::Stat:
    return QString("stat");

  case NSNetStore::Symlink:
    return QString("symlink");

  case NSNetStore::Upload:
    return QString("upload");

  case NSNetStore::LastAction:
    break;
  }

  return QString("unknown");
}


QString NSNetStore::GetLastSslErrorString()
{
  char err_str[120];

  ERR_error_string_n(ERR_peek_last_error(),err_str,120);
  return QString(err_str);
}


struct curl_slist *NSNetStore::AddAuthHeaders(struct curl_slist *cheaders,
					      NSNetStore::Action act,
					      const QString &path,
					      const QStringList &hdrs) const
{
  QString hdr1=
    NSNetStore::sigHeader1(QDateTime::currentDateTime(),random(),c_key_name);

  cheaders=curl_slist_append(cheaders,
			    (QString("X-Akamai-ACS-Auth-Data: ")+
			     hdr1).toUtf8().constData());
  cheaders=curl_slist_append(cheaders,
			    (QString("X-Akamai-ACS-Auth-Sign:")+
			     NSNetStore::sigHeader2(hdr1,act,path,c_secret,
						   hdrs)).toUtf8().constData());
  return cheaders;
}
