/* $XConsortium: IsMwmRun.c /main/7 1996/05/21 12:02:11 pascale $ */
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <Xm/XmP.h>
#include <Xm/MwmUtil.h>
#include "XmI.h"

/************************************************************************
 *
 *  XmIsMotifWMRunning
 *
 ************************************************************************/
Boolean 
XmIsMotifWMRunning(
	 Widget shell )
{
    Atom	motif_wm_info_atom;
    Atom	actual_type;
    int		actual_format;
    unsigned long num_items, bytes_after;
    PropMotifWmInfo	*prop = 0;
    Window	root = RootWindowOfScreen(XtScreen(shell));

    _XmWidgetToAppContext(shell);
 
    _XmAppLock(app);

    motif_wm_info_atom = XInternAtom(XtDisplay(shell),
				       _XA_MOTIF_WM_INFO,
				       FALSE);
    _XmProcessLock();

    XGetWindowProperty (XtDisplay(shell), 
			 root,
			 motif_wm_info_atom,
			 0, (long)PROP_MOTIF_WM_INFO_ELEMENTS,
			 FALSE, motif_wm_info_atom,
			 &actual_type, &actual_format,
			 &num_items, &bytes_after,
			 (unsigned char **) &prop);
    _XmProcessUnlock();

    if ((actual_type != motif_wm_info_atom) ||
	 (actual_format != 32) ||
	 (num_items < PROP_MOTIF_WM_INFO_ELEMENTS))
      {
	   if (prop != 0) XFree((char *)prop);
           _XmAppUnlock(app);
	   return (FALSE);
      }
    else
      {
	   Window	wm_window = (Window) prop->wmWindow;
	   Window	top, parent, *children;
	   unsigned int	num_children;
	   Boolean	returnVal;
	   Cardinal	i;

	   if (XQueryTree(XtDisplay(shell),
			  root, &top, &parent,
			  &children, &num_children))
	     {
		 i = 0; 
		 while ((i < num_children) && (children[i] != wm_window))
		   i++;
		 returnVal =  (i == num_children) ? FALSE : TRUE;
	     }
	   else
	     returnVal = FALSE;

	   if (prop) XFree((char *)prop);
	   if (children) XFree((char *)children);
           _XmAppUnlock(app);
	   return (returnVal);
      }
}
