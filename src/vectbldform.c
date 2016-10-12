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
#include "lights.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

static Widget vectbldform, vbldlabx, vbldlaby, vbldlabz;
static short xvflag, yvflag, zvflag, xvfld, yvfld, zvfld;


void vectbldformpop()
{
  XmString string;

   xvfld = vectfldx;
   yvfld = vectfldy;
   zvfld = vectfldz;

   if (vectfldx >= 0) 
      string = XmStringCreate(fieldname[vectfldx],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(vbldlabx,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (vectfldy >= 0) 
      string = XmStringCreate(fieldname[vectfldy],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(vbldlaby,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (vectfldz >= 0) 
      string = XmStringCreate(fieldname[vectfldz],
                              XmSTRING_DEFAULT_CHARSET);
   else string = XmStringCreate("NONE",
                                XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(vbldlabz,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(vectbldform);
}


void vectbldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(vectbldform);
}


void vectbldapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, cpvects, redraw;
  double *value;
  void nodespeedfill();
  double * readnodevalue(int ifld);

   vectfldx = xvfld;
   vectfldy = yvfld;
   vectfldz = zvfld;

   /*  Calculate node speed and write.  */
   nodespeedfill();

   /*  If cutplane exists and node vectors      */
   /*  requested, reset cutplane node vectors.  */
   cpvects = 0;
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      if (cutplane[i].npolys > 0 && cutplane[i].velflag && 
          cutplane[i].ndnodes > 0)
        {
         cpfillnvects(i);
         cpvects = 1;
        }
     }

   /*  If vector magnitude is selected for drawing as a     */
   /*  node or cell face, reset drawing arrays and redraw.  */
   redraw = 0;
   if (nodefieldactive == speedfld || cellfieldactive == speedfld || 
       gridfieldactive == speedfld)
     {
      value = readnodevalue(speedfld);
 
      if (nodefieldactive == speedfld && nodesflag > 0)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            nodefield[i] = value[i];
        }
      if (cellfieldactive == speedfld && cellsflag)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            cellfield[i] = value[i];
        }
      if (gridfieldactive == speedfld)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            gridfield[i] = value[i];
        }
      if (surffieldactive == speedfld && surfflag)
        {
         redraw = 1;
         for (i = 1; i < nnodes+1; i++)
            surffield[i] = value[i];
         setsurfnfldlab(surffieldactive);
        }

      free(value);
     }

   fastdraw = 0;
   if (vectorflag || cpvects || redraw)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      drawit();
     }
}


void vectbldnone(w,client_data,call_data)
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
      XtVaSetValues(vbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xvfld = -1;
     }

   if (arg == 1)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(vbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      yvfld = -1;
     }

   if (arg == 2)
     {
      string = XmStringCreate("NONE",
                                 XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(vbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zvfld = -1;
     }
}


void vectbldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   arg = (long int)client_data;

   /*  Set current component flag.  */
   xvflag = 0;   yvflag = 0;   zvflag = 0;
   if (arg == 0) xvflag = 1;
   if (arg == 1) yvflag = 1;
   if (arg == 2) zvflag = 1;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   if (arg == 0) ifld = xvfld;
   if (arg == 1) ifld = yvfld;
   if (arg == 2) ifld = zvfld;
   nfieldformpop(NFLDVECTBLD, ifld);
}


void makevectbldform(parent)
 Widget parent;
{
  Widget vbldclose, vbldapply, vbldblank, vbldlab, vbldxc, 
         vbldxnone, vbldxfld, vbldyc, vbldynone, vbldyfld,
         vbldzc, vbldznone, vbldzfld;
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Build Node Vector",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   vectbldform = XmCreateFormDialog(parent,"Vectbldform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   vbldclose = XtVaCreateManagedWidget("Vbldclose",
                  xmPushButtonGadgetClass,
                  vectbldform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(vbldclose,XmNactivateCallback,
                    vectbldclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   vbldapply = XtVaCreateManagedWidget("Vbldapply",
               xmPushButtonGadgetClass,
               vectbldform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, vbldclose,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(vbldapply,XmNactivateCallback,
                    vectbldapply,NULL);

   string = XmStringCreateLtoR(" \n ",XmSTRING_DEFAULT_CHARSET);
   vbldblank = XtVaCreateManagedWidget("Vbldblank",
               xmLabelWidgetClass,
               vectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, vbldapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("X-component",XmSTRING_DEFAULT_CHARSET);
   vbldxc = XtVaCreateManagedWidget("Vbldxc",
            xmLabelWidgetClass,
            vectbldform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, vbldblank,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   vbldxnone = XtVaCreateManagedWidget("Vbldxnone",
               xmPushButtonGadgetClass,
               vectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, vbldxc,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(vbldxnone,XmNactivateCallback,
                    vectbldnone,(caddr_t) 0);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   vbldxfld = XtVaCreateManagedWidget("Vbldxfld",
              xmPushButtonGadgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, vbldxnone,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(vbldxfld,XmNactivateCallback,
                    vectbldfld,(caddr_t) 0);

   string = XmStringCreate("Y-component",XmSTRING_DEFAULT_CHARSET);
   vbldyc = XtVaCreateManagedWidget("Vbldyc",
            xmLabelWidgetClass,
            vectbldform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, vbldxfld,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   vbldynone = XtVaCreateManagedWidget("Vbldynone",
               xmPushButtonGadgetClass,
               vectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, vbldyc,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(vbldynone,XmNactivateCallback,
                    vectbldnone,(caddr_t) 1);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   vbldyfld = XtVaCreateManagedWidget("Vbldyfld",
              xmPushButtonGadgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, vbldynone,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(vbldyfld,XmNactivateCallback,
                    vectbldfld,(caddr_t) 1);

   string = XmStringCreate("Z-component",XmSTRING_DEFAULT_CHARSET);
   vbldzc = XtVaCreateManagedWidget("Vbldzc",
            xmLabelWidgetClass,
            vectbldform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, vbldyfld,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("NONE",XmSTRING_DEFAULT_CHARSET);
   vbldznone = XtVaCreateManagedWidget("Vbldznone",
               xmPushButtonGadgetClass,
               vectbldform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, vbldzc,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(vbldznone,XmNactivateCallback,
                    vectbldnone,(caddr_t) 2);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   vbldzfld = XtVaCreateManagedWidget("Vbldzfld",
              xmPushButtonGadgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, vbldznone,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(vbldzfld,XmNactivateCallback,
                    vectbldfld,(caddr_t) 2);

   string = XmStringCreateLtoR("Active\n Fields:",
                               XmSTRING_DEFAULT_CHARSET);
   vbldlab = XtVaCreateManagedWidget("Vbldlab",
             xmLabelWidgetClass,
             vectbldform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, vbldapply,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, vbldxc,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   vbldlabx = XtVaCreateManagedWidget("Vbldlabx",
              xmLabelWidgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, vbldxc,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, vbldxc,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   vbldlaby = XtVaCreateManagedWidget("Vbldlaby",
              xmLabelWidgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, vbldyc,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, vbldyc,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("        ",
                            XmSTRING_DEFAULT_CHARSET);
   vbldlabz = XtVaCreateManagedWidget("Vbldlabz",
              xmLabelWidgetClass,
              vectbldform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, vbldzc,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, vbldzc,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

}


void fillnodevects()
{
  int i;
  void fillnodevalue(int ifld, double *value);

   /*  Allocate uic, wic, zic and read vector active fields.  */
   uic = (double *)malloc((nnodes+1)*sizeof(double));
   vic = (double *)malloc((nnodes+1)*sizeof(double));
   wic = (double *)malloc((nnodes+1)*sizeof(double));
   if (uic == NULL || vic == NULL || wic == NULL) memerr();

   for (i = 0; i < nnodes+1; i++) uic[i] = 0;
   if (vectfldx >= 0)
      fillnodevalue(vectfldx,uic);

   for (i = 0; i < nnodes+1; i++) vic[i] = 0;
   if (vectfldy >= 0)
      fillnodevalue(vectfldy,vic);

   for (i = 0; i < nnodes+1; i++) wic[i] = 0;
   if (vectfldz >= 0)
      fillnodevalue(vectfldz,wic);

   /*  If vector data filled from subvars,  */
   /*  reset data less than fieldmin to 0.  */
   if (vectfldx >= 0 && nsubvarsin[vectfldx])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (uic[i] < fieldmin[vectfldx]) uic[i] = 0.;
        }
     }
   if (vectfldy >= 0 && nsubvarsin[vectfldy])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (vic[i] < fieldmin[vectfldy]) vic[i] = 0.;
        }
     }
   if (vectfldz >= 0 && nsubvarsin[vectfldz])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (wic[i] < fieldmin[vectfldz]) wic[i] = 0.;
        }
     }
}


void nodespeedfill()
{
  int i;
  double *spfld, f;
  void fillnodevalue(int ifld, double *value);
  void writenodefldloc(int ifld, double *value);

   /*  Read vector active fields, allocate uic, wic, zic if needed.  */
   if (vectorflag == 0)
     {
      uic = (double *)malloc((nnodes+1)*sizeof(double));
      vic = (double *)malloc((nnodes+1)*sizeof(double));
      wic = (double *)malloc((nnodes+1)*sizeof(double));
      if (uic == NULL || vic == NULL || wic == NULL) memerr();
     }

   vectors = 1;

   for (i = 0; i < nnodes+1; i++) uic[i] = 0;
   if (vectfldx >= 0)
      fillnodevalue(vectfldx,uic);

   for (i = 0; i < nnodes+1; i++) vic[i] = 0;
   if (vectfldy >= 0)
      fillnodevalue(vectfldy,vic);

   for (i = 0; i < nnodes+1; i++) wic[i] = 0;
   if (vectfldz >= 0)
      fillnodevalue(vectfldz,wic);

   if (vectfldx < 0 && vectfldy < 0 && vectfldz < 0)
      vectors = 0;

   /*  If vector data filled from subvars,  */
   /*  reset data less than fieldmin to 0.  */
   if (vectfldx >= 0 && nsubvarsin[vectfldx])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (uic[i] < fieldmin[vectfldx]) uic[i] = 0.;
        }
     }
   if (vectfldy >= 0 && nsubvarsin[vectfldy])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (vic[i] < fieldmin[vectfldy]) vic[i] = 0.;
        }
     }
   if (vectfldz >= 0 && nsubvarsin[vectfldz])
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (wic[i] < fieldmin[vectfldz]) wic[i] = 0.;
        }
     }

   /*  Calculate node speed field.  */
   spfld=(double *)malloc((nnodes+1)*sizeof(double)); 
   if (spfld == NULL) memerr();
   maxspeed = 0;
   for (i = 1; i < nnodes+1; i++)
     {
      spfld[i] = sqrt((uic[i] * uic[i]) + 
                      (vic[i] * vic[i]) +
                      (wic[i] * wic[i]));
      maxspeed = (spfld[i] > maxspeed) ? 
                  spfld[i] : maxspeed;
     }
   if (vectorflag == 0)
     {  FREE(uic);  FREE(vic);  FREE(wic);  } 

  if (speedfld >= 0)
     {
      fieldmin[speedfld] = MAXVAL;
      for (i = 1; i < nnodes+1; i++)
        {
         fieldmin[speedfld] = (spfld[i] < fieldmin[speedfld]) ? 
                               spfld[i] : fieldmin[speedfld];
        }
      fieldmax[speedfld] = maxspeed;
      globfieldmin[speedfld] = fieldmin[speedfld];
      globfieldmax[speedfld] = fieldmax[speedfld];
      if (globfieldmax[speedfld] <= globfieldmin[speedfld])
        { 
         f = fabs(globfieldmin[speedfld]);
         if (f == 0.) f = 1.;
         globfieldmax[speedfld] = globfieldmin[speedfld] + f*0.00001;
         fieldmax[speedfld] = globfieldmax[speedfld];
        }
      nfld_nice_min[speedfld] = globfieldmin[speedfld];
      nfld_nice_max[speedfld] = globfieldmax[speedfld];

      /*  If red to blue by contour levels color scale used, reset.  */
      if (fieldcolortype == 3)
         setfldcolortype3(nfld_nice_min[speedfld], nfld_nice_max[speedfld], 
                          nfld_nice[speedfld], nfld_log[speedfld]);

      writenodefldloc(speedfld,spfld);
     }

   free(spfld);
}



void vectbldgetnfield(int arg)
{
  XmString string;

   if (xvflag == 1)
     {
      string = XmStringCreate(fieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(vbldlabx,XmNlabelString,string,NULL);
      XmStringFree(string);
      xvfld = arg;
     }

   if (yvflag == 1)
     {
      string = XmStringCreate(fieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(vbldlaby,XmNlabelString,string,NULL);
      XmStringFree(string);
      yvfld = arg;
     }

   if (zvflag == 1)
     {
      string = XmStringCreate(fieldname[arg],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(vbldlabz,XmNlabelString,string,NULL);
      XmStringFree(string);
      zvfld = arg;
     }
}
