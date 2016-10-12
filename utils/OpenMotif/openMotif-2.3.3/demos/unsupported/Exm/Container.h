/* $XConsortium: Container.h /main/4 1995/07/15 20:47:08 drk $ */
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
/*****************************************************************************
 *
 * Container.h: The widget public header file.
 *
 ****************************************************************************/


/* Ensure that the file be included only once. */
#ifndef _ExmContainer_h
#define _ExmContainer_h


/* Include appropriate files. */
#include <Xm/Xm.h> /* widget public header file for XmManager */


/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif


/* Define the widget class and widget record. */
externalref WidgetClass exmContainerWidgetClass;

typedef struct _ExmContainerClassRec * ExmContainerWidgetClass;
typedef struct _ExmContainerRec      * ExmContainerWidget;


/* Define an IsSubclass macro. */
#ifndef ExmIsContainer
#define ExmIsContainer(w) XtIsSubclass(w, exmContainerWidgetClass)
#endif


/* Define string equivalents of new resource names. */
#define ExmNfirstColumnWidth "firstColumnWidth"
#define ExmCFirstColumnWidth "FirstColumnWidth"

#define ExmNdetailOrder "detailOrder"
#define ExmCDetailOrder "DetailOrder"

#define ExmNdetailOrderCount "detailOrderCount"
#define ExmCDetailOrderCount "DetailOrderCount"

#define ExmNdetailTabList "detailTabList"
#define ExmCDetailTabList "DetailTabList"

#define ExmNdetailHeader "detailHeader"
#define ExmCDetailHeader "DetailHeader"

#define ExmNdetailHeaderCount "detailHeaderCount"
#define ExmCDetailHeaderCount "DetailHeaderCount"

#define ExmNselectColor "selectColor"
#define ExmCSelectColor "SelectColor"

#define ExmNlayoutType "layoutType"
#define ExmCLayoutType "LayoutType"


/* Specify the API for this widget. */
extern Widget ExmCreateContainer();


/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


/* Ensure that the file be included only once. */
#endif /* _ExmContainer_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */

