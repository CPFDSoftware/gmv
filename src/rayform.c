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

#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

#include "gmvdata.h"
#include "gmvread.h"
#include "formwidgets.h"
#include "xstuff.h"

#define RAYCOLORS 7

static Widget rfldsbut[MAXRAYFIELDS], rcolsbut[RAYCOLORS], raynone, 
              rayselerror, rayselnumtxt[MAXNUMSEL], raynumselform,
              rayselonbut, rayselbut;
static short currrayflag = 0, rfieldactive = -1;


void rayformpop()
{
  int i;

   XtVaSetValues(raynone,XmNset,FALSE,NULL);
   for (i = 0; i < RAYCOLORS; i++)
      XtVaSetValues(rcolsbut[i],XmNset,FALSE,NULL);
   for (i = 0; i < rnumvars; i++)
      XtVaSetValues(rfldsbut[i],XmNset,FALSE,NULL);
   if (rayflag == 0)
      XtVaSetValues(raynone,XmNset,TRUE,NULL);
   else if (rayflag < 0)
     {
      i = -rayflag - 1;
      XtVaSetValues(rcolsbut[i],XmNset,TRUE,NULL);
     }
   else
      XtVaSetValues(rfldsbut[rayfieldactive],XmNset,TRUE,NULL);

   currrayflag = rayflag;
   rfieldactive = rayfieldactive;

   MyManageChild(rayform);
}


void rayclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(raynumselform);
   XtUnmanageChild(rayform);
}


void raynumselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[30];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(rayselonbut,rayselnum_on,FALSE);

   /*  Fill text lines with ray number select data.  */
   if (nrayselnum > 0)
     {
      for (i = 0; i < nrayselnum; i++)
        {
         if (rayselnum_f[i] == rayselnum_l[i])
            sprintf(str,"%d",rayselnum_f[i]);
         else if (rayselnum_s[i] == 1)
            sprintf(str,"%d:%d",rayselnum_f[i],rayselnum_l[i]);
         else
            sprintf(str,"%d:%d:%d",rayselnum_f[i],rayselnum_l[i],
                                   rayselnum_s[i]);
         XmTextFieldSetString(rayselnumtxt[i],str);
        }
      for (i = nrayselnum; i < MAXNUMSEL; i++)
         XmTextFieldSetString(rayselnumtxt[i],"");
     }

   /*  Calculate select pop-up postition from widgets.  */
   XtTranslateCoords(rayselbut, p0,p0, &wx,&wy); 
   fx = wx;
   fy = wy + 80;
   XtVaSetValues(raynumselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(raynumselform);
}


void rayselnumclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(raynumselform);
}


void select_ray_color(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, i;

   arg = (long int)client_data;
   i = -arg;
   currrayflag = i;
}


void select_ray_field(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   rfieldactive = arg;
   currrayflag = arg + 1;
}


void ray_apply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   rayflag = currrayflag;
   rayfieldactive = rfieldactive;
   if (rayflag == 0) drawrayflag = 0;
   else drawrayflag = 1;

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void rayselapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ierr, tmpstrlen;
  unsigned long num;
  char *tmpstr;
  static char *token;
  static char str4[55]; 
  XmString string;
  char *chk_num = "0123456789: ", *chk_blank = " ";
  void setraysel();

   /*  Clear the error message widget.  */
   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(rayselerror,XmNlabelString,string,NULL);
   XmStringFree(string);   

   ierr = 0;

   /*  Read and check ray number values.  */
   if (rayselnum_on && ierr == 0)
     {
      nrayselnum = 0;
      i = 0;

      while(ierr == 0 && i < MAXNUMSEL)
        {
         tmpstr = XmTextFieldGetString(rayselnumtxt[i]);
         tmpstrlen = strlen(tmpstr);
         if (tmpstrlen > 0)
           {

            /*  Check for invalid characters.  */
            if (strspn(tmpstr,chk_num) != tmpstrlen)
              {
               ierr = 1;
               sprintf(str4,"Error - invalid character in line %d",i+1);
               string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
               XtVaSetValues(rayselerror,XmNlabelString,string,NULL);
               XmStringFree(string);
              }

            /*  If not all blanks, extract data from the string.  */
            if (ierr == 0 && strspn(tmpstr,chk_blank) != tmpstrlen)
              {

               /*  Get the first number from the set.  */
               token = strtok(tmpstr,":");
               num = strtoul(token,NULL,10);
               if (num > nrays && !rayidflag) num = nrays;
               rayselnum_f[nrayselnum] = num;

               /*  Get the last number from the set.  */
               rayselnum_l[nrayselnum] = num;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  if (num > nrays && !rayidflag) num = nrays;

                  /*  If first greater than last, switch.  */
                  if (num >= rayselnum_f[i])
                     rayselnum_l[nrayselnum] = num;
                  else
                    {
                     rayselnum_l[i] = rayselnum_f[i];
                     rayselnum_f[i] = num;
                    }
                 }

               /*  Get the stride from the set.  */
               rayselnum_s[nrayselnum] = 1;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  rayselnum_s[nrayselnum] = num;
                 }

               nrayselnum++;
              }
           }
         XtFree(tmpstr);
         i++;
        }

     }

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      setraysel();
      if (displistflag) drawnewlistflag = 1;
      drawnew3dflag = 1;
     }

   fastdraw = 0;
   drawit();
}


void rayselsetonbut(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   rayselnum_on = (short)XmToggleButtonGetState(w);
}


void makerayform(parent)
 Widget parent;
{
  Widget rayclosew, rayapply, raycollab, raycolswin, raycolbox,
         raysnumclose, raysapply, raysnumwin, raysnumform,
         raysnumlab[MAXNUMSEL];
  int i, nflds;
  char *rcols[RAYCOLORS] = {"Black","White","Grey","Red",
                            "Green","Yellow","Blue"};
  char str[20];
  Arg wargs[20];
  XmString string; 

   string = XmStringCreate("Rays",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   rayform = XmCreateFormDialog(parent,"Rays",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   rayclosew = XtVaCreateManagedWidget("Rayclosew",
               xmPushButtonGadgetClass,
               rayform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(rayclosew,XmNactivateCallback,rayclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   rayapply = XtVaCreateManagedWidget("Rayapply",
              xmPushButtonGadgetClass,
              rayform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, rayclosew,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rayapply,XmNactivateCallback,ray_apply,
                    NULL);
 
   string = XmStringCreate("Color by:",XmSTRING_DEFAULT_CHARSET);
   raycollab = XtVaCreateManagedWidget("Raycollab",
               xmLabelWidgetClass,
               rayform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, rayapply,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, raycollab);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 160);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   i++;
   raycolswin = XmCreateScrolledWindow(rayform,"Raycolswin",wargs,i);
   XtManageChild(raycolswin);

   i=0;
   raycolbox = XmCreateRadioBox(raycolswin,"Raycolbox",wargs,i);
   XtManageChild(raycolbox);
 
   /*  Create ray color buttons.  */
 
   string = XmStringCreate("None",
            XmSTRING_DEFAULT_CHARSET);
   raynone = XtVaCreateManagedWidget("Raynone",
             xmToggleButtonGadgetClass,
             raycolbox,
             XmNlabelString, string,
             XmNset, TRUE,
             XmNspacing, 2,
             XmNselectColor, yellowpix,
             NULL);
      XmStringFree(string);
      XtAddCallback(raynone,XmNvalueChangedCallback,
                    select_ray_color,(caddr_t) 0);

   for (i = 0; i < RAYCOLORS; i++)
     {
      string = XmStringCreate(rcols[i],
               XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Raycolsbut%d",i);
      rcolsbut[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonGadgetClass,
                    raycolbox,
                    XmNlabelString, string,
                    XmNspacing, 1,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    NULL);
         XmStringFree(string);
         XtAddCallback(rcolsbut[i],XmNvalueChangedCallback,
                       select_ray_color,(caddr_t) i+1);
   }
  
   /*  Create field buttons.  */
   nflds = rnumvars;
   for (i = 0; i < nflds; i++)
     {
      string = XmStringCreate(rfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Rayfldsbut%d",i);
      rfldsbut[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonGadgetClass,
                    raycolbox,
                    XmNlabelString, string,
                    XmNspacing, 1,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    NULL);
         XmStringFree(string);
         XtAddCallback(rfldsbut[i],XmNvalueChangedCallback,
                       select_ray_field,(caddr_t) i);
     }
  
   string = XmStringCreate("Select",
            XmSTRING_DEFAULT_CHARSET);
   rayselbut = XtVaCreateManagedWidget("Rayselbut",
               xmPushButtonGadgetClass,
               rayform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, raycolswin,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(rayselbut,XmNactivateCallback,raynumselformpop,NULL);


   /*  Create Ray Number selection menu.  */
   /***************************************/

   string = XmStringCreate("Ray Select by Number",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   raynumselform = XmCreateFormDialog(rayform,"Raynumselform",wargs,i);
      XmStringFree(string);

   XtManageChild(raynumselform);
   XtUnmanageChild(raynumselform);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   raysnumclose = XtVaCreateManagedWidget("Raysnumclose",
                  xmPushButtonGadgetClass,
                  raynumselform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(raysnumclose,XmNactivateCallback,rayselnumclose,NULL);
 
   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   raysapply = XtVaCreateManagedWidget("Raysnumclose",
               xmPushButtonGadgetClass,
               raynumselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, raysnumclose,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(raysapply,XmNactivateCallback,rayselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   rayselonbut = XtVaCreateManagedWidget("Rayselonbut",
                 xmToggleButtonWidgetClass,
                 raynumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, raysapply,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNselectColor, yellowpix,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(rayselonbut,XmNvalueChangedCallback,rayselsetonbut,
                    NULL);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   rayselerror = XtVaCreateManagedWidget("Rayselerror",
                 xmLabelWidgetClass,
                 raynumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, rayselonbut,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, rayselerror);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 150);
   i++;  XtSetArg(wargs[i],XmNheight, 250);
   i++;
   raysnumwin = XmCreateScrolledWindow(raynumselform,"Raysnumwin",wargs,i);
   XtManageChild(raysnumwin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   raysnumform = XmCreateForm(raysnumwin,"Raysnumform",wargs,i);

   string = XmStringCreate(" 1",XmSTRING_DEFAULT_CHARSET);
   raysnumlab[0] = XtVaCreateManagedWidget("Raysnumlab0",
                   xmLabelWidgetClass,
                   raysnumform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string); 

   rayselnumtxt[0] = XtVaCreateManagedWidget("Rayselnumtxt0",
                     xmTextFieldWidgetClass,
                     raysnumform,
                     XmNtopAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, raysnumlab[0],
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
      sprintf(str,"Raysnumlab%d",i);
      raysnumlab[i] = XtVaCreateManagedWidget("str",
                      xmLabelWidgetClass,
                      raysnumform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, rayselnumtxt[i-1],
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
      XmStringFree(string); 

      sprintf(str,"Rayselnumtxt%d",i);
      rayselnumtxt[i] = XtVaCreateManagedWidget(str,
                        xmTextFieldWidgetClass,
                        raysnumform,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopWidget, rayselnumtxt[i-1],
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftWidget, raysnumlab[i],
                        XmNrightAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_NONE,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 1,
                        XmNmaxLength, 29,
                        XmNcolumns, 29,
                        XmNvalue, "",
                        NULL);
     }
   XtManageChild(raysnumform);
}


void setraysel()
{
  int i, j, ifirst, ilast, istride, ii, id;
  short isel, jsel;

   /*  Loop through rays and set on.  */
   for (i = 0; i < nrays; i++) 
      rayselectflag[i] = 1;

   /*  Check ray number selection.  */
   if (rayselnum_on)
     {

      /*  Loop through rays, and get a ray id.  */
      for (ii = 0; ii < nrays; ii++)
        {
         id = ii + 1;
         if (rayidflag) id = rayids[ii];

         /*  Loop through active first, last, stride sets.  */
         for (j = 0; j < nrayselnum; j++)
           {
            ifirst = rayselnum_f[j];
            ilast = rayselnum_l[j];
            istride = rayselnum_s[j];
            for (i = ifirst; i <= ilast; i+=istride)
            if (ifirst <= id && id <= ilast)
              {
               for (i = ifirst; i <= ilast; i+=istride)
                 {
                  if (i == id)
                    {
                     isel = (int)rayselectflag[ii];

                     /*  If previously selected, set value to 2.  */
                     if (isel) rayselectflag[ii] = 2;
                    }
                 }
              }
           }
        }

      /*  Now loop through rays, and keep those with a 2.  */
      for (i = 0; i < nrays; i++)
        {
         jsel = 0;
         isel = (int)rayselectflag[i];
         if (isel == 2) jsel = 1;
         rayselectflag[i] = jsel;
        }
     }
}
