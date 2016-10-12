/* $XConsortium: filemanager.h /main/5 1995/07/15 20:45:20 drk $ */
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

#ifndef _filemanager_h
#define _filemanager_h

#include <dirent.h>
#include <sys/stat.h>

typedef struct _FileInfo {
  char *name;
  struct stat statbuf;
  Widget icon;      
  Boolean dirRead;
} FileInfoRec;

extern Widget fileviewer, dirOM, toplevel, mainW, gotoDialog;
extern char *currentdir;
extern Widget *dirLabel, displayLabel;
extern int ndirLabel;
extern char *paths[];
extern FileInfoRec *FI;
extern WidgetList IconGadgets;
extern int validFI;
extern Boolean showHidden;
extern time_t ltm;
extern char *deleteCommand;
extern XrmQuark app_class_quark;
extern XrmQuark app_quark;

extern Widget CreateInterface(char*, Widget);
extern char* fullpath(char*);
extern void readdirCB(Widget, char*, XmAnyCallbackStruct *);
extern void outlineCB(Widget, char*, XmContainerOutlineCallbackStruct *);
extern void manageCB(Widget, Widget, XtPointer);
extern void viewCB(Widget, char*, XtPointer);
extern void gotoCB(Widget widget, XtPointer i1, XtPointer cb);
extern void selectCB(Widget w, XtPointer ignore, XtPointer cb);
extern void quitCB(Widget, char*, XmAnyCallbackStruct*);
extern void newFolder(Widget widget, XtPointer ignore, XtPointer ignore2);
extern void deleteItem(Widget widget, XtPointer ignore, XtPointer ignore2);
extern void targetDestinationCallback(Widget, XtPointer,
				      XmDestinationCallbackStruct*);
extern void targetConvertCallback(Widget, XtPointer,
				  XmConvertCallbackStruct*);
extern char *getPathFromIcon(Widget);
extern void fixViewerSize(Widget w, XtPointer i1, XtPointer i2);
extern void showHiddenCB(Widget widget, XtPointer ignore,
			 XmToggleButtonCallbackStruct *callback_data);
extern void read_directory(Widget, char*);
extern int process_single_entry(Widget, char*, int, struct dirent*);

#endif /* _filemanager_h */
