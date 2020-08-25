## Makefile.am
##
## (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -I$(top_srcdir)/src/ns @QT4_CFLAGS@ @LIBCURL_CFLAGS@
LIBS = -L$(top_srcdir)/src/ns
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

noinst_PROGRAMS = netstore_test

dist_netstore_test_SOURCES = cmdswitch.cpp cmdswitch.h\
                             datetime.cpp datetime.h\
                             netstore_test.cpp netstore_test.h

nodist_netstore_test_SOURCES = moc_netstore_test.cpp

netstore_test_LDADD = @QT4_LIBS@ @LIBCURL_LIBS@ -lssl -lnetstore

CLEANFILES = *~\
             moc_*

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
