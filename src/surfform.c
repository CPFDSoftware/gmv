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

static Widget drawsurf, drawscont, drawsedge, drawsnums, smatsbut,
              snfldsbut, snfldlab, sflagsbut, sflaglab, surfflagform,
              ssfldsbut, ssfldlab, drawsvects, s0matbut, drawsshade,
              drawsfrefine, drawsnnums, sgrpsbut;
static short currsnfld, currsflag, currsfaces, currsedges, sfieldactive,
             scontflag, snumflag, currssfld, surfclrflag, sshadeflag,
             sfrefineflag, snodenumflag;


void surfformpop()
{
  int i;
  XmString string;

   XmToggleButtonGadgetSetState(drawsurf,surfflag,FALSE);
   XmToggleButtonGadgetSetState(drawscont,surfcontflag,FALSE);
   XmToggleButtonGadgetSetState(drawsshade,surfshadeflag,FALSE);
   XmToggleButtonGadgetSetState(drawsfrefine,surfrefineflag,FALSE);
   XmToggleButtonGadgetSetState(drawsedge,surfedgesflag,FALSE);
   XmToggleButtonGadgetSetState(drawsnums,surfnumflag,FALSE);
   XmToggleButtonGadgetSetState(drawsnnums,surfnodenumflag,FALSE);

   XmToggleButtonGadgetSetState(s0matbut,surf0matflag,FALSE);

   XmToggleButtonGadgetSetState(smatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(snfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ssfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sgrpsbut,FALSE,FALSE);
   if (surffieldactive == -2)
     {
      surfclrflag = 4; 
      XmToggleButtonGadgetSetState(sgrpsbut,TRUE,FALSE);
     }
   else if (surffieldactive == -1)
     {
      surfclrflag = 0; 
      XmToggleButtonGadgetSetState(smatsbut,TRUE,FALSE);
     }
   else if (surffieldactive >= numvars + snumvars)
     {
      surfclrflag = 3; 
      XmToggleButtonGadgetSetState(sflagsbut,TRUE,FALSE);
      currsflag = surffieldactive - numvars;
     }
   else if (surffieldactive < numvars)
     {
      surfclrflag = 1; 
      XmToggleButtonGadgetSetState(snfldsbut,TRUE,FALSE);
      currsnfld = surffieldactive;
     }
   else
     {
      surfclrflag = 2; 
      XmToggleButtonGadgetSetState(ssfldsbut,TRUE,FALSE);
      currssfld = surffieldactive - numvars;
     }

   i = currsnfld;
   if (numvars > 0)
     {
      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(snfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currssfld;
   if (snumvars > 0)
     {
      string = XmStringCreate(sfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(ssfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   i = currsflag;
   if (numflags_surf > 0)
     {
      string = XmStringCreate(flagname_surf[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sflaglab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   currsfaces = surfflag;
   currsedges = surfedgesflag;
   sfieldactive = surffieldactive;
   scontflag = surfcontflag;
   snumflag = surfnumflag;
   snodenumflag = surfnodenumflag;
   sshadeflag = surfshadeflag;
   sfrefineflag = surfrefineflag;

   MyManageChild(surfform);
}

void sclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfselform);
   XtUnmanageChild(surfexpform);
   if (numvars > 0)
      XtUnmanageChild(nfieldform);
   if (numflags_surf > 1)
      XtUnmanageChild(surfflagform);
   if (snumvars > 0)
      XtUnmanageChild(sfieldform);
   XtUnmanageChild(surfform);
}


void surf_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int nalloc;
  void fillnodevalue(int ifld, double *value);
  void fillsurfvalue(int ifld, double *value);
  void fillsurfgrpcolr();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   if (surfclrflag == 0) 
      sfieldactive = -1; 

   if (surfclrflag == 1)
      sfieldactive = currsnfld; 

   if (surfclrflag == 2)
      sfieldactive = currssfld + numvars; 

   if (surfclrflag == 3) 
      sfieldactive = currsflag + numvars + snumvars;

   if (surfclrflag == 4)
     {
      sfieldactive = -2;
      fillsurfgrpcolr();
     }
   else
     FREE(surfgrpcolr);

   if (sfrefineflag == 1 && surfclrflag == 1)
      surfrefineflag = 1;
   else
     {
      sfrefineflag = 0;
      surfrefineflag = 0;
      XmToggleButtonGadgetSetState(drawsfrefine,FALSE,FALSE);
     }

   surfflag= currsfaces;
   surfedgesflag = currsedges;
   surffieldactive = sfieldactive;
   surfcontflag = scontflag;
   surfnumflag = snumflag;
   surfnodenumflag = snodenumflag;
   surfshadeflag = sshadeflag;

   if ((surfflag|| surfedgesflag || svectorflag || surfnumflag) && 
       surffieldactive > -1 && surffieldactive < numvars + snumvars)
     {
      if (surffield == NULL)
        {
         nalloc = (nnodes >= nsurf) ? nnodes+1 : nsurf;
         surffield=(double *)malloc((nalloc)*sizeof(double));
         if (surffield == NULL) memerr();
        }

      if (surffieldactive < numvars)
         fillnodevalue(surffieldactive,surffield);
      else
         fillsurfvalue(surffieldactive-numvars,surffield);
     }
   else
      FREE(surffield);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sdrawflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   if (arg == 0) currsfaces = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) scontflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2) currsedges = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 3) snumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 4) sshadeflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 5) sfrefineflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 7) snodenumflag = (int)XmToggleButtonGadgetGetState(w);
   if (scontflag == 1 && currsfaces == 0)
     {
      scontflag = 0;
      XmToggleButtonGadgetSetState(drawscont,FALSE,FALSE);
     }
   if (sshadeflag == 1 && currsfaces == 0)
     {
      sshadeflag = 0;
      XmToggleButtonGadgetSetState(drawsshade,FALSE,FALSE);
     }
   if (sfrefineflag == 1 && (surfclrflag != 1 || currsfaces == 0))
     {
      sfrefineflag = 0;
      XmToggleButtonGadgetSetState(drawsfrefine,FALSE,FALSE);
     }
}


void s0matset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   surf0matflag = (int)XmToggleButtonGadgetGetState(w);
}


void svectdraw(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void fillsurfvects();

   svectorflag = (int)XmToggleButtonGadgetGetState(w);
   if (svectors == 0)
     {
      svectorflag = 0;
      XmToggleButtonGadgetSetState(w,svectorflag,FALSE);
     }
   else
     {
      if (svectorflag)
         fillsurfvects();
      else
        {
         FREE(suic);  FREE(svic);  FREE(swic);
        }
/*
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
*/
     }
}


void setsurfselect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   surfselformpop();
}


void setsurfexpl(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   surfexplodepop();
}


void surfcolorsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   surfclrflag = arg; 
   
   XmToggleButtonGadgetSetState(smatsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(snfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ssfldsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sflagsbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(sgrpsbut,FALSE,FALSE);

   if (arg == 0)
      XmToggleButtonGadgetSetState(smatsbut,TRUE,FALSE);

   if (arg == 1)
     {
      XmToggleButtonGadgetSetState(snfldsbut,TRUE,FALSE);
      if (datalimpopflag) fieldlimpop(currsnfld);
     }


   if (arg == 2)
     {
      XmToggleButtonGadgetSetState(ssfldsbut,TRUE,FALSE);
      if (datalimpopflag) sfieldlimpop(currssfld);
     }

   if (arg == 3)
      XmToggleButtonGadgetSetState(sflagsbut,TRUE,FALSE);

   if (arg == 4)
      XmToggleButtonGadgetSetState(sgrpsbut,TRUE,FALSE);
}


void snfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currsnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDSNFIELD, ifld);
}


void ssfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currssfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   sfieldformpop(SFLDSSFIELD, ifld);
}


void sflagformpop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(surfflagform);
}


void sfclose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfflagform);
}


void sselect_new_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string;

   arg = (long int)client_data;
   currsflag = arg;
   string = XmStringCreate(flagname_surf[arg],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sflaglab,XmNlabelString,string,NULL);
   XmStringFree(string);

   XtUnmanageChild(surfflagform);
}


void popbldsvect(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   svectbldformpop();
}


void makesurfform(parent)
 Widget parent;
{
  Widget surfclose, surfapply, surfsel, surfexpl, sfldtxt,
         snfldnewfld, sflagnewflag, sflagbox, sflagclose,
         ssfldnewfld, sflagbuts[MAXFLAGS], svectbar, svectmenu,
         bldsvect, svectbut;

  int i;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Surfaces",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfform = XmCreateFormDialog(parent,"Surfform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   surfclose = XtVaCreateManagedWidget("Surfclose",
               xmPushButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(surfclose,XmNactivateCallback,sclose,NULL);

   string = XmStringCreate(" Apply ",XmSTRING_DEFAULT_CHARSET);
   surfapply = XtVaCreateManagedWidget("Surfapply",
               xmPushButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, surfclose,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(surfapply,XmNactivateCallback,surf_apply,
                    NULL);
 
   string = XmStringCreate("Faces",
            XmSTRING_DEFAULT_CHARSET);
   drawsurf = XtVaCreateManagedWidget("Drawsurf",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, surfapply,
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
      XtAddCallback(drawsurf,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 0);
 
   string = XmStringCreate("Contours",
            XmSTRING_DEFAULT_CHARSET);
   drawscont = XtVaCreateManagedWidget("Drawscont",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawsurf,
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
      XtAddCallback(drawscont,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 1);
 
   string = XmStringCreate("Shaded",
            XmSTRING_DEFAULT_CHARSET);
   drawsshade = XtVaCreateManagedWidget("Drawsshade",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawscont,
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
      XtAddCallback(drawsshade,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 4);
 
   string = XmStringCreate("Refine",
            XmSTRING_DEFAULT_CHARSET);
   drawsfrefine = XtVaCreateManagedWidget("Drawsfrefine",
                 xmToggleButtonGadgetClass,
                 surfform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, drawsshade,
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
      XtAddCallback(drawsfrefine,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 5);
  
   string = XmStringCreate("Edges",
            XmSTRING_DEFAULT_CHARSET);
   drawsedge = XtVaCreateManagedWidget("Drawsedge",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, drawsfrefine,
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
      XtAddCallback(drawsedge,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 2);
  
   string = XmStringCreate("Surf Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawsnums = XtVaCreateManagedWidget("Drawsnums",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawsedge,
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
      XtAddCallback(drawsnums,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 3);
  
   string = XmStringCreate("Node Numbers",
            XmSTRING_DEFAULT_CHARSET);
   drawsnnums = XtVaCreateManagedWidget("Drawsnnums",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, drawsnums,
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
      XtAddCallback(drawsnnums,XmNvalueChangedCallback,
                    sdrawflags,(caddr_t) 7);
 
   /*  Create color select buttons.  */
 
   surfclrflag = 0; 
   string = XmStringCreate("Color By:",XmSTRING_DEFAULT_CHARSET);
   sfldtxt = XtVaCreateManagedWidget("Sfldtxt",
             xmLabelWidgetClass,
             surfform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, drawsnnums,
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
   smatsbut = XtVaCreateManagedWidget("Smatsbut",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sfldtxt,
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
      XtAddCallback(smatsbut,XmNvalueChangedCallback,
                    surfcolorsel,(caddr_t) 0);
  
   string = XmStringCreate("Node Field:",
            XmSTRING_DEFAULT_CHARSET);
   snfldsbut = XtVaCreateManagedWidget("Snfldsbut",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, smatsbut,
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
      XtAddCallback(snfldsbut,XmNvalueChangedCallback,
                    surfcolorsel,(caddr_t) 1);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   snfldlab = XtVaCreateManagedWidget("Snfldlab",
              xmLabelWidgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, snfldsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   snfldnewfld = XtVaCreateManagedWidget("Snfldnewfld",
                 xmPushButtonGadgetClass,
                 surfform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, snfldlab,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 40,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(snfldnewfld,XmNactivateCallback,snfldfld,
                    NULL);
      currsnfld = 0;
      if (numvars < 1)
        {
         XtSetSensitive(snfldsbut,FALSE);
         XtSetSensitive(snfldlab,FALSE);
         XtSetSensitive(snfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Surf Field:",
            XmSTRING_DEFAULT_CHARSET);
   ssfldsbut = XtVaCreateManagedWidget("Ssfldsbut",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, snfldnewfld,
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
      XtAddCallback(ssfldsbut,XmNvalueChangedCallback,
                    surfcolorsel,(caddr_t) 2);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   ssfldlab = XtVaCreateManagedWidget("Ssfldlab",
              xmLabelWidgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, ssfldsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   ssfldnewfld = XtVaCreateManagedWidget("Ssfldnewfld",
                 xmPushButtonGadgetClass,
                 surfform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ssfldlab,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNleftOffset, 40,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ssfldnewfld,XmNactivateCallback,ssfldfld,
                    NULL);
      currssfld = 0;
      if (snumvars < 1)
        {
         XtSetSensitive(ssfldsbut,FALSE);
         XtSetSensitive(ssfldlab,FALSE);
         XtSetSensitive(ssfldnewfld,FALSE);
        }
  
   string = XmStringCreate("Flag:",
            XmSTRING_DEFAULT_CHARSET);
   sflagsbut = XtVaCreateManagedWidget("Sflagsbut",
               xmToggleButtonGadgetClass,
               surfform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ssfldnewfld,
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
      XtAddCallback(sflagsbut,XmNvalueChangedCallback,
                    surfcolorsel,(caddr_t) 3);
 
   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   sflaglab = XtVaCreateManagedWidget("Sflaglab",
              xmLabelWidgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sflagsbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("New Flag",XmSTRING_DEFAULT_CHARSET);
   sflagnewflag = XtVaCreateManagedWidget("Sflagnewflag",
                  xmPushButtonWidgetClass,
                  surfform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sflaglab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNleftOffset, 40,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sflagnewflag,XmNactivateCallback,sflagformpop,
                    NULL);
      currsflag = 0;
      if (numflags_surf < 1)
        {
         XtSetSensitive(sflagsbut,FALSE);
         XtSetSensitive(sflaglab,FALSE);
        }
      if (numflags_surf < 2) XtSetSensitive(sflagnewflag,FALSE);
  
   string = XmStringCreate("Groups",
            XmSTRING_DEFAULT_CHARSET);
   sgrpsbut = XtVaCreateManagedWidget("Sgrpsbut",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sflagnewflag,
              XmNleftAttachment, XmATTACH_FORM,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNset, False,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(sgrpsbut,XmNvalueChangedCallback,
                    surfcolorsel,(caddr_t) 4);
      if (nsurfacegrps < 1) XtSetSensitive(sgrpsbut,FALSE);
  
   string = XmStringLtoRCreate("Show surfaces with\n0 material no.",
            XmSTRING_DEFAULT_CHARSET);
   s0matbut = XtVaCreateManagedWidget("S0matbut",
              xmToggleButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sgrpsbut,
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
      XtAddCallback(s0matbut,XmNvalueChangedCallback,
                    s0matset,0);
  
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, s0matbut);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 20);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   svectbar = XmCreateMenuBar(surfform,"Svectbar",wargs,i);
   svectmenu = XmCreatePulldownMenu(svectbar,"Svectmenu",NULL,0);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   drawsvects = XtVaCreateManagedWidget("Drawsvects",
                xmToggleButtonGadgetClass,
                svectmenu,
                XmNlabelString, string,
                XmNset, FALSE,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(drawsvects,XmNvalueChangedCallback,
                    svectdraw,NULL);

   string = XmStringCreate("Build Vector",XmSTRING_DEFAULT_CHARSET);
   bldsvect = XtVaCreateManagedWidget("Bldsvect",
              xmPushButtonGadgetClass,
              svectmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(bldsvect,XmNactivateCallback,popbldsvect,NULL);

   string = XmStringCreate("Vectors",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNlabelString,string);
   i++; XtSetArg(wargs[i], XmNsubMenuId,svectmenu);
   i++;
   svectbut = XmCreateCascadeButton(svectbar,"Svectbut",
                                    wargs,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(svectbut);
      if (snumvars < 2) XtSetSensitive(svectbut,FALSE);

   XtManageChild(svectbar);
 
   string = XmStringCreate("Select",XmSTRING_DEFAULT_CHARSET);
   surfsel = XtVaCreateManagedWidget("Surfsel",
             xmPushButtonWidgetClass,
             surfform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, svectbar,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(surfsel,XmNactivateCallback,setsurfselect,NULL);
  
   string = XmStringCreate("Explode",XmSTRING_DEFAULT_CHARSET);
   surfexpl = XtVaCreateManagedWidget("Surfexpl",
              xmPushButtonGadgetClass,
              surfform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, surfsel,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(surfexpl,XmNactivateCallback,setsurfexpl,NULL);

   /*  Make flag selection form, if needed.  */
   if (numflags_surf > 1)
     {
      string = XmStringCreate("Select Flag",XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      surfflagform = XmCreateFormDialog(sflagnewflag,"Surfflagform",
                                        wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      sflagclose = XtVaCreateManagedWidget("Sflagclose",
                   xmPushButtonGadgetClass,
                   surfflagform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(sflagclose,XmNactivateCallback,sfclose,NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, sflagclose);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      sflagbox = XmCreateRadioBox(surfflagform,"Sflagbox",wargs,i);
      XtManageChild(sflagbox);

      for (i = 0; i < numflags_surf; i++)
        {
         string = XmStringCreate(flagname_surf[i],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Sflagbuts%d",i);
         sflagbuts[i] = XtVaCreateManagedWidget(str,
                        xmToggleButtonGadgetClass,
                        sflagbox,
                        XmNlabelString, string,
                        XmNspacing, 1,
                        XmNselectColor, yellowpix,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        NULL);
            XmStringFree(string);
            XtAddCallback(sflagbuts[i],XmNvalueChangedCallback,
                          sselect_new_flag,(caddr_t) i);

         if (i == 0) XtVaSetValues(sflagbuts[0],XmNset,TRUE,NULL);
        }   
     }

   makesurfselform(surfsel);
   makesurfexpform(surfsel);
   if (snumvars > 1) makesvectbldform(surfsel);
/*
   makesurfselform(surfform);
   makesurfexpform(surfform);
   if (snumvars > 1) makesvectbldform(surfform);
*/
}


void surfformgetnfield(int i)
{
  int j;
  XmString string;

   currsnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(snfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(snfldsbut);
   if (j && datalimpopflag) fieldlimpop(currsnfld);
}


void setsurfnfldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(fieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(snfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(snfldsbut);
   if (j && datalimpopflag) fieldlimpop(currsnfld);
}


void surfformgetsfield(int i)
{
  int j;
  XmString string;

   currssfld = i;

   string = XmStringCreate(sfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ssfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(ssfldsbut);
   if (j && datalimpopflag) sfieldlimpop(currssfld);
}


void setsurfsfldlab(int ifld)
{
  int j;
  XmString string;

   string = XmStringCreate(sfieldname[ifld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(ssfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   j = XmToggleButtonGadgetGetState(ssfldsbut);
   if (j && datalimpopflag) sfieldlimpop(currssfld);
}


void setsurfgrpsensitivity()
{
   XtSetSensitive(sgrpsbut,TRUE);
}


void fillsurfgrpcolr()
{
  int i, j, k, ii, jj, kk, id;

   if (surfgrpcolr == NULL)
      surfgrpcolr = (short *)malloc(nsurf*sizeof(short));

   /*  Loop through surfaces, and get a surface id.  */
   for (ii = 0; ii < nsurf; ii++)
     {
      surfgrpcolr[ii] = 0;
      id = ii + 1;
      if (surfidflag) id = surfids[ii];

      for (j = 0; j < nsurfacegrps; j++)
        {
         kk = j + 1;
         if (kk > MAXMATS)
           {
            kk = kk % MAXMATS;
            if (kk == 0) kk = MAXMATS;
           }
         k = numsurfacegrp[j];
         for (i = 0; i < k; i++)
           {
            jj = surfacegrps[j][i];
            if (jj == id)  surfgrpcolr[ii] = kk;
           }
        }
     }
}
