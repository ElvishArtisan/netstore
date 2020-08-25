// netstore_test.h
//
// Test the Akamai NetStore Integration
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

#ifndef NETSTORE_TEST_H
#define NETSTORE_TEST_H

#include <cmdswitch.h>
#include <nsnetstore.h>

#include <QDateTime>
#include <QObject>
#include <QStringList>

#define NETSTORE_TEST_USAGE "[options]\n\nTest the NSNetStore class\n\nOptions are:\n--dir --host-name=<string> --cp-code=<string> --key-name=<acct-name> --secret=<string> --path=<path>\n     Get directory\n\n--sig-header-1 --datetime=yyyy-mm-ddThh:mm:ss --unique-id=<unsigned> --key-name=<acct-name>\n     Print Signature Header 1.\n\n--sig-headers --datetime=yyyy-mm-ddThh:mm:ss --unique-id=<unsigned> --key-name=<acct-name> --secret=<string> --action=<act> --path-<str> [add-header=<str> [...]]\n     Print both Signature Headers.\n\n"

class MainObject : public QObject
{
  Q_OBJECT;
 public:
  MainObject(QObject *parent=0);

 private:
  bool PrintDir(QString *err_msg);
  bool PrintSigHeader1(QString *err_msg) const;
  bool PrintSigHeaders(QString *err_msg) const;
  bool c_sig_header_1;
  bool c_sig_headers;
  bool c_dir;
  QDateTime c_datetime;
  unsigned c_unique_id;
  QString c_host_name;
  QString c_cp_code;
  QString c_key_name;
  QString c_secret;
  NSNetStore::Action c_action;
  QString c_path;
  QStringList c_headers;
};


#endif  // NETSTORE_TEST_H
