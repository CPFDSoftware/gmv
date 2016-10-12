/* $XConsortium: dainput.c /main/5 1995/07/15 20:47:20 drk $ */
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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>          /* to get _NO_PROTO */

#include "dainput.h"


/****************************************************************
 * base:
 ****************************************************************/
static char *base(char *str)
{
  static char basename[300];
  char *p;

  if (p = strrchr(str, '/'))
    (void) strcpy(basename, p+1);
  else
    (void) strcpy(basename, str);
  if (p = strrchr(basename, '.'))
    *p ='\0';
  return basename;
}


/**************************************************************
 * main:
 **************************************************************/
int main(int argc, char **argv)
{
  XtAppContext  app_context;
  Display     * display;
  Widget        top_level;
  char        * aclass = "XmdDaInput";
  char          name[132];
  char       ** temp_argv = argv;


  (void) strncpy(name, base(argv[0]), 132);

  /* handle the '-name' option */
  while (*temp_argv) {
    if (strcmp(*temp_argv, "-name") == 0) {
      (void) strncpy(name, *++temp_argv, 132);
      break;
    }
    temp_argv++;
  }

  /* Initialize the X Intrinsics */
  XtToolkitInitialize();

  /* Set up language environment */
  XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

  /* create application context */
  app_context = XtCreateApplicationContext();

  /* open up one display */
  display = XtOpenDisplay(app_context,
                          NULL,
                          name, aclass,
                          (XrmOptionDescRec *)NULL, 0,
                          &argc, argv);
  if (!display) {
    (void) fprintf(stderr, "Unable to open display\n");
    exit(0);
  }

  /* Create the user interface */
  top_level = DaCreateGui(name, aclass, argc, argv, display);
  if (top_level == NULL) {
    (void) fprintf(stderr, "Unable to create user interface\n");
    exit(1);
  }

  /* Realize and start event dispatching */
  XtRealizeWidget(top_level);
  XtAppMainLoop(app_context);

  return 0;    /* make compiler happy */
}

