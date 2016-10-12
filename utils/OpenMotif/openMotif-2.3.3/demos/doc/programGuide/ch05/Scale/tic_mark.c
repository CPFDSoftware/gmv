/* $XConsortium: tic_mark.c /main/5 1995/07/15 20:39:30 drk $ */
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
#include <stdio.h>
#include <Xm/XmAll.h>
#include <Xmd/RegEdit.h>

void        CreateScale(Widget);
extern void CreateMenus(Widget);
extern void HelpCB(Widget, XtPointer, XtPointer); 
extern void QuitCB(Widget, XtPointer, XtPointer);

Widget top_level; 

#define APP_CLASS "XmdScale"

static String fallbacks[] = {
"scale1.allowShellResize: True",
"scale1.value: 20",
NULL
};

int
main(int    argc,
     char **argv)
{
 XtAppContext app_context;
 Widget       main_window; 

   XtSetLanguageProc(NULL, NULL, NULL);
   top_level = XtVaOpenApplication(&app_context, APP_CLASS, 
                                NULL, 0, &argc, argv, 
                                fallbacks,sessionShellWidgetClass, 
                                NULL);
   XmdRegisterEditres(top_level);
   main_window = XtVaCreateManagedWidget("main_window", 
                                xmMainWindowWidgetClass, top_level,
                                NULL);

   CreateMenus(main_window);
   CreateScale(main_window); 

   XtRealizeWidget(top_level);
   XtAppMainLoop(app_context);

   return 0;    /* make compiler happy */
}


void
CreateScale(Widget parent_of_scale)
{
#define BOTTOM_OF_THERMOMETER -20
#define TOP_OF_THERMOMETER 40
#define NUM_TICS (((TOP_OF_THERMOMETER - BOTTOM_OF_THERMOMETER) / 10) + 1)
 Widget          scale1, tics[NUM_TICS];
 Arg             args[10];
 Cardinal        i, n;
 unsigned char   scale_orientation, tic_orientation;
 Dimension       tic_long_dim = 10, tic_short_dim = 5;
 Dimension       tic_width, tic_height;
 char            tic_name[10];

 /* Build the Scale. */
   scale1 = XtVaCreateManagedWidget("scale1",
                 xmScaleWidgetClass, parent_of_scale,
                 XmNslidingMode, XmTHERMOMETER,
                 XmNeditable, True,
                 XmNshowValue, True,
                 XmNminimum, BOTTOM_OF_THERMOMETER,
                 XmNmaximum, TOP_OF_THERMOMETER, 
                 NULL);
   
 /* Now build the tic marks. */
   n = 0;
   XtSetArg(args[n], XmNorientation, &scale_orientation);   n++;
   XtGetValues(scale1, args, n);
   if (scale_orientation == XmHORIZONTAL) {
       tic_orientation = XmVERTICAL;
       tic_width = tic_short_dim;
       tic_height = tic_long_dim;
   } else {
       tic_orientation = XmHORIZONTAL;
       tic_width = tic_long_dim;
       tic_height = tic_short_dim;
   }
   
   for (i = 0; i < NUM_TICS; i++) {
       sprintf(tic_name, "tic_%d", i);
       n = 0;
       XtSetArg(args[n], XmNseparatorType, XmSINGLE_LINE);  n++;
       XtSetArg(args[n], XmNorientation, tic_orientation);  n++;
       XtSetArg(args[n], XmNwidth, tic_width);              n++;
       XtSetArg(args[n], XmNheight, tic_height);            n++;
       tics[i] = XmCreateSeparatorGadget(scale1, tic_name,
                                           args, n);
   }

   XtManageChildren(tics, NUM_TICS);
}
