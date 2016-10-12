/* $XConsortium: container.c /main/5 1995/07/15 20:39:52 drk $ */
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

void   CreateContainer(Widget);
void   MakeACall(Widget, XtPointer, XtPointer);
extern void CreateMenus(Widget);
extern void HelpCB(Widget, XtPointer, XtPointer); 
extern void QuitCB(Widget, XtPointer, XtPointer);
Widget top_level; 

#define APP_CLASS "XmdContainer"

static String fallbacks[] = {
"container.allowShellResize: True",
"container*fontList: fixed",
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
   CreateContainer(main_window);

   XtRealizeWidget(top_level);
   XtAppMainLoop(app_context);

   return 0;    /* make compiler happy */
}


void
MakeACall(Widget        w,
          XtPointer     client_data, 
          XtPointer     call_data)
{
  printf("Ring!\n");
}


void
CreateContainer(Widget parent_of_container)
{
 Widget  container1;
 Widget  president, vice_president, dir_of_sales, dir_of_rnd, dir_of_mfr; 

   container1 = XtVaCreateWidget("Container",
   			xmContainerWidgetClass, parent_of_container,
   			XmNlayoutType, XmOUTLINE,
   			XmNselectionPolicy, XmBROWSE_SELECT,
   			XmNautomaticSelection, XmNO_AUTO_SELECT,
   			XmNentryViewType, XmSMALL_ICON,
   			NULL);
   XtAddCallback(container1, XmNselectionCallback, MakeACall,
                 (XtPointer)NULL);

   president = XtVaCreateManagedWidget("President",
   			xmIconGadgetClass, container1,
   			XmNoutlineState, XmEXPANDED,
                        NULL);
   vice_president = XtVaCreateManagedWidget("Vice-President",
   			xmIconGadgetClass, container1,
   			XmNentryParent, president,
   			XmNoutlineState, XmEXPANDED,
                        NULL);
   dir_of_sales = XtVaCreateManagedWidget("Director of Sales",
   			xmIconGadgetClass, container1,
   			XmNentryParent, vice_president,
                        NULL);
   dir_of_rnd = XtVaCreateManagedWidget("Director of R&D",
   			xmIconGadgetClass, container1,
   			XmNentryParent, vice_president,
                        NULL);
   dir_of_mfr = XtVaCreateManagedWidget("Director of Manufacturing",
   			xmIconGadgetClass, container1,
   			XmNentryParent, vice_president,
                        NULL);
   XtManageChild(container1);
}
