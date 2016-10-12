/* $XConsortium: dainput_ui.c /main/5 1995/07/15 20:47:32 drk $ */
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
#include <memory.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/CascadeBG.h>
#include <Xm/DrawingA.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/XmIm.h>
#include "dainput.h"

#define WI_DAINPUT 0
#define WI_MAINWINDOW 1
#define WI_MENUBAR 2
#define WI_FILE 3
#define WI_FILEMENU 4
#define WI_EXIT 5
#define WI_HELP 6
#define WI_HELPMENU 7
#define WI_OVERVIEW 8
#define WI_WORKAREA 9

/****************************************************************
 *
 *  Main C code for presentation component
 *
 ****************************************************************/

/**************************************************************
 * DaCreateGui:
 **************************************************************/
Widget DaCreateGui(char    * name,
		   char    * aclass,
		   int       argc,
		   char   ** argv,
		   Display * display)
{
  Arg args[15];
  Widget widget_array[10];
  int n;

  /***************** object of type : ApplicationShell *****************/
  n = 0;
  /* Setup argc & argv attribute */
  XtSetArg(args[n], XtNargc, argc); n++;
  XtSetArg(args[n], XtNargv, argv); n++;

  widget_array[WI_DAINPUT] = 
    XtAppCreateShell(name, aclass, applicationShellWidgetClass,
		     display, args, n);

  /***************** mainWindow : XmMainWindow *****************/
  widget_array[WI_MAINWINDOW] =
    XmCreateMainWindow(widget_array[WI_DAINPUT], "mainWindow", NULL, 0);

  /***************** menuBar : XmMenuBar *****************/
  widget_array[WI_MENUBAR] =
    XmCreateMenuBar(widget_array[WI_MAINWINDOW], "menuBar", NULL, 0);

  /***************** File : XmCascadeButton *****************/
  widget_array[WI_FILE] =
    XmCreateCascadeButton(widget_array[WI_MENUBAR], "File", NULL, 0);

  /***************** fileMenu : XmPulldownMenu *****************/
  widget_array[WI_FILEMENU] =
    XmCreatePulldownMenu(widget_array[WI_MENUBAR], "fileMenu", NULL, 0);
  {
    Arg args[1];
    XtSetArg(args[0], XmNsubMenuId, widget_array[WI_FILEMENU]);
    XtSetValues(widget_array[WI_FILE], args, 1);
  }

  /***************** Exit : XmPushButton *****************/
  widget_array[WI_EXIT] =
    XmCreatePushButton(widget_array[WI_FILEMENU], "Exit", NULL, 0);

  XtAddCallback(widget_array[WI_EXIT], 
		XmNactivateCallback, DaExit, NULL);

  /***************** Help : XmCascadeButton *****************/
  widget_array[WI_HELP] =
    XmCreateCascadeButton(widget_array[WI_MENUBAR], "Help", NULL, 0);

  /***************** helpMenu : XmPulldownMenu *****************/
  widget_array[WI_HELPMENU] =
    XmCreatePulldownMenu(widget_array[WI_MENUBAR], "helpMenu", NULL, 0);
  {
    Arg args[1];
    XtSetArg(args[0], XmNsubMenuId, widget_array[WI_HELPMENU]);
    XtSetValues(widget_array[WI_HELP], args, 1);
  }

  /***************** Overview : XmPushButton *****************/
  widget_array[WI_OVERVIEW] =
    XmCreatePushButton(widget_array[WI_HELPMENU], "Overview", NULL, 0);

  XtAddCallback(widget_array[WI_OVERVIEW], 
		XmNactivateCallback, DaOverView, NULL);

  /***************** workArea : XmDrawingArea *****************/
  n = 0;
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNheight, 35); n++;
  widget_array[WI_WORKAREA] =
    XmCreateDrawingArea(widget_array[WI_MAINWINDOW], "workArea", args, n);

  /* 
   * Register drawing area for input method support and set input 
   * context values.
   */
  XmImRegister(widget_array[WI_WORKAREA], 0);
  {
    Pixel bg, fg;
    Pixmap bg_pixmap;
    n = 0;
    XtSetArg(args[n], XmNbackground, &bg); n++;
    XtSetArg(args[n], XmNforeground, &fg); n++;
    XtSetArg(args[n], XmNbackgroundPixmap, &bg_pixmap);n++;
    XtGetValues(widget_array[WI_WORKAREA], args, n);
    n = 0;
    XtSetArg(args[n], XmNbackground, bg); n++;
    XtSetArg(args[n], XmNforeground, fg); n++;
    XtSetArg(args[n], XmNbackgroundPixmap, bg_pixmap);n++;
    XmImSetValues(widget_array[WI_WORKAREA], args, n);
  }

  /* 
   * Add event handler for focus events. This is needed for the
   * input method to correctly display the input area.
   */
  XtAddEventHandler(widget_array[WI_WORKAREA], 
		    EnterWindowMask|LeaveWindowMask|FocusChangeMask, 
		    False, DaFocusHandler, NULL);

  XtAddCallback(widget_array[WI_WORKAREA],
		XmNexposeCallback, DaRedrawText, NULL);

  XtAddCallback(widget_array[WI_WORKAREA],
		XmNinputCallback, DaInputText, NULL);

  XtAddCallback(widget_array[WI_WORKAREA],
		XmNresizeCallback, DaResizedText, NULL);


  /***************** menuBar : XmMenuBar *****************/
  n = 0;
  XtSetArg(args[n], XmNmenuHelpWidget, widget_array[WI_HELP]); n++;
  XtSetValues(widget_array[WI_MENUBAR], args, n);


  XtManageChild(widget_array[WI_EXIT]);
  XtManageChild(widget_array[WI_FILE]);
  XtManageChild(widget_array[WI_OVERVIEW]);
  XtManageChild(widget_array[WI_HELP]);
  XtManageChild(widget_array[WI_MENUBAR]);
  XtManageChild(widget_array[WI_WORKAREA]);
  XtManageChild(widget_array[WI_MAINWINDOW]);

  /*
   *   Return the first created widget.
   */
  return widget_array[WI_DAINPUT];
}

