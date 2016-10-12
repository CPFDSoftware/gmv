#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

static Widget traceminlab, tracemaxlab, tracemintxt, tracemaxtxt,
              traceerror, trfldnice, trfldlog, trclist[100];
static int tracelimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void tracelimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = tracelimactive;

   sprintf(str,"%-20.11g",fieldtrmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",fieldtrmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tracemaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldtrmin[i];
   fmax = globfieldtrmax[i];
   if (trfieldlimauto[i])
     {
      fmin = fieldtrmin[i];
      fmax = fieldtrmax[i];
     }
   filltext20(tracemintxt,fmin);
   filltext20(tracemaxtxt,fmax);

   XmToggleButtonGadgetSetState(trfldnice,trfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(trfldlog,trfld_log[i],FALSE);

   MyManageChild(tracelimform);
}


void closetracelim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(tracelimform);
}


void tracelim_select_active_field(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = (long int)client_data;
   tracelimactive = i;

   sprintf(str,"%-20.11g",fieldtrmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",fieldtrmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tracemaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(tracemintxt,globfieldtrmin[i]);
   filltext20(tracemaxtxt,globfieldtrmax[i]);
}


void tracelimapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ierr;
  double trcmin, trcmax;
  char *tmpstr;
  static char str1[] = 
           "Error - maximum must be greater than minimum.";
  static char str2[] = 
           "Error - maximum is less than data minimum.";
  static char str3[] = 
           "Error - minimum is greater than data maximum.";
  static char strerror[55];
  XmString string;

   /*  Read and check the new min and max values.  */
   tmpstr = XmTextFieldGetString(tracemintxt);
   trcmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(tracemaxtxt);
   trcmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = tracelimactive;
   trfieldlimauto[i] = 0;
   if (trcmin >= trcmax)
     {
      if (trcmin == 0. && trcmax == 0.)
         trfieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (trcmax < fieldtrmin[i] && trfieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (trcmin > fieldtrmax[i] && trfieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (trfld_log[i] && (trcmin <= 0. || trcmax <= 0.))
     {
      ierr = 1;
      trfld_log[i] = 0;
      XmToggleButtonGadgetSetState(trfldlog,trfld_log[i],FALSE);
      if (trcmin <= 0. && trcmax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (trcmin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(traceerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldtrmin[i] = trcmin;
      globfieldtrmax[i] = trcmax;
      if (trfieldlimauto[i])
        {
         globfieldtrmin[i] = fieldtrmin[i];
         globfieldtrmax[i] = fieldtrmax[i];
        }
      trfld_nice_min[i] = globfieldtrmin[i];
      trfld_nice_max[i] = globfieldtrmax[i];
      if (!trfieldlimauto[i]) 
         fldrange(i, 3, trfld_nice[i], trfld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(trfld_nice_min[i], trfld_nice_max[i], 
                          trfld_nice[i], trfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void tracelimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = tracelimactive;

   trfieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldtrmin[i] = fieldtrmin[i];
   globfieldtrmax[i] = fieldtrmax[i];
   trfld_nice_min[i] = globfieldtrmin[i];
   trfld_nice_max[i] = globfieldtrmax[i];
   trfld_log[i] = 0;
   trfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(trfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trfldlog,FALSE,FALSE);

   filltext20(tracemintxt,globfieldtrmin[i]);
   filltext20(tracemaxtxt,globfieldtrmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void tracelimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < numvartr; i++)
     {
      trfieldlimauto[i] = 0;

      globfieldtrmin[i] = fieldtrmin[i];
      globfieldtrmax[i] = fieldtrmax[i];
      trfld_nice_min[i] = globfieldtrmin[i];
      trfld_nice_max[i] = globfieldtrmax[i];
      trfld_log[i] = 0;
      trfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(trfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trfldlog,FALSE,FALSE);

   i = tracelimactive;

   sprintf(str,"%-20.11g",globfieldtrmin[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldtrmax[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void tracelimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = tracelimactive;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   trfieldlimauto[i] = 1;

   globfieldtrmin[i] = fieldtrmin[i];
   globfieldtrmax[i] = fieldtrmax[i];
   trfld_nice_min[i] = globfieldtrmin[i];
   trfld_nice_max[i] = globfieldtrmax[i];
   trfld_log[i] = 0;
   trfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(trfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trfldlog,FALSE,FALSE);

   sprintf(str,"%-20.11g",globfieldtrmin[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldtrmax[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void tracelimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < numvartr; i++)
     {
      trfieldlimauto[i] = 1;

      globfieldtrmin[i] = fieldtrmin[i];
      globfieldtrmax[i] = fieldtrmax[i];
      trfld_nice_min[i] = globfieldtrmin[i];
      trfld_nice_max[i] = globfieldtrmax[i];
      trfld_log[i] = 0;
      trfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(trfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trfldlog,FALSE,FALSE);

   i = tracelimactive;

   sprintf(str,"%-20.11g",globfieldtrmin[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldtrmax[i]);
   XtVaSetValues(tracemaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void trfldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = tracelimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(traceerror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) trfld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      trfld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (trfld_log[i] && (globfieldtrmin[i] <= 0. || globfieldtrmax[i] <= 0.))
        {
         ierr = 1;
         trfld_log[i] = 0;
         XmToggleButtonGadgetSetState(trfldlog,trfld_log[i],FALSE);
         if (globfieldtrmin[i] <= 0. && globfieldtrmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data ",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldtrmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(traceerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   trfld_nice_min[i] = globfieldtrmin[i];
   trfld_nice_max[i] = globfieldtrmax[i];
   fldrange(i, 3, trfld_nice[i], trfld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(trfld_nice_min[i], trfld_nice_max[i], 
                          trfld_nice[i], trfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void maketracelimform(parent)
 Widget parent;
{
  Widget trcclose, trcminlab, trcmaxlab, trcstat1, trcstat2,
         trcblank, trcreset, trcapply, trcswin, trcbox,
         trcresetall, trcautoreset, trcautoresetall;
  int i;
  Arg wargs[20];
  char str[20];
  XmString string;

   string = XmStringCreate("Set Particle Limits",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   tracelimform = XmCreateFormDialog(parent,"Tracelimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trcclose = XtVaCreateManagedWidget("Trcclose",
              xmPushButtonGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trcclose,XmNactivateCallback,closetracelim,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trcclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   trcswin = XmCreateScrolledWindow(tracelimform,"Trcswin",wargs,i);
   XtManageChild(trcswin);

   i = 0;
   trcbox = XmCreateRadioBox(trcswin,"Trcbox",wargs,i);
   XtManageChild(trcbox);

   for (i = 0; i < numvartr; i++)
     {
      string = XmStringCreate(fieldtrname[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Trclist%d",i);
      trclist[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   trcbox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   NULL);
         XmStringFree(string);
         XtAddCallback(trclist[i],XmNvalueChangedCallback,
                       tracelim_select_active_field,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(trclist[i],TRUE,FALSE);
     }

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   traceerror = XtVaCreateManagedWidget("Traceerror",
                xmLabelGadgetClass,
                tracelimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trcswin,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   trcblank = XtVaCreateManagedWidget("Trcblank",
              xmLabelGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, traceerror,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   trcstat1 = XtVaCreateManagedWidget("Trcstat1",
              xmLabelGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trcblank,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   traceminlab = XtVaCreateManagedWidget("Traceminlab",
                 xmLabelGadgetClass,
                 tracelimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trcblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trcstat1,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   tracemaxlab = XtVaCreateManagedWidget("Tracemaxlab",
                 xmLabelGadgetClass,
                 tracelimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trcblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, traceminlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   trcstat2 = XtVaCreateManagedWidget("Trcstat2",
              xmLabelGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trcstat1,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   tracemintxt = XtVaCreateManagedWidget("Tracemintxt",
                 xmTextFieldWidgetClass,
                 tracelimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trcstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trcstat2,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 20,
                 XmNvalue, "",
                 NULL);

   tracemaxtxt = XtVaCreateManagedWidget("Tracemaxtxt",
                 xmTextFieldWidgetClass,
                 tracelimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trcstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, tracemintxt,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 20,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   trcminlab = XtVaCreateManagedWidget("Trcminlab",
               xmLabelGadgetClass,
               tracelimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, traceerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, traceminlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   trcmaxlab = XtVaCreateManagedWidget("Trcmaxlab",
               xmLabelGadgetClass,
               tracelimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, traceerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, tracemaxlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   trcapply = XtVaCreateManagedWidget("Trcapply",
              xmPushButtonGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trcstat2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trcapply,XmNactivateCallback,tracelimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   trfldnice = XtVaCreateManagedWidget("Trfldnice",
               xmToggleButtonGadgetClass,
               tracelimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trcapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(trfldnice,XmNvalueChangedCallback,
                    trfldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   trfldlog = XtVaCreateManagedWidget("Trfldlog",
             xmToggleButtonGadgetClass,
             tracelimform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, trfldnice,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(trfldlog,XmNvalueChangedCallback,
                    trfldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   trcreset = XtVaCreateManagedWidget("Trcreset",
              xmPushButtonGadgetClass,
              tracelimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trfldlog,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trcreset,XmNactivateCallback,tracelimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   trcresetall = XtVaCreateManagedWidget("Trcresetall",
                 xmPushButtonGadgetClass,
                 tracelimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trfldlog,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trcreset,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(trcresetall,XmNactivateCallback,tracelimresetall,
                    NULL);
   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   trcautoreset = XtVaCreateManagedWidget("Trcautoreset",
                  xmPushButtonGadgetClass,
                  tracelimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trcreset,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trcautoreset,XmNactivateCallback,tracelimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   trcautoresetall = XtVaCreateManagedWidget("Trcautoresetall",
                     xmPushButtonGadgetClass,
                     tracelimform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, trcreset,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, trcautoreset,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(trcautoresetall,XmNactivateCallback,tracelimautoresetall,
                    NULL);
}


void settrfldlimlab(int ifld)
{
   XmString string;

   string = XmStringCreate(fieldtrname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trclist[ifld],XmNlabelString,string,NULL);
   XmStringFree(string);
}


void trfieldlimpop(int ifld)
{
   tracelimactive = ifld;
   tracelimformpop();
}


