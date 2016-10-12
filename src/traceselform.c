#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

#define TRSELON 5
static Widget tracefldselform, tracesphselform,
              tracenumselform, trselonbut[TRSELON], trselonbut2[TRSELON],
              traceboxselform;
static Widget trselfldlab, trselminlab, trselmaxlab, trselflderror,
              trselmintxt, trselmaxtxt, trselerror, trselsphxtxt,
              trselsphytxt, trselsphztxt, trselsphrtxt, trselnumtxt[MAXNUMSEL],
              trselsphin, trselsphout, trselboxx1txt, trselboxy1txt, 
              trselboxz1txt, trselboxin, trselboxx2txt, trselboxy2txt, 
              trselboxz2txt, trselboxout, trselboxx1sl, trselboxx2sl, 
              trselboxy1sl, trselboxy2sl, trselboxz1sl, trselboxz2sl;

static int fastboxcount = 0;


void traceselformpop()
{
   XmToggleButtonSetState(trselonbut[1],traceselfld_on,FALSE);
   XmToggleButtonSetState(trselonbut[2],traceselsph_on,FALSE);
   XmToggleButtonSetState(trselonbut[3],traceselnum_on,FALSE);
   XmToggleButtonSetState(trselonbut[4],traceselbox_on,FALSE);

   MyManageChild(traceselform);
}


void tracefldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (numvartr > 0)
     {
      i = traceselfield;

      string = XmStringCreate(fieldtrname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trselfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",fieldtrmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trselminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",fieldtrmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trselmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",trfldselmin[i]);
      XtVaSetValues(trselmintxt,XmNvalue,str,NULL);
      sprintf(str,"%.11g",trfldselmax[i]);
      XtVaSetValues(trselmaxtxt,XmNvalue,str,NULL);

      /*  Calculate trace range pop-up postition from widgets.  */
      XtTranslateCoords(trselerror, p0,p0, &wx,&wy); 
      fx = wx + 20;
      fy = wy + 50;
      XtVaSetValues(tracefldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(tracefldselform);
     }
}


void tracesphselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(trselonbut2[2],traceselsph_on,FALSE);

   sprintf(str,"%.11g",traceselsph_x);
   XmTextFieldSetString(trselsphxtxt,str);

   sprintf(str,"%.11g",traceselsph_y);
   XmTextFieldSetString(trselsphytxt,str);

   sprintf(str,"%.11g",traceselsph_z);
   XmTextFieldSetString(trselsphztxt,str);

   sprintf(str,"%.11g",traceselsph_r);
   XmTextFieldSetString(trselsphrtxt,str);

   if (traceselsph_in)
     {
      XmToggleButtonGadgetSetState(trselsphin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(trselsphout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(trselsphin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(trselsphout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(trselerror, p0,p0, &wx,&wy); 
   fx = wx + 40;
   fy = wy + 50;
   XtVaSetValues(tracesphselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(tracesphselform);
}


void tracenumselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[30];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(trselonbut2[3],traceselnum_on,FALSE);

   /*  Fill text lines with trace number select data.  */
   if (ntraceselnum > 0)
     {
      for (i = 0; i < ntraceselnum; i++)
        {
         if (traceselnum_f[i] == traceselnum_l[i])
            sprintf(str,"%d",traceselnum_f[i]);
         else if (traceselnum_s[i] == 1)
            sprintf(str,"%d:%d",traceselnum_f[i],traceselnum_l[i]);
         else
            sprintf(str,"%d:%d:%d",traceselnum_f[i],traceselnum_l[i],
                                   traceselnum_s[i]);
         XmTextFieldSetString(trselnumtxt[i],str);
        }
      for (i = ntraceselnum; i < MAXNUMSEL; i++)
         XmTextFieldSetString(trselnumtxt[i],"");
     }

   /*  Calculate number pop-up postition from widgets.  */
   XtTranslateCoords(trselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(tracenumselform, XmNx,fx, XmNy,fy, NULL);
   XFlush(dpy);

   MyManageChild(tracenumselform);
}


void traceboxselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;
  void settraceselboxwidgets();

   XmToggleButtonSetState(trselonbut2[4],traceselbox_on,FALSE);

   settraceselboxwidgets();

   if (traceselbox_in)
     {
      XmToggleButtonGadgetSetState(trselboxin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(trselboxout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(trselboxin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(trselboxout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(trselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(traceboxselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(traceboxselform);
}


void trselfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(trfieldform);
   XtUnmanageChild(tracefldselform);
}


void trselsphclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(tracesphselform);
}


void trselnumclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(tracenumselform);
}


void trselboxclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   traceselbox_draw = 0;
   expose_redraw();
   XtUnmanageChild(traceboxselform);
}


void trselclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (numvartr > 0)
      trselfldclose(NULL,NULL,NULL);
   XtUnmanageChild(tracesphselform);
   XtUnmanageChild(tracenumselform);
   trselboxclose(NULL,NULL,NULL);

   XtUnmanageChild(traceselform);
}


void trselsphsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   traceselsph_in = (short)XmToggleButtonGadgetGetState(trselsphin);
}


void trselboxsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   traceselbox_in = (short)XmToggleButtonGadgetGetState(trselboxin);
}


void trselsetonbut(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 1)
     {
      traceselfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(trselonbut[1],traceselfld_on,FALSE);
     }
   if (arg == 2)
     {
      traceselsph_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(trselonbut[2],traceselsph_on,FALSE);
      XmToggleButtonSetState(trselonbut2[2],traceselsph_on,FALSE);
     }
   if (arg == 3)
     {
      traceselnum_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(trselonbut[3],traceselnum_on,FALSE);
      XmToggleButtonSetState(trselonbut2[3],traceselnum_on,FALSE);
     }
   if (arg == 4)
     {
      traceselbox_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(trselonbut[4],traceselbox_on,FALSE);
      XmToggleButtonSetState(trselonbut2[4],traceselbox_on,FALSE);
     }
}


void trselfldset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{ 
  char *tmpstr;
  double tmpmin, tmpmax;
  XmString string;
  static char *str1 = 
           "Error - minimum must be less than maximum.";
  
   tmpstr = XmTextFieldGetString(trselmintxt);
   tmpmin = atof(tmpstr);
   XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(trselmaxtxt);
   tmpmax = atof(tmpstr);
   XtFree(tmpstr);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trselflderror,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (tmpmin > tmpmax)
     {
      string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trselflderror,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   else
     {
      traceselfields[traceselfield] = 1;
      trfldselmin[traceselfield] = tmpmin;
      trfldselmax[traceselfield] = tmpmax;
     }
}


void trselapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ierr, tmpstrlen;
  unsigned long num;
  char *tmpstr;
  float x1,y1,z1, x2,y2, z2;
  static char *token;
  static char *str2 = 
           "Error - search sphere radius must be greater than 0.";
  static char str4[55]; 
  XmString string;
  char *chk_num = "0123456789: ", *chk_blank = " ";
  void settracesel();
  
   ierr = 0;

   /*  Read and check the search sphere values.  */
   if (traceselsph_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(trselsphxtxt);
      traceselsph_x = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselsphytxt);
      traceselsph_y = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselsphztxt);
      traceselsph_z = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselsphrtxt);
      traceselsph_r = atof(tmpstr);
      XtFree(tmpstr);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(trselerror,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (traceselsph_r <= 0)
        {
         ierr = 1;
         string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(trselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check trace number values.  */
   if (traceselnum_on && ierr == 0)
     {
      ntraceselnum = 0;
      i = 0;

      while(ierr == 0 && i < MAXNUMSEL)
        {
         tmpstr = XmTextFieldGetString(trselnumtxt[i]);
         tmpstrlen = strlen(tmpstr);
         if (tmpstrlen > 0)
           {

            /*  Check for invalid characters.  */
            if (strspn(tmpstr,chk_num) != tmpstrlen)
              {
               ierr = 1;
               sprintf(str4,"Error - invalid character in line %d",i+1);
               string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
               XtVaSetValues(trselerror,XmNlabelString,string,NULL);
               XmStringFree(string);
              }

            /*  If not all blanks, extract data from the string.  */
            if (ierr == 0 && strspn(tmpstr,chk_blank) != tmpstrlen)
              {

               /*  Get the first number from the set.  */
               token = strtok(tmpstr,":");
               num = strtoul(token,NULL,10);
               if (num > ntracers && !traceridflag) num = ntracers;
               traceselnum_f[ntraceselnum] = num;

               /*  Get the last number from the set.  */
               traceselnum_l[ntraceselnum] = num;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  if (num > ntracers && !traceridflag) num = ntracers;

                  /*  If first greater than last, switch.  */
                  if (num >= traceselnum_f[i])
                     traceselnum_l[ntraceselnum] = num;
                  else
                    {
                     traceselnum_l[i] = traceselnum_f[i];
                     traceselnum_f[i] = num;
                    }
                 }

               /*  Get the stride from the set.  */
               traceselnum_s[ntraceselnum] = 1;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  traceselnum_s[ntraceselnum] = num;
                 }

               ntraceselnum++;
              }
           }
         XtFree(tmpstr);
         i++;
        }

     }

   /*  Read and set the search box values.  */
   if (traceselbox_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(trselboxx1txt);
      x1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselboxy1txt);
      y1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselboxz1txt);
      z1 = atof(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(trselboxx2txt);
      x2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselboxy2txt);
      y2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(trselboxz2txt);
      z2 = atof(tmpstr);
      XtFree(tmpstr);

      traceselbox_xmin = (x1 < x2) ? x1 : x2;
      traceselbox_ymin = (y1 < y2) ? y1 : y2;
      traceselbox_zmin = (z1 < z2) ? z1 : z2;
      traceselbox_xmax = (x1 > x2) ? x1 : x2;
      traceselbox_ymax = (y1 > y2) ? y1 : y2;
      traceselbox_zmax = (z1 > z2) ? z1 : z2;
     }

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      settracesel();
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void trseltrfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = traceselfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(trfieldform, XmNx,fx, XmNy,fy, NULL);
 
   trfieldformpop(TRFLDNODESEL, ifld);
}


void trselfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = traceselfield;

   traceselfields[i] = 0;
   trfldselmin[i] = fieldtrmin[i];
   trfldselmax[i] = fieldtrmax[i];

   sprintf(str,"%.11g",fieldtrmin[i]);
   XtVaSetValues(trselmintxt,XmNvalue,str,NULL);
   sprintf(str,"%.11g",fieldtrmax[i]);
   XtVaSetValues(trselmaxtxt,XmNvalue,str,NULL);
}


void trselfldresetall(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   
   for (i = 0; i < numvartr; i++)
     {
      traceselfields[i] = 0;
      trfldselmin[i] = fieldtrmin[i];
      trfldselmax[i] = fieldtrmax[i];   
     }
     
   i = traceselfield;

   sprintf(str,"%.11g",fieldtrmin[i]);
   XtVaSetValues(trselmintxt,XmNvalue,str,NULL);
   sprintf(str,"%.11g",fieldtrmax[i]);
   XtVaSetValues(trselmaxtxt,XmNvalue,str,NULL);
}


void trboxsubsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];

   traceselbox_draw = 1;
   arg = (long int)client_data;

   XmScaleGetValue(w,&ival);
   val = (float) ival / 1000.;
 
   if (arg == 0 || arg == 6)
     {
      traceselbox_xmin = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",traceselbox_xmin);
      XtVaSetValues(trselboxx1txt,XmNvalue,str,NULL);
      if (traceselbox_xmin > traceselbox_xmax)
        {
         XmScaleSetValue(trselboxx2sl,ival);
         XtVaSetValues(trselboxx2txt,XmNvalue,str,NULL);
         traceselbox_xmax = traceselbox_xmin;
        }
     }
   if (arg == 1 || arg == 7)
     {
      traceselbox_xmax = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",traceselbox_xmax);
      XtVaSetValues(trselboxx2txt,XmNvalue,str,NULL);
      if (traceselbox_xmin > traceselbox_xmax)
        {
         XmScaleSetValue(trselboxx1sl,ival);
         XtVaSetValues(trselboxx1txt,XmNvalue,str,NULL);
         traceselbox_xmin = traceselbox_xmax;
        }
     }
   if (arg == 2 || arg == 8)
     {
      traceselbox_ymin = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",traceselbox_ymin);
      XtVaSetValues(trselboxy1txt,XmNvalue,str,NULL);
      if (traceselbox_ymin > traceselbox_ymax)
        {
         XmScaleSetValue(trselboxy2sl,ival);
         XtVaSetValues(trselboxy2txt,XmNvalue,str,NULL);
         traceselbox_ymax = traceselbox_ymin;
        }
     }
   if (arg == 3 || arg == 9)
     {
      traceselbox_ymax = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",traceselbox_ymax);
      XtVaSetValues(trselboxy2txt,XmNvalue,str,NULL);
      if (traceselbox_ymin > traceselbox_ymax)
        {
         XmScaleSetValue(trselboxy1sl,ival);
         XtVaSetValues(trselboxy1txt,XmNvalue,str,NULL);
         traceselbox_ymin = traceselbox_ymax;
        }
     }
   if (arg == 4 || arg == 10)
     {
      traceselbox_zmin = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",traceselbox_zmin);
      XtVaSetValues(trselboxz1txt,XmNvalue,str,NULL);
      if (traceselbox_zmin > traceselbox_zmax)
        {
         XmScaleSetValue(trselboxz2sl,ival);
         XtVaSetValues(trselboxz2txt,XmNvalue,str,NULL);
         traceselbox_zmax = traceselbox_zmin;
        }
     }
   if (arg == 5 || arg == 11)
     {
      traceselbox_zmax = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",traceselbox_zmax);
      XtVaSetValues(trselboxz2txt,XmNvalue,str,NULL);
      if (traceselbox_zmin > traceselbox_zmax)
        {
         XmScaleSetValue(trselboxz1sl,ival);
         XtVaSetValues(trselboxz1txt,XmNvalue,str,NULL);
         traceselbox_zmin = traceselbox_zmax;
        }
     }

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


void trselboxtxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr, str[20];
  float val;
  void settraceselboxwidgets();

   traceselbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0 || arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      if (arg == 0)
        {
         traceselbox_xmin = val;
         if (traceselbox_xmin > traceselbox_xmax)
            traceselbox_xmax = traceselbox_xmin;
        }
      else
        {
         traceselbox_xmax = val;
         if (traceselbox_xmin > traceselbox_xmax)
            traceselbox_xmin = traceselbox_xmax;
        }
     }
 
   if (arg == 2 || arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      if (arg == 2)
        {
         traceselbox_ymin = val;
         if (traceselbox_ymin > traceselbox_ymax)
            traceselbox_ymax = traceselbox_ymin;
        }
      else
        {
         traceselbox_ymax = val;
         if (traceselbox_ymin > traceselbox_ymax)
            traceselbox_ymin = traceselbox_ymax;
        }
     }
 
   if (arg == 4 || arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      if (arg == 4)
        {
         traceselbox_zmin = val;
         if (traceselbox_zmin > traceselbox_zmax)
            traceselbox_zmax = traceselbox_zmin;
        }
      else
        {
         traceselbox_zmax = val;
         if (traceselbox_zmin > traceselbox_zmax)
            traceselbox_zmin = traceselbox_zmax;
        }
     }

   XtFree(tmpstr);

   settraceselboxwidgets();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void trnumclr(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char blank[1] = "";

   for (i = 0; i < MAXNUMSEL; i++)
     {
      XmTextFieldSetString(trselnumtxt[i],blank);
     }
}


void maketraceselform(parent)
 Widget parent;
{
  Widget trsclose, trsapply, trsfldbut, trsphbut,
         trsfldclose, trselfldlab1, trselfldbut, trsfldset,
         trselminlab1, trselmaxlab1, trselrangelab, trselminlab2,  
         trselmaxlab2, trselreset, trsphclose, trsphxlab, trsphylab,
         trsphzlab, trsphrlab, trsnumbut, trsnumclose, trsnumwin,
         trsnumform, trnumlab[MAXNUMSEL], trsfldapply, trselresetall,
         trsphapply, trsnumapply, trsphbox, trboxclose, trboxapply,
         trboxx1lab, trboxx2lab, trboxy1lab, trboxy2lab,
         trboxz1lab, trboxz2lab, trboxbox, trboxbut, trnumclrbut;
  int i, j;
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Particle Select",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   traceselform = XmCreateFormDialog(parent,"Tracer_Select",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trsclose = XtVaCreateManagedWidget("Trsclose",
             xmPushButtonGadgetClass,
             traceselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(trsclose,XmNactivateCallback,trselclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   trsapply = XtVaCreateManagedWidget("Trsapply",
             xmPushButtonGadgetClass,
             traceselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, trsclose,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(trsapply,XmNactivateCallback,trselapply,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   trselonbut[1] = XtVaCreateManagedWidget("Trselonbut1",
                  xmToggleButtonWidgetClass,
                  traceselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trsclose,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut[1],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 1);

   string = XmStringLtoRCreate("Particle Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   trsfldbut = XtVaCreateManagedWidget("Trsfldbut",
              xmPushButtonGadgetClass,
              traceselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trsclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, trselonbut[1],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trsfldbut,XmNactivateCallback,tracefldselformpop,NULL);
   if (numvartr == 0)
     {
      XtSetSensitive(trselonbut[1],FALSE);
      XtSetSensitive(trsfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   trselonbut[2] = XtVaCreateManagedWidget("Trselonbut2",
                  xmToggleButtonWidgetClass,
                  traceselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trsfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut[2],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 2);

   string = XmStringLtoRCreate("Search\nSphere",
                               XmSTRING_DEFAULT_CHARSET);
   trsphbut = XtVaCreateManagedWidget("Trsphbut",
               xmPushButtonGadgetClass,
               traceselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trsfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, trselonbut[2],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(trsphbut,XmNactivateCallback,tracesphselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   trselonbut[3] = XtVaCreateManagedWidget("Trselonbut3",
                  xmToggleButtonWidgetClass,
                  traceselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trsphbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut[3],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 3);

   string = XmStringLtoRCreate("Particle\nNumbers",
                               XmSTRING_DEFAULT_CHARSET);
   trsnumbut = XtVaCreateManagedWidget("Trsnumbut",
              xmPushButtonGadgetClass,
              traceselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, trsphbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, trselonbut[3],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(trsnumbut,XmNactivateCallback,tracenumselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   trselonbut[4] = XtVaCreateManagedWidget("Trselonbut4",
                  xmToggleButtonWidgetClass,
                  traceselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trsnumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut[4],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 4);

   string = XmStringLtoRCreate("Search\nBox",
                               XmSTRING_DEFAULT_CHARSET);
   trboxbut = XtVaCreateManagedWidget("Trboxbut",
               xmPushButtonGadgetClass,
               traceselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trsnumbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, trselonbut[4],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(trboxbut,XmNactivateCallback,traceboxselformpop,NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   trselerror = XtVaCreateManagedWidget("Trselerror",
               xmLabelWidgetClass,
               traceselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trboxbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 


   /*  Create Particle Field Data Range selection menu.  */
   /****************************************************/
 
   if (numvartr > 0)
     {
      string = XmStringCreate("Particle Select - Particle Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      tracefldselform = XmCreateFormDialog(traceselform,"Tracefldselform",
                                          wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      trsfldclose = XtVaCreateManagedWidget("Trsfldclose",
                    xmPushButtonGadgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(trsfldclose,XmNactivateCallback,trselfldclose,NULL);

      string = XmStringCreate("Set",XmSTRING_DEFAULT_CHARSET);
      trsfldset = XtVaCreateManagedWidget("Trsfldset",
                  xmPushButtonGadgetClass,
                  tracefldselform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trsfldclose,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(trsfldset,XmNactivateCallback,trselfldset,NULL);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      trselflderror = XtVaCreateManagedWidget("Trselflderror",
                      xmLabelWidgetClass,
                      tracefldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, trsfldclose,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string); 

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      trselfldlab1 = XtVaCreateManagedWidget("Trselfldlab1",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselflderror,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      trselfldlab = XtVaCreateManagedWidget("Trselfldlab",
                   xmLabelWidgetClass,
                   tracefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trselflderror,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, trselfldlab1,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      trselfldbut = XtVaCreateManagedWidget("Trselfldbut",
                   xmPushButtonGadgetClass,
                   tracefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trselfldlab1,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(trselfldbut,XmNactivateCallback,trseltrfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      trselminlab1 = XtVaCreateManagedWidget("Trselminlab1",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselfldbut,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      trselminlab = XtVaCreateManagedWidget("Trselminlab",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, trselminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      trselmaxlab1 = XtVaCreateManagedWidget("Trselmaxlab1",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselminlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      trselmaxlab = XtVaCreateManagedWidget("Trselmaxlab",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, trselmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      trselrangelab = XtVaCreateManagedWidget("Trselrangelab1",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselmaxlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      trselminlab2 = XtVaCreateManagedWidget("Trselminlab2",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselrangelab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      trselmintxt = XtVaCreateManagedWidget("Trselmintxt",
                   xmTextFieldWidgetClass,
                   tracefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trselrangelab,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, trselminlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      trselmaxlab2 = XtVaCreateManagedWidget("Trselmaxlab2",
                    xmLabelWidgetClass,
                    tracefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselminlab2,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      trselmaxtxt = XtVaCreateManagedWidget("Trselmaxtxt",
                   xmTextFieldWidgetClass,
                   tracefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trselminlab2,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, trselmaxlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      trselreset = XtVaCreateManagedWidget("Trselreset",
                  xmPushButtonGadgetClass,
                  tracefldselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, trselmaxtxt,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(trselreset,XmNactivateCallback,trselfldreset,
                       NULL);

      string = XmStringCreate("Reset All",XmSTRING_DEFAULT_CHARSET);
      trselresetall = XtVaCreateManagedWidget("Trselresetall",
                     xmPushButtonGadgetClass,
                     tracefldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, trselreset,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string);
         XtAddCallback(trselresetall,XmNactivateCallback,trselfldresetall,
                       NULL);
     }


   /*  Create Search Sphere selection menu.  */
   /*****************************************/

   string = XmStringCreate("Particle Select - Search Sphere",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   tracesphselform = XmCreateFormDialog(traceselform,"Tracesphselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trsphclose = XtVaCreateManagedWidget("Trsphclose",
                xmPushButtonGadgetClass,
                tracesphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trsphclose,XmNactivateCallback,trselsphclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   trsphapply = XtVaCreateManagedWidget("Trsphapply",
                xmPushButtonGadgetClass,
                tracesphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trsphclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trsphapply,XmNactivateCallback,trselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   trselonbut2[2] = XtVaCreateManagedWidget("Trselonbut22",
                   xmToggleButtonWidgetClass,
                   tracesphselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trsphclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut2[2],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 2);

   string = XmStringCreate("X:",XmSTRING_DEFAULT_CHARSET);
   trsphxlab = XtVaCreateManagedWidget("Trsphxlab",
               xmLabelWidgetClass,
               tracesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trselonbut2[2],
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   trselsphxtxt = XtVaCreateManagedWidget("Trselsphxtxt",
                 xmTextFieldWidgetClass,
                 tracesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trselonbut2[2],
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trsphxlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Y:",XmSTRING_DEFAULT_CHARSET);
   trsphylab = XtVaCreateManagedWidget("Trsphylab",
               xmLabelWidgetClass,
               tracesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trsphxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   trselsphytxt = XtVaCreateManagedWidget("Trselsphytxt",
                 xmTextFieldWidgetClass,
                 tracesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trsphxlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trsphylab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Z:",XmSTRING_DEFAULT_CHARSET);
   trsphzlab = XtVaCreateManagedWidget("Trsphzlab",
               xmLabelWidgetClass,
               tracesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trsphylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   trselsphztxt = XtVaCreateManagedWidget("Trselsphztxt",
                 xmTextFieldWidgetClass,
                 tracesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trsphylab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trsphzlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("R:",XmSTRING_DEFAULT_CHARSET);
   trsphrlab = XtVaCreateManagedWidget("Trsphrlab",
               xmLabelWidgetClass,
               tracesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, trsphzlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   trselsphrtxt = XtVaCreateManagedWidget("Trselsphrtxt",
                 xmTextFieldWidgetClass,
                 tracesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trsphzlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trsphrlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trselsphrtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   trsphbox = XmCreateRadioBox(tracesphselform,"Trsphbox",wargs,i);
   XtManageChild(trsphbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   trselsphin = XtVaCreateManagedWidget("Trselsphin",
               xmToggleButtonGadgetClass,
               trsphbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(trselsphin,XmNvalueChangedCallback,
                   trselsphsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   trselsphout = XtVaCreateManagedWidget("Trselsphout",
                xmToggleButtonGadgetClass,
                trsphbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(trselsphout,XmNvalueChangedCallback,
                   trselsphsetinopt,(caddr_t) 0);


   /*  Create Search Box selection menu.  */
   /***************************************/

   string = XmStringCreate("Particle Select - Search Box",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   traceboxselform = XmCreateFormDialog(traceselform,"Traceboxselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trboxclose = XtVaCreateManagedWidget("Trboxclose",
                xmPushButtonGadgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trboxclose,XmNactivateCallback,trselboxclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   trboxapply = XtVaCreateManagedWidget("Trboxapply",
                xmPushButtonGadgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trboxclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trboxapply,XmNactivateCallback,trselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   trselonbut2[4] = XtVaCreateManagedWidget("Trselonbut25",
                   xmToggleButtonWidgetClass,
                   traceboxselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trboxclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut2[4],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 4);

   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   trboxx1lab = XtVaCreateManagedWidget("Trboxx1lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselonbut2[4],
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxx1sl = XtVaCreateManagedWidget("Trselboxx1sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxx1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxx1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 0);
      XtAddCallback(trselboxx1sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 6);

   trselboxx1txt = XtVaCreateManagedWidget("Trselboxx1txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxx1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxx1txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   trboxx2lab = XtVaCreateManagedWidget("Trboxx2lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselonbut2[4],
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trselboxx1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxx2sl = XtVaCreateManagedWidget("Trselboxx2sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxx2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxx2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 1);
      XtAddCallback(trselboxx2sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 7);

   trselboxx2txt = XtVaCreateManagedWidget("Trselboxx2txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxx2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxx2txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   trboxy1lab = XtVaCreateManagedWidget("Trboxy1lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselboxx1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxy1sl = XtVaCreateManagedWidget("Trselboxy1sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxy1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxy1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 2);
      XtAddCallback(trselboxy1sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 8);

   trselboxy1txt = XtVaCreateManagedWidget("Trselboxy1txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxy1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxy1txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   trboxy2lab = XtVaCreateManagedWidget("Trboxy2lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselboxx1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trselboxy1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxy2sl = XtVaCreateManagedWidget("Trselboxy2sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxy2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxy2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 3);
      XtAddCallback(trselboxy2sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 9);

   trselboxy2txt = XtVaCreateManagedWidget("Trselboxy2txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxy2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxy2txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   trboxz1lab = XtVaCreateManagedWidget("Trboxz1lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselboxy1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxz1sl = XtVaCreateManagedWidget("Trselboxz1sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxz1lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxz1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 4);
      XtAddCallback(trselboxz1sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 10);

   trselboxz1txt = XtVaCreateManagedWidget("Trselboxz1txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxz1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxz1txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   trboxz2lab = XtVaCreateManagedWidget("Trboxz2lab",
                xmLabelWidgetClass,
                traceboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, trselboxy1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trselboxz1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   trselboxz2sl = XtVaCreateManagedWidget("Trselboxz2sl",
                 xmScaleWidgetClass,
                 traceboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, trboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, trboxz2lab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNminimum, 0,
                 XmNmaximum, 1000,
                 XmNorientation, XmHORIZONTAL,
                 XmNprocessingDirection, XmMAX_ON_RIGHT,
                 XmNvalue, 0,
                 XmNscaleWidth, 200,
                 XmNscaleMultiple, 1,
                 NULL);
      XtAddCallback(trselboxz2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 5);
      XtAddCallback(trselboxz2sl,XmNdragCallback,trboxsubsetsl,
                    (caddr_t) 11);

   trselboxz2txt = XtVaCreateManagedWidget("Trselboxz2txt",
                  xmTextFieldWidgetClass,
                  traceboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, trboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, trselboxz2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(trselboxz2txt,XmNactivateCallback,
                    trselboxtxt,(caddr_t) 5);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trselboxz1txt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   trboxbox = XmCreateRadioBox(traceboxselform,"trboxbox",wargs,i);
   XtManageChild(trboxbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   trselboxin = XtVaCreateManagedWidget("Trselboxin",
               xmToggleButtonGadgetClass,
               trboxbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(trselboxin,XmNvalueChangedCallback,
                   trselboxsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   trselboxout = XtVaCreateManagedWidget("Trselboxout",
                xmToggleButtonGadgetClass,
                trboxbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(trselboxout,XmNvalueChangedCallback,
                   trselboxsetinopt,(caddr_t) 0);


   /*  Create Particle Number selection menu.  */
   /****************************************/

   string = XmStringCreate("Particle Select - Particle Number",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   tracenumselform = XmCreateFormDialog(traceselform,"Tracenumselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   trsnumclose = XtVaCreateManagedWidget("Trsnumclose",
                xmPushButtonGadgetClass,
                tracenumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trsnumclose,XmNactivateCallback,trselnumclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   trsnumapply = XtVaCreateManagedWidget("Trsnumapply",
                xmPushButtonGadgetClass,
                tracenumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, trsnumclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(trsnumapply,XmNactivateCallback,trselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   trselonbut2[3] = XtVaCreateManagedWidget("Trselonbut23",
                   xmToggleButtonWidgetClass,
                   tracenumselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, trsnumclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(trselonbut2[3],XmNvalueChangedCallback,trselsetonbut,
                    (caddr_t) 3);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, trselonbut2[3]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 150);
   i++;  XtSetArg(wargs[i],XmNheight, 250);
   i++;
   trsnumwin = XmCreateScrolledWindow(tracenumselform,"Trsnumwin",wargs,i);
   XtManageChild(trsnumwin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   trsnumform = XmCreateForm(trsnumwin,"Trsnumform",wargs,i);

   string = XmStringCreate(" 1",XmSTRING_DEFAULT_CHARSET);
   trnumlab[0] = XtVaCreateManagedWidget("Trsnumlab0",
                 xmLabelWidgetClass,
                 trsnumform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   trselnumtxt[0] = XtVaCreateManagedWidget("Trselnumtxt0",
                   xmTextFieldWidgetClass,
                   trsnumform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, trnumlab[0],
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 29,
                   XmNcolumns, 29,
                   XmNvalue, "",
                   NULL);

   for (i = 1; i < MAXNUMSEL; i++)
     {
      sprintf(str,"%2d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Trsnumlab%d",i);
      trnumlab[i] = XtVaCreateManagedWidget("str",
                    xmLabelWidgetClass,
                    trsnumform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, trselnumtxt[i-1],
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string); 

      sprintf(str,"Trselnumtxt%d",i);
      trselnumtxt[i] = XtVaCreateManagedWidget(str,
                      xmTextFieldWidgetClass,
                      trsnumform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, trselnumtxt[i-1],
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, trnumlab[i],
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 1,
                      XmNmaxLength, 29,
                      XmNcolumns, 29,
                      XmNvalue, "",
                      NULL);
     }

   string = XmStringCreate("Clear Entries",
                               XmSTRING_DEFAULT_CHARSET);
   trnumclrbut = XtVaCreateManagedWidget("Trnumclrbut",
                 xmPushButtonGadgetClass,
                 tracenumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, trsnumwin,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(trnumclrbut,XmNactivateCallback,trnumclr,
                    (caddr_t) 0);

   XtManageChild(trsnumform);

}


void settracesel()
{
  int i, j, ii, id;
  long ifirst, ilast, istride;
  short isel, jsel;
  float xb, yb, zb;
  double rad, dist, xd, yd, zd;
  double value;

   /*  For search sphere test, square the sphere radius.  */
   rad = (double)traceselsph_r * (double)traceselsph_r;

   /*  Loop through traces and set selections.  */
   for (i = 0; i < ntracers; i++)
     {
      traceselectflag[i] = 1;

      /*  Check field range, if requested.  */
      isel = (int)traceselectflag[i];
      if (traceselfld_on && isel)
        {
         jsel = 1;
         for (j= 0; j < numvartr; j++)
         {
             if(traceselfields[j])
             {
               value = tracers[0].field[j][i];
               if (value < trfldselmin[j] || value > trfldselmax[j])
                 jsel = 0;
             }
         }

         traceselectflag[i] = jsel;
        }

      /*  Now check search sphere, if requested.  */
      isel = (int)traceselectflag[i];
      if (traceselsph_on && isel)
        {
         jsel = 0;

         xd = tracers[0].x[i] - (double)traceselsph_x;
         yd = tracers[0].y[i] - (double)traceselsph_y;
         zd = tracers[0].z[i] - (double)traceselsph_z;
         dist = xd*xd + yd*yd + zd*zd;
         if (traceselsph_in == 1 && dist <= rad)
            jsel = 1;
         if (traceselsph_in == 0 && dist >= rad)
            jsel = 1;

         traceselectflag[i] = jsel;
        }

      /*  Now check search box, if requested.  */
      isel = (int)traceselectflag[i];
      if (traceselbox_on && isel)
        {
         jsel = 0;

         xb = tracers[0].x[i];
         yb = tracers[0].y[i];
         zb = tracers[0].z[i];

         if (traceselbox_in == 1)
           {
            if (traceselbox_xmin <= xb && xb <= traceselbox_xmax &&
                traceselbox_ymin <= yb && yb <= traceselbox_ymax &&
                traceselbox_zmin <= zb && zb <= traceselbox_zmax)
              jsel = 1;
           }
         if (traceselbox_in == 0)
           {
            if (xb <= traceselbox_xmin || traceselbox_xmax <= xb ||
                yb <= traceselbox_ymin || traceselbox_ymax <= yb ||
                zb <= traceselbox_zmin || traceselbox_zmax <= zb)
               jsel = 1;
           }

         traceselectflag[i] = jsel;
        }
    }

   /*  Check trace number selection, and with  */
   /*  current select flag for those traces.   */
   if (traceselnum_on)
     {

      /*  Loop through traces, and get a trace id.  */
      for (ii = 0; ii < ntracers; ii++)
        {
         id = ii + 1;
         if (traceridflag) id = tracerids[ii];

         /*  Loop through active first, last, stride sets.  */
         for (j = 0; j < ntraceselnum; j++)
           {
            ifirst = traceselnum_f[j];
            ilast = traceselnum_l[j];
            istride = traceselnum_s[j];

            if (ifirst <= id && id <= ilast)
              {
               for (i = ifirst; i <= ilast; i+=istride)
                 {
                  if (i == id)
                    {
                     isel = (int)traceselectflag[ii];

                     /*  If previously selected, set value to 2.  */
                     if (isel) traceselectflag[ii] = 2;
                    }
                 }
              }
           }
        }

      /*  Now loop through traces, and keep those with a 2.  */
      for (i = 0; i < ntracers; i++)
        {
         jsel = 0;
         isel = (int)traceselectflag[i];
         if (isel == 2) jsel = 1;
         traceselectflag[i] = jsel;
        }
     }
}


void traceselgettrfield(int i)
{
  char str[20];
  XmString string;

  traceselfield = i;

   string = XmStringCreate(fieldtrname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trselfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",fieldtrmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trselminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",fieldtrmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(trselmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",trfldselmin[i]);
   XtVaSetValues(trselmintxt,XmNvalue,str,NULL);
   sprintf(str,"%.11g",trfldselmax[i]);
   XtVaSetValues(trselmaxtxt,XmNvalue,str,NULL);
}


void settraceselboxwidgets()
{
  int ival;
  float val;
  char str[20];

   XtRemoveCallback(trselboxx1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 0);
   XtRemoveCallback(trselboxx2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 1);
   XtRemoveCallback(trselboxy1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 2);
   XtRemoveCallback(trselboxy2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 3);
   XtRemoveCallback(trselboxz1sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 4);
   XtRemoveCallback(trselboxz2sl,XmNvalueChangedCallback,trboxsubsetsl,
                    (caddr_t) 5);
   XFlush(dpy);

   sprintf(str,"%.11g",traceselbox_xmin);
   XmTextFieldSetString(trselboxx1txt,str);
   val = (traceselbox_xmin - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxx1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",traceselbox_xmax);
   XmTextFieldSetString(trselboxx2txt,str);
   val = (traceselbox_xmax - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxx2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",traceselbox_ymin);
   XmTextFieldSetString(trselboxy1txt,str);
   val = (traceselbox_ymin - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxy1sl,XmNvalue,ival, NULL);

   sprintf(str,"%.11g",traceselbox_ymax);
   XmTextFieldSetString(trselboxy2txt,str);
   val = (traceselbox_ymax - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxy2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",traceselbox_zmin);
   XmTextFieldSetString(trselboxz1txt,str);
   val = (traceselbox_zmin - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxz1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",traceselbox_zmax);
   XmTextFieldSetString(trselboxz2txt,str);
   val = (traceselbox_zmax - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(trselboxz2sl, XmNvalue,ival, NULL);

   XFlush(dpy);
   XtAddCallback(trselboxx1sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 0);
   XtAddCallback(trselboxx2sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 1);
   XtAddCallback(trselboxy1sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 2);
   XtAddCallback(trselboxy2sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 3);
   XtAddCallback(trselboxz1sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 4);
   XtAddCallback(trselboxz2sl,XmNvalueChangedCallback,trboxsubsetsl,(caddr_t) 5);
   XFlush(dpy);
}

void settraceselwidgets()
{
  char str[20];

   settraceselboxwidgets();

   sprintf(str,"%.11g",traceselsph_x);
   XmTextFieldSetString(trselsphxtxt,str);

   sprintf(str,"%.11g",traceselsph_y);
   XmTextFieldSetString(trselsphytxt,str);

   sprintf(str,"%.11g",traceselsph_z);
   XmTextFieldSetString(trselsphztxt,str);

   sprintf(str,"%.11g",traceselsph_r);
   XmTextFieldSetString(trselsphrtxt,str);
}
