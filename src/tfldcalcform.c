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

static Widget tfcsel[MAXFLDCALC], tfldcalcform2, tfcname,
              tfcerror, tfcconst, tfcoper[MAXFLDOPERS],
              tftfldxlab, tftfldylab;
static int currtfc = 0;
static short tfldcalc_fldx, tfldcalc_fldy, tfldcalc_oper;
static char *sopername[MAXFLDOPERS] = {
                      "e**X        ", "10**X       ", "ln(X)       ",
                      "log(X)      ", "abs(X)      ", "sqrt(X)     ",
                      "X**2        ", "X**C        ", "X*C         ",
                      "1/X         ", "X/C         ", "X+C         ",
                      "X-C         ", "X+Y         ", "X-Y         ", 
                      "X*Y         ", "X/Y         ", "X**Y        " ,
                      "sin(X)      ", "cos(x)      ", "tan(X)      ",
                      "asin(X)     ", "acos(x)     ", "atan(X)     ",
                      "C*round(X)+Y" };


void tfldcalcformpop()
{
  int i;
  char str4[40];
  XmString string;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str4,"%s",tfieldcalc[i].name);   
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(tfcsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(tfldcalcform);
}


void tfldcalcformpop2()
{
  char str[40];
  XmString string;

   sprintf(str,"%s",tfieldcalc[currtfc].name);   
   XtVaSetValues(tfcname,XmNvalue,str,NULL);

   sprintf(str,"%5.3g",tfieldcalc[currtfc].cval);
   XtVaSetValues(tfcconst,XmNvalue,str,NULL);

   tfldcalc_fldx = tfieldcalc[currtfc].xfldno;
   if (tfldcalc_fldx == -1) tfldcalc_fldx = 0;
   string = XmStringCreate(fieldtrname[tfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tftfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   tfldcalc_fldy = tfieldcalc[currtfc].yfldno;
   if (tfldcalc_fldy == -1) tfldcalc_fldy = 0;
   string = XmStringCreate(fieldtrname[tfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tftfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);

   tfldcalc_oper = tfieldcalc[currtfc].oper;
   if (tfldcalc_oper == -1) tfldcalc_oper = 0;
   XmToggleButtonGadgetSetState(tfcoper[tfldcalc_oper],TRUE,TRUE);

   MyManageChild(tfldcalcform2);
}


void tfldcalc_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) currtfc = i;

   tfldcalcformpop2();
}


void tfldcalc_select_oper(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) tfldcalc_oper = i;
}


void tfldcalcxfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = tfldcalc_fldx;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(trfieldform, XmNx,fx, XmNy,fy, NULL);
 
   trfieldformpop(TRFLDFLDCALCX, ifld);
}


void tfldcalcyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = tfldcalc_fldy;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(trfieldform, XmNx,fx, XmNy,fy, NULL);
 
   trfieldformpop(TRFLDFLDCALCY, ifld);
}


void tfcdelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[40];
  XmString string;
  int fldno, i, redraw;

   sprintf(str,"FldCalc%d",currtfc+1);
   XtVaSetValues(tfcname,XmNvalue,str,NULL);

   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tfcsel[currtfc],XmNlabelString,string,NULL);
   XmStringFree(string);

   tfieldcalc[currtfc].xfldno = -1;
   tfieldcalc[currtfc].yfldno = -1;
   tfieldcalc[currtfc].oper = -1;
   tfieldcalc[currtfc].cval = 0.;
   strcpy(tfieldcalc[currtfc].name,str);
  
   /*  Reset field calc field to 0.  */
   fldno = tfldcalcbeg + currtfc;   
   for (i = 0; i < ntracers; i++) tracers[0].field[fldno][i] = 0;

   globfieldtrmin[fldno] = 0.;
   globfieldtrmax[fldno] = 0.;
   fieldtrmin[fldno] = 0.;
   fieldtrmax[fldno] = 0.;
   trfld_nice_min[fldno] = 0.;
   trfld_nice_max[fldno] = 0.;
   strcpy(fieldtrname[fldno],str);

   /*  Reset field button name for all menus.  */
   settrfieldname(fldno, fieldtrname[fldno]);

   /*  If this field is selected for drawing as a   */
   /*  cell face, reset drawing arrays and redraw.   */
   redraw = 0;
   if (tracefieldactive == fldno && tracerflag) redraw = 1;
     
   /*  Reset tracer field names.  */
   settrfldlab(fldno);
   settrfldlimlab(fldno);
   settrfieldname(fldno, fieldtrname[fldno]);

   /*  Set trace field button sensitivity off.  */
   settrfldsensitive(fldno,FALSE);

   fastdraw = 0;
   if (redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void tclosefldcalc(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0) XtUnmanageChild(tfldcalcform);
   if (arg == 1)
     {
      XtUnmanageChild(cfieldform);
      XtUnmanageChild(tfldcalcform2);
     }
}


void doittfldcalc(w,client_data,call_data)
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
  int dotfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);

   /*  Read the new field name and constant.  */
   tmpstr = XmTextFieldGetString(tfcname);
   strcpy(fldname,tmpstr);
   XtFree(tmpstr);

   tmpstr = XmTextFieldGetString(tfcconst);
   cval = atof(tmpstr); 
   XtFree(tmpstr);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tfcerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Process the new field.  */
   fldno = tfldcalcbeg + currtfc;
   if (fldno < numvartr)
     {

      /*  Calculate new field.  */
 
      ierr = dotfldcalc(tfldcalc_oper,cval,tfldcalc_fldx,tfldcalc_fldy, fldno);

      /*  Fill warning messages, if any.  */
      if (ierr > 0)
        {
         if (ierr == 1)
            string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         else
            string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(tfcerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      /*  Fill tfieldcalc.  */
      tfieldcalc[currtfc].xfldno = tfldcalc_fldx;
      tfieldcalc[currtfc].yfldno = tfldcalc_fldy;
      tfieldcalc[currtfc].oper = tfldcalc_oper;
      tfieldcalc[currtfc].cval = cval;
      strcpy(tfieldcalc[currtfc].name,fldname);
      strcpy(fieldtrname[fldno],fldname);

      string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(tfcsel[currtfc],XmNlabelString,string,NULL);
      XmStringFree(string);

      /*  Reset field button name for all menus.  */
      settrfldlab(fldno);
      settrfldlimlab(fldno);
      settrfieldname(fldno, fieldtrname[fldno]);

      /*  Set cell field button sensitivity on.  */
      settrfldsensitive(fldno,TRUE);
     }

   else
     {
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(tfcerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
     
   /*  If this field is to be drawn, redraw.  */
   redraw = 0;
   if (tracefieldactive == fldno && tracerflag) redraw = 1;

   XmUpdateDisplay(tfcerror);
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


void doalltfldcalc(void)
{
  /*  Calculate tracer field for all possible calc. fields.  */
  int i, fldno, ierr;
  int dotfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  
  for (i = 0; i < MAXFLDCALC; i++)
    {
     fldno = tfldcalcbeg + i;   
     if (fldno < numvartr && tfieldcalc[i].xfldno > -1)
       {
        ierr = dotfldcalc(tfieldcalc[i].oper,tfieldcalc[i].cval,
                          tfieldcalc[i].xfldno,tfieldcalc[i].yfldno,
                          fldno);
       }

    }
}


void maketfldcalcform(parent)
 Widget parent;
{
  Widget tfcclose, tfsellab, tfcselbox, tfcclab, tfnamelab,
         tfcoplab, tfcswino, tfcboxo, tfcfllabx, tfcflbutx,
         tfcfllaby, tfcflbuty, tfcadd, tfcdel, tfcclose2;
  int i;
  Arg wargs[20];
  char str[10];
  XmString string;

   string = XmStringCreate("Field Calc. - Particles",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   tfldcalcform = XmCreateFormDialog(parent,"tfldcalcform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   tfcclose = XtVaCreateManagedWidget("tfcclose",
               xmPushButtonGadgetClass,
               tfldcalcform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(tfcclose,XmNactivateCallback,tclosefldcalc,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Select Particle Field Calc",
                           XmSTRING_DEFAULT_CHARSET);
   tfsellab = XtVaCreateManagedWidget("tfsellab",
              xmLabelGadgetClass,
              tfldcalcform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, tfcclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, tfsellab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   tfcselbox = XmCreateRadioBox(tfldcalcform,"tfcselbox",wargs,i);
   XtManageChild(tfcselbox);

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str,"FldCalc%d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Fcsel%d",i);
      tfcsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 tfcselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(tfcsel[i],XmNvalueChangedCallback,
                       tfldcalc_select,
                       (caddr_t) i);
     }


   /*  Create field calc. build form.  */

   string = XmStringCreate("Selected Particle Field Calc.",
                            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   tfldcalcform2 = XmCreateFormDialog(tfldcalcform,"tfldcalcform2",wargs,i);
      XmStringFree(string);

   tfcname = XtVaCreateManagedWidget("tfcname",
             xmTextFieldWidgetClass,
             tfldcalcform2,
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
   tfnamelab = XtVaCreateManagedWidget("tfnamelab",
               xmLabelGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_FORM,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, tfcname,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("C=",XmSTRING_DEFAULT_CHARSET);
   tfcclab = XtVaCreateManagedWidget("tfcclab",
             xmLabelGadgetClass,
             tfldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, tfcname,
             XmNtopOffset, 15,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   tfcconst = XtVaCreateManagedWidget("tfcconst",
              xmTextFieldWidgetClass,
              tfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, tfcname,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, tfcclab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 10,
              XmNcolumns, 10,
              XmNvalue, "",
              NULL);

   string = XmStringCreate("Operators:",XmSTRING_DEFAULT_CHARSET);
   tfcoplab = XtVaCreateManagedWidget("tfcoplab",
              xmLabelGadgetClass,
              tfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, tfcconst,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, tfcoplab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   tfcswino = XmCreateScrolledWindow(tfldcalcform2,"tfcswino",wargs,i);
   XtManageChild(tfcswino);

   i = 0;
   tfcboxo = XmCreateRadioBox(tfcswino,"tfcboxo",wargs,i);
   XtManageChild(tfcboxo);

   for (i = 0; i < MAXFLDOPERS; i++)
     {
      string = XmStringCreate(sopername[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"tfcoper%d",i);
      tfcoper[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   tfcboxo,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   NULL);
         XmStringFree(string);
         XtAddCallback(tfcoper[i],XmNvalueChangedCallback,
                       tfldcalc_select_oper,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(tfcoper[i],TRUE,FALSE);
     }

   string = XmStringCreate("X Field:",XmSTRING_DEFAULT_CHARSET);
   tfcfllabx = XtVaCreateManagedWidget("tfcfllabx",
               xmLabelGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, tfcswino,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   tftfldxlab = XtVaCreateManagedWidget("tftfldxlab",
                xmLabelGadgetClass,
                tfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, tfcfllabx,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   tfcflbutx = XtVaCreateManagedWidget("tfcflbutx",
               xmPushButtonGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, tftfldxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(tfcflbutx,XmNactivateCallback,tfldcalcxfld,NULL);

   string = XmStringCreate("Y Field:",XmSTRING_DEFAULT_CHARSET);
   tfcfllaby = XtVaCreateManagedWidget("tfcfllaby",
               xmLabelGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, tfcflbutx,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   tftfldylab = XtVaCreateManagedWidget("tftfldylab",
                xmLabelGadgetClass,
                tfldcalcform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, tfcfllaby,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   tfcflbuty = XtVaCreateManagedWidget("tfcflbuty",
               xmPushButtonGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, tftfldylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(tfcflbuty,XmNactivateCallback,tfldcalcyfld,NULL);

   string = XmStringCreate("  ",XmSTRING_DEFAULT_CHARSET);
   tfcerror = XtVaCreateManagedWidget("tfcerror",
              xmLabelGadgetClass,
              tfldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, tfcflbuty,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   tfcadd = XtVaCreateManagedWidget("tfcadd",
            xmPushButtonGadgetClass,
            tfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, tfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(tfcadd,XmNactivateCallback,doittfldcalc,
                    (caddr_t) 2);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   tfcdel = XtVaCreateManagedWidget("tfcdel",
            xmPushButtonGadgetClass,
            tfldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, tfcerror,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, tfcadd,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(tfcdel,XmNactivateCallback,tfcdelete,NULL);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   tfcclose2 = XtVaCreateManagedWidget("tfcclose2",
               xmPushButtonGadgetClass,
               tfldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, tfcerror,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, tfcdel,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(tfcclose2,XmNactivateCallback,tclosefldcalc,
                    (caddr_t) 1);

}


void applytfldcalc(short oper, float cval, double *xvalue,
                   double *yvalue,double *retval, int *ierror, 
		   double *vmin, double *vmax)
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
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = exp(xvalue[i]);
           }
         break;

      case(1):  /*  10**X  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = pow(10., xvalue[i]);
           }
         break;

      case(2):  /*  ln(X)  */
         for (i = 0; i < ntracers; i++)
           {
            if (xvalue[i] > 0.)
               retval[i] = log(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(3):  /*  log(X)  */
         for (i = 0; i < ntracers; i++)
           {
            if (xvalue[i] > 0.)
               retval[i] = log10(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(4):  /*  abs(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = fabs(xvalue[i]);
           }
         break;

      case(5):  /*  sqrt(X)  */
         for (i = 0; i < ntracers; i++)
           {
            if (xvalue[i] >= 0.)
               retval[i] = sqrt(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(6):  /*  X**2  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] * xvalue[i];
           }
         break;

      case(7):  /*  X**C  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < ntracers; i++)
           {
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              retval[i] = pow(xvalue[i], (double) cval);
           }
         break;

      case(8):  /*  X*C  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] * cval;
           }
         break;

      case(9):  /*  1/X  */
         for (i = 0; i < ntracers; i++)
           {
            if (xvalue[i] != 0.)
               retval[i] = 1. / xvalue[i];
            else ierr = 1;
           }
         break;

      case(10):  /*  X/C  */
         for (i = 0; i < ntracers; i++)
           {
            if (cval != 0.)
               retval[i] = xvalue[i] / cval;
            else ierr = 1;
           }
         break;

      case(11):  /*  X+C  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] + cval;
           }
         break;

      case(12):  /*  X-C  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] - cval;
           }
         break;

      case(13):  /*  X+Y  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] + yvalue[i];
           }
         break;

      case(14):  /*  X-Y  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] - yvalue[i];
           }
         break;

      case(15):  /*  X*Y  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = xvalue[i] * yvalue[i];
           }
         break;

      case(16):  /*  X/Y  */
         for (i = 0; i < ntracers; i++)
           {
            if (yvalue[i] != 0.)
               retval[i] = xvalue[i] / yvalue[i];
            else ierr = 1;
           }
         break;

      case(17):  /*  X**Y  */
         f = cval - (float) floor((double) cval);
         for (i = 0; i < ntracers; i++)
           {
            f = yvalue[i] - (float) floor(yvalue[i]);
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              retval[i] = 
                 pow(xvalue[i], yvalue[i]);
           }
         break;

      case(18):  /*  sin(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = sin(xvalue[i]);
           }
         break;

      case(19):  /*  cos(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = cos(xvalue[i]);
           }
         break;

      case(20):  /*  tan(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = tan(xvalue[i]);
           }
         break;

      case(21):  /*  asin(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = asin(xvalue[i]);
           }
         break;

      case(22):  /*  acos(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = acos(xvalue[i]);
           }
         break;

      case(23):  /*  atan(X)  */
         for (i = 0; i < ntracers; i++)
           {
            retval[i] = atan(xvalue[i]);
           }
         break;

      /* Added to implement indexing into a colormap */
      /* Note that the current implementation does not work the */
      /* same way as the round() math library function in C99, */
      /* and the precision of the integer may cause truncation */ 
      case(24):  /*  C*round(X)+Y */
         for (i = 0; i < ntracers; i++)
           {
            isgn = (xvalue[i] < 0.) ? -1 : 1;
            itmp = (isgn * xvalue[i] + 0.5);
            retval[i] = cval * isgn  * itmp + yvalue[i];
           }
         break;
    }

   /*  Determine min. & max.  */
   val1 = MAXVAL;  val2 = -MAXVAL;
   for (i = 0; i < ntracers; i++)
     {
      val1 = (val1 < retval[i]) ? val1 : retval[i];
      val2 = (val2 > retval[i]) ? val2 : retval[i];
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


int dotfldcalc(short oper, float cval, short xfld, short yfld, 
               int fldno)
{
  int ierr, i, ntrhist, j;
  double vmin, vmax, vmn, vmx;
  void applytfldcalc(short oper, float cval, double *xvalue,
                     double *yvalue,double *retval, int *ierror, 
		     double *vmin, double *vmax);

   /*  Loop through the tracer history and apply the operator.  */
   ntrhist = ntracehist;
   if (ntracehist < 1) ntrhist = 1;
   for (i = 0; i < ntrhist; i++)
     {
      applytfldcalc(oper,cval,tracers[i].field[xfld],tracers[i].field[yfld],
                    tracers[i].field[fldno],&ierr,&vmn,&vmx);
      if (i == 0)
        {
	 vmin = vmn;
	 vmax = vmx;
	}
     }

   globfieldtrmin[fldno] = vmin;
   globfieldtrmax[fldno] = vmax;
   fieldtrmin[fldno] = vmin;
   fieldtrmax[fldno] = vmax;
   trfld_nice_min[fldno] = vmin;
   trfld_nice_max[fldno] = vmax;

   return ierr;
}


void fcalcxgettfield(int i)
{
  XmString string;

   tfldcalc_fldx = i;
   string = XmStringCreate(fieldtrname[tfldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tftfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void fcalcygettfield(int i)
{
  XmString string;

   tfldcalc_fldy = i;
   string = XmStringCreate(fieldtrname[tfldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(tftfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);
}
