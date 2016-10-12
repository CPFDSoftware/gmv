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

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "cutplane.h"
#include "lights.h"

#define MAXVAL 9.99999e32

static Widget svectbldform, svbldlabx, svbldlaby, svbldlabz;
static short xsvflag, ysvflag, zsvflag, xsvfld, ysvfld, zsvfld;


void svectbldformpop()
{
  XmString string;

   xsvfld = svectfldx;
   ysvfld = svectfldy;
   zsvfld = svectfldz;

   if (svectfldx >= 0) 
      string = XmStringCreate(sfieldname[svectfldx],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(svbldlabx,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (svectfldy >= 0) 
      string = XmStringCreate(sfieldname[svectfldy],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(svbldlaby,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (svectfldz >= 0) 
      string = XmStringCreate(sfieldname[svectfldz],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(svbldlabz,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(svectbldform);
}


void svectbldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(sfieldform);
   XtUnmanageChild(svectbldform);
}


void svectbldapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, redraw;
  double *value;
  void surfspeedfill();
  double * readsurfvalue(int ifld);

   svectfldx = xsvfld;
   svectfldy = ysvfld;
   svectfldz = zsvfld;

   /*  Calculate cell speed and write.  */
   surfspeedfill();

   /*  If vector mag. is selected for drawing as a  */
   /*  surface, reset drawing arrays and redraw.    */
   redraw = 0;
   if (surffieldactive == sspeedfld + numvars && surfflag)
     {
      value = readsurfvalue(sspeedfld);
      redraw = 1;
      for (i = 0; i < nsurf; i++)
         surffield[i] = value[i];
      free(value);
     }

   fastdraw = 0;
   if (svectorflag || redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void svectbldnone(w,client_data,call_data)
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
      XtVaSetValues(svbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xsvfld = -1;
     }

   if (arg == 1)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(svbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      ysvfld = -1;
     }

   if (arg == 2)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(svbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zsvfld = -1;
     }
}


void svectbldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   xsvflag = 0;   ysvflag = 0;   zsvflag = 0;
   if (arg == 0) xsvflag = 1;
   if (arg == 1) ysvflag = 1;
   if (arg == 2) zsvflag = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = xsvfld;
   if (arg == 1) ifld = ysvfld;
   if (arg == 2) ifld = zsvfld;
   sfieldformpop(SFLDVECTBLD, ifld);
}


void makesvectbldform(parent)
 Widget parent;
{
  Widget svbldclose, svbldapply, svbldblank, svbldlab, svbldxc, 
         svbldxnone, svbldxfld, svbldyc, svbldynone, svbldyfld,
         svbldzc, svbldznone, svbldzfld;
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Build Surface Vector",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   svectbldform = XmCreateFormDialog(parent,"Svectbldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   svbldclose = XtVaCreateManagedWidget("Svbldclose",
                xmPushButtonGadgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(svbldclose,XmNactivateCallback,
                    svectbldclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   svbldapply = XtVaCreateManagedWidget("Svbldapply",
                xmPushButtonGadgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, svbldclose,
                XmNleftOffset, 40,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(svbldapply,XmNactivateCallback,
                    svectbldapply,NULL);

   string = XmStringCreateLtoR(" \n ",XmSTRING_DEFAULT_CHARSET);
   svbldblank = XtVaCreateManagedWidget("Svbldblank",
                xmLabelWidgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, svbldapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("X-component",XmSTRING_DEFAULT_CHARSET);
   svbldxc = XtVaCreateManagedWidget("Svbldxc",
             xmLabelWidgetClass,
             svectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, svbldblank,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   svbldxnone = XtVaCreateManagedWidget("Svbldxnone",
                xmPushButtonGadgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, svbldxc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(svbldxnone,XmNactivateCallback,
                    svectbldnone,(caddr_t) 0);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   svbldxfld = XtVaCreateManagedWidget("Svbldxfld",
               xmPushButtonGadgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, svbldxnone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(svbldxfld,XmNactivateCallback,
                    svectbldfld,(caddr_t) 0);

   string = XmStringCreate("Y-component",XmSTRING_DEFAULT_CHARSET);
   svbldyc = XtVaCreateManagedWidget("Svbldyc",
             xmLabelWidgetClass,
             svectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, svbldxfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   svbldynone = XtVaCreateManagedWidget("Svbldynone",
                xmPushButtonGadgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, svbldyc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(svbldynone,XmNactivateCallback,
                    svectbldnone,(caddr_t) 1);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   svbldyfld = XtVaCreateManagedWidget("Svbldyfld",
               xmPushButtonGadgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, svbldynone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(svbldyfld,XmNactivateCallback,
                    svectbldfld,(caddr_t) 1);

   string = XmStringCreate("Z-component",XmSTRING_DEFAULT_CHARSET);
   svbldzc = XtVaCreateManagedWidget("Svbldzc",
             xmLabelWidgetClass,
             svectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, svbldyfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   svbldznone = XtVaCreateManagedWidget("Svbldznone",
                xmPushButtonGadgetClass,
                svectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, svbldzc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(svbldznone,XmNactivateCallback,
                    svectbldnone,(caddr_t) 2);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   svbldzfld = XtVaCreateManagedWidget("Svbldzfld",
               xmPushButtonGadgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, svbldznone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(svbldzfld,XmNactivateCallback,
                    svectbldfld,(caddr_t) 2);

   string = XmStringCreateLtoR("Active\n Fields:",
                               XmSTRING_DEFAULT_CHARSET);
   svbldlab = XtVaCreateManagedWidget("Svbldlab",
              xmLabelWidgetClass,
              svectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, svbldapply,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, svbldxc,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   svbldlabx = XtVaCreateManagedWidget("Svbldlabx",
               xmLabelWidgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, svbldxc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, svbldxc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   svbldlaby = XtVaCreateManagedWidget("Svbldlaby",
               xmLabelWidgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, svbldyc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, svbldyc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   svbldlabz = XtVaCreateManagedWidget("Svbldlabz",
               xmLabelWidgetClass,
               svectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, svbldzc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, svbldzc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

}


void fillsurfvects()
{
  int i;
  void fillsurfvalue(int ifld, double *value);

   /*  Allocate uic, wic, zic and read vector active fields.  */
   suic = (double *)malloc(nsurf*sizeof(double));
   svic = (double *)malloc(nsurf*sizeof(double));
   swic = (double *)malloc(nsurf*sizeof(double));
   if (suic == NULL || svic == NULL || swic == NULL) memerr();

   for (i = 0; i < nsurf; i++) suic[i] = 0;
   if (svectfldx >= 0)
      fillsurfvalue(svectfldx,suic);

   for (i = 0; i < nsurf; i++) svic[i] = 0;
   if (svectfldy >= 0)
      fillsurfvalue(svectfldy,svic);

   for (i = 0; i < nsurf; i++) swic[i] = 0;
   if (svectfldz >= 0)
      fillsurfvalue(svectfldz,swic);
}


void surfspeedfill()
{
  int i;
  double *spfld, f;
  void fillsurfvalue(int ifld, double *value);
  void writesurffldloc(int ifld, double *value);

   /*  Read vector active fields, allocate suic, swic, czic if needed.  */
   if (svectorflag == 0)
     {
      suic = (double *)malloc(nsurf*sizeof(double));
      svic = (double *)malloc(nsurf*sizeof(double));
      swic = (double *)malloc(nsurf*sizeof(double));
      if (suic == NULL || svic == NULL || swic == NULL) memerr();
     }

   svectors = 1;

   for (i = 0; i < nsurf; i++) suic[i] = 0;
   if (svectfldx >= 0)
      fillsurfvalue(svectfldx,suic);

   for (i = 0; i < nsurf; i++) svic[i] = 0;
   if (svectfldy >= 0)
      fillsurfvalue(svectfldy,svic);

   for (i = 0; i < nsurf; i++) swic[i] = 0;
   if (svectfldz >= 0)
      fillsurfvalue(svectfldz,swic);

   if (svectfldx < 0 && svectfldy < 0 && svectfldz < 0)
      svectors = 0;

   /*  Calculate maxspeed and speed if fields exist.  */
   spfld=(double *)malloc(nsurf*sizeof(double)); 
   if (spfld == NULL) memerr();
   smaxspeed = 0;
   for (i = 0; i < nsurf; i++)
     {
      spfld[i] = sqrt((suic[i] * suic[i]) + 
                      (svic[i] * svic[i]) +
                      (swic[i] * swic[i]));
      smaxspeed = (spfld[i] > smaxspeed) ? 
                   spfld[i] : smaxspeed;
     }
   if (svectorflag == 0)
     {  FREE(suic);  FREE(svic);  FREE(swic);  } 

   if (sspeedfld >= 0)
     {
      sfieldmin[sspeedfld] = MAXVAL;
      for (i = 0; i < nsurf; i++)
        {
         sfieldmin[sspeedfld] = (spfld[i] < sfieldmin[sspeedfld]) ? 
                                 spfld[i] : sfieldmin[sspeedfld];
        }
      sfieldmax[sspeedfld] = smaxspeed;
      globfieldsmin[sspeedfld] = sfieldmin[sspeedfld];
      globfieldsmax[sspeedfld] = sfieldmax[sspeedfld];
      if (globfieldsmax[sspeedfld] <= globfieldsmin[sspeedfld])
        { 
         f = fabs(globfieldsmin[sspeedfld]);
         if (f == 0.) f = 1.;
         globfieldsmax[sspeedfld] = globfieldsmin[sspeedfld] + f*0.00001;
         sfieldmax[sspeedfld] = globfieldsmax[sspeedfld];
        }
      sfld_nice_min[sspeedfld] = globfieldsmin[sspeedfld];
      sfld_nice_max[sspeedfld] = globfieldsmax[sspeedfld];

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(sfld_nice_min[sspeedfld], sfld_nice_max[sspeedfld], 
                          sfld_nice[sspeedfld], sfld_log[sspeedfld]);

      writesurffldloc(sspeedfld,spfld);
     }

   free(spfld);
}


void svectbldgetsfield(int arg)
{
  XmString string;

   if (xsvflag == 1)
     {
      string = XmStringCreate(sfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(svbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xsvfld = arg;
     }

   if (ysvflag == 1)
     {
      string = XmStringCreate(sfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(svbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      ysvfld = arg;
     }

   if (zsvflag == 1)
     {
      string = XmStringCreate(sfieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(svbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zsvfld = arg;
     }
}
