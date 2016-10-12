/* $XConsortium: MenuState.c /main/5 1995/07/15 20:52:55 drk $ */
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
#include <Xm/RowColumnP.h>
#include <Xm/ScreenP.h>
#include "MenuStateI.h"

/********    Static Function Declarations    ********/

static void ScreenDestroyCallback ( 
			Widget w, 
			XtPointer client_data, 
			XtPointer call_data );

/********    End Static Function Declarations    ********/


Widget
_XmGetRC_PopupPosted (
       Widget wid)
{
   if (XmIsRowColumn(wid))
      return (RC_PopupPosted(wid));
   else
      return NULL;
}

/*
 * The following two functions are used by menu and menu-item widgets to keep
 * track of whether we're in drag (button down) or traversal mode.
 */
Boolean
_XmGetInDragMode (
        Widget widget)
{
  return((_XmGetMenuState(widget))->MU_InDragMode);
}

void
_XmSetInDragMode(
        Widget widget,
#if NeedWidePrototypes
        int mode )
#else
        Boolean mode )
#endif
{
  (_XmGetMenuState(widget))->MU_InDragMode = mode;
}


/************************************************************************
 *
 * _XmGetMenuState(wid)
 *
 ************************************************************************/
XmMenuState
_XmGetMenuState(
        Widget wid)
{
   XmScreen scrn = (XmScreen) XmGetXmScreen(XtScreen(wid));
   XmMenuState menu_state = (XmMenuState)NULL;

   if ((XmScreen)NULL != scrn)
   {
     menu_state  = 
	(XmMenuState)((XmScreenInfo *)(scrn->screen.screenInfo))->menu_state;

     if ((XmMenuState)NULL == menu_state)
     {
      menu_state = (XmMenuState)XtMalloc(sizeof(XmMenuStateRec));
      ((XmScreenInfo *)(scrn->screen.screenInfo))->menu_state = 
		(XtPointer)menu_state;
      XtAddCallback((Widget)scrn, XtNdestroyCallback, 
		    ScreenDestroyCallback, (XtPointer) NULL);

      menu_state->RC_LastSelectToplevel = NULL;
      menu_state->RC_ButtonEventStatus.time = (unsigned) -1;
      menu_state->RC_ButtonEventStatus.verified = FALSE;
      menu_state->RC_ButtonEventStatus.waiting_to_be_managed = TRUE;
      /*menu_state->RC_ButtonEventStatus.event = (XButtonEvent)NULL;*/
      menu_state->RC_ReplayInfo.time = 0;
      menu_state->RC_ReplayInfo.toplevel_menu = NULL;
      menu_state->RC_activeItem = NULL;
      menu_state->RC_allowAcceleratedInsensitiveUnmanagedMenuItems = False;
      menu_state->RC_menuFocus.oldFocus = (Window)NULL;
      menu_state->RC_menuFocus.oldRevert = 0;
      menu_state->RC_menuFocus.oldWidget = NULL;

      menu_state->MS_LastManagedMenuTime = (Time)0L;

      menu_state->MU_InDragMode = False;
      menu_state->MU_CurrentMenuChild = NULL;
      menu_state->MU_InPMMode = False;
     }
   }

   return menu_state;
}

/*ARGSUSED*/
static void 
ScreenDestroyCallback
	( Widget w,
        XtPointer client_data,
        XtPointer call_data )	/* unused */
{
   XmScreen scrn = (XmScreen) XmGetXmScreen(XtScreen(w));
   if ((XmScreen)NULL != scrn) {
       XmMenuState menu_state = 
	   (XmMenuState)((XmScreenInfo *)
			 (scrn->screen.screenInfo))->menu_state;
	if ((XmMenuState)NULL != menu_state) {
	    XtFree((char*)menu_state);
   	}
   }
}
