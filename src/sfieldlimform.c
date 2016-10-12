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

static Widget sfieldlab, sfieldminlab, sfieldmaxlab, sfieldmintxt,
              sfieldmaxtxt, sfielderror, sfldnice, sfldlog;
static int sfieldlimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void sfieldlimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = sfieldlimactive;

   string = XmStringCreate(sfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",sfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",sfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldsmin[i];
   fmax = globfieldsmax[i];
   if (sfieldlimauto[i])
     {
      fmin = sfieldmin[i];
      fmax = sfieldmax[i];
     }
   filltext20(sfieldmintxt,fmin);
   filltext20(sfieldmaxtxt,fmax);

   XmToggleButtonGadgetSetState(sfldnice,sfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(sfldlog,sfld_log[i],FALSE);

   MyManageChild(sfieldlimform);
   XtUnmanageChild(sfieldform);
}


void closesfieldlim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(sfieldlimform);
}


void sfieldlimfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = sfieldlimactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   sfieldformpop(SFLDFLDLIM, ifld);
}


void sfieldlimapply(w,client_data,call_data)
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
   tmpstr = XmTextFieldGetString(sfieldmintxt);
   fldmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(sfieldmaxtxt);
   fldmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = sfieldlimactive;
   if (fldmin != sfieldmin[i] || fldmax != sfieldmax[i])
      sfieldlimauto[i] = 0;
   if (fldmin >= fldmax)
     {
      if (fldmin == 0. && fldmax == 0.)
         sfieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (fldmax < sfieldmin[i] && sfieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (fldmin > sfieldmax[i] && sfieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (sfld_log[i] && (fldmin <= 0. || fldmax <= 0.))
     {
      ierr = 1;
      sfld_log[i] = 0;
      XmToggleButtonGadgetSetState(sfldlog,sfld_log[i],FALSE);
      if (fldmin <= 0. && fldmax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (fldmin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldsmin[i] = fldmin;
      globfieldsmax[i] = fldmax;
      if (sfieldlimauto[i])
        {
         globfieldsmin[i] = sfieldmin[i];
         globfieldsmax[i] = sfieldmax[i];
        }
      sfld_nice_min[i] = globfieldsmin[i];
      sfld_nice_max[i] = globfieldsmax[i];
      if (!sfieldlimauto[i]) 
         fldrange(i, 2, sfld_nice[i], sfld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(sfld_nice_min[i], sfld_nice_max[i], 
                          sfld_nice[i], sfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void sfieldlimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = sfieldlimactive;

   sfieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldsmin[i] = sfieldmin[i];
   globfieldsmax[i] = sfieldmax[i];
   sfld_nice_min[i] = globfieldsmin[i];
   sfld_nice_max[i] = globfieldsmax[i];
   sfld_log[i] = 0;
   sfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(sfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sfldlog,FALSE,FALSE);

   filltext20(sfieldmintxt,globfieldsmin[i]);
   filltext20(sfieldmaxtxt,globfieldsmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}

void sfieldlimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < snumvars; i++)
     {
      sfieldlimauto[i] = 0;

      globfieldsmin[i] = sfieldmin[i];
      globfieldsmax[i] = sfieldmax[i];
      sfld_nice_min[i] = globfieldsmin[i];
      sfld_nice_max[i] = globfieldsmax[i];
      sfld_log[i] = 0;
      sfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(sfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sfldlog,FALSE,FALSE);

   i = sfieldlimactive;

   filltext20(sfieldmintxt,globfieldsmin[i]);
   filltext20(sfieldmaxtxt,globfieldsmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sfieldlimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = sfieldlimactive;

   sfieldlimauto[i] = 1;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldsmin[i] = sfieldmin[i];
   globfieldsmax[i] = sfieldmax[i];
   sfld_nice_min[i] = globfieldsmin[i];
   sfld_nice_max[i] = globfieldsmax[i];
   sfld_log[i] = 0;
   sfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(sfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sfldlog,FALSE,FALSE);

   filltext20(sfieldmintxt,globfieldsmin[i]);
   filltext20(sfieldmaxtxt,globfieldsmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sfieldlimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < snumvars; i++)
     {
      sfieldlimauto[i] = 1;

      globfieldsmin[i] = sfieldmin[i];
      globfieldsmax[i] = sfieldmax[i];
      sfld_nice_min[i] = globfieldsmin[i];
      sfld_nice_max[i] = globfieldsmax[i];
      sfld_log[i] = 0;
      sfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(sfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sfldlog,FALSE,FALSE);

   i = sfieldlimactive;

   filltext20(sfieldmintxt,globfieldsmin[i]);
   filltext20(sfieldmaxtxt,globfieldsmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sfldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = sfieldlimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) sfld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      sfld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (sfld_log[i] && (globfieldsmin[i] <= 0. || globfieldsmax[i] <= 0.))
        {
         ierr = 1;
         sfld_log[i] = 0;
         XmToggleButtonGadgetSetState(sfldlog,sfld_log[i],FALSE);
         if (globfieldsmin[i] <= 0. && globfieldsmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data ",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldsmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(sfielderror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   sfld_nice_min[i] = globfieldsmin[i];
   sfld_nice_max[i] = globfieldsmax[i];
   fldrange(i, 2, sfld_nice[i], sfld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(sfld_nice_min[i], sfld_nice_max[i], 
                          sfld_nice[i], sfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void makesfieldlimform(parent)
 Widget parent;
{
  Widget sfldclose, sfldminlab, sfldmaxlab, sfldlab1, sfldnewfld,
         sfldstat1, sfldstat2, sfldblank, sfldreset, sfldresetall,
         sfldautoreset, sfldautoresetall, sfldapply; 
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Set Surface Field Limits",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   sfieldlimform = XmCreateFormDialog(parent,"Sfieldlimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sfldclose = XtVaCreateManagedWidget("Sfldclose",
               xmPushButtonGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfldclose,XmNactivateCallback,closesfieldlim,
                    NULL);

   string = XmStringCreate("Field: ",XmSTRING_DEFAULT_CHARSET);
   sfldlab1 = XtVaCreateManagedWidget("Sfldlab1",
              xmLabelGadgetClass,
              sfieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfldclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   sfieldlab = XtVaCreateManagedWidget("Sfieldlab",
               xmLabelGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfldclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sfldlab1,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   sfldnewfld = XtVaCreateManagedWidget("Sfldnewfld",
                xmPushButtonGadgetClass,
                sfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sfldlab1,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(sfldnewfld,XmNactivateCallback,sfieldlimfld,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   sfielderror = XtVaCreateManagedWidget("Sfielderror",
                 xmLabelGadgetClass,
                 sfieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sfldnewfld,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   sfldblank = XtVaCreateManagedWidget("Sfldblank",
               xmLabelGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfielderror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   sfldstat1 = XtVaCreateManagedWidget("Sfldstat1",
               xmLabelGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfldblank,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   sfieldminlab = XtVaCreateManagedWidget("Sfieldminlab",
                  xmLabelGadgetClass,
                  sfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sfldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sfldstat1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                   ",XmSTRING_DEFAULT_CHARSET);
   sfieldmaxlab = XtVaCreateManagedWidget("Sfieldmaxlab",
                  xmLabelGadgetClass,
                  sfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sfldblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sfieldminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   sfldstat2 = XtVaCreateManagedWidget("Sfldstat2",
               xmLabelGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfldstat1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   sfieldmintxt = XtVaCreateManagedWidget("Sfieldmintxt",
                  xmTextFieldWidgetClass,
                  sfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sfldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sfldstat2,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   sfieldmaxtxt = XtVaCreateManagedWidget("Sfieldmaxtxt",
                  xmTextFieldWidgetClass,
                  sfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sfldstat1,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sfieldmintxt,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 20,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   sfldminlab = XtVaCreateManagedWidget("Sfldminlab",
                xmLabelGadgetClass,
                sfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sfielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, sfieldminlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   sfldmaxlab = XtVaCreateManagedWidget("Sfldmaxlab",
                xmLabelGadgetClass,
                sfieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sfielderror,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, sfieldmaxlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   sfldapply = XtVaCreateManagedWidget("Sfldapply",
               xmPushButtonGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfldstat2,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfldapply,XmNactivateCallback,sfieldlimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   sfldnice = XtVaCreateManagedWidget("Sfldnice",
              xmToggleButtonGadgetClass,
              sfieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfldapply,
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
      XtAddCallback(sfldnice,XmNvalueChangedCallback,
                    sfldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   sfldlog = XtVaCreateManagedWidget("Sfldlog",
            xmToggleButtonGadgetClass,
            sfieldlimform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, sfldnice,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(sfldlog,XmNvalueChangedCallback,
                    sfldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   sfldreset = XtVaCreateManagedWidget("Sfldreset",
               xmPushButtonGadgetClass,
               sfieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfldlog,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfldreset,XmNactivateCallback,sfieldlimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   sfldresetall = XtVaCreateManagedWidget("Sfldresetall",
                  xmPushButtonGadgetClass,
                  sfieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sfldlog,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sfldreset,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sfldresetall,XmNactivateCallback,sfieldlimresetall,
                    NULL);

   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   sfldautoreset = XtVaCreateManagedWidget("Sfldautoreset",
                   xmPushButtonGadgetClass,
                   sfieldlimform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sfldreset,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(sfldautoreset,XmNactivateCallback,sfieldlimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   sfldautoresetall = XtVaCreateManagedWidget("Fcldautoresetall",
                      xmPushButtonGadgetClass,
                      sfieldlimform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sfldreset,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, sfldautoreset,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
      XmStringFree(string);
      XtAddCallback(sfldautoresetall,XmNactivateCallback,sfieldlimautoresetall,
                    NULL);
}


void sfieldlimgetsfield(int i)
{
  char str[20];
  XmString string;

   sfieldlimactive = i;

   string = XmStringCreate(sfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",sfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",sfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(sfieldmintxt,globfieldsmin[i]);
   filltext20(sfieldmaxtxt,globfieldsmax[i]);

   XmToggleButtonGadgetSetState(sfldnice,sfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(sfldlog,sfld_log[i],FALSE);
}


void sfieldlimpop(int ifld)
{
   sfieldlimactive = ifld;
   sfieldlimformpop();
}
