/* $TOG: BBUtil.c /main/8 1997/04/15 10:23:34 dbl $ */
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
#include <Xm/PushBG.h>
#include <Xm/LabelG.h>
#include <Xm/TraitP.h>
#include <Xm/TakesDefT.h>
#include "BulletinBI.h"
#include "MessagesI.h"

/* defines for label string resources coming from message catalog */
#define OK_STRING         _XmMMsgResource_0001
#define CANCEL_STRING     _XmMMsgResource_0002
#define SELECTION_STRING  _XmMMsgResource_0003
#define APPLY_STRING      _XmMMsgResource_0004
#define HELP_STRING       _XmMMsgResource_0005
#define FILTER_STRING     _XmMMsgResource_0006
#define DIRLIST_STRING    _XmMMsgResource_0008
#define ITEMS_STRING      _XmMMsgResource_0009
#define DIRTEXT_STRING    _XmMMsgResource_0011
#define PROMPT_STRING     _XmMMsgResource_0012



/****************************************************************/
static char *
GetLabelString(
       XmLabelStringLoc l_loc )
{
    char *retval = (char*)NULL;
    switch (l_loc)
	{
	case XmOkStringLoc:
	    retval = OK_STRING;
	    break;
	case XmCancelStringLoc:
	    retval = CANCEL_STRING;
	    break;
	case XmSelectionStringLoc:
	    retval = SELECTION_STRING;
	    break;
	case XmApplyStringLoc:
	    retval = APPLY_STRING;
	    break;
	case XmHelpStringLoc:
	    retval = HELP_STRING;
	    break;
	case XmFilterStringLoc:
	    retval = FILTER_STRING;
	    break;
	case XmDirListStringLoc:
	    retval = DIRLIST_STRING;
	    break;
	case XmItemsStringLoc:
	    retval = ITEMS_STRING;
	    break;
	case XmDirTextStringLoc:
	    retval = DIRTEXT_STRING;
	    break;
	case XmPromptStringLoc:
	    retval = PROMPT_STRING;
	    break;
	default:
		/* internal error */
		break;
	}
    return retval;
}


/****************************************************************/
Widget 
_XmBB_CreateButtonG(
        Widget bb,
        XmString l_string,
	char *name,
        XmLabelStringLoc l_loc )
{
    Arg		        al[10] ;
    register Cardinal   ac = 0 ;
    Widget              button ;
    XmTakesDefaultTrait trait_default ;
    XmString            default_label_string_loc = NULL;
/****************/

    if(    l_string    )
	{
	    XtSetArg( al[ac], XmNlabelString, l_string) ; ac++ ;
        }
    else
	{
	    default_label_string_loc = XmStringCreate(GetLabelString(l_loc),
						      XmFONTLIST_DEFAULT_TAG);
	    XtSetArg( al[ac], XmNlabelString, default_label_string_loc); ac++;
	}
	    
    XtSetArg( al[ac], XmNstringDirection, BB_StringDirection( bb)) ; ac++ ;

    button = XmCreatePushButtonGadget( (Widget) bb, name, al, ac) ;

    trait_default = (XmTakesDefaultTrait) XmeTraitGet((XtPointer)
						      XtClass(button), 
						      XmQTtakesDefault) ;
    if (trait_default) 
	trait_default->showAsDefault  (button, XmDEFAULT_READY);

    if (default_label_string_loc)
	XmStringFree(default_label_string_loc);

    return( button ) ;
}

/****************************************************************/
Widget 
_XmBB_CreateLabelG(
        Widget bb,
        XmString l_string,
        char *name,
        XmLabelStringLoc l_loc )
{
            Arg		    al[10] ;
    register int            ac = 0 ;
    Widget                  label ;
    XmString                default_label_string_loc = NULL;
/****************/

    if(    l_string    )
	{
	    XtSetArg( al[ac], XmNlabelString, l_string) ; ac++ ;
        }
    else
	{
	    default_label_string_loc = XmStringCreate(GetLabelString(l_loc),
						      XmFONTLIST_DEFAULT_TAG);
	    XtSetArg( al[ac], XmNlabelString, default_label_string_loc); ac++;
	}

    XtSetArg( al[ac], XmNstringDirection, BB_StringDirection( bb)) ; ac++ ;
    XtSetArg( al[ac], XmNhighlightThickness, 0) ; ac++ ;
    XtSetArg( al[ac], XmNtraversalOn, False) ; ac++ ;
    XtSetArg( al[ac], XmNalignment, XmALIGNMENT_BEGINNING) ; ac++ ;

    label = XmCreateLabelGadget( bb, name, al, ac);

    if (default_label_string_loc)
	XmStringFree(default_label_string_loc);

    return( label ) ;
	
}

