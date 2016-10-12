MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = Imakefile README UTMXmText.man

noinst_PROGRAMS = UTMXmText

UTMXmText_SOURCES = UTMXmText.c


INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_srcdir)/demos/lib $(X_CFLAGS)

LDADD = -L$(top_builddir)/demos/lib/Xmd -lXmd $(top_builddir)/lib/Xm/libXm.la
