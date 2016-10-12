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

static Widget ffieldlist;
static int curr_fform;
static short ffieldsensitive[MAXFIELDS];

void ffieldformpop(int iform, int ifld)
{
   curr_fform = iform;
   XmListSelectPos(ffieldlist,ifld+1,FALSE);
   MyManageChild(ffieldform);
}


void closeffield(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(ffieldform);
}


void ffield_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data;
 XmListCallbackStruct *call_data;
{
  int ifld;

   ifld = call_data->item_position - 1;
   if (ffieldsensitive[ifld] == 0) return; 

   XtUnmanageChild(ffieldform);

   /*  Send selected field no. back to    */
   /*  initiating widget for processing.  */
   switch(curr_fform)
     {

      case(FFLDVECTBLD):  /*  cell fvectbldform  */
         fvectbldgetffield(ifld);
         break;

      case(FFLDFLDLIM):  /*  cell fieldlimform  */
         ffieldlimgetffield(ifld);
         break;

      case(FFLDCFFIELD):  /*  cell form  */
         cellformgetffield(ifld);
         break;

      case(FFLDGAFFIELD):  /*  grid analysis form  */
         /* gridformgetffield(ifld); */
         break;

#ifdef PHYSED
      case(FFLDPEFIELD):  /*  PhysEd face field select  */
         /* PEgetffield(ifld); */
         break;

      case(FFLDPEFIELDCP):  /*  PhysEd face field copy  */
         /* PEgetffieldcp(ifld);  */
         break;
#endif
     }

}


void makeffieldform(parent)
 Widget parent;
{
  Widget ffldclose; 
  int i;
  Arg wargs[MAXFIELDS+20];
  XmString string;
  XmString ffielditem[MAXFIELDS];

   string = XmStringCreate("Select Face Field",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   ffieldform = XmCreateFormDialog(parent,"Ffieldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ffldclose = XtVaCreateManagedWidget("Ffldclose",
               xmPushButtonGadgetClass,
               ffieldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ffldclose,XmNactivateCallback,closeffield,
                    NULL);

   for (i = 0; i < fnumvars; i++)
      ffielditem[i] = XmStringLtoRCreate(ffieldname[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, ffldclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 245);
   if (fnumvars > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, fnumvars);
      i++;  XtSetArg(wargs[i],XmNitems, ffielditem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmSINGLE_SELECT);
   i++;
   ffieldlist = (Widget)XmCreateScrolledList(ffieldform,"Ffieldlist",wargs,i);
   XtManageChild(ffieldlist);
   XtAddCallback(ffieldlist,XmNsingleSelectionCallback,ffield_select,NULL);
   for (i = 0; i < fnumvars; i++) 
      XmStringFree(ffielditem[i]);

   XmListSelectPos(ffieldlist,1,FALSE);

   for (i = 0; i < fnumvars; i++)
     {
      ffieldsensitive[i] = 1;
/*
      if (i >= ffldcalcbeg)
         ffieldsensitive[i] = 0;
*/
     }

}


void setffieldname(int ifld, char fldname[33])
{
  XmString string;

   string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(ffieldlist,&string,1,ifld+1);
   XmStringFree(string);
}

void setffldsensitive(int ifld, int onoff)
{
   ffieldsensitive[ifld] = onoff;
}

void addfacefield()
{
  int i;
  XmString string;

   i = fnumvars;

   string = XmStringCreate(ffieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(ffieldlist,string,i+1);
   XmStringFree(string);
   ffieldsensitive[i] = 1;
}
