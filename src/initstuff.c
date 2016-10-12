
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <sys/types.h>
#include <math.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#define LMNULL     0.0
#define AMBIENT    2
#define DIFFUSE    3
#define SPECULAR   4
#define SHININESS  5
#define ALPHA      7
 
#ifdef BATCH
#include <osmesa.h>
extern OSMesaContext ctx; 
extern short draw3dflag;
extern void *buffera;
#endif
 
#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "contour.h"
#include "cutplane.h"
#include "xstuff.h"
#include "fontdata.h"
 
GLfloat lightZeroPosition[] = {0.0, 0.0, 1.0, 0.0};
extern GLboolean    doubleBuffer;
extern GLXContext glx_glw_context;
extern void reset_isos_cuts();

void initstuff()
{
 const char *tmpstring, *tmpversion;
 float server_glversion;

   /*  Set up polyon offset.  */
   poly_offset = 0;
#if GL_VERSION_1_1
   poly_offset = 2;
#elif GL_EXT_polygon_offset
   /* glPolygonOffsetEXT(0.5,0.000002); */
   glEnable(GL_POLYGON_OFFSET_EXT);
   poly_offset = glIsEnabled(GL_POLYGON_OFFSET_EXT);
   glDisable(GL_POLYGON_OFFSET_EXT);
#endif

   /*  Check that the server can handle polygon offset.  */
   tmpversion = (char *)glGetString(GL_VERSION);
   sscanf(tmpversion,"%f",&server_glversion);
   tmpstring = (char *)glGetString(GL_EXTENSIONS);
   if (poly_offset == 1 &&
       strstr(tmpstring,"GL_EXT_polygon_offset") == NULL)
      poly_offset = 0;
   if (poly_offset == 2 && server_glversion == 1.0)
      poly_offset = 0;

#if GL_VERSION_1_1
   if (poly_offset == 2)
      glPolygonOffset(1.0, 2.);
      /* glPolygonOffset(1.0, 1.); */
#elif GL_EXT_polygon_offset
   if (poly_offset == 1)
      glPolygonOffsetEXT(0.5,0.000002);
      /* glPolygonOffsetEXT(1.,1./(float)0x10000); */
#endif
}

 
void initcolors()
{
 int i, j, r, g, b;
 float redcol,greencol,bluecol;
 int colindx;
 unsigned long tmpcolor[256];
 
   for (i = 1; i < MAXMATS+1; i++)
     {
      colindx = (i-1)%32;
      redcol = kcolors[colindx];
      greencol = kcolors[32+colindx];
      bluecol = kcolors[64+colindx];
      r = (int) (redcol*255.0);
      g = (int) (greencol*255.0);
      b = (int) (bluecol*255.0);
      matcolor[i-1] = ((r<<24) | (g << 16) | (b << 8) | (255));
     }

   xsize = windowwidth;
   ysize = windowheight;
   txtcolor = 0x000000ff;
 
   aspect = (float)xsize / (float)ysize;
   xscale[0] = 1.0; yscale[0] = 1.0; zscale[0] = 1.0;

   o_axisflag = 0;  o_axissetflag = 0;
   o_axis_x = xsize - 60;  o_axis_y = 60;

   /*  Initialize the field color map.  */
   for (i = 0; i < 64; i++)
     {
      tmpcolor[i] = ( (4*i) << 16 ) | ( 255 << 8 ) | 255;
      tmpcolor[i+64] = ( 255 << 16 ) | ( 4*(63-i)  << 8 ) | 255;
      tmpcolor[i+128] = (4*i) << 24 | ( 255 << 16 ) | 255;
      tmpcolor[i+192] = (255) << 24 | ( 4*(63-i) << 16 ) | 255;
     }
   jcolor[0] = (39 << 24) | (25 << 16) | (201 << 8) | 255;
   for (i = 0; i < 63; i++)
     {
      jcolor[i+1] = tmpcolor[i];
      jcolor[i+64] = tmpcolor[i+63];
     }
   for (i = 0; i < 64; i++)
     {
      jcolor[i+127] = tmpcolor[i+128];
      jcolor[i+191] = tmpcolor[i+192];
     }
   jcolor[255] = (255 << 24) | (25 << 16) | (171 << 8) | 255;
 
   /*  Initialize the material color map.  */
   j = 0;
   for (i = 1; i < MAXMATS+1; i++)
     {
      colindx = (i-1)%32;
      redcol = kcolors[colindx];
      greencol = kcolors[32+colindx];
      bluecol = kcolors[64+colindx];
      matrgba[j+0] = redcol;
      matrgba[j+1] = greencol;
      matrgba[j+2] = bluecol;
      matrgba[j+3] = 1.0;
      j += 4;
     }
 
   /*  Initialize contour colors.  */ 
   j = 0;
   for (i = 1; i < MAXCONTOURS+1; i++)
     {
      colindx = ((i-1)*13 + 7)%256;
      redcol = (float)(jcolor[colindx]>>24 & 255);
      greencol = (float)(jcolor[colindx]>>16 & 255);
      bluecol = (float)(jcolor[colindx]>>8 & 255);

      redcol = redcol/255.;
      greencol = greencol/255.;
      bluecol = bluecol/255.;
      conrgba[j+0] = redcol;
      conrgba[j+1] = greencol;
      conrgba[j+2] = bluecol;
      conrgba[j+3] = 1.0;
      j += 4;
     }
 
   /*  Initialize contour color for animation isosurface.  */
   j = 4 * MAXCONTOURS;
   redcol = 1.;
   greencol = 0.;
   bluecol = 1.;
   conrgba[j+0] = redcol;
   conrgba[j+1] = greencol;
   conrgba[j+2] = bluecol;
   conrgba[j+3] = 1.0;
 
   /*  Initialize contour color for isovolume.  */
   j = 4 * (MAXCONTOURS + 1);
   redcol = .6;
   greencol = .6;
   bluecol = .6;
   conrgba[j+0] = redcol;
   conrgba[j+1] = greencol;
   conrgba[j+2] = bluecol;
   conrgba[j+3] = 1.0;

   /*  Set isosurfaces, isovolume, cutplanes and cutlines.  */
   reset_isos_cuts(); 

   if (physedflag)
     {
      PEcfieldactive = 0;  PEnfieldactive = 0;  PEsfieldactive = 0;
     }
}


void initglobals()
{
  int i, j;
  double old, smallest;
  int iend;

   backgroundcolor = 0xffffffff;  txtcolor = 0xff000000;
   span = 1.;  aspect = 1.;  xpan = 0.;  ypan = 0.;  zpan = 0.;
   xtrans=0.;  ytrans=0.;  ztrans=0.;  rdgmvflag = 0;
   flypos[0] = 0.;  flypos[1] = 0;  flypos[2] = 0.;  fastdraw = 0;
   lightchanged = 0;  titleflag = 0;  anim_snapshotsflag = 0;
   animationflag = 0;  animvelflowflag = 0;  animfly_snapflag = 0;
   animationmenu_up = 0;  animflymenu_up = 0;  animfly_count = 0;
   nrefl_loops = 1;  xreflect = 0;  yreflect = 0;  zreflect = 0; 
   magnify = 1.;  axisflag = 1,  linesflag = 0;  polysflag = 0;
   polysubsetflag = 0;  timeflag =0;  cycleflag = 0;  snapshotsize = 0; 
   colorbarflag = 1;  colorbarpref = 0;  boundingboxflag = 0; 
   viewflag =0;  structuredflag = 0;  drawincr = 10;  bboxcoordflag = 1;
   polystiplflag =0;  struct2dflag =0;  structured_in = 0;
   cliponcellsel = 0;  fieldcolortype = 0;  vectorarrow = 1;  vectorlog = 0;
   pnt_size = 2;  pnt_shape = 0;  line_size = 2;  line_type = 0;
   nset_contours = 10; ncellfaces = 0;  ncelledges = 0;  errorfile = 0;
   nodeselbox_draw = 0;  cellselbox_draw = 0; surfselbox_draw = 0;  
   polyselbox_draw = 0;  velscale = 1.;  velarrowsize = 1.; 
   xscaleaxis = 1.;  yscaleaxis = 1.; zscaleaxis = 1.;  
   xscaleaxiso = 1.;  yscaleaxiso = 1.;  zscaleaxiso = 1.;
   frontclipvalue = 0.0;  backclipvalue = 1.0;  pexplodefact = 0.;
   p_s_flag = 0;  poly_offset = 0;  lightx = 0.;  lighty = 0.;
   distscaleflag = 0;  datalimpopflag = 0;  distscalesetflag = 0;
   traceselbox_draw = 0;  jpegflag = 1;  jpeg_quality = 90; sort_trans = 1;

   nodesflag =0;  nodenumflag = 0;  node0matflag =0;  nodefieldactive = -1;
   vectors = 0;  vectorflag = 0;  vectfldx = -1;  vectfldy = -1; 
   vectfldz = -1;  speedfld = -1;  fldcalc_has_data = 0;
   nodeselmat_on = 0;  nodeselfld_on = 0;  nodeselsph_on = 0;
   nodeselnum_on = 0;  nnodeselnum = 0;  nodeselsph_in = 1;
   nodeselbox_on = 0;  nodeselbox_in = 1;  nodeselgrp_in = 1;
   nodeselfield = 0;  nfldselmin = 0.;  nfldselmax = 0.;
   nodeandorflag[numflags_node] = 0;  nodeselsph_x = 0.; 
   nodeselsph_y = 0.;  nodeselsph_z = 0.;  nodeselsph_r = 0.;  
   nodeselbox_xmin = 0.;  nodeselbox_xmax = 0.;
   nodeselbox_ymin = 0.;  nodeselbox_ymax = 0.;
   nodeselbox_zmin = 0.;  nodeselbox_zmax = 0.;
   nodeidflag = 0;  nodeids = NULL;  faces_in = 0;
   xin = NULL;  yin = NULL;  zin = NULL;  nodefield = NULL;
   nodeghostflag = 0;  nnodeghosts = 0;  nodeghosts = NULL;

   cellsflag = 0;    celledgesflag = 0;  cellnumflag =0;  cellcontflag = 0;
   cell0matflag = 0;  cvectors = 0;   cvectorflag = 0;  cfldcalc_has_data = 0;
   cvectfldx = -1;  cvectfldy = -1;  cvectfldz = -1;  cspeedfld = -1;  
   cellshadeflag = 0;  facerefineflag = 0; cellnormflag = 0; cellselmat_on = 0;
   cellfieldactive = -1;  cellexpflag = -1;  cexplodefact = 0.;
   cellnodenumflag = 0;  cellselcfld_on = 0;  cellmedianedgeflag = 0; 
   cellfacenumflag = 0;  
   cellselnfld_on = 0;  cellselnfldopt = 0;  cellselsph_on = 0;  
   cellselnum_on = 0;  ncellselnum = 0;  cellselsph_in = 1;  
   cellselbox_on = 0;  cellselbox_in = 1; cellselfield = 0;  
   cellselnfield = 0;  cfldselmin = 0.;  cfldselmax = 0.;  
   cnfldselmin = 0.; cnfldselmax = 0.;  cellselgrp_in = 1;  faceselgrp_in = 1;
   cellandorflag[numflags_cell] = 0; cellselsph_x = 0.; 
   cellselsph_y = 0.; cellselsph_z = 0.; cellselsph_r = 0.; 
   cellselbox_xmin = 0.; cellselbox_xmax = 0.;
   cellselbox_ymin = 0.; cellselbox_ymax = 0.;
   cellselbox_zmin = 0.; cellselbox_zmax = 0.;
   cellidflag = 0;  cellids = NULL;  cellfield = NULL;
   facesel = NULL;  cellfacedraw = NULL;
   facenormx = NULL;  facenormy = NULL;  facenormz = NULL;
   cellgrpcolr = NULL;  nodegrpcolr = NULL;  surfgrpcolr = NULL;
   cellghostflag = 0;  ncellghosts = 0;  cellghosts = NULL;
   ghostcellflagid = 0;

   fvectors = 0;  fvectorflag = 0;
   fvectfldx = -1;  fvectfldy = -1;  fvectfldz = -1;  fspeedfld = -1;  

   gridsflag = 0;  gridedgesflag = 0;  gridcontflag = 0;  gridmedianflag = 0;
   gridmedianedgeflag = 0;  ganodenumflag = 0;  gacellnumflag = 0;  
   gridanalflag = 0;  gridfieldactive = -1;  gridfield = NULL;
   gridvoronoiflag = 0;  gridvoronoiedgeflag = 0;  gafacenumflag = 0;

   tracerflag = 0;  tracehistflag  = 0;  tracehistpoints = 1;  
   traceselnum_on = 0;  ntraceselnum = 0;  tracehiststop = 0;  
   tracehiststride = 1;  tracefieldactive = 0; traceridflag = 0;
   traceselfld_on = 0;  traceselsph_on = 0;  traceselsph_in = 1;
   traceselbox_on = 0;  traceselbox_in = 1;
   traceselfield = 0;
   nodeandorflag[numflags_node] = 0;  nodeselsph_x = 0.; 
   nodeselsph_y = 0.;  nodeselsph_z = 0.;  nodeselsph_r = 0.;  
   traceselbox_xmin = 0.;  traceselbox_xmax = 0.;
   traceselbox_ymin = 0.;  traceselbox_ymax = 0.;
   traceselbox_zmin = 0.;  traceselbox_zmax = 0.;
   tracerids = NULL;
  
   surfselbox_in = 1;  surfflag = 0;  surfcontflag = 0;  svectors = 0; 
   svectfldx = -1;  svectfldy = -1;  svectfldz = -1;  sspeedfld = -1;
   svectorflag = 0; surfshadeflag =0;  surfrefineflag = 0; sexplodefact = 0.; 
   surfedgesflag = 0; surfnumflag = 0;  surfnodenumflag = 0;  
   surf0matflag = 0;  surfselfield = 0;  surfselnfield = 0;
   sfldselmin = 0.;  sfldselmax = 0.;  snfldselmin = 0.; snfldselmax = 0.;
   surfandorflag[numflags_cell] = 0;  surfselmat_on = 0;  surfselsfld_on = 0; 
   surfselnfld_on = 0;  surfselnfldopt = 0;  surfselsph_on = 0;
   surfselnum_on = 0;  nsurfselnum = 0;  surfselsph_in = 1;  surfselgrp_in = 1;
   surfselbox_on = 0;  surffieldactive = -1;  surfexpflag = -1;
   sfldcalc_has_data = 0;  surffield = NULL;  surfidflag = 0;  surfids = NULL;
   mesh2dflag = 0;  ncell2dfaces = 0;

   uic = NULL;  vic = NULL;  wic = NULL; 
   cuic = NULL;  cvic = NULL;  cwic = NULL; 
   suic = NULL;  svic = NULL;  swic = NULL;

   for (i = 0;  i < MAXMATS; i++)
     {
      polygons[i] = NULL;   polyverts[i] = NULL;
      polydrawchk[i] = NULL;  mmatnames[i] = NULL;
     }
   for (i = 0; i < MAXFLAGS; i++)
     {
      flags[i] = NULL;  flagname_node[i] = NULL; 
      flagname_cell[i] = NULL;  flagname_surf[i] = NULL;
      for (j = 0;  j < MAXMATS; j++)
        {
         flagnames_node[i][j] = NULL;   flagnames_cell[i][j] = NULL;
         flagnames_surf[i][j] = NULL;
        }
     }
   for (i = 0; i < MAXFIELDS; i++)
     {
      nfieldlimauto[i] = 0;  fieldname[i] = NULL;
      cfieldlimauto[i] = 0;  cfieldname[i] = NULL;
      sfieldlimauto[i] = 0;  sfieldname[i] = NULL;
      ffieldlimauto[i] = 0;  ffieldname[i] = NULL;
     }
   for (i = 0; i < MAXTRFIELDS; i++) 
     {
      trfieldlimauto[i] = 0;  fieldtrname[i] = NULL;
      traceselfields[i] = 0;
      trfldselmin[i] = 0;  trfldselmax[i] = 0;
     }
   for (i = 0; i < MAXGROUPS; i++)
     {
      numcellgrp[i] = 0;  cellgrps[i] = NULL;
      numnodegrp[i] = 0;  nodegrps[i] = NULL;
      numfacegrp[i] = 0;  facegrps[i] = NULL;
      numsurfacegrp[i] = 0;  surfacegrps[i] = NULL;
     }

   /*  Reset isosurfaces, isovolume, cutplanes and cutlines.  */
   contourfieldactive = 0;  contourfield_draw = -1;  
   mcontourfield_draw = -1;  isovolfieldactive = 0;
   isovolfield_draw = -1;  cutplanefieldactive = 0;
   cutspherefieldactive = 0;
   for (i = 0; i < MAXCONTOURS+MAXMATS; i++)  mconval[i] = 0.;
   reset_isos_cuts();

   displistflag = 0;  drawnewlistflag = 0;  stereoflag = 0;  beepflag = 1;
   trackballflag = 0;  drawnew3dflag = 0;  textureflag = 1; drawrayflag = 0;
   rayflag = 0;  rayfieldactive = 0;  drawghosts = 0;  drawghostface = 1;
   drawnodeghosts = 0;  drawunselnodes= 0;  drawunselcells = 0;
   cencellid = -1;  cennodeid = -1;  centracerid = -1;
   if (movieflag == 0)
     {
      autordflag = 0;  nprobeflag = 0;  cprobeflag = 0; 
      fileselflag = 0;  displistflag = 0;  drawnewlistflag = 1;  
      stereowindow = 0;  eyescale = 1.0;  convscale = 14.;
      rbzoomflag = 0;  trackballflag = 0;  cennprobeflag = 0;
      cencprobeflag = 0;  surfselprobe_on = 0;  nsurfpolyverts = 0;
      cellmatselprobeflag = 0;  cellmatremprobeflag = 0;
      surfmatselprobeflag = 0;  surfmatremprobeflag = 0;
      polymatselprobeflag = 0;  polymatremprobeflag = 0;
     }

   /*  Calculate machine smallest number, myzero.  */
   iend = 1;
   smallest = 1.0e-5;
   while (iend)
     {
      if ((1.+smallest) > 1.)
        {
         old = smallest;
         smallest /= 2.;
        }
      else iend = 0;
     }
   myzero = old * 10.;

   /*  Set default distance scale start point.  */
   distscale_x = 120;
   distscale_y = 60;

   /*  Initialize colors.  */
   initcolors();
}

 
void initfont()
{
  int i;

   /*  Initialize fonts.  */ 
#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
     {
      poly_offset = 2;
      glPolygonOffset(1.0, 2.);
      /* glPolygonOffset(1.0, 1.); */
     }
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
   fontOffset2 = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(fontOffset2+i,GL_COMPILE);
      glBitmap(font20[i].w,font20[i].h,font20[i].x0,font20[i].y0,
               font20[i].dx,font20[i].dy,font20[i].bm);
      glEndList ();
     }

   fontOffset = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(fontOffset+i,GL_COMPILE);
      glBitmap(font14[i].w,font14[i].h,font14[i].x0,font14[i].y0,
               font14[i].dx,font14[i].dy,font14[i].bm);
      glEndList ();
     }

   /*  Initialize lights, line width and point size.  */
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);

   glLineWidth(2.0);
   glPointSize(2.0);
   /* glEnable(GL_LINE_SMOOTH); */

   /*  Save texture map for smooth field data.  */
   for (i = 0; i < 256; i++)
     {
      textures[3*i+0] = (float)(jcolor[i]>>24 & 255) / 255.;
      textures[3*i+1] = (float)(jcolor[i]>>16 & 255) / 255.;
      textures[3*i+2] = (float)(jcolor[i]>>8 & 255) / 255.;
     }
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, 
                GL_FLOAT, textures);
   glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

 
void initaxfont(Window win, GLXContext glx)
{
  int i;

   glXMakeCurrent(dpy,win,glx);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
/* 
   fontOffset2 = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(fontOffset2+i,GL_COMPILE);
      glBitmap(font20[i].w,font20[i].h,font20[i].x0,font20[i].y0,
               font20[i].dx,font20[i].dy,font20[i].bm);
      glEndList ();
     }
*/
   axfontOffset = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(axfontOffset+i,GL_COMPILE);
      glBitmap(font14[i].w,font14[i].h,font14[i].x0,font14[i].y0,
               font14[i].dx,font14[i].dy,font14[i].bm);
      glEndList ();
     }
}
 
void initmatfont(Window win, GLXContext glx)
{
  int i;

   glXMakeCurrent(dpy,win,glx);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
/* 
   fontOffset2 = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(fontOffset2+i,GL_COMPILE);
      glBitmap(font20[i].w,font20[i].h,font20[i].x0,font20[i].y0,
               font20[i].dx,font20[i].dy,font20[i].bm);
      glEndList ();
     }
*/
   matfontOffset = glGenLists(128);
   for (i = 0; i < 128; i++)
     {
      glNewList(matfontOffset+i,GL_COMPILE);
      glBitmap(font14[i].w,font14[i].h,font14[i].x0,font14[i].y0,
               font14[i].dx,font14[i].dy,font14[i].bm);
      glEndList ();
     }
}


void resettexture()
{
  int i;

   /*  Reset texture values.  */
   for (i = 0; i < 256; i++)
     {
      textures[3*i+0] = (float)(jcolor[i]>>24 & 255) / 255.;
      textures[3*i+1] = (float)(jcolor[i]>>16 & 255) / 255.;
      textures[3*i+2] = (float)(jcolor[i]>>8 & 255) / 255.;
     }
#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
   glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, 
                GL_FLOAT, textures);
}
