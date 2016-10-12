MAINTAINERCLEANFILES = Makefile.in

bindir = ${prefix}/share/Xm/todo
datadir = ${prefix}/share/Xm/todo

EXTRA_DIST = README XmdTodo.ad example.todo todo.man todo.help Imakefile
bin_PROGRAMS = todo
todo_SOURCES = todo.c io.c actions.c page.h
data_DATA = $(EXTRA_DIST) $(todo_SOURCES)

INCLUDES = -I$(srcdir)/../../../lib -I$(srcdir)/../../lib -I$(top_builddir)/lib ${X_CFLAGS} 
LDADD = -L../../lib/Xmd -lXmd -L../../lib/Exm -lExm ../../../lib/Xm/libXm.la 
