
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "cutplane.h"
#include "SciPlot.h"

#define MAXVAL 9.99999e32

static Widget clx1, cly1, clz1, clx2, cly2, clz2, waveht, clerror,
              wavebut, clradius, clnodes, cutlineform2,
              clsel[MAXCUTLINES], clmsg, clon, clnfldbut, clnfldnew,
              clnfldlab, clcfldbut, clcfldnew, clcfldlab, clnums,
              plot2dform, plot2d, clnodesform, clnode1, clnode2,
              clcellsform, clcell1, clcell2;
static float xl1, yl1, zl1, xl2, yl2, zl2, cldist, clwfac = 0.;
static int currcl = 0, clwflag = 0, clnodeson = 0, clnumson = 0,
           currclnfld, currclcfld;
static int blackno, redno, blueno, greenno, plot2d_up, line[20],
           lcolor[20], marker[20], xlogflag = 0, ylogflag = 0;
static char clsavfilnam[256];
void drawplot2d();


void cutlineformpop()
{
  int i, j, iplot;
  char str4[80];
  XmString string;

   iplot = 0;
   for (i = 0; i < MAXCUTLINES; i++)
     {
      if (cutlineon[i]) iplot = 1;
      if (ncutlinepts[i] > 0)
        {
         j = cutlinefield[i];
         if (j < numvars)
            sprintf(str4,"%2d node %s",i+1,fieldname[j]);
         else
            sprintf(str4,"%2d cell %s",i+1,cfieldname[j-numvars]);
        }
      else
         sprintf(str4,"%2d NONE",i+1);
     
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(clsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   XtSetSensitive(clnfldbut,TRUE);
   XtSetSensitive(clnfldlab,TRUE);
   XtSetSensitive(clnfldnew,TRUE);
   if (numvars < 1)
     {
      XtSetSensitive(clnfldbut,FALSE);
      XtSetSensitive(clnfldlab,FALSE);
      XtSetSensitive(clnfldnew,FALSE);
     }
   XtSetSensitive(clcfldbut,TRUE);
   XtSetSensitive(clcfldlab,TRUE);
   XtSetSensitive(clcfldnew,TRUE);
   if (cnumvars < 1)
     {
      XtSetSensitive(clcfldbut,FALSE);
      XtSetSensitive(clcfldlab,FALSE);
      XtSetSensitive(clcfldnew,FALSE);
     }

   MyManageChild(cutlineform);

   if (iplot == 0) plot2d_up = 0;
   if (iplot == 1) drawplot2d();
}


void cutlineformpop2()
{
  int ival, i, cpon;
  float val1, val2;
  char str[20], str4[55];
  XmString string;

   cutlinefieldactive = 0;
   if (ncutlinepts[currcl] > 0)
     {
      clwfac = wavefac[currcl];
      clwflag = waveflag[currcl];
      clnodeson = cutlinenodeson[currcl];
      clnumson = cutlinenumson[currcl];
      xl1 = ctlinx1[currcl];  xl2 = ctlinx2[currcl];
      yl1 = ctliny1[currcl];  yl2 = ctliny2[currcl];
      zl1 = ctlinz1[currcl];  zl2 = ctlinz2[currcl];
      cutlinefieldactive = cutlinefield[currcl];
      sprintf(str,"%11.7g",xl1);
      XtVaSetValues(clx1,XmNvalue,str,NULL);
      sprintf(str,"%11.7g",xl2);
      XtVaSetValues(clx2,XmNvalue,str,NULL);
      sprintf(str,"%11.7g",yl1);
      XtVaSetValues(cly1,XmNvalue,str,NULL);
      sprintf(str,"%11.7g",yl2);
      XtVaSetValues(cly2,XmNvalue,str,NULL);
      sprintf(str,"%11.7g",zl1);
      XtVaSetValues(clz1,XmNvalue,str,NULL);
      sprintf(str,"%11.7g",zl2);
      XtVaSetValues(clz2,XmNvalue,str,NULL);
     }

   ival = (int) (clwfac * 100.);
   XmScaleSetValue(waveht,ival);
   XmToggleButtonGadgetSetState(wavebut,clwflag,FALSE);
   XmToggleButtonGadgetSetState(clnodes,clnodeson,FALSE);
   XmToggleButtonGadgetSetState(clnums,clnumson,FALSE);
   XmToggleButtonGadgetSetState(clon,cutlineon[currcl],FALSE);
   cpon = 0;
   for (i = 0; i < MAXCUTPLANES; i++)
      cpon += cutplane_on[i];
   if (cpon == 0) XtSetSensitive(ctln2pt,FALSE);
   else XtSetSensitive(ctln2pt,TRUE);

   XmToggleButtonGadgetSetState(clnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clcfldbut,FALSE,FALSE);
   if (cutlinefieldactive < numvars)
     {
      XmToggleButtonGadgetSetState(clnfldbut,TRUE,FALSE);
      currclnfld = cutlinefieldactive;
     }
   else
     {
      XmToggleButtonGadgetSetState(clcfldbut,TRUE,FALSE);
      currclcfld = cutlinefieldactive - numvars;
     }

   string = XmStringCreate(fieldname[currclnfld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (cnumvars > 0)
     {
      string = XmStringCreate(cfieldname[currclcfld],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(clcfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   cldist = cutlineradius[currcl];
   sprintf(str,"%11.7g",cldist);
   XtVaSetValues(clradius,XmNvalue,str,NULL);

   sprintf(str4," 0 nodes in cutline %d",currcl+1);
   if (ncutlinepts[currcl] > 0)
     {
      val1 = 0.;  val2=0.;
      val1 = MAXVAL;  val2 = -MAXVAL;
      for (i = 0; i < ncutlinepts[currcl]; i++)
        {
         val1 = (val1 < cutlineval[currcl][i]) ?
                 val1 : cutlineval[currcl][i];
         val2 = (val2 > cutlineval[currcl][i]) ?
                 val2 : cutlineval[currcl][i];
        }
      sprintf(str4,"%d nodes in cutline %d, min=%6.4g, max=%6.4g",
              ncutlinepts[currcl],currcl+1,val1,val2);
     }
   string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clmsg,XmNlabelString,string,NULL);
   XmStringFree(string);

   MyManageChild(cutlineform2);
}


void cutline_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;
   currcl = i;

   cutlineformpop2();
}


void clfldtypesel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(clnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clcfldbut,FALSE,FALSE);

   if (arg == 0)
      XmToggleButtonGadgetSetState(clnfldbut,TRUE,FALSE);

   if (arg == 1)
      XmToggleButtonGadgetSetState(clcfldbut,TRUE,FALSE);
}


void clnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currclnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDNCUTLINE, ifld);
}


void clcfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currclcfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDCCUTLINE, ifld);
}


void clswitches(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      clwflag = (int)XmToggleButtonGadgetGetState(w);
      waveflag[currcl] = clwflag;
     }
   if (arg == 1)
     {
      clnodeson = (int)XmToggleButtonGadgetGetState(w);
      cutlinenodeson[currcl] = clnodeson;
     }
   if (arg == 2)
     {
      cutlineon[currcl] = (int)XmToggleButtonGadgetGetState(w);
      /* drawplot2d(); */
     }
   if (arg == 3)
     {
      clnumson = (int)XmToggleButtonGadgetGetState(w);
      cutlinenumson[currcl] = clnumson;
     }

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


void cldelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str4[55];
  XmString string;

   cutlineon[currcl] = 0;
   if (ncutlinepts[currcl] > 0)
     {
      FREE(cutlineval[currcl]);
      FREE(cutlinedist[currcl]);
      FREE(cutlinenodes[currcl]);
      ncutlinepts[currcl] = 0;
     }
   XmToggleButtonGadgetSetState(clon,cutlineon[currcl],FALSE);
   sprintf(str4," 0 nodes in cutline %d",currcl+1);
   string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clmsg,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str4,"%2d NONE",currcl+1);
   string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clsel[currcl],XmNlabelString,string,NULL);
   XmStringFree(string);

   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();

   SciPlotListDelete(plot2d,line[currcl]);
   SciPlotUpdate(plot2d);
   line[currcl] = -1;
   drawplot2d();
}


void closecutline(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      XtUnmanageChild(cutlineform);
      XtUnmanageChild(plot2dform);
      plot2d_up = 0;
     }
   if (arg == 1)
     {
      XtUnmanageChild(nfieldform);
      if (cnumvars > 0)
         XtUnmanageChild(cfieldform);
      XtUnmanageChild(clnodesform);
      XtUnmanageChild(clcellsform);
      XtUnmanageChild(cutlineform2);
     }
   if (arg == 2)
     {
      XtUnmanageChild(clnodesform);
     }
   if (arg == 3)
     {
      XtUnmanageChild(clcellsform);
     }
}


void doitcutline1(pa,pb,pc,pd,pe,pf)
 GLfloat pa,pb,pc,pd,pe,pf;
{
  char str[10];

   /*  Set the points.  */
   xl1 = (float)pa;  yl1 = (float)pb;  zl1 = (float)pc;
   xl2 = (float)pd;  yl2 = (float)pe;  zl2 = (float)pf;
 
   XmToggleButtonGadgetSetState(ctln1pt,FALSE,FALSE);
   sprintf(str,"%11.7g",xl1);
   XtVaSetValues(clx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",xl2);
   XtVaSetValues(clx2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",yl1);
   XtVaSetValues(cly1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",yl2);
   XtVaSetValues(cly2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",zl1);
   XtVaSetValues(clz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",zl2);
   XtVaSetValues(clz2,XmNvalue,str,NULL);
}



void doitcutline2(pa,pb,pc,pd,pe,pf, pg,ph,pi,pj,pk,pl)
 GLfloat pa,pb,pc,pd,pe,pf,pg,ph,pi,pj,pk,pl;
{
  double xp1, yp1, zp1, xp2, yp2, zp2, xp3, yp3, zp3, 
         xp4, yp4, zp4;
  double a, b, c, d, denom, epsln, sf, minsf;
  int i, icp;
  char str[10];

   /*  Make a cutline from 2 points projected normal to   */
   /*  the screen and intersecting the closest cutplane.  */
   xp1 = (double)pa; yp1 = (double)pb; zp1 = (double)pc;
   xp2 = (double)pd; yp2 = (double)pe; zp2 = (double)pf;
   xp3 = (double)pg; yp3 = (double)ph; zp3 = (double)pi;
   xp4 = (double)pj; yp4 = (double)pk; zp4 = (double)pl;
   a = globxicmax - globxicmin;
   b = globyicmax - globyicmin;
   c = globzicmax - globzicmin;
   epsln = myzero;

   minsf = 1.0e20;
   icp = -1;
   for (i = 0; i < MAXCUTPLANES; i++)
     {
      if (cutplane_on[i])
        {
         a = cutplane[i].aa;  b = cutplane[i].bb;
         c = cutplane[i].cc;  d = -cutplane[i].dd;

         /*  Find intersection point with first line.  */
         denom = a*(xp2-xp1) + b*(yp2-yp1) + c*(zp2-zp1);
         sf = -1.0e20;
         if (fabs(denom) > epsln)
            sf = (d - a*xp1 - b*yp1 - c*zp1) / denom;

         if (sf >= 0. && sf <= 1. && sf < minsf)
           {
            minsf = sf;
            icp = i;
           }
        }
     }

   /*  Get the intersection point for the first line.  */
   a = cutplane[icp].aa;  b = cutplane[icp].bb;
   c = cutplane[icp].cc;  d = -cutplane[icp].dd;
   sf = minsf;
   xl1 = sf*xp2 + xp1*(1.0 - sf);
   yl1 = sf*yp2 + yp1*(1.0 - sf);
   zl1 = sf*zp2 + zp1*(1.0 - sf);

   /*  Find intersection point with second line.  */
   denom = a*(xp4-xp3) + b*(yp4-yp3) + c*(zp4-zp3);
   sf = -1.0e20;
   if (fabs(denom) > epsln)
      sf = (d - a*xp3 - b*yp3 - c*zp3) / denom;
   xl2 = sf*xp4 + xp3*(1.0 - sf);
   yl2 = sf*yp4 + yp3*(1.0 - sf);
   zl2 = sf*zp4 + zp3*(1.0 - sf);

   XmToggleButtonGadgetSetState(ctln2pt,FALSE,FALSE);
   sprintf(str,"%11.7g",xl1);
   XtVaSetValues(clx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",xl2);
   XtVaSetValues(clx2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",yl1);
   XtVaSetValues(cly1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",yl2);
   XtVaSetValues(cly2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",zl1);
   XtVaSetValues(clz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",zl2);
   XtVaSetValues(clz2,XmNvalue,str,NULL);
}


void doitcutline(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float vlen, val1, val2;
  int i, k, nclpts, nfld, cfld, icell, jcell;
  char *tmpstr;
  static char str[55] = 
           "Error, you don't have 2 distinct points.";
  static char str2[55] = 
           "                                        ";
  static char str3[55] = 
           "Error, search radius must be > 0.       ";
  char str4[80];
  XmString string;
  double *value;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);

   /*  Read the 2 points.  */
   tmpstr = XmTextFieldGetString(clx1);  xl1 = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(cly1);  yl1 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(clz1);  zl1 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(clx2);  xl2 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(cly2);  yl2 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(clz2);  zl2 = atof(tmpstr); 
   XtFree(tmpstr);

   /*  Determine the current field (nodes  */
   /*  or cells) from toggle buttons.      */
   nfld = (int)XmToggleButtonGadgetGetState(clnfldbut);
   cfld = (int)XmToggleButtonGadgetGetState(clcfldbut);
   if (nfld) cutlinefieldactive = currclnfld;
   if (cfld) cutlinefieldactive = currclcfld + numvars;

   tmpstr = XmTextFieldGetString(clradius);  cldist = atof(tmpstr); 
   XtFree(tmpstr);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   vlen = (xl2-xl1)*(xl2-xl1) + (yl2-yl1)*(yl2-yl1) +
          (zl2-zl1)*(zl2-zl1);

   if (vlen == 0. || cldist <= 0.)
     {
      if (cldist <= 0.)
         string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
      if (vlen == 0.)
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(clerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   else 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);
  
      /*  Get values for the points along cutline.  */
      if (cutlinefieldactive < numvars)
         value = readnodevalue(currclnfld);
      else
         value = readcellvalue(currclcfld);

      makecutline(xl1,yl1,zl1,xl2,yl2,zl2,cldist,cutlinefieldactive,
                  value,&nclpts);
      free(value);

      /*  Fill cutline arrays from makecutline arrays.  */
      val1 = 0.;  val2=0.;
      if (nclpts > 0)
        {
         cutlineon[currcl] = 1;
         cutlinefield[currcl] = cutlinefieldactive;
         if (ncutlinepts[currcl] > 0)
           {
            FREE(cutlineval[currcl]);
            FREE(cutlinedist[currcl]);
            FREE(cutlinenodes[currcl]);
            FREE(cutlinenodesm[currcl]);
            FREE(cutlinenodesx[currcl]);
            FREE(cutlinenodesy[currcl]);
            FREE(cutlinenodesz[currcl]);
           }
         ncutlinepts[currcl] = nclpts;
         cutlineval[currcl] = (float *)malloc(nclpts*sizeof(float));
         cutlinedist[currcl] = (float *)malloc(nclpts*sizeof(float));
         cutlinenodes[currcl] = (int *)malloc(nclpts*sizeof(int));
         cutlinenodesm[currcl] = (int *)malloc(nclpts*sizeof(int));
         cutlinenodesx[currcl] = (float *)malloc(nclpts*sizeof(float));
         cutlinenodesy[currcl] = (float *)malloc(nclpts*sizeof(float));
         cutlinenodesz[currcl] = (float *)malloc(nclpts*sizeof(float));
         if (cutlineval[currcl] == NULL || cutlinedist[currcl] == NULL ||
             cutlinenodes[currcl] == NULL || cutlinenodesm[currcl] == NULL ||
             cutlinenodesx[currcl] == NULL || cutlinenodesy[currcl] == NULL ||
             cutlinenodesz[currcl] == NULL)
           {
            printf("Cutline error, can't allocate any more memory.\n");
            gmvexit();
           }

         val1 = MAXVAL;  val2 = -MAXVAL;
         for (i = 0; i < nclpts; i++)
           {
            cutlineval[currcl][i] = cutval[i];
            cutlinedist[currcl][i] = cutlist[i];
            k = cutnodes[i];
            if (cutlinefield[currcl] < numvars)
              {
               cutlinenodesm[currcl][i] = imat[k] - 1;
               cutlinenodesx[currcl][i] = nodes[k].x;
               cutlinenodesy[currcl][i] = nodes[k].y;
               cutlinenodesz[currcl][i] = nodes[k].z;
               cutlinenodes[currcl][i] = k;
               if (nodeidflag) cutlinenodes[currcl][i] = nodeids[k];
              }
            else
              {
               cutlinenodesm[currcl][i] = cellmats[k] - 1;
               icell = k;
               jcell = k;
               if (structuredflag == 1)
                 {
                  jcell = 0;
                  fillstructcell(icell,0);
                 }
               cutlinenodesx[currcl][i] = cells[jcell].xavg;
               cutlinenodesy[currcl][i] = cells[jcell].xavg;
               cutlinenodesz[currcl][i] = cells[jcell].xavg;
               cutlinenodes[currcl][i] = k + 1;
               if (cellidflag) cutlinenodes[currcl][i] = cellids[k];
              }
            if (cutlinenodesm[currcl][i] < 0 || 
                cutlinenodesm[currcl][i] > MAXMATS)
               cutlinenodesm[currcl][i] = -1;
            val1 = (val1 < cutval[i]) ? val1 : cutval[i];
            val2 = (val2 > cutval[i]) ? val2 : cutval[i];
           }
         ctlinx1[currcl] = xl1;  ctliny1[currcl] = yl1;
         ctlinz1[currcl] = zl1;  ctlinx2[currcl] = xl2;
         ctliny2[currcl] = yl2;  ctlinz2[currcl] = zl2;
         wavefac[currcl] = clwfac;
         waveflag[currcl] = clwflag;
         cutlinenodeson[currcl] = clnodeson;
         cutlinenumson[currcl] = clnumson;
         cutlineradius[currcl] = cldist;
         if (cutlinefieldactive < numvars)
            sprintf(str4,"%2d node %s",currcl+1,
                    fieldname[cutlinefieldactive]);
         else
            sprintf(str4,"%2d cell %s",currcl+1,
                    cfieldname[cutlinefieldactive-numvars]);
         string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(clsel[currcl],XmNlabelString,string,NULL);
         XmStringFree(string);
         free(cutnodes);  free(cutlist);  free(cutval);
         if (displistflag) drawnewlistflag = 1;
        }
      sprintf(str4,"%d points in cutline %d, min=%6.4g, max=%6.4g",
              nclpts,currcl+1,val1,val2);
      string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(clmsg,XmNlabelString,string,NULL);
      XmStringFree(string);
      XUndefineCursor(dpy,topwin);
      XmToggleButtonGadgetSetState(clon,cutlineon[currcl],FALSE);
     }

   XmUpdateDisplay(clmsg);
   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();

   /* drawplot2d(); */
}


void waveheight(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;

   XmScaleGetValue(waveht,&ival);
   clwfac = (float)ival/100.;
   wavefac[currcl] = clwfac;
   arg = (long int)client_data;
   if (waveflag[currcl])
     {
      if (displistflag) drawnewlistflag = 1;
      if (arg == 0) fastdraw = 0;
      if (arg == 1) fastdraw = 1;
      drawit();
     }
}


void clwrite(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string, string2, string3;

   fileselflag = CUTLINEOUT;
   string = XmStringCreate("Write Cutline to a File",
                           XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[CUTLINEOUT],
                           XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[CUTLINEOUT],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                           XmNpattern, string2,
                           XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);

      if (fsread[CUTLINEOUT])
        {
         string = XmStringCreate(clsavfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

   MyManageChild(file_sel2);
}


void clwritefile(char *fname) 
{
  FILE *out;
  int i, ic, k;
  float dist, x1, y1, z1, x2, y2, z2, s, firstdist;
  double totdist, dx, dy, dz;
  char str[] = 
    "Error - unable to open output file!\nCheck permissions.";
  XmString string;

   strcpy(clsavfilnam,fname);

   /*  Open file and save cutline data.  */
   if ((out = fopen(clsavfilnam,"w")) == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      return;
     }
   fprintf(out,"GMV cutline data\n");
   for (ic = 0; ic < MAXCUTLINES; ic++)
     {
      if (ncutlinepts[ic] > 0)
        {
         k = cutlinefield[ic];
         dx = (double)ctlinx2[ic] - (double)ctlinx1[ic];
         dy = (double)ctliny2[ic] - (double)ctliny1[ic];
         dz = (double)ctlinz2[ic] - (double)ctlinz1[ic];
         totdist = sqrt(dx*dx + dy*dy + dz*dz);
         s = cutlinedist[ic][0];
         x1 = ctlinx2[ic]*s + ctlinx1[ic]*(1.0 - s);
         y1 = ctliny2[ic]*s + ctliny1[ic]*(1.0 - s);
         z1 = ctlinz2[ic]*s + ctlinz1[ic]*(1.0 - s);
         firstdist = s * totdist;
         i = ncutlinepts[ic] - 1;
         s = cutlinedist[ic][i];
         x2 = ctlinx2[ic]*s + ctlinx1[ic]*(1.0 - s);
         y2 = ctliny2[ic]*s + ctliny1[ic]*(1.0 - s);
         z2 = ctlinz2[ic]*s + ctlinz1[ic]*(1.0 - s);
         if (k < numvars)
            fprintf(out,"node %s for line from %g, %g, %g to %g, %g, %g\n",
                    fieldname[k],x1,y1,z1,x2,y2,z2);
         else
            fprintf(out,"cell %s for line from %g, %g, %g to %g, %g, %g\n",
                    cfieldname[k-numvars],x1,y1,z1,x2,y2,z2);
         for (i = 0; i < ncutlinepts[ic]; i++)
           {
            dist = cutlinedist[ic][i]*totdist - firstdist;
            fprintf(out,"%e %e\n",dist,cutlineval[ic][i]);
           }
        }
     }

   fclose(out);
}


void cl2nodespop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(clnodesform);
}

void clnodes_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int n1, n2;
  char str[20];
  char *tmpstr;

   /*  Read the nodes.  */
   tmpstr = XmTextFieldGetString(clnode1);
   n1 = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(clnode2);
   n2 = atof(tmpstr);
   XtFree(tmpstr);

   if (n1 < 1) n1 = 1;
   if (n1 > nnodes) n1 = nnodes;
   if (n2 < 1) n2 = 1;
   if (n2 > nnodes) n2 = nnodes;

   /*  Set the points.  */
   xl1 = nodes[n1].x;  yl1 = nodes[n1].y;  zl1 = nodes[n1].z;
   xl2 = nodes[n2].x;  yl2 = nodes[n2].y;  zl2 = nodes[n2].z;
 
   sprintf(str,"%11.7g",xl1);
   XtVaSetValues(clx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",xl2);
   XtVaSetValues(clx2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",yl1);
   XtVaSetValues(cly1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",yl2);
   XtVaSetValues(cly2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",zl1);
   XtVaSetValues(clz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",zl2);
   XtVaSetValues(clz2,XmNvalue,str,NULL);

   XtUnmanageChild(clnodesform);
}


void cl2cellspop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   MyManageChild(clcellsform);
}

void clcells_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int n1, n2, i, j;
  char str[20];
  char *tmpstr;

   /*  Read the cells.  */
   tmpstr = XmTextFieldGetString(clcell1);
   n1 = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(clcell2);
   n2 = atof(tmpstr);
   XtFree(tmpstr);

   /*  Set the points.  */

   i = n1 - 1;
   if (i < 0) i = 0;
   if (i >= ncells) i = ncells - 1;
   j = i;
   if (structuredflag == 1)
     {
      j = 0;
      fillstructcell(i,0);
     }
   xl1 = cells[j].xavg;  yl1 = cells[j].yavg;  zl1 = cells[j].zavg;

   i = n2 - 1;
   if (i < 0) i = 0;
   if (i >= ncells) i = ncells - 1;
   j = i;
   if (structuredflag == 1)
     {
      j = 0;
      fillstructcell(i,0);
     }
   xl2 = cells[j].xavg;  yl2 = cells[j].yavg;  zl2 = cells[j].zavg;
 
   sprintf(str,"%11.7g",xl1);
   XtVaSetValues(clx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",xl2);
   XtVaSetValues(clx2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",yl1);
   XtVaSetValues(cly1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",yl2);
   XtVaSetValues(cly2,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",zl1);
   XtVaSetValues(clz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",zl2);
   XtVaSetValues(clz2,XmNvalue,str,NULL);

   XtUnmanageChild(clcellsform);
}


void setp2dlog(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, logflag;

   arg = (long int)client_data;
   logflag = (int)XmToggleButtonGadgetGetState(w);

   if (arg == 0)
     {
      xlogflag = logflag;
      if (logflag)
         XtVaSetValues(plot2d, XtNxLog,TRUE, NULL);
      else 
         XtVaSetValues(plot2d, XtNxLog,FALSE, NULL);
     }

   if (arg == 1)
     {
      ylogflag = logflag;
      if (logflag)
         XtVaSetValues(plot2d, XtNyLog,TRUE, NULL);
      else
         XtVaSetValues(plot2d, XtNyLog,FALSE, NULL);
     }

   if (xlogflag || ylogflag)
      XtVaSetValues(plot2d, XtNdrawMinor,TRUE, NULL);
   else
      XtVaSetValues(plot2d, XtNdrawMinor,FALSE, NULL);

   SciPlotUpdate(plot2d);
}



void makecutlineform(parent)
 Widget parent;
{
  Widget clclose, clxlab, clylab, clzlab, clp1lab, clp2lab, cldeflab,
         cldistlab, cladd, clsav, cldel, clselwin, clselbox, clclose2,
         clsellab, p2dlogx, p2dlogy, clnodesbut, clclose3, clnapply, 
         clnlab1, clnlab2, clcellsbut, clclose4, clcapply, clclab1,
         clclab2;
  void get_1_point(), get_2_points();
  int i;
  Arg wargs[20];
  char str[20];
  static char str2[55] = 
           "                                         ";
  XmString string;

   string = XmStringCreate("Cutline",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutlineform = XmCreateFormDialog(parent,"Cutlineform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   clclose = XtVaCreateManagedWidget("Clclose",
              xmPushButtonGadgetClass,
              cutlineform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clclose,XmNactivateCallback,closecutline,
                    (caddr_t) 0);

   string = XmStringCreate("Write to File",XmSTRING_DEFAULT_CHARSET);
   clsav = XtVaCreateManagedWidget("Clsav",
           xmPushButtonGadgetClass,
           cutlineform,
           XmNtopAttachment, XmATTACH_FORM,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, clclose,
           XmNleftOffset, 20,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(clsav,XmNactivateCallback,clwrite,NULL);

   string = XmStringCreate("Select Cutline",
                           XmSTRING_DEFAULT_CHARSET);
   clsellab = XtVaCreateManagedWidget("Clsellab",
              xmLabelGadgetClass,
              cutlineform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, clsellab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 250);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   clselwin = XmCreateScrolledWindow(cutlineform,"Clselwin",wargs,i);
   XtManageChild(clselwin);

   i = 0;
   clselbox = XmCreateRadioBox(clselwin,"Clselbox",wargs,i);
   XtManageChild(clselbox);

   for (i = 0; i < MAXCUTLINES; i++)
     {
      sprintf(str,"%2d NONE    ",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Clsel%d",i);
      clsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 clselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(clsel[i],XmNvalueChangedCallback,
                       cutline_select,
                       (caddr_t) i);
     }

   /*  Create cutline build form.  */

   string = XmStringCreate("Selected Cutline",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutlineform2 = XmCreateFormDialog(cutlineform,"Cutlineform2",wargs,i);
      XmStringFree(string);

   waveht = XtVaCreateManagedWidget("Waveht",
               xmScaleWidgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNminimum, -100,
               XmNmaximum, 100,
               XmNorientation, XmVERTICAL,
               XmNprocessingDirection, XmMAX_ON_TOP,
               XmNvalue, 0,
               XmNscaleHeight, 150,
               XmNscaleMultiple, 1,
               XmNshowValue, TRUE,
               NULL);
      XtAddCallback(waveht,XmNvalueChangedCallback,
                    waveheight,(caddr_t) 0);
      XtAddCallback(waveht,XmNdragCallback,waveheight,(caddr_t) 1);
 
   string = XmStringCreate("Wave ",XmSTRING_DEFAULT_CHARSET);
   wavebut = XtVaCreateManagedWidget("Wavebut",
             xmToggleButtonGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, waveht,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(wavebut,XmNvalueChangedCallback,clswitches,
                    (caddr_t) 0);

   string = XmStringCreate("1 Point",XmSTRING_DEFAULT_CHARSET);
   ctln1pt = XtVaCreateManagedWidget("Ctln1pt",
             xmToggleButtonGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, wavebut,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ctln1pt,XmNvalueChangedCallback,
                    get_1_point,(caddr_t) 1);
 
   string = XmStringCreate("2 Points on Cutplane",
                           XmSTRING_DEFAULT_CHARSET);
   ctln2pt = XtVaCreateManagedWidget("Ctln2pt",
             xmToggleButtonGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, ctln1pt,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(ctln2pt,XmNvalueChangedCallback,
                    get_2_points,(caddr_t) 1);
 
   string = XmStringCreate("2 Nodes",XmSTRING_DEFAULT_CHARSET);
   clnodesbut = XtVaCreateManagedWidget("Clnodesbut",
                xmPushButtonGadgetClass,
                cutlineform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ctln1pt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, wavebut,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(clnodesbut,XmNactivateCallback,cl2nodespop,NULL);
 
   string = XmStringCreate("2 Cells",XmSTRING_DEFAULT_CHARSET);
   clcellsbut = XtVaCreateManagedWidget("Clcellsbut",
                xmPushButtonGadgetClass,
                cutlineform2,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ctln1pt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, clnodesbut,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(clcellsbut,XmNactivateCallback,cl2cellspop,NULL);

   string = XmStringCreate("     X     ",XmSTRING_DEFAULT_CHARSET);
   clxlab = XtVaCreateManagedWidget("Clxlab",
            xmLabelGadgetClass,
            cutlineform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, clnodesbut,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, wavebut,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   string = XmStringCreate("     Y     ",XmSTRING_DEFAULT_CHARSET);
   clylab = XtVaCreateManagedWidget("Clylab",
          xmLabelGadgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clnodesbut,
          XmNtopOffset, 10,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, clxlab,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);
   string = XmStringCreate("     Z     ",XmSTRING_DEFAULT_CHARSET);
   clzlab = XtVaCreateManagedWidget("Clzlab",
          xmLabelGadgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clnodesbut,
          XmNtopOffset, 10,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, clylab,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   clx1 = XtVaCreateManagedWidget("Clx1",
          xmTextFieldWidgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clxlab,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, wavebut,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNmarginHeight, 0,
          XmNmarginWidth, 1,
          XmNmaxLength, 20,
          XmNcolumns, 11,
          XmNvalue, "",
          NULL);
   cly1 = XtVaCreateManagedWidget("Cly1",
          xmTextFieldWidgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clxlab,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, clx1,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNmarginHeight, 0,
          XmNmarginWidth, 1,
          XmNmaxLength, 20,
          XmNcolumns, 11,
          XmNvalue, "",
          NULL);
   clz1 = XtVaCreateManagedWidget("Clz1",
          xmTextFieldWidgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clxlab,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, cly1,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNmarginHeight, 0,
          XmNmarginWidth, 1,
          XmNmaxLength, 20,
          XmNcolumns, 11,
          XmNvalue, "",
          NULL);
   string = XmStringCreate("  P1",XmSTRING_DEFAULT_CHARSET);
   clp1lab = XtVaCreateManagedWidget("Clp1lab",
             xmLabelGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clxlab,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clz1,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   clx2 = XtVaCreateManagedWidget("Clx2",
          xmTextFieldWidgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clx1,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, wavebut,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNmarginHeight, 0,
          XmNmarginWidth, 1,
          XmNmaxLength, 20,
          XmNcolumns, 11,
          XmNvalue, "",
          NULL);
   cly2 = XtVaCreateManagedWidget("Cly2",
          xmTextFieldWidgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clx1,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, clx2,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNmarginHeight, 0,
          XmNmarginWidth, 1,
          XmNmaxLength, 20,
          XmNcolumns, 11,
          XmNvalue, "",
          NULL);
   clz2 = XtVaCreateManagedWidget("Clz2",
            xmTextFieldWidgetClass,
            cutlineform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, clx1,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, cly2,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
   string = XmStringCreate("  P2",XmSTRING_DEFAULT_CHARSET);
   clp2lab = XtVaCreateManagedWidget("Clp2lab",
             xmLabelGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clx1,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clz2,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string =
     XmStringCreate("Define cut line using 2 unique points.",
                     XmSTRING_DEFAULT_CHARSET);
   cldeflab = XtVaCreateManagedWidget("Cldeflab",
              xmLabelGadgetClass,
              cutlineform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clx2,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, wavebut,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   clerror = XtVaCreateManagedWidget("Clerror",
             xmLabelGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cldeflab,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, wavebut,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   clnodes = XtVaCreateManagedWidget("Clnodes",
             xmToggleButtonGadgetClass,
             cutlineform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, wavebut,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clnodes,XmNvalueChangedCallback,clswitches,
                    (caddr_t) 1);
 
   string = XmStringCreate("Nos.",XmSTRING_DEFAULT_CHARSET);
   clnums = XtVaCreateManagedWidget("Clnodes",
            xmToggleButtonGadgetClass,
            cutlineform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, clnodes,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNspacing, 1,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(clnums,XmNvalueChangedCallback,clswitches,
                    (caddr_t) 3);

   clradius = XtVaCreateManagedWidget("Clradius",
              xmTextFieldWidgetClass,
              cutlineform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clerror,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, wavebut,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 20,
              XmNcolumns, 11,
              XmNvalue, "",
              NULL);

   string = XmStringCreate("Search Radius",XmSTRING_DEFAULT_CHARSET);
   cldistlab = XtVaCreateManagedWidget("Cldistlab",
               xmLabelGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clerror,
               XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clradius,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   cladd = XtVaCreateManagedWidget("Cladd",
           xmPushButtonGadgetClass,
           cutlineform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, clradius,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, wavebut,
           XmNleftOffset, 20,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(cladd,XmNactivateCallback,doitcutline,
                    (caddr_t) 2);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   cldel = XtVaCreateManagedWidget("Cldel",
           xmPushButtonGadgetClass,
           cutlineform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, clradius,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, cladd,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(cldel,XmNactivateCallback,cldelete,NULL);
 
   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   clon = XtVaCreateManagedWidget("Clon",
          xmToggleButtonGadgetClass,
          cutlineform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, clradius,
          XmNtopOffset, 5,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, cldel,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          XmNspacing, 1,
          XmNselectColor, yellowpix,
          NULL);
      XmStringFree(string);
      XtAddCallback(clon,XmNvalueChangedCallback,clswitches,
                    (caddr_t) 2);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   clclose2 = XtVaCreateManagedWidget("Clclose2",
              xmPushButtonGadgetClass,
              cutlineform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clradius,
              XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, clon,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clclose2,XmNactivateCallback,closecutline,
                    (caddr_t) 1);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   clmsg = XtVaCreateManagedWidget("Clmsg",
           xmLabelGadgetClass,
           cutlineform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, cladd,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, wavebut,
           XmNleftOffset, 20,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   string = XmStringCreate("Node Field:",XmSTRING_DEFAULT_CHARSET);
   clnfldbut = XtVaCreateManagedWidget("Clnfldbut",
               xmToggleButtonGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(clnfldbut,XmNvalueChangedCallback,
                    clfldtypesel,(caddr_t) 0);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   clnfldlab = XtVaCreateManagedWidget("Clnfldlab",
               xmLabelGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clnfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   clnfldnew = XtVaCreateManagedWidget("Clnfldnew",
               xmPushButtonGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clnfldlab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(clnfldnew,XmNactivateCallback,clnfldfld,
                    NULL);
      currclnfld = 0;

   string = XmStringCreate("Cell Field:",XmSTRING_DEFAULT_CHARSET);
   clcfldbut = XtVaCreateManagedWidget("Clcfldbut",
               xmToggleButtonGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clnfldnew,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(clcfldbut,XmNvalueChangedCallback,
                    clfldtypesel,(caddr_t) 1);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   clcfldlab = XtVaCreateManagedWidget("Clcfldlab",
               xmLabelGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clcfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   clcfldnew = XtVaCreateManagedWidget("Clcfldnew",
               xmPushButtonGadgetClass,
               cutlineform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clcfldlab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, clp2lab,
               XmNleftOffset, 40,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(clcfldnew,XmNactivateCallback,clcfldfld,
                    NULL);
      currclcfld = 0;


   /*  Create cutline 2 nodes form.  */

   string = XmStringCreate("Cutline 2 Nodes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   clnodesform = XmCreateFormDialog(cutlineform2,"Clnodesform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   clclose3 = XtVaCreateManagedWidget("Clclose3",
              xmPushButtonGadgetClass,
              clnodesform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clclose3,XmNactivateCallback,closecutline,
                    (caddr_t) 2);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   clnapply = XtVaCreateManagedWidget("Clnapply",
              xmPushButtonGadgetClass,
              clnodesform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, clclose3,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clnapply,XmNactivateCallback,clnodes_apply,NULL);

   string = XmStringCreate("Node 1",XmSTRING_DEFAULT_CHARSET);
   clnlab1 = XtVaCreateManagedWidget("Cln1lab",
             xmLabelGadgetClass,
             clnodesform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclose3,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   clnode1 = XtVaCreateManagedWidget("Clnode1",
             xmTextFieldWidgetClass,
             clnodesform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclose3,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clnlab1,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);

   string = XmStringCreate("Node 2",XmSTRING_DEFAULT_CHARSET);
   clnlab2 = XtVaCreateManagedWidget("Cln2lab",
             xmLabelGadgetClass,
             clnodesform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clnlab1,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   clnode2 = XtVaCreateManagedWidget("Clnode2",
             xmTextFieldWidgetClass,
             clnodesform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clnlab1,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clnlab2,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);


   /*  Create cutline 2 cells form.  */

   string = XmStringCreate("Cutline 2 Cells",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   clcellsform = XmCreateFormDialog(cutlineform2,"Clcellsform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   clclose4 = XtVaCreateManagedWidget("Clclose3",
              xmPushButtonGadgetClass,
              clcellsform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clclose4,XmNactivateCallback,closecutline,
                    (caddr_t) 3);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   clcapply = XtVaCreateManagedWidget("Clcapply",
              xmPushButtonGadgetClass,
              clcellsform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, clclose4,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(clcapply,XmNactivateCallback,clcells_apply,NULL);

   string = XmStringCreate("Node 1",XmSTRING_DEFAULT_CHARSET);
   clclab1 = XtVaCreateManagedWidget("Clc1lab",
             xmLabelGadgetClass,
             clcellsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclose4,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   clcell1 = XtVaCreateManagedWidget("Clcell1",
             xmTextFieldWidgetClass,
             clcellsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclose4,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clclab1,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);

   string = XmStringCreate("Node 2",XmSTRING_DEFAULT_CHARSET);
   clclab2 = XtVaCreateManagedWidget("Clc2lab",
             xmLabelGadgetClass,
             clcellsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclab1,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   clcell2 = XtVaCreateManagedWidget("Clcell2",
             xmTextFieldWidgetClass,
             clcellsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clclab1,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, clclab2,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "",
             NULL);


   /*  Create SciPlot plotting widget form.  */

   string = XmStringCreate("Cutlines",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNdialogStyle, XmDIALOG_MODELESS);
   i++; XtSetArg(wargs[i], XmNnoResize, FALSE);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   plot2dform = XmCreateFormDialog(cutlineform,"Plot2dform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Log X",XmSTRING_DEFAULT_CHARSET);
   p2dlogx = XtVaCreateManagedWidget("P2dlogx",
             xmToggleButtonGadgetClass,
             plot2dform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string); 
      XtAddCallback(p2dlogx,XmNvalueChangedCallback,
                    setp2dlog,(caddr_t) 0);

   string = XmStringCreate("Log Y",XmSTRING_DEFAULT_CHARSET);
   p2dlogy = XtVaCreateManagedWidget("P2dlogy",
             xmToggleButtonGadgetClass,
             plot2dform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, p2dlogx,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string); 
      XtAddCallback(p2dlogy,XmNvalueChangedCallback,
                    setp2dlog,(caddr_t) 1);

   plot2d = XtVaCreateManagedWidget("Plot2d",
            sciplotWidgetClass,
            plot2dform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, p2dlogx,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_FORM,
            XmNbottomAttachment, XmATTACH_FORM,
            XtNheight, 250,
            XtNwidth, 300,
            XtNshowTitle, FALSE,
            XtNxLabel, "Line Distance",
            XtNyLabel, "Value",
            XtNbackground, whitepix,
            XtNforeground, blackpix,
            XtNdrawMinor, FALSE,
            XtNdrawMinorTics, FALSE,
            NULL);

   plot2d_up = 0;
   i = SciPlotStoreAllocatedColor(plot2d, redpix);
   redno = SciPlotStoreAllocatedColor(plot2d, redpix);
   blueno = SciPlotStoreAllocatedColor(plot2d, bluepix);
   greenno = SciPlotStoreAllocatedColor(plot2d, greenpix);
   blackno = SciPlotStoreAllocatedColor(plot2d, blackpix);

   /*  Set color and marker for each line.  */
   for (i = 0; i < 20; i+=4)
     {
      lcolor[i] = blackno; 
      lcolor[i+1] = redno;
      lcolor[i+2] = blueno;
      lcolor[i+3] = greenno;
     }
   for (i = 0; i < 4; i++)   marker[i] = XtMARKER_NONE;
   for (i = 4; i < 8; i++)   marker[i] = XtMARKER_CIRCLE;
   for (i = 8; i < 12; i++)  marker[i] = XtMARKER_SQUARE;
   for (i = 8; i < 16; i++)  marker[i] = XtMARKER_UTRIANGLE;
   for (i = 16; i < 20; i++) marker[i] = XtMARKER_DTRIANGLE;
   for (i = 0; i < 20; i++) line[i] = -1;
}


void cutlinegetnfield(int i)
{
  XmString string;

   currclnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void cutlinegetcfield(int i)
{
  XmString string;

   currclcfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(clcfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void drawplot2d()
{
  int iplot, maxpts, ic, i, j, k;
  float *xdist, *yval, dist, s, firstdist;
  double totdist, dx, dy, dz;
  char str[80];

   /*  Check to see if there are any lines to plot.  */
   iplot = 0;
   maxpts = 0;
   for (i = 0; i < MAXCUTLINES; i++)
     {
      if (cutlineon[i]) iplot = 1;
      if (ncutlinepts[i] > 0)
        {
         maxpts = (ncutlinepts[i] > maxpts) ? ncutlinepts[i] : maxpts;
        }
     }

   if (iplot == 0 || maxpts == 0)
     {
      XtUnmanageChild(plot2dform);
      plot2d_up = 0;
      return;
     }

   /*  Pop up the plot2d form if not up already.  */
   if (plot2d_up == 0) MyManageChild(plot2dform);
   plot2d_up = 1;

   /*  Allocate arrays to the largest no. of points.  */
   xdist = (float *)malloc(maxpts*sizeof(float));
   yval = (float *)malloc(maxpts*sizeof(float));

   /*  Loop through cutlines and draw those that are on.  */
   for (ic = 0; ic < MAXCUTLINES; ic++)
     {
      if (cutlineon[ic])
        {

         /*  Generate and fill the x-y arrays.  */
         k = cutlinefield[ic];
         dx = (double)ctlinx2[ic] - (double)ctlinx1[ic];
         dy = (double)ctliny2[ic] - (double)ctliny1[ic];
         dz = (double)ctlinz2[ic] - (double)ctlinz1[ic];
         totdist = sqrt(dx*dx + dy*dy + dz*dz);
         s = cutlinedist[ic][0];
         firstdist = s * totdist;
         j = ncutlinepts[ic];
         for (i = 0; i < j; i++)
           {
            dist = cutlinedist[ic][i]*totdist - firstdist;
            xdist[i] = dist;
            yval[i] = cutlineval[ic][i];
           }
         k = cutlinefield[ic];
         if (k < numvars)
            sprintf(str,"%d - %s",ic+1,fieldname[k]);
         else
            sprintf(str,"%d - %s",ic+1,cfieldname[k-numvars]);
         if (line[ic] > -1)
            SciPlotListDelete(plot2d,line[ic]);
         line[ic] = SciPlotListCreateFloat(plot2d,j,xdist,yval,str);
         SciPlotListSetStyle(plot2d,line[ic],lcolor[ic],marker[ic],
                              lcolor[ic],XtLINE_SOLID);
         SciPlotUpdate(plot2d);
        }
     }
   FREE(xdist);  FREE(yval);
}
