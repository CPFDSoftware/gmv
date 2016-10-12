MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/Tabstack
datadir = ${prefix}/share/Xm/Tabstack

bin_PROGRAMS = tabstack
tabstack_SOURCES = bxutils.c callbacks.c creation.c main.c creation.h
EXTRA_DIST = Imakefile
data_DATA = $(tabstack_SOURCES) $(EXTRA_DIST)            
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(TOPDIR)/lib -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
