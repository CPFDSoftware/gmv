MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/Outline
datadir = ${prefix}/share/Xm/Outline

EXTRA_DIST = Imakefile
bin_PROGRAMS = outline
outline_SOURCES = outline.c
data_DATA = $(outline_SOURCES) $(EXTRA_DIST)
LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(TOPDIR)/lib -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
