#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/DrawingA.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

static Widget clipform, fclipsl, bclipsl, fcliptxt, bcliptxt,
              titleform, titletxt, titletxt2, ncontourform, ncontourtxt,
              interactform, interactsl, lightform, clrbartitletxt; 


/*  Front and back clip control callbacks.  */

void pop_clip()
{
   MyManageChild(clipform);
}


void close_clip(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(clipform);
}


void clipper(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval, arg;

   XmScaleGetValue(w,&inval);
   arg = (long int)client_data;
   if (arg < 2) frontclipvalue = (float) (inval) / 100.;
   else backclipvalue = (float) (inval) / 100.;

   if (arg == 0 || arg == 2) fastdraw = 0;
   if (arg == 1 || arg == 3) fastdraw = 1;
   if (displistflag) drawnewlistflag = 1;
   drawit();
}


/* Title callbacks.  */

void pop_title(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XmTextFieldSetString(titletxt,titlestr);
   XmTextFieldSetString(titletxt2,titlestr2);
   MyManageChild(titleform);
}


void set_title(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr;

   tmpstr = XmTextFieldGetString(titletxt);
   strcpy(titlestr,tmpstr);
   XtFree(tmpstr);

   tmpstr = XmTextFieldGetString(titletxt2);
   strcpy(titlestr2,tmpstr);
   XtFree(tmpstr);
  
   arg = (long int)client_data;

   if (arg == 0) titleflag = 1;
   if (arg == 1) titleflag = 0;

   XtUnmanageChild(titleform);

   /* if (displistflag && titleflag < 2) drawnewlistflag = 1; */
   fastdraw = 0;
   drawit();
}


/*  Number of contours callbacks.  */

void pop_ncontours(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  char str[4];

   sprintf(str,"%d",nset_contours);
   XmTextFieldSetString(ncontourtxt,str);
   MyManageChild(ncontourform);
}


void ncontourset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr;

   arg = (long int)client_data;

   if (arg == 0)
     {
      tmpstr = XmTextFieldGetString(ncontourtxt);
      nset_contours = atoi(tmpstr);
      XtFree(tmpstr);

      if (fieldcolortype == 3) setfieldcolors();

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }

   if (arg == 1)
      XtUnmanageChild(ncontourform);
}


/*  Interacitvity control callbacks.  */

void pop_interact()
{
  int intval;

   intval = drawincr;
   XmScaleSetValue(interactsl,intval);  
   MyManageChild(interactform);
}


void interactivity(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval;

   XmScaleGetValue(w,&inval);
   drawincr = inval;
   if (displistflag) drawnewlistflag = 1;
}


void close_inter(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(interactform);
}


/*  Light callbacks.  */

void pop_light()
{
   MyManageChild(lightform);
}


void close_light(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(lightform);
}


/* Color bar title callbacks.  */

void pop_clrbartitle(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XmTextFieldSetString(clrbartitletxt,clrbartitlestr);
   MyManageChild(clrbartitleform);
}


void set_clrbartitle(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr;

   tmpstr = XmTextFieldGetString(clrbartitletxt);
   strcpy(clrbartitlestr,tmpstr);
   XtFree(tmpstr);
   XtUnmanageChild(clrbartitleform);

   fastdraw = 0;
   drawit();
}


void clrbartitleclose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(clrbartitleform);
}


void makemisc_ctlform(parent)
 Widget parent;
{
  Widget clipclose, titlelab, titlelab2, titledisp, titledel, titleclose,
         ncontlab, ncontclose, interclose, lightclose, clrbartitlelab,
         clrbartitleapply, corbartitleclose;
  int i;
  Arg args[20];
  XmString string;
  void lpexpose(), lpbpress(), lpmotion(), lpbrelease();

   /*  Create front and back clip popup form.  */
   string = XmStringCreate("Clip",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   clipform = XmCreateFormDialog(parent,"Clipform",args,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   clipclose = XtVaCreateManagedWidget("Clipclose",
               xmPushButtonGadgetClass,
               clipform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(clipclose,XmNactivateCallback,close_clip,NULL);

   string = XmStringCreate("FrontClip",XmSTRING_DEFAULT_CHARSET);
   fcliptxt = XtVaCreateManagedWidget("Fcliptxt",
              xmLabelWidgetClass,
              clipform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, clipclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   fclipsl = XtVaCreateManagedWidget("Fclipsl",
             xmScaleWidgetClass,
             clipform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, clipclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, fcliptxt,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, 0,
             XmNmaximum, 100,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNscaleWidth, 250,
             XmNvalue, 0,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(fclipsl,XmNvalueChangedCallback,clipper,
                    (caddr_t) 0);
      XtAddCallback(fclipsl,XmNdragCallback,clipper,
                    (caddr_t) 1);
 
   string = XmStringCreate("BackClip ",XmSTRING_DEFAULT_CHARSET);
   bcliptxt = XtVaCreateManagedWidget("Bcliptxt",
              xmLabelWidgetClass,
              clipform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fcliptxt,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   bclipsl = XtVaCreateManagedWidget("Bclipsl",
             xmScaleWidgetClass,
             clipform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fcliptxt,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, bcliptxt,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, 30,
             XmNmaximum, 100,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNscaleWidth, 250,
             XmNvalue, 100,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(bclipsl,XmNvalueChangedCallback,clipper,
                    (caddr_t) 2);
      XtAddCallback(bclipsl,XmNdragCallback,clipper,
                    (caddr_t) 3);

   /*  Create title popup form.  */
   string = XmStringCreate("Title",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   titleform = XmCreateFormDialog(poplevel,"Titleform",
                                  args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("Top Title:   ",
                           XmSTRING_DEFAULT_CHARSET);
   titlelab = XtVaCreateManagedWidget("Titlelab",
              xmLabelWidgetClass,
              titleform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   titletxt = XtVaCreateManagedWidget("Titletxt",
              xmTextFieldWidgetClass,
              titleform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, titlelab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmaxLength, 40,
              XmNcolumns, 40,
              XmNvalue, "",
              NULL);
 
   string = XmStringCreate("Bottom Title:",
                           XmSTRING_DEFAULT_CHARSET);
   titlelab2 = XtVaCreateManagedWidget("Titlelab2",
               xmLabelWidgetClass,
               titleform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, titletxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   titletxt2 = XtVaCreateManagedWidget("Titletxt2",
               xmTextFieldWidgetClass,
               titleform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, titletxt,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, titlelab2,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmaxLength, 40,
               XmNcolumns, 40,
               XmNvalue, "",
               NULL);
 
   string = XmStringCreate("Display",XmSTRING_DEFAULT_CHARSET);
   titledisp = XtVaCreateManagedWidget("Titledisp",
               xmPushButtonGadgetClass,
               titleform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, titletxt2,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(titledisp,XmNactivateCallback,set_title,
                    (caddr_t) 0);
 
   string = XmStringCreate("Delete",XmSTRING_DEFAULT_CHARSET);
   titledel = XtVaCreateManagedWidget("Titledel",
              xmPushButtonGadgetClass,
              titleform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, titletxt2,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, titledisp,
              XmNleftOffset, 40,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(titledel,XmNactivateCallback,set_title,
                    (caddr_t) 1);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   titleclose = XtVaCreateManagedWidget("Titleclose",
                xmPushButtonGadgetClass,
                titleform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, titletxt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, titledel,
                XmNleftOffset, 40,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(titleclose,XmNactivateCallback,set_title,
                    (caddr_t) 2);


   /*  Create number of contour levels popup form.  */
   string = XmStringCreate("Contour levels",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   ncontourform = XmCreateFormDialog(poplevel,"Ncontourform",
                                     args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("Number of Contour Levels:",
                           XmSTRING_DEFAULT_CHARSET);
   ncontlab = XtVaCreateManagedWidget("Ncontlab",
              xmLabelWidgetClass,
              ncontourform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   ncontourtxt = XtVaCreateManagedWidget("Ncontourtxt",
                 xmTextFieldWidgetClass,
                 ncontourform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ncontlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmaxLength, 3,
                 XmNcolumns, 3,
                 XmNvalue, " 10",
                 NULL);
      XtAddCallback(ncontourtxt,XmNactivateCallback,
                    ncontourset,(caddr_t) 0);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   ncontclose = XtVaCreateManagedWidget("Ncontclose",
                xmPushButtonGadgetClass,
                ncontourform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, ncontourtxt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(ncontclose,XmNactivateCallback,ncontourset,
                    (caddr_t) 1);
 
   /*  Make interactivity form and slider.  */
   string = XmStringCreate("Interactivity control",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   interactform = XmCreateFormDialog(poplevel,"Interactform",
                                     args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   interclose = XtVaCreateManagedWidget("Interclose",
                xmPushButtonGadgetClass,
                interactform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(interclose,XmNactivateCallback,close_inter,
                    NULL);

   interactsl = XtVaCreateManagedWidget("interactsl",
                xmScaleWidgetClass,
                interactform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, interclose,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, 1,
                XmNmaximum, 100,
                XmNvalue, 10,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNscaleWidth, 250,
                XmNscaleMultiple, 1,
                NULL);
      XtAddCallback(interactsl,XmNvalueChangedCallback,
                    interactivity,NULL);
 
   /*  Create light form and drawing area.  */
   string = XmStringCreate("Light control",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   lightform = XmCreateFormDialog(poplevel,"Lightform",
                                     args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   lightclose = XtVaCreateManagedWidget("Lightclose",
                xmPushButtonGadgetClass,
                lightform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(lightclose,XmNactivateCallback,close_light,
                    NULL);

   objlight = XtVaCreateManagedWidget("Objlight",
              xmDrawingAreaWidgetClass,
              lightform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, lightclose,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNbackground, whitepix,
              XmNforeground, redpix,
              XmNwidth, 100,
              XmNheight, 100,
              XmNrecomputeSize, FALSE,
              NULL);
      XtAddCallback(objlight,XmNexposeCallback,lpexpose,
                    NULL);
      XtAddEventHandler(objlight,ButtonPressMask,FALSE,
                        lpbpress,NULL);
      XtAddEventHandler(objlight,ButtonMotionMask,FALSE,
                        lpmotion,NULL);
      XtAddEventHandler(objlight,ButtonReleaseMask,FALSE,
                        lpbrelease,NULL);

   /*  Create color string title popup form.  */
   string = XmStringCreate("Color Bar Title",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   clrbartitleform = XmCreateFormDialog(poplevel,"Clrbartitleform",
                                  args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("Color Bar title :   ",
                           XmSTRING_DEFAULT_CHARSET);
   clrbartitlelab = XtVaCreateManagedWidget("Clrbartitlelab",
              xmLabelWidgetClass,
              clrbartitleform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
 
   clrbartitletxt = XtVaCreateManagedWidget("Clrbartitletxt",
              xmTextFieldWidgetClass,
              clrbartitleform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, clrbartitlelab,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNmaxLength, 40,
              XmNcolumns, 40,
              XmNvalue, "",
              NULL);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   clrbartitleapply = XtVaCreateManagedWidget("Clrbartitleapply",
               xmPushButtonGadgetClass,
               clrbartitleform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, clrbartitletxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(clrbartitleapply,XmNactivateCallback,set_clrbartitle,
                    (caddr_t) 0);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   corbartitleclose = XtVaCreateManagedWidget("Clrbartitleclose",
                xmPushButtonGadgetClass,
                clrbartitleform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, clrbartitletxt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, clrbartitleapply,
                XmNleftOffset, 40,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(corbartitleclose,XmNactivateCallback,clrbartitleclose,
                    (caddr_t) 0);
}
