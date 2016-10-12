#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "contour.h"

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#define MAXVAL 9.99999e32
#define MAXTXT 8

extern short alltetmesh;

static Widget ganodenumbut, gacellnumbut, ccountlab,
              nodestxt[MAXTXT], cellstxt[MAXTXT], cellbegtxt,
              cellendtxt, gadrawcell, gadrawcont, gadrawedge, gamatsbut,
              ganfldsbut, ganfldlab, gacfldsbut, gacfldlab, 
              gaflagsbut, gaflaglab, gaflagform, gadrawmedface, 
              gadrawmededge, gadrawvorface, gadrawvoredge, gafacenumbut;
static short currganfld, currgaflag, currgacfld, gridclrflag, gridinit = 0;
static int gridfldalloc, gridselalloc;


void gridanalformpop()
{
  int i, gridfldalloc;
  void gridanalinit();
  XmString string;

   gridanalinit();

   /*  Unselect all grid cells for display.  */
   if (gridanalflag == 0) 
      for (i = 0; i < ncells; i++) gridselectflag[i] = 0;

   if (gridedgesflag == 0 && gridsflag == 0 &&
       gridmedianedgeflag == 0 && gridmedianflag == 0 &&
       gridvoronoiedgeflag == 0 && gridvoronoiflag == 0)
      gridedgesflag = 1;

   XmToggleButtonGadgetSetState(gadrawedge,gridedgesflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawcell,gridsflag,FALSE);
   XmToggleButtonGadgetSetState(ganodenumbut,ganodenumflag,FALSE);
   XmToggleButtonGadgetSetState(gacellnumbut,gacellnumflag,FALSE);
   XmToggleButtonGadgetSetState(gafacenumbut,gafacenumflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawcont,gridcontflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawmedface,gridmedianflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawmededge,gridmedianedgeflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawvorface,gridvoronoiflag,FALSE);
   XmToggleButtonGadgetSetState(gadrawvoredge,gridvoronoiedgeflag,FALSE);

   XmToggleButtonGadgetSetState(gamatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ganfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(gacfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(gaflagsbut,FALSE,FALSE);
   if (gridfieldactive < 0)
     {
      gridclrflag = 0;
      XmToggleButtonGadgetSetState(gamatsbut,TRUE,FALSE);
     }
   else if (gridfieldactive >= numvars + cnumvars + fnumvars)
     {
      gridclrflag = 3;
      XmToggleButtonGadgetSetState(gaflagsbut,TRUE,FALSE);
      currgaflag = gridfieldactive - numvars;
     }
   else if (gridfieldactive < numvars)
     {
      gridclrflag = 1;
      XmToggleButtonGadgetSetState(ganfldsbut,TRUE,FALSE);
      currganfld = gridfieldactive;
     }
   else
     {
      gridclrflag = 2;
      XmToggleButtonGadgetSetState(gacfldsbut,TRUE,FALSE);
      currgacfld = gridfieldactive - numvars;
     }

   i = currganfld;
   if (numvars > 0)
     {
      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(ganfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currgacfld;
   if (cnumvars > 0)
     {
      string = XmStringCreate(cfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(gacfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currgaflag;
   if (numflags_cell > 0)
     {
      string = XmStringCreate(flagname_cell[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(gaflaglab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(gridanalform);
}


void gridclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (numvars > 0)
      XtUnmanageChild(nfieldform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   if (numflags_cell > 1)
      XtUnmanageChild(gaflagform);
   XtUnmanageChild(gridanalform);
}


void gridexit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void gridanalfree();

   /*  Free gridfield data and grid select arrays.  */
   gridanalfree();

   /*  Reset cell selection and flags.  */
   setcellsel();
   gridanalflag = 0;
   ganodenumflag = 0;
   gacellnumflag = 0;
   gafacenumflag = 0;
   gridsflag = 0;
   gridedgesflag = 0;
   gridcontflag = 0;
   gridmedianflag = 0;
   gridmedianedgeflag = 0;
   gridvoronoiflag = 0;
   gridvoronoiedgeflag = 0;
   gridinit = 0;
   setqueryprobe();

   gridclose(NULL,NULL,NULL);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void gridflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   if (arg == 0) gridsflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) gridcontflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2) gridedgesflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 3) gridmedianflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 4) gridmedianedgeflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 5) gridvoronoiflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 6) gridvoronoiedgeflag = (int)XmToggleButtonGadgetGetState(w);
   if (gridcontflag == 1 && gridsflag == 0)
     {
      gridcontflag = 0;
      XmToggleButtonGadgetSetState(gadrawcont,FALSE,FALSE);
     }
   if (gridmedianflag == 1 && gridmedianedgeflag == 0)
     {
      gridmedianedgeflag = 1;
      XmToggleButtonGadgetSetState(gadrawmededge,TRUE,FALSE);
     }
   if (gridvoronoiflag == 1 && gridvoronoiedgeflag == 0)
     {
      gridvoronoiedgeflag = 1;
      XmToggleButtonGadgetSetState(gadrawvoredge,TRUE,FALSE);
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void nums(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
      ganodenumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
      gacellnumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2)
      gafacenumflag = (int)XmToggleButtonGadgetGetState(w);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void gridcolorsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  void fillnodevalue(int ifld, double *value);
  void fillcellvalue(int ifld, double *value);

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(gamatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ganfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(gacfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(gaflagsbut,FALSE,FALSE);

   if (arg == 0)
     {
      gridclrflag = 0;
      gridfieldactive = -1; 
      XmToggleButtonGadgetSetState(gamatsbut,TRUE,FALSE);
     }

   if (arg == 1)
     {
      gridclrflag = 1;
      gridfieldactive = currganfld;
      XmToggleButtonGadgetSetState(ganfldsbut,TRUE,FALSE);

      fillnodevalue(currganfld,gridfield);
     }

   if (arg == 2)
     {
      gridclrflag = 2;
      gridfieldactive = currgacfld + numvars;
      XmToggleButtonGadgetSetState(gacfldsbut,TRUE,FALSE);

      fillcellvalue(currgacfld,gridfield);
     }

   if (arg == 3)
     {
      gridclrflag = 3;
      gridfieldactive = currgaflag + numvars + cnumvars + fnumvars;
      XmToggleButtonGadgetSetState(gaflagsbut,TRUE,FALSE);
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void gaclear(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char blank[1] = "";

   for (i = 0; i < MAXTXT; i++)
     {
      XmTextFieldSetString(nodestxt[i],blank);
      XmTextFieldSetString(cellstxt[i],blank);
     }
   XmTextFieldSetString(cellbegtxt,blank);
   XmTextFieldSetString(cellendtxt,blank);
}


void ganfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currganfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDGANFIELD, ifld);
}


void gacfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currgacfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDGACFIELD, ifld);
}


void gaflagformpop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(gaflagform);
}


void gafclose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(gaflagform);
}


void gaselect_new_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string;

   arg = (long int)client_data;
   currgaflag = arg;
   string = XmStringCreate(flagname_cell[arg],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(gaflaglab,XmNlabelString,string,NULL);
   XmStringFree(string);

   XtUnmanageChild(gaflagform);

   if (gridclrflag == 3)
      gridfieldactive = numvars + arg;

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void dispcell(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ival, ngn, ngc, gselnodes[MAXTXT], gselcells[MAXTXT];
  int i, j, k, itst1, itst2, icbeg, icend, cellcount, ii, id;
  short oldnodesflag, oldpolysflag, oldlinesflag, oldcellsflag,
        oldcellcontflag, oldcelledgesflag, oldtracerflag, oldnodenumflag,
        oldcellnumflag, oldcellmedianedgeflag; 
  int oldcontour_on[MAXCONTOURS+MAXMATS+2];
  char *tmpstr, tmpstr2[10];
  char str[30];
  XmString string;

   arg = (long int)client_data;

   /*  Get node and cell selection nos.  */
   ngn = 0;
   ngc = 0;
   for (i = 0; i < MAXTXT; i++)
     {
      tmpstr = XmTextFieldGetString(nodestxt[i]);
      ival = atoi(tmpstr);
      XtFree(tmpstr);
      if (ival > 0)
        {
         gselnodes[ngn] = ival;
         ngn++;
        }
   
      tmpstr = XmTextFieldGetString(cellstxt[i]);
      ival = atoi(tmpstr);
      XtFree(tmpstr);
      if (ival > 0)
        {
         gselcells[ngc] = ival;
         ngc++;
        }
     }

   /*  Get cell selection range.  */
   tmpstr = XmTextFieldGetString(cellbegtxt);
   icbeg = atoi(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(cellendtxt);
   icend = atoi(tmpstr);
   XtFree(tmpstr);
   if (icbeg > icend) icbeg = icend;
   sprintf(tmpstr2,"%d",icbeg);
   XmTextFieldSetString(cellbegtxt,tmpstr2);
   sprintf(tmpstr2,"%d",icend);
   XmTextFieldSetString(cellendtxt,tmpstr2);

   /*  Select cells for display.  */
   for (i = 0; i < ncells; i++)
     {

      /*  If new display, unselect all cells.  */
      if (arg == 0) gridselectflag[i] = 0;

      ii = i;
      if (structuredflag == 1)
        {
         ii = 0;
         fillstructcell(i,0);
        }
     
      /*  Select (add) cells that have all selected nodes.  */
      itst1 = 1;
      if (ngn == 0) itst1 = 0;
      for (j = 0; j < ngn; j++)
        {
         itst2 = 0;
         for (k = 0; k < cells[ii].ncnodes; k++)
           {
            id = cells[ii].cnodes[k];
            if (nodeidflag) id = nodeids[id];
            if (id == gselnodes[j])
              itst2 = 1;
           }
         if (itst2 == 0) itst1 = 0;
         if (itst1 == 0) break;
        }
      if (itst1 == 1) gridselectflag[i] = 1;

      /*  Set cells that have been selected.  */
      id = i+1;
      if (cellidflag) id = cellids[i];
      for (j = 0; j < ngc; j++)
        {
         if (id == gselcells[j]) gridselectflag[i] = 1;
        }

      /*  Check cell range.  */
      for (j = icbeg; j <= icend; j++)
        {
         if (j > 0 && id == j) gridselectflag[i] = 1;
        }
     }


   /*  Count cells selected.  */
   cellcount = 0;
   for (i = 0; i < ncells; i++)
      if (gridselectflag[i] == 1) cellcount++;
   sprintf(str,"%d cells selected",cellcount);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ccountlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Center on selected cells. */
   if (cellcount > 0)
     {
      gridanalflag = 1;
      oldnodesflag = nodesflag;
      oldnodenumflag = nodenumflag;
      oldpolysflag = polysflag;
      oldlinesflag = linesflag;
      oldcellsflag = cellsflag;
      oldcellnumflag = cellnumflag;
      oldcellcontflag = cellcontflag;
      oldcelledgesflag = celledgesflag;
      oldcellmedianedgeflag = cellmedianedgeflag;
      oldtracerflag = tracerflag;
      for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++) 
        oldcontour_on[i] = contour_on[i];
      nodesflag = 0;
      nodenumflag = 0;
      polysflag = 0;
      linesflag = 0;
      cellsflag = 0;
      cellnumflag = 0;
      celledgesflag = 0;
      cellcontflag = 0;
      tracerflag = 0;
      for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++) 
        contour_on[i] = 0;
      autocenter();
      nodesflag = oldnodesflag;
      nodenumflag = oldnodenumflag;
      polysflag = oldpolysflag;
      linesflag = oldlinesflag;
      cellsflag = oldcellsflag;
      cellnumflag = oldcellnumflag;
      celledgesflag = oldcelledgesflag;
      cellmedianedgeflag = oldcellmedianedgeflag;
      cellcontflag = oldcellcontflag;
      tracerflag = oldtracerflag;
      for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++) 
        contour_on[i] = oldcontour_on[i];
      setqueryprobe();
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void preadnodesel(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(w);
   XtFree(tmpstr);
}


void preadcellsel(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(w);
   XtFree(tmpstr);
}


void preadbegend(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(w);
   XtFree(tmpstr);
}


void makegridanalform(parent)
 Widget parent;
{
  Widget exitbut, dispbut, dispbut2, nsellab, lastwidget,
         csellab, cbeglab, cendlab, gafldtxt, ganfldnewfld, 
         gaflagnewflag, gacfldnewfld, gaflagbox, gaflagclose,
         gaflagbuts[MAXFLAGS], closebut, gaclearbut; 
  int i;
  Arg wargs[20];
  char str[20];
  static char str1[22] = 
           "Select Cells\nby nodes";
  static char str2[23] = 
           "Select Cells\nby number";
  XmString string;

   string = XmStringCreate("Grid Analysis",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   gridanalform = XmCreateFormDialog(parent,"GridAnalysis",wargs,i);
      XmStringFree(string);
   
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   closebut = XtVaCreateManagedWidget("Closebut",
              xmPushButtonGadgetClass,
              gridanalform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(closebut,XmNactivateCallback,gridclose,NULL);
 
   string = XmStringCreate(" Exit ",XmSTRING_DEFAULT_CHARSET);
   exitbut = XtVaCreateManagedWidget("Exitbut",
             xmPushButtonGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, closebut,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(exitbut,XmNactivateCallback,gridexit,NULL);
 
   string = XmStringCreateLtoR("Start New\nDisplay",
                               XmSTRING_DEFAULT_CHARSET);
   dispbut = XtVaCreateManagedWidget("Distbut",
             xmPushButtonGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, exitbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(dispbut,XmNactivateCallback,dispcell,
                    (caddr_t) 0);

   string = XmStringCreateLtoR("Add To\nDisplay",
                               XmSTRING_DEFAULT_CHARSET);
   dispbut2 = XtVaCreateManagedWidget("Distbut2",
              xmPushButtonGadgetClass,
              gridanalform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, exitbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, dispbut,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(dispbut2,XmNactivateCallback,dispcell,
                    (caddr_t) 1);

   string = XmStringLtoRCreate(str1,
                           XmSTRING_DEFAULT_CHARSET);
   nsellab = XtVaCreateManagedWidget("Nsellab",
             xmLabelGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, dispbut,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   lastwidget = nsellab;
   for (i = 0; i < MAXTXT; i++)
     {
      sprintf(str,"Nodestxt%d",i);
      nodestxt[i] = XtVaCreateManagedWidget(str,
                    xmTextFieldWidgetClass,
                    gridanalform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, lastwidget,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 8,
                    XmNcolumns, 8,
                    XmNvalue, "",
                    NULL);
         XtAddCallback(nodestxt[i],XmNactivateCallback,
                       preadnodesel,(caddr_t) i);
      lastwidget = nodestxt[i];
     }
 
   string = XmStringCreateLtoR("Clear\nEntries",
                               XmSTRING_DEFAULT_CHARSET);
   gaclearbut = XtVaCreateManagedWidget("Gaclearbut",
                xmPushButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, lastwidget,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(gaclearbut,XmNactivateCallback,gaclear,
                    (caddr_t) 0);

   string = XmStringLtoRCreate(str2,
                           XmSTRING_DEFAULT_CHARSET);
   csellab = XtVaCreateManagedWidget("Csellab",
             xmLabelGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, dispbut,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, nsellab,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   lastwidget = csellab;
   for (i = 0; i < MAXTXT; i++)
     {
      sprintf(str,"Cellstxt%d",i);
      cellstxt[i] = XtVaCreateManagedWidget(str,
                    xmTextFieldWidgetClass,
                    gridanalform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, lastwidget,
                    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                    XmNleftWidget, csellab,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 8,
                    XmNcolumns, 8,
                    XmNvalue, "",
                    NULL);
         XtAddCallback(cellstxt[i],XmNactivateCallback,
                       preadcellsel,(caddr_t) i);
      lastwidget = cellstxt[i];
     }

   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   cbeglab = XtVaCreateManagedWidget("Cbeglab",
             xmLabelGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, lastwidget,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, nsellab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   cellbegtxt = XtVaCreateManagedWidget("Cellbegtxt",
                xmTextFieldWidgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, lastwidget,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cbeglab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(cellbegtxt,XmNactivateCallback,preadbegend,
                    (caddr_t) 0);

   string = XmStringCreate("End  ",XmSTRING_DEFAULT_CHARSET);
   cendlab = XtVaCreateManagedWidget("Cendlab",
             xmLabelGadgetClass,
             gridanalform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cellbegtxt,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, nsellab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   cellendtxt = XtVaCreateManagedWidget("Cellendtxt",
                xmTextFieldWidgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cellbegtxt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cendlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(cellendtxt,XmNactivateCallback,preadbegend,
                    (caddr_t) 1);

   string = XmStringCreate("  ",XmSTRING_DEFAULT_CHARSET);
   ccountlab = XtVaCreateManagedWidget("Ccountlab",
               xmLabelGadgetClass,
               gridanalform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cellendtxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   /*  Create grid display controls.  */
 
   string = XmStringCreate("Faces",
            XmSTRING_DEFAULT_CHARSET);
   gadrawcell = XtVaCreateManagedWidget("Gadrawcell",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ccountlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(gadrawcell,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 0);
 
   string = XmStringCreate("Contours",
            XmSTRING_DEFAULT_CHARSET);
   gadrawcont = XtVaCreateManagedWidget("Gadrawcont",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, gadrawcell,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(gadrawcont,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 1);
  
   string = XmStringCreate("Edges",
            XmSTRING_DEFAULT_CHARSET);
   gadrawedge = XtVaCreateManagedWidget("Gadrawedge",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, gadrawcont,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(gadrawedge,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 2);
  
   string = XmStringCreate("Median Mesh Faces",
            XmSTRING_DEFAULT_CHARSET);
   gadrawmedface = XtVaCreateManagedWidget("Gadrawmedface",
                   xmToggleButtonGadgetClass,
                   gridanalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, gadrawedge,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(gadrawmedface,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 3);
  
   string = XmStringCreate("Median Mesh Edges",
            XmSTRING_DEFAULT_CHARSET);
   gadrawmededge = XtVaCreateManagedWidget("Gadrawmededge",
                   xmToggleButtonGadgetClass,
                   gridanalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, gadrawmedface,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(gadrawmededge,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 4);
  
   string = XmStringCreate("Voronoi Mesh Faces",
            XmSTRING_DEFAULT_CHARSET);
   gadrawvorface = XtVaCreateManagedWidget("Gadrawvorface",
                   xmToggleButtonGadgetClass,
                   gridanalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, gadrawmededge,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(gadrawvorface,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 5);
  
   string = XmStringCreate("Voronoi Mesh Edges",
            XmSTRING_DEFAULT_CHARSET);
   gadrawvoredge = XtVaCreateManagedWidget("Gadrawvoredge",
                   xmToggleButtonGadgetClass,
                   gridanalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, gadrawvorface,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(gadrawvoredge,XmNvalueChangedCallback,
                    gridflags,(caddr_t) 6);

   string = XmStringCreate("Node Numbers",XmSTRING_DEFAULT_CHARSET);
   ganodenumbut = XtVaCreateManagedWidget("Ganodenumbut",
                  xmToggleButtonGadgetClass,
                  gridanalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, gadrawvoredge,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  NULL);
      XmStringFree(string);
      XtAddCallback(ganodenumbut,XmNvalueChangedCallback,nums,
                    (caddr_t) 0);

   string = XmStringCreate("Cell Numbers",XmSTRING_DEFAULT_CHARSET);
   gacellnumbut = XtVaCreateManagedWidget("Gacellnumbut",
                  xmToggleButtonGadgetClass,
                  gridanalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ganodenumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  NULL);
      XmStringFree(string);
      XtAddCallback(gacellnumbut,XmNvalueChangedCallback,nums,
                    (caddr_t) 1);

   string = XmStringCreate("Face Numbers",XmSTRING_DEFAULT_CHARSET);
   gafacenumbut = XtVaCreateManagedWidget("Gafacenumbut",
                  xmToggleButtonGadgetClass,
                  gridanalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, gacellnumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  NULL);
      XmStringFree(string);
      XtAddCallback(gafacenumbut,XmNvalueChangedCallback,nums,
                    (caddr_t) 2);
 
   /*  Create color select buttons.  */
 
   string = XmStringCreate("Color By:",XmSTRING_DEFAULT_CHARSET);
   gafldtxt = XtVaCreateManagedWidget("Gafldtxt",
              xmLabelGadgetClass,
              gridanalform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, gafacenumbut,
              XmNtopOffset, 20,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
  
   string = XmStringCreate("Materials",
            XmSTRING_DEFAULT_CHARSET);
   gamatsbut = XtVaCreateManagedWidget("Gamatsbut",
               xmToggleButtonGadgetClass,
               gridanalform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, gafldtxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, TRUE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(gamatsbut,XmNvalueChangedCallback,
                    gridcolorsel,(caddr_t) 0);
  
   string = XmStringCreate("Node Field:",
            XmSTRING_DEFAULT_CHARSET);
   ganfldsbut = XtVaCreateManagedWidget("Ganfldsbut",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, gamatsbut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(ganfldsbut,XmNvalueChangedCallback,
                    gridcolorsel,(caddr_t) 1);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   ganfldlab = XtVaCreateManagedWidget("Ganfldlab",
               xmLabelGadgetClass,
               gridanalform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ganfldsbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ganfldnewfld = XtVaCreateManagedWidget("Ganfldnewfld",
                  xmPushButtonGadgetClass,
                  gridanalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ganfldlab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNleftOffset, 20,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(ganfldnewfld,XmNactivateCallback,ganfldfld,
                    NULL);
      currganfld = 0;
      if (numvars < 1)
        {
         XtSetSensitive(ganfldsbut,FALSE);
         XtSetSensitive(ganfldlab,FALSE);
         XtSetSensitive(ganfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Cell Field:",
            XmSTRING_DEFAULT_CHARSET);
   gacfldsbut = XtVaCreateManagedWidget("Gacfldsbut",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ganfldnewfld,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(gacfldsbut,XmNvalueChangedCallback,
                    gridcolorsel,(caddr_t) 2);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   gacfldlab = XtVaCreateManagedWidget("Gacfldlab",
               xmLabelGadgetClass,
               gridanalform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, gacfldsbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   gacfldnewfld = XtVaCreateManagedWidget("Gacfldnewfld",
                  xmPushButtonGadgetClass,
                  gridanalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, gacfldlab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNleftOffset, 20,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(gacfldnewfld,XmNactivateCallback,gacfldfld,
                    NULL);
      currgacfld = 0;
      if (cnumvars < 1)
        {
         XtSetSensitive(gacfldsbut,FALSE);
         XtSetSensitive(gacfldlab,FALSE);
         XtSetSensitive(gacfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Flag:",
            XmSTRING_DEFAULT_CHARSET);
   gaflagsbut = XtVaCreateManagedWidget("Gaflagsbut",
                xmToggleButtonGadgetClass,
                gridanalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, gacfldnewfld,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(gaflagsbut,XmNvalueChangedCallback,
                    gridcolorsel,(caddr_t) 3);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   gaflaglab = XtVaCreateManagedWidget("Gaflaglab",
               xmLabelGadgetClass,
               gridanalform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, gaflagsbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("New Flag",XmSTRING_DEFAULT_CHARSET);
   gaflagnewflag = XtVaCreateManagedWidget("Gaflagnewflag",
                   xmPushButtonGadgetClass,
                   gridanalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, gaflaglab,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNleftOffset, 20,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(gaflagnewflag,XmNactivateCallback,gaflagformpop,
                    NULL);
      currgaflag = 0;
      if (numflags_cell < 1)
        {
         XtSetSensitive(gaflagsbut,FALSE);
         XtSetSensitive(gaflaglab,FALSE);
        }
      if (numflags_cell < 2) XtSetSensitive(gaflagnewflag,FALSE);

   /*  Make flag selection form, if needed.  */
   if (numflags_cell > 1)
     {
      string = XmStringCreate("Select Flag",XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      gaflagform = XmCreateFormDialog(gridanalform,"Gaflagform",
                                      wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      gaflagclose = XtVaCreateManagedWidget("Gaflagclose",
                    xmPushButtonGadgetClass,
                    gaflagform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(gaflagclose,XmNactivateCallback,gafclose,NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, gaflagclose);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      gaflagbox = XmCreateRadioBox(gaflagform,"Gaflagbox",wargs,i);
      XtManageChild(gaflagbox);

      for (i = 0; i < numflags_cell; i++)
        {
         string = XmStringCreate(flagname_cell[i],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Gaflagbuts%d",i);
         gaflagbuts[i] = XtVaCreateManagedWidget(str,
                         xmToggleButtonGadgetClass,
                         gaflagbox,
                         XmNlabelString, string,
                         XmNspacing, 1,
                         XmNselectColor, yellowpix,
                         XmNmarginHeight, 0,
                         XmNmarginWidth, 0,
                         NULL);
            XmStringFree(string);
            XtAddCallback(gaflagbuts[i],XmNvalueChangedCallback,
                          gaselect_new_flag,(caddr_t) i);

         if (i == 0) XtVaSetValues(gaflagbuts[0],XmNset,TRUE,NULL);
        }   
     }

   /*  If the mesh is not all tets, unset voronoi buttons.  */
   if (alltetmesh == 0)
     {
      XtSetSensitive(gadrawvorface,FALSE);
      XtSetSensitive(gadrawvoredge,FALSE);
     }
}


void gridformgetnfield(int i)
{
  XmString string;
  void fillnodevalue(int ifld, double *value);

   currganfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ganfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (gridclrflag == 1)
     {
      gridfieldactive = currganfld;
      fillnodevalue(currganfld,gridfield);
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void setgridnfldlab(int ifld)
{
  XmString string;

   string = XmStringCreate(fieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ganfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void gridformgetcfield(int i)
{
  XmString string;
  void fillcellvalue(int ifld, double *value);

   currgacfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(gacfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (gridclrflag == 2)
     {
      gridfieldactive = currgacfld + numvars;
      fillcellvalue(currgacfld,gridfield);
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void setgridcfldlab(int i)
{
  XmString string;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(gacfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void gridanalinit()
{
  int i;

   if (gridinit == 0)
     {
      gridinit = 1;

      /*  Allocate gridfield data array.  */
      gridfldalloc = (nnodes >= ncells) ? nnodes+1 : ncells;
      gridfield=(double *)malloc(gridfldalloc * sizeof(double));
      if (gridfield == NULL) memerr();

      /*  Allocate gridselectflag.  */
      gridselalloc = ncells;
      gridselectflag = (char *)malloc(ncells*sizeof(char));
      if (gridselectflag == NULL) memerr();
     }
}


void gridanalfree()
{
   FREE(gridfield);
   FREE(gridselectflag);
   gridinit = 0;
}


void gridfillfield()
{
  int i, cactive, nalloc;
  void fillnodevalue(int ifld, double *value);
  void fillcellvalue(int ifld, double *value);

   /*  First check that the field size is large enough.  */
   nalloc = (nnodes >= ncells) ? nnodes+1 : ncells;
   if (nalloc > gridfldalloc)
     {
      gridfldalloc = nalloc;
      gridfield=(double *)realloc(gridfield,gridfldalloc*sizeof(double));
      if (gridfield == NULL) memerr();
     }

   /*  If drawing node or cell field data    */
   /*  gird analysis cells, fill gridfield.  */
   if (gridfieldactive > -1 && gridfieldactive < numvars + cnumvars)
     {
      if (gridfieldactive < numvars)
        {
         fillnodevalue(gridfieldactive,gridfield);
        }
      else
        {
         cactive = gridfieldactive - numvars;
         fillcellvalue(cactive,gridfield);
        }
     }

   /*  Now check that the gridselect array is large enough.  */
   if (gridselalloc < ncells)
     {
      nalloc = gridselalloc;
      gridselalloc = ncells;
      gridselectflag = (char *)malloc(ncells*sizeof(char));
      if (gridselectflag == NULL) memerr();
      for (i = nalloc; i < ncells; i++) gridselectflag[i] = 0;
     }
}
