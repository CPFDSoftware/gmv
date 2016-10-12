#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#include <GL/gl.h>

static Widget lnsize1, lnsize2, lnsize3,
              lnnorm, lnsmooth;


void linesizeformpop()
{
   XmToggleButtonGadgetSetState(lnsize1,FALSE,FALSE);
   XmToggleButtonGadgetSetState(lnsize2,FALSE,FALSE);
   XmToggleButtonGadgetSetState(lnsize3,FALSE,FALSE);
   switch(line_size)
     {
      case(1):
         XmToggleButtonGadgetSetState(lnsize1,TRUE,FALSE);
         break;
      case(2):
         XmToggleButtonGadgetSetState(lnsize2,TRUE,FALSE);
         break;
      case(3):
         XmToggleButtonGadgetSetState(lnsize3,TRUE,FALSE);
         break;
     }

   XmToggleButtonGadgetSetState(lnnorm,FALSE,FALSE);
   if (line_type == 0)
      XmToggleButtonGadgetSetState(lnnorm,TRUE,FALSE);
   XmToggleButtonGadgetSetState(lnsmooth,FALSE,FALSE);
   if (line_type == 1)
      XmToggleButtonGadgetSetState(lnsmooth,TRUE,FALSE);

   MyManageChild(linesizeform);
}

void lnsizeclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(linesizeform);
}


void lnsize_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  float lnsize;

   lnsize = (float)line_size;
   glLineWidth(lnsize);

   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void lnsizeflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, szflag;

   arg = (long int)client_data;
   szflag = (int)XmToggleButtonGadgetGetState(w);
   
   if (arg == 0 && szflag == 1)
     line_size = 1;
   
   if (arg == 1 && szflag == 1)
     line_size = 2;
   
   if (arg == 2 && szflag == 1)
     line_size = 3;
}


void typeflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, typeflag;

   arg = (long int)client_data;
   typeflag = (int)XmToggleButtonGadgetGetState(w);
   
   if (arg == 0 && typeflag == 1)
     line_type = 0;
   
   if (arg == 1 && typeflag == 1)
     line_type = 1;
}


void makelinesizeform(parent)
 Widget parent;
{
  Widget lnsclose, lnsapply, lnsizebox, linetypebox, 
         lnsizetxt, lntypetxt;
  int i;
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Line Width",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   linesizeform = XmCreateFormDialog(parent,"Linesizeform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   lnsclose = XtVaCreateManagedWidget("Lnsclose",
              xmPushButtonGadgetClass,
              linesizeform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(lnsclose,XmNactivateCallback,lnsizeclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   lnsapply = XtVaCreateManagedWidget("Lnsapply",
              xmPushButtonGadgetClass,
              linesizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, lnsclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(lnsapply,XmNactivateCallback,lnsize_apply,
                    NULL);
 
   string = XmStringCreate("Pixels:",XmSTRING_DEFAULT_CHARSET);
   lnsizetxt = XtVaCreateManagedWidget("Lnsizetxt",
               xmLabelWidgetClass,
               linesizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, lnsapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, lnsizetxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   lnsizebox = XmCreateRadioBox(linesizeform,"Lnsizebox",wargs,i);
   XtManageChild(lnsizebox);
 
   string = XmStringCreate("One",
            XmSTRING_DEFAULT_CHARSET);
   lnsize1 = XtVaCreateManagedWidget("Lnsize1",
             xmToggleButtonGadgetClass,
             lnsizebox,
             XmNlabelString, string,
             XmNset, TRUE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(lnsize1,XmNvalueChangedCallback,
                    lnsizeflags,(caddr_t) 0);
 
   string = XmStringCreate("Two",
            XmSTRING_DEFAULT_CHARSET);
   lnsize2 = XtVaCreateManagedWidget("Lnsize2",
             xmToggleButtonGadgetClass,
             lnsizebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(lnsize2,XmNvalueChangedCallback,
                    lnsizeflags,(caddr_t) 1);
 
   string = XmStringCreate("Three",
            XmSTRING_DEFAULT_CHARSET);
   lnsize3 = XtVaCreateManagedWidget("Lnsize3",
             xmToggleButtonGadgetClass,
             lnsizebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(lnsize3,XmNvalueChangedCallback,
                    lnsizeflags,(caddr_t) 2);

   /*  Create Line Type buttons.  */
 
   string = XmStringCreate("Type:",XmSTRING_DEFAULT_CHARSET);
   lntypetxt = XtVaCreateManagedWidget("Lntypetxt",
               xmLabelWidgetClass,
               linesizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, lnsizebox,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, lntypetxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   linetypebox = XmCreateRadioBox(linesizeform,"Linetypebox",wargs,i);
   XtManageChild(linetypebox);
  
   string = XmStringCreate("Regular (fast)",
            XmSTRING_DEFAULT_CHARSET);
   lnnorm = XtVaCreateManagedWidget("Lnnorm",
            xmToggleButtonGadgetClass,
            linetypebox,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(lnnorm,XmNvalueChangedCallback,
                    typeflags,(caddr_t) 0);
  
   string = XmStringCreate("Smooth (slow)",
            XmSTRING_DEFAULT_CHARSET);
   lnsmooth = XtVaCreateManagedWidget("Lnsmooth",
              xmToggleButtonGadgetClass,
              linetypebox,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(lnsmooth,XmNvalueChangedCallback,
                    typeflags,(caddr_t) 1);
}
