/* $XConsortium: ScrollFrameT.c /main/5 1995/07/15 20:55:20 drk $ */
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "XmI.h"
#include <Xm/TraitP.h>
#include <Xm/ScrollFrameT.h>
#include <Xm/NavigatorT.h>
#include "MessagesI.h"
#include "ScrollFramTI.h"

#define SWMessage3      _XmMMsgScrollFrameT_0000
#define SWMessage4      _XmMMsgScrollFrameT_0001



/************************************************************************
 *
 *  _XmSFAddNavigator convenience function
 *    Entering here, sf is an initialized scrollframe, and 
 *    scroll_frame_data is always valid (but the move_cb field can be 
 *    NULL, for pure APP_DEFINED case support).
 *
 *************************************<->***********************************/
void 
_XmSFAddNavigator(
    Widget sf,
    Widget nav,
    Mask dimMask,
    XmScrollFrameData scroll_frame_data)
{
    XmNavigatorTrait nav_trait ;
    XmNavigatorDataRec nav_data ;

    if ((nav_trait = (XmNavigatorTrait) 
	 XmeTraitGet((XtPointer) XtClass(nav), XmQTnavigator)) != NULL) {
	
	/* check for NULL move_cb */
	if (scroll_frame_data->move_cb)
	    nav_trait -> changeMoveCB(nav, scroll_frame_data->move_cb,
				      (XtPointer) scroll_frame_data->scrollable,
				      True) ;

	nav_data.valueMask = NavDimMask ;
	nav_data.dimMask = dimMask ;
	nav_trait -> setValue(nav, &nav_data, False);

	if (scroll_frame_data->num_nav_list == 
	    scroll_frame_data->num_nav_slots) {
	    /* Allocate more space */
	    scroll_frame_data->num_nav_slots +=  2;
	    scroll_frame_data->nav_list = 
		(WidgetList) XtRealloc((char*)scroll_frame_data->nav_list,
			scroll_frame_data->num_nav_slots * sizeof(Widget));
	}

	scroll_frame_data->nav_list[scroll_frame_data->num_nav_list] = nav;
	scroll_frame_data->num_nav_list++;

    } else {
	XmeWarning(sf, SWMessage3);
    }

}






/************************************************************************
 *
 *  _XmSFRemoveNavigator convenience function
 *
 *************************************<->***********************************/
void 
_XmSFRemoveNavigator(
    Widget sf,
    Widget nav,
    XmScrollFrameData scroll_frame_data)
{
    Cardinal	     position, i;
    XmNavigatorTrait nav_trait ;

    if ((nav_trait = (XmNavigatorTrait) 
	 XmeTraitGet((XtPointer) XtClass(nav), XmQTnavigator)) != NULL) {

	/* remove the move callback */
	if (scroll_frame_data->move_cb) 
	    nav_trait -> changeMoveCB(nav, scroll_frame_data->move_cb,
				(XtPointer) scroll_frame_data->scrollable,
				False) ;
    }  else {
	XmeWarning(sf, SWMessage4);
	return ;
    }

    for (position = 0; position<scroll_frame_data->num_nav_list; position++) {
        if (scroll_frame_data->nav_list[position] == nav) {
	    break;
	}
    }
    if (position == scroll_frame_data->num_nav_list) return;

    
    scroll_frame_data->num_nav_list--;
    for (i = position; i < scroll_frame_data->num_nav_list; i++) {
        scroll_frame_data->nav_list[i] = scroll_frame_data->nav_list[i+1];
    }

}


/************************************************************************
 *
 *  _XmSFUpdateNavigatorsValues convenience function
 *
 *************************************<->***********************************/
void
_XmSFUpdateNavigatorsValue(
        Widget sf,
        XmNavigatorData nav_data,
        Boolean notify)
{
    Cardinal i, num_nav_list ;
    Widget * nav_list ;
    Boolean inited ;

    /* there is a possibility that the SW was not inited for
       navigation business: APP_DEFINED where no scrollbar have
       been added yet */
    inited =  ((XmScrollFrameTrait) 
	       XmeTraitGet((XtPointer) XtClass(sf), XmQTscrollFrame))
	->getInfo(sf, NULL, &nav_list, &num_nav_list);

    if (!inited) return ;

    /* loop over the associated navigator list and call the change value
       method for each navigator */
    /* Updating the first navigator only if notify is True is not
       enough, since the dimension is pertinent */

    for (i=0; i < num_nav_list; i++) {
	Widget nav = nav_list[i] ;
	XmNavigatorSetValueProc	nav_setValue =
	    ((XmNavigatorTrait) 
	     XmeTraitGet((XtPointer) XtClass(nav), XmQTnavigator))->setValue;

	nav_setValue(nav, nav_data, notify);
    }
}

