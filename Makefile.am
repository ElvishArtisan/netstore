## Makefile.am
##
## Top level automake.am for NetStore
##
##   (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
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
## Use automake to process this into a Makefile.in

SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4

maintainer-clean-local:	
	rm -rf m4

rpm:	dist
	mkdir -p $(HOME)/rpmbuild/SOURCES
	cp netstore-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux netstore.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/netstore-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/RPMS/noarch/netstore-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/netstore-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/netstore-$(VERSION).tar.gz

EXTRA_DIST = autogen.sh\
             CODINGSTYLE\
             get_distro.pl\
             get_target.sh\
             INSTALL-WINDOWS\
             PACKAGE_VERSION\
             netstore.ism\
             netstore.pro\
             netstore.spec.in\
             UPGRADING

CLEANFILES = *~\
             *.mo\
             *.rpm\
             *.tar.gz\
             *.tgz\
             *.zip\
             py-compile

DISTCLEANFILES = config.guess\
                 config.sub\
                 ltmain.sh\
                 netstore\
                 netstore.spec

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       Makefile.in\
                       config.guess\
                       config.status\
                       config.sub\
                       configure\
                       depcomp\
                       install-sh\
                       libtool\
                       ltmain.sh\
                       missing\
                       mkinstalldirs\
                       netstore.spec
