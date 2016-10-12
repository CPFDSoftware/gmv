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
static char rcsid[] = "$XConsortium: Desktop.c /main/12 1995/07/14 10:17:30 drk $"
#endif
#endif
/*
*  (c) Copyright 1989, 1990  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
/*
*  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY  */
/*
*  (c) Copyright 1988 MICROSOFT CORPORATION */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <Xm/DesktopP.h>
#include <Xm/BaseClassP.h>
#include <Xm/ScreenP.h>
#include <Xm/DisplayP.h>


/********    Static Function Declarations    ********/

static void ClassPartInitialize( 
                        WidgetClass widgetClass) ;
static void ResParentDestroyed( 
                        Widget resParent,
                        XtPointer closure,
                        XtPointer callData) ;
static void Destroy( 
                        Widget wid) ;
static void InsertChild( 
                        Widget wid) ;
static void DeleteChild( 
                        Widget wid) ;
static void Initialize( 
                        Widget requested_widget,
                        Widget new_widget,
                        ArgList args,
                        Cardinal *num_args) ;

/********    End Static Function Declarations    ********/



static XtResource resources[] =
{
    {
	XmNdesktopParent,
	XmCDesktopParent, XmRWidget, sizeof (Widget),
	XtOffsetOf( struct _XmDesktopRec, desktop.parent),
	XmRImmediate, (XtPointer)NULL,
    },
    {
	XmNextensionType,
	XmCExtensionType, XmRExtensionType, sizeof (unsigned char),
	XtOffsetOf( struct _XmDesktopRec, ext.extensionType),
	XmRImmediate, (XtPointer)XmDESKTOP_EXTENSION,
    },
};


externaldef(xmdesktopclassrec) XmDesktopClassRec xmDesktopClassRec = {
    {	
	(WidgetClass) &xmExtClassRec,	/* superclass	*/   
	"Desktop",			/* class_name 		*/   
	sizeof(XmDesktopRec), 		/* size 		*/   
	NULL,				/* Class Initializer 	*/   
	ClassPartInitialize,	        /* class_part_init 	*/ 
	FALSE, 				/* Class init'ed ? 	*/   
	Initialize,		        /* initialize         	*/   
	NULL, 				/* initialize_notify    */ 
	NULL,	 			/* realize            	*/   
	NULL,	 			/* actions            	*/   
	0,				/* num_actions        	*/   
	resources,		        /* resources          	*/   
	XtNumber(resources),	/* resource_count     	*/   
	NULLQUARK, 			/* xrm_class          	*/   
	FALSE, 				/* compress_motion    	*/   
	FALSE, 				/* compress_exposure  	*/   
	FALSE, 				/* compress_enterleave	*/   
	FALSE, 				/* visible_interest   	*/   
	Destroy,			/* destroy            	*/   
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
    {					/* ext			*/
	NULL,				/* synthetic resources	*/
	0,				/* num syn resources	*/
	NULL,				/* extension		*/
    },
    {					/* desktop		*/
	NULL,				/* child_class		*/
	InsertChild,		        /* insert_child		*/
	DeleteChild,		        /* delete_child		*/
	NULL,				/* extension		*/
    },
};

externaldef(xmdesktopclass) WidgetClass 
      xmDesktopClass = (WidgetClass) &xmDesktopClassRec;
    

    
static void 
ClassPartInitialize(
        WidgetClass widgetClass )
{
    register XmDesktopClassPartPtr wcPtr;
    register XmDesktopClassPartPtr superPtr;
    
    wcPtr = (XmDesktopClassPartPtr)
      &(((XmDesktopObjectClass)widgetClass)->desktop_class);
    
    if (widgetClass != xmDesktopClass)
      /* don't compute possible bogus pointer */
      superPtr = (XmDesktopClassPartPtr)&(((XmDesktopObjectClass)widgetClass
				->core_class.superclass)->desktop_class);
    else
      superPtr = NULL;
    
    /* We don't need to check for null super since we'll get to xmDesktop
       eventually, and it had better define them!  */
    
    if (wcPtr->child_class == XmInheritClass) {
	wcPtr->child_class = 
	  superPtr->child_class;
    }
    if (wcPtr->insert_child == XtInheritInsertChild) {
	wcPtr->insert_child = superPtr->insert_child;
    }
    
    if (wcPtr->delete_child == XtInheritDeleteChild) {
	wcPtr->delete_child = superPtr->delete_child;
    }
    
}

/*ARGSUSED*/
static void 
ResParentDestroyed(
        Widget resParent,
        XtPointer closure,
        XtPointer callData )
{
    XmExtObject	me = (XmExtObject) closure ;
    if (!me->object.being_destroyed) {
      XtDestroyWidget((Widget) me);
    }
}

static void 
Destroy(
        Widget wid )
{
    XmDesktopObject w = (XmDesktopObject) wid ;
    XmDesktopObject		deskObj = (XmDesktopObject)w;
    Widget			deskParent;
    Widget			resParent = w->ext.logicalParent;
    
    if ((deskParent = deskObj->desktop.parent) != NULL) {
	if (XmIsScreen(deskParent)) {
	      XmScreenClass	deskParentClass = (XmScreenClass)
		XtClass(deskParent);
	      (*(deskParentClass->desktop_class.delete_child)) 
		((Widget) deskObj);
	  }
	  else {
	      XmDesktopObjectClass deskParentClass = (XmDesktopObjectClass)
		XtClass(deskParent);
	      (*(deskParentClass->desktop_class.delete_child)) 
		((Widget) deskObj);
	  }
    }
    
    /*
     * if we were created as a sibling of our primary then we have a
     * destroy callback on them.
     */
    if (resParent && !resParent->core.being_destroyed)
	XtRemoveCallback((Widget) resParent, 
			 XmNdestroyCallback,
			 ResParentDestroyed,
			 (XtPointer)w);

    XtFree((char *) w->desktop.children);
}

static void 
InsertChild(
        Widget wid )
{
    XmDesktopObject w = (XmDesktopObject) wid ;
    register Cardinal	     	position;
    register Cardinal        	i;
    register XmDesktopObject 	cw;
    register WidgetList      	children;
    
    cw = (XmDesktopObject) w->desktop.parent;
    children = cw->desktop.children;
    
    position = cw->desktop.num_children;
    
    if (cw->desktop.num_children == cw->desktop.num_slots) {
	/* Allocate more space */
	cw->desktop.num_slots +=  (cw->desktop.num_slots / 2) + 2;
	cw->desktop.children = children = 
	  (WidgetList) XtRealloc((char *) children,
		(unsigned) (cw->desktop.num_slots) * sizeof(Widget));
    }
    /* Ripple children up one space from "position" */
    for (i = cw->desktop.num_children; i > position; i--) {
	children[i] = children[i-1];
    }
    children[position] = (Widget)w;
    cw->desktop.num_children++;
}

static void 
DeleteChild(
        Widget wid )
{
    XmDesktopObject w = (XmDesktopObject) wid ;
    register Cardinal	     	position;
    register Cardinal	     	i;
    register XmDesktopObject 	cw;
    
    cw = (XmDesktopObject) w->desktop.parent;
    
    for (position = 0; position < cw->desktop.num_children; position++) {
        if (cw->desktop.children[position] == (Widget)w) {
	    break;
	}
    }
    if (position == cw->desktop.num_children) return;
    
    /* Ripple children down one space from "position" */
    cw->desktop.num_children--;
    for (i = position; i < cw->desktop.num_children; i++) {
        cw->desktop.children[i] = cw->desktop.children[i+1];
    }
}

/************************************************************************
 *
 *  DesktopInitialize
 *
 ************************************************************************/
/* ARGSUSED */
static void 
Initialize(
        Widget requested_widget,
        Widget new_widget,
        ArgList args,
        Cardinal *num_args )
{
    XmDesktopObject		deskObj = (XmDesktopObject)new_widget;
    Widget			deskParent;

    deskObj->desktop.num_children = 0;
    deskObj->desktop.children = NULL;
    deskObj->desktop.num_slots = 0;
    
    if ((deskParent = deskObj->desktop.parent) != NULL) {
	  if (XmIsScreen(deskParent)) {
	      XmScreenClass	deskParentClass = (XmScreenClass)
		XtClass(deskParent);
	      (*(deskParentClass->desktop_class.insert_child)) 
		((Widget) deskObj);
	  }
	  else {
	      XmDesktopObjectClass deskParentClass = (XmDesktopObjectClass)
		XtClass(deskParent);
	      (*(deskParentClass->desktop_class.insert_child)) 
		((Widget) deskObj);
	  }
    }
}

