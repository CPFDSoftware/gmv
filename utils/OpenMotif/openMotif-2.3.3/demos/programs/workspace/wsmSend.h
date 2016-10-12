/* $XConsortium: wsmSend.h /main/5 1995/07/15 20:46:36 drk $ */
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

#ifndef wsmSend_h
#define wsmSend_h

#include "wsm.h"

extern void CreateStartStateRequest(
WSMGetStateReply *get_state,
WSMRequest *request
				    );

extern void CreateReStartStateRequest(
WSMGetStateReply *get_state,
WSMRequest *request
				    );



extern void CreateReStartStateRequest2(
WSMGetStateReply *get_state,
WSMRequest *request
				    );

extern void GetChangeSpaceRequest(
Space*, 
WSMRequest*
);


extern void GetRegisterOldWindowReply(
WorkWindow*, WSMReply*
);


extern void GetRegisterWindowReply(
Window, WSMReply*
);


extern void SaveState(
WSMGetStateReply*,
Boolean
);

extern void RemoveWindow(
Window 
);


extern void DealWithMapNotify(
Widget,
XMapEvent *ev
);
extern void DealWithUnmapNotify(
Widget,
XUnmapEvent *ev
);



extern void DealWithClientMessage(
Widget,
XClientMessageEvent *ev
);



extern void DealWithDestroyNotify(
Widget,
XDestroyWindowEvent *ev
);

extern void DealWithDestroyMwmNotify();

extern Boolean GetCopyWindowRequest(
Window,
Space *,
Space *,
WSMRequest *
);




extern void CopyWindowToAllSpaces(
WorkWindow*,
WorkWindow*,
XrmValue*,
XrmValue*,
Space *
);


extern void CopyWindowToSpace(
WorkWindow*,
WorkWindow*,
XrmValue*,
XrmValue*,
Space *
);



extern Boolean GetMoveWindowRequest(
Window,
Space *,
Space *,
WSMRequest *
);




extern Boolean GetDeleteWindowRequest(
Window,
Space *,
WSMRequest *
);



extern Boolean GetLinkWindowRequest(
Window,
Space *,
Space *,
WSMRequest *
);


extern Boolean GetManageAllWindowsRequest(
WSMRequest*
					  );

#endif
