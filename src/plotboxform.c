#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/PushBG.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
 
static Widget glxmin, glxmax, glymin, glymax, glzmin, glzmax;
static float  vxmin, vxmax, vymin, vymax, vzmin, vzmax;


void plotboxformpop()
{
  char str[20];

   sprintf(str,"%#11.7g",globxicmin);
   XmTextFieldSetString(glxmin,str);
   sprintf(str,"%#11.7g",globxicmax);
   XmTextFieldSetString(glxmax,str);

   sprintf(str,"%#11.7g",globyicmin);
   XmTextFieldSetString(glymin,str);
   sprintf(str,"%#11.7g",globyicmax);
   XmTextFieldSetString(glymax,str);

   sprintf(str,"%#11.7g",globzicmin);
   XmTextFieldSetString(glzmin,str);
   sprintf(str,"%#11.7g",globzicmax);
   XmTextFieldSetString(glzmax,str);

   MyManageChild(plotboxform);
}

 
void plotbox(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ierr, ierrx, ierry, ierrz, arg;
  char *tmpstr;
  void setplotbox();
 
   arg = (int)client_data;
 
   if (arg < 6)
     {
      ierr = 0;
     }
 
   if (arg == 6)
     {
      ierrx = 0;
      tmpstr = XmTextFieldGetString(glxmin);
      vxmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(glxmax);
      vxmax = atof(tmpstr);
      XtFree(tmpstr);
      if (vxmin > vxmax) 
        {
         XmTextFieldSetString(glxmin,"Error");
         ierrx = 1;
        }
      ierry = 0;
      tmpstr = XmTextFieldGetString(glymin);
      vymin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(glymax);
      vymax = atof(tmpstr);
      XtFree(tmpstr);
      if (vymin > vymax) 
        {
         XmTextFieldSetString(glymin,"Error");
         ierry = 1;
        }
      ierrz = 0;
      tmpstr = XmTextFieldGetString(glzmin);
      vzmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(glzmax);
      vzmax = atof(tmpstr);
      XtFree(tmpstr);
      if (vzmin > vzmax) 
        {
         XmTextFieldSetString(glzmin,"Error");
         ierrz = 1;
        }
 
      ierr = ierrx + ierry + ierrz;
      if (ierr == 0)
        {
         globxicmino = globxicmin;
         globxicmaxo = globxicmax;
         globyicmino = globyicmin;
         globyicmaxo = globyicmax;
         globzicmino = globzicmin;
         globzicmaxo = globzicmax;
         globxicmin = vxmin;
         globxicmax = vxmax;
         globyicmin = vymin;
         globyicmax = vymax;
         globzicmin = vzmin;
         globzicmax = vzmax;
         setplotbox();
         /* if (displistflag) drawnewlistflag = 1; */
         fastdraw = 0;
         drawit();
        }
 
     }
 
   if (arg == 7)
     {
      XtUnmanageChild(plotboxform);
     }

}

 
void plotboxreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void setplotbox();

   /*  Reset the plot box to the input data. */
   globxicmino = globxicmin;
   globxicmaxo = globxicmax;
   globyicmino = globyicmin;
   globyicmaxo = globyicmax;
   globzicmino = globzicmin;
   globzicmaxo = globzicmax;
   globxicmin = probxicmin;
   globxicmax = probxicmax;
   globyicmin = probyicmin;
   globyicmax = probyicmax;
   globzicmin = probzicmin;
   globzicmax = probzicmax;

   setplotbox();
   /* if (displistflag) drawnewlistflag = 1; */
   fastdraw = 0;
   drawit();
} 

void makeplotboxform(parent)
 Widget parent;
{
  Widget xmnlab, xmxlab, ymnlab, ymxlab, zmnlab, zmxlab,
         plotboxdoit, plotboxclose, plotboxrset;
  Arg wargs[20];
  int i;
  char str[20];
  XmString string;
 
 
   string = XmStringCreate("Set Plot Box",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   plotboxform = XmCreateFormDialog(parent,"PlotBox",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("X min:",XmSTRING_DEFAULT_CHARSET);
   xmnlab = XtVaCreateManagedWidget("Xmnlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",0.);
   glxmin = XtVaCreateManagedWidget("Glxmin",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, xmnlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glxmin,XmNactivateCallback,plotbox,
                    (caddr_t) 0);
 
   string = XmStringCreate("X max:",XmSTRING_DEFAULT_CHARSET);
   xmxlab = XtVaCreateManagedWidget("Xmxlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, glxmin,
            XmNleftOffset, 10,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",1.);
   glxmax = XtVaCreateManagedWidget("Glxmax",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, xmxlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glxmax,XmNactivateCallback,plotbox,
                    (caddr_t) 1);
 
   string = XmStringCreate("Y min:",XmSTRING_DEFAULT_CHARSET);
   ymnlab = XtVaCreateManagedWidget("Ymnlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glxmin,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",0.);
   glymin = XtVaCreateManagedWidget("Glymin",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glxmin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, ymnlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glymin,XmNactivateCallback,plotbox,
                    (caddr_t) 2);
 
   string = XmStringCreate("Y max:",XmSTRING_DEFAULT_CHARSET);
   ymxlab = XtVaCreateManagedWidget("Ymxlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glxmin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, glymin,
            XmNleftOffset, 10,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",1.);
   glymax = XtVaCreateManagedWidget("Glymax",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glxmin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, ymxlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glymax,XmNactivateCallback,plotbox,
                    (caddr_t) 3);
 
   string = XmStringCreate("Z min:",XmSTRING_DEFAULT_CHARSET);
   zmnlab = XtVaCreateManagedWidget("Zmnlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glymin,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",0.);
   glzmin = XtVaCreateManagedWidget("Glzmin",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glymin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, zmnlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glzmin,XmNactivateCallback,plotbox,
                    (caddr_t) 4);
 
   string = XmStringCreate("Z max:",XmSTRING_DEFAULT_CHARSET);
   zmxlab = XtVaCreateManagedWidget("Zmxlab",xmLabelWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glymin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, glzmin,
            XmNleftOffset, 10,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   sprintf(str,"%#9.3g",1.);
   glzmax = XtVaCreateManagedWidget("Glzmax",xmTextFieldWidgetClass,
            plotboxform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, glymin,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, zmxlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNmarginHeight, 0,
            XmNmarginWidth, 1,
            XmNmaxLength, 12,
            XmNcolumns, 12,
            XmNvalue, str,
            NULL);
      XtAddCallback(glzmax,XmNactivateCallback,plotbox,
                    (caddr_t) 5);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   plotboxclose = XtVaCreateManagedWidget("Plotboxclose",
                  xmPushButtonGadgetClass,
                  plotboxform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, glzmin,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(plotboxclose,XmNactivateCallback,plotbox,
                    (caddr_t) 7);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   plotboxdoit = XtVaCreateManagedWidget("Plotboxdoit",
                 xmPushButtonGadgetClass,
                 plotboxform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, glzmin,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, plotboxclose,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(plotboxdoit,XmNactivateCallback,plotbox,
                    (caddr_t) 6);
 
   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   plotboxrset = XtVaCreateManagedWidget("Plotboxrset",
                 xmPushButtonGadgetClass,
                 plotboxform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, glzmin,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, plotboxdoit,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(plotboxrset,XmNactivateCallback,plotboxreset,
                    NULL); 
}
 

void setplotbox()
 
{
  char str[10];

   /*  Reset plotbox widets.  */
   sprintf(str,"%#11.7g",globxicmin);
   XmTextFieldSetString(glxmin,str);
   sprintf(str,"%#11.7g",globxicmax);
   XmTextFieldSetString(glxmax,str);

   sprintf(str,"%#11.7g",globyicmin);
   XmTextFieldSetString(glymin,str);
   sprintf(str,"%#11.7g",globyicmax);
   XmTextFieldSetString(glymax,str);

   sprintf(str,"%#11.7g",globzicmin);
   XmTextFieldSetString(glzmin,str);
   sprintf(str,"%#11.7g",globzicmax);
   XmTextFieldSetString(glzmax,str);
  
   /*  Reset global mins, maxs and picture scale  */
   /*  data, unless called from getattributes.    */ 
   xrange = globxicmax - globxicmin;
   yrange = globyicmax - globyicmin;
   zrange = globzicmax - globzicmin;
   span = sqrt(xrange*xrange + yrange*yrange + zrange*zrange);
 
   near_clip = 1000.0 * span;
   far_clip = -0.1 * span;
 
   /*  Reset center and center sliders.  */
   xtrans = -0.5 * (globxicmax+globxicmin);
   ytrans = -0.5 * (globyicmax+globyicmin);
   ztrans = -0.5 * (globzicmax+globzicmin);
 
   setcenterwidgets();

   /*  Reset subset edges and sliders.  */
   xminfdval = globxicmin;
   xmaxfdval = globxicmax;
   yminfdval = globyicmin;
   ymaxfdval = globyicmax;
   zminfdval = globzicmin;
   zmaxfdval = globzicmax;

   setsubsetwidgets();

   /*  Reset node and cell subset flags.  */
   setnodecellsubset();

   /*  Reset node, cell select and surface box selection data.  */
   cellselbox_xmin = globxicmin;
   cellselbox_xmax = globxicmax;
   cellselbox_ymin = globyicmin;
   cellselbox_ymax = globyicmax;
   cellselbox_zmin = globzicmin;
   cellselbox_zmax = globzicmax;
   nodeselbox_xmin = globxicmin;
   nodeselbox_xmax = globxicmax;
   nodeselbox_ymin = globyicmin;
   nodeselbox_ymax = globyicmax;
   nodeselbox_zmin = globzicmin;
   nodeselbox_zmax = globzicmax;
   surfselbox_xmin = globxicmin;
   surfselbox_xmax = globxicmax;
   surfselbox_ymin = globyicmin;
   surfselbox_ymax = globyicmax;
   surfselbox_zmin = globzicmin;
   surfselbox_zmax = globzicmax;
   traceselbox_xmin = globxicmin;
   traceselbox_xmax = globxicmax;
   traceselbox_ymin = globyicmin;
   traceselbox_ymax = globyicmax;
   traceselbox_zmin = globzicmin;
   traceselbox_zmax = globzicmax;

   /*  Reset polygon boundary cutoff data and widgets.  */
   xminbdval = globxicmin;
   xmaxbdval = globxicmax;
   yminbdval = globyicmin;
   ymaxbdval = globyicmax;
   zminbdval = globzicmin;
   zmaxbdval = globzicmax;

   if (polysin > 0)
     {
      setpolysubsetwidgets();
      setpolysubset();
     }

   /*  Reset bounding box sliders.  */
   xminbbvalo = xminbbval;
   xmaxbbvalo = xmaxbbval;
   yminbbvalo = yminbbval;
   ymaxbbvalo = ymaxbbval;
   zminbbvalo = zminbbval;
   zmaxbbvalo = zmaxbbval;
   xminbbval = globxicmin;
   xmaxbbval = globxicmax;
   yminbbval = globyicmin;
   ymaxbbval = globyicmax;
   zminbbval = globzicmin;
   zmaxbbval = globzicmax;
   
   setboundingboxwidgets();

}
