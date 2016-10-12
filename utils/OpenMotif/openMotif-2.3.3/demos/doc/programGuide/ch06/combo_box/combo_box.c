/* $XConsortium: combo_box.c /main/5 1995/07/15 20:39:34 drk $ */
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
#include <Xm/XmAll.h>
#include <Xmd/RegEdit.h>

void   CreateComboBox(Widget);
extern void CreateMenus(Widget);
extern void SelectionCB(Widget, XtPointer, XtPointer); 
extern void HelpCB(Widget, XtPointer, XtPointer); 
extern void QuitCB(Widget, XtPointer, XtPointer);
Widget top_level; 

#define APP_CLASS "XmdComboBox"

static String fallbacks[] = {
"combobox.allowShellResize: True",
"combobox*fontList: fixed",
NULL
};

int
main(int    argc,
     char **argv)
{
 XtAppContext app_context;
 Widget       main_window;

   XtSetLanguageProc(NULL, NULL, NULL);
   top_level = XtVaOpenApplication(&app_context, APP_CLASS, 
                                NULL, 0, &argc, argv, 
                                fallbacks,
                                sessionShellWidgetClass, 
                                NULL);
   XmdRegisterEditres(top_level);
   main_window = XtVaCreateManagedWidget("main_window", 
                                xmMainWindowWidgetClass, top_level,
                                NULL);

   CreateMenus(main_window);
   CreateComboBox(main_window);

   XtRealizeWidget(top_level);
   XtAppMainLoop(app_context);

   return 0;    /* make compiler happy */
}



void
CreateComboBox(Widget parent_of_combo_box)
{
#define NUM_LIST_ITEMS 5
   Widget          comboBox;
   Arg             args[10];
   Cardinal        n, i;
   XmString        ListItem[NUM_LIST_ITEMS];
   static char    *ListString[] = { "kiwi", 
                                    "raspberry", 
                                    "carambola", 
                                    "litchi",
                                    "coconut" };
            
   /* Create a list of XmStrings for the ComboBox List child */
     for (i=0; i < NUM_LIST_ITEMS; i++)
       ListItem[i] = XmStringCreate (ListString[i], XmSTRING_DEFAULT_CHARSET);


   /* Create a ComboBox of type XmDROP_DOWN_COMBO_BOX. */
   /* Resources passed to ComboBox are passed on to the
    * children of ComboBox.  So, in the argument list
    * below, the resources, XmNitems, and XmNitemCount
    * will be passed on to the List child of ComboBox.  */
   n=0; 
   XtSetArg (args[n], XmNcomboBoxType, XmDROP_DOWN_COMBO_BOX); n++;
   XtSetArg (args[n], XmNarrowSpacing, 5); n++;
   XtSetArg (args[n], XmNitems, ListItem); n++;
   XtSetArg (args[n], XmNitemCount, NUM_LIST_ITEMS); n++;
   comboBox = XmCreateComboBox (parent_of_combo_box, "ComboBox", args, n);
   XtManageChild (comboBox);
   XtAddCallback (comboBox, XmNselectionCallback, SelectionCB, 
      (XtPointer)NULL);

   /* Example of manipulating a child widget directly to set the
    * visibleItemCount on the list.  */
   n=0;
   XtSetArg (args[n], XmNvisibleItemCount, 5); n++;
   XtSetValues (XtNameToWidget (comboBox,"*List"), args, n);  
}



void
SelectionCB (Widget w, 
                   XtPointer client_data, 
                   XtPointer call_data)
{
 XmComboBoxCallbackStruct *cb = (XmComboBoxCallbackStruct *)call_data;
 XmStringCharSet	charset;
 XmStringDirection	direction;
 XmStringContext	context;
 Boolean		separator;
 char                  *item;

   /* This callback procedure prints the item that was just selected. */

   /* Retrieve the selected text string from XmString. */
     item = XmStringUnparse(cb->item_or_text, NULL, XmCHARSET_TEXT,
                            XmCHARSET_TEXT, NULL, 0, XmOUTPUT_ALL);
     printf ("ComboBox SelectionCB: item = %s\n\n", item);
}
