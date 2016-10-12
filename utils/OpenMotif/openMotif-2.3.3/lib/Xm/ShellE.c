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
static char rcsid[] = "$XConsortium: ShellE.c /main/10 1995/10/25 20:19:54 cde-sun $"
#endif
#endif
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
/*
*  (c) Copyright 1987, 1988, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY */

#include <Xm/ShellEP.h>
#include <X11/ShellP.h>
#include <Xm/VendorSEP.h>
#include <Xm/ScreenP.h>
#include "XmI.h"


/********    Static Function Declarations    ********/

static void ShellClassPartInitialize( 
                        WidgetClass w) ;
static void StructureNotifyHandler( 
                        Widget wid,
                        XtPointer closure,
                        XEvent *event,
                        Boolean *continue_to_dispatch) ;

/********    End Static Function Declarations    ********/


/***************************************************************************
 *
 * Class Record
 *
 ***************************************************************************/

#define Offset(field) XtOffsetOf( struct _XmShellExtRec, shell.field)

static XtResource shellResources[] =
{    
    {
	XmNuseAsyncGeometry, XmCUseAsyncGeometry, XmRBoolean, 
	sizeof(Boolean), Offset(useAsyncGeometry),
	XmRImmediate, FALSE,
    },
};
#undef Offset

externaldef(xmshellextclassrec)
XmShellExtClassRec xmShellExtClassRec = {
    {	
	(WidgetClass) &xmDesktopClassRec,/* superclass		*/   
	"Shell",			/* class_name 		*/   
	sizeof(XmShellExtRec),	 	/* size 		*/   
	NULL,		 		/* Class Initializer 	*/   
	ShellClassPartInitialize, 	/* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	NULL,				/* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	shellResources,			/* resources          	*/   
	XtNumber(shellResources),	/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	NULL,				/* destroy            	*/   
	NULL,		 		/* resize             	*/   
	NULL, 				/* expose             	*/   
	NULL,		 		/* set_values         	*/   
	NULL, 				/* set_values_hook      */ 
	NULL,			 	/* set_values_almost    */ 
	NULL,				/* get_values_hook      */ 
	NULL, 				/* accept_focus       	*/   
	XtVersion, 			/* intrinsics version 	*/   
	NULL, 				/* callback offsets   	*/   
	NULL,				/* tm_table           	*/   
	NULL, 				/* query_geometry       */ 
	NULL, 				/* display_accelerator  */ 
	NULL, 				/* extension            */ 
    },	
    {					/* ext */
	NULL,				/* synthetic resources	*/
	0,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	NULL,				/* child_class		*/
	XtInheritInsertChild,		/* insert_child		*/
	XtInheritDeleteChild,		/* delete_child		*/
	NULL,				/* extension		*/
    },
    {					/* shell ext		*/
	StructureNotifyHandler,	        /* structureNotify*/
	NULL,				/* extension		*/
    },
};

externaldef(xmShellExtobjectclass) WidgetClass 
  xmShellExtObjectClass = (WidgetClass) (&xmShellExtClassRec);


/************************************************************************
 *
 *  ClassPartInitialize
 *    Set up the inheritance mechanism for the routines exported by
 *    vendorShells class part.
 *
 ************************************************************************/
static void 
ShellClassPartInitialize(
        WidgetClass w )
{
    XmShellExtObjectClass wc = (XmShellExtObjectClass) w;
    XmShellExtObjectClass sc =
      (XmShellExtObjectClass) wc->object_class.superclass;
    
    if (wc == (XmShellExtObjectClass)xmShellExtObjectClass)
      return;

    if (wc->shell_class.structureNotifyHandler == XmInheritEventHandler)
      wc->shell_class.structureNotifyHandler = 
	sc->shell_class.structureNotifyHandler;
}

/************************************************************************
 *
 *  StructureNotifyHandler
 *
 ************************************************************************/
/* ARGSUSED */
static void 
StructureNotifyHandler(
        Widget wid,
        XtPointer closure,
        XEvent *event,
        Boolean *continue_to_dispatch )
{
    register ShellWidget 	w = (ShellWidget) wid;
    WMShellWidget 		wmshell = (WMShellWidget) w;
    Boolean  			sizechanged = FALSE;
    Position 			tmpx, tmpy;
    XmShellExtObject		shellExt = (XmShellExtObject) closure;
    XmVendorShellExtObject	vendorExt = (XmVendorShellExtObject)closure;
    XmVendorShellExtPart	*vePPtr;
    XmScreen			xmScreen;

    /*
     *  for right now if this is being used by overrides bug out
     */
    if (!XmIsVendorShell(wid))
      return;
    else
      vePPtr = (XmVendorShellExtPart *) &(vendorExt->vendor);

    if (XmIsScreen(vendorExt->desktop.parent))
      xmScreen = (XmScreen) (vendorExt->desktop.parent);
    else
      xmScreen = (XmScreen) XmGetXmScreen(XtScreen(wid));

    switch(event->type) {
      case MapNotify:
	break;
      case UnmapNotify:
	/*
	 * try to keep the pop up field synced up so it won't disallow
	 * a new pop up request.
	 */
	/* 
	 * make sure we have good coords
	 */
	XtTranslateCoords((Widget) w, 0, 0, &tmpx, &tmpy);
	/*
	 * if the offsets match up, then offset our values so we'll go in
	 * the same place the next time.
	 */
	if ((vePPtr->xAtMap != w->core.x) ||
	    (vePPtr->yAtMap != w->core.y))
	  {
	      if (xmScreen->screen.mwmPresent)
		{
		    if (vePPtr->lastOffsetSerial &&
			(vePPtr->lastOffsetSerial >= 
			 vendorExt->shell.lastConfigureRequest) &&
			((vePPtr->xOffset + vePPtr->xAtMap) == w->core.x) &&
			((vePPtr->yOffset + vePPtr->yAtMap) == w->core.y))
		      {
			  w->core.x -= vePPtr->xOffset;
			  w->core.y -= vePPtr->yOffset;
			  w->shell.client_specified &= ~_XtShellPositionValid;

			  vePPtr->externalReposition = False;
		      }
		    else
		      {
			  vePPtr->externalReposition = True;
		      }
		}
	      else
		vePPtr->externalReposition = True;
	  }
	break;

      case ConfigureNotify:
	/*
	 * only process configureNotifies that aren't stale
	 */
	if (event->xany.serial <
	    shellExt->shell.lastConfigureRequest)
	  {
	      /*
	       *  make sure the hard wired event handler in shell is not called
	       */
	      if (shellExt->shell.useAsyncGeometry)
		*continue_to_dispatch = False;
	  }
	else
	  {
#define NEQ(x)	( w->core.x != event->xconfigure.x )
	      if( NEQ(width) || NEQ(height) || NEQ(border_width) ) {
		  sizechanged = TRUE;
	      }
#undef NEQ
	      w->core.width = event->xconfigure.width;
	      w->core.height = event->xconfigure.height;
	      w->core.border_width = event->xconfigure.border_width;
	      if (event->xany.send_event /* ICCCM compliant synthetic ev */
		  /* || w->shell.override_redirect */
		  || w->shell.client_specified & _XtShellNotReparented)
		{
		    w->core.x = event->xconfigure.x;
		    w->core.y = event->xconfigure.y;
		    w->shell.client_specified |= _XtShellPositionValid;
		}
	      else w->shell.client_specified &= ~_XtShellPositionValid;
	      if (XtIsWMShell(wid) && !wmshell->wm.wait_for_wm) {
		  /* Consider trusting the wm again */
		  register WMShellPart *wmp = &(wmshell->wm);
#define EQ(x) (wmp->size_hints.x == w->core.x)
		  if (EQ(x) && EQ(y) && EQ(width) && EQ(height)) {
		      wmshell->wm.wait_for_wm = TRUE;
		  }
#undef EQ
	      }
	  }		    
	break;
      case ReparentNotify:
	if (event->xreparent.window == XtWindow(w)) {
	    if (event->xreparent.parent != RootWindowOfScreen(XtScreen(w)))
	      {
		  w->shell.client_specified &= ~_XtShellNotReparented;
		  /*
		   * check to see if it's mwm
		   */
		  if (!(xmScreen->screen.numReparented++))
		    xmScreen->screen.mwmPresent = 
		      XmIsMotifWMRunning( (Widget) w);
	      }
	    else
	      {
		  w->core.x = event->xreparent.x;
		  w->core.y = event->xreparent.y;
		  w->shell.client_specified |= _XtShellNotReparented;
		  xmScreen->screen.numReparented--;
	      }
	    w->shell.client_specified &= ~_XtShellPositionValid;
	}
	return;
	
      default:
	return;
    }
    
    if (sizechanged) {
      XtWidgetProc resize;

      _XmProcessLock();
      resize = XtClass(w)->core_class.resize;
      _XmProcessUnlock();

      if (resize != (XtWidgetProc) NULL)
	(*resize)((Widget) w);
    }
}
