/* $XConsortium: i18ninput.c /main/5 1995/07/15 20:45:37 drk $ */
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

static XtAppContext app_context;

static String fallback_reslist[] = {
  "*font.label.labelString:	Select font",
  "*font*button1.labelString:	Small",
  "*font*button1.renderTable:	*medium*-r-*--14*:",
  "*font*button2.labelString:	Medium",
  "*font*button2.renderTable:	*medium*-r-*--18*:",
  "*font*button3.labelString:	Large",
  "*font*button3.renderTable:	*medium*-r-*--24*:",
  "*color.label.labelString:	Select color",
  "*color*button1.labelString:	Red",
  "*color*button1.foreground:	#fe5151",
  "*color*button2.labelString:	Green",
  "*color*button2.foreground:	#00be51",
  "*color*button3.labelString:	Blue",
  "*color*button3.foreground:	#2aa1fd",
  "*XmText.renderTable:		*medium*-r-*--18*:",
  "*XmTextField.renderTable:	*medium*-r-*--18*:",
  NULL
  };

extern int dialog_init(int *argc, char **argv, Display *dpy);

/****************************************************************
 * base:
 ****************************************************************/
static char *base(char *str)
{
  static char basename[300];
  char *p;

  if ((p = strrchr(str, '/')))
    (void) strcpy(basename, p+1);
  else
    (void) strcpy(basename, str);
  if ((p = strrchr(basename, '.')))
    *p ='\0';
  return basename;
}


/**************************************************************
 * main:
 **************************************************************/
int main(int argc, char **argv)
{
  Display     * display;
  char          name[132];

  (void) strncpy(name, base(argv[0]), 132);

  /* Initialize the X Intrinsics */
  XtToolkitInitialize();

  /* Set up language environment for X */
  XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

  /* Create application context */
  app_context = XtCreateApplicationContext();
  XtAppSetFallbackResources(app_context, fallback_reslist);

  /* open up one display */
  display = XtOpenDisplay(app_context,
                          NULL,
                          name, "XmdI18nInput",
                          (XrmOptionDescRec *)NULL, 0,
                          &argc, argv);
  if (!display) {
    (void) fprintf(stderr, "Unable to open display\n");
    exit(0);
  }

  /* Call init functions. This creates the user interface 
     (placed in input.c) */
  if (dialog_init(&argc, argv, display) != 0)
    exit(1);

  XtAppMainLoop(app_context);

  return 0;    /* make compiler happy */
}

