MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/setDate
datadir = ${prefix}/share/Xm/setDate

bin_PROGRAMS = setDate
EXTRA_DIST = XmdSetDate.ad README setDate.man Imakefile
setDate_SOURCES = setDate.c
data_DATA = $(EXTRA_DIST) $(setDate_SOURCES)
LDADD = ../../../lib/Xm/libXm.la

INCLUDES = -I$(srcdir)/../../../lib -I$(top_builddir)/lib ${X_CFLAGS} 
