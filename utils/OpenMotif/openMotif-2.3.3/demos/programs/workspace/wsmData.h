/* $XConsortium: wsmData.h /main/5 1995/07/15 20:46:28 drk $ */
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

#ifndef wsmData_h
#define wsmData_h

/* i use the following code to switch between XrmValues and WSMWinData structures */


extern void UpdateXrm();

extern Boolean CreateWSMWinData(
XrmValue*,
Boolean,
WorkWindow*,
WSMWinData**,
int*
);




extern Boolean CreateStartWSMWinData(
XrmValue*,
Boolean,
WSMWinData*,
int,
WorkWindow*,
WSMWinData**,
int*
);



extern void	CreateStartHideWSMWinData(
Boolean,
WSMWinData *,
int,
WorkWindow*,
WSMWinData**,
int*
);



extern void	CreateHideWSMWinData(
Boolean,
WorkWindow*,
WSMWinData**,
int*
);


extern void	CreateUnhideWSMWinData(
XrmValue *,
WorkWindow*,
WSMWinData**,
int*
);

extern
Boolean
CreateValues(
WorkWindow *,
WSMWinData *data_list,
int num_data_list,
XrmValue **value_list_return
);



extern
void
FreeValues(
int num_values,
XrmValue *value_list
);

extern
void UpdateWinData(
WSMWinData*,
int,
WSMWinData*,
int
			);


#endif
