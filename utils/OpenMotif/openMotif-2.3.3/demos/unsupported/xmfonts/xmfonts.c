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
static char rcsid[] = "$XConsortium: xmfonts.c /main/5 1995/07/14 10:08:26 drk $"
#endif
#endif
/*
*  (c) Copyright 1989 HEWLETT-PACKARD COMPANY. */

/**---------------------------------------------------------------------
***	
***	file:		xmfonts.c
***
***
Xmfonts.ad:

  xmfonts*XmScrolledWindow.height:	432
  xmfonts*XmScrolledWindow.width:	390
  xmfonts*row_column.XmNnumColumns:	5
  xmfonts*row_column.XmNpacking:        XmPACK_COLUMN

***-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>


/*-------------------------------------------------------------
**	Forward Declarations
*/

Widget CreateApplication ();	/*  create main window		*/
Widget CreateFontSample ();	/*  create font display window	*/
Widget CreateHelp ();		/*  create help window		*/

void SelectFontCB ();		/*  callback for font buttons	*/
void CloseCB ();		/*  callback for close button	*/
void HelpCB ();			/*  callback for help button	*/
void QuitCB ();			/*  callback for quit button	*/



/*-------------------------------------------------------------
**	Global Variables
*/

#define MAX_ARGS 20

static XmStringCharSet	charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;


typedef struct {
  char *fontpattern;
  int   maxfont ;    /* number max of font to be displayed */
  int   maxlen ;     /* max length of font name to be displayed */
  Boolean   usegadget ;  
} ApplicationData, *ApplicationDataPtr;

ApplicationData AppData;

#define XtNfontPattern "fontPattern"
#define XtCFontPattern "FontPattern"
#define XtNmaxFont     "maxFont"
#define XtCMaxFont     "MaxFont"
#define XtNmaxLen      "maxLen"
#define XtCMaxLen      "MaxLen"
#define XtNuseGadget   "useGadget"
#define XtCUseGadget   "UseGadget"

static XtResource resources[] = {
  { XtNfontPattern, XtCFontPattern, XmRString, sizeof(String),
    XtOffset(ApplicationDataPtr, fontpattern), XmRString, "*" },
  { XtNmaxFont, XtCMaxFont, XmRInt, sizeof(int),
    XtOffset(ApplicationDataPtr, maxfont), XmRImmediate, (caddr_t) 1000 },
  { XtNmaxLen, XtCMaxLen, XmRInt, sizeof(int),
    XtOffset(ApplicationDataPtr, maxlen), XmRImmediate, (caddr_t) 10 },
  { XtNuseGadget, XtCUseGadget, XmRBoolean, sizeof (Boolean),
    XtOffset(ApplicationDataPtr, usegadget), XmRImmediate, (caddr_t) TRUE }
};

/*-------------------------------------------------------------
**	main		- main logic for application
*/
int main (argc,argv) 
	int		argc;
	char 		**argv;
{
	Display		*display;	/*  Display		*/
	Widget		app_shell;	/*  ApplicationShell	*/
	Widget		main_window;	/*  MainWindow		*/
	XtAppContext	app_context;

	/*	Initialize toolkit and open the display.
	*/
	app_shell = XtAppInitialize(&app_context, "XMdemos", NULL, 0,
			    &argc, argv, NULL, NULL, 0);


	XtGetApplicationResources(app_shell,
			      (XtPointer)&AppData,
			      resources,
			      XtNumber(resources),
			      NULL,
			      0);

	main_window = CreateApplication (app_shell);
	XtRealizeWidget (app_shell);

	/*	Get and dispatch events.
	*/
	XtAppMainLoop (app_context);

	return 0;    /* make compiler happy */
}



/*-------------------------------------------------------------
**	CreateApplication	- create main window
*/
Widget CreateApplication (parent) 
Widget		parent;		/*  parent widget	*/
{
	Widget		main_window;	/*  MainWindow		*/
	Widget		menu_bar;	/*  MenuBar		*/
	Widget		menu_pane;	/*  MenuPane		*/
	Widget		cascade;	/*  CascadeButton	*/
	Widget		frame;		/*  Frame		*/
	Widget		swindow;	/*  ScrolledWindow	*/
	Widget		row_column;	/*  RowColumn		*/
	Widget		button;		/*  PushButtonGadget	*/
	Widget		hsb, vsb;	/*  ScrollBars		*/

	Arg		args[MAX_ARGS];	/*  arg list		*/
	register int	n;		/*  arg count		*/

	char *	name;	
	int count, i  ;
	char ** fontnamelist ;
	
	XmString	label_string;


	/*	Create MainWindow.
	*/
	n = 0;
	main_window = XmCreateMainWindow (parent, "main_window", args, n);
	XtManageChild (main_window);

	/*	Create MenuBar in MainWindow.
	*/
	n = 0;
	menu_bar = XmCreateMenuBar (main_window, "menu_bar", args, n); 
	XtManageChild (menu_bar);

	/*	Create "Exit" PulldownMenu.
	*/
	n = 0;
	menu_pane = XmCreatePulldownMenu (menu_bar, "menu_pane", args, n);

	n = 0;
	button = XmCreatePushButton (menu_pane, "Quit", args, n);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, QuitCB, NULL);

	n = 0;
	XtSetArg (args[n], XmNsubMenuId, menu_pane);  n++;
	cascade = XmCreateCascadeButton (menu_bar, "Exit", args, n);
	XtManageChild (cascade);


	/*	Create "Help" button.
	*/
	n = 0;
	cascade = XmCreateCascadeButton (menu_bar, "Help", args, n);
	XtManageChild (cascade);
	XtAddCallback (cascade, XmNactivateCallback, HelpCB, NULL);

	n = 0;
	XtSetArg (args[n], XmNmenuHelpWidget, cascade);  n++;
	XtSetValues (menu_bar, args, n);


	/*	Create Frame MainWindow and ScrolledWindow in Frame.
	*/
	n = 0;
	frame = XmCreateFrame (main_window, "frame", args, n);
	XtManageChild (frame);

	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC);  n++;
	swindow = XmCreateScrolledWindow (frame, "swindow", args, n);
	XtManageChild (swindow);


	/*	Create RowColumn in ScrolledWindow to manage buttons.
	*/
	n = 0;

	row_column = XmCreateRowColumn (swindow, "row_column", args, n);
	XtManageChild (row_column);


	/*	Set MainWindow areas and add tab groups
	*/
	XmMainWindowSetAreas (main_window, menu_bar, NULL, NULL, NULL,
			frame);

	/*	Create a PushButton widget for each font.
		Get the font list from the server and keep
		only the font name whose length is less than 
		the given maxLen resource.
	*/
	
	fontnamelist = XListFonts(XtDisplay(main_window), 
				  AppData.fontpattern, 
				  AppData.maxfont, &count);
	if (!fontnamelist) {
	    fprintf(stderr,"No font on your server, exit\n");
	    exit(0);
	}

	/*  read one entry each time through the loop  */
	for (i = 0;  i < count;  i++ ) 
	{
		name = fontnamelist[i] ;
		if (strlen(name) > AppData.maxlen) continue ;

		/*  create PushButton in RowCol  */
		n = 0;
		label_string = XmStringCreateLtoR(name, charset);
		XtSetArg (args[n], XmNlabelString, label_string);  n++;
		if (AppData.usegadget) 
		    button = XmCreatePushButtonGadget (row_column, name, 
						       args, n);
		else 
		    button = XmCreatePushButton(row_column, name, 
						args, n);
		XtManageChild (button);
		XtAddCallback (button, XmNarmCallback, SelectFontCB, NULL);
		XmStringFree (label_string);
	}

	/*	Return MainWindow.
	*/

	XFreeFontNames(fontnamelist);
	return (main_window);
}



/*-------------------------------------------------------------
**	CreateFontSample	- create font display window
*/
Widget CreateFontSample (parent) 
Widget		parent;		/*  parent widget	*/
{
	Widget		message_box;		/*  MessageBox Dialog	*/
	Widget		button;
	Arg		args[MAX_ARGS];		/*  arg list		*/
	register int	n;			/*  arg count		*/
	
	char		*name = NULL;		/*  font name		*/
	XFontStruct	*font = NULL;		/*  font pointer	*/
	XmFontList	fontlist = NULL;	/*  fontlist pointer	*/
	static char	message[BUFSIZ];	/*  text sample		*/
	XmString	name_string = NULL;
	XmString	message_string = NULL;
	XmString	button_string = NULL;

	
	/*	Get font name.
	*/
	n = 0;
	XtSetArg (args[n], XmNlabelString, &name_string); n++;
	XtGetValues (parent, args, n);
	XmStringGetLtoR (name_string, charset, &name);


	/*	Load font and generate message to display. */
	if (name)
		font = XLoadQueryFont (XtDisplay (XtParent (parent)), name);
	if  (!font)
		sprintf (message, "Unable to load font: %s", name);
	else
	{
		fontlist = XmFontListCreate (font, charset);
		sprintf (message, "\
This is font %s.\n\
The quick brown fox jumps over the lazy dog.", name);
	}
	message_string = XmStringCreateLtoR (message, charset);
	button_string = XmStringCreateLtoR ("Close", charset);


	/*	Create MessageBox dialog.
	*/
	n = 0;
	XtSetArg (args[n], XmNdialogTitle, name_string);  n++;
	XtSetArg (args[n], XmNokLabelString, button_string);  n++;
	XtSetArg (args[n], XmNmessageString, message_string);  n++;
	message_box = XmCreateMessageDialog (XtParent (XtParent(parent)), 
					     "fontbox",
					     args, n);

	button = XmMessageBoxGetChild (message_box, XmDIALOG_MESSAGE_LABEL);
	if (fontlist) 
	{
		n = 0;
		XtSetArg (args[n], XmNfontList, fontlist);  n++;
		XtSetValues(button, args, n);
	}
	
	button = XmMessageBoxGetChild (message_box, XmDIALOG_CANCEL_BUTTON);
	XtUnmanageChild (button);
	button = XmMessageBoxGetChild (message_box, XmDIALOG_HELP_BUTTON);
	XtUnmanageChild (button);

	/*	Free strings and return MessageBox.
	*/
	if (fontlist) XmFontListFree (fontlist);
	if (name_string) XmStringFree (name_string);
	if (message_string) XmStringFree (message_string);
	if (button_string) XmStringFree (button_string);
	return (message_box);
}



/*-------------------------------------------------------------
**	CreateHelp		- create help window
*/
Widget CreateHelp (parent) 
	Widget		parent;		/*  parent widget	*/
{
	Widget		button;
	Widget		message_box;	/*  Message Dialog 	*/
	Arg		args[MAX_ARGS];	/*  arg list		*/
	register int	n;		/*  arg count		*/

	static char	message[BUFSIZ];	/*  help text	*/
	XmString	title_string = NULL;
	XmString	message_string = NULL;
	XmString	button_string = NULL;



	/*	Generate message to display.
	*/
	sprintf (message, "\
These are buttons for the fonts in your X11 server whose  \n\
name lengths are below the given maxLen application resource.\n\
The button label is the name of the font.  When you select \n\
a button, a small window will display a sample of the font.  \n\n\
Press the 'close' button to close a font window.  \n\
Select 'quit' from the 'exit' menu to exit this application.\0");
	message_string = XmStringCreateLtoR (message, charset);
	button_string = XmStringCreateLtoR ("Close", charset);
	title_string = XmStringCreateLtoR ("xmfonts help", charset);


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



/*-------------------------------------------------------------
**	SelectFontCB		- callback for font buttons
*/
void SelectFontCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	Widget		message_box;


	/*	Create font sample window.
	*/
	message_box = CreateFontSample (w);


	/*	Display font sample window.
	*/
	XtManageChild (message_box);
}



/*-------------------------------------------------------------
**	CloseCB			- callback for close button
*/
void CloseCB  (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  font pointer	*/
caddr_t		call_data;	/*  data from widget class  */
{
	XFontStruct	*font	= (XFontStruct *) client_data;
	Widget		message_box	= XtParent (w);
	Widget		shell	= XtParent (message_box);


	/*	Free font.
	*/
	if (font) XFreeFont (XtDisplay (w), font);


	/*	Unmanage and destroy widgets.
	*/
	XtUnmanageChild (message_box);
	XtDestroyWidget (shell);
}



/*-------------------------------------------------------------
**	HelpCB			- callback for help button
*/
void HelpCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from application   */
caddr_t		call_data;	/*  data from widget class  */
{
	Widget		message_box;		/*  MessageBox		*/


	/*	Create help window.
	*/
	message_box = CreateHelp (w);


	/*	Display help window.
	*/
	XtManageChild (message_box);
}



/*-------------------------------------------------------------
**	QuitCB			- callback for quit button
*/
void QuitCB (w, client_data, call_data) 
Widget		w;		/*  widget id		*/
caddr_t		client_data;	/*  data from applicaiton   */
caddr_t		call_data;	/*  data from widget class  */
{


	/*	Terminate the application.
	*/
	exit (0);
}
