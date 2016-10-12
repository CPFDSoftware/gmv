
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

static Widget cscenx, csceny, cscenz, csradius, csfldval, 
              objcsval, csclipbox, csclipsel, 
              csnfldbut, csnfldlab, cscfldbut, cscfldlab, 
              cssel[MAXCUTSPHERES], cutsphereform2, csonbut, csopts,
              csnmatbut, cscmatbut, csnfldnew, cscfldnew; 
static int currcsnfld, currcscfld, currcs;


void cutsphereformpop()
{
  int i, j;
  char str[10];
  XmString string;

   XmToggleButtonGadgetSetState(csnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cscmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(csnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cscfldbut,FALSE,FALSE);
   if (cutspherefieldforsphere == -1)
     {
      XmToggleButtonGadgetSetState(csnmatbut,TRUE,FALSE);
      currcsnfld = -1;
     }
   else if (cutspherefieldforsphere == -2)
     {
      XmToggleButtonGadgetSetState(cscmatbut,TRUE,FALSE);
      currcscfld = -1;
     }
   else if (cutspherefieldforsphere < numvars)
     {
      XmToggleButtonGadgetSetState(csnfldbut,TRUE,FALSE);
      currcsnfld = cutspherefieldforsphere;
     }
   else
     {
      XmToggleButtonGadgetSetState(cscfldbut,TRUE,FALSE);
      currcscfld = cutspherefieldforsphere - numvars;
     }

   string = XmStringCreate(fieldname[currcsnfld],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(csnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (cnumvars > 0)
     {
      string = XmStringCreate(cfieldname[currcscfld],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cscfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   for (i = 0; i < MAXCUTSPHERES; i++)
     {
      j = MAXCUTPLANES + 1 + i;
      if (cutplane[j].npolys > 0)
        {
         if (cutplane_on[j])
            sprintf(str,"%d ON  ",i+1);
         else
            sprintf(str,"%d OFF ",i+1);
        }
      else
         sprintf(str,"%d NONE",i+1);
     
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cssel[i],XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   XtSetSensitive(csnfldbut,TRUE);
   XtSetSensitive(csnfldlab,TRUE);
   XtSetSensitive(csnfldnew,TRUE);
   if (numvars < 1)
     {
      XtSetSensitive(csnfldbut,FALSE);
      XtSetSensitive(csnfldlab,FALSE);
      XtSetSensitive(csnfldnew,FALSE);
     }
   XtSetSensitive(cscfldbut,TRUE);
   XtSetSensitive(cscfldlab,TRUE);
   XtSetSensitive(cscfldnew,TRUE);
   if (cnumvars < 1)
     {
      XtSetSensitive(cscfldbut,FALSE);
      XtSetSensitive(cscfldlab,FALSE);
      XtSetSensitive(cscfldnew,FALSE);
     }

   MyManageChild(cutsphereform);
}


void cutsphereformpop2()
{
  char str[20];
  XmString string;

   sprintf(str,"%7.5g",cutplane[currcp].aa);
   XtVaSetValues(cscenx,XmNvalue,str,NULL);
   sprintf(str,"%7.5g",cutplane[currcp].bb);
   XtVaSetValues(csceny,XmNvalue,str,NULL);
   sprintf(str,"%7.5g",cutplane[currcp].cc);
   XtVaSetValues(cscenz,XmNvalue,str,NULL);
   sprintf(str,"%7.5g",cutplane[currcp].dd);
   XtVaSetValues(csradius,XmNvalue,str,NULL);

   XmToggleButtonGadgetSetState(csonbut,cutplane_on[currcp],FALSE);
   XmToggleButtonGadgetSetState(csclipbox,
                                cutplane[currcp].cliponbox,FALSE);
   XmToggleButtonGadgetSetState(csclipsel,
                                cutplane[currcp].cliponcellsel,FALSE);

   if (cutplane[currcp].npolys > 0)
      XtSetSensitive(csopts,TRUE);
   else
      XtSetSensitive(csopts,FALSE);

   sprintf(str,"Cutsphere %d",currcs+1);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cutsphereform2,XmNdialogTitle,string,NULL);
   XmStringFree(string);

   MyManageChild(cutsphereform2);
}


void cutsphere_select(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;
   currcs = i;
   currcp = i + MAXCUTPLANES + 1;

   cutsphereformpop2();
}


void csdisppop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplaneoptformpop();
}


void csfldtypesel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(csnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cscfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(csnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(cscmatbut,FALSE,FALSE);

   if (arg == 0)
      XmToggleButtonGadgetSetState(csnfldbut,TRUE,FALSE);

   if (arg == 1)
      XmToggleButtonGadgetSetState(cscfldbut,TRUE,FALSE);

   if (arg == 2)
      XmToggleButtonGadgetSetState(csnmatbut,TRUE,FALSE);

   if (arg == 3)
      XmToggleButtonGadgetSetState(cscmatbut,TRUE,FALSE);
}


void csnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcsnfld;

   /*  Calculate node field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDNCUTSPHERE, ifld);
}


void cscfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = currcscfld;

   /*  Calculate cell field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDCCUTSPHERE, ifld);
}


void csapplyfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, nmat, cmat, nfld, cfld, csflag;
  double *value;
  float aa, bb, cc, dd;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);
  void calccutsphere(int ip, float aa, float bb, float cc, float dd, 
                     double *value);

   /*  Determine the current field (nodes  */
   /*  or cells) from toggle buttons.      */
   nfld = (int)XmToggleButtonGadgetGetState(csnfldbut);
   cfld = (int)XmToggleButtonGadgetGetState(cscfldbut);
   nmat = (int)XmToggleButtonGadgetGetState(csnmatbut);
   cmat = (int)XmToggleButtonGadgetGetState(cscmatbut);
   if (nfld) cutspherefieldactive = currcsnfld;
   if (cfld) cutspherefieldactive = currcscfld + numvars;
   if (nmat) cutspherefieldactive = -1;
   if (cmat) cutspherefieldactive = -2;

   cutspherefieldforsphere = cutspherefieldactive;
   if (cutspherefieldforsphere == -1)
     {
      value=(double *)malloc((nnodes+1)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
         value[i] = (double)imat[i];
     }
   else if (cutspherefieldforsphere == -2)
     {
      value=(double *)malloc((ncells)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < ncells; i++)
         value[i] = (double)cellmats[i];
     }
   else if (cutspherefieldforsphere < numvars)
      value = readnodevalue(currcsnfld);
   else
      value = readcellvalue(currcscfld);

   /*  Apply field to currently active cutspheres.  */
   csflag = 0;
   for (i = 0; i < MAXCUTSPHERES; i++)
     {
      j = MAXCUTPLANES + 1 + i;
      if (cutplane[j].npolys > 0)
        {
         aa = cutplane[j].aa;  bb = cutplane[j].bb;
         cc = cutplane[j].cc;  dd = cutplane[j].dd;
         calccutsphere(j, aa, bb, cc, dd, value);
         csflag = 1;
        }
     }   
   free(value);

   if (csflag)
     {
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }
}


void closecutsphere(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(cutsphereform2);
   XtUnmanageChild(cutsphereform);
}


void closecutsphere2(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(cutsphereform2);
}


void csdelete(w,client_data,call_data)
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
   XmToggleButtonGadgetSetState(csonbut,FALSE,FALSE);

   sprintf(str,"%d NONE",currcs+1);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cssel[currcs],XmNlabelString,string,NULL);
   XmStringFree(string);

   XtSetSensitive(csopts,FALSE);

   setqueryprobe();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void cs_on(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[10];
  XmString string;

   cutplane_on[currcp] = XmToggleButtonGadgetGetState(w);

   if (cutplane[currcp].npolys > 0)
     {
      if (cutplane_on[currcp])
         sprintf(str,"%d ON  ",currcs+1);
      else
         sprintf(str,"%d OFF ",currcs+1);
     }
   else
     {
      cutplane_on[currcp] = 0;
      sprintf(str,"%d NONE",currcs+1);
     }
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cssel[currcs],XmNlabelString,string,NULL);
   XmStringFree(string);

   setqueryprobe();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void csclip_on_field_box2(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplane[currcp].cliponbox = (int)XmToggleButtonGadgetGetState(w);
}


void csclip_on_cell_sel2(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cutplane[currcp].cliponcellsel = (int)XmToggleButtonGadgetGetState(w);
}


void doitcutsphere(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float aa, bb, cc, dd;
  double *value;
  char *tmpstr;
  char str3[10];
  XmString string;
  int i, nfld, cfld, nmat, cmat;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);
  void calccutsphere(int ip, float aa, float bb, float cc, float dd, 
                     double *value);

   /*  Read the sphere center and radius.  */

   tmpstr = XmTextFieldGetString(cscenx);  aa = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(csceny);  bb = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(cscenz);  cc = atof(tmpstr); 
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(csradius);  dd = atof(tmpstr); 
   XtFree(tmpstr);

   /*  Determine the current field (nodes  */
   /*  or cells) from toggle buttons.      */
   nfld = (int)XmToggleButtonGadgetGetState(csnfldbut);
   cfld = (int)XmToggleButtonGadgetGetState(cscfldbut);
   nmat = (int)XmToggleButtonGadgetGetState(csnmatbut);
   cmat = (int)XmToggleButtonGadgetGetState(cscmatbut);
   if (nfld) cutspherefieldactive = currcsnfld;
   if (cfld) cutspherefieldactive = currcscfld + numvars;
   if (nmat) cutspherefieldactive = -1;
   if (cmat) cutspherefieldactive = -2;
  
   /*  Set the cutplane unit normal to 0 for sphere.  */
   cutplane[currcp].norm[0] = 0;
   cutplane[currcp].norm[1] = 0;
   cutplane[currcp].norm[2] = 0;

   /*  Set sphere center and radius.  */
   cutplane[currcp].aa = aa;  cutplane[currcp].bb = bb;  
   cutplane[currcp].cc = cc;  cutplane[currcp].dd = dd;
   cutspherefieldforsphere = cutspherefieldactive;
   if (cutspherefieldforsphere == -1)
     {
      value=(double *)malloc((nnodes+1)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
         value[i] = (double)imat[i];
     }
   else if (cutspherefieldforsphere == -2)
     {
      value=(double *)malloc((ncells)*sizeof(double));
      if (value == NULL) memerr();
      for (i = 0; i < ncells; i++)
         value[i] = (double)cellmats[i];
     }
   else if (cutspherefieldforsphere < numvars)
      value = readnodevalue(currcsnfld);
   else
      value = readcellvalue(currcscfld);

   calccutsphere(currcp, aa, bb, cc, dd, value);

   free(value);

   cutplane[currcp].x1 = 0.;
   cutplane[currcp].y1 = 0.;
   cutplane[currcp].z1 = 0.;
   cutplane[currcp].x2 = 0.;
   cutplane[currcp].y2 = 0.;
   cutplane[currcp].z2 = 0.;
   cutplane[currcp].x3 = 0.;
   cutplane[currcp].y3 = 0.;
   cutplane[currcp].z3 = 0.;

   if (cutplane[currcp].npolys > 0)
     {
      cutplane_on[currcp] = 1;
      XmToggleButtonGadgetSetState(csonbut,TRUE,FALSE);
      XtSetSensitive(csopts,TRUE);
      if (cutplane_on[currcp])
         sprintf(str3,"%d ON  ",currcs+1);
      else
         sprintf(str3,"%d OFF ",currcs+1);
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
     }
   else
     {
      cutplane_on[currcp] = 0;
      sprintf(str3,"%d NONE",currcs+1);
     }
   string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cssel[currcs],XmNlabelString,string,NULL);
   XmStringFree(string);

   XUndefineCursor(dpy,topwin);

   if (beepflag) XBell(dpy,100);
   XFlush(dpy);
   fastdraw = 0;
   drawit();
}


void get_value2(val)
 unsigned long val;
{
  unsigned long r, g, b, rt, gt, bt, diff, diffold;
  int ic, indx, k;
  float cvalue, gmin, gmax;
  char str[55], strv[20];
  XmString string;

  if (cutspherefieldforsphere < 0) return;

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
   if (cutspherefieldforsphere < numvars)
     {
      k = cutspherefieldforsphere;
      gmin = globfieldmin[k];
      gmax = globfieldmax[k];
     }
   else
     {
      k = cutspherefieldforsphere - numvars;
      gmin = globfieldcmin[k];
      gmax = globfieldcmax[k];
     }
   cvalue = gmin + ((float)indx) * (gmax-gmin)/255.0;
   sprintf(strv,"%10.5g",cvalue);
   strcpy(str," ");
   if (cutspherefieldforsphere < numvars)
      strcat(str,fieldname[k]);
   else 
      strcat(str,cfieldname[k]);
   strcat(str,strv);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcsval,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void makecutsphereform(parent)
 Widget parent;
{
  Widget csclose, xclab, yclab, zclab, rlab, csadd, csdel, csselbox,  
         csfldapply, csclose2;
  void get_1_point();
  int i;
  Arg wargs[20];
  char str[20];
  XmString string;

   string = XmStringCreate("Cutspheres",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutsphereform = XmCreateFormDialog(parent,"Cutsphere",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csclose = XtVaCreateManagedWidget("Csclose",
             xmPushButtonGadgetClass,
             cutsphereform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(csclose,XmNactivateCallback,closecutsphere,
                    NULL);
 
   string = XmStringCreate("Value",XmSTRING_DEFAULT_CHARSET);
   csfldval = XtVaCreateManagedWidget("Csfldval",
            xmPushButtonGadgetClass,
            cutsphereform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, csclose,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(csfldval,XmNactivateCallback,get_1_point,(caddr_t) 4);

   string = XmStringCreate("       ",
                           XmSTRING_DEFAULT_CHARSET);
   objcsval = XtVaCreateManagedWidget("Objcsval",
              xmLabelGadgetClass,
              cutsphereform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, csclose,
              XmNtopOffset, 15,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, csfldval,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("Node Field:",XmSTRING_DEFAULT_CHARSET);
   csnfldbut = XtVaCreateManagedWidget("Csnfldbut",
               xmToggleButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csfldval,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(csnfldbut,XmNvalueChangedCallback,
                    csfldtypesel,(caddr_t) 0);

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   csnfldlab = XtVaCreateManagedWidget("Csnfldlab",
               xmLabelGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csnfldbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   csnfldnew = XtVaCreateManagedWidget("Csnfldnew",
               xmPushButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csnfldlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(csnfldnew,XmNactivateCallback,csnfldfld,
                    NULL);
      currcsnfld = 0;

   string = XmStringCreate("Cell Field:",XmSTRING_DEFAULT_CHARSET);
   cscfldbut = XtVaCreateManagedWidget("Cscfldbut",
               xmToggleButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csnfldnew,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cscfldbut,XmNvalueChangedCallback,
                    csfldtypesel,(caddr_t) 1);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   cscfldlab = XtVaCreateManagedWidget("Cscfldlab",
               xmLabelGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cscfldbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   cscfldnew = XtVaCreateManagedWidget("Cscfldnew",
               xmPushButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cscfldlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cscfldnew,XmNactivateCallback,cscfldfld,
                    NULL);
      currcscfld = 0;

   string = XmStringCreate("Node Materials:",XmSTRING_DEFAULT_CHARSET);
   csnmatbut = XtVaCreateManagedWidget("Csnmatbut",
               xmToggleButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cscfldnew,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(csnmatbut,XmNvalueChangedCallback,
                    csfldtypesel,(caddr_t) 2);

   string = XmStringCreate("Cell Materials:",XmSTRING_DEFAULT_CHARSET);
   cscmatbut = XtVaCreateManagedWidget("Cscmatbut",
               xmToggleButtonGadgetClass,
               cutsphereform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csnmatbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string); 
      XtAddCallback(cscmatbut,XmNvalueChangedCallback,
                    csfldtypesel,(caddr_t) 3);
 
   string = XmStringCreate("Apply Field Change",XmSTRING_DEFAULT_CHARSET);
   csfldapply = XtVaCreateManagedWidget("Csfldapply",
                xmPushButtonGadgetClass,
                cutsphereform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cscmatbut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csfldapply,XmNactivateCallback,csapplyfld,NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, csfldapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   csselbox = XmCreateRadioBox(cutsphereform,"Csselbox",wargs,i);
   XtManageChild(csselbox);

   for (i = 0; i < MAXCUTSPHERES; i++)
     {
      sprintf(str,"%2d NONE",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Cssel%d",i);
      cssel[i] = XtVaCreateManagedWidget(str,
                 xmToggleButtonGadgetClass,
                 csselbox,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 0,
                 NULL);
         XmStringFree(string);
         XtAddCallback(cssel[i],XmNvalueChangedCallback,
                       cutsphere_select,
                       (caddr_t) i);
     }


   /*  Make cutsphere form 2.  */

   string = XmStringCreate("Cutsphere",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cutsphereform2 = XmCreateFormDialog(cutsphereform,"Cutsphereform2",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csclose2 = XtVaCreateManagedWidget("Csclose2",
              xmPushButtonGadgetClass,
              cutsphereform2,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(csclose2,XmNactivateCallback,closecutsphere2,
                    NULL);
 
   string = XmStringCreate("Clip on Field Subset",
            XmSTRING_DEFAULT_CHARSET);
   csclipbox = XtVaCreateManagedWidget("Csclipbox",
               xmToggleButtonGadgetClass,
               cutsphereform2,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csclose2,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNset, FALSE,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(csclipbox,XmNvalueChangedCallback,
                    csclip_on_field_box2,NULL);
  
   string = XmStringCreate("Clip on Cell Select",
            XmSTRING_DEFAULT_CHARSET);
   csclipsel = XtVaCreateManagedWidget("Csclipsel",
             xmToggleButtonGadgetClass,
             cutsphereform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, csclipbox,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(csclipsel,XmNvalueChangedCallback,
                    csclip_on_cell_sel2,NULL);

   string = XmStringCreate("X:",XmSTRING_DEFAULT_CHARSET);
   xclab = XtVaCreateManagedWidget("Xclab",
          xmLabelGadgetClass,
          cutsphereform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, csclipsel,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   cscenx = XtVaCreateManagedWidget("cscenx",
            xmTextFieldWidgetClass,
            cutsphereform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, csclipsel,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, xclab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 10,
            XmNcolumns, 10,
            XmNvalue, "",
            NULL);

   string = XmStringCreate("Y:",XmSTRING_DEFAULT_CHARSET);
   yclab = XtVaCreateManagedWidget("Yclab",
          xmLabelGadgetClass,
          cutsphereform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, cscenx,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   csceny = XtVaCreateManagedWidget("csceny",
            xmTextFieldWidgetClass,
            cutsphereform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, cscenx,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, yclab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 10,
            XmNcolumns, 10,
            XmNvalue, "",
            NULL);

   string = XmStringCreate("Z:",XmSTRING_DEFAULT_CHARSET);
   zclab = XtVaCreateManagedWidget("Zclab",
          xmLabelGadgetClass,
          cutsphereform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, csceny,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   cscenz = XtVaCreateManagedWidget("cscenz",
            xmTextFieldWidgetClass,
            cutsphereform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, csceny,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, zclab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 10,
            XmNcolumns, 10,
            XmNvalue, "",
            NULL);

   string = XmStringCreate("R:",XmSTRING_DEFAULT_CHARSET);
   rlab = XtVaCreateManagedWidget("Rlab",
          xmLabelGadgetClass,
          cutsphereform2,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, cscenz,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNlabelString, string,
          NULL);
      XmStringFree(string);

   csradius = XtVaCreateManagedWidget("Csradius",
              xmTextFieldWidgetClass,
              cutsphereform2,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cscenz,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, rlab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmarginHeight, 0,
              XmNmarginWidth, 1,
              XmNmaxLength, 10,
              XmNcolumns, 10,
              XmNvalue, "",
              NULL);

   string = XmStringCreate("Add",XmSTRING_DEFAULT_CHARSET);
   csadd = XtVaCreateManagedWidget("Csadd",
           xmPushButtonGadgetClass,
           cutsphereform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, rlab,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(csadd,XmNactivateCallback,doitcutsphere,
                    NULL);

   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   csdel = XtVaCreateManagedWidget("Csdel",
           xmPushButtonGadgetClass,
           cutsphereform2,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, rlab,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, csadd,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(csdel,XmNactivateCallback,csdelete,NULL);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   csonbut = XtVaCreateManagedWidget("Csonbut",
             xmToggleButtonGadgetClass,
             cutsphereform2,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, rlab,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, csdel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(csonbut,XmNvalueChangedCallback,cs_on,NULL);

   string = XmStringCreate("Options",XmSTRING_DEFAULT_CHARSET);
   csopts = XtVaCreateManagedWidget("Csopts",
            xmPushButtonGadgetClass,
            cutsphereform2,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, rlab,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, csonbut,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            XmNset, FALSE,
            XmNspacing, 2,
            XmNselectColor, yellowpix,
            NULL);
      XmStringFree(string);
      XtAddCallback(csopts,XmNactivateCallback,csdisppop,NULL);
}


void csgetdistnodes(float aa, float bb, float cc, float dd)
{
  double dist, dist2, rmin, rmax, xp, yp, zp;
  int i, cpdnodesalloc, gonode, ndnodes;

   /*  Search for points within the cutsphere search distance.  */

   dist = cutplane[currcp].dist * (span/2.);
   rmin = dd - dist; 
   if (rmin < 0.) rmin = 0.;
   rmax = dd + dist;
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
         xp = nodes[i].x - aa;
         yp = nodes[i].y - bb;
         zp = nodes[i].z - cc;
            
         dist2 = sqrt(xp*xp + yp*yp + zp*zp);

         if (rmin <= dist2 && dist2 <= rmax)
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


void csgetdistcells(float aa, float bb, float cc, float dd)
{
  double dist, dist2, rmin, rmax, xp, yp, zp;
  int i, cpdcellsalloc, jcell, gocell, ndcells;


   /*  Search for points within the cutsphere search distance.  */

   dist = cutplane[currcp].dist * (span/2.);
   rmin = dd - dist; 
   if (rmin < 0.) rmin = 0.;
   rmax = dd + dist;
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
         xp =cells[jcell].xavg - aa;
         yp =cells[jcell].yavg - bb;
         zp =cells[jcell].zavg - cc;

         dist2 = sqrt(xp*xp + yp*yp + zp*zp);

         if (rmin <= dist2 && dist2 <= rmax)
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



void calccutsphere(int ip, float aa, float bb, float cc, 
                  float dd, double *value)
{
  int j, totverts, npolys, clipbox, clipcellsel;

   npolys = 0;
   clipbox = cutplane[ip].cliponbox;
   clipcellsel = cutplane[ip].cliponcellsel;

   makecutsphere(aa,bb,cc,dd,clipbox,clipcellsel,value,&npolys);

   /*  Fill cutplane arrays from makecutsphere arrays.  */
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
      if (cutplane[ip].dist > .001)
        {
         if (cutplane[ip].nodeflag + cutplane[ip].velflag + 
             cutplane[ip].nnumflag > 0)
            csgetdistnodes(aa,bb,cc,dd);
         if (cutplane[ip].cellflag + cutplane[ip].cvelflag + 
             cutplane[ip].cnumflag > 0)
            csgetdistcells(aa,bb,cc,dd);
        }
     }
   free(vpcutpoly);  free(cutlist);  free(cutval);
}

void cutspheregetnfield(int i)
{
  XmString string;

   currcsnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(csnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   string = XmStringCreate("               ",
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcsval,XmNlabelString,string,NULL);
   XmStringFree(string);
}


void cutspheregetcfield(int i)
{
  XmString string;

   currcscfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cscfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   string = XmStringCreate("               ",
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objcsval,XmNlabelString,string,NULL);
   XmStringFree(string);
}
