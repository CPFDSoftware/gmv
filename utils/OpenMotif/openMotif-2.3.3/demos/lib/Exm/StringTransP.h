/* $XConsortium: StringTransP.h /main/4 1995/07/15 20:41:58 drk $ */
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
/*******************************************************************************
 *
 * StringTransP.h: The widget private header file for the ExmStringTransfer 
 *                 uniform transfer model demonstration widget.
 *
 ******************************************************************************/

/* Ensure that the file be included only once. */
#ifndef _ExmStringTransP_h
#define _ExmStringTransP_h

/* Allow for C++ compilation. */
#ifdef __cplusplus
extern "C" {
#endif


/* Include appropriate files. */
#include <Exm/StringTrans.h> /* public header file for the ExmStringTransfer widget */
#include <Exm/ExmStringP.h> /* private header file for the ExmString widget */

/* No new inheritable methods. */

/* Define the widget class part. */
typedef struct _ExmStringTransferClassPart
{
	XtPointer		extension;
} ExmStringTransferClassPart;


/* Define the full class record. */
typedef struct _ExmStringTransferClassRec
{
	CoreClassPart		core_class;
	XmPrimitiveClassPart	primitive_class;
	ExmSimpleClassPart	simple_class;
	ExmStringClassPart	string_class;
	ExmStringTransferClassPart	string_transfer_class;
} ExmStringTransferClassRec;


/* Make the full class record externally accessible. */
externalref ExmStringTransferClassRec exmStringTransferClassRec;

/* Define the widget instance part. */
typedef struct _ExmStringTransferPart
{
  /* Fields and resources for ExmStringTransfer. */
	XtCallbackList		               destination_callback;
	Boolean				       own_primary;
} ExmStringTransferPart;

/* Define the full instance record. */
typedef struct _ExmStringTransferRec
{
	CorePart		core;
	XmPrimitivePart		primitive;
	ExmSimplePart		simple;
	ExmStringPart		string;
	ExmStringTransferPart	string_transfer;
} ExmStringTransferRec, *ExmStringTransfer;


/* Define the subclassing level index to be used with ResolvePartOffset */
#define ExmStringTransferIndex (ExmStringIndex + 1)

/* Allow for C++ compilation. */
#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

/* Ensure that the file be included only once. */
#endif /* _ExmStringTransP_h */
