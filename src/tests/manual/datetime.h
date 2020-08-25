// datetime.h
//
// Parse and write dates/times in various standard formats. 
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

#ifndef DATETIME_H
#define DATETIME_H

#include <qstring.h>
#include <qdatetime.h>

//
// Function Prototypes
//

//
// Auto-detect the format (XML xs:dateTime or RFC822)
//
QDateTime ParseDateTime(const QString &str,bool *ok);

//
// XML xs:date format
//
QDate ParseXmlDate(const QString &str,bool *ok);
QString WriteXmlDate(const QDate &date);

//
// XML xs:time format
//
QTime ParseXmlTime(const QString &str,bool *ok,int *day_offset=NULL);
QString WriteXmlTime(const QTime &time);

//
// XML xs:dateTime format
//
QDateTime ParseXmlDateTime(const QString &str,bool *ok);
QString WriteXmlDateTime(const QDateTime &dt);

//
// RFC822 date/time format
//
QDateTime ParseRfc822DateTime(const QString &str,bool *ok);
QString WriteRfc822DateTime(const QDateTime &dt);

//
// Returns the UTC offset of the curently configured timezone (seconds)
//
int TimeZoneOffset();


#endif  // DATETIME_H
