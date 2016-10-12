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

static Widget velsl, vectarrow, arrowsize, vectlog;



void vectorctlformpop()
{
  int i, ival;

   if (velscale < 1)
     {
      i = (int) (velscale * 1000);
      ival = i - 1000;
     }
   if (velscale == 1) ival = 0;
   if (velscale > 1)
     {
      ival = (int) ((velscale - 1.) * 10.);
     }
   XmScaleSetValue(velsl,ival);
   ival = (int) (velarrowsize * 100);
   XmScaleSetValue(arrowsize,ival);
   XmToggleButtonGadgetSetState(vectarrow,vectorarrow,FALSE);
   XmToggleButtonGadgetSetState(vectlog,vectorlog,FALSE);

   MyManageChild(vectctlform);
}

void vcclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(vectctlform);
}


void setvelscale(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval, i;

   XmScaleGetValue(w,&inval);
   if (inval < 0)
     {
      i = 1000 + inval;
      if (i <= 0) i = 1;
      velscale = (float)i / 1000.;
     }
   if (inval == 0) velscale = 1;
   if (inval > 0)
     {
      velscale = 1. + (float)inval / 10.;
     }
   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void setvectarrow(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   vectorarrow = (int)XmToggleButtonGadgetGetState(w);

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void setvectlog(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   vectorlog = (int)XmToggleButtonGadgetGetState(w);

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void setarrowsize(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval;

   XmScaleGetValue(w,&inval);
   velarrowsize = inval / 100.;
   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void makevectctlform(parent)
 Widget parent;
{
  Widget vctlclose, velstxt, arrowtxt;
  int i;
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Vector Control",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   vectctlform = XmCreateFormDialog(parent,"VectorControl",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   vctlclose = XtVaCreateManagedWidget("Vctlclose",
               xmPushButtonGadgetClass,
               vectctlform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(vctlclose,XmNactivateCallback,vcclose,NULL);
 
   string = XmStringCreate("Vector Scale",XmSTRING_DEFAULT_CHARSET);
   velstxt = XtVaCreateManagedWidget("Velstxt",
             xmLabelWidgetClass,
             vectctlform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, vctlclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   velsl = XtVaCreateManagedWidget("Velsl",
           xmScaleWidgetClass,
           vectctlform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, vctlclose,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, velstxt,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNminimum, -1000,
           XmNmaximum, 1000,
           XmNorientation, XmHORIZONTAL,
           XmNprocessingDirection, XmMAX_ON_RIGHT,
           XmNscaleWidth, 200,
           XmNvalue, 0,
           XmNscaleMultiple, 1,
           NULL);
      XtAddCallback(velsl,XmNvalueChangedCallback,setvelscale,NULL);
 
   string = XmStringCreate("Arrow Heads On",XmSTRING_DEFAULT_CHARSET);
   vectarrow = XtVaCreateManagedWidget("Vectarrow",
               xmToggleButtonGadgetClass,
               vectctlform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, velsl,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(vectarrow,XmNvalueChangedCallback,setvectarrow,
                    NULL);
 
   string = XmStringCreate("Use Log",XmSTRING_DEFAULT_CHARSET);
   vectlog = XtVaCreateManagedWidget("Vectlog",
             xmToggleButtonGadgetClass,
             vectctlform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, velsl,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, vectarrow,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(vectlog,XmNvalueChangedCallback,setvectlog,
                    NULL);
 
   string = XmStringCreate("Arrow Head Size",XmSTRING_DEFAULT_CHARSET);
   arrowtxt = XtVaCreateManagedWidget("Arrowtxt",
              xmLabelWidgetClass,
              vectctlform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, vectarrow,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   arrowsize = XtVaCreateManagedWidget("Arrowsize",
               xmScaleWidgetClass,
               vectctlform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, vectarrow,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, arrowtxt,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNminimum, 50,
               XmNmaximum, 150,
               XmNorientation, XmHORIZONTAL,
               XmNprocessingDirection, XmMAX_ON_RIGHT,
               XmNscaleWidth, 100,
               XmNvalue, 100,
               XmNscaleMultiple, 1,
               NULL);
      XtAddCallback(arrowsize,XmNvalueChangedCallback,setarrowsize,NULL);
}
