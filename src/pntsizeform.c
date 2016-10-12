#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#include <GL/gl.h>

static Widget ptsize1, ptsize2, ptsize3, ptsize4, ptsize5, ptsizetxt,
              ptsquare, ptround;


void pntsizeformpop()
{
  char str[10];
  XmString string;

   XmToggleButtonGadgetSetState(ptsize1,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize2,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize3,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize4,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize5,FALSE,FALSE);

   sprintf(str," ");
   XmTextFieldSetString(ptsizetxt,str);

   switch(pnt_size)
     {
      case(1):
         XmToggleButtonGadgetSetState(ptsize1,TRUE,FALSE);
         break;
      case(2):
         XmToggleButtonGadgetSetState(ptsize2,TRUE,FALSE);
         break;
      case(4):
         XmToggleButtonGadgetSetState(ptsize3,TRUE,FALSE);
         break;
      case(6):
         XmToggleButtonGadgetSetState(ptsize4,TRUE,FALSE);
         break;
      case(8):
         XmToggleButtonGadgetSetState(ptsize5,TRUE,FALSE);
         break;
      default:
         sprintf(str,"%d",pnt_size);
         XmTextFieldSetString(ptsizetxt,str);
     }

   XmToggleButtonGadgetSetState(ptsquare,FALSE,FALSE);
   if (pnt_shape == 0)
      XmToggleButtonGadgetSetState(ptsquare,TRUE,FALSE);
   XmToggleButtonGadgetSetState(ptround,FALSE,FALSE);
   if (pnt_shape == 1)
      XmToggleButtonGadgetSetState(ptround,TRUE,FALSE);

   MyManageChild(pntsizeform);
}

void ptsizeclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(pntsizeform);
}


void ptsize_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  float ptsize;
  char *tmpstr;
  XmString string;

   tmpstr = XmTextFieldGetString(ptsizetxt);
   i = atoi(tmpstr);
   if (i > 0) pnt_size = i;
   XtFree(tmpstr);
   pntsizeformpop();

   ptsize = (float)pnt_size;
   glPointSize(ptsize);

   if (pnt_shape == 1)
      glEnable(GL_POINT_SMOOTH);
   else
      glDisable(GL_POINT_SMOOTH);

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void sizeflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, szflag;
  char str[10];

   arg = (long int)client_data;
   szflag = (int)XmToggleButtonGadgetGetState(w);

   sprintf(str," ");
   XmTextFieldSetString(ptsizetxt,str);
//   XmUpdateDisplay(ptsizetxt);
   
   if (arg == 0 && szflag == 1)
     pnt_size = 1;
   
   if (arg == 1 && szflag == 1)
     pnt_size = 2;
   
   if (arg == 2 && szflag == 1)
     pnt_size = 4;
   
   if (arg == 3 && szflag == 1)
     pnt_size = 6;
   
   if (arg == 4 && szflag == 1)
     pnt_size = 8;
}


void shapeflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, shpflag;

   arg = (long int)client_data;
   shpflag = (int)XmToggleButtonGadgetGetState(w);
   
   if (arg == 0 && shpflag == 1)
     pnt_shape = 0;
   
   if (arg == 1 && shpflag == 1)
     pnt_shape = 1;
}


void getptsizetxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(ptsizetxt);
   pnt_size = atoi(tmpstr);
   if (pnt_size < 1) pnt_size = 1;
   XtFree(tmpstr);

   XmToggleButtonGadgetSetState(ptsize1,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize2,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize3,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize4,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ptsize5,FALSE,FALSE);
}



void makepntsizeform(parent)
 Widget parent;
{
  Widget ptsclose, ptsapply, ptsizebox, ptshapebox, 
         ptsdrawtxt, pshapetxt, psizelab;
  int i;
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Point Size",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   pntsizeform = XmCreateFormDialog(parent,"Pntsizeform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ptsclose = XtVaCreateManagedWidget("Ptsclose",
              xmPushButtonGadgetClass,
              pntsizeform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ptsclose,XmNactivateCallback,ptsizeclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ptsapply = XtVaCreateManagedWidget("Ptsapply",
              xmPushButtonGadgetClass,
              pntsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ptsclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ptsapply,XmNactivateCallback,ptsize_apply,
                    NULL);
 
   string = XmStringCreate("Pixels:",XmSTRING_DEFAULT_CHARSET);
   ptsdrawtxt = XtVaCreateManagedWidget("Ptsdrawtxt",
                xmLabelWidgetClass,
                pntsizeform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ptsapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, ptsdrawtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ptsizebox = XmCreateRadioBox(pntsizeform,"Ptsizebox",wargs,i);
   XtManageChild(ptsizebox);
  
   string = XmStringCreate("One",
            XmSTRING_DEFAULT_CHARSET);
   ptsize1 = XtVaCreateManagedWidget("Ptsize1",
             xmToggleButtonGadgetClass,
             ptsizebox,
             XmNlabelString, string,
             XmNset, TRUE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptsize1,XmNvalueChangedCallback,
                    sizeflags,(caddr_t) 0);

   string = XmStringCreate("Two",
            XmSTRING_DEFAULT_CHARSET);
   ptsize2 = XtVaCreateManagedWidget("Ptsize2",
             xmToggleButtonGadgetClass,
             ptsizebox,
             XmNlabelString, string,
             XmNset, TRUE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptsize2,XmNvalueChangedCallback,
                    sizeflags,(caddr_t) 1);
 
   string = XmStringCreate("Four",
            XmSTRING_DEFAULT_CHARSET);
   ptsize3 = XtVaCreateManagedWidget("Ptsize3",
             xmToggleButtonGadgetClass,
             ptsizebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptsize3,XmNvalueChangedCallback,
                    sizeflags,(caddr_t) 2);
 
   string = XmStringCreate("Six",
            XmSTRING_DEFAULT_CHARSET);
   ptsize4 = XtVaCreateManagedWidget("Ptsize4",
             xmToggleButtonGadgetClass,
             ptsizebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptsize4,XmNvalueChangedCallback,
                    sizeflags,(caddr_t) 3);
  
   string = XmStringCreate("Eight",
            XmSTRING_DEFAULT_CHARSET);
   ptsize5 = XtVaCreateManagedWidget("Ptsize5",
             xmToggleButtonGadgetClass,
             ptsizebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptsize5,XmNvalueChangedCallback,
                    sizeflags,(caddr_t) 4);
  
   string = XmStringCreate("Enter Size:",
            XmSTRING_DEFAULT_CHARSET);
   psizelab = XtVaCreateManagedWidget("Psizelab",
              xmLabelWidgetClass,
              pntsizeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ptsizebox,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   ptsizetxt = XtVaCreateManagedWidget("Ptsizetxt",
               xmTextFieldWidgetClass,
               pntsizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ptsizebox,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, psizelab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 4,
               XmNcolumns, 4,
               XmNvalue, "",
               NULL);
      XtAddCallback(ptsizetxt,XmNactivateCallback,getptsizetxt,NULL);
 
   /*  Create shape buttons.  */
 
   string = XmStringCreate("Shape:",XmSTRING_DEFAULT_CHARSET);
   pshapetxt = XtVaCreateManagedWidget("Pshapetxt",
               xmLabelWidgetClass,
               pntsizeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, psizelab,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, pshapetxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ptshapebox = XmCreateRadioBox(pntsizeform,"ptshapebox",wargs,i);
   XtManageChild(ptshapebox);
  
   string = XmStringCreate("Square (fast)",
            XmSTRING_DEFAULT_CHARSET);
   ptsquare = XtVaCreateManagedWidget("Ptsquare",
              xmToggleButtonGadgetClass,
              ptshapebox,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(ptsquare,XmNvalueChangedCallback,
                    shapeflags,(caddr_t) 0);
  
   string = XmStringCreate("Round (slow)",
            XmSTRING_DEFAULT_CHARSET);
   ptround = XtVaCreateManagedWidget("Ptround",
             xmToggleButtonGadgetClass,
             ptshapebox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ptround,XmNvalueChangedCallback,
                    shapeflags,(caddr_t) 1);
}
