MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/Tree
datadir = ${prefix}/share/Xm/Tree

EXTRA_DIST = Imakefile
bin_PROGRAMS = tree
tree_SOURCES = bxutil-c.c  tree.c
data_DATA = $(EXTRA_DIST) $(tree_SOURCES)
TOPDIR = ../../..
LDADD = $(TOPDIR)/lib/Xm/libXm.la

INCLUDES = -I$(TOPDIR)/lib -I$(srcdir)/.. -I$(srcdir)/$(TOPDIR)/lib ${X_CFLAGS} 
