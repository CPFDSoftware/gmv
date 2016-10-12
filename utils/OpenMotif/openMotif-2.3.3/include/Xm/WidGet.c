/* $XConsortium: WidGet.c /main/6 1995/10/25 20:27:58 cde-sun $ */
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
#include <Xm/PrimitiveP.h>
#include <Xm/ManagerP.h>
#include <Xm/GadgetP.h>



/************************************************************************
 *
 *   XmWidgetGetBaselines
 *
 ************************************************************************/

Boolean
XmWidgetGetBaselines(
        Widget wid,
        Dimension **baselines,
        int *line_count)
{
  _XmWidgetToAppContext(wid);
  _XmAppLock(app);

  if (XmIsPrimitive(wid))
      {
	  XmPrimitiveClassExt              *wcePtr;
	  WidgetClass   wc = XtClass(wid);
	  
	  wcePtr = _XmGetPrimitiveClassExtPtr(wc, NULLQUARK);
	  
	  if (*wcePtr && (*wcePtr)->widget_baseline)
	      {
		_XmAppUnlock(app);
		return( (*((*wcePtr)->widget_baseline)) 
			 (wid, baselines, line_count)) ;
	      } 
      }
  else if (XmIsGadget(wid))
      {
	  XmGadgetClassExt              *wcePtr;
	  WidgetClass   wc = XtClass(wid);
	  
	  wcePtr = _XmGetGadgetClassExtPtr(wc, NULLQUARK);
	  
	  if (*wcePtr && (*wcePtr)->widget_baseline)
	      {
		_XmAppUnlock(app);
		return( (*((*wcePtr)->widget_baseline)) 
			 (wid, baselines, line_count)) ;
	      }
      }
  _XmAppUnlock(app);
  return (False);
}


/************************************************************************
 *
 *   XmWidgetDisplayRect
 *
 ************************************************************************/

Boolean
XmWidgetGetDisplayRect(
        Widget wid,
        XRectangle *displayrect)
{
    _XmWidgetToAppContext(wid);
    _XmAppLock(app);

    if (XmIsPrimitive(wid))
	{
	    XmPrimitiveClassExt              *wcePtr;
	    WidgetClass   wc = XtClass(wid);
	    
	    wcePtr = _XmGetPrimitiveClassExtPtr(wc, NULLQUARK);
	    
	    if (*wcePtr && (*wcePtr)->widget_display_rect)
		(*((*wcePtr)->widget_display_rect)) (wid, displayrect);
	    _XmAppUnlock(app);
	    return (True);
	}
    else if (XmIsGadget(wid))
	{
	    XmGadgetClassExt              *wcePtr;
	    WidgetClass   wc = XtClass(wid);
	    
	    wcePtr = _XmGetGadgetClassExtPtr(wc, NULLQUARK);
	    
	    if (*wcePtr && (*wcePtr)->widget_display_rect)
		(*((*wcePtr)->widget_display_rect)) (wid, displayrect);
	    _XmAppUnlock(app);
	    return (True);
	}
    else {
	_XmAppUnlock(app);
	return (False);
    }
}
