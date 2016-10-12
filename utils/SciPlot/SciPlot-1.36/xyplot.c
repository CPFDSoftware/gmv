/*
 * Copyright (C) 1996 by Rob McMullen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * Author: Rob McMullen <rwmcm@orion.ae.utexas.edu>
 *         http://www.ae.utexas.edu/~rwmcm
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/PushB.h>
#include <Xm/MainW.h>
#include "SciPlot.h"
#include "SciPlotUtil.h"

void
ArgProcess(Widget parent,int argc, char *argv[])
{
  int argloc;
  FILE *fd;

  if (argc > 1) {
    argloc = 1;
    while (argloc <= argc) {
      fd = fopen(argv[argloc++], "r");
      if (fd) {
        SciPlotReadDataFile(parent, fd);
        fclose(fd);
      }
    }
  }
  else {
    SciPlotReadDataFile(parent, stdin);
  }
}
  
int
main(int argc, char *argv[])
{
  XtAppContext app_con;
  Widget toplevel, mainw, dummy;

  toplevel = XtAppInitialize(&app_con, "SciPlot",
    NULL, 0, &argc, argv, NULL, NULL, 0);
  mainw=XmCreateMainWindow(toplevel,"dummy",NULL,0);
  dummy=XmCreatePushButton(mainw,"dummy",NULL,0);
  
  XtSetMappedWhenManaged(toplevel, False);
  XtSetMappedWhenManaged(mainw, False);
  XtManageChild(mainw);
  XtManageChild(dummy);
  XtRealizeWidget(toplevel);
  
  ArgProcess(mainw,argc,argv);

  XtAppMainLoop(app_con);

  return 0;
}
