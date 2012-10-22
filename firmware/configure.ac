#                                               -*- Autoconf -*-
# This file have been create using configure.ac from avr-libc project 
# Copyright (c) 2004,  Theodore A. Roth
# Copyright (c) 2005,2006,2007,2009  Anatoly Sokolov
# Copyright (c) 2005,2008  Joerg Wunsch
#
# ComEOS project
# Copyright (c) 2008,2009,  Romain Tartière
# Copyright (c) 2008,2009,  Carlos Carpentier
# Copyright (c) 2008,2009,  Romuald Conty
# 
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# Process this file with autoconf to produce a configure script.
#

dnl library versioning -- modify these on releases
dnl        macro name           value            description
m4_define([comeos_major],    [0])dnl           major version
m4_define([comeos_minor],    [9])dnl           minor version
m4_define([comeos_revision], [1])dnl           revision (dot-dot version)
m4_define([comeos_reldate],  [20090517])dnl    release date
dnl end of library versioning data

dnl odd minor number marks a development branch, append date to version there
m4_if(m4_eval(comeos_minor % 2 == 1), [0], [dnl
  m4_define([comeos_version],
            comeos_major.comeos_minor.comeos_revision)dnl
  ],[dnl
  m4_define([comeos_version],
            comeos_major.comeos_minor.comeos_revision.comeos_reldate)dnl
  ])dnl
m4_define([comeos_version_numeric],
          m4_eval(10000 * comeos_major + dnl
                    100 * comeos_minor + dnl
                          comeos_revision))dnl

AC_INIT(ComEOS, comeos_version, comeos@comeos.org)

AC_PREREQ(2.59)
AC_REVISION($Revision: 1.86 $)

AC_CONFIG_SRCDIR([src/comeos.c])
AC_CONFIG_HEADER([src/config.h])

dnl substitute M4 macros into shell variables
COMEOS_MAJOR=comeos_major
COMEOS_MINOR=comeos_minor
COMEOS_REVISION=comeos_revision
COMEOS_RELDATE=comeos_reldate
COMEOS_VERSION=comeos_version
COMEOS_VERSION_NUMERIC=comeos_version_numeric
dnl ...and trigger Makefile.in substitutions
AC_SUBST(COMEOS_MAJOR)
AC_SUBST(COMEOS_MINOR)
AC_SUBST(COMEOS_REVISION)
AC_SUBST(COMEOS_RELDATE)
AC_SUBST(COMEOS_VERSION)
AC_SUBST(COMEOS_VERSION_NUMERIC)

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# See if the user specified '--build=`../config.guess` --host=avr' when
# running ../configure.

AC_MSG_CHECKING([if configuring for cross compile])
if test "x${build_alias}" = "x${host_alias}"; then
	if test "x${build_alias}" = "x" ; then
		build_alias='`./config.guess`'
	fi
	AC_MSG_RESULT(no)
	AC_MSG_WARN()
	AC_MSG_WARN(ComEOS must be built using an avr cross-compiler.)
	AC_MSG_WARN(Try configuring with:)
	AC_MSG_WARN("$0 --build=$build_alias --host=avr")
	AC_MSG_WARN()
	AC_MSG_ERROR(aborting configure)
else
	AC_MSG_RESULT([yes])
fi

AC_MSG_CHECKING([if target host is avr])
case "$host" in
	avr* )
		AC_MSG_RESULT([yes])
		;;
	* )
		AC_MSG_RESULT(no)
		AC_MSG_WARN()
		AC_MSG_WARN(ComEOS must be built using an avr cross-compiler.)
		AC_MSG_WARN(Try configuring with:)
		AC_MSG_WARN("$0 --build=$build --host=avr")
		AC_MSG_WARN()
		AC_MSG_ERROR(aborting configure)
		;;
esac

AC_CHECK_HEADERS([avr/io.h],[],AC_MSG_ERROR(avr libc not found but required))

## TODO: Write a check for GNU Make

dnl The default check whether the C compiler can create an executable
dnl is inappropriate for us as it requires an existing library.
# AC_NO_EXECUTABLES

dnl Minimum version of Automake is 1.8.
dnl We don't want the gzip distribution tarball anymore.
AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-dist-gzip])

# We don't want touse the cflags from the environment since we need control
# of this when we're building the firmware.
CFLAGS="-funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99 -mmcu=atmega8 -mcall-prologues -Wstrict-prototypes -Wall -pedantic -Os -DF_CPU=1000000UL"

# Checks for programs.

AC_PROG_CC
AC_CHECK_TOOL(AS, as, as)
AM_PROG_AS
# AC_PROG_RANLIB
AC_CHECK_TOOL(AR, ar, ar)
AC_CHECK_TOOL(OBJCOPY, objcopy, objcopy)

# Make sure that we found the right avr cross-compiler.
case "${CC}" in
   *avr-gcc*) ;;
   *) AC_MSG_ERROR(Wrong C compiler found; check the PATH!) ;;
esac
case "${AS}" in
   *avr-as*) ;;
   *) AC_MSG_ERROR(Wrong assembler found; check the PATH!) ;;
esac
case "${AR}" in
   *avr-ar*) ;;
   *) AC_MSG_ERROR(Wrong archiver found; check the PATH!) ;;
esac
case "${OBJCOPY}" in
   *avr-objcopy*) ;;
   *) AC_MSG_ERROR(Wrong object copier found; check the PATH!) ;;
esac

AVROBJCOPY=${OBJCOPY}
AC_SUBST(AVROBJCOPY)

# case "${RANLIB}" in
#    *avr-ranlib*) ;;
#    *) AC_MSG_ERROR(Wrong ranlib found; check the PATH!) ;;
# esac

# AC_PROG_INSTALL
# AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl TODO: Check for various tools used to build the documentation. If anything
dnl is missing, don't build the docs by default.

dnl Disable building of dox by default since it they may not build properly on
dnl all systems. This isn't optimal, but how do we work around the user not
dnl having fig2dev and still build all the formats?

# dnl
# dnl @synopsis CHECK_PNG_UTILS
# dnl
# dnl This macro checks if pngtopnm and pnmtopng are installed. If they are not
# dnl installed we just fake it with "cat".
# dnl
# dnl We use these in the dox to insert transparency into the png images as such:
# dnl
# dnl   $ pngtopnm foo.png > tmp.pnm
# dnl   $ pnmtopng -transparent white tmp.pnm > foo.png
# dnl
# dnl @version 
# dnl @author Theodore A. Roth <troth@openavr.org> and Eric B. Weddington <eweddington@cso.atmel.com>
# dnl
# AC_DEFUN([CHECK_PNG_UTILS],[dnl
# dnl
# AC_MSG_CHECKING([for pngtopnm])
# has_pngtopnm=`pngtopnm --version 2>&1 | grep -c -i Version`
# if test "$has_pngtopnm" = "1"; then
# 	AC_MSG_RESULT(yes)
# 	PNGTOPNM="pngtopnm"
# else
# 	AC_MSG_RESULT(no)
# 	PNGTOPNM="cat"
# fi
# dnl
# AC_MSG_CHECKING([for pnmtopng])
# has_pnmtopng=`pnmtopng --version 2>&1 | grep -c -i Version`
# if test "$has_pnmtopng" = "1"; then
# 	AC_MSG_RESULT(yes)
# 	PNMTOPNG="pnmtopng"
# else
# 	AC_MSG_RESULT(no)
# 	PNMTOPNG="cat"
#     PNGTOPNM="cat"
# fi
# AC_SUBST(PNGTOPNM)
# AC_SUBST(PNMTOPNG)
# ])dnl
# 
# CHECK_PNG_UTILS

AM_CONDITIONAL(HAS_atmega32, true)

# Generate all files from *.in sources.

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT
