/* $XConsortium: SimpleP.h /main/5 1995/07/15 20:41:32 drk $ */
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
 * SimpleP.h: The widget private header file for the ExmSimple demonstration
 *            widget. 
 *
 ******************************************************************************/


/* Ensure that the file be included only once. */
#ifndef _ExmSimpleP_h
#define _ExmSimpleP_h

/* Include appropriate files. */
#include <Exm/Simple.h> /* public header file for the ExmSimple widget */
#include <Xm/PrimitiveP.h> /* private header file for the XmPrimitive widget */

/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif 

/* Make the following seven methods inheritable by subclasses of ExmSimple. */ 
#define ExmInheritDrawVisual	 ((XtWidgetProc) _XtInherit)
#define ExmInheritDrawShadow	 ((XtWidgetProc) _XtInherit)
#define ExmInheritCreateGC	 ((XtWidgetProc) _XtInherit)
#define ExmInheritDestroyGC	 ((XtWidgetProc) _XtInherit)
#define ExmInheritSelectGC	 ((ExmSelectGCProc) _XtInherit)
#define ExmInheritCalcVisualSize ((XtWidgetProc) _XtInherit)
#define ExmInheritCalcWidgetSize ((XtWidgetProc) _XtInherit)
#define ExmInheritReconfigure    ((ExmReconfigureProc) _XtInherit)

/* Provide typedefs for inheritable methods that do not already
   have an appropriate data type. */
typedef GC   (*ExmSelectGCProc)(
                        Widget);
typedef void (*ExmReconfigureProc)(
			WidgetClass,
			Widget,
			Widget) ;

/* Define the widget class part. */
typedef struct _ExmSimpleClassPart
{
	XtWidgetProc		draw_visual;
	XtWidgetProc		draw_shadow;
	XtWidgetProc		create_gc;
	XtWidgetProc		destroy_gc;
	ExmSelectGCProc		select_gc;
	XtWidgetProc		calc_visual_size;
	XtWidgetProc		calc_widget_size;
        ExmReconfigureProc      reconfigure; 
	XtPointer		extension;
} ExmSimpleClassPart;

/* Define the full class record. */
typedef struct _ExmSimpleClassRec
{
	CoreClassPart		core_class;
	XmPrimitiveClassPart	primitive_class;
	ExmSimpleClassPart	simple_class;
} ExmSimpleClassRec;

/* Define the subclassing level index to be used with ResolvePartOffset */
#define ExmSimpleIndex (XmPrimitiveIndex + 1)

/* Make the full class record externally accessible. */
externalref ExmSimpleClassRec exmSimpleClassRec;

/* Define the widget instance part. */
typedef struct _ExmSimplePart
{
 /* Provide space for the three resources of ExmSimple. */
	unsigned char		simple_shape;
	Dimension		margin_height;
        Dimension		margin_width;

 /* Provide space for the other protected fields of ExmSimple. */
 /* normal_gc holds a Graphics Context for rendering the visual
    when the value of XmNsensitive is True. */
	GC			normal_gc;

 /* insensitive_gc holds a Graphics Context for rendering the visual
    when the value of XmNsensitive is False. */
	GC			insensitive_gc;

 /* pref_width holds an integral value representing the widget's 
    current preferred width. */
	Dimension		pref_width;

 /* pref_width holds an integral value representing the widget's 
    current preferred height. */
	Dimension		pref_height;

 /* need_to_compute_width is a flag.  If its value is True, then the widget
    needs to renegotiate its width. */
	Boolean			need_to_compute_width;

 /* need_to_compute_height is a flag.  If its value is True, then the widget
    needs to renegotiate its height. */
	Boolean			need_to_compute_height;

 /* visual is an XRectangle value representing the bounding box of the
    widget's visual. */ 
	XRectangle              visual;

 /* need_to_reconfigure is a flag.  If its value is True, then the widget
    needs to call the reconfigure method. */ 
	Boolean			need_to_reconfigure;

 /* saved_foreground is used for the management of the selected state */ 
	Pixel			saved_foreground;
} ExmSimplePart;


/* Define the full instance record. */
typedef struct _ExmSimpleRec
{
	CorePart		core;
	XmPrimitivePart		primitive;
	ExmSimplePart		simple;
} ExmSimpleRec;

/* Allow for C++ compilation. */ 
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

/* Ensure that the file be included only once. */
#endif /* _ExmSimpleP_h */
/* Don't add anything after this #endif */
