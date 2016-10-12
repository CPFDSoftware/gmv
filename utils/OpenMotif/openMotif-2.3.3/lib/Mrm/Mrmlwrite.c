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
static char rcsid[] = "$XConsortium: Mrmlwrite.c /main/11 1996/11/13 14:02:48 drk $"
#endif
#endif

/* (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */

/*
 *++
 *  FACILITY:
 *
 *      UIL Resource Manager (URM):
 *
 *  ABSTRACT:
 *
 *	This module contains all the routines which write a Literal
 *	from a resource context into an IDB file.
 *
 *--
 */


/*
 *
 *  INCLUDE FILES
 *
 */

#include <Mrm/MrmAppl.h>
#include <Mrm/Mrm.h>
#include "MrmMsgI.h"


/*
 *
 *  TABLE OF CONTENTS
 *
 *	UrmPutIndexedLiteral		Write indexed literal to IDB file
 *
 *	UrmPutRIDLiteral		Write RID literal to IDB file
 *
 */



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutIndexedLiteral puts a URMgLiteral resource record in the
 *	database. Its content is the literal. The resource type, access,
 *	and locking attributes are assumed to be already set.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	index		case-sensitive index for the literal
 *	context_id	resource context containing literal
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	Others:		See UrmIdbPutIndexedResource
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
UrmPutIndexedLiteral (IDBFile			file_id ,
		      String			index ,
		      URMResourceContextPtr	context_id )
{

  /*
   *  Local variables
   */
  Cardinal	result;	/* return status */

  /*
   * Validate record, then set resource group and enter in the
   * IDB file.
   */
  if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error ("UrmPutIndexedLiteral", _MrmMMsg_0043,
			  file_id, context_id, MrmBAD_CONTEXT) ;

  UrmRCSetGroup (context_id, URMgLiteral) ;
  /*
   * The size, type, access, and lock fields should have already be
   * set.
   *
   *UrmRCSetSize (context_id, ) ;
   *UrmRCSetType (context_id, ) ;
   *UrmRCSetAccess (context_id, ) ;
   *UrmRCSetLock (context_id, ) ;
   */
  result = UrmIdbPutIndexedResource (file_id, index, context_id) ;
  return result ;

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutRIDLiteral puts a literal accessed by a resource id into the
 *	database. Its content is the literal. The resource type, access,
 *	and locking attributes are assumed to be already set.
 *
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	resource_id	resource id for the record
 *	context_id	resource context containing literal 
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	Others:		See UrmIdbPutRIDResource
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
UrmPutRIDLiteral (IDBFile		file_id ,
		  MrmResource_id	resource_id ,
		  URMResourceContextPtr	context_id )
{

  /*
   *  Local variables
   */
  Cardinal		result ;	/* function results */


  /*
   * Validate record, then set resource group and enter in the
   * IDB file.
   */
  if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error ("UrmPutRIDLiteral", _MrmMMsg_0043,
			  file_id, context_id, MrmBAD_CONTEXT) ;

  UrmRCSetGroup (context_id, URMgLiteral) ;
  UrmRCSetAccess (context_id, URMaPrivate) ;

  /*
   * The size, type, and lock fields are assumed to be set.
   *
   *UrmRCSetSize (context_id, ) ;
   *UrmRCSetType (context_id, ) ;
   *UrmRCSetLock (context_id, ) ;
   */

  result = UrmIdbPutRIDResource (file_id, resource_id, context_id) ;
  return result ;

}

