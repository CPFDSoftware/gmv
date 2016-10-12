/* $XConsortium: actions.c /main/5 1995/07/15 20:45:12 drk $ */
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
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <Xm/Xm.h>           /* Motif Toolkit */
#include <Mrm/MrmPublic.h>   /* Mrm */
#include <Xm/Container.h>
#include <Xm/SelectioB.h>
#include <Xm/TextF.h>
#include "filemanager.h"

void 
gotoCB(Widget widget, XtPointer i1, XtPointer cb)
{
  Widget text;
  char *str;

  text = XtNameToWidget(widget, "*Text");

  str = XmTextFieldGetString(text);

  readdirCB(widget, str, NULL);

  XtFree(str);
}

void 
manageCB(Widget widget, Widget w_to_manage, XtPointer callback_data)
{
  if (w_to_manage != (Widget) NULL)
    XtManageChild(w_to_manage);
}

void 
viewCB(Widget widget, char   *type, XtPointer callback_data)
{
  XtEnum size, spatial;

  if (strcmp(type, "large") == 0) {
    size = XmLARGE_ICON;
    spatial = XmSPATIAL;
  } else if (strcmp(type, "small") == 0) {
    size = XmSMALL_ICON;
    spatial = XmSPATIAL;
  } else if (strcmp(type, "detail") == 0) {
    size = XmSMALL_ICON;
    spatial = XmDETAIL;
  }

  XtVaSetValues(fileviewer,
		XmNlayoutType, spatial,
		XmNentryViewType, size,
		0, 0);

  fixViewerSize(XtParent(fileviewer), NULL, NULL);

  XmContainerRelayout(fileviewer);
}

void 
quitCB(Widget widget, char *tag, XmAnyCallbackStruct *callback_data)
{
  exit(0);
}

char* 
fullpath(char *filename)
{
  char buf[1024];
  char *rstr;

  /* This is for UNIX,  write something else for a non-UNIX OS */
  if (strcmp(currentdir, "/") == 0)
    sprintf(buf, "/%s", filename);
  else if (filename[0] == '/')
    sprintf(buf, "%s%s", currentdir, filename);
  else
    sprintf(buf, "%s/%s", currentdir, filename);

  rstr = XtNewString(buf);

  return(rstr);
}


void 
showHiddenCB(Widget widget, XtPointer ignore,
	     XmToggleButtonCallbackStruct *callback_data)
{
  showHidden = callback_data -> set;

  read_directory((Widget) NULL, ".");
}

void 
newFolder(Widget widget, XtPointer ignore, XtPointer ignore2)
{
  char buf[256];
  int i = 0;
  int status;
  Boolean done = False;
  struct stat statbuf;

  while(! done) {
    sprintf(buf, "%s/unnamed%d", currentdir, i);
    status = stat(buf, &statbuf);
    if (status != 0) {
      if (errno == ENOENT)
	done = True;
      else
	return; /* Bad error */
    }
    if (i >= 100) return;
    i++;
  }

  mkdir(buf, 0755);

}
     
void 
deleteItem(Widget widget, XtPointer ignore, XtPointer ignore2)
{
  WidgetList selected;
  Cardinal count;
  int i;

  /* First get list of selected items. */
  XtVaGetValues(fileviewer, 
		XmNselectedObjects, &selected,
		XmNselectedObjectCount, &count,
		NULL, NULL);

  if (count <= 0) return;
 
  for(i = 0; i < count; i++) {
    char buf[256];
    sprintf(buf, deleteCommand, getPathFromIcon(selected[i]));
    system(buf);
  }

  XtVaSetValues(fileviewer, XmNselectedObjectCount, 0, NULL, NULL);
}




