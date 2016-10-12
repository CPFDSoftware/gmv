#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "lights.h"
#include "formwidgets.h"
#include "xstuff.h"

static Widget clrtab0, clrtab1, clrtab2, clrtab3, clrtab4, clrfinp;
static char clrmapfilnam[256];


void fcoloreditformpop()
{
   XmToggleButtonGadgetSetState(clrtab0,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clrtab1,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clrtab2,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clrtab3,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clrtab4,FALSE,FALSE);
   XmToggleButtonGadgetSetState(clrfinp,FALSE,FALSE);
   switch(fieldcolortype)
     {
      case(0):
         XmToggleButtonGadgetSetState(clrtab0,TRUE,FALSE);
         break;
      case(1):
         XmToggleButtonGadgetSetState(clrtab1,TRUE,FALSE);
         break;
      case(2):
         XmToggleButtonGadgetSetState(clrtab2,TRUE,FALSE);
         break;
      case(3):
         XmToggleButtonGadgetSetState(clrtab3,TRUE,FALSE);
         break;
      case(4):
         XmToggleButtonGadgetSetState(clrtab4,TRUE,FALSE);
         break;
      case(-1):
         XmToggleButtonGadgetSetState(clrfinp,TRUE,FALSE);
         break;
     }

   MyManageChild(fcoloreditform);
}

void fcoloreditclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(fcoloreditform);
}


void clrtabflags(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, fcflag;
  XmString string, string2, string3;
  void setfieldcolors();

   arg = (long int)client_data;
   fcflag = (int)XmToggleButtonGadgetGetState(w);

   if (fcflag)
     {
      if (arg >= 0)
        {
         fieldcolortype = arg;
         setfieldcolors();
         if (displistflag) drawnewlistflag = 1;
         drawnew3dflag = 1;
         fastdraw = 0;
         drawit();
        }

      else
        {
         fileselflag = COLORMAPIN;
         string = XmStringCreate("Read Field Colormap",
                                 XmSTRING_DEFAULT_CHARSET);
         string2 = XmStringCreate(fspattern[COLORMAPIN],
                                  XmSTRING_DEFAULT_CHARSET);
         string3 = XmStringCreate(fsdir[COLORMAPIN],
                                  XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                           XmNpattern, string2,
                           XmNdirectory, string3, NULL);
         XmStringFree(string);
         XmStringFree(string2);
         XmStringFree(string3);

         if (fsread[COLORMAPIN])
           {
            string = XmStringCreate(clrmapfilnam,
                                    XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
            XmStringFree(string);
           }

         MyManageChild(file_sel2);
        }
     }
}


void readfcolormap(char *fname) 
{
  FILE *fin;
  int i, r, g, b;
  float rin, gin, bin;
  char keyword[9];
  XmString string, string1, string2;
  char str1[] =
   "Error - incorrect file format";
  char str2[] = 
   "Error - cannot open file: ";

   strcpy(clrmapfilnam,fname);

   /*  Open file and read colormap.  */
   fin = fopen(clrmapfilnam,"r");
   if (fin == NULL)
      {
       string1 = XmStringLtoRCreate(str2,
                                   XmSTRING_DEFAULT_CHARSET);
       string2 = XmStringLtoRCreate(filnam,
                                   XmSTRING_DEFAULT_CHARSET);
       string = XmStringConcat(string1,string2);
       XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
       XmStringFree(string);
       XmStringFree(string1);
       XmStringFree(string2);
       MyManageChild(errorform);
       return;
      }

   /*  Check for gmvcmap keyword.  */
   fscanf(fin, "%s", keyword);
   if (strcmp("gmvcmap", keyword))
      {
       string = XmStringLtoRCreate(str1,
                                   XmSTRING_DEFAULT_CHARSET);
       XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
       XmStringFree(string);
       MyManageChild(errorform);
       return;
      }

   /*  Read red, green, and blue elements of  */
   /*  color map and save r,g,b components.   */
   for (i = 0; i < 256; i++)
     {
      fscanf(fin, "%f %f %f", &rin, &gin, &bin);
      r = (int)((rin * 255.) + .5);
      g = (int)((gin * 255.) + .5);
      b = (int)((bin * 255.) + .5);
      jcolor[i] = (r << 24) |(g << 16 ) | (b << 8 ) | 255;
     }
   resettexture();

   fclose(fin);
   
   fieldcolortype = -1;
   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void makefcoloreditform(parent)
 Widget parent;
{
  Widget fcclose, fcbox, fctxt;
  int i;
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Field Color Map Selection",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fcoloreditform = XmCreateFormDialog(parent,"Fcoloreditform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fcclose = XtVaCreateManagedWidget("Fcclose",
             xmPushButtonGadgetClass,
             fcoloreditform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
     XmStringFree(string);
     XtAddCallback(fcclose,XmNactivateCallback,fcoloreditclose,NULL);
 
   string = XmStringCreate("Select a Field Color Map:",
                           XmSTRING_DEFAULT_CHARSET);
   fctxt = XtVaCreateManagedWidget("Fctxt",
           xmLabelWidgetClass,
           fcoloreditform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, fcclose,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, fctxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   fcbox = XmCreateRadioBox(fcoloreditform,"Fcbox",wargs,i);
   XtManageChild(fcbox);
 
   string = XmStringCreate("Blue to Red",
            XmSTRING_DEFAULT_CHARSET);
   clrtab0 = XtVaCreateManagedWidget("Clrtab0",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, TRUE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrtab0,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) 0);
 
   string = XmStringCreate("Red to Blue",
            XmSTRING_DEFAULT_CHARSET);
   clrtab1 = XtVaCreateManagedWidget("Clrtab1",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrtab1,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) 1);
 
   string = XmStringCreate("Greyscale",
            XmSTRING_DEFAULT_CHARSET);
   clrtab2 = XtVaCreateManagedWidget("Clrtab2",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrtab2,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) 2);
 
   string = XmStringCreate("Reverse Greyscale",
            XmSTRING_DEFAULT_CHARSET);
   clrtab4 = XtVaCreateManagedWidget("Clrtab4",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrtab4,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) 4);
  
   string = XmStringCreate("Blue to Red, by contour levels",
            XmSTRING_DEFAULT_CHARSET);
   clrtab3 = XtVaCreateManagedWidget("Clrtab3",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrtab3,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) 3);
  
   string = XmStringCreate("Read gmv colormap file",
            XmSTRING_DEFAULT_CHARSET);
   clrfinp = XtVaCreateManagedWidget("Clrfinp",
             xmToggleButtonGadgetClass,
             fcbox,
             XmNlabelString, string,
             XmNset, FALSE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(clrfinp,XmNvalueChangedCallback,
                    clrtabflags,(caddr_t) -1);
}


void setfieldcolors()
{
  int i, j, l, tmpcolor[256], iset;
  float kset;


   /*  Set field data color table according to selected type.  */
   switch(fieldcolortype)
     {

      case(0):  /*  blue to red  */
         for (i = 0; i < 64; i++)
           {
            tmpcolor[i] = ( (4*i) << 16 ) | ( 255 << 8 ) | 255;
            tmpcolor[i+64] = ( 255 << 16 ) | ( 4*(63-i)  << 8 ) | 255;
            tmpcolor[i+128] = ( 4*i ) << 24 | ( 255 << 16 ) | 255;
            tmpcolor[i+192] = ( 255 ) << 24| ( 4*(63-i) << 16 ) | 255;
           }
         jcolor[0] = (39 << 24) | (25 << 16) | (201 << 8) | 255;
         for (i = 0; i < 63; i++)
           {
            jcolor[i+1] = tmpcolor[i];
            jcolor[i+64] = tmpcolor[i+63];
           }
         for (i = 0; i < 64; i++)
           {
            jcolor[i+127] = tmpcolor[i+128];
            jcolor[i+191] = tmpcolor[i+192];
           }
         jcolor[255] = (255 << 24) | (25 << 16) | (171 << 8) | 255;
         break;

      case(1):  /*  red to blue  */
         for (i = 0; i < 64; i++)
           {
            tmpcolor[i] = ( (4*i) << 16 ) | ( 255 << 8 ) | 255;
            tmpcolor[i+64] = ( 255 << 16 ) | ( 4*(63-i)  << 8 ) | 255;
            tmpcolor[i+128] = ( 4*i ) << 24 | ( 255 << 16 ) | 255;
            tmpcolor[i+192] = ( 255 ) << 24| ( 4*(63-i) << 16 ) | 255;
           }
         jcolor[255] = (39 << 24) | (25 << 16) | (201 << 8) | 255;
         for (i = 0; i < 126; i++)
            jcolor[254-i] = tmpcolor[i];
         for (i = 0; i < 128; i++)
            jcolor[i+1] = tmpcolor[255-i];
         jcolor[0] = (255 << 24) | (25 << 16) | (171 << 8) | 255;
         break;

      case(2):  /*  greyscale  */
         for (i = 0; i < 256; i++)
             jcolor[i] = (i << 24) | (i << 16 ) | (i << 8 ) | 255;
         break;

      case(3):  /*  blue to red, by contour levels */
         for (i = 0; i < 64; i++)
           {
            tmpcolor[i] = ( (4*i) << 16 ) | ( 255 << 8 ) | 255;
            tmpcolor[i+64] = ( 255 << 16 ) | ( 4*(63-i)  << 8 ) | 255;
            tmpcolor[i+128] = ( 4*i ) << 24 | ( 255 << 16 ) | 255;
            tmpcolor[i+192] = ( 255 ) << 24 | ( 4*(63-i) << 16 ) | 255;
           }
         kset = 256. / (float)nset_contours;
         l = kset / 2;
         iset = 0;
         for (i = 0; i < 256; i++)
           {
            if (i > kset * (iset+1)) iset++;
            j = kset * iset + l;
            if (i < kset) j = 0;
            if (i > (255 - kset)) j = 255;
            jcolor[i] = tmpcolor[j];
           }
         break;


      case(4):  /*  reverse greyscale  */
         for (i = 0; i < 256; i++)
             tmpcolor[i] = jcolor[i] = (i << 24) | (i << 16 ) | (i << 8 ) | 255;
         for (i = 0; i < 256; i++)
             jcolor[i] = tmpcolor[255-i];
         break;
     }
   resettexture();
}


void setfldcolortype3(double fldmin, double fldmax, char niceflag, char logflag)
{
  int nintervals, nsets, k;
  double intervals[100];
  void getscale(double fldmin, double fldmax, int maxcont, char lineflag,
                char logflag, double *intervals, int *nintervals);

   getscale(fldmin, fldmax, nset_contours, niceflag, logflag,
            intervals, &nintervals);
   nsets = nintervals;
   k = nset_contours;
   nset_contours = nsets - 1;

   setfieldcolors();
   nset_contours = k;
}
