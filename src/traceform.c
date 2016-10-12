#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "gmvread.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget trnone, trpnts, trbigp, trnums, trvals, trsize,
              trfldsbut[MAXTRFIELDS], trhist, trfhlab, trselbut,
              trhiststridetxt, trhiststoptxt, trhistpts;
static short currtrflag = 0, trfieldactive;
static int prefixlen, suffixlen, init_fileno, SIZEflag;
static char histprefix[200];

#define IEEE 0
#define ASCII 1
#define IEEEI4R4 0
#define IEEEI4R8 2



void traceformpop()
{
  int i;
  char str[10];

   for (i = 0; i < numvartr; i++)
      XtVaSetValues(trfldsbut[i],XmNset,FALSE,NULL);
   if (numvartr > 0)
      XtVaSetValues(trfldsbut[tracefieldactive],XmNset,TRUE,NULL);

   XmToggleButtonGadgetSetState(trnone,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trpnts,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trbigp,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trnums,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trvals,FALSE,FALSE);
   XmToggleButtonGadgetSetState(trvals,FALSE,FALSE);
   if (tracerflag == 5 && SIZEflag == 0) tracerflag = 1;
   if (SIZEflag)
      XmToggleButtonGadgetSetState(trsize,FALSE,FALSE);
   switch(tracerflag)
     {
      case(0):
         XmToggleButtonGadgetSetState(trnone,TRUE,FALSE);
         break;
      case(1):
         XmToggleButtonGadgetSetState(trpnts,TRUE,FALSE);
         break;
      case(2):
         XmToggleButtonGadgetSetState(trbigp,TRUE,FALSE);
         break;
      case(3):
         XmToggleButtonGadgetSetState(trnums,TRUE,FALSE);
         break;
      case(4):
         XmToggleButtonGadgetSetState(trvals,TRUE,FALSE);
         break;
      case(5):
         XmToggleButtonGadgetSetState(trsize,TRUE,FALSE);
         break;
     }

   XmToggleButtonGadgetSetState(trhist,tracehistflag,FALSE);
   sprintf(str,"%d",tracehiststop);
   XmTextFieldSetString(trhiststoptxt,str);
   sprintf(str,"%d",tracehiststride);
   XmTextFieldSetString(trhiststridetxt,str);
   XmToggleButtonGadgetSetState(trhistpts,tracehistpoints,FALSE);

   currtrflag = tracerflag;
   trfieldactive = tracefieldactive;

   MyManageChild(traceform);
}


void trclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(traceselform);
   XtUnmanageChild(traceform);
}


void settraceselect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   traceselformpop();
}


void trace_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;
  void trhistread(Widget w,XtPointer client_data,XtPointer call_data);

   tracerflag = currtrflag;
   tracefieldactive = trfieldactive;

   if (!tracehistflag && ntracehist > 1)
     {
      for (i = 1; i < ntracehist; i++)
        {
         for (j = 0; j < tracers[i].numvar; j++)
           {
            free(tracers[i].field[j]);
           }
         free(tracers[i].x);
         free(tracers[i].y);
         free(tracers[i].z);
         tracers[i].ntracers = 0;
        }
      ntracehist = 1;
     }

   if (tracehistflag && ntracehist == 1)
      trhistread(NULL,NULL,NULL);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void traceflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, trflag;

   arg = (long int)client_data;
   trflag = (int)XmToggleButtonGadgetGetState(w);
   
   if (arg == 0 && trflag == 1)
     currtrflag = 0;
   
   if (arg == 1 && trflag == 1)
     currtrflag = 1;
   
   if (arg == 2 && trflag == 1)
     currtrflag = 2;
   
   if (arg == 3 && trflag == 1)
     currtrflag = 3;
  
   if (arg == 4 && trflag == 1)
     currtrflag = 4;

   if (arg == 5 && trflag == 1)
     currtrflag = 5;
}


void tracehist(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
      tracehistflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
      tracehistpoints = (int)XmToggleButtonGadgetGetState(w);
}


void trhistread(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k, trbeg,trno;
  char *tmpstr;
  XmString string;
  void rdtracehist(), settracesel();

   /*  Read trace history stop and stride.  */
   tmpstr = XmTextFieldGetString(trhiststoptxt);
   tracehiststop = atoi(tmpstr);
   XtFree(tmpstr);
   if (tracehiststop < 0) tracehiststop = 0;

   tmpstr = XmTextFieldGetString(trhiststridetxt);
   tracehiststride = atoi(tmpstr);
   XtFree(tmpstr);
   if (tracehiststride < 1) tracehiststride = 1;

   if (ntracehist > 1)
     {
      for (i = 1; i < ntracehist; i++)
        {
         for (j = 0; j < tracers[i].numvar; j++)
           {
            free(tracers[i].field[j]);
           }
         free(tracers[i].x);
         free(tracers[i].y);
         free(tracers[i].z);
         tracers[i].ntracers = 0;
        }
      ntracehist = 1;
     }

   XDefineCursor(dpy,topwin,waitcsr);

   rdtracehist();
   
   /*  Add tracer calc. fields to history tracers.  */
   if (ntracehist > 1)
     {
      for (i = 1; i < ntracehist; i++)
        {
	 trbeg = tracers[i].numvar;
         for (j = 0; j < MAXFLDCALC; j++)
           {
	    trno = trbeg + j;
            if (trno < MAXTRFIELDS)
              {
               tracers[i].field[trno] = 
	                           (double *)malloc((ntracers)*sizeof(double)); 
               if (tracers[i].field[trno] == NULL) memerr();  

               for (k = 0; k < ntracers; k++) 
	          tracers[i].field[trno][k] = 0;
	       tracers[i].numvar++;
              }    
	   }
        }
     }
   
   /*  Execute trace field calculations.  */
   doalltfldcalc();
   settracesel();

   XUndefineCursor(dpy,topwin);
   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trfhlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(trfhlab);

   if (tracehistflag)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void select_trace_field(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   trfieldactive = arg;
}


void maketraceform(parent)
 Widget parent;
{
  Widget traceclose, traceapply, trdrawtxt, trdrawbox, trfldtxt,
         trfldswin, trfldbox, trhlab1, trhlab2, trhrdbut;
  int i, nflds;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Particles",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   traceform = XmCreateFormDialog(parent,"Traceform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   traceclose = XtVaCreateManagedWidget("Traceclose",
                xmPushButtonGadgetClass,
                traceform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(traceclose,XmNactivateCallback,trclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   traceapply = XtVaCreateManagedWidget("Traceapply",
                xmPushButtonGadgetClass,
                traceform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, traceclose,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(traceapply,XmNactivateCallback,trace_apply,
                    NULL);
 
   string = XmStringCreate("Draw as:",XmSTRING_DEFAULT_CHARSET);
   trdrawtxt = XtVaCreateManagedWidget("Trdrawtxt",
               xmLabelWidgetClass,
               traceform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, traceapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trdrawtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   trdrawbox = XmCreateRadioBox(traceform,"Trdrawbox",wargs,i);
   XtManageChild(trdrawbox);
 
   string = XmStringCreate("None",
            XmSTRING_DEFAULT_CHARSET);
   trnone = XtVaCreateManagedWidget("Trnone",
              xmToggleButtonGadgetClass,
              trdrawbox,
              XmNlabelString, string,
              XmNset, TRUE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(trnone,XmNvalueChangedCallback,
                    traceflags,(caddr_t) 0);
 
   string = XmStringCreate("Regular Points",
            XmSTRING_DEFAULT_CHARSET);
   trpnts = XtVaCreateManagedWidget("Trpnts",
            xmToggleButtonGadgetClass,
            trdrawbox,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(trpnts,XmNvalueChangedCallback,
                    traceflags,(caddr_t) 1);
 
   string = XmStringCreate("Spheres",
            XmSTRING_DEFAULT_CHARSET);
   trbigp = XtVaCreateManagedWidget("Trbigp",
            xmToggleButtonGadgetClass,
            trdrawbox,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(trbigp,XmNvalueChangedCallback,
                    traceflags,(caddr_t) 2);
 
   string = XmStringCreate("Numbers",
            XmSTRING_DEFAULT_CHARSET);
   trnums = XtVaCreateManagedWidget("Trnums",
            xmToggleButtonGadgetClass,
            trdrawbox,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(trnums,XmNvalueChangedCallback,
                    traceflags,(caddr_t) 3);
  
   string = XmStringCreate("Values",
            XmSTRING_DEFAULT_CHARSET);
   trvals = XtVaCreateManagedWidget("Trvals",
            xmToggleButtonGadgetClass,
            trdrawbox,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(trvals,XmNvalueChangedCallback,
                    traceflags,(caddr_t) 4);

   /*  Determine if SIZE variable exists, if so,    */
   /*  add option to draw tracer with SIZE radius.  */
   SIZEflag = 0;
   for (i = 0; i < numvartr; i++)
      if (strcmp(fieldtrname[i],"SIZE") == 0 || 
          strncmp(fieldtrname[i],"SIZE ",5) == 0) SIZEflag = 1;
   if (SIZEflag)
     {
      string = XmStringCreate("Points with SIZE radius",
               XmSTRING_DEFAULT_CHARSET);
      trsize = XtVaCreateManagedWidget("Trvals",
               xmToggleButtonGadgetClass,
               trdrawbox,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
         XmStringFree(string);
         XtAddCallback(trsize,XmNvalueChangedCallback,
                       traceflags,(caddr_t) 5);
     }
 
   /*  Create field buttons.  */
 
   string = XmStringCreate("Fields",XmSTRING_DEFAULT_CHARSET);
   trfldtxt = XtVaCreateManagedWidget("Trfldtxt",
              xmLabelWidgetClass,
              traceform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trdrawbox,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trfldtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   trfldswin = XmCreateScrolledWindow(traceform,"Trfldswin",wargs,i);
   XtManageChild(trfldswin);

   i=0;
   trfldbox = XmCreateRadioBox(trfldswin,"Trfldbox",wargs,i);
   XtManageChild(trfldbox);
   nflds = numvartr;
   for (i = 0; i < nflds; i++)
     {
      string = XmStringCreate(fieldtrname[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Trfldsbut%d",i);
      trfldsbut[i] = XtVaCreateManagedWidget(str,
                     xmToggleButtonGadgetClass,
                     trfldbox,
                     XmNlabelString, string,
                     XmNspacing, 1,
                     XmNselectColor, yellowpix,
                     XmNmarginHeight, 0,
                     XmNmarginWidth, 0,
                     NULL);
         XmStringFree(string);
         XtAddCallback(trfldsbut[i],XmNvalueChangedCallback,
                       select_trace_field,(caddr_t) i);

      if (i == 0) XtVaSetValues(trfldsbut[0],XmNset,TRUE,NULL);
     }
  
   string = XmStringCreate("Select",
            XmSTRING_DEFAULT_CHARSET);
   trselbut = XtVaCreateManagedWidget("Trselbut",
              xmPushButtonWidgetClass,
              traceform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trfldbox,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trselbut,XmNactivateCallback,settraceselect,NULL);
  
   string = XmStringCreate("History",
            XmSTRING_DEFAULT_CHARSET);
   trhist = XtVaCreateManagedWidget("Trhist",
            xmToggleButtonGadgetClass,
            traceform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, trselbut,
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
      XtAddCallback(trhist,XmNvalueChangedCallback,
                    tracehist,(caddr_t) 0);
 
   string = XmStringCreate("Back to",XmSTRING_DEFAULT_CHARSET);
   trhlab1 = XtVaCreateManagedWidget("Trhlab1",
             xmLabelWidgetClass,
             traceform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, trhist,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   trhiststoptxt = XtVaCreateManagedWidget("Trhiststoptxt",
                   xmTextFieldWidgetClass,
                   traceform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trhist,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, trhlab1,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 4,
                   XmNcolumns, 4,
                   XmNvalue, "   0",
                   NULL);
 
   string = XmStringCreate("Stride ",XmSTRING_DEFAULT_CHARSET);
   trhlab2 = XtVaCreateManagedWidget("Trhlab2",
             xmLabelWidgetClass,
             traceform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, trhiststoptxt,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   trhiststridetxt = XtVaCreateManagedWidget("Trhiststridetxt",
                     xmTextFieldWidgetClass,
                     traceform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, trhiststoptxt,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, trhlab2,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNmarginHeight, 0,
                     XmNmarginWidth, 1,
                     XmNmaxLength, 4,
                     XmNcolumns, 4,
                     XmNvalue, "   1",
                     NULL);
  
   string = XmStringCreate("Read History Files",
            XmSTRING_DEFAULT_CHARSET);
   trhrdbut = XtVaCreateManagedWidget("Trhrdbut",
              xmPushButtonGadgetClass,
              traceform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trhiststridetxt,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trhrdbut,XmNactivateCallback,trhistread,NULL);
  
   string = XmStringCreate("History Points On",
            XmSTRING_DEFAULT_CHARSET);
   trhistpts = XtVaCreateManagedWidget("Trhistpts",
               xmToggleButtonGadgetClass,
               traceform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trhrdbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(trhistpts,XmNvalueChangedCallback,
                    tracehist,(caddr_t) 1);
 
   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   trfhlab = XtVaCreateManagedWidget("Trfhlab",
             xmLabelWidgetClass,
             traceform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, trhistpts,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   maketraceselform(trselbut);
}


void settrfldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(fieldtrname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trfldsbut[ifld],XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(trfldsbut[ifld]);
   if (j && datalimpopflag) trfieldlimpop(ifld);
}


int histrdchksuffix()
{
  int i, chksuffix, fnsize, currno;
  char chkend3[4], chkend4[5], chkend5[6];
  char str[] = 
    "Error - invalid suffix in current file name for trace history read.";
  XmString string;

   /*  Check that filename suffix is a 3 or 4 digit number.  */
   suffixlen = 0;
   fnsize = strlen(filnam);
   strncpy(chkend3, filnam+(fnsize-3), 3);
   strncpy(chkend4, filnam+(fnsize-4), 4);
   strncpy(chkend5, filnam+(fnsize-5), 5);

   /*  See if the suffix is numeric and get  */
   /*  the no. of digits in the suffix.      */
   i = atoi(chkend3);
   chksuffix = 0;
   if (i >= 0 && i <= 999)
     {
      if (i > 0) chksuffix = 1;
      else if (strncmp(chkend3,"000",3) == 0) chksuffix = 1;
      if (chksuffix)
        {
         suffixlen = 3;
         currno = i;
        }
     }
   i = atoi(chkend4);
   chksuffix = 0;
   if (i >= 0 && i <= 9999)
     {
      if (i > 0) chksuffix = 1;
      else if (strncmp(chkend4,"0000",4) == 0) chksuffix = 1;
      if (chksuffix)
        {
         suffixlen = 4;
         currno = i;
        }
     }
   i = atoi(chkend5);
   chksuffix = 0;
   if (i >= 0 && i <= 99999)
     {
      if (i > 0) chksuffix = 1;
      else if (strncmp(chkend5,"00000",5) == 0) chksuffix = 1;
      if (chksuffix)
        {
         suffixlen = 5;
         currno = i;
        }
     }

   /*  If invalid suffix set error message and return.  */
   if (suffixlen == 0)
     {
      errorfile = 2;

      if (movieflag)
         printf(" %s\n",str);
      else
        {
         string = XmStringLtoRCreate(str,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         MyManageChild(errorform);
        }
      return 0;
     }

   prefixlen = fnsize - suffixlen;
   strncpy(histprefix, filnam, prefixlen);
   init_fileno = currno;

   return 1;
}


int histchkfile(int seqno)
{
  int ifound, chkend;
  char chkfilenm[200], str[10], magic[9];
  FILE *fin;

   /*  Check to see if the file for this seq. no. exists.  */
   if (suffixlen == 3)
      sprintf(str,"%03d",seqno);
   else if (suffixlen == 4)
      sprintf(str,"%04d",seqno);
   else
      sprintf(str,"%05d",seqno);
   strcpy(chkfilenm, histprefix);
   strcat(chkfilenm, str);

   ifound = 1;
   fin = fopen(chkfilenm,"r");
   if (fin == NULL)
      ifound = 0;
   else
     {
      fread(magic,sizeof(char), 8, fin);
      if (strncmp(magic,"gmvinput",8) != 0) ifound = 0;
      if (ifound)
        {
         chkend = chk_gmvend(fin);
         if (!chkend) ifound = 0;
        }
      fclose(fin);
     }
   return ifound;
}


void rdtracehist()
{
  int seqno, iend, ifound, ierr, tracers_in;
  char str[10], histfile[200], str3[220];
  XmString string;

   /*  Check for valid numeric suffix.  */
   if (histrdchksuffix() == 0)
     {
      tracehistflag = 0;
      XmToggleButtonGadgetSetState(trhist,tracehistflag,FALSE);      
      return;
     }

   /*  Check for end of sequence.  */
   iend = 0;
   seqno = init_fileno - tracehiststride;
   if (seqno < 0 || seqno < tracehiststop) iend = 1;
   while (iend == 0)
     {

      /*  Check for existence of previous file.  */
      ifound = histchkfile(seqno);

      if (ifound)
        {
         if (suffixlen == 3)
            sprintf(str,"%03d",seqno);
         else if (suffixlen == 4)
            sprintf(str,"%04d",seqno);
         else
            sprintf(str,"%05d",seqno);
         strcpy(histfile, histprefix);
         strcat(histfile, str);

         if (movieflag)
            printf(" Reading Particle history from %s\n",histfile);
         else
           {
            sprintf(str3," Reading Particles from:\n %s",histfile);
            string = XmStringLtoRCreate(str3,
                                        XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(trfhlab,XmNlabelString,string,NULL);
            XmStringFree(string);
            XmUpdateDisplay(trfhlab);
           }

         /*  Open gmv file.  */
         ierr = gmvread_open(histfile);
         if (ierr > 0)
           {
            fprintf(stderr,"Skipping invalid GMV input file %s\n",histfile);
           }

         /*  Find and read tracer data.  */
         if (ierr == 0)
           {
            tracers_in = 1;
            while (tracers_in)
              {
               gmvread_data();
               if (gmv_data.keyword == TRACERS)
                 {
                  savetracers();
                  if (gmv_data.datatype == ENDKEYWORD)
                     tracers_in = 0;
                 }
              }
           }
         gmvread_close();
        }
      seqno -= tracehiststride;
      if (seqno < 0 || seqno < tracehiststop) iend = 1;
      if (ntracehist == MAXTRHIST) iend = 1;
     }

   if (movieflag == 0)
     {
      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trfhlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(trfhlab);
     }
}


void checkSIZEflag()
{
  int i;
   /*  Check for SIZE field.  */
   for (i = 0; i < numvartr; i++)
      if (strcmp(fieldtrname[i],"SIZE") == 0 || 
          strncmp(fieldtrname[i],"SIZE ",5) == 0) SIZEflag = 1;
   if (tracerflag == 5 && SIZEflag == 0) tracerflag = 1;
}
