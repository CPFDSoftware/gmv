/* $XConsortium: GridP.h /main/5 1995/07/15 20:40:53 drk $ */
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

/*******************************************************************************
 *
 * GridP.h: The widget private header file for the ExmGrid widget.
 *
 ******************************************************************************/

/* Ensure that the file be included only once. */
#ifndef _ExmGridP_h
#define _ExmGridP_h


/* Include appropriate files. */
#include <Exm/Grid.h>  /* public header file for ExmGrid */ 
#include <Xm/ManagerP.h> /* private header file for XmManager */


/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif


/* Make the following two methods inheritable by subclasses of ExmGrid. */ 
#define ExmInheritLayout   ((ExmLayoutProc) _XtInherit)
#define ExmInheritCalcSize ((ExmCalcSizeProc) _XtInherit)
#define ExmInheritNeedRelayout ((ExmNeedRelayoutProc) _XtInherit)


/* Define new data types for these two inheritable methods. */
typedef void (*ExmLayoutProc)(
			Widget,
			Widget);
typedef void (*ExmCalcSizeProc)(
                        Widget,
                        Widget,
                        Dimension *,
                        Dimension *);
typedef Boolean (*ExmNeedRelayoutProc)(
                        Widget,
                        Widget);

/* Define the widget class part. */
typedef struct
{
	ExmLayoutProc           layout;
        ExmCalcSizeProc         calc_size;
        ExmNeedRelayoutProc     need_relayout;
	XtPointer		extension;
} ExmGridClassPart;


/* Define the full class record. */
typedef struct _ExmGridClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	ExmGridClassPart	grid_class;
} ExmGridClassRec;

externalref ExmGridClassRec exmGridClassRec;


/* Define the widget instance part. */
typedef struct
{
 /* Provide space for the values of the four resources of ExmGrid. */
	short			rows;
	short			columns;
	Dimension		margin_width;
	Dimension		margin_height;
	XtCallbackList	        map_callback;
	XtCallbackList	        unmap_callback;
	Boolean		        default_position;
	XmRenderTable           button_render_table ;
	XmRenderTable           label_render_table ;
	XmRenderTable           text_render_table ;
        XmString	        dialog_title;

 /* processing_constraints is a flag.  If its value is True, then 
    it means that the ConstraintSetValues method is requesting a 
    geometry change. */
	Boolean			processing_constraints;

} ExmGridPart;

/* Establish an arbitrary limit */
#define EXM_GRID_MAX_NUMBER_OF_ROWS 100
#define EXM_GRID_MAX_NUMBER_OF_COLUMNS 100

/* Define the full instance record. */
typedef struct _ExmGridRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	ExmGridPart		grid;
} ExmGridRec;

/* Define the subclassing level index to be used with ResolvePartOffset */
#define ExmGridIndex (XmManagerIndex + 1)

/* Define the constraint part structure. */
typedef struct _ExmGridConstraintPart
{
	Dimension		grid_margin_width_within_cell;
	Dimension		grid_margin_height_within_cell;
} ExmGridConstraintPart, * ExmGridConstraint;


/* Define the full constraint structure. */
typedef struct _ExmGridConstraintRec
{
	XmManagerConstraintPart	manager;
	ExmGridConstraintPart	grid;
} ExmGridConstraintRec, * ExmGridConstraintPtr;


/* Define macros for this class. */
#define ExmGridCPart(w) \
        (&((ExmGridConstraintPtr) (w)->core.constraints)->grid)


/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _ExmGridP_h */
