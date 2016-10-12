/* $XConsortium: simple_drag.c /main/4 1995/07/15 20:40:14 drk $ */
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
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>
#include <Xm/MessageB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/Text.h>
#include <Xm/TransferP.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CreateMenus(Widget);
int  Play(Widget);
void HelpCB(Widget, XtPointer, XtPointer);
void QuitCB(Widget, XtPointer, XtPointer);
void OutputAnAtomName(Widget, Atom); 
void StartDrag(Widget, XEvent *); 
void ConvertCallback(Widget, XtPointer, XtPointer);

Widget  toplevel;

/******************************************************************
main: 
******************************************************************/
int 
main(int argc, char **argv)
{
    static Widget  MainWindow; 
    XtAppContext   app_context;
    Widget         Frame1, RC1;
    Widget         Text1, ScrollBar1; 
    char    dragTranslations[] = "#override <Btn2Down>: StartDrag()"; 
    static  XtActionsRec  dragActions[] = 
    {  
        {"StartDrag", (XtActionProc)StartDrag} 
    };
    XtTranslations            parsed_xlations;

    XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL); 

    toplevel = XtOpenApplication(&app_context, "Test", NULL, 0,
                                &argc, argv, NULL, sessionShellWidgetClass,
                                NULL, 0);

    MainWindow = XtVaCreateManagedWidget("MainWindow1", 
                                    xmMainWindowWidgetClass, toplevel,
                                        NULL);
                     
    CreateMenus(MainWindow);


    /* Create a RowColumn to contain the ScrollBar and Text widgets. */
    RC1 = XtVaCreateManagedWidget("RC1", 
                                    xmRowColumnWidgetClass, MainWindow, 
                                    NULL);

    /* Create a ScrollBar. */ 
    parsed_xlations = XtParseTranslationTable(dragTranslations);
    XtAppAddActions(app_context, dragActions, XtNumber(dragActions));
    ScrollBar1 = XtVaCreateManagedWidget("SB1", 
                                    xmScrollBarWidgetClass, RC1,
                                    XmNorientation, XmHORIZONTAL, 
                                    XmNtranslations, parsed_xlations,
                                    NULL);
  /* Associate a convert callback with the ScrollBar. */ 
   XtAddCallback(ScrollBar1, XmNconvertCallback, ConvertCallback, NULL);


  /* Create a text widget; it will be a potential drop site. */
   Text1 = XtVaCreateManagedWidget("Text", 
                                    xmTextWidgetClass, RC1,
                                    XmNeditMode, XmMULTI_LINE_EDIT,
                                    XmNrows, 25,
                                    XmNcolumns, 25, 
                                    NULL);

   XtRealizeWidget(toplevel);
   XtAppMainLoop(app_context);

   return 0;    /* make compiler happy */
}


/**************************************************************************
StartDrag: When the user presses <Btn2Down>, this routine will be called. 
**************************************************************************/
void
StartDrag(Widget w,
          XEvent *event)
{
 Arg       args[2];
 Cardinal  n=0;

  XtSetArg(args[n], XmNdragOperations, XmDROP_COPY); n++;
  XmeDragSource(w, NULL, event, args, n);
}


/**********************************************************************
ConvertCallback: Handles requests to convert targets.  Actually,
our ConvertCallback procedure only knows how to convert the following
targets:
  * MOTIF_EXPORT_TARGETS
  * TARGETS
  * COMPOUND_TEXT 
**********************************************************************/
void
ConvertCallback(Widget  w,
                     XtPointer ignore,
                     XtPointer call_data)
{
 XmConvertCallbackStruct  *ccs = (XmConvertCallbackStruct *)call_data;
 int  value;
 Atom COMPOUND_TEXT = XInternAtom(XtDisplay(w), XmSCOMPOUND_TEXT, False);
 Atom TARGETS = XInternAtom(XtDisplay(w), "TARGETS", False);
 Atom MOTIF_EXPORT_TARGETS =
   XInternAtom(XtDisplay(w), XmS_MOTIF_EXPORT_TARGETS, False);
 int n;

  printf("\nNow in ConvertCallback.\n");
  printf("\tSelection: ");
  OutputAnAtomName((Widget)w, ccs->selection);
  printf("\tTarget: ");
  OutputAnAtomName((Widget)w, ccs->target); 

 /* XmeDragSource is going to call ConvertCallback and ask
    it to convert MOTIF_EXPORT_TARGETS. */ 
  if ( (ccs->target == MOTIF_EXPORT_TARGETS) ||
       (ccs->target == TARGETS))
    {
 /* We have to create a list of targets that ConvertCallback
    can convert.  For simplicity, we are going to restrict
    that list to one target: COMPOUND_TEXT. */
     Atom *targs = (Atom *) XtMalloc(sizeof(Atom) * 1);
     if (targs == NULL) {
      ccs->status = XmCONVERT_REFUSE;
      printf("Refuse.\n");
      return;
     }
     n = 0;
     targs[n] = COMPOUND_TEXT; n++;

     ccs->value = (XtPointer) targs;
     ccs->type = XA_ATOM;
     ccs->length = n;
     ccs->format = 32;
     ccs->status = XmCONVERT_DONE;  /* Yes, we converted the target. */
   }


 /* If the drop site supports COMPOUND_TEXT as an import target, then
    the drop site will ask ConvertCallback to convert the 
    value to COMPOUND_TEXT format. */ 
   else if (ccs->target == COMPOUND_TEXT) {
     char     *passtext;
     char     *ctext;
     XmString  cstring;
     char      ValueAsAString[10];
 
  /* The part of the ScrollBar that we are transferring is its 
     XmNvalue resource. */ 
     XtVaGetValues(w, XmNvalue, &value, NULL);

  /* Convert XmNvalue to COMPOUND_TEXT. */
       sprintf(ValueAsAString, "%d", value);
       cstring = XmStringCreateLocalized(ValueAsAString);
       ctext = XmCvtXmStringToCT(cstring);
       passtext = XtMalloc(strlen(ctext)+1);
       memcpy(passtext, ctext, strlen(ctext)+1);
  /* Assign converted string to XmConvertCallbackStruct. */
     ccs->value  = (XtPointer)passtext;
     ccs->type   = XA_STRING;
     ccs->length = strlen(passtext);
     ccs->format = 8;
     ccs->status = XmCONVERT_DONE;
   }

   else  {
     /* Unexpected target. */
     ccs->status = XmCONVERT_REFUSE;
   } 
}



/**************************************************************************
CreateMenus: This function generates the menu bar and the submenus. 
**************************************************************************/
void 
CreateMenus(Widget parent_of_menu_bar)
{
 XmString   file, help;
 Widget     menubar, FilePullDown, HelpPullDown;
 Widget     overview, quit, Help1; 

 /* Create the menubar itself. */
   file = XmStringCreateSimple("File");
   help = XmStringCreateSimple("Help");
   
   menubar      = (Widget)XmCreateMenuBar(parent_of_menu_bar, "menubar", 
                                          NULL, 0);
   FilePullDown = (Widget)XmCreatePulldownMenu(menubar, "FilePullDown", 
                                               NULL, 0); 
   HelpPullDown = (Widget)XmCreatePulldownMenu(menubar, "HelpPullDown", 
                                                 NULL, 0); 

 /******************************FILE*********************************/
    XtVaCreateManagedWidget("File", xmCascadeButtonWidgetClass, menubar,
                             XmNlabelString, file,
                             XmNmnemonic, 'F', 
                             XmNsubMenuId, FilePullDown,
                             NULL);
    quit = XtVaCreateManagedWidget("Quit", xmPushButtonGadgetClass, 
                                    FilePullDown, NULL);
    XtAddCallback(quit, XmNactivateCallback, QuitCB, NULL);


 /******************************HELP*********************************/
    Help1 = XtVaCreateManagedWidget("Help", xmCascadeButtonWidgetClass, 
                             menubar,
                             XmNlabelString, help,
                             XmNmnemonic, 'H', 
                             XmNsubMenuId, HelpPullDown,
                             NULL);
    XtVaSetValues(menubar, XmNmenuHelpWidget, Help1, NULL);
    overview = XtVaCreateManagedWidget("Overview", xmPushButtonGadgetClass, 
                                    HelpPullDown, NULL);
    XtAddCallback(overview, XmNactivateCallback, HelpCB, (XtPointer)1);

    XmStringFree(file);
    XmStringFree(help);

    XtManageChild(menubar); 
}



/*********************************************************************
HelpCB: Provide help when the user asks for it.
*********************************************************************/
void
HelpCB(Widget   w,
       XtPointer cd,
       XtPointer cb
      )
{
 int       what_kind_of_help = (int)cd;  
 char      help_string[500]; 
 XmString  hs_as_cs; 
 Widget    dialog_general_help; 
 Arg       arg[3];

 sprintf(help_string, 
"This program demonstrates how to add a convert callback\n\
to an application and how to add drag capability to the \n\
ScrollBar widget.  You should place the cursor on the slider \n\
of the ScrollBar.  Then press <Btn2> to begin a drag operation.\n\
A DragIcon will appear.  Move the DragIcon to the Text widget \n\
and 'drop' the DragIcon anywhere inside the Text widget.\n\
The application will transfer the XmNvalue of the ScrollBar \n\
to the Text widget."); 

   hs_as_cs = XmStringCreateLtoR(help_string, 
                                 XmFONTLIST_DEFAULT_TAG);
   
   XtSetArg(arg[0], XmNmessageString, hs_as_cs);
   dialog_general_help = (Widget)XmCreateMessageDialog(toplevel, 
                                             "message", arg, 1);
   XmStringFree(hs_as_cs);
 
   switch (what_kind_of_help)  {
     case 1: XtManageChild(dialog_general_help);
             break;
     default: /* no other help */
             break; 
   }
          
}



/*******************************************************************************
QuitCB: Exit 
*******************************************************************************/
void
QuitCB(Widget w, XtPointer cd, XtPointer cb)
{
  exit(0);
}



/**********************************************************************
OutputAnAtomName: Translates a target from its internal atom format into 
a human readable character string.
**********************************************************************/
void
OutputAnAtomName(Widget w,
            Atom target)
{
 char  *AtomName = malloc(sizeof(char *) * 34);

  AtomName = XGetAtomName(XtDisplay(w), target);
  printf("\t%s\n", AtomName);
}
