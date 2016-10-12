MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/fontsel
datadir = ${prefix}/share/Xm/fontsel

bin_PROGRAMS = fontsel
fontsel_SOURCES = callbacks-c.c creation-c.c fontsel.c fontsel.h
EXTRA_DIST = Imakefile
data_DATA = $(EXTRA_DIST) $(fontsel_SOURCES)
LDADD = $(TOPDIR)/lib/Xm/libXm.la
TOPDIR = ../../..
INCLUDES = -I$(TOPDIR)/lib -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
