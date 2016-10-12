/* $XConsortium: interface.c /main/4 1995/07/15 20:39:38 drk $ */
/*
 * @OPENGROUP_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 * ALL RIGHTS RESERVED (MOTIF).  See the file named COPYRIGHT.MOTIF for
 * the full copyright text.
 * 
 * This software is subject to an open license. It may only be
 * used on, with or for operating systems which are themselves open
 * source systems. You must contact The Open Group for a license
 * allowing distribution and sublicensing of this software on, with,
 * or for operating systems which are not Open Source programs.
 * 
 * See http://www.opengroup.org/openmotif/license for full
 * details of the license agreement. Any use, reproduction, or
 * distribution of the program constitutes recipient's acceptance of
 * this agreement.
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 * PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 * WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 * OR FITNESS FOR A PARTICULAR PURPOSE
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 * NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 * EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 * 
 */
/*
 * HISTORY
 */
#include <stdio.h>
#include <stdlib.h>
#include <Xm/XmAll.h>

void CreateMenus(Widget);
void HelpCB(Widget, XtPointer, XtPointer); 
void QuitCB(Widget, XtPointer, XtPointer); 
extern Widget top_level;

/**************************************************************************
CreateMenus: This function generates the menu bar and the submenus. 
**************************************************************************/
void 
CreateMenus(Widget parent_of_menu_bar)
{
 XmString   file, help;
 Widget     menubar, FilePullDown, HelpPullDown;
 Widget     overview, quit, Help1; 

 /* Create the menubar itself. */
   file = XmStringCreateSimple("File");
   help = XmStringCreateSimple("Help");
   
   menubar      = (Widget)XmCreateMenuBar(parent_of_menu_bar, "menubar", 
                                          NULL, 0);
   FilePullDown = (Widget)XmCreatePulldownMenu(menubar, "FilePullDown", 
                                               NULL, 0); 
   HelpPullDown = (Widget)XmCreatePulldownMenu(menubar, "HelpPullDown", 
                                                 NULL, 0); 

 /******************************FILE*********************************/
    XtVaCreateManagedWidget("File", xmCascadeButtonWidgetClass, menubar,
                             XmNlabelString, file,
                             XmNmnemonic, 'F', 
                             XmNsubMenuId, FilePullDown,
                             NULL);
    quit = XtVaCreateManagedWidget("Quit", xmPushButtonGadgetClass, 
                                    FilePullDown, NULL);
    XtAddCallback(quit, XmNactivateCallback, QuitCB, NULL);


 /******************************HELP*********************************/
    Help1 = XtVaCreateManagedWidget("Help", xmCascadeButtonWidgetClass, 
                             menubar,
                             XmNlabelString, help,
                             XmNmnemonic, 'H', 
                             XmNsubMenuId, HelpPullDown,
                             NULL);
    XtVaSetValues(menubar, XmNmenuHelpWidget, Help1, NULL);
    overview = XtVaCreateManagedWidget("Overview", xmPushButtonGadgetClass, 
                                    HelpPullDown, NULL);
    XtAddCallback(overview, XmNactivateCallback, HelpCB, (XtPointer)1);

    XmStringFree(file);
    XmStringFree(help);

    XtManageChild(menubar); 
}



/*********************************************************************
HelpCB: This function is called when the user requests help.  This
        function displays a Message DialogBox. 
*********************************************************************/
void
HelpCB(Widget   w,
       XtPointer cd,
       XtPointer cb
      )
{
 int       what_kind_of_help = (int)cd;  
 char      help_string[400]; 
 XmString  hs_as_cs; 
 Widget    dialog_general_help; 
 Arg       arg[3];

 sprintf(help_string, 
"This program demonstrates how to use an XmComboBox in an application.\n\
You can make a selection by typing it directly into the TextField\n\
or by selecting the item from the DropDown list.  To make the list\n\
drop down, click on the arrow that appears next to the TextField.");

   hs_as_cs = XmStringCreateLtoR(help_string, 
                                 XmFONTLIST_DEFAULT_TAG);
   
   XtSetArg(arg[0], XmNmessageString, hs_as_cs);
   dialog_general_help = (Widget)XmCreateMessageDialog(top_level, 
                                             "message", arg, 1);
   XmStringFree(hs_as_cs);
 
   switch (what_kind_of_help)  {
     case 1: XtManageChild(dialog_general_help);
             break;
     default: /* no other help */
             break; 
   }
          
}



/*******************************************************************************
QuitCB: Exit 
*******************************************************************************/
void
QuitCB(Widget w, XtPointer cd, XtPointer cb)
{
  exit(0);
}
