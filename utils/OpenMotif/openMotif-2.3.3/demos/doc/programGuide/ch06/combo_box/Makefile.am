MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = Imakefile README combo_box.man

noinst_PROGRAMS = combo_box

combo_box_SOURCES = combo_box.c interface.c


INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib -I$(top_srcdir)/demos/lib $(X_CFLAGS)

LDADD = -L$(top_builddir)/demos/lib/Xmd -lXmd $(top_builddir)/lib/Xm/libXm.la
