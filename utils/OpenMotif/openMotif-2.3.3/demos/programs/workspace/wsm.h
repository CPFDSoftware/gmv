/* $TOG: wsm.h /main/6 1997/05/02 10:08:43 dbl $ */
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
#ifndef wsm_h
#define wsm_h

#include "Wsm/wsm_proto.h"

/*  Internal Data Strucures */

#define MAX_LENGTH 256

extern Widget utmShell;

typedef struct _Space {
  XrmQuark nameq;
  char name[MAX_LENGTH];
  Pixmap pixmap_label;
  char pixmap_name[MAX_LENGTH];
  char background[MAX_LENGTH];
  unsigned long pixel;
  Pixmap pixmap;
  struct _WorkWindowList *w_list;
  struct _Space *next;
} Space;

typedef struct _SpaceList {
 Space *space;
 struct _SpaceList *next;
} SpaceList;

typedef struct _WorkWindow {
  Window window;
  Boolean all_workspaces;
  Boolean linked;
  Boolean mapped;
  XrmQuarkList specifier_qlist;
  char *name;
  XrmQuarkList attrib_qlist;
  WSMAttribute *attrib_list;
  int num_attrib_list;
  Space *last_space;
  WSMWinData *win_data;
  Boolean used;
  struct _SpaceList *s_list;
  struct _WorkWindow *next;
  struct _WorkWindow *prev;
} WorkWindow;

typedef struct _WorkWindowList {
  WorkWindow *work_win;
  WorkWindow *sib_win;
  struct _WorkWindowList *next;
} WorkWindowList;

#ifdef DATA
typedef struct _DataList {
   WSMWinData **data;
   struct _DataList *next;
} DataList;
#endif


typedef struct _IndexStruct {
int hide;
int iconic;
} IndexStruct;

typedef struct _AtomStruct {
Atom save_property;
Atom wm_role_property;
Atom wm_client_leader;
Atom protocols_property;
Atom sm_client_id_property;
Atom own_selection;
} AtomStruct;

/* funcs */


extern void SendConnect(
Widget
			);


extern void SendLeaveRoom(
Space*
			  );




extern void SendSaveWsm(
char *
			  );

extern void RequestReceived(
Widget,
XtPointer,
WSMRequest*,
WSMReply*
				 );


extern void ChangeRoomCB(
Widget,
XtPointer,
XtPointer
			 );

extern void PrintRoomsCB(
Widget,
XtPointer,
XtPointer
		    );

extern void SendExtensionCB(
Widget,
XtPointer,
XtPointer
		    );

extern void SendConfigCB(
Widget,
XtPointer,
XtPointer
		    );

extern void SendStartGetStateCB(
Widget,
XtPointer,
XtPointer
		    );

extern void SendConnectCB(
Widget,
XtPointer,
XtPointer
       	    );



extern void ReceiveLeaveRoomRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);




extern void ReceiveSaveWsmRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void ReceiveEnterRoomRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void ReceiveConnectRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void ReceiveExtensionsRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);


extern void ReceiveConfigRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void ReceiveSetStateRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void SendStartSetStateRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);

extern void CopyRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);



extern void CopyWindow(
WorkWindow*,
Space*,
Space*
				);

extern void LinkRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);




extern void LinkWindow(
WorkWindow*,
Space*,
Space*
				);


extern void DeleteWindow(
WorkWindow*,
Space*
				);

extern void MoveRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);


extern void MoveWindow(
WorkWindow*,
Space*,
Space*
				);



extern void BackgroundRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);
extern void SetSpaceBackground(
Space*
			  );



extern void QuitCB(
Widget,
XtPointer,
XtPointer
			 );

extern void
ManageAllWindowsAndExit();

extern void ExitRCB(
Widget,
XtPointer,
WSMReply*,
WSMErrorCode
				);


extern void CleanUpAndExit();


#endif
