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

extern short noresizeflag;

static Widget windowsizeform, winwidthlab, winheightlab, winwidthtxt,
              winheighttxt, winsizerror;


void fill_winsize()
{
  char str[20];
  XmString string;

   sprintf(str,"%ld",windowwidth);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winwidthlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%ld",windowheight);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winheightlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%ld",windowwidth);
   XtVaSetValues(winwidthtxt,XmNvalue,str,NULL);
   sprintf(str,"%ld",windowheight);
   XtVaSetValues(winheighttxt,XmNvalue,str,NULL);
}


void pop_winsize()
{
   fill_winsize();

   MyManageChild(windowsizeform);
}


void closewinsiz(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(windowsizeform);
}


void winresize()
{
  int wwidth, wheight, topx, topy, dx, dy;
  unsigned int topw, toph, border_width, depth, dw, dh, dwidth, ddepth;
  Window root;

   xsize = windowwidth;
   ysize = windowheight;
   aspect = (float)xsize / (float)ysize;
   XGetGeometry(dpy,topwin,&root,&topx,&topy,&topw,&toph,
                &border_width,&depth);
   XGetGeometry(dpy,XtWindow(drawframe),&root,&dx,&dy,&dw,&dh,
                &dwidth,&ddepth);
   wwidth = topw - dw;
   wheight = toph - dh;
   XResizeWindow(dpy,topwin,wwidth+windowwidth+4,wheight+windowheight+4);
}


void winsizeapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ierr;
  int wwidth, wheight;
  char *tmpstr;
  static char str1[] = 
           "Error - size must be greater than 0.";
  static char strerror[55];
  char str[20];
  XmString string;

   /*  Read and check the new window size values.  */
   tmpstr = XmTextFieldGetString(winwidthtxt);
   wwidth = atoi(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(winheighttxt);
   wheight = atoi(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winsizerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   ierr = 0;
   if (wwidth <= 0 || wheight <= 0)
     {
      strcpy(strerror,str1);
      ierr = 1;
     }

   if (ierr > 0)
     {
      string = XmStringCreate(strerror,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(winsizerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (ierr == 0) 
     {
      noresizeflag = 1;
      if (wwidth > windowxmax) wwidth = windowxmax;
      if (wheight > windowymax) wheight = windowymax;
      windowwidth = wwidth;
      windowheight = wheight;

      sprintf(str,"%ld",windowwidth);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(winwidthlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%ld",windowheight);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(winheightlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      winresize();

      XFlush(dpy);
      glXWaitX();

      if (displistflag) drawnewlistflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void winsizereset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  XmString string;

   noresizeflag = 1;
   windowwidth = 900;
   windowheight = 700;

   sprintf(str,"%ld",windowwidth);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winwidthlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%ld",windowheight);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winheightlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%ld",windowwidth);
   XtVaSetValues(winwidthtxt,XmNvalue,str,NULL);
   sprintf(str,"%ld",windowheight);
   XtVaSetValues(winheighttxt,XmNvalue,str,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(winsizerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   winresize();

   XFlush(dpy);
   glXWaitX();

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void makewindowsizeform(parent)
 Widget parent;
{
  Widget winclose, winxlab, winylab, winstat1, winstat2,
         winblank, winreset, winapply; 
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Set Window Size",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   windowsizeform = XmCreateFormDialog(parent,"Windowsizeform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   winclose = XtVaCreateManagedWidget("Winclose",
              xmPushButtonGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(winclose,XmNactivateCallback,closewinsiz,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   winsizerror = XtVaCreateManagedWidget("Winsizerror",
                 xmLabelGadgetClass,
                 windowsizeform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, winclose,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   winblank = XtVaCreateManagedWidget("Fldblank",
              xmLabelGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, winsizerror,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Current: ",XmSTRING_DEFAULT_CHARSET);
   winstat1 = XtVaCreateManagedWidget("Winstat1",
              xmLabelGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, winblank,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
   winwidthlab = XtVaCreateManagedWidget("Winwidthlab",
                 xmLabelGadgetClass,
                 windowsizeform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, winblank,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, winstat1,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
 
   string = XmStringCreate("           ",XmSTRING_DEFAULT_CHARSET);
   winheightlab = XtVaCreateManagedWidget("winheightlab",
                  xmLabelGadgetClass,
                  windowsizeform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, winblank,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, winwidthlab,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
 
   string = XmStringCreate("New:     ",XmSTRING_DEFAULT_CHARSET);
   winstat2 = XtVaCreateManagedWidget("Winstat2",
              xmLabelGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, winstat1,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   winwidthtxt = XtVaCreateManagedWidget("Winwidthtxt",
                 xmTextFieldWidgetClass,
                 windowsizeform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, winstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, winstat2,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 5,
                 XmNcolumns, 5,
                 XmNvalue, "",
                 NULL);

   winheighttxt = XtVaCreateManagedWidget("Winheighttxt",
                 xmTextFieldWidgetClass,
                 windowsizeform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, winstat1,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, winwidthtxt,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 5,
                 XmNcolumns, 5,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Xsize",XmSTRING_DEFAULT_CHARSET);
   winxlab = XtVaCreateManagedWidget("Fldminlab",
               xmLabelGadgetClass,
               windowsizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, winsizerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNleftWidget, winwidthlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate(" Ysize",XmSTRING_DEFAULT_CHARSET);
   winylab = XtVaCreateManagedWidget("Fldmaxlab",
               xmLabelGadgetClass,
               windowsizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, winsizerror,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, winxlab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply Change",XmSTRING_DEFAULT_CHARSET);
   winapply = XtVaCreateManagedWidget("Fldapply",
              xmPushButtonGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, winstat2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(winapply,XmNactivateCallback,winsizeapply,
                    NULL);

   string = XmStringCreate("Reset (900x700)",XmSTRING_DEFAULT_CHARSET);
   winreset = XtVaCreateManagedWidget("Winreset",
              xmPushButtonGadgetClass,
              windowsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, winapply,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(winreset,XmNactivateCallback,winsizereset,
                    NULL);
}
