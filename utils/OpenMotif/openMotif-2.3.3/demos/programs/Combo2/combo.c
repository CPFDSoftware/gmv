/* simple test program */

#include <Xm/Xm.h>
#include <Xm/DropDown.h>
#include <Xm/XmAll.h>
#include <stdio.h>

static int done = 0;

static void quitCB(Widget w, XtPointer client, XtPointer call) 
{
	done = 1;
}

/* note! using names of the widgets as equivalent to the labels */
static char * values[] = {
	"Mercury", "Venus",
	"Earth", "Mars",
	"Jupiter", "Saturn",
	"Uranus", "Neptune", 
	"Pluto",
};

static void createQuit(Widget quit_parent)
{
	Widget button = XmCreatePushButton(quit_parent, "quit", NULL, 0);
	XtManageChild(button);
	XtAddCallback(button, XmNactivateCallback, quitCB, (XtPointer)NULL);
}

static void createScreen(Widget parent)
{
	Widget top = XmCreatePanedWindow(parent, "pane", NULL,0);

	Widget tab = XmCreateTabStack(top,"tab",NULL,0);

	XtManageChild(XmCreateScrolledText(tab,"explanation",NULL,0));
	
	{
	XmStringTable    tmp0;
	Arg args[10];
	int n, i = XtNumber(values);
	Widget combo;
	Widget rc = XmCreateRowColumn(tab, "rc", NULL,0);
	XtManageChild(rc);

	tmp0 = (XmStringTable) XtMalloc(i * sizeof(XmString));
	for(n = 0; n < i; n++)
		tmp0[n] = XmStringCreateLocalized(values[n]);
	n=0;
	XtSetArg(args[n], XmNitems, tmp0); n++;
	XtSetArg(args[n], XmNitemCount, i); n++;
	combo = XmCreateDropDown(rc, "combo", args, n);
	
	XtManageChild(combo);
	}	

	createQuit(top);

	XtManageChild(tab);
	XtManageChild(top);
}

#define CLASS "Combo"

int 
main (int argc,char *argv[])
{
	XtAppContext app_context;
	Widget app_shell;
	Display *display;

        XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL); 
 
        app_shell = XtVaOpenApplication ( &app_context, 
                                   CLASS, 
                                   NULL, 
                                   0, 
                                   &argc, 
                                   argv, 
                                   NULL, 
                                   sessionShellWidgetClass, 
                                   NULL );

	XtVaSetValues(app_shell,XmNallowShellResize, True, NULL);

	/* create application */
	createScreen(app_shell);
	
	XtRealizeWidget(app_shell);

	/*	Process events, unwrapping correctly.  */
	while (!done)
	{
		XEvent event;
		XtAppNextEvent(app_context, &event);
        	XtDispatchEvent(&event);
	}

	XtDestroyWidget(app_shell);
	XtDestroyApplicationContext(app_context);
	exit(0);
}

