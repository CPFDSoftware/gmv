/* $XConsortium: hellomotif.c /main/5 1995/07/15 20:47:36 drk $ */
/*
 * @OPENGROUP_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * Copyright (c) 1996, 1997, 1998, 1999, 2000 The Open Group
 * ALL RIGHTS RESERVED (MOTIF).  See the file named COPYRIGHT.MOTIF for
 * the full copyright text.
 * 
 * This software is subject to an open license. It may only be
 * used on, with or for operating systems which are themselves open
 * source systems. You must contact The Open Group for a license
 * allowing distribution and sublicensing of this software on, with,
 * or for operating systems which are not Open Source programs.
 * 
 * See http://www.opengroup.org/openmotif/license for full
 * details of the license agreement. Any use, reproduction, or
 * distribution of the program constitutes recipient's acceptance of
 * this agreement.
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, THE PROGRAM IS
 * PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 * WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY
 * OR FITNESS FOR A PARTICULAR PURPOSE
 * 
 * EXCEPT AS EXPRESSLY SET FORTH IN THIS AGREEMENT, NEITHER RECIPIENT
 * NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OR DISTRIBUTION OF THE PROGRAM OR THE
 * EXERCISE OF ANY RIGHTS GRANTED HEREUNDER, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 * 
 */
/*
 * HISTORY
 */
#include <Xm/XmAll.h>        

#define APP_CLASS "XmdHelloMotif"

/* Although it's really bad to hardcode Positions, keep this for
   a close hellomotif_in_c equivalence to the uil version */
#define NEW_BUTTON_X 11
/* not mentioning hardcoding labels... */
#define NEW_BUTTON_LABEL  "Goodbye\nWorld!"


static void Activate(
        Widget	widget,
	XtPointer tag,
	XtPointer data);


/*
 * Fallbacks: font, label and position go together in this demo 
 */
static String fallbacks[] = {
"hellomotif_in_c.allowShellResize: True",
"hellomotif_in_c*fontList: fixed",
"*helloworld_label.labelString:Press button once\\nto change label;\\ntwice to exit.",
"*helloworld_button.x: 15",
"*helloworld_button.y: 60",
"*helloworld_button.labelString:Hello\nWorld!",
NULL
};


/*
 *  Main program: create a shell, a pure bb with a label and a button 
 *                underneath it. Attach a callback to the button.
 */
int main(int argc, char *argv[])
{
    XtAppContext        app_context;
    Widget              top_level, helloworld_main,
                        helloworld_label, helloworld_button ;

    /* 
     * Initialize Xt and create a resizable shell 
     */

    top_level = XtVaAppInitialize(&app_context, APP_CLASS, 
				  NULL, 0, &argc, argv, fallbacks, NULL); 

    /*
     *  Create the widget tree: first the bb parent.
     */

    helloworld_main = XtVaCreateManagedWidget ("helloworld_main",
					       xmBulletinBoardWidgetClass,
					       top_level, NULL);

    /* the label */
    helloworld_label = XtVaCreateManagedWidget ("helloworld_label",
						xmLabelWidgetClass,
						helloworld_main, 
						NULL);

    /* the button underneath the label */
    helloworld_button = XtVaCreateManagedWidget ("helloworld_button",
						 xmPushButtonWidgetClass,
						 helloworld_main,
						 NULL);
    /* add the callback to change the button label */
    XtAddCallback(helloworld_button, XmNactivateCallback, Activate, NULL);

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
