#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <time.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

#define MAXVAL 9.99999e32

static Widget cnvxswinform, cnvxdisplab[2];

static int nconcavecells, *concavecells;
void concavetest();


void concaveformpop()
{
   MyManageChild(concaveform);
}


void concaveformclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(concaveform);
}


void concaveapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ii, id;
  char str[60];
  XmString string1, string2, string;
  void cnvxcalc(), cnvxclearwin();

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Clear window.  */
   cnvxclearwin();

   /*  Check unstructured mesh for concave faces, and reset      */
   /*  order so that vertex that causes the concavity is first.  */
   if (structuredflag == 1)
     {
      strcpy(str,
         "Concave test will only be\nperformed on unsturctured grids.");
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cnvxdisplab[0],XmNlabelString,string,NULL);
      XmStringFree(string);
      return;
     }
   concavetest();

   /*  Fill the label with the concave cell data.  */
   if (nconcavecells == 0)
     {
      strcpy(str,"No cells with concave faces found.");
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cnvxdisplab[0],XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   else
     {
      sprintf(str,"There are %d cells\nwith concave faces:",nconcavecells);
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cnvxdisplab[0],XmNlabelString,string,NULL);
      XmStringFree(string);

      strcpy(str,"");
      string1 = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      for (i = 0; i < nconcavecells; i++)
        {
         ii = concavecells[i];
         id = ii + 1;
         if (cellidflag) id = cellids[ii];
         sprintf(str," %d\n",id);
         string2 = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
         string = XmStringConcat(string1,string2);
         XmStringFree(string1);
         XmStringFree(string2);
         string1 = XmStringCopy(string);
         XmStringFree(string);
        }
      XtVaSetValues(cnvxdisplab[1],XmNlabelString,string1,NULL);
      XmStringFree(string1);
      FREE(concavecells);
      XmUpdateDisplay(cnvxdisplab[0]);
      XmUpdateDisplay(cnvxdisplab[1]);
     }


   XUndefineCursor(dpy,topwin);
   XFlush(dpy);
}


void makeconcaveform(parent)
 Widget parent;
{
  Widget cnvxclose, cnvxapply, cnvxlab1, cnvxswin, cnvxsep, lastwidget;
  char str[20];
  char hdrstr[] = "List cells that have concave\n or highly warped faces";
  int i, nv;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Test Concave",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   concaveform = XmCreateFormDialog(parent,"Concaveform",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cnvxclose = XtVaCreateManagedWidget("Cnvxclose",
                  xmPushButtonGadgetClass,
                  concaveform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cnvxclose,XmNactivateCallback,
                    concaveformclose,NULL);

   string = XmStringCreateLtoR(hdrstr,
            XmSTRING_DEFAULT_CHARSET);
   cnvxlab1 = XtVaCreateManagedWidget("Cnvxlab1",
               xmLabelWidgetClass,
               concaveform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cnvxclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cnvxapply = XtVaCreateManagedWidget("Cnvxapply",
               xmPushButtonGadgetClass,
               concaveform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cnvxlab1,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cnvxapply,XmNactivateCallback,
                    concaveapply,NULL);

   /*  Create convex cell labels.  */
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cnvxapply);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 260);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   cnvxswin = XmCreateScrolledWindow(concaveform,"Cnvxswin",wargs,i);
   XtManageChild(cnvxswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cnvxswinform = XmCreateForm(cnvxswin,"Cnvxswinform",wargs,i);
   XtManageChild(cnvxswinform);

   cnvxsep = XtVaCreateManagedWidget("Cnvxsep",
             xmSeparatorGadgetClass,
             cnvxswinform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNseparatorType, XmNO_LINE,
             NULL);

   lastwidget = cnvxsep;
   for (i = 0; i < 2; i++)
     {
      sprintf(str,"Cnvxdisplab%d",i);
      string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
      cnvxdisplab[i] = XtVaCreateManagedWidget(str,
                       xmLabelWidgetClass,
                       cnvxswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       NULL);
         XmStringFree(string);
      lastwidget = cnvxdisplab[i];
     }
}



void cnvxclearwin()
{
  int i;
  char str[20];
  Widget lastwidget;
  XmString string;

   for (i = 0; i < 2; i++)
     {
      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cnvxdisplab[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(cnvxdisplab[i]);
     }
}



void concavetest()
{
/*  Test to see if any faces are concave, if so, reorder the vertices  */
/*  so that the point that causes the concavity is first in the list.  */
/*  This will allow the concave face to be rendered correctly          */

  double x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double norm[3], d;
  int i, iface, iv, nv, k0, k1, k2, i2, i3, inorm, nvchk, ibeg;
  int tmpverts[1000], sign[1000], ichk, ichk2, convert;
  short *tmpcells;

  time_t time1, time2;
  double duration;

   nconcavecells = 0;

   tmpcells = (short *)malloc(ncells * sizeof(short));
   if (tmpcells == NULL)
     {
      printf("Not enough memory for concave test.\n");
      return;
     }
   for (i = 0; i < ncells; i++) tmpcells[i] = 0;

   time1 = time(NULL);

   /*  For 2D meshes start the test at 2D faces.  */
   ibeg = 0;
   if (mesh2dflag > 0) ibeg = ncell2dfaces;

   /*  Loop over the faces.  */
   for (iface = ibeg; iface < ncellfaces; iface++)
     {

      /*  Loop over the vertics of the face and calculate the cross product.  */
      nv = cellfaces[iface].nverts;

      /*  Skip if face has less than 2 points.  */
      if (nv < 3) continue;

      ichk = 0;
      inorm = -1;
      nvchk = 0;
      for (iv = 0; iv < nv; iv++)
        {
         k1 = cellfaces[iface].fverts[iv];
         tmpverts[iv] = k1;
         i2 = iv - 1;
         i3 = iv + 1;
         if (iv == 0) i2 = nv - 1;
         if (iv == nv - 1) i3 = 0;
         k0 = cellfaces[iface].fverts[i2];
         k2 = cellfaces[iface].fverts[i3];
         x1 = nodes[k1].x;
         y1 = nodes[k1].y;
         z1 = nodes[k1].z;
         x2 = nodes[k0].x;
         y2 = nodes[k0].y;
         z2 = nodes[k0].z;
         x3 = nodes[k2].x;
         y3 = nodes[k2].y;
         z3 = nodes[k2].z;
         norm[0] = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         norm[1] = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         norm[2] = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);

         /*  If all components of the normal are zero, then the      */
         /*  points are colinear or two of the points are the same.  */
         /*  Skip.                                                   */
         if (fabs(norm[0]) < myzero && fabs(norm[1]) < myzero &&
             fabs(norm[2]) < myzero) continue;

         /*  Find the largest component of the normal to use  */
         /*  to check for sign change, one time only.         */
         if (inorm == -1)
           {
            inorm = 0;
            d = fabs(norm[0]);
            for (i = 1; i < 3; i++)
              {
               if (fabs(norm[i]) > d)
                 {
                  d = fabs(norm[i]);
                 inorm = i;
                 }
              }
           }
         d = norm[inorm];
         sign[iv] = 1;
         if (d < 0) sign[iv] = -1;
         ichk += sign[iv];
         nvchk++;
        }

      /*  See if all vertices have the same sign.  */
      if (abs(ichk) != nvchk)
        {

         /*  Tag cells that use this face.  */
         i = cellfaces[iface].cell1;
         if (i > -1) tmpcells[i] = 1;
         i = cellfaces[iface].cell2;
         if (i > -1) tmpcells[i] = 1;

         /*  Find the first vertex that has an opposite sign.  */
         ichk2 = -1;
         if (ichk < 0) ichk2 = 1;
         convert = -1;
         for (iv = 0; iv < nv; iv++)
           {
            if (sign[iv] == ichk2) convert = iv;
            if (convert > -1) break;
           }

         /*  Reorder the vertices, starting with the convex vertex.  */
         for (iv = 0; iv < nv; iv++)
           {
            k1 = convert + iv;
            if (k1 > nv - 1) k1 = k1 - nv;
            cellfaces[iface].fverts[iv] = tmpverts[k1];
           }
        } 
     }
   time2 = time(NULL);
   duration = difftime(time2,time1);
   printf("concavetest took %f seconds\n",duration);

   for (i = 0; i < ncells; i++) nconcavecells += tmpcells[i];
   if (nconcavecells > 0)
     {
      concavecells = (int *)malloc(nconcavecells * sizeof(int));
      iv = 0;
      for (i = 0; i < ncells; i++)
        {
         if (tmpcells[i] == 1)
           {
            concavecells[iv] = i;
            iv++;
           }
        }
     }

   FREE(tmpcells);
}
