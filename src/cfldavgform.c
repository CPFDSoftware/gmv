#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#define MAXVAL 9.99999e32

static Widget cavgwlab, cavgflab, cavgswinform, cavgdisplab[MAXMATS+1],
       cavgftotlab, cavgwtotlab;
static short wavgflagc, favgflagc, wavgfldc = -1, favgfldc = -1;
static double caverages[MAXMATS+1], tottop, totbot;


void cfldavgformpop()
{
  XmString string;

   if (wavgfldc >= 0) 
      string = XmStringCreate(cfieldname[wavgfldc],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("ONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgwlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (favgfldc >= 0) 
      string = XmStringCreate(cfieldname[favgfldc],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgflab,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(cfldavgform);
}


void cfldavgformclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfieldform);
   XtUnmanageChild(cfldavgform);
}


void cfldavgapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ii, maxmatchar;
  char str[60], matnm[33];
  XmString string;
  void cavgcalc(), cavgclearwin();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Clear window and calculate averages.  */
   cavgclearwin();
   cavgcalc();
   
   /*  Display F and W totals.  */
   sprintf(str,"%14.10g",tottop);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgftotlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(cavgftotlab);
   
   sprintf(str,"%14.10g",totbot);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgwtotlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(cavgwtotlab);
   
   /*  Calculate max material name char. size.  */
   maxmatchar = 5;
   for (i = 0; i < maxcellmat; i++)
     {
      ii = strlen(mmatnames[i]);
      if (ii > maxmatchar) maxmatchar = ii;
     } 

   /*  Display material averages.  */
   for (i = 0; i < maxcellmat; i++)
     {
      strcpy(matnm,mmatnames[i]);
      sprintf(str,"%-*s %12.8g",maxmatchar,matnm,caverages[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cavgdisplab[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(cavgdisplab[i]);
     }

   /*  Display average for the whole problem.  */
   strcpy(matnm,"Total");
   sprintf(str,"%-*s %12.8g",maxmatchar,matnm,caverages[maxcellmat]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgdisplab[maxcellmat],XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(cavgdisplab[maxcellmat]);

   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void cfldavgformone(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string;

   string = XmStringCreate("One",
                              XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cavgwlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   wavgfldc = -1;
}


void cfldavgformfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   wavgflagc = 0;   favgflagc = 0;
   if (arg == 0) wavgflagc = 1;
   if (arg == 1) favgflagc = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = wavgfldc;
   if (arg == 1) ifld = favgfldc;
   cfieldformpop(CFLDAVG, ifld);
}


void makecfldavgform(parent)
 Widget parent;
{
  Widget cavgclose, cavgapply, cavglab1, cavgw, cavgone, cavgwfld, 
         cavgfc, cavgffld, cavgftot, cavgwtot, cavgswin, cavgsep, lastwidget;
  char str[20];
  int i, nv;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Average Cell Field",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cfldavgform = XmCreateFormDialog(parent,"Cfldavgform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cavgclose = XtVaCreateManagedWidget("Cavgclose",
                  xmPushButtonGadgetClass,
                  cfldavgform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cavgclose,XmNactivateCallback,
                    cfldavgformclose,NULL);

   string = XmStringCreateLtoR("Generate: Sum(W*F)/Sum(W)",
            XmSTRING_DEFAULT_CHARSET);
   cavglab1 = XtVaCreateManagedWidget("Cavglab1",
               xmLabelWidgetClass,
               cfldavgform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cavgclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("W: ",XmSTRING_DEFAULT_CHARSET);
   cavgw = XtVaCreateManagedWidget("Cavgw",
           xmLabelWidgetClass,
           cfldavgform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, cavglab1,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   string = XmStringCreate("One",XmSTRING_DEFAULT_CHARSET);
   cavgwlab = XtVaCreateManagedWidget("Cavgwlab",
              xmLabelWidgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavglab1,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cavgw,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("One",XmSTRING_DEFAULT_CHARSET);
   cavgone = XtVaCreateManagedWidget("Cavgone",
             xmPushButtonGadgetClass,
             cfldavgform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cavgw,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,               
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cavgone,XmNactivateCallback,
                    cfldavgformone,NULL);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cavgwfld = XtVaCreateManagedWidget("Cavgwfld",
              xmPushButtonGadgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavgone,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cavgwfld,XmNactivateCallback,
                    cfldavgformfld,(caddr_t) 0);

   string = XmStringCreate("F: ",XmSTRING_DEFAULT_CHARSET);
   cavgfc = XtVaCreateManagedWidget("Cavgfc",
            xmLabelWidgetClass,
            cfldavgform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cavgwfld,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("       ",XmSTRING_DEFAULT_CHARSET);
   cavgflab = XtVaCreateManagedWidget("Cavgflab",
              xmLabelWidgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavgwfld,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cavgfc,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cavgffld = XtVaCreateManagedWidget("Cavgffld",
              xmPushButtonGadgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavgflab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cavgffld,XmNactivateCallback,
                    cfldavgformfld,(caddr_t) 1);

   string = XmStringCreate("Total F = ",XmSTRING_DEFAULT_CHARSET);
   cavgftot = XtVaCreateManagedWidget("Cavgftot",
              xmLabelWidgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavgffld,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
   cavgftotlab = XtVaCreateManagedWidget("Cavgtotflab",
                 xmLabelWidgetClass,
                 cfldavgform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cavgffld,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cavgftot,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate("Total W = ",XmSTRING_DEFAULT_CHARSET);
   cavgwtot = XtVaCreateManagedWidget("Cavgwtot",
              xmLabelWidgetClass,
              cfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cavgftot,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
   cavgwtotlab = XtVaCreateManagedWidget("Cavgwtotlab",
                 xmLabelWidgetClass,
                 cfldavgform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cavgftot,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cavgwtot,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cavgapply = XtVaCreateManagedWidget("Cavgapply",
               xmPushButtonGadgetClass,
               cfldavgform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cavgwtot,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cavgapply,XmNactivateCallback,
                    cfldavgapply,NULL);

   /*  Create material labels.  */
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cavgapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 260);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   cavgswin = XmCreateScrolledWindow(cfldavgform,"Cavgswin",wargs,i);
   XtManageChild(cavgswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cavgswinform = XmCreateForm(cavgswin,"Cavgswinform",wargs,i);
   XtManageChild(cavgswinform);

   cavgsep = XtVaCreateManagedWidget("Cavgsep",
             xmSeparatorGadgetClass,
             cavgswinform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNseparatorType, XmNO_LINE,
             NULL);

   nv = 10;
   num_cavgmat = 10;
   lastwidget = cavgsep;
   for (i = 0; i < nv; i++)
     {
      sprintf(str,"Cavgdisplab%d",i);
      string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
      cavgdisplab[i] = XtVaCreateManagedWidget(str,
                       xmLabelWidgetClass,
                       cavgswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       NULL);
         XmStringFree(string);
      lastwidget = cavgdisplab[i];
     }
}


void cavgcalc()
{
  int i, j, k, l, gocell;
  double *wfieldc, *ffieldc, sumtop[MAXMATS], sumbot[MAXMATS], v;
  void fillcellvalue(int ifld, double *value);

   /*  Allocate field arrays.  */
   wfieldc = (double *)malloc(ncells*sizeof(double));
   ffieldc = (double *)malloc(ncells*sizeof(double));
   if (wfieldc == NULL || ffieldc == NULL) memerr();

   for (i = 0; i < ncells; i++) wfieldc[i] = 1.;
   if (wavgfldc >= 0)
      fillcellvalue(wavgfldc,wfieldc);

   for (i = 0; i < ncells; i++) ffieldc[i] = 1;
   if (favgfldc >= 0)
      fillcellvalue(favgfldc,ffieldc);

   /*  Calculate averages.  */
   tottop = 0;  totbot = 0;
   for (j = 0; j < MAXMATS; j++)
     {
      sumtop[j] = 0;  sumbot[j] = 0;
     }
   for (i = 0; i < ncells; i++)
     {
     
      /*  Average on selected cells and cells with material < 1.  */
      gocell = 1;
      gocell = (int)cellselectflag[i];
      if (cellmats[i] < 1) gocell = 0;
      
      if ((int)cellsubsetflag[i] == 0) gocell = 0;
      
      if (gocell)
        {
         v = wfieldc[i] * ffieldc[i];
         l = cellmats[i] - 1;
         if (l >= 0)
           {
            sumtop[l] += v;
            sumbot[l] += wfieldc[i];
            tottop += v;
            totbot += wfieldc[i];
           }
	}
     }

   k = maxcellmat;
   for (j = 0; j < k+1; j++) caverages[j] = 0.;
   for (j = 0; j < k; j++)
     {
      if (sumbot[j] != 0.)
         caverages[j] = sumtop[j] / sumbot[j];
     }
   if (totbot != 0)
     caverages[maxcellmat] = tottop/totbot;

   free(wfieldc);  free(ffieldc);
}



void cavgclearwin()
{
  int i;
  char str[20];
  Widget lastwidget;
  XmString string;

   /*  Check to see if there are more materials than labels.  */
   if (num_cavgmat <= maxcellmat+1)
     {

      lastwidget = cavgdisplab[num_cavgmat-1];
      for (i = num_cavgmat; i < maxcellmat+1; i++)
        {
         sprintf(str,"Cavgdisplab%d",i);
         string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
         cavgdisplab[i] = XtVaCreateManagedWidget(str,
                          xmLabelWidgetClass,
                          cavgswinform,
                          XmNtopAttachment, XmATTACH_WIDGET,
                          XmNtopWidget, lastwidget,
                          XmNleftAttachment, XmATTACH_FORM,
                          XmNrightAttachment, XmATTACH_NONE,
                          XmNbottomAttachment, XmATTACH_NONE,
                          XmNlabelString, string,
                          NULL);
            XmStringFree(string);
         lastwidget = cavgdisplab[i];
        }
      num_cavgmat = maxcellmat+1;
     }

   for (i = 0; i < num_cavgmat; i++)
     {
      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cavgdisplab[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(cavgdisplab[i]);
     }
     
  string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
  XtVaSetValues(cavgftotlab,XmNlabelString,string,NULL);
  XmStringFree(string);
  XmUpdateDisplay(cavgftotlab);
     
  string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
  XtVaSetValues(cavgwtotlab,XmNlabelString,string,NULL);
  XmStringFree(string);
  XmUpdateDisplay(cavgwtotlab);
}



void cavggetfield(int arg)
{
  XmString string;

   if (wavgflagc == 1)
     {
      string = XmStringCreate(cfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cavgwlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      wavgfldc = arg;
     }

   if (favgflagc == 1)
     {
      string = XmStringCreate(cfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cavgflab,XmNlabelString,string,NULL);
      XmStringFree(string);
      favgfldc = arg;
     }
}
