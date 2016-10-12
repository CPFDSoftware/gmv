#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>
#include <Xm/FileSB.h>
#include <Xm/ScrolledW.h>
 
#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "lights.h"

#include <math.h>
#include <stdio.h>

extern XtAppContext app;

#define MFLY 26
static Widget animflysnap, animflyql, animflystart, animflystop,
              animflynrots, flynrotstxt, flylistframe[MFLY-1];
static Window animflystopwin;
static short nflylist = 0, nrotslist[MFLY];
static float xflyposlist[MFLY], yflyposlist[MFLY], zflyposlist[MFLY];
static float xflyrotlist[MFLY], yflyrotlist[MFLY], zflyrotlist[MFLY];
static float xflyrotbeg, yflyrotbeg, zflyrotbeg,
             xflyrotend, yflyrotend, zflyrotend;
static float xflyposbeg, yflyposbeg, zflyposbeg,
             xflyposend, yflyposend, zflyposend;
static float xflyposinc, yflyposinc, zflyposinc,
             xflyrotinc, yflyrotinc, zflyrotinc;
static short nflyrots = 30, iflyrot;
static short animflystopflag = 0, animflyqlflag = 0, curr_snapflag;
static char flightfilnam[256];

void animflystrt(), animflydraw(), animflyreset();


void animflystuff(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string, string2, string3;

   arg = (long int)client_data;

   if (arg == 0)
     {
      animfly_snapflag = 0;
      curr_snapflag = 0;
      XmToggleButtonGadgetSetState(animflysnap,FALSE,FALSE);
      animfly_count = 0;
      animflymenu_up = 0;
      XtUnmanageChild(animflyform);
     }

   if (arg == 1)
     {
      animflyqlflag = (int)XmToggleButtonGadgetGetState(w);
      if (animflyqlflag == 1)
        {
         XmToggleButtonGadgetSetState(animflysnap,FALSE,FALSE);
         curr_snapflag = 0;
        }
     }

   if (arg == 2)
     {
      curr_snapflag = (int)XmToggleButtonGadgetGetState(w);
      if (curr_snapflag == 1)
        {
         XmToggleButtonGadgetSetState(animflyql,FALSE,FALSE);
         animflyqlflag = 0;
        }
     }

   if (arg == 4)
     {

      if (curr_snapflag) 
        {
         fileselflag = ANIMFLYSNAP;
         string = XmStringCreate("Animation Snapshot File Prefix",
                                 XmSTRING_DEFAULT_CHARSET);
         string2 = XmStringCreate(fspattern[ANIMFLYSNAP],
                                  XmSTRING_DEFAULT_CHARSET);
         string3 = XmStringCreate(fsdir[ANIMFLYSNAP],
                                  XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                                 XmNpattern, string2, 
                                 XmNdirectory, string3, NULL);
         XmStringFree(string);
         XmStringFree(string2);
         XmStringFree(string3);

         if (fsread[ANIMFLYSNAP])
           {
            string = XmStringCreate(animflyfilnam,
                                 XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
            XmStringFree(string);
           }

         MyManageChild(file_sel2);
        }
      else
         animflystrt();
     }
}


void preadanimfly(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int arg;

   arg = (long int)client_data;
   tmpstr = XmTextFieldGetString(w);

   switch (arg)
     {
      case(0): nflyrots = atoi(tmpstr);
     }

   XtFree(tmpstr);
}


void rdanimflyhdr(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;

   tmpstr = XmTextFieldGetString(w);
   strcpy(animflyfilnam,tmpstr);

   XtFree(tmpstr);
}


void animflysetpt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20], *tmpstr;
  XmString string;

   nflylist++;
   xflyposlist[nflylist] = flypos[0];
   yflyposlist[nflylist] = flypos[1];
   zflyposlist[nflylist] = flypos[2];
   xflyrotlist[nflylist] = xrot;
   yflyrotlist[nflylist] = yrot;
   zflyrotlist[nflylist] = zrot;
   tmpstr = XmTextFieldGetString(animflynrots);
   nrotslist[nflylist-1] = atoi(tmpstr);
   XtFree(tmpstr);

   /*  Add count to list.  */   
   sprintf(str,"%4d",nrotslist[nflylist-1]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(flylistframe[nflylist-1],
                 XmNlabelString,string,NULL);
   XmStringFree(string);
}


void animflydelpt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  XmString string;

   nflylist--;
   if (nflylist < 0) nflylist = 0;
   flypos[0] = xflyposlist[nflylist];
   flypos[1] = yflyposlist[nflylist];
   flypos[2] = zflyposlist[nflylist];
   xrot = xflyrotlist[nflylist];
   yrot = yflyrotlist[nflylist];
   zrot = zflyrotlist[nflylist];

   /*  Delete count from list.  */
   sprintf(str,"    ");
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(flylistframe[nflylist],
                 XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Set angle sliders for current angles.  */
   setangles();
   
   fastdraw = 0;
   drawit();
}

void animflyretpt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   flypos[0] = xflyposlist[nflylist];
   flypos[1] = yflyposlist[nflylist];
   flypos[2] = zflyposlist[nflylist];
   xrot = xflyrotlist[nflylist];
   yrot = yflyrotlist[nflylist];
   zrot = zflyrotlist[nflylist];

   /*  Set angle sliders for current angles.  */
   setangles();

   fastdraw = 0;
   drawit();
}


void animflysavget(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string, string2, string3;

   arg = (long int)client_data;
   if (arg == 0)
     {
      fileselflag = ANIMFLYSAV;
      string = XmStringCreate("Save Flight Control Points",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[ANIMFLYSAV],
                               XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[ANIMFLYSAV],
                               XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2, 
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[ANIMFLYSAV])
        {
         string = XmStringCreate(flightfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }
   if (arg == 1)
     {
      fileselflag = ANIMFLYGET;
      string = XmStringCreate("Get Flight Control Points",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[ANIMFLYGET],
                               XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[ANIMFLYGET],
                               XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2,
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[ANIMFLYGET])
        {
         string = XmStringCreate(flightfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }
}


void savflightfile(char *fname) 
{
  FILE *out;
  char str[] = 
    "Error - unable to open output file!\nCheck permissions.";
  XmString string;

   strcpy(flightfilnam,fname);

   /*  Open file and save flight path.  */
   if ((out = fopen(flightfilnam,"w")) == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      return;
     }
   fwrite("gmv-flight",sizeof(char),10,out);
   fwrite(&nflylist,sizeof(int),1,out);
   fwrite(nrotslist,sizeof(int),nflylist,out);
   fwrite(xflyposlist,sizeof(float),nflylist,out);
   fwrite(yflyposlist,sizeof(float),nflylist,out);
   fwrite(zflyposlist,sizeof(float),nflylist,out);
   fwrite(xflyrotlist,sizeof(float),nflylist,out);
   fwrite(yflyrotlist,sizeof(float),nflylist,out);
   fwrite(zflyrotlist,sizeof(float),nflylist,out);

   fclose(out);
}


void getflightfile(char *fname) 
{
  char magic[11];
  FILE *fin;
  int i;
  XmString string, string1, string2;
  char str[] = 
   "Error - this is not a GMV flight control file.\nTry another file.";
  char str2[] = 
   "Error - cannot open file: ";

   strcpy(flightfilnam,fname);

   /*  Read header to make sure this is a GMV flight file.  */
   fin = fopen(flightfilnam,"r");
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
   fread(magic,sizeof(char),10,fin);
   if (strncmp(magic,"gmv-flight",10) != 0)
      {
       fclose(fin);
       string = XmStringLtoRCreate(str,
                                   XmSTRING_DEFAULT_CHARSET);
       XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
       XmStringFree(string);
       MyManageChild(errorform);
       return;
      }

   /*  Read flight control data.  */
   fsread[ANIMFLYGET] = 1;
   fread(&nflylist,sizeof(int),1,fin);
   fread(nrotslist,sizeof(int),nflylist,fin);
   fread(xflyposlist,sizeof(float),nflylist,fin);
   fread(yflyposlist,sizeof(float),nflylist,fin);
   fread(zflyposlist,sizeof(float),nflylist,fin);
   fread(xflyrotlist,sizeof(float),nflylist,fin);
   fread(yflyrotlist,sizeof(float),nflylist,fin);
   fread(zflyrotlist,sizeof(float),nflylist,fin);
   fclose(fin);

   /*  Add count to list.  */   
   for (i = 0; i < nflylist; i++)
     {
      sprintf(str,"%4d",nrotslist[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(flylistframe[i],
                    XmNlabelString,string,NULL);
      XmStringFree(string);
     }
}


void makeanimflyform(parent)
 Widget parent;
{
  Widget animflyclose,flynumtxt1, animflysetcp, animflydelcp, animflyretcp,
         cptxt, frcptxt, cpnoswin, cpnoform, cpnosep, cpnos[MFLY-1],
         lastwidget, animflygetcp, animflysavcp;
  char str[20], str2[20];
  int i;
  Arg wargs[20];
  XmString string;

   string = XmStringCreate("Flight Animation",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animflyform = XmCreateFormDialog(parent,"Flight-Animation",wargs,i);
      XmStringFree(string);   

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   animflyclose = XtVaCreateManagedWidget("Animflyclose",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflyclose,XmNactivateCallback,animflystuff,
                    (caddr_t) 0);

   string = XmStringCreate("Quick Look",XmSTRING_DEFAULT_CHARSET);
   animflyql = XtVaCreateManagedWidget("Animflyql",
               xmToggleButtonGadgetClass,
               animflyform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animflyclose,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(animflyql,XmNvalueChangedCallback,animflystuff,
                    (caddr_t) 1);
 
   string = XmStringCreate("Snapshot",XmSTRING_DEFAULT_CHARSET);
   animflysnap = XtVaCreateManagedWidget("Animflysnap",
                 xmToggleButtonGadgetClass,
                 animflyform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animflyclose,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animflyql,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNselectColor, yellowpix,
                 NULL);
      XmStringFree(string);
      XtAddCallback(animflysnap,XmNvalueChangedCallback,animflystuff,
                    (caddr_t) 2);
   
   string = XmStringCreate("Start",XmSTRING_DEFAULT_CHARSET);
   animflystart = XtVaCreateManagedWidget("Animflystart",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflyql,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflystart,XmNactivateCallback,
                    animflystuff,(caddr_t) 4);
   
   string = XmStringCreate("Stop",XmSTRING_DEFAULT_CHARSET);
   animflystop = XtVaCreateManagedWidget("Animflystop",
                 xmPushButtonWidgetClass,
                 animflyform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animflyql,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animflystart,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNsensitive, FALSE,
                 NULL);
      XmStringFree(string);
      XtAddCallback(animflystop,XmNactivateCallback,
                    animflystuff, (caddr_t) 6);
 
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   flynrotstxt = XtVaCreateManagedWidget("Flynrotstxt",
                 xmLabelWidgetClass,
                 animflyform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animflyql,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animflystop,
                 XmNleftOffset, 10,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   animflynrots = XtVaCreateManagedWidget("Animflynrots",
                  xmTextFieldWidgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflystart,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 4,
                  XmNcolumns, 4,
                  XmNvalue, "30",
                  NULL);
      XtAddCallback(animflynrots,XmNactivateCallback,
                    preadanimfly,(caddr_t) 0);
   string = XmStringCreate("Control Point Frames",
                           XmSTRING_DEFAULT_CHARSET);
   flynumtxt1 = XtVaCreateManagedWidget("Flynumtxt1",
                xmLabelGadgetClass,
                animflyform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animflystart,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, animflynrots,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("Set New Control Point",
                           XmSTRING_DEFAULT_CHARSET);
   animflysetcp = XtVaCreateManagedWidget("Animflysetcp",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflynrots,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflysetcp,XmNactivateCallback,
                    animflysetpt,NULL);

   string = XmStringCreate("Delete Last Control Point",
                           XmSTRING_DEFAULT_CHARSET);
   animflydelcp = XtVaCreateManagedWidget("Animflydelcp",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflysetcp,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflydelcp,XmNactivateCallback,
                    animflydelpt,NULL);

   string = XmStringCreate("Return to Control Point",
                           XmSTRING_DEFAULT_CHARSET);
   animflyretcp = XtVaCreateManagedWidget("Animflyretcp",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflydelcp,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflyretcp,XmNactivateCallback,
                    animflyretpt,NULL);

   string = XmStringLtoRCreate("Control\nPoints",
                               XmSTRING_DEFAULT_CHARSET);
   cptxt = XtVaCreateManagedWidget("Cptxt",
           xmLabelWidgetClass,
           animflyform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, animflyretcp,
           XmNtopOffset, 10,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           NULL);
      XmStringFree(string);

   string = XmStringLtoRCreate("Frames per\nControl Point",
                               XmSTRING_DEFAULT_CHARSET);
   frcptxt = XtVaCreateManagedWidget("Frcptxt",
             xmLabelWidgetClass,
             animflyform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animflyretcp,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, cptxt,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   /*  Create control nos. scrolled window.  */
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cptxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 240);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   cpnoswin = XmCreateScrolledWindow(animflyform,"Cpnoswin",wargs,i);
   XtManageChild(cpnoswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cpnoform = XmCreateForm(cpnoswin,"Cpnoform",wargs,i);
   XtManageChild(cpnoform);

   cpnosep = XtVaCreateManagedWidget("Cpnosep",
             xmSeparatorGadgetClass,
             cpnoform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNseparatorType, XmNO_LINE,
             NULL);

   lastwidget = cpnosep;
   for (i = 0; i < MFLY-1; i++)
     {
      sprintf(str,"%4d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str2,"Cpnos%d",i);
      cpnos[i] = XtVaCreateManagedWidget(str2,
                 xmLabelWidgetClass,
                 cpnoform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, lastwidget,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
         XmStringFree(string);
      
      string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
      sprintf(str2,"Flylistframe%d",i);
      flylistframe[i] = XtVaCreateManagedWidget(str2,
                        xmLabelWidgetClass,
                        cpnoform,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopWidget, lastwidget,
                        XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                        XmNleftWidget, cpnos[i],
                        XmNleftOffset, 100,
                        XmNrightAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_NONE,
                        XmNlabelString, string,
                        NULL);
         XmStringFree(string);

      lastwidget = cpnos[i];
     }

   /*  Create save and get control point buttons.  */

   string = XmStringCreate("Save Control Points",
                           XmSTRING_DEFAULT_CHARSET);
   animflysavcp = XtVaCreateManagedWidget("Animflysavcp",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cpnoswin,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflysavcp,XmNactivateCallback,
                    animflysavget,(caddr_t) 0);

   string = XmStringCreate("Get Control Points",
                           XmSTRING_DEFAULT_CHARSET);
   animflygetcp = XtVaCreateManagedWidget("Animflgetcp",
                  xmPushButtonGadgetClass,
                  animflyform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animflysavcp,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animflygetcp,XmNactivateCallback,
                    animflysavget,(caddr_t) 1);
}


void animflystrt()
{
  int j, nframes;

   /*  Start animation sequences.  */
   animflystopwin = XtWindow(animflystop);
   
   if (nflylist > 0)
     {
      animfly_count = 0;
      animflystopflag = 0;
      animfly_snapflag = curr_snapflag;

      /*  Loop over the number of control points.  */
      for (j = 0; j <  nflylist; j++)
        {
         nframes = nrotslist[j];
         xflyposbeg = xflyposlist[j];
         yflyposbeg = yflyposlist[j];
         zflyposbeg = zflyposlist[j];
         xflyposend = xflyposlist[j+1];
         yflyposend = yflyposlist[j+1];
         zflyposend = zflyposlist[j+1];
         xflyrotbeg = xflyrotlist[j];
         yflyrotbeg = yflyrotlist[j];
         zflyrotbeg = zflyrotlist[j];
         xflyrotend = xflyrotlist[j+1];
         yflyrotend = yflyrotlist[j+1];
         zflyrotend = zflyrotlist[j+1];

         /*  Set rotation and translation increments.  */
         xflyposinc = (xflyposend-xflyposbeg)/(float)nframes;
         yflyposinc = (yflyposend-yflyposbeg)/(float)nframes;
         zflyposinc = (zflyposend-zflyposbeg)/(float)nframes;
         xflyrotinc = (xflyrotend-xflyrotbeg)/(float)nframes;
         yflyrotinc = (yflyrotend-yflyrotbeg)/(float)nframes;
         zflyrotinc = (zflyrotend-zflyrotbeg)/(float)nframes;
            
         /*  Loop over the number of frames and draw the images.  */
         XtSetSensitive(animflystart,FALSE);
         XtSetSensitive(animflystop,TRUE);

         for (iflyrot = 0; iflyrot < nframes; iflyrot++)
           {
            animfly_count++;
            animflydraw();
            if (animflystopflag == 1) break;
           }
         if (animflystopflag == 1) break;
        }
     }

   animflyreset();
}


void animflydraw()
{
  int irotno;
  char str[20];
  XmString string;
  XEvent stopevent;


   /*  Check for events while animation is running.  */
   if ((int)XtAppPending(app) != 0)
     {
      if (XCheckWindowEvent(dpy,animflystopwin,ButtonReleaseMask,
          &stopevent) != 0)
         animflystopflag = 1;
     }

   if (animflystopflag == 1) return;

   /*  Set new eye position and rotation.  */
   irotno = iflyrot + 1;
   flypos[0] = xflyposbeg + irotno*xflyposinc;
   flypos[1] = yflyposbeg + irotno*yflyposinc;
   flypos[2] = zflyposbeg + irotno*zflyposinc;
   xrot = xflyrotbeg + irotno*xflyrotinc;
   yrot = yflyrotbeg + irotno*yflyrotinc;
   zrot = zflyrotbeg + irotno*zflyrotinc;
                   
   fastdraw = 0;
   if (animflyqlflag == 1) fastdraw = 1;
   drawit();

   /*  Set frame counter.  */
   sprintf(str,"%4d",animfly_count);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(flynrotstxt,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(flynrotstxt);
   XFlush(dpy);
}


void animflyreset()
{
  int i;
  XmString string;

   /*  Reset button sensitivity.  */
   XtSetSensitive(animflystart,TRUE);
   XtSetSensitive(animflystop,FALSE);

   /*  Clear counter.  */
   animfly_count = 0;
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(flynrotstxt,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Clear control points, if snapshots completed.  */
   if (animfly_snapflag == 1 && animflystopflag == 0)
     {

      /*  Clear list.  */
      string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
      for (i = 0; i < nflylist; i++)
         XtVaSetValues(flylistframe[i],
                       XmNlabelString,string,NULL);
      XmStringFree(string);

      nflylist = 0;
      xflyposlist[0] = flypos[0];
      yflyposlist[0] = flypos[1];
      zflyposlist[0] = flypos[2];
      xflyrotlist[0] = xrot;
      yflyrotlist[0] = yrot;
      zflyrotlist[0] = zrot;
     }

   /*  Reset eye position and rotations to last control  */
   /*  point if stopped, or snapshots were not made.     */
   if (animflystopflag == 1 || animfly_snapflag == 0)
     {
      flypos[0] = xflyposlist[nflylist];
      flypos[1] = yflyposlist[nflylist];
      flypos[2] = zflyposlist[nflylist];
      xrot = xflyrotlist[nflylist];
      yrot = yflyrotlist[nflylist];
      zrot = zflyrotlist[nflylist];
     }

   animfly_snapflag = 0;
   curr_snapflag = 0;
   XmToggleButtonGadgetSetState(animflysnap,FALSE,FALSE);

   /*  Set angle sliders for current angles.  */
   setangles();
                   
   fastdraw = 0;
   drawit();
}


void setanimflyform()
{

   /*  If unfinished animations exist, set  */
   /*  eye position to last control point.  */
   if (nflylist > 0)
     {
      flypos[0] = xflyposlist[nflylist];
      flypos[1] = yflyposlist[nflylist];
      flypos[2] = zflyposlist[nflylist];
      xrot = xflyrotlist[nflylist];
      yrot = yflyrotlist[nflylist];
      zrot = zflyrotlist[nflylist];
     }

   /*  If no animations exist, set first  */
   /*  control point to eye position.     */
   if (nflylist == 0)
     {
      xflyposlist[0] = flypos[0];
      yflyposlist[0] = flypos[1];
      zflyposlist[0] = flypos[2];
      xflyrotlist[0] = xrot;
      yflyrotlist[0] = yrot;
      zflyrotlist[0] = zrot;
     }

   curr_snapflag = animfly_snapflag;

   /*  Set angle sliders for current angles.  */
   setangles();

   animflymenu_up = 1;
   MyManageChild(animflyform);
}
