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
/*   $XConsortium: UilCompGl.h /main/10 1995/07/14 09:32:57 drk $ */

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
**      This include file contains external declarations of all Uil
**	compiler globals excluding those which define the recognized
**	language which are declared in UilSymGl.h
**
**--
**/

#ifndef UilCompGl_h
#define UilCompGl_h

#include <setjmp.h>

/*
 * Defined in UilCmd.c
 */
externalref cmd_command_line_type		Uil_cmd_z_command;

/*
 * Defined in UilData
 */
externalref boolean				uil_v_case_sensitive;
externalref jmp_buf				uil_az_error_env_block;
externalref boolean				uil_az_error_env_valid;
#if debug_version
externalref boolean				uil_v_dump_tokens;
externalref boolean				uil_v_dump_symbols;
#endif
externalref status				uil_l_compile_status;
externalref Uil_compile_desc_type		*Uil_compile_desc_ptr;
externalref int					Uil_percent_complete;
externalref int					Uil_lines_processed;
externalref char				*Uil_current_file;

/*
 * Defined in UilDiags
 */
externalref int					Uil_message_count[];

/*
 * Defined in UilLexAna
 */
externalref int					Uil_lex_l_user_default_charset;
externalref sym_value_entry_type		*Uil_lex_az_charset_entry;
externalref int					Uil_lex_l_charset_specified;
externalref int					Uil_lex_l_localized;
externalref int					Uil_lex_l_literal_charset;
externalref sym_value_entry_type		*Uil_lex_az_literal_charset;
externalref char				*comment_text;
externalref int					comment_size;
/* % Complete */
externalref int                                Uil_characters_read;

/*
 * Defined in UilLstLst
 */
externalref char				Uil_lst_c_title2[];

/*
 * Defined in UilP2Out
 */
externalref IDBFile				out_az_idbfile_id;

/*
 * Defined in UilSarMod
 */
externalref src_source_record_type		*src_az_module_source_record;
externalref unsigned short int			*uil_urm_variant;
externalref unsigned short int			*uil_arg_compr;
externalref unsigned short int			*uil_reas_compr;
externalref unsigned short int			*uil_widget_compr;
externalref unsigned short int			*uil_child_compr;

/*
 * Defined in UilSrcSrc
 */
externalref src_source_buffer_type		*src_az_current_source_buffer;
externalref src_source_buffer_type		*src_az_avail_source_buffer;
externalref src_message_item_type		*src_az_orphan_messages;
externalref src_source_record_type		*src_az_current_source_record;
externalref src_source_record_type		*src_az_first_source_record;
externalref uil_fcb_type			*src_az_source_file_table[];
externalref int	    				src_l_last_source_file_number;
/* %COMPLETE */
externalref long                                Uil_file_size;

/*
 * Defined in UilSymStor
 */
externalref sym_name_entry_type			*sym_az_hash_table[];
externalref sym_value_entry_type		*sym_az_error_value_entry;
externalref sym_external_def_entry_type		*sym_az_external_def_chain;
externalref sym_forward_ref_entry_type		*sym_az_forward_ref_chain;
externalref sym_val_forward_ref_entry_type	*sym_az_val_forward_ref_chain;
externalref sym_module_entry_type		*sym_az_module_entry;
externalref sym_root_entry_type			*sym_az_root_entry;
externalref sym_section_entry_type		*sym_az_current_section_entry;
externalref sym_entry_type			*sym_az_entry_list_header;
externalref URMPointerListPtr			sym_az_allocated_nodes;
externalref URMPointerListPtr			sym_az_freed_nodes;

#endif /* UilCompGl_h */
/* DON'T ADD STUFF AFTER THIS #endif */
