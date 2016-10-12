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

static Widget objval1, objmatslidertxt, clipboxf, clipboxm,
              clipself, clipselm, contfmin, contfmax, isofldlab,
              objmatslider, objisosurf[MAXCONTOURS+MAXMATS], 
              objadd[MAXCONTOURS+MAXMATS], 
              objdel[MAXCONTOURS+MAXMATS],
              objisoval[MAXCONTOURS+MAXMATS],
              iscfclrbut, iscfclrlab, ismfclrbut, ismfclrlab;
static float isosurf_val;
static float matfrac = 0.5;
static int iscdrawfieldon, iscdrawfield, ismdrawfieldon, ismdrawfield;


void isosurfformfpop()
{
  int i;
  char str[55], str2[10];
  XmString string;

   for (i = 0; i < MAXCONTOURS; i++)
     {
      XmToggleButtonSetState(objisosurf[i],contour_on[i],FALSE);

      if (contour_on[i] == 1)
        {
         strcpy(str,fieldname[mconfield[i]]);
         sprintf(str2,"%5.3g",mconval[i]);
         strcat(str,str2);

         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objisoval[i],XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }         

   i = contourfieldactive;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(isofldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%5.3g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(contfmin,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%5.3g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(contfmax,XmNlabelString,string,NULL);
   XmStringFree(string);

   XmToggleButtonGadgetSetState(clipboxf,cliponbox,FALSE);
   XmToggleButtonGadgetSetState(clipself,cliponcellsel,FALSE);

   iscdrawfieldon = 0;
   iscdrawfield = contourfield_draw;
   XmToggleButtonSetState(iscfclrbut,FALSE,FALSE);
   if (iscdrawfield >= 0)
     {
      iscdrawfieldon = 1;
      XmToggleButtonSetState(iscfclrbut,TRUE,FALSE);
     }

   i = contourfield_draw;
   if (i < 0) i = 0;
   iscdrawfield = i;
   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(iscfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(isosurfformf);
}


void isosurfformmpop()
{
  int i;
  char str[10];
  XmString string;

   for (i = MAXCONTOURS; i < MAXCONTOURS+mmats; i++)
     {
      XmToggleButtonSetState(objisosurf[i],contour_on[i],FALSE);

      if (contour_on[i] == 1)
        {
         sprintf(str,"%5.3g",mconval[i]);
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objisoval[i],XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }         

   XmToggleButtonGadgetSetState(clipboxm,cliponbox,FALSE);
   XmToggleButtonGadgetSetState(clipselm,cliponcellsel,FALSE);

   ismdrawfieldon = 0;
   ismdrawfield = mcontourfield_draw;
   XmToggleButtonSetState(ismfclrbut,FALSE,FALSE);
   if (ismdrawfield > 0)
     {
      ismdrawfieldon = 1;
      XmToggleButtonSetState(ismfclrbut,TRUE,FALSE);
     }

   i = mcontourfield_draw;
   if (i < 0) i = 0;
   ismdrawfield = i;
   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ismfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(isosurfformm);
}


void closem(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(isosurfformm);
}


void closef(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(isosurfformf);
}


void clip_on_field_box(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cliponbox = (int)XmToggleButtonGadgetGetState(w);
   XmToggleButtonGadgetSetState(clipboxm,cliponbox,FALSE);
   XmToggleButtonGadgetSetState(clipboxf,cliponbox,FALSE);
}


void clip_on_cell_sel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cliponcellsel = (int)XmToggleButtonGadgetGetState(w);
   XmToggleButtonGadgetSetState(clipselm,cliponcellsel,FALSE);
   XmToggleButtonGadgetSetState(clipself,cliponcellsel,FALSE);
}


void add_isosurf(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, ctype;
  char str1[10], str3[55], *tmpstr;
  XmString string;
  double *value;
  double * readnodevalue(int ifld);
  void calcisosurf(int i, int ctype, float isoval, double *value);
  void isosurfgetfldclr();
  void misosurfgetfldclr();

   i = (long int)client_data;

   if (i < MAXCONTOURS)
     {
      tmpstr = XmTextFieldGetString(objval1);
      isosurf_val = atof(tmpstr);
      XtFree(tmpstr);
      strcpy(str3,fieldname[contourfieldactive]);
      sprintf(str1,"%5.3g",isosurf_val);
      strcat(str3,str1);
      value = readnodevalue(contourfieldactive);
      ctype = 0;
     }
   else
     {
      isosurf_val = matfrac;
      sprintf(str3,"%5.3g",matfrac);
      value = (double *) malloc((nnodes+1)*sizeof(double));
      for (j = 1; j < nnodes+1; j++)
         value[j] = (imat[j] == i-MAXCONTOURS+1) ? 1. : 0.;
      ctype = 1;
     }

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   calcisosurf(i,ctype,isosurf_val,value);

   if (contour_on[i] == 1)
     {
      mconfield[i] = contourfieldactive;
      if (i < MAXCONTOURS) mconval[i] = isosurf_val;
      else mconval[i] = matfrac;
      string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objisoval[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmToggleButtonSetState(objisosurf[i],TRUE,FALSE);
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
     }
   FREE(value);

   if (contour_on[i] == 1)
     {
      if (i < MAXCONTOURS)
        {
         if (iscdrawfieldon && contourdrawfield == NULL)
           {
            contourfield_draw = iscdrawfield;
            isosurfgetfldclr();
           }
        }
      else
        {
         if (ismdrawfieldon && mcontourdrawfield == NULL)
           {
            mcontourfield_draw = ismdrawfield;
            misosurfgetfldclr();
           }
        }
    }

   XUndefineCursor(dpy,topwin);

   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   fastdraw = 0;
   drawit();
}


void del_isosurf(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, chkcont, chkmcont;
  static char str3[20] = "     ";
  XmString string;
 
   i = (long int)client_data;
   contour_on[i] = 0;
   XmToggleButtonSetState(objisosurf[i],FALSE,FALSE);
   string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objisoval[i],XmNlabelString,string,NULL);
   XmStringFree(string);

   chkcont = 0;
   chkmcont = 0;
   for (j = 0; j < MAXCONTOURS; j++)
      if (contour_on[j]) chkcont = 1; 
   for (j = MAXCONTOURS; j < MAXCONTOURS+MAXMATS; j++)
      if (contour_on[j]) chkmcont = 1;
   if (chkcont == 0) FREE(contourdrawfield);
   if (chkmcont == 0) FREE(mcontourdrawfield);

   if (num_of_contour_polys[i] > 0)
     {
      num_of_contour_polys[i] = 0;
      FREE(contourlist[i]);

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}

void sel_desel_isosurf(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ibut;

   i = (long int)client_data;

   if (num_of_contour_polys[i] == 0)
      XmToggleButtonSetState(w,FALSE,FALSE);
   else
     {
      ibut = (int)XmToggleButtonGetState(w);
      contour_on[i] = (int)ibut;
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }

}


void isonfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = contourfieldactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDISOSURF, ifld);
}


void iscnfclrfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = iscdrawfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDISOSURFCLR, ifld);
}


void ismnfclrfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = ismdrawfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDISOSURFMCLR, ifld);
}


void setcfldclr(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ibut, chkcont, j;
  void isosurfgetfldclr();

   contourfield_draw = -1;
   iscdrawfieldon = 0;
   ibut = (int)XmToggleButtonGetState(w);
   if (ibut)
     {
      iscdrawfieldon = 1;
      contourfield_draw = iscdrawfield;
     }

   chkcont = 0;
   for (j = 0; j < MAXCONTOURS; j++)
      if (contour_on[j]) chkcont = 1; 

   if (chkcont)
     {
      isosurfgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void setmfldclr(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ibut, chkcont, j;
  void misosurfgetfldclr();

   mcontourfield_draw = -1;
   ismdrawfieldon = 0;
   ibut = (int)XmToggleButtonGetState(w);
   if (ibut)
     {
      ismdrawfieldon = 1;
      mcontourfield_draw = ismdrawfield;
     }

   chkcont = 0;
   for (j = MAXCONTOURS; j < MAXCONTOURS+MAXMATS; j++)
      if (contour_on[j]) chkcont = 1; 

   if (chkcont)
     {
      misosurfgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void matslider(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival;
  char str[10]; 
  XmString string;
 
   XmScaleGetValue(w,&ival);
   matfrac = (float)ival / 1000.;   
   sprintf(str,"%4.3f",matfrac);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objmatslidertxt,XmNlabelString,string,NULL);
   XmStringFree(string);
}

void preadcvalue(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(w);
   isosurf_val = atof(tmpstr);
   XtFree(tmpstr);
}

void makeisosurfform(parent)
 Widget parent;
{
  Widget isosclosef, matfraclab, lastwidget, convallab, 
         isosclosem, isosswinm, isosswinmform, isosmsep,
         isofldlab1, isofldbut, iscfclrlab1, iscfclrbut2,
         ismfclrlab1, ismfclrbut2;
  int i, matpixid;
  unsigned long matpix;
  char str[20];
  Arg wargs[20];
  XmString string, addstr, delstr, valstr; 

   /*  Make materials isosurface menu.  */

   string = XmStringCreate("Material Isosurfaces",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   isosurfformm = XmCreateFormDialog(parent,"Contourformm",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   isosclosem = XtVaCreateManagedWidget("Contclosem",
                xmPushButtonGadgetClass,
                isosurfformm,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(isosclosem,XmNactivateCallback,closem,NULL);
 
   string = XmStringCreate("Clip on Field Subset",
            XmSTRING_DEFAULT_CHARSET);
   clipboxm = XtVaCreateManagedWidget("Clipboxm",
              xmToggleButtonGadgetClass,
              isosurfformm,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, isosclosem,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(clipboxm,XmNvalueChangedCallback,
                    clip_on_field_box,NULL);
 
   string = XmStringCreate("Clip on Cell Selection",
            XmSTRING_DEFAULT_CHARSET);
   clipselm = XtVaCreateManagedWidget("Clipselm",
            xmToggleButtonGadgetClass,
            isosurfformm,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, clipboxm,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(clipselm,XmNvalueChangedCallback,
                    clip_on_cell_sel,NULL);
 
   objmatslider = XtVaCreateManagedWidget("Objmatslider",
                  xmScaleWidgetClass,
                  isosurfformm,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, clipselm,
                  XmNtopOffset, 20,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNminimum, 1,
                  XmNmaximum, 999,
                  XmNorientation, XmHORIZONTAL,
                  XmNprocessingDirection, XmMAX_ON_RIGHT,
                  XmNvalue, 500,
                  XmNscaleWidth, 200,
                  XmNscaleMultiple, 1,
                  NULL);
      XtAddCallback(objmatslider,XmNvalueChangedCallback,
                    matslider,NULL);
      XtAddCallback(objmatslider,XmNdragCallback,matslider,NULL);
   string = XmStringCreate("Mat-Frac",XmSTRING_DEFAULT_CHARSET);
   matfraclab = XtVaCreateManagedWidget("Matfraclab",
                xmLabelWidgetClass,
                isosurfformm,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, objmatslider,
                XmNtopOffset, 5,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   string = XmStringCreate(".500",XmSTRING_DEFAULT_CHARSET);
   objmatslidertxt = XtVaCreateManagedWidget("Objmatslidertxt",
                     xmLabelWidgetClass,
                     isosurfformm,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, objmatslider,
                     XmNtopOffset, 5,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, matfraclab,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);

   addstr = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   delstr = XmStringCreate("Del",XmSTRING_DEFAULT_CHARSET);
   valstr = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, objmatslidertxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 300);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   isosswinm = XmCreateScrolledWindow(isosurfformm,"Contswinm",wargs,i);
   XtManageChild(isosswinm);

   i=0; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   isosswinmform = XmCreateForm(isosswinm,"Contswinmform",wargs,i);

   isosmsep = XtVaCreateManagedWidget("Contmsep",
              xmSeparatorGadgetClass,
              isosswinmform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNseparatorType, XmNO_LINE,
              NULL);

   lastwidget = isosmsep;
   for (i = MAXCONTOURS; i < MAXCONTOURS+MAXMATS /*MAXCONTOURS+mmats should also work according to Frank Ortega*/; i++)
     {
      sprintf(str,"Objadd%d",i);
      objadd[i] = XtVaCreateManagedWidget(str,
                  xmPushButtonGadgetClass,
                  isosswinmform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, lastwidget,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, addstr,
                  XmNmarginHeight, 0,
                  NULL);
      XtAddCallback(objadd[i],XmNactivateCallback,add_isosurf,
                    (caddr_t) i);

      sprintf(str,"Objdel%d",i);
      objdel[i] = XtVaCreateManagedWidget(str,
                  xmPushButtonGadgetClass,
                  isosswinmform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, lastwidget,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, objadd[i],
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, delstr,
                  XmNmarginHeight, 0,
                  NULL);
      XtAddCallback(objdel[i],XmNactivateCallback,del_isosurf,
                    (caddr_t) i);

      string = XmStringCreate(mmatnames[i-MAXCONTOURS],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Objisosurf%d",i);
      objisosurf[i] = XtVaCreateManagedWidget(str,
                      xmToggleButtonWidgetClass,
                      isosswinmform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, lastwidget,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, objdel[i],
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      XmNspacing, 0,
                      XmNselectColor, yellowpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNvisibleWhenOff, FALSE,
                      NULL);
         XmStringFree(string);
         XtAddCallback(objisosurf[i],XmNvalueChangedCallback,
                       sel_desel_isosurf,(caddr_t) i);

      sprintf(str,"Objconval%d",i);
      objisoval[i] = XtVaCreateManagedWidget(str,
                     xmLabelWidgetClass,
                     isosswinmform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, lastwidget,
                     XmNtopOffset, 1,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, objisosurf[i],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, valstr,
                     NULL);

      if (isomats_active[i-MAXCONTOURS] > 0)
        {
         lastwidget = objadd[i];
        }
      else 
        {
         XtUnmanageChild(objadd[i]);
         XtUnmanageChild(objdel[i]);
         XtUnmanageChild(objisosurf[i]);
         XtUnmanageChild(objisoval[i]);
        }
     }
   XtManageChild(isosswinmform);

   string = XmStringLtoRCreate("Color Surface with\n Node Field ",
                               XmSTRING_DEFAULT_CHARSET);
   ismfclrbut = XtVaCreateManagedWidget("Ismfclrbut",
                xmToggleButtonWidgetClass,
                isosurfformm,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, isosswinm,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNselectColor, yellowpix,
                XmNspacing, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(ismfclrbut,XmNvalueChangedCallback,setmfldclr,
                    NULL);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   ismfclrlab1 = XtVaCreateManagedWidget("Ismfclrlab1",
                 xmLabelWidgetClass,
                 isosurfformm,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ismfclrbut,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 30,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   ismfclrlab = XtVaCreateManagedWidget("Ismfclrlab",
                xmLabelWidgetClass,
                isosurfformm,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ismfclrbut,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, ismfclrlab1,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ismfclrbut2 = XtVaCreateManagedWidget("Ismfclrbut2",
                 xmPushButtonGadgetClass,
                 isosurfformm,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ismfclrlab1,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 30,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ismfclrbut2,XmNactivateCallback,ismnfclrfld,
                    NULL);


   /*  Make field isosurface menu.  */

   string = XmStringCreate("Field Isosurfaces",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   isosurfformf = XmCreateFormDialog(parent,"Contourformf",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   isosclosef = XtVaCreateManagedWidget("Contclosef",
                xmPushButtonGadgetClass,
                isosurfformf,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(isosclosef,XmNactivateCallback,closef,NULL);
 
   string = XmStringCreate("Clip on Field Subset",
            XmSTRING_DEFAULT_CHARSET);
   clipboxf = XtVaCreateManagedWidget("Clipboxf",
              xmToggleButtonGadgetClass,
              isosurfformf,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, isosclosef,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(clipboxf,XmNvalueChangedCallback,
                    clip_on_field_box,NULL);

   string = XmStringCreate("Clip on Cell Selection",
            XmSTRING_DEFAULT_CHARSET);
   clipself = XtVaCreateManagedWidget("Clipself",
              xmToggleButtonGadgetClass,
              isosurfformf,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clipboxf,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(clipself,XmNvalueChangedCallback,
                 clip_on_cell_sel,NULL);

   XmStringFree(valstr);
   valstr = XmStringCreate("              ",
                           XmSTRING_DEFAULT_CHARSET);

   lastwidget = clipself;
   matpixid = 0;
   for (i = 0; i < MAXCONTOURS; i++)
     {
      sprintf(str,"Objadd%d",i);
      objadd[i] = XtVaCreateManagedWidget(str,
                  xmPushButtonGadgetClass,
                  isosurfformf,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, lastwidget,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, addstr,
                  XmNmarginHeight, 0,
                  NULL);
      XtAddCallback(objadd[i],XmNactivateCallback,add_isosurf,
                    (caddr_t) i);

      sprintf(str,"Objdel%d",i);
      objdel[i] = XtVaCreateManagedWidget(str,
                  xmPushButtonGadgetClass,
                  isosurfformf,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, lastwidget,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, objadd[i],
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, delstr,
                  XmNmarginHeight, 0,
                  NULL);
      XtAddCallback(objdel[i],XmNactivateCallback,del_isosurf,
                    (caddr_t) i);

      matpixid++;
      matpix = mat_pixels[matpixid];
      sprintf(str,"%2d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Objisosurf%d",i);
      objisosurf[i] = XtVaCreateManagedWidget(str,
                      xmToggleButtonWidgetClass,
                      isosurfformf,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, lastwidget,
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, objdel[i],
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      XmNspacing, 0,
                      XmNselectColor, yellowpix,
                      XmNbackground, matpix,
                      XmNforeground, blackpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNvisibleWhenOff, FALSE,
                      NULL);
         XmStringFree(string);
         XtAddCallback(objisosurf[i],XmNvalueChangedCallback,
                       sel_desel_isosurf,(caddr_t) i);

      sprintf(str,"Objconval%d",i);
      objisoval[i] = XtVaCreateManagedWidget(str,
                     xmLabelWidgetClass,
                     isosurfformf,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, lastwidget,
                     XmNtopOffset, 1,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, objisosurf[i],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, valstr,
                     NULL);

      lastwidget = objadd[i];
     }
   XmStringFree(valstr);
   XmStringFree(addstr);
   XmStringFree(delstr);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   isofldlab1 = XtVaCreateManagedWidget("Isofldlab1",
                xmLabelWidgetClass,
                isosurfformf,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, lastwidget,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   isofldlab = XtVaCreateManagedWidget("Isofldlab",
               xmLabelWidgetClass,
               isosurfformf,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, lastwidget,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, isofldlab1,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
   contfmin = XtVaCreateManagedWidget("Contfmin",
              xmLabelWidgetClass,
              isosurfformf,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, isofldlab1,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   objval1 = XtVaCreateManagedWidget("Objval1",
             xmTextFieldWidgetClass,
             isosurfformf,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, isofldlab1,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, contfmin,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);
      XtAddCallback(objval1,XmNactivateCallback,preadcvalue,NULL);
   string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
   contfmax = XtVaCreateManagedWidget("Contfmax",
              xmLabelWidgetClass,
              isosurfformf,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, isofldlab1,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, objval1,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string); 
   string = XmStringCreate("Contour Value",XmSTRING_DEFAULT_CHARSET);
   convallab = XtVaCreateManagedWidget("Convallab",
               xmLabelWidgetClass,
               isosurfformf,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, objval1,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, contfmin,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   isofldbut = XtVaCreateManagedWidget("Isofldbut",
               xmPushButtonGadgetClass,
               isosurfformf,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, convallab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNleftOffset, 20,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(isofldbut,XmNactivateCallback,isonfldfld,
                    NULL);

   string = XmStringLtoRCreate("Color Surface with\n Node Field ",
                               XmSTRING_DEFAULT_CHARSET);
   iscfclrbut = XtVaCreateManagedWidget("Iscfclrbut",
                xmToggleButtonWidgetClass,
                isosurfformf,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, isofldbut,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNselectColor, yellowpix,
                XmNspacing, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(iscfclrbut,XmNvalueChangedCallback,setcfldclr,
                    NULL);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   iscfclrlab1 = XtVaCreateManagedWidget("Iscfclrlab1",
                 xmLabelWidgetClass,
                 isosurfformf,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, iscfclrbut,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 30,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   iscfclrlab = XtVaCreateManagedWidget("Iscfclrlab",
                xmLabelWidgetClass,
                isosurfformf,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, iscfclrbut,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, iscfclrlab1,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   iscfclrbut2 = XtVaCreateManagedWidget("Iscfclrbut2",
                 xmPushButtonGadgetClass,
                 isosurfformf,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, iscfclrlab1,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 30,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(iscfclrbut2,XmNactivateCallback,iscnfclrfld,
                    NULL);
}


void setisoisoclr(int iso_no, unsigned long pixno)
{
   XtVaSetValues(objisosurf[iso_no],XmNbackground,pixno,NULL);
}


void isosurfgetnfield(int arg)
{
  int i;
  char str[20];
  double slmin, slmax;
  XmString string;
 
   i = arg;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(isofldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   contourfieldactive = i;
   slmin = fieldmin[i];
   slmax = fieldmax[i];
   slmax = (slmax <= slmin) ? 1.00001*slmin : slmax;
   sprintf(str,"%5.3g",slmin);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(contfmin,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%5.3g",slmax);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(contfmax,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void isosurfgetfldclr()
{
   if (contourfield_draw >= 0)
     {
      if (contourdrawfield == NULL)
        {
         contourdrawfield = (double *)malloc((nnodes+1)*sizeof(double));
         if (contourdrawfield == NULL) memerr();
        }
      fillnodevalue(contourfield_draw,contourdrawfield);
     }
}


void isosurfclrgetnfield(int arg)
{
  int i, j, chkcont;
  XmString string;
 
   i = arg;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(iscfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   chkcont = 0;
   for (j = 0; j < MAXCONTOURS; j++)
      if (contour_on[j]) chkcont = 1; 

   iscdrawfield = i;

   if (iscdrawfieldon && chkcont)
     {
      contourfield_draw = iscdrawfield;
      isosurfgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void misosurfgetfldclr()
{
   if (mcontourfield_draw >= 0)
     {
      if (mcontourdrawfield == NULL)
        {
         mcontourdrawfield=(double *)malloc((nnodes+1)*sizeof(double));
         if (mcontourdrawfield == NULL) memerr();
        }
      fillnodevalue(mcontourfield_draw,mcontourdrawfield);
     }
}


void misosurfclrgetnfield(int arg)
{
  int i, j, chkcont;
  XmString string;
 
   i = arg;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ismfclrlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   chkcont = 0;
   for (j = MAXCONTOURS; j < MAXCONTOURS+MAXMATS; j++)
      if (contour_on[j]) chkcont = 1; 

   ismdrawfield = i;

   if (ismdrawfieldon && chkcont)
     {
      mcontourfield_draw = ismdrawfield;
      misosurfgetfldclr();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void calcisosurf(int i, int ctype, float isoval, double *value)
{
  int npolys, j;

   npolys = 0;

   makeisosurf(isoval,ctype,value,&npolys);

   /*  If a contour already exists, free memory.  */
   if (num_of_contour_polys[i] > 0)
     {
      contour_on[i] = 0;
      num_of_contour_polys[i] = 0;
      FREE(contourlist[i]);
     }

   /*  Fill contour arrays from makeisosurf arrays.  */
   if (npolys > 0)
     {
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

   contour_on[i] = 1;
}
