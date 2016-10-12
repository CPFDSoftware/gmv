/* $XConsortium: ChColor.c /main/6 1995/10/25 19:55:45 cde-sun $ */
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
#include <Xm/ManagerP.h>
#include <Xm/PrimitiveP.h>
#include <Xm/GadgetP.h>
#include "XmI.h"

/*********************************************************************
 *
 *  XmChangeColor - change set of colors for existing widget, given 
 *                  background color
 *
 *********************************************************************/
void 
XmChangeColor(
	      Widget w,
	      Pixel background )

{
  Widget windowed_ancestor = w;
  Pixel foreground_ret;
  Pixel topshadow_ret;
  Pixel bottomshadow_ret;
  Pixel select_ret; 
  Pixel use_background = background;
  Pixel gadget_background;
  Arg args[5];

  /*
   * If the gadget is in BC mode (for colors), it must follow its parent's
   * color scheme.  We therefore ignore the background pixel that was
   * passed in.
   */

  _XmWidgetToAppContext(w);
  _XmAppLock(app);

  if (XmIsGadget(w))
    {
      Widget parent = XtParent(w);

      windowed_ancestor = parent;
      XtSetArg(args[0], XmNbackground, &gadget_background);
      XtGetValues(w,args,1);
      if (gadget_background == parent->core.background_pixel)
	use_background = parent->core.background_pixel;
    }

  XmGetColors(windowed_ancestor->core.screen, windowed_ancestor->core.colormap,
	      use_background, &foreground_ret, &topshadow_ret,
	      &bottomshadow_ret, NULL );

  if ( (XmIsManager(w)) ||  (XmIsPrimitive(w)) || (XmIsGadget(w)) )
    { 
      XtSetArg (args[0], XmNbackground, (XtArgVal) use_background);
      XtSetArg (args[1], XmNforeground, (XtArgVal) foreground_ret);
      XtSetArg (args[2], XmNtopShadowColor, (XtArgVal) topshadow_ret);
      XtSetArg (args[3], XmNbottomShadowColor, (XtArgVal) bottomshadow_ret);
      XtSetArg (args[4], XmNhighlightColor, (XtArgVal) foreground_ret);
      
      XtSetValues (w, args, 5);
      
      if (XmIsPrimitive(w) || XmIsGadget(w))
	{
	  if ( (XmIsScrollBar(w)) ||
	       (XmIsPushButton(w)) || (XmIsPushButtonGadget(w)) ||
	       (XmIsToggleButton(w)) || (XmIsToggleButtonGadget(w)) )
	    { 
	      XmGetColors( windowed_ancestor->core.screen,
			   windowed_ancestor->core.colormap,
			   background, NULL, NULL, NULL, &select_ret);

	      if (XmIsToggleButton(w) || XmIsToggleButtonGadget(w))
	        {
		  XtSetArg (args[0], XmNselectColor, (XtArgVal) select_ret);
		  XtSetArg (args[1], XmNunselectColor, (XtArgVal) use_background);
	          XtSetValues (w, args, 2);
	        }
		else
		{
	          if (XmIsScrollBar(w))
		    XtSetArg (args[0], XmNtroughColor, (XtArgVal) select_ret);
	          else if (XmIsPushButton(w) || XmIsPushButtonGadget(w))
		    XtSetArg (args[0], XmNarmColor, (XtArgVal) select_ret);
	          XtSetValues (w, args, 1);
	        }
	    }
	}
    }
  _XmAppUnlock(app);
}



