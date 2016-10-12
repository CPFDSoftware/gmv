#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>

#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/PushBG.h>

#include <math.h>
//#include <malloc.h>
 
#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"



void doPEdit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) PEnodefldselformpop(0);
   if (arg == 1) PEnodefldaddformpop();
   if (arg == 2) PEnodefldselformpop(1);
   if (arg == 3) PEnodegrpselformpop(0);
   if (arg == 4) PEnodegrpaddformpop();
   if (arg == 5) PEnodegrpselformpop(1);
   if (arg == 6) PEcellfldselformpop(0);
   if (arg == 7) PEcellfldaddformpop();
   if (arg == 8) PEcellfldselformpop(1);
   if (arg == 9) PEcellgrpselformpop(0);
   if (arg == 10) PEcellgrpaddformpop();
   if (arg == 11) PEcellgrpselformpop(1);
   if (arg == 12) PEsurffldselformpop(0);
   if (arg == 13) PEsurffldaddformpop();
   if (arg == 14) PEsurffldselformpop(1);
   if (arg == 15) PEsurfgrpselformpop(0);
   if (arg == 16) PEsurfgrpaddformpop();
   if (arg == 17) PEsurfgrpselformpop(1);
/*
   if (arg == 6) PEcellmatformpop();
   if (arg == 7) PEnodematformpop();
*/
}
  
 
void makePEmenu(Widget menu_bar)
{
  int i;
  Arg args[20];
  Widget menu_panePE, nodepemenu, nodefldPEedit, nodefldPEadd,
         nodegrpPEedit, nodegrpPEadd, nodepebut, cellpemenu,
         cellfldPEedit, cellfldPEadd, cellgrpPEedit, cellgrpPEadd,
         cellpebut, surfpemenu, surffldPEedit, surffldPEadd,
         surfgrpPEedit, surfgrpPEadd, surfpebut, cascadePE,
         nodefldPEdel, cellfldPEdel, surffldPEdel, 
         nodegrpPEdel, cellgrpPEdel, surfgrpPEdel;
  void doPEdit();

  XmString string;
 

   /*  Create edit menu to edit material and/or field data.  */
   menu_panePE = XmCreatePulldownMenu(menu_bar,"Menu_panePE",NULL,0);
 
   nodepemenu = XmCreatePulldownMenu(menu_panePE,"Nodepemenu",NULL,0);

   string = XmStringCreate("Edit a Node Field",XmSTRING_DEFAULT_CHARSET);
   nodefldPEedit = XtVaCreateManagedWidget("NodefldPEedit",
                   xmPushButtonGadgetClass,
                   nodepemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nodefldPEedit,XmNactivateCallback,doPEdit,(caddr_t) 0);

   string = XmStringCreate("Add a Node Field",XmSTRING_DEFAULT_CHARSET);
   nodefldPEadd = XtVaCreateManagedWidget("NodefldPEadd",
                  xmPushButtonGadgetClass,
                  nodepemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nodefldPEadd,XmNactivateCallback,doPEdit,(caddr_t) 1);

   string = XmStringCreate("Delete a Node Field",XmSTRING_DEFAULT_CHARSET);
   nodefldPEdel = XtVaCreateManagedWidget("NodefldPEdel",
                  xmPushButtonGadgetClass,
                  nodepemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nodefldPEdel,XmNactivateCallback,doPEdit,(caddr_t) 2);

   string = XmStringCreate("Edit a Node Group",XmSTRING_DEFAULT_CHARSET);
   nodegrpPEedit = XtVaCreateManagedWidget("NodegrpPEedit",
                   xmPushButtonGadgetClass,
                   nodepemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nodegrpPEedit,XmNactivateCallback,doPEdit,(caddr_t) 3);

   string = XmStringCreate("Add a Node Group",XmSTRING_DEFAULT_CHARSET);
   nodegrpPEadd = XtVaCreateManagedWidget("NodegrpPEadd",
                  xmPushButtonGadgetClass,
                  nodepemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nodegrpPEadd,XmNactivateCallback,doPEdit,(caddr_t) 4);

   string = XmStringCreate("Delete a Node Group",XmSTRING_DEFAULT_CHARSET);
   nodegrpPEdel = XtVaCreateManagedWidget("NodegrpPEdel",
                  xmPushButtonGadgetClass,
                  nodepemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nodegrpPEdel,XmNactivateCallback,doPEdit,(caddr_t) 5);

   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,nodepemenu);
   i++;
   nodepebut = XmCreateCascadeButton(menu_panePE,"Nodepebut",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(nodepebut);

   cellpemenu = XmCreatePulldownMenu(menu_panePE,"Cellpemenu",NULL,0);

   string = XmStringCreate("Edit a Cell Field",XmSTRING_DEFAULT_CHARSET);
   cellfldPEedit = XtVaCreateManagedWidget("CellfldPEedit",
                   xmPushButtonGadgetClass,
                   cellpemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cellfldPEedit,XmNactivateCallback,doPEdit,(caddr_t) 6);

   string = XmStringCreate("Add a Cell Field",XmSTRING_DEFAULT_CHARSET);
   cellfldPEadd = XtVaCreateManagedWidget("CellfldPEadd",
                  xmPushButtonGadgetClass,
                  cellpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cellfldPEadd,XmNactivateCallback,doPEdit,(caddr_t) 7);

   string = XmStringCreate("Delete a Cell Field",XmSTRING_DEFAULT_CHARSET);
   cellfldPEdel = XtVaCreateManagedWidget("CellfldPEdel",
                  xmPushButtonGadgetClass,
                  cellpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cellfldPEdel,XmNactivateCallback,doPEdit,(caddr_t) 8);

   string = XmStringCreate("Edit a Cell Group",XmSTRING_DEFAULT_CHARSET);
   cellgrpPEedit = XtVaCreateManagedWidget("CellgrpPEedit",
                   xmPushButtonGadgetClass,
                   cellpemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cellgrpPEedit,XmNactivateCallback,doPEdit,(caddr_t) 9);

   string = XmStringCreate("Add a Cell Group",XmSTRING_DEFAULT_CHARSET);
   cellgrpPEadd = XtVaCreateManagedWidget("CellgrpPEadd",
                  xmPushButtonGadgetClass,
                  cellpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cellgrpPEadd,XmNactivateCallback,doPEdit,(caddr_t) 10);

   string = XmStringCreate("Delete a Cell Group",XmSTRING_DEFAULT_CHARSET);
   cellgrpPEdel = XtVaCreateManagedWidget("CellgrpPEdel",
                  xmPushButtonGadgetClass,
                  cellpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cellgrpPEdel,XmNactivateCallback,doPEdit,(caddr_t) 11);

   string = XmStringCreate("Cells",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,cellpemenu);
   i++;
   cellpebut = XmCreateCascadeButton(menu_panePE,"Cellpebut",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cellpebut);

   surfpemenu = XmCreatePulldownMenu(menu_panePE,"Surfpemenu",NULL,0);

   string = XmStringCreate("Edit a Surface Field",XmSTRING_DEFAULT_CHARSET);
   surffldPEedit = XtVaCreateManagedWidget("SurffldPEedit",
                   xmPushButtonGadgetClass,
                   surfpemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(surffldPEedit,XmNactivateCallback,doPEdit,(caddr_t) 12);

   string = XmStringCreate("Add a Surface Field",XmSTRING_DEFAULT_CHARSET);
   surffldPEadd = XtVaCreateManagedWidget("SurffldPEadd",
                  xmPushButtonGadgetClass,
                  surfpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(surffldPEadd,XmNactivateCallback,doPEdit,(caddr_t) 13);

   string = XmStringCreate("Delete a Surface Field",XmSTRING_DEFAULT_CHARSET);
   surffldPEdel = XtVaCreateManagedWidget("SurffldPEdel",
                  xmPushButtonGadgetClass,
                  surfpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(surffldPEdel,XmNactivateCallback,doPEdit,(caddr_t) 14);

   string = XmStringCreate("Edit a Surface Group",XmSTRING_DEFAULT_CHARSET);
   surfgrpPEedit = XtVaCreateManagedWidget("SurfgrpPEedit",
                   xmPushButtonGadgetClass,
                   surfpemenu,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(surfgrpPEedit,XmNactivateCallback,doPEdit,(caddr_t) 15);

   string = XmStringCreate("Add a Surface Group",XmSTRING_DEFAULT_CHARSET);
   surfgrpPEadd = XtVaCreateManagedWidget("SurfgrpPEadd",
                  xmPushButtonGadgetClass,
                  surfpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(surfgrpPEadd,XmNactivateCallback,doPEdit,(caddr_t) 16);

   string = XmStringCreate("Delete a Surface Group",XmSTRING_DEFAULT_CHARSET);
   surfgrpPEdel = XtVaCreateManagedWidget("SurfgrpPEdel",
                  xmPushButtonGadgetClass,
                  surfpemenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(surfgrpPEdel,XmNactivateCallback,doPEdit,(caddr_t) 17);

   string = XmStringCreate("Surfaces",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,surfpemenu);
   i++;
   surfpebut = XmCreateCascadeButton(menu_panePE,"Surfpebut",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(surfpebut);

/*
   string = XmStringCreate("Edit/Add Cell Material",XmSTRING_DEFAULT_CHARSET);
   cellmatPE = XtVaCreateManagedWidget("CellmatPE",
               xmPushButtonGadgetClass,
               menu_panePE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cellmatPE,XmNactivateCallback,doPEdit,(caddr_t) 4);

   string = XmStringCreate("Edit/Add Node Material",XmSTRING_DEFAULT_CHARSET);
   nodematPE = XtVaCreateManagedWidget("NodematPE",
               xmPushButtonGadgetClass,
               menu_panePE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nodematPE,XmNactivateCallback,doPEdit,(caddr_t) 5);
*/
   string = XmStringCreate("Edit",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_panePE);
   i++;
   cascadePE = XmCreateCascadeButton(menu_bar,"CascadePE",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascadePE);
}


void makePEforms(Widget poplevel)
{

   /*  Create PEnodefldselform popup form.  */
   makePEnodefldselform(poplevel); 

   /*  Create PEnodefldaddform popup form.  */
   makePEnodefldaddform(poplevel);

   /*  Create PEnodefldeditform popup form.  */
   makePEnodefldeditform(toplevel);

   /*  Create PEnodegrpselform popup form.  */
   makePEnodegrpselform(poplevel);

   /*  Create PEnodegrpaddform popup form.  */
   makePEnodegrpaddform(poplevel);

   /*  Create PEnodegrpeditform popup form.  */
   makePEnodegrpeditform(toplevel);

   /*  Create PEcellfldselform popup form.  */
   makePEcellfldselform(poplevel);

   /*  Create PEcellfldaddform popup form.  */
   makePEcellfldaddform(poplevel);

   /*  Create PEcellfldeditform popup form.  */
   makePEcellfldeditform(toplevel);

   /*  Create PEcellgrpselform popup form.  */
   makePEcellgrpselform(poplevel);

   /*  Create PEcellgrpaddform popup form.  */
   makePEcellgrpaddform(poplevel);

   /*  Create PEcellgrpeditform popup form.  */
   makePEcellgrpeditform(toplevel);

   /*  Create PEsurffldselform popup form.  */
   makePEsurffldselform(poplevel);

   /*  Create PEsurffldaddform popup form.  */
   makePEsurffldaddform(poplevel);

   /*  Create PEsurffldeditform popup form.  */
   makePEsurffldeditform(toplevel);

   /*  Create PEsurfgrpdselform popup form.  */
   makePEsurfgrpselform(poplevel);

   /*  Create PEsurfgrpaddform popup form.  */
   makePEsurfgrpaddform(poplevel);

   /*  Create PEsurfgrpeditform popup form.  */
   makePEsurfgrpeditform(toplevel);
}


void makePEmatforms(Widget poplevel)
{

   /*  Create PEcellmatform popup form.  */
/*   makePEcellmatform(poplevel); */

   /*  Create PEnodematform popup form.  */
/*   makePEnodematform(poplevel); */
}


void popdownPEforms()
{
   XtUnmanageChild(PEcellfldeditform);
   XtUnmanageChild(PEcellfldselform);
   XtUnmanageChild(PEcellfldaddform);
   XtUnmanageChild(PEcellgrpeditform);
   XtUnmanageChild(PEcellgrpselform);
   XtUnmanageChild(PEcellgrpaddform);

   XtUnmanageChild(PEnodefldeditform);
   XtUnmanageChild(PEnodefldselform);
   XtUnmanageChild(PEnodefldaddform);
   XtUnmanageChild(PEnodegrpeditform);
   XtUnmanageChild(PEnodegrpselform);
   XtUnmanageChild(PEnodegrpaddform);

   XtUnmanageChild(PEsurffldeditform);
   XtUnmanageChild(PEsurffldselform);
   XtUnmanageChild(PEsurffldaddform);
   XtUnmanageChild(PEsurfgrpeditform);
   XtUnmanageChild(PEsurfgrpselform);
   XtUnmanageChild(PEsurfgrpaddform);
/*
   XtUnmanageChild(PEcellmatform);
   XtUnmanageChild(PEnodematform);
*/
}

void destroyPEforms()
{
/*
   XtDestroyWidget(PEcellmatform);
   XtDestroyWidget(PEnodematform);
*/
}
