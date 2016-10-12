
#ifdef NT
#include <windows.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

static Widget red, green, blue, result, trans, getcol, setcol, defcol,
             matbuts[MAXMATS], contbuts[MAXCONTOURS+2];
static int mr, mg, mb, getcolflag, setcolflag, defcolflag, maxmatbuts;
static float transpar;


void coloreditformpop()
{
  float redcol, greencol, bluecol;
  unsigned long matpix;
  int matpixid;

   redcol = (float)mr/255.0;
   greencol = (float)mg/255.0;
   bluecol = (float)mb/255.0;
   matpixid = 0;
   matpix = mat_pixels[matpixid];
   mat_color.pixel = matpix;
   mat_color.flags = DoRed | DoGreen | DoBlue;
   mat_color.red = (unsigned short)(redcol * 65535.);
   mat_color.green = (unsigned short)(greencol * 65535.);
   mat_color.blue = (unsigned short)(bluecol * 65535.);
   if (truecolorcmap)
     {
      XAllocColor(dpy,my_cmap,&mat_color);
      matpix = mat_color.pixel;
      mat_pixels[matpixid] = matpix;
      XtVaSetValues(result,XmNbackground,matpix,NULL);
     }
   else
      XStoreColor(dpy,my_cmap,&mat_color);

   MyManageChild(coloreditform);
}


void coloreditclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(coloreditform);
}


void coloredit_docolor(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float redcol, greencol, bluecol;
  unsigned long matpix;
  int ival, matpixid;

   XmScaleGetValue(red,&mr);
   XmScaleGetValue(green,&mg);
   XmScaleGetValue(blue,&mb);

   redcol = (float)mr/255.0;
   greencol = (float)mg/255.0;
   bluecol = (float)mb/255.0;
   matpixid = 0;
   matpix = mat_pixels[matpixid];
   mat_color.pixel = matpix;
   mat_color.flags = DoRed | DoGreen | DoBlue;
   mat_color.red = (unsigned short)(redcol * 65535.);
   mat_color.green = (unsigned short)(greencol * 65535.);
   mat_color.blue = (unsigned short)(bluecol * 65535.);
   if (truecolorcmap)
     {
      XAllocColor(dpy,my_cmap,&mat_color);
      matpix = mat_color.pixel;
      mat_pixels[matpixid] = matpix;
      XtVaSetValues(result,XmNbackground,matpix,NULL);
     }
   else
      XStoreColor(dpy,my_cmap,&mat_color);

   transpar = 1.;
   XmScaleGetValue(trans,&ival);
   transpar = (float) ival / 100.;
}


void coloredit_select_active_mat(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float redcol, greencol, bluecol;
  int i, j, arg, mt, matpixid, r, g, b, colindx;
  unsigned long matpix;
  unsigned long getbestcolor(float red, float green, float blue);

   arg = (long int)client_data;
   j = arg * 4;

   getcolflag = (int)XmToggleButtonGadgetGetState(getcol);
   setcolflag = (int)XmToggleButtonGadgetGetState(setcol);
   defcolflag = (int)XmToggleButtonGadgetGetState(defcol);

   for (i = 0; i < MAXCONTOURS+2; i++)
     {
      XmToggleButtonSetState(contbuts[i],FALSE,FALSE);
     }
   for (i = 0; i < maxmatbuts; i++)
     {
      XmToggleButtonGadgetSetState(matbuts[i],FALSE,FALSE);
     }
   XmToggleButtonGadgetSetState(matbuts[arg],TRUE,FALSE);

   if (setcolflag == 1 || defcolflag == 1)
     {
      if (setcolflag == 1)
        { r = mr;  g = mg;  b = mb; }
      if (defcolflag == 1)
        {
         colindx = (arg)%32;
         r = (int) (kcolors[colindx] * 255.);
         g = (int) (kcolors[32+colindx] * 255.);
         b = (int) (kcolors[64+colindx] * 255);
        }

      redcol = (float)r/255.0;
      greencol = (float)g/255.0;
      bluecol = (float)b/255.0;

      matrgba[0+j] = redcol;
      matrgba[1+j] = greencol;
      matrgba[2+j] = bluecol;
      matrgba[3+j] = transpar;
      matcolor[arg] = ((r<<24) | (g << 16) | (b << 8) | (255));

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }

   if (getcolflag == 1)
     {
      mr = (int)(matrgba[0+j] * 255.);
      mg = (int)(matrgba[1+j] * 255.);
      mb = (int)(matrgba[2+j] * 255.);
      mt = (int)(matrgba[3+j]*100.);

      redcol = (float)mr/255.0;
      greencol = (float)mg/255.0;
      bluecol = (float)mb/255.0;
      matpixid = 0;
      matpix = mat_pixels[matpixid];
      mat_color.pixel = matpix;
      mat_color.flags = DoRed | DoGreen | DoBlue;
      mat_color.red = (unsigned short)(redcol * 65535.);
      mat_color.green = (unsigned short)(greencol * 65535.);
      mat_color.blue = (unsigned short)(bluecol * 65535.);
      if (truecolorcmap)
        {
         XAllocColor(dpy,my_cmap,&mat_color);
         matpix = mat_color.pixel;
         mat_pixels[matpixid] = matpix;
         XtVaSetValues(result,XmNbackground,matpix,NULL);
        }
      else
         XStoreColor(dpy,my_cmap,&mat_color);

      XtRemoveCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XFlush(dpy);

      XmScaleSetValue(red,mr);
      XmScaleSetValue(green,mg);
      XmScaleSetValue(blue,mb);
      XmScaleSetValue(trans,mt);

      XFlush(dpy);
      XtAddCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XFlush(dpy);

/*
      XmToggleButtonGadgetSetState(getcol,FALSE,FALSE);
      XmToggleButtonGadgetSetState(setcol,TRUE,FALSE);
      XmToggleButtonGadgetSetState(defcol,FALSE,FALSE);
*/
     }
}


void coloredit_select_active_contour(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float redcol, greencol, bluecol;
  unsigned long matpix;
  int i, j, arg, mt, matpixid, r, g, b, colindx;
  unsigned long getbestcolor(float red, float green, float blue);
  void setcoloreditisoclr(int iso_no, unsigned long pixno);
  void setanimisoclr(int iso_no, unsigned long pixno);

   arg = (long int)client_data;
   j = arg * 4;

   getcolflag = (int)XmToggleButtonGadgetGetState(getcol);
   setcolflag = (int)XmToggleButtonGadgetGetState(setcol);
   defcolflag = (int)XmToggleButtonGadgetGetState(defcol);

   for (i = 0; i < MAXCONTOURS+2; i++)
     {
      XmToggleButtonSetState(contbuts[i],FALSE,FALSE);
     }
   for (i = 0; i < maxmatbuts; i++)
     {
      XmToggleButtonGadgetSetState(matbuts[i],FALSE,FALSE);
     }
   XmToggleButtonSetState(contbuts[arg],TRUE,FALSE);

   if (setcolflag == 1 || defcolflag == 1)
     {
      if (setcolflag == 1)
        { r = mr;  g = mg;  b = mb; }
      if (defcolflag == 1)
        {
         if (arg == MAXCONTOURS)
           { r = 255;  g = 0;  b = 255; }
         else if (arg == MAXCONTOURS+1)
           { r = 153;  g = 153;  b = 153; }
         else
           {
            colindx = ((arg)*13 + 7)%256;
            if (colindx < 64)
              {
               r = 0;
               g = 4 * colindx;
               b = 255;
              }
            else if (colindx < 128)
              {
               r = 0;
               g = 255;
               b =  4 * (63-colindx);
              }
            else if (colindx < 192)
              {
               r = 4 * colindx;
               g = 255;
               b = 0;
              }
            else
              {
               r = 255;
               g = 4 * (63-colindx);
               b = 0;
              }
           }
        }

      redcol = (float)r/255.0;
      greencol = (float)g/255.0;
      bluecol = (float)b/255.0;
      matpixid = arg + 1;
      matpix = mat_pixels[matpixid];
      if (!mesacmap && !truecolorcmap)
        {
         mat_color.pixel = matpix;
         mat_color.flags = DoRed | DoGreen | DoBlue;
         mat_color.red = (unsigned short)(redcol * 65535.);
         mat_color.green = (unsigned short)(greencol * 65535.);
         mat_color.blue = (unsigned short)(bluecol * 65535.);
         XStoreColor(dpy,my_cmap,&mat_color);
        }
      else
        {
         if (mesacmap)
            matpix = getbestcolor(redcol,greencol,bluecol);
         if (truecolorcmap)
           {
            mat_color.pixel = matpix;
            mat_color.flags = DoRed | DoGreen | DoBlue;
            mat_color.red = (unsigned short)(redcol * 65535.);
            mat_color.green = (unsigned short)(greencol * 65535.);
            mat_color.blue = (unsigned short)(bluecol * 65535.);
            XAllocColor(dpy,my_cmap,&mat_color);
            matpix = mat_color.pixel;
           }
         mat_pixels[matpixid] = matpix;
         setcoloreditisoclr(arg,matpix);
         if (arg < MAXCONTOURS)
            setanimisoclr(arg,matpix);
         if (ncells > 0 && arg < MAXCONTOURS)
            setisoisoclr(arg,matpix);
         if (ncells > 0 && arg == MAXCONTOURS+1)
            setisovolclr(matpix);
         XFlush(dpy);
        }

      conrgba[0+j] = redcol;
      conrgba[1+j] = greencol;
      conrgba[2+j] = bluecol;
      conrgba[3+j] = transpar;

      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
      fastdraw = 0;
      drawit();
     }

   if (getcolflag == 1)
     {
      mr = (int)(conrgba[0+j] * 255.);
      mg = (int)(conrgba[1+j] * 255.);
      mb = (int)(conrgba[2+j] * 255.);
      mt = (int)(conrgba[3+j]*100.);

      redcol = (float)mr/255.0;
      greencol = (float)mg/255.0;
      bluecol = (float)mb/255.0;
      matpixid = 0;
      matpix = mat_pixels[matpixid];
      mat_color.pixel = matpix;
      mat_color.flags = DoRed | DoGreen | DoBlue;
      mat_color.red = (unsigned short)(redcol * 65535.);
      mat_color.green = (unsigned short)(greencol * 65535.);
      mat_color.blue = (unsigned short)(bluecol * 65535.);
      if (truecolorcmap)
        {
         XAllocColor(dpy,my_cmap,&mat_color);
         matpix = mat_color.pixel;
         mat_pixels[matpixid] = matpix;
         XtVaSetValues(result,XmNbackground,matpix,NULL);
        }
      else
         XStoreColor(dpy,my_cmap,&mat_color);

      XtRemoveCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtRemoveCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XFlush(dpy);

      XmScaleSetValue(red,mr);
      XmScaleSetValue(green,mg);
      XmScaleSetValue(blue,mb);
      XmScaleSetValue(trans,mt);

      XFlush(dpy);
      XtAddCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XtAddCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
      XFlush(dpy);

/*
      XmToggleButtonGadgetSetState(getcol,FALSE,FALSE);
      XmToggleButtonGadgetSetState(setcol,TRUE,FALSE);
      XmToggleButtonGadgetSetState(defcol,FALSE,FALSE);
*/
     }
}

void coloredit_set_rgb(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  float redcol, greencol, bluecol;
  unsigned long matpix;
  int arg, matpixid;

   arg = (long int)client_data;

   if (arg == 0)        /*  copper  */
     {
      mr = 170;  mg = 55;  mb = 0;
     }
   if (arg == 1)        /*  steel  */
     {
      mr = 130;  mg = 130;  mb = 140;
     }
   if (arg == 2)        /*  shiny metal  */
     {
      mr = 230;  mg = 230;  mb = 230;
     }
   if (arg == 3)        /*  lead  */
     {
      mr = 50;  mg = 50;  mb = 50;
     }
   if (arg == 4)        /*  gold  */
     {
      mr = 255;  mg = 150;  mb = 0;
     }
   if (arg == 5)        /*  H. E.  */
     {
      mr = 255;  mg = 200;  mg = 100;
     }

   redcol = (float)mr/255.0;
   greencol = (float)mg/255.0;
   bluecol = (float)mb/255.0;
   matpixid = 0;
   matpix = mat_pixels[matpixid];
   mat_color.pixel = matpix;
   mat_color.flags = DoRed | DoGreen | DoBlue;
   mat_color.red = (unsigned short)(redcol * 65535.);
   mat_color.green = (unsigned short)(greencol * 65535.);
   mat_color.blue = (unsigned short)(bluecol * 65535.);
   if (truecolorcmap)
     {
      XAllocColor(dpy,my_cmap,&mat_color);
      matpix = mat_color.pixel;
      mat_pixels[matpixid] = matpix;
      XtVaSetValues(result,XmNbackground,matpix,NULL);
     }
   else
      XStoreColor(dpy,my_cmap,&mat_color);

   XtRemoveCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtRemoveCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtRemoveCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtRemoveCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XFlush(dpy);

   XmScaleSetValue(red,mr);
   XmScaleSetValue(green,mg);
   XmScaleSetValue(blue,mb);

   XFlush(dpy);
   XtAddCallback(red,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtAddCallback(green,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtAddCallback(blue,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XtAddCallback(trans,XmNvalueChangedCallback,coloredit_docolor,NULL);
   XFlush(dpy);
}



void makecoloreditform(parent)
 Widget parent;
{
  Widget matedclose, copperbut, steelbut, alumbut, leadbut, 
         goldbut, hebut, getsetbox, matswin, matbox, lastwidget,
         matlab, isolab;
  char str[20];
  unsigned long matpix;
  int i, maxflags, matpixid, maxgrps;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Materials, Isosurfaces, Isovolumes",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   coloreditform = XmCreateFormDialog(parent,"MatEdit",wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   matedclose = XtVaCreateManagedWidget("Matedclose",
                xmPushButtonGadgetClass,
                coloreditform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(matedclose,XmNactivateCallback,
                    coloreditclose,NULL);

   string = XmStringCreate("Copper",XmSTRING_DEFAULT_CHARSET);
   copperbut = XtVaCreateManagedWidget("Copperbut",
               xmPushButtonGadgetClass,
               coloreditform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, matedclose,
               XmNtopOffset, 20,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(copperbut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 0);

   string = XmStringCreate("Steel",XmSTRING_DEFAULT_CHARSET);
   steelbut = XtVaCreateManagedWidget("Steelbut",
              xmPushButtonGadgetClass,
              coloreditform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, copperbut,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(steelbut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 1);

   string = XmStringCreate("Aluminum",XmSTRING_DEFAULT_CHARSET);
   alumbut = XtVaCreateManagedWidget("Alumbut",
             xmPushButtonGadgetClass,
             coloreditform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, steelbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(alumbut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 2);

   string = XmStringCreate("Lead",XmSTRING_DEFAULT_CHARSET);
   leadbut = XtVaCreateManagedWidget("Leadbut",
             xmPushButtonGadgetClass,
             coloreditform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, alumbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(leadbut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 3);

   string = XmStringCreate("Gold",XmSTRING_DEFAULT_CHARSET);
   goldbut = XtVaCreateManagedWidget("Goldbut",
             xmPushButtonGadgetClass,
             coloreditform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, leadbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(goldbut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 4);

   string = XmStringCreate("H. E.",XmSTRING_DEFAULT_CHARSET);
   hebut = XtVaCreateManagedWidget("Hebut",
           xmPushButtonGadgetClass,
           coloreditform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, goldbut,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(hebut,XmNactivateCallback,
                    coloredit_set_rgb,(caddr_t) 5);

   matpixid = 0;
   matpix = mat_pixels[matpixid];
   mat_color.pixel = matpix;
   mat_color.flags = DoRed | DoGreen | DoBlue;
   mat_color.red = 0;
   mat_color.green = 0;
   mat_color.blue = 0;
   if (truecolorcmap)
     {
      XAllocColor(dpy,my_cmap,&mat_color);
      matpix = mat_color.pixel;
      mat_pixels[matpixid] = matpix;
     }
   else
      XStoreColor(dpy,my_cmap,&mat_color);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   result = XtVaCreateManagedWidget("Result",
             xmLabelWidgetClass,
             coloreditform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, hebut,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNbackground, matpix,
             XmNforeground, matpix,
             XmNborderWidth, 2,
             XmNheight, 130,
             XmNwidth, 130,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("Red",XmSTRING_DEFAULT_CHARSET);
   red = XtVaCreateManagedWidget("Red",
         xmScaleWidgetClass,
         coloreditform,
         XmNtopAttachment, XmATTACH_WIDGET,
         XmNtopWidget, result,
         XmNtopOffset, 5,
         XmNleftAttachment, XmATTACH_FORM,
         XmNrightAttachment, XmATTACH_NONE,
         XmNbottomAttachment, XmATTACH_NONE,
         XmNminimum, 0,
         XmNmaximum, 255,
         XmNorientation, XmHORIZONTAL,
         XmNprocessingDirection, XmMAX_ON_RIGHT,
         XmNvalue, 0,
         XmNscaleWidth, 130,
         XmNscaleMultiple, 1,
         XmNshowValue, TRUE,
         XmNbackground, redpix,
         XmNtitleString, string,
         NULL);
      XmStringFree(string);
      XtAddCallback(red,XmNvalueChangedCallback,
                    coloredit_docolor,NULL);
      XtAddCallback(red,XmNdragCallback,coloredit_docolor,NULL);

   string = XmStringCreate("Green",XmSTRING_DEFAULT_CHARSET);
   green = XtVaCreateManagedWidget("Green",
           xmScaleWidgetClass,
           coloreditform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, red,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNminimum, 0,
           XmNmaximum, 255,
           XmNorientation, XmHORIZONTAL,
           XmNprocessingDirection, XmMAX_ON_RIGHT,
           XmNvalue, 0,
           XmNscaleWidth, 130,
           XmNscaleMultiple, 1,
           XmNshowValue, TRUE,
           XmNbackground, greenpix,
           XmNtitleString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(green,XmNvalueChangedCallback,
                    coloredit_docolor,NULL);
      XtAddCallback(green,XmNdragCallback,coloredit_docolor,NULL);

   string = XmStringCreate("Blue",XmSTRING_DEFAULT_CHARSET);
   blue = XtVaCreateManagedWidget("Blue",
          xmScaleWidgetClass,
          coloreditform,
          XmNtopAttachment, XmATTACH_WIDGET,
          XmNtopWidget, green,
          XmNtopOffset, 5,
          XmNleftAttachment, XmATTACH_FORM,
          XmNrightAttachment, XmATTACH_NONE,
          XmNbottomAttachment, XmATTACH_NONE,
          XmNminimum, 0,
          XmNmaximum, 255,
          XmNorientation, XmHORIZONTAL,
          XmNprocessingDirection, XmMAX_ON_RIGHT,
          XmNvalue, 0,
          XmNscaleWidth, 130,
          XmNscaleMultiple, 1,
          XmNshowValue, TRUE,
          XmNbackground, bluepix,
          XmNtitleString, string,
          NULL);
      XmStringFree(string);
      XtAddCallback(blue,XmNvalueChangedCallback,
                    coloredit_docolor,NULL);
      XtAddCallback(blue,XmNdragCallback,coloredit_docolor,NULL);

   string = XmStringCreate("Trans",XmSTRING_DEFAULT_CHARSET);
   trans = XtVaCreateManagedWidget("Trans",
           xmScaleWidgetClass,
           coloreditform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, blue,
           XmNtopOffset, 5,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNminimum, 0,
           XmNmaximum, 100,
           XmNorientation, XmHORIZONTAL,
           XmNprocessingDirection, XmMAX_ON_RIGHT,
           XmNvalue, 100,
           XmNscaleWidth, 130,
           XmNscaleMultiple, 1,
           XmNshowValue, TRUE,
           XmNdecimalPoints, 2,
           XmNbackground, yellowpix,
           XmNtitleString, string,
           NULL);
      XmStringFree(string);
      XtAddCallback(trans,XmNvalueChangedCallback,
                    coloredit_docolor,NULL);

   i=0; XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_FORM);
   i++; XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
   i++; XtSetArg(wargs[i],XmNleftWidget, result);
   i++; XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++; XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   getsetbox = XmCreateRadioBox(coloreditform,"Getsetbox",wargs,i);
   XtManageChild(getsetbox);

   string = XmStringCreate("Get Color",XmSTRING_DEFAULT_CHARSET);
   getcol = XtVaCreateManagedWidget("Getcol",
            xmToggleButtonGadgetClass,
            getsetbox,
            XmNlabelString, string,
            XmNspacing, 1,
            XmNselectColor, yellowpix,
            XmNmarginHeight, 0,
            XmNmarginWidth, 0,
            XmNset, TRUE,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("Set Color",XmSTRING_DEFAULT_CHARSET);
   setcol = XtVaCreateManagedWidget("Setcol",
            xmToggleButtonGadgetClass,
            getsetbox,
            XmNlabelString, string,
            XmNspacing, 1,
            XmNselectColor, yellowpix,
            XmNmarginHeight, 0,
            XmNmarginWidth, 0,
            XmNset, FALSE,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("Set Default",
                           XmSTRING_DEFAULT_CHARSET);
   defcol = XtVaCreateManagedWidget("Defcol",
            xmToggleButtonGadgetClass,
            getsetbox,
            XmNlabelString, string,
            XmNspacing, 1,
            XmNselectColor, yellowpix,
            XmNmarginHeight, 0,
            XmNmarginWidth, 0,
            XmNset, FALSE,
            NULL);
      XmStringFree(string);

   /*  Create materials row column box with material buttons.  */
   string = XmStringCreate("Materials",
                           XmSTRING_DEFAULT_CHARSET);
   matlab = XtVaCreateManagedWidget("Matlab",
            xmLabelWidgetClass,
            coloreditform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, getsetbox,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, result,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   maxflags = 0;
   for (i = 0; i < numflags_cell; i++)
      if (flagtypes_cell[i] > maxflags)
         maxflags = flagtypes_cell[i];
   for (i = 0; i < numflags_node; i++)
      if (flagtypes_node[i] > maxflags)
         maxflags = flagtypes_node[i];

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, matlab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNleftWidget, result);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 145);
   i++;  XtSetArg(wargs[i],XmNheight, 500);
   i++;
   matswin = XmCreateScrolledWindow(coloreditform,"Matswin",wargs,i);
   XtManageChild(matswin);

   i=0;  XtSetArg(wargs[i],XmNspacing, 0);
   i++;  XtSetArg(wargs[i],XmNmarginHeight, 0);
   i++;
   matbox = XmCreateRowColumn(matswin,"Matbox",wargs,i);
   XtManageChild(matbox);

   maxgrps = nnodegrps;
   if (ncellgrps > maxgrps) maxgrps = ncellgrps;
   if (nfacegrps > maxgrps) maxgrps = nfacegrps;

   maxmatbuts = mmats;
   if (maxflags > maxmatbuts) maxmatbuts = maxflags;
   if (maxgrps > maxmatbuts) maxmatbuts = maxgrps;
   for (i = 0; i < maxmatbuts; i++)
     {
      if (i < mmats)
         string = XmStringCreate(mmatnames[i],
                                 XmSTRING_DEFAULT_CHARSET);
      else if (i < maxflags)
        {
         sprintf(str,"flag %d",i+1);
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
        }
      else
        {
         sprintf(str,"group %d",i+1);
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
        }
      sprintf(str,"Matbuts%d",i);
      matbuts[i] = XtVaCreateManagedWidget(str,
                   xmToggleButtonGadgetClass,
                   matbox,
                   XmNlabelString, string,
                   XmNshadowThickness, 0,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
         XmStringFree(string);
         XtAddCallback(matbuts[i],XmNvalueChangedCallback,
                       coloredit_select_active_mat,
                       (caddr_t) i);
/*
      if (i < mmats)
        {
         if (!(mat_has_active_pts[i] > 0 ||
             mat_has_active_polys[i] > 0 || i < maxflags))
            XtUnmanageChild(matbuts[i]);
        }
*/
     }

   string = XmStringCreate("Isos.",
                           XmSTRING_DEFAULT_CHARSET);
   isolab = XtVaCreateManagedWidget("Isolab",
            xmLabelWidgetClass,
            coloreditform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, getsetbox,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, matswin,
            XmNleftOffset, 5,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   lastwidget = isolab;
   matpixid = 0;
   for (i = 0; i < MAXCONTOURS+2; i++)
     {
      matpixid++;
      matpix = mat_pixels[matpixid];
      sprintf(str,"%2d",i+1);
      if (i < MAXCONTOURS)
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      else if (i == MAXCONTOURS)
         string = XmStringCreate("Anim.",XmSTRING_DEFAULT_CHARSET);
      else
         string = XmStringCreate("Volume",XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Contbuts%d",i);
      contbuts[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonWidgetClass,
                    coloreditform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, lastwidget,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, matswin,
                    XmNleftOffset, 5,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    XmNbackground, matpix,
                    XmNforeground, blackpix,
                    XmNshadowThickness, 0,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    XmNset, FALSE,
                    NULL);
         XmStringFree(string);
         XtAddCallback(contbuts[i],XmNvalueChangedCallback,
                       coloredit_select_active_contour,
                       (caddr_t) i);
      lastwidget = contbuts[i];
     }

   mr = 0;  mg = 0;  mb = 0;  transpar = 1.;

}


void setcoloreditisoclr(int iso_no, unsigned long pixno)
{
   XtVaSetValues(contbuts[iso_no],XmNbackground,pixno,NULL);
}
