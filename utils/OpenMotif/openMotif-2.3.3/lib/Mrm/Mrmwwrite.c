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
static char rcsid[] = "$XConsortium: Mrmwwrite.c /main/11 1996/11/13 14:08:21 drk $"
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
 *	This module contains all the routines which write a widget
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
 *	UrmPutIndexedWidget		Write indexed widget to IDB file
 *
 *	UrmPutRIDWidget			Write RID widget to IDB file
 *
 */



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutIndexedWidget puts a URMgWidget resource record in the
 *	database. Its contents are the widget record. The resource type is
 *	the value of the widget record's type field. The access is the
 *	value of the record's access field; the locking attribute is the
 *	value of the record's lock field.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	index		case-sensitive index for the widget
 *	context_id	resource context containing widget record
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	MrmBAD_WIDGET_REC	invalid widget record in context
 *	Others:		See UrmIdbPutIndexedResource
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
UrmPutIndexedWidget (IDBFile			file_id ,
		     String			index ,
		     URMResourceContextPtr	context_id )
{
  Cardinal		result ;	/* function results */
  RGMWidgetRecordPtr	widgetrec ;	/* Widget record in context */


  /*
   * Validate record, then set resource group and type, and enter in
   * IDB file.
   */
  if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error ("UrmPutIndexedWidget", _MrmMMsg_0043,
			  file_id, NULL, MrmBAD_CONTEXT) ;
  widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
  if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error ("UrmPutIndexedWidget", _MrmMMsg_0026,
			  file_id, context_id, MrmBAD_WIDGET_REC) ;

  UrmRCSetSize (context_id, widgetrec->size) ;
  UrmRCSetGroup (context_id, URMgWidget) ;
  UrmRCSetType (context_id, widgetrec->type) ;
  UrmRCSetAccess (context_id, widgetrec->access) ;
  UrmRCSetLock (context_id, widgetrec->lock) ;

  result = UrmIdbPutIndexedResource (file_id, index, context_id) ;
  return result ;

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	UrmPutRIDWidget puts a widget accessed by a resource id into the
 *	given database file. The resource type is the value of the widget
 *	record's type field. As usual for resources accessed by resource id,
 *	the access is always private; this routine resets the record's access
 *	field if necessary to be consistent with private access. The value
 *	of the locking attribute is the value of the record's lock field.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		file into which to write record
 *	resource_id	resource id for the record
 *	context_id	resource context containing widget record
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_CONTEXT	invalid resource context
 *	MrmBAD_WIDGET_REC	invalid widget record in context
 *	Others:		See UrmIdbPutRIDResource
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
UrmPutRIDWidget (IDBFile		file_id ,
		 MrmResource_id		resource_id ,
		 URMResourceContextPtr	context_id )
{
  Cardinal		result ;	/* function results */
  RGMWidgetRecordPtr	widgetrec ;	/* Widget record in context */


  /*
   * Validate record, then set resource group and type, and enter in
   * IDB file.
   */
  if ( ! UrmRCValid(context_id) )
    return Urm__UT_Error ("UrmPutRIDWidget", _MrmMMsg_0043,
			  file_id, NULL, MrmBAD_CONTEXT) ;
  widgetrec = (RGMWidgetRecordPtr) UrmRCBuffer (context_id) ;
  if ( ! UrmWRValid(widgetrec) )
    return Urm__UT_Error ("UrmPutRIDWidget", _MrmMMsg_0026,
			  file_id, context_id, MrmBAD_WIDGET_REC) ;

  UrmRCSetSize (context_id, widgetrec->size) ;
  UrmRCSetGroup (context_id, URMgWidget) ;
  UrmRCSetType (context_id, widgetrec->type) ;
  UrmRCSetAccess (context_id, URMaPrivate) ;
  UrmRCSetLock (context_id, widgetrec->lock) ;

  result = UrmIdbPutRIDResource (file_id, resource_id, context_id) ;
  return result ;

}

