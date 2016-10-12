MAINTAINERCLEANFILES = Makefile.in

noinst_PROGRAMS = dainput

dainput_SOURCES = dainput.c dainput_dlg.c dainput_ui.c

LDADD = ../../../lib/Xm/libXm.la

EXTRA_DIST = Imakefile

INCLUDES = -I$(srcdir)/../../../lib -I$(top_builddir)/lib ${X_CFLAGS} 

noinst_HEADERS = dainput.h
