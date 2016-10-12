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

static Widget subxminsl, subxmaxsl, subyminsl, subymaxsl, subzminsl,
              subzmaxsl, subxminsltxt, subxmaxsltxt, subyminsltxt,
              subymaxsltxt, subzminsltxt, subzmaxsltxt;
static int fastboxcount = 0;


void subsetformpop()
{
  void setsubsetwidgets();

   setsubsetwidgets();
   MyManageChild(subsetform);
}

void subsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];
  void setnodecellsubset();

   subsetbox_draw = 1;
   arg = (long int)client_data;
 
   if (arg < 12)
     {
      XmScaleGetValue(w,&ival);
      val = (float) ival / 1000.;
     }
 
   if (arg == 0 || arg == 6)
     {
      xminfdval = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%#11.7g",xminfdval);
      XtVaSetValues(subxminsltxt,XmNvalue,str,NULL);
      if (xminfdval > xmaxfdval)
        {
         XmScaleSetValue(subxmaxsl,ival);
         XtVaSetValues(subxmaxsltxt,XmNvalue,str,NULL);
         xmaxfdval = xminfdval;
        }
     }
   if (arg == 1 || arg == 7)
     {
      xmaxfdval = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%#11.7g",xmaxfdval);
      XtVaSetValues(subxmaxsltxt,XmNvalue,str,NULL);
      if (xminfdval > xmaxfdval)
        {
         XmScaleSetValue(subxminsl,ival);
         XtVaSetValues(subxminsltxt,XmNvalue,str,NULL);
         xminfdval = xmaxfdval;
        }
     }
   if (arg == 2 || arg == 8)
     {
      yminfdval = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%#11.7g",yminfdval);
      XtVaSetValues(subyminsltxt,XmNvalue,str,NULL);
      if (yminfdval > ymaxfdval)
        {
         XmScaleSetValue(subymaxsl,ival);
         XtVaSetValues(subymaxsltxt,XmNvalue,str,NULL);
         ymaxfdval = yminfdval;
        }
     }
   if (arg == 3 || arg == 9)
     {
      ymaxfdval = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%#11.7g",ymaxfdval);
      XtVaSetValues(subymaxsltxt,XmNvalue,str,NULL);
      if (yminfdval > ymaxfdval)
        {
         XmScaleSetValue(subyminsl,ival);
         XtVaSetValues(subyminsltxt,XmNvalue,str,NULL);
         yminfdval = ymaxfdval;
        }
     }
   if (arg == 4 || arg == 10)
     {
      zminfdval = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%#11.7g",zminfdval);
      XtVaSetValues(subzminsltxt,XmNvalue,str,NULL);
      if (zminfdval > zmaxfdval)
        {
         XmScaleSetValue(subzmaxsl,ival);
         XtVaSetValues(subzmaxsltxt,XmNvalue,str,NULL);
         zmaxfdval = zminfdval;
        }
     }
   if (arg == 5 || arg == 11)
     {
      zmaxfdval = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%#11.7g",zmaxfdval);
      XtVaSetValues(subzmaxsltxt,XmNvalue,str,NULL);
      if (zminfdval > zmaxfdval)
        {
         XmScaleSetValue(subzminsl,ival);
         XtVaSetValues(subzminsltxt,XmNvalue,str,NULL);
         zminfdval = zmaxfdval;
        }
     }
   if (arg == 12)
     {
      subsetbox_draw = 0;
      expose_redraw();
      XtUnmanageChild(subsetform);
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


void sltxtread(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr, str[20];
  float val;
  void setsubsetwidgets();

   subsetbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      xminfdval = val;
      if (xminfdval > xmaxfdval) xmaxfdval = xminfdval;
     }
   if (arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      xmaxfdval = val;
      if (xminfdval > xmaxfdval) xminfdval = xmaxfdval;
     }
 
   if (arg == 2)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      yminfdval = val;
      if (yminfdval > ymaxfdval) ymaxfdval = yminfdval;

     }
   if (arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      ymaxfdval = val;
      if (yminfdval > ymaxfdval) yminfdval = ymaxfdval;
     }
 
   if (arg == 4)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      zminfdval = val;
      if (zminfdval > zmaxfdval) zmaxfdval = zminfdval;
     }
   if (arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      zmaxfdval = val;
      if (zminfdval > zmaxfdval) zminfdval = zmaxfdval;
     }

   XtFree(tmpstr);

   setsubsetwidgets();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void subsetreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void setsubsetwidgets();
  void setnodecellsubset();
 
   xminfdval = globxicmin; xmaxfdval = globxicmax;
   yminfdval = globyicmin; ymaxfdval = globyicmax;
   zminfdval = globzicmin; zmaxfdval = globzicmax;

   setsubsetwidgets();
   setnodecellsubset();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void subsetapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float val;
  char *tmpstr;
  void setsubsetwidgets();
  void setnodecellsubset();

   /*  Re-read textfields.  */
   tmpstr = XmTextFieldGetString(subxminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xminfdval = val;
   if (xminfdval > xmaxfdval)
      xmaxfdval = xminfdval;

   tmpstr = XmTextFieldGetString(subxmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globxicmin) val = globxicmin;
   if (val > globxicmax) val = globxicmax;
   xmaxfdval = val;
   if (xminfdval > xmaxfdval)
      xminfdval = xmaxfdval;
 
   tmpstr = XmTextFieldGetString(subyminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   yminfdval = val;
   if (yminfdval > ymaxfdval)
      ymaxfdval = yminfdval;

   tmpstr = XmTextFieldGetString(subymaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globyicmin) val = globyicmin;
   if (val > globyicmax) val = globyicmax;
   ymaxfdval = val;
   if (yminfdval > ymaxfdval)
      yminfdval = ymaxfdval;
 
   tmpstr = XmTextFieldGetString(subzminsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zminfdval = val;
   if (zminfdval > zmaxfdval)
      zmaxfdval = zminfdval;
 
   tmpstr = XmTextFieldGetString(subzmaxsltxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < globzicmin) val = globzicmin;
   if (val > globzicmax) val = globzicmax;
   zmaxfdval = val;
   if (zminfdval > zmaxfdval)
      zminfdval = zmaxfdval;

   /*  Set data and draw.  */
   subsetbox_draw = 0;
   setsubsetwidgets();
   setnodecellsubset();
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void makesubsetform(parent)
 Widget parent;
{
  Widget xminlabel, xmaxlabel, yminlabel, ymaxlabel, zminlabel,
         zmaxlabel, subclose, subreset, subapply;
  int imin, imax, i;
  Arg wargs[10];
  XmString string; 

   string = XmStringCreate("Subset - Nodes, Cells, Particles",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   subsetform = XmCreateFormDialog(parent,"Subset",wargs,i);
      XmStringFree(string);

   imin = 0;
   imax = 1000;
   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   xminlabel = XtVaCreateManagedWidget("Xminlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subxminsl = XtVaCreateManagedWidget("Subxminsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, xminlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, xminlabel,
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
      XtAddCallback(subxminsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 0);
      XtAddCallback(subxminsl,XmNdragCallback,subsetsl,
                    (caddr_t) 6);
   subxminsltxt = XtVaCreateManagedWidget("Subxminsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, xminlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subxminsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subxminsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   xmaxlabel = XtVaCreateManagedWidget("Xmaxlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, subxminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subxmaxsl = XtVaCreateManagedWidget("Subxmaxsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, xmaxlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, xmaxlabel,
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
      XtAddCallback(subxmaxsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 1);
      XtAddCallback(subxmaxsl,XmNdragCallback,subsetsl,
                    (caddr_t) 7);
   subxmaxsltxt = XtVaCreateManagedWidget("Subxmaxsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, xmaxlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subxmaxsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subxmaxsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   yminlabel = XtVaCreateManagedWidget("Yminlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, subxminsltxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subyminsl = XtVaCreateManagedWidget("Subyminsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, yminlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, yminlabel,
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
      XtAddCallback(subyminsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 2);
      XtAddCallback(subyminsl,XmNdragCallback,subsetsl,
                    (caddr_t) 8);
   subyminsltxt = XtVaCreateManagedWidget("Subyminsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, yminlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subyminsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subyminsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   ymaxlabel = XtVaCreateManagedWidget("Ymaxlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, subxmaxsltxt,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, subyminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subymaxsl = XtVaCreateManagedWidget("Subymaxsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, ymaxlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, ymaxlabel,
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
      XtAddCallback(subymaxsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 3);
      XtAddCallback(subymaxsl,XmNdragCallback,subsetsl,
                    (caddr_t) 9);
   subymaxsltxt = XtVaCreateManagedWidget("Subymaxsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, ymaxlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subymaxsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subymaxsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   zminlabel = XtVaCreateManagedWidget("Zminlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, subyminsltxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subzminsl = XtVaCreateManagedWidget("Subzminsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, zminlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, zminlabel,
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
      XtAddCallback(subzminsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 4);
      XtAddCallback(subzminsl,XmNdragCallback,subsetsl,
                    (caddr_t) 10);
   subzminsltxt = XtVaCreateManagedWidget("Subzminsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, zminlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subzminsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subzminsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   zmaxlabel = XtVaCreateManagedWidget("Zmaxlabel",
               xmLabelWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, subymaxsltxt,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, subzminsltxt,
               XmNleftOffset, 20,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   subzmaxsl = XtVaCreateManagedWidget("Subzmaxsl",
               xmScaleWidgetClass,
               subsetform,
               XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
               XmNtopWidget, zmaxlabel,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, zmaxlabel,
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
      XtAddCallback(subzmaxsl,XmNvalueChangedCallback,subsetsl,
                    (caddr_t) 5);
      XtAddCallback(subzmaxsl,XmNdragCallback,subsetsl,
                    (caddr_t) 11);
   subzmaxsltxt = XtVaCreateManagedWidget("Subzmaxsltxt",
                  xmTextFieldWidgetClass,
                  subsetform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, zmaxlabel,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, subzmaxsl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(subzmaxsltxt,XmNactivateCallback,
                    sltxtread,(caddr_t) 5);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   subclose = XtVaCreateManagedWidget("Subclose",
              xmPushButtonGadgetClass,
              subsetform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, subzminsltxt,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(subclose,XmNactivateCallback,subsetsl,
                    (caddr_t) 12);
 
   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   subreset = XtVaCreateManagedWidget("Subreset",
              xmPushButtonGadgetClass,
              subsetform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, subzminsltxt,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, subclose,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(subreset,XmNactivateCallback,subsetreset,
                    NULL);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   subapply = XtVaCreateManagedWidget("Subapply",
              xmPushButtonGadgetClass,
              subsetform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, subzminsltxt,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, subreset,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(subapply,XmNactivateCallback,subsetapply,
                    NULL);
}


void setsubsetwidgets()

{
  int ival;
  double val;
  char str[20];

   XtRemoveCallback(subxminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 0);
   XtRemoveCallback(subxmaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 1);
   XtRemoveCallback(subyminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 2);
   XtRemoveCallback(subymaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 3);
   XtRemoveCallback(subzminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 4);
   XtRemoveCallback(subzmaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 5);
   XFlush(dpy);

   val = (xminfdval - globxicmin) / (globxicmax - globxicmin);
   ival = val * 1000.;
   XtVaSetValues(subxminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xminfdval);
   XtVaSetValues(subxminsltxt,XmNvalue,str,NULL);
 
   val = (xmaxfdval - globxicmin) / (globxicmax - globxicmin);
   ival = val * 1000.;
   XtVaSetValues(subxmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",xmaxfdval);
   XtVaSetValues(subxmaxsltxt,XmNvalue,str,NULL);
 
   val = (yminfdval - globyicmin) / (globyicmax-globyicmin);
   ival = val * 1000.;
   XtVaSetValues(subyminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",yminfdval);
   XtVaSetValues(subyminsltxt,XmNvalue,str,NULL);
 
   val = (ymaxfdval - globyicmin) / (globyicmax-globyicmin);
   ival = val * 1000.;
   XtVaSetValues(subymaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",ymaxfdval);
   XtVaSetValues(subymaxsltxt,XmNvalue,str,NULL);
 
   val = (zminfdval - globzicmin) / (globzicmax-globzicmin);
   ival = val * 1000.;
   XtVaSetValues(subzminsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zminfdval);
   XtVaSetValues(subzminsltxt,XmNvalue,str,NULL);
 
   val = (zmaxfdval - globzicmin) / (globzicmax-globzicmin);
   ival = val * 1000.;
   XtVaSetValues(subzmaxsl, XmNvalue,ival, NULL);
   sprintf(str,"%#11.7g",zmaxfdval);
   XtVaSetValues(subzmaxsltxt,XmNvalue,str,NULL);

   XFlush(dpy);
   XtAddCallback(subxminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 0);
   XtAddCallback(subxmaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 1);
   XtAddCallback(subyminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 2);
   XtAddCallback(subymaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 3);
   XtAddCallback(subzminsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 4);
   XtAddCallback(subzmaxsl,XmNvalueChangedCallback,subsetsl,(caddr_t) 5);
   XFlush(dpy);
}


void setnodecellsubset()
{
  int subchk, ip, icell, jcell, isurf;

   /*  Check and set node subset flags.  */
   for (ip = 1; ip < nnodes+1; ip++)
     {
      subchk = 1;
      if (nodes[ip].x > xmaxfdval) subchk = 0;
      if (nodes[ip].x < xminfdval) subchk = 0;
      if (nodes[ip].y > ymaxfdval) subchk = 0;
      if (nodes[ip].y < yminfdval) subchk = 0;
      if (nodes[ip].z > zmaxfdval) subchk = 0;
      if (nodes[ip].z < zminfdval) subchk = 0;
      nodesubsetflag[ip] = subchk;
     }

   /*  Check and set cell subset flags.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  Fill cell structure if structured grid.  */
      jcell = icell;
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,0);
        }
      subchk = 1;
      if (cells[jcell].xavg > xmaxfdval) subchk = 0;
      if (cells[jcell].xavg < xminfdval) subchk = 0;
      if (cells[jcell].yavg > ymaxfdval) subchk = 0;
      if (cells[jcell].yavg < yminfdval) subchk = 0;
      if (cells[jcell].zavg > zmaxfdval) subchk = 0;
      if (cells[jcell].zavg < zminfdval) subchk = 0;
      cellsubsetflag[icell] = subchk;
     }

   if (cellsflag) setfacesel();

   /*  Check and set surface subset flags.  */
   for (isurf = 0; isurf < nsurf; isurf++)
     {
      subchk = 1;
      if (surfs[isurf].xavg > xmaxfdval) subchk = 0;
      if (surfs[isurf].xavg < xminfdval) subchk = 0;
      if (surfs[isurf].yavg > ymaxfdval) subchk = 0;
      if (surfs[isurf].yavg < yminfdval) subchk = 0;
      if (surfs[isurf].zavg > zmaxfdval) subchk = 0;
      if (surfs[isurf].zavg < zminfdval) subchk = 0;
      surfsubsetflag[isurf] = subchk;
     }
}
