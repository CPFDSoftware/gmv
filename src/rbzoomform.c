#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushBG.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static float mag_sav = 1., xtrans_sav = 0., ytrans_sav = 0., ztrans_sav = 0,
             xpan_sav = 0., ypan_sav = 0., zpan_sav = 0.;
static int saved_zoom = 0;



void rbzoomformpop()
{
   MyManageChild(rbzoomform);
}


void rzclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   rbzoomflag = 0;
   XUndefineCursor(dpy,windowid);
   XtUnmanageChild(rbzoomform);
}


void rzflags(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0)
     {
      rbzoomflag = 1;
      XDefineCursor(dpy,windowid,crosshair);
     }
   if (arg == 1)
     {
      rbzoomflag = 0;
      draw_expose_callback(NULL,NULL,NULL);      
     }
   if (arg == 2 && rbzoomflag == 2)
     {
      calcrbzoom();
      xpan = 0.;  ypan = 0.;  zpan = 0.;
     }
   if (arg == 3)
     {
      mag_sav = magnify; 
      xtrans_sav = xtrans;
      ytrans_sav = ytrans;
      ztrans_sav = ztrans;
      xpan_sav = xpan;
      ypan_sav = ypan;
      zpan_sav = zpan;
      saved_zoom = 1;
     }
   if (arg == 4 && saved_zoom == 1)
     {
      magnify = mag_sav; 
      xtrans = xtrans_sav;
      ytrans = ytrans_sav;
      ztrans = ztrans_sav;
      xpan = xpan_sav;
      ypan = ypan_sav;
      zpan = zpan_sav;
     }
   if (arg == 5)
     {
      magnify = 1.; 
      xtrans = -0.5 * (globxicmax + globxicmin);
      ytrans = -0.5 * (globyicmax + globyicmin);
      ztrans = -0.5 * (globzicmax + globzicmin);
      xpan = 0.;  ypan = 0.;  zpan = 0.;
     }

   if ((arg == 2 && rbzoomflag ==2) || (arg == 4 && saved_zoom == 1) ||
        arg == 5)
     {
      rbzoomflag = 0;
      setcenterwidgets();
      setmag();
      XFlush(dpy);
      draw_expose_callback(NULL,NULL,NULL);
      /* if (displistflag) drawnewlistflag = 1; */
      fastdraw = 0;
      drawit();
     }
}


void makerbzoomform(parent)
 Widget parent;
{
  Widget rbzclose, rbzdraw, rbzundo, rbzapply, rbzsave, rbzretsave,
         rbzretorig;
  int i;
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Zoom (Rubberband)",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   rbzoomform = XmCreateFormDialog(parent,"Rbzoomform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   rbzclose = XtVaCreateManagedWidget("Rbzclose",
              xmPushButtonGadgetClass,
              rbzoomform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rbzclose,XmNactivateCallback,rzclose,NULL);
 
   string = XmStringCreate("Draw Zoom Box",XmSTRING_DEFAULT_CHARSET);
   rbzdraw = XtVaCreateManagedWidget("Rbzdraw",
             xmPushButtonGadgetClass,
             rbzoomform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, rbzclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(rbzdraw,XmNactivateCallback,rzflags,(caddr_t) 0);
 
   string = XmStringCreate("Undo Zoom Box",XmSTRING_DEFAULT_CHARSET);
   rbzundo = XtVaCreateManagedWidget("Rbzundo",
             xmPushButtonGadgetClass,
             rbzoomform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, rbzdraw,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(rbzundo,XmNactivateCallback,rzflags,(caddr_t) 1);
 
   string = XmStringCreate("Apply Zoom",XmSTRING_DEFAULT_CHARSET);
   rbzapply = XtVaCreateManagedWidget("Rbzapply",
              xmPushButtonGadgetClass,
              rbzoomform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, rbzundo,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rbzapply,XmNactivateCallback,rzflags,(caddr_t) 2);
 
   string = XmStringCreate("Save Zoom",XmSTRING_DEFAULT_CHARSET);
   rbzsave = XtVaCreateManagedWidget("Rbzsave",
             xmPushButtonGadgetClass,
             rbzoomform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, rbzapply,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(rbzsave,XmNactivateCallback,rzflags,(caddr_t) 3);
 
   string = XmStringCreate("Return to Saved Zoom",XmSTRING_DEFAULT_CHARSET);
   rbzretsave = XtVaCreateManagedWidget("Rbztetsave",
                xmPushButtonGadgetClass,
                rbzoomform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rbzsave,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(rbzretsave,XmNactivateCallback,rzflags,(caddr_t) 4);
 
   string = XmStringCreate("Return to Original",XmSTRING_DEFAULT_CHARSET);
   rbzretorig = XtVaCreateManagedWidget("Rbztetorig",
                xmPushButtonGadgetClass,
                rbzoomform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rbzretsave,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(rbzretorig,XmNactivateCallback,rzflags,(caddr_t) 5);
}
