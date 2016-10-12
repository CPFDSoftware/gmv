
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/PushBG.h>

#include <stdlib.h>
#include <sys/types.h>
 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include "GLwMDrawA.h"
#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

#ifdef __sgi
#include <X11/extensions/SGIStereo.h>
#endif

extern GLXContext glx_glw_context, glx_axglw_context, glx_stglw_context;
extern GLboolean  doubleBuffer;
extern char monstat[40];

static Widget stereoclose;
static unsigned int wwidth, wheight, stw, sth;
static short oldviewflag;
static int monw;


void stereoformpop(int retviewflag)
{
  Window  root;
  int  x, y, i, stat;
  unsigned int border_width, wdepth;
  static short ifirst = 1;
  float ptsize, lnsize;
  GLfloat lightPosition[4];
 
   stat = XGetGeometry(dpy,topwin,&root,&x,&y,&wwidth,&wheight,
                       &border_width,&wdepth);
   stereoflag = 1;
   oldviewflag = retviewflag;
   if (viewflag == 3) viewflag = 1;
   else viewflag = 2;

   /* popdownall(); */
   XtUnmanageChild(form);
#ifdef __sgi
   if (stereowindow)
     {
      if (monw < 1600)
         system("/usr/gfx/setmon -n 1024x768_114s");
      if (monw == 1600)
         system("/usr/gfx/setmon -n 1600x1024_100s");
      if (monw > 1600)
         system("/usr/gfx/setmon -n 1360x768_114s");
     }
   else
      system("/usr/gfx/setmon -n STR_BOT");
#endif
   XFlush(dpy);
   XtVaSetValues(toplevel,XmNmwmDecorations,0,NULL);
   XtVaSetValues(toplevel,XmNx,0, XmNy,0, XmNwidth,stw + 10,
                 XmNheight,sth + 10, NULL);
   XtVaSetValues(stereoform,XmNmwmDecorations,0, NULL);
   MyManageChild(stereoform);
   MyManageChild(stglw);
   /* XmProcessTraversal(stglw, XmTRAVERSE_CURRENT); */
   XmProcessTraversal(stereoform, XmTRAVERSE_CURRENT);
   XFlush(dpy);
   MyManageChild(stereoclose);
   winstid = XtWindow(stglw);
   glXMakeCurrent(dpy,winstid,glx_stglw_context);

   /*  Set lighting model and position for stereo window.  */
   glEnable(GL_LIGHT0);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   lightPosition[0] = (GLfloat)lightx;
   lightPosition[1] = (GLfloat)lighty;
   lightPosition[2] = (GLfloat)1.0;
   lightPosition[3] = (GLfloat)0.0;
   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

   /*  Set point size and shape for this window.  */
   ptsize = (float)pnt_size;
   glPointSize(ptsize);
   if (pnt_shape == 1)
      glEnable(GL_POINT_SMOOTH);
   else
      glDisable(GL_POINT_SMOOTH);

   /*  Set line width for this window.  */
   lnsize = (float)line_size;
   glLineWidth(lnsize);
   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   /*  If first time, set texture map.  */
   if (ifirst)
     {
      ifirst = 0;
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
   else
     {

      /*  Reset texture values.  */
      for (i = 0; i < 256; i++)
        {
         textures[3*i+0] = (float)(jcolor[i]>>24 & 255) / 255.;
         textures[3*i+1] = (float)(jcolor[i]>>16 & 255) / 255.;
         textures[3*i+2] = (float)(jcolor[i]>>8 & 255) / 255.;
        }
      glTexSubImage1D(GL_TEXTURE_1D,0,0,256,GL_RGB,GL_FLOAT,textures);
     }

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}

void stclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Widget v;
  char retmon[80];

   /*  Clear the stereo buffers to black.  */
   /* ichk = (int)glXMakeCurrent(dpy,winstid,glx_stglw_context); */

   /*  Clear left eye buffers.  */
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
   glClearDepth(1.0);
   glClearColor(0.0,0.0,0.0,1.0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);

   /*  Clear right eye buffers.  */
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
   glClearDepth(1.0);
   glClearColor(0.0,0.0,0.0,1.0);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);

   stereoflag = 0;
   XmProcessTraversal(stglw, XmTRAVERSE_HOME);
   XtUnmanageChild(stereoform);

#ifdef __sgi
   sprintf(retmon,"/usr/gfx/setmon -n %s",monstat);
   system(retmon);
#endif

   XFlush(dpy);
   XtVaSetValues(toplevel,XmNmwmDecorations,1,NULL);
   XtVaSetValues(toplevel,XmNx,10, XmNy,30, 
                 XmNwidth,wwidth,  XmNheight,wheight, NULL);
   XtManageChild(form);

   viewflag = oldviewflag;
   v = view1;
   if (viewflag == 1) v = view2;
   if (viewflag == 2) v = view3;
   XmToggleButtonGadgetSetState(view4,FALSE,FALSE);
   XmToggleButtonGadgetSetState(view5,FALSE,FALSE);
   XmToggleButtonGadgetSetState(v,TRUE,TRUE);
   XFlush(dpy);
 
   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


/*  GLX callbacks.  */

void init_stwindow(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 XtPointer call_data;
{
  int ichk;

   glx_stglw_context = glXCreateContext(XtDisplay(stglw), vi, 
                                        glx_glw_context, GL_TRUE);
   winstid = XtWindow(stglw);
   /* glXMakeCurrent(dpy,winstid,glx_stglw_context); */

      XFlush(dpy);
      ichk = (int)glXMakeCurrent(dpy,winstid,glx_stglw_context);
      glClearDepth(1.0);
      glClearColor(1.0,1.0,1.0,1.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winstid);
}

 
void expose_stwindow(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 caddr_t call_data;
{
   drawit();
}


void stinput(w, client_data, event)
 Widget w; 
 caddr_t client_data;
 XKeyEvent *event;
{
  char buffer[20];
  int bufsize = 20, count;
  KeySym key;

   count = XLookupString(event,buffer,bufsize,&key,NULL);
   if (count == 1)
     {
       if (strncmp(buffer,"x",1) == 0)
          stclose(NULL,NULL,NULL);
       if (strncmp(buffer,"m",1) == 0)
         {
          eyescale += 0.1;
          drawit();
         }
       if (strncmp(buffer,"n",1) == 0)
         {
          eyescale -= 0.1;
          if (eyescale < 0.) eyescale = 0.;
          drawit();
         }
       if (strncmp(buffer,"r",1) == 0)
         {
          eyescale = 1.;
          drawit();
         }
     }
}


void makestereoform(parent)
 Widget parent;
{
  int i;
  char str[20];  Arg args[20];
  XmString string;
  void input(), expose_stwindow(), init_stwindow(), stinput();

   /*  Set window sizes depending on monitor capability.  */
   monw = 1280;
#ifdef IRSTEREO
   strncpy(str,monstat,4);
   monw = atoi(str);
#endif
#ifdef __sgi
   if (stereowindow)
     {
      if (monw < 1600)
        {
         stw = 1024;  sth = 768;
        }
      if (monw == 1600)
        {
         stw = 1600;  sth = 1024;
        }
      if (monw > 1600)
        {
         stw = 1360;  sth = 768;
        }
      stwinw = stw;  stwinh = sth;
     }
   else
     {
      stw = 1280; sth = 1024;
      stwinw = 1280;  stwinh = 492;
     }
#else
   stw = windowxmax;  sth = windowymax;
   stwinw = stw;  stwinh = sth;
#endif

   stereoform = XtVaCreateWidget("Stereoform",xmMainWindowWidgetClass,
                                 parent, XmNheight,sth + 10, XmNwidth,stw + 10,
                                 XmNtraversalOn, TRUE, NULL);
      XtAddEventHandler(stereoform,KeyPressMask,NULL,stinput,(caddr_t) 0);

   /*  Create stereo drawing window.  */
   i=0; XtSetArg(args[i], XmNheight,sth);
   i++; XtSetArg(args[i], XmNwidth,stw);
   i++; XtSetArg(args[i], GLwNvisualInfo, vi);
   i++; XtSetArg(args[i], GLwNinstallColormap, TRUE);
   /* i++; XtSetArg(args[i], XmNtraversalOn, TRUE); */
   /* i++; XtSetArg(args[i], XmNnavigationType, XmTAB_GROUP); */
   stglw = GLwCreateMDrawingArea(stereoform,"Stglw",args,i);
      XtAddCallback(stglw,GLwNexposeCallback,expose_stwindow,
                    (caddr_t) 0);
      XtAddCallback(stglw,GLwNginitCallback,init_stwindow,
                    (caddr_t) 0);
      XtAddCallback(stglw,GLwNinputCallback,input,(caddr_t) 0);

   XtVaSetValues(stereoform,NULL,NULL,NULL,NULL,stglw);
 
   string = XmStringCreate("Exit",XmSTRING_DEFAULT_CHARSET);
   stereoclose = XtVaCreateManagedWidget("Stereoclose",
               xmPushButtonGadgetClass,
               stereoform,
               XmNx,0,
               XmNy,0,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(stereoclose,XmNactivateCallback,stclose,NULL);
}
