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
static char rcsid[] = "$XConsortium: MrmIindex.c /main/13 1996/11/13 13:57:31 drk $"
#endif
#endif

/* (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */



/*
 *++
 *  FACILITY:
 *
 *      UIL Resource Manager (URM): IDB Facility
 *	Index management routines
 *
 *  ABSTRACT:
 *
 *	These routines manage the index of an IDB file, including entering
 *	data entries accessed by index. These routines are read or common
 *	(used by both read and writing (MrmIindexw.c)).
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
#include <Mrm/IDB.h>
#include "MrmMsgI.h"


/*
 *
 *  TABLE OF CONTENTS
 *
 *	Idb__INX_ReturnItem		- Return the data entry for an index
 *
 *	Idb__INX_FindIndex		- Search the index
 *
 *	Idb__INX_SearchIndex		- Search a record for an index
 *
 *	Idb__INX_GetBTreeRecord		- Read a record in the B-tree
 *
 *	Idb__INX_FindResources		- Search the index for resources 
 *					  matching the filter
 *
 */


/*
 *
 *  DEFINE and MACRO DEFINITIONS
 *
 */

/*
 * Macros which validate index records in buffers
 */
#define	Idb__INX_ValidLeaf(buffer) \
     (_IdbBufferRecordType(buffer)==IDBrtIndexLeaf)
#define	Idb__INX_ValidNode(buffer) \
     (_IdbBufferRecordType(buffer)==IDBrtIndexNode)
#define	Idb__INX_ValidRecord(buffer) \
     (_IdbBufferRecordType(buffer)==IDBrtIndexLeaf ||  \
      _IdbBufferRecordType(buffer)==IDBrtIndexNode)



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	Idb__INX_ReturnItem locates a data entry in the file, and returns
 *	the data entry pointer (without reading the data record).
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		Open IDB file in which to write entry
 *	index		The entry's case-sensitive index
 *	data_entry	To return data entry pointer for data
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	some other failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
Idb__INX_ReturnItem (IDBFile			file_id,
		     char			*index,
		     IDBDataHandle		*data_entry)
{

  /*
   *  Local variables
   */
  Cardinal		result ;	/* function results */
  IDBRecordBufferPtr	bufptr ;	/* buffer containing entry */
  MrmCount		entndx ;	/* entry index */
  IDBIndexLeafRecordPtr	leafrec ;	/* index leaf record */
  IDBIndexNodeRecordPtr	noderec ;	/* index node record */

  /*
   * Attempt to find the index
   */
  result = Idb__INX_FindIndex (file_id, index, &bufptr, &entndx) ;
  switch ( result )
    {
    case MrmINDEX_GT:
    case MrmINDEX_LT:
      return MrmNOT_FOUND ;
    case MrmSUCCESS:
      break ;
    default:
      return result ;
    }

  /*
   * Point into the buffer, and retrieve the data pointer
   */
  switch ( _IdbBufferRecordType (bufptr) )
    {
    case IDBrtIndexLeaf:
      leafrec = (IDBIndexLeafRecordPtr) bufptr->IDB_record ;
      data_entry->rec_no = leafrec->index[entndx].data.internal_id.rec_no ;
      data_entry->item_offs =
	leafrec->index[entndx].data.internal_id.item_offs ;
      return MrmSUCCESS ;
    case IDBrtIndexNode:
      noderec = (IDBIndexNodeRecordPtr) bufptr->IDB_record ;
      data_entry->rec_no = noderec->index[entndx].data.internal_id.rec_no ;
      data_entry->item_offs =
	noderec->index[entndx].data.internal_id.item_offs ;
      return MrmSUCCESS ;
    default:
      return Urm__UT_Error ("Idb__INX_ReturnItem", _MrmMMsg_0010,
			    file_id, NULL, MrmBAD_RECORD) ;
    }

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	Idb__INX_FindIndex finds the index record containing an index entry,
 *	and returns the buffer containing that record. It is used both as the
 *	low-level routine for locating an index for retrieving a data entry,
 *	and for locating the record in which a new index should be inserted.
 *	Thus the interpretation of the return code is:
 *
 *	MrmSUCCESS	found the index, the index record is in the buffer
 *			and the index_return locates the entry
 *	MrmINDEX_GT	buffer contains the leaf index record which should
 *	MrmINDEX_LT	contain the index, and index_return locates the entry
 *			in the buffer at which search terminated. The result
 *			value indicates how the given index orders against
 *			the entry in index_return.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		Open IDB file in which to find index
 *	index		Case-sensitive index string
 *	buffer_return	To return pointer to buffer containing index record
 *	index_return	To return item's index in the records index vector
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmINDEX_GT	index not found, but orders greater-than entry at
 *			index_return
 *	MrmINDEX_LT	index not found, but orders less-than entry at
 *			index_return
 *	MrmFAILURE	some other failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
Idb__INX_FindIndex (IDBFile			file_id,
		    char			*index,
		    IDBRecordBufferPtr		*buffer_return,
		    MrmCount			*index_return)
{

  /*
   *  Local variables
   */
  Cardinal		result ;	/* function results */

  /*
   * Initialize search at the root of the index, then continue searching
   * until either the index is found or search terminates at some leaf record.
   */
  if ( !file_id->index_root ) return MrmFAILURE ;
  result = Idb__BM_GetRecord (file_id, file_id->index_root, buffer_return) ;
  if ( result != MrmSUCCESS ) return result ;
  if ( ! Idb__INX_ValidRecord(*buffer_return) )
    return Urm__UT_Error ("Idb__INX_FindIndex", _MrmMMsg_0010,
			  file_id, NULL, MrmBAD_RECORD) ;

  do  {
    result =
      Idb__INX_SearchIndex (file_id, index, *buffer_return, index_return) ;
    if ( _IdbBufferRecordType(*buffer_return) == IDBrtIndexLeaf) return result ;
    switch ( result )
      {
      case MrmINDEX_GT:
      case MrmINDEX_LT:
	result = Idb__INX_GetBtreeRecord
	  (file_id, buffer_return, *index_return, result) ;
	if (result != MrmSUCCESS )
	  {
	    if (result == MrmNOT_FOUND)
	      result = MrmEOF;
	    return result ;
	  }
	break ;
      default:
	return result ;
      }
  } while ( TRUE ) ;

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	Idb__INX_SearchIndex searches a record for an index. The record
 *	may be either a leaf or a node record. If the index is found,
 *	index_return is its entry in the records index vector. If it is not
 *	found, then index_return locates the entry in the record at which
 *	search terminated.
 *
 *	Thus the interpretation of the return code is:
 *
 *	MrmSUCCESS	found the index, and the index_return locates the entry
 *	MrmINDEX_GT	index orders greater-than the entry at index_return
 *	MrmINDEX_LT	index orders less-than the entry at index_return
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		Open IDB file in which to find index
 *	index		Case-sensitive index string
 *	buffer		Buffer containing record to be searched
 *	index_return	To return item's index in the records index vector
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmINDEX_GT	index not found, but orders greater-than entry at
 *			index_return
 *	MrmINDEX_LT	index not found, but orders less-than entry at
 *			index_return
 *	MrmFAILURE	some other failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
Idb__INX_SearchIndex (IDBFile			file_id,
		      char			*index,
		      IDBRecordBufferPtr	buffer,
		      MrmCount			*index_return)
{

  /*
   *  Local variables
   */
  MrmType		buftyp ;	/* buffer type */
  IDBIndexLeafRecordPtr	leafrec ;	/* index leaf record */
  IDBIndexLeafHdrPtr	leafhdr ;	/* index leaf header */
  IDBIndexNodeRecordPtr	noderec ;	/* index node record */
  IDBIndexNodeHdrPtr	nodehdr ;	/* index node header */
  IDBIndexLeafEntryPtr	leaf_ndxvec ;	/* index leaf entry vector */
  IDBIndexNodeEntryPtr	node_ndxvec ;	/* index node entry vector */
  MrmCount		ndxcnt ;	/* number of entries in vector */
  char			*stgbase ;	/* base adddress for string offsets */
  int			lowlim ;	/* binary search lower limit index */
  int			uprlim ;	/* binary search upper limit index */
  char			*ndxstg ;	/* pointer to current index string */
  int			cmpres ;	/* strncmp result */


  /*
   * Set up search pointers based on the record type
   */
  buftyp = _IdbBufferRecordType (buffer) ;
  switch ( buftyp )
    {
    case IDBrtIndexLeaf:
      leafrec = (IDBIndexLeafRecordPtr) buffer->IDB_record ;
      leafhdr = (IDBIndexLeafHdrPtr) &leafrec->leaf_header ;
      leaf_ndxvec = leafrec->index ;
      ndxcnt = leafhdr->index_count ;
      stgbase = (char *) leafrec->index ;
      break ;
    case IDBrtIndexNode:
      noderec = (IDBIndexNodeRecordPtr) buffer->IDB_record ;
      nodehdr = (IDBIndexNodeHdrPtr) &noderec->node_header ;
      node_ndxvec = noderec->index ;
      ndxcnt = nodehdr->index_count ;
      stgbase = (char *) noderec->index ;
      break ;
    default:
      return Urm__UT_Error ("Idb__INX_SearchIndex", _MrmMMsg_0010,
			    file_id, NULL, MrmBAD_RECORD) ;
    }

  /*
   * Search the index vector for the given index (binary search)
   */
  Idb__BM_MarkActivity (buffer) ;
  for ( lowlim=0,uprlim=ndxcnt-1 ; lowlim<=uprlim ; )
    {
      *index_return = (lowlim+uprlim) / 2 ;
      ndxstg = (buftyp==IDBrtIndexLeaf) ?
        (char *) stgbase + leaf_ndxvec[*index_return].index_stg :
        (char *) stgbase + node_ndxvec[*index_return].index_stg ;
      cmpres = strncmp (index, ndxstg, IDBMaxIndexLength) ;
      if ( cmpres == 0 ) return MrmSUCCESS ;
      if ( cmpres < 0 ) uprlim = *index_return - 1 ;
      if ( cmpres > 0 ) lowlim = *index_return + 1 ;
    }

  /*
   * Not found, result determined by final ordering.
   */
  return (cmpres>0) ? MrmINDEX_GT : MrmINDEX_LT ;

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This routine reads in the next level index record in the B-tree
 *	associated with some entry in the current record (i.e. the one
 *	currently contained in the buffer). The buffer pointer is reset.
 *	The order variable indicates which record to read:
 *		MrmINDEX_GT - read the record ordering greater-than the entry
 *		MrmINDEX_LT - read the record ordering less-than the entry
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		Open IDB file from which to read record
 *	buffer_return	points to current buffer; reset to buffer read in
 *	entry_index	entry in current buffer to use as reference
 *	order		MrmINDEX_GT for GT ordered record, else MrmINDEX_LT
 *			for LT ordered record.
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmBAD_ORDER	Order variable has illegal value
 *	MrmBAD_RECORD	new record not an index record
 *	MrmFAILURE	some other failure
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
Idb__INX_GetBtreeRecord ( IDBFile		file_id,
			  IDBRecordBufferPtr	*buffer_return,
			  MrmCount		entry_index,
			  Cardinal		order)
{

  /*
   *  Local variables
   */
  Cardinal		result ;	/* function results */
  IDBIndexNodeRecordPtr	recptr ;	/* node record in buffer */
  IDBRecordNumber	recno ;		/* Record number to read in */

  /*
   * Set buffer pointers
   */
  recptr = (IDBIndexNodeRecordPtr) (*buffer_return)->IDB_record ;

  /*
   * Retrieve the record number
   */
  switch ( order )
    {
    case MrmINDEX_GT:
      recno = recptr->index[entry_index].GT_record ;
      break ;
    case MrmINDEX_LT:
      recno = recptr->index[entry_index].LT_record ;
      break ;
    default:
      return Urm__UT_Error ("Idb__INX_GetBTreeRecord", _MrmMMsg_0010,
			    file_id, NULL, MrmBAD_ORDER) ;
    }

  /*
   * Retrieve and sanity check the record
   */
  result = Idb__BM_GetRecord (file_id, recno, buffer_return) ;
  if ( result != MrmSUCCESS ) return result ;
  if ( ! Idb__INX_ValidRecord(*buffer_return) )
    return Urm__UT_Error ("Idb__INX_GetBTreeRecord", _MrmMMsg_0010,
			  file_id, NULL, MrmBAD_RECORD) ;

  /*
   * Record successfully retrieved
   */
  return MrmSUCCESS ;

}



/*
 *++
 *
 *  PROCEDURE DESCRIPTION:
 *
 *	This is the internal routine which searches the database for
 *	indexed resources matching a filter. It starts at the current node,
 *	then recurses down the BTree inspecting every entry. Each entry
 *	which matches the filter is appended to the index list.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_id		The IDB file id returned by XmIdbOpenFile
 *	recno		The record to be searched. If a node entry,
 *			then each pointed-to record is also searched.
 *	group_filter	if not null, entries found must match this group
 *	type_filter	if not null, entries found must match this type
 *	index_list	A pointer list in which to return index
 *			strings for matches. The required strings
 *			are automatically allocated.
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	MrmSUCCESS	operation succeeded
 *	MrmFAILURE	operation failed, no further reason
 *
 *  SIDE EFFECTS:
 *
 *--
 */

Cardinal 
Idb__INX_FindResources (IDBFile			file_id,
			IDBRecordNumber		recno,
			MrmGroup		group_filter,
			MrmType			type_filter,
			URMPointerListPtr	index_list)
{

  /*
   *  Local variables
   */
  Cardinal		result ;	/* function results */
  IDBRecordBufferPtr	bufptr ;	/* buffer containing entry */
  int			entndx ;	/* entry loop index */
  IDBIndexLeafRecordPtr	leafrec ;	/* index leaf record */
  IDBIndexLeafHdrPtr	leafhdr ;	/* index leaf header */
  IDBIndexNodeRecordPtr	noderec ;	/* index node record */
  IDBIndexNodeHdrPtr	nodehdr ;	/* index node header */
  IDBIndexLeafEntryPtr	leaf_ndxvec ;	/* index leaf entry vector */
  IDBIndexNodeEntryPtr	node_ndxvec ;	/* index node entry vector */
  MrmCount		ndxcnt ;	/* number of entries in vector */
  char			*stgbase ;	/* base adddress for string offsets */



  /*
   * Read the record in, then bind pointers and process the record.
   */
  result = Idb__BM_GetRecord (file_id, recno, &bufptr) ;
  if ( result != MrmSUCCESS ) return result ;

  switch ( _IdbBufferRecordType (bufptr) )
    {

      /*
       * Simply apply the filter to all entries in the leaf record
       */
    case IDBrtIndexLeaf:
      leafrec = (IDBIndexLeafRecordPtr) bufptr->IDB_record ;
      leafhdr = (IDBIndexLeafHdrPtr) &leafrec->leaf_header ;
      leaf_ndxvec = leafrec->index ;
      ndxcnt = leafhdr->index_count ;
      stgbase = (char *) leafrec->index ;

      for ( entndx=0 ; entndx<ndxcnt ; entndx++ )
	{
	  IDBDataHandle	entry_data;

	  entry_data.rec_no = leaf_ndxvec[entndx].data.internal_id.rec_no;
	  entry_data.item_offs =
	    leaf_ndxvec[entndx].data.internal_id.item_offs;

	  if ( Idb__DB_MatchFilter(file_id, entry_data, group_filter, 
				   type_filter) )
	    UrmPlistAppendString (index_list,
				  stgbase+leaf_ndxvec[entndx].index_stg) ;
	  Idb__BM_MarkActivity (bufptr) ;
	}
      return MrmSUCCESS ;

      /*
       * Process the first LT record, then process each index followed by
       * its GT record. This will produce a correctly ordered list. The
       * record is read again, and all pointers bound, after each FindResources
       * call in order to guarantee that buffer turning has not purged the
       * current record from memory
       */
    case IDBrtIndexNode:
      noderec = (IDBIndexNodeRecordPtr) bufptr->IDB_record ;
      nodehdr = (IDBIndexNodeHdrPtr) &noderec->node_header ;
      node_ndxvec = noderec->index ;
      ndxcnt = nodehdr->index_count ;
      stgbase = (char *) noderec->index ;
      result = Idb__INX_FindResources
	(file_id, node_ndxvec[0].LT_record,
	 group_filter, type_filter, index_list) ;
      if ( result != MrmSUCCESS ) return result ;

      for ( entndx=0 ; entndx<ndxcnt ; entndx++ )
	{
	  IDBDataHandle	entry_data;

	  entry_data.rec_no = node_ndxvec[entndx].data.internal_id.rec_no;
	  entry_data.item_offs =
	    node_ndxvec[entndx].data.internal_id.item_offs;

	  Idb__BM_GetRecord (file_id, recno, &bufptr) ;
	  noderec = (IDBIndexNodeRecordPtr) bufptr->IDB_record ;
	  nodehdr = (IDBIndexNodeHdrPtr) &noderec->node_header ;
	  node_ndxvec = noderec->index ;
	  stgbase = (char *) noderec->index ;
	  if ( Idb__DB_MatchFilter
	       (file_id, entry_data, group_filter, type_filter) )
	    UrmPlistAppendString (index_list,
				  stgbase+node_ndxvec[entndx].index_stg) ;
	  result = Idb__INX_FindResources
	    (file_id, node_ndxvec[entndx].GT_record,
	     group_filter, type_filter, index_list) ;
	  if ( result != MrmSUCCESS ) return result ;
	}
      return MrmSUCCESS ;

    default:
      return Urm__UT_Error ("Idb__INX_FindResources", _MrmMMsg_0010,
			    file_id, NULL, MrmBAD_RECORD) ;
    }

}

