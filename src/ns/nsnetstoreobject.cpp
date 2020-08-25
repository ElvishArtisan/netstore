// nsnetstoreobject.cpp
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

#include "nsnetstoreobject.h"

NSNetStoreObject::NSNetStoreObject()
{
  c_type=NSNetStoreObject::LastType;
  c_size=0;
  c_implicit=false;
}


NSNetStoreObject::Type NSNetStoreObject::type() const
{
  return c_type;
}


void NSNetStoreObject::setType(NSNetStoreObject::Type type)
{
  c_type=type;
}


QString NSNetStoreObject::typeText() const
{
  return NSNetStoreObject::typeText(c_type);
}


QString NSNetStoreObject::name() const
{
  return c_name;
}


void NSNetStoreObject::setName(const QString &str)
{
  c_name=str;
}


uint64_t NSNetStoreObject::size() const
{
  return c_size;
}


void NSNetStoreObject::setSize(uint64_t bytes)
{
  c_size=bytes;
}


QDateTime NSNetStoreObject::modifiedDateTime() const
{
  return c_modified_datetime;
}


void NSNetStoreObject::setModifiedDateTime(const QDateTime &dt)
{
  c_modified_datetime=dt;
}


QString NSNetStoreObject::md5Sum() const
{
  return c_md5_sum;
}


void NSNetStoreObject::setMd5Sum(const QString &hash)
{
  c_md5_sum=hash;
}


bool NSNetStoreObject::isImplicit() const
{
  return c_implicit;
}


void NSNetStoreObject::setImplicit(bool state)
{
  c_implicit=state;
}


bool NSNetStoreObject::isType(const NSNetStoreObject &other)
{
  return other.type()==c_type;
}


QString NSNetStoreObject::typeText(NSNetStoreObject::Type type)
{
  QString ret="unknown";

  switch(type) {
  case NSNetStoreObject::File:
    ret="file";
    break;

  case NSNetStoreObject::Symlink:
    ret="symlink";
    break;

  case NSNetStoreObject::Dir:
    ret="dir";
    break;

  case NSNetStoreObject::LastType:
    break;
  }

  return ret;
}
