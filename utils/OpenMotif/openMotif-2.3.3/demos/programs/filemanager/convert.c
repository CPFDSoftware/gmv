/* $XConsortium: convert.c /main/5 1995/07/15 20:45:16 drk $ */
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

#include <Xm/Xm.h>
#include <Xm/Container.h>
#include <Xm/Transfer.h>
#include "filemanager.h"

void 
targetConvertCallback(Widget wid, XtPointer ignore,
		      XmConvertCallbackStruct *cs)
{
  Atom XA_TARGETS = XInternAtom(XtDisplay(wid), XmSTARGETS, False);
  Atom XA_FILE = XInternAtom(XtDisplay(wid), XmSFILE, False);
  Atom XA_FILENAME = XInternAtom(XtDisplay(wid), XmSFILE_NAME, False);
  Atom XA_MOTIF_EXPORTS = 
    XInternAtom(XtDisplay(wid), XmS_MOTIF_EXPORT_TARGETS, False);
  Atom XA_MOTIF_REQUIRED = 
    XInternAtom(XtDisplay(wid), XmS_MOTIF_CLIPBOARD_TARGETS, False);
  Atom XA_MOTIF_DROP =
    XInternAtom(XtDisplay(wid), XmS_MOTIF_DROP, False);

  if (cs -> selection == XA_MOTIF_DROP &&
      (cs -> target == XA_TARGETS ||
       cs -> target == XA_MOTIF_EXPORTS ||
       cs -> target == XA_MOTIF_REQUIRED)) {
    Atom *targs;
    targs = (Atom *) XtMalloc(sizeof(Atom) * 2);
    targs[0] = XA_FILE;
    targs[1] = XA_FILENAME;
    cs -> value = (XtPointer) targs;
    cs -> length = 2;
    cs -> type = XA_ATOM;
    cs -> format = 32;
    cs -> status = XmCONVERT_MERGE;
  } else if (cs -> target == XA_FILE ||
	     cs -> target == XA_FILENAME) {
    if (cs -> location_data == NULL) {
      WidgetList selected;
      Cardinal count;
      int i;

      /* First get list of selected items. */
      XtVaGetValues(fileviewer, 
		    XmNselectedObjects, &selected,
		    XmNselectedObjectCount, &count,
		    NULL, NULL);

      if (count > 0) {
	char *rval = NULL;
	int curpos = 0;

	for(i = 0; i < count; i++) {
	  char *path;
	  int pathlen;

	  path = getPathFromIcon(selected[i]);
	  pathlen = strlen(path) + 1;
	  rval = XtRealloc(rval, curpos + pathlen);
	  /* This will include the terminating NULL byte.  Important,
	     do not remove this NULL byte,  it acts as a separator */
	  strncpy(rval, path, pathlen);
	  curpos += pathlen;
	}
	cs -> format = 8;
	cs -> length = curpos - 1;
	cs -> type = XA_STRING;
	cs -> status = XmCONVERT_DONE;
      } else {
	cs -> status = XmCONVERT_REFUSE;
      }
    } else {
      char *path;

      path = getPathFromIcon(cs -> location_data);

      cs -> value = path;
      if (path != NULL)
	cs -> length = strlen(path);
      else
	cs -> length = 0;
      cs -> format = 8;
      cs -> type = XA_STRING;
      cs -> status = XmCONVERT_DONE;
    }
  } 
}

void 
targetDestinationCallback(Widget w, XtPointer ignore,
			  XmDestinationCallbackStruct *cs)
{
  /* Later */
}
