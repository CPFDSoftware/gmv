#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/Scale.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>
 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "gmvdata.h"
#include "lights.h"
#include "xstuff.h"
#include "formwidgets.h"
#include "contour.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32
#define MYPI 3.14159265358979323846

extern XtAppContext app;

static Widget animfadeform, animcpform, animexpform, animisoform,
              animcsform, animsubform; 
static Widget qlobj, ssobj, objnrotsanim, objxrotbeg, objxrotend,
              objyrotbeg, objyrotend, objzrotbeg, objzrotend,
              objxcenbeg, objxcenend, objycenbeg, objycenend,
              objzcenbeg, objzcenend, objmagbeg, objmagend, objanimdoit,
              objanimfade, transbeg, transend, objnrotstxt,
              matfadebut[MAXMATS], contfadebut[MAXCONTOURS],
              matfadetext[MAXMATS], contfadetext[MAXCONTOURS],
              animcpbut, animcpbeg, animcpend, animstop, animexpbut,
              animexpbeg, animexpend, animpexptext, animcexptext,
              animisobut, animisobeg, animisoend, animisominlab,
              animisomaxlab, animpause, animprev, animnext,
              animresume, animpausego, animpauseno, animclose,
              animisoval, animcploc, animisofldlab, animcpnfldbut, 
              animcpnfldlab, animcpcfldbut, animcpcfldlab, 
              animcpbegtxt, animcpendtxt, animcsbut, animcsx, animcsy,
              animcsz, animcsrbeg, animcsrend, animcsnfldbut, 
              animcsnfldlab, animcscfldbut, animcscfldlab, animcsloc,
              animsubbut, animsubxmin1, animsubxmin2, animsubxmax1,
              animsubxmax2, animsubymin1, animsubymin2, animsubymax1,
              animsubymax2, animsubzmin1, animsubzmin2, animsubzmax1,
              animsubzmax2, animcpnmatbut, animcpcmatbut, animcsnmatbut, 
              animcscmatbut;
static Window animstopwin, animpausewin;
static float xrotbeg = 0.0, yrotbeg = 0.0, zrotbeg = 0.0,
             xrotend = 360.0, yrotend = 0.0, zrotend = 0.0,
             xcenbeg = 0.0, ycenbeg = 0.0, zcenbeg = 0.0,
             xcenend = 0.0, ycenend = 0.0, zcenend = 0.0,
             magbeg = 1.0, magend=1.0,
             pexpbeg = 0.0, pexpend=0.0, animpexpinc,
             cexpbeg = 0.0, cexpend=0.0, animcexpinc,
             cpxmin, cpxmax, cpymin, cpymax, cpzmin, cpzmax, cpmin, cpmax,
             animaa, animbb, animcc, animdd, animddbeg, animddend,
             cpx1, cpy1, cpz1, cpx2, cpy2, cpz2, cpx3, cpy3, cpz3,
             ddinc, old_pexplodefact, old_cexplodefact,
             isobeg, isoend, isoinc,
             fadematbeg[MAXMATS],fadematend[MAXMATS],
             fadecontbeg[MAXCONTOURS],fadecontend[MAXCONTOURS],
             transbegval, transendval, csxcen, csycen, cszcen, 
             csrbeg, csrend, csrinc, subxminbeg, subxminend,
             subxmaxbeg, subxmaxend, subyminbeg, subyminend,
             subymaxbeg, subymaxend, subzminbeg, subzminend,
             subzmaxbeg, subzmaxend, oldxminfdval, oldxmaxfdval,
             oldyminfdval, oldymaxfdval, oldzminfdval, oldzmaxfdval,
             oldxminbdval, oldxmaxbdval, oldyminbdval, oldymaxbdval,
             oldzminbdval, oldzmaxbdval, subxminbeg, subxminend, 
             subxmaxbeg, subxmaxend, subyminbeg, subyminend, subymaxbeg,
             subymaxend, subzminbeg, subzminend, subzmaxbeg, subzmaxend,
             subxmininc, subxmaxinc, subymininc, subymaxinc, subzmininc,
             subzmaxinc;
float animfademat[MAXMATS],animfadematinc[MAXMATS],
      animfadecont[MAXCONTOURS],animfadecontinc[MAXCONTOURS];
float transmatold[MAXMATS],transcontold[MAXCONTOURS];
static float xanimrotinc, yanimrotinc, zanimrotinc,
             xanimceninc, yanimceninc, zanimceninc, animmaginc;
static int nrots = 30, irots, prots;
static double *value, *isofield, *csvalue;
static short animcpflag = 0, animcpfieldactive = 0, animstopflag = 0,
             animpexpflag = 0, animcexpflag = 0, animcellexpflag = -1,
             quicklookflag = 0, animisoflag = 0, animisofieldactive = 0,
             animpauseflag = 0, animpausequit = 0, animcpdir = 0,
             animcsflag = 0, animcsfieldactive, animsubflag = 0;
static int old_cutplanefieldforplane, old_cellexpflag, 
           old_cutspherefieldforsphere;
static int curracpnfld, curracpcfld, curracsnfld, curracscfld;
void anim_play(XtPointer client_data,XtIntervalId *id),
     animset(), animdraw(), animreset(), animpausectl(),
     animcp_set_dirval(), animstart();


void animationstuff(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, arg, ival;
  XmString string, string2, string3;
  int fadeflag;
  void animfadeclose(Widget,XtPointer,XtPointer);
  void animcpclose(Widget,XtPointer,XtPointer);
  void animexpclose(Widget,XtPointer,XtPointer);
  void animisoclose(Widget,XtPointer,XtPointer);
  void animcsclose(Widget,XtPointer,XtPointer);
  void animsubclose(Widget,XtPointer,XtPointer);

   arg = (long int)client_data;

   if (arg == 0)
     {
      animfadeclose(animfadeform,NULL,NULL);
      if (ncells > 0 && numvars > 0)
        {
         animcpclose(animcpform,NULL,NULL);
         animisoclose(animisoform,NULL,NULL);
         animcsclose(animcsform,NULL,NULL);
        }
      if (polysin == 1 || ncells > 0)
         animexpclose(animexpform,NULL,NULL);
      animsubclose(animsubform,NULL,NULL);
      animationmenu_up = 0;
      XtUnmanageChild(animationform);
     }

   if (arg == 1)
     {
      quicklookflag = (int)XmToggleButtonGadgetGetState(w);
      if (quicklookflag == 1)
        {
         XmToggleButtonGadgetSetState(ssobj,FALSE,FALSE);
         anim_snapshotsflag = 0;
        }
     }

   if (arg == 2)
     {
      anim_snapshotsflag = (int)XmToggleButtonGadgetGetState(w);
      if (anim_snapshotsflag == 1)
        {
         XmToggleButtonGadgetSetState(qlobj,FALSE,FALSE);
         quicklookflag = 0;
        }
     }

   if (arg == 3)
     {
      fadeflag = 0;
      for (i = 0; i < MAXMATS; i++)
        if (animfadematflag[i] ==1) fadeflag = 1;
      for (i = 0; i < MAXCONTOURS; i++)
        if (animfadecontflag[i] ==1) fadeflag = 1;
      if (fadeflag == 0)
         XmToggleButtonGadgetSetState(objanimfade,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(objanimfade,TRUE,FALSE);

      XmScaleGetValue(transbeg,&ival);
      transbegval = (float)ival / 100.;
      XmScaleGetValue(transend,&ival);
      transendval = (float)ival / 100.;

      MyManageChild(animfadeform);
     }

   if (arg == 4)
     {

      if (anim_snapshotsflag)
        {
         fileselflag = ANIMSNAP;
         string = XmStringCreate("Animation Snapshot File Prefix",
                                 XmSTRING_DEFAULT_CHARSET);
         string2 = XmStringCreate(fspattern[ANIMSNAP],
                                  XmSTRING_DEFAULT_CHARSET);
         string3 = XmStringCreate(fsdir[ANIMSNAP],
                                  XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                                 XmNpattern, string2,
                                 XmNdirectory, string3, NULL);
         XmStringFree(string);
         XmStringFree(string2);
         XmStringFree(string3);

         if (fsread[ANIMSNAP])
           {
            string = XmStringCreate(animfilnam,
                                 XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
           XmStringFree(string);
           }

         MyManageChild(file_sel2);
        }
      else
         animstart();
     }

   if (arg == 5)
     {
      if (animcpflag == 0) 
         XmToggleButtonGadgetSetState(animcpbut,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(animcpbut,TRUE,FALSE);
      XmUpdateDisplay(animcpbut);

      XmToggleButtonGadgetSetState(animcpnfldbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcpcfldbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcpnmatbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcpcmatbut,FALSE,FALSE);
      if (animcpfieldactive == -1)
        {
         XmToggleButtonGadgetSetState(animcpnmatbut,TRUE,FALSE);
        }
      else if (animcpfieldactive == -2)
        {
         XmToggleButtonGadgetSetState(animcpcmatbut,TRUE,FALSE);
        }
      else if (animcpfieldactive < numvars)
        {
         curracpnfld = animcpfieldactive;
         XmToggleButtonGadgetSetState(animcpnfldbut,TRUE,FALSE);
        }
      else
        {
         curracpcfld = animcpfieldactive - numvars;
         XmToggleButtonGadgetSetState(animcpcfldbut,TRUE,FALSE);
        }

      string = XmStringCreate(fieldname[curracpnfld],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcpnfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (cnumvars > 0)
        {
         string = XmStringCreate(cfieldname[curracpcfld],
                                 XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(animcpcfldlab,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      animcp_set_dirval();
      currcp = MAXCUTPLANES;
      MyManageChild(animcpform);
     }

   if (arg == 6)
     {
      animstopflag = 1;
      animpausequit = 1;
     }

   if (arg == 7)
     {
      if (animpexpflag == 0 && animcexpflag == 0) 
         XmToggleButtonGadgetSetState(animexpbut,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(animexpbut,TRUE,FALSE);
      XmUpdateDisplay(animexpbut);
  
      MyManageChild(animexpform);
     }

   if (arg == 8)
     {
      if (animisoflag == 0) 
         XmToggleButtonGadgetSetState(animisobut,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(animisobut,TRUE,FALSE);
      XmUpdateDisplay(animisobut);

      string = XmStringCreate(fieldname[animisofieldactive],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animisofldlab,XmNlabelString,string,NULL);
      XmStringFree(string);
  
      MyManageChild(animisoform);
     }

   if (arg == 9)
     {
      animpauseflag = 1;
     }

   if (arg == 10)
     {
      if (animcsflag == 0) 
         XmToggleButtonGadgetSetState(animcsbut,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(animcsbut,TRUE,FALSE);
      XmUpdateDisplay(animcsbut);

      XmToggleButtonGadgetSetState(animcsnfldbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcscfldbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcsnmatbut,FALSE,FALSE);
      XmToggleButtonGadgetSetState(animcscmatbut,FALSE,FALSE);
      if (animcsfieldactive == -1)
        {
         XmToggleButtonGadgetSetState(animcsnmatbut,TRUE,FALSE);
        }
      else if (animcsfieldactive == -2)
        {
         XmToggleButtonGadgetSetState(animcscmatbut,TRUE,FALSE);
        }
      else if (animcsfieldactive < numvars)
        {
         curracpnfld = animcsfieldactive;
         XmToggleButtonGadgetSetState(animcsnfldbut,TRUE,FALSE);
        }
      else
        {
         curracpcfld = animcsfieldactive - numvars;
         XmToggleButtonGadgetSetState(animcscfldbut,TRUE,FALSE);
        }

      string = XmStringCreate(fieldname[curracsnfld],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcsnfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (cnumvars > 0)
        {
         string = XmStringCreate(cfieldname[curracscfld],
                                 XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(animcscfldlab,XmNlabelString,string,NULL);
         XmStringFree(string);
        }

      currcp = MAXCUTPLANES+MAXCUTSPHERES+1;
      MyManageChild(animcsform);
     }

   if (arg == 11)
     {
      if (animsubflag == 0) 
         XmToggleButtonGadgetSetState(animsubbut,FALSE,FALSE);
      else
         XmToggleButtonGadgetSetState(animsubbut,TRUE,FALSE);
      XmUpdateDisplay(animsubbut);
  
      MyManageChild(animsubform);
     }
}


void preadanim(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int arg;

   arg = (long int)client_data;
   tmpstr = XmTextFieldGetString(w);

   switch (arg)
     {
      case( 1): xrotbeg = atof(tmpstr);
                break;
      case( 2): xrotend = atof(tmpstr);
                break;
      case( 3): yrotbeg = atof(tmpstr);
                break;
      case( 4): yrotend = atof(tmpstr);
                break;
      case( 5): zrotbeg = atof(tmpstr);
                break;
      case( 6): zrotend = atof(tmpstr);
                break;
      case( 7): xcenbeg = -atof(tmpstr);
                break;
      case( 8): xcenend = -atof(tmpstr);
                break;
      case( 9): ycenbeg = -atof(tmpstr);
                break;
      case(10): ycenend = -atof(tmpstr);
                break;
      case(11): zcenbeg = -atof(tmpstr);
                break;
      case(12): zcenend = -atof(tmpstr);
                break;
      case(13): magbeg = atof(tmpstr);
                break;
      case(14): magend = atof(tmpstr);
                break;
      case(15): nrots = atoi(tmpstr);
     }

   XtFree(tmpstr);

}


void anim_dotrans(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival;

   XmScaleGetValue(transbeg,&ival);
   transbegval = (float)ival / 100.;
   XmScaleGetValue(transend,&ival);
   transendval = (float)ival / 100.;
}


void animfade_set_mat(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str1[20], str2[20];
  XmString string;
      
   i = (long int)client_data;
   sprintf(str1,"           ");
   animfadematflag[i] = 0;
   if (XmToggleButtonGetState(w) == 1)
     {
      animfadematflag[i] = 1;
      fadematbeg[i] = transbegval;
      fadematend[i] = transendval;
      sprintf(str1,"%4.2f",transbegval);
      sprintf(str2,"   %4.2f",transendval);
      strcat(str1,str2);
     }
   string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(matfadetext[i],XmNlabelString,string,NULL);
   XmStringFree(string);
}

void animfade_set_cont(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str1[20], str2[20];     
  XmString string;
      
   i = (long int)client_data;
   sprintf(str1,"           ");
   animfadecontflag[i] = 0;
   if (XmToggleButtonGetState(w) == 1)
     {
      animfadecontflag[i] = 1;
      fadecontbeg[i] = transbegval;
      fadecontend[i] = transendval;
      sprintf(str1,"%4.2f",transbegval);
      sprintf(str2,"   %4.2f",transendval);
      strcat(str1,str2);
     }
   string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(contfadetext[i],XmNlabelString,string,NULL);
   XmStringFree(string);
}


void animfadeclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, fadeflag;

   fadeflag = 0;
   for (i = 0; i < MAXMATS; i++)
     if (animfadematflag[i] == 1) fadeflag = 1;
   for (i = 0; i < MAXCONTOURS; i++)
     if (animfadecontflag[i] == 1) fadeflag = 1;
   if (fadeflag == 0)
      XmToggleButtonGadgetSetState(objanimfade,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(objanimfade,TRUE,FALSE);

   XtUnmanageChild(animfadeform);
}


void animvelflowset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{     
   animvelflowflag = (short)XmToggleButtonGetState(w);
}


void animcpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (animcpflag == 0) 
      XmToggleButtonGadgetSetState(animcpbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animcpbut,TRUE,FALSE);
   XmUpdateDisplay(animcpbut);
  
   XtUnmanageChild(nfieldform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(animcpform);
}


void animcpdisp(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cutplaneoptformpop();
}


void animcp_set_dirval()
{
  int imin, imax;
  char str1[20], str2[20];

   if (animcpdir == 1)
     {
      animaa = 1.;  animbb = 0.;  animcc = 0.;
      animddbeg = cpxmin;  animddend = cpxmax;
      cpmin = cpxmin;  cpmax = cpxmax;
     }

   if (animcpdir == 2)
     {
      animaa = 0.;  animbb = 1.;  animcc = 0.;
      animddbeg = cpymin;  animddend = cpymax;
      cpmin = cpymin;  cpmax = cpymax;
     }

   if (animcpdir == 3)
     {
      animaa = 0.;  animbb = 0.;  animcc = 1.;
      animddbeg = cpzmin;  animddend = cpzmax;
      cpmin = cpzmin;  cpmax = cpzmax;
     }

   if (animcpdir == 4)
     {
      animaa = 0.;  animbb = 0.;  animcc = 0.;
      cpx1 = 0.;  cpy1 = 0.;  cpz1 = 0.;
      cpx2 = 1.;  cpy2 = 0.;  cpz2 = 0.;
      cpx3 = 0.;  cpy3 = 1.;  cpz3 = 0.;
      animddbeg = 0.;  animddend = 360.;
      cpmin = 0.;  cpmax = 360.;
     }

   if (animcpdir == 5)
     {
      animaa = 0.;  animbb = 0.;  animcc = 0.;
      cpx1 = 0.;  cpy1 = 0.;  cpz1 = 0.;
      cpx2 = 0.;  cpy2 = 1.;  cpz2 = 0.;
      cpx3 = 1.;  cpy3 = 0.;  cpz3 = 0.;
      animddbeg = 0.;  animddend = 360.;
      cpmin = 0.;  cpmax = 360.;
     }

   if (animcpdir == 6)
     {
      animaa = 0.;  animbb = 0.;  animcc = 0.;
      cpx1 = 0.;  cpy1 = 0.;  cpz1 = 0.;
      cpx2 = 0.;  cpy2 = 0.;  cpz2 = 1.;
      cpx3 = 1.;  cpy3 = 0.;  cpz3 = 0.;
      animddbeg = 0.;  animddend = 360.;
      cpmin = 0.;  cpmax = 360.;
     }

   if (animcpdir > 0)
     {
      imin = (int) (animddbeg * 1000.);
      imax = (int) (animddend * 1000.);
      XtVaSetValues(animcpbeg, XmNminimum,imin, XmNmaximum,imax,
                               XmNvalue,imin, NULL);
      XtVaSetValues(animcpend, XmNminimum,imin, XmNmaximum,imax,
                               XmNvalue,imax, NULL);
      sprintf(str1,"%-11.7g",animddbeg);
      sprintf(str2,"%11.7g",animddend);
     }
   else
     {
      strcpy(str1," ");
      strcpy(str2," ");
     }
   XtVaSetValues(animcpbegtxt,XmNvalue,str1,NULL);
   XtVaSetValues(animcpendtxt,XmNvalue,str2,NULL);
   XmUpdateDisplay(animcpbeg); 
   XmUpdateDisplay(animcpend); 
   XmUpdateDisplay(animcpbegtxt); 
   XmUpdateDisplay(animcpendtxt); 
}


void animcp_select_dir(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, flag;

   arg = (long int)client_data;
   flag = (int)XmToggleButtonGadgetGetState(w);

   if (arg > 0 && flag == 1) animcpflag = 1;

   if (arg == 0 && flag == 1) animcpflag = 0;

   animcpdir = arg;

   animcp_set_dirval();

   if (animcpflag == 0) 
      XmToggleButtonGadgetSetState(animcpbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animcpbut,TRUE,FALSE);
   XmUpdateDisplay(animcpbut); 
}


void anim_cpsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];
 
   arg = (long int)client_data;
 
   XmScaleGetValue(w,&ival);
   val = (float) ival / 1000.;
 
   if (arg == 0)
     {
      animddbeg = val;
      sprintf(str,"%-11.7g",animddbeg);
      XtVaSetValues(animcpbegtxt,XmNvalue,str,NULL);
      XmUpdateDisplay(animcpbegtxt); 
     }

   if (arg == 1)
     {
      animddend = val;
      sprintf(str,"%11.7g",animddend);
      XtVaSetValues(animcpendtxt,XmNvalue,str,NULL);
      XmUpdateDisplay(animcpendtxt); 
     }
}

void animcpread(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void animcpreread();
   animcpreread();
}

void animcpreread()
{
  int arg, ival;
  char *tmpstr, str[20];
  float val;

   /*  Re-read textfields.  */
   tmpstr = XmTextFieldGetString(animcpbegtxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < cpmin) val = cpmin;
   if (val > cpmax) val = cpmax;
   ival = (int) (val * 1000.);
   animddbeg = val;
   sprintf(str,"%#11.7g",animddbeg);
   XtVaSetValues(animcpbegtxt,XmNvalue,str,NULL);
   XtVaSetValues(animcpbeg, XmNvalue,ival, NULL);
   if (animddbeg > animddend)
     {
      XmScaleSetValue(animcpend,ival);
      XtVaSetValues(animcpendtxt,XmNvalue,str,NULL);
      animddend = animddbeg;
     }

   tmpstr = XmTextFieldGetString(animcpendtxt);
   val = atof(tmpstr);
   XtFree(tmpstr);
   if (val < cpmin) val = cpmin;
   if (val > cpmax) val = cpmax;
   ival = (int) (val * 1000.);
   animddend = val;
   sprintf(str,"%#11.7g",animddend);
   XtVaSetValues(animcpendtxt,XmNvalue,str,NULL);
   XtVaSetValues(animcpend, XmNvalue,ival, NULL);
   if (animddbeg > animddend)
     {
      XmScaleSetValue(animcpbeg,ival);
      XtVaSetValues(animcpbegtxt,XmNvalue,str,NULL);
      animddbeg = animddend;
     }
}


void animcp_fldsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(animcpnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcpcfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcpnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcpcmatbut,FALSE,FALSE);

   if (arg == 0)
     {
      animcpfieldactive = curracpnfld;
      XmToggleButtonGadgetSetState(animcpnfldbut,TRUE,FALSE);
     }

   if (arg == 1)
     {
      animcpfieldactive = curracpcfld + numvars;
      XmToggleButtonGadgetSetState(animcpcfldbut,TRUE,FALSE);
     }

   if (arg == 2)
     {
      animcpfieldactive = -1;
      XmToggleButtonGadgetSetState(animcpnmatbut,TRUE,FALSE);
     }

   if (arg == 3)
     {
      animcpfieldactive = -2;
      XmToggleButtonGadgetSetState(animcpcmatbut,TRUE,FALSE);
     }
}


void animcpnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = curracpnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDANIMCUTP, ifld);
}


void animcpcfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = curracpcfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDANIMCUTP, ifld);
}


void animcsclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (animcsflag == 0) 
      XmToggleButtonGadgetSetState(animcsbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animcsbut,TRUE,FALSE);
   XmUpdateDisplay(animcsbut);
  
   XtUnmanageChild(nfieldform);
   if (cnumvars > 0)
      XtUnmanageChild(cfieldform);
   XtUnmanageChild(cutplaneoptform);
   XtUnmanageChild(animcsform);
}


void animcs_on_off(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, flag;

   arg = (long int)client_data;
   flag = (int)XmToggleButtonGadgetGetState(w);

   if (arg == 0 && flag == 1) animcsflag = 1; 
   if (arg == 1 && flag == 1) animcsflag = 0; 

   if (animcsflag == 0) 
      XmToggleButtonGadgetSetState(animcsbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animcsbut,TRUE,FALSE);
   XmUpdateDisplay(animcsbut);
}


void animcs_fldsel(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   
   XmToggleButtonGadgetSetState(animcsnfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcscfldbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcsnmatbut,FALSE,FALSE);
   XmToggleButtonGadgetSetState(animcscmatbut,FALSE,FALSE);

   if (arg == 0)
     {
      animcsfieldactive = curracsnfld;
      XmToggleButtonGadgetSetState(animcsnfldbut,TRUE,FALSE);
     }

   if (arg == 1)
     {
      animcsfieldactive = curracscfld + numvars;
      XmToggleButtonGadgetSetState(animcscfldbut,TRUE,FALSE);
     }

   if (arg == 2)
     {
      animcsfieldactive = -1;
      XmToggleButtonGadgetSetState(animcsnmatbut,TRUE,FALSE);
     }

   if (arg == 3)
     {
      animcsfieldactive = -2;
      XmToggleButtonGadgetSetState(animcscmatbut,TRUE,FALSE);
     }
}


void animcsnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = curracsnfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDANIMCUTS, ifld);
}


void animcscfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = curracscfld;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDANIMCUTS, ifld);
}


void anim_exp_polys(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   animpexpflag = (short)XmToggleButtonGadgetGetState(w);
}


void anim_exp_cells(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   animcexpflag = (short)XmToggleButtonGadgetGetState(w);
}


void animexpapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int ival1, ival2;
  char str1[20], str2[20];     
  XmString string;

   sprintf(str1,"           ");
   if (animpexpflag == 1)
     {
      XmScaleGetValue(animexpbeg,&ival1);
      pexpbeg = (float) ival1 / 100.;
      XmScaleGetValue(animexpend,&ival2);
      pexpend = (float) ival2 / 100.;
      sprintf(str1," %d",ival1);
      sprintf(str2,"   %d",ival2);
      strcat(str1,str2);
     }
   string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animpexptext,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str1,"           ");
   if (animcexpflag == 1)
     {
      XmScaleGetValue(animexpbeg,&ival1);
      cexpbeg = (float) ival1 / 100.;
      XmScaleGetValue(animexpend,&ival2);
      cexpend = (float) ival2 / 100.;
      sprintf(str1," %d",ival1);
      sprintf(str2,"   %d",ival2);
      strcat(str1,str2);
     }
   string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animcexptext,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (animpexpflag == 0 && animcexpflag == 0) 
      XmToggleButtonGadgetSetState(animexpbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animexpbut,TRUE,FALSE);
   XmUpdateDisplay(animexpbut);
}


void anim_cell_expflag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   animcellexpflag = arg - 1;
}


void animexpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{

   if (animpexpflag == 0 && animcexpflag == 0) 
      XmToggleButtonGadgetSetState(animexpbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animexpbut,TRUE,FALSE);
   XmUpdateDisplay(animexpbut);
  
   XtUnmanageChild(animexpform);
}


void animiso_on_off(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, flag;

   arg = (long int)client_data;
   flag = (int)XmToggleButtonGadgetGetState(w);

   if (arg == 0 && flag == 1) animisoflag = 1; 
   if (arg == 1 && flag == 1) animisoflag = 0; 

   if (animisoflag == 0) 
      XmToggleButtonGadgetSetState(animisobut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animisobut,TRUE,FALSE);
   XmUpdateDisplay(animisobut);
}


void animiso_txt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int arg;

   arg = (long int)client_data;
   tmpstr = XmTextFieldGetString(w);

   if (arg == 1) isobeg = atof(tmpstr);
   if (arg == 2) isoend = atof(tmpstr);

   XtFree(tmpstr);

}


void animisoclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{

   if (animisoflag == 0) 
      XmToggleButtonGadgetSetState(animisobut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animisobut,TRUE,FALSE);
   XmUpdateDisplay(animisobut);
  
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(animisoform);
}


void animisonfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = animisofieldactive;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDANIMISO, ifld);
}


void animsubclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (animsubflag == 0) 
      XmToggleButtonGadgetSetState(animsubbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animsubbut,TRUE,FALSE);
   XmUpdateDisplay(animsubbut);
   XtUnmanageChild(animsubform);
}


void animsub_on_off(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, flag;

   arg = (long int)client_data;
   flag = (int)XmToggleButtonGadgetGetState(w);

   if (arg == 0 && flag == 1) animsubflag = 1; 
   if (arg == 1 && flag == 1) animsubflag = 0; 

   if (animsubflag == 0) 
      XmToggleButtonGadgetSetState(animsubbut,FALSE,FALSE);
   else
      XmToggleButtonGadgetSetState(animsubbut,TRUE,FALSE);
   XmUpdateDisplay(animsubbut);
}


void animsubreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void getanimsublims(), animsubfill();

   getanimsublims();
   animsubfill();
}


void animpausestuff(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  int arg;

   arg = (long int)client_data;

   if (arg == 0) irots--;

   if (arg == 1) irots++;

   if (arg == 2) animpausequit = 1;

   if (arg == 3)
     {
      tmpstr = XmTextFieldGetString(animpauseno);
      irots = atof(tmpstr);
      XtFree(tmpstr);
     }

   if (arg == 0 || arg == 1 || arg == 3)
     {
      if (irots < 0) irots = 0;
      if (irots > nrots) irots = nrots;
      animationflag = 1;
      animdraw();
      animationflag = 0;
     }
}


void makeanimationform(parent)
 Widget parent;
{
  Widget animnumtxt, rotlab, trnlab, maglab, beglab, endlab,
         xrotlab, yrotlab, zrotlab, xcenlab, ycenlab, zcenlab,
         maglab2, fadeclose, fadelab, lastwidget, lastleft, animsep,
         blank1, blank2, animvelflowbt, fadeswin, blank3,
         fadeswinform; 
  char str[20];
  char rotstr[30] = "              ROTATION";
  char trnstr[50] = "                   TRANSLATE CENTER";
  char magstr[30] = "          MAGNIFY";
  int i, matpixid;
  unsigned long matpix;
  Arg wargs[20];
  XmString string;
  void makeanimcpform(), makeanimexpform(), makeanimisoform(), 
       makeanimcsform(), makeanimsubform();

   animcpflag = 0;  animcpfieldactive = 0;
   animpexpflag = 0;  animcexpflag = 0;  animcellexpflag = -1;
   animisoflag = 0;  animisofieldactive = 0;  animsubflag = 0;

   string = XmStringCreate("Animation",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animationform = XmCreateFormDialog(parent,"Animation",wargs,i);
      XmStringFree(string);   

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   animclose = XtVaCreateManagedWidget("Animclose",
               xmPushButtonGadgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(animclose,XmNactivateCallback,animationstuff,
                    (caddr_t) 0);

   objnrotsanim = XtVaCreateManagedWidget("Objrotsanim",
                  xmTextFieldWidgetClass,
                  animationform,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animclose,
                  XmNleftOffset, 40,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmaxLength, 4,
                  XmNcolumns, 4,
                  XmNvalue, "30",
                  NULL);
      XtAddCallback(objnrotsanim,XmNactivateCallback,
                    preadanim,(caddr_t) 15);
   string = XmStringCreate("Frames",XmSTRING_DEFAULT_CHARSET);
   animnumtxt = XtVaCreateManagedWidget("Animumtxt",
                xmLabelGadgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, objnrotsanim,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, objnrotsanim,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
 
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   objnrotstxt = XtVaCreateManagedWidget("Objnrotstxt",
                 xmLabelWidgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, objnrotsanim,
                 XmNleftOffset, 10,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);

   string = XmStringCreate("Fade   ",XmSTRING_DEFAULT_CHARSET);
   objanimfade = XtVaCreateManagedWidget("Objanimfade",
                 xmToggleButtonGadgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, objnrotstxt,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNset, FALSE,
                 NULL);
      XmStringFree(string);
      XtAddCallback(objanimfade,XmNvalueChangedCallback,
                       animationstuff, (caddr_t) 3);

   string = XmStringCreate("Vector Flow",XmSTRING_DEFAULT_CHARSET);
   animvelflowbt = XtVaCreateManagedWidget("Animvelflowbt",
                   xmToggleButtonGadgetClass,
                   animationform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, objanimfade,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(animvelflowbt,XmNvalueChangedCallback,
                    animvelflowset,NULL);

   string = XmStringCreate("Subset",XmSTRING_DEFAULT_CHARSET);
   animsubbut = XtVaCreateManagedWidget("Animsubbut",
                xmToggleButtonGadgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, animvelflowbt,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNset, FALSE,
                NULL);
      XmStringFree(string);
      XtAddCallback(animsubbut,XmNvalueChangedCallback,
                    animationstuff, (caddr_t) 11);

   if (polysin == 1 || ncells > 0)
     {
      string = XmStringCreate("Explode",XmSTRING_DEFAULT_CHARSET);
      animexpbut = XtVaCreateManagedWidget("Animexpbut",
                   xmToggleButtonGadgetClass,
                   animationform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, objanimfade,
                   XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNleftWidget, objanimfade,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNset, FALSE,
                   NULL);
         XmStringFree(string);
         XtAddCallback(animexpbut,XmNvalueChangedCallback,
                       animationstuff, (caddr_t) 7);
     }

   if (ncells > 0 && numvars > 0)
     {
      string = XmStringCreate("Cut Plane",XmSTRING_DEFAULT_CHARSET);
      animcpbut = XtVaCreateManagedWidget("Animcpbut",
                  xmToggleButtonGadgetClass,
                  animationform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, objanimfade,
                  XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNleftWidget, animvelflowbt,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
         XmStringFree(string);
         XtAddCallback(animcpbut,XmNvalueChangedCallback,
                       animationstuff, (caddr_t) 5);

      string = XmStringCreate("Isosurface",XmSTRING_DEFAULT_CHARSET);
      animisobut = XtVaCreateManagedWidget("Animsiobut",
                   xmToggleButtonGadgetClass,
                   animationform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, animsubbut,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNset, FALSE,
                   NULL);
         XmStringFree(string);
         XtAddCallback(animisobut,XmNvalueChangedCallback,
                       animationstuff, (caddr_t) 8);

      string = XmStringCreate("Cut Sphere",XmSTRING_DEFAULT_CHARSET);
      animcsbut = XtVaCreateManagedWidget("Animcsbut",
                  xmToggleButtonGadgetClass,
                  animationform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, objanimfade,
                  XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNleftWidget, animsubbut,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
         XmStringFree(string);
         XtAddCallback(animcsbut,XmNvalueChangedCallback,
                       animationstuff, (caddr_t) 10);
     }

   string = XmStringCreate(rotstr,XmSTRING_DEFAULT_CHARSET);
   rotlab = XtVaCreateManagedWidget("Rotlab",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, animnumtxt,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   string = XmStringCreate(trnstr,XmSTRING_DEFAULT_CHARSET);
   trnlab = XtVaCreateManagedWidget("Trnlab",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, animnumtxt,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, rotlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   string = XmStringCreate(magstr,XmSTRING_DEFAULT_CHARSET);
   maglab = XtVaCreateManagedWidget("Maglab",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, animnumtxt,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, trnlab,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   beglab = XtVaCreateManagedWidget("Beglab",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, rotlab,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
   string = XmStringCreate("End  ",XmSTRING_DEFAULT_CHARSET);
   endlab = XtVaCreateManagedWidget("Endlab",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, beglab,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   objxrotbeg = XtVaCreateManagedWidget("Objxrotbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, beglab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objxrotbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 1);
   objxrotend = XtVaCreateManagedWidget("Objxrotend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, endlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objxrotend,XmNactivateCallback,preadanim,
                    (caddr_t) 2);
   objyrotbeg = XtVaCreateManagedWidget("Objyrotbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objxrotbeg,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objyrotbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 3);
   objyrotend = XtVaCreateManagedWidget("Objyrotend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objxrotend,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objyrotend,XmNactivateCallback,preadanim,
                    (caddr_t) 4);
   objzrotbeg = XtVaCreateManagedWidget("Objzrotbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objyrotbeg,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objzrotbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 5);
   objzrotend = XtVaCreateManagedWidget("Objzrotend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objyrotend,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "0.0",
                NULL);
      XtAddCallback(objzrotend,XmNactivateCallback,preadanim,
                    (caddr_t) 6);

   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   blank1 = XtVaCreateManagedWidget("Blank1",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, rotlab,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objzrotbeg,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   objxcenbeg = XtVaCreateManagedWidget("Objxcenbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, blank1,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objxcenbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 7);
   objxcenend = XtVaCreateManagedWidget("Objxcenend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, blank1,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objxcenend,XmNactivateCallback,preadanim,
                    (caddr_t) 8);
   objycenbeg = XtVaCreateManagedWidget("Objycenbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objxcenbeg,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objycenbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 9);
   objycenend = XtVaCreateManagedWidget("Objycenend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objxcenend,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objycenend,XmNactivateCallback,preadanim,
                    (caddr_t) 10);
   objzcenbeg = XtVaCreateManagedWidget("Objzcenbeg",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, rotlab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objycenbeg,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objzcenbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 11);
   objzcenend = XtVaCreateManagedWidget("Objzcenend",
                xmTextFieldWidgetClass,
                animationform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, beglab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objycenend,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
      XtAddCallback(objzcenend,XmNactivateCallback,preadanim,
                    (caddr_t) 12);

   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   blank2 = XtVaCreateManagedWidget("Blank2",
            xmLabelGadgetClass,
            animationform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, rotlab,
            XmNleftAttachment, XmATTACH_WIDGET,
            XmNleftWidget, objzcenbeg,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   objmagbeg = XtVaCreateManagedWidget("Objmagbeg",
               xmTextFieldWidgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, rotlab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, blank2,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 10,
               XmNcolumns, 8,
               XmNvalue, "",
               NULL);
      XtAddCallback(objmagbeg,XmNactivateCallback,preadanim,
                    (caddr_t) 13);
   objmagend = XtVaCreateManagedWidget("Objmagend",
               xmTextFieldWidgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, beglab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, blank2,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNmarginHeight, 0,
               XmNmarginWidth, 1,
               XmNmaxLength, 10,
               XmNcolumns, 8,
               XmNvalue, "",
               NULL);
      XtAddCallback(objmagend,XmNactivateCallback,preadanim,
                    (caddr_t) 14);

   string = XmStringCreate("  xrot",XmSTRING_DEFAULT_CHARSET);
   xrotlab = XtVaCreateManagedWidget("Xrotlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objxrotend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objxrotend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   string = XmStringCreate("  yrot",XmSTRING_DEFAULT_CHARSET);
   yrotlab = XtVaCreateManagedWidget("Zrotlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objyrotend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objyrotend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   string = XmStringCreate("  zrot",XmSTRING_DEFAULT_CHARSET);
   zrotlab = XtVaCreateManagedWidget("Zrotlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objzrotend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objzrotend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("  xcen",XmSTRING_DEFAULT_CHARSET);
   xcenlab = XtVaCreateManagedWidget("Xcenlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objxcenend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objxcenend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   string = XmStringCreate("  ycen",XmSTRING_DEFAULT_CHARSET);
   ycenlab = XtVaCreateManagedWidget("Ycenlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objycenend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objycenend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   string = XmStringCreate("  zcen",XmSTRING_DEFAULT_CHARSET);
   zcenlab = XtVaCreateManagedWidget("Zcenlab",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objzcenend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objzcenend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("  mag.",XmSTRING_DEFAULT_CHARSET);
   maglab2 = XtVaCreateManagedWidget("Maglab2",
             xmLabelGadgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objmagend,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objmagend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   animsep = XtVaCreateManagedWidget("Animsep",
             xmSeparatorWidgetClass,
             animationform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, xrotlab,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             NULL);

   string = XmStringCreate("Quick Look",XmSTRING_DEFAULT_CHARSET);
   qlobj = XtVaCreateManagedWidget("Qlobj",
           xmToggleButtonGadgetClass,
           animationform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, animsep,
           XmNleftAttachment, XmATTACH_FORM,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(qlobj,XmNvalueChangedCallback,animationstuff,
                    (caddr_t) 1);
 
   string = XmStringCreate("Snapshot",XmSTRING_DEFAULT_CHARSET);
   ssobj = XtVaCreateManagedWidget("Ssobj",
           xmToggleButtonGadgetClass,
           animationform,
           XmNtopAttachment, XmATTACH_WIDGET,
           XmNtopWidget, animsep,
           XmNleftAttachment, XmATTACH_WIDGET,
           XmNleftWidget, qlobj,
           XmNleftOffset, 10,
           XmNrightAttachment, XmATTACH_NONE,
           XmNbottomAttachment, XmATTACH_NONE,
           XmNlabelString, string,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(ssobj,XmNvalueChangedCallback,animationstuff,
                    (caddr_t) 2);
   
   string = XmStringCreate("Start",XmSTRING_DEFAULT_CHARSET);
   objanimdoit = XtVaCreateManagedWidget("Objanimdoit",
                 xmPushButtonGadgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animsep,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, ssobj,
                 XmNleftOffset, 20,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(objanimdoit,XmNactivateCallback,
                    animationstuff,(caddr_t) 4);
   
   string = XmStringCreate("Stop",XmSTRING_DEFAULT_CHARSET);
   animstop = XtVaCreateManagedWidget("Animstop",
              xmPushButtonWidgetClass,
              animationform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, animsep,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, objanimdoit,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNsensitive, FALSE,
              NULL);
      XmStringFree(string);
      XtAddCallback(animstop,XmNactivateCallback,
                    animationstuff, (caddr_t) 6);
   
   string = XmStringCreate("Pause",XmSTRING_DEFAULT_CHARSET);
   animpause = XtVaCreateManagedWidget("Animpause",
               xmPushButtonWidgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animsep,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, animstop,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNsensitive, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(animpause,XmNactivateCallback,
                    animationstuff, (caddr_t) 9);

   string = XmStringCreate("Prev.",XmSTRING_DEFAULT_CHARSET);
   animprev = XtVaCreateManagedWidget("Animprev",
               xmPushButtonGadgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animsep,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, animpause,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNsensitive, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(animprev,XmNactivateCallback,
                    animpausestuff, (caddr_t) 0);

   string = XmStringCreate("Next",XmSTRING_DEFAULT_CHARSET);
   animnext = XtVaCreateManagedWidget("Animnext",
               xmPushButtonGadgetClass,
               animationform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animsep,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, animprev,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNsensitive, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(animnext,XmNactivateCallback,
                    animpausestuff, (caddr_t) 1);

   string = XmStringCreate("Resume",XmSTRING_DEFAULT_CHARSET);
   animresume = XtVaCreateManagedWidget("Animresume",
                 xmPushButtonGadgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animsep,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animnext,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNsensitive, FALSE,
                 NULL);
      XmStringFree(string);
      XtAddCallback(animresume,XmNactivateCallback,
                    animpausestuff, (caddr_t) 2);

   string = XmStringCreate("Go To Frame:",XmSTRING_DEFAULT_CHARSET);
   animpausego = XtVaCreateManagedWidget("Animpausego",
                 xmPushButtonGadgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animsep,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animresume,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 XmNsensitive, FALSE,
                 NULL);
      XmStringFree(string);
      XtAddCallback(animpausego,XmNactivateCallback,
                    animpausestuff, (caddr_t) 3);

   animpauseno = XtVaCreateManagedWidget("Animpauseno",
                 xmTextFieldWidgetClass,
                 animationform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animsep,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, animpausego,
                 /* XmNleftOffset, 40, */
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 4,
                 XmNcolumns, 4,
                 XmNvalue, "  ",
                 NULL);
/*
      XtAddCallback(animpauseno,XmNactivateCallback,
                    animpausestuff,(caddr_t) 4);
*/


   /*  Make fade form.  */

   string = XmStringCreate("Fade",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animfadeform = XmCreateFormDialog(animationform,"Animation_Fade",
                                     wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   fadeclose = XtVaCreateManagedWidget("Fadeclose",
               xmPushButtonGadgetClass,
               animfadeform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(fadeclose,XmNactivateCallback,
                    animfadeclose,NULL);

   string = XmStringCreate("TRANSPARENCY",XmSTRING_DEFAULT_CHARSET);
   fadelab = XtVaCreateManagedWidget("Fadelab",
             xmLabelGadgetClass,
             animfadeform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, fadeclose,
             XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   transbeg = XtVaCreateManagedWidget("Transbeg",
              xmScaleWidgetClass,
              animfadeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fadelab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNminimum, 0,
              XmNmaximum, 100,
              XmNorientation, XmHORIZONTAL,
              XmNprocessingDirection, XmMAX_ON_RIGHT,
              XmNvalue, 100,
              XmNscaleWidth, 100,
              XmNscaleMultiple, 1,
              XmNshowValue, TRUE,
              XmNdecimalPoints, 2,
              XmNbackground, yellowpix,
              XmNtitleString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(transbeg,XmNvalueChangedCallback,
                    anim_dotrans,NULL);

   string = XmStringCreate("End",XmSTRING_DEFAULT_CHARSET);
   transend = XtVaCreateManagedWidget("Transend",
              xmScaleWidgetClass,
              animfadeform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, fadelab,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, transbeg,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNminimum, 0,
              XmNmaximum, 100,
              XmNorientation, XmHORIZONTAL,
              XmNprocessingDirection, XmMAX_ON_RIGHT,
              XmNvalue, 100,
              XmNscaleWidth, 100,
              XmNscaleMultiple, 1,
              XmNshowValue, TRUE,
              XmNdecimalPoints, 2,
              XmNbackground, yellowpix,
              XmNtitleString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(transend,XmNvalueChangedCallback,
                    anim_dotrans,NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, transbeg);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 300);
   i++;  XtSetArg(wargs[i],XmNheight, 300);
   i++;
   fadeswin = XmCreateScrolledWindow(animfadeform,"Fadeswin",wargs,i);
   XtManageChild(fadeswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   fadeswinform = XmCreateForm(fadeswin,"Fadeswinform",wargs,i);
   XtManageChild(fadeswinform);

   blank3 = XtVaCreateManagedWidget("Blank3",
            xmSeparatorGadgetClass,
            fadeswinform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNseparatorType, XmNO_LINE,
            NULL);

   lastwidget = blank3;
   for (i = 0; i < MAXMATS /*mmats should also work according to Frank Ortega*/; i++)
     {
      string = XmStringCreate(mmatnames[i],
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Matfadebut%d",i);
      matfadebut[i] = XtVaCreateManagedWidget(str,
                      xmToggleButtonWidgetClass,
                      fadeswinform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, lastwidget,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      XmNspacing, 0,
                      XmNselectColor, yellowpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNvisibleWhenOff, FALSE,
                      NULL);
         XmStringFree(string);
         XtAddCallback(matfadebut[i],XmNvalueChangedCallback,
                       animfade_set_mat,(caddr_t) i);

      string = XmStringCreate("           ",
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Matfadetext%d",i);
      matfadetext[i] = XtVaCreateManagedWidget(str,
                       xmLabelWidgetClass,
                       fadeswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNtopOffset, 1,
                       XmNleftAttachment, XmATTACH_WIDGET,
                       XmNleftWidget, matfadebut[i],
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       NULL);
         XmStringFree(string);

      if (mat_has_active_pts[i] > 0 ||
          mat_has_active_polys[i] > 0)
        {
         lastwidget = matfadebut[i];
         lastleft = matfadetext[i];
        }
      else
        {
         XtUnmanageChild(matfadebut[i]);
         XtUnmanageChild(matfadetext[i]);
        }
     }


   /* lastwidget = blank3; */
   matpixid = 1;
   for (i = 0; i < MAXCONTOURS; i++)
     {
      matpixid++;
      matpix = mat_pixels[matpixid];
      sprintf(str,"%2d",i+1);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Contfadebut%d",i);
      contfadebut[i] = XtVaCreateManagedWidget(str,
                       xmToggleButtonWidgetClass,
                       fadeswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       XmNspacing, 0,
                       XmNselectColor, yellowpix,
                       XmNbackground, matpix,
                       XmNforeground, blackpix,
                       XmNmarginHeight, 0,
                       XmNmarginWidth, 0,
                       XmNvisibleWhenOff, FALSE,
                       NULL);
         XmStringFree(string);
         XtAddCallback(contfadebut[i],XmNvalueChangedCallback,
                       animfade_set_cont,(caddr_t) i);

      string = XmStringCreate("           ",
                              XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Contfadetext%d",i);
      contfadetext[i] = XtVaCreateManagedWidget(str,
                        xmLabelWidgetClass,
                        fadeswinform,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopWidget, lastwidget,
                        XmNtopOffset, 1,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftWidget, contfadebut[i],
                        XmNrightAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_NONE,
                        XmNlabelString, string,
                        NULL);
         XmStringFree(string);

      lastwidget = contfadebut[i];
     }

   /*  Make animcpform and animisoform, if needed.  */
   if (ncells > 0 && numvars > 0)
     {
      makeanimcpform();
      makeanimisoform();
      makeanimcsform();
     }

   /*  Make animexpform, if needed.  */
   if (polysin == 1 || ncells > 0) makeanimexpform();

   /*  Make subsetform animsubform.  */
   makeanimsubform();

}


void makeanimcpform()
{
  Widget cpclose, cpbox1, acpnfldnew, animcpnone, animcpx, animcpy,
         animcpz, animcpax, animcpay, animcpaz, acpcfldnew, acpdisp;
  int i;
  Arg wargs[20];
  XmString string;
  void getanimcplims();

   /*  Make animation cut plane form.  */

   string = XmStringCreate("Cut Plane",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animcpform = XmCreateFormDialog(animationform,"Animcpform",
                                   wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cpclose = XtVaCreateManagedWidget("Cpclose",
             xmPushButtonGadgetClass,
             animcpform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cpclose,XmNactivateCallback,animcpclose,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cpclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cpbox1 = XmCreateRadioBox(animcpform,"Cpbox1",wargs,i);
   XtManageChild(cpbox1);

   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   animcpnone = XtVaCreateManagedWidget("Animcpnone",
                xmToggleButtonGadgetClass,
                cpbox1,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, TRUE,
                NULL);
      XmStringFree(string);
      XtAddCallback(animcpnone,XmNvalueChangedCallback,
                    animcp_select_dir, (caddr_t) 0);

   string = XmStringCreate("Along X",XmSTRING_DEFAULT_CHARSET);
   animcpx = XtVaCreateManagedWidget("Animcpx",
             xmToggleButtonGadgetClass,
             cpbox1,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             XmNmarginHeight, 0,
             XmNmarginWidth, 0,
             NULL);
      XmStringFree(string);
      XtAddCallback(animcpx,XmNvalueChangedCallback,
                    animcp_select_dir,(caddr_t) 1);

   string = XmStringCreate("Along Y",XmSTRING_DEFAULT_CHARSET);
   animcpy = XtVaCreateManagedWidget("Animcpy",
             xmToggleButtonGadgetClass,
             cpbox1,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             XmNmarginHeight, 0,
             XmNmarginWidth, 0,
             NULL);
      XmStringFree(string);
      XtAddCallback(animcpy,XmNvalueChangedCallback,
                    animcp_select_dir, (caddr_t) 2);

   string = XmStringCreate("Along Z",XmSTRING_DEFAULT_CHARSET);
   animcpz = XtVaCreateManagedWidget("Animcpz",
             xmToggleButtonGadgetClass,
             cpbox1,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             XmNmarginHeight, 0,
             XmNmarginWidth, 0,
             NULL);
      XmStringFree(string);
      XtAddCallback(animcpz,XmNvalueChangedCallback,
                    animcp_select_dir,(caddr_t) 3);

   string = XmStringCreate("Around X",XmSTRING_DEFAULT_CHARSET);
   animcpax = XtVaCreateManagedWidget("Animcpax",
              xmToggleButtonGadgetClass,
              cpbox1,
              XmNlabelString, string,
              XmNspacing, 1,
              XmNselectColor, yellowpix,
              XmNmarginHeight, 0,
              XmNmarginWidth, 0,
              NULL);
      XmStringFree(string);
      XtAddCallback(animcpax,XmNvalueChangedCallback,
                    animcp_select_dir,(caddr_t) 4);

   string = XmStringCreate("Around Y",XmSTRING_DEFAULT_CHARSET);
   animcpay = XtVaCreateManagedWidget("Animcpay",
              xmToggleButtonGadgetClass,
              cpbox1,
              XmNlabelString, string,
              XmNspacing, 1,
              XmNselectColor, yellowpix,
              XmNmarginHeight, 0,
              XmNmarginWidth, 0,
              NULL);
      XmStringFree(string);
      XtAddCallback(animcpay,XmNvalueChangedCallback,
                    animcp_select_dir, (caddr_t) 5);

   string = XmStringCreate("Around Z",XmSTRING_DEFAULT_CHARSET);
   animcpaz = XtVaCreateManagedWidget("Animcpaz",
              xmToggleButtonGadgetClass,
              cpbox1,
              XmNlabelString, string,
              XmNspacing, 1,
              XmNselectColor, yellowpix,
              XmNmarginHeight, 0,
              XmNmarginWidth, 0,
              NULL);
      XmStringFree(string);
      XtAddCallback(animcpaz,XmNvalueChangedCallback,
                    animcp_select_dir,(caddr_t) 6);

   animcpbegtxt = XtVaCreateManagedWidget("Animcpbegtxt",
                  xmTextFieldWidgetClass,
                  animcpform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cpbox1,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(animcpbegtxt,XmNactivateCallback,
                    animcpread,(caddr_t) 0);

   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   animcpbeg = XtVaCreateManagedWidget("Animcpbeg",
               xmScaleWidgetClass,
               animcpform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animcpbegtxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNminimum, 0,
               XmNmaximum, 100,
               XmNorientation, XmHORIZONTAL,
               XmNprocessingDirection, XmMAX_ON_RIGHT,
               XmNvalue, 0,
               XmNscaleWidth, 200,
               XmNscaleMultiple, 1,
               XmNtitleString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(animcpbeg,XmNvalueChangedCallback,
                    anim_cpsl,(caddr_t) 0);
      XtAddCallback(animcpbeg,XmNdragCallback,
                    anim_cpsl,(caddr_t) 0);

   animcpendtxt = XtVaCreateManagedWidget("Animcpentxt",
                  xmTextFieldWidgetClass,
                  animcpform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animcpbeg,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_NONE,
                  XmNrightAttachment, XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(animcpendtxt,XmNactivateCallback,
                    animcpread,(caddr_t) 1);

   string = XmStringCreate("End",XmSTRING_DEFAULT_CHARSET);
   animcpend = XtVaCreateManagedWidget("Animcpend",
               xmScaleWidgetClass,
               animcpform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animcpendtxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNminimum, 0,
               XmNmaximum, 100,
               XmNorientation, XmHORIZONTAL,
               XmNprocessingDirection, XmMAX_ON_RIGHT,
               XmNvalue, 100,
               XmNscaleWidth, 200,
               XmNscaleMultiple, 1,
               XmNtitleString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(animcpend,XmNvalueChangedCallback,
                    anim_cpsl,(caddr_t) 1);
      XtAddCallback(animcpend,XmNdragCallback,
                    anim_cpsl,(caddr_t) 1);

   string = XmStringCreate("Options",XmSTRING_DEFAULT_CHARSET);
   acpdisp = XtVaCreateManagedWidget("Acpdisp",
             xmPushButtonGadgetClass,
             animcpform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcpend,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(acpdisp,XmNactivateCallback,animcpdisp,
                    NULL);

   string = XmStringCreate("Node Field:",XmSTRING_DEFAULT_CHARSET);
   animcpnfldbut = XtVaCreateManagedWidget("Animcpnfldbut",
                   xmToggleButtonGadgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acpdisp,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcpnfldbut,XmNvalueChangedCallback,
                    animcp_fldsel,(caddr_t) 0);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   animcpnfldlab = XtVaCreateManagedWidget("Animcpnfldlab",
                   xmLabelWidgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcpnfldbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   acpnfldnew = XtVaCreateManagedWidget("Acpnfldnew",
                xmPushButtonGadgetClass,
                animcpform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcpnfldlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);
         XtAddCallback(acpnfldnew,XmNactivateCallback,animcpnfldfld,
                       NULL);
      curracpnfld = 0;

   string = XmStringCreate("Cell Field:",XmSTRING_DEFAULT_CHARSET);
   animcpcfldbut = XtVaCreateManagedWidget("Animcpcfldbut",
                   xmToggleButtonGadgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acpnfldnew,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcpcfldbut,XmNvalueChangedCallback,
                    animcp_fldsel,(caddr_t) 1);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   animcpcfldlab = XtVaCreateManagedWidget("Animcpcfldlab",
                   xmLabelWidgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcpcfldbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   acpcfldnew = XtVaCreateManagedWidget("Acpcfldnew",
                xmPushButtonGadgetClass,
                animcpform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcpcfldlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);
         XtAddCallback(acpcfldnew,XmNactivateCallback,animcpcfldfld,
                       NULL);
      curracpcfld = 0;
      if (cnumvars < 1)
        {
         XtSetSensitive(animcpcfldbut,FALSE);
         XtSetSensitive(animcpcfldlab,FALSE);
         XtSetSensitive(acpcfldnew,FALSE);
        }

   string = XmStringCreate("Node Materials:",XmSTRING_DEFAULT_CHARSET);
   animcpnmatbut = XtVaCreateManagedWidget("Animcpnmatbut",
                   xmToggleButtonGadgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acpcfldnew,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcpnmatbut,XmNvalueChangedCallback,
                    animcp_fldsel,(caddr_t) 2);

   string = XmStringCreate("Cell Materials:",XmSTRING_DEFAULT_CHARSET);
   animcpcmatbut = XtVaCreateManagedWidget("Animcpcmatbut",
                   xmToggleButtonGadgetClass,
                   animcpform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcpnmatbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcpcmatbut,XmNvalueChangedCallback,
                    animcp_fldsel,(caddr_t) 3);
 
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   animcploc = XtVaCreateManagedWidget("Animcploc",
               xmLabelWidgetClass,
               animcpform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animcpcmatbut,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   getanimcplims();
}


void getanimcplims()
{

   /*  Determine x,y,z mins & maxs from cell  */
   /*  x,y,z mins & maxs times scaleaxis.     */
   cpxmin = cellsxmin * xscaleaxis;  
   cpymin = cellsymin * yscaleaxis;  
   cpzmin = cellszmin * zscaleaxis;  
   cpxmax = cellsxmax * xscaleaxis;
   cpymax = cellsymax * yscaleaxis;
   cpzmax = cellszmax * zscaleaxis;

   if (cpxmin < globxicmin) cpxmin = globxicmin;
   if (cpxmax > globxicmax) cpxmax = globxicmax;
   if (cpymin < globyicmin) cpymin = globyicmin;
   if (cpymax > globyicmax) cpymax = globyicmax;
   if (cpzmin < globzicmin) cpzmin = globzicmin;
   if (cpzmax > globzicmax) cpzmax = globzicmax;
}


void makeanimexpform()
{
  Widget expclose, expapply,exppolybut, expcellbut, expbox,
         exptxt, cexpsel[MAXFLAGS+1];
  int i;
  char str[20];
  Arg wargs[20];
  XmString string;

   /*  Make animation explode form.  */

   string = XmStringCreate("Explode",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animexpform = XmCreateFormDialog(animationform,"Animexpform",
                                    wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   expclose = XtVaCreateManagedWidget("Expclose",
              xmPushButtonGadgetClass,
              animexpform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(expclose,XmNactivateCallback,animexpclose,
                    NULL);

   string = XmStringCreate(" Apply ",XmSTRING_DEFAULT_CHARSET);
   expapply = XtVaCreateManagedWidget("Expapply",
              xmPushButtonGadgetClass,
              animexpform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, expclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(expapply,XmNactivateCallback,animexpapply,
                    NULL);

   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   animexpbeg = XtVaCreateManagedWidget("Animexpbeg",
                xmScaleWidgetClass,
                animexpform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, expapply,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, 0,
                XmNmaximum, 1000,
                XmNorientation, XmHORIZONTAL,
                XmNprocessingDirection, XmMAX_ON_RIGHT,
                XmNvalue, 0,
                XmNscaleWidth, 200,
                XmNscaleMultiple, 1,
                XmNshowValue, TRUE,
                XmNtitleString, string,
                NULL);
      XmStringFree(string);

   string = XmStringCreate("End",XmSTRING_DEFAULT_CHARSET);
   animexpend = XtVaCreateManagedWidget("Animexpend",
                  xmScaleWidgetClass,
                  animexpform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animexpbeg,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNminimum, 0,
                  XmNmaximum, 1000,
                  XmNorientation, XmHORIZONTAL,
                  XmNprocessingDirection, XmMAX_ON_RIGHT,
                  XmNvalue, 0,
                  XmNscaleWidth, 200,
                  XmNscaleMultiple, 1,
                  XmNshowValue, TRUE,
                  XmNtitleString, string,
                  NULL);
      XmStringFree(string);

   string = XmStringCreate("Polygons",XmSTRING_DEFAULT_CHARSET);
   exppolybut = XtVaCreateManagedWidget("Exppolybut",
                xmToggleButtonGadgetClass,
                animexpform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animexpend,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(exppolybut,XmNvalueChangedCallback,
                    anim_exp_polys, (caddr_t) 0);

   string = XmStringCreate("           ",
                           XmSTRING_DEFAULT_CHARSET);
   animpexptext = XtVaCreateManagedWidget("Animpexptext",
                  xmLabelWidgetClass,
                  animexpform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animexpend,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, exppolybut,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

   string = XmStringCreate("cells",XmSTRING_DEFAULT_CHARSET);
   expcellbut = XtVaCreateManagedWidget("Expcellbut",
                xmToggleButtonGadgetClass,
                animexpform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, exppolybut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(expcellbut,XmNvalueChangedCallback,
                    anim_exp_cells, (caddr_t) 0);

   string = XmStringCreate("           ",
                           XmSTRING_DEFAULT_CHARSET);
   animcexptext = XtVaCreateManagedWidget("Animcexptext",
                  xmLabelWidgetClass,
                  animexpform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, exppolybut,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, expcellbut,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
 
   string = XmStringCreate("Explode cells on:",
                           XmSTRING_DEFAULT_CHARSET);
   exptxt = XtVaCreateManagedWidget("Exptxt",
            xmLabelWidgetClass,
            animexpform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, expcellbut,
            XmNtopOffset, 5,
            XmNleftAttachment, XmATTACH_FORM,
            XmNleftOffset, 25,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, exptxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNleftOffset, 25);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   expbox = XmCreateRadioBox(animexpform,"Expbox",wargs,i);
   XtManageChild(expbox);
   for (i = 0; i <= numflags_cell; i++)
     {
      if (i == 0) 
         string = XmStringCreate("Mat. No.",
                                 XmSTRING_DEFAULT_CHARSET);
      else 
         string = XmStringCreate(flagname_cell[i-1],
                                 XmSTRING_DEFAULT_CHARSET);
      sprintf(str,"Cexpsel%d",i);
      cexpsel[i] = XtVaCreateManagedWidget(str,
                    xmToggleButtonGadgetClass,
                    expbox,
                    XmNlabelString, string,
                    XmNspacing, 1,
                    XmNselectColor, yellowpix,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 0,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cexpsel[i],XmNvalueChangedCallback,
                       anim_cell_expflag,(caddr_t) i);

      if (i == 0) XtVaSetValues(cexpsel[0],XmNset,TRUE,NULL);
     }

}


void makeanimisoform()
{
  Widget isoclose, isobox1, isoon, isooff, aisofldlab1,
         aisofldbut, isolab1, isolab2, isolab3, isolab4;
  int i;
  char str[20];
  Arg wargs[20];
  XmString string;

   /*  Make animation isosurface form.  */

   string = XmStringCreate("Isosurface",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animisoform = XmCreateFormDialog(animationform,"Animisoform",
                                    wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   isoclose = XtVaCreateManagedWidget("Isoclose",
              xmPushButtonGadgetClass,
              animisoform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(isoclose,XmNactivateCallback,animisoclose,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, isoclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   isobox1 = XmCreateRadioBox(animisoform,"Isobox1",wargs,i);
   XtManageChild(isobox1);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   isoon = XtVaCreateManagedWidget("Isoon",
               xmToggleButtonGadgetClass,
               isobox1,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(isoon,XmNvalueChangedCallback,
                    animiso_on_off, (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   isooff = XtVaCreateManagedWidget("Isooff",
                xmToggleButtonGadgetClass,
                isobox1,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, TRUE,
                NULL);
      XmStringFree(string);
      XtAddCallback(isooff,XmNvalueChangedCallback,
                    animiso_on_off, (caddr_t) 1);

   string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
   aisofldlab1 = XtVaCreateManagedWidget("Aisofldlab1",
                 xmLabelWidgetClass,
                 animisoform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, isobox1,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
         XmStringFree(string); 

   string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
   animisofldlab = XtVaCreateManagedWidget("Animisofldlab",
                   xmLabelWidgetClass,
                   animisoform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, isobox1,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, aisofldlab1,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   aisofldbut = XtVaCreateManagedWidget("Aisofldbut",
                xmPushButtonGadgetClass,
                animisoform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, aisofldlab1,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);
         XtAddCallback(aisofldbut,XmNactivateCallback,animisonfldfld,
                       NULL);
 
   string = XmStringCreate("Data Range:",XmSTRING_DEFAULT_CHARSET);
   isolab1 = XtVaCreateManagedWidget("Isolab1",
             xmLabelGadgetClass,
             animisoform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, aisofldbut,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   isobeg = fieldmin[0];  isoend = fieldmax[0];
   sprintf(str,"%10.5g",isobeg);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   animisominlab = XtVaCreateManagedWidget("Animisominlab",
                   xmLabelGadgetClass,
                   animisoform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, isolab1,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
 
   string = XmStringCreate(" to ",XmSTRING_DEFAULT_CHARSET);
   isolab2 = XtVaCreateManagedWidget("Isolab2",
             xmLabelGadgetClass,
             animisoform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, isolab1,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, animisominlab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   sprintf(str,"%10.5g",isoend);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   animisomaxlab = XtVaCreateManagedWidget("Animisomaxlab",
                   xmLabelGadgetClass,
                   animisoform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, isolab1,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, isolab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);

   sprintf(str,"%10.5g",isobeg);
   animisobeg = XtVaCreateManagedWidget("Animisobeg",
                 xmTextFieldWidgetClass,
                 animisoform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animisominlab,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNleftWidget, animisominlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 10,
                 XmNcolumns, 10,
                 XmNvalue, str,
                 NULL);
      XtAddCallback(animisobeg,XmNactivateCallback,animiso_txt,
                    (caddr_t) 0);

   sprintf(str,"%10.5g",isoend);
   animisoend = XtVaCreateManagedWidget("Animisoend",
                 xmTextFieldWidgetClass,
                 animisoform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, animisominlab,
                 XmNtopOffset, 10,
                 XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNleftWidget, animisomaxlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 10,
                 XmNcolumns, 10,
                 XmNvalue, str,
                 NULL);
      XtAddCallback(animisoend,XmNactivateCallback,animiso_txt,
                    (caddr_t) 1);
 
   string = XmStringCreate("Begin",XmSTRING_DEFAULT_CHARSET);
   isolab3 = XtVaCreateManagedWidget("Isolab3",
             xmLabelGadgetClass,
             animisoform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animisobeg,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, animisobeg,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   string = XmStringCreate("End",XmSTRING_DEFAULT_CHARSET);
   isolab4 = XtVaCreateManagedWidget("Isolab4",
             xmLabelGadgetClass,
             animisoform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animisobeg,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, animisoend,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
 
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   animisoval = XtVaCreateManagedWidget("Animisoval",
                xmLabelWidgetClass,
                animisoform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, isolab3,
                XmNtopOffset, 10,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
}


void makeanimcsform()
{
  Widget csclose, csbox1, acsnfldnew, animcsoff, animcson, acscfldnew,
         acsdisp, csxlab, csylab, cszlab, csrblab, csrelab;
  int i;
  Arg wargs[20];
  XmString string;

   /*  Make animation cut sphere form.  */

   string = XmStringCreate("Cut Sphere",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animcsform = XmCreateFormDialog(animationform,"Animcsform",
                                   wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csclose = XtVaCreateManagedWidget("Csclose",
             xmPushButtonGadgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(csclose,XmNactivateCallback,animcsclose,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, csclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   csbox1 = XmCreateRadioBox(animcsform,"Csbox1",wargs,i);
   XtManageChild(csbox1);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   animcson = XtVaCreateManagedWidget("Animcson",
              xmToggleButtonGadgetClass,
              csbox1,
              XmNlabelString, string,
              XmNspacing, 1,
              XmNselectColor, yellowpix,
              XmNmarginHeight, 0,
              XmNmarginWidth, 0,
              XmNset, FALSE,
              NULL);
      XmStringFree(string);
      XtAddCallback(animcson,XmNvalueChangedCallback,
                    animcs_on_off, (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   animcsoff = XtVaCreateManagedWidget("Animcsoff",
               xmToggleButtonGadgetClass,
               csbox1,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               NULL);
      XmStringFree(string);
      XtAddCallback(animcsoff,XmNvalueChangedCallback,
                    animcs_on_off,(caddr_t) 1);
 
   string = XmStringCreate("X Cen:",XmSTRING_DEFAULT_CHARSET);
   csxlab = XtVaCreateManagedWidget("Csxlab",
            xmLabelGadgetClass,
            animcsform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, csbox1,
            XmNtopOffset, 10,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   animcsx = XtVaCreateManagedWidget("Animcsx",
             xmTextFieldWidgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, csbox1,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, csxlab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "0",
             NULL);
 
   string = XmStringCreate("Y Cen:",XmSTRING_DEFAULT_CHARSET);
   csylab = XtVaCreateManagedWidget("Csylab",
            xmLabelGadgetClass,
            animcsform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, animcsx,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   animcsy = XtVaCreateManagedWidget("Animcsy",
             xmTextFieldWidgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcsx,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, csylab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "0",
             NULL);
 
   string = XmStringCreate("Z Cen:",XmSTRING_DEFAULT_CHARSET);
   cszlab = XtVaCreateManagedWidget("Cszlab",
            xmLabelGadgetClass,
            animcsform,
            XmNtopAttachment, XmATTACH_WIDGET,
            XmNtopWidget, animcsy,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);

   animcsz = XtVaCreateManagedWidget("Animcsz",
             xmTextFieldWidgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcsy,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, cszlab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 10,
             XmNcolumns, 10,
             XmNvalue, "0",
             NULL);
 
   string = XmStringCreate("R Beg:",XmSTRING_DEFAULT_CHARSET);
   csrblab = XtVaCreateManagedWidget("Csrblab",
             xmLabelGadgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcsz,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   animcsrbeg = XtVaCreateManagedWidget("Animcsrbeg",
                xmTextFieldWidgetClass,
                animcsform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcsz,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, csrblab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 10,
                XmNcolumns, 10,
                XmNvalue, "0",
                NULL);
 
   string = XmStringCreate("R End:",XmSTRING_DEFAULT_CHARSET);
   csrelab = XtVaCreateManagedWidget("Csrelab",
             xmLabelGadgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcsrbeg,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   animcsrend = XtVaCreateManagedWidget("Animcsrend",
                xmTextFieldWidgetClass,
                animcsform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcsrbeg,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, csrelab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 10,
                XmNcolumns, 10,
                XmNvalue, "0",
                NULL);

   string = XmStringCreate("Options",XmSTRING_DEFAULT_CHARSET);
   acsdisp = XtVaCreateManagedWidget("Acsdisp",
             xmPushButtonGadgetClass,
             animcsform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, animcsrend,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(acsdisp,XmNactivateCallback,animcpdisp,
                    NULL);

   string = XmStringCreate("Node Field:",XmSTRING_DEFAULT_CHARSET);
   animcsnfldbut = XtVaCreateManagedWidget("Animcsnfldbut",
                   xmToggleButtonGadgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acsdisp,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcsnfldbut,XmNvalueChangedCallback,
                    animcs_fldsel,(caddr_t) 0);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   animcsnfldlab = XtVaCreateManagedWidget("Animcsnfldlab",
                   xmLabelWidgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcsnfldbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   acsnfldnew = XtVaCreateManagedWidget("Acsnfldnew",
                xmPushButtonGadgetClass,
                animcsform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcsnfldlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);
         XtAddCallback(acsnfldnew,XmNactivateCallback,animcsnfldfld,
                       NULL);
      curracsnfld = 0;

   string = XmStringCreate("Cell Field:",XmSTRING_DEFAULT_CHARSET);
   animcscfldbut = XtVaCreateManagedWidget("Animcscfldbut",
                   xmToggleButtonGadgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acsnfldnew,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcscfldbut,XmNvalueChangedCallback,
                    animcs_fldsel,(caddr_t) 1);

   string = XmStringCreate("None    ",XmSTRING_DEFAULT_CHARSET);
   animcscfldlab = XtVaCreateManagedWidget("Animcscfldlab",
                   xmLabelWidgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcscfldbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 

   string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
   acscfldnew = XtVaCreateManagedWidget("Acscfldnew",
                xmPushButtonGadgetClass,
                animcsform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, animcscfldlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNleftOffset, 20,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);
         XtAddCallback(acscfldnew,XmNactivateCallback,animcscfldfld,
                       NULL);
      curracscfld = 0;
      if (cnumvars < 1)
        {
         XtSetSensitive(animcscfldbut,FALSE);
         XtSetSensitive(animcscfldlab,FALSE);
         XtSetSensitive(acscfldnew,FALSE);
        }

   string = XmStringCreate("Node Materials:",XmSTRING_DEFAULT_CHARSET);
   animcsnmatbut = XtVaCreateManagedWidget("Animcpnmatbut",
                   xmToggleButtonGadgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, acscfldnew,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcsnmatbut,XmNvalueChangedCallback,
                    animcs_fldsel,(caddr_t) 2);

   string = XmStringCreate("Cell Materials:",XmSTRING_DEFAULT_CHARSET);
   animcscmatbut = XtVaCreateManagedWidget("Animcpcmatbut",
                   xmToggleButtonGadgetClass,
                   animcsform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, animcsnmatbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string); 
      XtAddCallback(animcscmatbut,XmNvalueChangedCallback,
                    animcs_fldsel,(caddr_t) 3);
 
   string = XmStringCreate("    ",XmSTRING_DEFAULT_CHARSET);
   animcsloc = XtVaCreateManagedWidget("Animcsloc",
               xmLabelWidgetClass,
               animcsform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, animcscmatbut,
               XmNtopOffset, 10,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
}


void makeanimsubform()
{
  Widget subclose, subbox, animsubon, animsuboff, animsubbeglab, 
         animsubendlab, animsubrsbut, animsubxminlab, animsubxmaxlab, 
         animsubyminlab, animsubymaxlab, animsubzminlab, animsubzmaxlab;
  int i;
  Arg wargs[20];
  XmString string;
  void getanimsublims(), animsubfill();

   /*  Make animation subset form.  */

   string = XmStringCreate("Subset",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   animsubform = XmCreateFormDialog(animationform,"Animsubform",
                                    wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   subclose = XtVaCreateManagedWidget("Subclose",
              xmPushButtonGadgetClass,
              animsubform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(subclose,XmNactivateCallback,animsubclose,
                    NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, subclose);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   subbox = XmCreateRadioBox(animsubform,"Subbox",wargs,i);
   XtManageChild(subbox);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   animsubon = XtVaCreateManagedWidget("Animsubon",
               xmToggleButtonGadgetClass,
               subbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(animsubon,XmNvalueChangedCallback,
                    animsub_on_off, (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   animsuboff = XtVaCreateManagedWidget("Animsuboff",
                xmToggleButtonGadgetClass,
                subbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                NULL);
      XmStringFree(string);
      XtAddCallback(animsuboff,XmNvalueChangedCallback,
                    animsub_on_off,(caddr_t) 1);

   string = XmStringCreate("       Begin       ",XmSTRING_DEFAULT_CHARSET);
   animsubbeglab = XtVaCreateManagedWidget("Animsubbeglab",
                   xmLabelGadgetClass,
                   animsubform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, subbox,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);

   string = XmStringCreate("  End",XmSTRING_DEFAULT_CHARSET);
   animsubendlab = XtVaCreateManagedWidget("Animsubendlab",
                  xmLabelGadgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, subbox,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubbeglab,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);

   string = XmStringCreate("xmin:",XmSTRING_DEFAULT_CHARSET);
   animsubxminlab = XtVaCreateManagedWidget("Animsubxminlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubbeglab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubxmin1 = XtVaCreateManagedWidget("Animsubxmin1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubbeglab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubxminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubxmin2= XtVaCreateManagedWidget("Animsubxmine",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubbeglab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubxmin1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("xmax:",XmSTRING_DEFAULT_CHARSET);
   animsubxmaxlab = XtVaCreateManagedWidget("Animsubxmaxlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubxminlab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubxmax1 = XtVaCreateManagedWidget("Animsubxmax1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubxminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubxmaxlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubxmax2 = XtVaCreateManagedWidget("Animsubxmax2",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubxminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubxmax1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("ymin:",XmSTRING_DEFAULT_CHARSET);
   animsubyminlab = XtVaCreateManagedWidget("Animsubyminlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubxmaxlab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubymin1 = XtVaCreateManagedWidget("Animsubymin1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubxmaxlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubyminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubymin2= XtVaCreateManagedWidget("Animsubymine",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubxmaxlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubymin1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("ymax:",XmSTRING_DEFAULT_CHARSET);
   animsubymaxlab = XtVaCreateManagedWidget("Animsubymaxlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubyminlab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubymax1 = XtVaCreateManagedWidget("Animsubymax1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubyminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubymaxlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubymax2 = XtVaCreateManagedWidget("Animsubymax2",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubyminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubymax1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("zmin:",XmSTRING_DEFAULT_CHARSET);
   animsubzminlab = XtVaCreateManagedWidget("Animsubzminlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubymaxlab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubzmin1 = XtVaCreateManagedWidget("Animsubzmin1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubymaxlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubzminlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubzmin2= XtVaCreateManagedWidget("Animsubzmine",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubymaxlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubzmin1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("zmax:",XmSTRING_DEFAULT_CHARSET);
   animsubzmaxlab = XtVaCreateManagedWidget("Animsubzmaxlab",
                    xmLabelGadgetClass,
                    animsubform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, animsubzminlab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string);

   animsubzmax1 = XtVaCreateManagedWidget("Animsubzmax1",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubzminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubzmaxlab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   animsubzmax2 = XtVaCreateManagedWidget("Animsubzmax2",
                  xmTextFieldWidgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubzminlab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, animsubzmax1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 11,
                  XmNvalue, "",
                  NULL);

   string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
   animsubrsbut = XtVaCreateManagedWidget("Animsubrsbut",
                  xmPushButtonGadgetClass,
                  animsubform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, animsubzmaxlab,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(animsubrsbut,XmNactivateCallback,animsubreset,
                    NULL);

   getanimsublims();
   animsubfill();
}


void getanimsublims()
{
   subxminbeg = subxminend = globxicmin;
   subxmaxbeg = subxmaxend = globxicmax;
   subyminbeg = subyminend = globyicmin;
   subymaxbeg = subymaxend = globyicmax;
   subzminbeg = subzminend = globzicmin;
   subzmaxbeg = subzmaxend = globzicmax;
}

void animsubfill()
{
  char str[20];

   sprintf(str,"%#11.7g",subxminbeg);
   XmTextFieldSetString(animsubxmin1,str);
   XmTextFieldSetString(animsubxmin2,str);
   
   sprintf(str,"%#11.7g",subxmaxbeg);
   XmTextFieldSetString(animsubxmax1,str);
   XmTextFieldSetString(animsubxmax2,str);

   sprintf(str,"%#11.7g",subyminbeg);
   XmTextFieldSetString(animsubymin1,str);
   XmTextFieldSetString(animsubymin2,str);
   
   sprintf(str,"%#11.7g",subymaxbeg);
   XmTextFieldSetString(animsubymax1,str);
   XmTextFieldSetString(animsubymax2,str);

   sprintf(str,"%#11.7g",subzminbeg);
   XmTextFieldSetString(animsubzmin1,str);
   XmTextFieldSetString(animsubzmin2,str);
   
   sprintf(str,"%#11.7g",subzmaxbeg);
   XmTextFieldSetString(animsubzmax1,str);
   XmTextFieldSetString(animsubzmax2,str);
}


void animstart()
{
  char *tmpstr;
  int i;

   /*  Process animation loop.  */
   animstopwin = XtWindow(animstop);
   animpausewin = XtWindow(animpause);
   tmpstr = XmTextFieldGetString(objnrotsanim);
   nrots = atoi(tmpstr);  XtFree(tmpstr);

   if ((nrots > 0) && (animationflag == 0))
     {
      animationflag = 1;
      anim_iterations = nrots + 1;
      animation_count = 0;
      fastdraw = 0;

      animset();

      /*  Loop over the number of frames and draw the images.  */
      irots = 0;
      XtSetSensitive(objanimdoit,FALSE);
      XtSetSensitive(animstop,TRUE);
      if (anim_snapshotsflag == 0)
         XtSetSensitive(animpause,TRUE);
      XtSetSensitive(animclose,FALSE);

      animstopflag = 0;
      animpauseflag = 0;
      for (i = 0; i < anim_iterations; i++)
        {
         irots = i;
         animdraw();
         if (animpauseflag == 1) animpausectl();
         if (animstopflag == 1) break;
        }
     }

   animreset();           
}

void animset()
{
  int i, j, cpfld, csfld;
  char *tmpstr;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);

   /*  Read the data.  */

   tmpstr = XmTextFieldGetString(objxrotbeg);
   xrotbeg = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objxrotend);
   xrotend = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objyrotbeg);
   yrotbeg = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objyrotend);
   yrotend = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objzrotbeg);
   zrotbeg = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objzrotend);
   zrotend = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objxcenbeg);
   xcenbeg = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objxcenend);
   xcenend = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objycenbeg);
   ycenbeg = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objycenend);
   ycenend = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objzcenbeg);
   zcenbeg = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objzcenend);
   zcenend = -atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objmagbeg);
   magbeg = atof(tmpstr);  XtFree(tmpstr);
   tmpstr = XmTextFieldGetString(objmagend);
   magend = atof(tmpstr);  XtFree(tmpstr);

   /*  Set rotation, translation and zoom increments.  */
   xanimrotinc = (xrotend-xrotbeg)/(float)nrots;
   yanimrotinc = (yrotend-yrotbeg)/(float)nrots;
   //zanimrotinc = -(zrotend-zrotbeg)/(float)nrots;
   zanimrotinc = (zrotend-zrotbeg)/(float)nrots;

   xanimceninc = (xcenend-xcenbeg)/(float)nrots;
   yanimceninc = (ycenend-ycenbeg)/(float)nrots;
   zanimceninc = (zcenend-zcenbeg)/(float)nrots;

   animmaginc = (magend-magbeg)/(float)nrots;

   /*  Save old transparent values and set fade increments.  */
   for (i = 0; i < MAXMATS; i++)
     {
      j = i*4;
      transmatold[i] = matrgba[3+j];

      if (animfadematflag[i] == 1)
         animfadematinc[i] = (fadematend[i]-fadematbeg[i])/
                             (float)nrots;
     }

   for (i = 0; i < MAXCONTOURS; i++)
     {
      j = i*4;
      transcontold[i] = conrgba[3+j];

      if (animfadecontflag[i] == 1)
         animfadecontinc[i] = (fadecontend[i]-fadecontbeg[i])/
                              (float)nrots;
     }

   /*  Set increments, generate animation cutplane normals,  */
   /*  and fill animation cutplane structure data.           */
   if (animcpflag == 1)
     {
      animcpreread();
      old_cutplanefieldforplane = cutplanefieldforplane;
      cutplanefieldforplane = animcpfieldactive;
      ddinc = (animddend-animddbeg)/(float)nrots;
      currcp = MAXCUTPLANES;
      cutplane_on[MAXCUTPLANES] = 1;
      if (animcpdir > 0 && animcpdir < 4)
        {
         cutplane[MAXCUTPLANES].norm[0] = animaa;
         cutplane[MAXCUTPLANES].norm[1] = animbb;
         cutplane[MAXCUTPLANES].norm[2] = animcc;
        }
      cutplane[MAXCUTPLANES].cliponbox = 0;
      cutplane[MAXCUTPLANES].cliponcellsel = 0;

      /*  Get cutplane field.  */
      cpfld = animcpfieldactive;
      if (cpfld == -1)
        {
         value=(double *)malloc((nnodes+1)*sizeof(double));
         if (value == NULL) memerr();
         for (i = 0; i < nnodes+1; i++)
            value[i] = (double)imat[i];
        }
      else if (cpfld == -2)
        {
         value=(double *)malloc((ncells)*sizeof(double));
         if (value == NULL) memerr();
         for (i = 0; i < ncells; i++)
            value[i] = (double)cellmats[i];
        }
      else if (cpfld < numvars)
         value = readnodevalue(cpfld);
      else
         value = readcellvalue(cpfld-numvars);
     }

   /*  Set isosurface increments, read data field    */
   /*  and get isosurface beginning and end values.  */
   if (animisoflag == 1)
     {
      isofield = readnodevalue(animisofieldactive);

      tmpstr = XmTextFieldGetString(animisobeg);
      isobeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animisoend);
      isoend = atof(tmpstr);  XtFree(tmpstr);

      isoinc = (isoend-isobeg)/(float)nrots;
     }

   /*  Set increments, read animation cutsphere data,  */
   /*  and fill animation cutsphere structure data.    */
   if (animcsflag == 1)
     {
      old_cutspherefieldforsphere = cutspherefieldforsphere;
      cutspherefieldforsphere = animcsfieldactive;

      tmpstr = XmTextFieldGetString(animcsx);
      csxcen = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animcsy);
      csycen = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animcsz);
      cszcen = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animcsrbeg);
      csrbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animcsrend);
      csrend = atof(tmpstr);  XtFree(tmpstr);

      csrinc = (csrend - csrbeg)/(float)nrots;
      currcp = MAXCUTPLANES+MAXCUTSPHERES+1;
      cutplane_on[currcp] = 1;
      cutplane[currcp].norm[0] = csxcen;
      cutplane[currcp].norm[1] = csycen;
      cutplane[currcp].norm[2] = cszcen;
      cutplane[currcp].cliponbox = 0;
      cutplane[currcp].cliponcellsel = 0;

      /*  Get cutsphere field.  */
      csfld = animcsfieldactive;
      if (csfld == -1)
        {
         csvalue = (double *)malloc((nnodes+1)*sizeof(double));
         if (csvalue == NULL) memerr();
         for (i = 0; i < nnodes+1; i++)
            csvalue[i] = (double)imat[i];
        }
      else if (csfld == -2)
        {
         csvalue = (double *)malloc((ncells)*sizeof(double));
         if (csvalue == NULL) memerr();
         for (i = 0; i < ncells; i++)
            csvalue[i] = (double)cellmats[i];
        }
      else if (csfld < numvars)
         csvalue = readnodevalue(csfld);
      else
         csvalue = readcellvalue(csfld-numvars);
     }

   /*  Set increments, read animation subset data, set increments.  */
   if (animsubflag == 1)
     {
      oldxminfdval = xminfdval;
      oldxmaxfdval = xmaxfdval;
      oldyminfdval = yminfdval;
      oldymaxfdval = ymaxfdval;
      oldzminfdval = zminfdval;
      oldzmaxfdval = zmaxfdval;

      oldxminbdval = xminbdval;
      oldxmaxbdval = xmaxbdval;
      oldyminbdval = yminbdval;
      oldymaxbdval = ymaxbdval;
      oldzminbdval = zminbdval;
      oldzmaxbdval = zmaxbdval;

      tmpstr = XmTextFieldGetString(animsubxmin1);
      subxminbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubxmin2);
      subxminend = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubxmax1);
      subxmaxbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubxmax2);
      subxmaxend = atof(tmpstr);  XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(animsubymin1);
      subyminbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubymin2);
      subyminend = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubymax1);
      subymaxbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubymax2);
      subymaxend = atof(tmpstr);  XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(animsubzmin1);
      subzminbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubzmin2);
      subzminend = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubzmax1);
      subzmaxbeg = atof(tmpstr);  XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(animsubzmax2);
      subzmaxend = atof(tmpstr);  XtFree(tmpstr);

      subxmininc = (subxminend - subxminbeg)/(float)nrots;
      subxmaxinc = (subxmaxend - subxmaxbeg)/(float)nrots;
      subymininc = (subyminend - subyminbeg)/(float)nrots;
      subymaxinc = (subymaxend - subymaxbeg)/(float)nrots;
      subzmininc = (subzminend - subzminbeg)/(float)nrots;
      subzmaxinc = (subzmaxend - subzmaxbeg)/(float)nrots;
     }

   /*  Save old polygon explode data, set increments.  */
   if (animpexpflag == 1)
     {
      old_pexplodefact = pexplodefact;
      animpexpinc = (pexpend-pexpbeg)/(float)nrots;
     }

   /*  Save old cell explode data, set increments  */
   /*  and set vectors on material or flag data.   */
   if (animcexpflag == 1)
     {
      old_cexplodefact = cexplodefact;
      animcexpinc = (cexpend-cexpbeg)/(float)nrots;
      old_cellexpflag = cellexpflag;
      cellexpflag = animcellexpflag;
      cellexpvects();
     }
}


void calcanimrotcp()
{
  double n1, n2, n3, vlen;
  double v1,v2;
  double theta1;

   /*  Calculate third point from angle (animdd).  */
   theta1 = MYPI * (animdd / 180.);
   v1 = sin(theta1);
   v2 = cos(theta1);

   if (animcpdir == 4)
     {
      cpx3 = 0.;  cpy3 = v2;  cpz3 = v1;
     }
   if (animcpdir == 5)
     {
      cpx3 = v2;  cpy3 = 0.;  cpz3 = v1;
     }
   if (animcpdir == 6)
     {
      cpx3 = v2;  cpy3 = v1;  cpz3 = 0;
     }

   n1 = (cpy2-cpy1)*(cpz3-cpz1)-(cpz2-cpz1)*(cpy3-cpy1);
   n2 = (cpz2-cpz1)*(cpx3-cpx1)-(cpx2-cpx1)*(cpz3-cpz1);
   n3 = (cpx2-cpx1)*(cpy3-cpy1)-(cpy2-cpy1)*(cpx3-cpx1);
   vlen = n1*n1 + n2*n2 + n3*n3;

   animaa = n1 / vlen;
   animbb = n2 / vlen;
   animcc = n3 / vlen;
   animdd = -cpx1*animaa-cpy1*animbb-cpz1*animcc;
}


void animdraw()
{
  int i, j, ctype;
  char str[80];
  float isoval, csrr;
  XmString string;
  XEvent stopevent;
  void calccutplane(int i, float aa, float bb, float cc, 
                    float dd, double *value);
  void calccutsphere(int i, float xc, float yc, float zc, 
                     float rr, double *value);
  void calcisosurf(int i, int ctype, float isoval, double *value);


   /*  Check for events while animation is running.  */
   if ((int)XtAppPending(app) != 0)
     {
      if (XCheckWindowEvent(dpy,animstopwin,ButtonReleaseMask,
          &stopevent) != 0)
         animstopflag = 1;
      if (XCheckWindowEvent(dpy,animpausewin,ButtonReleaseMask,
          &stopevent) != 0)
         animpauseflag = 1;
     }

   if (animstopflag == 1 || animpauseflag == 1) return;

   /*  Set frame counter.  */
   sprintf(str,"%4d",irots);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objnrotstxt,XmNlabelString,string,NULL);
   XmStringFree(string);
   XmUpdateDisplay(objnrotstxt);

   /*  Increment rotation, center and magnification.  */
   xanimrot = xrotbeg + (irots * xanimrotinc);
   yanimrot = yrotbeg + (irots * yanimrotinc); 
   zanimrot = zrotbeg + (irots * zanimrotinc);
   xanimcen = xcenbeg + (irots * xanimceninc);
   yanimcen = ycenbeg + (irots * yanimceninc); 
   zanimcen = zcenbeg + (irots * zanimceninc);
   animmag = magbeg + (irots * animmaginc);
   if (irots == anim_iterations-1)
     {
      xanimrot = xrotend;
      yanimrot = yrotend;
      zanimrot = zrotend;
      xanimcen = xcenend;
      yanimcen = ycenend;
      zanimcen = zcenend;
      animmag = magend;
     }

   /*  Increment material and isosurface fades.  */
   for (i = 0; i < MAXMATS; i++)
     {
      if (animfadematflag[i] == 1) 
        {
         animfademat[i] = fadematbeg[i] + (irots * animfadematinc[i]);
         if (animfademat[i] < 0.) animfademat[i] = 0.;
         if (animfademat[i] > 1.) animfademat[i] = 1.;
         j = i*4;
            matrgba[3+j] = animfademat[i];
         drawnew3dflag = 1;
        }
     }

   for (i = 0; i < MAXCONTOURS; i++) 
     {
      if (animfadecontflag[i] == 1)
        {
         animfadecont[i] = fadecontbeg[i] + (irots * animfadecontinc[i]);
         if (animfadecont[i] < 0.) animfadecont[i] = 0.;
         if (animfadecont[i] > 1.) animfadecont[i] = 1.;
         j = i*4;
         conrgba[3+j] = animfadecont[i];
         drawnew3dflag = 1;
        }
     }

   /*  Calculate new cutplane, if requested.  */
   if (animcpflag == 1)
     {
      animdd = -(animddbeg + (irots * ddinc));
      if (irots == anim_iterations-1) animdd = -animddend;
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      /*  If rotating cutplane, calculate cutplane.  */
      if (animcpdir > 3) calcanimrotcp();

      calccutplane(MAXCUTPLANES, animaa, animbb, animcc, animdd, value);

      XUndefineCursor(dpy,topwin);

      sprintf(str,"Cut Plane at %6.4g",-animdd);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcploc,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animcploc);
      drawnew3dflag = 1;
     }

   /*  Calculate animation isosurface, if requested.  */
   if (animisoflag == 1)
     {
      i = MAXCONTOURS + MAXMATS;
      isoval = isobeg + (irots * isoinc);
      if (irots == anim_iterations-1) isoval = isoend;
      ctype = 0;
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      calcisosurf(i,ctype,isoval,isofield);

      XUndefineCursor(dpy,topwin);

      sprintf(str,"Isosurface value: %6.4g",isoval);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animisoval,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animisoval);
      drawnew3dflag = 1;
     }

   /*  Calculate new cutsphere, if requested.  */
   if (animcsflag == 1)
     {
      csrr = csrbeg + (irots * csrinc);
      if (irots == anim_iterations-1) csrr = csrend;
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      calccutsphere(MAXCUTPLANES+MAXCUTSPHERES+1, csxcen, csycen, 
                    cszcen, csrr, csvalue);

      XUndefineCursor(dpy,topwin);

      sprintf(str,"Cut Sphere at r = %6.4g",csrr);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcsloc,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animcsloc);
      drawnew3dflag = 1;
     }

   /*  Set polygon explode data, if requested.  */
   if (animpexpflag == 1)
     {
      pexplodefact = pexpbeg + (irots * animpexpinc);   
      if (irots == anim_iterations-1) pexplodefact = pexpend;
      drawnew3dflag = 1;
     }

   /*  Set cell explode data, if requested.  */
   if (animcexpflag == 1)
     {
      cexplodefact = cexpbeg + (irots * animcexpinc);   
      if (irots == anim_iterations-1) cexplodefact = cexpend;
      drawnew3dflag = 1;
     }

   /*  Redraw display list for vector flow.  */
   if ((vectorflag == 1 || cvectorflag == 1 || svectorflag == 1 || 
        fvectorflag == 1) && animvelflowflag == 1)
      drawnew3dflag = 1;

   /*  Set subset data, if requested.  */
   if (animsubflag == 1)
     {
      xminfdval = xminbdval = subxminbeg + (irots * subxmininc);
      xmaxfdval = xmaxbdval = subxmaxbeg + (irots * subxmaxinc);
      yminfdval = yminbdval = subyminbeg + (irots * subymininc);
      ymaxfdval = ymaxbdval = subymaxbeg + (irots * subymaxinc);
      zminfdval = zminbdval = subzminbeg + (irots * subzmininc);
      zmaxfdval = zmaxbdval = subzmaxbeg + (irots * subzmaxinc);
      setnodecellsubset();
      if (polysflag == 1 || linesflag == 1)
         setpolysubset();
      drawnew3dflag = 1;
     }

   XFlush(dpy);

   fastdraw = 0;             
   if (quicklookflag == 1) fastdraw = 1;
   drawit();
}


void animreset()
{
  int i, j;
  char str[20];
  XmString string;
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);


   /*  Reset button sensitivity.  */
   irots = 0;
   XtSetSensitive(objanimdoit,TRUE);
   XtSetSensitive(animstop,FALSE);
   XtSetSensitive(animpause,FALSE);
   XtSetSensitive(animclose,TRUE);

   /*  Blank out counter.  */
   sprintf(str,"    ");
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(objnrotstxt,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Reset old tranparencies.  */
   for (i = 0; i < MAXMATS; i++) 
      {
       j = i*4;
       matrgba[3+j] = transmatold[i];
      }

   for (i = 0; i < MAXCONTOURS; i++) 
     {
      j = i*4;
      conrgba[3+j] = transcontold[i];
     }

   /*  Free animation cutplane, if needed.  */ 
   if (animcpflag == 1)
     {
      if (cutplane[MAXCUTPLANES].npolys > 0)
        {
         free(cutplane[MAXCUTPLANES].polylist);
         free(cutplane[MAXCUTPLANES].vertsperpoly);
         free(value);
        }
      cutplane[MAXCUTPLANES].npolys = 0;
      cutplane_on[MAXCUTPLANES] = 0;
      cutplanefieldforplane = old_cutplanefieldforplane;

      sprintf(str,"    ");
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcploc,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animcploc);
     }

   /*  Free animation isosurface.  */ 
   if (animisoflag == 1)
     {
      i = MAXCONTOURS + MAXMATS;
      contour_on[i] = 0;
      if (num_of_contour_polys[i] > 0)
        {
         free (contourlist[i]);
        }
      num_of_contour_polys[i] = 0;
      free(isofield);

      sprintf(str,"    ");
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animisoval,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animisoval);
     }

   /*  Free animation cutsphere, if needed.  */ 
   if (animcsflag == 1)
     {
      i = MAXCUTPLANES+MAXCUTSPHERES+1;
      if (cutplane[i].npolys > 0)
        {
         free(cutplane[i].polylist);
         free(cutplane[i].vertsperpoly);
         free(csvalue);
        }
      cutplane[i].npolys = 0;
      cutplane_on[i] = 0;
      cutspherefieldforsphere = old_cutspherefieldforsphere;

      sprintf(str,"    ");
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(animcsloc,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(animcsloc);
     }
    
   animationflag = 0; animation_count = 0;

   /*  Reset old polygon explode data.  */
   if (animpexpflag == 1)
     {
      pexplodefact = old_pexplodefact;
     }

   /*  Reset old cell explode data, and set  */
   /*  vectors on material or flag data.     */
   if (animcexpflag == 1)
     {
      cexplodefact = old_cexplodefact;
      cellexpflag = old_cellexpflag;
      cellexpvects();
     }

   /*  Reset subset data.  */
   if (animsubflag == 1)
     {
      xminfdval = oldxminfdval;
      xmaxfdval = oldxmaxfdval;
      yminfdval = oldyminfdval;
      ymaxfdval = oldymaxfdval;
      zminfdval = oldzminfdval;
      zmaxfdval = oldzmaxfdval;

      xminbdval = oldxminbdval;
      xmaxbdval = oldxmaxbdval;
      yminbdval = oldyminbdval;
      ymaxbdval = oldymaxbdval;
      zminbdval = oldzminbdval;
      zmaxbdval = oldzmaxbdval;

      setnodecellsubset();
      if (polysflag == 1 || linesflag == 1)
         setpolysubset();
     }

   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void setanimationform()
{
  char str[20];
 
   sprintf(str,"%#8.3f",-xtrans);
   XmTextFieldSetString(objxcenbeg,str);
   XmTextFieldSetString(objxcenend,str);
   xcenbeg = xtrans;
   xcenend = xtrans;

   sprintf(str,"%#8.3f",-ytrans);
   XmTextFieldSetString(objycenbeg,str);
   XmTextFieldSetString(objycenend,str);
   ycenbeg = ytrans;
   ycenend = ytrans;

   sprintf(str,"%#8.3f",-ztrans);
   XmTextFieldSetString(objzcenbeg,str);
   XmTextFieldSetString(objzcenend,str);
   zcenbeg = ztrans;
   zcenend = ztrans;

   sprintf(str,"%#8.3f",magnify);
   XmTextFieldSetString(objmagbeg,str);
   XmTextFieldSetString(objmagend,str);
   magbeg = magnify;
   magend = magnify; 

   XtManageChild(animationform);
   animationmenu_up = 1;
}


void animpausectl()
{
  float savxrot, savyrot, savzrot, savmag, savxpan, savypan, savzpan;
  XEvent pauseevent;

   prots = irots;
   irots--;
   /* XtSetSensitive(animstop,FALSE); */
   XtSetSensitive(animpause,FALSE);
   XtSetSensitive(animprev,TRUE);
   XtSetSensitive(animnext,TRUE);
   XtSetSensitive(animresume,TRUE);
   XtSetSensitive(animpausego,TRUE);

   savxrot = xrot;  savyrot = yrot;  savzrot = zrot;
   savmag = magnify;
   savxpan = xpan;  savypan = ypan;  savzpan = zpan;

   animpauseflag = 0;
   animpausequit = 0;
   animationflag = 0;
   while (animpausequit == 0)
     {
      XtAppNextEvent(app, &pauseevent);
      XtDispatchEvent(&pauseevent);
     }

   XtSetSensitive(animstop,TRUE);
   if (anim_snapshotsflag == 0)
      XtSetSensitive(animpause,TRUE);
   XtSetSensitive(animprev,FALSE);
   XtSetSensitive(animnext,FALSE);
   XtSetSensitive(animresume,FALSE);
   XtSetSensitive(animpausego,FALSE);
   irots = prots;

   xrot = savxrot;  yrot = savyrot;  zrot = savzrot;
   magnify = savmag;
   xpan = savxpan;  ypan = savypan;  zpan = savzpan;

   animpauseflag = 0;
   animationflag = 1;
}


void setanimisoclr(int iso_no, unsigned long pixno)
{
   XtVaSetValues(contfadebut[iso_no],XmNbackground,pixno,NULL);
}


void animisogetnfield(int i)
{
  char str[20];
  XmString string;

   animisofieldactive = i;

   string = XmStringCreate(fieldname[animisofieldactive],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animisofldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   isobeg = fieldmin[i];  isoend = fieldmax[i];

   sprintf(str,"%10.5g",isobeg);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animisominlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%11.5g",isoend);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animisomaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%10.5g",isobeg);
   XtVaSetValues(animisobeg,XmNvalue,str,NULL);
   sprintf(str,"%10.5g",isoend);
   XtVaSetValues(animisoend,XmNvalue,str,NULL);
}


void animcutpgetnfield(int i)
{
  XmString string;

   curracpnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animcpnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (animcpfieldactive < numvars && animcpfieldactive >= 0)
      animcpfieldactive = i;
}


void animcutpgetcfield(int i)
{
  XmString string;

   curracpcfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animcpcfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (animcpfieldactive >= numvars)
      animcpfieldactive = i + numvars;
}


void animcutsgetnfield(int i)
{
  XmString string;

   curracsnfld = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animcsnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (animcsfieldactive < numvars && animcsfieldactive >= 0)
      animcsfieldactive = i;
}


void animcutsgetcfield(int i)
{
  XmString string;

   curracscfld = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(animcscfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   if (animcsfieldactive >= numvars)
      animcsfieldactive = i + numvars;
}
