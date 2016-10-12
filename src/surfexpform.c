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
#include "formwidgets.h"
#include "xstuff.h"

static Widget sexplode, sexpbut[MAXFLAGS+1];
static int oldsurfexpflag;



void surfexplodepop()
{
  int i, ival;

   for (i = 0; i <= numflags_surf; i++)
      XtVaSetValues(sexpbut[i],XmNset,FALSE,NULL);
   XtVaSetValues(sexpbut[surfexpflag+1],XmNset,TRUE,NULL);
   ival = (int) (sexplodefact * 100.);
   XmScaleSetValue(sexplode,ival);

   MyManageChild(surfexpform);
}

void seclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfexpform);
}


void seapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int ival;
  void surfexpvects();

   XmScaleGetValue(sexplode,&ival);
   sexplodefact = (float)ival / 100.;
   if (oldsurfexpflag != surfexpflag)
     {
      oldsurfexpflag = surfexpflag;
      surfexpvects();
     }
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void select_surf_expflag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   surfexpflag = arg - 1;
}


void makesurfexpform(parent)
 Widget parent;
{
  Widget sexpclose, sexpapply, sexptxt, sexpbox;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Surf Explode",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfexpform = XmCreateFormDialog(parent,"Surfexpform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sexpclose = XtVaCreateManagedWidget("Sexpclose",
               xmPushButtonGadgetClass,
               surfexpform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sexpclose,XmNactivateCallback,seclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   sexpapply = XtVaCreateManagedWidget("Sexpapply",
               xmPushButtonGadgetClass,
               surfexpform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sexpclose,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sexpapply,XmNactivateCallback,seapply,NULL);

   string = XmStringCreate("Explode %",XmSTRING_DEFAULT_CHARSET);
   sexplode = XtVaCreateManagedWidget("Sexplode",
              xmScaleWidgetClass,
              surfexpform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sexpclose,
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
   sexptxt = XtVaCreateManagedWidget("Sexptxt",
             xmLabelWidgetClass,
             surfexpform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, sexplode,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sexptxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   sexpbox = XmCreateRadioBox(surfexpform,"Sexpbox",wargs,i);
   XtManageChild(sexpbox);
   for (i = 0; i <= numflags_surf; i++)
     {
      if (i == 0) 
         string = XmStringCreate("Mat. No.",
                                 XmSTRING_DEFAULT_CHARSET);
      else 
         string = XmStringCreate(flagname_surf[i-1],
                                 XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Sexpbut%d",i);
      sexpbut[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonGadgetClass,
                    sexpbox,
                    XmNlabelString, string,
                    XmNspacing, 1,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sexpbut[i],XmNvalueChangedCallback,
                       select_surf_expflag,(caddr_t) i);

      if (i == 0) XtVaSetValues(sexpbut[0],XmNset,TRUE,NULL);
     }
   oldsurfexpflag = -99;
}


void surfexpvects()

{
  int i, j, isurf, count[MAXMATS], nv, tcount;
  double xt, yt, zt, xsum[MAXMATS], ysum[MAXMATS], zsum[MAXMATS];
  float adj;
  int numpaths = 16, ip, kp;
  int path[16][3] = { {1,0,1}, {0,1,1}, {-1,0,1}, {0,-1,1},
                      {1,0,-1}, {0,1,-1}, {-1,0,-1}, {0,-1,-1},
                      {1,1,1}, {1,-1,1}, {-1,1,1}, {-1,-1,1},
                      {1,1,-1}, {1,-1,-1}, {-1,1,-1}, {-1,-1,-1} };

   /*  Loop through surfaces and sum x,y,z's  */  
   /*  of the avg. vertex of each facet.      */
   xt = 0;  yt = 0;  zt = 0;
   tcount = 0;
   if (surfexpflag == -1) nv = maxsurfmat;
   else nv = flagtypes_surf[surfexpflag];

   for (i = 0; i < MAXMATS; i++)
     {
      count[i] = 0;
      xsum[i] = 0;  ysum[i] = 0;  zsum[i] = 0;
     }

   for (isurf = 0; isurf < nsurf; isurf++)
     {
      if (surfexpflag == -1) i = (int)surfmats[isurf] - 1;
      else i = (int)surfflags[surfexpflag][isurf] - 1;

      xsum[i] += surfs[isurf].xavg;
      ysum[i] += surfs[isurf].yavg;
      zsum[i] += surfs[isurf].zavg;
      count[i]++;
      xt += surfs[isurf].xavg;
      yt += surfs[isurf].yavg;
      zt += surfs[isurf].zavg;
      tcount++;
     }

   /*  Get average x,y,z for all surfs.  */
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
      surfexpx[i] = xsum[i] - xt;
      surfexpy[i] = ysum[i] - yt;
      surfexpz[i] = zsum[i] - zt;
     }

   /*  Look for duplicates, and adjust the vector.  */
   adj = span * 0.05;
   tcount = 0;
   for (i = 1; i < nv; i++)
     {
      for (j = 0;  j < i; j++)
        {
         if (surfexpx[i] == surfexpx[j] && 
             surfexpy[i] == surfexpy[j] && 
             surfexpz[i] == surfexpz[j])
           {
            ip = tcount % numpaths;
            kp = tcount / numpaths + 1;
            surfexpx[i] += adj * kp * path[ip][0];
            surfexpy[i] += adj * kp * path[ip][1];
            surfexpz[i] += adj * kp * path[ip][2];
            tcount++;
           }
        } 
     }
}
