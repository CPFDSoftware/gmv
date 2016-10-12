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
/*   $XConsortium: ExtObjectP.h /main/11 1995/07/14 10:32:48 drk $ */
/* (c) Copyright 1989, 1990  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/* (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
/* (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY  */
/* (c) Copyright 1988 MICROSOFT CORPORATION */
#ifndef _XmExtObjectP_h
#define _XmExtObjectP_h

#include <Xm/XmP.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  XmCACHE_EXTENSION = 1,		XmDESKTOP_EXTENSION,
  XmSHELL_EXTENSION,			XmPROTOCOL_EXTENSION,
  XmDEFAULT_EXTENSION
};

#ifndef XmIsExtObject
#define XmIsExtObject(w)	XtIsSubclass(w, xmExtObjectClass)
#endif /* XmIsExtObject */

#define XmLOGICAL_PARENT_RESOURCE	(0x80 << sizeof(Cardinal))

/* Class record constants */

typedef struct _XmExtRec *XmExtObject;
typedef struct _XmExtClassRec *XmExtObjectClass;

externalref WidgetClass xmExtObjectClass;

/* Class Extension definitions */

typedef struct _XmExtClassPart {
  XmSyntheticResource *syn_resources;   
  int		       num_syn_resources;   
  XtPointer	       extension;
} XmExtClassPart, *XmExtClassPartPtr;

typedef struct _XmExtClassRec {
  ObjectClassPart object_class;
  XmExtClassPart  ext_class;
} XmExtClassRec;

typedef struct {
  Widget	logicalParent;
  unsigned char	extensionType;
} XmExtPart, *XmExtPartPtr;

externalref XmExtClassRec xmExtClassRec;

typedef struct _XmExtRec {
  ObjectPart object;
  XmExtPart  ext;
} XmExtRec;


#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif  /* _XmExtObjectP_h */
