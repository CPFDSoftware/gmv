/* $XConsortium: ExmMrm.c /main/4 1995/07/15 20:40:36 drk $ */
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

#include <stdio.h>
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>

#include <Exm/Simple.h>
#include <Exm/ExmString.h>
#include <Exm/StringTrans.h>
#include <Exm/CommandB.h>
#include <Exm/MenuB.h>
#include <Exm/Grid.h>
#include <Exm/TabB.h>
#include <Exm/Panner.h>

/**********************************************************************
 *
 * ExmMrmInitialize - register Exm widget classes with Mrm
 *
 *********************************************************************/

int ExmMrmInitialize()
{
    MrmRegisterClass (MrmwcUnknown, "ExmSimple",
			"ExmCreateSimple", ExmCreateSimple,
			exmSimpleWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmString",
			"ExmCreateString", ExmCreateString,
			exmStringWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmStringTransfer",
			"ExmCreateStringTransfer", ExmCreateStringTransfer,
			exmStringTransferWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmGrid",
			"ExmCreateGrid", ExmCreateGrid,
			exmGridWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmCommandButton",
			"ExmCreateCommandButton", ExmCreateCommandButton,
			exmCommandButtonWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmMenuButton",
			"ExmCreateMenuButton", ExmCreateMenuButton,
			exmMenuButtonWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmTabButton",
			"ExmCreateTabButton", ExmCreateTabButton,
			exmTabButtonWidgetClass);
    MrmRegisterClass (MrmwcUnknown, "ExmPanner",
			"ExmCreatePanner", ExmCreatePanner,
			exmPannerWidgetClass);
    return (0);
}

