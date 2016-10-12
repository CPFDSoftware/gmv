/* 
 *  @OPENGROUP_COPYRIGHT@
 *  COPYRIGHT NOTICE
 *  Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 *  Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 *  ALL RIGHTS RESERVED (MOTIF). See the file named COPYRIGHT.MOTIF for
 *  the full copyright text.
 *  
 *  This software is subject to an open license. It may only be
 *  used on, with or for operating systems which are themselves open
 *  source systems. You must contact The Open Group for a license
 *  allowing distribution and sublicensing of this software on, with,
 *  or for operating systems which are not Open Source programs.
 *  
 *  See http://www.opengroup.org/openmotif/license for full
 *  details of the license agreement. Any use, reproduction, or
 *  distribution of the program constitutes recipient's acceptance of
 *  this agreement.
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 *  PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 *  WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 *  OR FITNESS FOR A PARTICULAR PURPOSE
 *  
 *  EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 *  NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 *  EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGES.
*/ 
/* 
 * HISTORY
*/ 
#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$XConsortium: Dest.c /main/12 1995/09/19 23:00:44 cde-sun $"
#endif
#endif
/* (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/* (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <Xm/XmP.h>
#include <Xm/DisplayP.h>
#include "XmI.h"
#include "DestI.h"

/* 
   This function is used for setting the "last editable widget on which a
   select, edit, insert, or paste operation was performed and is a destination
   for quick paste and certain clipboard functions" (for this display 
   connection) and is not necessarily a text widget.  
   Under current usage by Motif internals:
   This function is for squirreling away the widget that has the destination
   cursor so that it can be retrieved when pasting from a menu.  Called by 
   _XmTextSetDestinationSelection.
*/
void _XmSetDestination (Display *dpy, Widget w)
{
      XmDisplay   dd = (XmDisplay) XmGetXmDisplay(dpy);	/* w may be NULL */
      if ((XmDisplay)NULL != dd)
	((XmDisplayInfo *)(dd->display.displayInfo))->destinationWidget =
		w;	
}

/* This public function retrieves the widget saved by _XmSetDestination. */
Widget XmGetDestination (Display *display)
{
      XmDisplay   dd = (XmDisplay) XmGetXmDisplay(display);
      Widget w = (Widget)NULL;
      _XmDisplayToAppContext(display);

      _XmAppLock(app);
      if ((XmDisplay)NULL != dd)
	 w = ((XmDisplayInfo *)(dd->display.displayInfo))->destinationWidget;
      _XmAppUnlock(app);
      return w;
}
