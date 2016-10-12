#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
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

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget drawnode, drawpts, drawsphs, drawvects, drawnnum, nmatsbut, 
              nfldsbut, nfldlab, nflagsbut, nflaglab, nodeflagform, 
              n0matbut, ngrpsbut, drawnghost, nunselbut;
static short currnfld, currnflag, nfieldactive, ndrawflag, ndrawpts, ndrawsph, 
             nnumflag;



void nodeformpop()
{
  int i;
  XmString string;

   ndrawpts = 1;  ndrawsph = 0;
   if (nodesflag == 2) ndrawsph = 1;
   if (ndrawsph == 1) ndrawpts = 0;
   XmToggleButtonGadgetSetState(drawnode,nodesflag,FALSE);
   XmToggleButtonGadgetSetState(drawpts,ndrawpts,FALSE);
   XmToggleButtonGadgetSetState(drawsphs,ndrawsph,FALSE);
   XmToggleButtonGadgetSetState(drawvects,vectorflag,FALSE);
   XmToggleButtonGadgetSetState(drawnnum,nodenumflag,FALSE);

   XmToggleButtonGadgetSetState(n0matbut,node0matflag,FALSE);
   XmToggleButtonGadgetSetState(nunselbut,drawunselnodes,FALSE);

   XmToggleButtonGadgetSetState(nmatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(nfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(nflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ngrpsbut,FALSE,FALSE);
   if (nodefieldactive == -2) 
      XmToggleButtonGadgetSetState(ngrpsbut,TRUE,FALSE);
   else if (nodefieldactive == -1) 
      XmToggleButtonGadgetSetState(nmatsbut,TRUE,FALSE);
   else if (nodefieldactive >= numvars)
     {
      XmToggleButtonGadgetSetState(nflagsbut,TRUE,FALSE);
      currnflag = nodefieldactive - numvars;
     }
   else
     {
      XmToggleButtonGadgetSetState(nfldsbut,TRUE,FALSE);
      currnfld = nodefieldactive;
     }

   i = currnfld;
   if (numvars > 0)
     {
      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currnflag;
   if (numflags_node > 0)
     {
      string = XmStringCreate(flagname_node[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nflaglab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   if (nodeghostflag)
      XmToggleButtonGadgetSetState(drawnghost,drawnodeghosts,FALSE);

   ndrawflag = nodesflag;
   nfieldactive = nodefieldactive;
   nnumflag = nodenumflag;

   MyManageChild(nodeform);
}

void nclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodeselform);
   if (numvars > 0)
      XtUnmanageChild(nfieldform);
   if (numflags_node > 1)
      XtUnmanageChild(nodeflagform);
   XtUnmanageChild(nodeform);
}


void node_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  double * readnodevalue(int ifld);
  void fillnodevalue(int ifld, double *value);
  void fillnodegrpcolr();
  void drawghostnodeoptions();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   nodesflag = ndrawflag;
   if (ndrawsph && nodesflag > 0) nodesflag = 2;
   nodefieldactive = nfieldactive;
   nodenumflag = nnumflag;
   setqueryprobe();

   /*  Check ghost node options.   */
   if (nodeghostflag) drawghostnodeoptions();

   if (nodefieldactive > -1 && nodefieldactive < numvars && 
       (nodesflag || nodenumflag || vectorflag))
     {
      if (nodefield == NULL)
         nodefield = readnodevalue(nodefieldactive);
      else
         fillnodevalue(nodefieldactive,nodefield);
     }
   else
      FREE(nodefield);

   if (nodefieldactive == -2)
      fillnodegrpcolr();
   else
     FREE(nodegrpcolr);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void nodeflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   if (arg == 0)
     ndrawflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1)
     nnumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2)
     ndrawpts = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 3)
     ndrawsph = (int)XmToggleButtonGadgetGetState(w);
/*
   if (nnumflag == 1 && ndrawflag == 0)
     {
      nnumflag = 0;
      XmToggleButtonGadgetSetState(drawnnum,FALSE,FALSE);
     }
*/
}


void n0matset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) node0matflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) drawunselnodes = (int)XmToggleButtonGadgetGetState(w);
}


void drawnghostset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   drawnodeghosts = (int)XmToggleButtonGadgetGetState(w);
}


void nvectdraw(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void fillnodevects();

   vectorflag = (int)XmToggleButtonGadgetGetState(w);
   if (vectors == 0)
     {
      vectorflag = 0;
      XmToggleButtonGadgetSetState(w,vectorflag,FALSE);
     }
   else
     {
      if (vectorflag)
         fillnodevects();
      else
        {
         FREE(uic);  FREE(vic);  FREE(wic);
        }
/*
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
*/
     }
}


void nodecolorsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(nmatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(nfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(nflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ngrpsbut,FALSE,FALSE);

   if (arg == 0)
     {
      nfieldactive = -1; 
      XmToggleButtonGadgetSetState(nmatsbut,TRUE,FALSE);
     }

   if (arg == 1)
     {
      nfieldactive = currnfld;
      XmToggleButtonGadgetSetState(nfldsbut,TRUE,FALSE);
      if (datalimpopflag) fieldlimpop(currnfld);
     }

   if (arg == 2)
     {
      nfieldactive = currnflag + numvars;
      XmToggleButtonGadgetSetState(nflagsbut,TRUE,FALSE);
     }

   if (arg == 3)
     {
      nfieldactive = -2; 
      XmToggleButtonGadgetSetState(ngrpsbut,TRUE,FALSE);
     }
}


void nfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDNFIELD, ifld);
}


void setnodeselect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   nodeselformpop();
}


void nflagformpop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(nodeflagform);
}


void nfclose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodeflagform);
}


void nselect_new_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string;

   arg = (long int)client_data;
   currnflag = arg;
   string = XmStringCreate(flagname_node[arg],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nflaglab,XmNlabelString,string,NULL);
   XmStringFree(string);

   XtUnmanageChild(nodeflagform);

   if (nfieldactive >= numvars)
      nfieldactive = numvars + arg;
}


void popbldvect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   vectbldformpop();
}


void makenodeform(parent)
 Widget parent;
{
  Widget nodeclose, nodeapply, vectbar, vectmenu, bldvect, vectbut,
         nodesel, nfldtxt, nfldnewfld, nflagnewflag, lastwidget,
         nflagbox, nflagclose, nflagbuts[MAXFLAGS], ndrawbox;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodeform = XmCreateFormDialog(parent,"Nodes",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nodeclose = XtVaCreateManagedWidget("Nodeclose",
               xmPushButtonGadgetClass,
               nodeform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nodeclose,XmNactivateCallback,nclose,NULL);

   string = XmStringCreate(" Apply ",XmSTRING_DEFAULT_CHARSET);
   nodeapply = XtVaCreateManagedWidget("Nodeapply",
               xmPushButtonGadgetClass,
               nodeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nodeclose,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nodeapply,XmNactivateCallback,node_apply,
                    NULL);
 
   string = XmStringCreate("Nodes",
            XmSTRING_DEFAULT_CHARSET);
   drawnode = XtVaCreateManagedWidget("Drawnode",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nodeapply,
              XmNtopOffset, 10,
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
      XtAddCallback(drawnode,XmNvalueChangedCallback,
                    nodeflags,(caddr_t) 0);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, drawnode);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 40);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ndrawbox = XmCreateRadioBox(nodeform,"Ndrawbox",wargs,i);
   XtManageChild(ndrawbox);
 
   string = XmStringCreate("Points",
            XmSTRING_DEFAULT_CHARSET);
   drawpts = XtVaCreateManagedWidget("Drawpts",
              xmToggleButtonGadgetClass,
              ndrawbox,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(drawpts,XmNvalueChangedCallback,
                    nodeflags,(caddr_t) 2);
 
   string = XmStringCreate("Spheres",
            XmSTRING_DEFAULT_CHARSET);
   drawsphs = XtVaCreateManagedWidget("Drawsphs",
              xmToggleButtonGadgetClass,
              ndrawbox,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(drawsphs,XmNvalueChangedCallback,
                    nodeflags,(caddr_t) 3);
  
   string = XmStringCreate("Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawnnum = XtVaCreateManagedWidget("Drawnnum",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawsphs,
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
      XtAddCallback(drawnnum,XmNvalueChangedCallback,
                    nodeflags,(caddr_t) 1);
 
   /*  Create color select buttons.  */
 
   string = XmStringCreate("Color By:",XmSTRING_DEFAULT_CHARSET);
   nfldtxt = XtVaCreateManagedWidget("Nfldtxt",
             xmLabelWidgetClass,
             nodeform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, drawnnum,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
  
   string = XmStringCreate("Materials",
            XmSTRING_DEFAULT_CHARSET);
   nmatsbut = XtVaCreateManagedWidget("Nmatsbut",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nfldtxt,
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
      XtAddCallback(nmatsbut,XmNvalueChangedCallback,
                    nodecolorsel,(caddr_t) 0);
  
   string = XmStringCreate("Node Field:",
            XmSTRING_DEFAULT_CHARSET);
   nfldsbut = XtVaCreateManagedWidget("Nfldsbut",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nmatsbut,
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
      XtAddCallback(nfldsbut,XmNvalueChangedCallback,
                    nodecolorsel,(caddr_t) 1);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   nfldlab = XtVaCreateManagedWidget("Nfldlab",
             xmLabelWidgetClass,
             nodeform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, nfldsbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 40,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   nfldnewfld = XtVaCreateManagedWidget("Nfldnewfld",
               xmPushButtonGadgetClass,
               nodeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nfldlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNleftOffset, 40,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nfldnewfld,XmNactivateCallback,nfldfld,
                    NULL);
      currnfld = 0;
      if (numvars < 1)
        {
         XtSetSensitive(nfldsbut,FALSE);
         XtSetSensitive(nfldlab,FALSE);
         XtSetSensitive(nfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Flag:",
            XmSTRING_DEFAULT_CHARSET);
   nflagsbut = XtVaCreateManagedWidget("Nflagsbut",
               xmToggleButtonGadgetClass,
               nodeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nfldnewfld,
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
      XtAddCallback(nflagsbut,XmNvalueChangedCallback,
                    nodecolorsel,(caddr_t) 2);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   nflaglab = XtVaCreateManagedWidget("Nflaglab",
              xmLabelWidgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nflagsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Flag",XmSTRING_DEFAULT_CHARSET);
   nflagnewflag = XtVaCreateManagedWidget("Nflagnewflag",
                  xmPushButtonWidgetClass,
                  nodeform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nflaglab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNleftOffset, 40,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nflagnewflag,XmNactivateCallback,nflagformpop,
                    NULL);
      currnflag = 0;
      if (numflags_node < 1)
        {
         XtSetSensitive(nflagsbut,FALSE);
         XtSetSensitive(nflaglab,FALSE);
        }
      if (numflags_node < 2) XtSetSensitive(nflagnewflag,FALSE);
  
   string = XmStringCreate("Groups",
            XmSTRING_DEFAULT_CHARSET);
   ngrpsbut = XtVaCreateManagedWidget("Ngrpsbut",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nflagnewflag,
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
      XtAddCallback(ngrpsbut,XmNvalueChangedCallback,
                    nodecolorsel,(caddr_t) 3);
      if (nnodegrps < 1) XtSetSensitive(ngrpsbut,FALSE);
  
   string = XmStringLtoRCreate("Show unselected\nnodes in black.",
            XmSTRING_DEFAULT_CHARSET);
   nunselbut = XtVaCreateManagedWidget("Nunselbut",
               xmToggleButtonGadgetClass,
               nodeform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ngrpsbut,
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
      XtAddCallback(nunselbut,XmNvalueChangedCallback,
                    n0matset,(caddr_t) 1);
  
   string = XmStringLtoRCreate("Show nodes with\n0 material no.",
            XmSTRING_DEFAULT_CHARSET);
   n0matbut = XtVaCreateManagedWidget("N0matbut",
              xmToggleButtonGadgetClass,
              nodeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nunselbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, FALSE,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(n0matbut,XmNvalueChangedCallback,
                    n0matset,(caddr_t) 0);

   lastwidget = n0matbut;
 
   if (nodeghostflag)
     {
      string = XmStringLtoRCreate("Show ghost nodes.",
               XmSTRING_DEFAULT_CHARSET);
      drawnghost = XtVaCreateManagedWidget("Drawnghost",
                   xmToggleButtonGadgetClass,
                   nodeform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, n0matbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
         XmStringFree(string);
         XtAddCallback(drawnghost,XmNvalueChangedCallback,
                       drawnghostset,0);

      lastwidget = drawnghost;
     }
 
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, lastwidget);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 20);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   vectbar = XmCreateMenuBar(nodeform,"Vectbar",wargs,i);
   vectmenu = XmCreatePulldownMenu(vectbar,"Vectmenu",NULL,0);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   drawvects = XtVaCreateManagedWidget("Drawvects",
               xmToggleButtonGadgetClass,
               vectmenu,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(drawvects,XmNvalueChangedCallback,
                    nvectdraw,NULL);

   string = XmStringCreate("Build Vector",XmSTRING_DEFAULT_CHARSET);
   bldvect = XtVaCreateManagedWidget("Bldvect",
             xmPushButtonGadgetClass,
             vectmenu,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bldvect,XmNactivateCallback,popbldvect,NULL);

   string = XmStringCreate("Vectors",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNlabelString,string);
   i++; XtSetArg(wargs[i], XmNsubMenuId,vectmenu);
   i++;
   vectbut = XmCreateCascadeButton(vectbar,"Vectbut",
                                   wargs,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(vectbut);
      if (numvars < 2) XtSetSensitive(vectbut,FALSE);

   XtManageChild(vectbar);
 
   string = XmStringCreate("Select",XmSTRING_DEFAULT_CHARSET);
   nodesel = XtVaCreateManagedWidget("Nodesel",
             xmPushButtonWidgetClass,
             nodeform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, vectbar,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(nodesel,XmNactivateCallback,
                    setnodeselect,NULL);

   /*  Make flag selection form, if needed.  */
   if (numflags_node > 1)
     {
      string = XmStringCreate("Select Flag",XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      nodeflagform = XmCreateFormDialog(nflagnewflag,"Nodeflagform",
                                        wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      nflagclose = XtVaCreateManagedWidget("Nflagclose",
                   xmPushButtonGadgetClass,
                   nodeflagform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(nflagclose,XmNactivateCallback,nfclose,NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, nflagclose);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      nflagbox = XmCreateRadioBox(nodeflagform,"Nflagbox",wargs,i);
      XtManageChild(nflagbox);

      for (i = 0; i < numflags_node; i++)
        {
         string = XmStringCreate(flagname_node[i],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Nflagbuts%d",i);
         nflagbuts[i] = XtVaCreateManagedWidget(str,
                        xmToggleButtonGadgetClass,
                        nflagbox,
                        XmNlabelString, string,
                        XmNspacing, 1,
                        XmNselectColor, yellowpix,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        NULL);
            XmStringFree(string);
            XtAddCallback(nflagbuts[i],XmNvalueChangedCallback,
                          nselect_new_flag,(caddr_t) i);

         if (i == 0) XtVaSetValues(nflagbuts[0],XmNset,TRUE,NULL);
        }
     }

   makenodeselform(nodesel);
   if (numvars > 1) makevectbldform(nodesel);
}


void nodeformgetnfield(int i)
{
  int j;
  XmString string;

   currnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (nfieldactive > -1 && nfieldactive < numvars)
      nfieldactive = currnfld;

   j = XmToggleButtonGadgetGetState(nfldsbut);
   if (j && datalimpopflag) fieldlimpop(currnfld);
}


void setnodefldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(fieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(nfldsbut);
   if (j && datalimpopflag) fieldlimpop(currnfld);
}


void setnodegrpsensitivity()
{
   XtSetSensitive(ngrpsbut,TRUE);
}


void fillnodegrpcolr()
{
  int i, j, k, ii, jj, kk, id;

   if (nodegrpcolr == NULL)
      nodegrpcolr = (short *)malloc((nnodes+1)*sizeof(short));

   /*  Loop through nodes, and get a node id.  */
   for (ii = 1; ii < nnodes+1; ii++)
     {
      nodegrpcolr[ii] = 0;
      id = ii;
      if (nodeidflag) id = nodeids[ii];

      for (j = 0; j < nnodegrps; j++)
        {
         kk = j + 1;
         if (kk > MAXMATS)
           {
            kk = kk % MAXMATS;
            if (kk == 0) kk = MAXMATS;
           }
         k = numnodegrp[j];
         for (i = 0; i < k; i++)
           {
            jj = nodegrps[j][i];
            if (jj == id)  nodegrpcolr[ii] = kk;
           }
        }
     }
}


void drawghostnodeoptions()
{
  int i, j;

   for (i = 0; i < nnodeghosts; i++)
     {
      j = nodeghosts[i];
      nodeselectflag[j] = drawnodeghosts;
     }
}
