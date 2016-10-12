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

static Widget sfieldlist;
static int curr_sform;
static short sfieldsensitive[MAXFIELDS];


void sfieldformpop(int iform, int ifld)
{
   curr_sform = iform;
   XmListSelectPos(sfieldlist,ifld+1,FALSE);
   MyManageChild(sfieldform);
}


void closesfield(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(sfieldform);
}


void sfield_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data;
 XmListCallbackStruct *call_data;
{
  int ifld;

   ifld = call_data->item_position - 1;
   if (sfieldsensitive[ifld] == 0) return; 

   XtUnmanageChild(sfieldform);

   /*  Send selected field no. back to    */
   /*  initiating widget for processing.  */
   switch(curr_sform)
     {

      case(SFLDVECTBLD):  /*  surface svectbldform  */
         svectbldgetsfield(ifld);
         break;

      case(SFLDFLDLIM):  /*  surface fieldlimform  */
         sfieldlimgetsfield(ifld);
         break;

      case(SFLDSSFIELD):  /*  surface form  */
         surfformgetsfield(ifld);
         break;

      case(SFLDFLDCALCX):  /*  surface fieldcalc x field  */
         fcalcxgetsfield(ifld);
         break;

      case(SFLDFLDCALCY):  /*  surface fieldcalc y field  */
         fcalcygetsfield(ifld);
         break;

      case(SFLDSURFSEL):  /*  surface selection  */
         surfselgetsfield(ifld);
         break;

#ifdef PHYSED
      case(SFLDPEFIELDCP):  /*  PhysEd surface field copy  */
         PEgetsfieldcp(ifld);
         break;
#endif
     }

}


void makesfieldform(parent)
 Widget parent;
{
  Widget sfldclose; 
  int i;
  Arg wargs[MAXFIELDS+20];
  XmString string;
  XmString sfielditem[MAXFIELDS];

   string = XmStringCreate("Select Surface Field",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   sfieldform = XmCreateFormDialog(parent,"Sfieldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sfldclose = XtVaCreateManagedWidget("Sfldclose",
               xmPushButtonGadgetClass,
               sfieldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sfldclose,XmNactivateCallback,closesfield,
                    NULL);

   for (i = 0; i < snumvars; i++)
      sfielditem[i] = XmStringLtoRCreate(sfieldname[i],
                                         XmSTRING_DEFAULT_CHARSET);
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sfldclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 245);
   if (snumvars > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, snumvars);
      i++;  XtSetArg(wargs[i],XmNitems, sfielditem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmSINGLE_SELECT);
   i++;
   sfieldlist = (Widget)XmCreateScrolledList(sfieldform,"Sfieldlist",wargs,i);
   XtManageChild(sfieldlist);
   XtAddCallback(sfieldlist,XmNsingleSelectionCallback,sfield_select,NULL);
   for (i = 0; i < snumvars; i++) 
      XmStringFree(sfielditem[i]);

   XmListSelectPos(sfieldlist,1,FALSE);

   for (i = 0; i < snumvars; i++)
     {
      sfieldsensitive[i] = 1;
      if (i >= sfldcalcbeg)
         sfieldsensitive[i] = 0;
     }
}


void setsfieldname(int ifld, char fldname[9])
{
  XmString string;

   string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(sfieldlist,&string,1,ifld+1);
   XmStringFree(string);
}

void setsfldsensitive(int ifld, int onoff)
{
   sfieldsensitive[ifld] = onoff;
}

void addsurffield()
{
  int i;
  XmString string;

   i = snumvars;

   string = XmStringCreate(sfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(sfieldlist,string,i+1);
   XmStringFree(string);
   sfieldsensitive[i] = 1;
}
