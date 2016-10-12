#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "xstuff.h"
#include "formwidgets.h"

static Widget cexplode, cexpbut[MAXFLAGS+1];
static int oldcellexpflag;



void cellexplodepop()
{
  int i, ival;

   for (i = 0; i <= numflags_cell; i++)
      XtVaSetValues(cexpbut[i],XmNset,FALSE,NULL);
   XtVaSetValues(cexpbut[cellexpflag+1],XmNset,TRUE,NULL);
   ival = (int) (cexplodefact * 100.);
   XmScaleSetValue(cexplode,ival);

   MyManageChild(cellexpform);
}

void ceclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellexpform);
}


void ceapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int ival;
  void cellexpvects();

   XmScaleGetValue(cexplode,&ival);
   cexplodefact = (float)ival / 100.;
   if (oldcellexpflag != cellexpflag)
     {
      oldcellexpflag = cellexpflag;
      cellexpvects();
     }
   if (cellsflag) setfacesel();

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void select_cell_expflag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   cellexpflag = arg - 1;
}


void makecellexpform(parent)
 Widget parent;
{
  Widget cexpclose, cexpapply, cexptxt, cexpbox;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Cell Explode",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellexpform = XmCreateFormDialog(parent,"Cell_Explode",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cexpclose = XtVaCreateManagedWidget("Cexpclose",
               xmPushButtonGadgetClass,
               cellexpform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cexpclose,XmNactivateCallback,ceclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cexpapply = XtVaCreateManagedWidget("Cexpapply",
               xmPushButtonGadgetClass,
               cellexpform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cexpclose,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cexpapply,XmNactivateCallback,ceapply,NULL);

   string = XmStringCreate("Explode %",XmSTRING_DEFAULT_CHARSET);
   cexplode = XtVaCreateManagedWidget("Cexplode",
              xmScaleWidgetClass,
              cellexpform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cexpclose,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNminimum, 0,
              XmNmaximum, 1000,
              XmNorientation, XmHORIZONTAL,
              XmNprocessingDirection, XmMAX_ON_RIGHT,
              XmNvalue, 0,
              XmNscaleWidth, 200,
              XmNscaleMultiple, 1,
              XmNshowValue, TRUE,
              XmNtitleString, string,
              NULL);
      XmStringFree(string);

   /*  Create explode on buttons.  */
 
   string = XmStringCreate("Explode on:",XmSTRING_DEFAULT_CHARSET);
   cexptxt = XtVaCreateManagedWidget("Cexptxt",
             xmLabelWidgetClass,
             cellexpform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cexplode,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cexptxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cexpbox = XmCreateRadioBox(cellexpform,"Cexpbox",wargs,i);
   XtManageChild(cexpbox);
   for (i = 0; i <= numflags_cell; i++)
     {
      if (i == 0) 
         string = XmStringCreate("Mat. No.",
                                 XmSTRING_DEFAULT_CHARSET);
      else 
         string = XmStringCreate(flagname_cell[i-1],
                                 XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Cexpbut%d",i);
      cexpbut[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonGadgetClass,
                    cexpbox,
                    XmNlabelString, string,
                    XmNspacing, 1,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cexpbut[i],XmNvalueChangedCallback,
                       select_cell_expflag,(caddr_t) i);

      if (i == 0) XtVaSetValues(cexpbut[0],XmNset,TRUE,NULL);
     }
   oldcellexpflag = -99;
}


void cellexpvects()

{
  int i, j, icell, jcell, count[MAXMATS], nv, tcount;
  double xt, yt, zt, xsum[MAXMATS], ysum[MAXMATS], zsum[MAXMATS];
  float adj;
  int numpaths = 16, ip, kp;
  int path[16][3] = { {1,0,1}, {0,1,1}, {-1,0,1}, {0,-1,1},
                      {1,0,-1}, {0,1,-1}, {-1,0,-1}, {0,-1,-1},
                      {1,1,1}, {1,-1,1}, {-1,1,1}, {-1,-1,1},
                      {1,1,-1}, {1,-1,-1}, {-1,1,-1}, {-1,-1,-1} };

   /*  Loop through cells and sum x,y,z's  */  
   /*  of the avg. vertex of each cell.    */
   xt = 0;  yt = 0;  zt = 0;
   tcount = 0;
   if (cellexpflag == -1) nv = maxcellmat;
   else nv = flagtypes_cell[cellexpflag];

   for (i = 0; i < MAXMATS; i++)
     {
      count[i] = 0;
      xsum[i] = 0;  ysum[i] = 0;  zsum[i] = 0;
     }

   for (icell = 0; icell < ncells; icell++)
     {
      jcell = icell;
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,0);
        }
      if (cellexpflag == -1) i = (int)cellmats[icell] - 1;
      else i = (int)cellflags[cellexpflag][icell] - 1;

      xsum[i] += cells[jcell].xavg;
      ysum[i] += cells[jcell].yavg;
      zsum[i] += cells[jcell].zavg;
      count[i]++;
      xt += cells[jcell].xavg;
      yt += cells[jcell].yavg;
      zt += cells[jcell].zavg;
      tcount++;
     }

   /*  Get average x,y,z for all cells.  */
   if (tcount > 0)
     {
      xt /= (float)tcount;
      yt /= (float)tcount;
      zt /= (float)tcount;
     }
   if (xreflect == 1) xt = 0;
   if (yreflect == 1) yt = 0;
   if (zreflect == 1) zt = 0;

   /*  Get average x,y,z for each flag type  */
   /*  and generate type average vectors.    */
   for (i = 0; i < nv; i++)
     {
      if (count[i] > 0)
        {
         xsum[i] /= (float)count[i];
         ysum[i] /= (float)count[i];
         zsum[i] /= (float)count[i];
        }
      cellexpx[i] = xsum[i] - xt;
      cellexpy[i] = ysum[i] - yt;
      cellexpz[i] = zsum[i] - zt;
     }

   /*  Look for duplicates, and adjust the vector.  */
   adj = span * 0.05;
   tcount = 0;
   for (i = 1; i < nv; i++)
     {
      for (j = 0;  j < i; j++)
        {
         if (cellexpx[i] == cellexpx[j] && 
             cellexpy[i] == cellexpy[j] && 
             cellexpz[i] == cellexpz[j])
           {
            ip = tcount % numpaths;
            kp = tcount / numpaths + 1;
            cellexpx[i] += adj * kp * path[ip][0];
            cellexpy[i] += adj * kp * path[ip][1];
            cellexpz[i] += adj * kp * path[ip][2];
            tcount++;
           }
        } 
     }
}
