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
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/DialogS.h>
#include <Xm/MainW.h>
#include <Xm/PanedW.h>

#include "SciPlot.h"

/* Globals */
static int DialogCount=0;		/* Number of dialogs on screen */


typedef struct {
  char *text;
  int num;
} textnumpair;

textnumpair colors_list[] =
{
  {"blue", 0},
  {"red", 0},
  {"ForestGreen", 0},
  {"DarkGoldenrod", 0},
  {"orange", 0},
  {"magenta", 0},
  {"grey", 0},
  {"purple", 0},
};

#define NUM_COLORS 8

textnumpair marker_list[] =
{
  {"XtMARKER_SQUARE", XtMARKER_SQUARE},
  {"XtMARKER_CIRCLE", XtMARKER_CIRCLE},
  {"XtMARKER_UTRIANG ", XtMARKER_UTRIANGLE},
  {"XtMARKER_DTRIANG ", XtMARKER_DTRIANGLE},
  {"XtMARKER_DIAMOND", XtMARKER_DIAMOND},
  {"XtMARKER_HOURGLA ", XtMARKER_HOURGLASS},
  {"XtMARKER_BOWTIE", XtMARKER_BOWTIE},
  {"XtMARKER_LTRIANG ", XtMARKER_LTRIANGLE},
  {"XtMARKER_RTRIANG", XtMARKER_RTRIANGLE},
  {"XtMARKER_FCIRCLE ", XtMARKER_FCIRCLE},
  {"XtMARKER_FSQUARE", XtMARKER_FSQUARE},
  {"XtMARKER_FUTRIAN", XtMARKER_FUTRIANGLE},
  {"XtMARKER_FDTRIAN ", XtMARKER_FDTRIANGLE},
  {"XtMARKER_FDIAMON ", XtMARKER_FDIAMOND},
  {"XtMARKER_FHOURGL", XtMARKER_FHOURGLASS},
  {"XtMARKER_FBOWTIE", XtMARKER_FBOWTIE},
  {"XtMARKER_FLTRIAN", XtMARKER_FLTRIANGLE},
  {"XtMARKER_FRTRIAN", XtMARKER_FRTRIANGLE}
};

#define NUM_MARKERS 18


/* String Utilities */

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif



/*
 *  Utils
 */

/* Internal structure for each plot dialog */

typedef struct {
  Widget label1;
  Widget label2;
  Widget label3;
  Widget title;
  Widget xlabel;
  Widget ylabel;
  Widget shell;
  Widget plot;
  Widget text;
} PlotDialogData;

static void
QuitCallback(Widget btn, XtPointer client, XtPointer call)
{
  exit(0);
}

static void
DismissCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;

  XtPopdown(stuff->shell);
  XtDestroyWidget(stuff->shell);
  free(stuff);
  DialogCount--;
  if (DialogCount == 0)
    exit(0);
}

static void
LogXCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNxLog,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNxLog,        state,
    NULL);
}

static void
LogYCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNyLog,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNyLog,        state,
    NULL);
}

static void
NumbersXCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNxAxisNumbers,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNxAxisNumbers,        state,
    NULL);
}

static void
NumbersYCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNyAxisNumbers,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNyAxisNumbers,        state,
    NULL);
}

static void
OriginXCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNxOrigin,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNxOrigin,        state,
    NULL);
}

static void
OriginYCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNyOrigin,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNyOrigin,        state,
    NULL);
}

static void
MajorCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNdrawMajor,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNdrawMajor,        state,
    NULL);
}

static void
MinorCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNdrawMinor,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNdrawMinor,        state,
    NULL);
}

static void
LegendCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNshowLegend,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNshowLegend,        state,
    NULL);
}

static void
TitleChangeCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  char *ret;

  ret=XmTextFieldGetString(btn);
  XtVaSetValues(stuff->plot,
    XtNplotTitle, ret,
    NULL);
  XtFree(ret);
}

static void
TitleCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNshowTitle,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNshowTitle,        state,
    NULL);
#ifdef DEBUG
  if (1) {
    char label[256],*txt1,*txt2,*txt3;
    static int count=0;
    
    sprintf(label,"Title #%d",++count);
    XtVaSetValues(stuff->plot,
      XtNplotTitle, label,
      NULL);
    XtVaGetValues(stuff->plot,
      XtNplotTitle, &txt1,
      XtNxLabel, &txt2,
      XtNyLabel, &txt3,
      NULL);
    printf("title=%s\nx label=%s\ny label=%s\n",txt);
#endif
}

static void
LabelXChangeCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  char *ret;

  ret=XmTextFieldGetString(btn);
  XtVaSetValues(stuff->plot,
    XtNxLabel, ret,
    NULL);
  XtFree(ret);
}

static void
LabelXCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNshowXLabel,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNshowXLabel,        state,
    NULL);
#ifdef DEBUG
  if (1) {
    char label[256];
    static int count=0;
    
    sprintf(label,"X Label #%d",++count);
    XtVaSetValues(stuff->plot,
      XtNxLabel, label,
      NULL);
  }
#endif
}

static void
LabelYChangeCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  char *ret;

  ret=XmTextFieldGetString(btn);
  XtVaSetValues(stuff->plot,
    XtNyLabel, ret,
    NULL);
  XtFree(ret);
}

static void
LabelYCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  Boolean state;

  XtVaGetValues(stuff->plot,
    XtNshowYLabel,        &state,
    NULL);
  state=!state;
  XtVaSetValues(stuff->plot,
    XtNshowYLabel,        state,
    NULL);
#ifdef DEBUG
  if (1) {
    char label[256];
    static int count=0;
    
    sprintf(label,"Y Label #%d",++count);
    XtVaSetValues(stuff->plot,
      XtNyLabel, label,
      NULL);
  }
#endif
}

static void
PSCallback(Widget btn, XtPointer client, XtPointer call)
{
  PlotDialogData *stuff = (PlotDialogData *) client;
  char *filename;
  
#ifdef EXTRA_FANCY
  char txt[1024],*ptr;

  ptr=XmTextFieldGetString(stuff->label1);
  strcpy(txt,ptr);
  strcat(txt,"\n");
  XtFree(ptr);
  ptr=XmTextFieldGetString(stuff->label2);
  strcat(txt,ptr);
  strcat(txt,"\n\n");
  XtFree(ptr);
  ptr=XmTextFieldGetString(stuff->label3);
  strcat(txt,ptr);
  strcat(txt,"\n");
  XtFree(ptr);
                
  filename = XmTextFieldGetString(stuff->text);
  if (strlen(filename) > 0)
    SciPlotPSCreateFancy(stuff->plot,filename,
      TRUE,txt,NULL);
  XtFree(filename);
#else
  filename = XmTextFieldGetString(stuff->text);
  if (strlen(filename) > 0)
    SciPlotPSCreateColor(stuff->plot,filename);
  XtFree(filename);
#endif
}


/*
 * Create a plot dialog using Motif widgets
 */
static PlotDialogData *
SciPlotDialogInternal(Widget parent, char *title)
{
  Widget paned, box, topbox, botbox, btn, t1;
  PlotDialogData *info;
  char *text;
#ifdef EXTRA_FANCY
  char buf[256];
  time_t  now;
  struct tm *t;
#endif
  
  info = (PlotDialogData *) malloc(sizeof(PlotDialogData));
  DialogCount++;

  text = malloc(strlen(title) + 256);
  sprintf(text, "Plot #%d: %s ", DialogCount, title);

  paned=XmCreateFormDialog(parent,text,NULL,0);
  info->shell=XtParent(paned);
  XtVaSetValues(paned,
    XmNdialogStyle,XmDIALOG_MODELESS,
    XmNnoResize,False,
    XmNautoUnmanage,False,
    NULL);
  
  topbox = box = XtVaCreateManagedWidget("box",
    xmRowColumnWidgetClass,paned,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNpacking,     XmPACK_COLUMN,
    XmNnumColumns,  2,
    XmNorientation, XmVERTICAL,
    NULL);

#ifdef EXTRA_FANCY
  btn = XtVaCreateManagedWidget("Desc 1:",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("Desc 2:",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("Desc 3:",
    xmLabelWidgetClass, box,
    NULL);
#endif
  btn = XtVaCreateManagedWidget("Xlabel:",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("Ylabel:",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("Title:",
    xmLabelWidgetClass, box,
    NULL);
  
#ifdef EXTRA_FANCY
  info->label1 = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    XmNskipAdjust, True,
    NULL);
  info->label2 = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    XmNskipAdjust, True,
    NULL);
  now=time(NULL);
  t=localtime(&now);
  strftime(buf,256,"%c",t);
  info->label3 = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    XmNvalue, buf,
    XmNcursorPosition, 100,
    XmNskipAdjust, True,
    NULL);
#endif
  info->xlabel = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    NULL);
  info->ylabel = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    NULL);
  info->title = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    NULL);
  XtAddCallback(info->xlabel, XmNactivateCallback, LabelXChangeCallback, (XtPointer) info);
  XtAddCallback(info->ylabel, XmNactivateCallback, LabelYChangeCallback, (XtPointer) info);
  XtAddCallback(info->title, XmNactivateCallback, TitleChangeCallback, (XtPointer) info);
  
  box = XtVaCreateManagedWidget("box",
    xmRowColumnWidgetClass,paned,
    XmNleftAttachment,      XmATTACH_FORM,
    XmNrightAttachment,     XmATTACH_FORM,
    XmNbottomAttachment,    XmATTACH_FORM,
    XmNorientation,         XmHORIZONTAL,
    XmNpacking,             XmPACK_TIGHT,
    NULL);
  btn = XtVaCreateManagedWidget("Quit",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, QuitCallback, (XtPointer) NULL);

  btn = XtVaCreateManagedWidget("Dismiss",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, DismissCallback, (XtPointer) info);

  btn = XtVaCreateManagedWidget("Make Postscript",
    xmPushButtonWidgetClass, box,
    NULL);
  sprintf(text, "plot%d.ps", DialogCount);
  t1 = XtVaCreateManagedWidget("entry",
    xmTextFieldWidgetClass, box,
    XmNwidth, 200,
    XmNvalue, text,
    XmNcursorPosition, 100,
    NULL);
  info->text = t1;
  XtAddCallback(btn, XmNactivateCallback, PSCallback, (XtPointer) info);

  botbox = box = XtVaCreateManagedWidget("box",
    xmRowColumnWidgetClass,paned,
    XmNleftAttachment,      XmATTACH_FORM,
    XmNrightAttachment,     XmATTACH_FORM,
    XmNbottomAttachment,    XmATTACH_WIDGET,
    XmNbottomWidget, box,
    XmNorientation,         XmHORIZONTAL,
    XmNpacking,             XmPACK_TIGHT,
    NULL);
  btn = XtVaCreateManagedWidget("Log",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("X",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, LogXCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Y",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, LogYCallback, (XtPointer) info);

  btn = XtVaCreateManagedWidget("Origin",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("X",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, OriginXCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Y",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, OriginYCallback, (XtPointer) info);

  btn = XtVaCreateManagedWidget("Numbers",
    xmLabelWidgetClass, box,
    NULL);
  btn = XtVaCreateManagedWidget("X",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, NumbersXCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Y",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, NumbersYCallback, (XtPointer) info);

  btn = XtVaCreateManagedWidget("Major",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, MajorCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Minor",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, MinorCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Title",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, TitleCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("X Label",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, LabelXCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Y Label",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, LabelYCallback, (XtPointer) info);
  btn = XtVaCreateManagedWidget("Legend",
    xmPushButtonWidgetClass, box,
    NULL);
  XtAddCallback(btn, XmNactivateCallback, LegendCallback, (XtPointer) info);

  info->plot = XtVaCreateManagedWidget("plot",
    sciplotWidgetClass, paned,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, topbox,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, botbox,
    XtNheight, (XtArgVal) 600,
    XtNwidth, (XtArgVal) 500,
    XtNplotTitle, title,
/*     XtNlabelFont, XtFONT_TIMES|8, */
/*     XtNtitleFont, XtFONT_TIMES|8, */
/*     XtNtitleMargin, 4, */
/*     XtNaxisFont, XtFONT_TIMES|8, */
    NULL);

  free(text);
  XtManageChild(paned);
  XtManageChild(info->shell);
  return info;
}

static void
SciPlotDialogInternalPopup(PlotDialogData *info)
{
  char *x,*y,*t;
  
  XtVaGetValues(info->plot,
    XtNplotTitle, &t,
    XtNxLabel, &x,
    XtNyLabel, &y,
    NULL);
  XmTextFieldSetString(info->title,t);
  XmTextFieldSetString(info->xlabel,x);
  XmTextFieldSetString(info->ylabel,y);
  
  XtManageChild(info->shell);
}

Widget
SciPlotDialog(Widget parent, char *title)
{
  PlotDialogData *info;

  info = SciPlotDialogInternal(parent,title);
  return info->plot;
}

void
SciPlotDialogPopup(Widget w)
{
  XtManageChild(XtParent(XtParent(w)));
}
