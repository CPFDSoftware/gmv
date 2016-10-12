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
static char rcsid[] = "$XConsortium: hellomotif.c /main/6 1995/07/14 09:42:12 drk $"
#endif
#endif
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
#include <stdlib.h>
#include <Xm/XmAll.h>       /* Motif Toolkit */
#include <Mrm/MrmPublic.h>  /* Mrm Toolkit */


/* Although it's really bad to hardcode Positions, keep this for
   now for compatibility */
#define NEW_BUTTON_X 11
/* not mentioning hardcoding labels... look at hellomotif_i18n for a 
   better example  */
#define NEW_BUTTON_LABEL  "Goodbye\nWorld!"


static void Activate(
        Widget	widget,
	XtPointer tag,
	XtPointer data);



static String uid_vec[]={"hellomotif.uid"}; /* MRM database file list   */
static MrmRegisterArg	reg_vec[] = {       /* Callback bindings */
    {"helloworld_button_activate", (XtPointer)Activate}
};


/*
 *  Main program: fetch the hellomotif.uil description.
 */
int main(int argc, char *argv[])
{
    XtAppContext        app_context;
    Widget              top_level, helloworld_main;
    MrmCode		mrm_class ;
    MrmHierarchy	mrm_hierarchy;		

    /*
     *  Initialize the MRM
     */

    MrmInitialize ();

    /* 
     * Initialize Xt and create a resizable shell 
     */

    top_level = XtVaAppInitialize(&app_context, "XmdHelloMotif", 
				  NULL, 0, &argc, argv, NULL, 
				  XmNallowShellResize, True, NULL); 

    /*
     *  Create the Mrm hierarchy
     */

    if (MrmOpenHierarchy (XtNumber(uid_vec),	    /* number of files	    */
			  uid_vec, 		    /* files     	    */
			  NULL,			    /* os_ext_list (null)   */
			  &mrm_hierarchy)	    /* ptr to returned id   */
	!= MrmSUCCESS) {
	XtError ("can't open hierarchy\n");
    }

    /*
     * 	Register our callback routines so that the resource manager can 
     * 	resolve them at widget-creation time.
     */

    if (MrmRegisterNames (reg_vec, XtNumber(reg_vec)) != MrmSUCCESS)
	XtError("can't register names\n");

    /*
     *  Call MRM to fetch and create the widgets 
     */

    if (MrmFetchWidget (mrm_hierarchy,
			"helloworld_main",
			top_level,
			&helloworld_main,
			&mrm_class)
	!= MrmSUCCESS)
	XtError ("can't fetch interface\n");

    /*
     *  Make the toplevel widget "manage" the main window (or whatever the
     *  the uil defines as the topmost widget).  This will
     *  cause it to be "realized" when the toplevel widget is "realized"
     */

    XtManageChild(helloworld_main);
    
    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed
     */

    XtRealizeWidget(top_level);

    /*
     *  Loop and process events
     */

    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}


static void 
Activate(
        Widget	widget,
	XtPointer tag,
	XtPointer data)

{
    static Boolean first_time = True ;

    /* First click: change the pushbutton label and recenter */
    if (first_time) {
	XmString new_button_label = XmStringGenerate(NEW_BUTTON_LABEL,
						     NULL, 
						     XmCHARSET_TEXT, NULL);
	XtVaSetValues(widget, 
		      XmNx, NEW_BUTTON_X,
		      XmNlabelString, new_button_label,
		      NULL);

	XmStringFree(new_button_label);
	first_time = False ;
    } else {
	exit(0);
    }
}

 
