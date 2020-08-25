// netstore_test.cpp
//
// Test the Akamai NetStore Integration for Rivendell
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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QStringList>

#include "cmdswitch.h"
#include "datetime.h"
#include "netstore_test.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  QString err_msg;
  bool ok=false;

  c_sig_header_1=false;
  c_sig_headers=false;
  c_dir=false;
  c_unique_id=0;
  c_action=NSNetStore::LastAction;

  //
  // Process command switches
  //
  CmdSwitch *cmd=new CmdSwitch(qApp->argc(),qApp->argv(),"netstore_test",
			       NETSTORE_TEST_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--dir") {
      c_dir=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--sig-header-1") {
      c_sig_header_1=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--sig-headers") {
      c_sig_headers=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--add-header") {
      c_headers.push_back(cmd->value(i));
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--datetime") {
      c_datetime=ParseXmlDateTime(cmd->value(i),&ok);
      if(!ok) {
	fprintf(stderr,
		"netstore_test: invalid --datetime string \"%s\"",cmd->value(i).toUtf8().constData());
	exit(1);
      }
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--action") {
      for(int j=0;j<NSNetStore::LastAction;j++) {
	NSNetStore::Action action=(NSNetStore::Action)j;
	if(cmd->value(i)==NSNetStore::actionText(action)) {
	  c_action=action;
	  cmd->setProcessed(i,true);
	}
      }
      if(c_action==NSNetStore::LastAction) {
	fprintf(stderr,"netstore_test: unknown action \"%s\" specified\n",
		cmd->value(8).toUtf8().constData());
	exit(1);
      }
    }
    if(cmd->key(i)=="--cp-code") {
      c_cp_code=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--host-name") {
      c_host_name=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--key-name") {
      c_key_name=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--path") {
      c_path=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--secret") {
      c_secret=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--unique-id") {
      c_unique_id=cmd->value(i).toUInt(&ok);
      if(!ok) {
	fprintf(stderr,"invalid --unique-id \"%s\"\n",
		cmd->value(i).toUtf8().constData());
	exit(1);
      }
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"unknown option \"%s\"\n",cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }

  //
  // Run Test
  //
  if(c_dir) {
    if(!PrintDir(&err_msg)) {
      fprintf(stderr,"netstore_test: %s\n",err_msg.toUtf8().constData());
      exit(1);
    }
    exit(0);
  }
  if(c_sig_header_1) {
    if(!PrintSigHeader1(&err_msg)) {
      fprintf(stderr,"netstore_test: %s\n",err_msg.toUtf8().constData());
      exit(1);
    }
    exit(0);
  }
  if(c_sig_headers) {
    if(!PrintSigHeaders(&err_msg)) {
      fprintf(stderr,"netstore_test: %s\n",err_msg.toUtf8().constData());
      exit(1);
    }
    exit(0);
  }

  exit(0);
}


bool MainObject::PrintDir(QString *err_msg)
{
  bool ret=false;

  if(c_host_name.isEmpty()) {
    *err_msg="--host-name=<string> is required";
    return false;
  }
  if(c_cp_code.isEmpty()) {
    *err_msg="--cp-code=<string> is required";
    return false;
  }
  if(c_key_name.isEmpty()) {
    *err_msg="--key-name=<acct-name> is required";
    return false;
  }
  if(c_secret.isEmpty()) {
    *err_msg="--secret=<string> is required";
    return false;
  }
  if(c_path.isEmpty()) {
    *err_msg="--path=<string> is required";
    return false;
  }

  NSNetStore *ns=
    new NSNetStore(c_host_name,c_cp_code,c_key_name,c_secret,this);
  QList<NSNetStoreObject> objs;
  ret=ns->dir(objs,c_path);
  delete ns;

  return ret;
}


bool MainObject::PrintSigHeader1(QString *err_msg) const
{
  if(c_datetime.isNull()) {
    *err_msg="--datetime=yyyy-mm-ddThh:mm:ss required";
    return false;
  }
  if(c_unique_id==0) {
    *err_msg="--unique-id=<unsigned> required";
    return false;
  }
  if(c_key_name.isEmpty()) {
    *err_msg="--key-name=<acct-name> is required";
    return false;
  }

  printf("X-Akamai-ACS-Auth-Data: %s\n",
	 NSNetStore::sigHeader1(c_datetime,c_unique_id,c_key_name).
	 toUtf8().constData());

  return true;
}


bool MainObject::PrintSigHeaders(QString *err_msg) const
{
  if(c_datetime.isNull()) {
    *err_msg="--datetime=yyyy-mm-ddThh:mm:ss required";
    return false;
  }
  if(c_unique_id==0) {
    *err_msg="--unique-id=<unsigned> required";
    return false;
  }
  if(c_key_name.isEmpty()) {
    *err_msg="--key-name=<acct-name> is required";
    return false;
  }
  if(c_secret.isEmpty()) {
    *err_msg="--secret=<string> is required";
    return false;
  }
  if(c_path.isEmpty()) {
    *err_msg="--path=<string> is required";
    return false;
  }
  if(c_action==NSNetStore::LastAction) {
    *err_msg="--action=<act> is required";
    return false;
  }


  QString hdr1=NSNetStore::sigHeader1(c_datetime,c_unique_id,c_key_name);
  printf("X-Akamai-ACS-Auth-Data: %s\n",hdr1.toUtf8().constData());
  printf("X-Akamai-ACS-Auth-Sign: %s\n",
  	 NSNetStore::sigHeader2(hdr1,c_action,c_path,c_secret,c_headers).
	 toUtf8().constData());

  return true;
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();
  return a.exec();
}
