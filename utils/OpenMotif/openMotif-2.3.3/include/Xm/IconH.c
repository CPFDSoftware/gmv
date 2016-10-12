/* $XConsortium: IconH.c /main/5 1995/07/15 20:52:17 drk $ */
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
/*
 * IconH.c: The IconHeader widget methods.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <Xm/IconHP.h>

/********    Static Function Declarations    ********/

static	void			ClassPartInitialize(
					WidgetClass	wc);
static Widget GetContainerParent(Widget);

/********    End Static Function Declarations    ********/

static	XtResource		resources[] = 
	{
		{
		XmNcontainerID,XmCContainerID,XmRWidget,
		sizeof(Widget),
		XtOffset(XmIconHeader,iconh.container_ID),
			XmRImmediate,(XtPointer)NULL},

	};

/* That should not be necessary, but inheriting extension is
   not very well understood yet */

static XmGadgetClassExtRec GadClassExtRec = {
    NULL,
    NULLQUARK,
    XmGadgetClassExtVersion,
    sizeof(XmGadgetClassExtRec),
    XmInheritBaselineProc,                  /* widget_baseline */
    XmInheritDisplayRectProc,               /* widget_display_rect */
    XmInheritMarginsProc,                   /* widget_margins */
};


externaldef( xmiconheaderclassrec) XmIconHeaderClassRec	xmIconHeaderClassRec =
{	/* RectObjClassPart */
	{	
		(WidgetClass) &xmIconGadgetClassRec, /* superclass	*/
		"XmIconHeader",			/* class_name		*/
		sizeof (XmIconHeaderRec),	/* widget_size		*/
		NULL,				/* class_initialize	*/
		ClassPartInitialize,		/* class_part_initialize*/
		False,				/* class_inited		*/
		NULL,			        /* initialize		*/
		NULL,				/* initialize_hook	*/
		NULL,				/* realize		*/
		NULL,				/* actions		*/
		0,				/* num_actions		*/
		resources,			/* resources		*/
		XtNumber (resources),		/* num_resources	*/
		NULLQUARK,			/* xrm_class		*/
		True,				/* compress_motion	*/
		True,				/* compress_exposure	*/
		True,				/* compress_enterleave	*/
		False,				/* visible_interest	*/
		NULL,		 	        /* destroy		*/
		NULL,				/* resize		*/
		XtInheritExpose,	        /* expose		*/
		NULL,			        /* set_values		*/
		NULL,				/* set_values_hook	*/
		XtInheritSetValuesAlmost,	/* set_values_almost	*/
		NULL,				/* get_values_hook	*/
		NULL,				/* accept_focus		*/
		XtVersion,			/* version		*/
		NULL,				/* callback private	*/
		NULL,				/* tm_table		*/
		XtInheritQueryGeometry,		/* query_geometry	*/
		NULL,				/* display_accelerator	*/
		NULL,				/* extension		*/
	},

	/* XmGadget Class Part */
	{
	XmInheritBorderHighlight,		/* border_highlight	*/
	XmInheritBorderUnhighlight,		/* border_unhighlight	*/
	NULL,					/* arm_and_activate	*/
	XmInheritInputDispatch,			/* input_dispatch	*/
	XmInheritVisualChange,			/* visual_change	*/
	NULL,				        /* get_resources	*/
	0,		                        /* num_get_resources	*/
	NULL,					/* class_cache_part	*/
	(XtPointer)&GadClassExtRec,             /* extension		*/
	},
	/* XmIconGadget Class Part */
	{
	    GetContainerParent,		        /* get_container_parent	*/
	    NULL,		                /* extension	*/
        },
	/* XmIconHeader Class Part */
	{
	    NULL,		                /* extension	*/
        },
};

externaldef(xmiconheaderclass) WidgetClass
	xmIconHeaderClass=(WidgetClass)&xmIconHeaderClassRec;




/*----------------
| RectObj methods |
----------------*/

/************************************************************************
 * ClassPartInitialize
 *      Parms(IconGadgetClass)
 *              returns void
 *
 *      Set Motif Fast subclass initialize bit.
 ************************************************************************/
static void
ClassPartInitialize(
	WidgetClass	wc)
{
    _XmFastSubclassInit(wc,XmICONHEADER_BIT);

}


/************************************************************************
 * GetContainerParent class method
 *
 ************************************************************************/
static Widget
GetContainerParent(
	Widget		wid)
{
    return (((XmIconHeader)(wid))->iconh.container_ID);
}


/*-------------------
| External functions |
-------------------*/
/************************************************************************
 * XmCreateIconHeader
 * 
 * Create an instance of a xmIconHeaderClass widget and
 * return it's id.
 ************************************************************************/
Widget
XmCreateIconHeader(
	Widget		parent,
	char		*name,
	ArgList		arglist,
	Cardinal	argcount)
{
	return(XtCreateWidget(name,xmIconHeaderClass,parent,arglist,argcount));
}
