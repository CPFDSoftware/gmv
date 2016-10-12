/* $XConsortium: AdjView.c /main/5 1995/07/15 20:43:19 drk $ */
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
/*******************************************************************
   The following code is aimed at constraining the work_window size
   in an AUTOMATIC scrolledwindow so that it never becomes smaller
   than the view, in other word, so that you can never see the clipwindow
   background.
   First there are two callback/handler routines and then the convenience
   function that installs them on the clip and work window.
*******************************************************************/

#include <Xm/XmAll.h>
#include "AdjView.h"

static void ClipResize (Widget clip_window, 
			XtPointer client_data, XtPointer call_data) ;
static void WorkResize(Widget work_window, 
		       XtPointer client_data, XEvent *event, Boolean *cont);



/* The following hack needs more thinking, it appears as if the scrollbar 
   highlight thickness must be removed from the work_window new size.
   If that is the model, then the real vertical or scrollbar ht values would
   have to be used instead of this constant */
#define SB_HT 2

static void 
ClipResize (Widget clip_window, XtPointer client_data, XtPointer call_data) 
{
    Widget work_window = (Widget) client_data ;
    static Dimension prev_clip_width = 0, prev_clip_height = 0 ;
    Dimension clip_width, clip_height, work_width, work_height ;

    /* if we're getting bigger, check the current work_window size
       and if we're bigger than it, resize the work_window so it just
       fits the clipwindow.
       if we're getting smaller, ask the preferred work_window size,
       and if we're still bigger than it, resize the work_window so it just
       fits the clipwindow, else resize it to its preferred size. */

    XtVaGetValues(clip_window, XmNwidth, &clip_width, 
		  XmNheight, &clip_height, NULL);

    if (clip_width > prev_clip_width) {
	XtVaGetValues(work_window, XmNwidth, &work_width, NULL);
	if (clip_width > work_width) {
	    XtVaSetValues(work_window, XmNwidth, clip_width - SB_HT, NULL);
	}
    } else {
	XtWidgetGeometry  preferred;
	XtQueryGeometry(work_window, NULL, &preferred);
	if (clip_width > preferred.width) {
	    XtVaSetValues(work_window, XmNwidth, clip_width - SB_HT, NULL);
	} else {
	    XtVaSetValues(work_window, XmNwidth, preferred.width, NULL);
	}
    }    
    prev_clip_width = clip_width ;

    if (clip_height > prev_clip_height) {
	XtVaGetValues(work_window, XmNheight, &work_height, NULL);
	if (clip_height > work_height) {
	    XtVaSetValues(work_window, XmNheight, clip_height - SB_HT, NULL);
	}
    } else {
	XtWidgetGeometry  preferred;
	XtQueryGeometry(work_window, NULL, &preferred);
	if (clip_height > preferred.height) {
	    XtVaSetValues(work_window, XmNheight, clip_height - SB_HT, NULL);
	} else {
	    XtVaSetValues(work_window, XmNheight, preferred.height, NULL);
	}
    }    
    prev_clip_height = clip_height ;
}

static void 
WorkResize(Widget work_window, XtPointer client_data, XEvent *event, Boolean *cont)
{
    Widget clip_window = (Widget) client_data ;
    Dimension clip_width, work_width, clip_height, work_height ;

    /* if the work_window is getting smaller than its clip_window
       parent, resize it so it just fits. */
    XtVaGetValues(work_window, XmNwidth, &work_width, 
		  XmNheight, &work_height, NULL);
    XtVaGetValues(clip_window, XmNwidth, &clip_width, 
		  XmNheight, &clip_height, NULL);
    if (clip_width > work_width + 1) {
	XtVaSetValues(work_window, XmNwidth, clip_width - SB_HT, NULL);
    }
    if (clip_height > work_height + 1) {
	XtVaSetValues(work_window, XmNheight, clip_height - SB_HT, NULL);
    }
}

/*---------That's the public API ----------------------*/
void 
XmdSWAdjustView (Widget sw)
{
    Widget clip_window, work_window ;
    unsigned char scrolling_policy ;

    /* get the clipwindow and _a_ workwindow.
       To be really correct, this code would have to figure out 
       the case where there are more than one work window */
    XtVaGetValues(sw, XmNclipWindow, &clip_window, XmNworkWindow, 
		  &work_window, XmNscrollingPolicy, &scrolling_policy, NULL);

    if (scrolling_policy != XmAUTOMATIC) return ;

     /* attach callback to ClipWindow so that Container never
       goes smaller than clip window. We know ClipWindow is
       a DrawingArea subclass, so we can use a resizeCallback */
    XtAddCallback(clip_window, XmNresizeCallback, ClipResize, 
		  (XtPointer) work_window);
    /* also attach handler to Container sp that it never
       goes smaller than clip window by itself */
    XtAddEventHandler(work_window, StructureNotifyMask, False, WorkResize, 
		      (XtPointer) clip_window);
}

