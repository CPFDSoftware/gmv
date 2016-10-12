#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>

#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
 
static Widget matsbut[MAXMATS], objneworder[MAXMATS],
       objcurrorder[MAXMATS];
static int matorderlast = -1;
static char matselected[MAXMATS] = {MAXMATS * 0};
static int neworder[MAXMATS];

void setmatorderbuts();


void matorderpop()
{
   setmatorderbuts();
   MyManageChild(matorderform);
}

void matorderclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(matorderform);
   setmatorderbuts();
}
 
void matorderdoit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
 
   if (nmat_order > matorderlast + 1)
     {
      for (i = 0; i < MAXMATS; i++)
        {
         if (mat_has_active_polys[i] > 0)
           {
            if ((int)matselected[i] == 0)
              {
               matselected[i] = 1;
               matorderlast++;
               neworder[matorderlast] = i;
              }
           }
        }
     }
 
   for (i = 0; i < nmat_order; i++) mat_order[i] = neworder[i]; 
   setmatorderbuts();
 
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}
 
void matorderundo(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
 
   for (i = 0; i < nmat_order; i++)
     {
      neworder[i] = 0;
      sprintf(str,"%8d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objneworder[i],XmNlabelString,string,
                                   XmNbackground,whitepix,NULL);
      XmStringFree(string);
     }
   matorderlast = -1;
   for (i = 0; i < MAXMATS; i++) matselected[i] = 0;
   for (i = 0; i < MAXMATS; i++)
     {
      if (mat_has_active_polys[i] > 0)
         XtVaSetValues(matsbut[i],XmNset,FALSE,NULL);
     }
}
 
void matorder_select_mat(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int j;
  char str[40];
  XmString string;
 
   j = (long int) client_data;
   if ((int)matselected[j] == 1)
      XtVaSetValues(w,XmNset,TRUE,NULL);
   if ((int)matselected[j] == 0)
     {
      matselected[j] = 1;
      matorderlast++;
      neworder[matorderlast] = j;
      sprintf(str,"%s",mmatnames[j]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objneworder[matorderlast],
                    XmNlabelString,string,NULL);
      XmStringFree(string);
     }
}
 
void makematorderform(parent)
 Widget parent;
 
{
  Widget matodoit, matoundo, matoclose, matolab1, matolab2,
         matolab3, matswin1, matbox1, matswin2, matbox2,
         matswin3, matbox3;
  int i, j;
  Arg wargs[20];
  char str[20];
  XmString string;
 
   string = XmStringCreate("Mat Order",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   matorderform = XmCreateFormDialog(parent,"MatOrder",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   matodoit = XtVaCreateManagedWidget("Matodoit",
              xmPushButtonGadgetClass,
              matorderform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(matodoit,XmNactivateCallback,matorderdoit,NULL);
 
   string = XmStringCreate("Undo",XmSTRING_DEFAULT_CHARSET);
   matoundo = XtVaCreateManagedWidget("Matoundo",
              xmPushButtonGadgetClass,
              matorderform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, matodoit,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(matoundo,XmNactivateCallback,matorderundo,NULL);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   matoclose = XtVaCreateManagedWidget("Matoclose",
               xmPushButtonGadgetClass,
               matorderform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, matoundo,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(matoclose,XmNactivateCallback,matorderclose,NULL);
 
   /*  Create material toggle buttons.  */
 
   string = XmStringCreate("Materials ",XmSTRING_DEFAULT_CHARSET);
   matolab1 = XtVaCreateManagedWidget("Matolab1",xmLabelWidgetClass,
              matorderform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, matodoit,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, matolab1);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 155);
   i++;  XtSetArg(wargs[i],XmNheight, 300);
   i++;
   matswin1 = XmCreateScrolledWindow(matorderform,"Matswin1",wargs,i);
   XtManageChild(matswin1);

   i=0;  XtSetArg(wargs[i],XmNspacing, 0);
   i++;  XtSetArg(wargs[i],XmNmarginHeight, 0);
   i++;
   matbox1 = XmCreateRowColumn(matswin1,"Matbox1",wargs,i);
   XtManageChild(matbox1);

   for (i = 0; i < MAXMATS /*mmats should also work according to Frank Ortega*/; i++)
     {
      string = XmStringCreate(mmatnames[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Matsbut%d",i);
      matsbut[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonWidgetClass,
                   matbox1,
                   XmNlabelString, string,
                   XmNspacing, 0,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNvisibleWhenOff, FALSE,
                   NULL);
         XmStringFree(string);
         XtAddCallback(matsbut[i],XmNvalueChangedCallback,
                       matorder_select_mat,(caddr_t) i);

      if (mat_has_active_polys[i] == 0)
         XtUnmanageChild(matsbut[i]);
     }

   /*  Create new order labels.  */
 
   string = XmStringCreate("New Order",XmSTRING_DEFAULT_CHARSET);
   matolab2 = XtVaCreateManagedWidget("Matolab2",xmLabelWidgetClass,
              matorderform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, matodoit,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, matswin1,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, matolab2);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNleftWidget, matswin1);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 10);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 140);
   i++;  XtSetArg(wargs[i],XmNheight, 300);
   i++;
   matswin2 = XmCreateScrolledWindow(matorderform,"Matswin2",wargs,i);
   XtManageChild(matswin2);

   i = 0;
   matbox2 = XmCreateRowColumn(matswin2,"Matbox2",wargs,i);
   XtManageChild(matbox2);

   for (i = 0; i < MAXMATS /*mmats should also work according to Frank Ortega*/; i++)
     {
      sprintf(str,"%8d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Objneworder%d",i);
      objneworder[i] = XtVaCreateManagedWidget(str,
                       xmLabelWidgetClass,
                       matbox2,
                       XmNlabelString, string,
                       XmNbackground, whitepix,
                       NULL);
         XmStringFree(string);

      if (i >= nmat_order)
         XtUnmanageChild(objneworder[i]);
      }

   /*  Create current order labels.  */
 
   string = XmStringCreate("Current ",XmSTRING_DEFAULT_CHARSET);
   matolab3 = XtVaCreateManagedWidget("Matolab3",xmLabelWidgetClass,
              matorderform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, matodoit,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, matswin2,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, matolab3);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNleftWidget, matswin2);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 10);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 140);
   i++;  XtSetArg(wargs[i],XmNheight, 300);
   i++;
   matswin3 = XmCreateScrolledWindow(matorderform,"Matswin3",wargs,i);
   XtManageChild(matswin3);

   i = 0;
   matbox3 = XmCreateRowColumn(matswin3,"Matbox3",wargs,i);
   XtManageChild(matbox3);

   for (i = 0; i < MAXMATS /*mmats should also work according to Frank Ortega*/; i++)
     {
      j = mat_order[i];
      sprintf(str,"%s",mmatnames[j]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Objcurrorder%d",i);
      objcurrorder[i] = XtVaCreateManagedWidget(str,
                        xmLabelWidgetClass,
                        matbox3,
                        XmNlabelString, string,
                        NULL);
         XmStringFree(string);

      if (i >= nmat_order)
         XtUnmanageChild(objcurrorder[i]);
      }

}
 
void setmatorderbuts()
{
  int i, j;
  char str[20];
  XmString string;
 
   matorderlast = -1;
   for (i = 0; i < MAXMATS; i++) matselected[i] = 0;
   for (i = 0; i < nmat_order; i++)
     {
      neworder[i] = 0;
      sprintf(str,"%8d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objneworder[i],
                    XmNlabelString,string,
                    XmNbackground,whitepix,NULL);
      XmStringFree(string);
 
      j = mat_order[i];
      sprintf(str,"%s",mmatnames[j]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objcurrorder[i],
                    XmNlabelString,string,NULL);
      XmStringFree(string);
     }
 
   for (i = 0; i < MAXMATS; i++)
     {
      if (mat_has_active_polys[i] > 0)
         XtVaSetValues(matsbut[i],XmNset,FALSE,NULL);
     }
 
}
