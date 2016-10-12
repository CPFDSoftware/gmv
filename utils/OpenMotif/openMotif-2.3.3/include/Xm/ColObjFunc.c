/* $XConsortium: ColObjFunc.c /main/7 1995/10/25 19:56:10 cde-sun $ */
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


#include "XmI.h"
#include "ColorObjI.h"


/**********************************************************************/
/** XmeUseColorObj()                                                 **/
/**           Return False if color is not working for some reason.  **/
/**                                                                  **/
/**           Could be due to useColorObj resource == False, or any  **/
/**           problem with the color server or color object.         **/
/**                                                                  **/
/**********************************************************************/
Boolean 
XmeUseColorObj( void )
{
    XmColorObj tmpColorObj = _XmDefaultColorObj;

    _XmProcessLock();
    if (!tmpColorObj ||
      !tmpColorObj->color_obj.colorIsRunning ||
      !tmpColorObj->color_obj.useColorObj) {
      _XmProcessUnlock();
      return False;
    }
    else {
      _XmProcessUnlock();
      return True;
    }
}



/**********************************************************************/
/** Following entries kept for bc with CDE (they'll be moved to
 **       obsolete module later                                      **/
/**                                                                  **/
/**********************************************************************/
Boolean 
_XmGetPixelData(
        int screen,
        int *colorUse,
        XmPixelSet *pixelSet,
        short *a,
        short *i,
        short *p,
        short *s )
{
    return XmeGetPixelData( screen, colorUse, pixelSet, a, i, p, s );
}

Boolean 
_XmGetIconControlInfo(
        Screen  *screen,
	Boolean *useMaskRtn,
        Boolean *useMultiColorIconsRtn,
        Boolean *useIconFileCacheRtn)
{
    return XmeGetIconControlInfo(screen, useMaskRtn, 
			  useMultiColorIconsRtn, useIconFileCacheRtn);
}

Boolean 
_XmUseColorObj( void )
{
    return XmeUseColorObj();
}

