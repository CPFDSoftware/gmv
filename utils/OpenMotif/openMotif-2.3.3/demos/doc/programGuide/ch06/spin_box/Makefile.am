MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = Imakefile README spin_box.man

noinst_PROGRAMS = spin_box

spin_box_SOURCES = spin_box.c interface.c


INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_srcdir)/demos/lib $(X_CFLAGS)

LDADD = -L$(top_builddir)/demos/lib/Xmd -lXmd $(top_builddir)/lib/Xm/libXm.la
