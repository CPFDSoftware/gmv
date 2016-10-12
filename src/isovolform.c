#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "contour.h"
#include "xstuff.h"

static Widget ivval1, ivval2, ivonbut, clipboxv,  clipselv,
              ivfmin, ivfmax, ivfldlab, ivfclrbut, ivfclrlab;
static int ivdrawfieldon, ivdrawfield;


void isovolformpop()
{
  int i;
  char str[20];
  XmString string;

   i = isovolfieldactive;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"Min: %.9g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfmin,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max: %.9g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfmax,XmNlabelString,string,NULL);
   XmStringFree(string);

   XmToggleButtonSetState(ivonbut,isovol_on,FALSE);
   if (isovol_on)
     {
      sprintf(str,"%.9g",isovol_val1);
      XmTextFieldSetString(ivval1,str);
      sprintf(str,"%.9g",isovol_val2);
      XmTextFieldSetString(ivval2,str);
     }
   XmToggleButtonGadgetSetState(clipboxv,ivcliponbox,FALSE);
   XmToggleButtonGadgetSetState(clipselv,ivcliponcellsel,FALSE);

   ivdrawfieldon = 0;
   ivdrawfield = isovolfield_draw;
   XmToggleButtonSetState(ivfclrbut,FALSE,FALSE);
   if (ivdrawfield >= 0)
     {
      ivdrawfieldon = 1;
      XmToggleButtonSetState(ivfclrbut,TRUE,FALSE);
     }

   i = isovolfield_draw;
   if (i < 0) i = 0;
   ivdrawfield = i;
   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(isovolform);
}


void ivclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(isovolform);
}


void ivclip_on_box(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   ivcliponbox = (int)XmToggleButtonGadgetGetState(w);
   XmToggleButtonGadgetSetState(clipboxv,ivcliponbox,FALSE);
}


void ivclip_on_cell_sel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   ivcliponcellsel = (int)XmToggleButtonGadgetGetState(w);
   XmToggleButtonGadgetSetState(clipselv,ivcliponcellsel,FALSE);
}


void add_isovol(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr1, *tmpstr2;
  char str1[20], str3[90];
  double *value;
  double * readnodevalue(int ifld);
  void calcisovolume(float isoval1, float isoval2, double *value);
  void isovolgetfldclr();

   tmpstr1 = XmTextFieldGetString(ivval1);
   isovol_val1 = atof(tmpstr1);
   XtFree(tmpstr1);
   strcpy(str3,fieldname[isovolfieldactive]);
   sprintf(str1," %.9g",isovol_val1);
   strcat(str3,str1);
   tmpstr2 = XmTextFieldGetString(ivval2);
   isovol_val2 = atof(tmpstr2);
   XtFree(tmpstr2);
   sprintf(str1," to %.9g",isovol_val2);
   strcat(str3,str1);
   value = readnodevalue(isovolfieldactive);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   calcisovolume(isovol_val1,isovol_val2,value);

   if (isovol_on == 1)
      XmToggleButtonSetState(ivonbut,TRUE,FALSE);
   FREE(value);
   if (isovol_on && ivdrawfieldon)
     {
      isovolfield_draw = ivdrawfield;
      isovolgetfldclr();
     }

   XUndefineCursor(dpy,topwin);

   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void del_isovol(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
 
   isovol_on = 0;
   i = MAXMATS + MAXCONTOURS + 1;
   contour_on[i] = 0;
   XmToggleButtonSetState(ivonbut,FALSE,FALSE);
   FREE(isovoldrawfield);

   if (num_of_isovol_polys > 0)
     {
      num_of_isovol_polys = 0;
      num_of_contour_polys[i] = 0;
      FREE(contourlist[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void sel_desel_isovol(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ibut, i;

   i = MAXMATS + MAXCONTOURS + 1;
   if (num_of_isovol_polys == 0)
      XmToggleButtonSetState(w,FALSE,FALSE);
   else
     {
      ibut = (int)XmToggleButtonGetState(w);
      isovol_on = (int)ibut;
      contour_on[i] = ibut;
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void ivnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = isovolfieldactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDISOVOL, ifld);
}


void ivnfclrfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = ivdrawfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDISOVOLCLR, ifld);
}


void setfldclr(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ibut;
  void isovolgetfldclr();

   isovolfield_draw = -1;
   ivdrawfieldon = 0;
   ibut = (int)XmToggleButtonGetState(w);
   if (ibut)
     {
      ivdrawfieldon = 1;
      isovolfield_draw = ivdrawfield;
     }

   if (isovol_on)
     {
      isovolgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void preadivval(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  float isovol_val;

   tmpstr = XmTextFieldGetString(w);
   isovol_val = atof(tmpstr);
   XtFree(tmpstr);
}


void makeisovolform(parent)
 Widget parent;
{
  Widget closev, ivvallab1, ivvallab2, ivfldlab1, ivfldbut,
         ivaddbut, ivdelbut, ivfclrlab1, ivfclrbut2;
  int i, matpixid;
  unsigned long matpix;
  Arg wargs[20];
  XmString string; 


   string = XmStringCreate("Isovolume",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   isovolform = XmCreateFormDialog(parent,"Isovolform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   closev = XtVaCreateManagedWidget("Isovclose",
            xmPushButtonGadgetClass,
            isovolform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(closev,XmNactivateCallback,ivclose,NULL);
 
   string = XmStringCreate("Clip on Field Subset",
            XmSTRING_DEFAULT_CHARSET);
   clipboxv = XtVaCreateManagedWidget("Clipboxv",
              xmToggleButtonGadgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, closev,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(clipboxv,XmNvalueChangedCallback,
                    ivclip_on_box,NULL);
 
   string = XmStringCreate("Clip on Cell Selection",
            XmSTRING_DEFAULT_CHARSET);
   clipselv = XtVaCreateManagedWidget("Clipselv",
              xmToggleButtonGadgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clipboxv,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(clipselv,XmNvalueChangedCallback,
                    ivclip_on_cell_sel,NULL);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   ivfldlab1 = XtVaCreateManagedWidget("Ivfldlab1",
               xmLabelWidgetClass,
               isovolform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clipselv,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   ivfldlab = XtVaCreateManagedWidget("Ivfldlab",
              xmLabelWidgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clipselv,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ivfldlab1,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string); 

   string = XmStringCreate("Min: ",XmSTRING_DEFAULT_CHARSET);
   ivfmin = XtVaCreateManagedWidget("ivfmin",
            xmLabelWidgetClass,
            isovolform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, ivfldlab1,
            XmNleftAttachment, XmATTACH_FORM,
            XmNleftOffset, 20,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
    XmStringFree(string);

   string = XmStringCreate("Max: ",XmSTRING_DEFAULT_CHARSET);
   ivfmax = XtVaCreateManagedWidget("ivfmax",
            xmLabelWidgetClass,
            isovolform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, ivfmin,
            XmNleftAttachment, XmATTACH_FORM,
            XmNleftOffset, 20,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
    XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ivfldbut = XtVaCreateManagedWidget("Ivfldbut",
              xmPushButtonGadgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ivfmax,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNleftOffset, 20,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ivfldbut,XmNactivateCallback,ivnfldfld,
                    NULL);

   string = XmStringCreate("Isovolume Range:",XmSTRING_DEFAULT_CHARSET);
   ivvallab1 = XtVaCreateManagedWidget("Ivvallab1",
               xmLabelWidgetClass,
               isovolform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ivfldbut,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   ivval1 = XtVaCreateManagedWidget("Ivval1",
            xmTextFieldWidgetClass,
            isovolform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, ivvallab1,
            XmNleftAttachment, XmATTACH_FORM,
            XmNleftOffset, 20,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 10,
            XmNcolumns, 10,
            XmNvalue, "",
            NULL);
      XtAddCallback(ivval1,XmNactivateCallback,preadivval,(caddr_t) 0);

   string = XmStringCreate("    to",XmSTRING_DEFAULT_CHARSET);
   ivvallab2 = XtVaCreateManagedWidget("Ivvallab2",
               xmLabelWidgetClass,
               isovolform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ivval1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   ivval2 = XtVaCreateManagedWidget("Ivval2",
            xmTextFieldWidgetClass,
            isovolform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, ivvallab2,
            XmNleftAttachment, XmATTACH_FORM,
            XmNleftOffset, 20,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 10,
            XmNcolumns, 10,
            XmNvalue, "",
            NULL);
      XtAddCallback(ivval2,XmNactivateCallback,preadivval,(caddr_t) 1);

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   ivaddbut = XtVaCreateManagedWidget("Ivaddbut",
              xmPushButtonGadgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ivval2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNleftOffset, 5,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ivaddbut,XmNactivateCallback,add_isovol,
                    NULL);

   string = XmStringCreate("Del",XmSTRING_DEFAULT_CHARSET);
   ivdelbut = XtVaCreateManagedWidget("Ivdelbut",
              xmPushButtonGadgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ivval2,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ivaddbut,
              XmNleftOffset, 5,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ivdelbut,XmNactivateCallback,del_isovol,
                    NULL);

   matpixid = MAXCONTOURS + 2;
   matpix = mat_pixels[matpixid];
   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   ivonbut = XtVaCreateManagedWidget("Ivonbut",
             xmToggleButtonWidgetClass,
             isovolform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ivval2,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, ivdelbut,
             XmNleftOffset, 5,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNselectColor, yellowpix,
             XmNspacing, 0,
             XmNbackground, matpix,
             XmNforeground, blackpix,
             XmNmarginHeight, 0,
             XmNmarginWidth, 0,
             XmNvisibleWhenOff, FALSE,
             NULL);
      XmStringFree(string);
      XtAddCallback(ivonbut,XmNvalueChangedCallback,sel_desel_isovol,
                    NULL);

   string = XmStringLtoRCreate("Color Surface with\n Node Field ",
                               XmSTRING_DEFAULT_CHARSET);
   ivfclrbut = XtVaCreateManagedWidget("Ivfclrbut",
               xmToggleButtonWidgetClass,
               isovolform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ivaddbut,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNselectColor, yellowpix,
               XmNspacing, 2,
               NULL);
      XmStringFree(string);
      XtAddCallback(ivfclrbut,XmNvalueChangedCallback,setfldclr,
                    NULL);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   ivfclrlab1 = XtVaCreateManagedWidget("Ivfclrlab1",
                xmLabelWidgetClass,
                isovolform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ivfclrbut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 30,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   ivfclrlab = XtVaCreateManagedWidget("Ivfclrlab",
              xmLabelWidgetClass,
              isovolform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ivfclrbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ivfclrlab1,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ivfclrbut2 = XtVaCreateManagedWidget("Ivfclrbut2",
                xmPushButtonGadgetClass,
                isovolform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ivfclrlab1,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 30,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ivfclrbut2,XmNactivateCallback,ivnfclrfld,
                    NULL);

}


void isovolfgetnfield(int arg)
{
  int i;
  char str[20];
  double slmin, slmax;
  XmString string;
 
   i = arg;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   isovolfieldactive = i;
   slmin = fieldmin[i];
   slmax = fieldmax[i];
   sprintf(str,"Min: %.9g",slmin);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfmin,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max: %.9g",slmax);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfmax,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void isovolgetfldclr()
{
   if (isovolfield_draw >= 0)
     {
      if (isovoldrawfield == NULL)
        {
         isovoldrawfield=(double *)malloc((nnodes+1)*sizeof(double));
         if (isovoldrawfield == NULL) memerr();
        }
      fillnodevalue(isovolfield_draw,isovoldrawfield);
     }
}


void isovolfclrgetnfield(int arg)
{
  int i;
  XmString string;
 
   i = arg;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ivfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   ivdrawfield = i;

   if (ivdrawfieldon && isovol_on)
     {
      isovolfield_draw = ivdrawfield;
      isovolgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void calcisovolume(float isovolval1, float isovolval2, double *value)
{
  int npolys, j, i;

   npolys = 0;
   i = MAXCONTOURS + MAXMATS + 1;

   makeisovol(isovolval1,isovolval2,value,&npolys);

   /*  If an isovolume already exists, free memory.  */
   if (num_of_isovol_polys > 0)
     {
      isovol_on = 0;
      num_of_isovol_polys = 0;
      num_of_contour_polys[i] = 0;
      FREE(contourlist[i]);
     }

   /*  Fill isosurface arrays from makeisovol arrays  */
   /*  so that isovolumes are drawn as isosurfaces.   */
   if (npolys > 0)
     {
      num_of_isovol_polys = npolys;
      num_of_contour_polys[i] = npolys;
      contourlist[i] = (struct isocont *)malloc(npolys*
                                            sizeof(struct isocont));
      for (j = 0; j < npolys; j++)
        {
         contourlist[i][j].vx[0] = conlist[j].vx[0];
         contourlist[i][j].vy[0] = conlist[j].vy[0];
         contourlist[i][j].vz[0] = conlist[j].vz[0];
         contourlist[i][j].vx[1] = conlist[j].vx[1];
         contourlist[i][j].vy[1] = conlist[j].vy[1];
         contourlist[i][j].vz[1] = conlist[j].vz[1];
         contourlist[i][j].vx[2] = conlist[j].vx[2];
         contourlist[i][j].vy[2] = conlist[j].vy[2];
         contourlist[i][j].vz[2] = conlist[j].vz[2];
         contourlist[i][j].nx = conlist[j].nx;
         contourlist[i][j].ny = conlist[j].ny;
         contourlist[i][j].nz = conlist[j].nz;
         contourlist[i][j].fact[0] = conlist[j].fact[0];
         contourlist[i][j].node1[0] = conlist[j].node1[0];
         contourlist[i][j].node2[0] = conlist[j].node2[0];
         contourlist[i][j].fact[1] = conlist[j].fact[1];
         contourlist[i][j].node1[1] = conlist[j].node1[1];
         contourlist[i][j].node2[1] = conlist[j].node2[1];
         contourlist[i][j].fact[2] = conlist[j].fact[2];
         contourlist[i][j].node1[2] = conlist[j].node1[2];
         contourlist[i][j].node2[2] = conlist[j].node2[2];
        }
     }
   FREE(conlist);

   isovol_on = 1;
   contour_on[i] = 1;
}


void setisovolclr(unsigned long pixno)
{
   XtVaSetValues(ivonbut,XmNbackground,pixno,NULL);
}
