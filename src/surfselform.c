
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
//#include <malloc.h>
#include <math.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

#define NSELON 7
static Widget surfmatselform, surfsfldselform, surfnfldselform, 
              surfsphselform, surfnumselform, sselonbut[NSELON],
              sselonbut2[NSELON], surfboxselform, surfgrpselform,
              surfprbselform, surfpolyselform, smatselprobe, smatremprobe;
static Widget smatsellist[MAXFLAGS+1], smatandbut[MAXFLAGS+1],
              smatorbut[MAXFLAGS+1], sselsfldlab, sselsminlab, sselsmaxlab,
              sselsmintxt, sselsmaxtxt, sselerror, sselnfldlab, 
              sselnminlab, sselnmaxlab, sselnmintxt, sselnmaxtxt,
              sselnfldall, sselnfldany, sselsphxtxt,
              sselsphytxt, sselsphztxt, sselsphrtxt,
              sselnumtxt[MAXNUMSEL], sselsphin, sselsphout,
              sselboxx1txt, sselboxy1txt, sselboxz1txt, sselboxin,
              sselboxx2txt, sselboxy2txt, sselboxz2txt, sselboxout,
              sselboxx1sl, sselboxx2sl, sselboxy1sl, sselboxy2sl,
              sselboxz1sl, sselboxz2sl, surfgrplist, sselgrpin, sselgrpout,
              ssprbmousebut, sselprbin, sselprbout, ssprbpolybut,
              sspolymousebut, sspolyclosebut, sspolyacceptbut; 
static int fastboxcount = 0, *polysurfnos, npolysurfnosalloc = 0, npolysurfs;
static short surfselprb_in = 1;


void surfselformpop()
{
   XmToggleButtonSetState(sselonbut[0],surfselmat_on,FALSE);
   XmToggleButtonSetState(sselonbut[1],surfselsfld_on,FALSE);
   XmToggleButtonSetState(sselonbut[2],surfselnfld_on,FALSE);
   XmToggleButtonSetState(sselonbut[3],surfselsph_on,FALSE);
   XmToggleButtonSetState(sselonbut[4],surfselnum_on,FALSE);
   XmToggleButtonSetState(sselonbut[5],surfselbox_on,FALSE);
   XmToggleButtonSetState(sselonbut[6],surfselsurfgrp_on,FALSE);

   MyManageChild(surfselform);
}


void surfmatselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(sselonbut2[0],surfselmat_on,FALSE);

   for (i = 0; i <= numflags_surf; i++)
     {
      if (i == 0) k = maxsurfmat;
      else k = flagtypes_surf[i-1];
      XmListDeselectAllItems(smatsellist[i]);
      for (j = 0; j < k; j++)
        {
         if (surfselflag[i][j] == 1)
            XmListSelectPos(smatsellist[i],j+1,FALSE);
        }
     }

   if (numflags_surf > 0)
     {
      for (i = 0; i < numflags_surf; i++)
        {
         if (surfandorflag[i] == 0)
           {
            XmToggleButtonGadgetSetState(smatandbut[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(smatorbut[i],FALSE,FALSE);
           }
         if (surfandorflag[i] == 1)
           {
            XmToggleButtonGadgetSetState(smatorbut[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(smatandbut[i],FALSE,FALSE);
           }
        }
     }

   /*  Calculate material pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx;
   fy = wy + 50;
   XtVaSetValues(surfmatselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(surfmatselform);
}


void surfsfldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (snumvars > 0)
     {
      XmToggleButtonSetState(sselonbut2[1],surfselsfld_on,FALSE);

      i = surfselfield;

      string = XmStringCreate(sfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselsfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",sfieldmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselsminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",sfieldmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselsmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (sfldselmin == 0. && sfldselmax == 0.)
        {
         sfldselmin = sfieldmin[i];
         sfldselmax = sfieldmax[i];
        }
      sprintf(str,"%.11g",sfldselmin);
      XmTextFieldSetString(sselsmintxt,str);
      sprintf(str,"%.11g",sfldselmax);
      XmTextFieldSetString(sselsmaxtxt,str);

      /*  Calculate surface field pop-up postition from widgets.  */
      XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
      fx = wx + 20;
      fy = wy + 50;
      XtVaSetValues(surfsfldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(surfsfldselform);
     }
}


void surfnfldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (numvars > 0)
     {
      XmToggleButtonSetState(sselonbut2[2],surfselnfld_on,FALSE);

      i = surfselnfield;

      if (surfselnfldopt)
        {
         XmToggleButtonGadgetSetState(sselnfldany,FALSE,FALSE);         
         XmToggleButtonGadgetSetState(sselnfldall,TRUE,FALSE);         
        }
      else
        {
         XmToggleButtonGadgetSetState(sselnfldany,TRUE,FALSE);         
         XmToggleButtonGadgetSetState(sselnfldall,FALSE,FALSE);         
        }

      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselnfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",fieldmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselnminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",fieldmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(sselnmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (snfldselmin == 0. && snfldselmax == 0.)
        {
         snfldselmin = fieldmin[i];
         snfldselmax = fieldmax[i];
        }
      sprintf(str,"%.11g",snfldselmin);
      XmTextFieldSetString(sselnmintxt,str);
      sprintf(str,"%.11g",snfldselmax);
      XmTextFieldSetString(sselnmaxtxt,str);

      if (surfselnfldopt)
        {
         XmToggleButtonGadgetSetState(sselnfldany,FALSE,FALSE);
         XmToggleButtonGadgetSetState(sselnfldall,TRUE,FALSE);
        }
      else
        {
         XmToggleButtonGadgetSetState(sselnfldany,TRUE,FALSE);
         XmToggleButtonGadgetSetState(sselnfldall,FALSE,FALSE);
        }

      /*  Calculate node field pop-up postition from widgets.  */
      XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
      fx = wx + 40;
      fy = wy + 50;
      XtVaSetValues(surfnfldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(surfnfldselform);
     }
}


void surfsphselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(sselonbut2[3],surfselsph_on,FALSE);

   sprintf(str,"%.11g",surfselsph_x);
   XmTextFieldSetString(sselsphxtxt,str);

   sprintf(str,"%.11g",surfselsph_y);
   XmTextFieldSetString(sselsphytxt,str);

   sprintf(str,"%.11g",surfselsph_z);
   XmTextFieldSetString(sselsphztxt,str);

   sprintf(str,"%.11g",surfselsph_r);
   XmTextFieldSetString(sselsphrtxt,str);

   if (surfselsph_in)
     {
      XmToggleButtonGadgetSetState(sselsphin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(sselsphout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(sselsphin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(sselsphout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(surfsphselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(surfsphselform);
}


void surfnumselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[30];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(sselonbut2[4],surfselnum_on,FALSE);

   /*  Fill text lines with surface number select data.  */
   if (nsurfselnum > 0)
     {
      for (i = 0; i < nsurfselnum; i++)
        {
         if (surfselnum_f[i] == surfselnum_l[i])
            sprintf(str,"%ld",surfselnum_f[i]);
         else if (surfselnum_s[i] == 1)
            sprintf(str,"%ld:%ld",surfselnum_f[i],surfselnum_l[i]);
         else
            sprintf(str,"%ld:%ld:%ld",surfselnum_f[i],surfselnum_l[i],
                                   surfselnum_s[i]);
         XmTextFieldSetString(sselnumtxt[i],str);
        }
      for (i = nsurfselnum; i < MAXNUMSEL; i++)
         XmTextFieldSetString(sselnumtxt[i],"");
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx + 80;
   fy = wy + 50;
   XtVaSetValues(surfnumselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(surfnumselform);
}


void surfboxselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;
  void setsurfselboxwidgets();

   XmToggleButtonSetState(sselonbut2[5],surfselbox_on,FALSE);

   setsurfselboxwidgets();

   if (surfselbox_in)
     {
      XmToggleButtonGadgetSetState(sselboxin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(sselboxout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(sselboxin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(sselboxout,TRUE,FALSE); 
     }

   /*  Calculate box pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(surfboxselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(surfboxselform);
}


void surfgrpselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;
  Position  wx, wy, fx, fy, p0 = 0;

   if (surfselgrp_in)
     {
      XmToggleButtonGadgetSetState(sselgrpin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(sselgrpout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(sselgrpin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(sselgrpout,TRUE,FALSE); 
     }

   XmToggleButtonSetState(sselonbut2[6],surfselsurfgrp_on,FALSE);
   XmListDeselectAllItems(surfgrplist);

   for (j = 0; j < nsurfgrpselected; j++)
     {
      i = surfgrpselected[j];
      XmListSelectPos(surfgrplist,i,FALSE);
     }

   /*  Calculate surface group pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx + 30;
   fy = wy + 60;
   XtVaSetValues(surfgrpselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(surfgrpselform);
}


void surfprbselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;
  int i;

   /*  Calculate node group pop-up postition from widgets.  */
   XtTranslateCoords(sselerror, p0,p0, &wx,&wy); 
   fx = wx + 30;
   fy = wy + 60;
   XtVaSetValues(surfprbselform, XmNx,fx, XmNy,fy, NULL);

   if (surfselprobe_on == 0)
     {
      for (i = 0; i < nsurf; i++) surfselectflagp[i] = 0;
     }
   surfselprobe_on = 1;
   surfflag = 1;
   surfedgesflag = 1;

   XmToggleButtonGadgetSetState(ssprbmousebut,surfselprobopt,FALSE);         
   XmToggleButtonGadgetSetState(ssprbpolybut,FALSE,FALSE);         
             
   if (surfselprb_in)
     {
      XmToggleButtonGadgetSetState(sselprbin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(sselprbout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(sselprbin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(sselprbout,TRUE,FALSE); 
     }

   MyManageChild(surfprbselform);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sselmatclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfmatselprobeflag = 0;
   XmToggleButtonSetState(smatselprobe,FALSE,FALSE);
   surfmatremprobeflag = 0;
   XmToggleButtonSetState(smatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   XtUnmanageChild(surfmatselform);
}


void sselsfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(sfieldform);
   XtUnmanageChild(surfsfldselform);
}


void sselnfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(surfnfldselform);
}


void sselsphclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfsphselform);
}


void sselnumclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfnumselform);
}


void sselboxclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselbox_draw = 0;
   expose_redraw();
   XtUnmanageChild(surfboxselform);
}


void surfgrpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfgrpselform);
}


void sselprbclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfprbselform);
}


void sselclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(surfmatselform);
   XUndefineCursor(dpy,windowid);
   surfmatselprobeflag = 0;
   XmToggleButtonSetState(smatselprobe,FALSE,FALSE);
   surfmatremprobeflag = 0;
   XmToggleButtonSetState(smatremprobe,FALSE,FALSE);
   if (snumvars > 0)
      sselsfldclose(NULL,NULL,NULL);
   if (numvars > 0)
      sselnfldclose(NULL,NULL,NULL);
   XtUnmanageChild(surfsphselform);
   XtUnmanageChild(surfnumselform);
   sselboxclose(NULL,NULL,NULL);
   if (nsurfacegrps > 0 || physedflag)
      XtUnmanageChild(surfgrpselform);
   XtUnmanageChild(surfprbselform);

   XtUnmanageChild(surfselform);
}


void sselnfldsetopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselnfldopt = (short)XmToggleButtonGadgetGetState(sselnfldall);
}


void sselsphsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselsph_in = (short)XmToggleButtonGadgetGetState(sselsphin);
}


void sselboxsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselbox_in = (short)XmToggleButtonGadgetGetState(sselboxin);
}


void sselgrpsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselgrp_in = (short)XmToggleButtonGadgetGetState(sselgrpin);
}


void sselsetonbut(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      surfselmat_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[0],surfselmat_on,FALSE);
      XmToggleButtonSetState(sselonbut2[0],surfselmat_on,FALSE);
     }
   if (arg == 1)
     {
      surfselsfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[1],surfselsfld_on,FALSE);
      XmToggleButtonSetState(sselonbut2[1],surfselsfld_on,FALSE);
     }
   if (arg == 2)
     {
      surfselnfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[2],surfselnfld_on,FALSE);
      XmToggleButtonSetState(sselonbut2[2],surfselnfld_on,FALSE);
     }
   if (arg == 3)
     {
      surfselsph_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[3],surfselsph_on,FALSE);
      XmToggleButtonSetState(sselonbut2[3],surfselsph_on,FALSE);
     }
   if (arg == 4)
     {
      surfselnum_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[4],surfselnum_on,FALSE);
      XmToggleButtonSetState(sselonbut2[4],surfselnum_on,FALSE);
     }
   if (arg == 5)
     {
      surfselbox_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[5],surfselbox_on,FALSE);
      XmToggleButtonSetState(sselonbut2[5],surfselbox_on,FALSE);
     }
   if (arg == 6)
     {
      surfselsurfgrp_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(sselonbut[6],surfselsurfgrp_on,FALSE);
      XmToggleButtonSetState(sselonbut2[6],surfselsurfgrp_on,FALSE);
     }
}


void sselapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k, ierr, tmpstrlen, *grpselected, *smatselected, nlist;
  unsigned long num;
  char *tmpstr;
  float x1,y1,z1, x2,y2,z2;
  static char *token;
  static char *str1 = 
           "Error - surf range minimum must be less than maximum.";
  static char *str2 = 
           "Error - node range minimum must be greater than minimum.";
  static char *str3 = 
           "Error - search sphere radius must be greater than 0.";
  static char str4[55]; 
  XmString string;
  char *chk_num = "0123456789: ", *chk_blank = " ";
  void setsurfsel();

   /*  Turn off surface material select and remove probe buttons.  */
   surfmatselprobeflag = 0;
   XmToggleButtonSetState(smatselprobe,FALSE,FALSE);
   surfmatremprobeflag = 0;
   XmToggleButtonSetState(smatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   /*  Get and set the surface material and surface flags selection.  */
   for (i = 0; i <= numflags_surf; i++)
     {
      j = XmListGetSelectedPos(smatsellist[i],&smatselected,&nlist);
      if (!j) nlist = 0;
      for (j = 0; j < MAXMATS; j++) surfselflag[i][j] = 0;
      for (j = 0; j < nlist; j++)
        { 
         k = smatselected[j] - 1;
         surfselflag[i][k] = 1;
        }
      FREE(smatselected);
     }

   /*  Read and check the surface range values.  */
   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   surfselbox_draw = 0;
   ierr = 0;
   if (snumvars > 0 && surfselsfld_on)
     {
      tmpstr = XmTextFieldGetString(sselsmintxt);
      sfldselmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselsmaxtxt);
      sfldselmax = atof(tmpstr);
      XtFree(tmpstr);

      if (sfldselmin > sfldselmax)
        {
         ierr = 1;
         string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(sselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check the node range values.  */
   if (numvars > 0 && surfselnfld_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(sselnmintxt);
      snfldselmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselnmaxtxt);
      snfldselmax = atof(tmpstr);
      XtFree(tmpstr);

      if (snfldselmin > snfldselmax)
        {
         ierr = 1;
         string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(sselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check the search sphere values.  */
   if (surfselsph_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(sselsphxtxt);
      surfselsph_x = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselsphytxt);
      surfselsph_y = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselsphztxt);
      surfselsph_z = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselsphrtxt);
      surfselsph_r = atof(tmpstr);
      XtFree(tmpstr);

      if (surfselsph_r <= 0)
        {
         ierr = 1;
         string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(sselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check surf number values.  */
   if (surfselnum_on && ierr == 0)
     {
      nsurfselnum = 0;
      i = 0;

      while(ierr == 0 && i < MAXNUMSEL)
        {
         tmpstr = XmTextFieldGetString(sselnumtxt[i]);
         tmpstrlen = strlen(tmpstr);
         if (tmpstrlen > 0)
           {

            /*  Check for invalid characters.  */
            if (strspn(tmpstr,chk_num) != tmpstrlen)
              {
               ierr = 1;
               sprintf(str4,"Error - invalid character in line %d",i+1);
               string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
               XtVaSetValues(sselerror,XmNlabelString,string,NULL);
               XmStringFree(string);
              }

            /*  If not all blanks, extract data from the string.  */
            if (ierr == 0 && strspn(tmpstr,chk_blank) != tmpstrlen)
              {

               /*  Get the first number from the set.  */
               token = strtok(tmpstr,":");
               num = strtoul(token,NULL,10);
               if (num > nsurf && !surfidflag) num = nsurf;
               surfselnum_f[nsurfselnum] = num;

               /*  Get the last number from the set.  */
               surfselnum_l[nsurfselnum] = num;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  if (num > nsurf && !surfidflag) num = nsurf;

                  /*  If first greater than last, switch.  */
                  if (num >= surfselnum_f[i])
                     surfselnum_l[nsurfselnum] = num;
                  else
                    {
                     surfselnum_l[i] = surfselnum_f[i];
                     surfselnum_f[i] = num;
                    }
                 }

               /*  Get the stride from the set.  */
               surfselnum_s[nsurfselnum] = 1;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  surfselnum_s[nsurfselnum] = num;
                 }

               nsurfselnum++;
              }
           }
         XtFree(tmpstr);
         i++;
        }

     }

   /*  Read and set the search box values.  */
   if (surfselbox_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(sselboxx1txt);
      x1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselboxy1txt);
      y1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselboxz1txt);
      z1 = atof(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(sselboxx2txt);
      x2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselboxy2txt);
      y2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(sselboxz2txt);
      z2 = atof(tmpstr);
      XtFree(tmpstr);

      surfselbox_xmin = (x1 < x2) ? x1 : x2;
      surfselbox_ymin = (y1 < y2) ? y1 : y2;
      surfselbox_zmin = (z1 < z2) ? z1 : z2;
      surfselbox_xmax = (x1 > x2) ? x1 : x2;
      surfselbox_ymax = (y1 > y2) ? y1 : y2;
      surfselbox_zmax = (z1 > z2) ? z1 : z2;
     }

   /*  Read and set surface group selections.  */
   nsurfgrpselected = 0;
   if (surfselsurfgrp_on && ierr == 0)
     {
      j = XmListGetSelectedPos(surfgrplist,&grpselected,&nsurfgrpselected);
      if (!j) nsurfgrpselected = 0;
      for (i = 0; i < MAXGROUPS; i++) surfgrpselected[i] = 0;
      for (i = 0; i < nsurfgrpselected; i++) 
         surfgrpselected[i] = grpselected[i];
      FREE(grpselected);
     }

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      setsurfsel();
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void ssset_on(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;

   i = (long int)client_data;
   if (i == 0) k = maxsurfmat;
   else k = flagtypes_surf[i-1];

   XmListDeselectAllItems(smatsellist[i]);

   for (j = 0; j < k; j++)
     {
      XmListSelectPos(smatsellist[i],j+1,FALSE);
     }
}


void ssset_off(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;

   XmListDeselectAllItems(smatsellist[i]);
}


void select_surf_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, arg;

   arg = (long int)client_data;
   i = arg / MAXMATS;
   j = arg - (i*MAXMATS);
   surfselflag[i][j] = 
      (short)XmToggleButtonGetState(w);
}


void sselect_and_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) surfandorflag[i] = 1;
   if (j == 1) surfandorflag[i] = 0;
}


void sselect_or_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) surfandorflag[i] = 0;
   if (j == 1) surfandorflag[i] = 1;
}


void sselsfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = surfselfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(sfieldform, XmNx,fx, XmNy,fy, NULL);
 
   sfieldformpop(SFLDSURFSEL, ifld);
}


void sselsfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = surfselfield;

   sfldselmin = sfieldmin[i];
   sfldselmax = sfieldmax[i];

   sprintf(str,"%.11g",sfieldmin[i]);
   XmTextFieldSetString(sselsmintxt,str);
   sprintf(str,"%.11g",sfieldmax[i]);
   XmTextFieldSetString(sselsmaxtxt,str);
}


void sselnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = surfselnfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDSURFSEL, ifld);
}


void sselnfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = surfselnfield;

   snfldselmin = fieldmin[i];
   snfldselmax = fieldmax[i];

   sprintf(str,"%.11g",fieldmin[i]);
   XmTextFieldSetString(sselnmintxt,str);
   sprintf(str,"%.11g",fieldmax[i]);
   XmTextFieldSetString(sselnmaxtxt,str);
}


void ssboxsubsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];

   surfselbox_draw = 1;
   arg = (long int)client_data;

   XmScaleGetValue(w,&ival);
   val = (float) ival / 1000.;
 
   if (arg == 0 || arg == 6)
     {
      surfselbox_xmin = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",surfselbox_xmin);
      XtVaSetValues(sselboxx1txt,XmNvalue,str,NULL);
      if (surfselbox_xmin > surfselbox_xmax)
        {
         XmScaleSetValue(sselboxx2sl,ival);
         XtVaSetValues(sselboxx2txt,XmNvalue,str,NULL);
         surfselbox_xmax = surfselbox_xmin;
        }
     }
   if (arg == 1 || arg == 7)
     {
      surfselbox_xmax = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",surfselbox_xmax);
      XtVaSetValues(sselboxx2txt,XmNvalue,str,NULL);
      if (surfselbox_xmin > surfselbox_xmax)
        {
         XmScaleSetValue(sselboxx1sl,ival);
         XtVaSetValues(sselboxx1txt,XmNvalue,str,NULL);
         surfselbox_xmin = surfselbox_xmax;
        }
     }
   if (arg == 2 || arg == 8)
     {
      surfselbox_ymin = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",surfselbox_ymin);
      XtVaSetValues(sselboxy1txt,XmNvalue,str,NULL);
      if (surfselbox_ymin > surfselbox_ymax)
        {
         XmScaleSetValue(sselboxy2sl,ival);
         XtVaSetValues(sselboxy2txt,XmNvalue,str,NULL);
         surfselbox_ymax = surfselbox_ymin;
        }
     }
   if (arg == 3 || arg == 9)
     {
      surfselbox_ymax = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",surfselbox_ymax);
      XtVaSetValues(sselboxy2txt,XmNvalue,str,NULL);
      if (surfselbox_ymin > surfselbox_ymax)
        {
         XmScaleSetValue(sselboxy1sl,ival);
         XtVaSetValues(sselboxy1txt,XmNvalue,str,NULL);
         surfselbox_ymin = surfselbox_ymax;
        }
     }
   if (arg == 4 || arg == 10)
     {
      surfselbox_zmin = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",surfselbox_zmin);
      XtVaSetValues(sselboxz1txt,XmNvalue,str,NULL);
      if (surfselbox_zmin > surfselbox_zmax)
        {
         XmScaleSetValue(sselboxz2sl,ival);
         XtVaSetValues(sselboxz2txt,XmNvalue,str,NULL);
         surfselbox_zmax = surfselbox_zmin;
        }
     }
   if (arg == 5 || arg == 11)
     {
      surfselbox_zmax = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",surfselbox_zmax);
      XtVaSetValues(sselboxz2txt,XmNvalue,str,NULL);
      if (surfselbox_zmin > surfselbox_zmax)
        {
         XmScaleSetValue(sselboxz1sl,ival);
         XtVaSetValues(sselboxz1txt,XmNvalue,str,NULL);
         surfselbox_zmin = surfselbox_zmax;
        }
     }

   if (arg < 6)
     {
      fastdraw = 0;
      drawfastbox(fastboxcount);
      fastboxcount = 0;
     }
   else
     {
      fastdraw = 1;
      drawfastbox(fastboxcount);
      fastboxcount++;
     }
}


void sselboxtxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr, str[20];
  float val;
  void setsurfselboxwidgets();

   surfselbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0 || arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      if (arg == 0)
        {
         surfselbox_xmin = val;
         if (surfselbox_xmin > surfselbox_xmax)
            surfselbox_xmax = surfselbox_xmin;
        }
      else
        {
         surfselbox_xmax = val;
         if (surfselbox_xmin > surfselbox_xmax)
            surfselbox_xmin = surfselbox_xmax;
        }
     }
 
   if (arg == 2 || arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      if (arg == 2)
        {
         surfselbox_ymin = val;
         if (surfselbox_ymin > surfselbox_ymax)
            surfselbox_ymax = surfselbox_ymin;
        }
      else
        {
         surfselbox_ymax = val;
         if (surfselbox_ymin > surfselbox_ymax)
            surfselbox_ymin = surfselbox_ymax;
        }
     }
 
   if (arg == 4 || arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      if (arg == 4)
        {
         surfselbox_zmin = val;
         if (surfselbox_zmin > surfselbox_zmax)
            surfselbox_zmax = surfselbox_zmin;
        }
      else
        {
         surfselbox_zmax = val;
         if (surfselbox_zmin > surfselbox_zmax)
            surfselbox_zmin = surfselbox_zmax;
        }
     }

   XtFree(tmpstr);

   setsurfselboxwidgets();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void sselmouse(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   surfselprobopt = (short)XmToggleButtonGadgetGetState(ssprbmousebut);
   if (surfselprobopt)
     {
      surfselprobe_on = 1;
      surfselpolyopt = 0;
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else 
     {
      XUndefineCursor(dpy,windowid);
      XFlush(dpy);
     }
}


#define INSIDE 1
#define OUTSIDE 2
#define ON 567

int getsurfprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int j, k, jsurf, surfid, itst;
  short gosurf, *matchk;
  double xl1, yl1, zl1, xl2, yl2, zl2, xn, yn, zn;
  double aa, bb, cc, dd, x1, y1, z1, denomck;
  double denom, dist, mins;
  double ax, ay, az, bx, by, bz, s, xp, yp, zp;
  struct node xyzs[1000];
  int ptinface(double xip, double yip, double zip, int nv, struct node *xyzs,
               double A, double B, double C);

   /*  Set the points.  */
   xl1 = (double)pa;  yl1 = (double)pb;  zl1 = (double)pc;
   xl2 = (double)pd;  yl2 = (double)pe;  zl2 = (double)pf;

   /*  Set line end distance elements.  */
   ax = xl2 - xl1;
   ay = yl2 - yl1;
   az = zl2 - zl1;

   /*  Set to material color or group color.  */
   if (surffieldactive == -2) matchk = surfgrpcolr;
   else matchk = surfmats;

   /*  Loop over selected surfaces to find the shortest  */ 
   /*  distance from the surface center to the viewer.   */
   mins = MAXVAL;
   surfid = -1;
   for (jsurf = 0; jsurf < nsurf; jsurf++)
     {

      /*  Check surface material, subset and select.  */
      gosurf = 0;
      if (surfedgesflag == 1 || surfflag == 1 || surfnumflag == 1)
        {
         gosurf = (int)surfselectflag[jsurf];

         /*  Eliminate surfaces with material < 1.  */
         if (surf0matflag == 0 && matchk[jsurf] < 1) gosurf = 0;
 
         /*  See if the surface lies within the subset.  */
         if ((int)surfsubsetflag[jsurf] == 0) gosurf = 0;
        }

      /*  If active surface, check surface ave. distance to line.  */
      if (gosurf)
        {

         /*  Calculate projection point of surface on the line     */
         /*  and the distance from the surface to the projection.  */
         xn = surfs[jsurf].xavg;
         yn = surfs[jsurf].yavg;
         zn = surfs[jsurf].zavg;
         bx = xn - xl1;
         by = yn - yl1;
         bz = zn - zl1;
         s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
         xp = s*xl2 + xl1*(1.0 - s);
         yp = s*yl2 + yl1*(1.0 - s);
         zp = s*zl2 + zl1*(1.0 - s);
         dist = sqrt((xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + (zp-zn)*(zp-zn));

         /*  Check that the line is within the max surface radius.     */
         if (dist <= maxsurfradius[jsurf])
           {

            /*  Get the surface normal and calculate the        */
            /*  intersection of the probe line to the surface.  */ 
            aa = surfs[jsurf].xnorm;
            bb = surfs[jsurf].ynorm;
            cc = surfs[jsurf].znorm;
            k = surfs[jsurf].fverts[0];
            x1 = nodes[k].x;
            y1 = nodes[k].y;
            z1 = nodes[k].z;
            dd = -x1*aa - y1*bb - z1*cc;
            denom = aa*ax + bb*ay + cc*az;
            denomck = (float)(fabs((double)denom));
            s = 1.1;
            if (denomck > 1.0e-10)
               s = -(aa*xl1 + bb*yl1 + cc*zl1 + dd) / denom;
            if (fabs((double)s) < 9.0e-7) s = 0.;
            denomck = s - 1.;
            if (fabs((double)denomck) < 9.0e-7) s = 1.;
            if (s >= 0. && s <= 1.)
              {
               xn = xl1 + s * (xl2 - xl1);
               yn = yl1 + s * (yl2 - yl1);
               zn = zl1 + s * (zl2 - zl1);

               /*  Check that the intersection point lies in  */
               /*  the face, if so, save closest to viewer.   */
               for (j = 0; j < surfs[jsurf].nverts; j++)
                  {
                   k = surfs[jsurf].fverts[j];
                   xyzs[j].x = nodes[k].x;
                   xyzs[j].y = nodes[k].y;
                   xyzs[j].z = nodes[k].z;
                  }
               itst = ptinface(xn, yn, zn, surfs[jsurf].nverts,
                      xyzs, aa, bb, cc);
               if (itst != OUTSIDE)
                 {              
                  if (s < mins)
                    {
                     surfid = jsurf;
                     mins = s;
                    }
                 }
              }
           } 
        }
     }
   return surfid;
}


void doitsprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int surfid;
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Get the surface number.  */
   surfid = getsurfprobe(pa,pb,pc,pd,pe,pf);
   if (surfid >= 0)
     {
      if (surfselectflagp[surfid] == 0)
         surfselectflagp[surfid] = 1;
      else
         surfselectflagp[surfid] = 0;
     }

   XUndefineCursor(dpy,topwin);
   XFlush(dpy);

   glPopMatrix();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();

   get_1_point(NULL, (caddr_t) 3, NULL);
}


void getsurfpoly()
{
  int i, j, k, isurf, jsurf, itst, ivert;
  short gosurf, *matchk;
  double *psx, *psy, *psz, xavg, yavg, pxl, pyl, pradius, 
         xn, yn, zn, s, xp, yp, zp, ax, ay, az, bx, by, bz,
         xmin, xmax, ymin, ymax;
  double maxdist, dist, xd, yd, xl1, xl2, yl1, yl2, zl1, zl2, xl3, yl3, zl3;
  struct node xyzs[1000];
  GLint vport[4];
  GLdouble mvmatrix[16], projmatrix[16], px, py, pz, wx, wy, wz;
  int ptinface(double xip, double yip, double zip, int nv, struct node *xyzs,
               double A, double B, double C);

   /*  Calculate the polygon center and bounding box.  */
   xavg = yavg = 0;
   xmin = ymin = windowheight + 100;
   xmax = ymax = 0;
   for (i = 0; i < nsurfpolyverts; i++)
     {
      xavg += surfpolyx[i];
      yavg += surfpolyy[i];
      if (surfpolyx[i] < xmin) xmin = surfpolyx[i];
      if (surfpolyy[i] < ymin) ymin = surfpolyy[i];
      if (surfpolyx[i] > xmax) xmax = surfpolyx[i];
      if (surfpolyy[i] > ymax) ymax = surfpolyy[i];
     }
   xavg /= nsurfpolyverts;
   yavg /= nsurfpolyverts;

   /*  Get the polygon vertex farthest away from the center.  */
   ivert = -1;
   maxdist = 0;
   for (i = 0; i < nsurfpolyverts; i++)
     {
      xd = surfpolyx[i];  yd = surfpolyy[i];
      dist = (xd-xavg)*(xd-xavg) + (yd-yavg)*(yd-yavg);
      if (dist > maxdist)
        {
         maxdist = dist;
         ivert = i;
        }
     }
   pxl = surfpolyx[ivert];
   pyl = surfpolyy[ivert];

   /*  Get the data needed to project surface centers onto screen units.  */
   glPushMatrix();
   if (viewflag == 0)
      glScalef(magnify,magnify,magnify);
   if (viewflag == 1)
      glScalef(1.0,1.0,1.0);
   glTranslatef(xpan, ypan, 0.);
   glTranslatef(0., 0., -((GLfloat)span));
   glRotatef( -( (GLfloat)(zrot)), 0., 0., 1.);
   glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
   glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
   glTranslatef(xtrans, ytrans, ztrans);
   glFlush();
   glGetIntegerv(GL_VIEWPORT, vport);
   glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

   /*  Project the polygon center and the far vertex  */
   /*  to generate a search cirle for the surfaces.   */
   xn = xavg;
   yn = (vport[3] - yavg - 1);
   gluUnProject(xn,yn,(GLdouble)0.0,
                mvmatrix,projmatrix,vport,&xl1,&yl1,&zl1);
   gluUnProject(xn,yn,(GLdouble)1.0,
                mvmatrix,projmatrix,vport,&xl2,&yl2,&zl2);   
   xn = pxl;
   yn = (vport[3] - pyl - 1); 
   gluUnProject(xn,yn,(GLdouble)0.0,
                mvmatrix,projmatrix,vport,&xl3,&yl3,&zl3);
   pradius = sqrt((xl1-xl3)*(xl1-xl3) + (yl1-yl3)*(yl1-yl3) +
                  (zl1-zl3)*(zl1-zl3));

   /*  Set line end distance elements.  */
   ax = xl2 - xl1;
   ay = yl2 - yl1;
   az = zl2 - zl1;

   /*  Set to material color or group color.  */
   if (surffieldactive == -2) matchk = surfgrpcolr;
   else matchk = surfmats;

   /*  Loop over selected surfaces to find surfaces that    */
   /*  lie within the search radius of the polygon center.  */
   if (npolysurfnosalloc == 0)
     {
      npolysurfs = 0;
      npolysurfnosalloc = 1000;
      polysurfnos = (int *)malloc(npolysurfnosalloc*sizeof(int));
     }
   for (jsurf = 0; jsurf < nsurf; jsurf++)
     {

      /*  Check surface material, subset and select.  */
      gosurf = 0;
      if (surfedgesflag == 1 || surfflag == 1 || surfnumflag == 1)
        {
         gosurf = (int)surfselectflag[jsurf];

         /*  Eliminate surfaces with material < 1.  */
         if (surf0matflag == 0 && matchk[jsurf] < 1) gosurf = 0;
 
         /*  See if the surface lies within the subset.  */
         if ((int)surfsubsetflag[jsurf] == 0) gosurf = 0;
        }

      /*  If active surface, check that the surface    */
      /*  center lies within the polgon bounding box.  */
      if (gosurf)
        {

         /*  Calculate projection point of surface on the line     */
         /*  and the distance from the surface to the projection.  */
         xn = surfs[jsurf].xavg;
         yn = surfs[jsurf].yavg;
         zn = surfs[jsurf].zavg;
         bx = xn - xl1;
         by = yn - yl1;
         bz = zn - zl1;
         s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
         xp = s*xl2 + xl1*(1.0 - s);
         yp = s*yl2 + yl1*(1.0 - s);
         zp = s*zl2 + zl1*(1.0 - s);
         dist = sqrt((xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + (zp-zn)*(zp-zn));

         /*  Check that the line is within the max radius.  */
         if (dist <= pradius + maxsurfradius[jsurf])
           {

            /*  Save the polygon ids.    */
            /*  Check for reallocation.  */
            if (npolysurfs+1 > npolysurfnosalloc)
              {
               npolysurfnosalloc += 1000;
               polysurfnos = (int *)realloc(polysurfnos,
                                        npolysurfnosalloc*sizeof(double));
              }
            polysurfnos[npolysurfs] = jsurf;
            npolysurfs++;
           }
        }
     }

   /*  Convert selected surface centers to window coordinates.  */
   psx = (double *)malloc(npolysurfs*sizeof(double));
   psy = (double *)malloc(npolysurfs*sizeof(double));
   psz = (double *)malloc(npolysurfs*sizeof(double));
   for (i = 0; i < npolysurfs; i++)
     {
      jsurf = polysurfnos[i];
      px = surfs[jsurf].xavg;
      py = surfs[jsurf].yavg;
      pz = surfs[jsurf].zavg;
      gluProject(px,py,pz,mvmatrix,projmatrix,vport,&wx,&wy,&wz);
      psx[i] = wx;
      psy[i] = vport[3] - wy + 1;
      psz[i] = wz;
     }

   /*  If any surfaces lie within the polygon search radius,      */
   /*  eliminate those whose center lies behind another surface.  */
   for (i = 0; i < npolysurfs-1; i++)
     {
      isurf = polysurfnos[i];
      if (isurf == -1) continue;
      for (j = 0; j < surfs[isurf].nverts; j++)
        {
         k = surfs[isurf].fverts[j];
         px = nodes[k].x;
         py = nodes[k].y;
         pz = nodes[k].z;
         gluProject(px,py,pz,mvmatrix,projmatrix,vport,&wx,&wy,&wz);
         xyzs[j].x = wx;
         xyzs[j].y = vport[3] - wy + 1;
         xyzs[j].z = wz;
        }
      for (j = i+1; j < npolysurfs; j++)
        {
         jsurf = polysurfnos[j];
         xn = psx[j];
         yn = psy[j];
         zn = psz[j];
         itst = ptinface(xn, yn, zn, surfs[jsurf].nverts,
                         xyzs, 0., 0., 1.);
         if (itst != OUTSIDE)
           {
            if (psz[j] < psz[i]) polysurfnos[i] = -1;
            if (psz[i] < psz[j]) polysurfnos[j] = -1;
           }
        }
     }
   glPopMatrix();

   /*  Now check that the surfaces lie in the polygon bounding box.  */
   for (i = 0; i < npolysurfs; i++)
     {
      isurf = polysurfnos[i];
      if (isurf >= 0)
        {
         xn = psx[i];
         yn = psy[i];
         if (xn < xmin || xn > xmax) polysurfnos[i] = -1;
         if (yn < ymin || yn > ymax) polysurfnos[i] = -1;
        }
     }

   /*  Fill x,y,z structure for polygon.  */
   for (i = 0; i < nsurfpolyverts; i++)
     {
      xyzs[i].x = surfpolyx[i];
      xyzs[i].y = surfpolyy[i];
      xyzs[i].z = 0.;
     }

   /*  Now check that the surfaces lie in the polygon.  */
   for (i = 0; i < npolysurfs; i++)
     {
      isurf = polysurfnos[i];
      if (isurf >= 0)
        {
         xn = psx[i];
         yn = psy[i];
         itst = ptinface(xn, yn, 0., nsurfpolyverts, xyzs, 0., 0., 1.);

         /*  If outsied the polygon, eliminate.  */
         if (itst == OUTSIDE) polysurfnos[i] = -1;
        }
     }

   /*  Set surface selection info.  */
   for (i = 0; i < npolysurfs; i++)
     {
      isurf = polysurfnos[i];
      if (isurf >= 0) surfselectflagp[isurf] = 1;
     }

   FREE(psx);  FREE(psy);  FREE(psz);
}


void sselpoly(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  short polyopt;

   polyopt = (short)XmToggleButtonGadgetGetState(ssprbpolybut);
   if (polyopt)
     {
      surfselpolyopt = 0;
      surfselprobe_on = 1;
      surfselprobopt = 0;
      XmToggleButtonGadgetSetState(sspolymousebut,FALSE,TRUE);        
      XmToggleButtonGadgetSetState(sspolyclosebut,FALSE,FALSE);        
      MyManageChild(surfpolyselform);
     }

   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void sseldrawpoly(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  short arg;
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   arg = (short)XmToggleButtonGadgetGetState(sspolymousebut);
   if (arg)
     {
      XtSetSensitive(sspolyacceptbut,FALSE);
      nsurfpolyverts = 0;
      surfselpolyopt = 1;
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else 
     {
      XUndefineCursor(dpy,topwin);
      XFlush(dpy);
     }
   fastdraw = 0;
   drawit();
}


void sselclosepoly(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  short arg;

   arg = (short)XmToggleButtonGadgetGetState(sspolyclosebut);
   if (arg)
     {
      XtSetSensitive(sspolyacceptbut,TRUE);
      surfselpolyopt = 2;
      XUndefineCursor(dpy,topwin);
      XFlush(dpy);
      getsurfpoly();

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
      drawselpoly(0);
     }
}


void sselacceptpoly(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i, isurf;

   arg = (long int)client_data;

   XmToggleButtonSetState(sspolymousebut,FALSE,FALSE);
   XmToggleButtonSetState(sspolyclosebut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(ssprbmousebut,FALSE,FALSE);         
   XmToggleButtonGadgetSetState(ssprbpolybut,FALSE,FALSE);         
   XtSetSensitive(sspolyacceptbut,FALSE);

   /*  If cancel button, remove selected surfaces.  */
   if (arg == 1)
     {
      for (i = 0; i < npolysurfs; i++)
        {
         isurf = polysurfnos[i];
         if (isurf >= 0) surfselectflagp[isurf] = 0;
        }
      npolysurfs = 0;
     }

   surfselpolyopt = 0;
   FREE(polysurfnos);
   npolysurfnosalloc = 0;
   XtUnmanageChild(surfpolyselform);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void sselprbsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   surfselprb_in = (short)XmToggleButtonGadgetGetState(sselprbin);
}


void sselprbapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   /*  Reset flags and widgets.  */
   XmToggleButtonGadgetSetState(ssprbmousebut,FALSE,TRUE);         
   XtUnmanageChild(surfprbselform);
   XmToggleButtonGadgetSetState(ssprbpolybut,FALSE,FALSE);         
   surfselprobopt = 0;
   surfselpolyopt = 0;
   surfselprobe_on = 0;

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  If selection is inside, reset surfselectflag.  */
   if (surfselprb_in == 1)
     {
      for (i = 0; i < nsurf; i++)
        {
         surfselectflag[i] = surfselectflagp[i];
         surfselectflagp[i] = 0;
        }
     }

   /*  If selection is outside, reset surfselectflag.  */
   else
     {
      for (i = 0; i < nsurf; i++)
        {
         if (surfselectflag[i] == 1 && surfselectflagp[i] == 0)
            surfselectflag[i] = 1;
         else
            surfselectflag[i] = 0;
         surfselectflagp[i] = 0;
        }      
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void smatsel_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   surfmatselprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (surfmatselprobeflag)
     {
      surfmatremprobeflag = 0;
      XmToggleButtonSetState(smatremprobe,FALSE,FALSE);
      XmListDeselectAllItems(smatsellist[0]);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


void smatrem_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   surfmatremprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (surfmatremprobeflag)
     {
      surfmatselprobeflag = 0;
      XmToggleButtonSetState(smatselprobe,FALSE,FALSE);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


void smatprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int surfid, id, goodid;
  char str[10];

   /*  Get the surface number.  */
   surfid = getsurfprobe(pa,pb,pc,pd,pe,pf);

   /*  Set material toggle button.  */
   id = -1;
   if (surfid >= 0 && surfid < nsurf)
      id = surfmats[surfid] - 1;
   goodid = 1;
   if (id < 0 || id >= maxsurfmat) goodid = 0;

   if (surfmatselprobeflag && goodid)
     {
      XmListSelectPos(smatsellist[0],id+1,FALSE);
     }
   if (surfmatremprobeflag && goodid)
     {
      XmListDeselectPos(smatsellist[0],id+1,FALSE);
     }

   XFlush(dpy);
}


void ssnumclr(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char blank[1] = "";

   for (i = 0; i < MAXNUMSEL; i++)
     {
      XmTextFieldSetString(sselnumtxt[i],blank);
     }
}


void makesurfselform(parent)
 Widget parent;
{
  Widget ssclose, ssapply, ssmatbut, sssfldbut, ssnfldbut, sssphbut,
         ssmatclose, sstxt[MAXFLAGS+1], sandorbox[MAXFLAGS],
         sson[MAXFLAGS+1], ssoff[MAXFLAGS+1], sssfldclose, 
         sselsfldlab1, sselsfldbut, sselsminlab1, sselsmaxlab1, 
         sselsrangelab, sselsminlab2, sselsmaxlab2, sselsreset,
         ssnfldclose, sselnfldlab1, sselnfldbut, sselnminlab1, 
         sselnmaxlab1, sselnrangelab, sselnminlab2, sselnmaxlab2, 
         sselnreset, sssphclose, sssphxlab, sssphylab, sssphzlab,
         sssphrlab, ssnfldbox, ssnumbut, ssnumclose, ssnumwin,
         ssnumform, ssnumlab[MAXNUMSEL], ssmatapply, ssnfldapply,
         sssfldapply, sssphapply, ssnumapply, sssphbox, ssboxclose,
         ssboxapply, ssboxx1lab, ssboxx2lab, ssboxy1lab, ssboxy2lab,
         ssboxz1lab, ssboxz2lab, ssboxbox, ssboxbut, sssgrpclose,
         sssgrpapply, ssprbclose, ssprbapply, ssprbbox, ssgrpbox,
         sspolybox, sspolycancelbut, ssnumclrbut;
  Widget lastleft, surfselswin, sswinform, sssgrpbut, ssprbbut;
  int i, j, jj, k, kk, winw;
  char str[20];
  Arg wargs[MAXGROUPS+20];
  XmString string; 
  XmString surfmatitem[MAXMATS];
  XmString surfgrpitem[MAXGROUPS];


   string = XmStringCreate("Surf Select",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfselform = XmCreateFormDialog(parent,"Surf_Select",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ssclose = XtVaCreateManagedWidget("Ssclose",
             xmPushButtonGadgetClass,
             surfselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(ssclose,XmNactivateCallback,sselclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ssapply = XtVaCreateManagedWidget("Ssapply",
             xmPushButtonGadgetClass,
             surfselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, ssclose,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(ssapply,XmNactivateCallback,sselapply,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[0] = XtVaCreateManagedWidget("Sselonbut0",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssclose,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[0],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Materials\nand Flags",
                               XmSTRING_DEFAULT_CHARSET);
   ssmatbut = XtVaCreateManagedWidget("Ssmatbut",
             xmPushButtonGadgetClass,
             surfselform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, ssclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, sselonbut[0],
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(ssmatbut,XmNactivateCallback,surfmatselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[1] = XtVaCreateManagedWidget("Sselonbut1",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssmatbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[1],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 1);

   string = XmStringLtoRCreate("Surf Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   sssfldbut = XtVaCreateManagedWidget("Sssfldbut",
               xmPushButtonGadgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ssmatbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sselonbut[1],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sssfldbut,XmNactivateCallback,surfsfldselformpop,NULL);
   if (snumvars == 0)
     {
      XtSetSensitive(sselonbut[1],FALSE);
      XtSetSensitive(sssfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[2] = XtVaCreateManagedWidget("Sselonbut2",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sssfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[2],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 2);

   string = XmStringLtoRCreate("Node Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   ssnfldbut = XtVaCreateManagedWidget("Ssnfldbut",
               xmPushButtonGadgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sssfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sselonbut[2],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ssnfldbut,XmNactivateCallback,surfnfldselformpop,NULL);
   if (numvars == 0)
     {
      XtSetSensitive(sselonbut[2],FALSE);
      XtSetSensitive(ssnfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[3] = XtVaCreateManagedWidget("Sselonbut3",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssnfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[3],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 3);

   string = XmStringLtoRCreate("Search\nSphere",
                               XmSTRING_DEFAULT_CHARSET);
   sssphbut = XtVaCreateManagedWidget("Sssphbut",
               xmPushButtonGadgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ssnfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sselonbut[3],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sssphbut,XmNactivateCallback,surfsphselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[4] = XtVaCreateManagedWidget("Sselonbut4",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, sssphbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[4],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 4);

   string = XmStringLtoRCreate("Surf\nNumbers",
                               XmSTRING_DEFAULT_CHARSET);
   ssnumbut = XtVaCreateManagedWidget("Ssnumbut",
              xmPushButtonGadgetClass,
              surfselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sssphbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, sselonbut[4],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ssnumbut,XmNactivateCallback,surfnumselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[5] = XtVaCreateManagedWidget("Sselonbut5",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssnumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[5],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 5);

   string = XmStringLtoRCreate("Search\nBox",
                               XmSTRING_DEFAULT_CHARSET);
   ssboxbut = XtVaCreateManagedWidget("Ssboxbut",
               xmPushButtonGadgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ssnumbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sselonbut[5],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ssboxbut,XmNactivateCallback,surfboxselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   sselonbut[6] = XtVaCreateManagedWidget("Sselonbut5",
                  xmToggleButtonWidgetClass,
                  surfselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssboxbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut[6],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 6);

   string = XmStringLtoRCreate("Surface\nGroups",
                               XmSTRING_DEFAULT_CHARSET);
   sssgrpbut = XtVaCreateManagedWidget("Sssgrpbut",
               xmPushButtonGadgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ssboxbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, sselonbut[6],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(sssgrpbut,XmNactivateCallback,surfgrpselformpop,NULL);

   if (physedflag)
     {
      string = XmStringLtoRCreate("Interactive\nSelection",
                                  XmSTRING_DEFAULT_CHARSET);
      ssprbbut = XtVaCreateManagedWidget("Ssprbbut",
                 xmPushButtonGadgetClass,
                 surfselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sssgrpbut,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNleftOffset, 10,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
         XmStringFree(string);
         XtAddCallback(ssprbbut,XmNactivateCallback,surfprbselformpop,NULL);
     }

   if (nsurfacegrps == 0 && physedflag == 0)
     {
      XtSetSensitive(sselonbut[6],FALSE);
      XtSetSensitive(sssgrpbut,FALSE);
     }

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   sselerror = XtVaCreateManagedWidget("Sselerror",
               xmLabelWidgetClass,
               surfselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sssgrpbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 


   /*  Create Materials and Flags selection menu.  */
   /************************************************/

   string = XmStringCreate("Surf Select - Materials and Flags",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfmatselform = XmCreateFormDialog(surfselform,"Surfmatselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ssmatclose = XtVaCreateManagedWidget("Ssmatclose",
                xmPushButtonGadgetClass,
                surfmatselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssmatclose,XmNactivateCallback,sselmatclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ssmatapply = XtVaCreateManagedWidget("Ssmatapply",
                xmPushButtonGadgetClass,
                surfmatselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, ssmatclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssmatapply,XmNactivateCallback,sselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   sselonbut2[0] = XtVaCreateManagedWidget("Sselonbut20",
                   xmToggleButtonWidgetClass,
                   surfmatselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, ssmatclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut2[0],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 0);

   winw = 180;
   if (numflags_surf > 0) winw = 250;
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sselonbut2[0]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   /* i++;  XtSetArg(wargs[i],XmNwidth, winw); */
   i++;  XtSetArg(wargs[i],XmNheight, 320);
   i++;
   surfselswin = XmCreateScrolledWindow(surfmatselform,"Surfselswin",wargs,i);
   XtManageChild(surfselswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   sswinform = XmCreateForm(surfselswin,"Sswinform",wargs,i);

   /*  Create material selections.  */

   string = XmStringCreate("Materials",XmSTRING_DEFAULT_CHARSET);
   sstxt[0] = XtVaCreateManagedWidget("Sstxt0",
              xmLabelWidgetClass,
              sswinform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   sson[0] = XtVaCreateManagedWidget("Sson0",
             xmPushButtonGadgetClass,
             sswinform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, sstxt[0],
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(sson[0],XmNactivateCallback,ssset_on,
                    (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   ssoff[0] = XtVaCreateManagedWidget("Ssoff0",
              xmPushButtonGadgetClass,
              sswinform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, sstxt[0],
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, sson[0],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ssoff[0],XmNactivateCallback,ssset_off,
                    (caddr_t) 0);

   for (i = 0; i < maxsurfmat; i++)
      surfmatitem[i] = XmStringLtoRCreate(mmatnames[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sson[0]);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   if (maxsurfmat > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, maxsurfmat);
      i++;  XtSetArg(wargs[i],XmNitems, surfmatitem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
   i++;
   smatsellist[0] = (Widget)XmCreateScrolledList(sswinform,
                                                 "Smatsellist0",wargs,i);
   XtManageChild(smatsellist[0]);
   for (i = 0; i < maxsurfmat; i++) 
      XmStringFree(surfmatitem[i]);

   /*  Create interactive mouse material selection buttons.  */
 
   string = XmStringLtoRCreate("Select Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   smatselprobe = XtVaCreateManagedWidget("Smatselprobe",
                  xmToggleButtonGadgetClass,
                  sswinform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, smatsellist[0],
                  XmNtopOffset, 5,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(smatselprobe,XmNvalueChangedCallback,
                    smatsel_probe,NULL);
 
   string = XmStringLtoRCreate("Remove Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   smatremprobe = XtVaCreateManagedWidget("Smatremprobe",
                  xmToggleButtonGadgetClass,
                  sswinform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, smatselprobe,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(smatremprobe,XmNvalueChangedCallback,
                    smatrem_probe,NULL);
  
   /*  Create sets of and/or and flag selection buttons.  */
 
   lastleft = smatsellist[0];
   if (numflags_surf > 0)
     {
      for (j = 0; j < numflags_surf; j++)
        {
         jj = j + 1;
         i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNtopWidget, sson[j]);
         i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNleftWidget, lastleft);
         i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;
         sprintf(str,"Sandorbox%d",j);
         sandorbox[j] = XmCreateRadioBox(sswinform,str,wargs,i);
         XtManageChild(sandorbox[j]);

         string = XmStringCreate("And",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Sandbuts%d",j);
         smatandbut[j] = XtVaCreateManagedWidget(str,
                       xmToggleButtonGadgetClass,
                       sandorbox[j],
                       XmNlabelString, string,
                       XmNspacing, 1,
                       XmNselectColor, yellowpix,
                       XmNmarginHeight, 0,
                       XmNmarginWidth, 0,
                       XmNset, TRUE,
                       NULL);
            XmStringFree(string);
            XtAddCallback(smatandbut[j],XmNvalueChangedCallback,
                          sselect_and_flag,(caddr_t) j);
         string = XmStringCreate("Or",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Sorbuts%d",j);
         smatorbut[j] = XtVaCreateManagedWidget(str,
                      xmToggleButtonGadgetClass,
                      sandorbox[j],
                      XmNlabelString, string,
                      XmNspacing, 1,
                      XmNselectColor, yellowpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNset, FALSE,
                      NULL);
            XmStringFree(string);
            XtAddCallback(smatorbut[j],XmNvalueChangedCallback,
                          sselect_or_flag,(caddr_t) j);

         string = XmStringCreate(flagname_surf[j],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Sstxt%d",jj);
         sstxt[jj] = XtVaCreateManagedWidget(str,
                     xmLabelWidgetClass,
                     sswinform,
                     XmNtopAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, sandorbox[j],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string);

         string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Sson%d",jj);
         sson[jj] = XtVaCreateManagedWidget(str,
                    xmPushButtonGadgetClass,
                    sswinform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sstxt[jj],
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sandorbox[j],
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string);
            XtAddCallback(sson[jj],XmNactivateCallback,ssset_on,
                          (caddr_t) jj);

         string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Ssoff%d",jj);
         ssoff[jj] = XtVaCreateManagedWidget(str,
                     xmPushButtonGadgetClass,
                     sswinform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sstxt[jj],
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, sson[jj],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
            XmStringFree(string);
            XtAddCallback(ssoff[jj],XmNactivateCallback,ssset_off,
                          (caddr_t) jj);

         kk = flagtypes_surf[j];
         for (i = 0; i < kk; i++)
            surfmatitem[i] = XmStringLtoRCreate(flagnames_surf[j][i],
                                                XmSTRING_DEFAULT_CHARSET);

         k = j + 1;
         i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNtopWidget, sson[jj]);
         i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNleftWidget, sandorbox[j]);
         i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNheight, 160);
         if (kk > 0)
           {
            i++;  XtSetArg(wargs[i],XmNitemCount, kk);
            i++;  XtSetArg(wargs[i],XmNitems, surfmatitem);
           }
         i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
         i++;
         sprintf(str,"Smatsellist%d",k);
         smatsellist[k] = (Widget)XmCreateScrolledList(sswinform,
                                                       str,wargs,i);
         XtManageChild(smatsellist[k]);
         for (i = 0; i < kk; i++) 
            XmStringFree(surfmatitem[i]);

         lastleft = smatsellist[k];
        } 
     }
     
   XtManageChild(sswinform);


   /*  Create Surf Field Data Range selection menu.  */
   /**************************************************/

    if (snumvars > 0)
     {
      string = XmStringCreate("Surf Select - Surf Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      surfsfldselform = XmCreateFormDialog(surfselform,"Surfsfldselform",
                                           wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      sssfldclose = XtVaCreateManagedWidget("Sssfldclose",
                    xmPushButtonGadgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sssfldclose,XmNactivateCallback,sselsfldclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      sssfldapply = XtVaCreateManagedWidget("Sssfldapply",
                    xmPushButtonGadgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sssfldclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sssfldapply,XmNactivateCallback,sselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      sselonbut2[1] = XtVaCreateManagedWidget("Sselonbut21",
                      xmToggleButtonWidgetClass,
                      surfsfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sssfldclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(sselonbut2[1],XmNvalueChangedCallback,sselsetonbut,
                       (caddr_t) 1);

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      sselsfldlab1 = XtVaCreateManagedWidget("Sselsfldlab1",
                     xmLabelWidgetClass,
                     surfsfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselonbut2[1],
                     XmNtopOffset, 10,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselsfldlab = XtVaCreateManagedWidget("Sselsfldlab",
                    xmLabelWidgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselonbut2[1],
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselsfldlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      sselsfldbut = XtVaCreateManagedWidget("Sselsfldbut",
                    xmPushButtonGadgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselsfldlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sselsfldbut,XmNactivateCallback,sselsfldfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      sselsminlab1 = XtVaCreateManagedWidget("Sselminlab1",
                     xmLabelWidgetClass,
                     surfsfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselsfldbut,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselsminlab = XtVaCreateManagedWidget("Sselcminlab",
                    xmLabelWidgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselsfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselsminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      sselsmaxlab1 = XtVaCreateManagedWidget("Sselcmaxlab1",
                     xmLabelWidgetClass,
                     surfsfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselsminlab1,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselsmaxlab = XtVaCreateManagedWidget("Sselcmaxlab",
                    xmLabelWidgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselsminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselsmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      sselsrangelab = XtVaCreateManagedWidget("Sselcrangelab",
                      xmLabelWidgetClass,
                      surfsfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sselsmaxlab1,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      sselsminlab2 = XtVaCreateManagedWidget("Sselcminlab2",
                     xmLabelWidgetClass,
                     surfsfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselsrangelab,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      sselsmintxt = XtVaCreateManagedWidget("Sselmintxt",
                   xmTextFieldWidgetClass,
                   surfsfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sselsrangelab,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, sselsminlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      sselsmaxlab2 = XtVaCreateManagedWidget("Sselmaxlab2",
                    xmLabelWidgetClass,
                    surfsfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselsminlab2,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      sselsmaxtxt = XtVaCreateManagedWidget("Sselmaxtxt",
                   xmTextFieldWidgetClass,
                   surfsfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sselsminlab2,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, sselsmaxlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      sselsreset = XtVaCreateManagedWidget("Sselcreset",
                   xmPushButtonGadgetClass,
                   surfsfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sselsmaxtxt,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(sselsreset,XmNactivateCallback,sselsfldreset,
                       NULL);
     }


   /*  Create Node Field Data Range selection menu.  */
   /**************************************************/

    if (numvars > 0)
     {
      string = XmStringCreate("Surf Select - Node Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      surfnfldselform = XmCreateFormDialog(surfselform,"Surfnfldselform",
                                           wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      ssnfldclose = XtVaCreateManagedWidget("Ssnfldclose",
                    xmPushButtonGadgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(ssnfldclose,XmNactivateCallback,sselnfldclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      ssnfldapply = XtVaCreateManagedWidget("Ssnfldapply",
                    xmPushButtonGadgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, ssnfldclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(ssnfldapply,XmNactivateCallback,sselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      sselonbut2[2] = XtVaCreateManagedWidget("Sselonbut22",
                      xmToggleButtonWidgetClass,
                      surfnfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, ssnfldclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(sselonbut2[2],XmNvalueChangedCallback,sselsetonbut,
                       (caddr_t) 2);

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      sselnfldlab1 = XtVaCreateManagedWidget("Sselnfldlab1",
                     xmLabelWidgetClass,
                     surfnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselonbut2[2],
                     XmNtopOffset, 10,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselnfldlab = XtVaCreateManagedWidget("Sselnfldlab",
                    xmLabelWidgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselonbut2[2],
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselnfldlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      sselnfldbut = XtVaCreateManagedWidget("Sselnfldbut",
                    xmPushButtonGadgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnfldlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sselnfldbut,XmNactivateCallback,sselnfldfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      sselnminlab1 = XtVaCreateManagedWidget("Sselnminlab1",
                     xmLabelWidgetClass,
                     surfnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselnfldbut,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselnminlab = XtVaCreateManagedWidget("Sselnminlab",
                    xmLabelWidgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselnminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      sselnmaxlab1 = XtVaCreateManagedWidget("Sselnmaxlab1",
                     xmLabelWidgetClass,
                     surfnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselnminlab1,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      sselnmaxlab = XtVaCreateManagedWidget("Sselnmaxlab",
                    xmLabelWidgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselnmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      sselnrangelab = XtVaCreateManagedWidget("Sselnrangelab",
                      xmLabelWidgetClass,
                      surfnfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sselnmaxlab1,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      sselnminlab2 = XtVaCreateManagedWidget("Sselnminlab2",
                     xmLabelWidgetClass,
                     surfnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselnrangelab,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      sselnmintxt = XtVaCreateManagedWidget("Sselnmintxt",
                    xmTextFieldWidgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnrangelab,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselnminlab2,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 20,
                    XmNcolumns, 18,
                    XmNvalue, "",
                    NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      sselnmaxlab2 = XtVaCreateManagedWidget("Sselnmaxlab2",
                     xmLabelWidgetClass,
                     surfnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sselnminlab2,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      sselnmaxtxt = XtVaCreateManagedWidget("Sselnmaxtxt",
                    xmTextFieldWidgetClass,
                    surfnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnminlab2,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sselnmaxlab2,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 20,
                    XmNcolumns, 18,
                    XmNvalue, "",
                    NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      sselnreset = XtVaCreateManagedWidget("Sselnreset",
                   xmPushButtonGadgetClass,
                   surfnfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sselnmaxtxt,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(sselnreset,XmNactivateCallback,sselnfldreset,
                       NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, sselnreset);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      ssnfldbox = XmCreateRadioBox(surfnfldselform,"Ssnfldbox",wargs,i);
      XtManageChild(ssnfldbox);
 
      string = XmStringCreate("Any Nodes",
               XmSTRING_DEFAULT_CHARSET);
      sselnfldany = XtVaCreateManagedWidget("Sselnfldany",
                    xmToggleButtonGadgetClass,
                    ssnfldbox,
                    XmNlabelString, string,
                    XmNset, TRUE,
                    XmNspacing, 2,
                    XmNselectColor, yellowpix,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sselnfldany,XmNvalueChangedCallback,
                       sselnfldsetopt,(caddr_t) 0);
 
      string = XmStringCreate("All Nodes",
               XmSTRING_DEFAULT_CHARSET);
      sselnfldall = XtVaCreateManagedWidget("Sselnfldall",
                  xmToggleButtonGadgetClass,
                  ssnfldbox,
                  XmNlabelString, string,
                  XmNset, FALSE,
                  XmNspacing, 2,
                  XmNselectColor, yellowpix,
                  NULL);
         XmStringFree(string);
         XtAddCallback(sselnfldall,XmNvalueChangedCallback,
                       sselnfldsetopt,(caddr_t) 1);
     }


   /*  Create Search Sphere selection menu.  */
   /*****************************************/

   string = XmStringCreate("Surf Select - Search Sphere",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfsphselform = XmCreateFormDialog(surfselform,"Surfsphselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   sssphclose = XtVaCreateManagedWidget("Sssphclose",
                xmPushButtonGadgetClass,
                surfsphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(sssphclose,XmNactivateCallback,sselsphclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   sssphapply = XtVaCreateManagedWidget("Sssphapply",
                xmPushButtonGadgetClass,
                surfsphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, sssphclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(sssphapply,XmNactivateCallback,sselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   sselonbut2[3] = XtVaCreateManagedWidget("Sselonbut23",
                   xmToggleButtonWidgetClass,
                   surfsphselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, sssphclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut2[3],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 3);

   string = XmStringCreate("X:",XmSTRING_DEFAULT_CHARSET);
   sssphxlab = XtVaCreateManagedWidget("Sssphxlab",
               xmLabelWidgetClass,
               surfsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sselonbut2[3],
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   sselsphxtxt = XtVaCreateManagedWidget("Sselsphxtxt",
                 xmTextFieldWidgetClass,
                 surfsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sselonbut2[3],
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, sssphxlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Y:",XmSTRING_DEFAULT_CHARSET);
   sssphylab = XtVaCreateManagedWidget("Sssphylab",
               xmLabelWidgetClass,
               surfsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sssphxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   sselsphytxt = XtVaCreateManagedWidget("Sselsphytxt",
                 xmTextFieldWidgetClass,
                 surfsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sssphxlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, sssphylab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Z:",XmSTRING_DEFAULT_CHARSET);
   sssphzlab = XtVaCreateManagedWidget("Sssphzlab",
               xmLabelWidgetClass,
               surfsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sssphylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   sselsphztxt = XtVaCreateManagedWidget("Sselsphztxt",
                 xmTextFieldWidgetClass,
                 surfsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sssphylab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, sssphzlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("R:",XmSTRING_DEFAULT_CHARSET);
   sssphrlab = XtVaCreateManagedWidget("Sssphrlab",
               xmLabelWidgetClass,
               surfsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, sssphzlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   sselsphrtxt = XtVaCreateManagedWidget("Sselsphrtxt",
                 xmTextFieldWidgetClass,
                 surfsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, sssphzlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, sssphrlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sselsphrtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   sssphbox = XmCreateRadioBox(surfsphselform,"Sssphbox",wargs,i);
   XtManageChild(sssphbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   sselsphin = XtVaCreateManagedWidget("Sselsphin",
               xmToggleButtonGadgetClass,
               sssphbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(sselsphin,XmNvalueChangedCallback,
                   sselsphsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   sselsphout = XtVaCreateManagedWidget("Sselsphout",
                xmToggleButtonGadgetClass,
                sssphbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(sselsphout,XmNvalueChangedCallback,
                   sselsphsetinopt,(caddr_t) 0);


   /*  Create Search Box selection menu.  */
   /***************************************/

   string = XmStringCreate("Surf Select - Search Box",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfboxselform = XmCreateFormDialog(surfselform,"Surfboxselform",
                                       wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ssboxclose = XtVaCreateManagedWidget("Ssboxclose",
                xmPushButtonGadgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssboxclose,XmNactivateCallback,sselboxclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ssboxapply = XtVaCreateManagedWidget("Ssboxapply",
                xmPushButtonGadgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, ssboxclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssboxapply,XmNactivateCallback,sselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   sselonbut2[5] = XtVaCreateManagedWidget("Sselonbut25",
                   xmToggleButtonWidgetClass,
                   surfboxselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, ssboxclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut2[5],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 5);

   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   ssboxx1lab = XtVaCreateManagedWidget("Ssboxx1lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselonbut2[5],
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxx1sl = XtVaCreateManagedWidget("Sselboxx1sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxx1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxx1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 0);
      XtAddCallback(sselboxx1sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 6);

   sselboxx1txt = XtVaCreateManagedWidget("Sselboxx1txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxx1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxx1txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   ssboxx2lab = XtVaCreateManagedWidget("Ssboxx2lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselonbut2[5],
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, sselboxx1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxx2sl = XtVaCreateManagedWidget("Sselboxx2sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxx2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxx2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 1);
      XtAddCallback(sselboxx2sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 7);

   sselboxx2txt = XtVaCreateManagedWidget("Sselboxx2txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxx2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxx2txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   ssboxy1lab = XtVaCreateManagedWidget("Ssboxy1lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselboxx1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxy1sl = XtVaCreateManagedWidget("Sselboxy1sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxy1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxy1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 2);
      XtAddCallback(sselboxy1sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 8);

   sselboxy1txt = XtVaCreateManagedWidget("Sselboxy1txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxy1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxy1txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   ssboxy2lab = XtVaCreateManagedWidget("Ssboxy2lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselboxx1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, sselboxy1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxy2sl = XtVaCreateManagedWidget("Sselboxy2sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxy2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxy2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 3);
      XtAddCallback(sselboxy2sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 9);

   sselboxy2txt = XtVaCreateManagedWidget("Sselboxy2txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxy2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxy2txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   ssboxz1lab = XtVaCreateManagedWidget("Ssboxz1lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselboxy1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxz1sl = XtVaCreateManagedWidget("Sselboxz1sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxz1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxz1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 4);
      XtAddCallback(sselboxz1sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 10);

   sselboxz1txt = XtVaCreateManagedWidget("Sselboxz1txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxz1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxz1txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   ssboxz2lab = XtVaCreateManagedWidget("Ssboxz2lab",
                xmLabelWidgetClass,
                surfboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, sselboxy1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, sselboxz1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   sselboxz2sl = XtVaCreateManagedWidget("Sselboxz2sl",
                 xmScaleWidgetClass,
                 surfboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ssboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssboxz2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(sselboxz2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 5);
      XtAddCallback(sselboxz2sl,XmNdragCallback,ssboxsubsetsl,
                    (caddr_t) 11);

   sselboxz2txt = XtVaCreateManagedWidget("Sselboxz2txt",
                  xmTextFieldWidgetClass,
                  surfboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ssboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, sselboxz2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(sselboxz2txt,XmNactivateCallback,
                    sselboxtxt,(caddr_t) 5);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sselboxz1txt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ssboxbox = XmCreateRadioBox(surfboxselform,"Ssboxbox",wargs,i);
   XtManageChild(ssboxbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   sselboxin = XtVaCreateManagedWidget("Sselboxin",
               xmToggleButtonGadgetClass,
               ssboxbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(sselboxin,XmNvalueChangedCallback,
                   sselboxsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   sselboxout = XtVaCreateManagedWidget("Sselboxout",
                xmToggleButtonGadgetClass,
                ssboxbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(sselboxout,XmNvalueChangedCallback,
                   sselboxsetinopt,(caddr_t) 0);


   /*  Create Surf Number selection menu.  */
   /****************************************/

   string = XmStringCreate("Surf Select - Surf Number",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfnumselform = XmCreateFormDialog(surfselform,"Surfnumselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ssnumclose = XtVaCreateManagedWidget("Ssnumclose",
                xmPushButtonGadgetClass,
                surfnumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssnumclose,XmNactivateCallback,sselnumclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ssnumapply = XtVaCreateManagedWidget("Ssnumapply",
                xmPushButtonGadgetClass,
                surfnumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, ssnumclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssnumapply,XmNactivateCallback,sselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   sselonbut2[4] = XtVaCreateManagedWidget("Sselonbut24",
                   xmToggleButtonWidgetClass,
                   surfnumselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, ssnumclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(sselonbut2[4],XmNvalueChangedCallback,sselsetonbut,
                    (caddr_t) 4);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, sselonbut2[4]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 150);
   i++;  XtSetArg(wargs[i],XmNheight, 250);
   i++;
   ssnumwin = XmCreateScrolledWindow(surfnumselform,"Ssnumwin",wargs,i);
   XtManageChild(ssnumwin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   ssnumform = XmCreateForm(ssnumwin,"Ssnumform",wargs,i);

   string = XmStringCreate(" 1",XmSTRING_DEFAULT_CHARSET);
   ssnumlab[0] = XtVaCreateManagedWidget("Ssnumlab0",
                 xmLabelWidgetClass,
                 ssnumform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   sselnumtxt[0] = XtVaCreateManagedWidget("Sselnumtxt0",
                   xmTextFieldWidgetClass,
                   ssnumform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, ssnumlab[0],
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 29,
                   XmNcolumns, 29,
                   XmNvalue, "",
                   NULL);

   for (i = 1; i < MAXNUMSEL; i++)
     {
      sprintf(str,"%2d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Ssnumlab%d",i);
      ssnumlab[i] = XtVaCreateManagedWidget("str",
                    xmLabelWidgetClass,
                    ssnumform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, sselnumtxt[i-1],
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string); 

      sprintf(str,"Sselnumtxt%d",i);
      sselnumtxt[i] = XtVaCreateManagedWidget(str,
                      xmTextFieldWidgetClass,
                      ssnumform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sselnumtxt[i-1],
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, ssnumlab[i],
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 1,
                      XmNmaxLength, 29,
                      XmNcolumns, 29,
                      XmNvalue, "",
                      NULL);
     }

   string = XmStringCreate("Clear Entries",
                               XmSTRING_DEFAULT_CHARSET);
   ssnumclrbut = XtVaCreateManagedWidget("Ssnumclrbut",
                 xmPushButtonGadgetClass,
                 surfnumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, ssnumwin,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(ssnumclrbut,XmNactivateCallback,ssnumclr,
                    (caddr_t) 0);

   XtManageChild(ssnumform);


   /*  Create Surface Groups selection menu.  */
   /************************************************/

   if (nsurfacegrps > 0 || physedflag)
     {
      string = XmStringCreate("Surface Select - Surface Groups",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      surfgrpselform = XmCreateFormDialog(surfselform,"Surfgrpselform",wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      sssgrpclose = XtVaCreateManagedWidget("Sssgrpclose",
                    xmPushButtonGadgetClass,
                    surfgrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sssgrpclose,XmNactivateCallback,surfgrpclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      sssgrpapply = XtVaCreateManagedWidget("Sssgrpapply",
                    xmPushButtonGadgetClass,
                    surfgrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, sssgrpclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(sssgrpapply,XmNactivateCallback,sselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      sselonbut2[6] = XtVaCreateManagedWidget("Sselonbut26",
                      xmToggleButtonWidgetClass,
                      surfgrpselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, sssgrpclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(sselonbut2[6],XmNvalueChangedCallback,sselsetonbut,
                       (caddr_t) 6);

      for (i = 0; i < nsurfacegrps; i++)
         surfgrpitem[i] = XmStringLtoRCreate(surfacegrpname[i],
                                             XmSTRING_DEFAULT_CHARSET);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, sselonbut2[6]);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNheight, 250);
      if (nsurfacegrps > 0)
        {
         i++;  XtSetArg(wargs[i],XmNitemCount, nsurfacegrps);
         i++;  XtSetArg(wargs[i],XmNitems, surfgrpitem);
        }
      i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmEXTENDED_SELECT);
      i++;
      surfgrplist = (Widget)XmCreateScrolledList(surfgrpselform,
                                                 "Surfgrplist",wargs,i);
      XtManageChild(surfgrplist);
      for (i = 0; i < nsurfacegrps; i++) 
         XmStringFree(surfgrpitem[i]);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, surfgrplist);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      ssgrpbox = XmCreateRadioBox(surfgrpselform,"Ssgrpbox",wargs,i);
      XtManageChild(ssgrpbox);

      string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
      sselgrpin = XtVaCreateManagedWidget("Sselgrpin",
                  xmToggleButtonGadgetClass,
                  ssgrpbox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
        XmStringFree(string);
        XtAddCallback(sselgrpin,XmNvalueChangedCallback,
                      sselgrpsetinopt,(caddr_t) 0);

      string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
      sselgrpout = XtVaCreateManagedWidget("Sselgrpout",
                   xmToggleButtonGadgetClass,
                   ssgrpbox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
        XmStringFree(string);
        XtAddCallback(sselgrpout,XmNvalueChangedCallback,
                      sselgrpsetinopt,(caddr_t) 0);
     }


   /*  Create surface interactive selection menu (physed only).  */
   /**************************************************************/

   surfselectflagp = (char *)malloc(nsurf*sizeof(char));
   if (surfselectflagp == NULL) memerr();
   for (i = 0; i < nsurf; i++) surfselectflagp[i] = 0;

   if (gmvflag) return;

   string = XmStringCreate("Surf Select - Interactive Selection",
                            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfprbselform = XmCreateFormDialog(surfselform,"Surfprbselform",
                      wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ssprbclose = XtVaCreateManagedWidget("Ssprbclose",
                xmPushButtonGadgetClass,
                surfprbselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssprbclose,XmNactivateCallback,sselprbclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   ssprbapply = XtVaCreateManagedWidget("Ssprbapply",
                xmPushButtonGadgetClass,
                surfprbselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, ssprbclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ssprbapply,XmNactivateCallback,sselprbapply,NULL);

   string = XmStringCreate("Left Mouse Click", XmSTRING_DEFAULT_CHARSET);
   ssprbmousebut = XtVaCreateManagedWidget("Ssprbmousebut",
                   xmToggleButtonGadgetClass,
                   surfprbselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, ssprbapply,
                   XmNtopOffset, 20,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
     XmStringFree(string);
     XtAddCallback(ssprbmousebut,XmNvalueChangedCallback,
                   sselmouse,NULL);

   string = XmStringCreate("Polygon Area", XmSTRING_DEFAULT_CHARSET);
   ssprbpolybut = XtVaCreateManagedWidget("Ssprbpolybut",
                  xmToggleButtonGadgetClass,
                  surfprbselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, ssprbmousebut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
     XmStringFree(string);
     XtAddCallback(ssprbpolybut,XmNvalueChangedCallback,
                   sselpoly,NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, ssprbpolybut);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 20);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   ssprbbox = XmCreateRadioBox(surfprbselform,"Ssprbbox",wargs,i);
   XtManageChild(ssprbbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   sselprbin = XtVaCreateManagedWidget("Sselprbin",
               xmToggleButtonGadgetClass,
               ssprbbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(sselprbin,XmNvalueChangedCallback,
                   sselprbsetinopt,NULL);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   sselprbout = XtVaCreateManagedWidget("Sselprbout",
                xmToggleButtonGadgetClass,
                ssprbbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(sselprbout,XmNvalueChangedCallback,
                   sselprbsetinopt,NULL);


   string = XmStringCreate("Surf Select - Polygon Area",
                            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   surfpolyselform = XmCreateFormDialog(surfprbselform,"Surfpolyselform",
                      wargs,i);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, ssprbapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 20);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   sspolybox = XmCreateRadioBox(surfpolyselform,"Sspolybox",wargs,i);
   XtManageChild(sspolybox);

   string = XmStringCreate("Draw Polygon Vertices", XmSTRING_DEFAULT_CHARSET);
   sspolymousebut = XtVaCreateManagedWidget("Sspolymousebut",
                   xmToggleButtonGadgetClass,
                   sspolybox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
     XmStringFree(string);
     XtAddCallback(sspolymousebut,XmNvalueChangedCallback,
                   sseldrawpoly,NULL);

   string = XmStringCreate("Close Polygon", XmSTRING_DEFAULT_CHARSET);
   sspolyclosebut = XtVaCreateManagedWidget("Sspolyclosebut",
                  xmToggleButtonGadgetClass,
                  sspolybox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
     XmStringFree(string);
     XtAddCallback(sspolyclosebut,XmNvalueChangedCallback,
                   sselclosepoly,NULL);

   string = XmStringCreate("Accept Polygon",XmSTRING_DEFAULT_CHARSET);
   sspolyacceptbut = XtVaCreateManagedWidget("Sspolyacceptbut",
                     xmPushButtonGadgetClass,
                     surfpolyselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sspolybox,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(sspolyacceptbut,XmNactivateCallback,
                    sselacceptpoly,(caddr_t) 0);
      XtSetSensitive(sspolyacceptbut,FALSE);

   string = XmStringCreate("Cancel",XmSTRING_DEFAULT_CHARSET);
   sspolycancelbut = XtVaCreateManagedWidget("Sspolycancelbut",
                     xmPushButtonGadgetClass,
                     surfpolyselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, sspolyacceptbut,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
      XmStringFree(string);
      XtAddCallback(sspolycancelbut,XmNactivateCallback,
                    sselacceptpoly,(caddr_t) 1);
}


void setsurfsel()
{
  int i, j, k, nf, isurf, ii, jj, id, igrp;
  long ifirst, ilast, istride;
  short isel, jsel, nseln;
  double rad, dist, xd, yd, zd;
  double *svalue, *nvalue, xb, yb, zb;
  double * readsurfvalue(int ifld);
  double * readnodevalue(int ifld);

   /*  Allocate array and read field(s) for range selection, if needed.  */
   if (surfselsfld_on)
      svalue = readsurfvalue(surfselfield);
   if (surfselnfld_on)
      nvalue = readnodevalue(surfselnfield);

   /*  For search sphere test, square the sphere radius.  */
   rad = (double)surfselsph_r * (double)surfselsph_r;

   /*  Loop through surfaces and set selections.  */
   for (i = 0; i < nsurf; i++)
     {
      surfselectflag[i] = 1;

      /*  Set material selections first.  */
      if (surfselmat_on)
        {
         surfselectflag[i] = 0;

         for (k = 0; k < maxsurfmat; k++)
            if (surfmats[i]-1 == k && surfselflag[0][k] == 1)
                surfselectflag[i] = 1;
         if (maxsurfmat < 1) surfselectflag[i] = 1;

         /*  Add surfaces with a 0 material no.  */
         if (surfmats[i] < 1) surfselectflag[i] = 1;

         /*  Now check selection flags.  */
         for (j = 1; j <= numflags_surf; j++)
           {
            nf = flagtypes_surf[j-1];
            jsel = 0;
            for (k = 0; k < nf; k++)
               if (surfflags[j-1][i]-1 == k && surfselflag[j][k] == 1)
                  jsel = 1;

            /*  And/or with previous select flag.  */
            isel = surfselectflag[i];
            surfselectflag[i] = 0;
            if (surfandorflag[j-1] == 0)
               if (isel == 1 && jsel == 1) surfselectflag[i] = 1;
            if (surfandorflag[j-1] == 1)
               if (isel == 1 || jsel == 1) surfselectflag[i] = 1;
           }
        }

      /*  Now check surface field range, if requested.  */
      isel = (int)surfselectflag[i];
      if (surfselsfld_on && isel)
        {
         jsel = 0;
         if (sfldselmin <= svalue[i] && svalue[i] <= sfldselmax)
            jsel = 1;

         surfselectflag[i] = jsel;
        }

      /*  If node field range or search sphere requested,  */
      /*  and structered grid, set surface data.           */
      isurf = i;
      
      /*  Now check node field range, if requested.  */
      isel = (int)surfselectflag[i];
      if (surfselnfld_on && isel)
        {
         jsel = 0;
         nseln = 0;

         /*  Loop through surf nodes.  */
         for (j = 0; j < surfs[isurf].nverts; j++)
           {
            k = surfs[isurf].fverts[j];
            if (snfldselmin <= nvalue[k] && nvalue[k] <= snfldselmax)
               nseln++;
           }
         if (surfselnfldopt == 0 && nseln > 0)
            jsel = 1;
         if (surfselnfldopt == 1 && nseln == surfs[isurf].nverts) 
            jsel = 1;

         surfselectflag[i] = jsel;
        }

      /*  Now check search sphere, if requested.  */
      isel = (int)surfselectflag[i];
      if (surfselsph_on && isel)
        {
         jsel = 0;

         xd = (double)surfs[isurf].xavg - (double)surfselsph_x;
         yd = (double)surfs[isurf].yavg - (double)surfselsph_y;
         zd = (double)surfs[isurf].zavg - (double)surfselsph_z;
         dist = xd*xd + yd*yd + zd*zd;
         if (surfselsph_in == 1 && dist <= rad)
            jsel = 1;
         if (surfselsph_in == 0 && dist >= rad)
            jsel = 1;

         surfselectflag[i] = jsel;
        }

      /*  Now check search box, if requested.  */
      isel = (int)surfselectflag[i];
      if (surfselbox_on && isel)
        {
         jsel = 0;

         xb = surfs[isurf].xavg;
         yb = surfs[isurf].yavg;
         zb = surfs[isurf].zavg;

         if (surfselbox_in == 1)
           {
            if (surfselbox_xmin <= xb && xb <= surfselbox_xmax &&
                surfselbox_ymin <= yb && yb <= surfselbox_ymax &&
                surfselbox_zmin <= zb && zb <= surfselbox_zmax)
              jsel = 1;
           }
         if (surfselbox_in == 0)
           {
            if (xb <= surfselbox_xmin || surfselbox_xmax <= xb ||
                yb <= surfselbox_ymin || surfselbox_ymax <= yb ||
                zb <= surfselbox_zmin || surfselbox_zmax <= zb)
               jsel = 1;
           }

         surfselectflag[i] = jsel;
        }
    }

   if (surfselsfld_on) free(svalue);
   if (surfselnfld_on) free(nvalue);

   /*  Check surface number selection, and with  */
   /*  current select flag for those surfaces.   */
   if (surfselnum_on)
     {

      /*  Loop through surfaces, and get a surface id.  */
      for (ii = 0; ii < nsurf; ii++)
        {
         id = ii + 1;
         if (surfidflag) id = surfids[ii];

         /*  Loop through active first, last, stride sets.  */
         for (j = 0; j < nsurfselnum; j++)
           {
            ifirst = surfselnum_f[j];
            ilast = surfselnum_l[j];
            istride = surfselnum_s[j];
            if (ifirst <= id && id <= ilast)
              {
               for (i = ifirst; i <= ilast; i+=istride)
                 {
                  if (i == id)
                    {
                     isel = (int)surfselectflag[ii];

                     /*  If previously selected, set value to 2.  */
                     if (isel) surfselectflag[ii] = 2;
                    }
                 }
              }
           }
        }

      /*  Now loop through surfaces, and keep those with a 2.  */
      for (i = 0; i < nsurf; i++)
        {
         jsel = 0;
         isel = (int)surfselectflag[i];
         if (isel == 2) jsel = 1;
         surfselectflag[i] = jsel;
        }
     }

   /*  Check surface group selection, and with   */
   /*  current select flag for those surfaces.   */
   if (surfselsurfgrp_on && nsurfgrpselected > 0)
     {

      /*  Loop through surfaces, and get a surface id.  */
      for (ii = 0; ii < nsurf; ii++)
        {
         id = ii + 1;
         if (surfidflag) id = surfids[ii];

         /*  If previously selected, loop through selected  */ 
         /*  surface groups and set selected surface to 2.  */
         isel = (int)surfselectflag[ii];
         if (isel)
           {
            if (surfselgrp_in == 0) surfselectflag[ii] = 2;
            for (j = 0; j < nsurfgrpselected; j++)
              {
               igrp = surfgrpselected[j] - 1;
               k = numsurfacegrp[igrp];
               for (i = 0; i < k; i++)
                 {
                  jj = surfacegrps[igrp][i];
                  if (jj == id)
                    {
                     if (surfselgrp_in == 1) surfselectflag[ii] = 2;
                     if (surfselgrp_in == 0) surfselectflag[ii] = 0;
                    }
                 }
              }
           }
        }

      /*  Now loop through surfaces, and keep those with a 2.  */
      for (i = 0; i < nsurf; i++)
        {
         jsel = 0;
         isel = (int)surfselectflag[i];
         if (isel == 2) jsel = 1;
         surfselectflag[i] = jsel;
        }
     }
}


void surfselgetsfield(int i)
{
  char str[20];
  XmString string;

   surfselfield = i;

   string = XmStringCreate(sfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselsfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",sfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselsminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",sfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselsmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sfldselmin = sfieldmin[i];
   sfldselmax = sfieldmax[i];
   sprintf(str,"%.11g",sfieldmin[i]);
   XmTextFieldSetString(sselsmintxt,str);
   sprintf(str,"%.11g",sfieldmax[i]);
   XmTextFieldSetString(sselsmaxtxt,str);
}


void surfselgetnfield(int i)
{
  char str[20];
  XmString string;

   surfselnfield = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselnminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(sselnmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   snfldselmin = fieldmin[i];
   snfldselmax = fieldmax[i];
   sprintf(str,"%.11g",fieldmin[i]);
   XmTextFieldSetString(sselnmintxt,str);
   sprintf(str,"%.11g",fieldmax[i]);
   XmTextFieldSetString(sselnmaxtxt,str);
}


void setsurfselboxwidgets()
{
  int ival;
  float val;
  char str[20];

   XtRemoveCallback(sselboxx1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 0);
   XtRemoveCallback(sselboxx2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 1);
   XtRemoveCallback(sselboxy1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 2);
   XtRemoveCallback(sselboxy2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 3);
   XtRemoveCallback(sselboxz1sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 4);
   XtRemoveCallback(sselboxz2sl,XmNvalueChangedCallback,ssboxsubsetsl,
                    (caddr_t) 5);
   XFlush(dpy);

   sprintf(str,"%.11g",surfselbox_xmin);
   XmTextFieldSetString(sselboxx1txt,str);
   val = (surfselbox_xmin - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxx1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",surfselbox_xmax);
   XmTextFieldSetString(sselboxx2txt,str);
   val = (surfselbox_xmax - globxicmin) / (globxicmax - globxicmin);;
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxx2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",surfselbox_ymin);
   XmTextFieldSetString(sselboxy1txt,str);
   val = (surfselbox_ymin - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxy1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",surfselbox_ymax);
   XmTextFieldSetString(sselboxy2txt,str);
   val = (surfselbox_ymax - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxy2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",surfselbox_zmin);
   XmTextFieldSetString(sselboxz1txt,str);
   val = (surfselbox_zmin - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxz1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",surfselbox_zmax);
   XmTextFieldSetString(sselboxz2txt,str);
   val = (surfselbox_zmax - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(sselboxz2sl, XmNvalue,ival, NULL);

   XFlush(dpy);
   XtAddCallback(sselboxx1sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 0);
   XtAddCallback(sselboxx2sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 1);
   XtAddCallback(sselboxy1sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 2);
   XtAddCallback(sselboxy2sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 3);
   XtAddCallback(sselboxz1sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 4);
   XtAddCallback(sselboxz2sl,XmNvalueChangedCallback,ssboxsubsetsl,(caddr_t) 5);
   XFlush(dpy);
}


void setsurfselwidgets()
{
  char str[20];

   setsurfselboxwidgets();

   sprintf(str,"%.11g",surfselsph_x);
   XmTextFieldSetString(sselsphxtxt,str);

   sprintf(str,"%.11g",surfselsph_y);
   XmTextFieldSetString(sselsphytxt,str);

   sprintf(str,"%.11g",surfselsph_z);
   XmTextFieldSetString(sselsphztxt,str);

   sprintf(str,"%.11g",surfselsph_r);
   XmTextFieldSetString(sselsphrtxt,str);
}

void addsurfselgrplist(int i)
{
  XmString item;

   item = XmStringLtoRCreate(surfacegrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(surfgrplist,item,0);
}

void setsurfselgrplist(int i)
{
  XmString item[2];

   item[0] = XmStringLtoRCreate(surfacegrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(surfgrplist,item,1,i+1);
}

