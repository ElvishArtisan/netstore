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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" @QT4_CFLAGS@ -Wno-strict-aliasing
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

lib_LTLIBRARIES = libnetstore.la
dist_libnetstore_la_SOURCES = nsnetstore.cpp nsnetstore.h

nodist_libnetstore_la_SOURCES = moc_nsnetstore.cpp

libnetstore_la_LDFLAGS = @QT4_LIBS@

CLEANFILES = *~\
             moc_*\
             *.lib\
             *.obj\
             *.qm

DISTCLEANFILES = rdpaths.h

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
