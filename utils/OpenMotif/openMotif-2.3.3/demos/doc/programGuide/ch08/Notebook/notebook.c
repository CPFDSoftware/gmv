/* $XConsortium: notebook.c /main/5 1995/07/15 20:40:05 drk $ */
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

void   CreateNotebook(Widget);
extern void CreateMenus(Widget);
extern void HelpCB(Widget, XtPointer, XtPointer); 
extern void QuitCB(Widget, XtPointer, XtPointer);
Widget top_level; 

#define APP_CLASS "XmdNotebook"

int
main(int    argc,
     char **argv)
{
 XtAppContext app_context;
 Widget       main_window;

   XtSetLanguageProc(NULL, NULL, NULL);
   top_level = XtVaOpenApplication(&app_context, APP_CLASS, 
                                NULL, 0, &argc, argv, 
                                NULL,
                                sessionShellWidgetClass, 
                                NULL);
   XmdRegisterEditres(top_level);
   main_window = XtVaCreateManagedWidget("main_window", 
                                xmMainWindowWidgetClass, top_level,
                                NULL);

   CreateMenus(main_window);
   CreateNotebook(main_window);

   XtRealizeWidget(top_level);
   XtAppMainLoop(app_context);

   return 0;    /* make compiler happy */
}


void
CreateNotebook(Widget parent_of_notebook)
{
#define PAGES_IN_NOTEBOOK 7 
#define NUMBER_OF_MAJOR_TABS 2
 Widget notebook, frame;
 char buff[80];
 int i;
 static char *info[PAGES_IN_NOTEBOOK+1] = {
                                  "dummy",
                                  "apples are high in fiber",
                                  "bananas are high in Potassium",
                                  "oranges are high in Vitamin C",
                                  "celery",
                                  "lettuce",
                                  "sweet potato",
                                  "carrot"
                                 };
 
   notebook = XtVaCreateWidget("notebook", xmNotebookWidgetClass, 
                               parent_of_notebook, NULL);
	
 /* Create the pages of the Notebook. */ 
   for (i=1; i<=PAGES_IN_NOTEBOOK; i++) {
      /* Create a frame on every page. */
	frame = XtVaCreateManagedWidget("frame", 
                     xmFrameWidgetClass, notebook,
                     XmNnotebookChildType, XmPAGE,
                     XmNpageNumber,i,
		     NULL);

      /* Place the page contents (a string) on each page. */
	XtVaCreateManagedWidget(info[i], 
                     xmLabelWidgetClass, frame,
		     NULL);
   }

   XtVaCreateManagedWidget("tropical only",
                     xmLabelWidgetClass, notebook,
                     XmNnotebookChildType, XmSTATUS_AREA,
                     XmNpageNumber, 2,
                     NULL);

 /* Create major tabs to divide the pages into categories. */ 
   XtVaCreateManagedWidget("fruits",
                xmPushButtonWidgetClass, notebook,
                XmNnotebookChildType, XmMAJOR_TAB,
                XmNpageNumber, 1,
                NULL);
   XtVaCreateManagedWidget("vegetables",
                xmPushButtonWidgetClass, notebook,
                XmNnotebookChildType, XmMAJOR_TAB,
                XmNpageNumber, 4,
                NULL);
   

 /* Create some minor tabs to divide the categories into 
    subcategories. */ 
   XtVaCreateManagedWidget("green", 
                xmPushButtonWidgetClass, notebook,
                XmNnotebookChildType, XmMINOR_TAB,
                XmNpageNumber, 4,
                NULL);
   XtVaCreateManagedWidget("orange", 
                xmPushButtonWidgetClass, notebook,
                XmNnotebookChildType, XmMINOR_TAB,
                XmNpageNumber, 6,
                NULL);

   XtManageChild(notebook);

}
