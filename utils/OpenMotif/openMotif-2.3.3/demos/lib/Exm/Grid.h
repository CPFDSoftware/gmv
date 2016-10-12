/* $XConsortium: Grid.h /main/5 1995/07/15 20:40:49 drk $ */
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
 * Grid.h: The widget public header file for the ExmGrid demonstration
 *         widget.
 *
 ******************************************************************************/


/* Ensure that the file be included only once. */
#ifndef _ExmGrid_h
#define _ExmGrid_h


/* Include appropriate files. */
#include <Xm/Xm.h> /* widget public header file for XmManager */


/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif


/* Define the widget class and widget record. */
externalref WidgetClass exmGridWidgetClass;

typedef struct _ExmGridClassRec * ExmGridWidgetClass;
typedef struct _ExmGridRec      * ExmGridWidget;


/* Define an IsSubclass macro. */
#ifndef ExmIsGrid
#define ExmIsGrid(w) XtIsSubclass(w, exmGridWidgetClass)
#endif


/* Define string equivalents of new resource names. */
#define ExmNgridMarginWidthWithinCell "gridMarginWidthWithinCell"
#define ExmNgridMarginHeightWithinCell "gridMarginHeightWithinCell"

#define ExmCGridMarginWidthWithinCell  "GridMarginWidthWithinCell"
#define ExmCGridMarginHeightWithinCell "GridMarginHeightWithinCell"


/* Specify the API for this widget. */
extern Widget ExmCreateGrid(
			Widget parent,
			char   *name,
			ArgList arglist,
			Cardinal argcount);
extern Widget ExmCreateGridDialog(
			Widget parent,
			char   *name,
			ArgList arglist,
			Cardinal argcount);

/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _ExmGrid_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */

