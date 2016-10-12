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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$XConsortium: TrackLoc.c /main/12 1995/10/25 20:24:41 cde-sun $"
#endif
#endif
/*
*  (c) Copyright 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */

#include <Xm/XmP.h>
#include "XmI.h"
#include "MessagesI.h"

#define GRABPTRERROR    _XmMMsgCascadeB_0003


/********    Static Function Declarations    ********/

static Widget _XmInputInWidget( 
                        Widget w,
#if NeedWidePrototypes
                        int x,
                        int y) ;
#else
                        Position x,
                        Position y) ;
#endif /* NeedWidePrototypes */

/********    End Static Function Declarations    ********/



/******************************************************/
/* copy from XmeGadgetAtPoint, buts works for widget too,
   only used in this module so far, so let it static */
static Widget 
_XmInputInWidget(
        Widget w,
#if NeedWidePrototypes
        int x,
        int y)
#else
        Position x,
        Position y)
#endif /* NeedWidePrototypes */
{
   int i;
   Widget child;
   CompositeWidget cw = (CompositeWidget) w;
   
   /* loop over the child list to find if there is one at x,y */
   /* well, overlapping won't really work, since I have no standard way to
      check visibility */
   for (i = 0; i < cw->composite.num_children; i++) {
      child = cw->composite.children[i];
      if (XtIsManaged (child)) {
         if (x >= child->core.x && y >= child->core.y && 
             x < child->core.x + child->core.width    && 
             y < child->core.y + child->core.height) 
            return (child);
      }
   }
   return (NULL);
}





/* new tracking locate function that returns the event that trigerred
   the return + this function uses XNextEvent instead of just XMaskEvent
   which was only eating some events and thus causing problem when
   back to the application */

Widget 
XmTrackingEvent(
        Widget widget,
        Cursor cursor,
#if NeedWidePrototypes
        int confineTo,
#else
        Boolean confineTo,
#endif /* NeedWidePrototypes */
	XEvent * pev)
{
    Window      w, confine_to = None;
    Time        lastTime;
    Widget      child;
    Boolean     key_has_been_pressed= False;
    Widget      target ;
    Position x, y ;
    XtAppContext app;
    
    if (widget == NULL) return(NULL);

    app = XtWidgetToApplicationContext(widget);
    _XmAppLock(app);

    w = XtWindowOfObject(widget);
    if (confineTo) confine_to = w;

    lastTime = XtLastTimestampProcessed(XtDisplay(widget));
    XmUpdateDisplay(widget);    

    if (XtGrabPointer(widget, True, 
          /* The following truncation of masks is due to a bug in the Xt API.*/
		      (unsigned int) (ButtonPressMask | ButtonReleaseMask), 
		      GrabModeAsync, GrabModeAsync, 
		      confine_to, cursor, lastTime) != GrabSuccess) {
	XmeWarning(widget, GRABPTRERROR);
	_XmAppUnlock(app);
	return NULL ;
    }

    while (True) {
	/* eat all events, not just button's */
        XNextEvent(XtDisplay(widget), pev);
	/* track only button1release and non first keyrelease */
        if (((pev->type == ButtonRelease) && 
	     (pev->xbutton.button & Button1)) || 
	    ((pev->type == KeyRelease) && key_has_been_pressed)) {

            if ((!confineTo) && (pev->xbutton.window == w)) {
		/* this is the case where we are not confine, so the user
		   can click outside the window, but we want to return
		   NULL */
		if ((pev->xbutton.x < 0) || (pev->xbutton.y < 0) ||
		    (pev->xbutton.x > widget->core.width) ||
		    (pev->xbutton.y > widget->core.height))
		    {
			XtUngrabPointer(widget, lastTime);
			_XmAppUnlock(app);
			return(NULL);
		    }
	    }

	    target = XtWindowToWidget(pev->xbutton.display,
				      pev->xbutton.window);
	    
/* New algorithm that solves the problem of mouse insensitive widgets: 
    ( i.e you can't get help on Label.)
   When we get the Btn1Up event with the window in which the event ocurred,
   and convert it to a widget.  If that widget is a primitive, return it.
   Otherwise, it is a composite or a shell, and we do the following:

   Walk down the child list (for gadgets AND WIDGETS), looking for one which
   contains the x,y.  If none is found, return the manager itself,
   If a primitive or gadget is found, return the object found.
   If a manager is found, execute the above algorithm recursively with
   that manager. */

	    if (target) {
		/* do not change the original pev coordinates */
		x = pev->xbutton.x ;
		y = pev->xbutton.y ;
		/* do not enter the loop for primitive */
		while (XtIsComposite(target) || XtIsShell(target)) {
		    if ((child = _XmInputInWidget(target, x,y)) != NULL) {
			target = child;
			/* if a gadget or a primitive is found, return */
			if (!XtIsComposite(child)) break ;

			/* otherwise, loop */
			x = x - XtX(child) ;
			y = y - XtY(child) ;
		    } else break ;
		    /* no child found at this place: return the manager */
		}
	    }
            break ;
        } else if (pev->type == KeyPress) key_has_been_pressed = True ;
	/* to avoid exiting on the first keyreleased coming from
	   the keypress activatation of the function itself */
    }

    XtUngrabPointer(widget, lastTime);
    _XmAppUnlock(app);
    return(target);
}



/* reimplementation of the old function using the new one and a dummy event */
Widget 
XmTrackingLocate(
        Widget widget,
        Cursor cursor,
#if NeedWidePrototypes
        int confineTo )
#else
        Boolean confineTo )
#endif /* NeedWidePrototypes */
{
    XEvent ev ;

    return XmTrackingEvent(widget, cursor, confineTo, &ev);
}



/***********************************************************************
 * Certain widgets, such as those in a menu, would like the application
 * to look non-scraged (i.e. all exposure events have been processed)
 * before invoking a callback which takes a long time to do its thing.
 * This function grabs all exposure events off the queue, and processes
 * them.
 ***********************************************************************/

void 
XmUpdateDisplay(
        Widget w )
{
   XEvent event;
   Display * display = XtDisplay(w);
   _XmWidgetToAppContext(w);

   _XmAppLock(app);
   XSync (display, 0);

   while (XCheckMaskEvent(display, ExposureMask, &event))
      XtDispatchEvent(&event);
   _XmAppUnlock(app);
}
