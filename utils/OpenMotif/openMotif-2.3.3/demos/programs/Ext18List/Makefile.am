MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/Ext18list
datadir = ${prefix}/share/Xm/Ext18list

bin_PROGRAMS = ext18list
SUBDIRS = pixmaps
ext18list_SOURCES = util-c.c callbacks-c.c creation.c ext18list.c \
         extlist.h
EXTRA_DIST = Imakefile
data_DATA = $(EXTRA_DIST) $(ext18list_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(TOPDIR)/lib -I.. -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
