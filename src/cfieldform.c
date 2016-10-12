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

static Widget cfieldlist;
static int curr_cform;
static short cfieldsensitive[MAXFIELDS];


void cfieldformpop(int iform, int ifld)
{
   curr_cform = iform;
   XmListSelectPos(cfieldlist,ifld+1,FALSE);
   MyManageChild(cfieldform);
}


void closecfield(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfieldform);
}


void cfield_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data;
 XmListCallbackStruct *call_data;
{
  int ifld;

   ifld = call_data->item_position - 1;
   if (cfieldsensitive[ifld] == 0) return; 

   XtUnmanageChild(cfieldform);

   /*  Send selected field no. back to    */
   /*  initiating widget for processing.  */
   switch(curr_cform)
     {

      case(CFLDQUERY):  /*  queryform  */
         querygetcfield(ifld);
         break;

      case(CFLDVECTBLD):  /*  cell cvectbldform  */
         cvectbldgetcfield(ifld);
         break;

      case(CFLDFLDLIM):  /*  cell fieldlimform  */
         cfieldlimgetcfield(ifld);
         break;

      case(CFLDCCFIELD):  /*  cell form  */
         cellformgetcfield(ifld);
         break;

      case(CFLDCCUTPLANE):  /*  cutplane form  */
         cutplanegetcfield(ifld);
         break;

      case(CFLDGACFIELD):  /*  grid analysis form  */
         gridformgetcfield(ifld);
         break;

      case(CFLDCCUTLINE):  /*  cutline form  */
         cutlinegetcfield(ifld);
         break;

      case(CFLDFLDCALCX):  /*  cell fieldcalc x field  */
         fcalcxgetcfield(ifld);
         break;

      case(CFLDFLDCALCY):  /*  cell fieldcalc y field  */
         fcalcygetcfield(ifld);
         break;

      case(CFLDANIMCUTP):  /*  animation cutplane  */
         animcutpgetcfield(ifld);
         break;

      case(CFLDCELLSEL):  /*  cell selection  */
         cellselgetcfield(ifld);
         break;

      case(CFLDCCUTSPHERE):  /*  cutsphere form  */
         cutspheregetcfield(ifld);
         break;

      case(CFLDANIMCUTS):  /*  animation cutsphere  */
         animcutsgetcfield(ifld);
         break;

      case(CFLDAVG):  /*  average cell field  */
         cavggetfield(ifld);
         break;

#ifdef PHYSED
      case(CFLDPEFIELDCP):  /*  PhysEd cell field copy  */
         PEgetcfieldcp(ifld);
         break;

#endif
     }

}


void makecfieldform(parent)
 Widget parent;
{
  Widget cfldclose; 
  int i;
  Arg wargs[MAXFIELDS+20];
  XmString string;
  XmString cfielditem[MAXFIELDS];

   string = XmStringCreate("Select Cell Field",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cfieldform = XmCreateFormDialog(parent,"Cfieldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cfldclose = XtVaCreateManagedWidget("Cfldclose",
               xmPushButtonGadgetClass,
               cfieldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cfldclose,XmNactivateCallback,closecfield,
                    NULL);

   for (i = 0; i < cnumvars; i++)
      cfielditem[i] = XmStringLtoRCreate(cfieldname[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cfldclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 245);
   i++;
   if (cnumvars > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, cnumvars);
      i++;  XtSetArg(wargs[i],XmNitems, cfielditem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmSINGLE_SELECT);
   i++;
   cfieldlist = (Widget)XmCreateScrolledList(cfieldform,"Cfieldlist",wargs,i);
   XtManageChild(cfieldlist);
   XtAddCallback(cfieldlist,XmNsingleSelectionCallback,cfield_select,NULL);
   for (i = 0; i < cnumvars; i++) 
      XmStringFree(cfielditem[i]);

   XmListSelectPos(cfieldlist,1,FALSE);

   for (i = 0; i < cnumvars; i++)
     {
      cfieldsensitive[i] = 1;
      if (i >= cfldcalcbeg)
         cfieldsensitive[i] = 0;
     }
}


void setcfieldname(int ifld, char fldname[33])
{
  XmString string;

   string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(cfieldlist,&string,1,ifld+1);
   XmStringFree(string);
}

void setcfldsensitive(int ifld, int onoff)
{
   cfieldsensitive[ifld] = onoff;
}

void addcellfield()
{
  int i;
  XmString string;

   i = cnumvars;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(cfieldlist,string,i+1);
   XmStringFree(string);
   cfieldsensitive[i] = 1;
}
