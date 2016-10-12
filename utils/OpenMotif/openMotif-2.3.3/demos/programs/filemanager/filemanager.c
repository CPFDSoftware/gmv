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
static char rcsid[] = "$XConsortium: filemanager.c /main/6 1995/07/14 09:41:39 drk $"
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <Xm/Xm.h>           /* Motif Toolkit */
#include <Xm/Container.h>
#include "filemanager.h"

Display  	*display;
XtAppContext  	app_context;
Widget toplevel, mainW, dirOM, fileviewer;
Widget gotoDialog, displayLabel;

static int ErrorHandler(Display *display, XErrorEvent *event);
static void UpdateDir(XtPointer, XtIntervalId*);

#define APP_CLASS "XmdFilemanager"
time_t ltm = 0;
unsigned int updateTime;
char *deleteCommand;
XrmQuark app_class_quark;
XrmQuark app_quark;

char * fallback_resources[] = {
"*updateTime:				2000", /* Check on directory every two seconds */
"*deleteCommand:			rm %s",
"*types.o.largeIcon:			obj.xpm",
"*types.c.largeIcon:			code.xpm",
"*types.h.largeIcon:			code.xpm",
"*types.xpm.largeIcon:			pix.xpm",
"*types.xpm.smallIcon:			s_pix.xpm",
"*types.xpm.action:			vueicon -f %s",
"*types.filemanager.action:		%s",
"*types.default_file.largeIcon:		file.xpm",
"*types.default_file.smallIcon:		s_file.xpm",
"*types.default_dir.largeIcon:		dir.xpm",
"*types.default_dir.smallIcon:		s_dir.xpm",
"*types.default_exec.largeIcon:		exec.xpm",
"*types.default_exec.smallIcon:		s_exec.xpm",
"*types.default_none.largeIcon:		none.xpm",
"*types.default_none.smallIcon:		s_none.xpm",
"*types.default_file.action:		$EDITOR %s",
"*types.default_exec.action:		xterm -e %s",
"*XmContainer.OutlineButton.shadowThickness: 0",
/* "*XmContainer.OutlineButton*armPixmap:	arm.xpm", */
"*XmContainer*foreground:		black",
"*XmContainer*background:		gray90",
"*XmContainer.selectColor:		lightblue",
"*XmContainer.outlineIndentation:	30",
"*XmContainer.outlineColumnWidth:	6cm",
"*XmContainer.detailTabList:		+3cm,+3cm,+3cm",
"*XmIconGadget.renderTable:		*helvetica-bold-r-*-10-*",
"*XmIconGadget.shadowThickness:		0",
"*show_hidden.labelString:		Show Hidden Files",
"*show_hidden.indicatorOn:		XmINDICATOR_CHECK",
"*show_hidden.indicatorSize:		15",
"*help_manager.helpFile:		filemanager",
NULL
};

/*
 *  Main program
 */
int
main(int argc, char* argv[])
{
  Arg		args[10];
  int 		n = 0;
  Pixel		fg, bg;
  Display	*display;
  int		time;
  char		*str;
  int 		i;

  XtSetArg(args[n], XmNallowShellResize, True);  n++;
  toplevel = XtVaAppInitialize(&app_context, APP_CLASS, NULL, 0, &argc, argv,
			       fallback_resources, NULL);

  app_class_quark = XrmStringToQuark(APP_CLASS);
  app_quark = XrmStringToQuark(argv[0]);

  mainW = CreateInterface("main", toplevel);

  /* First try the resource database,  then use the fallbacks below */
  str = XGetDefault(XtDisplay(toplevel), APP_CLASS, "updateTime");
  if (str == NULL)
    str = XGetDefault(XtDisplay(toplevel), argv[0], "updateTime");
  if (str == NULL)
    updateTime = 2000;
  else
    updateTime = atoi(str);

  str = XGetDefault(XtDisplay(toplevel), APP_CLASS, "deleteCommand");
  if (str == NULL)
    str = XGetDefault(XtDisplay(toplevel), argv[0], "deleteCommand");
  if (str == NULL)
    deleteCommand = "rm %s";
  else
    deleteCommand = str;

  /* Find the widgets in the hierarchy */
  dirOM = XtNameToWidget(mainW, "*dirOM");

  dirLabel = (Widget *) XtMalloc(sizeof(Widget) * ndirLabel);
  for (i = 0; i < ndirLabel; i++) {
    char buf[16];
    sprintf(buf, "*l%d", i);
    dirLabel[i] = XtNameToWidget(mainW, buf);
    paths[i] = NULL;
  }

  fileviewer = XtNameToWidget(mainW, "*container");
  gotoDialog = XtNameToWidget(mainW, "*gotoDialog");
  
  displayLabel = XtNameToWidget(mainW, "*Where");

  XtVaGetValues(fileviewer, XmNforeground, &bg,	
		XmNbackground, &fg, NULL, NULL);

  XtManageChild(mainW);

  /* Install our special error handler */
  XSetErrorHandler((XErrorHandler) ErrorHandler);

  /* Add the UTM callbacks on the container area */
  XtAddCallback(fileviewer, XmNdestinationCallback, 
		(XtCallbackProc) targetDestinationCallback, NULL);
  XtAddCallback(fileviewer, XmNconvertCallback, 
		(XtCallbackProc) targetConvertCallback, NULL);
  XtAddCallback(XtParent(fileviewer), XmNresizeCallback,
		fixViewerSize, NULL);

  readdirCB(fileviewer, ".", NULL);

  XtRealizeWidget(toplevel);

  XmContainerRelayout(fileviewer);

  XtAppAddTimeOut(app_context, updateTime,
		  UpdateDir, (XtPointer) 0);

  XtAppMainLoop(app_context);

  return 0;    /* make compiler happy */
}

/* Adjust the size of the underlying container widget to be
   at least large enough to fit in the clipwindow */
void 
fixViewerSize(Widget w, XtPointer i1, XtPointer i2) 
{
  Dimension width, height, container_w, container_h;

  /* Get the correct dimensions of the clipwindow */
  XtVaGetValues(w, XtNwidth, &width, XtNheight, &height, NULL, NULL);

  /* Get the correct dimensions of the container */
  XtVaGetValues(fileviewer, XtNwidth, &container_w,
		XtNheight, &container_h, NULL, NULL);

  XtVaSetValues(fileviewer, XtNwidth, width, NULL, NULL);
  if (container_h < height) 
    XtVaSetValues(fileviewer, XtNheight, height, NULL, NULL);
}

/* Error handler for X protocol errors.  Continue after error */

static int 
ErrorHandler(Display *display, XErrorEvent *event)
{
  char errortext[100];
  XmString tmp;

  XGetErrorText(display, event -> error_code, errortext, 100);

  printf("X Protocol error: %s\n", errortext);
  printf("XID %ld serial %ld major %d minor %d\n",
	 event -> resourceid,
	 event -> serial,
	 event -> request_code,
	 event -> minor_code);

  return 0;
}

/* Check for modification of current directory.  We don't handle updates
   in subdirectories (at least yet),  but this *is* free software */
static void 
UpdateDir(XtPointer data, XtIntervalId *id) 
{
  struct stat buf;

  XtAppAddTimeOut(app_context, updateTime, 
		  (XtTimerCallbackProc) UpdateDir, 0);

  if (stat(currentdir, &buf) != 0) {
    printf("Can't stat directory: %s\n", currentdir);
    return;
  }

  if (ltm != buf.st_ctime) {
    read_directory((Widget) NULL, ".");
  }
}

