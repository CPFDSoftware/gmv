
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "GLwMDrawA.h"
#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

static Widget matsbut, snapbut, matcbdraw, matcbglw;
static short matsel = 1, cflagsel = 0, nflagsel = 0, cflagno = 0, nflagno = 0,
             matcolorbarform_isup = 0;
static int maxcharsize, numsets;
static long matcbwidth = 200, matcbheight = 500, lastwidth = 200;
static Window matcbwindowid = NULL;
static GLXContext matcb_context = NULL;
extern GLboolean doubleBuffer;
extern GLXContext glx_glw_context;

void drawmatcolorbar(int snapflag);
void matcb_apply(Widget w, XtPointer c1, XtPointer c2);


void matcolorbarformpop()
{
  int i;

   MyManageChild(matcolorbarform);

   matcolorbarform_isup = 1;

   matcb_apply(NULL, NULL, NULL);
}

void matcb_close(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(matcolorbarform);

   matcolorbarform_isup = 0;
}


void matcb_snap(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string, string2, string3;

   fileselflag = MATBARSNAP;
   string = XmStringCreate("Save Material/Flag Snapshot",
                            XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[MATBARSNAP],
                            XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[MATBARSNAP],
                            XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                            XmNpattern, string2,
                            XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);

   if (fsread[MATBARSNAP])
     {
      string = XmStringCreate(matbarfilnam,
                           XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
      XmStringFree(string);
     }

   MyManageChild(file_sel2);
}


void matcb_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   int i, kk;

   /*  See how many materials or flags to display.  */
   if (matsel) numsets = maxcellmat;
   if (cflagsel) numsets = flagtypes_cell[cflagno];
   if (nflagsel) numsets = flagtypes_node[nflagno];

   /*  Determine the maximum size of the material/flag names.  */
   maxcharsize = 0;
   for (i = 0; i < numsets; i++)
     {
      if (matsel) kk = strlen(mmatnames[i]);
      if (cflagsel) kk = strlen(flagnames_cell[cflagno][i]);
      if (nflagsel) kk = strlen(flagnames_node[nflagno][i]);
      if (kk > maxcharsize) maxcharsize = kk;
     }
   drawmatcolorbar(0);
}


void init_matcbwindow(w,client_data,call_data)
 Widget w;
 caddr_t client_data;
 XtPointer call_data;
{
  int i;

   matcbwindowid  = XtWindow(w);
   matcb_context = glXCreateContext(XtDisplay(w), vi, 
                                    glx_glw_context, GL_TRUE);
   glXMakeCurrent(dpy,matcbwindowid,matcb_context);

   initmatfont(matcbwindowid,matcb_context);
}


void matcb_expose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   drawmatcolorbar(0);
}


void matcb_resize(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  GLwDrawingAreaCallbackStruct *call;

   call = (GLwDrawingAreaCallbackStruct *)call_data;
   lastwidth = (GLint)call->width;
   matcbheight = (GLint)call->height;

   glXWaitX();

   drawmatcolorbar(0);
}


void makematcolorbarform(parent)
 Widget parent;
{
  Widget matcbclose, matcbsnap, matcolorbarform2;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Material/Flags Color Bar",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   matcolorbarform = XmCreateFormDialog(parent,"Matcolorbar",wargs,i);
      XmStringFree(string);

   matcolorbarform2 = XtVaCreateManagedWidget("Matcolorbar2",
                      xmFormWidgetClass,
                      matcolorbarform,
                      XmNtopAttachment, XmATTACH_FORM,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      NULL);
 
   XtManageChild(matcolorbarform2);
   
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   matcbclose = XtVaCreateManagedWidget("Matcbclose",
                xmPushButtonGadgetClass,
                matcolorbarform2,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(matcbclose,XmNactivateCallback,matcb_close,NULL);

   string = XmStringCreate("Snapshot",XmSTRING_DEFAULT_CHARSET);
   matcbsnap = XtVaCreateManagedWidget("Matcbsnapapply",
                xmPushButtonGadgetClass,
                matcolorbarform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, matcbclose,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(matcbsnap,XmNactivateCallback,matcb_snap,
                    NULL);

   /*  Create OpenGL drawing window frame.  */
   matcbdraw = XtVaCreateManagedWidget("Matcbdraw",
               xmFrameWidgetClass,
               matcolorbarform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, matcolorbarform2,
               XmNrightAttachment, XmATTACH_FORM,
               XmNbottomAttachment, XmATTACH_FORM,
               XmNheight, matcbheight + 4,
               XmNwidth, matcbwidth + 4,
               NULL);
      XtManageChild(matcbdraw);
   
   /*  Create OpenGL drawing window.  */
   i = 0;
   XtSetArg(wargs[i], XmNheight,matcbheight);i++;
   XtSetArg(wargs[i], XmNwidth,matcbwidth);i++; 
   XtSetArg(wargs[i], GLwNvisualInfo, vi);i++;
   if (mesacmap)
     { XtSetArg(wargs[i], XmNcolormap, my_cmap);i++; }
   matcbglw = GLwCreateMDrawingArea(matcbdraw,"Matcbglw",wargs,i);
      XtManageChild (matcbglw); 
      XtAddCallback(matcbglw,GLwNexposeCallback,matcb_expose,
                    (caddr_t) 0);
      XtAddCallback(matcbglw,GLwNresizeCallback,matcb_resize,
                    (caddr_t) 0);
      XtAddCallback(matcbglw,GLwNginitCallback,init_matcbwindow,
                    (caddr_t) 0);

}


void drawmatcolorbar(int snapflag)
{
  int ncols, nrows = 32, ybeg, dy = 15, dx = 25, x, y, i, j, 
      kk, nr, nrleft, xpend, ypend;
  float aspct; 
  char title[100], name[50];
  unsigned int *sarray;
  float v1[3];
  int wx, wy;
  unsigned int dw, dh, dwidth, ddepth;
  Window root, framewin;
  GLfloat mr, mg, mb;

   /*  Get the current OpenGL window height to set the beginging y.  */
   XFlush(dpy);
   XGetGeometry(dpy,matcbwindowid,&root,&wx,&wy,&dw,&dh,
                 &dwidth,&ddepth);
   matcbheight = dh;
   ybeg = matcbheight - 20;
   lastwidth = dw;

   /*  Determine the number of rows that fit within the height.  */
   nrows = (matcbheight - (2 * dy) + 5) / dy;
   if (nrows < 1) nrows = 1;

   /*  Determine how many colums to display.  */
   ncols = numsets / nrows;
   if (numsets == nrows) ncols = 1;
   nrleft = numsets % nrows;
   if (nrleft > 0)
     {
      ncols++;
     }

   /*  Determine the line width depending  */
   /*  on the maximum character size.      */
   matcbwidth = 10 * maxcharsize + dx + 5;

   /*  Set up the OpenGL window.  */
   glXWaitX();
   glXMakeCurrent(dpy,matcbwindowid,matcb_context);
   glViewport(0,0,lastwidth,matcbheight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.,lastwidth,0.,matcbheight,0.,1.);
   glMatrixMode(GL_MODELVIEW);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);

   mr = (float)((backgroundcolor)&0xff) / 255.;
   mg = (float)((backgroundcolor)>>8&0xff) / 255.;
   mb = (float)((backgroundcolor)>>16&0xff) / 255.;
   if (mr > .997) mr = 1.0;
   if (mg > .997) mg = 1.0;
   if (mb > .997) mb = 1.0;
   glClearColor(mr,mg,mb,1.0);
   glClear(GL_COLOR_BUFFER_BIT);

   /*  Set the colorbar text color.  */
   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   /*  Draw header.  */
   if (matsel) strcpy(title,"Materials");
   if (cflagsel) sprintf(title,"Cell Flag - %s",flagname_cell[cflagno]);
   if (nflagsel) sprintf(title,"Node Flag - %s",flagname_node[nflagno]);
   x = 10;
   if (ncols > 1)
     {
      kk = strlen(title) / 2;
      x = ((ncols * matcbwidth) / 2) - (kk * 10);
      if (x < 10) x = 10; 
     }
   glRasterPos3f(x,ybeg,0.);
   glPushAttrib (GL_LIST_BIT);
   glListBase(matfontOffset);
   glCallLists(strlen(title),GL_UNSIGNED_BYTE,(GLubyte *)title);
   glPopAttrib();

   /*  Loop through the columns.  */
   kk = 0;
   for (j = 0; j < ncols; j++)
     {

      /*  Set up starting location.  */
      x = j * matcbwidth + 10;
      y = matcbheight - (2 * dy) + 5;

      /*  Loop through rows.  */
      nr = nrows;
      if (j == ncols-1 && nrleft > 0) nr = nrleft;
      for (i = 0; i < nr; i++)
        {
         if (matsel) strcpy(name,mmatnames[kk]);
         if (cflagsel) strcpy(name,flagnames_cell[cflagno][kk]);
         if (nflagsel) strcpy(name,flagnames_node[nflagno][kk]);

         /*  Get the material color.  */
         mr = matrgba[4*kk+0];
         mg = matrgba[4*kk+1];
         mb = matrgba[4*kk+2];
         kk++;

         /*  Draw the color box.  */
         v1[2] = 0;
         glColor3f(mr,mg,mb);
         glBegin(GL_POLYGON);
         v1[0] = x;
         v1[1] = y;
         glVertex3fv(v1);
         v1[0] = x;
         v1[1] = y - dy;
         glVertex3fv(v1);
         v1[0] = x + dx;
         v1[1] = y - dy;
         glVertex3fv(v1);
         v1[0] = x + dx;
         v1[1] = y;
         glVertex3fv(v1);
         glEnd();
 
         /*  Draw the name.  */
         if (txtcolor < 256) glColor3f(0.,0.,0.);
         else glColor3f(1.,1.,1.);
         glRasterPos3f(x+dx+5.,y-dy,0.);
         glPushAttrib (GL_LIST_BIT);
         glListBase(matfontOffset);
         glCallLists(strlen(name), GL_UNSIGNED_BYTE, (GLubyte *) name);
         glPopAttrib();

         y = y - dy;
        }
     }
   
   glFlush();
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, matcbwindowid);
   glXWaitGL();

   /*  Save snapshot, if requested.  */
   if (snapflag)
     {
      /* xpend = lastwidth;  ypend = matcbheight; */
      XGetGeometry(dpy,matcbwindowid,&root,&wx,&wy,&dw,&dh,
                   &dwidth,&ddepth);
      xpend = dw;  ypend = dh;
      i = xpend * ypend;
      sarray = (unsigned int *)malloc(i*sizeof(unsigned int));
      if (doubleBuffer == GL_TRUE) glReadBuffer(GL_BACK);
      else glReadBuffer(GL_FRONT);
      glReadPixels(0,0,xpend,ypend,GL_RGBA,GL_UNSIGNED_BYTE,sarray);
      xpend--;  ypend--;
      savescreen(matbarfilnam,0,xpend,0,ypend,sarray);
      free(sarray);
     }
}



void setmatcolorbar()
{
  int drawtype, allvars;
  
   /*  First see if cells are being drawn (first priority).  */
   drawtype = -1;
   if (cellsflag ||celledgesflag || cellnumflag || cellnodenumflag ||
       cellfacenumflag || cellmedianedgeflag == 1)
     {

      /*  Next see if cells are colored by material or flags.  */
      allvars = numvars + cnumvars + fnumvars;
      if (cellfieldactive == -1 || cellfieldactive >= allvars)
        {
         drawtype = 0;
         matsel = 0;  cflagsel = 0;  nflagsel = 0;
         if (cellfieldactive == -1)  /*  Materials.  */
           {
            matsel = 1;
           }
         else   /*  Flag.  */
           {
            cflagsel = 1;
            cflagno = cellfieldactive - allvars;
           }      
        }
     }

   /*  If cell materials or flags not active, check nodes.  */
   if (drawtype == -1 && (nodesflag > 0 || nodenumflag == 1))
     {

      /*  See if nodes are colored by material or flags.  */
      if (nodefieldactive == -1 || nodefieldactive >= numvars)
        {
         drawtype = 1;
         matsel = 0;  cflagsel = 0;  nflagsel = 0;
         if (nodefieldactive == -1)  /*  Materials.  */
           {
            matsel = 1;
           }
         else   /*  Flag.  */
           {
            nflagsel = 1;
            nflagno = nodefieldactive - numvars;
           }      
        }
     }

   /*  If the material color bar form is up, draw.  */
   if (matcolorbarform_isup) matcb_apply(NULL, NULL, NULL);
}
