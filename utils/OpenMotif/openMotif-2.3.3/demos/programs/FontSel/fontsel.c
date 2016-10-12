/*
 * Copyright 1994, Integrated Computer Solutions, Inc.
 *
 * All Rights Reserved.
 *
 * Author: Rick Umali
 *
 * fontsel.c
 *
 */

/**************************************************************
 *		INCLUDE FILES
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fontsel.h"

#include <Xm/Xm.h>
#include <Xm/ToggleB.h>
#include <Xm/FontS.h>

/**************************************************************
 *		GLOBALS
 **************************************************************/
Widget G_font = NULL;

/**************************************************************
 *		FORWARD DECLARATIONS
 **************************************************************/
Widget CreateDemoForm(Widget);

/**************************************************************
 *		DEFINES
 **************************************************************/

/**************************************************************
 *		FALLBACKS
 **************************************************************/
static String fallbacks[] = {
    /*
     * General fallback resources.
     */
    "*fontList: -*-helvetica-medium-r-*-*-*-140-*-*-*-*-*-*",
    "*hype_label*fontList: -*-helvetica-bold-r-*-*-*-140-*-*-*-*-*-*",
    "*hype_label.marginWidth: 10",
    "*hype_label.marginHeight: 10",
    "*show_font_tog.labelString: Show Font on Widget",
    "*explain_showfont.labelString: Explain...",
    "*cur_font_pb.labelString: Show Current Font...",
    "*explain_curfont.labelString: Explain...",
    "*other_pb.labelString: Other Resources...",
    "*quit_pb.labelString: Quit",
    "*resform*rgbFileLabel.labelString: XmNrgbFile",
    
    NULL,
};
/**************************************************************
 *		PUBLIC (GLOBAL) CODE
 **************************************************************/

/*
 * Function Name: InitializeDemoForm
 * Description:   
 * Arguments:     form - 
 * Returns:       nothing
 *
 */
void 
InitializeDemoForm(Widget form)
{
    Widget w;

    w = XtNameToWidget(form, "*show_font_tog");
    if (w == NULL) {
	fprintf(stderr, "InitializeDemoForm: cannot find show_font_tog\n");
	exit(1);
    }

    XmToggleButtonSetState(w, False, False);

    /* Set the global variable */
    G_font = XtNameToWidget(form, "*fontsel");
    if (G_font == NULL) {
	fprintf(stderr, "InitializeDemoForm: cannot find fontsel\n");
	exit(1);
    }
}

/*
 * Function Name: main
 * Description:   
 * Arguments:     the usual suspects
 * Returns:       nothing
 *
 */
int
main(argc, argv)
int argc;
char **argv;
{
    Arg args[5];
    Cardinal argcnt;
    Widget top, demowindow;
    XtAppContext app;

    argcnt = 0;
    XtSetArg(args[argcnt], XmNtitle, "Font Selector Demo"); argcnt++;
    XtSetArg(args[argcnt], XmNiconName, "Font Selector Demo"); argcnt++;
    XtSetArg(args[argcnt], XmNallowShellResize, True); argcnt++;
    top = XtAppInitialize(&app, "ColorSelector", NULL, 0,
			  &argc, argv, fallbacks, args, argcnt);

    demowindow = CreateDemoForm(top);
    XtManageChild(demowindow);

    InitializeDemoForm(demowindow);

    XtRealizeWidget(top);

    XtAppMainLoop(app);
}

