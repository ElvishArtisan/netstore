dnl configure.ac
dnl
dnl Autoconf configuration for NetStore.
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2020 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT([netstore],m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE([1.9 tar-pax])
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

#
# API Library Versioning
#
# From http://www.gnu.org/software/libtool/manual.html#Updating-version-info
#
#   1. Start with version information of 0:0:0 for each libtool library.
#   2. Update the version information only immediately before a public
#      release of your software. More frequent updates are unnecessary,
#      and only guarantee that the current interface number gets larger
#      faster.
#   3. If the library source code has changed at all since the last update,
#      then increment 'REVISION' (c:r:a becomes c:r+1:a).
#   4. If any interfaces have been added, removed, or changed since the last
#      update, increment 'CURRENT', and set 'REVISION' to 0.
#   5. If any interfaces have been added since the last public release, then
#      increment 'AGE'.
#   6. If any interfaces have been removed since the last public release,
#      then set 'AGE' to 0.
CURRENT=0
REVISION=0
AGE=0
POINT=$(($CURRENT - $AGE))

AC_SUBST(INTERFACE_CURRENT,$CURRENT)
AC_SUBST(INTERFACE_REVISION,$REVISION)
AC_SUBST(INTERFACE_AGE,$AGE)
AC_SUBST(INTERFACE_MAJOR,$POINT)
AC_SUBST(INTERFACE_MINOR,$AGE)
AC_SUBST(INTERFACE_POINT,$REVISION)

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)

# to avoid rpath usage :
# http://wiki.debian.net/index.cgi?RpathIssue
case ${host} in
  *-pc-linux-gnu)
    AC_MSG_RESULT([Fixing libtool for -rpath problems.])
    sed < libtool > libtool-2 \
    's/^hardcode_libdir_flag_spec.*$/hardcode_libdir_flag_spec=" -D__LIBTOOL_IS_A_FOOL__ "/'
    mv libtool-2 libtool
    chmod 755 libtool
  ;;
esac


#
# Check for Qt4 (Mandatory)
#
PKG_CHECK_MODULES(QT4,QtCore QtXml,,[AC_MSG_ERROR([*** Qt4 not found ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt4,[moc-qt4],[moc])
AC_SUBST(QT_MOC,$MOC_NAME)


#
# Check for OpenSSL
#
AC_CHECK_HEADER(openssl/sha.h,[],[AC_MSG_ERROR([*** OpenSSL not found ***])])


#
# Check for LibCurl
#
PKG_CHECK_MODULES(LIBCURL,libcurl,,[AC_MSG_ERROR([*** LibCurl not found ***])])


#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine number of available CPU cores for building packages
#
AX_COUNT_CPUS([],[])
if test $CPU_COUNT -lt 2 ; then
   AC_SUBST(CPUS_AVAIL,1)
else
   if test $CPU_COUNT -gt 16 ; then
      AC_SUBST(CPUS_AVAIL,16)
   else
      AC_SUBST(CPUS_AVAIL,$(($CPU_COUNT-1)))
   fi
fi

#
# Determine Distro
#
AR_GET_DISTRO()
AC_SUBST(DISTRO,$ar_gcc_distro)

#
# Check for OpenSSL
#
AC_CHECK_HEADER(openssl/sha.h,[],[AC_MSG_ERROR([*** OpenSSL not found ***])])

#
# DocBook
#
if test -z $DOCBOOK_DISABLED ; then
  USING_DOCBOOK=yes
fi
AM_CONDITIONAL([DOCBOOK_AM], [test "$USING_DOCBOOK" = yes])
if test $DOCBOOK_STYLESHEETS ; then
   ln -s $DOCBOOK_STYLESHEETS helpers/docbook
fi

#AC_SUBST(DOCBOOK_STYLESHEETS,$DOCBOOK_STYLESHEETS)

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
dnl  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
dnl  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 
AC_SUBST(RPM_DATESTAMP,`date +%a\ %b\ %d\ %Y`)

AC_CONFIG_FILES([\
    src/Makefile \
    src/ns/Makefile \
    src/tests/Makefile \
    src/tests/manual/Makefile \
    netstore.spec \
    Makefile \
    ])
AC_OUTPUT()

#
# Configuration Results
#
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
