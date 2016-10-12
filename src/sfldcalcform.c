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

static Widget sfcsel[MAXFLDCALC], sfldcalcform2, sfcname,
              sfcerror, sfcconst, sfcoper[MAXFLDOPERS],
              sfcfldxlab, sfcfldylab;
static int currsfc = 0, sfldcalc_fldx, sfldcalc_fldy, sfldcalc_oper;
static char *sopername[MAXFLDOPERS] = {
                     "e**X        ", "10**X       ", "ln(X)       ",
                     "log(X)      ", "abs(X)      ", "sqrt(X)     ",
                     "X**2        ", "X**C        ", "X*C         ",
                     "1/X         ", "X/C         ", "X+C         ",
                     "X-C         ", "X+Y         ", "X-Y         ", 
                     "X*Y         ", "X/Y         ", "X**Y        ",
                     "sin(X)      ", "cos(x)      ", "tan(X)      ",
                     "asin(X)     ", "acos(x)     ", "atan(X)     ",
                     "C*round(X)+Y", };

void sfldcalcformpop()
{
  int i;
  char str4[40];
  XmString string;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str4,"%s",sfieldcalc[i].name);   
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sfcsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(sfldcalcform);
}


void sfldcalcformpop2()
{
  char str[40];
  XmString string;

   sprintf(str,"%s",sfieldcalc[currsfc].name);   
   XtVaSetValues(sfcname,XmNvalue,str,NULL);

   sprintf(str,"%5.3g",sfieldcalc[currsfc].cval);
   XtVaSetValues(sfcconst,XmNvalue,str,NULL);

   sfldcalc_fldx = sfieldcalc[currsfc].xfldno;
   if (sfldcalc_fldx == -1) sfldcalc_fldx = 0;
   string = XmStringCreate(sfieldname[sfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sfldcalc_fldy = sfieldcalc[currsfc].yfldno;
   if (sfldcalc_fldy == -1) sfldcalc_fldy = 0;
   string = XmStringCreate(sfieldname[sfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sfldcalc_oper = sfieldcalc[currsfc].oper;
   if (sfldcalc_oper == -1) sfldcalc_oper = 0;
   XmToggleButtonGadgetSetState(sfcoper[sfldcalc_oper],TRUE,TRUE);

   MyManageChild(sfldcalcform2);
}


void sfldcalc_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) currsfc = i;

   sfldcalcformpop2();
}


void sfldcalc_select_oper(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) sfldcalc_oper = i;
}


void sfldcalcxfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = sfldcalc_fldx;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   sfieldformpop(SFLDFLDCALCX, ifld);
}


void sfldcalcyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = sfldcalc_fldy;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   sfieldformpop(SFLDFLDCALCY, ifld);
}


void sfcdelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[40];
  XmString string;
  int fldno, i, redraw;
  double *value;
  void writesurffldloc(int ifld, double *value);

   sprintf(str,"FldCalc%d",currsfc+1);
   XtVaSetValues(sfcname,XmNvalue,str,NULL);

   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcsel[currsfc],XmNlabelString,string,NULL);
   XmStringFree(string);

   sfieldcalc[currsfc].xfldno = -1;
   sfieldcalc[currsfc].yfldno = -1;
   sfieldcalc[currsfc].oper = -1;
   sfieldcalc[currsfc].cval = 0.;
   strcpy(sfieldcalc[currsfc].name,str);
  
   /*  Reset field calc field to 0.  */
   value=(double *)malloc(nsurf*sizeof(double));
   if (value == NULL) memerr();
   for (i = 0; i < nsurf; i++) value[i] = 0;
   fldno = sfldcalcbeg + currsfc;   
   writesurffldloc(fldno,value);

   globfieldsmin[fldno] = 0.;
   globfieldsmax[fldno] = 0.;
   sfieldmin[fldno] = 0.;
   sfieldmax[fldno] = 0.;
   sfld_nice_min[fldno] = 0.;
   sfld_nice_max[fldno] = 0.;
   strcpy(sfieldname[fldno],str);

   /*  Reset field button name for all menus.  */
   setsfieldname(fldno, sfieldname[fldno]);

   /*  If this field is selected for drawing as a      */
   /*  surface face, reset drawing arrays and redraw.  */
   redraw = 0;
   if (surffieldactive == fldno + numvars && surfflag)
     {
      redraw = 1;
      for (i = 0; i < nsurf; i++)
         surffield[i] = value[i];
      setsurfsfldlab(fldno);
     }
   free(value);

   /*  Set surface field button sensitivity off.  */
   setsfldsensitive(fldno,FALSE);

   fastdraw = 0;
   if (redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void sclosefldcalc(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0) XtUnmanageChild(sfldcalcform);
   if (arg == 1)
     {
      XtUnmanageChild(sfieldform);
      XtUnmanageChild(sfldcalcform2);
     }
}


void doitsfldcalc(w,client_data,call_data)
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
  int dosfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  double * readsurfvalue(int ifld);

   /*  Read the new field name and constant.  */
   tmpstr = XmTextFieldGetString(sfcname);
   strcpy(fldname,tmpstr);
   XtFree(tmpstr);

   tmpstr = XmTextFieldGetString(sfcconst);
   cval = atof(tmpstr); 
   XtFree(tmpstr);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Process the new field.  */
   fldno = sfldcalcbeg + currsfc;   
   if (fldno < snumvars)
     {

      /*  Calculate new field.  */
      ierr = dosfldcalc(sfldcalc_oper,cval,sfldcalc_fldx,
                        sfldcalc_fldy, fldno);

      /*  Fill warning messages, if any.  */
      if (ierr > 0)
        {
         if (ierr == 1)
            string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         else
            string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(sfcerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      /*  Fill sfieldcalc.  */
      sfieldcalc[currsfc].xfldno = sfldcalc_fldx;
      sfieldcalc[currsfc].yfldno = sfldcalc_fldy;
      sfieldcalc[currsfc].oper = sfldcalc_oper;
      sfieldcalc[currsfc].cval = cval;
      strcpy(sfieldcalc[currsfc].name,fldname);
      strcpy(sfieldname[fldno],fldname);

      string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sfcsel[currsfc],XmNlabelString,string,NULL);
      XmStringFree(string);

      /*  Reset field button name for all menus.  */
      setsfieldname(fldno, sfieldname[fldno]);

      /*  Set surface field button sensitivity on.  */
      setsfldsensitive(fldno,TRUE);
     }

   else
     {
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sfcerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   /*  If this field is selected for drawing as a      */
   /*  surface face, reset drawing arrays and redraw.  */
   redraw = 0;
   if (surffieldactive == (fldno+numvars))
     {
      value = readsurfvalue(fldno);

      if (surffieldactive == fldno + numvars && surfflag)
        {
         redraw = 1;
         for (i = 0; i < nsurf; i++)
            surffield[i] = value[i];
         setsurfsfldlab(fldno);
        }

      free(value);
     }

   XmUpdateDisplay(sfcerror);
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


void makesfldcalcform(parent)
 Widget parent;
{
  Widget sfcclose, sfcsellab, sfcselbox, sfcclab, sfcnamelab,
         sfcoplab, sfcswino, sfcboxo, sfcfllabx, sfcflbutx,
         sfcfllaby, sfcflbuty, sfcadd, sfcdel, sfcclose2;
  int i;
  Arg wargs[20];
  char str[10];
  XmString string;

   string = XmStringCreate("Field Calc. - Surface",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   sfldcalcform = XmCreateFormDialog(parent,"Sfldcalcform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sfcclose = XtVaCreateManagedWidget("Sfcclose",
               xmPushButtonGadgetClass,
               sfldcalcform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfcclose,XmNactivateCallback,sclosefldcalc,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Select Surface Field Calc",
                           XmSTRING_DEFAULT_CHARSET);
   sfcsellab = XtVaCreateManagedWidget("Sfcsellab",
              xmLabelGadgetClass,
              sfldcalcform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfcclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sfcsellab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   sfcselbox = XmCreateRadioBox(sfldcalcform,"Sfcselbox",wargs,i);
   XtManageChild(sfcselbox);

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str,"FldCalc%d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Fcsel%d",i);
      sfcsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 sfcselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(sfcsel[i],XmNvalueChangedCallback,
                       sfldcalc_select,
                       (caddr_t) i);
     }


   /*  Create field calc. build form.  */

   string = XmStringCreate("Selected Surface Field Calc.",
                            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   sfldcalcform2 = XmCreateFormDialog(sfldcalcform,"Sfldcalcform2",wargs,i);
      XmStringFree(string);

   sfcname = XtVaCreateManagedWidget("Sfcname",
             xmTextFieldWidgetClass,
             sfldcalcform2,
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
   sfcnamelab = XtVaCreateManagedWidget("Sfcnamelab",
                xmLabelGadgetClass,
                sfldcalcform2,
                XmNtopAttachment, XmATTACH_FORM,
                XmNtopOffset, 5,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, sfcname,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("C=",XmSTRING_DEFAULT_CHARSET);
   sfcclab = XtVaCreateManagedWidget("Sfcclab",
             xmLabelGadgetClass,
             sfldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, sfcname,
             XmNtopOffset, 15,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   sfcconst = XtVaCreateManagedWidget("Sfcconst",
              xmTextFieldWidgetClass,
              sfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfcname,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, sfcclab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 10,
              XmNcolumns, 10,
              XmNvalue, "",
              NULL);

   string = XmStringCreate("Operators:",XmSTRING_DEFAULT_CHARSET);
   sfcoplab = XtVaCreateManagedWidget("Sfcoplab",
              xmLabelGadgetClass,
              sfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfcconst,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sfcoplab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   sfcswino = XmCreateScrolledWindow(sfldcalcform2,"Sfcswino",wargs,i);
   XtManageChild(sfcswino);

   i = 0;
   sfcboxo = XmCreateRadioBox(sfcswino,"Sfcboxo",wargs,i);
   XtManageChild(sfcboxo);

   for (i = 0; i < MAXFLDOPERS; i++)
     {
      string = XmStringCreate(sopername[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Sfcoper%d",i);
      sfcoper[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   sfcboxo,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   NULL);
         XmStringFree(string);
         XtAddCallback(sfcoper[i],XmNvalueChangedCallback,
                       sfldcalc_select_oper,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(sfcoper[i],TRUE,FALSE);
     }

   string = XmStringCreate("X Field:",XmSTRING_DEFAULT_CHARSET);
   sfcfllabx = XtVaCreateManagedWidget("Sfcfllabx",
               xmLabelGadgetClass,
               sfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfcswino,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   sfcfldxlab = XtVaCreateManagedWidget("Sfcfldxlab",
                xmLabelGadgetClass,
                sfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sfcfllabx,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   sfcflbutx = XtVaCreateManagedWidget("Sfcflbutx",
               xmPushButtonGadgetClass,
               sfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfcfldxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfcflbutx,XmNactivateCallback,sfldcalcxfld,NULL);

   string = XmStringCreate("Y Field:",XmSTRING_DEFAULT_CHARSET);
   sfcfllaby = XtVaCreateManagedWidget("Sfcfllaby",
               xmLabelGadgetClass,
               sfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfcflbutx,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   sfcfldylab = XtVaCreateManagedWidget("Sfcfldylab",
                xmLabelGadgetClass,
                sfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sfcfllaby,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   sfcflbuty = XtVaCreateManagedWidget("Sfcflbuty",
               xmPushButtonGadgetClass,
               sfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfcfldylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfcflbuty,XmNactivateCallback,sfldcalcyfld,NULL);

   string = XmStringCreate("  ",XmSTRING_DEFAULT_CHARSET);
   sfcerror = XtVaCreateManagedWidget("Sfcerror",
              xmLabelGadgetClass,
              sfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfcflbuty,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   sfcadd = XtVaCreateManagedWidget("Sfcadd",
            xmPushButtonGadgetClass,
            sfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, sfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(sfcadd,XmNactivateCallback,doitsfldcalc,
                    (caddr_t) 2);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   sfcdel = XtVaCreateManagedWidget("Sfcdel",
            xmPushButtonGadgetClass,
            sfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, sfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, sfcadd,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(sfcdel,XmNactivateCallback,sfcdelete,NULL);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sfcclose2 = XtVaCreateManagedWidget("Sfcclose2",
               xmPushButtonGadgetClass,
               sfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sfcerror,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sfcdel,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfcclose2,XmNactivateCallback,sclosefldcalc,
                    (caddr_t) 1);

}


void applysfldcalc(short oper, float cval, double *xvalue,
                   double *yvalue,int *ierror, double *vmin,
                   double *vmax)
{
  int i, ierr, isgn, itmp;
  double val1, val2;
  float f;

   /*  This routine performs unary or binary operations on the data  */
   /*  in xvalue and yvalue and determines the min. and max. value.  */
   ierr = 0;
   
   switch(oper)
     {

      case(0):  /*  e**X  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = exp(xvalue[i]);
           }
         break;

      case(1):  /*  10**X  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = pow(10., xvalue[i]);
           }
         break;

      case(2):  /*  ln(X)  */
         for (i = 0; i < nsurf; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(3):  /*  log(X)  */
         for (i = 0; i < nsurf; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log10(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(4):  /*  abs(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = fabs(xvalue[i]);
           }
         break;

      case(5):  /*  sqrt(X)  */
         for (i = 0; i < nsurf; i++)
           {
            if (xvalue[i] >= 0.)
               xvalue[i] = sqrt(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(6):  /*  X**2  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] * xvalue[i];
           }
         break;

      case(7):  /*  X**C  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < nsurf; i++)
           {
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = pow(xvalue[i], (double) cval);
           }
         break;

      case(8):  /*  X*C  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] * cval;
           }
         break;

      case(9):  /*  1/X  */
         for (i = 0; i < nsurf; i++)
           {
            if (xvalue[i] != 0.)
               xvalue[i] = 1. / xvalue[i];
            else ierr = 1;
           }
         break;

      case(10):  /*  X/C  */
         for (i = 0; i < nsurf; i++)
           {
            if (cval != 0.)
               xvalue[i] = xvalue[i] / cval;
            else ierr = 1;
           }
         break;

      case(11):  /*  X+C  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] + cval;
           }
         break;

      case(12):  /*  X-C  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] - cval;
           }
         break;

      case(13):  /*  X+Y  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] + yvalue[i];
           }
         break;

      case(14):  /*  X-Y  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] - yvalue[i];
           }
         break;

      case(15):  /*  X*Y  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = xvalue[i] * yvalue[i];
           }
         break;

      case(16):  /*  X/Y  */
         for (i = 0; i < nsurf; i++)
           {
            if (yvalue[i] != 0.)
               xvalue[i] = xvalue[i] / yvalue[i];
            else ierr = 1;
           }
         break;

      case(17):  /*  X**Y  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < nsurf; i++)
           {
            f = yvalue[i] - (float) floor(yvalue[i]);
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = 
                 pow(xvalue[i], yvalue[i]);
           }
         break;

      case(18):  /*  sin(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = sin(xvalue[i]);
           }
         break;

      case(19):  /*  cos(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = cos(xvalue[i]);
           }
         break;

      case(20):  /*  tan(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = tan(xvalue[i]);
           }
         break;

      case(21):  /*  asin(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = asin(xvalue[i]);
           }
         break;

      case(22):  /*  acos(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = acos(xvalue[i]);
           }
         break;

      case(23):  /*  atan(X)  */
         for (i = 0; i < nsurf; i++)
           {
            xvalue[i] = atan(xvalue[i]);
           }
         break;

      /* Added to implement indexing into a colormap */
      /* Note that the current implementation does not work the */
      /* same way as the round() math library function in C99, */
      /* and the precision of the integer may cause truncation */ 
      case(24):  /*  C*round(X)+Y */
         for (i = 0; i < nsurf; i++)
           {
            isgn = (xvalue[i] < 0.) ? -1 : 1;
            itmp = (isgn * xvalue[i] + 0.5);
            xvalue[i] = cval * isgn  * itmp + yvalue[i];
           }
         break;         
    }

   /*  Determine min. & max.  */
   val1 = MAXVAL;  val2 = -MAXVAL;
   for (i = 0; i < nsurf; i++)
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


int dosfldcalc(short oper, float cval, short xfld, short yfld, 
               int fldno)
{
  int ierr, i;
  double *xvalue, *yvalue, vmin, vmax, *field;
  double * readsurfvalue(int ifld);
  void writesurffldloc(int ifld, double *value);

   /*  If first field calc., add to surface field file.  */
   if (sfldcalc_has_data == 0)
     {
      field=(double *)malloc((nsurf)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (i = 0; i < nsurf; i++) field[i] = 0;

      for (i = 0; i < MAXFLDCALC; i++)
        {
         if (snumvars < MAXFIELDS)
            writesurffld(field);
        }
      free(field);
      sfldcalc_has_data = 1;
     }
  
   /*  Get values for the selected field.  */
   xvalue = readsurfvalue(xfld);
   if (yfld > -1)
      yvalue = readsurfvalue(yfld);

   /*  Apply the operator.  */
   applysfldcalc(oper,cval,xvalue,yvalue,&ierr,&vmin,&vmax);

   /*  Write out the new field data,  */
   /*  fill the field min. and max.   */
   writesurffldloc(fldno,xvalue);

   globfieldsmin[fldno] = vmin;
   globfieldsmax[fldno] = vmax;
   sfieldmin[fldno] = vmin;
   sfieldmax[fldno] = vmax;
   sfld_nice_min[fldno] = vmin;
   sfld_nice_max[fldno] = vmax;

   free(xvalue);
   if (yfld > -1) free(yvalue);

   return ierr;
}



void fcalcxgetsfield(int i)
{
  XmString string;

   sfldcalc_fldx = i;
   string = XmStringCreate(sfieldname[sfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void fcalcygetsfield(int i)
{
  XmString string;

   sfldcalc_fldy = i;
   string = XmStringCreate(sfieldname[sfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sfcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);
}
