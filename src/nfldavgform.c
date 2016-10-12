#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#define MAXVAL 9.99999e32

static Widget navgwlab, navgflab, navgswinform, navgdisplab[MAXMATS+1],
       navgftotlab, navgwtotlab;

static short wavgflag, favgflag, wavgfld = -1, favgfld = -1;
static double naverages[MAXMATS + 1], tottop, totbot;


void nfldavgformpop()
{
  XmString string;

   if (wavgfld >= 0) 
      string = XmStringCreate(fieldname[wavgfld],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("ONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgwlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (favgfld >= 0) 
      string = XmStringCreate(fieldname[favgfld],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgflab,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(nfldavgform);
}


void nfldavgclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(nfldavgform);
}


void nfldavgapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ii, maxmatchar;
  char str[60], matnm[33];
  XmString string;
  void navgcalc(), navgclearwin();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Clear window and calculate averages.  */
   navgclearwin();
   navgcalc();
   
   /*  Display F and W totals.  */
   sprintf(str,"%14.10g",tottop);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgftotlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(navgftotlab);
   
   sprintf(str,"%14.10g",totbot);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgwtotlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(navgwtotlab);

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
      sprintf(str,"%-*s %12.8g",maxmatchar,matnm,naverages[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(navgdisplab[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(navgdisplab[i]);
     }

   /*  Display average for the whole problem.  */
   strcpy(matnm,"Total");
   sprintf(str,"%-*s %12.8g",maxmatchar,matnm,naverages[maxcellmat]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgdisplab[maxcellmat],XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(navgdisplab[maxcellmat]);

   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void nfldavgone(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string;

   string = XmStringCreate("One",
                              XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(navgwlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   wavgfld = -1;
}


void nfldavgfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   wavgflag = 0;   favgflag = 0;
   if (arg == 0) wavgflag = 1;
   if (arg == 1) favgflag = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = wavgfld;
   if (arg == 1) ifld = favgfld;
   nfieldformpop(NFLDAVG, ifld);
}


void makenfldavgform(parent)
 Widget parent;
{
  Widget navgclose, navgapply, navglab1, navgw, navgone, navgwfld, 
         navgfc, navgffld, navgftot, navgwtot, navgswin, navgsep, lastwidget;
  char str[20];
  int i, nv;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Average Node Field",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nfldavgform = XmCreateFormDialog(parent,"Nfldavgform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   navgclose = XtVaCreateManagedWidget("Navgclose",
                  xmPushButtonGadgetClass,
                  nfldavgform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(navgclose,XmNactivateCallback,
                    nfldavgclose,NULL);

   string = XmStringCreateLtoR("Generate: Sum(W*F)/Sum(W)",
            XmSTRING_DEFAULT_CHARSET);
   navglab1 = XtVaCreateManagedWidget("Navglab1",
               xmLabelWidgetClass,
               nfldavgform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, navgclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("W: ",XmSTRING_DEFAULT_CHARSET);
   navgw = XtVaCreateManagedWidget("Navgw",
           xmLabelWidgetClass,
           nfldavgform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, navglab1,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   string = XmStringCreate("One",XmSTRING_DEFAULT_CHARSET);
   navgwlab = XtVaCreateManagedWidget("Navgwlab",
              xmLabelWidgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navglab1,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, navgw,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("One",XmSTRING_DEFAULT_CHARSET);
   navgone = XtVaCreateManagedWidget("Navgone",
             xmPushButtonGadgetClass,
             nfldavgform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, navgw,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,               
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(navgone,XmNactivateCallback,
                    nfldavgone,NULL);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   navgwfld = XtVaCreateManagedWidget("Navgwfld",
              xmPushButtonGadgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navgone,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(navgwfld,XmNactivateCallback,
                    nfldavgfld,(caddr_t) 0);

   string = XmStringCreate("F: ",XmSTRING_DEFAULT_CHARSET);
   navgfc = XtVaCreateManagedWidget("Navgfc",
            xmLabelWidgetClass,
            nfldavgform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, navgwfld,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("       ",XmSTRING_DEFAULT_CHARSET);
   navgflab = XtVaCreateManagedWidget("Navgflab",
              xmLabelWidgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navgwfld,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, navgfc,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   navgffld = XtVaCreateManagedWidget("Navgffld",
              xmPushButtonGadgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navgflab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(navgffld,XmNactivateCallback,
                    nfldavgfld,(caddr_t) 1);

   string = XmStringCreate("Total F = ",XmSTRING_DEFAULT_CHARSET);
   navgftot = XtVaCreateManagedWidget("Navgftot",
              xmLabelWidgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navgffld,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
   navgftotlab = XtVaCreateManagedWidget("Navgtotflab",
                 xmLabelWidgetClass,
                 nfldavgform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, navgffld,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, navgftot,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate("Total W = ",XmSTRING_DEFAULT_CHARSET);
   navgwtot = XtVaCreateManagedWidget("Navgwtot",
              xmLabelWidgetClass,
              nfldavgform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, navgftot,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
   navgwtotlab = XtVaCreateManagedWidget("Navgwtotlab",
                 xmLabelWidgetClass,
                 nfldavgform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, navgftot,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, navgwtot,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   navgapply = XtVaCreateManagedWidget("Navgapply",
               xmPushButtonGadgetClass,
               nfldavgform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, navgwtot,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(navgapply,XmNactivateCallback,
                    nfldavgapply,NULL);

   /*  Create material labels.  */
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, navgapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 260);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   navgswin = XmCreateScrolledWindow(nfldavgform,"Navgswin",wargs,i);
   XtManageChild(navgswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   navgswinform = XmCreateForm(navgswin,"Navgswinform",wargs,i);
   XtManageChild(navgswinform);

   navgsep = XtVaCreateManagedWidget("Navgsep",
             xmSeparatorGadgetClass,
             navgswinform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNseparatorType, XmNO_LINE,
             NULL);

   nv = 10;
   num_navgmat = 10;
   lastwidget = navgsep;
   for (i = 0; i < nv; i++)
     {
      sprintf(str,"Navgdisplab%d",i);
      string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
      navgdisplab[i] = XtVaCreateManagedWidget(str,
                       xmLabelWidgetClass,
                       navgswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       NULL);
         XmStringFree(string);
      lastwidget = navgdisplab[i];
     }
}


void navgcalc()
{
  int i, j, k, l, gonode;
  double *wfield, *ffield, sumtop[MAXMATS], sumbot[MAXMATS], v;
  void fillnodevalue(int ifld, double *value);

   /*  Allocate field arrays.  */
   wfield = (double *)malloc((nnodes+1)*sizeof(double));
   ffield = (double *)malloc((nnodes+1)*sizeof(double));
   if (wfield == NULL || ffield == NULL) memerr();

   for (i = 0; i < nnodes+1; i++) wfield[i] = 1.;
   if (wavgfld >= 0)
      fillnodevalue(wavgfld,wfield);

   for (i = 0; i < nnodes+1; i++) ffield[i] = 1;
   if (favgfld >= 0)
      fillnodevalue(favgfld,ffield);

   /*  Calculate averages.  */
   tottop = 0;  totbot = 0;
   for (j = 0; j < MAXMATS; j++)
     {
      sumtop[j] = 0;  sumbot[j] = 0;
     }
   for (i = 1; i < nnodes+1; i++)
     {
     
      /*  Average on selected nodes and nodes with material < 1.  */
      gonode = 1;
      gonode = (int)nodeselectflag[i];
      if (imat[i] < 1) gonode = 0;
      
      if ((int)nodesubsetflag[i] == 0) gonode = 0;
      
      if (gonode)
        {
         v = wfield[i] * ffield[i];
         l = imat[i] - 1;
         if (l >= 0)
           {
            sumtop[l] += v;
            sumbot[l] += wfield[i];
            tottop += v;
            totbot += wfield[i];
           }
	}
     }

   k = maxcellmat;
   for (j = 0; j < k+1; j++) naverages[j] = 0.;
   for (j = 0; j < k; j++)
     {
      if (sumbot[j] != 0.)
         naverages[j] = sumtop[j] / sumbot[j];
     }
   if (totbot != 0)
     naverages[maxcellmat] = tottop/totbot;

   free(wfield);  free(ffield);
}



void navgclearwin()
{
  int i;
  char str[20];
  Widget lastwidget;
  XmString string;

   /*  Check to see if there are more materials than labels.  */
   if (num_navgmat <= maxcellmat+1)
     {

      lastwidget = navgdisplab[num_navgmat-1];
      for (i = num_navgmat; i < maxcellmat+1; i++)
        {
         sprintf(str,"Navgdisplab%d",i);
         string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
         navgdisplab[i] = XtVaCreateManagedWidget(str,
                          xmLabelWidgetClass,
                          navgswinform,
                          XmNtopAttachment, XmATTACH_WIDGET,
                          XmNtopWidget, lastwidget,
                          XmNleftAttachment, XmATTACH_FORM,
                          XmNrightAttachment, XmATTACH_NONE,
                          XmNbottomAttachment, XmATTACH_NONE,
                          XmNlabelString, string,
                          NULL);
            XmStringFree(string);
         lastwidget = navgdisplab[i];
        }
      num_navgmat = maxcellmat+1;
     }

   for (i = 0; i < num_navgmat; i++)
     {
      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(navgdisplab[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(navgdisplab[i]);
     }
     
  string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
  XtVaSetValues(navgftotlab,XmNlabelString,string,NULL);
  XmStringFree(string);
  XmUpdateDisplay(navgftotlab);
     
  string = XmStringCreate("              ",XmSTRING_DEFAULT_CHARSET);
  XtVaSetValues(navgwtotlab,XmNlabelString,string,NULL);
  XmStringFree(string);
  XmUpdateDisplay(navgwtotlab);
}



void navggetfield(int arg)
{
  XmString string;

   if (wavgflag == 1)
     {
      string = XmStringCreate(fieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(navgwlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      wavgfld = arg;
     }

   if (favgflag == 1)
     {
      string = XmStringCreate(fieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(navgflab,XmNlabelString,string,NULL);
      XmStringFree(string);
      favgfld = arg;
     }
}
