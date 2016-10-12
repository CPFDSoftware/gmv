OBJSO =  main.o readgmv.o get_statistics.o initstuff.o gmvread.o \
	init_motif.o callbacks.o drawit.o drawaxes.o savescreen.o cbtun.o \
	drawpolygons.o drawpolylines.o drawnodepoints.o \
        drawcellfaces.o drawcellhiddenedges.o drawvectors.o centerform.o \
	subsetform.o matorderform.o snapshotform.o scaleaxisform.o \
	polysubsetform.o isosurfform.o makeisosurf.o \
	drawisosurface.o cutplaneform.o makecutplane.o \
	drawcutplane.o coloreditform.o attributes.o writevars.o \
        animationform.o polygonform.o drawcelledges.o nodeform.o \
        cellform.o nodeselform.o cellselform.o cellexpform.o \
        drawanimvelflow.o gridanalform.o traceform.o drawtracers.o \
	icosadraw.o plotboxform.o imagestuff.o fieldlimform.o \
        tracelimform.o drawcolorbar.o boundingboxform.o drawboundingbox.o \
	drawsubsetbox.o animflyform.o vectbldform.o tfldcalcform.o \
        cutlineform.o makecutline.o drawcutline.o fldcalcform.o \
        queryform.o nfieldform.o cfieldform.o cfieldlimform.o \
	cfldcalcform.o cvectbldform.o fldfiles_io.o isovolform.o \
	makeisovol.o pntsizeform.o autordform.o readgmvamr.o gen_amr.o \
	stereoform.o linesizeform.o fcoloreditform.o SciPlot.o \
	cutsphereform.o makecutsphere.o backgrndform.o gmvrc_io.o \
	vectorctlform.o surfform.o surfselform.o surfexpform.o \
	svectbldform.o sfieldform.o sfieldlimform.o sfldcalcform.o \
	drawsurfaces.o drawsurfedges.o rbzoomform.o nfldavgform.o \
	cfldavgform.o drawelem.o misc_ctlform.o windowsizeform.o \
	ffieldform.o ffieldlimform.o fvectbldform.o traceselform.o \
	trfieldform.o readgmvray.o rayform.o drawrays.o raylimform.o \
	concaveform.o readgmvinfo.o matcolorbarform.o readexodus.o \
	readx3d.o x3dread.o Ball.o BallAux.o BallMath.o GLwDrawA.o GLwMDrawA.o

#----------------------
# Set compiler options.

CCLNK = cc
MESAHOME = ../utils/Mesa/Mesa-7.8.2
GLWHOME = $(MESAHOME)/include/GL
MOTIFHOME = ../utils/OpenMotif/openMotif-2.3.3
JPEGHOME = ../utils/JPEG
EXODUS = ../utils/ExodusII
EXOLNK = ../utils/ExodusII/lib/libexoIIv2c.a
NETLNK = ../utils/ExodusII/lib/libnetcdf.a
EXOLNK_64 = ../utils/ExodusII/lib64/libexoIIv2c.a
NETLNK_64 = ../utils/ExodusII/lib64/libnetcdf.a

ifeq ($(debug),on)
  DEBUGFLAG     = -g -DDEBUG
  STRIPGMV64MESA  =
  STRIPGMV64GL    =
  STRIPGMV64BATCH =
  STRIPGMV64GL_CYGWIN =
  STRIPGMV64BATCH_CYGWIN =
else
  DEBUGFLAG     =
  STRIPGMV64MESA  = strip linux64Mesa
  STRIPGMV64GL    = strip linux64ogl
  STRIPGMV64BATCH = strip linux64Batch
  STRIPGMV64GL_CYGWIN = strip cygwinogl
  STRIPGMV64BATCH_CYGWIN = strip cygwinBatch
endif

#linux opengl                  - use make linuxogl2
MESAHOME9 = ../../Mesa/Mesa6.4.2
GLWHOME9 = $(MESAHOME9)/include/GL
MOTIFHOME9 = /usr/X11R6/
JPEGHOME9 = ../../JPEG
CCLINE9 = gcc -c -O3 -funroll-loops -mieee-fp -DLINUX -I$(JPEGHOME9)/include -I$(GLWHOME9) -I$(MOTIFHOME9)/include -I$(EXODUS)/include
LIBS9 = -L$(JPEGHOME9)/lib -L$(MOTIFHOME9)/lib \
-lGL -lGLU $(MESAHOME9)/lib/libGLw.a \
/usr/X11R6/lib/libXm.a -lXt -lXmu -lXext -lXpm -lX11 -lm -ljpeg $(EXOLNK) $(NETLNK)
OBJS9 = $(OBJSO)
CCLNK9 = gcc -O3

#linux opengl with Mesa 	- use make linuxmesa
MESAHOME10 = ../../Mesa/Mesa6.4.2
GLWHOME10 = $(MESAHOME10)/include/GL
MOTIFHOME10 = /usr/X11R6/
JPEGHOME10 = ../../JPEG
CCLINE10 = gcc -c -O3 -funroll-loops -mieee-fp -DLINUX -DMESA \
-I$(JPEGHOME10)/include -I$(MESAHOME10)/include -I$(GLWHOME10) -I$(MOTIFHOME10)/include -I$(EXODUS)/include
LIBS10 = -L$(JPEGHOME10)/lib -L$(MESAHOME10)/lib -L$(MOTIFHOME10)/lib \
$(MESAHOME10)/lib/libGL.a -lGLU $(MESAHOME10)/lib/libGLw.a \
/usr/X11R6/lib/libXm.a -lXt -lXmu -lXext -lXpm -lX11 -lm -lpthread -ljpeg $(EXOLNK) $(NETLNK)
OBJS10 = $(OBJSO)
CCLNK10 = gcc -O3

#linux opengl with Mesa 	- use make linuxbatch
MESAHOME33 = ../../Mesa/Mesa6.4.2
GLWHOME33 = $(MESAHOME33)/include/GL
MOTIFHOME33 = /usr/X11R6/
JPEGHOME33 = ../../JPEG
CCLINE33 = gcc -c -O3 -funroll-loops -mieee-fp -DLINUX -DMESA -DBATCH \
-I$(JPEGHOME33)/include -I$(MESAHOME33)/include -I$(GLWHOME33) -I$(MOTIFHOME33)/include -I$(EXODUS)/include 
LIBS33 = -L$(JPEGHOME33)/lib -L$(MESAHOME33)/lib -L$(MOTIFHOME33)/lib \
-lGL -lGLU -lGLw -lOSMesa \
-lXm -lXt -lXmu -lXext -lXpm -lX11 -lm -lpthread -ljpeg $(EXOLNK) $(NETLNK)
OBJS33 = $(OBJSO)
CCLNK33 = gcc -O3

#linux 64 bit opengl                  - use make linux64ogl2
CCLINE21 = gcc $(DEBUGFLAG) -c -O3 -m64 -funroll-loops -mieee-fp -DLINUX -I/usr/include -I$(JPEGHOME)/include -I$(GLWHOME) -I$(MOTIFHOME)/include -I$(EXODUS)/include -I.
LIBS21 = -L$(JPEGHOME)/lib64 -L/usr/lib64 -L/usr/X11R6/lib64 -L$(MOTIFHOME)/lib64 \
-lGL -lGLU \
-lpng -ljpeg -lXm -lXt -lXft -lXmu -lSM -lICE -lXrender -lfontconfig -lfreetype -lXext -lXpm -lm -lX11 -lpthread -ldl \
$(EXOLNK_64) $(NETLNK_64)
OBJS21 = $(OBJSO)
CCLNK21 = gcc $(DEBUGFLAG) -O3 -m64

#linux 64 bit opengl with Mesa 	       - use make linux64mesa
CCLINE22 = gcc $(DEBUGFLAG) -c -O3 -m64 -funroll-loops -mieee-fp -DLINUX -DMESA \
-I$(JPEGHOME)/include -I$(MESAHOME)/include -I$(GLWHOME) -I$(MOTIFHOME)/include -I$(EXODUS)/include -I.
LIBS22 = -L$(JPEGHOME)/lib64 -L/usr/lib64 -L/usr/X11R6/lib64 -L$(MOTIFHOME)/lib64  \
$(MESAHOME)/lib64/libGL.a $(MESAHOME)/lib64/libGLU.a \
-ljpeg -lXm -lXt -lSM -lICE -lXext -lm -lX11 -lpthread -ldl \
$(EXOLNK_64) $(NETLNK_64)
OBJS22 = $(OBJSO)
CCLNK22 = gcc $(DEBUGFLAG) -O3 -m64

#linux 64 bit opengl with Mesa Batch     - use make linux64batch
CCLINE38 = gcc $(DEBUGFLAG) -c -O3 -m64 -funroll-loops -mieee-fp -DLINUX -DBATCH \
-I$(JPEGHOME)/include -I$(MESAHOME)/include -I$(GLWHOME) -I$(MOTIFHOME)/include -I$(EXODUS)/include  -I.
LIBS38 = -L$(JPEGHOME)/lib64 -L/usr/lib64 -L/usr/X11R6/lib64 -L$(MOTIFHOME)/lib64  \
$(MESAHOME)/lib64/libGL.a $(MESAHOME)/lib64/libGLU.a $(MESAHOME)/lib64/libOSMesa.a \
-ljpeg -lXm -lXt -lSM -lICE -lXext -lm -lX11 -lpthread -ldl \
$(EXOLNK_64) $(NETLNK_64)
OBJS38 = $(OBJSO)
CCLNK38 = gcc $(DEBUGFLAG) -O3 -m64

#cygwin batch 		 	- use make cygwinbatch2
MESAHOME_CYG = ../utils/cygwin/Mesa/Mesa6.4.2
GLWHOME_CYG = $(MESAHOME_CYG)/include/GL
MOTIFHOME_CYG = /usr/X11R6/include/Xm
EXODUS_CYG = ../utils/cygwin/ExodusII
EXOLNK_CYG = $(EXODUS_CYG)/lib/libexoIIv2c.a
NETLNK_CYG = $(EXODUS_CYG)/lib/libnetcdf.a
LIBHOME_CYG = /lib
#order matters for semi-static linking
LIBS_CYG = $(LIBHOME_CYG)/libXm.dll.a $(LIBHOME_CYG)/libXt.dll.a \
$(MESAHOME_CYG)/lib/libGL.a $(MESAHOME_CYG)/lib/libGLU.a \
$(LIBHOME_CYG)/libX11.dll.a $(LIBHOME_CYG)/libXext.dll.a \
$(LIBHOME_CYG)/libjpeg.a $(LIBHOME_CYG)/libcygwin.a \
$(EXOLNK_CYG) $(NETLNK_CYG)
CCLINE_CYG_BATCH = gcc $(DEBUGFLAG) -c -O3 -funroll-loops -mieee-fp -DNT -DBATCH \
 -I$(GLWHOME_CYG) -I$(MOTIFHOME_CYG) -I$(EXODUS_CYG)/include -I.
# -L$(X11R6HOME_CYG_BATCH)/lib64 -L$(JPEGHOME)/lib64
LIBS_CYG_BATCH = $(LIBS_CYG) $(MESAHOME_CYG)/lib/libOSMesa.a
OBJS_CYG_BATCH = $(OBJSO)
CCLNK_CYG_BATCH = gcc -static -static-libgcc -Wl,--large-address-aware $(DEBUGFLAG)

#cygwin opengl 		 	- use make cygwinogl2
#X11R6HOME18 = /usr/X11R6
#JPEGHOME18 = ../utils/JPEG
# -I$(X11R6HOME18)/include  -I$(JPEGHOME)/include 
CCLINE18 = gcc -c -O3 -funroll-loops -mieee-fp -DNT \
 -I$(GLWHOME_CYG) -I$(MOTIFHOME_CYG) -I$(EXODUS_CYG)/include -I.
LIBS18 = $(LIBS_CYG)
OBJS18 = $(OBJSO)
CCLNK18 = gcc -static -static-libgcc -Wl,--large-address-aware $(DEBUGFLAG)

#IMac OS X opengl 		 - use make imacxogl2
MESAHOME23 = ../../Mesa/Mesa6.4.2
GLWHOME23 = $(MESAHOME23)/include/GL
GLHOME23 = /usr/X11R6
MOTIFHOME23 = ../../OpenMotif
JPEGHOME23 = ../../JPEG
CCLINE23 = gcc -c -O3 -funroll-loops -DMACX -DIMAC -I$(JPEGHOME23)/include  \
-I/usr/include -I$(MOTIFHOME23)/include -I$(MESAHOME23)/include -I$(GLWHOME23) -I$(GLHOME23)/include -I$(EXODUS)/include
LIBS23 = -L$(JPEGHOME23)/lib -L$(GLHOME23)/lib \
-lGL -lGLU $(MESAHOME23)/lib/libGLw.a $(MOTIFHOME23)/lib/libXm.a \
-lXt -lX11 -lXext -lXmu -lm -ljpeg $(EXOLNK) $(NETLNK)
OBJS23 = $(OBJSO)
CCLNK23 = gcc -O3 -flat-namespace

#imacx opengl with Mesa 	- use make imacxmesa
MESAHOME24 = ../../Mesa/Mesa6.4.2
GLWHOME24 = $(MESAHOME24)/include/GL
MOTIFHOME24 = ../../OpenMotif
X11R6HOME24 = /usr/X11R6
JPEGHOME24 = ../../JPEG
CCLINE24 = gcc -c -O3 -funroll-loops -DMACX -DIMAC \
-I$(JPEGHOME24)/include -I/usr/include -I$(MOTIFHOME24)/include -I$(MESAHOME24)/include -I$(GLWHOME24) -I$(X11R6HOME24)/include -I$(EXODUS)/include
LIBS24 = -L$(JPEGHOME24)/lib -L$(MESAHOME24)/lib -L$(MOTIFHOME24)/lib -L$(X11R6HOME24)/lib \
$(MESAHOME24)/lib/libGL.a $(MESAHOME24)/lib/libGLU.a $(MESAHOME24)/lib/libGLw.a \
$(MOTIFHOME24)/lib/libXm.a -lXt -lX11 -lXext -lXmu -lm -ljpeg $(EXOLNK) $(NETLNK)
OBJS24 = $(OBJSO)
CCLNK24 = gcc -O3 -flat-namespace

#imacx opengl with Mesa 	- use make imacxbatch
MESAHOME39 = ../../Mesa/Mesa6.4.2
GLWHOME39 = $(MESAHOME39)/include/GL
MOTIFHOME39 = ../../OpenMotif
X11R6HOME39 = /usr/X11R6
JPEGHOME39 = ../../JPEG
CCLINE39 = gcc -c -O3 -funroll-loops -DMACX -DIMAC -DBATCH \
-I$(JPEGHOME39)/include -I/usr/include -I$(MOTIFHOME39)/include -I$(MESAHOME39)/include -I$(GLWHOME39) -I$(X11R6HOME39)/include -I$(EXODUS)/include
LIBS39 = -L$(JPEGHOME39)/lib -L$(MESAHOME39)/lib -L$(MOTIFHOME39)/lib -L$(X11R6HOME39)/lib \
$(MESAHOME39)/lib/libGL.a $(MESAHOME39)/lib/libGLU.a $(MESAHOME39)/lib/libOSMesa.a $(MESAHOME39)/lib/libGLw.a \
$(MOTIFHOME39)/lib/libXm.a -lXt -lX11 -lXext -lXmu -lm -ljpeg $(EXOLNK) $(NETLNK)
OBJS39 = $(OBJSO)
CCLNK39 = gcc -O3 -flat-namespace


#--------------------------------------
#Set variables and recall the makefile.

nooptions : default

linuxogl2 :
	make -j4 -e CCLINE="$(CCLINE9)" LIBS="$(LIBS9)" OBJS="$(OBJS9)" CCLNK="$(CCLNK9)" linuxogl

linuxmesa :
	make -j4 -e CCLINE="$(CCLINE10)" LIBS="$(LIBS10)" OBJS="$(OBJS10)" CCLNK="$(CCLNK10)" linuxMesa

ntogl2 :
	make -j4 -e CCLINE="$(CCLINE17)" LIBS="$(LIBS17)" OBJS="$(OBJS17)" CCLNK="$(CCLNK17)" ntogl

cygwinogl2 :
	make -j4 -e CCLINE="$(CCLINE18)" LIBS="$(LIBS18)" OBJS="$(OBJS18)" CCLNK="$(CCLNK18)" cygwinogl

linux64ogl2 :
	make -j4 -e CCLINE="$(CCLINE21)" LIBS="$(LIBS21)" OBJS="$(OBJS21)" CCLNK="$(CCLNK21)" linux64ogl

linux64mesa :
	make -j4 -e CCLINE="$(CCLINE22)" LIBS="$(LIBS22)" OBJS="$(OBJS22)" CCLNK="$(CCLNK22)" linux64Mesa

imacxogl2 :
	make -j4 -e CCLINE="$(CCLINE23)" LIBS="$(LIBS23)" OBJS="$(OBJS23)" CCLNK="$(CCLNK23)" imacxogl

imacxmesa :
	make -j4 -e CCLINE="$(CCLINE24)" LIBS="$(LIBS24)" OBJS="$(OBJS24)" CCLNK="$(CCLNK24)" imacxMesa


# Batch versions.

linuxbatch :
	make -j4 -e CCLINE="$(CCLINE33)" LIBS="$(LIBS33)" OBJS="$(OBJS33)" CCLNK="$(CCLNK33)" linuxBatch

linux64batch :
	make -j4 -e CCLINE="$(CCLINE38)" LIBS="$(LIBS38)" OBJS="$(OBJS38)" CCLNK="$(CCLNK38)" linux64Batch

cygwinbatch2 :
	make -j4 -e CCLINE="$(CCLINE_CYG_BATCH)" LIBS="$(LIBS_CYG_BATCH)" OBJS="$(OBJS_CYG_BATCH)" CCLNK="$(CCLNK_CYG_BATCH)" cygwinbatch

imacxbatch :
	make -j4 -e CCLINE="$(CCLINE39)" LIBS="$(LIBS39)" OBJS="$(OBJS39)" CCLNK="$(CCLNK39)" imacxBatch


#-----------------------------------------
# Recalled targets.

gmv : snapshot_old $(OBJS)
	$(CCLNK) -o gmv $(OBJS) $(LIBS)

linuxogl : snapshot_old $(OBJS)
	$(CCLNK) -o linuxogl $(OBJS) $(LIBS)
	strip linuxogl
 
linuxMesa : snapshot_old $(OBJS)
	$(CCLNK) -o linuxMesa $(OBJS) $(LIBS)
	strip linuxMesa

ntogl: snapshot_old $(OBJS)
	$(CCLNK) -o ntogl $(OBJS) $(LIBS)
	strip ntogl

cygwinogl: snapshot_old $(OBJS)
	$(CCLNK) -o cygwinogl $(OBJS) $(LIBS)
	$(STRIPGMV64GL_CYGWIN)
#	strip cygwinogl.exe -o cygwinogl

linux64ogl : snapshot_old $(OBJS)
	$(CCLNK) -o linux64ogl $(OBJS) $(LIBS)
	$(STRIPGMV64GL)

linux64Mesa : snapshot_old $(OBJS)
	$(CCLNK) -o linux64Mesa $(OBJS) $(LIBS)
	$(STRIPGMV64MESA)

imacxogl: snapshot_mac $(OBJS)
	$(CCLNK) -o imacxogl $(OBJS) $(LIBS)

imacxMesa: snapshot_mac $(OBJS)
	$(CCLNK) -o imacxMesa $(OBJS) $(LIBS)


# Batch versions.

linuxBatch : snapshot_old $(OBJS)
	$(CCLNK) -o linuxBatch $(OBJS) $(LIBS)
	strip linuxBatch
 
linux64Batch : snapshot_old $(OBJS)
	$(CCLNK) -o linux64Batch $(OBJS) $(LIBS)
	$(STRIPGMV64BATCH)
	
cygwinbatch: snapshot_old $(OBJS)
	$(CCLNK) -o cygwinBatch $(OBJS) $(LIBS)
	$(STRIPGMV64BATCH_CYGWIN)
	
imacxBatch : snapshot_mac $(OBJS)
	$(CCLNK) -o imacxBatch $(OBJS) $(LIBS)
	strip imacxBatch


#----------------
# Source targets.
 
main.o : main.c main.h gmvdata.h lights.h formwidgets.h contour.h \
	cutplane.h xstuff.h
	$(CCLINE) main.c
 
readgmv.o : readgmv.c gmvdata.h gmvread.h
	$(CCLINE) readgmv.c
 
cbtun.o : cbtun.c gmvdata.h
	$(CCLINE) cbtun.c
 
initstuff.o : initstuff.c gmvdata.h lights.h contour.h cutplane.h
	$(CCLINE) initstuff.c
 
init_motif.o : init_motif.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) init_motif.c
 
callbacks.o : callbacks.c gmvdata.h formwidgets.h lights.h \
	xstuff.h contour.h cutplane.h
	$(CCLINE) callbacks.c
 
drawit.o : drawit.c gmvdata.h formwidgets.h lights.h \
	contour.h xstuff.h cutplane.h
	$(CCLINE) drawit.c
 
drawaxes.o : drawaxes.c gmvdata.h
	$(CCLINE) drawaxes.c
 
drawpolygons.o : drawpolygons.c lights.h gmvdata.h stipples.h
	$(CCLINE) drawpolygons.c
 
drawpolylines.o : drawpolylines.c lights.h gmvdata.h
	$(CCLINE) drawpolylines.c
 
get_statistics.o : get_statistics.c gmvdata.h
	$(CCLINE) get_statistics.c
 
drawnodepoints.o : drawnodepoints.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawnodepoints.c
 
drawvectors.o : drawvectors.c gmvdata.h setfldcolr.h
	$(CCLINE) drawvectors.c
 
drawcellfaces.o : drawcellfaces.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawcellfaces.c
 
drawcellhiddenedges.o : drawcellhiddenedges.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawcellhiddenedges.c
 
drawisosurface.o : drawisosurface.c gmvdata.h contour.h setfldcolr.h
	$(CCLINE) drawisosurface.c
 
centerform.o : centerform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) centerform.c
 
subsetform.o : subsetform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) subsetform.c
 
matorderform.o : matorderform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) matorderform.c
 
isosurfform.o : isosurfform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) isosurfform.c
 
makeisosurf.o : makeisosurf.c gmvdata.h contour.h
	$(CCLINE) makeisosurf.c
 
coloreditform.o : coloreditform.c formwidgets.h gmvdata.h xstuff.h lights.h 
	$(CCLINE) coloreditform.c
 
cutplaneform.o : cutplaneform.c cutplane.h formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cutplaneform.c
 
makecutplane.o : makecutplane.c cutplane.h gmvdata.h
	$(CCLINE) makecutplane.c
 
drawcutplane.o : drawcutplane.c cutplane.h gmvdata.h setfldcolr.h
	$(CCLINE) drawcutplane.c
 
savescreen.o : savescreen.c gmvdata.h
	$(CCLINE) savescreen.c
 
attributes.o : attributes.c gmvdata.h lights.h contour.h
	$(CCLINE) attributes.c
 
writevars.o : writevars.c gmvdata.h formwidgets.h
	$(CCLINE) writevars.c
 
animationform.o : animationform.c formwidgets.h gmvdata.h \
	xstuff.h cutplane.h contour.h lights.h
	$(CCLINE) animationform.c
 
polysubsetform.o : polysubsetform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) polysubsetform.c
 
snapshotform.o : snapshotform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) snapshotform.c
 
scaleaxisform.o : scaleaxisform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) scaleaxisform.c
 
polygonform.o : polygonform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) polygonform.c
 
drawcelledges.o : drawcelledges.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawcelledges.c
 
nodeform.o : nodeform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) nodeform.c
 
cellform.o : cellform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) cellform.c
 
nodeselform.o : nodeselform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) nodeselform.c
 
cellselform.o : cellselform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) cellselform.c
 
cellexpform.o : cellexpform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cellexpform.c
 
drawanimvelflow.o : drawanimvelflow.c gmvdata.h setfldcolr.h
	$(CCLINE) drawanimvelflow.c
 
gridanalform.o : gridanalform.c gmvdata.h xstuff.h contour.h
	$(CCLINE) gridanalform.c
 
traceform.o : traceform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) traceform.c
 
drawtracers.o : drawtracers.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawtracers.c
 
icosadraw.o : icosadraw.c
	$(CCLINE) icosadraw.c
 
plotboxform.o : plotboxform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) plotboxform.c

imagestuff.o : imagestuff.c image.h 
	$(CCLINE) imagestuff.c
 
fieldlimform.o : fieldlimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) fieldlimform.c
 
tracelimform.o : tracelimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) tracelimform.c

drawcolorbar.o : drawcolorbar.c gmvdata.h cutplane.h
	$(CCLINE) drawcolorbar.c
 
boundingboxform.o : boundingboxform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) boundingboxform.c

drawboundingbox.o : drawboundingbox.c gmvdata.h
	$(CCLINE) drawboundingbox.c

drawsubsetbox.o : drawsubsetbox.c gmvdata.h
	$(CCLINE) drawsubsetbox.c
 
animflyform.o : animflyform.c formwidgets.h gmvdata.h
	$(CCLINE) animflyform.c
 
vectbldform.o : vectbldform.c gmvdata.h formwidgets.h xstuff.h
	$(CCLINE) vectbldform.c
	 
tfldcalcform.o : tfldcalcform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) tfldcalcform.c
 
cutlineform.o : cutlineform.c cutplane.h formwidgets.h \
	gmvdata.h xstuff.h SciPlot.h
	$(CCLINE) cutlineform.c
 
makecutline.o : makecutline.c cutplane.h gmvdata.h
	$(CCLINE) makecutline.c
 
drawcutline.o : drawcutline.c cutplane.h gmvdata.h setfldcolr.h
	$(CCLINE) drawcutline.c
 
fldcalcform.o : fldcalcform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) fldcalcform.c
 
queryform.o : queryform.c formwidgets.h gmvdata.h xstuff.h cutplane.h
	$(CCLINE) queryform.c
 
nfieldform.o : nfieldform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) nfieldform.c
 
cfieldform.o : cfieldform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cfieldform.c
 
cfieldlimform.o : cfieldlimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cfieldlimform.c
 
cfldcalcform.o : cfldcalcform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cfldcalcform.c
 
cvectbldform.o : cvectbldform.c gmvdata.h formwidgets.h xstuff.h
	$(CCLINE) cvectbldform.c
 
fldfiles_io.o : fldfiles_io.c
	$(CCLINE) fldfiles_io.c
 
isovolform.o : isovolform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) isovolform.c
 
makeisovol.o : makeisovol.c gmvdata.h contour.h
	$(CCLINE) makeisovol.c
 
pntsizeform.o : pntsizeform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) pntsizeform.c
 
autordform.o : autordform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) autordform.c
 
readgmvamr.o : readgmvamr.c gmvdata.h gmvread.h
	$(CCLINE) readgmvamr.c
 
gen_amr.o : gen_amr.c gmvdata.h
	$(CCLINE) gen_amr.c
 
stereoform.o : stereoform.c gmvdata.h formwidgets.h xstuff.h
	$(CCLINE) stereoform.c
 
linesizeform.o : linesizeform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) linesizeform.c
 
fcoloreditform.o : fcoloreditform.c formwidgets.h gmvdata.h xstuff.h lights.h
	$(CCLINE) fcoloreditform.c
 
SciPlot.o : SciPlot.h SciPlotP.h xstuff.h
	$(CCLINE) SciPlot.c
 
cutsphereform.o : cutsphereform.c cutplane.h formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cutsphereform.c
 
makecutsphere.o : makecutsphere.c cutplane.h gmvdata.h
	$(CCLINE) makecutsphere.c
 
backgrndform.o : backgrndform.c formwidgets.h gmvdata.h xstuff.h lights.h
	$(CCLINE) backgrndform.c
 
gmvrc_io.o : gmvrc_io.c formwidgets.h gmvdata.h 
	$(CCLINE) gmvrc_io.c
 
vectorctlform.o : vectorctlform.c formwidgets.h gmvdata.h xstuff.h 
	$(CCLINE) vectorctlform.c
 
surfform.o : surfform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) surfform.c
 
surfselform.o : surfselform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) surfselform.c
 
surfexpform.o : surfexpform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) surfexpform.c
 
svectbldform.o : svectbldform.c gmvdata.h formwidgets.h xstuff.h
	$(CCLINE) svectbldform.c
 
sfieldform.o : sfieldform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) sfieldform.c
 
sfieldlimform.o : sfieldlimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) sfieldlimform.c
 
sfldcalcform.o : sfldcalcform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) sfldcalcform.c
 
drawsurfaces.o : drawsurfaces.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawsurfaces.c
 
drawsurfedges.o : drawsurfedges.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawsurfedges.c
 
rbzoomform.o : rbzoomform.c formwidgets.h gmvdata.h xstuff.h 
	$(CCLINE) rbzoomform.c
 
nfldavgform.o : nfldavgform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) nfldavgform.c
 
cfldavgform.o : cfldavgform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) cfldavgform.c
 
drawelem.o : drawelem.c gmvdata.h lights.h
	$(CCLINE) drawelem.c
 
misc_ctlform.o : misc_ctlform.c gmvdata.h xstuff.h
	$(CCLINE) misc_ctlform.c
 
windowsizeform.o : windowsizeform.c gmvdata.h xstuff.h
	$(CCLINE) windowsizeform.c
 
ffieldform.o : ffieldform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) ffieldform.c
 
ffieldlimform.o : ffieldlimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) ffieldlimform.c
 
fvectbldform.o : fvectbldform.c gmvdata.h formwidgets.h xstuff.h
	$(CCLINE) fvectbldform.c
 
traceselform.o : traceselform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) traceselform.c
 
trfieldform.o : trfieldform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) trfieldform.c

gmvread.o : gmvread.c gmvread.h gmvrayread.h
	$(CCLINE) gmvread.c

readgmvray.o : readgmvray.c gmvdata.h gmvread.h gmvrayread.h
	$(CCLINE) readgmvray.c
 
drawrays.o : drawrays.c lights.h gmvdata.h setfldcolr.h
	$(CCLINE) drawrays.c
 
rayform.o : rayform.c formwidgets.h gmvdata.h lights.h xstuff.h
	$(CCLINE) rayform.c
 
raylimform.o : raylimform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) raylimform.c
 
concaveform.o : concaveform.c formwidgets.h gmvdata.h xstuff.h
	$(CCLINE) concaveform.c
 
readgmvinfo.o : readgmvinfo.c gmvdata.h  gmvread.h
	$(CCLINE) readgmvinfo.c
 
matcolorbarform.o : matcolorbarform.c formwidgets.h gmvdata.h xstuff.h lights.h
	$(CCLINE) matcolorbarform.c
 
readexodus.o : readexodus.c gmvdata.h
	$(CCLINE) readexodus.c
 
readx3d.o : readx3d.c gmvdata.h x3dread.h gmvread.h
	$(CCLINE) readx3d.c

x3dread.o : x3dread.c x3dread.h
	$(CCLINE) x3dread.c

Ball.o: Ball.c Ball.h BallMath.h
	$(CCLINE) Ball.c
 
BallAux.o: BallAux.c BallAux.h
	$(CCLINE) BallAux.c
 
BallMath.o: BallMath.c BallMath.h BallAux.h
	$(CCLINE) BallMath.c
 
GLwDrawA.o: GLwDrawA.c GLwDrawA.h
	$(CCLINE) GLwDrawA.c
 
GLwMDrawA.o: GLwMDrawA.c GLwMDrawA.h
	$(CCLINE) GLwMDrawA.c
 
clean:
	rm -f *.o *.u

default:
	@echo "Type one of the following:"
	@echo "  make linuxogl2       linux OpenGL"
	@echo "  make linuxmesa       linux with Mesa"
	@echo "  make linuxbatch      linux batch with Mesa"
	@echo "  make linux64ogl2     linux 64-bit OpenGL"
	@echo "  make linux64mesa     linux 64-bit linux with Mesa"
	@echo "  make linux64batch    linux 64-bit linux batch with Mesa"
	@echo "  make cygwinbatch2    Windows Cygwin OpenGL"
	@echo "  make cygwinogl2      Windows Cygwin OpenGL"
	@echo "  make imacxogl2       Intel Mac OS X OpenGL"
	@echo "  make imacxMesa       Intel Mac OS X with Mesa"
	@echo "  make imacxbatch      Intel Mac OS X batch with Mesa"

snapshot_mac :
	cp savescreen.macx.c savescreen.c 
	cp imagestuff.macx.c imagestuff.c

snapshot_old :
	cp savescreen.old.c savescreen.c 
	cp imagestuff.old.c imagestuff.c

