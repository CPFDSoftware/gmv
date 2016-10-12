MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = $(BUILT_SOURCES)
EXTRA_DIST = Mrm.msg Imakefile

INCLUDES = -I$(srcdir)/.. -I.. -I../Xm ${X_CFLAGS}

mrmdir = $(includedir)/Mrm

libMrm_la_LIBADD = ../Xm/libXm.la ${X_LIBS} -lXt ${X_PRE_LIBS} -lX11 ${X_EXTRA_LIBS}

libMrm_la_LDFLAGS = -version-info @CURRENT@:@REVISION@:@AGE@

lib_LTLIBRARIES = libMrm.la

noinst_HEADERS = IDB.h \
Mrm.h \
MrmMsgI.h \
MrmWidget.h

mrm_HEADERS = MrmDecls.h	MrmPublic.h	MrmAppl.h	MrmosI.h

SRCS =	MrmIbuffer.c	MrmIentry.c	MrmIfile.c	MrmIheader.c \
	MrmIindex.c	MrmIindexw.c	MrmIrid.c	MrmIswap.c \
	MrmItop.c	MrmItopw.c	Mrmappl.c \
	Mrmcontext.c	Mrmerror.c	Mrmhier.c	Mrmicon.c \
	Mrminit.c	Mrmlread.c	Mrmlwrite.c	Mrmmodule.c \
	Mrmos.c \
	Mrmptrlist.c	Mrmtable.c	Mrmtime.c	Mrmvm.c \
	Mrmwci.c	Mrmwcrw.c	Mrmwcrwr.c	Mrmwread.c \
	Mrmwrefs.c	Mrmwvalues.c	Mrmwwrite.c	MrmMessages.c

libMrm_la_SOURCES = $(SRCS)

BUILT_SOURCES = $(CAT_FILES)

Makefile: $(BUILT_SOURCES)

if MessageCatalog

CAT_FILES = Mrm.cat MrmMsgCatI.h

$(CAT_FILES): Mrm.msg
	$(mkinstalldirs) $(top_builddir)/localized/C/msg
	$(top_builddir)/localized/util/mkcatdefs MrmMsgCatI.h $(srcdir)/Mrm.msg >$(top_builddir)/localized/C/msg/Mrm.msg
	gencat Mrm.cat $(top_builddir)/localized/C/msg/Mrm.msg

    SPLINT_EXTRAFLAGS =  -I/usr/include -I. +posixlib -nestcomment
else
    SPLINT_EXTRAFLAGS =  -I/usr/include -I. -DNO_MESSAGE_CATALOG +posixlib -nestcomment
endif

splint:
	$(SPLINT) $(INCLUDES) $(SPLINT_EXTRAFLAGS) $(top_srcdir)/lib/Mrm/*.c
