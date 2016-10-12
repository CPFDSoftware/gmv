#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>
#include <time.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget ardfirst, ardlast, ardstride, arddir[3],
              ardtime, ardstart, ardpause, ardresume, ardquit,
              ardsnapform, ardsnapfn, ardsnapon, ardsnapno,
              ardsnapbut, ardstepback, ardstepnext;
static int curr_seqno, prefixlen, suffixlen, init_fileno,
           autofirst, autolast, autostride, autodir = 0,
           autosnapflag = 0, autosnaptype = 0, snap_seqno,
           gridrd = 0, seqbeg, seqend;
static char autoprefix[200], autosnapprefix[80];
double autotimechk = 0.;
time_t time1;


void autordformpop(int arg)
{
  int i;
  int autordchksuffix();

   /*  Set grid type read, 0 - new grid, 1 - same grid.  */
   gridrd = arg;

   /*  If auto read not active, get file  */ 
   /*  prefix and check filename suffix.  */
   if (autordflag == 0)
     {
      if (autordchksuffix() == 0) return;

      XtSetSensitive(ardstart,TRUE);
      XtSetSensitive(ardpause,FALSE);
      XtSetSensitive(ardstepback,FALSE);
      XtSetSensitive(ardstepnext,FALSE);
      XtSetSensitive(ardresume,FALSE);
      XtSetSensitive(ardquit,FALSE);
     }

   for (i = 0; i < 3; i++)
     {
      XmToggleButtonGadgetSetState(arddir[i],FALSE,FALSE);
      if (autodir == i) 
         XmToggleButtonGadgetSetState(arddir[i],TRUE,FALSE);
     }

   XtSetSensitive(ardsnapbut,TRUE);
   if (autosnapflag == 1)
     {
      XmToggleButtonGadgetSetState(ardsnapbut,TRUE,FALSE);
      XmToggleButtonGadgetSetState(ardsnapon,TRUE,FALSE);
     }
   if (autosnapflag == 0)
     {
      XmToggleButtonGadgetSetState(ardsnapbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(ardsnapon,FALSE,FALSE);
     }
   
   MyManageChild(autordform);
}


void autoclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(ardsnapform);
   XtUnmanageChild(autordform);
}


void auto_set_dir(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, teston;

   arg = (long int)client_data;
   teston = XmToggleButtonGadgetGetState(w);

   if (arg == 0 && teston == 1)
      autodir = 0;
   if (arg == 1 && teston == 1)
      autodir = 1;
   if (arg == 2 && teston == 1)
      autodir = 2;
}


void start_autord(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ierr;
  char *tmpstr, str2[10];
  int autordchksuffix(), autord_chk_input();

   /*  Get file name prefix and check that  */
   /*  the file name suffix is numeric.     */
   if (autordchksuffix() == 0) return;

   /*  Check first, last, stride input.  */
   autordflag = 0;
   ierr = autord_chk_input();

   if (ierr) return;

   /*  If auto snapshots wanted, get the data.  */
   if (autosnapflag)
     {
      tmpstr = XmTextFieldGetString(ardsnapno);
      snap_seqno = atoi(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(ardsnapfn);
      strcpy(autosnapprefix,tmpstr);
      XtFree(tmpstr);
     }

   autordflag = 1;
   rdgmvflag = 1;
   if (gridrd == 1) rdgmvflag = 2;
   curr_seqno = autofirst - autostride;
   /* if (autofirst == init_fileno) curr_seqno = autofirst; */
   if (autofirst == init_fileno)
     {
      curr_seqno = autofirst;

      /*  If autosnapshots wanted, set file name and flags.  */
      if (autosnapflag)
        {
         sprintf(str2,"%04d",snap_seqno);
         snap_seqno++;
         strcpy(snapshotfilnam,autosnapprefix);
         strcat(snapshotfilnam, str2);
         if (jpegflag)
            strcat(snapshotfilnam, ".jpg");
         else
            strcat(snapshotfilnam, ".rgb");
         XmTextFieldSetString(ardsnapno,str2);
         XmUpdateDisplay(ardsnapno);

         snapshotsize = autosnaptype + 1;
         drawnew3dflag = 1;
         fastdraw = 0;
         drawit();
        }
     }

   strncpy(autoprefix, filnam, prefixlen);
   autoprefix[prefixlen] = (char) 0;

   tmpstr = XmTextFieldGetString(ardtime);
   autotimechk = atof(tmpstr);
   if (autotimechk < .001) autotimechk = .001;
   XtFree(tmpstr);

   XtSetSensitive(ardstart,FALSE);
   XtSetSensitive(ardpause,TRUE);
   XtSetSensitive(ardstepback,FALSE);
   XtSetSensitive(ardstepnext,FALSE);
   XtSetSensitive(ardresume,FALSE);
   XtSetSensitive(ardquit,TRUE);

   XtUnmanageChild(ardsnapform);
   XtSetSensitive(ardsnapbut,FALSE);

   time1 = time(NULL);
}


void pause_autord(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   autordflag = 2;   

   XtSetSensitive(ardstart,TRUE);
   XtSetSensitive(ardpause,FALSE);
   XtSetSensitive(ardstepback,TRUE);
   XtSetSensitive(ardstepnext,TRUE);
   XtSetSensitive(ardresume,TRUE);
   XtSetSensitive(ardquit,TRUE);

   XtSetSensitive(ardsnapbut,TRUE);
}


void step_autord(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ierr;
  int ifound, idir;
  char str[10];

   arg = (long int)client_data;

   /*  Check first, last, stride input.  */
   ierr = autord_chk_input();
   if (ierr) return;

   /*  Increment sequence (back or next) depending  */
   /*  on arg and check for end of sequence.        */
   if (arg == 0) idir = -1;
   else idir = 1;

   /*  Get the next sequence file.  */
   ifound = autogetnextfile(idir);

   /*  If next sequence file exists, read the file.  */
   if (ifound)
     {
      if (suffixlen == 3)
         sprintf(str,"%03d",curr_seqno);
      else if (suffixlen == 4)
         sprintf(str,"%04d",curr_seqno);
      else
         sprintf(str,"%05d",curr_seqno);
      strcpy(filnam, autoprefix);
      strcat(filnam, str);

      readfile();
     }
}


void resume_autord(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int ierr;


   /*  Check first, last, stride input.  */
   ierr = autord_chk_input();

   if (ierr) return;

   /*  If auto snapshots wanted, get the data.  */
   if (autosnapflag)
     {
      tmpstr = XmTextFieldGetString(ardsnapno);
      snap_seqno = atoi(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(ardsnapfn);
      strcpy(autosnapprefix,tmpstr);
      XtFree(tmpstr);
     }

   tmpstr = XmTextFieldGetString(ardtime);
   autotimechk = atof(tmpstr);
   if (autotimechk < .001) autotimechk = .001;
   XtFree(tmpstr);

   autordflag = 1;

   XtSetSensitive(ardstart,FALSE);
   XtSetSensitive(ardpause,TRUE);
   XtSetSensitive(ardstepback,FALSE);
   XtSetSensitive(ardstepnext,FALSE);
   XtSetSensitive(ardresume,FALSE);
   XtSetSensitive(ardquit,TRUE);

   XtUnmanageChild(ardsnapform);
   XtSetSensitive(ardsnapbut,FALSE);
}


void quit_autord(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   autordflag = 0;
   rdgmvflag = 0;
   autosnapflag = 0;

   XtSetSensitive(ardstart,TRUE);
   XtSetSensitive(ardpause,FALSE);
   XtSetSensitive(ardstepback,FALSE);
   XtSetSensitive(ardstepnext,FALSE);
   XtSetSensitive(ardresume,FALSE);
   XtSetSensitive(ardquit,FALSE);

   XtSetSensitive(ardsnapbut,TRUE);
}


int autord_chk_input()
{
  int i, j, k, ierr;
  char *tmpstr;
  XmString string;

   /*  Read and check text input (First, Last, Stride).  */

   /*  Check that first is less than last.  */
   autordflag = 0;
   ierr = 0;
   tmpstr = XmTextFieldGetString(ardfirst);
   i = atoi(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(ardlast);
   j = atoi(tmpstr);
   XtFree(tmpstr);
   if (i >= j)
     {
      ierr = 1;
      errorfile = 0;
      string = XmStringLtoRCreate(
                  "Error - Last must be greater than First",
                                  XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      MyManageChild(errorform);
      return ierr;
     }

   /*  Check that stride > 0.  */
   tmpstr = XmTextFieldGetString(ardstride);
   k = atoi(tmpstr);
   XtFree(tmpstr);
   if (k <= 0)
     {
      ierr = 1;
      errorfile = 0;
      string = XmStringLtoRCreate(
                  "Error - Stride must be greater than 0",
                                  XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      MyManageChild(errorform);
      return ierr;
     }

   autofirst = i;
   autolast = j;
   autostride = k;
   seqbeg = i;
   seqend = j;
 
   if (autodir == 2)
     {
      autofirst = j;
      autolast = i;
      autostride = -k;
     }

   return 0;
}


void ardsnapformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{

   if (autosnapflag == 0) 
      XmToggleButtonGadgetSetState(ardsnapbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(ardsnapbut,TRUE,FALSE);
   XmUpdateDisplay(ardsnapbut);

   MyManageChild(ardsnapform);
}


void autosnapclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(ardsnapform);
}


void auto_set_snaptype(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, teston;

   arg = (long int)client_data;
   teston = XmToggleButtonGadgetGetState(w);

   if (arg == 0 && teston == 1)
      autosnaptype = 0;
   if (arg == 1 && teston == 1)
      autosnaptype = 1;
}


void auto_snap_on(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   autosnapflag = XmToggleButtonGadgetGetState(w);

   if (autosnapflag == 0)
     {
      XmToggleButtonGadgetSetState(ardsnapbut,FALSE,FALSE);
      snapshotsize = 0;
     }
   else
     {
      XmToggleButtonGadgetSetState(ardsnapbut,TRUE,FALSE);
      tmpstr = XmTextFieldGetString(ardsnapno);
      snap_seqno = atoi(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(ardsnapfn);
      strcpy(autosnapprefix,tmpstr);
      XtFree(tmpstr);
     }
}


void makeautordform(parent)
 Widget parent;
{
  Widget ardclose, ardlabf, ardlabl, ardlabs, ardlabd, ardbox, 
         ardlabt, ardsnapclose, ardsnaplab1, ardsnaplabs, 
         ardsnaplab2;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 


   string = XmStringCreate("Auto Read",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   autordform = XmCreateFormDialog(parent,"Autordform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ardclose = XtVaCreateManagedWidget("Ardclose",
              xmPushButtonGadgetClass,
              autordform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ardclose,XmNactivateCallback,autoclose,NULL);

   string = XmStringCreate("First: ",XmSTRING_DEFAULT_CHARSET);
   ardlabf = XtVaCreateManagedWidget("Ardlabf",
             xmLabelGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string); 

   ardfirst = XtVaCreateManagedWidget("Ardfirst",
              xmTextFieldWidgetClass,
              autordform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ardclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ardlabf,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 5,
              XmNcolumns, 5,
              XmNvalue, "0",
              NULL);
      /* XtAddCallback(ardfirst,XmNactivateCallback,NULL,0); */
 
   string = XmStringCreate("Last: ",XmSTRING_DEFAULT_CHARSET);
   ardlabl = XtVaCreateManagedWidget("Ardlabl",
             xmLabelGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardfirst,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string); 

   ardlast = XtVaCreateManagedWidget("Ardlast",
             xmTextFieldWidgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardfirst,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, ardlabl,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 5,
             XmNcolumns, 5,
             XmNvalue, "99999",
             NULL);
      /* XtAddCallback(ardlast,XmNactivateCallback,NULL,0); */
 
   string = XmStringCreate("Stride: ",XmSTRING_DEFAULT_CHARSET);
   ardlabs = XtVaCreateManagedWidget("Ardlabs",
             xmLabelGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardlast,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string); 

   ardstride = XtVaCreateManagedWidget("Ardstride",
               xmTextFieldWidgetClass,
               autordform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ardlast,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, ardlabs,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 4,
               XmNcolumns, 4,
               XmNvalue, "1",
               NULL);
      /* XtAddCallback(ardstride,XmNactivateCallback,NULL,0); */
 
   string = XmStringCreate("Direction:",XmSTRING_DEFAULT_CHARSET);
   ardlabd = XtVaCreateManagedWidget("Ardlabd",
             xmLabelGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardstride,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string); 

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, ardlabd);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 10);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ardbox = XmCreateRadioBox(autordform,"Ardbox",wargs,i);
   XtManageChild(ardbox);

   for (i = 0; i < 3; i++)
     {
      if (i == 0)
         string = XmStringCreate("Forwards",
                                 XmSTRING_DEFAULT_CHARSET);
      if (i == 1)
         string = XmStringCreate("Forward to Latest",
                                 XmSTRING_DEFAULT_CHARSET);
      if (i == 2)
         string = XmStringCreate("Backwards",
                                 XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"arddir%d",i);
      arddir[i] = XtVaCreateManagedWidget(str,
                  xmToggleButtonGadgetClass,
                  ardbox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  NULL);
         XmStringFree(string);
         XtAddCallback(arddir[i],XmNvalueChangedCallback,
                       auto_set_dir,(caddr_t) i);
     }
 
   string = XmStringLtoRCreate("Search time\n (sec.): ",
                           XmSTRING_DEFAULT_CHARSET);
   ardlabt = XtVaCreateManagedWidget("Ardlabt",
             xmLabelGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardbox,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string); 

   ardtime = XtVaCreateManagedWidget("Ardtime",
              xmTextFieldWidgetClass,
              autordform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ardbox,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ardlabt,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 5,
              XmNcolumns, 5,
              XmNvalue, "0.001",
              NULL);
      /* XtAddCallback(ardtime,XmNactivateCallback,NULL,0); */

   string = XmStringCreate("Auto Snapshots",
                            XmSTRING_DEFAULT_CHARSET);
   ardsnapbut = XtVaCreateManagedWidget("Ardsnapbut",
                xmToggleButtonGadgetClass,
                autordform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ardlabt,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(ardsnapbut,XmNvalueChangedCallback,
                    ardsnapformpop,0);

   string = XmStringCreate("Start",
            XmSTRING_DEFAULT_CHARSET);
   ardstart = XtVaCreateManagedWidget("Ardstart",
              xmPushButtonGadgetClass,
              autordform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ardsnapbut,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ardstart,XmNactivateCallback,
                    start_autord,NULL);
 
   string = XmStringCreate("Pause",
            XmSTRING_DEFAULT_CHARSET);
   ardpause = XtVaCreateManagedWidget("Ardpause",
              xmPushButtonGadgetClass,
              autordform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ardstart,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ardpause,XmNactivateCallback,
                    pause_autord,NULL);
 
   string = XmStringCreate("Step Back",
            XmSTRING_DEFAULT_CHARSET);
   ardstepback = XtVaCreateManagedWidget("Ardstepback",
                 xmPushButtonGadgetClass,
                 autordform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ardpause,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ardstepback,XmNactivateCallback,
                    step_autord,(caddr_t) 0);
 
   string = XmStringCreate("Step Next",
            XmSTRING_DEFAULT_CHARSET);
   ardstepnext = XtVaCreateManagedWidget("Ardstepnext",
                 xmPushButtonGadgetClass,
                 autordform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ardstepback,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ardstepnext,XmNactivateCallback,
                    step_autord,(caddr_t) 1);
 
   string = XmStringCreate("Resume",
            XmSTRING_DEFAULT_CHARSET);
   ardresume = XtVaCreateManagedWidget("Ardresume",
               xmPushButtonGadgetClass,
               autordform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ardstepnext,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ardresume,XmNactivateCallback,
                    resume_autord,NULL);
 
   string = XmStringCreate("Quit",
            XmSTRING_DEFAULT_CHARSET);
   ardquit = XtVaCreateManagedWidget("Ardquit",
             xmPushButtonGadgetClass,
             autordform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ardresume,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(ardquit,XmNactivateCallback,
                    quit_autord,NULL);


   /*  Create auto snapshot form.  */

   string = XmStringCreate("Auto Snapshots",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   ardsnapform = XmCreateFormDialog(autordform,"Ardsnapform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ardsnapclose = XtVaCreateManagedWidget("Ardsnapclose",
                  xmPushButtonGadgetClass,
                  ardsnapform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(ardsnapclose,XmNactivateCallback,autosnapclose,NULL);
 
   string = XmStringLtoRCreate("File Prefix:",
                           XmSTRING_DEFAULT_CHARSET);
   ardsnaplab1 = XtVaCreateManagedWidget("Ardsnaplab1",
                 xmLabelGadgetClass,
                 ardsnapform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ardsnapclose,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   ardsnapfn = XtVaCreateManagedWidget("Ardsnapfn",
               xmTextFieldWidgetClass,
               ardsnapform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ardsnaplab1,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmaxLength, 80,
               XmNcolumns, 40,
               XmNvalue, "rsnapgmv",
               NULL);

   string = XmStringCreate("Suffix No.: ",XmSTRING_DEFAULT_CHARSET);
   ardsnaplabs = XtVaCreateManagedWidget("Ardsnaplabs",
                 xmLabelGadgetClass,
                 ardsnapform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ardsnapfn,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   ardsnapno = XtVaCreateManagedWidget("Ardsnapno",
               xmTextFieldWidgetClass,
               ardsnapform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ardsnapfn,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, ardsnaplabs,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 4,
               XmNcolumns, 4,
               XmNvalue, "0",
               NULL);

   string = XmStringCreate("On",
                            XmSTRING_DEFAULT_CHARSET);
   ardsnapon = XtVaCreateManagedWidget("Ardsnapon",
               xmToggleButtonGadgetClass,
               ardsnapform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ardsnapno,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               NULL);
      XmStringFree(string);
      XtAddCallback(ardsnapon,XmNvalueChangedCallback,
                    auto_snap_on,0);
 
   string = XmStringCreate(
                         "Note - Be sure the drawing window is clear.",
                          XmSTRING_DEFAULT_CHARSET);
   ardsnaplab2 = XtVaCreateManagedWidget("Ardsnaplab2",
                 xmLabelGadgetClass,
                 ardsnapform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ardsnapon,
                 XmNtopOffset, 20,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 
}


int autordchksuffix()
{
  int i, chksuffix, fnsize, currno, j;
  char chkend3[4], chkend4[5], chkend5[6];
  char str[] = 
    "Error - invalid suffix in current file name for auto read.";
  char *strnums = "0123456789";
  XmString string;

   /*  Check that filename suffix is a 3, 4 or 5 digit number.  */
   suffixlen = 0;
   fnsize = strlen(filnam);
   strncpy(chkend3, filnam+(fnsize-3), 3);
   strncpy(chkend4, filnam+(fnsize-4), 4);
   strncpy(chkend5, filnam+(fnsize-5), 5);
   *(chkend3 + 3) = (char) 0;  // Add terminating character;
   *(chkend4 + 4) = (char) 0;  // Add terminating character;
   *(chkend5 + 5) = (char) 0;  // Add terminating character;

   /*  See if the suffix is numeric and get  */
   /*  the no. of digits in the suffix.      */
   i = atoi(chkend3);
   chksuffix = 0;
   if (i >= 0 && i <= 999)
     {
      if (i > 0) chksuffix = 1;
      else if (strncmp(chkend3,"000",3) == 0) chksuffix = 1;
      if (strspn(chkend3,strnums) != strlen(chkend3)) chksuffix = 0;
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
      if (strspn(chkend4,strnums) != strlen(chkend4)) chksuffix = 0;
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
      if (strspn(chkend5,strnums) != strlen(chkend5)) chksuffix = 0;
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
      string = XmStringLtoRCreate(str,
                                  XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      MyManageChild(errorform);
      return 0;
     }

   prefixlen = fnsize - suffixlen;
   strncpy(autoprefix, filnam, prefixlen);
   autoprefix[prefixlen] = (char) 0;
   init_fileno = currno;

   return 1;
}


int autochkfile(int seqno)
{
  int ifound, chkend;
  char chkfilenm[300], str[10], magic[9];
  FILE *fin;

   /*  Check to see if the file for this seq. no. exists.  */
   if (suffixlen == 3)
      sprintf(str,"%03d",seqno);
   else if (suffixlen == 4)
      sprintf(str,"%04d",seqno);
   else
      sprintf(str,"%05d",seqno);
   strcpy(chkfilenm, autoprefix);
   strcat(chkfilenm, str);

   ifound = 1;
   fin = fopen(chkfilenm,"r");
   if (fin == NULL)
      ifound = 0;
   else
     {
      fread(magic,sizeof(char), 8, fin);
      if (strncmp(magic,"gmvinput",8) != 0 &&
          strncmp(magic,"eulerian",8) != 0) ifound = 0;
      if (ifound && strncmp(magic,"gmvinput",8) == 0)
        {
         chkend = chk_gmvend(fin);
         if (!chkend) ifound = 0;
        }
      fclose(fin);
     }
   return ifound;
}


int autogetnextfile(int dirflag)
{
  int seqno, stride, ifound, isearch, iend, icount;

   /*  Find the next available gmv file, even if   */
   /*  it's not the next file of the sequence.     */
   /*  input:  dirflag - direction flag, +1 or -1  */
   /*  return: 0 - no file found.                  */
   /*          1 - file found.                     */
   /*          curr_seqno set to the file found.   */
   /*  Note, the search for the next file is now   */
   /*  limited to 50 sequences.                    */

   stride = dirflag * autostride;
   seqno = curr_seqno + stride;
   iend = 0;
   if (autodir < 2 && seqno > autolast) iend = 1;
   if (autodir == 2 && seqno < autolast) iend = 1;
   if (seqno > seqend) iend = 1;
   if (seqno < seqbeg) iend = 1;
   if (iend) return 0;

   isearch = 1;
   ifound = 0;
   icount = 0;
   while (isearch)
     {
      ifound = autochkfile(seqno);
      if (ifound)
        {
         isearch = 0;
         curr_seqno = seqno;
        }
      else
        {
         seqno = seqno + stride;
         if (autodir < 2 && seqno > autolast) isearch = 0;
         if (autodir == 2 && seqno < autolast) isearch = 0;
         if (seqno > seqend) isearch = 0;
         if (seqno < seqbeg) isearch = 0;
         icount++;
         if (icount > 50) isearch = 0;
        }
     }
   return ifound;
}


void autoread()
{
  int i, seqno, iend, ifound, lastseq, idir;
  char str[10], str2[10];
  time_t time2;
  double duration;
  static int usetime = 1, itime = 0;
 
   /*  See if search time reached.  */
   time2 = time(NULL);
   duration = difftime(time2,time1);
#ifdef MACX
   /*  Add this for Mac OS X, since time doesn't work.  */
   itime++;
   if (itime < 1000) return;
   else
     {
      itime = 0;
     }
#else

   /*  If the next file is not found, use a longer pause before  */
   /*  looking for the next file so that events can be caught.   */
   if (usetime)
     {
      time2 = time(NULL);
      duration = difftime(time2,time1);
      if (duration < autotimechk) return;
     }
   else
     {
      for (iend = 0; iend < 10000; iend++)
        i = iend;
      itime++;
      if (itime < 10000) return;
      else
        {
         itime = 0;
        } 
     }
#endif

   /*  Check for end of sequence.  */
   iend = 0;
   seqno = curr_seqno + autostride;
   if (autodir < 2 && seqno > autolast) iend = 1;
   if (autodir == 2 && seqno < autolast) iend = 1;
   if (iend)
     {
      pause_autord(NULL,NULL,NULL);
      return;
     }

   /*  If pause button pressed, return.  */
   if (autordflag == 2) return;

   /*  Check for existence of next sequence file.  */
   if (autodir == 0 || autodir == 2)
     {
      idir = 1;
      ifound = autogetnextfile(idir);
     }

   /*  Look for lastest sequence file.  */
   if (autodir == 1)
     {
      lastseq = -1;
      seqno = curr_seqno + autostride;
      iend = 0;
      while (iend == 0)
        {
         ifound = autochkfile(seqno);
         if (ifound) lastseq = seqno;
         else iend = 1;
         seqno += autostride;       
        }
      if (lastseq > 0)
        {
         ifound = 1;
         seqno = lastseq;
        }
      if (ifound) curr_seqno = seqno;
     }

   /*  If next sequence file exists, read the file.  */
   if (ifound)
     {
      usetime = 1;
      if (suffixlen == 3)
         sprintf(str,"%03d",curr_seqno);
      else if (suffixlen == 4)
         sprintf(str,"%04d",curr_seqno);
      else
         sprintf(str,"%05d",curr_seqno);
      strcpy(filnam, autoprefix);
      strcat(filnam, str);

      /*  If autosnapshots wanted, set file name and flags.  */
      if (autosnapflag)
        {
         sprintf(str2,"%04d",snap_seqno);
         snap_seqno++;
         strcpy(snapshotfilnam,autosnapprefix);
         strcat(snapshotfilnam, str2);
         if (jpegflag)
            strcat(snapshotfilnam, ".jpg");
         else
            strcat(snapshotfilnam, ".rgb");
         XmTextFieldSetString(ardsnapno,str2);
         XmUpdateDisplay(ardsnapno);

         snapshotsize = autosnaptype + 1;
        }

      readfile();

      XSync(dpy,0);
      XFlush(dpy);

      /*  Set the new check time.  */
      time1 = time(NULL);
     }

   /*  If next sequence does not exist and the current  */
   /*  sequence number is less than the current file    */
   /*  number, increment the current sequence number.   */
   else
     {
      if (curr_seqno < init_fileno)
         curr_seqno = seqno;
      else usetime = 0;
     }
}
