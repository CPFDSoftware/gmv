/* $XConsortium: helloint.c /main/5 1995/07/15 20:45:32 drk $ */
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
#include <stdlib.h>		/* for getenv() */
#include <Xm/Xm.h>              /* Motif Toolkit */
#include <Mrm/MrmPublic.h>      /* Mrm Toolkit */

#define APP_CLASS "XmdHelloInt"

static void Activate(Widget, XtPointer,	XtPointer);

/* 
 * MRM database file list 
 */
static String uid_vec[]={"helloint.uid","l_strings.uid"}; 
/* 
 * Callback bindings
 */
static MrmRegisterArg reg_vec[] = {       
  {"helloworld_button_activate",(XtPointer)Activate}
};

/*
 * MRM Hierachy
 */
static MrmHierarchy s_mrm_hierarchy;		


/****************************************************************
 *
 *  Main program. Fetch UIL interface description and enter 
 *  event loop.
 *
 ****************************************************************/
int main(int argc, 
	 char *argv[])
{
  XtAppContext app_context;
  Widget       top_level, helloworld_main;
  MrmCode      mrm_class;
  char       * applresdir;

  /*
   *  This demo requires XAPPLRESDIR to be set to '.'. Mrm
   *  will only report that l_strings.uid could not be found,
   *  so, to be a little more friendly, we help the user. This
   *  it not a check that normally would be done, since the uid
   *  files would be installed in the appropriate directory.
   */

  if ((applresdir = (char *)getenv("XAPPLRESDIR")) == NULL ||
      strcmp(applresdir, ".") != 0) {
    XtError("Environment variable XAPPLRESDIR must be set to \".\"\n");
  }

  /*
   *  Set up language environment.
   */
  XtSetLanguageProc(NULL, NULL, NULL);
  
  /*
   *  Initialize MRM
   */
  
  MrmInitialize ();
  
  /* 
   * Initialize Xt and create a resizable shell 
   */
  
  top_level = XtVaAppInitialize(&app_context, "XmdHelloInt", 
				NULL, 0, &argc, argv, NULL, 
				XmNallowShellResize, True, NULL); 

  /*
   * Create the Mrm hierarchy
   */
  
  if (MrmOpenHierarchy (XtNumber(uid_vec),	    /* number of files	    */
			uid_vec, 		    /* files     	    */
			NULL,			    /* os_ext_list (null)   */
			&s_mrm_hierarchy)	    /* ptr to returned id   */
      != MrmSUCCESS) {
    XtError ("can't open hierarchy\n");
  }
  
  /*
   * Register our callback routines so that the resource manager can 
   * resolve them at widget-creation time.
   */

  if (MrmRegisterNames (reg_vec, XtNumber(reg_vec)) != MrmSUCCESS) {
    XtError("can't register names\n");
  }

  /*
   *  Call MRM to fetch and create the widgets 
   */
  
  if (MrmFetchWidget (s_mrm_hierarchy, 
		      "helloworld_main", 
		      top_level,
		      &helloworld_main, 
		      &mrm_class) != MrmSUCCESS) {
    XtError ("can't fetch interface\n");
  }
  
  /*
   * Manage the widget returned by Mrm.
   */

  XtManageChild(helloworld_main);

  /*
   * Realize the widget tree.
   */
  XtRealizeWidget(top_level);
   
  /*
   *  Loop and process events
   */
  
  XtAppMainLoop(app_context);

  /* UNREACHABLE */
  return (0);
}


static void Activate(Widget    widget,
		     XtPointer client_data,
		     XtPointer callback_data)
{
  static Boolean first_time = True;
  Arg arglist[1];
  
  if (first_time) {
    /*
     * Get "Goodbye World!" in the language of choice and
     * set the label string of widget to this string.
     */
    XtSetArg(arglist[0], XmNlabelString, "bye_label");
    MrmFetchSetValues(s_mrm_hierarchy, widget, arglist, 1);
    first_time = False;
  } else {
    /*
     * We're done...
     */
    exit(0);
  }
}
