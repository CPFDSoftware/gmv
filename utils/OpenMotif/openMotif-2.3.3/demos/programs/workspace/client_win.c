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
static char rcsid[] = "$XConsortium: client_win.c /main/5 1995/07/14 09:47:48 drk $"
#endif
#endif
/* $XConsortium: client_win.c /main/5 1995/07/14 09:47:48 drk $ */
/*
 * Edit history
 * 01/04/92 aja - return the property
 */

/* 
 * Copyright 1989 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static Window TryChildren();

/* Find a window with WM_STATE, else return win itself, as per ICCCM */
static Atom WM_STATE = None;

Window IsClientWindow(dpy, win, retdata)
	Display *dpy;
	Window win;
	unsigned int **retdata;
{
	Atom type = None;
	int format;
	unsigned long nitems, after;
	unsigned int	*data;
	Window inf;
	/* WM_STATE        *data */

	*retdata = NULL;

	if(WM_STATE == None) {
		WM_STATE = XInternAtom(dpy, "WM_STATE", True);
	}
	if (!WM_STATE) return win;

	XGetWindowProperty(dpy, win, WM_STATE, 0, 2, False, AnyPropertyType,
		       &type, &format, &nitems, &after, (unsigned char **)&data);
	if (type) {
		*retdata = data;
		return win;
	}
	inf = TryChildren(dpy, win, WM_STATE, retdata);
	if (!inf)
		inf = win;
	return inf;
}

static Window TryChildren (dpy, win, WM_STATE, retdata)
	Display *dpy;
	Window win;
	Atom WM_STATE;
	unsigned int **retdata;
{
	Window root, parent;
	Window *children;
	unsigned int nchildren;
	unsigned int i;
	Atom type = None;
	int format;
	unsigned long nitems, after;
	unsigned int *data;
	Window inf = 0;

	if (!XQueryTree(dpy, win, &root, &parent, &children, &nchildren))
		return 0;
	for (i = 0; !inf && (i < nchildren); i++) {
		XGetWindowProperty(dpy, children[i], WM_STATE, 0, 2, False,
			AnyPropertyType, &type, &format, &nitems,
			&after, (unsigned char **)&data);
		if (type) {
			inf = children[i];
			*retdata = data;
		}
	}
	for (i = 0; !inf && (i < nchildren); i++)
		inf = TryChildren(dpy, children[i], WM_STATE, retdata);
	if (children) XFree((char *)children);
	return inf;
}
