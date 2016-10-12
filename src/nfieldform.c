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

static Widget nfieldlist;
static int curr_nform;
static short nfieldsensitive[MAXFIELDS];


void nfieldformpop(int iform, int ifld)
{
   curr_nform = iform;
   XmListSelectPos(nfieldlist,ifld+1,FALSE);
   MyManageChild(nfieldform);
}


void closenfield(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
}


void nfield_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data;
 XmListCallbackStruct *call_data;
{
  int ifld;

   ifld = call_data->item_position - 1;
   if (nfieldsensitive[ifld] == 0) return; 

   XtUnmanageChild(nfieldform);

   /*  Send selected field no. back to    */
   /*  initiating widget for processing.  */
   switch(curr_nform)
     {

      case(NFLDQUERY):  /*  queryform  */
         querygetnfield(ifld);
         break;

      case(NFLDVECTBLD):  /*  node vectbldform  */
         vectbldgetnfield(ifld);
         break;

      case(NFLDFLDLIM):  /*  node fieldlimform  */
         fieldlimgetnfield(ifld);
         break;

      case(NFLDNFIELD):  /*  node form  */
         nodeformgetnfield(ifld);
         break;

      case(NFLDCNFIELD):  /*  cell form  */
         cellformgetnfield(ifld);
         break;

      case(NFLDISOSURF):  /*  isosurface form  */
         isosurfgetnfield(ifld);
         break;

      case(NFLDNCUTPLANE):  /*  cutplane form  */
         cutplanegetnfield(ifld);
         break;

      case(NFLDGANFIELD):  /*  gridanalysis form  */
         gridformgetnfield(ifld);
         break;

      case(NFLDNCUTLINE):  /*  cutline form  */
         cutlinegetnfield(ifld);
         break;

      case(NFLDFLDCALCX):  /*  fieldcalc x field  */
         fcalcxgetnfield(ifld);
         break;

      case(NFLDFLDCALCY):  /*  fieldcalc y field  */
         fcalcygetnfield(ifld);
         break;

      case(NFLDANIMISO):  /*  animation isosurface  */
         animisogetnfield(ifld);
         break;

      case(NFLDANIMCUTP):  /*  animation cutplane  */
         animcutpgetnfield(ifld);
         break;

      case(NFLDNODESEL):  /*  node selection  */
         nodeselgetnfield(ifld);
         break;

      case(NFLDISOVOL):  /*  isovolume  */
         isovolfgetnfield(ifld);
         break;

      case(NFLDCELLSEL):  /*  cell node field selection  */
         cellselgetnfield(ifld);
         break;

      case(NFLDISOVOLCLR):  /*  isovolume field color  */
         isovolfclrgetnfield(ifld);
         break;

      case(NFLDISOSURFCLR):  /*  isosurface field color  */
         isosurfclrgetnfield(ifld);
         break;

      case(NFLDISOSURFMCLR):  /*  material isosurface field color  */
         misosurfclrgetnfield(ifld);
         break;

      case(NFLDNCUTSPHERE):  /*  cutsphere form  */
         cutspheregetnfield(ifld);
         break;

      case(NFLDANIMCUTS):  /*  animation cutsphere  */
         animcutsgetnfield(ifld);
         break;

      case(NFLDSNFIELD):  /*  surface node field  */
         surfformgetnfield(ifld);
         break;

      case(NFLDSURFSEL):  /*  surf node field selection  */
         surfselgetnfield(ifld);
         break;

      case(NFLDAVG):  /*  average node field  */
         navggetfield(ifld);
         break;

#ifdef PHYSED
      case(NFLDPEFIELDCP):  /*  PhysEd node field copy  */
         PEgetnfieldcp(ifld);
         break;
#endif

     }

}


void makenfieldform(parent)
 Widget parent;
{
  Widget nfldclose; 
  int i;
  Arg wargs[MAXFIELDS+20];
  XmString string;
  XmString nfielditem[MAXFIELDS];

   string = XmStringCreate("Select Node Field",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nfieldform = XmCreateFormDialog(parent,"Nfieldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nfldclose = XtVaCreateManagedWidget("Nfldclose",
               xmPushButtonGadgetClass,
               nfieldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nfldclose,XmNactivateCallback,closenfield,
                    NULL);

   for (i = 0; i < numvars; i++)
      nfielditem[i] = XmStringLtoRCreate(fieldname[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nfldclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 245);
   if (numvars > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, numvars);
      i++;  XtSetArg(wargs[i],XmNitems, nfielditem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmSINGLE_SELECT);
   i++;
   nfieldlist = (Widget)XmCreateScrolledList(nfieldform,"Nfieldlist",wargs,i);
   XtManageChild(nfieldlist);
   XtAddCallback(nfieldlist,XmNsingleSelectionCallback,nfield_select,NULL);
   for (i = 0; i < numvars; i++) 
      XmStringFree(nfielditem[i]);

   XmListSelectPos(nfieldlist,1,FALSE);

   for (i = 0; i < numvars; i++)
     {
      nfieldsensitive[i] = 1;
      if (i >= fldcalcbeg && i < fldcalcbeg + MAXFLDCALC && fldcalcbeg > 0)
         nfieldsensitive[i] = 0;
     }
}


void setnfieldname(int ifld, char fldname[33])
{
  XmString string;

   string = XmStringCreate(fldname,XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(nfieldlist,&string,1,ifld+1);
   XmStringFree(string);
}


void setnfldsensitive(int ifld, int onoff)
{
   nfieldsensitive[ifld] = onoff;
}


void addnodefield()
{
  int i;
  XmString string;

   i = numvars;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(nfieldlist,string,i+1);
   XmStringFree(string);
   nfieldsensitive[i] = 1;
}
