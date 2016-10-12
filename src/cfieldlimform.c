#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/TextF.h>
#include <Xm/ToggleBG.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

static Widget cfieldlab, cfieldminlab, cfieldmaxlab, cfieldmintxt,
              cfieldmaxtxt, cfielderror, cfldnice, cfldlog;
static int cfieldlimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void cfieldlimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = cfieldlimactive;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",cfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",cfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldcmin[i];
   fmax = globfieldcmax[i];
   if (cfieldlimauto[i])
     {
      fmin = cfieldmin[i];
      fmax = cfieldmax[i];
     }
   filltext20(cfieldmintxt,fmin);
   filltext20(cfieldmaxtxt,fmax);

   XmToggleButtonGadgetSetState(cfldnice,cfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(cfldlog,cfld_log[i],FALSE);

   MyManageChild(cfieldlimform);
   XtUnmanageChild(cfieldform);
}


void closecfieldlim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfieldlimform);
}


void cfieldlimfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = cfieldlimactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDFLDLIM, ifld);
}


void cfieldlimapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ierr;
  double fldmin, fldmax;
  char *tmpstr;
  static char str1[] = 
           "Error - maximum must be greater than minimum.";
  static char str2[] = 
           "Warning - maximum is less than data minimum.";
  static char str3[] = 
           "Warning - minimum is greater than data maximum.";
  static char strerror[55];
  XmString string;
  void getscale(double llimit, double ulimit, int maxcont, char lineflag,
                char logflag, double *intervals, int *nintervals);

   /*  Read and check the new min and max values.  */
   tmpstr = XmTextFieldGetString(cfieldmintxt);
   fldmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(cfieldmaxtxt);
   fldmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = cfieldlimactive;
   if (fldmin != cfieldmin[i] || fldmax != cfieldmax[i])
      cfieldlimauto[i] = 0;
   if (fldmin >= fldmax)
     {
      if (fldmin == 0. && fldmax == 0.)
         cfieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (fldmax < cfieldmin[i] && cfieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (fldmin > cfieldmax[i] && cfieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (cfld_log[i] && (fldmin <= 0. || fldmax <= 0.))
     {
      ierr = 1;
      cfld_log[i] = 0;
      XmToggleButtonGadgetSetState(cfldlog,cfld_log[i],FALSE);
      if (fldmin <= 0. && fldmax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (fldmin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldcmin[i] = fldmin;
      globfieldcmax[i] = fldmax;
      if (cfieldlimauto[i])
        {
         globfieldcmin[i] = cfieldmin[i];
         globfieldcmax[i] = cfieldmax[i];
        }
      cfld_nice_min[i] = globfieldcmin[i];
      cfld_nice_max[i] = globfieldcmax[i];
      if (!cfieldlimauto[i]) 
         fldrange(i, 1, cfld_nice[i], cfld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(cfld_nice_min[i], cfld_nice_max[i], 
                          cfld_nice[i], cfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void cfieldlimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = cfieldlimactive;

   cfieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldcmin[i] = cfieldmin[i];
   globfieldcmax[i] = cfieldmax[i];
   cfld_nice_min[i] = globfieldcmin[i];
   cfld_nice_max[i] = globfieldcmax[i];
   cfld_log[i] = 0;
   cfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(cfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfldlog,FALSE,FALSE);

   filltext20(cfieldmintxt,globfieldcmin[i]);
   filltext20(cfieldmaxtxt,globfieldcmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}

void cfieldlimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < cnumvars; i++)
     {
      cfieldlimauto[i] = 0;

      globfieldcmin[i] = cfieldmin[i];
      globfieldcmax[i] = cfieldmax[i];
      cfld_nice_min[i] = globfieldcmin[i];
      cfld_nice_max[i] = globfieldcmax[i];
      cfld_log[i] = 0;
      cfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(cfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfldlog,FALSE,FALSE);

   i = cfieldlimactive;

   filltext20(cfieldmintxt,globfieldcmin[i]);
   filltext20(cfieldmaxtxt,globfieldcmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cfieldlimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  XmString string;

   i = cfieldlimactive;

   cfieldlimauto[i] = 1;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldcmin[i] = cfieldmin[i];
   globfieldcmax[i] = cfieldmax[i];
   cfld_nice_min[i] = globfieldcmin[i];
   cfld_nice_max[i] = globfieldcmax[i];
   cfld_log[i] = 0;
   cfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(cfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfldlog,FALSE,FALSE);

   filltext20(cfieldmintxt,globfieldcmin[i]);
   filltext20(cfieldmaxtxt,globfieldcmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cfieldlimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < cnumvars; i++)
     {
      cfieldlimauto[i] = 1;

      globfieldcmin[i] = cfieldmin[i];
      globfieldcmax[i] = cfieldmax[i];
      cfld_nice_min[i] = globfieldcmin[i];
      cfld_nice_max[i] = globfieldcmax[i];
      cfld_log[i] = 0;
      cfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(cfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfldlog,FALSE,FALSE);

   i = cfieldlimactive;

   filltext20(cfieldmintxt,globfieldcmin[i]);
   filltext20(cfieldmaxtxt,globfieldcmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cfldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = cfieldlimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) cfld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      cfld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (cfld_log[i] && (globfieldcmin[i] <= 0. || globfieldcmax[i] <= 0.))
        {
         ierr = 1;
         cfld_log[i] = 0;
         XmToggleButtonGadgetSetState(cfldlog,cfld_log[i],FALSE);
         if (globfieldcmin[i] <= 0. && globfieldcmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data ",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldcmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(cfielderror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   cfld_nice_min[i] = globfieldcmin[i];
   cfld_nice_max[i] = globfieldcmax[i];
   fldrange(i, 1, cfld_nice[i], cfld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);


      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(cfld_nice_min[i], cfld_nice_max[i], 
                          cfld_nice[i], cfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void makecfieldlimform(parent)
 Widget parent;
{
  Widget cfldclose, cfldminlab, cfldmaxlab, cfldlab1, cfldnewfld,
         cfldstat1, cfldstat2, cfldblank, cfldreset, cfldresetall,
         cfldautoreset, cfldautoresetall, cfldapply; 
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Set Cell Field Limits",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cfieldlimform = XmCreateFormDialog(parent,"Cfieldlimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cfldclose = XtVaCreateManagedWidget("Cfldclose",
               xmPushButtonGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfldclose,XmNactivateCallback,closecfieldlim,
                    NULL);

   string = XmStringCreate("Field: ",XmSTRING_DEFAULT_CHARSET);
   cfldlab1 = XtVaCreateManagedWidget("Cfldlab1",
              xmLabelGadgetClass,
              cfieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfldclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   cfieldlab = XtVaCreateManagedWidget("Cfieldlab",
               xmLabelGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfldclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cfldlab1,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cfldnewfld = XtVaCreateManagedWidget("Cfldnewfld",
                xmPushButtonGadgetClass,
                cfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfldlab1,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cfldnewfld,XmNactivateCallback,cfieldlimfld,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   cfielderror = XtVaCreateManagedWidget("Cfielderror",
                 xmLabelGadgetClass,
                 cfieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cfldnewfld,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   cfldblank = XtVaCreateManagedWidget("Cfldblank",
               xmLabelGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfielderror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   cfldstat1 = XtVaCreateManagedWidget("Cfldstat1",
               xmLabelGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfldblank,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   cfieldminlab = XtVaCreateManagedWidget("Cfieldminlab",
                  xmLabelGadgetClass,
                  cfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfldstat1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   cfieldmaxlab = XtVaCreateManagedWidget("Cfieldmaxlab",
                  xmLabelGadgetClass,
                  cfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfieldminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   cfldstat2 = XtVaCreateManagedWidget("Cfldstat2",
               xmLabelGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfldstat1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   cfieldmintxt = XtVaCreateManagedWidget("Cfieldmintxt",
                  xmTextFieldWidgetClass,
                  cfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfldstat2,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   cfieldmaxtxt = XtVaCreateManagedWidget("Cfieldmaxtxt",
                  xmTextFieldWidgetClass,
                  cfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfieldmintxt,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   cfldminlab = XtVaCreateManagedWidget("Cfldminlab",
                xmLabelGadgetClass,
                cfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, cfieldminlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   cfldmaxlab = XtVaCreateManagedWidget("Cfldmaxlab",
                xmLabelGadgetClass,
                cfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, cfieldmaxlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   cfldapply = XtVaCreateManagedWidget("Cfldapply",
               xmPushButtonGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfldstat2,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfldapply,XmNactivateCallback,cfieldlimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   cfldnice = XtVaCreateManagedWidget("Cfldnice",
              xmToggleButtonGadgetClass,
              cfieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfldapply,
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
      XtAddCallback(cfldnice,XmNvalueChangedCallback,
                    cfldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   cfldlog = XtVaCreateManagedWidget("Cfldlog",
            xmToggleButtonGadgetClass,
            cfieldlimform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cfldnice,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(cfldlog,XmNvalueChangedCallback,
                    cfldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   cfldreset = XtVaCreateManagedWidget("Cfldreset",
               xmPushButtonGadgetClass,
               cfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfldlog,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfldreset,XmNactivateCallback,cfieldlimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   cfldresetall = XtVaCreateManagedWidget("Cfldresetall",
                  xmPushButtonGadgetClass,
                  cfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfldlog,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfldreset,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cfldresetall,XmNactivateCallback,cfieldlimresetall,
                    NULL);

   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   cfldautoreset = XtVaCreateManagedWidget("Cfldautoreset",
                   xmPushButtonGadgetClass,
                   cfieldlimform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cfldreset,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cfldautoreset,XmNactivateCallback,cfieldlimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   cfldautoresetall = XtVaCreateManagedWidget("Fcldautoresetall",
                      xmPushButtonGadgetClass,
                      cfieldlimform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cfldreset,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, cfldautoreset,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
      XmStringFree(string);
      XtAddCallback(cfldautoresetall,XmNactivateCallback,cfieldlimautoresetall,
                    NULL);
}


void cfieldlimgetcfield(int i)
{
  char str[20];
  XmString string;

   cfieldlimactive = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",cfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",cfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(cfieldmintxt,globfieldcmin[i]);
   filltext20(cfieldmaxtxt,globfieldcmax[i]);

   XmToggleButtonGadgetSetState(cfldnice,cfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(cfldlog,cfld_log[i],FALSE);
}


void cfieldlimpop(int ifld)
{
   cfieldlimactive = ifld;
   cfieldlimformpop();
}
