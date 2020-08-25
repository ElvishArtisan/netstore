// nsnetstoreobject.h
//
// Container class for Akamai ObjectStore (NS4) object attributes
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

#ifndef NSNETSTOREOBJECT_H
#define NSNETSTOREOBJECT_H

#include <stdint.h>

#include <QDateTime>
#include <QString>

class NSNetStoreObject
{
 public:
  enum Type {File=0,Symlink=1,Dir=2,LastType=3};
  NSNetStoreObject();
  Type type() const;
  void setType(Type type);
  QString typeText() const;
  QString name() const;
  void setName(const QString &str);
  uint64_t size() const;
  void setSize(uint64_t bytes);
  QDateTime modifiedDateTime() const;
  void setModifiedDateTime(const QDateTime &dt);
  QString md5Sum() const;
  void setMd5Sum(const QString &hash);
  bool isImplicit() const;
  void setImplicit(bool state);
  bool isType(const NSNetStoreObject &other);
  static QString typeText(Type type);

 private:
  Type c_type;
  QString c_name;
  uint64_t c_size;
  QDateTime c_modified_datetime;
  QString c_md5_sum;
  bool c_implicit;
};


#endif  // NSNETSTOREOBJECT_H
