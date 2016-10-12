#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>
#include <Xm/TextF.h>

#include <stdlib.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget bbonbut, bbcoordbut, bbxminsl, bbxmaxsl, bbyminsl,
              bbymaxsl, bbzminsl, bbzmaxsl, bbxminsltxt,
              bbxmaxsltxt, bbyminsltxt, bbymaxsltxt, bbzminsltxt,
              bbzmaxsltxt;


void boundingboxpop()
{
  void setboundingboxwidgets();

   XmToggleButtonGadgetSetState(bbonbut,boundingboxflag,FALSE);
   XmToggleButtonGadgetSetState(bbcoordbut,bboxcoordflag,FALSE);
   setboundingboxwidgets();
   MyManageChild(boundingboxform);
}


void boundingboxsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];
 
   arg = (long int)client_data;
 
   if (arg < 6 || arg > 8)
     {
      XmScaleGetValue(w,&ival);
      val = (float) ival / 1000.;
     }
 
   if (arg == 0 || arg == 9)
     {
      xminbbval = (xmaxbbvalo-xminbbvalo) * val + xminbbvalo;
      sprintf(str,"%#11.7g",xminbbval);
      XtVaSetValues(bbxminsltxt,XmNvalue,str,NULL);
      if (xminbbval > xmaxbbval)
        {
         XmScaleSetValue(bbxmaxsl,ival);
         XtVaSetValues(bbxmaxsltxt,XmNvalue,str,NULL);
         xmaxbbval = xminbbval;
        }
     }
   if (arg == 1 || arg == 10)
     {
      xmaxbbval = (xmaxbbvalo-xminbbvalo) * val + xminbbvalo;
      sprintf(str,"%#11.7g",xmaxbbval);
      XtVaSetValues(bbxmaxsltxt,XmNvalue,str,NULL);
      if (xminbbval > xmaxbbval)
        {
         XmScaleSetValue(bbxminsl,ival);
         XtVaSetValues(bbxminsltxt,XmNvalue,str,NULL);
         xminbbval = xmaxbbval;
        }
     }
   if (arg == 2 || arg == 11)
     {
      yminbbval = (ymaxbbvalo-yminbbvalo) * val + yminbbvalo;
      sprintf(str,"%#11.7g",yminbbval);
      XtVaSetValues(bbyminsltxt,XmNvalue,str,NULL);
      if (yminbbval > ymaxbbval)
        {
         XmScaleSetValue(bbymaxsl,ival);
         XtVaSetValues(bbymaxsltxt,XmNvalue,str,NULL);
         ymaxbbval = yminbbval;
        }
     }
   if (arg == 3 || arg == 12)
     {
      ymaxbbval = (ymaxbbvalo-yminbbvalo) * val + yminbbvalo;
      sprintf(str,"%#11.7g",ymaxbbval);
      XtVaSetValues(bbymaxsltxt,XmNvalue,str,NULL);
      if (yminbbval > ymaxbbval)
        {
         XmScaleSetValue(bbyminsl,ival);
         XtVaSetValues(bbyminsltxt,XmNvalue,str,NULL);
         yminbbval = ymaxbbval;
        }
     }
   if (arg == 4 || arg == 13)
     {
      zminbbval = (zmaxbbvalo-zminbbvalo) * val + zminbbvalo;
      sprintf(str,"%#11.7g",zminbbval);
      XtVaSetValues(bbzminsltxt,XmNvalue,str,NULL);
      if (zminbbval > zmaxbbval)
        {
         XmScaleSetValue(bbzmaxsl,ival);
         XtVaSetValues(bbzmaxsltxt,XmNvalue,str,NULL);
         zmaxbbval = zminbbval;
        }
     }
   if (arg == 5 || arg == 14)
     {
      zmaxbbval = (zmaxbbvalo-zminbbvalo) * val + zminbbvalo;
      sprintf(str,"%#11.7g",zmaxbbval);
      XtVaSetValues(bbzmaxsltxt,XmNvalue,str,NULL);
      if (zminbbval > zmaxbbval)
        {
         XmScaleSetValue(bbzminsl,ival);
         XtVaSetValues(bbzminsltxt,XmNvalue,str,NULL);
         zminbbval = zmaxbbval;
        }
     }
   if (arg == 6)
     {
      boundingboxflag = (short)XmToggleButtonGadgetGetState(w);
     }
   if (arg == 7)
     {
      bboxcoordflag = (short)XmToggleButtonGadgetGetState(w);
     }
   if (arg == 8)
     {
      XtUnmanageChild(boundingboxform);
     }
   
   if (arg > 8)
     {
      fastdraw = 1;
      drawit();
     }
   if (arg < 8)
     {
      fastdraw = 0;
      drawit();
     }
}


void boundingboxtxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float val;
  char *tmpstr;
  void setboundingboxwidgets();

   /*  Read all textfields.  */
   tmpstr = XmTextFieldGetString(bbxminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xminbbval = val;

   tmpstr = XmTextFieldGetString(bbxmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xmaxbbval = val;
   if (xminbbval > xmaxbbval)
      xminbbval = xmaxbbval;
 
   tmpstr = XmTextFieldGetString(bbyminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   yminbbval = val;

   tmpstr = XmTextFieldGetString(bbymaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   ymaxbbval = val;
   if (yminbbval > ymaxbbval)
      yminbbval = ymaxbbval;
 
   tmpstr = XmTextFieldGetString(bbzminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zminbbval = val;
 
   tmpstr = XmTextFieldGetString(bbzmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zmaxbbval = val;
   if (zminbbval > zmaxbbval)
      zminbbval = zmaxbbval;

   setboundingboxwidgets();

   fastdraw = 0;
   drawit();
}

void makeboundingboxform(parent)
 Widget parent;
{
  Widget xminbblab, xmaxbblab, yminbblab, ymaxbblab, zminbblab,
         zmaxbblab, bbclose;
  int imin, imax, i;
  Arg wargs[10];
  XmString string; 

   string = XmStringCreate("Bounding Box",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   boundingboxform = XmCreateFormDialog(parent,"BoundingBox",wargs,i);
      XmStringFree(string);

   imin = 0;
   imax = 1000;
   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   xminbblab = XtVaCreateManagedWidget("Xminbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbxminsl = XtVaCreateManagedWidget("Bbxminbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, xminbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, xminbblab,
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
      XtAddCallback(bbxminsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 0);
      XtAddCallback(bbxminsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 9);
   bbxminsltxt = XtVaCreateManagedWidget("Bbxminbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, xminbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbxminsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbxminsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   xmaxbblab = XtVaCreateManagedWidget("Xmaxbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, bbxminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbxmaxsl = XtVaCreateManagedWidget("Bbxmaxbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, xmaxbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, xmaxbblab,
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
      XtAddCallback(bbxmaxsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 1);
      XtAddCallback(bbxmaxsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 10);
   bbxmaxsltxt = XtVaCreateManagedWidget("Bbxmaxbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, xmaxbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbxmaxsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbxmaxsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   yminbblab = XtVaCreateManagedWidget("Yminbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, xminbblab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbyminsl = XtVaCreateManagedWidget("Bbyminbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, yminbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, yminbblab,
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
      XtAddCallback(bbyminsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 2);
      XtAddCallback(bbyminsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 11);
   bbyminsltxt = XtVaCreateManagedWidget("Bbyminbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, yminbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbyminsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbyminsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   ymaxbblab = XtVaCreateManagedWidget("Ymaxbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, xmaxbblab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, bbyminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbymaxsl = XtVaCreateManagedWidget("Bbymaxbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, ymaxbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, ymaxbblab,
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
      XtAddCallback(bbymaxsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 3);
      XtAddCallback(bbymaxsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 12);
   bbymaxsltxt = XtVaCreateManagedWidget("Bbymaxbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, ymaxbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbymaxsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbymaxsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   zminbblab = XtVaCreateManagedWidget("Zminbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, yminbblab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbzminsl = XtVaCreateManagedWidget("Bbzminbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, zminbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, zminbblab,
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
      XtAddCallback(bbzminsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 4);
      XtAddCallback(bbzminsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 13);
   bbzminsltxt = XtVaCreateManagedWidget("Bbzminbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, zminbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbzminsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbzminsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   zmaxbblab = XtVaCreateManagedWidget("Zmaxbblab",
               xmLabelWidgetClass,
               boundingboxform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, ymaxbblab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, bbzminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   bbzmaxsl = XtVaCreateManagedWidget("Bbzmaxbb",
              xmScaleWidgetClass,
              boundingboxform,
              XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNtopWidget, zmaxbblab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, zmaxbblab,
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
      XtAddCallback(bbzmaxsl,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 5);
      XtAddCallback(bbzmaxsl,XmNdragCallback,boundingboxsl,
                    (caddr_t) 14);
   bbzmaxsltxt = XtVaCreateManagedWidget("Bbzmaxbbtxt",
                 xmTextFieldWidgetClass,
                 boundingboxform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, zmaxbblab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, bbzmaxsl,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 11,
                 XmNvalue, "",
                 NULL);
      XtAddCallback(bbzmaxsltxt,XmNactivateCallback,
                    boundingboxtxt,(caddr_t) 0);
  
   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   bbonbut = XtVaCreateManagedWidget("Bbonbut",
             xmToggleButtonGadgetClass,
             boundingboxform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, zminbblab,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(bbonbut,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 6);
  
   string = XmStringCreate("Show Coordinates",XmSTRING_DEFAULT_CHARSET);
   bbcoordbut = XtVaCreateManagedWidget("Bbcoordbut",
                xmToggleButtonGadgetClass,
                boundingboxform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, zminbblab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, bbonbut,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNspacing, 2,
                XmNselectColor, yellowpix,
                NULL);
      XmStringFree(string);
      XtAddCallback(bbcoordbut,XmNvalueChangedCallback,boundingboxsl,
                    (caddr_t) 7);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   bbclose = XtVaCreateManagedWidget("Bbclose",
             xmPushButtonGadgetClass,
             boundingboxform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, zminbblab,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, bbcoordbut,
             XmNleftOffset, 20,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bbclose,XmNactivateCallback,boundingboxsl,
                    (caddr_t) 8);

}


void setboundingboxwidgets()

{
  int ival;
  float val;
  char str[20];

   XtRemoveCallback(bbxminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 0);
   XtRemoveCallback(bbxmaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 1);
   XtRemoveCallback(bbyminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 2);
   XtRemoveCallback(bbymaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 3);
   XtRemoveCallback(bbzminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 4);
   XtRemoveCallback(bbzmaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 5);
   XFlush(dpy);

   val = (xminbbval - xminbbvalo) / (xmaxbbvalo-xminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbxminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xminbbval);
   XtVaSetValues(bbxminsltxt,XmNvalue,str,NULL);

   val = (xmaxbbval - xminbbvalo) / (xmaxbbvalo-xminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbxmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xmaxbbval);
   XtVaSetValues(bbxmaxsltxt,XmNvalue,str,NULL);
 
   val = (yminbbval - yminbbvalo) / (ymaxbbvalo-yminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbyminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",yminbbval);
   XtVaSetValues(bbyminsltxt,XmNvalue,str,NULL);
 
   val = (ymaxbbval - yminbbvalo) / (ymaxbbvalo-yminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbymaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",ymaxbbval);
   XtVaSetValues(bbymaxsltxt,XmNvalue,str,NULL);
 
   val = (zminbbval - zminbbvalo) / (zmaxbbvalo-zminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbzminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zminbbval);
   XtVaSetValues(bbzminsltxt,XmNvalue,str,NULL);
 
   val = (zmaxbbval - zminbbvalo) / (zmaxbbvalo-zminbbvalo);
   ival = (int) (val * 1000.);
   XtVaSetValues(bbzmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zmaxbbval);
   XtVaSetValues(bbzmaxsltxt,XmNvalue,str,NULL);

   XFlush(dpy);
   XtAddCallback(bbxminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 0);
   XtAddCallback(bbxmaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 1);
   XtAddCallback(bbyminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 2);
   XtAddCallback(bbymaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 3);
   XtAddCallback(bbzminsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 4);
   XtAddCallback(bbzmaxsl,XmNvalueChangedCallback,boundingboxsl,(caddr_t) 5);
   XFlush(dpy);
}
