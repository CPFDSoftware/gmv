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
static char rcsid[] = "$TOG: uilsymdump.c /main/6 1998/04/17 11:26:22 mgreess $"
#endif
#endif
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */

#include <uil/UilDef.h>
#include <stdio.h>
#include <ctype.h>

/*
**
** Table of Contents
**
*/
Uil_status_type		MessageCB		();
Uil_status_type	        StatusCB		();


/*
** globals
*/
FILE	*source_file;


/*
**  Entry point
*/
int
main(int argc, char *argv[])
{
    Uil_command_type	    command_desc;
    Uil_compile_desc_type   compile_desc;
    Uil_status_type	    return_status;
    int			    x = 1, y = 1;
    char		    *include_list[3];
    char		    file_name[255], listing_name[255], uid_name[255];

    /*
    ** Ask for the file name.
    */
    printf("Enter file to be compiled (leave off the .uil): ");

    *file_name = '\0';
    fgets (file_name, sizeof(file_name), stdin);
    if (strlen(file_name) && file_name[strlen(file_name)-1] == '\n')
      file_name[strlen(file_name)-1] = '\0';

    strcpy (uid_name, file_name);	strcat (uid_name, ".uid");
    strcpy (listing_name, file_name);	strcat (listing_name, ".lis");
    strcat (file_name, ".uil");

    /*
    **  Fill in the command decs
    */
    command_desc.resource_file = uid_name;
    command_desc.listing_file = listing_name;
    command_desc.include_dir_count = 2;
    command_desc.include_dir = (char **)include_list;
    command_desc.source_file = file_name;
    include_list[0] = "com:";
    include_list[1] = "../include";


    command_desc.listing_file_flag = 1;
    command_desc.resource_file_flag = 1;
    command_desc.machine_code_flag = 1;
    command_desc.report_info_msg_flag = 1;
    command_desc.report_warn_msg_flag = 1;
    command_desc.parse_tree_flag = 1;
    command_desc.status_update_delay = 400;

    return_status = Uil(&command_desc, &compile_desc, MessageCB, (char *)&x,
			StatusCB, (char *)&y);

    UilDumpSymbolTable( (sym_entry_type *)compile_desc.parse_tree_root );
    return return_status;
}

Uil_status_type	MessageCB(  message_user_data, 
			    msg_number, 
			    msg_severity,
			    msg_text,
			    src_text, 
			    ptr_text, 
			    loc_text,
			    message_summary)

    int    *message_user_data;	    /* Opaque, user-supplied argument */
    int	    msg_number;	    /* constant to identify the error */
    int	    msg_severity;   /* severity level */
    char    *msg_text;	    /* text of message */
    char    *src_text;	    /* text of source line on which error occured */
    char    *ptr_text;	    /* source column information */
    char    *loc_text;	    /* location line */
    unsigned int (*message_summary)[Uil_k_max_status+1];
 
{
    printf ("Message(%d) -- %d-%d-%s\n    %s\n    %s\n    %s\n",
		*message_user_data, msg_number, msg_severity, msg_text,
		src_text, ptr_text, loc_text);

    *message_user_data = *message_user_data + 1;
    return Uil_k_continue;
}



Uil_status_type	StatusCB(	status_user_data, 
				percent_complete, 
				lines_processed,
				current_file,
				message_summary)

    int			*status_user_data;
    unsigned int	percent_complete;
    unsigned int	lines_processed;
    char		*current_file;
    unsigned int (*message_summary)[Uil_k_max_status+1];

{
    printf("Status(%d) -- Percent: %d,  Lines: %d, File: %s\n",
	    *status_user_data, percent_complete, lines_processed, current_file);

    *status_user_data = *status_user_data + 1;
    return Uil_k_continue;
}

