/* $XConsortium: DrHiDash.c /main/5 1995/07/15 20:50:42 drk $ */
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


#include "XmI.h"
#include "DrawI.h"

/****************************_XmDrawHighlight***************************
 *
 * This function modifies the given gc, which therefore needs to be created
 *   using XCreateGC or XtAllocateGC.
 *
 ***************************************************************************/

void _XmDrawHighlight(Display *display, Drawable d, 
			  GC gc, 
#if NeedWidePrototypes
                          int x, int y, 
			  int width, int height,
			  int highlight_thickness,
#else
                          Position x, Position y, 
			  Dimension width, Dimension height,
			  Dimension highlight_thickness,
#endif /* NeedWidePrototypes */
                   int line_style)
{
   XSegment seg[4];
   register Dimension half_hl = highlight_thickness/2 ;
   register Dimension cor = highlight_thickness % 2 ;
   XGCValues gcvalues;

   if (!d || !highlight_thickness || !width || !height) return ;

   /* the XmList dash case relies on this particular order of X segments */
   
   seg[0].x1 = seg[2].x1 = x ;
   seg[0].y1 = seg[0].y2 = y + half_hl ;
   seg[0].x2 = x + width - highlight_thickness ;
   seg[1].x1 = seg[1].x2 = x + width - half_hl - cor;
   seg[1].y1 = seg[3].y1 = y ;
   seg[3].y2 = y + height - half_hl;
   seg[2].y1 = seg[2].y2 = y + height - half_hl - cor;
   seg[3].x1 = seg[3].x2 = x + half_hl ;
   seg[2].x2 = x + width ;
   seg[1].y2 = y + height ;

   /* first save the current values we want to change */
   XGetGCValues(display, gc,
		GCLineWidth|GCLineStyle|GCCapStyle|GCJoinStyle,
		&gcvalues);
   /* change them and draw the lines */
   XSetLineAttributes(display, gc,  highlight_thickness, line_style, 
		      CapButt, JoinMiter);
   XDrawSegments (display, d, gc, seg, 4);

   /* put them back */
   XSetLineAttributes(display, gc,  
		      gcvalues.line_width, gcvalues.line_style, 
		      gcvalues.cap_style, gcvalues.join_style);
  
   /** note that the above is a hack, a read-only GC shoudl not 
     be modified, period */
}
