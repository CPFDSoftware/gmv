
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/Frame.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include <stdio.h>

#include "GLwMDrawA.h"
#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

static Widget redsl, greensl, bluesl, resultbg;
static int mr, mg, mb;
static Window bgwindowid = NULL;
static GLXContext bg_context = NULL;
extern GLboolean doubleBuffer;
extern GLXContext glx_glw_context;


void color_resultbg()
{
  float redcol, greencol, bluecol;

   redcol = (float)mr/255.0;
   greencol = (float)mg/255.0;
   bluecol = (float)mb/255.0;

   glXWaitX();
   glXMakeCurrent(dpy,bgwindowid,bg_context);
   glClearColor(redcol,greencol,bluecol,1.0);
   glClear(GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, bgwindowid);
   glClear(GL_COLOR_BUFFER_BIT);
   if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, bgwindowid);
   glXWaitGL();
}


void backgrnd_docolor(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XmScaleGetValue(redsl,&mr);
   XmScaleGetValue(greensl,&mg);
   XmScaleGetValue(bluesl,&mb);

   color_resultbg();
}


void backgrndsetsl()
{
   XtRemoveCallback(redsl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtRemoveCallback(greensl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtRemoveCallback(bluesl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XFlush(dpy);
   XmScaleSetValue(redsl,mr);
   XmScaleSetValue(greensl,mg);
   XmScaleSetValue(bluesl,mb);
}


void backgrndformpop()
{
   mr = (int) (backgroundcolor & 255);
   mg = (int) ((backgroundcolor >> 8) & 255);
   mb = (int) ((backgroundcolor >> 16) & 255);
   backgrndsetsl();

   MyManageChild(backgrndform);
   XFlush(dpy);

   XtAddCallback(redsl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtAddCallback(greensl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtAddCallback(bluesl,XmNvalueChangedCallback,backgrnd_docolor,NULL);

   color_resultbg();
}


void backgrndclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(backgrndform);
}


void backgrndapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   backgroundcolor = ((mb << 16) | (mg << 8) | mr);
   txtcolor = 0xffffffff;
   if (mg > 150 || (mr > 220 && mb > 220)) txtcolor = 0x000000ff;

   fastdraw = 0;
   if (drawunselcells || drawunselnodes)
      drawnew3dflag = 1;
   drawit();
}


void bgsetcolrbut(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0)
     {
      mr = 255;  mg = 255;  mb = 255;
     }
   if (arg == 1)
     {
      mr = 127;  mg = 127;  mb = 127;
     }
   if (arg == 2)
     {
      mr = 0;  mg = 0;  mb = 0;
     }

   backgrndsetsl();
   XFlush(dpy);

   XtAddCallback(redsl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtAddCallback(greensl,XmNvalueChangedCallback,backgrnd_docolor,NULL);
   XtAddCallback(bluesl,XmNvalueChangedCallback,backgrnd_docolor,NULL);

   color_resultbg();
}


void init_bgwindow(w,client_data,call_data)
 Widget w;
 caddr_t client_data;
 XtPointer call_data;
{
   bgwindowid  = XtWindow(w);
   bg_context = glXCreateContext(XtDisplay(w), vi, 
                                 glx_glw_context, GL_TRUE);
   glXMakeCurrent(dpy,bgwindowid ,bg_context);
}


void backgrnd_expose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   color_resultbg();
}


void makebackgrndform(parent)
 Widget parent;
{
  Widget bgclose, bgapply, bgframe, bgwhite, bggrey, bgblack;
  int i;
  Arg wargs[20];
  XmString string;
  void init_bgwindow();

   string = XmStringCreate("Background Color",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   backgrndform = XmCreateFormDialog(parent,"Backround",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   bgclose = XtVaCreateManagedWidget("Bgclose",
             xmPushButtonGadgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bgclose,XmNactivateCallback,
                    backgrndclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   bgapply = XtVaCreateManagedWidget("Bgapply",
             xmPushButtonGadgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, bgclose,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bgapply,XmNactivateCallback,
                    backgrndapply,NULL);
 
   bgframe = XtVaCreateManagedWidget("Bgframe",
             xmFrameWidgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, bgapply,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNheight, 130,
             XmNwidth, 130,
             NULL);
      XtManageChild(bgframe);

   i = 0;
   XtSetArg(wargs[i], XmNheight,windowheight);i++;
   XtSetArg(wargs[i], XmNwidth,windowwidth);i++; 
   XtSetArg(wargs[i], GLwNvisualInfo, vi);i++;
   if (mesacmap)
     { XtSetArg(wargs[i], XmNcolormap, my_cmap);i++; }
   resultbg = GLwCreateMDrawingArea(bgframe,"Resultbg",wargs,i);
      XtManageChild (resultbg); 
      XtAddCallback(resultbg,GLwNexposeCallback,backgrnd_expose,
                    (caddr_t) 0);
      XtAddCallback(resultbg,GLwNginitCallback,init_bgwindow,
                    (caddr_t) 0);


   string = XmStringCreate("Red",XmSTRING_DEFAULT_CHARSET);
   redsl = XtVaCreateManagedWidget("Redsl",
           xmScaleWidgetClass,
           backgrndform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, bgframe,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNminimum, 0,
           XmNmaximum, 255,
           XmNorientation, XmHORIZONTAL,
           XmNprocessingDirection, XmMAX_ON_RIGHT,
           XmNvalue, 0,
           XmNscaleWidth, 130,
           XmNscaleMultiple, 1,
           XmNshowValue, TRUE,
           XmNbackground, redpix,
           XmNtitleString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(redsl,XmNvalueChangedCallback,
                    backgrnd_docolor,NULL);
      XtAddCallback(redsl,XmNdragCallback,backgrnd_docolor,NULL);

   string = XmStringCreate("Green",XmSTRING_DEFAULT_CHARSET);
   greensl = XtVaCreateManagedWidget("Greensl",
             xmScaleWidgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, redsl,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, 0,
             XmNmaximum, 255,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, 0,
             XmNscaleWidth, 130,
             XmNscaleMultiple, 1,
             XmNshowValue, TRUE,
             XmNbackground, greenpix,
             XmNtitleString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(greensl,XmNvalueChangedCallback,
                    backgrnd_docolor,NULL);
      XtAddCallback(greensl,XmNdragCallback,backgrnd_docolor,NULL);

   string = XmStringCreate("Blue",XmSTRING_DEFAULT_CHARSET);
   bluesl = XtVaCreateManagedWidget("Bluesl",
            xmScaleWidgetClass,
            backgrndform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, greensl,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNminimum, 0,
            XmNmaximum, 255,
            XmNorientation, XmHORIZONTAL,
            XmNprocessingDirection, XmMAX_ON_RIGHT,
            XmNvalue, 0,
            XmNscaleWidth, 130,
            XmNscaleMultiple, 1,
            XmNshowValue, TRUE,
            XmNbackground, bluepix,
            XmNtitleString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(bluesl,XmNvalueChangedCallback,
                    backgrnd_docolor,NULL);
      XtAddCallback(bluesl,XmNdragCallback,backgrnd_docolor,NULL);

   string = XmStringCreate("White",XmSTRING_DEFAULT_CHARSET);
   bgwhite = XtVaCreateManagedWidget("Bgwhite",
             xmPushButtonGadgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, bluesl,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bgwhite,XmNactivateCallback,
                    bgsetcolrbut,(caddr_t) 0);

   string = XmStringCreate("Grey",XmSTRING_DEFAULT_CHARSET);
   bggrey = XtVaCreateManagedWidget("Bggrey",
            xmPushButtonGadgetClass,
            backgrndform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, bgwhite,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(bggrey,XmNactivateCallback,
                    bgsetcolrbut,(caddr_t) 1);

   string = XmStringCreate("Black",XmSTRING_DEFAULT_CHARSET);
   bgblack = XtVaCreateManagedWidget("Bgblack",
             xmPushButtonGadgetClass,
             backgrndform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, bggrey,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bgblack,XmNactivateCallback,
                    bgsetcolrbut,(caddr_t) 2);

   mr = 0;  mg = 0;  mb = 0;
}
