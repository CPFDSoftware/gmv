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
/*   $XConsortium: FileSB.h /main/12 1995/07/14 10:33:32 drk $ */
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _XmFSelect_h
#define _XmFSelect_h

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Type definitions for FileSB resources: */


typedef void (*XmQualifyProc)( Widget, XtPointer, XtPointer) ;
typedef void (*XmSearchProc)( Widget, XtPointer) ;



/* Class record constants */

externalref WidgetClass xmFileSelectionBoxWidgetClass;

typedef struct _XmFileSelectionBoxClassRec * XmFileSelectionBoxWidgetClass;
typedef struct _XmFileSelectionBoxRec      * XmFileSelectionBoxWidget;


#ifndef XmIsFileSelectionBox
#define XmIsFileSelectionBox(w) (XtIsSubclass((w),xmFileSelectionBoxWidgetClass))
#endif


/********    Public Function Declarations    ********/

extern Widget XmFileSelectionBoxGetChild( 
                        Widget fs,
#if NeedWidePrototypes
                        unsigned int which) ;
#else
                        unsigned char which) ;
#endif /* NeedWidePrototypes */
extern void XmFileSelectionDoSearch( 
                        Widget fs,
                        XmString dirmask) ;
extern Widget XmCreateFileSelectionBox( 
                        Widget p,
                        String name,
                        ArgList args,
                        Cardinal n) ;
extern Widget XmCreateFileSelectionDialog( 
                        Widget ds_p,
                        String name,
                        ArgList fsb_args,
                        Cardinal fsb_n) ;
extern Widget XmVaCreateFileSelectionBox(
                        Widget parent,
                        char *name,
                        ...);
extern Widget XmVaCreateManagedFileSelectionBox(
                        Widget parent,
                        char *name,
                        ...);

/********    End Public Function Declarations    ********/


#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _XmFSelect_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
