/* $XConsortium: onHelp.c /main/4 1995/07/15 20:44:13 drk $ */
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

#include <stdlib.h>
#include <Xm/Xm.h>
#include "Help.h"

void quitCB( Widget widget, char *tag, XmAnyCallbackStruct *callback_data )
{
  exit(0);
}

/*
 *  Main program
 */
int main(int argc, String argv[])
{
  Arg	args[10];
  XtAppContext app_context;
  int n;
  Widget toplevel, help;
  Cardinal size;
  XmRendition rend[10];
  XmRenderTable rt;
  int i;
  XmTab tabs[5];
  XmTabList tablist;
  Widget dismiss;

  toplevel = XtVaAppInitialize(&app_context, "HelpOnHelp", NULL, 0, 
			       &argc, argv, NULL, 0);
  n = 0;
  XtSetArg(args[n], XmdNhelpFile, "helpOn"); n++;
  help = XmdCreateHelp(toplevel, "help", args, n);
  dismiss = XtNameToWidget(help, "*Dismiss");
  XtAddCallback(dismiss, XmNactivateCallback,
		(XtCallbackProc) quitCB, NULL);
  /* Get the rendertable and add some new renditions */
  XtVaGetValues(help, XmNrenderTable, &rt, NULL, NULL);
  /* Make the tabs for the tables */
  n = 0;
  tabs[n] = XmTabCreate(2.0, XmINCHES, XmABSOLUTE,
			XmALIGNMENT_BEGINNING, NULL); n++;
  tabs[n] = XmTabCreate(4.0, XmINCHES, XmABSOLUTE,
			XmALIGNMENT_BEGINNING, NULL); n++;
  tablist = XmTabListInsertTabs(NULL, tabs, n, 0);
  for(i = 0; i < n; i++) XmTabFree(tabs[i]);

  /* Add extra renditions needed for the help file */
  n = 0; i = 0;
  XtSetArg(args[n], XmNtabList, tablist); n++;
  rend[i] = XmRenditionCreate(help, "table", args, n); i++;
  n = 0;
  XtSetArg(args[n], XmNfontName, "*courier-medium-r-*-12-*"); n++;
  XtSetArg(args[n], XmNfontType, XmFONT_IS_FONT); n++;
  rend[i] = XmRenditionCreate(help, "program", args, n); i++; 
  n = 0;
  XtSetArg(args[n], XmNunderlineType, XmSINGLE_LINE); n++;
  rend[i] = XmRenditionCreate(help, "underline", args, n); i++; 
  /* Make a copy so that setvalues will work correctly */
  rt = XmRenderTableCopy(rt, NULL, 0);
  rt = XmRenderTableAddRenditions(rt, rend, i, XmMERGE_NEW);
  for(n = 0; n < i; n++) XmRenditionFree(rend[n]);

  XtVaSetValues(help, XmNrenderTable, rt, NULL, NULL);
  XmRenderTableFree(rt);

  XtManageChild(help);

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);

  return 0;    /* make compiler happy */
}
