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
static char rcsid[] = "$XConsortium: wmlutils.c /main/8 1995/08/29 11:11:24 drk $"
#endif
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


/*
*  (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */

/*
 * This file contains utilities used by WML.
 */


#include "wml.h"

#if defined(__STDC__)
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>


/*
 * Utility to allocate dynamic space for a string, and return the
 * dynamic copy. Produces a NULL on null input.
 */

char *wmlAllocateString (stg)
    char	*stg;

{

char		*dynstg;	/* the dynamic copy */


if ( stg == NULL ) return NULL;

dynstg = (char *) malloc (strlen(stg)+1);
strcpy (dynstg, stg);
return dynstg;

}



/*
 * Utility to convert a string to upper case. The conversion happens in
 * place, destroying the original string.
 */

void wmlUpperCaseString (stg)
    char	*stg;

{

int		ndx;		/* loop index */


if ( stg == NULL ) return;
for ( ndx=0 ; ndx<strlen(stg) ; ndx++ )
    stg[ndx] = _upper (stg[ndx]);

}



/*
 * Routines for accessing and manipulating dynamic handle lists.
 */


/*
 * Initialize a dynamic handle list. Allocate a vector of the given
 * size, and set the count and number used (0).
 *
 *	listptr		the list to be inited
 *	size		# entries in handle vector
 *	is_ordered	TRUE is list is to be ordered
 */

void wmlInitHList (listptr, size, is_ordered)
    DynamicHandleListDefPtr	listptr;
    int				size;
    int				is_ordered;

{

listptr->cnt = 0;
listptr->max = size;
listptr->ordered = is_ordered;
listptr->hvec = (ObjectHandleDefPtr) malloc(size*sizeof(ObjectHandleDef));

return;

}



/*
 * Routine to resize a dynamic handle list. Increases the size if required,
 * but does nothing if the list is already big enough.
 *
 *	listptr		the dynamic list
 *	new_size	new list size
 */

void wmlResizeHList (listptr, new_size)
    DynamicHandleListDefPtr	listptr;
    int				new_size;

{    

ObjectHandleDefPtr	new_vec;	/* reallocated vector */


if ( listptr->max >= new_size ) return;
listptr->max = new_size;
new_vec = (ObjectHandleDefPtr) realloc
    (listptr->hvec, new_size*sizeof(ObjectHandleDef));
listptr->hvec = new_vec;

return;

}



/*
 * Routine to clear a dynamic handle list. It leaves the handle vector intact,
 * but frees all the allocated names. The count is reset to 0.
 * but does nothing if the list is already big enough.
 *
 *	listptr		the dynamic list
 */

void wmlClearHList (listptr)
    DynamicHandleListDefPtr	listptr;

{    

int		ndx;		/* current index in list */


for ( ndx=0 ; ndx<listptr->cnt ; ndx++ )
    {
    free (listptr->hvec[ndx].objname);
    listptr->hvec[ndx].objname = NULL;
    }
listptr->cnt = 0;

return;

}



/*
 * Function to find a name in a dynamic list. This will function on both
 * ordered and unordered lists.
 *
 *	listptr		the dynamic list
 *	name		the name to look up in the list
 *
 * returns:
 *	>= 0		name found, index in list
 *	< 0		name not found
 */

int wmlFindInHList (listptr, name)
    DynamicHandleListDefPtr	listptr;
    char			*name;

{

int		ndx;		/* current index in list */
int		londx;		/* low index */
int		hindx;		/* high index */
int		midndx;		/* midpoint index */
int		cmpres;		/* strcmp result */


/*
 * Binary search if ordered, brute force otherwise
 */
if ( listptr->ordered )
    {
    for ( londx=0,hindx=listptr->cnt-1 ; hindx>=londx ; )
	{
	midndx = (londx+hindx) / 2;
	cmpres = strcmp (name, listptr->hvec[midndx].objname);
	if ( cmpres < 0 )
	    hindx = midndx - 1;
	if ( cmpres > 0 )
	    londx = midndx + 1;
	if ( cmpres == 0 )
	    return midndx;
	}
    return -1;
    }
else
    {
    for ( ndx=0 ; ndx<listptr->cnt ; ndx++ )
	if ( strcmp(name,listptr->hvec[ndx].objname) == 0 )
	    return ndx;
    return -1;
    }

}



/*
 * Routine to insert an entry into a list. The insertion is ordered or
 * unordered depending on the way the list is marked. Unordered lists
 * insert at the end. This routine assumes no duplicates will be entered
 * in the list.
 *
 *	listptr		the list
 *	name		the name under which to insert
 *	obj		the object to insert
 */

void wmlInsertInHList (listptr, name, obj)
    DynamicHandleListDefPtr	listptr;
    char			*name;
    ObjectPtr			obj;

{

int		ndx;		/* current index in list */
int		londx;		/* low index */
int		hindx;		/* high index */
int		midndx;		/* midpoint index */
int		newndx;		/* new entry index */
int		cmpres;		/* strcmp result */


/*
 * Guarantee enough space in the list
 */
wmlResizeHList (listptr, listptr->cnt+1);

/*
 * Binary search and insert if ordered, brute force otherwise
 */
if ( listptr->ordered )
    {
    if ( listptr->cnt == 0 )
	{
	listptr->hvec[0].objname = wmlAllocateString (name);
	listptr->hvec[0].objptr = obj;
	listptr->cnt += 1;
	return;
	}
    for ( londx=0,hindx=listptr->cnt-1 ; hindx>=londx ; )
	{
	midndx = (londx+hindx) / 2;
	cmpres = strcmp (name, listptr->hvec[midndx].objname);
	if ( cmpres == 0 )
	    {
	    printf ("\nwmlInsertInHList: duplicate name '%s'found\n", name);
	    return;
	    }
	if ( londx == hindx ) break;
	if ( cmpres < 0 )
	    hindx = midndx - 1;
	if ( cmpres > 0 )
	    londx = midndx + 1;
	}
    /*
     * The new entry will go either at midndx or after midndx. Move down
     * the vector appropriately.
     */
    if ( cmpres < 0 )
	newndx = midndx;
    else
	newndx = midndx + 1;
    for ( ndx=listptr->cnt-1 ; ndx>=newndx ; ndx-- )
	{
	listptr->hvec[ndx+1].objname = listptr->hvec[ndx].objname;
	listptr->hvec[ndx+1].objptr = listptr->hvec[ndx].objptr;
	}
    listptr->hvec[newndx].objname = wmlAllocateString (name);
    listptr->hvec[newndx].objptr = obj;
    listptr->cnt += 1;
    return;
    }
else
    {
    listptr->hvec[listptr->cnt].objname = wmlAllocateString (name);
    listptr->hvec[listptr->cnt].objptr = obj;
    listptr->cnt += 1;
    return;
    }

}


/*
 * Routine to insert an entry into a token list. The insertion is ordered.
 * This routine allows duplicates
 *
 *	listptr		the list
 *	name		the name under which to insert
 *	obj		the object to insert
 */
void wmlInsertInKeyList (listptr, name, obj)
    DynamicHandleListDefPtr	listptr;
    char			*name;
    ObjectPtr			obj;

{
  int		ndx;				  /* current index in list */
  int		londx;				  /* low index */
  int		hindx;				  /* high index */
  int		midndx;				  /* midpoint index */
  int		newndx;				  /* new entry index */
  int		cmpres;				  /* strcmp result */

  /*
   * Guarantee enough space in the list
   */
  wmlResizeHList (listptr, listptr->cnt+1);

  /*
   * Binary search and insert
   */
  if ( listptr->cnt == 0 )
    {
      listptr->hvec[0].objname = wmlAllocateString (name);
      listptr->hvec[0].objptr = obj;
      listptr->cnt += 1;
      return;
    }
  for ( londx=0,hindx=listptr->cnt-1 ; hindx>=londx ; )
    {
      midndx = (londx+hindx) / 2;
      cmpres = strcmp (name, listptr->hvec[midndx].objname);
      if ( londx == hindx ) break;
      if ( cmpres < 0 )
	hindx = midndx - 1;
      if ( cmpres >= 0 )
	londx = midndx + 1;
    }
  /*
   * The new entry will go either at midndx or after midndx. Move down
   * the vector appropriately.
   */
  if ( cmpres < 0 )
    newndx = midndx;
  else
    newndx = midndx + 1;
  for ( ndx=listptr->cnt-1 ; ndx>=newndx ; ndx-- )
    {
      listptr->hvec[ndx+1].objname = listptr->hvec[ndx].objname;
      listptr->hvec[ndx+1].objptr = listptr->hvec[ndx].objptr;
    }
  listptr->hvec[newndx].objname = wmlAllocateString (name);
  listptr->hvec[newndx].objptr = obj;
  listptr->cnt += 1;
  return;
}


/*
 * Indicate if a resource is in a resource reference list by returning its
 * reference pointer.
 */

WmlClassResDefPtr wmlResolveResIsMember (resobj, resref)
    WmlResourceDefPtr		resobj;
    WmlClassResDefPtr		resref;

{

while ( resref != NULL )
    {
    if ( resref->act_resource == resobj ) return resref;
    resref = resref->next;
    }
return NULL;

}


/*
 * Indicate if a child is in a child reference list by returning its
 * reference pointer.
 */

WmlClassChildDefPtr wmlResolveChildIsMember (childobj, childref)
    WmlChildDefPtr		childobj;
    WmlClassChildDefPtr		childref;

{

while ( childref != NULL )
    {
    if ( childref->act_child == childobj ) return childref;
    childref = childref->next;
    }
return NULL;

}


