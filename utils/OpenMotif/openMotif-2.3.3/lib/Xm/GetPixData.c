/* $XConsortium: GetPixData.c /main/6 1995/10/25 20:05:46 cde-sun $ */
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


#include <Xm/XmP.h>
#include "ImageCachI.h"
#include "XmI.h"

/*******************************************************************
 *
 * XmeGetPixmapData.
 *   see if this pixmap is in the cache. If it is then return all the
 *   gory details about it. If not put it in the cache first.
 *   The RETURN pointers can be NULL.
 *   This function never fails (unless pixmap bad id) and always returns
 *   valid information for depth, with, height, etc .
 *   It returns True if the pixmap was in the cache to start with
 *   and True if it had to fetch it to the server first.
 *
 *******************************************************************/
Boolean 
XmeGetPixmapData(
    Screen *screen,
    Pixmap pixmap,
    char **image_name,
    int *depth,
    Pixel *foreground,
    Pixel *background,
    int *hot_x,
    int *hot_y,
    unsigned int *width,
    unsigned int *height)
{
    char *loc_image_name;
    int loc_depth;
    Pixel loc_foreground;
    Pixel loc_background;
    int loc_hot_x;
    int loc_hot_y;
    unsigned int loc_width = 0;
    unsigned int loc_height;
    XtAppContext app;
    
    app = XtDisplayToApplicationContext(DisplayOfScreen(screen));
    
    _XmAppLock(app);
    
    /* support passed NULL argument */
    if (!image_name) image_name = &loc_image_name ;
    if (!depth) depth = &loc_depth ;
    if (!background) background = &loc_background ;
    if (!foreground) foreground = &loc_foreground ;
    if (!hot_x) hot_x = &loc_hot_x ;
    if (!hot_y) hot_y = &loc_hot_y ;
    if (!width) width = &loc_width ;
    if (!height) height = &loc_height ;

    if (_XmGetPixmapData(screen, pixmap, image_name, depth, foreground,
			 background, hot_x, hot_y, width, height)) {
	_XmAppUnlock(app);
	return True ;
    }

    
    /* not in the cache, generate an incomplete entry in the pixmap cache */
    /* Use a magic name, which will have _XmCachePixmap not cache this
       one in the pixmap_data name based cache */

    if (_XmCachePixmap(pixmap, screen, DIRECT_PIXMAP_CACHED, 1, 0, 0, 0, 0)) {
	/* and query again */
	_XmGetPixmapData(screen, pixmap, image_name, depth, foreground,
			 background, hot_x, hot_y, width, height);
    }

    _XmAppUnlock(app);

    return (False);
}

