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

static Widget ffieldlab, ffieldminlab, ffieldmaxlab, ffieldmintxt,
              ffieldmaxtxt, ffielderror, ffldnice, ffldlog;
static int ffieldlimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void ffieldlimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = ffieldlimactive;

   string = XmStringCreate(ffieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",ffieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",ffieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldfmin[i];
   fmax = globfieldfmax[i];
   if (ffieldlimauto[i])
     {
      fmin = ffieldmin[i];
      fmax = ffieldmax[i];
     }
   filltext20(ffieldmintxt,fmin);
   filltext20(ffieldmaxtxt,fmax);

   XmToggleButtonGadgetSetState(ffldnice,ffld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(ffldlog,ffld_log[i],FALSE);

   MyManageChild(ffieldlimform);
   XtUnmanageChild(ffieldform);
}


void closeffieldlim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(ffieldlimform);
}


void ffieldlimfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = ffieldlimactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(ffieldform, XmNx,fx, XmNy,fy, NULL);
 
   ffieldformpop(FFLDFLDLIM, ifld);
}


void ffieldlimapply(w,client_data,call_data)
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
   tmpstr = XmTextFieldGetString(ffieldmintxt);
   fldmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(ffieldmaxtxt);
   fldmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = ffieldlimactive;
   if (fldmin != ffieldmin[i] || fldmax != ffieldmax[i])
      ffieldlimauto[i] = 0;
   if (fldmin >= fldmax)
     {
      if (fldmin == 0. && fldmax == 0.)
         ffieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (fldmax < ffieldmin[i] && ffieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (fldmin > ffieldmax[i] && ffieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (ffld_log[i] && (fldmin <= 0. || fldmax <= 0.))
     {
      ierr = 1;
      ffld_log[i] = 0;
      XmToggleButtonGadgetSetState(ffldlog,ffld_log[i],FALSE);
      if (fldmin <= 0. && fldmax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (fldmin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldfmin[i] = fldmin;
      globfieldfmax[i] = fldmax;
      if (ffieldlimauto[i])
        {
         globfieldfmin[i] = ffieldmin[i];
         globfieldfmax[i] = ffieldmax[i];
        }
      ffld_nice_min[i] = globfieldfmin[i];
      ffld_nice_max[i] = globfieldfmax[i];
      if (!ffieldlimauto[i]) 
         fldrange(i, 1, ffld_nice[i], ffld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(ffld_nice_min[i], ffld_nice_max[i], 
                          ffld_nice[i], ffld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void ffieldlimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = ffieldlimactive;

   ffieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldfmin[i] = ffieldmin[i];
   globfieldfmax[i] = ffieldmax[i];
   ffld_nice_min[i] = globfieldfmin[i];
   ffld_nice_max[i] = globfieldfmax[i];
   ffld_log[i] = 0;
   ffld_nice[i] = 0;
   XmToggleButtonGadgetSetState(ffldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ffldlog,FALSE,FALSE);

   filltext20(ffieldmintxt,globfieldfmin[i]);
   filltext20(ffieldmaxtxt,globfieldfmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}

void ffieldlimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < fnumvars; i++)
     {
      ffieldlimauto[i] = 0;

      globfieldfmin[i] = ffieldmin[i];
      globfieldfmax[i] = ffieldmax[i];
      ffld_nice_min[i] = globfieldfmin[i];
      ffld_nice_max[i] = globfieldfmax[i];
      ffld_log[i] = 0;
      ffld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(ffldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ffldlog,FALSE,FALSE);

   i = ffieldlimactive;

   filltext20(ffieldmintxt,globfieldfmin[i]);
   filltext20(ffieldmaxtxt,globfieldfmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void ffieldlimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = ffieldlimactive;

   ffieldlimauto[i] = 1;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldfmin[i] = ffieldmin[i];
   globfieldfmax[i] = ffieldmax[i];
   ffld_nice_min[i] = globfieldfmin[i];
   ffld_nice_max[i] = globfieldfmax[i];
   ffld_log[i] = 0;
   ffld_nice[i] = 0;
   XmToggleButtonGadgetSetState(ffldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ffldlog,FALSE,FALSE);

   filltext20(ffieldmintxt,globfieldfmin[i]);
   filltext20(ffieldmaxtxt,globfieldfmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void ffieldlimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < fnumvars; i++)
     {
      ffieldlimauto[i] = 1;

      globfieldfmin[i] = ffieldmin[i];
      globfieldfmax[i] = ffieldmax[i];
      ffld_nice_min[i] = globfieldfmin[i];
      ffld_nice_max[i] = globfieldfmax[i];
      ffld_log[i] = 0;
      ffld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(ffldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ffldlog,FALSE,FALSE);

   i = ffieldlimactive;

   filltext20(ffieldmintxt,globfieldfmin[i]);
   filltext20(ffieldmaxtxt,globfieldfmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void ffldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = ffieldlimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) ffld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      ffld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (ffld_log[i] && (globfieldfmin[i] <= 0. || globfieldfmax[i] <= 0.))
        {
         ierr = 1;
         ffld_log[i] = 0;
         XmToggleButtonGadgetSetState(ffldlog,ffld_log[i],FALSE);
         if (globfieldfmin[i] <= 0. && globfieldfmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data ",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldfmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(ffielderror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   ffld_nice_min[i] = globfieldfmin[i];
   ffld_nice_max[i] = globfieldfmax[i];
   fldrange(i, 1, ffld_nice[i], ffld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(ffld_nice_min[i], ffld_nice_max[i], 
                          ffld_nice[i], ffld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void makeffieldlimform(parent)
 Widget parent;
{
  Widget ffldclose, ffldminlab, ffldmaxlab, ffldlab1, ffldnewfld,
         ffldstat1, ffldstat2, ffldblank, ffldreset, ffldresetall,
         ffldautoreset, ffldautoresetall, ffldapply; 
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
   ffieldlimform = XmCreateFormDialog(parent,"Ffieldlimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ffldclose = XtVaCreateManagedWidget("Ffldclose",
               xmPushButtonGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ffldclose,XmNactivateCallback,closeffieldlim,
                    NULL);

   string = XmStringCreate("Field: ",XmSTRING_DEFAULT_CHARSET);
   ffldlab1 = XtVaCreateManagedWidget("Ffldlab1",
              xmLabelGadgetClass,
              ffieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ffldclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   ffieldlab = XtVaCreateManagedWidget("Ffieldlab",
               xmLabelGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffldclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, ffldlab1,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ffldnewfld = XtVaCreateManagedWidget("Ffldnewfld",
                xmPushButtonGadgetClass,
                ffieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ffldlab1,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ffldnewfld,XmNactivateCallback,ffieldlimfld,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   ffielderror = XtVaCreateManagedWidget("Ffielderror",
                 xmLabelGadgetClass,
                 ffieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ffldnewfld,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   ffldblank = XtVaCreateManagedWidget("Ffldblank",
               xmLabelGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffielderror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   ffldstat1 = XtVaCreateManagedWidget("Ffldstat1",
               xmLabelGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffldblank,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   ffieldminlab = XtVaCreateManagedWidget("Ffieldminlab",
                  xmLabelGadgetClass,
                  ffieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ffldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, ffldstat1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                     ",XmSTRING_DEFAULT_CHARSET);
   ffieldmaxlab = XtVaCreateManagedWidget("Ffieldmaxlab",
                  xmLabelGadgetClass,
                  ffieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ffldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, ffieldminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   ffldstat2 = XtVaCreateManagedWidget("Ffldstat2",
               xmLabelGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffldstat1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   ffieldmintxt = XtVaCreateManagedWidget("Ffieldmintxt",
                  xmTextFieldWidgetClass,
                  ffieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ffldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, ffldstat2,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   ffieldmaxtxt = XtVaCreateManagedWidget("Ffieldmaxtxt",
                  xmTextFieldWidgetClass,
                  ffieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ffldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, ffieldmintxt,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   ffldminlab = XtVaCreateManagedWidget("Ffldminlab",
                xmLabelGadgetClass,
                ffieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ffielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, ffieldminlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   ffldmaxlab = XtVaCreateManagedWidget("Ffldmaxlab",
                xmLabelGadgetClass,
                ffieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ffielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, ffieldmaxlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   ffldapply = XtVaCreateManagedWidget("Ffldapply",
               xmPushButtonGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffldstat2,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ffldapply,XmNactivateCallback,ffieldlimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   ffldnice = XtVaCreateManagedWidget("Ffldnice",
              xmToggleButtonGadgetClass,
              ffieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ffldapply,
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
      XtAddCallback(ffldnice,XmNvalueChangedCallback,
                    ffldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   ffldlog = XtVaCreateManagedWidget("Ffldlog",
            xmToggleButtonGadgetClass,
            ffieldlimform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, ffldnice,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(ffldlog,XmNvalueChangedCallback,
                    ffldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   ffldreset = XtVaCreateManagedWidget("Ffldreset",
               xmPushButtonGadgetClass,
               ffieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ffldlog,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ffldreset,XmNactivateCallback,ffieldlimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   ffldresetall = XtVaCreateManagedWidget("Ffldresetall",
                  xmPushButtonGadgetClass,
                  ffieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ffldlog,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, ffldreset,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(ffldresetall,XmNactivateCallback,ffieldlimresetall,
                    NULL);

   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   ffldautoreset = XtVaCreateManagedWidget("Ffldautoreset",
                   xmPushButtonGadgetClass,
                   ffieldlimform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, ffldreset,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(ffldautoreset,XmNactivateCallback,ffieldlimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   ffldautoresetall = XtVaCreateManagedWidget("Fcldautoresetall",
                      xmPushButtonGadgetClass,
                      ffieldlimform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, ffldreset,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, ffldautoreset,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
      XmStringFree(string);
      XtAddCallback(ffldautoresetall,XmNactivateCallback,ffieldlimautoresetall,
                    NULL);
}


void ffieldlimgetffield(int i)
{
  char str[20];
  XmString string;

   ffieldlimactive = i;

   string = XmStringCreate(ffieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",ffieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",ffieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ffieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(ffieldmintxt,globfieldfmin[i]);
   filltext20(ffieldmaxtxt,globfieldfmax[i]);

   XmToggleButtonGadgetSetState(ffldnice,ffld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(ffldlog,ffld_log[i],FALSE);
}


void ffieldlimpop(int ifld)
{
   ffieldlimactive = ifld;
   ffieldlimformpop();
}
