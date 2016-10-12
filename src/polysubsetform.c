#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget psubxminsl, psubxmaxsl, psubyminsl, psubymaxsl, psubzminsl,
              psubzmaxsl, psubxminsltxt, psubxmaxsltxt, psubyminsltxt,
              psubymaxsltxt, psubzminsltxt, psubzmaxsltxt;
static int fastboxcount = 0;


void polysubsetformpop()
{
  void setpolysubsetwidgets();

   setpolysubsetwidgets();
   MyManageChild(polysubsetform);
}

void psubsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];

   polyselbox_draw = 1;
   arg = (long int)client_data;
 
   if (arg < 12)
     {
      XmScaleGetValue(w,&ival);
      val = (float) ival / 1000.;
     }
 
   if (arg == 0 || arg == 6)
     {
      xminbdval = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%#11.7g",xminbdval);
      XtVaSetValues(psubxminsltxt,XmNvalue,str,NULL);
      if (xminbdval > xmaxbdval)
        {
         XmScaleSetValue(psubxmaxsl,ival);
         XtVaSetValues(psubxmaxsltxt,XmNvalue,str,NULL);
         xmaxbdval = xminbdval;
        }
     }
   if (arg == 1 || arg == 7)
     {
      xmaxbdval = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%#11.7g",xmaxbdval);
      XtVaSetValues(psubxmaxsltxt,XmNvalue,str,NULL);
      if (xminbdval > xmaxbdval)
        {
         XmScaleSetValue(psubxminsl,ival);
         XtVaSetValues(psubxminsltxt,XmNvalue,str,NULL);
         xminbdval = xmaxbdval;
        }
     }
   if (arg == 2 || arg == 8)
     {
      yminbdval = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%#11.7g",yminbdval);
      XtVaSetValues(psubyminsltxt,XmNvalue,str,NULL);
      if (yminbdval > ymaxbdval)
        {
         XmScaleSetValue(psubymaxsl,ival);
         XtVaSetValues(psubymaxsltxt,XmNvalue,str,NULL);
         ymaxbdval = yminbdval;
        }
     }
   if (arg == 3 || arg == 9)
     {
      ymaxbdval = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%#11.7g",ymaxbdval);
      XtVaSetValues(psubymaxsltxt,XmNvalue,str,NULL);
      if (yminbdval > ymaxbdval)
        {
         XmScaleSetValue(psubyminsl,ival);
         XtVaSetValues(psubyminsltxt,XmNvalue,str,NULL);
         yminbdval = ymaxbdval;
        }
     }
   if (arg == 4 || arg == 10)
     {
      zminbdval = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%#11.7g",zminbdval);
      XtVaSetValues(psubzminsltxt,XmNvalue,str,NULL);
      if (zminbdval > zmaxbdval)
        {
         XmScaleSetValue(psubzmaxsl,ival);
         XtVaSetValues(psubzmaxsltxt,XmNvalue,str,NULL);
         zmaxbdval = zminbdval;
        }
     }
   if (arg == 5 || arg == 11)
     {
      zmaxbdval = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%#11.7g",zmaxbdval);
      XtVaSetValues(psubzmaxsltxt,XmNvalue,str,NULL);
      if (zminbdval > zmaxbdval)
        {
         XmScaleSetValue(psubzminsl,ival);
         XtVaSetValues(psubzminsltxt,XmNvalue,str,NULL);
         zminbdval = zmaxbdval;
        }
     }
   if (arg == 12)
     {
      polyselbox_draw = 0;
      expose_redraw();
      XtUnmanageChild(polysubsetform);
     }
   
   if (arg < 12)
     {
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
}


void psltxtread(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, ival;
  char *tmpstr, str[20];
  float val;
  void setpolysubset();

   polyselbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      xminbdval = val;
      if (xminbdval > xmaxbdval) xmaxbdval = xminbdval;
     }
   if (arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      xmaxbdval = val;
      if (xminbdval > xmaxbdval) xminbdval = xmaxbdval;
     }
 
   if (arg == 2)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      yminbdval = val;
      if (yminbdval > ymaxbdval) ymaxbdval = yminbdval;
     }
   if (arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      ymaxbdval = val;
      if (yminbdval > ymaxbdval) yminbdval = ymaxbdval;
     }
 
   if (arg == 4)
     {
      if (val < globxicmin) val = globzicmin;
      if (val > globxicmax) val = globzicmax;
      zminbdval = val;
      if (zminbdval > zmaxbdval) zmaxbdval = zminbdval;
     }
   if (arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      zmaxbdval = val;
      if (zminbdval > zmaxbdval) zminbdval = zmaxbdval;
     }

   XtFree(tmpstr);

   setpolysubset();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void psubsetreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void setpolysubsetwidgets();
  void setpolysubset();
 
   xminbdval = globxicmin; xmaxbdval = globxicmax;
   yminbdval = globyicmin; ymaxbdval = globyicmax;
   zminbdval = globzicmin; zmaxbdval = globzicmax;

   setpolysubsetwidgets();
   setpolysubset();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void psubsetapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float val;
  char *tmpstr;
  void setpolysubset();

   /*  Re-read textfields.  */
   tmpstr = XmTextFieldGetString(psubxminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xminbdval = val;
   if (xminbdval > xmaxbdval)
      xmaxbdval = xminbdval;

   tmpstr = XmTextFieldGetString(psubxmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xmaxbdval = val;
   if (xminbdval > xmaxbdval)
      xminbdval = xmaxbdval;
 
   tmpstr = XmTextFieldGetString(psubyminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   yminbdval = val;
   if (yminbdval > ymaxbdval)
      ymaxbdval = yminbdval;

   tmpstr = XmTextFieldGetString(psubymaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   ymaxbdval = val;
   if (yminbdval > ymaxbdval)
      yminbdval = ymaxbdval;
 
   tmpstr = XmTextFieldGetString(psubzminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zminbdval = val;
   if (zminbdval > zmaxbdval)
      zmaxbdval = zminbdval;
 
   tmpstr = XmTextFieldGetString(psubzmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zmaxbdval = val;
   if (zminbdval > zmaxbdval)
      zminbdval = zmaxbdval;

   /*  Set data and draw.  */
   polyselbox_draw = 0;
   setpolysubset();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void makepolysubsetform(parent)
 Widget parent;
{
  Widget pxminlabel, pxmaxlabel, pyminlabel, pymaxlabel, pzminlabel,
         pzmaxlabel, psubclose, psubreset, psubapply;
  int imin, imax, i;
  Arg wargs[10];
  XmString string; 

   string = XmStringCreate("Polygon Subset",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   polysubsetform = XmCreateFormDialog(parent,"Polysubsetform",wargs,i);
      XmStringFree(string);

   imin = 0;
   imax = 1000;
   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   pxminlabel = XtVaCreateManagedWidget("Pxminlabel",
                xmLabelWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   psubxminsl = XtVaCreateManagedWidget("Psubxminsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pxminlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pxminlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imin,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubxminsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 0);
      XtAddCallback(psubxminsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 6);
   psubxminsltxt = XtVaCreateManagedWidget("Psubxminsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pxminlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubxminsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubxminsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   pxmaxlabel = XtVaCreateManagedWidget("Pxmaxlabel",
                xmLabelWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, psubxminsltxt,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   psubxmaxsl = XtVaCreateManagedWidget("Psubxmaxsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pxmaxlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pxmaxlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imax,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubxmaxsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 1);
      XtAddCallback(psubxmaxsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 7);
   psubxmaxsltxt = XtVaCreateManagedWidget("Psubxmaxsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pxmaxlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubxmaxsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubxmaxsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   pyminlabel = XtVaCreateManagedWidget("Pyminlabel",
               xmLabelWidgetClass,
               polysubsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, psubxminsltxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   psubyminsl = XtVaCreateManagedWidget("Psubyminsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pyminlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pyminlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imin,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubyminsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 2);
      XtAddCallback(psubyminsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 8);
   psubyminsltxt = XtVaCreateManagedWidget("Psubyminsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pyminlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubyminsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubyminsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   pymaxlabel = XtVaCreateManagedWidget("Ppymaxlabel",
                xmLabelWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, psubxmaxsltxt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, psubyminsltxt,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   psubymaxsl = XtVaCreateManagedWidget("Psubymaxsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pymaxlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pymaxlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imax,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubymaxsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 3);
      XtAddCallback(psubymaxsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 9);
   psubymaxsltxt = XtVaCreateManagedWidget("Psubymaxsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pymaxlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubymaxsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubymaxsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   pzminlabel = XtVaCreateManagedWidget("Pzminlabel",
                xmLabelWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, psubyminsltxt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   psubzminsl = XtVaCreateManagedWidget("Psubzminsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pzminlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pzminlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imin,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubzminsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 4);
      XtAddCallback(psubzminsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 10);
   psubzminsltxt = XtVaCreateManagedWidget("Psubzminsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pzminlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubzminsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubzminsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   pzmaxlabel = XtVaCreateManagedWidget("Pzmaxlabel",
                xmLabelWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, psubymaxsltxt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, psubzminsltxt,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
   psubzmaxsl = XtVaCreateManagedWidget("Psubzmaxsl",
                xmScaleWidgetClass,
                polysubsetform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, pzmaxlabel,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, pzmaxlabel,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, imin,
                XmNmaximum, imax,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, imax,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(psubzmaxsl,XmNvalueChangedCallback,psubsetsl,
                    (caddr_t) 5);
      XtAddCallback(psubzmaxsl,XmNdragCallback,psubsetsl,
                    (caddr_t) 11);
   psubzmaxsltxt = XtVaCreateManagedWidget("Psubzmaxsltxt",
                   xmTextFieldWidgetClass,
                   polysubsetform,
                   XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNtopWidget, pzmaxlabel,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, psubzmaxsl,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 11,
                   XmNvalue, "",
                   NULL);
      XtAddCallback(psubzmaxsltxt,XmNactivateCallback,
                    psltxtread,(caddr_t) 5);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   psubclose = XtVaCreateManagedWidget("Psubclose",
               xmPushButtonGadgetClass,
               polysubsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, psubzminsltxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(psubclose,XmNactivateCallback,psubsetsl,
                    (caddr_t) 12);
 
   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   psubreset = XtVaCreateManagedWidget("Psubreset",
               xmPushButtonGadgetClass,
               polysubsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, psubzminsltxt,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, psubclose,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(psubreset,XmNactivateCallback,psubsetreset,
                    NULL);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   psubapply = XtVaCreateManagedWidget("Psubapply",
               xmPushButtonGadgetClass,
               polysubsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, psubzminsltxt,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, psubreset,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(psubapply,XmNactivateCallback,psubsetapply,
                    NULL);
}


void setpolysubsetwidgets()

{
  int ival;
  double val;
  char str[20];

   XtRemoveCallback(psubxminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 0);
   XtRemoveCallback(psubxmaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 1);
   XtRemoveCallback(psubyminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 2);
   XtRemoveCallback(psubymaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 3);
   XtRemoveCallback(psubzminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 4);
   XtRemoveCallback(psubzmaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 5);
   XFlush(dpy);

   val = (xminbdval - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(psubxminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xminbdval);
   XtVaSetValues(psubxminsltxt,XmNvalue,str,NULL);
 
   val = (xmaxbdval - globxicmin) / (globxicmax - globxicmin);
   ival = val * 1000.;
   XtVaSetValues(psubxmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xmaxbdval);
   XtVaSetValues(psubxmaxsltxt,XmNvalue,str,NULL);
 
   val = (yminbdval - globyicmin) / (globyicmax-globyicmin);
   ival = val * 1000.;
   XtVaSetValues(psubyminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",yminbdval);
   XtVaSetValues(psubyminsltxt,XmNvalue,str,NULL);
 
   val = (ymaxbdval - globyicmin) / (globyicmax-globyicmin);
   ival = val * 1000.;
   XtVaSetValues(psubymaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",ymaxbdval);
   XtVaSetValues(psubymaxsltxt,XmNvalue,str,NULL);
 
   val = (zminbdval - globzicmin) / (globzicmax-globzicmin);
   ival = val * 1000.;
   XtVaSetValues(psubzminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zminbdval);
   XtVaSetValues(psubzminsltxt,XmNvalue,str,NULL);
 
   val = (zmaxbdval - globzicmin) / (globzicmax-globzicmin);
   ival = val * 1000.;
   XtVaSetValues(psubzmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zmaxbdval);
   XtVaSetValues(psubzmaxsltxt,XmNvalue,str,NULL);

   XFlush(dpy);
   XtAddCallback(psubxminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 0);
   XtAddCallback(psubxmaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 1);
   XtAddCallback(psubyminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 2);
   XtAddCallback(psubymaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 3);
   XtAddCallback(psubzminsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 4);
   XtAddCallback(psubzmaxsl,XmNvalueChangedCallback,psubsetsl,(caddr_t) 5);
   XFlush(dpy);
}


void setpolysubset()

{
  int i, j, k, m;
  long nv;
  double xck, yck, zck; 
 
   if (polysflag == 0 && linesflag == 0)
      readpolyfile();

   for (i = 0; i < maxmatno; i++)
     {     

      for (j = 0; j < npolyspermat[i]; j ++)
        {        
         polydrawchk[i][j] = 0;

         m = (int)polygons[i][j].nverts;
         nv = polygons[i][j].polyvertloc;
         for (k = 0; k < m; k++)
           {
            xck = polyverts[i][k+nv].x;
            yck = polyverts[i][k+nv].y;
            zck = polyverts[i][k+nv].z;

            if (xminbdval <= xck && xck <= xmaxbdval &&            
                yminbdval <= yck && yck <= ymaxbdval && 
                zminbdval <= zck && zck <= zmaxbdval) 
               polydrawchk[i][j] = 1;

            if (polydrawchk[i][j] == 1) break;
           }
        }
     }

   if (polysflag == 0 && linesflag == 0)
     {
      for (i = 0; i < MAXMATS; i++)
        {
         FREE(polygons[i]);
         FREE(polyverts[i]);
        }
     }
}
