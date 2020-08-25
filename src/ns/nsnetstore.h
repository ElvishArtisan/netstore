// nsnetstore.h
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

#ifndef NSNETSTORE_H
#define NSNETSTORE_H

#include <curl/curl.h>

#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QStringList>

#include <nsnetstoreobject.h>

#define NSNETSTORE_AUTH_ENCRYPTION_FORMAT 5

class NSNetStore : public QObject
{
 Q_OBJECT
 public:
 enum Action {Delete=0,Dir=1,Download=2,Du=3,List=4,Mkdir=5,Mtime=6,
	      QuickDelete=7,Rename=8,Rmdir=9,Stat=10,Symlink=11,Upload=12,
	      LastAction=13};
  NSNetStore(const QString &hostname,const QString &cp_code,
	     const QString &keyname,const QString &secret,QObject *parent=0);
  NSNetStore(QObject *parent=0);
  QString hostName() const;
  void setHostName(const QString &str);
  QString cpCode() const;
  void setCpCode(const QString &str);
  QString keyName() const;
  void setKeyName(const QString &str);
  QString secret() const;
  void setSecret(const QString &str);
  bool dir(QList<NSNetStoreObject> &objs,const QString &path);
  static QString sigHeader1(const QDateTime &dt,unsigned unique_id,
			    const QString &keyname);
  static QString sigHeader2(const QString &hdr1,Action act,const QString &path,
			    const QString &secret,
			    const QStringList &hdrs=QStringList());
  static QString actionText(Action act);

 private:
  static QString GetLastSslErrorString();
  struct curl_slist *AddAuthHeaders(struct curl_slist *headers,
				    Action act,const QString &path,
				    const QStringList &hdrs=QStringList())
    const;
  QString c_host_name;
  QString c_cp_code;
  QString c_key_name;
  QString c_secret;
  friend size_t __NSNetStore_DownloadCB(void *ptr,size_t size,size_t nmemb,
				     void *userdata);
  QByteArray c_cb_data;
};


#endif  // NSNETSTORE_H
