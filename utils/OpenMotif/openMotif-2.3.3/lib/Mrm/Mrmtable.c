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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#ifdef REV_INFO
#ifndef lint
static char rcsid[] = "$XConsortium: Mrmtable.c /main/13 1996/11/13 14:04:22 drk $"
#endif
#endif
/* (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */

/*
 *++
 *  FACILITY:
 *
 *      UIL Manager (URM)
 *
 *  ABSTRACT:
 *
 *      This module contains the keyword tables and functions used by to
 *	support the compressing and uncompressing of strings in URM.
 *
 *--
 */


/*
 *
 *  INCLUDE FILES
 *
 */

#include <X11/Intrinsic.h>
#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>
#include "MrmMsgI.h"


/*
 *
 *  TABLE of CONTENTS
 *
 *	Urm__FixupCompressionTable	make table memory resident
 *
 *	Urm__FindClassDescriptor	find descriptor in file/memory
 *
 *	Urm__UncompressCode		uncompress a code
 *
 *	Urm__IsSubtreeResource		predicate for subtree resources
 */

/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine fixes up a file-based compression table. It resolves
 *	offsets into memory pointers. If requested, it then looks up
 *	each resulting string and attempts to replace it with a
 *	function pointer (from the function hash table).
 *
 *  FORMAL PARAMETERS:
 *
 *	ctable		the compression table to fix up
 *	qfindcldesc	if TRUE, attempt to look up the strings as
 *			indexes of class descriptors
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if all fixup operations succeed
 *	MrmFAILURE	if any operation fails (usually function lookup)    
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

Cardinal 
Urm__FixupCompressionTable (UidCompressionTablePtr	ctable,
			    Boolean			qfindcldesc,
			    Boolean			byte_swapped)
{
  /*
   *  Local variables
   */
  int			fixndx ;	/* table fixup loop index */
  Cardinal		result = MrmSUCCESS;
  WCIClassDescPtr	cldesc;		/* for class descriptor */
  Cardinal		clres;		/* lookup result */
  /*
   * Swap bytes if necessary and fix up the table offsets to be pointers
   */
  if ( byte_swapped )
    {
      swapbytes(ctable->validation);
      swapbytes(ctable->num_entries);
    }
  for ( fixndx=UilMrmMinValidCode ; fixndx<ctable->num_entries ; fixndx++ )
    {
      if ( byte_swapped ) swapbytes(ctable->entry[fixndx].stoffset);
      ctable->entry[fixndx].cstring = 
	((char *)ctable+ctable->entry[fixndx].stoffset);
    }	
  /*
   * Look up each string as a function if requested
   */
  if ( qfindcldesc )
    for ( fixndx=UilMrmMinValidCode ; fixndx<ctable->num_entries ; fixndx++ )
      {
	clres = Urm__WCI_LookupClassDescriptor
	  (ctable->entry[fixndx].cstring, &cldesc);
	if ( clres == MrmSUCCESS )
	  ctable->entry[fixndx].cldesc = cldesc;
	else
	  {
	    ctable->entry[fixndx].cldesc = NULL;
	    result = MrmFAILURE;
	  }
      }

  return result;

}    



/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine finds a class descriptor corresponding to a class
 *	compression code or name. It looks up the class in the file's
 *	compression table if possible. Otherwise, it uses the older built-in
 *	tables.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		IDB file in which to find compression table
 *	code		compression code to be uncompressed
 *	name		the class name; the convenience function name.
 *	class_return	to return the class descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if class descriptor found
 *	MrmFAILURE	otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

Cardinal 
Urm__FindClassDescriptor (IDBFile		cfile,
			  MrmCode		code,
			  char			*name,
			  WCIClassDescPtr	*class_return)
{

  /*
   * Use the built-in tables if the file has none. Else a simple lookup.
   */
  if ( code == UilMrmUnknownCode )
    return Urm__WCI_LookupClassDescriptor (name, class_return);
  if ( cfile->class_ctable == NULL )
    return Urm__UT_Error ("Urm__FindClassDescriptor", _MrmMMsg_0050,
			  NULL, NULL, MrmFAILURE);
  if ( code < UilMrmMinValidCode )
    return MrmFAILURE;
  if ( code >= cfile->class_ctable->num_entries )
    return MrmFAILURE;
  *class_return = cfile->class_ctable->entry[code].cldesc;
  if ( *class_return == NULL )
    return MrmFAILURE;
  return MrmSUCCESS;

}    



/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine returns the string corresponding to a compression code.
 *	It looks up the code in the file's compression table if it has
 *	one, else falls back on the old built-in tables. The code is looked
 *	up in the resource tables.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		IDB file in which to find compression table
 *	code		compression code to be uncompressed
 *	stg_return	to return result of uncompression
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	if uncompression successful
 *	MrmFAILURE	otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

Cardinal
Urm__UncompressCode (IDBFile			cfile,
		     MrmCode			code,
		     String			*stg_return)
{

  /*
   * A simple lookup in the file's compression tables.
   */
  if ( cfile->resource_ctable == NULL )
    return Urm__UT_Error ("Urm__UncompressCode", _MrmMMsg_0050,
			  NULL, NULL, MrmFAILURE);
  if ( code < UilMrmMinValidCode )
    return MrmFAILURE;
  if ( code >= cfile->resource_ctable->num_entries )
    return MrmFAILURE;
  *stg_return = cfile->resource_ctable->entry[code].cstring;
  return MrmSUCCESS;

}    



/*
 *++
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine checks to see if a resource is marked as rooting a
 *	widget subtree, that is, is a resource which requires that a
 *	widget subtree be instantiated as its value.
 *
 *	Initial version uses crude check. To be replaced by table lookup.
 *
 *  FORMAL PARAMETERS:
 *
 *	cfile		file containing table information
 *	code		code to be checked
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	TRUE		if subtree value is required
 *	FALSE		otherwise
 *
 * SIDE EFFECTS:
 *
 *	none
 *
 *--
 */

Boolean 
Urm__IsSubtreeResource (IDBFile			cfile,
			MrmCode			code)
{

  /*
   * Do a string comparison for for the subtree resources.
   */
  if ( code < UilMrmMinValidCode )
    return FALSE;
  if ( code >= cfile->resource_ctable->num_entries )
    return FALSE;

  if ( strcmp(cfile->resource_ctable->entry[code].cstring,XmNsubMenuId) == 0 )
    return TRUE;
  return FALSE;

}    


