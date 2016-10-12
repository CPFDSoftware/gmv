/* $XConsortium: wsmStruct.h /main/5 1995/07/15 20:46:41 drk $ */
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

#ifndef wsmStruct_h
#define wsmStruct_h

#include "wsm.h"

extern Space* CreateSpace(
XrmQuark,
char*
);



extern Boolean SetSpacePixel(
Space *,
char*
);


extern Boolean SpacePixelSet(
Space *
);



extern Boolean SetSpacePixmap(
Space *,
char*
);




extern Boolean SpacePixmapSet(
Space *
);



extern Boolean SetSpaceLabelPixmap(
Space *,
char*
);




extern Boolean SpaceLabelPixmapSet(
Space *
);


extern Boolean CreateInternalStructure(
WorkWindow*,
XrmQuark*
);


extern WorkWindow* CreateWorkWindow(
Window
);


extern WorkWindow* ReCreateWorkWindow(
Window
);



extern void SetWorkWindowValues(
WorkWindow*,
Boolean,
Boolean
);

extern Space* GetSpace(
XrmQuark
		       );



extern Space* GetSpaceFromName(
char*
		       );




extern int GetSpaceID(
Space*
		       );



extern int GetNumberSpaces();


extern WorkWindow* GetWorkWindowID(
Space*,
int
);


extern Space* GetSpaceFromID(
int
);


extern WorkWindow* GetWorkWindow(
Window
				 );


extern WorkWindow* GetWorkWindowClient(
Window,
WorkWindow***,
int*
				 );




extern Boolean GetWorkWindowClientIDs(
int,
Space*,
int**,
int*
				 );




extern WorkWindow* GetIconWorkWindow(
Window
				 );




extern void AddSpaceToWindow(
Space*,
WorkWindow*
				 );



extern void AddWindowToSpace(
Space*,
WorkWindow*
				 );


extern void RemoveWorkWindowFromSpace(
Space*,
WorkWindow*
				 );



extern void RemoveSpaceFromWindow(
Space*,
WorkWindow*
				 );



extern void RemoveWorkWindow(
WorkWindow*,
Boolean
				 );



extern void RemoveSpace(
Space*
				 );



extern Boolean IsWorkWindowInSpace(
WorkWindow*,
Space*
				 );



extern Boolean IsSpaceInWorkWindow(
Space*,
WorkWindow*
				 );


extern void InitializeWsm();

extern void CompleteRestartWSM();
extern void RestartWSM();
extern void FinishRestartWSM();

extern Window GetWSMWindow(
WorkWindow*
			   );

extern void MapWorkWindow(
WorkWindow*
			  );

extern void UnmapWorkWindow(
WorkWindow*
			    );




extern void UnmapCurrentSpace(
			    );

#endif
