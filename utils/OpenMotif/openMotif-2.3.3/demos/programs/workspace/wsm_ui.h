/* $XConsortium: wsm_ui.h /main/4 1995/07/15 20:46:54 drk $ */
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

#ifndef wsmUI_h
#define wsmUI_h

#include "wsm.h"

typedef struct _WSM_UI{
Widget wsm_row_column;
Widget *space_button;
int num_space_buttons;
Widget configure_form;
Widget from_list;
Widget to_list;
Widget copy_toggle;
Widget link_toggle;
Widget window_toggle;
Widget client_toggle;
Widget from_option_menu;
Widget *from_option_button;
Widget to_option_menu;
Widget *to_option_button;
Widget delete_button;
Widget move_button;
Space *from_space;
Space *to_space;
WorkWindow *w_window;
Widget delete_form;
Widget delete_list;
Widget ok_button;
Widget name_form;
Widget name_list;
Widget name_text;
Widget pixmap_text;
Widget background_form;
Widget background_list;
Widget background_text;
Widget save_as_form;
Widget save_as_text;
Widget occupy_form;
Widget occupy_list;
Widget copy_occupy_toggle;
Widget link_occupy_toggle;
WorkWindow* occupy_window;
} WSM_UI;

#endif
