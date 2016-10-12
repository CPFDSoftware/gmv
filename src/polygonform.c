#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/Scale.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>

#include <stdlib.h>
#include <sys/types.h>
#include <math.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

#define MAXVAL 9.99999e32

static Widget pshade, pline, pexplode, stipplebut, pmatsellist,
              pmatselprobe, pmatremprobe;
static int numlist;
static short shadedraw, linedraw, listmat[MAXMATS];


void polygonformpop()
{
  int ival, i, j;

   XmListDeselectAllItems(pmatsellist);

   for (j = 0; j < numlist; j++)
     {
      i = listmat[j];
      if (active_mat[i])
         XmListSelectPos(pmatsellist,j+1,FALSE);
     }

   ival = (int) (pexplodefact * 100.);
   XmScaleSetValue(pexplode,ival);
   XmToggleButtonGadgetSetState(pshade,polysflag,FALSE);
   XmToggleButtonGadgetSetState(pline,linesflag,FALSE);
   XmToggleButtonGadgetSetState(stipplebut,polystiplflag,FALSE);

   shadedraw = polysflag;
   linedraw = linesflag;

   MyManageChild(polygonform);
}


void plclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   polymatselprobeflag = 0;
   XmToggleButtonSetState(pmatselprobe,FALSE,FALSE);
   polymatremprobeflag = 0;
   XmToggleButtonSetState(pmatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   XtUnmanageChild(matorderform);
   XtUnmanageChild(polysubsetform);
   XtUnmanageChild(polygonform);
}


void poly_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k, l, *pmatselected, nlist;
  void readpolyfile();

   /*  Turn off polygon material select and remove probe buttons.  */
   polymatselprobeflag = 0;
   XmToggleButtonSetState(pmatselprobe,FALSE,FALSE);
   polymatremprobeflag = 0;
   XmToggleButtonSetState(pmatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   /*  Get and set the polygon material selection.  */
   j = XmListGetSelectedPos(pmatsellist,&pmatselected,&nlist);
   if (!j) nlist = 0;
   for (j = 0; j < MAXMATS; j++) active_mat[j] = 0;
   for (j = 0; j < nlist; j++)
     { 
      k = pmatselected[j] - 1;
      l = listmat[k];
      active_mat[l] = 1;
     }
   FREE(pmatselected);

   /*  Read polygon data if needed.  */
   if ((polysflag == 0 && linesflag == 0) && 
       (shadedraw == 1 || linedraw == 1))
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);
      readpolyfile();
     }

   /*  Release polygon memory if not drawing.  */
   if (shadedraw == 0 && linedraw == 0)
     {
      for (i = 0; i < mmats; i++)
        {
         j = npolyspermat[i];
         if (j > 0)
           {
            FREE(polygons[i]);
            FREE(polyverts[i]);
           }
        }
     }

   polysflag = shadedraw;
   linesflag = linedraw;

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void polyflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   if ( arg == 0 ) shadedraw = (int)XmToggleButtonGadgetGetState(w);
   if ( arg == 1 ) linedraw = (int)XmToggleButtonGadgetGetState(w);
}


void polyexplode(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival;
 
   XmScaleGetValue(w,&ival);
   pexplodefact = (float)ival / 100.;

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void domatorder(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   matorderpop();
}


void mat_state(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, arg;

   arg = (long int)client_data;

   XmListDeselectAllItems(pmatsellist);      

   if (arg == 1)
     {
      for (i = 0; i < numlist; i++)
        {
         XmListSelectPos(pmatsellist,i+1,FALSE);      
        }
      }
}


void setstipple(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   polystiplflag = (int)XmToggleButtonGadgetGetState(w);

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void pmatsel_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   polymatselprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (polymatselprobeflag)
     {
      polymatremprobeflag = 0;
      XmToggleButtonSetState(pmatremprobe,FALSE,FALSE);
      XmListDeselectAllItems(pmatsellist);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else
      XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


void pmatrem_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   polymatremprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (polymatremprobeflag)
     {
      polymatselprobeflag = 0;
      XmToggleButtonSetState(pmatselprobe,FALSE,FALSE);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


#define INSIDE 1
#define OUTSIDE 2
#define ON 567

void getpolyprobe(float pa, float pb, float pc, float pd, float pe, float pf,
                 int *imat, int *polyno)
{
  int i, j, k, l, m, itst, matid, polyid;
  short gopoly, *matchk;
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

   /*  Loop over selected surfaces to find the shortest  */ 
   /*  distance from the surface center to the viewer.   */
   mins = MAXVAL;
   matid = -1;
   polyid = -1;

   for (i = 0; i < maxmatno; i++)
     {     

      if (active_mat[i])
        {

         for (j = 0; j < npolyspermat[i]; j ++)
           {        

            /*  Check polygon subset and select.  */
            gopoly = 0;
            if (polysflag == 1 || linesflag == 1)
              {
               gopoly = (int)polydrawchk[i][j];
              }

            /*  If active polygon, check polygon ave. distance to line.  */
            if (gopoly)
              {

               /*  Calculate projection point of surface on the line     */
               /*  and the distance from the surface to the projection.  */
               xn = polygons[i][j].xavg;
               yn = polygons[i][j].yavg;
               zn = polygons[i][j].zavg;
               bx = xn - xl1;
               by = yn - yl1;
               bz = zn - zl1;
               s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
               xp = s*xl2 + xl1*(1.0 - s);
               yp = s*yl2 + yl1*(1.0 - s);
               zp = s*zl2 + zl1*(1.0 - s);
               dist = sqrt((xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + 
                           (zp-zn)*(zp-zn));

               /*  Check that the line is within the max polygon radius.  */
               if (dist <= polygons[i][j].maxradius)
                 {

                  /*  Get the surface normal and calculate the        */
                  /*  intersection of the probe line to the surface.  */ 
                  aa = polygons[i][j].xnorm;
                  bb = polygons[i][j].ynorm;
                  cc = polygons[i][j].znorm;
                  k = polygons[i][j].polyvertloc;
                  x1 = polyverts[i][k].x;
                  y1 = polyverts[i][k].y;
                  z1 = polyverts[i][k].z;
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
                     for (l = 0; l < polygons[i][j].nverts; l++)
                        {
                         xyzs[l].x = polyverts[i][k+l].x;
                         xyzs[l].y = polyverts[i][k+l].y;
                         xyzs[l].z = polyverts[i][k+l].z;
                        }
                     itst = ptinface(xn, yn, zn, (int)polygons[i][j].nverts,
                            xyzs, aa, bb, cc);
                     if (itst != OUTSIDE)
                       {              
                        if (s < mins)
                          {
                           matid = i;
                           polyid = j;
                           mins = s;
                          }
                       }
                    }
                 }
              }
           } 
        }
     }
   *imat = matid;
   *polyno = polyid;
}


void pmatprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int i, polyid, id, goodid, imat;

   /*  Get the polygon material and id number.  */
   getpolyprobe(pa,pb,pc,pd,pe,pf, &imat,&polyid);

   /*  Set material toggle button.  */
   id = -1;
   for (i = 0; i < numlist; i++)
     {
      if (imat == listmat[i]) id = i;
     }
   goodid = 1;
   if (id < 0 || id >= MAXMATS) goodid = 0;
   if (polymatselprobeflag && goodid)
     {
      XmListSelectPos(pmatsellist,id+1,FALSE);
     }
   if (polymatremprobeflag && goodid)
     {
      XmListDeselectPos(pmatsellist,id+1,FALSE);
     }

   XFlush(dpy);
}


void makepolygonform(parent)
 Widget parent;
{
  Widget polyclose, polyapply, orderbut, onbut, offbut, matstxt;
  void dopolysubset();
  int i;
  Arg wargs[20];
  XmString string; 
  XmString polymatitem[MAXMATS];

   string = XmStringCreate("Polygons",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   polygonform = XmCreateFormDialog(parent,"Polygons",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   polyclose = XtVaCreateManagedWidget("Polyclose",
               xmPushButtonGadgetClass,
               polygonform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(polyclose,XmNactivateCallback,plclose,NULL);

   string = XmStringCreate(" Apply ",XmSTRING_DEFAULT_CHARSET);
   polyapply = XtVaCreateManagedWidget("Polyappbut",
                xmPushButtonGadgetClass,
                polygonform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, polyclose,
                XmNtopOffset, 20,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(polyapply,XmNactivateCallback,poly_apply,
                    NULL);

   string = XmStringCreate("Shaded",
            XmSTRING_DEFAULT_CHARSET);
   pshade = XtVaCreateManagedWidget("Pshade",
            xmToggleButtonGadgetClass,
            polygonform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, polyapply,
            XmNtopOffset, 5,
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
      XtAddCallback(pshade,XmNvalueChangedCallback,polyflags,
                    (caddr_t) 0);
 
   string = XmStringCreate("Lines",
            XmSTRING_DEFAULT_CHARSET);
   pline = XtVaCreateManagedWidget("Plinebut",
           xmToggleButtonGadgetClass,
           polygonform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, pshade,
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
      XtAddCallback(pline,XmNvalueChangedCallback,polyflags,
                    (caddr_t) 1);

   /*  Create material buttons.  */
 
   string = XmStringCreate("Materials: ",XmSTRING_DEFAULT_CHARSET);
   matstxt = XtVaCreateManagedWidget("Matstxt",
             xmLabelWidgetClass,
             polygonform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, pline,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   onbut = XtVaCreateManagedWidget("Onbut",
           xmPushButtonGadgetClass,
           polygonform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, matstxt,
           XmNleftAttachment, XmATTACH_FORM,
           XmNleftOffset, 20,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(onbut,XmNactivateCallback,mat_state,
                    (caddr_t) 1);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   offbut = XtVaCreateManagedWidget("Offbut",
            xmPushButtonGadgetClass,
            polygonform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, matstxt,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, onbut,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(offbut,XmNactivateCallback,mat_state,
                    (caddr_t) 0);

   numlist = 0;
   for (i = 0; i < MAXMATS; i++)
   {
       listmat[i] = -1;
       if (mat_has_active_polys[i])
       {
           polymatitem[numlist] = XmStringLtoRCreate(mmatnames[i],
                                                     XmSTRING_DEFAULT_CHARSET);
           listmat[numlist] = i;
           numlist++;
       }
   }

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, offbut);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 20);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   if (numlist > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, numlist);
      i++;  XtSetArg(wargs[i],XmNitems, polymatitem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
   i++;
   pmatsellist = (Widget)XmCreateScrolledList(polygonform,
                                              "Pmatsellist",wargs,i);
   XtManageChild(pmatsellist);
   for (i = 0; i < numlist; i++) 
      XmStringFree(polymatitem[i]);

   /*  Create interactive mouse material selection buttons.  */

   string = XmStringLtoRCreate("Select Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   pmatselprobe = XtVaCreateManagedWidget("Pmatselprobe",
                  xmToggleButtonGadgetClass,
                  polygonform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, pmatsellist,
                  XmNtopOffset, 5,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(pmatselprobe,XmNvalueChangedCallback,
                    pmatsel_probe,NULL);
 
   string = XmStringLtoRCreate("Remove Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   pmatremprobe = XtVaCreateManagedWidget("Pmatremprobe",
                  xmToggleButtonGadgetClass,
                  polygonform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, pmatselprobe,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(pmatremprobe,XmNvalueChangedCallback,
                    pmatrem_probe,NULL);

   /*  Create explode and subset buttons.  */

   string = XmStringCreate("Explode %",XmSTRING_DEFAULT_CHARSET);
   pexplode = XtVaCreateManagedWidget("Pexplode",
              xmScaleWidgetClass,
              polygonform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, pmatremprobe,
              XmNtopOffset, 20,
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
      XtAddCallback(pexplode,XmNvalueChangedCallback,
                    polyexplode,NULL);
 
   string = XmStringCreate("Subset",XmSTRING_DEFAULT_CHARSET);
   psubset = XtVaCreateManagedWidget("Psubset",
             xmPushButtonGadgetClass,
             polygonform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, pexplode,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(psubset,XmNactivateCallback,dopolysubset,NULL);
  
   string = XmStringCreate("Mat Order",XmSTRING_DEFAULT_CHARSET);
   orderbut = XtVaCreateManagedWidget("Orderbut",
              xmPushButtonGadgetClass,
              polygonform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, psubset,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(orderbut,XmNactivateCallback,domatorder,NULL);

   string = XmStringCreate("Use Stipple Patterns",XmSTRING_DEFAULT_CHARSET);
   stipplebut = XtVaCreateManagedWidget("Stipplebut",
                xmToggleButtonGadgetClass,
                polygonform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, orderbut,
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
      XtAddCallback(stipplebut,XmNvalueChangedCallback,
                    setstipple,(caddr_t) 0);

   makepolysubsetform(polygonform);
   makematorderform(polygonform);
}

