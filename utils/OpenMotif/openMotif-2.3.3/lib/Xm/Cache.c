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
static char rcsid[] = "$XConsortium: Cache.c /main/12 1995/07/14 10:12:26 drk $"
#endif
#endif
/* (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/* (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "CacheI.h"
#include <Xm/GadgetP.h>


/********    Static Function Declarations    ********/


/********    End Static Function Declarations    ********/


/************************************************************************
 *
 *  _XmCacheDelete
 *	Delete an existing cache record.  NOTE: <data> is a pointer to the
 *      fourth field in the cache record - It is *not* a pointer to the
 *	cache record itself!
 *
 ************************************************************************/
void 
_XmCacheDelete(
        XtPointer data )
{
    XmGadgetCachePtr ptr;

    ptr = (XmGadgetCachePtr) DataToGadgetCache(data);
    if (--ptr->ref_count <= 0) {
      (ptr->prev)->next = ptr->next;
      if (ptr->next)			/* not the last record */
        (ptr->next)->prev = ptr->prev;
      XtFree( (char *) ptr );
    }
}

/************************************************************************
 *
 *  _XmCacheCopy
 *	Copy <size> bytes from <src> to <dest>. 
 *
 ************************************************************************/
void 
_XmCacheCopy(
        XtPointer src,
        XtPointer dest,
        size_t size )
{
    memcpy( dest, src, size);
}

/************************************************************************
 *
 *  _XmCachePart
 *	Pass in a pointer, <cpart>, to <size> bytes of a temporary Cache
 *	record.  
 *	- If the Class cache head is NULL (no entries yet!), allocate a new
 *	  cache record, copy in temporary Cache bytes, append it to the 
 *	  class-cache linked list, and return the address.
 *	- Else, run through the class linked list.
 *	  = If a match is found, increment the ref_count and return the 
 *	    address.
 *	  = Else, allocate a new cache record, copy in temporary Cache bytes,
 *	    append it to the class-cache linked list, and return the address.
 *
 ************************************************************************/
XtPointer 
_XmCachePart(
        XmCacheClassPartPtr cp,
        XtPointer cpart,
        size_t size )
{
    XmGadgetCachePtr ptr, last;
    
    if (ClassCacheHead(cp).next == NULL)       /* First one */
    {
	ClassCacheHead(cp).next = 
	  (struct _XmGadgetCache *)XtMalloc( size + 
					    XtOffsetOf(XmGadgetCacheRef, data));
        ptr = (XmGadgetCachePtr)ClassCacheHead(cp).next;

        ClassCacheCopy(cp)(cpart, CacheDataPtr(ptr), size );  
	ptr-> ref_count = 1;
        ptr-> next = NULL;
	ptr-> prev = (struct _XmGadgetCache *)&ClassCacheHead(cp);
        return (CacheDataPtr(ptr));
    }    
    ptr = (XmGadgetCachePtr)ClassCacheHead(cp).next;
    do
    {

        if ((ClassCacheCompare(cp)( cpart, CacheDataPtr(ptr))))
        {
            ptr->ref_count++;
            return ((XtPointer) CacheDataPtr(ptr));
        }
        else
        {
            last = ptr;
            ptr = (XmGadgetCachePtr)ptr->next;
        }
    } while (ptr);
    
    /* Malloc a new rec off of last, fill it out*/
    ptr = (XmGadgetCachePtr)XtMalloc( size + 
				     XtOffsetOf(XmGadgetCacheRef, data) );
    last->next = (struct _XmGadgetCache *)ptr;
    ClassCacheCopy(cp)(cpart, CacheDataPtr(ptr), size);
    ptr-> ref_count = 1;
    ptr-> next = NULL;
    ptr-> prev = last;
    return (CacheDataPtr(ptr));
}

