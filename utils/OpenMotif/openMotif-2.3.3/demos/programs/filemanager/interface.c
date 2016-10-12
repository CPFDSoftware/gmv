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

#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$XConsortium: interface.c /main/6 1995/07/14 09:41:50 drk $"
#endif
#endif

#include <Xm/Xm.h>           /* Motif Toolkit */
#include <Xm/CascadeBG.h>
#include <Xm/Container.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SelectioB.h>
#include <Xm/ToggleBG.h>
#include <Xmd/Menus.h>
#include <Xmd/Help.h>
#include "filemanager.h"

Widget 
CreateMenuButton(char* name, Widget parent, XtCallbackProc callback,
		 XtPointer data, char mnemonic)
{
  Widget button;
  Arg args[2];
  int n = 0;

  if (mnemonic != 0) {
    XtSetArg(args[n], XmNmnemonic, mnemonic);
    n++;
  }

  button = XmCreatePushButtonGadget(parent, name, args, n);
  XtManageChild(button);

  XtAddCallback(button, XmNactivateCallback, callback, data);
  return(button);
}

Widget 
CreateInterface(char* name, Widget parent)
{
  Widget top, menubar, selected, view, *selected_menu, *view_menu,
         view_pulldown, form, dirOM, where, sw, dirMenu, lroot, 
         help, *help_menu, helpDialog, container, view_sub_menu,
         show_hidden;
  Arg args[30];
  int n;
  Cardinal size;
  XmString tmp, tmp2;
  Pixel white, black, lightblue, gray;
  Display *display;
  Screen *screen;
  XmRendition rend[1];
  XmString headings[10];
  int num_headings;

  /* Create widgets */
  top = XmCreateMainWindow(parent, name, NULL, 0);
  XtManageChild(top);

  /* Goto dialog */
  n = 0;
  tmp = XmStringCreateLocalized("Enter new directory");
  tmp2 = XmStringCreateLocalized("Goto");
  XtSetArg(args[n], XmNselectionLabelString, tmp); n++;
  XtSetArg(args[n], XmNokLabelString, tmp2); n++;
  gotoDialog = XmCreatePromptDialog(top, "gotoDialog", args, n);
  XtAddCallback(gotoDialog, XmNokCallback, gotoCB, NULL);
  XmStringFree(tmp);
  XmStringFree(tmp2);

  /* Menu bar */
  menubar = XmCreateMenuBar(top, "menubar", NULL, 0);
  XtManageChild(menubar);

  XmdCreateMenu(SELECTED_MENU, menubar, &selected_menu, &size);
  XtUnmanageChildren(selected_menu, size);
  XtManageChild(selected_menu[SELECTED_NEW]);
  XtAddCallback(selected_menu[SELECTED_NEW], 
		XmNactivateCallback, 
		(XtCallbackProc) newFolder, NULL);
  XtManageChild(selected_menu[SELECTED_OPEN]);
  XtAddCallback(selected_menu[SELECTED_OPEN], 
		XmNactivateCallback, 
		(XtCallbackProc) manageCB, (XtPointer) gotoDialog);
  XtManageChild(selected_menu[SELECTED_DELETE]);
  XtAddCallback(selected_menu[SELECTED_DELETE], 
		XmNactivateCallback, 
		(XtCallbackProc) deleteItem, NULL);
  XtManageChild(selected_menu[SELECTED_EXIT]);
  XtAddCallback(selected_menu[SELECTED_EXIT],
  		XmNactivateCallback, 
		(XtCallbackProc) quitCB, NULL);

  view_pulldown = XmdCreateMenu(VIEW_MENU, menubar, &view_menu, &size);
  XtUnmanageChildren(view_menu, size);
  XtManageChild(view_menu[VIEW_CHANGE]);

  show_hidden = XmCreateToggleButtonGadget(view_pulldown, "show_hidden",
					   NULL, 0);
  XtManageChild(show_hidden);
  XtAddCallback(show_hidden, XmNvalueChangedCallback, 
		(XtCallbackProc) showHiddenCB, NULL);

  view_sub_menu = XmCreatePulldownMenu(view_pulldown, "viewSubMenu",
				       NULL, 0);
  XtVaSetValues(view_menu[VIEW_CHANGE], XmNsubMenuId, 
		view_sub_menu, NULL, NULL);

  CreateMenuButton("Large", view_sub_menu, 
		   (XtCallbackProc) viewCB, "large", 'L');
  CreateMenuButton("Small", view_sub_menu, 
		   (XtCallbackProc) viewCB, "small", 'S');
  CreateMenuButton("Detail", view_sub_menu, 
		   (XtCallbackProc) viewCB, "detail", 'D');

  helpDialog = XmdCreateHelpDialog(top, "help_manager", NULL, 0);

  XmdCreateMenu(HELP_MENU, menubar, &help_menu, &size);
  XtUnmanageChildren(help_menu, size);
  XtManageChild(help_menu[HELP_OVERVIEW]);
  XtAddCallback(help_menu[HELP_OVERVIEW], XmNactivateCallback,
		(XtCallbackProc) manageCB, (XtPointer) helpDialog);

  /* Main window work area */
  n = 0;
  XtSetArg(args[n], XmNrubberPositioning, False); n++;
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  form = XmCreateForm(top, "form", args, n);
  XtManageChild(form);

  dirMenu = XmCreatePulldownMenu(top, "dirMenu", NULL, 0);

  /* Create children */
  for(n = 0; n < 9; n++) {
    char buffer[10];
    
    sprintf(buffer, "l%d", n);
    CreateMenuButton(buffer, dirMenu, NULL, NULL, 0);
  }

  /* Create the last child */
  tmp = XmStringCreateLocalized("/");
  n = 0;
  XtSetArg(args[n], XmNlabelString, tmp); n++;
  lroot = XmCreatePushButtonGadget(dirMenu, "lroot", args, n);
  XtAddCallback(lroot, XmNactivateCallback, 
		(XtCallbackProc) readdirCB, (XtPointer) "/");
  XtManageChild(lroot);
  XmStringFree(tmp);

  tmp = XmStringCreateLocalized("Directory");
  n = 0;
  XtSetArg(args[n], XmNlabelString, tmp); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_NONE); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  XtSetArg(args[n], XmNsubMenuId, dirMenu); n++;
  dirOM = XmCreateOptionMenu(form, "dirOM", args, n);
  XtManageChild(dirOM);
  XmStringFree(tmp);

  n = 0;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, dirOM); n++;
  XtSetArg(args[n], XmNleftOffset, 30); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, dirOM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, dirOM); n++;
  where = XmCreateLabelGadget(form, "Where", args, n);
  XtManageChild(where);

  n = 0;
  XtSetArg(args[n], XmNwidth, 450); n++;
  XtSetArg(args[n], XmNheight, 350); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, dirOM); n++;
  XtSetArg(args[n], XmNtopOffset, 5); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNresizable, True); n++;
  XtSetArg(args[n], XmNleftOffset, 5); n++;
  XtSetArg(args[n], XmNrightOffset, 5); n++;
  XtSetArg(args[n], XmNbottomOffset, 5); n++;
  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
  XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmSTATIC); n++;
  XtSetArg(args[n], XmNscrolledWindowMarginWidth, 5); n++;
  XtSetArg(args[n], XmNscrolledWindowMarginHeight, 5); n++;
  sw = XmCreateScrolledWindow(form, "sw", args, n);
  XtManageChild(sw);

  num_headings = 0;
  headings[num_headings++] = XmStringCreateLocalized("Filename");
  headings[num_headings++] = XmStringCreateLocalized("Owner");
  headings[num_headings++] = XmStringCreateLocalized("Permissions");
  headings[num_headings++] = XmStringCreateLocalized("Size");

  n = 0;
  XtSetArg(args[n], XmNspatialIncludeModel, XmAPPEND); n++;
  XtSetArg(args[n], XmNspatialStyle, XmGRID); n++;
  XtSetArg(args[n], XmNspatialResizeModel, XmGROW_MINOR); n++;
  XtSetArg(args[n], XmNspatialSnapModel, XmCENTER); n++;
  XtSetArg(args[n], XmNdetailColumnHeading, headings); n++;
  XtSetArg(args[n], XmNdetailColumnHeadingCount, num_headings); n++;
  container = XmCreateContainer(sw, "container", args, n);
  XtManageChild(container);
  XtAddCallback(container, XmNdefaultActionCallback, 
		(XtCallbackProc) selectCB, NULL);
  XtAddCallback(container, XmNoutlineChangedCallback, 
		(XtCallbackProc) outlineCB, NULL);

  for(n = 0; n < num_headings; n++)
    XmStringFree(headings[n]);

  return(top);
}
