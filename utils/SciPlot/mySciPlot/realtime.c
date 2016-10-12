/*
 * Copyright (C) 1995 by Rob McMullen
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
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "SciPlot.h"
#include "SciPlotUtil.h"

XtAppContext app_con;
int line;
float xdata[10];
float ydata[10];

/* ARGSUSED */
static void 
Update(XtPointer client_data, XtIntervalId * idp)
{
  Widget plot = (Widget) client_data;
  int index;

  index=rand()%10;
  if (index>0) ydata[index]+=1.0;
  SciPlotListUpdateFromFloat(plot, line, 10, xdata, ydata);
  if (SciPlotQuickUpdate(plot)) {
      SciPlotUpdate(plot);
  }
  
  XtAppAddTimeOut(app_con,500,Update,plot);
}

int
main(int argc, char *argv[])
{
  Widget toplevel, dummy, plot;
  int i;

  toplevel = XtAppInitialize(&app_con, "SciPlot",
    NULL, 0, &argc, argv, NULL, NULL, 0);
  dummy = XtVaCreateManagedWidget("dummy",
    coreWidgetClass, toplevel,
                                  XtNwidth,10,
                                  XtNheight,10,
    NULL);
  XtSetMappedWhenManaged(toplevel, False);

  plot=SciPlotDialog(toplevel,"Real Time Test");
  for (i=0; i<10; i++) {
      xdata[i]=i+1.0;
      ydata[i]=i/2.0;
  }
  line = SciPlotListCreateFromFloat(plot, 10, xdata, ydata, "race");
  
  XtRealizeWidget(toplevel);

  SciPlotUpdate(plot);

  srand((unsigned int) getpid()); /* Initialize it with a fairly random seed */
  XtAppAddTimeOut(app_con,500,Update,plot);

  XtAppMainLoop(app_con);

  return 0;
}
