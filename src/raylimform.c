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

static Widget rayminlab, raymaxlab, raymintxt, raymaxtxt,
              rayerror, rfldnice, rfldlog;
static int raylimactive = 0;
void setfldcolortype3(double fldmin, double fldmax, char niceflag, 
                      char logflag);
void filltext20(Widget wtext, double value); 


void raylimformpop()
{
  int i;
  float fmin, fmax;
  char str[20];
  XmString string;

   i = raylimactive;

   sprintf(str,"%-20.11g",rfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",rfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(raymaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fmin = globfieldrmin[i];
   fmax = globfieldrmax[i];
   if (rfieldlimauto[i])
     {
      fmin = rfieldmin[i];
      fmax = rfieldmax[i];
     }
   filltext20(raymintxt,fmin);
   filltext20(raymaxtxt,fmax);

   XmToggleButtonGadgetSetState(rfldnice,rfld_nice[i],FALSE);
   XmToggleButtonGadgetSetState(rfldlog,rfld_log[i],FALSE);

   MyManageChild(raylimform);
}


void closeraylim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(raylimform);
}


void raylim_select_active_field(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = (long int)client_data;
   raylimactive = i;

   sprintf(str,"%-20.11g",rfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%-20.11g",rfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(raymaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   filltext20(raymintxt,globfieldrmin[i]);
   filltext20(raymaxtxt,globfieldrmax[i]);
}


void raylimapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ierr;
  double raymin, raymax;
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
   tmpstr = XmTextFieldGetString(raymintxt);
   raymin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(raymaxtxt);
   raymax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   i = raylimactive;
   rfieldlimauto[i] = 0;
   if (raymin >= raymax)
     {
      if (raymin == 0. && raymax == 0.)
         rfieldlimauto[i] = 1;
      else
        {
         strcpy(strerror,str1);
         ierr = 2;
        }
     }

   if (raymax < rfieldmin[i] && rfieldlimauto[i] == 0)
     {
      strcpy(strerror,str2);
      ierr = 1;
     }

   if (raymin > rfieldmax[i] && rfieldlimauto[i] == 0)
     {
      strcpy(strerror,str3);
      ierr = 1;
     }

   if (rfld_log[i] && (raymin <= 0. || raymax <= 0.))
     {
      ierr = 1;
      rfld_log[i] = 0;
      XmToggleButtonGadgetSetState(rfldlog,rfld_log[i],FALSE);
      if (raymin <= 0. && raymax <= 0.)
         strcpy(strerror,"Cannot use Log on Negative Data.");
      if (raymin <= 0.)
         strcpy(strerror,"For log, reset minimum > 0.");
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(rayerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr < 2) 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      globfieldrmin[i] = raymin;
      globfieldrmax[i] = raymax;
      if (rfieldlimauto[i])
        {
         globfieldrmin[i] = rfieldmin[i];
         globfieldrmax[i] = rfieldmax[i];
        }
      rfld_nice_min[i] = globfieldrmin[i];
      rfld_nice_max[i] = globfieldrmax[i];
      if (!rfieldlimauto[i]) 
         fldrange(i, 4, rfld_nice[i], rfld_log[i]);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(rfld_nice_min[i], rfld_nice_max[i], 
                          rfld_nice[i], rfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void raylimreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = raylimactive;

   rfieldlimauto[i] = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   globfieldrmin[i] = rfieldmin[i];
   globfieldrmax[i] = rfieldmax[i];
   rfld_nice_min[i] = globfieldrmin[i];
   rfld_nice_max[i] = globfieldrmax[i];
   rfld_log[i] = 0;
   rfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(rfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(rfldlog,FALSE,FALSE);

   filltext20(raymintxt,globfieldrmin[i]);
   filltext20(raymaxtxt,globfieldrmax[i]);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void raylimresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < rnumvars; i++)
     {
      rfieldlimauto[i] = 0;

      globfieldrmin[i] = rfieldmin[i];
      globfieldrmax[i] = rfieldmax[i];
      rfld_nice_min[i] = globfieldrmin[i];
      rfld_nice_max[i] = globfieldrmax[i];
      rfld_log[i] = 0;
      rfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(rfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(rfldlog,FALSE,FALSE);

   i = raylimactive;

   sprintf(str,"%-20.11g",globfieldrmin[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldrmax[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void raylimautoreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   i = raylimactive;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   rfieldlimauto[i] = 1;

   globfieldrmin[i] = rfieldmin[i];
   globfieldrmax[i] = rfieldmax[i];
   rfld_nice_min[i] = globfieldrmin[i];
   rfld_nice_max[i] = globfieldrmax[i];
   rfld_log[i] = 0;
   rfld_nice[i] = 0;
   XmToggleButtonGadgetSetState(rfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(rfldlog,FALSE,FALSE);

   sprintf(str,"%-20.11g",globfieldrmin[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldrmax[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void raylimautoresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   for (i = 0; i < rnumvars; i++)
     {
      rfieldlimauto[i] = 1;

      globfieldrmin[i] = rfieldmin[i];
      globfieldrmax[i] = rfieldmax[i];
      rfld_nice_min[i] = globfieldrmin[i];
      rfld_nice_max[i] = globfieldrmax[i];
      rfld_log[i] = 0;
      rfld_nice[i] = 0;
     }

   XmToggleButtonGadgetSetState(rfldnice,FALSE,FALSE);
   XmToggleButtonGadgetSetState(rfldlog,FALSE,FALSE);

   i = raylimactive;

   sprintf(str,"%-20.11g",globfieldrmin[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);
   sprintf(str,"%-20.11g",globfieldrmax[i]);
   XtVaSetValues(raymaxtxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  If red to blue by contour levels color scale used, reset.  */
   if (fieldcolortype == 3) setfieldcolors();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void rfldflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, ierr;
  XmString string;

   arg = (long int)client_data;
   i = raylimactive;
   ierr = 0;

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayerror,XmNlabelString,string,NULL);
   XmStringFree(string);
   
   if (arg == 0) rfld_nice[i] = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     {
      rfld_log[i] = (int)XmToggleButtonGadgetGetState(w);
      if (rfld_log[i] && (globfieldrmin[i] <= 0. || globfieldrmax[i] <= 0.))
        {
         ierr = 1;
         rfld_log[i] = 0;
         XmToggleButtonGadgetSetState(rfldlog,rfld_log[i],FALSE);
         if (globfieldrmin[i] <= 0. && globfieldrmax[i] <= 0.)
            string = XmStringCreate("Cannot use Log on Negative Data ",
                     XmSTRING_DEFAULT_CHARSET);
         if (globfieldrmin[i] <= 0.)
            string = XmStringCreate("For log, reset minimum > 0.",
                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(rayerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   rfld_nice_min[i] = globfieldrmin[i];
   rfld_nice_max[i] = globfieldrmax[i];
   fldrange(i, 4, rfld_nice[i], rfld_log[i]);

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(rfld_nice_min[i], rfld_nice_max[i], 
                          rfld_nice[i], rfld_log[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void makeraylimform(parent)
 Widget parent;
{
  Widget rayclose, raystat1, raystat2, rayblank, rayreset, rayapply,
         rayswin, raybox, raylist[100], rayresetall, rayautoreset,
         rayautoresetall;
  int i;
  Arg wargs[20];
  char str[20];
  XmString string;

   string = XmStringCreate("Set Ray Limits",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   raylimform = XmCreateFormDialog(parent,"Raylimform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   rayclose = XtVaCreateManagedWidget("Rayclose",
              xmPushButtonGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rayclose,XmNactivateCallback,closeraylim,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, rayclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   rayswin = XmCreateScrolledWindow(raylimform,"Rayswin",wargs,i);
   XtManageChild(rayswin);

   i = 0;
   raybox = XmCreateRadioBox(rayswin,"Raybox",wargs,i);
   XtManageChild(raybox);

   for (i = 0; i < rnumvars; i++)
     {
      string = XmStringCreate(rfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Raylist%d",i);
      raylist[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   raybox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   NULL);
         XmStringFree(string);
         XtAddCallback(raylist[i],XmNvalueChangedCallback,
                       raylim_select_active_field,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(raylist[i],TRUE,FALSE);
     }

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   rayerror = XtVaCreateManagedWidget("Rayerror",
                xmLabelGadgetClass,
                raylimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rayswin,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   rayblank = XtVaCreateManagedWidget("Rayblank",
              xmLabelGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, rayerror,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Original: ",XmSTRING_DEFAULT_CHARSET);
   raystat1 = XtVaCreateManagedWidget("Raystat1",
              xmLabelGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, rayblank,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   rayminlab = XtVaCreateManagedWidget("Rayminlab",
                 xmLabelGadgetClass,
                 raylimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, rayblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, raystat1,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("                    ",XmSTRING_DEFAULT_CHARSET);
   raymaxlab = XtVaCreateManagedWidget("Raymaxlab",
                 xmLabelGadgetClass,
                 raylimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, rayblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, rayminlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current:  ",XmSTRING_DEFAULT_CHARSET);
   raystat2 = XtVaCreateManagedWidget("Raystat2",
              xmLabelGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, raystat1,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   raymintxt = XtVaCreateManagedWidget("Raymintxt",
                xmTextFieldWidgetClass,
                raylimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, raystat1,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, raystat2,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 20,
                XmNcolumns, 20,
                XmNvalue, "",
                NULL);

   raymaxtxt = XtVaCreateManagedWidget("Raymaxtxt",
                xmTextFieldWidgetClass,
                raylimform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, raystat1,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, raymintxt,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 20,
                XmNcolumns, 20,
                XmNvalue, "",
                NULL);

   string = XmStringCreate(" Minimum",XmSTRING_DEFAULT_CHARSET);
   rayminlab = XtVaCreateManagedWidget("Rayminlab",
               xmLabelGadgetClass,
               raylimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, rayerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, rayminlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate(" Maximum",XmSTRING_DEFAULT_CHARSET);
   raymaxlab = XtVaCreateManagedWidget("Raymaxlab",
               xmLabelGadgetClass,
               raylimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, rayerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, raymaxlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   rayapply = XtVaCreateManagedWidget("Rayapply",
              xmPushButtonGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, raystat2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rayapply,XmNactivateCallback,raylimapply,
                    NULL);
 
   string = XmStringCreate("Use Nice Intervals",
            XmSTRING_DEFAULT_CHARSET);
   rfldnice = XtVaCreateManagedWidget("Rfldnice",
               xmToggleButtonGadgetClass,
               raylimform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, rayapply,
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
      XtAddCallback(rfldnice,XmNvalueChangedCallback,
                    rfldflags,(caddr_t) 0);
 
   string = XmStringCreate("Display As Log",
            XmSTRING_DEFAULT_CHARSET);
   rfldlog = XtVaCreateManagedWidget("Rfldlog",
             xmToggleButtonGadgetClass,
             raylimform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, rfldnice,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(rfldlog,XmNvalueChangedCallback,
                    rfldflags,(caddr_t) 1);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   rayreset = XtVaCreateManagedWidget("Rayreset",
              xmPushButtonGadgetClass,
              raylimform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, rfldlog,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rayreset,XmNactivateCallback,raylimreset,
                    NULL);

   string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
   rayresetall = XtVaCreateManagedWidget("Rayresetall",
                 xmPushButtonGadgetClass,
                 raylimform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, rfldlog,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, rayreset,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(rayresetall,XmNactivateCallback,raylimresetall,
                    NULL);
   string = XmStringCreate("Auto Reset",XmSTRING_DEFAULT_CHARSET);
   rayautoreset = XtVaCreateManagedWidget("Rayautoreset",
                  xmPushButtonGadgetClass,
                  raylimform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, rayreset,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(rayautoreset,XmNactivateCallback,raylimautoreset,
                    NULL);

   string = XmStringCreate("Auto Reset All",XmSTRING_DEFAULT_CHARSET);
   rayautoresetall = XtVaCreateManagedWidget("Rayautoresetall",
                     xmPushButtonGadgetClass,
                     raylimform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, rayreset,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, rayautoreset,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(rayautoresetall,XmNactivateCallback,raylimautoresetall,
                    NULL);
}


void rfieldlimpop(int ifld)
{
   raylimactive = ifld;
   raylimformpop();
}
