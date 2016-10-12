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

static Widget fieldlab, fieldminlab, fieldmaxlab, fieldmintxt,
              fieldmaxtxt, fielderror, fldnice, fldlog;
static int fieldlimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void fieldlimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = fieldlimactive;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldmin[i];
   fmax = globfieldmax[i];
   if (nfieldlimauto[i])
     {
      fmin = fieldmin[i];
      fmax = fieldmax[i];
     }
   filltext20(fieldmintxt,fmin);
   filltext20(fieldmaxtxt,fmax);

   XmToggleButtonGadgetSetState(fldnice,nfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(fldlog,nfld_log[i],FALSE);

   MyManageChild(fieldlimform);
   XtUnmanageChild(nfieldform);
}


void closefieldlim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(fieldlimform);
}


void fieldlimfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = fieldlimactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDFLDLIM, ifld);
}


void fieldlimapply(w,client_data,call_data)
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

   /*  Read and check the new min and max values.  */
   tmpstr = XmTextFieldGetString(fieldmintxt);
   fldmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(fieldmaxtxt);
   fldmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = fieldlimactive;
   if (fldmin != fieldmin[i] || fldmax != fieldmax[i])
      nfieldlimauto[i] = 0;
   if (fldmin >= fldmax)
     {
      if (fldmin == 0. && fldmax == 0.)
         nfieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (fldmax < fieldmin[i] && nfieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (fldmin > fieldmax[i] && nfieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (nfld_log[i] && (fldmin <= 0. || fldmax <= 0.))
     {
      ierr = 1;
      nfld_log[i] = 0;
      XmToggleButtonGadgetSetState(fldlog,nfld_log[i],FALSE);
      if (fldmin <= 0. && fldmax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (fldmin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fielderror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldmin[i] = fldmin;
      globfieldmax[i] = fldmax;
      if (nfieldlimauto[i])
        {
         globfieldmin[i] = fieldmin[i];
         globfieldmax[i] = fieldmax[i];
        }
      nfld_nice_min[i] = globfieldmin[i];
      nfld_nice_max[i] = globfieldmax[i];
      if (!nfieldlimauto[i]) 
         fldrange(i, 0, nfld_nice[i], nfld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(nfld_nice_min[i], nfld_nice_max[i], 
                          nfld_nice[i], nfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void fieldlimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = fieldlimactive;

   nfieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldmin[i] = fieldmin[i];
   globfieldmax[i] = fieldmax[i];
   nfld_nice_min[i] = globfieldmin[i];
   nfld_nice_max[i] = globfieldmax[i];
   nfld_log[i] = 0;
   nfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(fldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(fldlog,FALSE,FALSE);

   filltext20(fieldmintxt,globfieldmin[i]);
   filltext20(fieldmaxtxt,globfieldmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void fieldlimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < numvars; i++)
     {
      nfieldlimauto[i] = 0;

      globfieldmin[i] = fieldmin[i];
      globfieldmax[i] = fieldmax[i];
      nfld_nice_min[i] = globfieldmin[i];
      nfld_nice_max[i] = globfieldmax[i];
      nfld_log[i] = 0;
      nfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(fldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(fldlog,FALSE,FALSE);

   i = fieldlimactive;

   filltext20(fieldmintxt,globfieldmin[i]);
   filltext20(fieldmaxtxt,globfieldmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void fieldlimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  XmString string;

   i = fieldlimactive;

   nfieldlimauto[i] = 1;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldmin[i] = fieldmin[i];
   globfieldmax[i] = fieldmax[i];
   nfld_nice_min[i] = globfieldmin[i];
   nfld_nice_max[i] = globfieldmax[i];
   nfld_log[i] = 0;
   nfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(fldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(fldlog,FALSE,FALSE);

   filltext20(fieldmintxt,globfieldmin[i]);
   filltext20(fieldmaxtxt,globfieldmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void fieldlimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < numvars; i++)
     {
      nfieldlimauto[i] = 1;

      globfieldmin[i] = fieldmin[i];
      globfieldmax[i] = fieldmax[i];
      nfld_nice_min[i] = globfieldmin[i];
      nfld_nice_max[i] = globfieldmax[i];
      nfld_log[i] = 0;
      nfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(fldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(fldlog,FALSE,FALSE);

   i = fieldlimactive;

   filltext20(fieldmintxt,globfieldmin[i]);
   filltext20(fieldmaxtxt,globfieldmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void fldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = fieldlimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fielderror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) nfld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      nfld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (nfld_log[i] && (globfieldmin[i] <= 0. || globfieldmax[i] <= 0.))
        {
         ierr = 1;
         nfld_log[i] = 0;
         XmToggleButtonGadgetSetState(fldlog,nfld_log[i],FALSE);
         if (globfieldmin[i] <= 0. && globfieldmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data.",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(fielderror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   nfld_nice_min[i] = globfieldmin[i];
   nfld_nice_max[i] = globfieldmax[i];
   fldrange(i, 0, nfld_nice[i], nfld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(nfld_nice_min[i], nfld_nice_max[i], 
                          nfld_nice[i], nfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void makefieldlimform(parent)
 Widget parent;
{
  Widget fldclose, fldminlab, fldmaxlab, fldlab1, fldnewfld,
         fldstat1, fldstat2, fldblank, fldreset, fldresetall,
         fldautoreset, fldautoresetall, fldapply; 
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Set Node Field Limits",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fieldlimform = XmCreateFormDialog(parent,"Fieldlimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fldclose = XtVaCreateManagedWidget("Fldclose",
              xmPushButtonGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fldclose,XmNactivateCallback,closefieldlim,
                    NULL);

   string = XmStringCreate("Field: ",XmSTRING_DEFAULT_CHARSET);
   fldlab1 = XtVaCreateManagedWidget("Fldlab1",
             xmLabelGadgetClass,
             fieldlimform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fldclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   fieldlab = XtVaCreateManagedWidget("Fieldlab",
              xmLabelGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fldclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, fldlab1,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fldnewfld = XtVaCreateManagedWidget("Fldnewfld",
               xmPushButtonGadgetClass,
               fieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fldlab1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(fldnewfld,XmNactivateCallback,fieldlimfld,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   fielderror = XtVaCreateManagedWidget("Fielderror",
                xmLabelGadgetClass,
                fieldlimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, fldnewfld,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   fldblank = XtVaCreateManagedWidget("Fldblank",
              xmLabelGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fielderror,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   fldstat1 = XtVaCreateManagedWidget("Fldstat1",
              xmLabelGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fldblank,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   fieldminlab = XtVaCreateManagedWidget("Fieldminlab",
                 xmLabelGadgetClass,
                 fieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, fldblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, fldstat1,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                     ",XmSTRING_DEFAULT_CHARSET);
   fieldmaxlab = XtVaCreateManagedWidget("Fieldmaxlab",
                 xmLabelGadgetClass,
                 fieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, fldblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, fieldminlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   fldstat2 = XtVaCreateManagedWidget("Fldstat2",
              xmLabelGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fldstat1,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   fieldmintxt = XtVaCreateManagedWidget("Fieldmintxt",
                 xmTextFieldWidgetClass,
                 fieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, fldstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, fldstat2,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 20,
                 XmNvalue, "",
                 NULL);

   fieldmaxtxt = XtVaCreateManagedWidget("Fieldmaxtxt",
                 xmTextFieldWidgetClass,
                 fieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, fldstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, fieldmintxt,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 20,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   fldminlab = XtVaCreateManagedWidget("Fldminlab",
               xmLabelGadgetClass,
               fieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fielderror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, fieldminlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   fldmaxlab = XtVaCreateManagedWidget("Fldmaxlab",
               xmLabelGadgetClass,
               fieldlimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fielderror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, fieldmaxlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   fldapply = XtVaCreateManagedWidget("Fldapply",
              xmPushButtonGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fldstat2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fldapply,XmNactivateCallback,fieldlimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   fldnice = XtVaCreateManagedWidget("Fldnice",
             xmToggleButtonGadgetClass,
             fieldlimform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fldapply,
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
      XtAddCallback(fldnice,XmNvalueChangedCallback,
                    fldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   fldlog = XtVaCreateManagedWidget("Fldlog",
            xmToggleButtonGadgetClass,
            fieldlimform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, fldnice,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(fldlog,XmNvalueChangedCallback,
                    fldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   fldreset = XtVaCreateManagedWidget("Fldreset",
              xmPushButtonGadgetClass,
              fieldlimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fldlog,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fldreset,XmNactivateCallback,fieldlimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   fldresetall = XtVaCreateManagedWidget("Fldresetall",
                 xmPushButtonGadgetClass,
                 fieldlimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, fldlog,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, fldreset,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(fldresetall,XmNactivateCallback,fieldlimresetall,
                    NULL);
   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   fldautoreset = XtVaCreateManagedWidget("Fldautoreset",
                  xmPushButtonGadgetClass,
                  fieldlimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, fldreset,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(fldautoreset,XmNactivateCallback,fieldlimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   fldautoresetall = XtVaCreateManagedWidget("Fldautoresetall",
                     xmPushButtonGadgetClass,
                     fieldlimform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, fldreset,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, fldautoreset,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(fldautoresetall,XmNactivateCallback,fieldlimautoresetall,
                    NULL);
}


void fieldlimgetnfield(int i)
{
  char str[20];
  XmString string;

   fieldlimactive = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%-20.11g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fieldmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(fieldmintxt,globfieldmin[i]);
   filltext20(fieldmaxtxt,globfieldmax[i]);

   XmToggleButtonGadgetSetState(fldnice,nfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(fldlog,nfld_log[i],FALSE);
}


void fieldlimpop(int ifld)
{
   fieldlimactive = ifld;
   fieldlimformpop();
}


void filltext20(Widget wtext, double value)
{
   char str[21];
   sprintf(str,"%.11g",value);
   XmTextFieldSetString(wtext,str);
}
