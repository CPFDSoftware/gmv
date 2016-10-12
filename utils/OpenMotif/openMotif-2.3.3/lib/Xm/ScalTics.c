/* $XConsortium: ScalTics.c /main/7 1995/12/06 21:39:58 cde-sun $ */
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



#include <Xm/Xm.h>
#include <Xm/XmosP.h>  /* for allocate local */
#include <Xm/Scale.h>
#include <Xm/SeparatoG.h>
#include "XmI.h"

/************************************************************************
 *
 *  XmScaleSetTicks
 *
 ************************************************************************/
void
XmScaleSetTicks(
        Widget scale,
        int big_every,
        Cardinal num_med,
        Cardinal num_small, 
	Dimension  size_big,
	Dimension  size_med,
	Dimension  size_small)
{
    Widget *sep ;
    Cardinal n, i, j, k, sep_num;
    int real_num_big, real_num_med, real_num_small;
    Arg args[5];
    int max, min ;
    unsigned char orient ;
    char * dim_res ;

    _XmWidgetToAppContext(scale);

    _XmAppLock(app);

    /* Some checking first */
    if (size_big == 0) { _XmAppUnlock(app); return ; }
    if (size_med == 0) num_med = 0 ;
    if (size_small == 0) num_small = 0 ;

    /* big_every is the number of values between big tics, while
       num_med and num_small are the number of tics between resp.
       big and med values */

    /* compute num_big first */
    n = 0 ;
    XtSetArg(args[n], XmNmaximum, &max); n++;
    XtSetArg(args[n], XmNminimum, &min); n++;
    XtSetArg(args[n], XmNorientation, &orient); n++;
    XtGetValues(scale, args, n);
    
    real_num_big = ((max - min) / big_every) + 1 ;
    if (real_num_big < 2) { _XmAppUnlock(app); return ; }

    real_num_med = (real_num_big - 1) * num_med;
    real_num_small = (real_num_big + real_num_med - 1) * num_small;
    sep_num = real_num_big + real_num_med + real_num_small;

    sep = (Widget*) ALLOCATE_LOCAL(sep_num * sizeof(Widget));

    if (orient == XmHORIZONTAL) {
	dim_res = XmNheight;
	orient = XmVERTICAL;
    } else {
	dim_res = XmNwidth ;
	orient = XmHORIZONTAL;
    }
    XtSetArg(args[0], XmNmargin, 0); 
    XtSetArg(args[1], XmNorientation, orient); 

    sep_num = 0 ;
    for (i=0; i < real_num_big; i++) {
	n = 2 ;
	XtSetArg(args[n], dim_res, size_big); n++ ;
	sep[sep_num] = XmCreateSeparatorGadget(scale, "BigTic", args, n); 
	sep_num++;

	if (i == real_num_big - 1) break ;

	for (k=0; k < num_small; k++) {
	    n = 2 ;
	    XtSetArg(args[n], dim_res, size_small); n++;
	    XtSetArg(args[n], XmNseparatorType, XmSINGLE_LINE); n++;
	    sep[sep_num] = XmCreateSeparatorGadget(scale, "SmallTic", 
						   args, n); 
	    sep_num++;
	}

	for (j=0; j < num_med; j++) {
	    n = 2 ;
	    XtSetArg(args[n], dim_res, size_med); n++;
	    sep[sep_num] = XmCreateSeparatorGadget(scale, "MedTic", args, n); 
	    sep_num++;
	    for (k=0; k < num_small; k++) {
		n = 2 ;
		XtSetArg(args[n], dim_res, size_small); n++;
		XtSetArg(args[n], XmNseparatorType, XmSINGLE_LINE); n++;
		sep[sep_num] = XmCreateSeparatorGadget(scale, "SmallTic", 
						       args, n); 
		sep_num++;
	    }
	    
	}
    }

    XtManageChildren(sep, sep_num);			
    DEALLOCATE_LOCAL((char*)sep);

    _XmAppUnlock(app);
}


