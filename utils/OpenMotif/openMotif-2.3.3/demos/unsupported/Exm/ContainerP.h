/* $XConsortium: ContainerP.h /main/4 1995/07/15 20:47:12 drk $ */
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

/****************************************************************************
 *
 * ContainerP.h: The widget private header file for the ExmContainer widget.
 *
 ****************************************************************************/

/* Ensure that the file be included only once. */
#ifndef _ExmContainerP_h
#define _ExmContainerP_h


/* Include appropriate files. */
#include <Exm/Container.h>  /* public header file for ExmContainer */ 
#include <Exm/GridP.h>      /* private header file for Grid */


/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif


/* Define the widget class part. */
typedef struct
{
	XtPointer		extension;
} ExmContainerClassPart;


/* Define the full class record. */
typedef struct _ExmContainerClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	ExmGridClassPart	grid_class;
	ExmContainerClassPart	container_class;
} ExmContainerClassRec;

externalref ExmContainerClassRec exmContainerClassRec;


/* Define the widget instance part. */
typedef struct
{
 /* Provide space for the values of the resources of ExmContainer. */
	Dimension		first_column_width;
	Cardinal *		detail_order;
	Cardinal 		detail_order_count;
	XmTabList               detail_tablist ;
	XmStringTable           detail_header ;
	Cardinal                detail_header_count;
	Pixel                   select_color;
	unsigned char           layout_type;

	Widget                  icon_header;

} ExmContainerPart;


/* Define the full instance record. */
typedef struct _ExmContainerRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	ExmGridPart	        grid;
	ExmContainerPart	container;
} ExmContainerRec;


/* Define the constraint part structure. */
typedef struct _ExmContainerConstraintPart
{
	char unused ;
} ExmContainerConstraintPart, * ExmContainerConstraint;


/* Define the full constraint structure. */
typedef struct _ExmContainerConstraintRec
{
	XmManagerConstraintPart	        manager;
	ExmGridConstraintPart	        grid;
	ExmContainerConstraintPart	container;
} ExmContainerConstraintRec, * ExmContainerConstraintPtr;


/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _ExmContainerP_h */
