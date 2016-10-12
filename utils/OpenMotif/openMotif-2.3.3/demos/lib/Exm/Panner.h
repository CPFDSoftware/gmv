/* $XConsortium: Panner.h /main/5 1995/07/15 20:41:15 drk $ */
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
/* Ensure that this file be included only once. */
#ifndef _ExmPanner_h
#define _ExmPanner_h

/* Include appropriate header file(s). */
#include <Exm/Simple.h>  /* public header file for the ExmSimple widget */


/* Allow for compilation by C++. */
#ifdef __cplusplus
extern "C" {
#endif

#define ExmNcanvasWidth "canvasWidth"
#define ExmCCanvasWidth "CanvasWidth"
#define ExmNcanvasHeight "canvasHeight"
#define ExmCCanvasHeight "CanvasHeight"
#define ExmNrubberBand "rubberBand"
#define ExmCRubberBand "RubberBand"
#define ExmNsliderX "sliderX"
#define ExmCSliderX "SliderX"
#define ExmNsliderY "sliderY"
#define ExmCSliderY "SliderY"
#define ExmNsliderWidth "sliderWidth"
#define ExmCSliderWidth "SliderWidth"
#define ExmNsliderHeight "sliderHeight"
#define ExmCSliderHeight "SliderHeight"
#define ExmNreportCallback "reportCallback"
#define ExmCReportCallback "reportCallback"


/*
 * ExmPannerReport - this structure is used by the reportCallback of the
 * Panner, Porthole, Viewport, and Scrollbar widgets to report its position.
 * All fields must be filled in, although the changed field may be used as
 * a hint as to which fields have been altered since the last report.
 */
typedef struct {
    int     reason;
    XEvent  *event;
    Mask changed;		/* mask, see below */
    Position slider_x, slider_y;	/* location of slider within outer */
    Dimension slider_width, slider_height;  /* size of slider */
    Dimension canvas_width, canvas_height;  /* size of canvas */
} ExmPannerCallbackStruct;

#define ExmPRSliderX		(1 << 0)
#define ExmPRSliderY		(1 << 1)
#define ExmPRSliderWidth	(1 << 2)
#define ExmPRSliderHeight	(1 << 3)
#define ExmPRCanvasWidth	(1 << 4)
#define ExmPRCanvasHeight	(1 << 5)

/* external declarations */
externalref WidgetClass exmPannerWidgetClass;

typedef struct _ExmPannerClassRec *ExmPannerWidgetClass;
typedef struct _ExmPannerRec      *ExmPannerWidget;


/* Define an IsSubclass macro. */
#ifndef ExmIsPanner
#define ExmIsPanner(w)	XtIsSubclass(w, exmPannerWidgetClass)
#endif 

/********    Public Function Declarations    ********/

extern Widget ExmCreatePanner( 
                        Widget parent,
                        char *name,
                        ArgList arglist,
                        Cardinal argcount) ;
extern void ExmPannerSetValues( 
                        Widget w,
                        int hvalue,
                        int vvalue,
#if NeedWidePrototypes
                        int notify) ;
#else
                        Boolean notify) ;
#endif /* NeedWidePrototypes */

/********    End Public Function Declarations    ********/

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


#endif /* _ExmPanner_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
