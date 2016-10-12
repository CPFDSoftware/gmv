/* $XConsortium: HelpP.h /main/4 1995/07/15 20:43:36 drk $ */
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

/* Ensure that the file be included only once. */
#ifndef _XmdHelpP_h
#define _XmdHelpP_h


/* Include appropriate files. */
#include "Help.h" 
#include <Xm/ManagerP.h>

/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  XtPointer		extension;
} XmdHelpClassPart;

/* Define the full class record. */
typedef struct _XmdHelpClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	XmdHelpClassPart	help_class;
} XmdHelpClassRec;

externalref XmdHelpClassRec xmdHelpClassRec;

/* Define the widget instance part. */
typedef struct
{
	char*			help_path;
	char*			help_file;
	XmRenderTable		rendertable;
	Dimension		spacing;
  	/* Internal instance fields */
 	Cardinal		num_titles;
	Cardinal		allocated_titles;
  	XmStringTable		titles;
	XmStringTable		help_text;
	/* Internal widgets (but in child list) */
	Widget			notebook;
	Widget			text_display;
	Widget			dismiss;
} XmdHelpPart;

/* Define the full instance record. */
typedef struct _XmdHelpRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmdHelpPart		help;
} XmdHelpRec;

/* Define the subclassing level index to be used with ResolvePartOffset */
#define XmdHelpIndex (XmManagerIndex + 1)

/* Define the constraint part structure. */
typedef struct _XmdHelpConstraintPart
{
  int		dummy;
} XmdHelpConstraintPart, * XmdHelpConstraint;

/* Define the full constraint structure. */
typedef struct _XmdHelpConstraintRec
{
	XmManagerConstraintPart	manager;
	XmdHelpConstraintPart	help;
} XmdHelpConstraintRec, * XmdHelpConstraintPtr;

/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _XmdHelpP_h */

