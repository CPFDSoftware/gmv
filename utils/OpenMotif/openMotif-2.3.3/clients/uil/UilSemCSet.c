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
static char rcsid[] = "$TOG: UilSemCSet.c /main/10 1997/03/12 15:21:53 dbl $"
#endif
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


/*
*  (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */

/*
**++
**  FACILITY:
**
**      User Interface Language Compiler (UIL)
**
**  ABSTRACT:
**
**      This file contains and routines related to the semantics of
**	character sets.
**	semantic validation.
**
**--
**/


/*
**
**  INCLUDE FILES
**
**/

#include <Xm/Xm.h>

#include "UilDefI.h"

/*
**
**  DEFINE and MACRO DEFINITIONS
**
**/



/*
**
**  EXTERNAL VARIABLE DECLARATIONS
**
**/


/*
**
**  GLOBAL VARIABLE DECLARATIONS
**
**/



/*
**
**  OWN VARIABLE DECLARATIONS
**
**/



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function returns a pointer to a character set name given
**	its sym_k_..._charset code and possibly a userdefined charset
**	value entry.
**
**  FORMAL PARAMETERS:
**
**	l_charset	charset of the string (token value)
**	az_charset_entry   charset of the string (symbol table value entry)
**
**  IMPLICIT INPUTS:
**
**      charset data tables
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      pointer to name (must NOT be freed)
**
**  SIDE EFFECTS:
**
**      error messages may be issued for objects that are still undefined
**	or of the wrong type
**
**--
**/

char *sem_charset_name (l_charset, az_charset_entry)
    int				l_charset;
    sym_value_entry_type	*az_charset_entry;

{

int		charset;		/* mapped character set */

charset = sem_map_subclass_to_charset (l_charset);
switch ( charset )
    {
    case sym_k_fontlist_default_tag:
      return XmFONTLIST_DEFAULT_TAG;      
    case sym_k_userdefined_charset:
        /*
	 ** If the charset is user-defined, then fetch info from the symbol 
	 ** table entry for it.						   
	 */
	_assert (az_charset_entry!=NULL, "null userdefined charset entry");
	return az_charset_entry->value.c_value;
    default:
	return charset_xmstring_names_table[charset];
    }

}



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function returns information about the character set specified
**	(writing direction and sixteen_bit properties).
**
**  FORMAL PARAMETERS:
**
**	l_charset	charset of the string (token value)
**	az_charset_entry   charset of the string (symbol table value entry)
**	direction	string writing direction
**	sixteen_bit	Boolean return value
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      none
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

void sem_charset_info
    (l_charset, az_charset_entry, write_direction, parse_direction,  sixteen_bit)

int			l_charset;
sym_value_entry_type	*az_charset_entry;
int			*write_direction;
int			*parse_direction;
int			*sixteen_bit;

{

int		charset;	/* mapped character set */
    
charset = sem_map_subclass_to_charset (l_charset);
switch (charset) 
    {
    /*
     ** If the charset is user-defined, then fetch info from the symbol 
     ** table entry for it.						   
     */
    case sym_k_userdefined_charset:
        {
	*write_direction = az_charset_entry->b_direction;
	*parse_direction = az_charset_entry->b_direction;
	*sixteen_bit = (az_charset_entry->b_aux_flags &
			sym_m_sixteen_bit) != 0;
	break;
	}
    default:
	{
	*write_direction = charset_writing_direction_table[charset];
	*parse_direction = charset_parsing_direction_table[charset];
	if ( charset_character_size_table[charset] != sym_k_onebyte_charsize )
	    *sixteen_bit = TRUE;
	else
	    *sixteen_bit = FALSE;
	break;
	}
	}
}



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This function returns the charset id corresponding to the
**	charset specified in a $LANG variable. User-defined character
**	sets are not recognized - it must be one available in the
**	the compiler tables.
**
**	The name match is case-insensitive.
**
**  FORMAL PARAMETERS:
**
**	lang_charset	string naming a character set
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      none
**
**  FUNCTION VALUE:
**
**      0	no match - character set not found
**	>0	character set code from sym_k_..._charset.
**		sym_k_userdefined_charset is never returned.
**		
**
**  SIDE EFFECTS:
**
**      none
**
**--
**/

int sem_charset_lang_name (lang_charset)
    char		*lang_charset;

{

char		uname[200];	/* upper-case character set name */
int		ndx;		/* loop index */


/*
 * Convert name to upper case, then search table (which is already in
 * upper case).
 */
strcpy (uname, lang_charset);
for ( ndx=0 ; ndx<(int)strlen(uname) ; ndx++ )
    uname[ndx] = _upper (uname[ndx]);

for ( ndx=0 ; ndx<(int)charset_lang_table_max ; ndx++ )
    if ( strcmp(uname,charset_lang_names_table[ndx]) == 0 )
	return (int)charset_lang_codes_table[ndx];
return 0;

}


