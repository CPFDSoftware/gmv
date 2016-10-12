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
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#define MAXFLDOPERS 25
#define MAXVAL 9.99999e32

static Widget cfcsel[MAXFLDCALC], cfldcalcform2, cfcname,
              cfcerror, cfcconst, cfcoper[MAXFLDOPERS],
              cfcfldxlab, cfcfldylab;
static int currcfc = 0, cfldcalc_fldx, cfldcalc_fldy, cfldcalc_oper;
static char *copername[MAXFLDOPERS] = {
                      "e**X        ", "10**X       ", "ln(X)       ",
                      "log(X)      ", "abs(X)      ", "sqrt(X)     ",
                      "X**2        ", "X**C        ", "X*C         ",
                      "1/X         ", "X/C         ", "X+C         ",
                      "X-C         ", "X+Y         ", "X-Y         ", 
                      "X*Y         ", "X/Y         ", "X**Y        " ,
                      "sin(X)      ", "cos(x)      ", "tan(X)      ",
                      "asin(X)     ", "acos(x)     ", "atan(X)     ",
                      "C*round(X)+Y" };


void cfldcalcformpop()
{
  int i;
  char str4[40];
  XmString string;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str4,"%s",cfieldcalc[i].name);   
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfcsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(cfldcalcform);
}


void cfldcalcformpop2()
{
  char str[40];
  XmString string;

   sprintf(str,"%s",cfieldcalc[currcfc].name);   
   XtVaSetValues(cfcname,XmNvalue,str,NULL);

   sprintf(str,"%5.3g",cfieldcalc[currcfc].cval);
   XtVaSetValues(cfcconst,XmNvalue,str,NULL);

   cfldcalc_fldx = cfieldcalc[currcfc].xfldno;
   if (cfldcalc_fldx == -1) cfldcalc_fldx = 0;
   string = XmStringCreate(cfieldname[cfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cfldcalc_fldy = cfieldcalc[currcfc].yfldno;
   if (cfldcalc_fldy == -1) cfldcalc_fldy = 0;
   string = XmStringCreate(cfieldname[cfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cfldcalc_oper = cfieldcalc[currcfc].oper;
   if (cfldcalc_oper == -1) cfldcalc_oper = 0;
   XmToggleButtonGadgetSetState(cfcoper[cfldcalc_oper],TRUE,TRUE);

   MyManageChild(cfldcalcform2);
}


void cfldcalc_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) currcfc = i;

   cfldcalcformpop2();
}


void cfldcalc_select_oper(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) cfldcalc_oper = i;
}


void cfldcalcxfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = cfldcalc_fldx;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDFLDCALCX, ifld);
}


void cfldcalcyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = cfldcalc_fldy;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDFLDCALCY, ifld);
}


void cfcdelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[40];
  XmString string;
  int fldno, i, redraw;
  double *value;
  void writecellfldloc(int ifld, double *value);

   sprintf(str,"FldCalc%d",currcfc+1);
   XtVaSetValues(cfcname,XmNvalue,str,NULL);

   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcsel[currcfc],XmNlabelString,string,NULL);
   XmStringFree(string);

   cfieldcalc[currcfc].xfldno = -1;
   cfieldcalc[currcfc].yfldno = -1;
   cfieldcalc[currcfc].oper = -1;
   cfieldcalc[currcfc].cval = 0.;
   strcpy(cfieldcalc[currcfc].name,str);
  
   /*  Reset field calc field to 0.  */
   value=(double *)malloc(ncells*sizeof(double));
   if (value == NULL) memerr();
   for (i = 0; i < ncells; i++) value[i] = 0;
   fldno = cfldcalcbeg + currcfc;   
   writecellfldloc(fldno,value);

   globfieldcmin[fldno] = 0.;
   globfieldcmax[fldno] = 0.;
   cfieldmin[fldno] = 0.;
   cfieldmax[fldno] = 0.;
   cfld_nice_min[fldno] = 0.;
   cfld_nice_max[fldno] = 0.;
   strcpy(cfieldname[fldno],str);

   /*  Reset field button name for all menus.  */
   setcfieldname(fldno, cfieldname[fldno]);

   /*  If this field is selected for drawing as a   */
   /*  cell face, reset drawing arrays and redraw.   */
   redraw = 0;
   if (cellfieldactive == fldno + numvars && cellsflag)
     {
      redraw = 1;
      for (i = 0; i < ncells; i++)
         cellfield[i] = value[i];
      setcellcfldlab(fldno);
     }
   if (gridfieldactive == fldno + numvars)
     {
      redraw = 1;
      for (i = 0; i < ncells; i++)
         gridfield[i] = value[i];
      setgridcfldlab(fldno);
     }
   free(value);

   /*  Set cell field button sensitivity off.  */
   setcfldsensitive(fldno,FALSE);

   fastdraw = 0;
   if (redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void cclosefldcalc(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0) XtUnmanageChild(cfldcalcform);
   if (arg == 1)
     {
      XtUnmanageChild(cfieldform);
      XtUnmanageChild(cfldcalcform2);
     }
}


void doitcfldcalc(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float cval;
  double *value;
  int i, ierr, fldno, redraw;
  char *tmpstr, fldname[33];
  static char str[55] = 
           "Warning, invalid data.";
  static char str2[55] = "   ";
  static char str3[55] = 
           "Warning, invalid data.";
  static char str4[55] =
           "Error, new field no. out of range. ";
  XmString string;
  int docfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  double * readcellvalue(int ifld);

   /*  Read the new field name and constant.  */
   tmpstr = XmTextFieldGetString(cfcname);
   strcpy(fldname,tmpstr);
   XtFree(tmpstr);

   tmpstr = XmTextFieldGetString(cfcconst);
   cval = atof(tmpstr); 
   XtFree(tmpstr);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Process the new field.  */
   fldno = cfldcalcbeg + currcfc;   
   if (fldno < cnumvars)
     {

      /*  Calculate new field.  */
      ierr = docfldcalc(cfldcalc_oper,cval,cfldcalc_fldx,
                        cfldcalc_fldy, fldno);

      /*  Fill warning messages, if any.  */
      if (ierr > 0)
        {
         if (ierr == 1)
            string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         else
            string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(cfcerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      /*  Fill cfieldcalc.  */
      cfieldcalc[currcfc].xfldno = cfldcalc_fldx;
      cfieldcalc[currcfc].yfldno = cfldcalc_fldy;
      cfieldcalc[currcfc].oper = cfldcalc_oper;
      cfieldcalc[currcfc].cval = cval;
      strcpy(cfieldcalc[currcfc].name,fldname);
      strcpy(cfieldname[fldno],fldname);

      string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfcsel[currcfc],XmNlabelString,string,NULL);
      XmStringFree(string);

      /*  Reset field button name for all menus.  */
      setcfieldname(fldno, cfieldname[fldno]);

      /*  Set cell field button sensitivity on.  */
      setcfldsensitive(fldno,TRUE);
     }

   else
     {
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfcerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   /*  If this field is selected for drawing as a node  */
   /*  or cell face, reset drawing arrays and redraw.   */
   redraw = 0;
   if (cellfieldactive == (fldno+numvars) || 
       gridfieldactive == (fldno+numvars))
     {
      value = readcellvalue(fldno);

      if (cellfieldactive == fldno + numvars && cellsflag)
        {
         redraw = 1;
         for (i = 0; i < ncells; i++)
            cellfield[i] = value[i];
         setcellcfldlab(fldno);
        }
      if (gridfieldactive == fldno + numvars)
        {
         redraw = 1;
         for (i = 0; i < ncells; i++)
            gridfield[i] = value[i];
         setgridcfldlab(fldno);
        }

      free(value);
     }

   XmUpdateDisplay(cfcerror);
   XUndefineCursor(dpy,topwin);
   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   fastdraw = 0;
   if (redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void makecfldcalcform(parent)
 Widget parent;
{
  Widget cfcclose, cfcsellab, cfcselbox, cfcclab, cfcnamelab,
         cfcoplab, cfcswino, cfcboxo, cfcfllabx, cfcflbutx,
         cfcfllaby, cfcflbuty, cfcadd, cfcdel, cfcclose2;
  int i;
  Arg wargs[20];
  char str[10];
  XmString string;

   string = XmStringCreate("Field Calc. - Cells",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cfldcalcform = XmCreateFormDialog(parent,"Cfldcalcform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cfcclose = XtVaCreateManagedWidget("Cfcclose",
               xmPushButtonGadgetClass,
               cfldcalcform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfcclose,XmNactivateCallback,cclosefldcalc,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Select Cell Field Calc",
                           XmSTRING_DEFAULT_CHARSET);
   cfcsellab = XtVaCreateManagedWidget("Cfcsellab",
              xmLabelGadgetClass,
              cfldcalcform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfcclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cfcsellab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cfcselbox = XmCreateRadioBox(cfldcalcform,"Cfcselbox",wargs,i);
   XtManageChild(cfcselbox);

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str,"FldCalc%d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Fcsel%d",i);
      cfcsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 cfcselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(cfcsel[i],XmNvalueChangedCallback,
                       cfldcalc_select,
                       (caddr_t) i);
     }


   /*  Create field calc. build form.  */

   string = XmStringCreate("Selected Cell Field Calc.",
                            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cfldcalcform2 = XmCreateFormDialog(cfldcalcform,"Cfldcalcform2",wargs,i);
      XmStringFree(string);

   cfcname = XtVaCreateManagedWidget("Cfcname",
             xmTextFieldWidgetClass,
             cfldcalcform2,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 32,
             XmNcolumns, 16,
             XmNvalue, "",
             NULL);

   string = XmStringCreate("New Field Name",XmSTRING_DEFAULT_CHARSET);
   cfcnamelab = XtVaCreateManagedWidget("Cfcnamelab",
                xmLabelGadgetClass,
                cfldcalcform2,
                XmNtopAttachment, XmATTACH_FORM,
                XmNtopOffset, 5,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cfcname,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("C=",XmSTRING_DEFAULT_CHARSET);
   cfcclab = XtVaCreateManagedWidget("Cfcclab",
             xmLabelGadgetClass,
             cfldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cfcname,
             XmNtopOffset, 15,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   cfcconst = XtVaCreateManagedWidget("Cfcconst",
              xmTextFieldWidgetClass,
              cfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfcname,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cfcclab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 10,
              XmNcolumns, 10,
              XmNvalue, "",
              NULL);

   string = XmStringCreate("Operators:",XmSTRING_DEFAULT_CHARSET);
   cfcoplab = XtVaCreateManagedWidget("Cfcoplab",
              xmLabelGadgetClass,
              cfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfcconst,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cfcoplab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   cfcswino = XmCreateScrolledWindow(cfldcalcform2,"Cfcswino",wargs,i);
   XtManageChild(cfcswino);

   i = 0;
   cfcboxo = XmCreateRadioBox(cfcswino,"Cfcboxo",wargs,i);
   XtManageChild(cfcboxo);

   for (i = 0; i < MAXFLDOPERS; i++)
     {
      string = XmStringCreate(copername[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Cfcoper%d",i);
      cfcoper[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   cfcboxo,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   NULL);
         XmStringFree(string);
         XtAddCallback(cfcoper[i],XmNvalueChangedCallback,
                       cfldcalc_select_oper,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(cfcoper[i],TRUE,FALSE);
     }

   string = XmStringCreate("X Field:",XmSTRING_DEFAULT_CHARSET);
   cfcfllabx = XtVaCreateManagedWidget("Cfcfllabx",
               xmLabelGadgetClass,
               cfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfcswino,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   cfcfldxlab = XtVaCreateManagedWidget("Cfcfldxlab",
                xmLabelGadgetClass,
                cfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfcfllabx,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cfcflbutx = XtVaCreateManagedWidget("Cfcflbutx",
               xmPushButtonGadgetClass,
               cfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfcfldxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfcflbutx,XmNactivateCallback,cfldcalcxfld,NULL);

   string = XmStringCreate("Y Field:",XmSTRING_DEFAULT_CHARSET);
   cfcfllaby = XtVaCreateManagedWidget("Cfcfllaby",
               xmLabelGadgetClass,
               cfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfcflbutx,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   cfcfldylab = XtVaCreateManagedWidget("Cfcfldylab",
                xmLabelGadgetClass,
                cfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfcfllaby,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cfcflbuty = XtVaCreateManagedWidget("Cfcflbuty",
               xmPushButtonGadgetClass,
               cfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfcfldylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfcflbuty,XmNactivateCallback,cfldcalcyfld,NULL);

   string = XmStringCreate("  ",XmSTRING_DEFAULT_CHARSET);
   cfcerror = XtVaCreateManagedWidget("Cfcerror",
              xmLabelGadgetClass,
              cfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cfcflbuty,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   cfcadd = XtVaCreateManagedWidget("Cfcadd",
            xmPushButtonGadgetClass,
            cfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(cfcadd,XmNactivateCallback,doitcfldcalc,
                    (caddr_t) 2);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   cfcdel = XtVaCreateManagedWidget("Cfcdel",
            xmPushButtonGadgetClass,
            cfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, cfcadd,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(cfcdel,XmNactivateCallback,cfcdelete,NULL);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cfcclose2 = XtVaCreateManagedWidget("Cfcclose2",
               xmPushButtonGadgetClass,
               cfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfcerror,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cfcdel,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfcclose2,XmNactivateCallback,cclosefldcalc,
                    (caddr_t) 1);

}


void applycfldcalc(short oper, float cval, double *xvalue,
                   double *yvalue,int *ierror, double *vmin,
                   double *vmax)
{
  int i, ierr, isgn, itmp;
  double val1, val2;
  float f;

   /*  This routine performs a unary operation on the data  */
   /*  in value and determines the min. and max. value      */
   ierr = 0;
   
   switch(oper)
     {

      case(0):  /*  e**X  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = exp(xvalue[i]);
           }
         break;

      case(1):  /*  10**X  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = pow(10., xvalue[i]);
           }
         break;

      case(2):  /*  ln(X)  */
         for (i = 0; i < ncells; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(3):  /*  log(X)  */
         for (i = 0; i < ncells; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log10(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(4):  /*  abs(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = fabs(xvalue[i]);
           }
         break;

      case(5):  /*  sqrt(X)  */
         for (i = 0; i < ncells; i++)
           {
            if (xvalue[i] >= 0.)
               xvalue[i] = sqrt(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(6):  /*  X**2  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] * xvalue[i];
           }
         break;

      case(7):  /*  X**C  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < ncells; i++)
           {
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = pow(xvalue[i], (double) cval);
           }
         break;

      case(8):  /*  X*C  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] * cval;
           }
         break;

      case(9):  /*  1/X  */
         for (i = 0; i < ncells; i++)
           {
            if (xvalue[i] != 0.)
               xvalue[i] = 1. / xvalue[i];
            else ierr = 1;
           }
         break;

      case(10):  /*  X/C  */
         for (i = 0; i < ncells; i++)
           {
            if (cval != 0.)
               xvalue[i] = xvalue[i] / cval;
            else ierr = 1;
           }
         break;

      case(11):  /*  X+C  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] + cval;
           }
         break;

      case(12):  /*  X-C  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] - cval;
           }
         break;

      case(13):  /*  X+Y  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] + yvalue[i];
           }
         break;

      case(14):  /*  X-Y  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] - yvalue[i];
           }
         break;

      case(15):  /*  X*Y  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = xvalue[i] * yvalue[i];
           }
         break;

      case(16):  /*  X/Y  */
         for (i = 0; i < ncells; i++)
           {
            if (yvalue[i] != 0.)
               xvalue[i] = xvalue[i] / yvalue[i];
            else ierr = 1;
           }
         break;

      case(17):  /*  X**Y  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < ncells; i++)
           {
            f = yvalue[i] - (float) floor(yvalue[i]);
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = 
                 pow(xvalue[i], yvalue[i]);
           }
         break;

      case(18):  /*  sin(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = sin(xvalue[i]);
           }
         break;

      case(19):  /*  cos(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = cos(xvalue[i]);
           }
         break;

      case(20):  /*  tan(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = tan(xvalue[i]);
           }
         break;

      case(21):  /*  asin(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = asin(xvalue[i]);
           }
         break;

      case(22):  /*  acos(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = acos(xvalue[i]);
           }
         break;

      case(23):  /*  atan(X)  */
         for (i = 0; i < ncells; i++)
           {
            xvalue[i] = atan(xvalue[i]);
           }
         break;

      /* Added to implement indexing into a colormap */
      /* Note that the current implementation does not work the */
      /* same way as the round() math library function in C99, */
      /* and the precision of the integer may cause truncation */ 
      case(24):  /*  C*round(X)+Y */
         for (i = 0; i < ncells; i++)
           {
            isgn = (xvalue[i] < 0.) ? -1 : 1;
            itmp = (isgn * xvalue[i] + 0.5);
            xvalue[i] = cval * isgn  * itmp + yvalue[i];
           }
         break;
    }

   /*  Determine min. & max.  */
   val1 = MAXVAL;  val2 = -MAXVAL;
   for (i = 0; i < ncells; i++)
     {
      if (cellmats[i] > 0)
        {
         val1 = (val1 < xvalue[i]) ? val1 : xvalue[i];
         val2 = (val2 > xvalue[i]) ? val2 : xvalue[i];
        }
     }

   if (val2 <= val1)
     {
      f = fabs((double)val1);
      if (f == 0.) f = 1.;
      val2 = val1 + f*0.00001;
     }

   *vmin = val1;  *vmax = val2;
   *ierror = ierr;
}


int docfldcalc(short oper, float cval, short xfld, short yfld, 
               int fldno)
{
  int ierr, i;
  double *xvalue, *yvalue, vmin, vmax, *field;
  double * readcellvalue(int ifld);
  void writecellfldloc(int ifld, double *value);

   /*  If first field calc., add to cell field file.  */
   if (cfldcalc_has_data == 0)
     {
      field=(double *)malloc((ncells)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (i = 0; i < ncells; i++) field[i] = 0;

      for (i = 0; i < MAXFLDCALC; i++)
        {
         if (cnumvars < MAXFIELDS)
            writecellfld(field);
        }
      free(field);
      cfldcalc_has_data = 1;
     }
  
   /*  Get values for the selected field.  */
   xvalue = readcellvalue(xfld);
   if (yfld > -1)
      yvalue = readcellvalue(yfld);

   /*  Apply the operator.  */
   applycfldcalc(oper,cval,xvalue,yvalue,&ierr,&vmin,&vmax);

   /*  Write out the new field data,  */
   /*  fill the field min. and max.   */
   writecellfldloc(fldno,xvalue);

   globfieldcmin[fldno] = vmin;
   globfieldcmax[fldno] = vmax;
   cfieldmin[fldno] = vmin;
   cfieldmax[fldno] = vmax;
   cfld_nice_min[fldno] = vmin;
   cfld_nice_max[fldno] = vmax;

   free(xvalue);
   if (yfld > -1) free(yvalue);

   return ierr;
}



void fcalcxgetcfield(int i)
{
  XmString string;

   cfldcalc_fldx = i;
   string = XmStringCreate(cfieldname[cfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void fcalcygetcfield(int i)
{
  XmString string;

   cfldcalc_fldy = i;
   string = XmStringCreate(cfieldname[cfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);
}
