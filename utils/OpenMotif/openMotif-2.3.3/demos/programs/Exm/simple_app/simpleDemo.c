/* $XConsortium: simpleDemo.c /main/5 1995/07/15 20:44:25 drk $ */
/*
 *  @OPENGROUP_COPYRIGHT@
 *  COPYRIGHT NOTICE
 *  Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 *  Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 *  ALL RIGHTS RESERVED (MOTIF). See the file named COPYRIGHT.MOTIF for
 *  the full copyright text.
 *  
 *  This software is subject to an open license. It may only be
 *  used on, with or for operating systems which are themselves open
 *  source systems. You must contact The Open Group for a license
 *  allowing distribution and sublicensing of this software on, with,
 *  or for operating systems which are not Open Source programs.
 *  
 *  See http://www.opengroup.org/openmotif/license for full
 *  details of the license agreement. Any use, reproduction, or
 *  distribution of the program constitutes recipient's acceptance of
 *  this agreement.
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 *  PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 *  WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 *  OR FITNESS FOR A PARTICULAR PURPOSE
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 *  NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 *  EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGES.
 */
/*
 * HISTORY
 */


/****************************************************************************
 ****************************************************************************
 **
 **   File:     simpleDemo.c
 **
 **   Description: C program to demonstrate the ExmSimple widget. 
 **
 ****************************************************************************
 ****************************************************************************/


#include <sys/file.h>

/*  Standard C headers  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  X headers  */
#include <X11/IntrinsicP.h>

/*  Xm headers  */
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/CascadeBG.h>
#include <Exm/Simple.h>
#include <Exm/ExmString.h>
#include <Exm/Grid.h>
#include <Exm/MenuB.h>

/* Definitions. */
#define      APP_CLASS  "XMdemo"

/*******************************************************************************
exercise_ExmSimple: Realize four ExmSimple widgets. 
*******************************************************************************/
void 
exercise_ExmSimple(Widget parent)
{
 Widget   simpleWidget1, simpleWidget2, simpleWidget3, simpleWidget4;

 /* Instantiate a default ExmSimple widget; the default shape is an oval. */
  simpleWidget1 = XtVaCreateManagedWidget("Simple1", 
                               exmSimpleWidgetClass, parent, 
                               NULL); 

 /* Override the default shape; draw a rectangle instead. */ 
  simpleWidget2 = XtVaCreateManagedWidget("Simple2", 
                               exmSimpleWidgetClass, parent, 
                               ExmNsimpleShape,      ExmSHAPE_RECTANGLE,
                               NULL);

 /* Override the default shadow thickness and margin width. */ 
  simpleWidget3 = XtVaCreateManagedWidget("Simple3", 
                               exmSimpleWidgetClass, parent,
                               ExmNsimpleShape,     ExmSHAPE_OVAL,
                               XmNshadowThickness,  14,
                               XmNmarginWidth,      9,
                               NULL);

 /* Experiment with a different unit type. */
  simpleWidget4 = XtVaCreateManagedWidget("Simple4", 
                               exmSimpleWidgetClass, parent, 
                               ExmNsimpleShape,     ExmSHAPE_RECTANGLE,
                               XmNunitType,         Xm100TH_MILLIMETERS,
                               XmNmarginHeight,     1000,
                               NULL);
}


/*******************************************************************************
QuitCB: Exit the program when the user selects 'Quit'
*******************************************************************************/
void
QuitCB(Widget w, XtPointer ignore, XtPointer cbstruct)
{
  exit(0);
}



/*******************************************************************************
main: Set up the application, including the mainWindow and menuBar.
*******************************************************************************/
int 
main (int argc, char **argv)
{
  Display	*display;
  Widget       	shell, mainWindow, workRegion, menuBar, cascade, menuitem,
           	menu;
  XtAppContext 	AppContext;
  XmString	str;

 /* Do standard Motif application start-up. */
  XtToolkitInitialize();
  AppContext = XtCreateApplicationContext();
  if ((display = XtOpenDisplay (AppContext, NULL, argv[0], APP_CLASS,
				NULL, 0, &argc, argv)) == NULL)
    {
      fprintf (stderr,"\n%s:  Can't open display\n", argv[0]);
      exit(1);
    }

  shell = XtVaAppCreateShell(argv[0], APP_CLASS, applicationShellWidgetClass,
			     display, XmNallowShellResize, True, NULL);
			     

  mainWindow = XtVaCreateManagedWidget("myMainWindow", xmMainWindowWidgetClass, 
                                       shell, NULL);

 /* Create a menu bar with one simple menu. */
  menuBar = XmCreateMenuBar(mainWindow, "menuBar", NULL, 0);
  XtManageChild(menuBar);

  cascade = XmCreateCascadeButtonGadget(menuBar, "File", NULL, 0);
  XtManageChild(cascade);

  menu = XmCreatePulldownMenu(menuBar, "FileMenu", NULL, 0);

  XtVaSetValues(cascade, XmNsubMenuId, menu, 0);

  str = XmStringCreateLocalized("Quit");
  menuitem = XtVaCreateManagedWidget("item", xmPushButtonWidgetClass,
				     menu,
				     XmNlabelString, str,
				     NULL);
  XmStringFree(str);
  XtAddCallback(menuitem, XmNactivateCallback, QuitCB, NULL);

 /* Create a XmRowColumn widget; the ExmSimple widgets will ultimately be
    managed by this XmRowColumn. */
  workRegion = XtVaCreateManagedWidget("myWorkRegion", xmRowColumnWidgetClass,
                                       mainWindow, 
                                       NULL);
  
  exercise_ExmSimple(workRegion);

  XtRealizeWidget(shell);

  XtAppMainLoop(AppContext);

  return 0;    /* make compiler happy */
}
