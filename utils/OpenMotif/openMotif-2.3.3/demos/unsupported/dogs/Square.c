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
#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$TOG: Square.c /main/6 1998/03/25 18:18:13 csn $"
#endif
#endif

/*****************************************************************************
*
*  Square.c - widget source file
*
******************************************************************************/
	
#include <stdio.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <Xm/BulletinBP.h>
#include "SquareP.h"

#define Width(w) XmField((w),square_offsets,Core,width,Dimension)
#define Height(w) XmField((w),square_offsets,Core,height,Dimension)
#define BorderWidth(w) XmField((w),square_offsets,Core,border_width,Dimension)
#define MajorDimension(w) XmField((w),square_offsets,Square,major_dimension,int)

#define MakeSquare(w) XmConstraintField((w),square_constraint_offsets, \
			Square,make_square, Boolean)

static void ClassInitialize();
static void Initialize();
static Boolean SetValues();
static void ConstraintInitialize();
static Boolean ConstraintSetValues();

static XmPartResource resources[] = {
    {
    SquareNmajorDimension, SquareCMajorDimension, XmRInt, sizeof(int),
    XmPartOffset(Square,major_dimension), XmRImmediate, (caddr_t)SquareWIDTH
    }
};

static XmPartResource constraints[] = {
   {
    SquareNmakeSquare, SquareCMakeSquare, XmRBoolean, sizeof(Boolean),
    XmConstraintPartOffset(Square,make_square),
    XmRImmediate, (caddr_t)False
   }
};


externaldef(squareclassrec) SquareClassRec squareClassRec =
{
   {                    /* core_class fields    */
      (WidgetClass) &xmBulletinBoardClassRec,   /* superclass   */
      "Square",                 /* class_name           */
      sizeof(SquarePart),       /* widget_size          */
      ClassInitialize,          /* class_initialize     */
      NULL,                     /* class init part proc */
      False,                    /* class_inited         */
      Initialize,               /* initialize           */
      NULL,                     /* initialize_notify    */
      XtInheritRealize,         /* realize              */
      NULL,                     /* actions              */
      0,                        /* num_actions          */
      (XtResourceList)resources,/* resources            */
      XtNumber(resources),      /* num_resources        */
      NULLQUARK,                /* xrm_class            */
      False,                    /* compress_motion      */
      XtExposeCompressMaximal,  /* compress_exposure    */
      False,                    /* compress_enterleave  */
      False,                    /* visible_interest     */
      NULL,                     /* destroy              */
      XtInheritResize,          /* resize               */
      XtInheritExpose,          /* expose               */
      SetValues,                /* set_values           */
      NULL,                     /* set_values_hook      */
      XtInheritSetValuesAlmost, /* set_values_almost    */
      NULL,                     /* get_values_hook      */
      NULL,                     /* accept_focus         */
      XtVersionDontCheck,       /* version              */
      NULL,                     /* callback_private     */
      XtInheritTranslations,    /* tm_table             */
      XtInheritQueryGeometry,   /* Query Geometry proc  */
      NULL,                     /* disp accelerator     */
      NULL,                     /* extension            */    
   },

   {                    /* composite_class fields */
      XtInheritGeometryManager, /* geometry_manager       */
      XtInheritChangeManaged,   /* change_managed         */
      XtInheritInsertChild,     /* insert_child           */
      XtInheritDeleteChild,     /* delete_child           */
      NULL,                     /* extension              */
   },

   {                    /* constraint_class fields */
      (XtResourceList)constraints,  /* constraint resource     */
      XtNumber(constraints),        /* number of constraints   */
      sizeof(SquareConstraintRec), /* size of constraint      */
      ConstraintInitialize,         /* initialization          */
      NULL,                         /* destroy proc            */
      ConstraintSetValues,          /* set_values proc         */
      NULL,                         /* extension               */
   },

   {                    /* manager_class fields   */
      XtInheritTranslations,        /* translations           */
      NULL,                         /* syn_resources          */
      0,                            /* num_syn_resources      */
      NULL,                         /* syn_cont_resources     */
      0,                            /* num_syn_cont_resources */
      XmInheritParentProcess,       /* parent_process         */
      NULL,                         /* extension              */
   },

   {                    /* bulletin_board_class fields */
      FALSE,                        /* always_install_accelerators */
      NULL,                         /* geo_matrix_create  */
      XmInheritFocusMovedProc,      /* focus_moved_proc   */
      NULL,                         /* extension          */
   },

   {                    /* square_class fields  */
      NULL,                         /* extension          */
   }
};

externaldef(squarewidgetclass) WidgetClass 
	squareWidgetClass = (WidgetClass) &squareClassRec;

static XmOffsetPtr square_offsets;		/* Part offsets table */
static XmOffsetPtr square_constraint_offsets;	/* Constraint offsets table */

/**********************************************************************
 *
 * SquareCreate - Convenience routine, used by Uil/Mrm.
 *
 *********************************************************************/

Widget SquareCreate(parent, name, arglist, nargs)
    Widget parent;
    char *name;
    Arg *arglist;
    int nargs;
{
    return(XtCreateWidget (name, squareWidgetClass, parent, arglist, nargs));
}

/**********************************************************************
 *
 * SquareMrmInitialize - register Square widget class with Mrm
 *
 *********************************************************************/

int SquareMrmInitialize()
{
    return(MrmRegisterClass (MrmwcUnknown, "Square", "SquareCreate",
			     SquareCreate, (WidgetClass)&squareClassRec));
}

/**********************************************************************
 *
 * Class methods
 *
 *********************************************************************/

static void ClassInitialize()
{
    XmResolveAllPartOffsets(squareWidgetClass, &square_offsets,
				&square_constraint_offsets);
}

static void Initialize(req, new)
    SquareWidget req;
    SquareWidget new;
{
    if (MajorDimension(new) != SquareWIDTH &&
	MajorDimension(new) != SquareHEIGHT) {
	XtWarning("Square: invalid majorDimension");
	MajorDimension(new) = SquareWIDTH;
    }
}

static Boolean SetValues(curr, req, new)
    SquareWidget curr;
    SquareWidget req;
    SquareWidget new;
{
    if (MajorDimension(new) != SquareWIDTH &&
	MajorDimension(new) != SquareHEIGHT) {
	XtWarning("Square: invalid majorDimension");
	MajorDimension(new) = MajorDimension(curr);
    }
    return (False);
}

static void ConstraintInitialize (req, new)
    Widget req;
    Widget new;
{
    Dimension m;

    if(MakeSquare(new) == True) {
        if (MajorDimension(XtParent(new))==SquareWIDTH)
            m = Width(new);
        else
            m = Height(new);

	XtResizeWidget(new, m, m, BorderWidth(new));
    }
}

static Boolean ConstraintSetValues (old, ref, new)
    Widget old;
    Widget ref;
    Widget new;
{
    Boolean redraw = False;

    if (MakeSquare(new) != MakeSquare(old)) {
	if(MakeSquare(new)==True) {
            if (MajorDimension(XtParent(new))==SquareWIDTH)
        	Height(new) = Width(new);
            else
        	Width(new) = Height(new);
	}
	else {
	    XtWidgetGeometry gi;
	    XtWidgetGeometry gp;

            if (MajorDimension(XtParent(new))==SquareWIDTH)
        	Height(new) = Height(new)/2;
            else
        	Width(new) = Width(new)/2;

	    gi.request_mode = CWWidth | CWHeight;
	    gi.width = Width(new);
	    gi.height = Height(new);
	    if (XtQueryGeometry(new, &gi, &gp) == XtGeometryAlmost) {
		if (gp.request_mode && CWWidth != 0) Width(new) = gp.width;
		if (gp.request_mode && CWHeight != 0) Height(new) = gp.height;
	    }
	}
	redraw = True;
    }
    return (redraw);
}

