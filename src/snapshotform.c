#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushBG.h>
#include <Xm/LabelG.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
 
#include <math.h>
 
static Widget jsobj, fsobj, psobj;
static short snapfullsize = 0, ps_lines = 0;
 

void snapshotformpop()
{
   if (ps_lines == 0)
      snapfullsize = 1;
   
   XmToggleButtonGadgetSetState(jsobj,0,FALSE);
   XmToggleButtonGadgetSetState(fsobj,0,FALSE);
   if (jpegflag)
      XmToggleButtonGadgetSetState(jsobj,snapfullsize,FALSE);
   else
      XmToggleButtonGadgetSetState(fsobj,snapfullsize,FALSE);
   XmToggleButtonGadgetSetState(psobj,ps_lines,FALSE);

   XtSetSensitive(psobj,TRUE);
   if (displistflag)
      XtSetSensitive(psobj,FALSE);

   MyManageChild(snapshotform);
}


void snapshotstuff(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string, string2, string3;
 
   arg = (long int)client_data;
 
   if (arg == 0)
     {
      snapfullsize = (int)XmToggleButtonGadgetGetState(w);
      if (snapfullsize == 1)
        {
         jpegflag = 1;
         ps_lines = 0;
         p_s_flag = 0;
         strcpy(fspattern[3],"*jpg");
         strcpy(fspattern[4],"*jpg*");
         strcpy(fspattern[5],"*jpg*");
        }
     }
 
   if (arg == 1)
     {
      snapfullsize = (int)XmToggleButtonGadgetGetState(w);
      if (snapfullsize == 1)
        {
         jpegflag = 0;
         ps_lines = 0;
         p_s_flag = 0;
         strcpy(fspattern[3],"*rgb");
         strcpy(fspattern[4],"*rgb*");
         strcpy(fspattern[5],"*rgb*");
        }
     }

   if (arg == 2)
     {
      if (snapfullsize == 0 && ps_lines == 0)
        {
         snapfullsize = 1;
         if (jpegflag)
           XmToggleButtonGadgetSetState(jsobj,TRUE,FALSE);
         else
           XmToggleButtonGadgetSetState(fsobj,TRUE,FALSE);
        }
      if (snapfullsize == 1) snapshotsize = 3;
      if (ps_lines == 1)
        {
         p_s_flag = 2;
         if (displistflag)
           {
            p_s_flag = 0;
            ps_lines = 0;
            XmToggleButtonGadgetSetState(psobj,FALSE,FALSE);
            snapshotformpop();
           }
        }

      fileselflag = SNAPSHOT;
      string = XmStringCreate("Save Snapshot",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[SNAPSHOT],
                               XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[SNAPSHOT],
                               XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2,
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[SNAPSHOT])
        {
         string = XmStringCreate(snapshotfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }

   if (arg == 3)
     {
      p_s_flag = 0;
      XtUnmanageChild(snapshotform);
     }
 
   if (arg == 4)
     {
      ps_lines = (int)XmToggleButtonGadgetGetState(w);
      if (ps_lines == 1)
        {
         snapfullsize = 0;
         if (displistflag)
           {
            ps_lines = 0;
            XmToggleButtonGadgetSetState(psobj,FALSE,FALSE);
            snapshotformpop();
           }
        }
     }
 
}
 
 
void makesnapshotform(parent)
 Widget parent;
{
  Widget snapclose, snapdoit, snapbox, snapmsg;
  int i;
  Arg wargs[10];
  XmString string;
 
   string = XmStringCreate("Snap Shot",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   /* i++; XtSetArg(wargs[i], XmNcolormap, top_cmap); */
   i++;
   snapshotform = XmCreateFormDialog(parent,"SnapShot",wargs,i);
      XmStringFree(string);
 
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNorientation, XmHORIZONTAL);
   i++;
   snapbox = XmCreateRadioBox(snapshotform,"Snapbox",wargs,i);
   XtManageChild(snapbox);
 
   string = XmStringCreate("JPEG",XmSTRING_DEFAULT_CHARSET);
   jsobj = XtVaCreateManagedWidget("Jsobj",
           xmToggleButtonGadgetClass,
           snapbox,
           XmNlabelString, string,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(jsobj,XmNvalueChangedCallback,snapshotstuff,
                    (caddr_t) 0);
 
   string = XmStringCreate("SGI RGB",XmSTRING_DEFAULT_CHARSET);
   fsobj = XtVaCreateManagedWidget("Fsobj",
           xmToggleButtonGadgetClass,
           snapbox,
           XmNlabelString, string,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(fsobj,XmNvalueChangedCallback,snapshotstuff,
                    (caddr_t) 1);
 
 
   string = XmStringCreate("PS Lines",XmSTRING_DEFAULT_CHARSET);
   psobj = XtVaCreateManagedWidget("Psobj",
           xmToggleButtonGadgetClass,
           snapbox,
           XmNlabelString, string,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(psobj,XmNvalueChangedCallback,snapshotstuff,
                    (caddr_t) 4);
 
   string = XmStringCreate("Snap",XmSTRING_DEFAULT_CHARSET);
   snapdoit = XtVaCreateManagedWidget("Snapdoit",
              xmPushButtonGadgetClass,
              snapshotform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, snapbox,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(snapdoit,XmNactivateCallback,snapshotstuff,
                    (caddr_t) 2);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   snapclose = XtVaCreateManagedWidget("Snapclose",
              xmPushButtonGadgetClass,
              snapshotform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, snapbox,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, snapdoit,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(snapclose,XmNactivateCallback,snapshotstuff,
                    (caddr_t) 3);

   string = XmStringLtoRCreate(
                     "Note: Display List must\nbe off for PS Lines",
                           XmSTRING_DEFAULT_CHARSET);
   snapmsg = XtVaCreateManagedWidget("Snapmsg",
              xmLabelGadgetClass,
              snapshotform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, snapclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
}
