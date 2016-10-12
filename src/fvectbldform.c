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
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

#define MAXVAL 9.99999e32

static Widget fvectbldform, fvbldlabx, fvbldlaby, fvbldlabz;
static short xfvflag, yfvflag, zfvflag, xfvfld, yfvfld, zfvfld;


void fvectbldformpop()
{
  XmString string;

   xfvfld = fvectfldx;
   yfvfld = fvectfldy;
   zfvfld = fvectfldz;

   if (fvectfldx >= 0) 
      string = XmStringCreate(ffieldname[fvectfldx],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fvbldlabx,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (fvectfldy >= 0) 
      string = XmStringCreate(ffieldname[fvectfldy],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fvbldlaby,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (fvectfldz >= 0) 
      string = XmStringCreate(ffieldname[fvectfldz],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(fvbldlabz,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(fvectbldform);
}


void fvectbldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(ffieldform);
   XtUnmanageChild(fvectbldform);
}


void fvectbldapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, redraw;
  double *value;
  void facespeedfill();
  double * readfacevalue(int ifld);

   fvectfldx = xfvfld;
   fvectfldy = yfvfld;
   fvectfldz = zfvfld;

   /*  Calculate face speed and write.  */
   facespeedfill();

   /*  If vector mag. is selected for drawing as a  */
   /*  cell face, reset drawing arrays and redraw.  */
   redraw = 0;
   if (cellfieldactive == fspeedfld + numvars + cnumvars && cellsflag)
     {
      value = readfacevalue(fspeedfld);
      redraw = 1;
      for (i = 0; i < ncellfaces; i++)
         cellfield[i] = value[i];
      free(value);
     }

   fastdraw = 0;
   if (fvectorflag || redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void fvectbldnone(w,client_data,call_data)
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
      XtVaSetValues(fvbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xfvfld = -1;
     }

   if (arg == 1)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fvbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      yfvfld = -1;
     }

   if (arg == 2)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fvbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zfvfld = -1;
     }
}


void fvectbldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   xfvflag = 0;   yfvflag = 0;   zfvflag = 0;
   if (arg == 0) xfvflag = 1;
   if (arg == 1) yfvflag = 1;
   if (arg == 2) zfvflag = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(ffieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = xfvfld;
   if (arg == 1) ifld = yfvfld;
   if (arg == 2) ifld = zfvfld;
   ffieldformpop(FFLDVECTBLD, ifld);
}


void makefvectbldform(parent)
 Widget parent;
{
  Widget fvbldclose, fvbldapply, fvbldblank, fvbldlab, fvbldxc, 
         fvbldxnone, fvbldxfld, fvbldyc, fvbldynone, fvbldyfld,
         fvbldzc, fvbldznone, fvbldzfld;
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Build Face Vector",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fvectbldform = XmCreateFormDialog(parent,"Fvectbldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fvbldclose = XtVaCreateManagedWidget("Fvbldclose",
                xmPushButtonGadgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fvbldclose,XmNactivateCallback,
                    fvectbldclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   fvbldapply = XtVaCreateManagedWidget("Fvbldapply",
                xmPushButtonGadgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, fvbldclose,
                XmNleftOffset, 40,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fvbldapply,XmNactivateCallback,
                    fvectbldapply,NULL);

   string = XmStringCreateLtoR(" \n ",XmSTRING_DEFAULT_CHARSET);
   fvbldblank = XtVaCreateManagedWidget("Fvbldblank",
                xmLabelWidgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, fvbldapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("X-component",XmSTRING_DEFAULT_CHARSET);
   fvbldxc = XtVaCreateManagedWidget("Fvbldxc",
             xmLabelWidgetClass,
             fvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fvbldblank,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   fvbldxnone = XtVaCreateManagedWidget("Fvbldxnone",
                xmPushButtonGadgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, fvbldxc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fvbldxnone,XmNactivateCallback,
                    fvectbldnone,(caddr_t) 0);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fvbldxfld = XtVaCreateManagedWidget("Fvbldxfld",
               xmPushButtonGadgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fvbldxnone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(fvbldxfld,XmNactivateCallback,
                    fvectbldfld,(caddr_t) 0);

   string = XmStringCreate("Y-component",XmSTRING_DEFAULT_CHARSET);
   fvbldyc = XtVaCreateManagedWidget("Fvbldyc",
             xmLabelWidgetClass,
             fvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fvbldxfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   fvbldynone = XtVaCreateManagedWidget("Fvbldynone",
                xmPushButtonGadgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, fvbldyc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fvbldynone,XmNactivateCallback,
                    fvectbldnone,(caddr_t) 1);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fvbldyfld = XtVaCreateManagedWidget("Fvbldyfld",
               xmPushButtonGadgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fvbldynone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(fvbldyfld,XmNactivateCallback,
                    fvectbldfld,(caddr_t) 1);

   string = XmStringCreate("Z-component",XmSTRING_DEFAULT_CHARSET);
   fvbldzc = XtVaCreateManagedWidget("Fvbldzc",
             xmLabelWidgetClass,
             fvectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fvbldyfld,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   fvbldznone = XtVaCreateManagedWidget("Fvbldznone",
                xmPushButtonGadgetClass,
                fvectbldform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, fvbldzc,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fvbldznone,XmNactivateCallback,
                    fvectbldnone,(caddr_t) 2);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   fvbldzfld = XtVaCreateManagedWidget("Fvbldzfld",
               xmPushButtonGadgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, fvbldznone,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(fvbldzfld,XmNactivateCallback,
                    fvectbldfld,(caddr_t) 2);

   string = XmStringCreateLtoR("Active\n Fields:",
                               XmSTRING_DEFAULT_CHARSET);
   fvbldlab = XtVaCreateManagedWidget("Fvbldlab",
              xmLabelWidgetClass,
              fvectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fvbldapply,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, fvbldxc,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   fvbldlabx = XtVaCreateManagedWidget("Fvbldlabx",
               xmLabelWidgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, fvbldxc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, fvbldxc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   fvbldlaby = XtVaCreateManagedWidget("Fvbldlaby",
               xmLabelWidgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, fvbldyc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, fvbldyc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   fvbldlabz = XtVaCreateManagedWidget("Fvbldlabz",
               xmLabelWidgetClass,
               fvectbldform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, fvbldzc,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, fvbldzc,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

}


void fillfacevects()
{
  int i;
  void fillfacevalue(int ifld, double *value);

   /*  Allocate uic, wic, zic and read vector active fields.  */
   fuic = (double *)malloc(ncellfaces*sizeof(double));
   fvic = (double *)malloc(ncellfaces*sizeof(double));
   fwic = (double *)malloc(ncellfaces*sizeof(double));
   if (fuic == NULL || fvic == NULL || fwic == NULL) memerr();

   for (i = 0; i < ncellfaces; i++) fuic[i] = 0;
   if (fvectfldx >= 0)
      fillfacevalue(fvectfldx,fuic);

   for (i = 0; i < ncellfaces; i++) fvic[i] = 0;
   if (fvectfldy >= 0)
      fillfacevalue(fvectfldy,fvic);

   for (i = 0; i < ncellfaces; i++) fwic[i] = 0;
   if (fvectfldz >= 0)
      fillfacevalue(fvectfldz,fwic);

   /*  If vector data filled from subvars,   */
   /*  reset data less than ffieldmin to 0.  */
   if (fvectfldx >= 0 && fsubvarsin[fvectfldx])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fuic[i] < ffieldmin[fvectfldx]) fuic[i] = 0.;
        }
     }
   if (fvectfldy >= 0 && fsubvarsin[fvectfldy])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fvic[i] < ffieldmin[fvectfldy]) fvic[i] = 0.;
        }
     }
   if (fvectfldz >= 0 && fsubvarsin[fvectfldz])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fwic[i] < ffieldmin[fvectfldz]) fwic[i] = 0.;
        }
     }
}


void facespeedfill()
{
  int i;
  double *spfld, f;
  void fillfacevalue(int ifld, double *value);
  void writefacefldloc(int ifld, double *value);

   /*  Read vector active fields, allocate fuic, fwic, czic if needed.  */
   if (fvectorflag == 0)
     {
      fuic = (double *)malloc(ncellfaces*sizeof(double));
      fvic = (double *)malloc(ncellfaces*sizeof(double));
      fwic = (double *)malloc(ncellfaces*sizeof(double));
      if (fuic == NULL || fvic == NULL || fwic == NULL) memerr();
     }

   fvectors = 1;

   for (i = 0; i < ncellfaces; i++) fuic[i] = 0;
   if (fvectfldx >= 0)
      fillfacevalue(fvectfldx,fuic);

   for (i = 0; i < ncellfaces; i++) fvic[i] = 0;
   if (fvectfldy >= 0)
      fillfacevalue(fvectfldy,fvic);

   for (i = 0; i < ncellfaces; i++) fwic[i] = 0;
   if (fvectfldz >= 0)
      fillfacevalue(fvectfldz,fwic);

   if (fvectfldx < 0 && fvectfldy < 0 && fvectfldz < 0)
      fvectors = 0;

   /*  If vector data filled from subvars,   */
   /*  reset data less than ffieldmin to 0.  */
   if (fvectfldx >= 0 && fsubvarsin[fvectfldx])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fuic[i] < ffieldmin[fvectfldx]) fuic[i] = 0.;
        }
     }
   if (fvectfldy >= 0 && fsubvarsin[fvectfldy])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fvic[i] < ffieldmin[fvectfldy]) fvic[i] = 0.;
        }
     }
   if (fvectfldz >= 0 && fsubvarsin[fvectfldz])
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (fwic[i] < ffieldmin[fvectfldz]) fwic[i] = 0.;
        }
     }

   /*  Calculate maxspeed and speed if fields exist.  */
   spfld=(double *)malloc(ncellfaces*sizeof(double)); 
   if (spfld == NULL) memerr();
   fmaxspeed = 0;
   for (i = 0; i < ncellfaces; i++)
     {
      spfld[i] = sqrt((fuic[i] * fuic[i]) + 
                      (fvic[i] * fvic[i]) +
                      (fwic[i] * fwic[i]));
      fmaxspeed = (spfld[i] > fmaxspeed) ? 
                   spfld[i] : fmaxspeed;
     }
   if (fvectorflag == 0)
     {  FREE(fuic);  FREE(fvic);  FREE(fwic);  } 

   if (fspeedfld >= 0)
     {
      ffieldmin[fspeedfld] = MAXVAL;
      for (i = 0; i < ncellfaces; i++)
        {
         ffieldmin[fspeedfld] = (spfld[i] < ffieldmin[fspeedfld]) ? 
                                 spfld[i] : ffieldmin[fspeedfld];
        }
      ffieldmax[fspeedfld] = fmaxspeed;
      globfieldfmin[fspeedfld] = ffieldmin[fspeedfld];
      globfieldfmax[fspeedfld] = ffieldmax[fspeedfld];
      if (globfieldfmax[fspeedfld] <= globfieldfmin[fspeedfld])
        { 
         f = fabs(globfieldfmin[fspeedfld]);
         if (f == 0.) f = 1.;
         globfieldfmax[fspeedfld] = globfieldfmin[fspeedfld] + f*0.00001;
         ffieldmax[fspeedfld] = globfieldfmax[fspeedfld];
        }
      ffld_nice_min[fspeedfld] = globfieldfmin[fspeedfld];
      ffld_nice_max[fspeedfld] = globfieldfmax[fspeedfld];

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(ffld_nice_min[fspeedfld], ffld_nice_max[fspeedfld], 
                          ffld_nice[fspeedfld], ffld_log[fspeedfld]);

      writefacefldloc(fspeedfld,spfld);
     }

   free(spfld);
}


void fvectbldgetffield(int arg)
{
  XmString string;

   if (xfvflag == 1)
     {
      string = XmStringCreate(ffieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fvbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xfvfld = arg;
     }

   if (yfvflag == 1)
     {
      string = XmStringCreate(ffieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fvbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      yfvfld = arg;
     }

   if (zfvflag == 1)
     {
      string = XmStringCreate(ffieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(fvbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zfvfld = arg;
     }
}
