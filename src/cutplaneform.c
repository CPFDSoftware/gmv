
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
#include "lights.h"
#include "xstuff.h"
#include "cutplane.h"

static Widget objx1, objy1, objz1, objx2, objy2, objz2, objx3, objy3,
              objz3, objheight, objval, objcpval, objveldist, objerror,
              htbut, edgebut, cpcobobj, cpccsobj, velbut, nodebut, 
              nnumbut, linebut, cpnfldbut, cpnfldlab, cpcfldbut, 
              cpcfldlab, cvelbut, cellbut, cnumbut, facebut, 
              cpsel[MAXCUTPLANES], cutplaneform2, cponbut, cpopts,
              cpnmatbut, cpcmatbut, cpnfldnew, cpcfldnew; 
static float x1, y1a, z1, x2, y2, z2, x3, y3, z3;
static int currcpnfld, currcpcfld;


void cutplaneformpop()
{
  int i;
  char str[10];
  XmString string;

   XmToggleButtonGadgetSetState(cpnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpcmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpcfldbut,FALSE,FALSE);
   if (cutplanefieldforplane == -1)
     {
      XmToggleButtonGadgetSetState(cpnmatbut,TRUE,FALSE);
      currcpnfld = -1;
     }
   else if (cutplanefieldforplane == -2)
     {
      XmToggleButtonGadgetSetState(cpcmatbut,TRUE,FALSE);
      currcpcfld = -1;
     }
   else if (cutplanefieldforplane < numvars)
     {
      XmToggleButtonGadgetSetState(cpnfldbut,TRUE,FALSE);
      currcpnfld = cutplanefieldforplane;
     }
   else
     {
      XmToggleButtonGadgetSetState(cpcfldbut,TRUE,FALSE);
      currcpcfld = cutplanefieldforplane - numvars;
     }

   string = XmStringCreate(fieldname[currcpnfld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cpnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (cnumvars > 0)
     {
      string = XmStringCreate(cfieldname[currcpcfld],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cpcfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   for (i = 0; i < MAXCUTPLANES; i++)
     {
      if (cutplane[i].npolys > 0)
        {
         if (cutplane_on[i])
            sprintf(str,"%d ON  ",i+1);
         else
            sprintf(str,"%d OFF ",i+1);
        }
      else
         sprintf(str,"%d NONE",i+1);
     
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cpsel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   XtSetSensitive(cpnfldbut,TRUE);
   XtSetSensitive(cpnfldlab,TRUE);
   XtSetSensitive(cpnfldnew,TRUE);
   if (numvars < 1)
     {
      XtSetSensitive(cpnfldbut,FALSE);
      XtSetSensitive(cpnfldlab,FALSE);
      XtSetSensitive(cpnfldnew,FALSE);
     }
   XtSetSensitive(cpcfldbut,TRUE);
   XtSetSensitive(cpcfldlab,TRUE);
   XtSetSensitive(cpcfldnew,TRUE);
   if (cnumvars < 1)
     {
      XtSetSensitive(cpcfldbut,FALSE);
      XtSetSensitive(cpcfldlab,FALSE);
      XtSetSensitive(cpcfldnew,FALSE);
     }

   MyManageChild(cutplaneform);
}


void cutplaneformpop2()
{
  char str[20];
  XmString string;

   sprintf(str,"%11.7g",cutplane[currcp].x1);
   XtVaSetValues(objx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].x2);
   XtVaSetValues(objx2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].x3);
   XtVaSetValues(objx3,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",cutplane[currcp].y1);
   XtVaSetValues(objy1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].y2);
   XtVaSetValues(objy2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].y3);
   XtVaSetValues(objy3,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",cutplane[currcp].z1);
   XtVaSetValues(objz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].z2);
   XtVaSetValues(objz2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",cutplane[currcp].z3);
   XtVaSetValues(objz3,XmNvalue,str,NULL);

   XmToggleButtonGadgetSetState(cponbut,cutplane_on[currcp],FALSE);
   XmToggleButtonGadgetSetState(cpcobobj,
                                cutplane[currcp].cliponbox,FALSE);
   XmToggleButtonGadgetSetState(cpccsobj,
                                cutplane[currcp].cliponcellsel,FALSE);

   if (cutplane[currcp].npolys > 0)
      XtSetSensitive(cpopts,TRUE);
   else
      XtSetSensitive(cpopts,FALSE);

   sprintf(str,"Cutplane %d",currcp+1);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cutplaneform2,XmNdialogTitle,string,NULL);
   XmStringFree(string);

   MyManageChild(cutplaneform2);
}


void cutplane_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;
   currcp = i;

   cutplaneformpop2();
}


void cutplaneoptformpop() 
{
  int ival;

   XmToggleButtonGadgetSetState(cpcobobj,cutplane[currcp].cliponbox,FALSE);
   XmToggleButtonGadgetSetState(cpccsobj,cutplane[currcp].cliponcellsel,FALSE);

   ival = (int) (cutplane[currcp].heightfac * 100.);
   XmScaleSetValue(objheight,ival);
   ival = (int) (cutplane[currcp].dist * 100.);
   XmScaleSetValue(objveldist,ival);
   XmToggleButtonGadgetSetState(htbut,cutplane[currcp].heightflag,FALSE);
   XmToggleButtonGadgetSetState(facebut,cutplane[currcp].facesflag,FALSE);
   XmToggleButtonGadgetSetState(edgebut,cutplane[currcp].edgesflag,FALSE);
   XmToggleButtonGadgetSetState(linebut,cutplane[currcp].linesflag,FALSE);
   XmToggleButtonGadgetSetState(velbut,cutplane[currcp].velflag,FALSE);
   XmToggleButtonGadgetSetState(nodebut,cutplane[currcp].nodeflag,FALSE);
   XmToggleButtonGadgetSetState(nnumbut,cutplane[currcp].nnumflag,FALSE);
   XmToggleButtonGadgetSetState(cvelbut,cutplane[currcp].cvelflag,FALSE);
   XmToggleButtonGadgetSetState(cellbut,cutplane[currcp].cellflag,FALSE);
   XmToggleButtonGadgetSetState(cnumbut,cutplane[currcp].cnumflag,FALSE);

   MyManageChild(cutplaneoptform);
}


void cpdisppop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplaneoptformpop();
}


void cpfldtypesel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(cpnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpcfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cpcmatbut,FALSE,FALSE);

   if (arg == 0)
      XmToggleButtonGadgetSetState(cpnfldbut,TRUE,FALSE);

   if (arg == 1)
      XmToggleButtonGadgetSetState(cpcfldbut,TRUE,FALSE);

   if (arg == 2)
      XmToggleButtonGadgetSetState(cpnmatbut,TRUE,FALSE);

   if (arg == 3)
      XmToggleButtonGadgetSetState(cpcmatbut,TRUE,FALSE);
}


void cpnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcpnfld;

   /*  Calculate node field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDNCUTPLANE, ifld);
}


void cpcfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcpcfld;

   /*  Calculate cell field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDCCUTPLANE, ifld);
}


void cpapplyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, nmat, cmat, nfld, cfld, cpflag;
  double *value;
  float aa, bb, cc, dd;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);
  void calccutplane(int ip, float aa, float bb, float cc, float dd, 
                    double *value);

   /*  Determine the current field (nodes  */
   /*  or cells) from toggle buttons.      */
   nfld = (int)XmToggleButtonGadgetGetState(cpnfldbut);
   cfld = (int)XmToggleButtonGadgetGetState(cpcfldbut);
   nmat = (int)XmToggleButtonGadgetGetState(cpnmatbut);
   cmat = (int)XmToggleButtonGadgetGetState(cpcmatbut);
   if (nfld) cutplanefieldactive = currcpnfld;
   if (cfld) cutplanefieldactive = currcpcfld + numvars;
   if (nmat) cutplanefieldactive = -1;
   if (cmat) cutplanefieldactive = -2;

   cutplanefieldforplane = cutplanefieldactive;
   if (cutplanefieldforplane == -1)
     {
      value=(double *)malloc((nnodes+1)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
         value[i] = (double)imat[i];
     }
   else if (cutplanefieldforplane == -2)
     {
      value=(double *)malloc((ncells)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < ncells; i++)
         value[i] = (double)cellmats[i];
     }
   else if (cutplanefieldforplane < numvars)
      value = readnodevalue(currcpnfld);
   else
      value = readcellvalue(currcpcfld);

   /*  Apply field to currently active cutplanes.  */
   cpflag = 0;
   for (i = 0; i < MAXCUTPLANES; i++)
     {
      if (cutplane[i].npolys > 0)
        {
         aa = cutplane[i].aa;  bb = cutplane[i].bb;
         cc = cutplane[i].cc;  dd = cutplane[i].dd;
         calccutplane(i, aa, bb, cc, dd, value);
         cpflag = 1;
        }
     }   
   free(value);

   if (cpflag)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void closecutplane(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(cutplaneform2);
   XtUnmanageChild(cutplaneform);
}


void closecutplane2(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(cutplaneform2);
}


void closecpdisp(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cutplaneoptform);
}


void cpdelete(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[10];
  XmString string;

   if (cutplane[currcp].npolys > 0)
     {
      FREE(cutplane[currcp].polylist);
      FREE(cutplane[currcp].vertsperpoly);
      FREE(cutplane[currcp].vect[0]);
      FREE(cutplane[currcp].vect[1]);
      FREE(cutplane[currcp].vect[2]);
      FREE(cutplane[currcp].cvect[0]);
      FREE(cutplane[currcp].cvect[1]);
      FREE(cutplane[currcp].cvect[2]);
      FREE(cutplane[currcp].dnodes);
      FREE(cutplane[currcp].dcells);
     }
   cutplane[currcp].npolys = 0;
   cutplane_on[currcp] = 0;
   XmToggleButtonGadgetSetState(cponbut,FALSE,FALSE);

   sprintf(str,"%d NONE",currcp+1);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cpsel[currcp],XmNlabelString,string,NULL);
   XmStringFree(string);

   XtSetSensitive(cpopts,FALSE);

   setqueryprobe();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cp_on(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[10];
  XmString string;

   cutplane_on[currcp] = XmToggleButtonGadgetGetState(w);

   if (cutplane[currcp].npolys > 0)
     {
      if (cutplane_on[currcp])
         sprintf(str,"%d ON  ",currcp+1);
      else
         sprintf(str,"%d OFF ",currcp+1);
     }
   else
     {
      cutplane_on[currcp] = 0;
      sprintf(str,"%d NONE",currcp+1);
     }
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cpsel[currcp],XmNlabelString,string,NULL);
   XmStringFree(string);

   setqueryprobe();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void clip_on_field_box2(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplane[currcp].cliponbox = (int)XmToggleButtonGadgetGetState(w);
}


void clip_on_cell_sel2(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplane[currcp].cliponcellsel = (int)XmToggleButtonGadgetGetState(w);
}


void doitcutplane2(a,b,c,d,e,f,g,h,i)
 GLfloat a,b,c,d,e,f,g,h,i;
{
  char str[10];

   x1 = (float)a; y1a = (float)b; z1 = (float)c;
   x3 = (float)d; y3 = (float)e; z3 = (float)f;
   x2 = (float)g; y2 = (float)h; z2 = (float)i;

   XmToggleButtonGadgetSetState(obj2pts,FALSE,FALSE);
   sprintf(str,"%11.7g",x1);
   XtVaSetValues(objx1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",x2);
   XtVaSetValues(objx2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",x3);
   XtVaSetValues(objx3,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",y1a);
   XtVaSetValues(objy1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",y2);
   XtVaSetValues(objy2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",y3);
   XtVaSetValues(objy3,XmNvalue,str,NULL);

   sprintf(str,"%11.7g",z1);
   XtVaSetValues(objz1,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",z2);
   XtVaSetValues(objz2,XmNvalue,str,NULL);
   sprintf(str,"%11.7g",z3);
   XtVaSetValues(objz3,XmNvalue,str,NULL);
}


void doitcutplane(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float aa, bb, cc, dd, vlen, cpnorm[3];
  double *value;
  char *tmpstr;
  static char str[55] = 
           "Error - points do not form a plane.";
  static char str2[55] = 
           "                                   ";
  char str3[10];
  XmString string;
  int i, nfld, cfld, nmat, cmat;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);
  void calccutplane(int ip, float aa, float bb, float cc, float dd, 
                    double *value);

   /*  Read the 3 points.  */

   tmpstr = XmTextFieldGetString(objx1);  x1 = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objy1);  y1a = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objz1);  z1 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objx2);  x2 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objy2);  y2 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objz2);  z2 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objx3);  x3 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objy3);  y3 = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objz3);  z3 = atof(tmpstr); 
   XtFree(tmpstr);

   /*  Determine the current field (nodes  */
   /*  or cells) from toggle buttons.      */
   nfld = (int)XmToggleButtonGadgetGetState(cpnfldbut);
   cfld = (int)XmToggleButtonGadgetGetState(cpcfldbut);
   nmat = (int)XmToggleButtonGadgetGetState(cpnmatbut);
   cmat = (int)XmToggleButtonGadgetGetState(cpcmatbut);
   if (nfld) cutplanefieldactive = currcpnfld;
   if (cfld) cutplanefieldactive = currcpcfld + numvars;
   if (nmat) cutplanefieldactive = -1;
   if (cmat) cutplanefieldactive = -2;
  
   /*  Find a unit normal to the cutplane.  */

   cpnorm[0] = (y2-y1a)*(z3-z1)-(z2-z1)*(y3-y1a);
   cpnorm[1] = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
   cpnorm[2] = (x2-x1)*(y3-y1a)-(y2-y1a)*(x3-x1);

   vlen = cpnorm[0]*cpnorm[0] + cpnorm[1]*cpnorm[1] +
          cpnorm[2]*cpnorm[2];

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (vlen == 0.0)
     {
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   else 
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);
      vlen = sqrt(vlen);
      cpnorm[0] = cpnorm[0]/vlen;
      cpnorm[1] = cpnorm[1]/vlen;
      cpnorm[2] = cpnorm[2]/vlen;
      cutplane[currcp].norm[0] = cpnorm[0];
      cutplane[currcp].norm[1] = cpnorm[1];
      cutplane[currcp].norm[2] = cpnorm[2];

      /*  Find coefficients of the plane  */
      /*  passing through the 3 points.   */
      aa = cpnorm[0];
      bb = cpnorm[1];
      cc = cpnorm[2];
      dd = -x1*aa-y1a*bb-z1*cc;

      cutplane[currcp].aa = aa;  cutplane[currcp].bb = bb;  
      cutplane[currcp].cc = cc;  cutplane[currcp].dd = dd;
      cutplanefieldforplane = cutplanefieldactive;
      if (cutplanefieldforplane == -1)
        {
         value=(double *)malloc((nnodes+1)*sizeof(double));
         if (value == NULL) memerr();
         for (i = 0; i < nnodes+1; i++)
            value[i] = (double)imat[i];
        }
      else if (cutplanefieldforplane == -2)
        {
         value=(double *)malloc((ncells)*sizeof(double));
         if (value == NULL) memerr();
         for (i = 0; i < ncells; i++)
            value[i] = (double)cellmats[i];
        }
      else if (cutplanefieldforplane < numvars)
         value = readnodevalue(currcpnfld);
      else
         value = readcellvalue(currcpcfld);

      calccutplane(currcp, aa, bb, cc, dd, value);

      free(value);

      cutplane[currcp].x1 = x1;
      cutplane[currcp].y1 = y1a;
      cutplane[currcp].z1 = z1;
      cutplane[currcp].x2 = x2;
      cutplane[currcp].y2 = y2;
      cutplane[currcp].z2 = z2;
      cutplane[currcp].x3 = x3;
      cutplane[currcp].y3 = y3;
      cutplane[currcp].z3 = z3;

      if (cutplane[currcp].npolys > 0)
        {
         cutplane_on[currcp] = 1;
         XmToggleButtonGadgetSetState(cponbut,TRUE,FALSE);
         XtSetSensitive(cpopts,TRUE);
         if (cutplane_on[currcp])
            sprintf(str3,"%d ON  ",currcp+1);
         else
            sprintf(str3,"%d OFF ",currcp+1);
         if (displistflag) drawnewlistflag = 1;
         drawnew3dflag = 1;
        }
      else
        {
         cutplane_on[currcp] = 0;
         sprintf(str3,"%d NONE",currcp+1);
        }
      string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cpsel[currcp],XmNlabelString,string,NULL);
      XmStringFree(string);

      XUndefineCursor(dpy,topwin);
     }

   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   fastdraw = 0;
   drawit();
}


void preadcutplane(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int arg;

   arg = (long int)client_data;
   tmpstr = XmTextFieldGetString(w);

   if (arg == 1) x1 = atof(tmpstr);
   if (arg == 2) y1a = atof(tmpstr);
   if (arg == 3) z1 = atof(tmpstr);
   if (arg == 4) x2 = atof(tmpstr);
   if (arg == 5) y2 = atof(tmpstr);
   if (arg == 6) z2 = atof(tmpstr);
   if (arg == 7) x3 = atof(tmpstr);
   if (arg == 8) y3 = atof(tmpstr);
   if (arg == 9) z3 = atof(tmpstr);
   XtFree(tmpstr);
}

void surfheight(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;

   XmScaleGetValue(objheight,&ival);
   cutplane[currcp].heightfac = (float)ival/100.;
   arg = (long int)client_data;
   if (cutplane[currcp].heightflag && arg == 0)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void switches(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, itype;
  void cpgetdistnodes(float aa, float bb, float cc, float dd);
  void cpgetdistcells(float aa, float bb, float cc, float dd);
  void csgetdistnodes(float aa, float bb, float cc, float dd);
  void csgetdistcells(float aa, float bb, float cc, float dd);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Determine whether cutplane or cutsphere.  */
   itype = 1;
   if (currcp > MAXCUTPLANES) itype = 0;

   arg = (long int)client_data;
   if (arg == 0) 
      cutplane[currcp].heightflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 1) 
      cutplane[currcp].edgesflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 2)
     {
      cutplane[currcp].velflag = (int)XmToggleButtonGadgetGetState(w);
      if (vectors == 0)
        {
         cutplane[currcp].velflag = 0;
         XmToggleButtonGadgetSetState(w,cutplane[currcp].velflag,FALSE);
        }
      if (cutplane[currcp].velflag == 0)
        {
         FREE(cutplane[currcp].vect[0]);
         FREE(cutplane[currcp].vect[1]);
         FREE(cutplane[currcp].vect[2]);
        }
     }
   if (arg == 3) 
      cutplane[currcp].nodeflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 4) 
      cutplane[currcp].linesflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 5) 
      cutplane[currcp].nnumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 6)
     {
      cutplane[currcp].cvelflag = (int)XmToggleButtonGadgetGetState(w);
      if (cvectors == 0)
        {
         cutplane[currcp].cvelflag = 0;
         XmToggleButtonGadgetSetState(w,cutplane[currcp].cvelflag,FALSE);
        }
      if (cutplane[currcp].cvelflag == 0)
        {
         FREE(cutplane[currcp].cvect[0]);
         FREE(cutplane[currcp].cvect[1]);
         FREE(cutplane[currcp].cvect[2]);
        }
     }
   if (arg == 7) 
      cutplane[currcp].cellflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 8) 
      cutplane[currcp].cnumflag = (int)XmToggleButtonGadgetGetState(w);
   if (arg == 9) 
      cutplane[currcp].facesflag = (int)XmToggleButtonGadgetGetState(w);

   if (cutplane[currcp].nodeflag + cutplane[currcp].velflag + 
       cutplane[currcp].nnumflag > 0)
     {
      if (itype)
      cpgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                     cutplane[currcp].cc,cutplane[currcp].dd);
      else
      csgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                     cutplane[currcp].cc,cutplane[currcp].dd);
     }
   if (cutplane[currcp].cellflag + cutplane[currcp].cvelflag + 
       cutplane[currcp].cnumflag > 0)
     {
      if (itype)
         cpgetdistcells(cutplane[currcp].aa,cutplane[currcp].bb,
                        cutplane[currcp].cc,cutplane[currcp].dd);
      else
         csgetdistcells(cutplane[currcp].aa,cutplane[currcp].bb,
                        cutplane[currcp].cc,cutplane[currcp].dd);
     }

   setqueryprobe();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void veldist(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float veldistin;
  int ival, arg, itype;
  short cpnodesav, cpvelsav, cpnnumsav;
  void cpgetdistnodes(float aa, float bb, float cc, float dd);
  void cpgetdistcells(float aa, float bb, float cc, float dd);
  void csgetdistnodes(float aa, float bb, float cc, float dd);
  void csgetdistcells(float aa, float bb, float cc, float dd);

   arg = (long int)client_data;

   /*  Determine whether cutplane or cutsphere.  */
   itype = 1;
   if (currcp > MAXCUTPLANES) itype = 0;

   /*  Set cutplane search distance and look      */
   /*  for nodes and cells within that distance.  */

   XmScaleGetValue(objveldist,&ival);
   veldistin = (float)ival/100.;
   cutplane[currcp].dist = veldistin;

   if (arg == 1)
     {
      cpnodesav = cutplane[currcp].nodeflag;
      cpvelsav = cutplane[currcp].velflag;
      cpnnumsav = cutplane[currcp].nnumflag;
      cutplane[currcp].nodeflag = 1;
      cutplane[currcp].velflag = 0;
      cutplane[currcp].nnumflag = 0;
      if (itype)
         cpgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                        cutplane[currcp].cc,cutplane[currcp].dd);
      else
         csgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                        cutplane[currcp].cc,cutplane[currcp].dd);
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 1;
      drawit();
      cutplane[currcp].nodeflag = cpnodesav;
      cutplane[currcp].velflag = cpvelsav;
      cutplane[currcp].nnumflag = cpnnumsav; 
     }
   else
     {
      if (cutplane[currcp].nodeflag + cutplane[currcp].velflag + 
          cutplane[currcp].nnumflag > 0)
        {
         if (itype)
            cpgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                           cutplane[currcp].cc,cutplane[currcp].dd);
         else
            csgetdistnodes(cutplane[currcp].aa,cutplane[currcp].bb,
                           cutplane[currcp].cc,cutplane[currcp].dd);
        }
      if (cutplane[currcp].cellflag + cutplane[currcp].cvelflag + 
          cutplane[currcp].cnumflag > 0)
        {
         if (itype)
            cpgetdistcells(cutplane[currcp].aa,cutplane[currcp].bb,
                           cutplane[currcp].cc,cutplane[currcp].dd);
         else
            csgetdistcells(cutplane[currcp].aa,cutplane[currcp].bb,
                           cutplane[currcp].cc,cutplane[currcp].dd);
        }
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      setqueryprobe();
      drawit();
     }

}


void get_value(val)
 unsigned long val;
{
  unsigned long r, g, b, rt, gt, bt, diff, diffold;
  int ic, indx, k;
  float cvalue, gmin, gmax;
  char str[55], strv[20];
  XmString string;

  if (cutplanefieldforplane < 0) return;

   r = ((val>>24) & 255);
   g = ((val>>16) & 255);
   b = ((val>>8) & 255);

   diffold = 999999999;    
   for (ic = 0; ic < 256; ic++)
     {
      rt = ((jcolor[ic]>>24) & 255);
      gt = ((jcolor[ic]>>16) & 255);
      bt = ((jcolor[ic]>>8) & 255);
      diff = (r-rt)*(r-rt) + (g-gt)*(g-gt) + (b-bt)*(b-bt);
      if (diff < diffold)
        {
         diffold = diff;
         indx = ic;
        }
     }
   if (cutplanefieldforplane < numvars)
     {
      k = cutplanefieldforplane;
      gmin = globfieldmin[k];
      gmax = globfieldmax[k];
     }
   else
     {
      k = cutplanefieldforplane - numvars;
      gmin = globfieldcmin[k];
      gmax = globfieldcmax[k];
     }
   cvalue = gmin + ((float)indx) * (gmax-gmin)/255.0;
   sprintf(strv,"%10.5g",cvalue);
   strcpy(str," ");
   if (cutplanefieldforplane < numvars)
      strcat(str,fieldname[k]);
   else 
      strcat(str,cfieldname[k]);
   strcat(str,strv);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcpval,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void makecutplaneform(parent)
 Widget parent;
{
  Widget cpclose, xlab, ylab, zlab, p1lab, p2lab, p3lab, deflab, 
         cpadd, cpdel, vellab, cpdispclose, cpselbox, cpfldapply, 
         cpclose2;
  void get_1_point(), get_2_points();
  int i;
  Arg wargs[20];
  char str[20];
  static char str2[55] = 
           "                                         ";
  XmString string;

   string = XmStringCreate("Cutplanes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutplaneform = XmCreateFormDialog(parent,"Cutplane",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cpclose = XtVaCreateManagedWidget("Cpclose",
             xmPushButtonGadgetClass,
             cutplaneform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cpclose,XmNactivateCallback,closecutplane,
                    NULL);
 
   string = XmStringCreate("Value",XmSTRING_DEFAULT_CHARSET);
   objval = XtVaCreateManagedWidget("Objval",
            xmPushButtonGadgetClass,
            cutplaneform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cpclose,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(objval,XmNactivateCallback,get_1_point,NULL);

   string = XmStringCreate("       ",
                           XmSTRING_DEFAULT_CHARSET);
   objcpval = XtVaCreateManagedWidget("Objcpval",
              xmLabelGadgetClass,
              cutplaneform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cpclose,
              XmNtopOffset, 15,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, objval,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("Node Field:",XmSTRING_DEFAULT_CHARSET);
   cpnfldbut = XtVaCreateManagedWidget("Cpnfldbut",
               xmToggleButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, objval,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cpnfldbut,XmNvalueChangedCallback,
                    cpfldtypesel,(caddr_t) 0);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   cpnfldlab = XtVaCreateManagedWidget("Cpnfldlab",
               xmLabelGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpnfldbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cpnfldnew = XtVaCreateManagedWidget("Cpnfldnew",
               xmPushButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpnfldlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cpnfldnew,XmNactivateCallback,cpnfldfld,
                    NULL);
      currcpnfld = 0;

   string = XmStringCreate("Cell Field:",XmSTRING_DEFAULT_CHARSET);
   cpcfldbut = XtVaCreateManagedWidget("Cpcfldbut",
               xmToggleButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpnfldnew,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cpcfldbut,XmNvalueChangedCallback,
                    cpfldtypesel,(caddr_t) 1);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   cpcfldlab = XtVaCreateManagedWidget("Cpcfldlab",
               xmLabelGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpcfldbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cpcfldnew = XtVaCreateManagedWidget("Cpcfldnew",
               xmPushButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpcfldlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cpcfldnew,XmNactivateCallback,cpcfldfld,
                    NULL);
      currcpcfld = 0;

   string = XmStringCreate("Node Materials:",XmSTRING_DEFAULT_CHARSET);
   cpnmatbut = XtVaCreateManagedWidget("Cpnmatbut",
               xmToggleButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpcfldnew,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cpnmatbut,XmNvalueChangedCallback,
                    cpfldtypesel,(caddr_t) 2);

   string = XmStringCreate("Cell Materials:",XmSTRING_DEFAULT_CHARSET);
   cpcmatbut = XtVaCreateManagedWidget("Cpcmatbut",
               xmToggleButtonGadgetClass,
               cutplaneform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cpnmatbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cpcmatbut,XmNvalueChangedCallback,
                    cpfldtypesel,(caddr_t) 3);
 
   string = XmStringCreate("Apply Field Change",XmSTRING_DEFAULT_CHARSET);
   cpfldapply = XtVaCreateManagedWidget("Cpfldapply",
                xmPushButtonGadgetClass,
                cutplaneform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cpcmatbut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cpfldapply,XmNactivateCallback,cpapplyfld,NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cpfldapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cpselbox = XmCreateRadioBox(cutplaneform,"Cpselbox",wargs,i);
   XtManageChild(cpselbox);

   for (i = 0; i < MAXCUTPLANES; i++)
     {
      sprintf(str,"%2d NONE",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Cpsel%d",i);
      cpsel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 cpselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(cpsel[i],XmNvalueChangedCallback,
                       cutplane_select,
                       (caddr_t) i);
     }


   /*  Make cutplane form 2.  */

   string = XmStringCreate("Cutplane",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutplaneform2 = XmCreateFormDialog(cutplaneform,"Cutplaneform2",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cpclose2 = XtVaCreateManagedWidget("Cpclose2",
              xmPushButtonGadgetClass,
              cutplaneform2,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cpclose2,XmNactivateCallback,closecutplane2,
                    NULL);

   string = XmStringCreate("2 Points",XmSTRING_DEFAULT_CHARSET);
   obj2pts = XtVaCreateManagedWidget("Obj2pts",
             xmToggleButtonGadgetClass,
             cutplaneform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cpclose2,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(obj2pts,XmNvalueChangedCallback,
                    get_2_points,NULL);

   string = XmStringCreate("     X     ",XmSTRING_DEFAULT_CHARSET);
   xlab = XtVaCreateManagedWidget("Xlab",
          xmLabelGadgetClass,
          cutplaneform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, obj2pts,
          XmNtopOffset, 10,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);
   string = XmStringCreate("     Y     ",XmSTRING_DEFAULT_CHARSET);
   ylab = XtVaCreateManagedWidget("Ylab",
          xmLabelGadgetClass,
          cutplaneform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, obj2pts,
          XmNtopOffset, 10,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, xlab,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);
   string = XmStringCreate("     Z     ",XmSTRING_DEFAULT_CHARSET);
   zlab = XtVaCreateManagedWidget("Zlab",
          xmLabelGadgetClass,
          cutplaneform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, obj2pts,
          XmNtopOffset, 10,
          XmNleftAttachment, XmATTACH_WIDGET,
          XmNleftWidget, ylab,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   objx1 = XtVaCreateManagedWidget("Objx1",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, xlab,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objx1,XmNactivateCallback,preadcutplane,
                    (caddr_t) 1);
   objy1 = XtVaCreateManagedWidget("Objy1",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, xlab,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objx1,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objy1,XmNactivateCallback,preadcutplane,
                    (caddr_t) 2);
   objz1 = XtVaCreateManagedWidget("Objz1",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, xlab,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objy1,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objz1,XmNactivateCallback,preadcutplane,
                    (caddr_t) 3);
   string = XmStringCreate("P1",XmSTRING_DEFAULT_CHARSET);
   p1lab = XtVaCreateManagedWidget("P1lab",
           xmLabelGadgetClass,
           cutplaneform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, xlab,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, objz1,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   objx2 = XtVaCreateManagedWidget("Objx2",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx1,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objx2,XmNactivateCallback,preadcutplane,
                    (caddr_t) 4);
   objy2 = XtVaCreateManagedWidget("Objy2",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx1,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objx2,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objy2,XmNactivateCallback,preadcutplane,
                    (caddr_t) 5);
   objz2 = XtVaCreateManagedWidget("Objz2",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx1,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objy2,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objz2,XmNactivateCallback,preadcutplane,
                    (caddr_t) 6);
   string = XmStringCreate("P2",XmSTRING_DEFAULT_CHARSET);
   p2lab = XtVaCreateManagedWidget("P2lab",
           xmLabelGadgetClass,
           cutplaneform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, objx1,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, objz2,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   objx3 = XtVaCreateManagedWidget("Objx3",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx2,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objx3,XmNactivateCallback,preadcutplane,
                    (caddr_t) 7);
   objy3 = XtVaCreateManagedWidget("Objy3",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx2,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objx3,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objy3,XmNactivateCallback,preadcutplane,
                    (caddr_t) 8);
   objz3 = XtVaCreateManagedWidget("Objz3",
            xmTextFieldWidgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx2,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objy3,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 20,
            XmNcolumns, 11,
            XmNvalue, "",
            NULL);
      XtAddCallback(objz3,XmNactivateCallback,preadcutplane,
                    (caddr_t) 9);
   string = XmStringCreate("P3",XmSTRING_DEFAULT_CHARSET);
   p3lab = XtVaCreateManagedWidget("P3lab",
           xmLabelGadgetClass,
           cutplaneform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, objx2,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, objz3,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   string =
     XmStringCreate("Define cut plane using 3 points.",
                     XmSTRING_DEFAULT_CHARSET);
   deflab = XtVaCreateManagedWidget("Deflab",
            xmLabelGadgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objx3,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
   objerror = XtVaCreateManagedWidget("Objerror",
              xmLabelGadgetClass,
              cutplaneform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, deflab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   cpadd = XtVaCreateManagedWidget("Cpadd",
           xmPushButtonGadgetClass,
           cutplaneform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, objerror,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(cpadd,XmNactivateCallback,doitcutplane,
                    NULL);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   cpdel = XtVaCreateManagedWidget("Cpdel",
           xmPushButtonGadgetClass,
           cutplaneform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, objerror,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, cpadd,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(cpdel,XmNactivateCallback,cpdelete,NULL);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   cponbut = XtVaCreateManagedWidget("Cponbut",
             xmToggleButtonGadgetClass,
             cutplaneform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objerror,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, cpdel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cponbut,XmNvalueChangedCallback,cp_on,NULL);

   string = XmStringCreate("Options",XmSTRING_DEFAULT_CHARSET);
   cpopts = XtVaCreateManagedWidget("Cpopts",
            xmPushButtonGadgetClass,
            cutplaneform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objerror,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, cponbut,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(cpopts,XmNactivateCallback,cpdisppop,NULL);
 
   string = XmStringCreate("Clip on Field Subset",
            XmSTRING_DEFAULT_CHARSET);
   cpcobobj = XtVaCreateManagedWidget("Cpcobobj",
             xmToggleButtonGadgetClass,
             cutplaneform2,
             XmNtopAttachment, XmATTACH_FORM,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, obj2pts,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cpcobobj,XmNvalueChangedCallback,
                    clip_on_field_box2,NULL);
  
   string = XmStringCreate("Clip on Cell Select",
            XmSTRING_DEFAULT_CHARSET);
   cpccsobj = XtVaCreateManagedWidget("Cpccsobj",
             xmToggleButtonGadgetClass,
             cutplaneform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cpcobobj,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, obj2pts,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cpccsobj,XmNvalueChangedCallback,
                    clip_on_cell_sel2,NULL);


   /*  Make cutplane display options form.  */

   string = XmStringCreate("Cutplane Options",
            XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutplaneoptform = XmCreateFormDialog(parent,"Cpdispform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cpdispclose = XtVaCreateManagedWidget("Cpdispclose",
                 xmPushButtonGadgetClass,
                 cutplaneoptform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(cpdispclose,XmNactivateCallback,closecpdisp,
                    NULL);

   string = XmStringCreate("Faces",XmSTRING_DEFAULT_CHARSET);
   facebut = XtVaCreateManagedWidget("Facebut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cpdispclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(facebut,XmNvalueChangedCallback,switches,
                    (caddr_t) 9);

   string = XmStringCreate("Contour Lines",XmSTRING_DEFAULT_CHARSET);
   linebut = XtVaCreateManagedWidget("Linebut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, facebut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(linebut,XmNvalueChangedCallback,switches,
                    (caddr_t) 4);

   string = XmStringCreate("Edges",XmSTRING_DEFAULT_CHARSET);
   edgebut = XtVaCreateManagedWidget("Edgebut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, linebut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(edgebut,XmNvalueChangedCallback,switches,
                    (caddr_t) 1);

   objheight = XtVaCreateManagedWidget("Objheight",
               xmScaleWidgetClass,
               cutplaneoptform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, edgebut,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNminimum, -100,
               XmNmaximum, 100,
               XmNorientation, XmVERTICAL,
               XmNprocessingDirection, XmMAX_ON_TOP,
               XmNvalue, 5,
               XmNscaleHeight, 200,
               XmNscaleMultiple, 1,
               NULL);
      XtAddCallback(objheight,XmNvalueChangedCallback,
                    surfheight,(caddr_t) 0);
      XtAddCallback(objheight,XmNdragCallback,surfheight,(caddr_t) 1);
 
   string = XmStringCreate("Ht.",XmSTRING_DEFAULT_CHARSET);
   htbut = XtVaCreateManagedWidget("Htbut",
           xmToggleButtonGadgetClass,
           cutplaneoptform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, objheight,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           XmNspacing, 1,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(htbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 0);

   objveldist = XtVaCreateManagedWidget("Objveldist",
                xmScaleWidgetClass,
                cutplaneoptform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, edgebut,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, htbut,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, 0,
                XmNmaximum, 100,
                XmNorientation, XmVERTICAL,
                XmNprocessingDirection, XmMAX_ON_TOP,
                XmNvalue, 0,
                XmNscaleHeight, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(objveldist,XmNvalueChangedCallback,
                    veldist,(caddr_t) 0);
      /* XtAddCallback(objveldist,XmNdragCallback,veldist,(caddr_t) 1); */
   string = XmStringCreate("Dist.",XmSTRING_DEFAULT_CHARSET);
   vellab = XtVaCreateManagedWidget("Vellab",
            xmLabelGadgetClass,
            cutplaneoptform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, objveldist,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, htbut,
            XmNleftOffset, 10,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Node Vect.",XmSTRING_DEFAULT_CHARSET);
   velbut = XtVaCreateManagedWidget("Velbut",
            xmToggleButtonGadgetClass,
            cutplaneoptform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, edgebut,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objveldist,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNspacing, 1,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(velbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 2);
 
   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   nodebut = XtVaCreateManagedWidget("Nodebut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, velbut,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, objveldist,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(nodebut,XmNvalueChangedCallback,switches,
                    (caddr_t) 3);
 
   string = XmStringCreate("Node Nos.",XmSTRING_DEFAULT_CHARSET);
   nnumbut = XtVaCreateManagedWidget("Nnumbut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, nodebut,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, objveldist,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(nnumbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 5);
 
   string = XmStringCreate("Cell Vect.",XmSTRING_DEFAULT_CHARSET);
   cvelbut = XtVaCreateManagedWidget("Cvelbut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, nnumbut,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, objveldist,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cvelbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 6);
 
   string = XmStringCreate("Cells",XmSTRING_DEFAULT_CHARSET);
   cellbut = XtVaCreateManagedWidget("Cellbut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cvelbut,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, objveldist,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cellbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 7);
 
   string = XmStringCreate("Cell Nos.",XmSTRING_DEFAULT_CHARSET);
   cnumbut = XtVaCreateManagedWidget("Cnumbut",
             xmToggleButtonGadgetClass,
             cutplaneoptform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cellbut,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, objveldist,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(cnumbut,XmNvalueChangedCallback,switches,
                    (caddr_t) 8);
}


void cpfillnvects(int icp)
{
  int iv, ifld, j, ii;
  double *vin;
  void fillnodevalue(int ifld, double *value);

   vin = (double *)malloc((nnodes+1)*sizeof(double));
   if (vin == NULL) memerr();

   for (iv = 0; iv < 3; iv++)
     {
      if (iv == 0) ifld = vectfldx;
      if (iv == 1) ifld = vectfldy;
      if (iv == 2) ifld = vectfldz;
      fillnodevalue(ifld, vin);

      FREE(cutplane[icp].vect[iv]);
      cutplane[icp].vect[iv] = 
           (float *)malloc(cutplane[icp].ndnodes*sizeof(float));
      if (cutplane[icp].vect[iv] == NULL) memerr();

      for (j = 0; j < cutplane[icp].ndnodes; j++)
        {
         ii = cutplane[icp].dnodes[j];
         cutplane[icp].vect[iv][j] = vin[ii];
        }         
     }

   free(vin);
}


void cpgetdistnodes(float aa, float bb, float cc, float dd)
{
  double dist, dist2, denom, xpart, ypart, zpart;
  int i, cpdnodesalloc, gonode, ndnodes;

   /*  Search for points within the cutplane search distance.  */

   dist = cutplane[currcp].dist * (span/2.);
   denom = sqrt(aa*aa + bb*bb + cc*cc);
   ndnodes = 0;

   FREE(cutplane[currcp].dnodes);
   cpdnodesalloc = nnodes / 10.;
   cutplane[currcp].dnodes = (int *)malloc((cpdnodesalloc)*sizeof(int));
   if (cutplane[currcp].dnodes == NULL) memerr();
   for (i = 1; i < nnodes+1; i++)
     {

      /*  If subset clip is on, check node.  */
      gonode = 1;
      if (cutplane[currcp].cliponbox) gonode = (int)nodesubsetflag[i];

      if (gonode)
        {
         xpart = aa * nodes[i].x;
         ypart = bb * nodes[i].y;
         zpart = cc * nodes[i].z;
            
         dist2 = (xpart + ypart + zpart + dd) / denom;
         if (dist2 < 0.) dist2 = -dist2;

         if (dist2 <= dist)
           {

            /*  Reallocate more memory if needed.  */
            if (ndnodes + 1 > cpdnodesalloc)
              {
               cpdnodesalloc += nnodes/10;
               cutplane[currcp].dnodes = 
                           (int *)realloc(cutplane[currcp].dnodes,
                                         (cpdnodesalloc)*sizeof(int));
               if (cutplane[currcp].dnodes == NULL) memerr();
              }

            /*  Save node id.  */
            cutplane[currcp].dnodes[ndnodes] = i;
            ndnodes++;
           }
        }
     }

   if (ndnodes > 0)
      cutplane[currcp].dnodes = 
             (int *)realloc(cutplane[currcp].dnodes,ndnodes*sizeof(int));
   else FREE(cutplane[currcp].dnodes);

   cutplane[currcp].ndnodes = ndnodes;

   /*  Fill cutplane node vector arrays, if requested.  */
   if (cutplane[currcp].velflag && ndnodes > 0)
      cpfillnvects(currcp);
}


void cpfillcvects(int icp)
{
  int iv, ifld, j, ii;
  double *vin;
  void fillcellvalue(int ifld, double *value);

   vin = (double *)malloc(ncells*sizeof(double));
   if (vin == NULL) memerr();

   for (iv = 0; iv < 3; iv++)
     {
      if (iv == 0) ifld = cvectfldx;
      if (iv == 1) ifld = cvectfldy;
      if (iv == 2) ifld = cvectfldz;
      fillcellvalue(ifld, vin);

      FREE(cutplane[icp].cvect[iv]);
      cutplane[icp].cvect[iv] = (float *)
                    malloc(cutplane[icp].ndcells*sizeof(float));
      if (cutplane[icp].cvect[iv] == NULL) memerr();

      for (j = 0; j < cutplane[icp].ndcells; j++)
        {
         ii = cutplane[icp].dcells[j];
         cutplane[icp].cvect[iv][j] = vin[ii];
        }         
     }

   free(vin);
}


void cpgetdistcells(float aa, float bb, float cc, float dd)
{
  float dist, dist2, denom, xpart, ypart, zpart;
  int i, cpdcellsalloc, jcell, gocell, ndcells;


   /*  Search for points within the cutplane search distance.  */

   dist = cutplane[currcp].dist * (span/2.);
   denom = sqrt(aa*aa + bb*bb + cc*cc);
   ndcells = 0;

   FREE(cutplane[currcp].dcells);
   cpdcellsalloc = ncells / 10.;
   if (cpdcellsalloc < 10) cpdcellsalloc = 10;
   cutplane[currcp].dcells = (int *)malloc((cpdcellsalloc)*sizeof(int));
   if (cutplane[currcp].dcells == NULL) memerr();
   for (i = 0; i < ncells; i++)
     {

      /*  If subset clip is on, check cell.  */
      gocell = 1;
      if (cutplane[currcp].cliponbox) gocell = (int)cellsubsetflag[i];

      /*  If cell select is on, check cell.  */
      if (cutplane[currcp].cliponcellsel) gocell = (int)cellselectflag[i];

      if (gocell)
        {      
         jcell = i;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(i,0);
           }
         xpart = aa * cells[jcell].xavg;
         ypart = bb * cells[jcell].yavg;
         zpart = cc * cells[jcell].zavg;
            
         dist2 = (xpart + ypart + zpart + dd) / denom;
         if (dist2 < 0.) dist2 = -dist2;

         if (dist2 <= dist)
           {

            /*  Reallocate more memory if needed.  */
            if (ndcells + 1 > cpdcellsalloc)
              {
               cpdcellsalloc += ncells/10 + 1;
               cutplane[currcp].dcells = 
                      (int *)realloc(cutplane[currcp].dcells,
                                     (cpdcellsalloc)*sizeof(int));
               if (cutplane[currcp].dcells == NULL) memerr();
              }

            /*  Save cell id.  */
            cutplane[currcp].dcells[ndcells] = i;
            ndcells++;
           }
        }
     }

   if (ndcells > 0)
      cutplane[currcp].dcells = 
             (int *)realloc(cutplane[currcp].dcells,ndcells*sizeof(int));
   else FREE(cutplane[currcp].dcells);

   cutplane[currcp].ndcells = ndcells;

   /*  Fill cutplane cell vector arrays, if requested.  */
   if (cutplane[currcp].cvelflag && ndcells > 0)
      cpfillcvects(currcp);
}



void calccutplane(int ip, float aa, float bb, float cc, 
                  float dd, double *value)
{
  int j, totverts, npolys, clipbox, clipcellsel;

   npolys = 0;
   clipbox = cutplane[ip].cliponbox;
   clipcellsel = cutplane[ip].cliponcellsel;

   makecutplane(aa,bb,cc,dd,clipbox,clipcellsel,value,&npolys);

   /*  Fill cutplane arrays from makecutplane arrays.  */
   if (cutplane[ip].npolys > 0)
     {
      FREE(cutplane[ip].polylist);
      FREE(cutplane[ip].vertsperpoly);
     }
   cutplane[ip].npolys = npolys;
   if (npolys > 0)
     {
      cutplane[ip].vertsperpoly = 
         (short *)malloc(npolys*sizeof(short));
      totverts = 0;
      for (j = 0; j < npolys; j++)
        {
         totverts += vpcutpoly[j];
         cutplane[ip].vertsperpoly[j] = vpcutpoly[j];
        }
      cutplane[ip].polylist = 
         (float *)malloc(4*totverts*sizeof(float));
      for (j = 0; j < totverts; j++)
        {
         cutplane[ip].polylist[4*j] = cutval[j];
         cutplane[ip].polylist[4*j+1] = cutlist[3*j];
         cutplane[ip].polylist[4*j+2] = cutlist[3*j+1];
         cutplane[ip].polylist[4*j+3] = cutlist[3*j+2];
        }

      /*  Get nodes within dist.  */
      /* if (cutplane[ip].dist > .001) */
      if (cutplane[ip].dist >= 0)
        {
         if (cutplane[ip].nodeflag + cutplane[ip].velflag + 
             cutplane[ip].nnumflag > 0)
            cpgetdistnodes(aa,bb,cc,dd);
         if (cutplane[ip].cellflag + cutplane[ip].cvelflag + 
             cutplane[ip].cnumflag > 0)
            cpgetdistcells(aa,bb,cc,dd);
        }
     }
   free(vpcutpoly);  free(cutlist);  free(cutval);
}

void cutplanegetnfield(int i)
{
  XmString string;

   currcpnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cpnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   string = XmStringCreate("               ",
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcpval,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void cutplanegetcfield(int i)
{
  XmString string;

   currcpcfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cpcfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   string = XmStringCreate("               ",
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcpval,XmNlabelString,string,NULL);
   XmStringFree(string);
}
