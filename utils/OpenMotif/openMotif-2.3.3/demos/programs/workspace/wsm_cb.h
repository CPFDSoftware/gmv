/* $XConsortium: wsm_cb.h /main/5 1995/07/15 20:46:46 drk $ */
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
#ifndef wsmCB_h
#define wsmCB_h

#include "wsm_ui.h"

extern void InitializeInterface(
				WSM_UI*
				);




extern void HideWsm();

extern void ShowWsm();

extern Widget CreateWorkspacePanel(
				   Widget,
				   WSM_UI*,
				   Boolean
				   );




extern void CreateNewSpaceButton(
				 int,
				 char*,
				 WSM_UI*
				 );



extern void SendLeaveRoomCB(
			    Widget,
			    XtPointer,
			    XtPointer
			    );





extern void NewActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void HideActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void SaveActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void ExitCB(
Widget,
XtPointer,
XtPointer
);

extern void CreateFromOptionButton(
int,
char*
);



extern void CreateToOptionButton(
int,
char*
);

extern void UpdateList(
Widget,
Space*
);

extern void UpdateBothList(
Space*
);

extern void CreateConfigureCB(
Widget,
XtPointer,
XtPointer
);

extern void MoveCB(
Widget,
XtPointer,
XtPointer
);


extern void DeleteCB(
Widget,
XtPointer,
XtPointer
);


extern void ToWorkspaceCB(
Widget,
XtPointer,
XtPointer
);

extern void FromWorkspaceCB(
Widget,
XtPointer,
XtPointer
);

extern void Copy(
WSM_UI*
);


extern void Link(
WSM_UI*
);

extern void OccupyCB(
Widget,
XtPointer,
XtPointer
);


extern void SelectFromListCB(
Widget,
XtPointer,
XtPointer
);



extern void MultSelectFromListCB(
Widget,
XtPointer,
XtPointer
);




extern void ClientModeCB(
Widget,
XtPointer,
XtPointer
);


extern void WindowModeCB(
Widget,
XtPointer,
XtPointer
);



extern void SelectToListCB(
Widget,
XtPointer,
XtPointer
);

extern void DismissConfigureCB(
Widget,
XtPointer,
XtPointer
);


extern void CreateNameCB(
Widget,
XtPointer,
XtPointer
);


extern void UpdateSpaceList(
Widget
);


extern void SelectNameSpaceCB(
Widget,
XtPointer,
XtPointer
);


extern void ChangeSpaceName(
WSM_UI*,
Space*,
int
);


extern void NameActivateCB(
Widget,
XtPointer,
XtPointer
);



extern void PixmapActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void NameOkActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void DismissNameCB(
Widget,
XtPointer,
XtPointer
);




extern void CreateBackgroundCB(
Widget,
XtPointer,
XtPointer
);


extern void SelectBackgroundSpaceCB(
Widget,
XtPointer,
XtPointer
);


extern void BackgroundActivateCB(
Widget,
XtPointer,
XtPointer
);


extern void DismissBackgroundCB(
Widget,
XtPointer,
XtPointer
);


extern void CreateDeleteCB(
Widget,
XtPointer,
XtPointer
);


extern void DismissDeleteCB(
Widget,
XtPointer,
XtPointer
);


extern void DeleteActivateCB(
Widget,
XtPointer,
XtPointer
);



extern void SelectDeleteCB(
Widget,
XtPointer,
XtPointer
);


extern void SaveAsCB(
Widget,
XtPointer,
XtPointer
);


extern void SaveAsOkCB(
Widget,
XtPointer,
XtPointer
);


extern void DismissSaveAsCB(
Widget,
XtPointer,
XtPointer
);





extern void CreateOccupy(
WorkWindow*
);


extern void DismissOccupyCB(
Widget,
XtPointer,
XtPointer
);


extern void OccupyActivateCB(
Widget,
XtPointer,
XtPointer
);



extern void SelectOccupyCB(
Widget,
XtPointer,
XtPointer
);



extern void CopyModeCB(
Widget,
XtPointer,
XtPointer
);


extern void LinkModeCB(
Widget,
XtPointer,
XtPointer
);

#endif
