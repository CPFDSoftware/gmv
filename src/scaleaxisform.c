#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/PushBG.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"


#define MAXVAL 9.99999e32

static Widget objxsaxis, objysaxis, objzsaxis;


void scaleaxisformpop()
{
  char str[20];

   sprintf(str,"%#9.3g",xscaleaxis);
   XmTextFieldSetString(objxsaxis,str);
 
   sprintf(str,"%#9.3g",yscaleaxis);
   XmTextFieldSetString(objysaxis,str);
 
   sprintf(str,"%#9.3g",zscaleaxis);
   XmTextFieldSetString(objzsaxis,str);

   MyManageChild(scaleaxisform);   
}

 
void scaleaxis(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ierr, ierrx, ierry, ierrz, arg;
  float val, valx, valy, valz;
  char *tmpstr;
  void setscaleaxis(short icall);
 
   arg = (long int)client_data;
 
   if (arg < 3)
     {
      ierr = 0;
      tmpstr = XmTextFieldGetString(w);
      val = atof(tmpstr);
      XtFree(tmpstr);
      /* if (val < .00001) ierr = 1; */
      /* if (val > 100000.) ierr = 1; */
      if (val < 1.0e-10) ierr = 1;
      if (val > 1.0e10) ierr = 1;
      if (ierr == 1) XmTextFieldSetString(w,"Error");
     }
 
   if (arg == 3)
     {
      ierrx = 0;
      tmpstr = XmTextFieldGetString(objxsaxis);
      valx = atof(tmpstr);
      XtFree(tmpstr);
      /* if (valx < .00001) ierrx = 1; */
      /* if (valx > 100000.) ierrx = 1; */
      if (valx < 1.0e-10) ierr = 1;
      if (valx > 1.0e10) ierr = 1;
      if (ierrx == 1) XmTextFieldSetString(objxsaxis,"Error");
      ierry = 0;
      tmpstr = XmTextFieldGetString(objysaxis);
      valy = atof(tmpstr);
      XtFree(tmpstr);
      /* if (valy < .00001) ierry = 1; */
      /* if (valy > 100000.) ierry = 1; */
      if (valy < 1.0e-10) ierry = 1;
      if (valy > 1.0e10) ierry = 1;
      if (ierry == 1) XmTextFieldSetString(objysaxis,"Error");
      ierrz = 0;
      tmpstr = XmTextFieldGetString(objzsaxis);
      valz = atof(tmpstr);
      XtFree(tmpstr);
      /* if (valz < .00001) ierrz = 1; */
      /* if (valz > 100000.) ierrz = 1; */
      if (valz < 1.0e-10) ierrz = 1;
      if (valz > 1.0e10) ierrz = 1;
      if (ierrz == 1) XmTextFieldSetString(objzsaxis,"Error");
 
      ierr = ierrx + ierry + ierrz;
      if (ierr == 0)
        {
         xscaleaxiso = xscaleaxis;
         yscaleaxiso = yscaleaxis;
         zscaleaxiso = zscaleaxis;
         xscaleaxis = valx;
         yscaleaxis = valy;
         zscaleaxis = valz;
         setscaleaxis(0);
         if (displistflag) drawnewlistflag = 1;
         drawnew3dflag = 1;
         fastdraw = 0;
         drawit();
        }
 
     }
 
   if (arg == 4)
     {
      XtUnmanageChild(scaleaxisform);
     } 
}

void makescaleaxisform(parent)
 Widget parent;
{
  Widget xaxslab, yaxslab, zaxslab, scaleaxdoit, scaleaxclose;
  Arg wargs[20];
  int i;
  char str[20];
  XmString string;
 
 
   string = XmStringCreate("Scale Axes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   scaleaxisform = XmCreateFormDialog(parent,"ScaleAxis",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("X Scale Factor:",
                           XmSTRING_DEFAULT_CHARSET);
   xaxslab = XtVaCreateManagedWidget("Xaxslab",xmLabelWidgetClass,
             scaleaxisform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
   sprintf(str,"%#9.3g",xscaleaxis);
   objxsaxis = XtVaCreateManagedWidget("Objxsaxis",
               xmTextFieldWidgetClass,
               scaleaxisform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, xaxslab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 12,
               XmNcolumns, 12,
               XmNvalue, str,
               NULL);
      XmStringFree(string);
      XtAddCallback(objxsaxis,XmNactivateCallback,scaleaxis,
                    (caddr_t) 0);
 
   string = XmStringCreate("Y Scale Factor:",
                           XmSTRING_DEFAULT_CHARSET);
   yaxslab = XtVaCreateManagedWidget("Yaxslab",xmLabelWidgetClass,
             scaleaxisform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objxsaxis,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
   sprintf(str,"%#9.3g",yscaleaxis);
   objysaxis = XtVaCreateManagedWidget("Objysaxis",
               xmTextFieldWidgetClass,
               scaleaxisform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, objxsaxis,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, yaxslab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 12,
               XmNcolumns, 12,
               XmNvalue, str,
               NULL);
      XmStringFree(string);
      XtAddCallback(objysaxis,XmNactivateCallback,scaleaxis,
                    (caddr_t) 1);
 
   string = XmStringCreate("Z Scale Factor:",
                           XmSTRING_DEFAULT_CHARSET);
   zaxslab = XtVaCreateManagedWidget("Zaxslab",xmLabelWidgetClass,
             scaleaxisform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objysaxis,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
   sprintf(str,"%#9.3g",zscaleaxis);
   objzsaxis = XtVaCreateManagedWidget("Objzsaxis",
               xmTextFieldWidgetClass,
               scaleaxisform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, objysaxis,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, zaxslab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 12,
               XmNcolumns, 12,
               XmNvalue, str,
               NULL);
      XmStringFree(string);
      XtAddCallback(objzsaxis,XmNactivateCallback,scaleaxis,
                    (caddr_t) 2);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   scaleaxdoit = XtVaCreateManagedWidget("Scaleaxdoit",
                 xmPushButtonGadgetClass,
                 scaleaxisform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, objzsaxis,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(scaleaxdoit,XmNactivateCallback,scaleaxis,
                    (caddr_t) 3);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   scaleaxclose = XtVaCreateManagedWidget("Scaleaxclose",
                 xmPushButtonGadgetClass,
                 scaleaxisform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, objzsaxis,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, scaleaxdoit,
                 XmNleftOffset, 40,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(scaleaxclose,XmNactivateCallback,scaleaxis,
                    (caddr_t) 4);
 
}

 
void setscaleaxis(short icall)
 
{
  int i, j, k, m;
  long nv;
  char str[10];
  double xdif, ydif, zdif;
  void clminradcalc();
 
   /*  Scale polygon vertex data, if any.  */
   if (polysin)
     {
      if (polysflag == 0 && linesflag == 0)
         readpolyfile();
      for (i = 0; i < maxmatno; i++)
        {
 
         for (j = 0; j < npolyspermat[i]; j ++)
           {
 
            m = (int)polygons[i][j].nverts;
            nv = polygons[i][j].polyvertloc;
            for (k = 0; k < m; k++)
              {
               polyverts[i][k+nv].x /= xscaleaxiso;
               polyverts[i][k+nv].y /= yscaleaxiso;
               polyverts[i][k+nv].z /= zscaleaxiso;
               polyverts[i][k+nv].x *= xscaleaxis;
               polyverts[i][k+nv].y *= yscaleaxis;
               polyverts[i][k+nv].z *= zscaleaxis;
              } 
           }
        }

      /*  Write out polygon data and release memory.  */
      writepolyfile();

      /*  Read polygon data if polygons are displayed.  */
      if (polysflag || linesflag)
         readpolyfile();
     }
 
   /*  Scale node coordinates.  */
 
   for (i = 1; i < nnodes+1; i++)
     {
      nodes[i].x /= xscaleaxiso;
      nodes[i].y /= yscaleaxiso;
      nodes[i].z /= zscaleaxiso;
      nodes[i].x *= xscaleaxis;
      nodes[i].y *= yscaleaxis;
      nodes[i].z *= zscaleaxis;
     }
 
   /*  Scale cell avgs.  */
 
   if (structuredflag == 0)
     {
      for (i = 0; i < ncells; i++)
        {
         cells[i].xavg /= xscaleaxiso;
         cells[i].yavg /= yscaleaxiso;
         cells[i].zavg /= zscaleaxiso;
         cells[i].xavg *= xscaleaxis;
         cells[i].yavg *= yscaleaxis;
         cells[i].zavg *= zscaleaxis;
        }  
      }

   /*  Scale surface avgs.  */
 
   if (nsurf > 0)
     {
      for (i = 0; i < nsurf; i++)
        {
         surfs[i].xavg /= xscaleaxiso;
         surfs[i].yavg /= yscaleaxiso;
         surfs[i].zavg /= zscaleaxiso;
         surfs[i].xavg *= xscaleaxis;
         surfs[i].yavg *= yscaleaxis;
         surfs[i].zavg *= zscaleaxis;
        }  
      }

   /*  Scale tracers.  */

   if (ntracers > 0)
     {
      for (i = 0; i < ntracers; i++)
        {
         tracers[0].x[i] /= xscaleaxiso;
         tracers[0].y[i] /= yscaleaxiso;
         tracers[0].z[i] /= zscaleaxiso;
         tracers[0].x[i] *= xscaleaxis;
         tracers[0].y[i] *= yscaleaxis;
         tracers[0].z[i] *= zscaleaxis;
        }

      /*  See if there's any history tracers.  */
      if (ntracehist > 1)
        {
         for (j = 1; j < ntracehist; j++)
           {
            for (i = 0; i < ntracers; i++)
              {
               tracers[j].x[i] /= xscaleaxiso;
               tracers[j].y[i] /= yscaleaxiso;
               tracers[j].z[i] /= zscaleaxiso;
               tracers[j].x[i] *= xscaleaxis;
               tracers[j].y[i] *= yscaleaxis;
               tracers[j].z[i] *= zscaleaxis;
              }
            
           }
        }
     }

   /*  Reset scaleaxis data.  */
 
   if (movieflag == 0)
     {
      sprintf(str,"%#9.3g",xscaleaxis);
      XmTextFieldSetString(objxsaxis,str);
 
      sprintf(str,"%#9.3g",yscaleaxis);
      XmTextFieldSetString(objysaxis,str);
 
      sprintf(str,"%#9.3g",zscaleaxis);
      XmTextFieldSetString(objzsaxis,str);
     }
 
   /*  Reset global mins, maxs and picture scale  */
   /*  data, unless called from getattributes.    */
 
   if (icall == 0)
     {

      /*  Recalculate clminradius, clmaxradius and search info.  */
      clminradcalc();

      /*  Reset globals from original mins  */
      /*   and maxes times scaleaxis.       */
      globxicmin = allxmin * xscaleaxis;
      globyicmin = allymin * yscaleaxis;
      globzicmin = allzmin * zscaleaxis;
      globxicmax = allxmax * xscaleaxis;
      globyicmax = allymax * yscaleaxis;
      globzicmax = allzmax * zscaleaxis;

      xdif = fabs(globxicmax - globxicmin);
      ydif = fabs(globyicmax - globyicmin);
      zdif = fabs(globzicmax - globzicmin);
      globxicmin = globxicmin - .01 * xdif;
      globyicmin = globyicmin - .01 * ydif;
      globzicmin = globzicmin - .01 * zdif;
      globxicmax = globxicmax + .01 * xdif;
      globyicmax = globyicmax + .01 * ydif;
      globzicmax = globzicmax + .01 * zdif;

      xrange = globxicmax - globxicmin;
      yrange = globyicmax - globyicmin;
      zrange = globzicmax - globzicmin;
      span = sqrt(xrange*xrange + yrange*yrange + zrange*zrange);
 
      globxicmino = globxicmin; globxicmaxo = globxicmax;
      globyicmino = globyicmin; globyicmaxo = globyicmax;
      globzicmino = globzicmin; globzicmaxo = globzicmax;
 
      near_clip = 1000.0*span;
      far_clip = -0.1*span;
 
      xtrans = -0.5*(globxicmax+globxicmin);
      ytrans = -0.5*(globyicmax+globyicmin);
      ztrans = -0.5*(globzicmax+globzicmin);
 
      xminbdval /= xscaleaxiso;
      xmaxbdval /= xscaleaxiso;
      yminbdval /= yscaleaxiso;
      ymaxbdval /= yscaleaxiso;
      zminbdval /= zscaleaxiso;
      zmaxbdval /= zscaleaxiso;
      xminbdval *= xscaleaxis;
      xmaxbdval *= xscaleaxis;
      yminbdval *= yscaleaxis;
      ymaxbdval *= yscaleaxis;
      zminbdval *= zscaleaxis;
      zmaxbdval *= zscaleaxis;

      xminfdval /= xscaleaxiso;
      xmaxfdval /= xscaleaxiso;
      yminfdval /= yscaleaxiso;
      ymaxfdval /= yscaleaxiso;
      zminfdval /= zscaleaxiso;
      zmaxfdval /= zscaleaxiso; 
      xminfdval *= xscaleaxis;
      xmaxfdval *= xscaleaxis;
      yminfdval *= yscaleaxis;
      ymaxfdval *= yscaleaxis;
      zminfdval *= zscaleaxis;
      zmaxfdval *= zscaleaxis;

      xminbbval /= xscaleaxiso;
      xmaxbbval /= xscaleaxiso;
      yminbbval /= yscaleaxiso;
      ymaxbbval /= yscaleaxiso;
      zminbbval /= zscaleaxiso;
      zmaxbbval /= zscaleaxiso; 
      xminbbval *= xscaleaxis;
      xmaxbbval *= xscaleaxis;
      yminbbval *= yscaleaxis;
      ymaxbbval *= yscaleaxis;
      zminbbval *= zscaleaxis;
      zmaxbbval *= zscaleaxis;

      xminbbvalo /= xscaleaxiso;
      xmaxbbvalo /= xscaleaxiso;
      yminbbvalo /= yscaleaxiso;
      ymaxbbvalo /= yscaleaxiso;
      zminbbvalo /= zscaleaxiso;
      zmaxbbvalo /= zscaleaxiso; 
      xminbbvalo *= xscaleaxis;
      xmaxbbvalo *= xscaleaxis;
      yminbbvalo *= yscaleaxis;
      ymaxbbvalo *= yscaleaxis;
      zminbbvalo *= zscaleaxis;
      zmaxbbvalo *= zscaleaxis;

      nodeselbox_xmin /= xscaleaxiso;
      nodeselbox_xmax /= xscaleaxiso;
      nodeselbox_ymin /= yscaleaxiso;
      nodeselbox_ymax /= yscaleaxiso;
      nodeselbox_zmin /= zscaleaxiso;
      nodeselbox_zmax /= zscaleaxiso;
      nodeselbox_xmin *= xscaleaxis;
      nodeselbox_xmax *= xscaleaxis;
      nodeselbox_ymin *= yscaleaxis;
      nodeselbox_ymax *= yscaleaxis;
      nodeselbox_zmin *= zscaleaxis;
      nodeselbox_zmax *= zscaleaxis;

      cellselbox_xmin /= xscaleaxiso;
      cellselbox_xmax /= xscaleaxiso;
      cellselbox_ymin /= yscaleaxiso;
      cellselbox_ymax /= yscaleaxiso;
      cellselbox_zmin /= zscaleaxiso;
      cellselbox_zmax /= zscaleaxiso;
      cellselbox_xmin *= xscaleaxis;
      cellselbox_xmax *= xscaleaxis;
      cellselbox_ymin *= yscaleaxis;
      cellselbox_ymax *= yscaleaxis;
      cellselbox_zmin *= zscaleaxis;
      cellselbox_zmax *= zscaleaxis;

      surfselbox_xmin /= xscaleaxiso;
      surfselbox_xmax /= xscaleaxiso;
      surfselbox_ymin /= yscaleaxiso;
      surfselbox_ymax /= yscaleaxiso;
      surfselbox_zmin /= zscaleaxiso;
      surfselbox_zmax /= zscaleaxiso;
      surfselbox_xmin *= xscaleaxis;
      surfselbox_xmax *= xscaleaxis;
      surfselbox_ymin *= yscaleaxis;
      surfselbox_ymax *= yscaleaxis;
      surfselbox_zmin *= zscaleaxis;
      surfselbox_zmax *= zscaleaxis;

      traceselbox_xmin /= xscaleaxiso;
      traceselbox_xmax /= xscaleaxiso;
      traceselbox_ymin /= yscaleaxiso;
      traceselbox_ymax /= yscaleaxiso;
      traceselbox_zmin /= zscaleaxiso;
      traceselbox_zmax /= zscaleaxiso;
      traceselbox_xmin *= xscaleaxis;
      traceselbox_xmax *= xscaleaxis;
      traceselbox_ymin *= yscaleaxis;
      traceselbox_ymax *= yscaleaxis;
      traceselbox_zmin *= zscaleaxis;
      traceselbox_zmax *= zscaleaxis;

      nodeselsph_x /= xscaleaxiso;
      nodeselsph_y /= yscaleaxiso;
      nodeselsph_z /= zscaleaxiso;
      nodeselsph_x *= xscaleaxis;
      nodeselsph_y *= yscaleaxis;
      nodeselsph_z *= zscaleaxis;

      cellselsph_x /= xscaleaxiso;
      cellselsph_y /= yscaleaxiso;
      cellselsph_z /= zscaleaxiso;
      cellselsph_x *= xscaleaxis;
      cellselsph_y *= yscaleaxis;
      cellselsph_z *= zscaleaxis;

      surfselsph_x /= xscaleaxiso;
      surfselsph_y /= yscaleaxiso;
      surfselsph_z /= zscaleaxiso;
      surfselsph_x *= xscaleaxis;
      surfselsph_y *= yscaleaxis;
      surfselsph_z *= zscaleaxis;

      traceselsph_x /= xscaleaxiso;
      traceselsph_y /= yscaleaxiso;
      traceselsph_z /= zscaleaxiso;
      traceselsph_x *= xscaleaxis;
      traceselsph_y *= yscaleaxis;
      traceselsph_z *= zscaleaxis;
 
      /*  Reset widgets.  */

      setcenterwidgets();
      setsubsetwidgets();
      setboundingboxwidgets();
      if (nnodes > 0)
         setnodeselwidgets();
      if (ncells > 0)
         setcellselwidgets();
      if (nsurf > 0)
         setsurfselwidgets();
      /* setnodecellsubset(); */

      if (polysin > 0)
         setpolysubsetwidgets();

      /*  Set animation subset widgets.  */
      getanimsublims();
      animsubfill();

      /*  Reset box draw flags.  */
      subsetbox_draw = 0;
      cellselbox_draw = 0;
      nodeselbox_draw = 0;
      surfselbox_draw = 0;
      polyselbox_draw = 0;
      traceselbox_draw = 0;

      /*  Set animation cutplane limits.  */
      if (ncells > 0 && numvars > 0)
        {
         getanimcplims();
         animcp_set_dirval();
        }
     }
}
