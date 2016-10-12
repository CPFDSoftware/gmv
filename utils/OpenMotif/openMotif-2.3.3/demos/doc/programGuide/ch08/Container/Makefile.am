MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = Imakefile README container.man

noinst_PROGRAMS = container

container_SOURCES = container.c  interface.c

INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_srcdir)/demos/lib $(X_CFLAGS)

LDADD = -L$(top_builddir)/demos/lib/Xmd -lXmd $(top_builddir)/lib/Xm/libXm.la
