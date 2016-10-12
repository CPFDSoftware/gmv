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

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget trfieldlist;
static int curr_trform;
static short trfieldsensitive[MAXTRFIELDS];


void trfieldformpop(int iform, int ifld)
{
   curr_trform = iform;
   XmListSelectPos(trfieldlist,ifld+1,FALSE);
   MyManageChild(trfieldform);
}


void closetrfield(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(trfieldform);
}


void trfield_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data;
 XmListCallbackStruct *call_data;
{
  int ifld;

   ifld = call_data->item_position - 1;
   if (trfieldsensitive[ifld] == 0) return; 

   XtUnmanageChild(trfieldform);

   /*  Send selected field no. back to    */
   /*  initiating widget for processing.  */
   switch(curr_trform)
     {

      case(TRFLDFLDLIM):  /*  tracer fieldlimform  */
         /* fieldlimgettrfield(ifld); */
         break;

      case(TRFLDNFIELD):  /*  tracer form  */
         /* traceformgettrfield(ifld); */
         break;

      case(TRFLDNODESEL):  /*  tracer selection  */
         traceselgettrfield(ifld);
         break;

      case(TRFLDFLDCALCX):  /*  fieldcalc x field  */
         fcalcxgettfield(ifld);
         break;

      case(TRFLDFLDCALCY):  /*  fieldcalc y field  */
         fcalcygettfield(ifld);
         break;
     }

}


void maketrfieldform(parent)
 Widget parent;
{
  Widget trfldclose; 
  int i;
  Arg wargs[MAXTRFIELDS+20];
  XmString string;
  XmString trfielditem[MAXTRFIELDS];

   string = XmStringCreate("Select Particle Field",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   trfieldform = XmCreateFormDialog(parent,"Trfieldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trfldclose = XtVaCreateManagedWidget("Trfldclose",
               xmPushButtonGadgetClass,
               trfieldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(trfldclose,XmNactivateCallback,closetrfield,
                    NULL);

   for (i = 0; i < numvartr; i++)
      trfielditem[i] = XmStringLtoRCreate(fieldtrname[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trfldclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 245);
   if (numvartr > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, numvartr);
      i++;  XtSetArg(wargs[i],XmNitems, trfielditem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmSINGLE_SELECT);
   i++;
   trfieldlist = (Widget)XmCreateScrolledList(trfieldform,"Trfieldlist",
                                              wargs,i);
   XtManageChild(trfieldlist);
   XtAddCallback(trfieldlist,XmNsingleSelectionCallback,trfield_select,NULL);
   for (i = 0; i < numvartr; i++) 
      XmStringFree(trfielditem[i]);

   XmListSelectPos(trfieldlist,1,FALSE);

   for (i = 0; i < numvartr; i++)
     {
      trfieldsensitive[i] = 1;
     }
}


void settrfieldname(int ifld, char fldname[33])
{
  XmString string;

   string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(trfieldlist,&string,1,ifld+1);
   XmStringFree(string);
}


void settrfldsensitive(int ifld, int onoff)
{
   trfieldsensitive[ifld] = onoff;
}
