/* $XConsortium: PrintP.h /main/4 1995/07/15 20:43:58 drk $ */
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
#ifndef _XmdPrintP_h
#define _XmdPrintP_h

/* Include appropriate files. */
#include "Print.h" 
#include <Xm/FormP.h>

/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  XtPointer		extension;
} XmdPrintClassPart;

/* Define the full class record. */
typedef struct _XmdPrintClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	XmBulletinBoardClassPart  bulletin_board_class;
	XmFormClassPart     	form_class;
	XmdPrintClassPart	print_class;
} XmdPrintClassRec;

externalref XmdPrintClassRec xmdPrintClassRec;

/* Define the widget instance part. */
typedef struct
{
	char*			printer_list;
	Boolean			two_sided;
	Boolean			reverse_order;
	Boolean			print_all;
	int			from_page;
	int			to_page;
	int			number_of_copies;
	XtCallbackList		print_callback;
  	/* Internal instance fields */
 	Cardinal		num_printers;
	char**			printers;
	Boolean*		is_printer;
	Boolean			adding_printer;
	/* Internal widgets which require access */
	Widget			copies_tf;
	Widget			two_sided_toggle;
	Widget			reverse_order_toggle;
	Widget			from_all_rc;
	Widget			from_tf;
	Widget			to_tf;
	Widget			from_toggle;
	Widget			all_toggle;
	Widget			destination_om;
	Widget			destination_pd;
	Widget			new_printer_dialog;
	Cardinal		num_om_items;
	WidgetList		om_items;
} XmdPrintPart;

/* Define the full instance record. */
typedef struct _XmdPrintRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmBulletinBoardPart 	bulletin_board;
	XmFormPart     		form;
	XmdPrintPart		print;
} XmdPrintRec;

/* Define the constraint part structure. */
typedef struct _XmdPrintConstraintPart
{
  int		dummy;
} XmdPrintConstraintPart, * XmdPrintConstraint;

/* Define the full constraint structure. */
typedef struct _XmdPrintConstraintRec
{
	XmManagerConstraintPart	manager;
	/* No bulletin board constraint part */
	XmFormConstraintPart    form;
	XmdPrintConstraintPart	print;
} XmdPrintConstraintRec, * XmdPrintConstraintPtr;

/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _XmdPrintP_h */

