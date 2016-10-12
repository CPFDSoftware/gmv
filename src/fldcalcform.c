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

static Widget fcsel[MAXFLDCALC], fldcalcform2, fcname, fcerror,
              fcconst, fcoper[MAXFLDOPERS], fcfldxlab,
              fcfldylab;
static int currfc = 0, fldcalc_fldx, fldcalc_fldy, fldcalc_oper;
static char *opername[MAXFLDOPERS] = {
                     "e**X        ", "10**X       ", "ln(X)       ",
                     "log(X)      ", "abs(X)      ", "sqrt(X)     ",
                     "X**2        ", "X**C        ", "X*C         ",
                     "1/X         ", "X/C         ", "X+C         ",
                     "X-C         ", "X+Y         ", "X-Y         ", 
                     "X*Y         ", "X/Y         ", "X**Y        ",
                     "sin(X)      ", "cos(x)      ", "tan(X)      ",
                     "asin(X)     ", "acos(x)     ", "atan(X)     ",
                     "C*round(X)+Y", };


void fldcalcformpop()
{
  int i;
  char str4[10];
  XmString string;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str4,"%s",fieldcalc[i].name);   
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fcsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(fldcalcform);
}


void fldcalcformpop2()
{
  char str[40];
  XmString string;

   sprintf(str,"%s",fieldcalc[currfc].name);   
   XtVaSetValues(fcname,XmNvalue,str,NULL);

   sprintf(str,"%5.3g",fieldcalc[currfc].cval);
   XtVaSetValues(fcconst,XmNvalue,str,NULL);

   fldcalc_fldx = fieldcalc[currfc].xfldno;
   if (fldcalc_fldx == -1) fldcalc_fldx = 0;
   string = XmStringCreate(fieldname[fldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fldcalc_fldy = fieldcalc[currfc].yfldno;
   if (fldcalc_fldy == -1) fldcalc_fldy = 0;
   string = XmStringCreate(fieldname[fldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);

   fldcalc_oper = fieldcalc[currfc].oper;
   if (fldcalc_oper == -1) fldcalc_oper = 0;
   XmToggleButtonGadgetSetState(fcoper[fldcalc_oper],TRUE,TRUE);

   MyManageChild(fldcalcform2);
}


void fldcalc_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) currfc = i;

   fldcalcformpop2();
}


void fldcalc_select_oper(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (onflag) fldcalc_oper = i;
}


void fldcalcxfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = fldcalc_fldx;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDFLDCALCX, ifld);
}


void fldcalcyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = fldcalc_fldy;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDFLDCALCY, ifld);
}


void fcdelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[40];
  XmString string;
  int fldno, i, redraw;
  double *value;
  void writenodefldloc(int ifld, double *value);

   sprintf(str,"FldCalc%d",currfc+1);
   XtVaSetValues(fcname,XmNvalue,str,NULL);

   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcsel[currfc],XmNlabelString,string,NULL);
   XmStringFree(string);

   fieldcalc[currfc].xfldno = -1;
   fieldcalc[currfc].yfldno = -1;
   fieldcalc[currfc].oper = -1;
   fieldcalc[currfc].cval = 0.;
   strcpy(fieldcalc[currfc].name,str);
  
   /*  Reset field calc field to 0.  */
   value=(double *)malloc((nnodes+1)*sizeof(double));
   if (value == NULL) memerr();
   for (i = 1; i < nnodes+1; i++) value[i] = 0;
   fldno = fldcalcbeg + currfc;
   writenodefldloc(fldno,value);  

   globfieldmin[fldno] = 0.;
   globfieldmax[fldno] = 0.;
   fieldmin[fldno] = 0.;
   fieldmax[fldno] = 0.;
   nfld_nice_min[fldno] = 0.;
   nfld_nice_max[fldno] = 0.;
   strcpy(fieldname[fldno],str);

   /*  Reset field button name for all menus.  */
   setnfieldname(fldno, fieldname[fldno]);

   /*  If this field is selected for drawing as a node  */
   /*  or cell face, reset drawing arrays and redraw.   */
   redraw = 0;
   if (nodefieldactive == fldno && nodesflag > 0)
     {
      redraw = 1;
      for (i = 1; i < nnodes+1; i++)
         nodefield[i] = value[i];
      setnodefldlab(nodefieldactive);
     }
   if (cellfieldactive == fldno && cellsflag)
     {
      redraw = 1;
      for (i = 1; i < nnodes+1; i++)
         cellfield[i] = value[i];
      setcellnfldlab(cellfieldactive);
     }
   if (gridfieldactive == fldno)
     {
      redraw = 1;
      for (i = 1; i < nnodes+1; i++)
         gridfield[i] = value[i];
      setgridnfldlab(gridfieldactive);
     }
   if (surffieldactive == fldno && surfflag)
     {
      redraw = 1;
      for (i = 1; i < nnodes+1; i++)
         surffield[i] = value[i];
      setsurfnfldlab(surffieldactive);
     }
   free(value);

   /*  Set node field button sensitivity off.  */
   setnfldsensitive(fldno,FALSE);

   fastdraw = 0;
   if (redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void closefldcalc(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0) XtUnmanageChild(fldcalcform);
   if (arg == 1)
     {
      XtUnmanageChild(nfieldform);
      XtUnmanageChild(fldcalcform2);
     }
}


void doitfldcalc(w,client_data,call_data)
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
  int donfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  double * readnodevalue(int ifld);

   /*  Read the new field name and constant.  */
   tmpstr = XmTextFieldGetString(fcname);
   strcpy(fldname,tmpstr);
   XtFree(tmpstr);

   tmpstr = XmTextFieldGetString(fcconst);
   cval = atof(tmpstr); 
   XtFree(tmpstr);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Process the new field.  */
   fldno = fldcalcbeg + currfc;   
   if (fldno < numvars)
     {

      /*  Calculate new field.  */
      ierr = donfldcalc(fldcalc_oper,cval,fldcalc_fldx,
                        fldcalc_fldy, fldno);

      /*  Fill warning messages, if any.  */
      if (ierr > 0)
        {
         if (ierr == 1)
            string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         else
            string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(fcerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      /*  Write out the new field data, fill     */
      /*  fieldcalc and the field min. and max.  */
      fieldcalc[currfc].xfldno = fldcalc_fldx;
      fieldcalc[currfc].yfldno = fldcalc_fldy;
      fieldcalc[currfc].oper = fldcalc_oper;
      fieldcalc[currfc].cval = cval;
      strcpy(fieldcalc[currfc].name,fldname);

      strcpy(fieldname[fldno],fldname);

      string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fcsel[currfc],XmNlabelString,string,NULL);
      XmStringFree(string);

      /*  Reset field button name for all menus.  */
      setnfieldname(fldno, fieldname[fldno]);

      /*  Set node field button sensitivity on.  */
      setnfldsensitive(fldno,TRUE);
     }

   else
     {
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fcerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   /*  If this field is selected for drawing as a node  */
   /*  or cell face, reset drawing arrays and redraw.   */
   redraw = 0;
   if (nodefieldactive == fldno || cellfieldactive == fldno || 
       gridfieldactive == fldno)
     {
      value = readnodevalue(fldno);
 
      if (nodefieldactive == fldno && nodesflag > 0)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            nodefield[i] = value[i];
         setnodefldlab(nodefieldactive);
        }
      if (cellfieldactive == fldno && cellsflag)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            cellfield[i] = value[i];
         setcellnfldlab(cellfieldactive);
        }
      if (gridfieldactive == fldno)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            gridfield[i] = value[i];
         setgridnfldlab(gridfieldactive);
        }
      if (surffieldactive == fldno && surfflag)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            surffield[i] = value[i];
         setsurfnfldlab(surffieldactive);
        }

      free(value);
     }

   XmUpdateDisplay(fcerror);
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


void makefldcalcform(parent)
 Widget parent;
{
  Widget fcclose, fcsellab, fcselbox, fcclab, fcnamelab, 
         fcoplab, fcswino, fcboxo, fcfllabx, fcflbutx,
         fcfllaby, fcflbuty, fcadd, fcdel, fcclose2;
  int i;
  Arg wargs[20];
  char str[20];
  XmString string;

   string = XmStringCreate("Field Calc. - Nodes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fldcalcform = XmCreateFormDialog(parent,"Fldcalcform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fcclose = XtVaCreateManagedWidget("Fcclose",
              xmPushButtonGadgetClass,
              fldcalcform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fcclose,XmNactivateCallback,closefldcalc,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Select Node Field Calc",
                           XmSTRING_DEFAULT_CHARSET);
   fcsellab = XtVaCreateManagedWidget("Fcsellab",
              xmLabelGadgetClass,
              fldcalcform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, fcsellab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   fcselbox = XmCreateRadioBox(fldcalcform,"Fcselbox",wargs,i);
   XtManageChild(fcselbox);

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sprintf(str,"FldCalc%d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Fcsel%d",i);
      fcsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 fcselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(fcsel[i],XmNvalueChangedCallback,
                       fldcalc_select,
                       (caddr_t) i);
     }


   /*  Create field calc. build form.  */

   string = XmStringCreate("Selected Node Field Calc.",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fldcalcform2 = XmCreateFormDialog(fldcalcform,"Fldcalcform2",wargs,i);
      XmStringFree(string);

   fcname = XtVaCreateManagedWidget("Fcname",
            xmTextFieldWidgetClass,
            fldcalcform2,
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
   fcnamelab = XtVaCreateManagedWidget("Fcnamelab",
               xmLabelGadgetClass,
               fldcalcform2,
               XmNtopAttachment, XmATTACH_FORM,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, fcname,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("C=",XmSTRING_DEFAULT_CHARSET);
   fcclab = XtVaCreateManagedWidget("Fcclab",
            xmLabelGadgetClass,
            fldcalcform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, fcname,
            XmNtopOffset, 15,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   fcconst = XtVaCreateManagedWidget("Fcconst",
             xmTextFieldWidgetClass,
             fldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fcname,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, fcclab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);

   string = XmStringCreate("Operators:",XmSTRING_DEFAULT_CHARSET);
   fcoplab = XtVaCreateManagedWidget("Fcoplab",
             xmLabelGadgetClass,
             fldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fcconst,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, fcoplab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   fcswino = XmCreateScrolledWindow(fldcalcform2,"Fcswino",wargs,i);
   XtManageChild(fcswino);

   i = 0;
   fcboxo = XmCreateRadioBox(fcswino,"Fcboxo",wargs,i);
   XtManageChild(fcboxo);

   for (i = 0; i < MAXFLDOPERS; i++)
     {
      string = XmStringCreate(opername[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Fcoper%d",i);
      fcoper[i] = XtVaCreateManagedWidget(str,
                  xmToggleButtonGadgetClass,
                  fcboxo,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  NULL);
         XmStringFree(string);
         XtAddCallback(fcoper[i],XmNvalueChangedCallback,
                       fldcalc_select_oper,
                       (caddr_t) i);

      if (i == 0) 
         XmToggleButtonGadgetSetState(fcoper[i],TRUE,FALSE);
     }

   string = XmStringCreate("X Field:",XmSTRING_DEFAULT_CHARSET);
   fcfllabx = XtVaCreateManagedWidget("Fcfllabx",
              xmLabelGadgetClass,
              fldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcswino,
              XmNtopOffset, 20,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   fcfldxlab = XtVaCreateManagedWidget("Fcfldxlab",
               xmLabelGadgetClass,
               fldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fcfllabx,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNleftOffset, 20,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fcflbutx = XtVaCreateManagedWidget("Fcflbutx",
              xmPushButtonGadgetClass,
              fldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcfldxlab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fcflbutx,XmNactivateCallback,fldcalcxfld,NULL);

   string = XmStringCreate("Y Field:",XmSTRING_DEFAULT_CHARSET);
   fcfllaby = XtVaCreateManagedWidget("Fcfllaby",
              xmLabelGadgetClass,
              fldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcflbutx,
              XmNtopOffset, 20,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   fcfldylab = XtVaCreateManagedWidget("Fcfldylab",
               xmLabelGadgetClass,
               fldcalcform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fcfllaby,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNleftOffset, 20,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fcflbuty = XtVaCreateManagedWidget("Fcflbuty",
              xmPushButtonGadgetClass,
              fldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcfldylab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fcflbuty,XmNactivateCallback,fldcalcyfld,NULL);

   string = XmStringCreate("  ",XmSTRING_DEFAULT_CHARSET);
   fcerror = XtVaCreateManagedWidget("Fcerror",
             xmLabelGadgetClass,
             fldcalcform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fcflbuty,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   fcadd = XtVaCreateManagedWidget("Fcadd",
           xmPushButtonGadgetClass,
           fldcalcform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, fcerror,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(fcadd,XmNactivateCallback,doitfldcalc,
                    (caddr_t) 2);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   fcdel = XtVaCreateManagedWidget("Fcdel",
           xmPushButtonGadgetClass,
           fldcalcform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, fcerror,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, fcadd,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(fcdel,XmNactivateCallback,fcdelete,NULL);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fcclose2 = XtVaCreateManagedWidget("Fcclose2",
              xmPushButtonGadgetClass,
              fldcalcform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcerror,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, fcdel,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(fcclose2,XmNactivateCallback,closefldcalc,
                    (caddr_t) 1);

}


void applyfldcalc(short oper, float cval, double *xvalue,
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
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = exp(xvalue[i]);
           }
         break;

      case(1):  /*  10**X  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = pow(10., xvalue[i]);
           }
         break;

      case(2):  /*  ln(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(3):  /*  log(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (xvalue[i] > 0.)
               xvalue[i] = log10(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(4):  /*  abs(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = fabs(xvalue[i]);
           }
         break;

      case(5):  /*  sqrt(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (xvalue[i] >= 0.)
               xvalue[i] = sqrt(xvalue[i]);
            else ierr = 2;
           }
         break;

      case(6):  /*  X**2  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] * xvalue[i];
           }
         break;

      case(7):  /*  X**C  */
         f = cval - (float) floor((double) cval);
         for (i = 1; i < nnodes+1; i++)
           {
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = pow(xvalue[i], (double) cval);
           }
         break;

      case(8):  /*  X*C  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] * cval;
           }
         break;

      case(9):  /*  1/X  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (xvalue[i] != 0.)
               xvalue[i] = 1. / xvalue[i];
            else ierr = 1;
           }
         break;

      case(10):  /*  X/C  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (cval != 0.)
               xvalue[i] = xvalue[i] / cval;
            else ierr = 1;
           }
         break;

      case(11):  /*  X+C  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] + cval;
           }
         break;

      case(12):  /*  X-C  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] - cval;
           }
         break;

      case(13):  /*  X+Y  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] + yvalue[i];
           }
         break;

      case(14):  /*  X-Y  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] - yvalue[i];
           }
         break;

      case(15):  /*  X*Y  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = xvalue[i] * yvalue[i];
           }
         break;

      case(16):  /*  X/Y  */
         for (i = 1; i < nnodes+1; i++)
           {
            if (yvalue[i] != 0.)
               xvalue[i] = xvalue[i] / yvalue[i];
            else ierr = 1;
           }
         break;

      case(17):  /*  X**Y  */
         f = cval -  (float) floor((double) cval);
         for (i = 1; i < nnodes+1; i++)
           {
            f = yvalue[i] - (float) floor(yvalue[i]);
            if (f != 0. && xvalue[i] < 0.) ierr = 2;
            else
              xvalue[i] = pow(xvalue[i],yvalue[i]);
           }
         break;

      case(18):  /*  sin(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = sin(xvalue[i]);
           }
         break;

      case(19):  /*  cos(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = cos(xvalue[i]);
           }
         break;

      case(20):  /*  tan(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = tan(xvalue[i]);
           }
         break;

      case(21):  /*  asin(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = asin( xvalue[i]);
           }
         break;

      case(22):  /*  acos(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = acos(xvalue[i]);
           }
         break;

      case(23):  /*  atan(X)  */
         for (i = 1; i < nnodes+1; i++)
           {
            xvalue[i] = atan(xvalue[i]);
           }
         break;
      
      /* Added to implement indexing into a colormap */
      /* Note that the current implementation does not work the */
      /* same way as the round() math library function in C99, */
      /* and the precision of the integer may cause truncation */ 
      case(24):  /*  C*round(X)+Y */
         for (i = 1; i < nnodes+1; i++)
           {
            isgn = (xvalue[i] < 0.) ? -1 : 1;
            itmp = (isgn * xvalue[i] + 0.5);
            xvalue[i] = cval * isgn * itmp + yvalue[i];
           }
         break;
    }

   /*  Determine min. & max.  */
   val1 = MAXVAL;  val2 = -MAXVAL;
   for (i = 1; i < nnodes+1; i++)
     {
      if (imat[i] > 0)
        {
         val1 = (val1 < xvalue[i]) ? val1 : xvalue[i];
         val2 = (val2 > xvalue[i]) ? val2 : xvalue[i];
        }
     }

   if (val2 <= val1)
     {
      f = fabs(val1);
      if (f == 0.) f = 1.;
      val2 = val1 + f*0.00001;
     }

   *vmin = val1;  *vmax = val2;
   *ierror = ierr;
}


int donfldcalc(short oper, float cval, short xfld, short yfld, 
               int fldno)
{
  int ierr, i;
  double *xvalue, *yvalue, vmin, vmax, *field;
  double * readnodevalue(int ifld);
  void writenodefldloc(int ifld, double *value);

   /*  If first field calc., add to node field file.  */
   if (fldcalc_has_data == 0)
     {
      field=(double *)malloc((nnodes+1)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (i = 1; i < nnodes+1; i++) field[i] = 0;

      for (i = 0; i < MAXFLDCALC; i++)
        {
         if (numvars < MAXFIELDS)
            writenodefld(field);            
        }
      free(field);
      fldcalc_has_data = 1;
     }
  
   /*  Get values for the selected field.  */
   xvalue = readnodevalue(xfld);
   if (yfld > -1)
      yvalue = readnodevalue(yfld);

   /*  Apply the operator.  */
   applyfldcalc(oper,cval,xvalue,yvalue,&ierr,&vmin,&vmax);

   /*  Write out the new field data,  */
   /*  fill the field min. and max.   */
   writenodefldloc(fldno,xvalue);

   globfieldmin[fldno] = vmin;
   globfieldmax[fldno] = vmax;
   fieldmin[fldno] = vmin;
   fieldmax[fldno] = vmax;
   nfld_nice_min[fldno] = vmin;
   nfld_nice_max[fldno] = vmax;

   free(xvalue);
   if (yfld > -1) free(yvalue);

   return ierr;
}



void fcalcxgetnfield(int i)
{
  XmString string;

   fldcalc_fldx = i;
   string = XmStringCreate(fieldname[fldcalc_fldx],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcfldxlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void fcalcygetnfield(int i)
{
  XmString string;

   fldcalc_fldy = i;
   string = XmStringCreate(fieldname[fldcalc_fldy],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fcfldylab,XmNlabelString,string,NULL);
   XmStringFree(string);
}
