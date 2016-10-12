/* $XConsortium: xmapdef.c /main/4 1995/07/15 20:47:41 drk $ */
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
/*
**  This demo shows an APPLICATION_DEFINED scrolled window.
**
**  It's a file viewer that uses a FSB for filename input and
**  a viewport with a vertical scrollbar to see the file.
**  The file is shown using the per screen font resource.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <Xm/XmAll.h>

/*-------------------------------------------------------------
**	Forwarded functions
*/
void CreateApplication ();	
void WarnUser ();	
Widget CreateHelp ();		

/*      Xt callbacks 
*/
void DrawCB ();			
void ValueCB ();			
void OpenCB ();			
void ReadCB ();			
void QuitCB ();			
void HelpCB ();			

/*-------------------------------------------------------------
**      File i/o and drawing stuff
*/
void InitFile ();			
Boolean BuildLineTable ();			
void ReDraw ();			
void ReSize ();			

typedef struct {
    Widget work_area  ;
    Widget v_scrb ;
    String file_name ;
    XFontStruct * font_struct ;
    GC draw_gc ;
    char ** lines ;
    int num_lines ;
} FileData ;


/*-------------------------------------------------------------
**	    Main body
*/
int main(argc, argv) 
int argc; char **argv;
{
    XtAppContext app_context;
    Widget      toplevel ;
    FileData    filedata ;
    
    toplevel = XtAppInitialize(&app_context, "XMdemos", NULL, 0,
			       &argc, argv, NULL, NULL, 0);

    InitFile(toplevel, &filedata, argc, argv);

    CreateApplication (toplevel, &filedata);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);

    return 0;    /* make compiler happy */
}


/*-------------------------------------------------------------
**	Create a app_defined Main Window with a Menubar to load a file
**      Add the vertical scrollbar and the workarea to filedata.
*/
void CreateApplication (parent, filedata) 
Widget		parent;	
FileData *       filedata;
{
    Widget main_window, menu_bar, menu_pane, cascade, 
           button ;
    Arg args[5];	
    int	n ;		   


    /*	Create app_defined MainWindow.
     */
    n = 0;
    XtSetArg (args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED);  n++;
    main_window = XmCreateMainWindow (parent, "main_window", args, n);
    XtManageChild (main_window);


    /*	Create MenuBar in MainWindow.
     */
    n = 0;
    menu_bar = XmCreateMenuBar (main_window, "menu_bar", args, n); 
    XtManageChild (menu_bar);


    /*	Create "File" PulldownMenu with Load and Quit buttons
     */
    n = 0;
    menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", args, n);

    n = 0;
    button = XmCreatePushButton (menu_pane, "Open...", args, n);
    XtManageChild (button);
    /* pass the graphic id to the save function */
    XtAddCallback (button, XmNactivateCallback, OpenCB, (XtPointer)filedata);
    n = 0;
    button = XmCreatePushButton (menu_pane, "Quit", args, n);
    XtManageChild (button);
    XtAddCallback (button, XmNactivateCallback, QuitCB, NULL);

    n = 0;
    XtSetArg (args[n], XmNsubMenuId, menu_pane);  n++;
    cascade = XmCreateCascadeButton (menu_bar, "File", args, n);
    XtManageChild (cascade);


    /*	Create "Help" PulldownMenu with Help button.
     */
    n = 0;
    menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", args, n);

    n = 0;
    button = XmCreatePushButton (menu_pane, "Help", args, n);
    XtManageChild (button);
    XtAddCallback (button, XmNactivateCallback, HelpCB, NULL);

    n = 0;
    XtSetArg (args[n], XmNsubMenuId, menu_pane);  n++;
    cascade = XmCreateCascadeButton (menu_bar, "Help", args, n);
    XtManageChild (cascade);

    n = 0;
    XtSetArg (args[n], XmNmenuHelpWidget, cascade);  n++;
    XtSetValues (menu_bar, args, n);


    /*	Create vertical scrollbar only
     */
     n = 0;
    XtSetArg (args[n], XmNorientation, XmVERTICAL);  n++;
    filedata->v_scrb = XmCreateScrollBar (main_window, "v_scrb", args, n);
    XtAddCallback (filedata->v_scrb, XmNvalueChangedCallback, ValueCB, 
		   (XtPointer)filedata);
    XtManageChild (filedata->v_scrb);


    /*	Create work_area in MainWindow 
     */
    n = 0;
    filedata->work_area = XmCreateDrawingArea(main_window, "work_area", args, n);
    XtAddCallback (filedata->work_area, XmNexposeCallback, DrawCB, 
		   (XtPointer)filedata);
    XtAddCallback (filedata->work_area, XmNresizeCallback, DrawCB, 
		   (XtPointer)filedata);
    XtManageChild (filedata->work_area);


    /*	Set MainWindow areas 
     */
    XmMainWindowSetAreas (main_window, menu_bar, NULL, NULL, 
			  filedata->v_scrb,
			  filedata->work_area);

}

/*-------------------------------------------------------------
**	OpenCB			- callback for Open button
*/
void OpenCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	static Widget fsb_box = NULL ;

	if (!fsb_box) {
	    fsb_box = XmCreateFileSelectionDialog (w, "Load file", 
						   NULL, 0);
	    /* just propagate the graphic information */
	    XtAddCallback (fsb_box, XmNokCallback, ReadCB, client_data);
	}    

	XtManageChild (fsb_box);
}


/*-------------------------------------------------------------
**	QuitCB			- callback for quit button
*/
void QuitCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from applicaiton   */
caddr_t		call_data;	/*  data from widget class  */
{
    exit (0);
}


/*-------------------------------------------------------------
**	HelpCB			- callback for help button
*/
void HelpCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	static Widget message_box = NULL ;

	if (!message_box) message_box = CreateHelp (w);

	XtManageChild (message_box);
}



/*-------------------------------------------------------------
**	ReadCB	- callback for fsb activate
*/
void ReadCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
    FileData * filedata = (FileData *) client_data ;
    String file_name ;
    Arg args[5];	
    int	n, slider_size ;	
    Dimension height ;

    file_name = XmTextGetString(
		 XmFileSelectionBoxGetChild(w, XmDIALOG_TEXT)) ;

    if (!BuildLineTable(filedata, file_name)) {
	WarnUser (w, "Cannot open %s\n", file_name);
    } else {
	filedata->file_name = file_name ;

	/* ok, we have a new file, so reset some values */
	n = 0;
	XtSetArg (args[n], XmNheight, &height);  n++;
	XtGetValues (filedata->work_area, args, n);

	slider_size = (height - 4) / (filedata->font_struct->ascent
				      + filedata->font_struct->descent) ;
	if (slider_size <= 0) slider_size = 1 ;
	if (slider_size > filedata->num_lines) 
	    slider_size = filedata->num_lines ;

	n = 0 ;
	XtSetArg (args[n], XmNsliderSize, slider_size);  n++;
	XtSetArg (args[n], XmNmaximum, filedata->num_lines);  n++;
	XtSetArg (args[n], XmNvalue, 0);  n++;
	XtSetValues (filedata->v_scrb, args, n);

	/* clear and redraw */
	XClearWindow(XtDisplay(filedata->work_area),
		     XtWindow(filedata->work_area));
	ReDraw (filedata);
    }
}

/*-------------------------------------------------------------
**	ValueCB		- callback for scrollbar
*/
void ValueCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
    FileData * filedata = (FileData *) client_data ;

    /* clear and redraw, dumb dumb.. */
    XClearWindow(XtDisplay(filedata->work_area),
		 XtWindow(filedata->work_area));
    ReDraw(filedata);
}



/*-------------------------------------------------------------
**	DrawCB			- callback for drawing area
*/
void DrawCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
    
    XmDrawingAreaCallbackStruct * dacs =
	(XmDrawingAreaCallbackStruct *) call_data ;
    FileData * filedata = (FileData *) client_data ;
    XSetWindowAttributes xswa;
    
    static Boolean first_time = True ;

    switch (dacs->reason) {
    case XmCR_EXPOSE: 
	if (first_time) {
	    /* Change once the bit gravity of the Drawing Area; default
	       is north west and we want forget, so that resize 
	       always generates exposure events */
	    first_time = False ;
	    xswa.bit_gravity = ForgetGravity ;
	    XChangeWindowAttributes(XtDisplay(w), XtWindow(w),
				    CWBitGravity, &xswa);
	}

	ReDraw(filedata) ;

	break ;
    case XmCR_RESIZE: 
	ReSize(filedata) ;

	break ;
    }
}

/*-------------------------------------------------------------
**	CreateHelp		- create help window
*/
Widget CreateHelp (parent) 
	Widget		parent;		/*  parent widget	*/
{
	Widget		button;
	Widget		message_box;	/*  Message Dialog 	*/
	Arg		args[20];	/*  arg list		*/
	register int	n;		/*  arg count		*/

	static char	message[1000];	/*  help text	*/
	XmString	title_string = NULL;
	XmString	message_string = NULL;
	XmString	button_string = NULL;

	/*	Generate message to display.
	*/
	sprintf (message, "\
Use the Open button in the File menu to load a new file.\n\n\
Use the ScrollBar to scroll and the window manager to resize the main\n\
window and see the slider size change.\n\n\
You can specify which font to display the test using the\n\
XmNfont screen resource.");

	message_string = XmStringCreateLtoR (message, 
					     XmSTRING_DEFAULT_CHARSET);
	button_string = XmStringCreateLtoR ("Close", 
					    XmSTRING_DEFAULT_CHARSET);
	title_string = XmStringCreateLtoR ("General Help", 
					   XmSTRING_DEFAULT_CHARSET);


	/*	Create MessageBox dialog.
	*/
	n = 0;
	XtSetArg (args[n], XmNdialogTitle, title_string);  n++;
	XtSetArg (args[n], XmNokLabelString, button_string);  n++;
	XtSetArg (args[n], XmNmessageString, message_string);  n++;
	message_box = XmCreateMessageDialog (parent, "helpbox", args, n);

	button = XmMessageBoxGetChild (message_box, XmDIALOG_CANCEL_BUTTON);
	XtUnmanageChild (button);
	button = XmMessageBoxGetChild (message_box, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild (button);


	/*	Free strings and return MessageBox.
	*/
	if (title_string) XmStringFree (title_string);
	if (message_string) XmStringFree (message_string);
	if (button_string) XmStringFree (button_string);

	return (message_box);
}

void WarnUser (widget, format, name)
Widget widget ;
String format ;
String name ;
{
    /* better ui needed */
    printf(format, name);
}
	

/*************************** FILE STUFF **********************************/

void InitFile(widget, filedata, argc, argv)
Widget		widget;	
FileData *       filedata;
int argc;     char **argv;
{
    Arg args[5];	
    int	n ;		   
    XGCValues val ;
    
    filedata->lines = NULL ;
    filedata->num_lines = 0 ;

    /* get the X font from the screen object and make a gc */
    n = 0;

/* do not use the screen object routine in 1.1 or lesser */
#if XmREVISION < 2
    filedata->font_struct = XLoadQueryFont(XtDisplay(widget), "fixed");
#else
    XtSetArg (args[n], XmNfont, &(filedata->font_struct));  n++;
    XtGetValues (XmGetXmScreen(XDefaultScreenOfDisplay(XtDisplay(widget))), 
		 args, n); 
#endif
    val.font = filedata->font_struct->fid ;
    filedata->draw_gc = XtGetGC(widget, GCFont, &val);

    if (argc == 2) {
	if (BuildLineTable(filedata, argv[1])) {
	    filedata->file_name = argv[1] ;
	} else {
	    filedata->file_name = "" ;
	    WarnUser(widget, "Cannot open %s\n", argv[1]);
	}
    } else {
	filedata->file_name = "" ;
    }
}


Boolean BuildLineTable(filedata, file_name)
FileData * filedata ;
String file_name ;
{
    FILE  *in_file ;
    char linebuff[256] ;

    if ((in_file = fopen(file_name, "r")) == NULL) {
	return False ;
    } else {
	/* free the current data */
	if (filedata->num_lines) {
	    while (filedata->num_lines-- >= 0) {
		XtFree(filedata->lines[filedata->num_lines]) ;
	    }
	    XtFree ((char*)filedata->lines) ;
	    filedata->lines = NULL ;
	    filedata->num_lines = 0 ;
	}
	/* allocate and fill new data */
	while (fgets (linebuff, 256, in_file)) {
	    filedata->num_lines ++ ;
	    /* better fragmentation needed... */
	    filedata->lines = (char**) XtRealloc((char*)filedata->lines, 
					filedata->num_lines * sizeof(char*)) ;
	    filedata->lines[filedata->num_lines-1] = XtNewString(linebuff);
	} 
	return True ;
    }
}

void ReDraw(filedata)
FileData * filedata ;
{
    /* Display as many line as slider_size actually shows, since
       slider_size is computed relative to the work_area height */

    Cardinal i ;
    int value, slider_size ;
    Arg args[5];	
    int	n ;	
    Position y ;

    if (filedata->num_lines == 0) return ;

    n = 0;
    XtSetArg (args[n], XmNvalue, &value);  n++;
    XtSetArg (args[n], XmNsliderSize, &slider_size);  n++;
    XtGetValues (filedata->v_scrb, args, n);
	
    for (i = value, y = 2 + filedata->font_struct->ascent; 
	 i < value + slider_size ; 
	 i++, y += (filedata->font_struct->ascent
		    + filedata->font_struct->descent)) {
	XDrawString(XtDisplay(filedata->work_area),
		    XtWindow(filedata->work_area),
		    filedata->draw_gc,
		    4, y,
		    filedata->lines[i], strlen(filedata->lines[i]));
    }
}

void ReSize(filedata)
FileData * filedata ;
{
    /* Just update the scrollbar internals here, don't bother to redisplay
       since the gravity is none */

    Arg args[5];	
    int	n ;	
    int value, slider_size ;
    Dimension height ;

    if (filedata->num_lines == 0) return ;

    n = 0;
    XtSetArg (args[n], XmNheight, &height);  n++;
    XtGetValues (filedata->work_area, args, n);
	
    /* sliderSize is the number of visible lines */
    slider_size = (height - 4) / (filedata->font_struct->ascent
				  + filedata->font_struct->descent) ;
    if (slider_size <= 0) slider_size = 1 ;
    if (slider_size > filedata->num_lines) 
	slider_size = filedata->num_lines ;

    n = 0;
    XtSetArg (args[n], XmNvalue, &value);  n++;
    XtGetValues (filedata->v_scrb, args, n);

    /* value shouldn't change that often but there are cases
       where it matters */
    if (value > filedata->num_lines - slider_size) 
	value = filedata->num_lines - slider_size;

    n = 0;
    XtSetArg (args[n], XmNsliderSize, slider_size);  n++;
    XtSetArg (args[n], XmNvalue, value);  n++;
    XtSetArg (args[n], XmNmaximum, filedata->num_lines);  n++;
    XtSetValues (filedata->v_scrb, args, n);
}

