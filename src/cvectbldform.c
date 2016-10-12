#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "cutplane.h"
#include "lights.h"

#define MAXVAL 9.99999e32

static Widget cvectbldform, cvbldlabx, cvbldlaby, cvbldlabz;
static short xcvflag, ycvflag, zcvflag, xcvfld, ycvfld, zcvfld;


void cvectbldformpop()
{
  XmString string;

   xcvfld = cvectfldx;
   ycvfld = cvectfldy;
   zcvfld = cvectfldz;

   if (cvectfldx >= 0) 
      string = XmStringCreate(cfieldname[cvectfldx],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cvbldlabx,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (cvectfldy >= 0) 
      string = XmStringCreate(cfieldname[cvectfldy],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cvbldlaby,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (cvectfldz >= 0) 
      string = XmStringCreate(cfieldname[cvectfldz],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cvbldlabz,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(cvectbldform);
}


void cvectbldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfieldform);
   XtUnmanageChild(cvectbldform);
}


void cvectbldapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, redraw;
  double *value;
  void cellspeedfill();
  double * readcellvalue(int ifld);

   cvectfldx = xcvfld;
   cvectfldy = ycvfld;
   cvectfldz = zcvfld;

   /*  Calculate cell speed and write.  */
   cellspeedfill();

   /*  If cutplanes or cutspheres exists and cell      */
   /*  vectors requested, reset cutplane cell vectors. */
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      if (cutplane[i].npolys > 0 && cutplane[i].cvelflag && 
          cutplane[i].ndcells > 0)
         cpfillcvects(i);
     }

   /*  If vector mag. is selected for drawing as a  */
   /*  cell face, reset drawing arrays and redraw.  */
   redraw = 0;
   value = readcellvalue(cspeedfld);
   if (cellfieldactive == cspeedfld + numvars && cellsflag)
     {
      redraw = 1;
      for (i = 0; i < ncells; i++)
         cellfield[i] = value[i];
     }
   if (gridfieldactive == cspeedfld + numvars)
     {
      redraw = 1;
      for (i = 0; i < ncells; i++)
         gridfield[i] = value[i];
     }
   free(value);

   fastdraw = 0;
   if (cvectorflag || redraw ||
       (cutplane[i].npolys > 0 && cutplane[i].cvelflag && 
        cutplane[i].ndcells > 0))
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void cvectbldnone(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string;

   arg = (long int)client_data;

   if (arg == 0)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xcvfld = -1;
     }

   if (arg == 1)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      ycvfld = -1;
     }

   if (arg == 2)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zcvfld = -1;
     }
}


void cvectbldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   xcvflag = 0;   ycvflag = 0;   zcvflag = 0;
   if (arg == 0) xcvflag = 1;
   if (arg == 1) ycvflag = 1;
   if (arg == 2) zcvflag = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = xcvfld;
   if (arg == 1) ifld = ycvfld;
   if (arg == 2) ifld = zcvfld;
   cfieldformpop(CFLDVECTBLD, ifld);
}


void makecvectbldform(parent)
 Widget parent;
{
  Widget cvbldclose, cvbldapply, cvbldblank, cvbldlab, cvbldxc, 
         cvbldxnone, cvbldxfld, cvbldyc, cvbldynone, cvbldyfld,
         cvbldzc, cvbldznone, cvbldzfld;
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Build Cell Vector",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cvectbldform = XmCreateFormDialog(parent,"Cvectbldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cvbldclose = XtVaCreateManagedWidget("Cvbldclose",
                xmPushButtonGadgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cvbldclose,XmNactivateCallback,
                    cvectbldclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cvbldapply = XtVaCreateManagedWidget("Cvbldapply",
                xmPushButtonGadgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cvbldclose,
                XmNleftOffset, 40,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cvbldapply,XmNactivateCallback,
                    cvectbldapply,NULL);

   string = XmStringCreateLtoR(" \n ",XmSTRING_DEFAULT_CHARSET);
   cvbldblank = XtVaCreateManagedWidget("Cvbldblank",
                xmLabelWidgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cvbldapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("X-component",XmSTRING_DEFAULT_CHARSET);
   cvbldxc = XtVaCreateManagedWidget("Cvbldxc",
             xmLabelWidgetClass,
             cvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cvbldblank,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   cvbldxnone = XtVaCreateManagedWidget("Cvbldxnone",
                xmPushButtonGadgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cvbldxc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cvbldxnone,XmNactivateCallback,
                    cvectbldnone,(caddr_t) 0);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cvbldxfld = XtVaCreateManagedWidget("Cvbldxfld",
               xmPushButtonGadgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cvbldxnone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cvbldxfld,XmNactivateCallback,
                    cvectbldfld,(caddr_t) 0);

   string = XmStringCreate("Y-component",XmSTRING_DEFAULT_CHARSET);
   cvbldyc = XtVaCreateManagedWidget("Cvbldyc",
             xmLabelWidgetClass,
             cvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cvbldxfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   cvbldynone = XtVaCreateManagedWidget("Cvbldynone",
                xmPushButtonGadgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cvbldyc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cvbldynone,XmNactivateCallback,
                    cvectbldnone,(caddr_t) 1);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cvbldyfld = XtVaCreateManagedWidget("Cvbldyfld",
               xmPushButtonGadgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cvbldynone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cvbldyfld,XmNactivateCallback,
                    cvectbldfld,(caddr_t) 1);

   string = XmStringCreate("Z-component",XmSTRING_DEFAULT_CHARSET);
   cvbldzc = XtVaCreateManagedWidget("Cvbldzc",
             xmLabelWidgetClass,
             cvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cvbldyfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   cvbldznone = XtVaCreateManagedWidget("Cvbldznone",
                xmPushButtonGadgetClass,
                cvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cvbldzc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cvbldznone,XmNactivateCallback,
                    cvectbldnone,(caddr_t) 2);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cvbldzfld = XtVaCreateManagedWidget("Cvbldzfld",
               xmPushButtonGadgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cvbldznone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cvbldzfld,XmNactivateCallback,
                    cvectbldfld,(caddr_t) 2);

   string = XmStringCreateLtoR("Active\n Fields:",
                               XmSTRING_DEFAULT_CHARSET);
   cvbldlab = XtVaCreateManagedWidget("Cvbldlab",
              xmLabelWidgetClass,
              cvectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cvbldapply,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cvbldxc,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   cvbldlabx = XtVaCreateManagedWidget("Cvbldlabx",
               xmLabelWidgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, cvbldxc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cvbldxc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   cvbldlaby = XtVaCreateManagedWidget("Cvbldlaby",
               xmLabelWidgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, cvbldyc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cvbldyc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   cvbldlabz = XtVaCreateManagedWidget("Cvbldlabz",
               xmLabelWidgetClass,
               cvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, cvbldzc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cvbldzc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

}


void fillcellvects()
{
  int i;
  void fillcellvalue(int ifld, double *value);

   /*  Allocate uic, wic, zic and read vector active fields.  */
   cuic = (double *)malloc(ncells*sizeof(double));
   cvic = (double *)malloc(ncells*sizeof(double));
   cwic = (double *)malloc(ncells*sizeof(double));
   if (cuic == NULL || cvic == NULL || cwic == NULL) memerr();

   for (i = 0; i < ncells; i++) cuic[i] = 0;
   if (cvectfldx >= 0)
      fillcellvalue(cvectfldx,cuic);

   for (i = 0; i < ncells; i++) cvic[i] = 0;
   if (cvectfldy >= 0)
      fillcellvalue(cvectfldy,cvic);

   for (i = 0; i < ncells; i++) cwic[i] = 0;
   if (cvectfldz >= 0)
      fillcellvalue(cvectfldz,cwic);

   /*  If vector data filled from subvars,   */
   /*  reset data less than cfieldmin to 0.  */
   if (cvectfldx >= 0 && csubvarsin[cvectfldx])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cuic[i] < cfieldmin[cvectfldx]) cuic[i] = 0.;
        }
     }
   if (cvectfldy >= 0 && csubvarsin[cvectfldy])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cvic[i] < cfieldmin[cvectfldy]) cvic[i] = 0.;
        }
     }
   if (cvectfldz >= 0 && csubvarsin[cvectfldz])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cwic[i] < cfieldmin[cvectfldz]) cwic[i] = 0.;
        }
     }
}


void cellspeedfill()
{
  int i;
  double *spfld, f;
  void fillcellvalue(int ifld, double *value);
  void writecellfldloc(int ifld, double *value);

   /*  Read vector active fields, allocate cuic, cwic, czic if needed.  */
   if (cvectorflag == 0)
     {
      cuic = (double *)malloc(ncells*sizeof(double));
      cvic = (double *)malloc(ncells*sizeof(double));
      cwic = (double *)malloc(ncells*sizeof(double));
      if (cuic == NULL || cvic == NULL || cwic == NULL) memerr();
     }

   cvectors = 1;

   for (i = 0; i < ncells; i++) cuic[i] = 0;
   if (cvectfldx >= 0)
      fillcellvalue(cvectfldx,cuic);

   for (i = 0; i < ncells; i++) cvic[i] = 0;
   if (cvectfldy >= 0)
      fillcellvalue(cvectfldy,cvic);

   for (i = 0; i < ncells; i++) cwic[i] = 0;
   if (cvectfldz >= 0)
      fillcellvalue(cvectfldz,cwic);

   if (cvectfldx < 0 && cvectfldy < 0 && cvectfldz < 0)
      cvectors = 0;

   /*  If vector data filled from subvars,   */
   /*  reset data less than cfieldmin to 0.  */
   if (cvectfldx >= 0 && csubvarsin[cvectfldx])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cuic[i] < cfieldmin[cvectfldx]) cuic[i] = 0.;
        }
     }
   if (cvectfldy >= 0 && csubvarsin[cvectfldy])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cvic[i] < cfieldmin[cvectfldy]) cvic[i] = 0.;
        }
     }
   if (cvectfldz >= 0 && csubvarsin[cvectfldz])
     {
      for (i = 0; i < ncells; i++)
        {
         if (cwic[i] < cfieldmin[cvectfldz]) cwic[i] = 0.;
        }
     }

   /*  Calculate maxspeed and speed if fields exist.  */
   spfld=(double *)malloc(ncells*sizeof(double)); 
   if (spfld == NULL) memerr();
   cmaxspeed = 0;
   for (i = 0; i < ncells; i++)
     {
      spfld[i] = sqrt((cuic[i] * cuic[i]) + 
                      (cvic[i] * cvic[i]) +
                      (cwic[i] * cwic[i]));
      cmaxspeed = (spfld[i] > cmaxspeed) ? 
                   spfld[i] : cmaxspeed;
     }
   if (cvectorflag == 0)
     {  FREE(cuic);  FREE(cvic);  FREE(cwic);  } 

   if (cspeedfld >= 0)
     {
      cfieldmin[cspeedfld] = MAXVAL;
      for (i = 0; i < ncells; i++)
        {
         cfieldmin[cspeedfld] = (spfld[i] < cfieldmin[cspeedfld]) ? 
                                 spfld[i] : cfieldmin[cspeedfld];
        }
      cfieldmax[cspeedfld] = cmaxspeed;
      globfieldcmin[cspeedfld] = cfieldmin[cspeedfld];
      globfieldcmax[cspeedfld] = cfieldmax[cspeedfld];
      if (globfieldcmax[cspeedfld] <= globfieldcmin[cspeedfld])
        { 
         f = fabs(globfieldcmin[cspeedfld]);
         if (f == 0.) f = 1.;
         globfieldcmax[cspeedfld] = globfieldcmin[cspeedfld] + f*0.00001;
         cfieldmax[cspeedfld] = globfieldcmax[cspeedfld];
        }
      cfld_nice_min[cspeedfld] = globfieldcmin[cspeedfld];
      cfld_nice_max[cspeedfld] = globfieldcmax[cspeedfld];

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(cfld_nice_min[cspeedfld], cfld_nice_max[cspeedfld], 
                          cfld_nice[cspeedfld], cfld_log[cspeedfld]);

      writecellfldloc(cspeedfld,spfld);
     }

   free(spfld);
}


void cvectbldgetcfield(int arg)
{
  XmString string;

   if (xcvflag == 1)
     {
      string = XmStringCreate(cfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xcvfld = arg;
     }

   if (ycvflag == 1)
     {
      string = XmStringCreate(cfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      ycvfld = arg;
     }

   if (zcvflag == 1)
     {
      string = XmStringCreate(cfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cvbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zcvfld = arg;
     }
}
