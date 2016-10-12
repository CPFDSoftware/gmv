#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/ToggleBG.h>
#include <Xm/LabelG.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "contour.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

static Widget cenxval, cenxvaltxt, cenyval, cenyvaltxt, cenzval, cenzvaltxt,
              centernodeform, centercellform, cennodetxt, cennodeprobe,
              cencelltxt, cencellprobe, centertracerform, centracertxt, 
              orgtracerbut;


void centerformpop()
{
  void setcenterwidgets();

   setcenterwidgets();
   MyManageChild(centerform);
}
 
 
void originsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ival;
  float val;
  char str[20];
  void autocenter();
 
   arg = (long int)client_data;
 
   if (arg < 6)
     {
      XmScaleGetValue(w,&ival);
      val = (float) ival / 1000.;
     }
 
   if (arg == 0 || arg == 3)
     {
      xtrans = -((globxicmax-globxicmin) * val + globxicmin);
      sprintf(str,"%#11.5g",-xtrans);
      XtVaSetValues(cenxvaltxt,XmNvalue,str,NULL);
     }
   if (arg == 1 || arg == 4)
     {
      ytrans = -((globyicmax-globyicmin) * val + globyicmin);
      sprintf(str,"%#11.5g",-ytrans);
      XtVaSetValues(cenyvaltxt,XmNvalue,str,NULL);
    }
   if (arg == 2 || arg == 5)
     {
      ztrans = -((globzicmax-globzicmin) * val + globzicmin);
      sprintf(str,"%#11.5g",-ztrans);
      XtVaSetValues(cenzvaltxt,XmNvalue,str,NULL);
     }

   if (arg < 6)
     {
      cencellid = -1;
      cennodeid = -1;
      centracerid = -1;
      if (arg < 3) fastdraw = 0;
      else fastdraw = 1;
      drawit();
     }

   if (arg == 6)
     {
      cennprobeflag = 0;
      XmToggleButtonGadgetSetState(cennodeprobe,FALSE,FALSE);
      cencprobeflag = 0;
      XmToggleButtonGadgetSetState(cencellprobe,FALSE,FALSE);
      XUndefineCursor(dpy,windowid);

      XtUnmanageChild(centerform);
     }

   if (arg == 7)
     {
      autocenter();
      fastdraw = 0;
      drawit();
     }
}


void centxtread(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  float val;
  void setcenterwidgets();

   tmpstr = XmTextFieldGetString(cenxvaltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
 
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;

   xtrans = -val;

   tmpstr = XmTextFieldGetString(cenyvaltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
 
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;

   ytrans = -val;

   tmpstr = XmTextFieldGetString(cenzvaltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
 
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;

   ztrans = -val;

   setcenterwidgets();

   fastdraw = 0;
   drawit();
}
 

void orgnode(Widget w,XtPointer client_data,XtPointer call_data)
{
  void centernodeformpop();
  
   centernodeformpop();
}
 

void orgcell(Widget w,XtPointer client_data,XtPointer call_data)
{
  void centercellformpop();

   centercellformpop();
}
 

void orgtracer(Widget w,XtPointer client_data,XtPointer call_data)
{
  void centertracerformpop();

   centertracerformpop();
}


void setcenterprobe()
{
  int i, cp_nodes, cp_cells;

   /*  Check for cutplane nodes and cells.  */
   cp_nodes = 0;  cp_cells = 0;
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES; i++)
     {
      if (cutplane_on[i] > 0)
        {
         if (cutplane[i].ndnodes > 0 && 
             (cutplane[i].nodeflag == 1 || cutplane[i].nnumflag == 1  ||
              cutplane[i].velflag == 1))
            cp_nodes = 1;
         if (cutplane[i].ndcells > 0 && 
             (cutplane[i].cellflag == 1 || cutplane[i].cnumflag == 1 ||
              cutplane[i].cvelflag))
            cp_cells = 1;
        }
     }

   if (nodesflag > 0 || vectorflag == 1 || nodenumflag == 1 || cp_nodes)
      XtSetSensitive(cennodeprobe,TRUE);
   else
     {
      XtSetSensitive(cennodeprobe,FALSE);
      XmToggleButtonGadgetSetState(cennodeprobe,FALSE,TRUE);
     }

   if (ncells > 0)
     {
      if (celledgesflag == 1 || cellsflag == 1 || cellnumflag == 1 ||
          gridanalflag == 1 || cp_cells)
         XtSetSensitive(cencellprobe,TRUE);
      else
        {
         XtSetSensitive(cencellprobe,FALSE);
         XmToggleButtonGadgetSetState(cencellprobe,FALSE,TRUE);
        }
     }

   if (viewflag > 0)
     {
      XtSetSensitive(cennodeprobe,FALSE);
      XmToggleButtonGadgetSetState(cennodeprobe,FALSE,TRUE);
      if (ncells > 0)
        {
         XtSetSensitive(cencellprobe,FALSE);
         XmToggleButtonGadgetSetState(cencellprobe,FALSE,TRUE);
        }
     }
   XmUpdateDisplay(cennodeprobe);
   if (ncells > 0) XmUpdateDisplay(cencellprobe);
}


void centernodeformpop()
{
   MyManageChild(centernodeform);
}


void cennodequit(Widget w,XtPointer client_data,XtPointer call_data)
{
   cennprobeflag = 0;
   XmToggleButtonGadgetSetState(cennodeprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);

   XtUnmanageChild(centernodeform);
}


int cennode_set()
{
  int i, cennodeno;

   cennodeno = cennodeid;
   if (nodeidflag)
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (cennodeid == nodeids[i]) cennodeno = i;
        }
     }
   if (cennodeno > 0 && cennodeno <= nnodes)
     {
      xtrans = -nodes[cennodeno].x;
      ytrans = -nodes[cennodeno].y;
      ztrans = -nodes[cennodeno].z;
      xpan = 0.;
      ypan = 0.;
     }
   return cennodeno;
}


void cennode_apply(Widget w,XtPointer client_data,XtPointer call_data)
{
  char *tmpstr;
  int cennodeno, id;
  int cennode_set();
  void setcenterwidgets();

   tmpstr = XmTextFieldGetString(cennodetxt);
   id = atoi(tmpstr);
   XtFree(tmpstr);
   cennodeid = id;
   cennodeno = cennode_set();
   if (cennodeno > 0 && cennodeno <= nnodes)
     {
      cencellid = -1;
      centracerid = -1;
      setcenterwidgets();
      fastdraw = 0;
      drawit();
     }
}


void cennode_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   cennprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (cennprobeflag)
     {
      get_1_point(NULL, (caddr_t) 2, NULL);
     }
   else XUndefineCursor(dpy,windowid);
}


void cennprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int nodeid, id;
  char str[10];

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Get the node number.  */
   nodeid = getnodeprobe(pa,pb,pc,pd,pe,pf);

   /*  Set nodeid in text.  */
   id = nodeid;
   if (nodeidflag) id = nodeids[nodeid];
   sprintf(str,"%d",id);
   XtVaSetValues(cennodetxt,XmNvalue,str,NULL);
   XmUpdateDisplay(cennodetxt);
   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void centercellformpop()
{
   MyManageChild(centercellform);
}


void cencellquit(Widget w,XtPointer client_data,XtPointer call_data)
{
   cencprobeflag = 0;
   XmToggleButtonGadgetSetState(cencellprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);

   XtUnmanageChild(centercellform);
}



int cencell_set()
{
  int i, cencellno, id, icell, jcell;

   cencellno = cencellid;
   if (cellidflag)
     {
      for (i = 0; i < ncells; i++)
        {
         if (cencellid == cellids[i]) cencellno = i+1;
        }
     }
   if (cencellno > 0 && cencellno <= ncells)
     {
      icell = cencellno - 1;
      jcell = icell;
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,0);
        }
     }
   if (jcell >= 0 && jcell < ncells)
     {
      xtrans = -cells[jcell].xavg;
      ytrans = -cells[jcell].yavg;
      ztrans = -cells[jcell].zavg;
      xpan = 0.;
      ypan = 0.;
     }
   return jcell;
}


void cencell_apply(Widget w,XtPointer client_data,XtPointer call_data)
{
  char *tmpstr;
  int id, jcell;
  int cencell_set();
  void setcenterwidgets();

   tmpstr = XmTextFieldGetString(cencelltxt);
   cencellid = atoi(tmpstr);
   XtFree(tmpstr);

   jcell = cencell_set();
   if (jcell >= 0 && jcell < ncells)
     {
      cennodeid = -1;
      centracerid = -1;
      setcenterwidgets();
      fastdraw = 0;
      drawit();
     }
}


void cencell_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   cencprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (cencprobeflag)
     {
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else XUndefineCursor(dpy,windowid);
}


void cencprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int cellid, id;
  char str[10];

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Get the cell number.  */
   cellid = getcellprobe(pa,pb,pc,pd,pe,pf);

   /*  Set cellid in text.  */
   id = cellid + 1;
   if (cellidflag) id = cellids[cellid];
   sprintf(str,"%d",id);
   XtVaSetValues(cencelltxt,XmNvalue,str,NULL);
   XmUpdateDisplay(cencelltxt);
   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void centertracerformpop()
{
   MyManageChild(centertracerform);
}


void centracerquit(Widget w,XtPointer client_data,XtPointer call_data)
{
   XtUnmanageChild(centertracerform);
}


int centracer_set()
{
  int i, centracerno, id, itracer;

   centracerno = centracerid;
   if (traceridflag)
     {
      for (i = 0; i < ntracers; i++)
        {
         if (centracerid == tracerids[i]) centracerno = i+1;
        }
     }
   if (centracerno > 0 && centracerno <= ntracers)
     {
      itracer = centracerno - 1;
     }
   if (itracer >= 0 && itracer < ntracers)
     {
      xtrans = -tracers[0].x[itracer];
      ytrans = -tracers[0].y[itracer];
      ztrans = -tracers[0].z[itracer];
      xpan = 0.;
      ypan = 0.;
     }
   return itracer;
}


void centracer_apply(Widget w,XtPointer client_data,XtPointer call_data)
{
  char *tmpstr;
  int id, jtracer;
  int centracer_set();
  void setcenterwidgets();

   tmpstr = XmTextFieldGetString(centracertxt);
   centracerid = atoi(tmpstr);
   XtFree(tmpstr);

   jtracer = centracer_set();
   if (jtracer >= 0 && jtracer < ntracers)
     {
      cennodeid = -1;
      cencellid = -1;
      setcenterwidgets();
      fastdraw = 0;
      drawit();
     }
}


void makecenterform(parent)
 Widget parent;
 
{
  Widget orgclose, xlabel, ylabel, zlabel, orgautobut, orgnodebut,
         orgcellbut, cennodequitbut, cennodeapply, cennodelab,
         cencellquitbut, cencellapply, cencelllab,
         centracerquitbut, centracerapply, centracerlab;
  int imin, imax, ival, i;
  Arg wargs[10];
  char str[20];
  XmString string;
 
   string = XmStringCreate("Center",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   centerform = XmCreateFormDialog(parent,"Center",wargs,i);
      XmStringFree(string);

   imin = 0;
   imax = 1000;
   ival = 0.;
   sprintf(str,"%9.3g",0.);
   string = XmStringCreate("X",XmSTRING_DEFAULT_CHARSET);
   xlabel = XtVaCreateManagedWidget("Xlabel",xmLabelWidgetClass,
            centerform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   cenxval = XtVaCreateManagedWidget("Cenxval",xmScaleWidgetClass,
             centerform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, xlabel,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, xlabel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, imin,
             XmNmaximum, imax,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, ival,
             XmNscaleWidth, 700,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(cenxval,XmNvalueChangedCallback,originsl,
                    (caddr_t) 0);
      XtAddCallback(cenxval,XmNdragCallback,originsl,
                    (caddr_t) 3);
   cenxvaltxt = XtVaCreateManagedWidget("Cenxvaltxt",
                xmTextFieldWidgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, xlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cenxval,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 20,
                XmNcolumns, 11,
                XmNvalue, "",
                NULL);
      XtAddCallback(cenxvaltxt,XmNactivateCallback,
                    centxtread,(caddr_t) 0);

   string = XmStringCreate("Y",XmSTRING_DEFAULT_CHARSET);
   ylabel = XtVaCreateManagedWidget("Ylabel",xmLabelWidgetClass,
            centerform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cenxvaltxt,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   cenyval = XtVaCreateManagedWidget("Cenyval",xmScaleWidgetClass,
             centerform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, ylabel,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, ylabel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, imin,
             XmNmaximum, imax,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, ival,
             XmNscaleWidth, 700,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(cenyval,XmNvalueChangedCallback,originsl,
                    (caddr_t) 1);
      XtAddCallback(cenyval,XmNdragCallback,originsl,
                    (caddr_t) 4);
   cenyvaltxt = XtVaCreateManagedWidget("Cenyvaltxt",
                xmTextFieldWidgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, ylabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cenyval,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 20,
                XmNcolumns, 11,
                XmNvalue, "",
                NULL);
      XtAddCallback(cenyvaltxt,XmNactivateCallback,
                    centxtread,(caddr_t) 1);

   string = XmStringCreate("Z",XmSTRING_DEFAULT_CHARSET);
   zlabel = XtVaCreateManagedWidget("Zlabel",xmLabelWidgetClass,
            centerform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cenyvaltxt,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   cenzval = XtVaCreateManagedWidget("Cenzval",xmScaleWidgetClass,
             centerform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, zlabel,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, zlabel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, imin,
             XmNmaximum, imax,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, ival,
             XmNscaleWidth, 700,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(cenzval,XmNvalueChangedCallback,originsl,
                    (caddr_t) 2);
      XtAddCallback(cenzval,XmNdragCallback,originsl,
                    (caddr_t) 5);
   cenzvaltxt = XtVaCreateManagedWidget("Cenzvaltxt",
                xmTextFieldWidgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, zlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cenzval,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 20,
                XmNcolumns, 11,
                XmNvalue, "",
                NULL);
      XtAddCallback(cenzvaltxt,XmNactivateCallback,
                    centxtread,(caddr_t) 2);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   orgclose = XtVaCreateManagedWidget("Orgclose",
              xmPushButtonGadgetClass,
              centerform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, zlabel,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(orgclose,XmNactivateCallback,originsl,
                    (caddr_t) 6);
 
   string = XmStringCreate("Auto Center",XmSTRING_DEFAULT_CHARSET);
   orgautobut = XtVaCreateManagedWidget("Orgautobut",
                xmPushButtonGadgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, zlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, orgclose,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(orgautobut,XmNactivateCallback,originsl,
                    (caddr_t) 7);
 
   string = XmStringCreate("Center on Node",XmSTRING_DEFAULT_CHARSET);
   orgnodebut = XtVaCreateManagedWidget("Cennodebut",
                xmPushButtonWidgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, zlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, orgautobut,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(orgnodebut,XmNactivateCallback,orgnode,NULL);
 
   string = XmStringCreate("Center on Cell",XmSTRING_DEFAULT_CHARSET);
   orgcellbut = XtVaCreateManagedWidget("Orgcellbut",
                xmPushButtonWidgetClass,
                centerform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, zlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, orgnodebut,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(orgcellbut,XmNactivateCallback,orgcell,NULL);
 
   string = XmStringCreate("Center on Particle",XmSTRING_DEFAULT_CHARSET);
   orgtracerbut = XtVaCreateManagedWidget("Orgtracerbut",
                  xmPushButtonWidgetClass,
                  centerform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, zlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, orgcellbut,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(orgtracerbut,XmNactivateCallback,orgtracer,NULL);

   /*  Create Center on Node form.  */
   string = XmStringCreate("Center on Node",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   centernodeform = XmCreateFormDialog(orgnodebut,"Center Node",wargs,i);
      XmStringFree(string);
  
   string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
   cennodequitbut = XtVaCreateManagedWidget("Cennodequitbut",
                    xmPushButtonGadgetClass,
                    centernodeform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);
      XtAddCallback(cennodequitbut,XmNactivateCallback,cennodequit,NULL);

   string = XmStringLtoRCreate("Apply",
                               XmSTRING_DEFAULT_CHARSET);
   cennodeapply = XtVaCreateManagedWidget("Cennodeapply",
                  xmPushButtonGadgetClass,
                  centernodeform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cennodequitbut,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cennodeapply,XmNactivateCallback,cennode_apply,NULL);

   /*  Create node input widgets.  */
   string = XmStringCreate("Node:",XmSTRING_DEFAULT_CHARSET);
   cennodelab = XtVaCreateManagedWidget("Cennodelab",
                xmLabelGadgetClass,
                centernodeform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cennodeapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   cennodetxt = XtVaCreateManagedWidget("Cennodetxt",
               xmTextFieldWidgetClass,
               centernodeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cennodeapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cennodelab,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 8,
               XmNcolumns, 8,
               XmNvalue, "",
               NULL);

   string = XmStringLtoRCreate("Get Node No. From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   cennodeprobe = XtVaCreateManagedWidget("Cennodeprobe",
                  xmToggleButtonGadgetClass,
                  centernodeform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cennodetxt,
                  XmNtopOffset, 5,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cennodeprobe,XmNvalueChangedCallback,
                    cennode_probe,NULL);

   /*  Create Center on Cell form.  */
   string = XmStringCreate("Center on Cell",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   centercellform = XmCreateFormDialog(orgcellbut,"Center Cell",wargs,i);
      XmStringFree(string);
  
   string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
   cencellquitbut = XtVaCreateManagedWidget("Cencellquitbut",
                    xmPushButtonGadgetClass,
                    centercellform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);
      XtAddCallback(cencellquitbut,XmNactivateCallback,cencellquit,NULL);

   string = XmStringLtoRCreate("Apply",
                               XmSTRING_DEFAULT_CHARSET);
   cencellapply = XtVaCreateManagedWidget("Cencellapply",
                  xmPushButtonGadgetClass,
                  centercellform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cencellquitbut,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cencellapply,XmNactivateCallback,cencell_apply,NULL);

   /*  Create cell input widgets.  */
   string = XmStringCreate("Cell:",XmSTRING_DEFAULT_CHARSET);
   cencelllab = XtVaCreateManagedWidget("Cencelllab",
                xmLabelGadgetClass,
                centercellform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cencellapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   cencelltxt = XtVaCreateManagedWidget("Cencelltxt",
                xmTextFieldWidgetClass,
                centercellform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cencellapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cencelllab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);

   string = XmStringLtoRCreate("Get Cell No. From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   cencellprobe = XtVaCreateManagedWidget("Cencellprobe",
                  xmToggleButtonGadgetClass,
                  centercellform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cencelltxt,
                  XmNtopOffset, 5,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cencellprobe,XmNvalueChangedCallback,
                    cencell_probe,NULL);

   /*  Create Center on Particle form.  */
   string = XmStringCreate("Center on Particle",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   centertracerform = XmCreateFormDialog(orgtracerbut,"Center_Tracer",wargs,i);
      XmStringFree(string);
  
   string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
   centracerquitbut = XtVaCreateManagedWidget("Centracerquitbut",
                      xmPushButtonGadgetClass,
                     centertracerform,
                     XmNtopAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(centracerquitbut,XmNactivateCallback,centracerquit,NULL);

   string = XmStringLtoRCreate("Apply",
                               XmSTRING_DEFAULT_CHARSET);
   centracerapply = XtVaCreateManagedWidget("Centracerapply",
                    xmPushButtonGadgetClass,
                    centertracerform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, centracerquitbut,
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    XmNselectColor, yellowpix,
                    XmNset, FALSE,
                    NULL);
      XmStringFree(string);
      XtAddCallback(centracerapply,XmNactivateCallback,centracer_apply,NULL);

   /*  Create tracer input widgets.  */
   string = XmStringCreate("Particle :",XmSTRING_DEFAULT_CHARSET);
   centracerlab = XtVaCreateManagedWidget("Centracerlab",
                  xmLabelGadgetClass,
                  centertracerform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, centracerapply,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);

   centracertxt = XtVaCreateManagedWidget("Centracertxt",
                  xmTextFieldWidgetClass,
                  centertracerform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, centracerapply,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, centracerlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 8,
                  XmNcolumns, 8,
                  XmNvalue, "",
                  NULL);
}


void setcenterwidgets()
{
  int ival;
  float val;
  char str[20];

   XtRemoveCallback(cenxval,XmNvalueChangedCallback,originsl,(caddr_t) 0);
   XtRemoveCallback(cenyval,XmNvalueChangedCallback,originsl,(caddr_t) 1);
   XtRemoveCallback(cenzval,XmNvalueChangedCallback,originsl,(caddr_t) 2);
   XFlush(dpy);

   val = (-xtrans - globxicmin) / (globxicmax-globxicmin);
   ival = (int) (val * 1000.);
   XmScaleSetValue(cenxval,ival);
   sprintf(str,"%#11.5g",-xtrans);
   XtVaSetValues(cenxvaltxt,XmNvalue,str,NULL);

   val = (-ytrans - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XmScaleSetValue(cenyval,ival);
   sprintf(str,"%#11.5g",-ytrans);
   XtVaSetValues(cenyvaltxt,XmNvalue,str,NULL);

   val = (-ztrans - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XmScaleSetValue(cenzval,ival);
   sprintf(str,"%#11.5g",-ztrans);
   XtVaSetValues(cenzvaltxt,XmNvalue,str,NULL);

   sprintf(str,"%d",cencellid);
   XtVaSetValues(cencelltxt,XmNvalue,str,NULL);

   sprintf(str,"%d",cennodeid);
   XtVaSetValues(cennodetxt,XmNvalue,str,NULL);

   sprintf(str,"%d",centracerid);
   XtVaSetValues(centracertxt,XmNvalue,str,NULL);

   if (ntracers <= 0) XtSetSensitive(orgtracerbut,FALSE);
   else XtSetSensitive(orgtracerbut,TRUE);

   XFlush(dpy);
   XtAddCallback(cenxval,XmNvalueChangedCallback,originsl,(caddr_t) 0);
   XtAddCallback(cenyval,XmNvalueChangedCallback,originsl,(caddr_t) 1);
   XtAddCallback(cenzval,XmNvalueChangedCallback,originsl,(caddr_t) 2);
   XFlush(dpy);
}


void autocenter()
{
  double xcmin, ycmin, zcmin, xcmax, ycmax, zcmax;
  double xmin, xmax, ymin, ymax, zmin, zmax, xck, yck, zck;
  int ip, gopt, icell, jcell, gocell, gotrace, i, j, k, m;
  int isurf, gosurf;
  short *nmatchk, *cmatchk, *smatchk;
  long nv;

   /*  This routine automatically centers the visible image.  */
   xcmin = MAXVAL;  xcmax = -MAXVAL;
   ycmin = MAXVAL;  ycmax = -MAXVAL;
   zcmin = MAXVAL;  zcmax = -MAXVAL;

   /*  Set to material or group color.  */
   if (nodefieldactive == -2) nmatchk = nodegrpcolr;
   else nmatchk = imat;
   if (cellfieldactive == -2) cmatchk = cellgrpcolr;
   else cmatchk = cellmats;
   if (surffieldactive == -2) smatchk = surfgrpcolr;
   else smatchk = surfmats;

   /*  First check nodes.  */
   if (nodesflag > 0 || nodenumflag == 1)
     {
      for (ip = 1; ip < nnodes+1; ip++)
        { 

         /*  See if the point has been selected for display.  */
         gopt = (int)nodeselectflag[ip];
 
         /*  Eliminate nodes with material < 1.  */
         if (node0matflag == 0 && nmatchk[ip] < 1) gopt = 0;
 
         /*  See if the point lies within the subset.  */
         if ((int)nodesubsetflag[ip] == 0) gopt = 0;

         /*  Get node mins and maxs.  */
         if (gopt == 1)
           {
            xck = nodes[ip].x;
            yck = nodes[ip].y;
            zck = nodes[ip].z;
            xcmin = (xck < xcmin) ? xck : xcmin;
            ycmin = (yck < ycmin) ? yck : ycmin;
            zcmin = (zck < zcmin) ? zck : zcmin;
            xcmax = (xck > xcmax) ? xck : xcmax;
            ycmax = (yck > ycmax) ? yck : ycmax;
            zcmax = (zck > zcmax) ? zck : zcmax;
           }
        } 
     }

   /*  Next check cells.  */
   if (cellsflag == 1 || celledgesflag == 1 || cellnumflag == 1 ||
       cellmedianedgeflag == 1)
     {
      for (icell = 0; icell < ncells; icell++)
        {

         /*  See if the cell has been selected for display.  */
         gocell = (int)cellselectflag[icell];

         /*  Eliminate cells with material < 1    */
         /*  for regular cells unless requested.  */
         if (cmatchk[icell] < 1 && cell0matflag == 0) gocell = 0;

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (gocell == 1 && structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,0);
           }

         xck = cells[jcell].xavg;
         yck = cells[jcell].yavg;
         zck = cells[jcell].zavg;
 
         /*  See if the cell lies within the subset.  */
         if ((int)cellsubsetflag[icell] == 0)
            gocell = 0;

         /*  Get cell mins and maxs.  */
         if (gocell == 1)
           {
            xmin = MAXVAL;  xmax = -MAXVAL;
            ymin = MAXVAL;  ymax = -MAXVAL;
            zmin = MAXVAL;  zmax = -MAXVAL;
            for (j = 0; j < cells[jcell].ncnodes; j++)
              {
               m = cells[jcell].cnodes[j];
               xck = nodes[m].x;
               yck = nodes[m].y;
               zck = nodes[m].z;
               xmin = (xmin < xck) ? xmin : xck;
               xmax = (xmax > xck) ? xmax : xck;
               ymin = (ymin < yck) ? ymin : yck;
               ymax = (ymax > yck) ? ymax : yck;
               zmin = (zmin < zck) ? zmin : zck;
               zmax = (zmax > zck) ? zmax : zck;
              }
            xcmin = (xmin < xcmin) ? xmin : xcmin;
            ycmin = (ymin < ycmin) ? ymin : ycmin;
            zcmin = (zmin < zcmin) ? zmin : zcmin;
            xcmax = (xmax > xcmax) ? xmax : xcmax;
            ycmax = (ymax > ycmax) ? ymax : ycmax;
            zcmax = (zmax > zcmax) ? zmax : zcmax;
           }
        }
     }

   /*  Next check polygons.  */
   if (polysflag == 1 || linesflag == 1)
     {
      for (i = 0; i < mmats; i++)
        {
         if (active_mat[i] == 1)
           {
            for (j = 0; j < npolyspermat[i]; j++)
              {  
               if (polydrawchk[i][j] == 1)
                 {
                  m = (int)polygons[i][j].nverts;
                  nv = polygons[i][j].polyvertloc;
                  for (k = 0; k < m; k++)
                    {
                     xck = polyverts[i][k+nv].x;
                     yck = polyverts[i][k+nv].y;
                     zck = polyverts[i][k+nv].z;
                     xcmin = (xck < xcmin) ? xck : xcmin;
                     ycmin = (yck < ycmin) ? yck : ycmin;
                     zcmin = (zck < zcmin) ? zck : zcmin;
                     xcmax = (xck > xcmax) ? xck : xcmax;
                     ycmax = (yck > ycmax) ? yck : ycmax;
                     zcmax = (zck > zcmax) ? zck : zcmax;
                    }
                 }
              }
           }
        }
     }

   /*  Check contours.  */
   for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++) 
     {
      if (contour_on[i] == 1)
        {
         for (j = 0; j < num_of_contour_polys[i]; j++)
           {
            for (k = 0; k < 3; k++)
              {
               xck = contourlist[i][j].vx[k];
               yck = contourlist[i][j].vy[k];
               zck = contourlist[i][j].vz[k];
               xcmin = (xck < xcmin) ? xck : xcmin;
               ycmin = (yck < ycmin) ? yck : ycmin;
               zcmin = (zck < zcmin) ? zck : zcmin;
               xcmax = (xck > xcmax) ? xck : xcmax;
               ycmax = (yck > ycmax) ? yck : ycmax;
               zcmax = (zck > zcmax) ? zck : zcmax;
              }
           }
        }
     }

   /*  Check grid analysis cells.  */
   if (gridanalflag == 1)
     {
      for (icell = 0; icell < ncells; icell++)
        {

         /*  See if the cell has been selected for display.  */
         gocell = (int)gridselectflag[icell];

         /*  Eliminate cells with material < 1.  */
         if (cmatchk[icell] < 1) gocell = 0;

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (gocell == 1 && structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,0);
           }
 
         /*  Get cell mins and maxs.  */
         if (gocell == 1)
           {
            xmin = MAXVAL;  xmax = -MAXVAL;
            ymin = MAXVAL;  ymax = -MAXVAL;
            zmin = MAXVAL;  zmax = -MAXVAL;
            for (j = 0; j < cells[jcell].ncnodes; j++)
              {
               m = cells[jcell].cnodes[j];
               xck = nodes[m].x;
               yck = nodes[m].y;
               zck = nodes[m].z;
               xmin = (xmin < xck) ? xmin : xck;
               xmax = (xmax > xck) ? xmax : xck;
               ymin = (ymin < yck) ? ymin : yck;
               ymax = (ymax > yck) ? ymax : yck;
               zmin = (zmin < zck) ? zmin : zck;
               zmax = (zmax > zck) ? zmax : zck;
              }
            xcmin = (xmin < xcmin) ? xmin : xcmin;
            ycmin = (ymin < ycmin) ? ymin : ycmin;
            zcmin = (zmin < zcmin) ? zmin : zcmin;
            xcmax = (xmax > xcmax) ? xmax : xcmax;
            ycmax = (ymax > ycmax) ? ymax : ycmax;
            zcmax = (zmax > zcmax) ? zmax : zcmax;
           }
        }
     }

   /*  Check tracers.  */
   if (tracerflag > 0)
     {
      for (i = 0; i < ntracers; i++)
        {
         gotrace = (int)traceselectflag[icell];

         if (gotrace)
           {
            xck = tracers[0].x[i];
            yck = tracers[0].y[i];
            zck = tracers[0].z[i];
            xcmin = (xck < xcmin) ? xck : xcmin;
            ycmin = (yck < ycmin) ? yck : ycmin;
            zcmin = (zck < zcmin) ? zck : zcmin;
            xcmax = (xck > xcmax) ? xck : xcmax;
            ycmax = (yck > ycmax) ? yck : ycmax;
            zcmax = (zck > zcmax) ? zck : zcmax;
           }
        }
     }

   /*  Check surfaces.  */
   if (nsurf > 0)
     {
      if (surfflag == 1 || surfedgesflag == 1 || surfnumflag == 1)
        {
         for (isurf = 0; isurf < nsurf; isurf++)
           {

            /*  See if the surface has been selected for display.  */
            gosurf = (int)surfselectflag[isurf];

            /*  Eliminate surfaces with material < 1    */
            /*  for regular surfaces unless requested.  */
            if (smatchk[isurf] < 1 && surf0matflag == 0) gosurf = 0;

            xck = surfs[isurf].xavg;
            yck = surfs[isurf].yavg;
            zck = surfs[isurf].zavg;
 
            /*  See if the surface lies within the subset.  */
            if ((int)surfsubsetflag[isurf] == 0)
               gosurf = 0;

            /*  Get surface mins and maxs.  */
            if (gosurf == 1)
              {
               xmin = MAXVAL;  xmax = -MAXVAL;
               ymin = MAXVAL;  ymax = -MAXVAL;
               zmin = MAXVAL;  zmax = -MAXVAL;
               for (j = 0; j < surfs[isurf].nverts; j++)
                 {
                  m = surfs[isurf].fverts[j];
                  xck = nodes[m].x;
                  yck = nodes[m].y;
                  zck = nodes[m].z;
                  xmin = (xmin < xck) ? xmin : xck;
                  xmax = (xmax > xck) ? xmax : xck;
                  ymin = (ymin < yck) ? ymin : yck;
                  ymax = (ymax > yck) ? ymax : yck;
                  zmin = (zmin < zck) ? zmin : zck;
                  zmax = (zmax > zck) ? zmax : zck;
                 }
               xcmin = (xmin < xcmin) ? xmin : xcmin;
               ycmin = (ymin < ycmin) ? ymin : ycmin;
               zcmin = (zmin < zcmin) ? zmin : zcmin;
               xcmax = (xmax > xcmax) ? xmax : xcmax;
               ycmax = (ymax > ycmax) ? ymax : ycmax;
               zcmax = (zmax > zcmax) ? zmax : zcmax;
              }
           }
        }
     }

   /*  Check boundaries.  */
   if (xcmin > xcmax)
     {
      xcmin = globxicmin;
      xcmax = globxicmax;
     }
   if (ycmin > ycmax)
     {
      ycmin = globyicmin;
      ycmax = globyicmax;
     }
   if (zcmin > zcmax)
     {
      zcmin = globzicmin;
      zcmax = globzicmax;
     }

   /*  Set center and sliders.  */
   xtrans = -0.5 * (xcmax + xcmin);
   ytrans = -0.5 * (ycmax + ycmin);
   ztrans = -0.5 * (zcmax + zcmin);
  
   setcenterwidgets();

   /*  Reset pan to origin.  */
   xpan = 0.;  ypan = 0.;
}
