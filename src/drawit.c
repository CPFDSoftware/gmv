
#ifdef NT
#include <windows.h>
#endif

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xutil.h>
#include <Xm/Xm.h>
#include <Xm/TextF.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#ifdef BATCH
#include <osmesa.h>
extern OSMesaContext ctx;
extern void *buffera;
#endif
extern char batchfilnam[256];

#ifdef __sgi
#include <X11/extensions/SGIStereo.h>
#endif

#include "gmvdata.h"
#include "lights.h"
#include "xstuff.h"
#include "formwidgets.h"
#include "contour.h"
#include "cutplane.h"
#include "Ball.h"

float fclip, bclip;
extern GLXContext glx_axglw_context, glx_glw_context, glx_stglw_context;
extern GLboolean doubleBuffer;
extern GLfloat Ballmatrix[4][4];
extern int interactivemouse;

/* TJ added 2012-08-21 for write variable support at command line */
extern short writenodes_flag, writecells_flag,
             writeparticles_flag, writesurfaces_flag;
extern char  writenodes_filename[256], writecells_filename[256],
             writeparticles_filename[256], writesurfaces_filename[256];

GLdouble wavdx1,wavdx2,wavdy1,wavdy2,wavdz1,wavdz2;
unsigned int *depthbuff = NULL, *exposeimage = NULL;
unsigned int *parray = NULL;

static int fovy, istereo = 0;
static float magnify1;
static double maxdist, diststart = 120;
int numpts, numlines, numvects;
short tritypes_on[6], tritypes_on2[6], tritypes_on3[6];


void return3d()
{
   /*  Return to 3d mode.  */
   glPopAttrib();
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (viewflag == 0)
      glOrtho(-span*.5*aspect,span*.5*aspect,-span*.5,span*.5,fclip,bclip);
   if (viewflag > 0)
      gluPerspective(fovy/10.,aspect,fclip,bclip);
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
}


void setmag_clip()
{
  float minfclip;

   /*  Set magnification depending on view type.  */
   if (viewflag == 0) magnify1 = magnify;
   if (viewflag == 1)
     {
      magnify1 = 1.;
      fovy = (int)(600./magnify);
      if (!stereoflag)
        {
         if (fovy < 10.) fovy = 10.;
         if (fovy > 1790.) fovy = 1790.;
        }
     }
   if (viewflag == 2)
     {
      magnify1 = 1.;
      fovy = 600;
      if (stereoflag) fovy = 800;
     }

   /*  Set front and back clip planes depending on view type.  */

   fclip = span*magnify1*(frontclipvalue*2.);
   bclip = 2.*span*magnify1*backclipvalue;
   
   if (fclip != 0.)
      bclip = ((bclip-fclip) < 0.01) ? fclip*1.0001 : bclip;
   minfclip = span / 100.;
   /* minfclip = 0.1; */
   if (viewflag == 1 && fclip < minfclip) fclip = minfclip;
   if (viewflag == 2)
     {
      minfclip /= 100.;
      fclip = minfclip;
      if (minfclip < .01) fclip = .01;
      if (minfclip > 10.) fclip = 10.;
      if (stereoflag) fclip *= 10.;
      bclip = 3.*span;
     }
}

void drawit()
{
  int i, j, itype;
  char number1[5], filnamss[256], tmpname[256];
  int xpend, ypend;
  void drawwinaxis();
  GLfloat lightPosition[4];
  void buildelems();
  void drawimage();

   /*  Build drawing elements.  */
   if (drawnew3dflag)
     {
      FREE(drawarray);  FREE(drawarray2);  FREE(plvarray);
      FREE(drawarray3);  FREE(plvarray3);
      drawarray_alloc = 0;  drawarray2_alloc = 0;  plvarray_alloc = 0;
      drawarray3_alloc = 0;  plvarray3_alloc = 0;
      buildelems();
      if (displistflag) drawnewlistflag = 1;

      /*  Count the number of points, lines and vectors to draw.  */
      numpts = 0;  numlines = 0;  numvects = 0;
      for (i = 0; i < numplv; i++)
        {
         itype = plvarray[i].itype;
         if (itype < LSOLID) numpts++;
         else if (itype == LSOLID || itype == LSMOOTH || itype == THIST)
            numlines++;
         else numvects++;
        }

      /*  Determine which triangle types to draw.  */
      for (i = 0; i < 6; i++)
        {
         tritypes_on[i] = 0;
         tritypes_on2[i] = 0;
         tritypes_on3[i] = 0;
       }
      for (i = 0; i < numtris; i++)
        {
         j = drawarray[i].itype;
         tritypes_on[j] = 1;
        }
      for (i = 0; i < numtris2; i++)
        {
         j = drawarray2[i].itype;
         tritypes_on2[j] = 1;
        }
      for (i = 0; i < numtris3; i++)
        {
         j = drawarray3[i].itype;
         tritypes_on3[j] = 1;
        }

      /*  Reset material colorbar, for interactive execution only.  */
      if (movieflag == 0) setmatcolorbar();

      /*  Set flag to z sort tranparent objects.  */
      sort_trans = 1;
     }

   if (movieflag == 0)
     {
      if (fastdraw == 0)
        {
         XDefineCursor(dpy,topwin,waitcsr);
         XFlush(dpy);
         if (exposeimage == NULL)
           {
            i = windowxmax * windowymax;
            depthbuff = (unsigned int *)malloc(i*sizeof(unsigned int));
            exposeimage = (unsigned int *)malloc(i*sizeof(unsigned int));
            if (depthbuff == NULL || exposeimage == NULL) memerr();
           }
        }
     }

   if (lightchanged) 
     {
      lightPosition[0] = (GLfloat)lightx;
      lightPosition[1] = (GLfloat)lighty;
      lightPosition[2] = (GLfloat)1.0;
      lightPosition[3] = (GLfloat)0.0;
      glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

      lightchanged = 0;
     }

   /*  Set magnification and front,back clip planes.  */
   setmag_clip();

   /*  Draw the window axes.  */
   if (movieflag == 0 && animationflag == 0 && stereoflag == 0) 
      drawwinaxis();

   /*  Set up drawing window.  */
   if (!stereoflag)
#ifdef BATCH
      OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
      glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
   else
      glXMakeCurrent(dpy,winstid,glx_stglw_context);

   /*  Draw non-stereo 3D and 2d objects.  */
   if (!stereoflag)
     {
      glViewport(0,0,windowwidth+1,windowheight+1);
      drawimage();
     }

   else     /*  Draw stereo frames.  */
     {

      istereo = 1;

      /*  Draw left eye view, if not BATCH.  */   
#ifndef BATCH
      glViewport(0,0,stwinw,stwinh);

      glXWaitGL();
#ifdef __sgi
      if (stereowindow)
         glDrawBuffer(GL_BACK_LEFT);
      else
         XSGISetStereoBuffer(dpy, winstid, STEREO_BUFFER_LEFT);
#else 
      glDrawBuffer(GL_BACK_LEFT);
#endif
      XSync(dpy, 0);

      drawimage();
#endif  /*  End of ifndef BATCH.  */

      /*  If BATCH, capture left eye view.  */
#ifdef BATCH
      glViewport(0,0,windowwidth+1,windowheight+1);
      drawimage();
      xpend = strlen(batchfilnam) - 4;
      strncpy(tmpname,batchfilnam,xpend);
      strcpy(filnamss,tmpname);
      if (jpegflag) strcat(filnamss,"_l.jpg");
      else strcat(filnamss,"_l.rgb");
      xpend = xsize - 1;  ypend = ysize - 1;
      savescreen(filnamss,0,xpend,0,ypend,buffera);
#endif

      /*  Draw right eye view.  */
      istereo = 2;

#ifndef BATCH
      glXWaitGL();
#ifdef __sgi
      if (stereowindow)
         glDrawBuffer(GL_BACK_RIGHT);
      else
         XSGISetStereoBuffer(dpy, winstid, STEREO_BUFFER_RIGHT);
#else
      glDrawBuffer(GL_BACK_RIGHT);
#endif
      XSync(dpy, 0);
      drawimage();
#endif  /*  End of indef BATCH.  */

      /*  If BATCH, capture right eye view.  */
#ifdef BATCH
      glViewport(0,0,windowwidth+1,windowheight+1);
      drawimage();
      strcpy(filnamss,tmpname);
      if (jpegflag) strcat(filnamss,"_r.jpg");
      else strcat(filnamss,"_r.rgb");
      xpend = xsize - 1;  ypend = ysize - 1;
      savescreen(filnamss,0,xpend,0,ypend,buffera);
#endif

      istereo = 0;
     }

   /*  Return to 3D mode.  */
   return3d();

   /*  If snapshot is to be made, save the image.  */
   if ((animationflag == 1 && anim_snapshotsflag == 1) || 
       snapshotflag == 1 || snapshotsize > 0 || animfly_snapflag == 1)
     {
      xpend = xsize;  ypend = ysize;
#ifndef BATCH
      if (doubleBuffer == GL_TRUE) glReadBuffer(GL_BACK);
      else glReadBuffer(GL_FRONT);
      if (parray == NULL)
        {
         i = windowxmax * windowymax;
         parray = (unsigned int *)malloc(i*sizeof(unsigned int));
         if (parray == NULL) memerr();
        }
      glReadPixels(0,0,xpend,ypend,GL_RGBA,GL_UNSIGNED_BYTE,parray);
#endif
      xpend--;  ypend--;
     }

   /*  Save image for expose events.  */
   if (fastdraw == 0 && animationflag == 0 && animfly_snapflag == 0 &&
       snapshotflag == 0 && snapshotsize == 0 && 
       anim_snapshotsflag == 0 && stereoflag == 0 && 
       drawincr >= 99)
     {
      if (doubleBuffer == GL_TRUE) glReadBuffer(GL_BACK);
      else glReadBuffer(GL_FRONT);
      glReadPixels(0,0,xsize,ysize,GL_DEPTH_COMPONENT,
                  GL_UNSIGNED_BYTE,depthbuff);
      glReadPixels(0,0,xsize,ysize,GL_RGBA,GL_UNSIGNED_BYTE,exposeimage);
     } 
      
   if (doubleBuffer == GL_TRUE)
     {
      if (stereoflag == 0)
         glXSwapBuffers(dpy, windowid);
      else
         glXSwapBuffers(dpy, winstid);
     }
   glFlush();

   /*  Save snapshot.  */
   if (snapshotflag == 1 && stereoflag == 0)
     {
      strcpy(filnamss,batchfilnam);
#ifdef BATCH
      savescreen(filnamss,0,xpend,0,ypend,buffera);
#else
      savescreen(filnamss,0,xpend,0,ypend,parray);
#endif
     }

   /* if movieflag is on, then gmvexit(); */
   if (movieflag == 1)
   {
     /* TJ added 2012-08-21 write out variables requested by command line */
     if( writenodes_flag == 1 )
     {
       setVartypeAndWrite( 0, writenodes_filename ); //0=nodes
     }
     if( writecells_flag == 1 )
     {
       setVartypeAndWrite( 1, writecells_filename ); //1=cells
     }
     if( writeparticles_flag == 1 )
     {
       setVartypeAndWrite( 2, writeparticles_filename ); //2=particles
     }
     if( writesurfaces_flag == 1 )
     {
       setVartypeAndWrite( 3, writesurfaces_filename ); //3=surfaces
     }

     exit(0);
   }

   if ((anim_snapshotsflag == 1) && (animation_count > 0)) 
     {
      sprintf(number1,"%d",animation_count);
      strcpy(filnamss,animfilnam);
      if (animation_count < 10)
        {
         strcat(filnamss,"00");
         strcat(filnamss,number1);
        } 
      else if (animation_count < 100)
        {
         strcat(filnamss,"0");
         strcat(filnamss,number1);
        } 
      else 
        {
         strcat(filnamss,number1);
        }
      if (jpegflag) strcat(filnamss,".jpg");
      else strcat(filnamss,".rgb");
      savescreen(filnamss,0,xpend,0,ypend,parray);
      if (beepflag) XBell(dpy,100);
     }

   if (snapshotsize > 0)
     {
      strcpy(filnamss,snapshotfilnam);

      if (snapshotsize == 1)
         savescreen(filnamss,0,xpend,0,ypend,parray);

      snapshotsize = 0;
      glViewport(0,0,windowwidth+1,windowheight+1);

      if (beepflag) XBell(dpy,100);
     }

   if ((animfly_snapflag == 1) && (animfly_count > 0)) 
     {
      sprintf(number1,"%d",animfly_count);
      strcpy(filnamss,animflyfilnam);
      if (animfly_count < 10)
        {
         strcat(filnamss,"00");
         strcat(filnamss,number1);
        } 
      else if (animfly_count < 100)
        {
         strcat(filnamss,"0");
         strcat(filnamss,number1);
        } 
      else 
        {
         strcat(filnamss,number1);
        }
      savescreen(filnamss,0,xpend,0,ypend,parray);
      if (beepflag) XBell(dpy,100);
     }

   if (animationflag == 1)
     {
      if (animation_count == anim_iterations) 
        {
         glViewport(0,0,windowwidth+1,windowheight+1);
        }
     }

   if (movieflag == 0) XUndefineCursor(dpy,topwin);
}


void drawimage()
{
  int i, cutlineflg;
  short tmp_fastdraw;
  float prevmatrix[16], eye;
  GLdouble dsx1, dsy1, dsz1, dsx2, dsy2, dsz2;
  GLfloat backr, backg, backb;
  GLfloat wavprojmatrix[16], wavmodelvmatrix[16];
  GLdouble dwavprojmatrix[16], dwavmodelvmatrix[16];
  GLuint sx, sy;
  GLint vport[4];
  void draw3d(), draw2d();

   /*  Set up view.  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (viewflag == 0)
      glOrtho(-span*.5*aspect,span*.5*aspect,-span*.5,span*.5,fclip,bclip);
   if (viewflag > 0)
      gluPerspective(fovy/10.,aspect,fclip,bclip);

   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);
   /* glEnable(GL_LIGHTING); */

   /*  Scale, magnify and set view depending on view type.  */
   glPushMatrix();

   /*  Translate for stereo view.  */
   if (stereoflag)
     {
      eye = eyescale * 0.008 * (bclip - fclip) / magnify;
      if (viewflag == 2) eye = eyescale * 0.0015 * span;
      if (istereo == 1)
         glTranslatef(eye, 0., 0.);
      if (istereo == 2)
         glTranslatef(-eye, 0., 0.);
     }

   if (viewflag < 2)
     {
      glScalef(magnify1, magnify1, magnify1);
      glTranslatef(xpan, ypan, 0.);
      glTranslatef(0., 0., -((GLfloat)span));
      if (interactivemouse == 0)
        {
         glRotatef( -( (GLfloat)(zrot)), 0., 0., 1.);
         glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
         glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
        }
      else
        {
         glMultMatrixf(Ballmatrix);
        }
     }
   if (viewflag == 2)
     {
      glTranslatef(-xpan,-ypan,-zpan);
      xpan = 0.;  ypan = 0.;  zpan = 0.;
      glRotatef((GLfloat)(90.-zrot),0.,0.,1.);
      glRotatef((GLfloat)yrot,0.,1.,0.);
      glRotatef((GLfloat)xrot,0.,0.,1.);
      glTranslatef(-flypos[0],-flypos[1],-flypos[2]);
     }

   if (viewflag == 0) glEnable(GL_NORMALIZE);
   else glDisable(GL_NORMALIZE);

   /*  Add animation data, if animation is on.  */
   if (animationflag == 1)
     {
      animation_count++;

      glRotatef(zanimrot,0.0,0.0,1.0);
      glRotatef(yanimrot,0.0,1.0,0.0);
      glRotatef(xanimrot,1.0,0.0,0.0);
      glScalef(1./magnify1, 1./magnify1, 1./magnify1);
      if (viewflag > 0)
         glScalef(1./magnify, 1./magnify, 1./magnify);
      glScalef(animmag, animmag, animmag);
      glTranslatef(xanimcen, yanimcen, zanimcen);
     }  /* end if (animationflag == 1) */

   /*  Translate and set up current fly-through   */
   /*  position if fly-through is on.             */
   if (animationflag == 0 && viewflag < 2)
      glTranslatef(xtrans, ytrans, ztrans);
   if (viewflag == 2)
     {
      glGetFloatv(GL_MODELVIEW_MATRIX,prevmatrix);
      flypos[0] = -(prevmatrix[0]*prevmatrix[12] +
                    prevmatrix[1]*prevmatrix[13] +
                    prevmatrix[2]*prevmatrix[14]);
      flypos[1] = -(prevmatrix[4]*prevmatrix[12] +
                    prevmatrix[5]*prevmatrix[13] +
                    prevmatrix[6]*prevmatrix[14]);
      flypos[2] = -(prevmatrix[8]*prevmatrix[12] +
                    prevmatrix[9]*prevmatrix[13] +
                    prevmatrix[10]*prevmatrix[14]);
     }

   /*  For cutlines, if any, create a plane normal to the screen  */
   /*  and along the cutline so that the wave is always visible.  */
   /*  First create a line normal to the screen.                  */
   /*  Also, get two points for distance scale.                   */
   cutlineflg = 0;
   for (i = 0; i < MAXCUTLINES; i++)
     {
      if (cutlineon[i] == 1) cutlineflg = 1;
     }
   if (cutlineflg || distscaleflag)
     {
      glGetFloatv(GL_PROJECTION_MATRIX,wavprojmatrix);
      glGetFloatv(GL_MODELVIEW_MATRIX,wavmodelvmatrix);
      sx = windowwidth/2;  sy = windowheight/2;
      vport[0] = 0;
      vport[1] = 0;
      vport[2] = windowwidth+1;
      vport[3] = windowheight+1;
      for ( i = 0; i < 16; i++ )
        {
         dwavmodelvmatrix[i] = (GLdouble)wavmodelvmatrix[i];
         dwavprojmatrix[i] = (GLdouble)wavprojmatrix[i];
        }
      gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)0.0,
                   dwavmodelvmatrix,dwavprojmatrix,vport,
                   &wavdx1,&wavdy1,&wavdz1);
      gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)0.5,
                    dwavmodelvmatrix,dwavprojmatrix,vport,
                    &wavdx2,&wavdy2,&wavdz2);
      if (distscaleflag)
        {
         sx = distscale_x;  sy = distscale_y;
         gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)0.0,
                      dwavmodelvmatrix,dwavprojmatrix,vport,
                      &dsx1,&dsy1,&dsz1);
         sx = windowwidth+1 - 20;  sy = distscale_y;
         gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)0.0,
                      dwavmodelvmatrix,dwavprojmatrix,vport,
                      &dsx2,&dsy2,&dsz2);
         maxdist = sqrt((dsx2-dsx1)*(dsx2-dsx1) + (dsy2-dsy1)*(dsy2-dsy1) +
                        (dsz2-dsz1)*(dsz2-dsz1));
        }
     }

   glClearDepth(1.0);
   backr = (float)((backgroundcolor)&0xff) / 255.;
   backg = (float)((backgroundcolor)>>8&0xff) / 255.;
   backb = (float)((backgroundcolor)>>16&0xff) / 255.;
   if (backr > .997) backr = 1.0;
   if (backg > .997) backg = 1.0;
   if (backb > .997) backb = 1.0;
   glClearColor(backr,backg,backb,1.0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   /*  Draw the axes, if requested.  */
   if (axisflag == 1) drawaxes(1);

   /*  Draw the bounding box, if requested.  */
   if (boundingboxflag == 1) drawboundingbox();

   /*  Draw the subset bounding box, if needed.  */
   if (fastdraw == 1 && drawincr > 1 &&
       (nodesflag > 0 || tracerflag > 0 ||
        celledgesflag == 1 || cellsflag == 1 || vectorflag == 1 ||
        cvectorflag == 1 || nodenumflag == 1 || cellnumflag == 1 ||
        cellmedianedgeflag == 1) && 
       rbzoomflag == 0 && subsetbox_draw == 0 && cellselbox_draw == 0 && 
       nodeselbox_draw == 0 && surfselbox_draw == 0 &&
       polyselbox_draw == 0 && traceselbox_draw == 0) 
      drawsubsetbox();

   if (subsetbox_draw)
      drawsubsetbox();

   /*  Draw the cell selection box, if needed.  */
   if (cellselbox_draw) 
      drawselectbox(0);

   /*  Draw the node selection box, if needed.  */
   if (nodeselbox_draw) 
      drawselectbox(1);

   /*  Draw the surface selection box, if needed.  */
   if (surfselbox_draw) 
      drawselectbox(2);

   /*  Draw the polygon selection box, if needed.  */
   if (polyselbox_draw) 
      drawselectbox(3);

   /*  Draw the trace selection box, if needed.  */
   if (traceselbox_draw) 
      drawselectbox(4);

   /*  Imediate draw mode.  */
   if (!displistflag)
     {
      draw3d();
      if (!stereoflag)
         draw2d();
     }

   /*  Display list mode.  */
   if (displistflag)
     {
      if (drawnewlistflag)
        {
         glDeleteLists(DLISTALL,2);
         tmp_fastdraw = fastdraw;
         fastdraw = 0;
         glNewList(DLISTALL,GL_COMPILE_AND_EXECUTE);
         draw3d();
         glEndList();
         if (!stereoflag) draw2d();

         fastdraw = 1;
         glNewList(DLISTINT,GL_COMPILE);
         draw3d();
         glEndList();

         fastdraw = tmp_fastdraw; 
         drawnewlistflag = 0;   
        }
      else
        {
         if (fastdraw == 0)
            glCallList(DLISTALL);
         else
            glCallList(DLISTINT);
         draw2d();
        }
     }
}


void draw3d()
{
  void drawdata(int transtype);

   /*  Draw solid triangles and points, lines and vectors.  */
   drawdata(0);

   /*  Draw transparent triangles.  */
   if (numtris2 > 0)
      drawdata(1);

   /*  Draw cell hidden line wireframe.  */
   if (numtris3 > 0)
      drawdata(2);

   drawnew3dflag = 0;
   glPopMatrix();
}

void drawdata(int transflg)
{
  int i, reflloops, reflflag, cutlineflag, ntris, nlines, nvects, nplv,
      npts;
  short nttypes[6];
  struct trielem *drawtris;
  struct plvelem *drawplv;
  void drawtriangles(int refl, int nlines, short nttypes[6],
                     struct trielem *drawtris, int ntris);
  void drawitplv(int npts, int nlines, struct plvelem *drawplv, int nvects,
                 int nplv, int hiddenflag);
  void sort_trans_tris(struct trielem * drawtrans, int ntris);

   if (transflg == 0)
     {
      ntris = numtris;
      drawtris = drawarray;
      for (i = 0; i < 6; i++)
         nttypes[i] = tritypes_on[i];
      npts = numpts;
      nlines = numlines;
      nvects = numvects;
      drawplv = plvarray;
      nplv = numplv;
     }
   if (transflg == 1)
     {

      /*  Sort tranparent triangles if needed.  */
      if (sort_trans && fastdraw == 0)
        {
         sort_trans_tris(drawarray2,numtris2);
         sort_trans = 0;
        }

      ntris = numtris2;
      drawtris = drawarray2;
      for (i = 0; i < 6; i++)
         nttypes[i] = tritypes_on2[i];
      nlines = numlines;
     }
   if (transflg == 2)
     {
      ntris = numtris3;
      drawtris = drawarray3;
      for (i = 0; i < 6; i++)
         nttypes[i] = tritypes_on3[i];
      npts = numpts;
      nlines = numplv3;
      nvects = 0;
      drawplv = plvarray3;
      nplv = numplv3;
     }

   /*  Loop through reflections.  */
   for (reflloops = 0; reflloops < nrefl_loops; reflloops++)
     {
      if (reflloops == 0 || (reflloops > 2 && reflloops < 6)) 
         reflflag = 0;
      else reflflag = 1;

      glPushMatrix();
      glScalef(xscale[reflloops], yscale[reflloops], zscale[reflloops]);
      glDepthMask(GL_TRUE);
      glDisable(GL_BLEND);

      /*  Draw sorted transparent triangles.  */
      if (transflg == 1)
        {
         glDepthMask(GL_FALSE);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

         //glEnable(GL_CULL_FACE);
         //glCullFace(GL_BACK);
         drawtriangles(reflflag,numlines,nttypes,drawtris,ntris);
         //glDisable(GL_CULL_FACE);

/*
         glEnable(GL_CULL_FACE);
         if (fastdraw == 0)
           {
            glCullFace(GL_FRONT);
            drawtriangles(reflflag,numlines,nttypes,drawtris,ntris);
           }
         glCullFace(GL_BACK);
         drawtriangles(reflflag,numlines,nttypes,drawtris,ntris);
         glDisable(GL_CULL_FACE);
*/
        }

      /*  For hidden lines (transflag == 2) draw the triangles  */
      /*  with color mask off.  This is done after everything   */
      /*  else is drawn, controlled by drawdata.                */
      if (transflg == 2)
        {
         glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
         drawtriangles(reflflag,nlines,nttypes,drawtris,ntris);
         glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
         drawitplv(npts,nlines,drawplv,nvects,nplv,1);
        }

      /*  Draw points, lines, and vectors (plv).  */
      if (numplv > 0 && transflg == 0)
         drawitplv(npts,nlines,drawplv,nvects,nplv,0);

      /*  Draw triangles.  */
      /* if (ntris > 0 && transflg < 2) */
      if (ntris > 0 && transflg == 0)
         drawtriangles(reflflag,numlines,nttypes,drawtris,ntris);

      /*  Draw cutlines.  */
      if (transflg == 0)
        {
         cutlineflag = 0;
         for (i = 0; i < MAXCUTLINES; i++)
           {
            if (cutlineon[i] == 1)
              {
               drawcutline(i);
               cutlineflag = 1;
              }
           }
        }

      /*  Reset reflected matrix.  */
      glPopMatrix();
    }

   /*  Draw cutline 2d plot.  */
   if (transflg == 0 && cutlineflag == 1 && movieflag == 0)
      drawplot2d();
/*
   glPopMatrix();
*/
   glDepthMask(GL_TRUE);
   glDisable(GL_BLEND);
}


void draw2d()
{
  int i, j, titlelen, cpon, ison, ismon, ivon, cson, nints, chkpos;
  float xtimepos, ytimepos, cyclepos, xds, v1[3], v2[3], Ox, Oy;
  double mindist, distints[6];
  char strt[20];
  void drawflycross();

   /*  Use 2D mode to draw colorbar, time & cycle no.  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.,xsize,0.,ysize,-1.,1.);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);

   /*  Draw a color bar, if needed.  */
   cpon = 0;
   for (i = 0; i < MAXCUTPLANES+1; i++)
      cpon += cutplane_on[i];
   cson = 0;
   for (i = MAXCUTPLANES+1; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
      cson += cutplane_on[i];
   ison = 0;
   if (contourfield_draw > -1)
     {
      for (i = 0; i < MAXCONTOURS; i++)
         ison += contour_on[i];
     }
   ismon = 0;
   if (mcontourfield_draw > -1)
     {
      for (i = MAXCONTOURS; i < MAXCONTOURS+MAXMATS; i++)
         ismon += contour_on[i];
     }
   ivon = 0;
   if (isovol_on && isovolfield_draw > -1)
      ivon = 1;
   if (colorbarflag == 1 && fastdraw == 0 &&
       (nodesflag > 0 || tracerflag > 0 || cellsflag == 1 || 
        celledgesflag == 1 || cellmedianedgeflag == 1 || 
        vectorflag == 1 || cvectorflag == 1 || nodenumflag == 1 ||
        cellnumflag == 1 || surfnumflag == 1 || 
        cpon > 0 || ison > 0 || ismon > 0 || ivon > 0 || cson > 0 ||
        surfflag == 1 || surfedgesflag == 1 || svectorflag == 1 ||
        cellhiddenedgesflag))
      drawcolorbar();

   /*  Print the problem time, cycle no. & title.  */
   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);
   /* glColor4ubv((GLubyte *)&(txtcolor)); */
       
   /*  Set x,y postition for labels.  */
   xtimepos = xsize - 150.;
   ytimepos = ysize - 25.;
   cyclepos = 10.;
/*
   if ((animationflag == 1 && anim_snapshotsflag == 1) ||
       snapshotsize == 2 || movieflag == 1 || animfly_snapflag == 1)
     {
      xtimepos = xsize - 240.;
      ytimepos = ysize - 55.;
      cyclepos = 30.;
     } 
*/

   if (timeflag == 1)
     {
      sprintf(strt,"%10.7e",ptime);
      glRasterPos3f(xtimepos, ytimepos, 0.);
      glPushAttrib(GL_LIST_BIT);
      glListBase(fontOffset2);
      glCallLists(strlen(strt), GL_UNSIGNED_BYTE, (GLubyte *) strt);
      glPopAttrib();
     }
 
   if (cycleflag == 1)
     {
      sprintf(strt,"%d",cycleno);
      glRasterPos3f(10., ytimepos, 0.);
      glPushAttrib(GL_LIST_BIT);
      glListBase(fontOffset2);
      glCallLists(strlen(strt), GL_UNSIGNED_BYTE, (GLubyte *) strt);
      glPopAttrib();
     }

   if (titleflag == 1)
     {
      titlelen = strlen(titlestr);
      xtimepos = xsize/2. - (titlelen * 12)/2.;
      glRasterPos3f(xtimepos, ytimepos, 0.);
      glPushAttrib(GL_LIST_BIT);
      glListBase(fontOffset2);
      glCallLists(strlen(titlestr), GL_UNSIGNED_BYTE,
                  (GLubyte *) titlestr);
      glPopAttrib();

      titlelen = strlen(titlestr2);
      xtimepos = xsize/2. - (titlelen * 12)/2.;
      glRasterPos3f(xtimepos, 10, 0.);
      glPushAttrib(GL_LIST_BIT);
      glListBase(fontOffset2);
      glCallLists(strlen(titlestr2), GL_UNSIGNED_BYTE,
                  (GLubyte *) titlestr2);
      glPopAttrib();
     }

   if (distscaleflag == 1)
     {
      mindist = 0;
      getscale(mindist, maxdist, 6, 1, 0, distints, &nints);
      xtimepos = distscale_x;
      ytimepos = distscale_y - 10;
      xds = (windowwidth+1 - distscale_x - 20) * (distints[1] / maxdist);
      chkpos = distscale_x + xds * nints;
      if (chkpos > windowwidth) nints--;
      for (i = 0; i < nints; i++)
        {
         v1[0] = xtimepos + xds * i;
         v1[1] = ytimepos;
         v1[2] = 0.;
         v2[0] = v1[0];
         v2[1] = ytimepos + 10.;
         v2[2] = 0.;
         glBegin(GL_LINE_STRIP);
         glVertex3fv(v1);
         glVertex3fv(v2);
         glEnd();
         glRasterPos3fv(v2);
         glPushAttrib(GL_LIST_BIT);
         glListBase(fontOffset);
         sprintf(strt,"%-8.5g",distints[i]);
         glCallLists(strlen(strt), GL_UNSIGNED_BYTE, (GLubyte *) strt);
         if (i < nints-1)
           {
            for (j = 1; j < 4; j++)
              {
               v1[0] += .25 * xds;
               v2[0] = v1[0];
               v2[1] = ytimepos + 6.;
               if (j == 2) v2[1] = ytimepos + 9.;
               glBegin(GL_LINE_STRIP);
               glVertex3fv(v1);
               glVertex3fv(v2);
               glEnd();
              }
           }
        }
      v1[0] = xtimepos;
      v1[1] = ytimepos;
      v1[2] = 0.;
      v2[0] = xtimepos + xds * (nints-1);
      v2[1] = ytimepos;
      v2[2] = 0.;
      glBegin(GL_LINE_STRIP);
      glVertex3fv(v1);
      glVertex3fv(v2);
      glEnd();
     }

   /*  Draw flight crosshair.  */
   if (viewflag == 2 && 
       ((animationflag == 0 && anim_snapshotsflag == 0) && 
        snapshotflag == 0 && snapshotsize == 0 &&
        animfly_snapflag == 0))
      drawflycross();

  /*  Draw highlight axis, if requested.  */
  if (viewflag < 2 && o_axisflag)
     {

      /*  Set view.  */
      glPushMatrix();
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.,xsize,0.,ysize,-60.,60.);
      glMatrixMode(GL_MODELVIEW);
      //o_axis_x = xsize - 60.;
      //o_axis_y = 60.;
      Ox = (float)o_axis_x;
      Oy = (float)o_axis_y;
      glTranslatef(Ox, Oy, 0.);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);

      /*  Set rotation.  */
      glRotatef( -( (GLfloat)(zrot) ), 0., 0., 1.);
      glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
      glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
      if (animationflag == 1)
        {
         glRotatef(zanimrot,0.0,0.0,1.0);
         glRotatef(yanimrot,0.0,1.0,0.0);
         glRotatef(xanimrot,1.0,0.0,0.0);
        }

      /*  Set thick line and call drawaxes.  */
      glLineWidth(3.);
      drawaxes(3);

      /*  Reset line width.  */
      glLineWidth((GLfloat)line_size);
      glPopAttrib();
     }
}


void drawwinaxis()
{
  unsigned long tempcolor;
  float oldxrot, oldyrot;
  double xa, ya, za, ra;

   if (viewflag == 2)
     {
      oldxrot = xrot;  oldyrot = yrot;
      xa = (double)flypos[0] + (double)xtrans;
      ya = (double)flypos[1] + (double)ytrans;
      za = (double)flypos[2] + (double)ztrans;
      ra = sqrt(xa*xa + ya*ya + za*za);
      xrot = 90. - (180./3.14592654) * atan2(xa,ya);
      yrot = 90. - (180./3.14592654) * acos((double)(za/ra));
      if (xrot < -180.) xrot += 360.;
      else if (xrot > 180.) xrot -= 360.;
      if (yrot < -180.) yrot += 360.;
      else if (yrot > 180.) yrot -= 360.;
      if (zrot < -180.) zrot += 360.;
      else if (zrot > 180.) zrot -= 360.;     
     }

   glXMakeCurrent(dpy,winaxisid,glx_axglw_context);
 
   tempcolor = txtcolor;
   txtcolor = 0xff;
   glPushMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-span*.5*aspect,span*.5*aspect,-span*.5,span*.5,0.,span*2.);
   glMatrixMode(GL_MODELVIEW);
   glTranslatef(0., 0., -((GLfloat)span));
   glRotatef( -( (GLfloat)(zrot) ), 0., 0., 1.);
   glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
   glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
   glClearDepth(1.0);
   glClearColor(1.0,1.0,1.0,1.0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   drawaxes(2);
   glPopMatrix();
   txtcolor = tempcolor;
 
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winaxisid);

   if (viewflag == 2)
     {
      xrot = oldxrot;  yrot = oldyrot;
     }
}


void drawflycross()
{
  float xcr, ycr, v1[2];

   /*  Draw red crosshair through center of window,  */
   /*  then blue and green lines next to red lines.  */
   xcr = xsize / 2.;
   ycr = ysize / 2.;

   glColor3f(1., 0., 0.);
   v1[0] = xcr;  v1[1] = 0.;
   glBegin(GL_LINE_STRIP);
   glVertex2fv(v1);
   v1[1] = ysize;
   glVertex2fv(v1);
   glEnd();
   v1[0] = 0.;  v1[1] = ycr;
   glBegin(GL_LINE_STRIP);
   glVertex2fv(v1);
   v1[0] = xsize;
   glVertex2fv(v1);
   glEnd();

   glColor3f(0., 0., 1.);
   v1[0] = xcr - 100.;  v1[1] = ycr + 100.;
   glBegin(GL_LINE_STRIP);
   glVertex2fv(v1);
   v1[0] = xcr + 100.;  v1[1] = ycr - 100.;
   glVertex2fv(v1);
   glEnd();

   glColor3f(0., 1., 0.);
   v1[0] = xcr - 100.;  v1[1] = ycr - 100.;
   glBegin(GL_LINE_STRIP);
   glVertex2fv(v1);
   v1[0] = xcr + 100.;  v1[1] = ycr + 100.;
   glVertex2fv(v1);
   glEnd(); 
 
}


void buildelems()
{
  int i, j, ii, solidflg;
  float chkalpha;

   /*  Generate drawing elements.  */
   drawarray = (struct trielem *)malloc(1000*sizeof(struct trielem));
   drawarray_alloc = 1000;
   numtris = 0;
   plvarray = (struct plvelem *)malloc(1000*sizeof(struct plvelem));
   plvarray_alloc = 1000;
   numplv = 0;
   drawarray2 = (struct trielem *)malloc(100*sizeof(struct trielem));
   drawarray2_alloc = 100;
   numtris2 = 0;
   drawarray3 = (struct trielem *)malloc(10*sizeof(struct trielem));
   drawarray3_alloc = 10;
   numtris3 = 0;
   plvarray3 = (struct plvelem *)malloc(10*sizeof(struct plvelem));
   plvarray3_alloc = 10;
   numplv3 = 0;

   /*  Generate polygon lines and/or solid faces triangles.  */
   for (ii = 0; ii < nmat_order; ii++) 
     {
      i = mat_order[ii];
      if ((linesflag == 1) && (active_mat[i])) 
         drawpolylines(i);  
      if ((polysflag == 1) && (active_mat[i])) 
         drawpolygons(i);
     } 

   /*  Generate node vectors.  */
   if (vectorflag == 1)
     {
      if (animationflag == 1 && animvelflowflag == 1)
         drawanimvelflow(1);
      else drawvectors(1);
     }

   /*  Generate node points.  */
   if (nodesflag > 0 || nodenumflag == 1)
      drawnodepoints();

   /*  Generate tracer points.  */
   if (tracerflag > 0) drawtracers();

   /*  Generate grid analysis cell edge lines and/or face triangles.  */
   if (gridanalflag == 1)
     {
      if (gridedgesflag || gacellnumflag || ganodenumflag ||
          gafacenumflag || gridmedianedgeflag || gridvoronoiedgeflag)
         drawcelledges(1);
      if (gridsflag == 1 || gridmedianflag == 1 || gridvoronoiflag == 1)
         drawcellfaces(1);
     }

   /*  Generate surface edge lines and/or face tirangles.  */
   if (surfedgesflag == 1 || surfnumflag == 1)
      drawsurfedges();
   if (surfflag == 1) drawsurfaces();

   /*  Generate cell edge lines and/or face triangles.  */
   if (celledgesflag || cellnumflag || cellnodenumflag ||
       cellfacenumflag || cellmedianedgeflag == 1)
      drawcelledges(0);
   if (cellsflag == 1) drawcellfaces(0);

   /*  Generate hidden cell edges triangles and lines.  */
   if (cellhiddenedgesflag) drawcellhiddenedges();

   /*  Draw unselected cells as black wireframe.  */
   if (mesh2dflag == 0 && drawunselcells)
      drawcelledges(2);

   /*  Generate cell vectors.  */
   if (cvectorflag == 1)
     {
      if (animationflag == 1 && animvelflowflag == 1)
         drawanimvelflow(0);
      else drawvectors(0);
     }

   /*  Generate surface vectors.  */
   if (svectorflag == 1)
     {
      if (animationflag == 1 && animvelflowflag == 1)
         drawanimvelflow(2);
      else drawvectors(2);
     }

   /*  Generate face vectors.  */
   if (fvectorflag == 1)
     {
      if (animationflag == 1 && animvelflowflag == 1)
         drawanimvelflow(3);
      else drawvectors(3);
     }

   /*  Generate cutplanes elements.  */
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      if (cutplane_on[i] > 0) drawcutplane(i);
     }

   /*  Generate isosurface triangles.  */
   for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++) 
     {
      if (contour_on[i] == 1 && num_of_contour_polys[i] > 0)
        {
         if (i < MAXCONTOURS || i >= MAXCONTOURS+MAXMATS)
           {
            j = i*4;
            if (i == MAXCONTOURS+MAXMATS) j = MAXCONTOURS * 4;
            if (i == MAXCONTOURS+MAXMATS+1) j = (MAXCONTOURS+1) * 4;
            chkalpha = conrgba[3+j];
           }
         else
           {
            j = (i-MAXCONTOURS)*4;
            chkalpha = matrgba[3+j];
           }
         solidflg = 1;
         if (chkalpha > .999) solidflg = 0;
         drawisosurface(i,solidflg);
        }
     }

   /*  Generate rays.  */
   if (drawrayflag) drawrays();
}


void drawfastbox(int fastboxcount)
{
  int fov;
  float magnify2, fclip2, bclip2, minfclip2;

   if (drawincr < 99)
     {
      drawit();
      return;
     }

   /*  Draw box on top of pixel image.  */
#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
   glClearDepth(1.0);
   glClearColor(1.0,1.0,1.0,1.0);
   /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
 
   /*  Set to 2d mode to place pixel image.  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fclip2 = 0.;
   bclip2 = 2.*span*magnify;
   glOrtho(0.,xsize,0.,ysize,0.,1.);
   glMatrixMode(GL_MODELVIEW);
   /* glDrawBuffer(GL_FRONT_AND_BACK); */
   if (fastboxcount == 0)
     {
      glClear(GL_DEPTH_BUFFER_BIT);
      glRasterPos3f(0.,0.,0.);
      glDrawPixels(xsize,ysize,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,depthbuff);
     }
   glDisable(GL_DEPTH_TEST);
   glRasterPos3f(0.,0.,0.);
   glDrawPixels(xsize,ysize,GL_RGBA,GL_UNSIGNED_BYTE,exposeimage);
   glEnable(GL_DEPTH_TEST);
   glDepthMask(GL_FALSE);

   /*  Return to 3D mode and draw box(s).  */

   /*  Set magnification depending on view type.  */
   if (viewflag == 0) magnify2 = magnify;
   if (viewflag == 1)
     {
      magnify2 = 1.;
      fov = (int)(600./magnify);
      if (!stereoflag)
        {
         if (fov < 10.) fov = 10.;
         if (fov > 1790.) fov = 1790.;
        }
     }
   if (viewflag == 2)
     {
      magnify2 = 1.;
      fov = 600;
      if (stereoflag) fov = 800;
     }

   glViewport(0,0,windowwidth+1,windowheight+1);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fclip2 = 0.;
   bclip2 = 2.*span*magnify2;
   minfclip2 = span / 100.;
   if (viewflag == 1 && fclip2 < minfclip2) fclip2 = minfclip2;
   if (viewflag == 2)
     {
      minfclip2 /= 100.;
      fclip2 = minfclip2;
      if (minfclip2 < .01) fclip2 = .01;
      if (minfclip2 > 10.) fclip2 = 10.;
      if (stereoflag) fclip2 *= 10.;
      bclip2 = 3.*span;
     }
   if (viewflag == 0)
      glOrtho(-span*.5*aspect,span*.5*aspect,-span*.5,span*.5,fclip2,bclip2);
   if (viewflag > 0)
      gluPerspective(fov/10.,aspect,fclip2,bclip2);
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);

   /*  Set up rotation, pan, etc.  */
   glPushMatrix();
   if (viewflag < 2)
     {
      glScalef(magnify2, magnify2, magnify2);
      glTranslatef(xpan, ypan, 0.);
      glTranslatef(0., 0., -((GLfloat)span));
      glRotatef( -( (GLfloat)(zrot)), 0., 0., 1.);
      glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
      glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
     }
   if (viewflag == 2)
     {
      glTranslatef(-xpan,-ypan,-zpan);
      xpan = 0.;  ypan = 0.;  zpan = 0.;
      glRotatef((GLfloat)(90.-zrot),0.,0.,1.);
      glRotatef((GLfloat)yrot,0.,1.,0.);
      glRotatef((GLfloat)xrot,0.,0.,1.);
      glTranslatef(-flypos[0],-flypos[1],-flypos[2]);
     }
   glTranslatef(xtrans, ytrans, ztrans);

   /*  Draw the subset bounding box, if needed.  */
   if (subsetbox_draw)
      drawsubsetbox();

   /*  Draw the cell selection box, if needed.  */
   if (cellselbox_draw) 
     {
      drawselectbox(0);
     }

   /*  Draw the node selection box, if needed.  */
   if (nodeselbox_draw) 
      drawselectbox(1);

   /*  Draw the surface selection box, if needed.  */
   if (surfselbox_draw) 
      drawselectbox(2);

   /*  Draw the polygon selection box, if needed.  */
   if (polyselbox_draw) 
      drawselectbox(3);

   /*  Draw the trace selection box, if needed.  */
   if (traceselbox_draw) 
      drawselectbox(4);

   glPopMatrix();
   if (doubleBuffer == GL_TRUE)
     {
      glXSwapBuffers(dpy, windowid);
     }
   glDepthMask(GL_TRUE);
   glFlush();
}


void drawrbzoom(int rbx1, int rby1, int rbx2, int rby2)
{
  int rby3, rby4;

#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
 
   if (drawincr < 99)
     {
      fastdraw = 1;
      if (rbzoomflag == 2) fastdraw = 0;
      setmag_clip();
      drawimage();
      fastdraw = 0;
     }

   /*  Set to 2d mode.  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fclip = 0.;
   bclip = 2.*span*magnify;
   glOrtho(0.,xsize,0.,ysize,0.,1.);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_DEPTH_TEST);

   if (drawincr >= 99)
     {
      glRasterPos3f(0.,0.,.0);
      glDrawPixels(xsize,ysize,GL_RGBA,GL_UNSIGNED_BYTE,exposeimage);
     }

   glDisable(GL_LIGHTING);
   glLineWidth(2.0);
   glDisable(GL_LINE_SMOOTH);

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);
   rby3 = windowheight - rby1;
   rby4 = windowheight - rby2;
   glBegin(GL_LINE_LOOP);
   glVertex2i(rbx1,rby3);
   glVertex2i(rbx2,rby3);
   glVertex2i(rbx2,rby4);
   glVertex2i(rbx1,rby4);
   glEnd();

   if (doubleBuffer == GL_TRUE)
     {
      glXSwapBuffers(dpy, windowid);
     }
   glFlush();

   glLineWidth((float)line_size);
   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   glPopMatrix();
   /*  Return to 3d mode.  */
   if (rbzoomflag == 2)
   return3d();
   glFlush();
}


void drawselpoly(int motion)
{
  int i, xx, yy;
  float pt_size;

   /* if (nsurfpolyverts < 2 && motion == 0) return; */

#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
 
   if (drawincr < 99)
     {
      fastdraw = 0;
      /*
      if (surfselpolyopt == 2) fastdraw = 0;
      if (motion) fastdraw = 1;
      if (nsurfpolyverts == 1 && !motion) fastdraw = 0;
      */
      setmag_clip();
      drawimage();
      fastdraw = 0;
     }

   /*  Set to 2d mode.  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fclip = 0.;
   bclip = 2.*span*magnify;
   glOrtho(0.,xsize,0.,ysize,0.,1.);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_DEPTH_TEST);

   if (drawincr >= 99)
     {
      glRasterPos3f(0.,0.,.0);
      glDrawPixels(xsize,ysize,GL_RGBA,GL_UNSIGNED_BYTE,exposeimage);
     }

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   if (nsurfpolyverts < 2)
     {
      pt_size = line_size + 3;
      glPointSize(pt_size);
      glBegin(GL_POINTS);
      xx = surfpolyx[0];
      yy = windowheight - surfpolyy[0];
      glVertex2i(xx,yy);
      glEnd();
      pt_size = (float)pnt_size;
      glPointSize(pt_size);

      if (!motion)
        {
         if (doubleBuffer == GL_TRUE)
           {
            glXSwapBuffers(dpy, windowid);
           }
         glFlush();
         return;
        }
      if (nsurfpolyverts == 0 && motion) return;
     }

   glDisable(GL_LIGHTING);
   glLineWidth(2.0);
   glDisable(GL_LINE_SMOOTH);

   glBegin(GL_LINE_STRIP);
   for (i = 0; i < nsurfpolyverts; i++)
     {
      xx = surfpolyx[i];
      yy = windowheight - surfpolyy[i];
      glVertex2i(xx,yy);
     }
   if (motion)
     {
      xx = surfpolyx[nsurfpolyverts];
      yy = windowheight - surfpolyy[nsurfpolyverts];
      glVertex2i(xx,yy);
     }
   if (surfselpolyopt == 2)
     {
      xx = surfpolyx[0];
      yy = windowheight - surfpolyy[0];
      glVertex2i(xx,yy);
     }
   glEnd();

   if (doubleBuffer == GL_TRUE)
     {
      glXSwapBuffers(dpy, windowid);
     }
   glFlush();

   glLineWidth((float)line_size);
   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   glPopMatrix();
   /*  Return to 3d mode.  */
   if (surfselpolyopt == 2)
      return3d();
   glFlush();
}


void expose_redraw()
{
  short tmp_animationflag, tmp_anim_snapshotsflag, tmp_snapshotsize,
        tmp_animfly_snapflag;

   if (stereoflag == 0 && drawincr >= 99)
     {
#ifdef BATCH
      OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
      glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
      glClearDepth(1.0);
      glClearColor(1.0,1.0,1.0,1.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
 
      /*  Set to 2d mode.  */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.,xsize,0.,ysize,0.,1.);
      glMatrixMode(GL_MODELVIEW);
      glRasterPos3f(0.,0.,.0);
      glDrawBuffer(GL_FRONT_AND_BACK);
      glDrawPixels(xsize,ysize,GL_RGBA,GL_UNSIGNED_BYTE,exposeimage);
      glDrawBuffer(GL_FRONT_AND_BACK);
      if (doubleBuffer == GL_TRUE)
         glDrawBuffer(GL_BACK);

      /*  Return to 3D mode.  */
      return3d();

     }
   else
     {
      tmp_animationflag = animationflag;
      tmp_anim_snapshotsflag = anim_snapshotsflag;
      tmp_snapshotsize = snapshotsize;
      tmp_animfly_snapflag = animfly_snapflag;
      animationflag = 0;
      anim_snapshotsflag = 0;
      snapshotsize = 0;
      animfly_snapflag = 0;
      drawit();
      animationflag = tmp_animationflag;
      anim_snapshotsflag = tmp_anim_snapshotsflag;
      snapshotsize = tmp_snapshotsize;
      animfly_snapflag = tmp_animfly_snapflag;
     }
}


void draworientbox()
{
  int fov;
  float magnify2, fclip2, bclip2, minfclip2;
  float v1[3], v2[3], extra;
  char str[10];

  /*  This routine draws a bounding box.  */

   if (drawincr < 99 || viewflag == 2)
     {
      drawit();
      return;
     }

#ifdef BATCH
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,xsize,ysize);
#else
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif
   glClearDepth(1.0);
   glClearColor(1.0,1.0,1.0,1.0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   /*  Set magnification depending on view type.  */
   if (viewflag == 0) magnify2 = magnify;
   if (viewflag == 1)
     {
      magnify2 = 1.;
      fov = (int)(600./magnify);
      if (!stereoflag)
        {
         if (fov < 10.) fov = 10.;
         if (fov > 1790.) fov = 1790.;
        }
     }
   if (viewflag == 2)
     {
      magnify2 = 1.;
      fov = 600;
      if (stereoflag) fov = 800;
     }

   glViewport(0,0,windowwidth+1,windowheight+1);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   fclip2 = 0.;
   bclip2 = 2.*span*magnify2;
   minfclip2 = span / 100.;
   if (viewflag == 1 && fclip2 < minfclip2) fclip2 = minfclip2;
   if (viewflag == 2)
     {
      minfclip2 /= 100.;
      fclip2 = minfclip2;
      if (minfclip2 < .01) fclip2 = .01;
      if (minfclip2 > 10.) fclip2 = 10.;
      if (stereoflag) fclip2 *= 10.;
      bclip2 = 3.*span;
     }
   if (viewflag == 0)
      glOrtho(-span*.5*aspect,span*.5*aspect,-span*.5,span*.5,fclip2,bclip2);
   if (viewflag > 0)
      gluPerspective(fov/10.,aspect,fclip2,bclip2);
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);
   glLineWidth(2.0);
   glDisable(GL_LINE_SMOOTH);

   /*  Set up rotation, pan, etc.  */
   glPushMatrix();
   if (viewflag < 2)
     {
      glScalef(magnify2, magnify2, magnify2);
      glTranslatef(xpan, ypan, 0.);
      glTranslatef(0., 0., -((GLfloat)span));
      glRotatef( -( (GLfloat)(zrot)), 0., 0., 1.);
      glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
      glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
     }
   if (viewflag == 2)
     {
      glTranslatef(-xpan,-ypan,-zpan);
      xpan = 0.;  ypan = 0.;  zpan = 0.;
      glRotatef((GLfloat)(90.-zrot),0.,0.,1.);
      glRotatef((GLfloat)yrot,0.,1.,0.);
      glRotatef((GLfloat)xrot,0.,0.,1.);
      glTranslatef(-flypos[0],-flypos[1],-flypos[2]);
     }
   glTranslatef(xtrans, ytrans, ztrans);

   glDisable(GL_LIGHTING);

   /*  Offset polygons.  */
   if (poly_offset > 0)
     {
#if GL_VERSION_1_1
      if (poly_offset == 2)
         glEnable(GL_POLYGON_OFFSET_FILL);
#elif GL_EXT_polygon_offset
      if (poly_offset == 1)
         glEnable(GL_POLYGON_OFFSET_EXT);
#endif
     }

   /*  Draw X faces in red.  */
   glColor3f(1.0,0.0,0.0);
   glBegin(GL_POLYGON);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   glEnd();

   glBegin(GL_POLYGON);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   glEnd();

   /*  Draw Y faces in green.  */
   glColor3f(0.0,1.0,0.0);
   glBegin(GL_POLYGON);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   glEnd();

   glBegin(GL_POLYGON);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   glEnd();

   /*  Draw Z faces in blue.  */
   glColor3f(0.0,0.0,1.0);
   glBegin(GL_POLYGON);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   glVertex3fv(v1);
   glEnd();

   glBegin(GL_POLYGON);
   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   glVertex3fv(v1);
   glEnd();

   /*  Draw X and lines in black.  */
   /* glColor3f(1.0,0.0,0.0); */
   glColor3f(0.0,0.0,0.0);
   extra = .01 * (xmaxbbval - xminbbval);
   glRasterPos3f(xmaxbbval+extra,yminbbval,zminbbval);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset2);
   strcpy(str,"X");
   glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
   glPopAttrib ();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .25 * (ymaxbbval - yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .25 * (ymaxbbval - yminbbval);
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .25 * (ymaxbbval - yminbbval);
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .25 * (ymaxbbval - yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .5 * (ymaxbbval - yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .5 * (ymaxbbval - yminbbval);
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .5 * (ymaxbbval - yminbbval);
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .5 * (ymaxbbval - yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .75 * (ymaxbbval - yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .75 * (ymaxbbval - yminbbval);
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .75 * (ymaxbbval - yminbbval);
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .75 * (ymaxbbval - yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   /*  Draw Y and lines in black.  */
   /* glColor3f(0.0,1.0,0.0); */
   glColor3f(0.0,0.0,0.0);
   extra = .01 * (ymaxbbval - yminbbval);
   glRasterPos3f(xminbbval,ymaxbbval+extra,zminbbval);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset2);
   strcpy(str,"Y");
   glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
   glPopAttrib ();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .25 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .5 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval + .75 * (zmaxbbval -zminbbval);
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   /*  Draw Z and lines in black.  */
   /* glColor3f(0.0,0.0,1.0); */
   glColor3f(0.0,0.0,0.0);
   extra = .01 * (zmaxbbval - zminbbval);
   glRasterPos3f(xminbbval,yminbbval,zmaxbbval+extra);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset2);
   strcpy(str,"Z");
   glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
   glPopAttrib ();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .25 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .5 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval + .75 * (xmaxbbval - xminbbval);
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .25 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = yminbbval + .25 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval + .25 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .25 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .5 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = yminbbval + .5 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval + .5 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .5 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval + .75 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = yminbbval + .75 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval + .75 * (ymaxbbval -yminbbval);
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval + .75 * (ymaxbbval -yminbbval);
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   glPopMatrix();
   if (doubleBuffer == GL_TRUE)
     {
      glXSwapBuffers(dpy, windowid);
     }

   glEnable(GL_LIGHTING);

   glLineWidth((float)line_size);
   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   if (poly_offset > 0)
     {
#if GL_VERSION_1_1
      if (poly_offset == 2)
         glDisable(GL_POLYGON_OFFSET_FILL);
#elif GL_EXT_polygon_offset
      if (poly_offset == 1)
         glDisable(GL_POLYGON_OFFSET_EXT);
#endif
     }
}


struct elem {double z; int loc;} *table;


int z_compare(void * e1, void * e2)
{
  /*  Compare routine for qsort.  */
  double v1, v2;

   v1 = ((struct elem *)e1)->z;
   v2 = ((struct elem *)e2)->z;
   return (v1 > v2) ? -1 : (v1 < v2) ? 1 : 0;
}


void copy_elem(struct trielem *t1, struct trielem *t2)
{
   t2->itype = t1->itype;
   t2->x1 = t1->x1;
   t2->y1 = t1->y1;
   t2->z1 = t1->z1;
   t2->x2 = t1->x2;
   t2->y2 = t1->y2;
   t2->z2 = t1->z2;
   t2->x3 = t1->x3;
   t2->y3 = t1->y3;
   t2->z3 = t1->z3;
   t2->xnorm = t1->xnorm;
   t2->ynorm = t1->ynorm;
   t2->znorm = t1->znorm;
   t2->iclr1 = t1->iclr1;
   t2->iclr2 = t1->iclr2;
   t2->iclr3 = t1->iclr3;
}


void sort_trans_tris(struct trielem * drawtrans, int ntris)
{
  /*  This routine sorts the projected z value of the first vertex    */
  /*  in the triangles.  The trianges are then placed in this order.  */ 

  int i, j;
  struct trielem *tmptrans;
  GLint vport[4];
  GLdouble mvmatrix[16], projmatrix[16], px, py, pz, wx, wy, wz;

   /*  Get the projection matrices.  */
   glGetIntegerv(GL_VIEWPORT, vport);
   glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

   /*  Allocate and build table.  */
   table = (struct elem *)malloc(ntris * sizeof(struct elem));
   for (i = 0; i < ntris; i++)
     {

      /*  Project the first triangle vertex into screen coordinates.  */
      px = drawtrans[i].x1;  py = drawtrans[i].y1;  pz = drawtrans[i].z1;
      gluProject(px,py,pz,mvmatrix,projmatrix,vport,&wx,&wy,&wz);
      table[i].z = wz;
      table[i].loc = i;
     }

   /*  Sort the z values with qsort.  */
   qsort((void *)table, (size_t) ntris, sizeof(struct elem), z_compare);

   /*  Now reset the triangles in drawtrans according to sort order.  */
   tmptrans = (struct trielem *)malloc(ntris * sizeof(struct trielem));
   for (i = 0; i < ntris; i++) copy_elem(&drawtrans[i],&tmptrans[i]);
   for (i = 0; i < ntris; i++)
     {
      j = table[i].loc;
      copy_elem(&tmptrans[j],&drawtrans[i]);
     }

   free(table);  free(tmptrans);
}
