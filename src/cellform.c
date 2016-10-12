#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>
#include <math.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget drawcell, drawcont, drawedge, drawcnums, cmatsbut,
              cnfldsbut, cnfldlab, cflagsbut, cflaglab, cellflagform,
              ccfldsbut, ccfldlab, drawcvects, c0matbut, drawshade,
              drawfrefine, drawmedianedge, drawnnums, drawnorm, drawfnums,
              drawhiddenedge, cellcolrform, cffldsbut, cffldlab, drawfvects, 
              cgrpsbut,cfgrpsbut, drawcghost, drawnext2ghost, cwfrmbut, 
              cbhddnbut;
static short currcnfld, currcflag, currfaces, curredges, cfieldactive,
             ccontflag, cnumflag, currccfld, cellclrflag, cshadeflag,
             cfrefineflag, cmedianedgeflag, cnodenumflag, cnormflag,
             currcffld, cfacenumflag, currhiddenedges;


void cellformpop()
{
   if (cellsflag || celledgesflag || cellmedianedgeflag || cellnumflag ||
       cellnodenumflag || cellfacenumflag) 
      cellhiddenedgesflag = 0;
   XmToggleButtonGadgetSetState(drawcell,cellsflag,FALSE);
   XmToggleButtonGadgetSetState(drawcont,cellcontflag,FALSE);
   XmToggleButtonGadgetSetState(drawshade,cellshadeflag,FALSE);
   XmToggleButtonGadgetSetState(drawfrefine,facerefineflag,FALSE);
   XmToggleButtonGadgetSetState(drawnorm,cellnormflag,FALSE);
   XmToggleButtonGadgetSetState(drawedge,celledgesflag,FALSE);
   XmToggleButtonGadgetSetState(drawmedianedge,cellmedianedgeflag,FALSE);
   XmToggleButtonGadgetSetState(drawcnums,cellnumflag,FALSE);
   XmToggleButtonGadgetSetState(drawnnums,cellnodenumflag,FALSE);
   XmToggleButtonGadgetSetState(drawfnums,cellfacenumflag,FALSE);
   XmToggleButtonGadgetSetState(drawhiddenedge,cellhiddenedgesflag,FALSE);

   XmToggleButtonGadgetSetState(c0matbut,cell0matflag,FALSE);
   XmToggleButtonGadgetSetState(cwfrmbut,drawunselcells,FALSE);

   if (cellghostflag)
     {
      XmToggleButtonGadgetSetState(drawcghost,drawghosts,FALSE);
      XmToggleButtonGadgetSetState(drawnext2ghost,drawghostface,FALSE);
     }

   currfaces = cellsflag;
   curredges = celledgesflag;
   cfieldactive = cellfieldactive;
   ccontflag = cellcontflag;
   cnumflag = cellnumflag;
   cnodenumflag = cellnodenumflag;
   cshadeflag = cellshadeflag;
   cfrefineflag = facerefineflag;
   cnormflag = cellnormflag;
   cmedianedgeflag = cellmedianedgeflag;
   cfacenumflag = cellfacenumflag;
   currhiddenedges = cellhiddenedgesflag;

   MyManageChild(cellform);
}

void cclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellselform);
   XtUnmanageChild(cellexpform);
   if (numvars > 0)
      XtUnmanageChild(nfieldform);
   if (numflags_cell > 1)
      XtUnmanageChild(cellflagform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   if (fnumvars > 0)
      XtUnmanageChild(ffieldform);
   XtUnmanageChild(cellcolrform);
   XtUnmanageChild(cellform);
}


void cellcolrformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  XmString string;

   XmToggleButtonGadgetSetState(cmatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cnfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ccfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cffldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cgrpsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfgrpsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cbhddnbut,FALSE,FALSE);
   if (cellfieldactive == -4)
     {
      cellclrflag = 7; 
      XmToggleButtonGadgetSetState(cbhddnbut,TRUE,FALSE);
     }
   else if (cellfieldactive == -3)
     {
      cellclrflag = 6; 
      XmToggleButtonGadgetSetState(cfgrpsbut,TRUE,FALSE);
     }
   else if (cellfieldactive == -2)
     {
      cellclrflag = 5; 
      XmToggleButtonGadgetSetState(cgrpsbut,TRUE,FALSE);
     }
   else if (cellfieldactive == -1)
     {
      cellclrflag = 0; 
      XmToggleButtonGadgetSetState(cmatsbut,TRUE,FALSE);
     }
   else if (cellfieldactive >= numvars + cnumvars + fnumvars)
     {
      cellclrflag = 4; 
      XmToggleButtonGadgetSetState(cflagsbut,TRUE,FALSE);
      currcflag = cellfieldactive - (numvars + cnumvars + fnumvars);
     }
   else if (cellfieldactive < numvars)
     {
      cellclrflag = 1; 
      XmToggleButtonGadgetSetState(cnfldsbut,TRUE,FALSE);
      currcnfld = cellfieldactive;
     }
   else if (cellfieldactive < numvars + cnumvars)
     {
      cellclrflag = 2; 
      XmToggleButtonGadgetSetState(ccfldsbut,TRUE,FALSE);
      currccfld = cellfieldactive - numvars;
     }
   else
     {
      cellclrflag = 3; 
      XmToggleButtonGadgetSetState(cffldsbut,TRUE,FALSE);
      currcffld = cellfieldactive - (numvars + cnumvars);
     }

   i = currcnfld;
   if (numvars > 0)
     {
      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cnfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currccfld;
   if (cnumvars > 0)
     {
      string = XmStringCreate(cfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(ccfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currcffld;
   if (fnumvars > 0)
     {
      string = XmStringCreate(ffieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cffldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currcflag;
   if (numflags_cell > 0)
     {
      string = XmStringCreate(flagname_cell[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cflaglab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   MyManageChild(cellcolrform);
}

void clrclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (numvars > 0)
      XtUnmanageChild(nfieldform);
   if (numflags_cell > 1)
      XtUnmanageChild(cellflagform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   if (fnumvars > 0)
      XtUnmanageChild(ffieldform);
   XtUnmanageChild(cellcolrform);
}


void cell_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int nalloc;
  void fillnodevalue(int ifld, double *value);
  void fillcellvalue(int ifld, double *value);
  void fillfacevalue(int ifld, double *value);
  void fillcellgrpcolr();
  void fillfacegrpcolr();
  void setfacesel();
  void drawghostcelloptions();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   if (cellclrflag == 0) 
      cfieldactive = -1; 

   if (cellclrflag == 1)
      cfieldactive = currcnfld; 

   if (cellclrflag == 2)
      cfieldactive = currccfld + numvars; 

   if (cellclrflag == 3)
      cfieldactive = currcffld + numvars + cnumvars; 

   if (cellclrflag == 4) 
      cfieldactive = currcflag + numvars + cnumvars + fnumvars;

   if (cellclrflag == 5)
     {
      cfieldactive = -2;
      fillcellgrpcolr();
     }
   else
     FREE(cellgrpcolr);

   if (cellclrflag == 6)
     {
      cfieldactive = -3;
      fillfacegrpcolr();
     }
   else
     FREE(facegrpcolr);

   if (cellclrflag == 7)
      cfieldactive = -4;

   if (ccontflag == 1 && cellclrflag == 1)
      ccontflag = 1;
   else
     {
      ccontflag = 0;
      cellcontflag = 0;
      XmToggleButtonGadgetSetState(drawcont,FALSE,FALSE);
     }

   if (cfrefineflag == 1 && currfaces == 1)
      facerefineflag = 1;
   else
     {
      cfrefineflag = 0;
      facerefineflag = 0;
      XmToggleButtonGadgetSetState(drawfrefine,FALSE,FALSE);
     }

   cellsflag = currfaces;
   celledgesflag = curredges;
   cellcontflag = ccontflag;
   cellfieldactive = cfieldactive;
   cellnumflag = cnumflag;
   cellnodenumflag = cnodenumflag;
   cellshadeflag = cshadeflag;
   cellnormflag = cnormflag;
   cellmedianedgeflag = cmedianedgeflag;
   cellfacenumflag = cfacenumflag;
   cellhiddenedgesflag = currhiddenedges;
   setqueryprobe();

   if ((cellsflag || celledgesflag || cellmedianedgeflag || cvectorflag ||
        fvectorflag || cellnumflag || cellhiddenedgesflag) && 
       cellfieldactive > -1 && cellfieldactive < numvars + cnumvars + fnumvars)
     {
      if (cellfield == NULL)
        {
         nalloc = (nnodes >= ncells) ? nnodes+1 : ncells;
         if (fnumvars > 0)
           {
            if (ncellfaces > nalloc) nalloc = ncellfaces;
           }
         cellfield=(double *)malloc((nalloc)*sizeof(double));
         if (cellfield == NULL) memerr();
        }

      if (cellfieldactive < numvars)
         fillnodevalue(cellfieldactive,cellfield);
      else if (cellfieldactive < numvars + cnumvars)
         fillcellvalue(cellfieldactive-numvars,cellfield);
      else
         fillfacevalue(cellfieldactive-(numvars+cnumvars),cellfield);
     }
   else
      FREE(cellfield);

   /*  Check ghost cell options.   */
   if (cellghostflag) drawghostcelloptions();

   if (cellsflag) setfacesel();

   if (cellsflag == 0)
     {
      FREE(facesel);
      FREE(cellfacedraw);
    }
   if (cellshadeflag == 0 && cellnormflag == 0)
     {
      FREE(facenormx);
      FREE(facenormy);
      FREE(facenormz);
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cdrawflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, chkhidden;

   arg = (long int)client_data;
   
   if (arg == 0) currfaces = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) ccontflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2) curredges = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 3) cnumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 4) cshadeflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 5) cfrefineflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 6) cmedianedgeflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 7) cnodenumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 8) cnormflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 9) cfacenumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 10) currhiddenedges = (int)XmToggleButtonGadgetGetState(w);
   chkhidden = currhiddenedges;
   if (arg < 10) chkhidden = 0;
   if (chkhidden == 1)
     {
      if (currfaces == 1)
        {
         currfaces = 0;
         XmToggleButtonGadgetSetState(drawcell,FALSE,FALSE);
        }
      if (curredges == 1)
        {
         curredges = 0;
         XmToggleButtonGadgetSetState(drawedge,FALSE,FALSE);
        }
      if (cmedianedgeflag == 1)
        {
         cmedianedgeflag = 0;
         XmToggleButtonGadgetSetState(drawmedianedge,FALSE,FALSE);
        }
      if (cnumflag == 1)
        {
         cnumflag = 0;
         XmToggleButtonGadgetSetState(drawcnums,FALSE,FALSE);
        }
      if (cnodenumflag == 1)
        {
         cnodenumflag = 0;
         XmToggleButtonGadgetSetState(drawnnums,FALSE,FALSE);
        }
      if (cfacenumflag == 1)
        {
         cfacenumflag = 0;
         XmToggleButtonGadgetSetState(drawfnums,FALSE,FALSE);
        }
     }
   if (ccontflag == 1 && currfaces == 0)
     {
      ccontflag = 0;
      XmToggleButtonGadgetSetState(drawcont,FALSE,FALSE);
     }
   if (cshadeflag == 1 && currfaces == 0)
     {
      cshadeflag = 0;
      XmToggleButtonGadgetSetState(drawshade,FALSE,FALSE);
     }
   if (cnormflag == 1 && currfaces == 0)
     {
      cnormflag = 0;
      XmToggleButtonGadgetSetState(drawnorm,FALSE,FALSE);
     }
   if (ccontflag == 1 && (cellclrflag != 1 || currfaces == 0))
     {
      ccontflag = 0;
      XmToggleButtonGadgetSetState(drawcont,FALSE,FALSE);
     }
   /* if (cfrefineflag == 1 && (cellclrflag != 1 || currfaces == 0)) */
   if (cfrefineflag == 1 && currfaces == 0)
     {
      cfrefineflag = 0;
      XmToggleButtonGadgetSetState(drawfrefine,FALSE,FALSE);
     }
   if (currfaces || curredges || cmedianedgeflag || cnumflag || 
       cnodenumflag || cfacenumflag)
     {
      currhiddenedges = 0;
      XmToggleButtonGadgetSetState(drawhiddenedge,FALSE,FALSE);
     }
}


void c0matset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) cell0matflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) drawunselcells = (int)XmToggleButtonGadgetGetState(w);
}


void drawghostset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) drawghosts = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) drawghostface = (int)XmToggleButtonGadgetGetState(w);
}


void cvectdraw(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void fillcellvects();

   cvectorflag = (int)XmToggleButtonGadgetGetState(w);
   if (cvectors == 0)
     {
      cvectorflag = 0;
      XmToggleButtonGadgetSetState(w,cvectorflag,FALSE);
     }
   else
     {
      if (cvectorflag)
         fillcellvects();
      else
        {
         FREE(cuic);  FREE(cvic);  FREE(cwic);
        }
     }
}


void fvectdraw(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void fillfacevects();

   fvectorflag = (int)XmToggleButtonGadgetGetState(w);
   if (fvectors == 0)
     {
      fvectorflag = 0;
      XmToggleButtonGadgetSetState(w,fvectorflag,FALSE);
     }
   else
     {
      if (fvectorflag)
         fillfacevects();
      else
        {
         FREE(fuic);  FREE(fvic);  FREE(fwic);
        }
     }
}


void setcellselect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cellselformpop();
}


void setcellexpl(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cellexplodepop();
}


void cellcolorsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   cellclrflag = arg; 
   
   XmToggleButtonGadgetSetState(cmatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cnfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ccfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cffldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cgrpsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cfgrpsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cbhddnbut,FALSE,FALSE);

   if (arg == 0)
      XmToggleButtonGadgetSetState(cmatsbut,TRUE,FALSE);

   if (arg == 1)
     {
      XmToggleButtonGadgetSetState(cnfldsbut,TRUE,FALSE);
      if (datalimpopflag) fieldlimpop(currcnfld);
     }

   if (arg == 2)
     {
      XmToggleButtonGadgetSetState(ccfldsbut,TRUE,FALSE);
      if (datalimpopflag) cfieldlimpop(currccfld);
     }

   if (arg == 3)
     {
      XmToggleButtonGadgetSetState(cffldsbut,TRUE,FALSE);
      if (datalimpopflag) ffieldlimpop(currcffld);
     }

   if (arg == 4)
      XmToggleButtonGadgetSetState(cflagsbut,TRUE,FALSE);

   if (arg == 5)
      XmToggleButtonGadgetSetState(cgrpsbut,TRUE,FALSE);

   if (arg == 6)
     {
      if (cellsflag)
         XmToggleButtonGadgetSetState(cfgrpsbut,TRUE,FALSE);
      else
        {
         XmToggleButtonGadgetSetState(cmatsbut,TRUE,FALSE);
         cellclrflag = 0;
        }
     }

   if (arg == 7)
     {
      if (!currhiddenedges)
        {
         XmToggleButtonGadgetSetState(cbhddnbut,FALSE,FALSE);
         XmToggleButtonGadgetSetState(cmatsbut,TRUE,FALSE);
         cellclrflag = 0;
        }
      else
         XmToggleButtonGadgetSetState(cbhddnbut,TRUE,FALSE);
     }
}


void cnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDCNFIELD, ifld);
}


void ccfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currccfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDCCFIELD, ifld);
}


void cffldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcffld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(ffieldform, XmNx,fx, XmNy,fy, NULL);
 
   ffieldformpop(FFLDCFFIELD, ifld);
}


void cflagformpop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(cellflagform);
}


void cfclose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellflagform);
}


void cselect_new_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string;

   arg = (long int)client_data;
   currcflag = arg;
   string = XmStringCreate(flagname_cell[arg],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cflaglab,XmNlabelString,string,NULL);
   XmStringFree(string);

   XtUnmanageChild(cellflagform);
}


void popbldcvect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cvectbldformpop();
}


void popbldfvect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   fvectbldformpop();
}


void makecellform(parent)
 Widget parent;
{
  Widget cellclose, cellapply, cellsel, cellexpl, cclrbut, cclrclose,
         cnfldnewfld, cflagnewflag, cflagbox, cflagclose, ccfldnewfld,
         cffldnewfld, cflagbuts[MAXFLAGS], cvectbar, cvectmenu,
         bldcvect, cvectbut, fvectbar, fvectmenu, bldfvect, fvectbut,
         lastwidget;

  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Cells",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellform = XmCreateFormDialog(parent,"Cells",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cellclose = XtVaCreateManagedWidget("Cellclose",
               xmPushButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cellclose,XmNactivateCallback,cclose,NULL);

   string = XmStringCreate(" Apply ",XmSTRING_DEFAULT_CHARSET);
   cellapply = XtVaCreateManagedWidget("Cellapply",
               xmPushButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cellclose,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cellapply,XmNactivateCallback,cell_apply,
                    NULL);
 
   string = XmStringCreate("Faces",
            XmSTRING_DEFAULT_CHARSET);
   drawcell = XtVaCreateManagedWidget("Drawcell",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cellapply,
              XmNtopOffset, 5,
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
      XtAddCallback(drawcell,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 0);
 
   string = XmStringCreate("Contours",
            XmSTRING_DEFAULT_CHARSET);
   drawcont = XtVaCreateManagedWidget("Drawcont",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawcell,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(drawcont,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 1);
 
   string = XmStringCreate("Shaded",
            XmSTRING_DEFAULT_CHARSET);
   drawshade = XtVaCreateManagedWidget("Drawshade",
               xmToggleButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawcont,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(drawshade,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 4);
 
   string = XmStringCreate("Refine",
            XmSTRING_DEFAULT_CHARSET);
   drawfrefine = XtVaCreateManagedWidget("Drawfrefine",
                 xmToggleButtonGadgetClass,
                 cellform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, drawshade,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 40,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNset, FALSE,
                 XmNspacing, 2,
                 XmNselectColor, yellowpix,
                 NULL);
      XmStringFree(string);
      XtAddCallback(drawfrefine,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 5);
 
   string = XmStringCreate("Test Normals",
            XmSTRING_DEFAULT_CHARSET);
   drawnorm = XtVaCreateManagedWidget("Drawnorm",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawfrefine,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(drawnorm,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 8);
  
   string = XmStringCreate("Edges",
            XmSTRING_DEFAULT_CHARSET);
   drawedge = XtVaCreateManagedWidget("Drawedge",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawnorm,
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
      XtAddCallback(drawedge,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 2);
  
   string = XmStringCreate("Cell Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawcnums = XtVaCreateManagedWidget("Drawcnums",
               xmToggleButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawedge,
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
      XtAddCallback(drawcnums,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 3);
  
   string = XmStringCreate("Node Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawnnums = XtVaCreateManagedWidget("Drawnnums",
               xmToggleButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawcnums,
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
      XtAddCallback(drawnnums,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 7);
  
   string = XmStringCreate("Face Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawfnums = XtVaCreateManagedWidget("Drawfnums",
               xmToggleButtonGadgetClass,
               cellform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawnnums,
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
      XtAddCallback(drawfnums,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 9);
  
   string = XmStringCreate("Median Mesh Edges",
            XmSTRING_DEFAULT_CHARSET);
   drawmedianedge = XtVaCreateManagedWidget("Drawmedianedge",
                    xmToggleButtonGadgetClass,
                    cellform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, drawfnums,
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
      XtAddCallback(drawmedianedge,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 6);
  
   string = XmStringCreate("Hidden Line Edges",
            XmSTRING_DEFAULT_CHARSET);
   drawhiddenedge = XtVaCreateManagedWidget("Drawhiddenedge",
                    xmToggleButtonGadgetClass,
                    cellform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, drawmedianedge,
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
      XtAddCallback(drawhiddenedge,XmNvalueChangedCallback,
                    cdrawflags,(caddr_t) 10);
 
   cellclrflag = 0; 
   string = XmStringCreate("Color By:",XmSTRING_DEFAULT_CHARSET);
   cclrbut = XtVaCreateManagedWidget("Cclrbut",
             xmPushButtonWidgetClass,
             cellform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, drawhiddenedge,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cclrbut,XmNactivateCallback,cellcolrformpop,NULL);
  
   string = XmStringLtoRCreate("Show unselected cells\nin black wireframe.",
            XmSTRING_DEFAULT_CHARSET);
   cwfrmbut = XtVaCreateManagedWidget("Cwfrmbut",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cclrbut,
              XmNtopOffset, 20,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(cwfrmbut,XmNvalueChangedCallback,
                    c0matset,(caddr_t) 1);
  
   string = XmStringLtoRCreate("Show cells with\n0 material no.",
            XmSTRING_DEFAULT_CHARSET);
   c0matbut = XtVaCreateManagedWidget("C0matbut",
              xmToggleButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cwfrmbut,
              /* XmNtopOffset, 20, */
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(c0matbut,XmNvalueChangedCallback,
                    c0matset,(caddr_t) 0);

   lastwidget = c0matbut;
 
   if (cellghostflag)
     {
      string = XmStringLtoRCreate("Show ghost cells.",
               XmSTRING_DEFAULT_CHARSET);
      drawcghost = XtVaCreateManagedWidget("Drawcghost",
                   xmToggleButtonGadgetClass,
                   cellform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, c0matbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
         XmStringFree(string);
         XtAddCallback(drawcghost,XmNvalueChangedCallback,
                       drawghostset,(caddr_t)0);
  
      string = XmStringLtoRCreate("Show faces next\nto ghost cells.",
               XmSTRING_DEFAULT_CHARSET);
      drawnext2ghost = XtVaCreateManagedWidget("Drawnext2ghost",
                       xmToggleButtonGadgetClass,
                       cellform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, drawcghost,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       XmNset, FALSE,
                       XmNspacing, 2,
                       XmNselectColor, yellowpix,
                       NULL);
         XmStringFree(string);
         XtAddCallback(drawnext2ghost,XmNvalueChangedCallback,
                       drawghostset,(caddr_t)1);

      lastwidget = drawnext2ghost;
     }

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, lastwidget);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cvectbar = XmCreateMenuBar(cellform,"Cvectbar",wargs,i);
   cvectmenu = XmCreatePulldownMenu(cvectbar,"Cvectmenu",NULL,0);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   drawcvects = XtVaCreateManagedWidget("Drawcvects",
                xmToggleButtonGadgetClass,
                cvectmenu,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(drawcvects,XmNvalueChangedCallback,
                    cvectdraw,NULL);

   string = XmStringCreate("Build Vector",XmSTRING_DEFAULT_CHARSET);
   bldcvect = XtVaCreateManagedWidget("Bldcvect",
              xmPushButtonGadgetClass,
              cvectmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(bldcvect,XmNactivateCallback,popbldcvect,NULL);

   string = XmStringCreate("Cell Vectors",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNlabelString,string);
   i++; XtSetArg(wargs[i], XmNsubMenuId,cvectmenu);
   i++;
   cvectbut = XmCreateCascadeButton(cvectbar,"Cvectbut",
                                    wargs,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cvectbut);
      if (cnumvars < 2) XtSetSensitive(cvectbut,FALSE);

   XtManageChild(cvectbar);

  
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cvectbar);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   fvectbar = XmCreateMenuBar(cellform,"Fvectbar",wargs,i);
   fvectmenu = XmCreatePulldownMenu(fvectbar,"Fvectmenu",NULL,0);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   drawfvects = XtVaCreateManagedWidget("Drawfvects",
                xmToggleButtonGadgetClass,
                fvectmenu,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(drawfvects,XmNvalueChangedCallback,
                    fvectdraw,NULL);

   string = XmStringCreate("Build Vector",XmSTRING_DEFAULT_CHARSET);
   bldfvect = XtVaCreateManagedWidget("Bldfvect",
              xmPushButtonGadgetClass,
              fvectmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(bldfvect,XmNactivateCallback,popbldfvect,NULL);

   string = XmStringCreate("Face Vectors",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNlabelString,string);
   i++; XtSetArg(wargs[i], XmNsubMenuId,fvectmenu);
   i++;
   fvectbut = XmCreateCascadeButton(fvectbar,"Fvectbut",
                                    wargs,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(fvectbut);
      if (fnumvars < 2) XtSetSensitive(fvectbut,FALSE);

   XtManageChild(fvectbar);
 
   string = XmStringCreate("Select",XmSTRING_DEFAULT_CHARSET);
   cellsel = XtVaCreateManagedWidget("Cellsel",
             xmPushButtonWidgetClass,
             cellform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fvectbar,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cellsel,XmNactivateCallback,setcellselect,NULL);
  
   string = XmStringCreate("Explode",XmSTRING_DEFAULT_CHARSET);
   cellexpl = XtVaCreateManagedWidget("Cellexpl",
              xmPushButtonGadgetClass,
              cellform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cellsel,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cellexpl,XmNactivateCallback,setcellexpl,NULL);
 
   /*  Create color select form (Color By) and buttons.  */

   string = XmStringCreate("Cell Color By:",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellcolrform = XmCreateFormDialog(cclrbut,"Cellcolrform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cclrclose = XtVaCreateManagedWidget("Cclrclose",
               xmPushButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cclrclose,XmNactivateCallback,clrclose,NULL);
  
   string = XmStringCreate("Materials",
            XmSTRING_DEFAULT_CHARSET);
   cmatsbut = XtVaCreateManagedWidget("Cmatsbut",
              xmToggleButtonGadgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cclrclose,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, TRUE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(cmatsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 0);
  
   string = XmStringCreate("Node Field:",
            XmSTRING_DEFAULT_CHARSET);
   cnfldsbut = XtVaCreateManagedWidget("Cnfldsbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cmatsbut,
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
      XtAddCallback(cnfldsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 1);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   cnfldlab = XtVaCreateManagedWidget("Cnfldlab",
              xmLabelWidgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cnfldsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cnfldnewfld = XtVaCreateManagedWidget("Cnfldnewfld",
                 xmPushButtonGadgetClass,
                 cellcolrform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cnfldlab,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 40,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(cnfldnewfld,XmNactivateCallback,cnfldfld,
                    NULL);
      currcnfld = 0;
      if (numvars < 1)
        {
         XtSetSensitive(cnfldsbut,FALSE);
         XtSetSensitive(cnfldlab,FALSE);
         XtSetSensitive(cnfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Cell Field:",
            XmSTRING_DEFAULT_CHARSET);
   ccfldsbut = XtVaCreateManagedWidget("Ccfldsbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cnfldnewfld,
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
      XtAddCallback(ccfldsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 2);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   ccfldlab = XtVaCreateManagedWidget("Ccfldlab",
              xmLabelWidgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ccfldsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ccfldnewfld = XtVaCreateManagedWidget("Ccfldnewfld",
                 xmPushButtonGadgetClass,
                 cellcolrform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ccfldlab,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 40,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ccfldnewfld,XmNactivateCallback,ccfldfld,
                    NULL);
      currccfld = 0;
      if (cnumvars < 1)
        {
         XtSetSensitive(ccfldsbut,FALSE);
         XtSetSensitive(ccfldlab,FALSE);
         XtSetSensitive(ccfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Face Field:",
            XmSTRING_DEFAULT_CHARSET);
   cffldsbut = XtVaCreateManagedWidget("Cffldsbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ccfldnewfld,
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
      XtAddCallback(cffldsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 3);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   cffldlab = XtVaCreateManagedWidget("Cffldlab",
              xmLabelWidgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cffldsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cffldnewfld = XtVaCreateManagedWidget("Cffldnewfld",
                 xmPushButtonGadgetClass,
                 cellcolrform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cffldlab,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 40,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(cffldnewfld,XmNactivateCallback,cffldfld,
                    NULL);
      currcffld = 0;
      if (fnumvars < 1)
        {
         XtSetSensitive(cffldsbut,FALSE);
         XtSetSensitive(cffldlab,FALSE);
         XtSetSensitive(cffldnewfld,FALSE);
        }
  
   string = XmStringCreate("Flag:",
            XmSTRING_DEFAULT_CHARSET);
   cflagsbut = XtVaCreateManagedWidget("Cflagsbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cffldnewfld,
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
      XtAddCallback(cflagsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 4);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   cflaglab = XtVaCreateManagedWidget("Cflaglab",
              xmLabelWidgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cflagsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Flag",XmSTRING_DEFAULT_CHARSET);
   cflagnewflag = XtVaCreateManagedWidget("Cflagnewflag",
                  xmPushButtonWidgetClass,
                  cellcolrform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cflaglab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNleftOffset, 40,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cflagnewflag,XmNactivateCallback,cflagformpop,
                    NULL);
      currcflag = 0;
      if (numflags_cell < 1)
        {
         XtSetSensitive(cflagsbut,FALSE);
         XtSetSensitive(cflaglab,FALSE);
        }
      if (numflags_cell < 2) XtSetSensitive(cflagnewflag,FALSE);
  
   string = XmStringCreate("Cell Groups",
            XmSTRING_DEFAULT_CHARSET);
   cgrpsbut = XtVaCreateManagedWidget("Cgrpsbut",
              xmToggleButtonGadgetClass,
              cellcolrform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cflagnewflag,
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
      XtAddCallback(cgrpsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 5);
      if (ncellgrps < 1) XtSetSensitive(cgrpsbut,FALSE);
  
   string = XmStringCreate("Face Groups",
            XmSTRING_DEFAULT_CHARSET);
   cfgrpsbut = XtVaCreateManagedWidget("Cfgrpsbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cgrpsbut,
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
      XtAddCallback(cfgrpsbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 6);
      if (nfacegrps < 1) XtSetSensitive(cfgrpsbut,FALSE);
  
   string = XmStringCreate("Black hidden edges",
            XmSTRING_DEFAULT_CHARSET);
   cbhddnbut = XtVaCreateManagedWidget("Cbhddnbut",
               xmToggleButtonGadgetClass,
               cellcolrform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cfgrpsbut,
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
      XtAddCallback(cbhddnbut,XmNvalueChangedCallback,
                    cellcolorsel,(caddr_t) 7);

   /*  Make flag selection form, if needed.  */
   if (numflags_cell > 1)
     {
      string = XmStringCreate("Select Flag",XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cellflagform = XmCreateFormDialog(cflagnewflag,"Cellflagform",
                                        wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      cflagclose = XtVaCreateManagedWidget("Cflagclose",
                   xmPushButtonGadgetClass,
                   cellflagform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(cflagclose,XmNactivateCallback,cfclose,NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, cflagclose);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      cflagbox = XmCreateRadioBox(cellflagform,"Cflagbox",wargs,i);
      XtManageChild(cflagbox);

      for (i = 0; i < numflags_cell; i++)
        {
         string = XmStringCreate(flagname_cell[i],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Cflagbuts%d",i);
         cflagbuts[i] = XtVaCreateManagedWidget(str,
                        xmToggleButtonGadgetClass,
                        cflagbox,
                        XmNlabelString, string,
                        XmNspacing, 1,
                        XmNselectColor, yellowpix,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        NULL);
            XmStringFree(string);
            XtAddCallback(cflagbuts[i],XmNvalueChangedCallback,
                          cselect_new_flag,(caddr_t) i);

         if (i == 0) XtVaSetValues(cflagbuts[0],XmNset,TRUE,NULL);
        }   
     }

   makecellselform(cellsel);
   makecellexpform(cellsel);
   if (cnumvars > 1) makecvectbldform(cellsel);
   if (fnumvars > 1) makefvectbldform(cellsel);
/*
   makecellselform(cellform);
   makecellexpform(cellform);
   if (cnumvars > 1) makecvectbldform(cellform);
   if (fnumvars > 1) makefvectbldform(cellform);
*/
}


void cellformgetnfield(int i)
{
  int j;
  XmString string;

   currcnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(cnfldsbut);
   if (j && datalimpopflag) fieldlimpop(currcnfld);
}


void setcellnfldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(fieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(cnfldsbut);
   if (j && datalimpopflag) fieldlimpop(currcnfld);
}


void cellformgetcfield(int i)
{
  int j;
  XmString string;

   currccfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ccfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(ccfldsbut);
   if (j && datalimpopflag) cfieldlimpop(currccfld);
}


void setcellcfldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(cfieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ccfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(ccfldsbut);
   if (j && datalimpopflag) cfieldlimpop(currccfld);
}


void cellformgetffield(int i)
{
  int j;
  XmString string;

   currcffld = i;

   string = XmStringCreate(ffieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cffldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(cffldsbut);
   if (j && datalimpopflag) ffieldlimpop(currcffld);
}


void setcellffldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(ffieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cffldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(cffldsbut);
   if (j && datalimpopflag) cfieldlimpop(currcffld);
}


void setcellgrpsensitivity()
{
   XtSetSensitive(cgrpsbut,TRUE);
}


void setcellfgrpsensitivity()
{
   XtSetSensitive(cfgrpsbut,TRUE);
}


void setfacesel()
{
  int i, j, k, l, ii, icell, jcell, iface, jface, id, igrp;
  double xno, yno, zno;
  int iexp1, iexp2, *facecell1, *facecell2;
  short explodeflag, gocell, *matcolr, isel;
  char str[] = 
    "Error - not enough memory for face normals.\nCell shadeing turned off.";
  XmString string;
  void getfacenorm(int iface, double *xnorm, double *ynorm, double *znorm);

   /*  Select faces to draw.  */

   /*  Allocate cellfacedraw array.  */
   if (cellfacedraw == NULL)
      cellfacedraw = (char *)malloc(ncells * sizeof(char));

   /*  Allocate facesel, and face normal arrays if needed.  */
   if ((cellshadeflag || cellnormflag) && facenormx == NULL)
     {
      FREE(facenormx);
      FREE(facenormy);
      FREE(facenormz);
      facenormx = (float *)malloc(ncellfaces * sizeof(float));
      facenormy = (float *)malloc(ncellfaces * sizeof(float));
      facenormz = (float *)malloc(ncellfaces * sizeof(float));
      if (facenormx == NULL || facenormy == NULL || facenormz == NULL)
        {
         if (!movieflag)
           {
            errorfile = 2;
            string = XmStringLtoRCreate(str, XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
            XtManageChild(errorform);
            XmStringFree(string);
            XmToggleButtonGadgetSetState(drawshade,FALSE,FALSE);
           }
         else
           {
            fprintf(stderr, "Error - not enough memory for cell shading!\n");
           }
         cellshadeflag = 0;  cshadeflag = 0;
         cellnormflag = 0;  cnormflag = 0;
         FREE(facenormx);
         FREE(facenormy);
         FREE(facenormz);
        }
     }
   if (facesel == NULL) 
      facesel = (char *)malloc(ncellfaces * sizeof(char));

   for (i = 0; i < ncellfaces; i++) 
     facesel[i] = 0;

   if (cellshadeflag || cellnormflag)
     {
      for (i = 0; i < ncellfaces; i++)
        {
         facenormx[i] = 0.;
         facenormy[i] = 0.;
         facenormz[i] = 0.;
        } 
     }

   /*  Check for explode for these cells.  */
   explodeflag = 0;
   if (cexplodefact > 0.)
      explodeflag = 1;

   /*  If structured mesh, allocate facecells arrays.  */
   if (structuredflag == 1)
     {
      facecell1 = (int *)malloc(ncellfaces * sizeof(int));
      facecell2 = (int *)malloc(ncellfaces * sizeof(int));
     }

   /*  Set material or group color.  */
   matcolr = cellmats;
   if (cellfieldactive == -2) matcolr = cellgrpcolr;

   /*  Loop through cells to set selected cells.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)cellselectflag[icell];

      /*  Eliminate cells with material   */ 
      /*  or group < 1 unless requested.  */
      if (matcolr[icell] < 1 && cell0matflag == 0) gocell = 0;
 
      /*  See if the cell lies within the subset.  */
      if ((int)cellsubsetflag[icell] == 0)
         gocell = 0;

      cellfacedraw[icell] = gocell;

      /*  For structured mesh, get face data.  */
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,1);
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];
            facecell1[iface] = cellfaces[i].cell1;
            facecell2[iface] = cellfaces[i].cell2;
           }
        }
     }

   /*  Loop through faces and select faces.  */
   for (i = 0; i < ncellfaces; i++)
     {
      facesel[i] = 0;

      /*  If face groups not used, select face if outside face.  */
      if (!cellselfacegrp_on)
        {
         if (structuredflag == 1)
           {
            k = facecell1[i];
            l = facecell2[i];
           }
         else
           {
            k = cellfaces[i].cell1;
            l = cellfaces[i].cell2;
           }
         if (k > -1)
           {
            if ((int)cellfacedraw[k] == 1 && l == -1)
               facesel[i] = 1;
           }
         if (l > -1)
           {
            if ((int)cellfacedraw[l] == 1 && k == -1)
               facesel[i] = 1;
           }
         if ((int)facesel[i] == 0 && k > -1 && l > -1)
           {
            if ((int)cellfacedraw[k] == 1 && (int)cellfacedraw[l] == 0)
               facesel[i] = 1;
            if ((int)cellfacedraw[l] == 1 && (int)cellfacedraw[k] == 0)
               facesel[i] = 1;
           }

         /*  Check for different explode types.  */
         if ((int)facesel[i] == 0 && explodeflag == 1)
           {
            if (cellexpflag == -1)
              {
               iexp1 = (int)cellmats[k] - 1;
               iexp2 = (int)cellmats[l] - 1;
              }
            else
              {
               iexp1 = (int)cellflags[cellexpflag][k] - 1;
               iexp2 = (int)cellflags[cellexpflag][l] - 1;
              }
            if (iexp1 != iexp2) 
              facesel[i] = 1;
           }

         /*  Add/remove faces adjacent to ghost cells, as requested.  */
         if (l > -1 && cellghostflag)
           {
            if (cellflags[ghostcellflagid][l] == 2)
              {
               facesel[i] = drawghostface;
              }
           }

         /*  Add ghost faces, if requested.  */
         if (k > -1 && cellghostflag && drawghosts)
           {
            if (cellflags[ghostcellflagid][k] == 2)
              facesel[i] = 1;
           }
        }

      /*  If face groups selected, select those faces.  */
      if (cellselfacegrp_on && nfacegrpselected > 0)
        {
         id = i + 1;
         if (faceidflag) id = faceids[i];

         /*  Loop through selected face groups.  */
         isel = 0;
         for (j = 0; j < nfacegrpselected; j++)
           {
            if (isel) break;
            igrp = facegrpselected[j] - 1;
            k = numfacegrp[igrp];
            for (ii = 0; ii < k; ii++)
              {
               if (isel) break;
               l = facegrps[igrp][ii];
               if (l == id)
                 {
                  facesel[i] = 1;
                  isel = 1;
                 }
              }
           }
        }
     }

   /*  For 2D meshes, turn on the 1D faces or the    */
   /*  2D faces, depending on what's being colored.  */
   if (mesh2dflag > 0)
     {
      for (i = 0; i < ncellfaces; i++) facesel[i] = 0;
      if (mesh2dflag == 1)
        {
         for (i = ncell2dfaces; i < ncellfaces; i++) facesel[i] = 1;
        }
      if (mesh2dflag == 2)
        {
         if (cellclrflag == 3 || cellclrflag == 6)
           {
            for (i = 0; i < ncell2dfaces; i++) facesel[i] = 1;
           }
         else
           {
            for (i = ncell2dfaces; i < ncellfaces; i++) facesel[i] = 1;
           }
        }
     }

   /*  If 3D faces are colored by face group,  */
   /*  eliminate faces with group color < 1.   */
   if (cellfieldactive == -3)
     {
      for (i = 0; i < ncellfaces; i++)
        {
         if (facegrpcolr[i] < 1) facesel[i] = 0;
        }

      /*  Add faces with face groups.  */
      for (i = 0; i < ncellfaces; i++)
        {
         if (facegrpcolr[i] > 0) facesel[i] = 1;
        }
     }

   if (structuredflag == 1)
     {
      free(facecell1);  free(facecell2);
     }

   /*  Loop over cells and reset cellfacedraw flag */
   /*  for cells that have faces to draw.  Also, if   */
   /*  shaded faces requested, calculate face nomals. */ 
   for (icell = 0; icell < ncells; icell++)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)cellfacedraw[icell];

      /*  Check selected cell for a face to draw.  */
      if (gocell == 1)
        {

         /*  Fill cell structure faces for structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Loop over cell faces to see if any are drawn.  */
         k = 0;
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];
            if ((int)facesel[iface] == 1) k = 1;
           }
         if (k == 0) gocell = 0;
        }

      if (gocell == 0) cellfacedraw[icell] = 0;

      /*  If a cell face is drawn, calculate  */
      /*  face normals, if needed.            */
      if (gocell == 1 && (cellshadeflag || cellnormflag))
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Loop over cell faces.  */
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];

            /*  Get face if selected.  */
            if ((int)facesel[iface] == 1)
              {
               jface = iface;
               if (structuredflag == 1) jface = i;

               /*  Get and save face normal.  */
               getfacenorm(jface, &xno, &yno, &zno);
               facenormx[iface] = xno;
               facenormy[iface] = yno;
               facenormz[iface] = zno;
              }
           }
        }
     }
}


void getfacenorm(int jface, double *xnorm, double *ynorm, double *znorm)
{
  int j, k, l, ierr, nfv;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3, xno, yno, zno, t,
         dist, epslon = 1.0e-10;

   /*  Check that there are more than two vertices in    */
   /*  the face.  If not, return zeros for the normals.  */
   l = cellfaces[jface].nverts;
   if (l < 3)
     {
      *xnorm = 0.;
      *ynorm = 0.;
      *znorm = 0.;
      return;
     }
        
   /*  Calculate face average location to     */
   /*  use as the first point of the normal.  */
   x1 = 0.;  y1 = 0.;  z1 = 0.;
   for (j = 0; j < l; j++)
     {
      k = cellfaces[jface].fverts[j];
      x1 += nodes[k].x;
      y1 += nodes[k].y;
      z1 += nodes[k].z;
     }
   x1 /= (double)l;
   y1 /= (double)l;
   z1 /= (double)l;

   k = cellfaces[jface].fverts[0];
   x2 = nodes[k].x;  y2 = nodes[k].y;  z2 = nodes[k].z;
   k = cellfaces[jface].fverts[1];
   x3 = nodes[k].x;  y3 = nodes[k].y;  z3 = nodes[k].z;
   xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
   yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
   zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
   t = sqrt(xno*xno + yno*yno + zno*zno);

   /*  Check that we have a valid normal.  */
   if (t == 0.)
     {

      /*  Find a second point.  */
      nfv = cellfaces[jface].nverts;
      ierr = 0;
      j = 1;
      dist = 0.;
      while (dist < epslon && j < nfv)
        {
         k = cellfaces[jface].fverts[j];
         x2 = nodes[k].x;  y2 = nodes[k].y;  z2 = nodes[k].z;
         dist = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) +
                (z2-z1)*(z2-z1);
         j++;
        }
      if (j == nfv) ierr = 1;

      /*  Find a third non-linear point.  */
      dist = 0.;
      while (dist < epslon && j < nfv)
        {
         k = cellfaces[jface].fverts[j];
         x3 = nodes[k].x;  y3 = nodes[k].y;  z3 = nodes[k].z;
         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         dist =  xno*xno + yno*yno + zno*zno;
         j++;
        }
      if (j == nfv && dist < epslon)
         ierr = 1;

      /*  Recalculate the normal components.  */
      if (ierr == 0)
        {
         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         t = sqrt(xno*xno + yno*yno + zno*zno);
        }
      else
        {
         xno = 1.;  yno = 1.;  zno = 1.;  t = 1.;
        }
     }
   *xnorm = xno/t;
   *ynorm = yno/t;
   *znorm = zno/t;
}


void fillcellgrpcolr()
{
  int i, j, k, ii, jj, kk, id;

   if (cellgrpcolr == NULL)
      cellgrpcolr = (short *)malloc(ncells*sizeof(short));
   for (i = 0; i < ncells; i++) cellgrpcolr[i] = 0;

   /*  If no cellids exist, fill the cell group color.  */
   if (!cellidflag)
     {
      for (j = 0; j < ncellgrps; j++)
        {
         kk = j + 1;
         if (kk > MAXMATS)
           {
            kk = kk % MAXMATS;
            if (kk == 0) kk = MAXMATS;
           }
         k = numcellgrp[j];
         for (i = 0; i < k; i++)
           {
            jj = cellgrps[j][i] - 1;
            cellgrpcolr[jj] = kk;
           }
        }      
     }

   /*  If cellid flag exist, loop through cells, and get a cell id.  */
   else
     {
      for (ii = 0; ii < ncells; ii++)
        {
         id = ii + 1;
         if (cellidflag) id = cellids[ii];

         for (j = 0; j < ncellgrps; j++)
           {
            kk = j + 1;
            if (kk > MAXMATS)
              {
               kk = kk % MAXMATS;
               if (kk == 0) kk = MAXMATS;
              }
            k = numcellgrp[j];
            for (i = 0; i < k; i++)
              {
               jj = cellgrps[j][i];
               if (jj == id)  cellgrpcolr[ii] = kk;
              }
           }
        }
     }
}


void fillfacegrpcolr()
{
  int i, j, k, ii, jj, kk, id;

   if (facegrpcolr == NULL)
      facegrpcolr = (short *)malloc(ncellfaces*sizeof(short));
   for (i = 0; i < ncellfaces; i++) facegrpcolr[i] = 0;

   /*  If no faceids exist, fill the face group color.  */
   if (!faceidflag)
     {
      for (j = 0; j < nfacegrps; j++)
        {
         kk = j + 1;
         if (kk > MAXMATS)
           {
            kk = kk % MAXMATS;
            if (kk == 0) kk = MAXMATS;
           }
         k = numfacegrp[j];
         for (i = 0; i < k; i++)
           {
            jj = facegrps[j][i] - 1;
            facegrpcolr[jj] = kk;
           }
        }      
     }

   /*  If faceids exist, loop through faces, and get a face id.  */
   else
     {
      for (ii = 0; ii < ncellfaces; ii++)
        {
         id = ii + 1;
         if (faceidflag) id = faceids[ii];

         for (j = 0; j < nfacegrps; j++)
           {
            kk = j + 1;
            if (kk > MAXMATS)
              {
               kk = kk % MAXMATS;
               if (kk == 0) kk = MAXMATS;
              }
            k = numfacegrp[j];
            for (i = 0; i < k; i++)
              {
               jj = facegrps[j][i];
               if (jj == id) facegrpcolr[ii] = kk;
              }
           }
        }
     }
}


void drawghostcelloptions()
{
  int i, j;

   /*  Add or remove ghost cells as requested.  */
   for (i = 0; i < ncellghosts; i ++)
     {
      j = cellghosts[i];
      cellselectflag[j] = drawghosts;
     }

   /*  Add or remove cells adjacent to ghost cells as requested.  */
/*
   for (i = 0; i < ncellghosts; i ++)
     {
      ii = cellghosts[i];
      for (j = 0; j < cells[ii].nfaces; j++)
        {
         jj = cells[ii].facenos[j];
         if (cellfaces[jj].cell2 >= 0)
           {
            k = cellfaces[jj].cell2;
            if (cellflags[ghostcellflagid][k] == 1)
               cellselectflag[k] = drawghostface;
           }
        }
     }
*/
}
