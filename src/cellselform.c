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
#include <stdio.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

#define NSELON 8
static Widget cellmatselform, cellcfldselform, cellnfldselform, 
              cellsphselform, cellnumselform, cselonbut[NSELON],
              cselonbut2[NSELON], cellboxselform, cellgrpselform,
              facegrpselform, cmatselprobe, cmatremprobe;
static Widget cmatsellist[MAXFLAGS+1], cmatandbut[MAXFLAGS+1],
              cmatorbut[MAXFLAGS+1], cselcfldlab, cselcminlab, cselcmaxlab,
              cselcmintxt, cselcmaxtxt, cselerror, cselnfldlab, 
              cselnminlab, cselnmaxlab, cselnmintxt, cselnmaxtxt,
              cselnfldall, cselnfldany, cselsphxtxt,
              cselsphytxt, cselsphztxt, cselsphrtxt,
              cselnumtxt[MAXNUMSEL], cselsphin, cselsphout,
              cselboxx1txt, cselboxy1txt, cselboxz1txt, cselboxin,
              cselboxx2txt, cselboxy2txt, cselboxz2txt, cselboxout,
              cselboxx1sl, cselboxx2sl, cselboxy1sl, cselboxy2sl,
              cselboxz1sl, cselboxz2sl, cellgrplist, cselgrpin, 
              cselgrpout, facegrplist, fselgrpin, fselgrpout;
static int fastboxcount = 0;


void cellselformpop()
{
   XmToggleButtonSetState(cselonbut[0],cellselmat_on,FALSE);
   XmToggleButtonSetState(cselonbut[1],cellselcfld_on,FALSE);
   XmToggleButtonSetState(cselonbut[2],cellselnfld_on,FALSE);
   XmToggleButtonSetState(cselonbut[3],cellselsph_on,FALSE);
   XmToggleButtonSetState(cselonbut[4],cellselnum_on,FALSE);
   XmToggleButtonSetState(cselonbut[5],cellselbox_on,FALSE);
   XmToggleButtonSetState(cselonbut[6],cellselcellgrp_on,FALSE);
   XmToggleButtonSetState(cselonbut[7],cellselfacegrp_on,FALSE);

   MyManageChild(cellselform);
}


void cellmatselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(cselonbut2[0],cellselmat_on,FALSE);

   for (i = 0; i <= numflags_cell; i++)
     {
      if (i == 0) k = maxcellmat;
      else k = flagtypes_cell[i-1];
      XmListDeselectAllItems(cmatsellist[i]);
      for (j = 0; j < k; j++)
        {
         if (cellselflag[i][j] == 1)
            XmListSelectPos(cmatsellist[i],j+1,FALSE);
        }
     }

   if (numflags_cell > 0)
     {
      for (i = 0; i < numflags_cell; i++)
        {
         if (cellandorflag[i] == 0)
           {
            XmToggleButtonGadgetSetState(cmatandbut[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(cmatorbut[i],FALSE,FALSE);
           }
         if (cellandorflag[i] == 1)
           {
            XmToggleButtonGadgetSetState(cmatorbut[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(cmatandbut[i],FALSE,FALSE);
           }
        }
     }

   /*  Calculate material pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx;
   fy = wy + 50;
   XtVaSetValues(cellmatselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(cellmatselform);
}


void cellcfldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (cnumvars > 0)
     {
      XmToggleButtonSetState(cselonbut2[1],cellselcfld_on,FALSE);

      i = cellselfield;

      string = XmStringCreate(cfieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselcfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",cfieldmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselcminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",cfieldmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselcmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (cfldselmin == 0. && cfldselmax == 0.)
        {
         cfldselmin = cfieldmin[i];
         cfldselmax = cfieldmax[i];
        }
      sprintf(str,"%.11g",cfldselmin);
      XmTextFieldSetString(cselcmintxt,str);
      sprintf(str,"%.11g",cfldselmax);
      XmTextFieldSetString(cselcmaxtxt,str);

      /*  Calculate cell field pop-up postition from widgets.  */
      XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
      fx = wx + 20;
      fy = wy + 50;
      XtVaSetValues(cellcfldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(cellcfldselform);
     }
}


void cellnfldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (numvars > 0)
     {
      XmToggleButtonSetState(cselonbut2[2],cellselnfld_on,FALSE);

      i = cellselnfield;

      if (cellselnfldopt)
        {
         XmToggleButtonGadgetSetState(cselnfldany,FALSE,FALSE);         
         XmToggleButtonGadgetSetState(cselnfldall,TRUE,FALSE);         
        }
      else
        {
         XmToggleButtonGadgetSetState(cselnfldany,TRUE,FALSE);         
         XmToggleButtonGadgetSetState(cselnfldall,FALSE,FALSE);         
        }

      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselnfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",fieldmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselnminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",fieldmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cselnmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (cnfldselmin == 0. && cnfldselmax == 0.)
        {
         cnfldselmin = fieldmin[i];
         cnfldselmax = fieldmax[i];
        }
      sprintf(str,"%.11g",cnfldselmin);
      XmTextFieldSetString(cselnmintxt,str);
      sprintf(str,"%.11g",cnfldselmax);
      XmTextFieldSetString(cselnmaxtxt,str);

      if (cellselnfldopt)
        {
         XmToggleButtonGadgetSetState(cselnfldany,FALSE,FALSE);
         XmToggleButtonGadgetSetState(cselnfldall,TRUE,FALSE);
        }
      else
        {
         XmToggleButtonGadgetSetState(cselnfldany,TRUE,FALSE);
         XmToggleButtonGadgetSetState(cselnfldall,FALSE,FALSE);
        }

      /*  Calculate node field pop-up postition from widgets.  */
      XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
      fx = wx + 40;
      fy = wy + 50;
      XtVaSetValues(cellnfldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(cellnfldselform);
     }
}


void cellsphselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(cselonbut2[3],cellselsph_on,FALSE);

   sprintf(str,"%.11g",cellselsph_x);
   XmTextFieldSetString(cselsphxtxt,str);

   sprintf(str,"%.11g",cellselsph_y);
   XmTextFieldSetString(cselsphytxt,str);

   sprintf(str,"%.11g",cellselsph_z);
   XmTextFieldSetString(cselsphztxt,str);

   sprintf(str,"%.11g",cellselsph_r);
   XmTextFieldSetString(cselsphrtxt,str);

   if (cellselsph_in)
     {
      XmToggleButtonGadgetSetState(cselsphin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(cselsphout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(cselsphin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(cselsphout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(cellsphselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(cellsphselform);
}


void cellnumselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[30];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(cselonbut2[4],cellselnum_on,FALSE);

   /*  Fill text lines with cell number select data.  */
   if (ncellselnum > 0)
     {
      for (i = 0; i < ncellselnum; i++)
        {
         if (cellselnum_f[i] == cellselnum_l[i])
            sprintf(str,"%ld",cellselnum_f[i]);
         else if (cellselnum_s[i] == 1)
            sprintf(str,"%ld:%ld",cellselnum_f[i],cellselnum_l[i]);
         else
            sprintf(str,"%ld:%ld:%ld",cellselnum_f[i],cellselnum_l[i],
                                   cellselnum_s[i]);
         XmTextFieldSetString(cselnumtxt[i],str);
        }
      for (i = ncellselnum; i < MAXNUMSEL; i++)
         XmTextFieldSetString(cselnumtxt[i],"");
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx + 80;
   fy = wy + 50;
   XtVaSetValues(cellnumselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(cellnumselform);
}


void cellboxselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;
  void setcellselboxwidgets();

   XmToggleButtonSetState(cselonbut2[5],cellselbox_on,FALSE);

   setcellselboxwidgets();

   if (cellselbox_in)
     {
      XmToggleButtonGadgetSetState(cselboxin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(cselboxout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(cselboxin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(cselboxout,TRUE,FALSE); 
     }

   /*  Calculate box pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(cellboxselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(cellboxselform);
}


void cellgrpselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;
  Position  wx, wy, fx, fy, p0 = 0;

   if (cellselgrp_in)
     {
      XmToggleButtonGadgetSetState(cselgrpin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(cselgrpout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(cselgrpin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(cselgrpout,TRUE,FALSE); 
     }

   XmToggleButtonSetState(cselonbut2[5],cellselcellgrp_on,FALSE);
   XmListDeselectAllItems(cellgrplist);

   for (j = 0; j < ncellgrpselected; j++)
     {
      i = cellgrpselected[j];
      XmListSelectPos(cellgrplist,i,FALSE);
     }

   /*  Calculate cell group pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx + 30;
   fy = wy + 60;
   XtVaSetValues(cellgrpselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(cellgrpselform);
}


void facegrpselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;
  Position  wx, wy, fx, fy, p0 = 0;

   if (faceselgrp_in)
     {
      XmToggleButtonGadgetSetState(fselgrpin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(fselgrpout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(fselgrpin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(fselgrpout,TRUE,FALSE); 
     }

   XmToggleButtonSetState(cselonbut2[7],cellselfacegrp_on,FALSE);
   XmListDeselectAllItems(facegrplist);

   for (j = 0; j < nfacegrpselected; j++)
     {
      i = facegrpselected[j];
      XmListSelectPos(facegrplist,i,FALSE);
     }

   /*  Calculate cell group pop-up postition from widgets.  */
   XtTranslateCoords(cselerror, p0,p0, &wx,&wy); 
   fx = wx + 30;
   fy = wy + 60;
   XtVaSetValues(facegrpselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(facegrpselform);
}


void cselmatclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellmatselprobeflag = 0;
   XmToggleButtonSetState(cmatselprobe,FALSE,FALSE);
   cellmatremprobeflag = 0;
   XmToggleButtonSetState(cmatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   XtUnmanageChild(cellmatselform);
}


void cselcfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfieldform);
   XtUnmanageChild(cellcfldselform);
}


void cselnfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(cellnfldselform);
}


void cselsphclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellsphselform);
}


void cselnumclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellnumselform);
}


void cselboxclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellselbox_draw = 0;
   expose_redraw();
   XtUnmanageChild(cellboxselform);
}


void cellgrpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellgrpselform);
}


void facegrpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(facegrpselform);
}


void cselclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cellmatselform);
   XUndefineCursor(dpy,windowid);
   cellmatselprobeflag = 0;
   XmToggleButtonSetState(cmatselprobe,FALSE,FALSE);
   cellmatremprobeflag = 0;
   XmToggleButtonSetState(cmatremprobe,FALSE,FALSE);
   if (cnumvars > 0)
      cselcfldclose(NULL,NULL,NULL);
   if (numvars > 0)
      cselnfldclose(NULL,NULL,NULL);
   XtUnmanageChild(cellsphselform);
   XtUnmanageChild(cellnumselform);
   cselboxclose(NULL,NULL,NULL);
   if (ncellgrps > 0 || physedflag)
      XtUnmanageChild(cellgrpselform);
   if (nfacegrps > 0)
      XtUnmanageChild(facegrpselform);
   XFlush(dpy);

   XtUnmanageChild(cellselform);
}


void cselnfldsetopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellselnfldopt = (short)XmToggleButtonGadgetGetState(cselnfldall);
}


void cselsphsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellselsph_in = (short)XmToggleButtonGadgetGetState(cselsphin);
}


void cselboxsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellselbox_in = (short)XmToggleButtonGadgetGetState(cselboxin);
}


void cselgrpsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   cellselgrp_in = (short)XmToggleButtonGadgetGetState(cselgrpin);
}


void fselgrpsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   faceselgrp_in = (short)XmToggleButtonGadgetGetState(fselgrpin);
}


void cselsetonbut(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      cellselmat_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[0],cellselmat_on,FALSE);
      XmToggleButtonSetState(cselonbut2[0],cellselmat_on,FALSE);
     }
   if (arg == 1)
     {
      cellselcfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[1],cellselcfld_on,FALSE);
      XmToggleButtonSetState(cselonbut2[1],cellselcfld_on,FALSE);
     }
   if (arg == 2)
     {
      cellselnfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[2],cellselnfld_on,FALSE);
      XmToggleButtonSetState(cselonbut2[2],cellselnfld_on,FALSE);
     }
   if (arg == 3)
     {
      cellselsph_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[3],cellselsph_on,FALSE);
      XmToggleButtonSetState(cselonbut2[3],cellselsph_on,FALSE);
     }
   if (arg == 4)
     {
      cellselnum_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[4],cellselnum_on,FALSE);
      XmToggleButtonSetState(cselonbut2[4],cellselnum_on,FALSE);
     }
   if (arg == 5)
     {
      cellselbox_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[5],cellselbox_on,FALSE);
      XmToggleButtonSetState(cselonbut2[5],cellselbox_on,FALSE);
     }
   if (arg == 6)
     {
      cellselcellgrp_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[6],cellselcellgrp_on,FALSE);
      XmToggleButtonSetState(cselonbut2[6],cellselcellgrp_on,FALSE);
     }
   if (arg == 7)
     {
      cellselfacegrp_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(cselonbut[7],cellselfacegrp_on,FALSE);
      XmToggleButtonSetState(cselonbut2[7],cellselfacegrp_on,FALSE);
     }
}


void cselapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k, ierr, tmpstrlen, *grpselected, *cmatselected, nlist;
  unsigned long num;
  char *tmpstr;
  float x1,y1,z1, x2,y2,z2;
  static char *token;
  static char *str1 = 
           "Error - cell range minimum must be less than maximum.";
  static char *str2 = 
           "Error - node range minimum must be greater than minimum.";
  static char *str3 = 
           "Error - search sphere radius must be greater than 0.";
  static char str4[55]; 
  XmString string;
  char *chk_num = "0123456789: ", *chk_blank = " ";
  void setcellsel();

   /*  Turn off cell material select and remove probe buttons.  */
   cellmatselprobeflag = 0;
   XmToggleButtonSetState(cmatselprobe,FALSE,FALSE);
   cellmatremprobeflag = 0;
   XmToggleButtonSetState(cmatremprobe,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);
   XFlush(dpy);

   /*  Get and set the cell material and cell flags selection.  */
   for (i = 0; i <= numflags_cell; i++)
     {
      j = XmListGetSelectedPos(cmatsellist[i],&cmatselected,&nlist);
      if (!j) nlist = 0;
      for (j = 0; j < MAXMATS; j++) cellselflag[i][j] = 0;
      for (j = 0; j < nlist; j++)
        { 
         k = cmatselected[j] - 1;
         cellselflag[i][k] = 1;
        }
      FREE(cmatselected);
     }

   /*  Read and check the cell range values.  */
   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselerror,XmNlabelString,string,NULL);
   XmStringFree(string);

   cellselbox_draw = 0;
   ierr = 0;
   if (cnumvars > 0 && cellselcfld_on)
     {
      tmpstr = XmTextFieldGetString(cselcmintxt);
      cfldselmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselcmaxtxt);
      cfldselmax = atof(tmpstr);
      XtFree(tmpstr);

      if (cfldselmin > cfldselmax)
        {
         ierr = 1;
         string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(cselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check the node range values.  */
   if (numvars > 0 && cellselnfld_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(cselnmintxt);
      cnfldselmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselnmaxtxt);
      cnfldselmax = atof(tmpstr);
      XtFree(tmpstr);

      if (cnfldselmin > cnfldselmax)
        {
         ierr = 1;
         string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(cselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check the search sphere values.  */
   if (cellselsph_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(cselsphxtxt);
      cellselsph_x = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselsphytxt);
      cellselsph_y = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselsphztxt);
      cellselsph_z = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselsphrtxt);
      cellselsph_r = atof(tmpstr);
      XtFree(tmpstr);

      if (cellselsph_r <= 0)
        {
         ierr = 1;
         string = XmStringCreate(str3,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(cselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check cell number values.  */
   if (cellselnum_on && ierr == 0)
     {
      ncellselnum = 0;
      i = 0;

      while(ierr == 0 && i < MAXNUMSEL)
        {
         tmpstr = XmTextFieldGetString(cselnumtxt[i]);
         tmpstrlen = strlen(tmpstr);
         if (tmpstrlen > 0)
           {

            /*  Check for invalid characters.  */
            if (strspn(tmpstr,chk_num) != tmpstrlen)
              {
               ierr = 1;
               sprintf(str4,"Error - invalid character in line %d",i+1);
               string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
               XtVaSetValues(cselerror,XmNlabelString,string,NULL);
               XmStringFree(string);
              }

            /*  If not all blanks, extract data from the string.  */
            if (ierr == 0 && strspn(tmpstr,chk_blank) != tmpstrlen)
              {

               /*  Get the first number from the set.  */
               token = strtok(tmpstr,":");
               num = strtoul(token,NULL,10);
               if (num > ncells && !cellidflag) num = ncells;
               cellselnum_f[ncellselnum] = num;

               /*  Get the last number from the set.  */
               cellselnum_l[ncellselnum] = num;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  if (num > ncells && !cellidflag) num = ncells;

                  /*  If first greater than last, switch.  */
                  if (num >= cellselnum_f[i])
                     cellselnum_l[ncellselnum] = num;
                  else
                    {
                     cellselnum_l[i] = cellselnum_f[i];
                     cellselnum_f[i] = num;
                    }
                 }

               /*  Get the stride from the set.  */
               cellselnum_s[ncellselnum] = 1;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  cellselnum_s[ncellselnum] = num;
                 }

               ncellselnum++;
              }
           }
         XtFree(tmpstr);
         i++;
        }

     }

   /*  Read and set the search box values.  */
   if (cellselbox_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(cselboxx1txt);
      x1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselboxy1txt);
      y1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselboxz1txt);
      z1 = atof(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(cselboxx2txt);
      x2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselboxy2txt);
      y2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(cselboxz2txt);
      z2 = atof(tmpstr);
      XtFree(tmpstr);

      cellselbox_xmin = (x1 < x2) ? x1 : x2;
      cellselbox_ymin = (y1 < y2) ? y1 : y2;
      cellselbox_zmin = (z1 < z2) ? z1 : z2;
      cellselbox_xmax = (x1 > x2) ? x1 : x2;
      cellselbox_ymax = (y1 > y2) ? y1 : y2;
      cellselbox_zmax = (z1 > z2) ? z1 : z2;
     }

   /*  Read and set cell group selections.  */
   ncellgrpselected = 0;
   if (cellselcellgrp_on && ierr == 0)
     {
      j = XmListGetSelectedPos(cellgrplist,&grpselected,&ncellgrpselected);
      if (!j) ncellgrpselected = 0;
      for (i = 0; i < MAXGROUPS; i++) cellgrpselected[i] = 0;
      for (i = 0; i < ncellgrpselected; i++) 
         cellgrpselected[i] = grpselected[i];
      FREE(grpselected);
     }

   /*  Read and set face group selections.  */
   nfacegrpselected = 0;
   if (cellselfacegrp_on && ierr == 0)
     {
      j = XmListGetSelectedPos(facegrplist,&grpselected,&nfacegrpselected);
      if (!j) nfacegrpselected = 0;
      for (i = 0; i < MAXGROUPS; i++) facegrpselected[i] = 0;
      for (i = 0; i < nfacegrpselected; i++) 
         facegrpselected[i] = grpselected[i];
      FREE(grpselected);
     }

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      setcellsel();
      if (cellsflag) setfacesel();
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void csset_on(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;

   i = (long int)client_data;
   if (i == 0) k = maxcellmat;
   else k = flagtypes_cell[i-1];

   XmListDeselectAllItems(cmatsellist[i]);

   for (j = 0; j < k; j++)
     {
      XmListSelectPos(cmatsellist[i],j+1,FALSE);
     }
}


void csset_off(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;

   XmListDeselectAllItems(cmatsellist[i]);
}


void cselect_and_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) cellandorflag[i] = 1;
   if (j == 1) cellandorflag[i] = 0;
}


void cselect_or_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) cellandorflag[i] = 0;
   if (j == 1) cellandorflag[i] = 1;
}


void cselcfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = cellselfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDCELLSEL, ifld);
}


void cselcfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = cellselfield;

   cfldselmin = cfieldmin[i];
   cfldselmax = cfieldmax[i];

   sprintf(str,"%.11g",cfieldmin[i]);
   XmTextFieldSetString(cselcmintxt,str);
   sprintf(str,"%.11g",cfieldmax[i]);
   XmTextFieldSetString(cselcmaxtxt,str);
}


void cselnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = cellselnfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDCELLSEL, ifld);
}


void cselnfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = cellselnfield;

   cnfldselmin = fieldmin[i];
   cnfldselmax = fieldmax[i];

   sprintf(str,"%.11g",fieldmin[i]);
   XmTextFieldSetString(cselnmintxt,str);
   sprintf(str,"%.11g",fieldmax[i]);
   XmTextFieldSetString(cselnmaxtxt,str);
}


void csboxsubsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];

   cellselbox_draw = 1;
   arg = (long int)client_data;

   XmScaleGetValue(w,&ival);
   val = (float) ival / 1000.;
 
   if (arg == 0 || arg == 6)
     {
      cellselbox_xmin = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",cellselbox_xmin);
      XtVaSetValues(cselboxx1txt,XmNvalue,str,NULL);
      if (cellselbox_xmin > cellselbox_xmax)
        {
         XmScaleSetValue(cselboxx2sl,ival);
         XtVaSetValues(cselboxx2txt,XmNvalue,str,NULL);
         cellselbox_xmax = cellselbox_xmin;
        }
     }
   if (arg == 1 || arg == 7)
     {
      cellselbox_xmax = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",cellselbox_xmax);
      XtVaSetValues(cselboxx2txt,XmNvalue,str,NULL);
      if (cellselbox_xmin > cellselbox_xmax)
        {
         XmScaleSetValue(cselboxx1sl,ival);
         XtVaSetValues(cselboxx1txt,XmNvalue,str,NULL);
         cellselbox_xmin = cellselbox_xmax;
        }
     }
   if (arg == 2 || arg == 8)
     {
      cellselbox_ymin = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",cellselbox_ymin);
      XtVaSetValues(cselboxy1txt,XmNvalue,str,NULL);
      if (cellselbox_ymin > cellselbox_ymax)
        {
         XmScaleSetValue(cselboxy2sl,ival);
         XtVaSetValues(cselboxy2txt,XmNvalue,str,NULL);
         cellselbox_ymax = cellselbox_ymin;
        }
     }
   if (arg == 3 || arg == 9)
     {
      cellselbox_ymax = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",cellselbox_ymax);
      XtVaSetValues(cselboxy2txt,XmNvalue,str,NULL);
      if (cellselbox_ymin > cellselbox_ymax)
        {
         XmScaleSetValue(cselboxy1sl,ival);
         XtVaSetValues(cselboxy1txt,XmNvalue,str,NULL);
         cellselbox_ymin = cellselbox_ymax;
        }
     }
   if (arg == 4 || arg == 10)
     {
      cellselbox_zmin = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",cellselbox_zmin);
      XtVaSetValues(cselboxz1txt,XmNvalue,str,NULL);
      if (cellselbox_zmin > cellselbox_zmax)
        {
         XmScaleSetValue(cselboxz2sl,ival);
         XtVaSetValues(cselboxz2txt,XmNvalue,str,NULL);
         cellselbox_zmax = cellselbox_zmin;
        }
     }
   if (arg == 5 || arg == 11)
     {
      cellselbox_zmax = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",cellselbox_zmax);
      XtVaSetValues(cselboxz2txt,XmNvalue,str,NULL);
      if (cellselbox_zmin > cellselbox_zmax)
        {
         XmScaleSetValue(cselboxz1sl,ival);
         XtVaSetValues(cselboxz1txt,XmNvalue,str,NULL);
         cellselbox_zmin = cellselbox_zmax;
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


void cselboxtxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr, str[20];
  float val;
  void setcellselboxwidgets();

   cellselbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0 || arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      if (arg == 0)
        {
         cellselbox_xmin = val;
         if (cellselbox_xmin > cellselbox_xmax) 
            cellselbox_xmax = cellselbox_xmin;
        }
      else
        {
         cellselbox_xmax = val;
         if (cellselbox_xmin > cellselbox_xmax)
            cellselbox_xmin = cellselbox_xmax;
        }
     }
 
   if (arg == 2 || arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      if (arg == 2)
        {
         cellselbox_ymin = val;
         if (cellselbox_ymin > cellselbox_ymax)
            cellselbox_ymax = cellselbox_ymin;
        }
      else
        {
         cellselbox_ymax = val;
         if (cellselbox_ymin > cellselbox_ymax)
            cellselbox_ymin = cellselbox_ymax;
        }
     }
 
   if (arg == 4 || arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      if (arg == 4)
        {
         cellselbox_zmin = val;
         if (cellselbox_zmin > cellselbox_zmax)
            cellselbox_zmax = cellselbox_zmin;
        }
      else
        {
         cellselbox_zmax = val;
         if (cellselbox_zmin > cellselbox_zmax)
            cellselbox_zmin = cellselbox_zmax;
        }
     }

   XtFree(tmpstr);

   setcellselboxwidgets();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void cmatsel_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   cellmatselprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (cellmatselprobeflag)
     {
      cellmatremprobeflag = 0;
      XmToggleButtonSetState(cmatremprobe,FALSE,FALSE);
      XmListDeselectAllItems(cmatsellist[0]);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else
      XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


void cmatrem_probe(Widget w,XtPointer client_data,XtPointer call_data)
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   cellmatremprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (cellmatremprobeflag)
     {
      cellmatselprobeflag = 0;
      XmToggleButtonSetState(cmatselprobe,FALSE,FALSE);
      get_1_point(NULL, (caddr_t) 3, NULL);
     }
   else XUndefineCursor(dpy,windowid);
   XFlush(dpy);
}


void cmatprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int cellid, id, goodid;

   /*  Get the cell number.  */
   cellid = getcellprobe(pa,pb,pc,pd,pe,pf);

   /*  Set material toggle button.  */
   id = -1;
   if (cellid >= 0 && cellid < ncells)
      id = cellmats[cellid] - 1;
   goodid = 1;
   if (id < 0 || id >= maxcellmat) goodid = 0;

   if (cellmatselprobeflag && goodid)
     {
      XmListSelectPos(cmatsellist[0],id+1,FALSE);
     }
   if (cellmatremprobeflag && goodid)
     {
      XmListDeselectPos(cmatsellist[0],id+1,FALSE);
     }

   XFlush(dpy);
}


void csnumclr(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char blank[1] = "";

   for (i = 0; i < MAXNUMSEL; i++)
     {
      XmTextFieldSetString(cselnumtxt[i],blank);
     }
}



void makecellselform(parent)
 Widget parent;
{
  Widget cslclose, cslapply, csmatbut, cslcfldbut, cslnfldbut, cssphbut,
         csmatclose, cstxt[MAXFLAGS+1], candorbox[MAXFLAGS],
         cson[MAXFLAGS+1], csoff[MAXFLAGS+1], cscfldclose, 
         cselcfldlab1, cselcfldbut, cselcminlab1, cselcmaxlab1, 
         cselcrangelab, cselcminlab2, cselcmaxlab2, cselcreset,
         csnfldclose, cselnfldlab1, cselnfldbut, cselnminlab1, 
         cselnmaxlab1, cselnrangelab, cselnminlab2, cselnmaxlab2, 
         cselnreset, cssphclose, cssphxlab, cssphylab, cssphzlab,
         cssphrlab, csnfldbox, csnumbut, csnumclose, csnumwin,
         csnumform, csnumlab[MAXNUMSEL], csmatapply, csnfldapply,
         cscfldapply, cssphapply, csnumapply, cssphbox, csboxclose,
         csboxapply, csboxx1lab, csboxx2lab, csboxy1lab,
         csboxy2lab, csboxz1lab, csboxz2lab, csboxbox, csboxbut,
         cscgrpclose, cscgrpapply, csfgrpclose, csfgrpapply,
         csgrpbox, fsgrpbox, csnumclrbut;
  Widget lastleft, cellselswin, cswinform, cscgrpbut, csfgrpbut;
  int i, j, jj, k, kk, winw;
  char str[20];
  Arg wargs[MAXGROUPS+20];
  XmString string;
  XmString cellmatitem[MAXMATS];
  XmString cellgrpitem[MAXGROUPS], facegrpitem[MAXGROUPS];


   string = XmStringCreate("Cell Select",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellselform = XmCreateFormDialog(parent,"Cell_Select",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cslclose = XtVaCreateManagedWidget("Clsclose",
              xmPushButtonGadgetClass,
              cellselform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cslclose,XmNactivateCallback,cselclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cslapply = XtVaCreateManagedWidget("Cslapply",
              xmPushButtonGadgetClass,
              cellselform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cslclose,
              XmNleftOffset, 10,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cslapply,XmNactivateCallback,cselapply,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[0] = XtVaCreateManagedWidget("Cselonbut0",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cslclose,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[0],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Materials\nand Flags",
                               XmSTRING_DEFAULT_CHARSET);
   csmatbut = XtVaCreateManagedWidget("Csmatbut",
              xmPushButtonGadgetClass,
              cellselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cslclose,
              XmNtopOffset, 10,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cselonbut[0],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(csmatbut,XmNactivateCallback,cellmatselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[1] = XtVaCreateManagedWidget("Cselonbut1",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, csmatbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[1],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 1);

   string = XmStringLtoRCreate("Cell Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   cslcfldbut = XtVaCreateManagedWidget("Cslcfldbut",
                xmPushButtonGadgetClass,
                cellselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, csmatbut,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cselonbut[1],
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cslcfldbut,XmNactivateCallback,cellcfldselformpop,NULL);
   if (cnumvars == 0)
     {
      XtSetSensitive(cselonbut[1],FALSE);
      XtSetSensitive(cslcfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[2] = XtVaCreateManagedWidget("Cselonbut2",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cslcfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[2],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 2);

   string = XmStringLtoRCreate("Node Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   cslnfldbut = XtVaCreateManagedWidget("Cslnfldbut",
                xmPushButtonGadgetClass,
                cellselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cslcfldbut,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cselonbut[2],
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cslnfldbut,XmNactivateCallback,cellnfldselformpop,NULL);
   if (numvars == 0)
     {
      XtSetSensitive(cselonbut[2],FALSE);
      XtSetSensitive(cslnfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[3] = XtVaCreateManagedWidget("Cselonbut3",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cslnfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[3],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 3);

   string = XmStringLtoRCreate("Search\nSphere",
                               XmSTRING_DEFAULT_CHARSET);
   cssphbut = XtVaCreateManagedWidget("Cssphbut",
               xmPushButtonGadgetClass,
               cellselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cslnfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cselonbut[3],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cssphbut,XmNactivateCallback,cellsphselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[4] = XtVaCreateManagedWidget("Cselonbut4",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cssphbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[4],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 4);

   string = XmStringLtoRCreate("Cell\nNumbers",
                               XmSTRING_DEFAULT_CHARSET);
   csnumbut = XtVaCreateManagedWidget("Csnumbut",
              xmPushButtonGadgetClass,
              cellselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cssphbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cselonbut[4],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(csnumbut,XmNactivateCallback,cellnumselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[5] = XtVaCreateManagedWidget("Cselonbut5",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, csnumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[5],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 5);

   string = XmStringLtoRCreate("Search\nBox",
                               XmSTRING_DEFAULT_CHARSET);
   csboxbut = XtVaCreateManagedWidget("Csboxbut",
               xmPushButtonGadgetClass,
               cellselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csnumbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cselonbut[5],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(csboxbut,XmNactivateCallback,cellboxselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[6] = XtVaCreateManagedWidget("Cselonbut6",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, csboxbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[6],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 6);

   string = XmStringLtoRCreate("Cell\nGroups",
                               XmSTRING_DEFAULT_CHARSET);
   cscgrpbut = XtVaCreateManagedWidget("Cscgrpbut",
               xmPushButtonGadgetClass,
               cellselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csboxbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cselonbut[6],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(cscgrpbut,XmNactivateCallback,cellgrpselformpop,NULL);
   if (ncellgrps == 0 && physedflag == 0)
     {
      XtSetSensitive(cselonbut[6],FALSE);
      XtSetSensitive(cscgrpbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   cselonbut[7] = XtVaCreateManagedWidget("Cselonbut7",
                  xmToggleButtonWidgetClass,
                  cellselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cscgrpbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut[7],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 7);

   string = XmStringLtoRCreate("Face\nGroups",
                               XmSTRING_DEFAULT_CHARSET);
   csfgrpbut = XtVaCreateManagedWidget("Csfgrpbut",
               xmPushButtonGadgetClass,
               cellselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cscgrpbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, cselonbut[7],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(csfgrpbut,XmNactivateCallback,facegrpselformpop,NULL);
   if (nfacegrps == 0)
     {
      XtSetSensitive(cselonbut[7],FALSE);
      XtSetSensitive(csfgrpbut,FALSE);
     }

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   cselerror = XtVaCreateManagedWidget("Cselerror",
               xmLabelWidgetClass,
               cellselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, csfgrpbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 


   /*  Create Materials and Flags selection menu.  */
   /************************************************/

   string = XmStringCreate("Cell Select - Materials and Flags",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellmatselform = XmCreateFormDialog(cellselform,"Cellmatselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csmatclose = XtVaCreateManagedWidget("Csmatclose",
                xmPushButtonGadgetClass,
                cellmatselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csmatclose,XmNactivateCallback,cselmatclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   csmatapply = XtVaCreateManagedWidget("Csmatapply",
                xmPushButtonGadgetClass,
                cellmatselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, csmatclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csmatapply,XmNactivateCallback,cselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   cselonbut2[0] = XtVaCreateManagedWidget("Cselonbut20",
                   xmToggleButtonWidgetClass,
                   cellmatselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, csmatclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut2[0],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 0);

   winw = 180;
   if (numflags_cell > 0) winw = 250;
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cselonbut2[0]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
/*   i++;  XtSetArg(wargs[i],XmNwidth, winw); */
   i++;  XtSetArg(wargs[i],XmNheight, 320);
   i++;
   cellselswin = XmCreateScrolledWindow(cellmatselform,"Cellselswin",wargs,i);
   XtManageChild(cellselswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cswinform = XmCreateForm(cellselswin,"Cswinform",wargs,i);

   /*  Create material selections.  */

   string = XmStringCreate("Materials",XmSTRING_DEFAULT_CHARSET);
   cstxt[0] = XtVaCreateManagedWidget("Cstxt0",
              xmLabelWidgetClass,
              cswinform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   cson[0] = XtVaCreateManagedWidget("Cson0",
             xmPushButtonGadgetClass,
             cswinform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, cstxt[0],
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(cson[0],XmNactivateCallback,csset_on,
                    (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   csoff[0] = XtVaCreateManagedWidget("Csoff0",
              xmPushButtonGadgetClass,
              cswinform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, cstxt[0],
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, cson[0],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(csoff[0],XmNactivateCallback,csset_off,
                    (caddr_t) 0);

   for (i = 0; i < maxcellmat; i++)
      cellmatitem[i] = XmStringLtoRCreate(mmatnames[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cson[0]);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   if (maxcellmat > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, maxcellmat);
      i++;  XtSetArg(wargs[i],XmNitems, cellmatitem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
   i++;
   cmatsellist[0] = (Widget)XmCreateScrolledList(cswinform,
                                                 "Cmatsellist0",wargs,i);
   XtManageChild(cmatsellist[0]);
   for (i = 0; i < maxcellmat; i++) 
      XmStringFree(cellmatitem[i]);

   /*  Create interactive mouse material selection buttons.  */
 
   string = XmStringLtoRCreate("Select Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   cmatselprobe = XtVaCreateManagedWidget("Cmatselprobe",
                  xmToggleButtonGadgetClass,
                  cswinform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cmatsellist[0],
                  XmNtopOffset, 5,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cmatselprobe,XmNvalueChangedCallback,
                    cmatsel_probe,NULL);
 
   string = XmStringLtoRCreate("Remove Material\n From Screen",
                               XmSTRING_DEFAULT_CHARSET);
   cmatremprobe = XtVaCreateManagedWidget("Cmatremprobe",
                  xmToggleButtonGadgetClass,
                  cswinform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cmatselprobe,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cmatremprobe,XmNvalueChangedCallback,
                    cmatrem_probe,NULL);

  
   /*  Create sets of and/or and flag selection buttons.  */
 
   lastleft = cmatsellist[0];
   if (numflags_cell > 0)
     {
      for (j = 0; j < numflags_cell; j++)
        {
         jj = j + 1;
         i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNtopWidget, cson[j]);
         i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNleftWidget, lastleft);
         i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;
         sprintf(str,"Candorbox%d",j);
         candorbox[j] = XmCreateRadioBox(cswinform,str,wargs,i);
         XtManageChild(candorbox[j]);

         string = XmStringCreate("And",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Candbuts%d",j);
         cmatandbut[j] = XtVaCreateManagedWidget(str,
                       xmToggleButtonGadgetClass,
                       candorbox[j],
                       XmNlabelString, string,
                       XmNspacing, 1,
                       XmNselectColor, yellowpix,
                       XmNmarginHeight, 0,
                       XmNmarginWidth, 0,
                       XmNset, TRUE,
                       NULL);
            XmStringFree(string);
            XtAddCallback(cmatandbut[j],XmNvalueChangedCallback,
                          cselect_and_flag,(caddr_t) j);
         string = XmStringCreate("Or",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Corbuts%d",j);
         cmatorbut[j] = XtVaCreateManagedWidget(str,
                      xmToggleButtonGadgetClass,
                      candorbox[j],
                      XmNlabelString, string,
                      XmNspacing, 1,
                      XmNselectColor, yellowpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNset, FALSE,
                      NULL);
            XmStringFree(string);
            XtAddCallback(cmatorbut[j],XmNvalueChangedCallback,
                          cselect_or_flag,(caddr_t) j);

         string = XmStringCreate(flagname_cell[j],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Cstxt%d",jj);
         cstxt[jj] = XtVaCreateManagedWidget(str,
                     xmLabelWidgetClass,
                     cswinform,
                     XmNtopAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, candorbox[j],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string);

         string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Cson%d",jj);
         cson[jj] = XtVaCreateManagedWidget(str,
                    xmPushButtonGadgetClass,
                    cswinform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cstxt[jj],
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, candorbox[j],
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string);
            XtAddCallback(cson[jj],XmNactivateCallback,csset_on,
                          (caddr_t) jj);

         string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Csoff%d",jj);
         csoff[jj] = XtVaCreateManagedWidget(str,
                     xmPushButtonGadgetClass,
                     cswinform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cstxt[jj],
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, cson[jj],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
            XmStringFree(string);
            XtAddCallback(csoff[jj],XmNactivateCallback,csset_off,
                          (caddr_t) jj);

         kk = flagtypes_cell[j];
         for (i = 0; i < kk; i++)
            cellmatitem[i] = XmStringLtoRCreate(flagnames_cell[j][i],
                                                XmSTRING_DEFAULT_CHARSET);

         k = j + 1;
         i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNtopWidget, cson[jj]);
         i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNleftWidget, candorbox[j]);
         i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNheight, 160);
         if (kk > 0)
           {
            i++;  XtSetArg(wargs[i],XmNitemCount, kk);
            i++;  XtSetArg(wargs[i],XmNitems, cellmatitem);
           }
         i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
         i++;
         sprintf(str,"Cmatsellist%d",k);
         cmatsellist[k] = (Widget)XmCreateScrolledList(cswinform,
                                                       str,wargs,i);
         XtManageChild(cmatsellist[k]);
         for (i = 0; i < kk; i++) 
            XmStringFree(cellmatitem[i]);

         lastleft = cmatsellist[k];
        } 
     }
     
   XtManageChild(cswinform);


   /*  Create Cell Field Data Range selection menu.  */
   /**************************************************/

    if (cnumvars > 0)
     {
      string = XmStringCreate("Cell Select - Cell Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cellcfldselform = XmCreateFormDialog(cellselform,"Cellcfldselform",
                                           wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      cscfldclose = XtVaCreateManagedWidget("Cscfldclose",
                    xmPushButtonGadgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cscfldclose,XmNactivateCallback,cselcfldclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      cscfldapply = XtVaCreateManagedWidget("Cscfldapply",
                    xmPushButtonGadgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cscfldclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cscfldapply,XmNactivateCallback,cselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      cselonbut2[1] = XtVaCreateManagedWidget("Cselonbut21",
                      xmToggleButtonWidgetClass,
                      cellcfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cscfldclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(cselonbut2[1],XmNvalueChangedCallback,cselsetonbut,
                       (caddr_t) 1);

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      cselcfldlab1 = XtVaCreateManagedWidget("Cselcfldlab1",
                     xmLabelWidgetClass,
                     cellcfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselonbut2[1],
                     XmNtopOffset, 10,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselcfldlab = XtVaCreateManagedWidget("Cselcfldlab",
                    xmLabelWidgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselonbut2[1],
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselcfldlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      cselcfldbut = XtVaCreateManagedWidget("Cselcfldbut",
                    xmPushButtonGadgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselcfldlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cselcfldbut,XmNactivateCallback,cselcfldfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      cselcminlab1 = XtVaCreateManagedWidget("Cselminlab1",
                     xmLabelWidgetClass,
                     cellcfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselcfldbut,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselcminlab = XtVaCreateManagedWidget("Cselcminlab",
                    xmLabelWidgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselcfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselcminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      cselcmaxlab1 = XtVaCreateManagedWidget("Cselcmaxlab1",
                     xmLabelWidgetClass,
                     cellcfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselcminlab1,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselcmaxlab = XtVaCreateManagedWidget("Cselcmaxlab",
                    xmLabelWidgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselcminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselcmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      cselcrangelab = XtVaCreateManagedWidget("Cselcrangelab",
                      xmLabelWidgetClass,
                      cellcfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cselcmaxlab1,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      cselcminlab2 = XtVaCreateManagedWidget("Cselcminlab2",
                     xmLabelWidgetClass,
                     cellcfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselcrangelab,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      cselcmintxt = XtVaCreateManagedWidget("Cselmintxt",
                   xmTextFieldWidgetClass,
                   cellcfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cselcrangelab,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, cselcminlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      cselcmaxlab2 = XtVaCreateManagedWidget("Cselmaxlab2",
                    xmLabelWidgetClass,
                    cellcfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselcminlab2,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      cselcmaxtxt = XtVaCreateManagedWidget("Cselmaxtxt",
                   xmTextFieldWidgetClass,
                   cellcfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cselcminlab2,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, cselcmaxlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      cselcreset = XtVaCreateManagedWidget("Cselcreset",
                   xmPushButtonGadgetClass,
                   cellcfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cselcmaxtxt,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(cselcreset,XmNactivateCallback,cselcfldreset,
                       NULL);
     }


   /*  Create Node Field Data Range selection menu.  */
   /**************************************************/

    if (numvars > 0)
     {
      string = XmStringCreate("Cell Select - Node Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cellnfldselform = XmCreateFormDialog(cellselform,"Cellnfldselform",
                                           wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      csnfldclose = XtVaCreateManagedWidget("Csnfldclose",
                    xmPushButtonGadgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(csnfldclose,XmNactivateCallback,cselnfldclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      csnfldapply = XtVaCreateManagedWidget("Csnfldapply",
                    xmPushButtonGadgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, csnfldclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(csnfldapply,XmNactivateCallback,cselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      cselonbut2[2] = XtVaCreateManagedWidget("Cselonbut22",
                      xmToggleButtonWidgetClass,
                      cellnfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, csnfldclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(cselonbut2[2],XmNvalueChangedCallback,cselsetonbut,
                       (caddr_t) 2);

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      cselnfldlab1 = XtVaCreateManagedWidget("Cselnfldlab1",
                     xmLabelWidgetClass,
                     cellnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselonbut2[2],
                     XmNtopOffset, 10,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselnfldlab = XtVaCreateManagedWidget("Cselnfldlab",
                    xmLabelWidgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselonbut2[2],
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselnfldlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      cselnfldbut = XtVaCreateManagedWidget("Cselnfldbut",
                    xmPushButtonGadgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnfldlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cselnfldbut,XmNactivateCallback,cselnfldfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      cselnminlab1 = XtVaCreateManagedWidget("Cselnminlab1",
                     xmLabelWidgetClass,
                     cellnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselnfldbut,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselnminlab = XtVaCreateManagedWidget("Cselnminlab",
                    xmLabelWidgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselnminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      cselnmaxlab1 = XtVaCreateManagedWidget("Cselnmaxlab1",
                     xmLabelWidgetClass,
                     cellnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselnminlab1,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      cselnmaxlab = XtVaCreateManagedWidget("Cselnmaxlab",
                    xmLabelWidgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselnmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      cselnrangelab = XtVaCreateManagedWidget("Cselnrangelab",
                      xmLabelWidgetClass,
                      cellnfldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cselnmaxlab1,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      cselnminlab2 = XtVaCreateManagedWidget("Cselnminlab2",
                     xmLabelWidgetClass,
                     cellnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselnrangelab,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      cselnmintxt = XtVaCreateManagedWidget("Cselnmintxt",
                    xmTextFieldWidgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnrangelab,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselnminlab2,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 20,
                    XmNcolumns, 20,
                    XmNvalue, "",
                    NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      cselnmaxlab2 = XtVaCreateManagedWidget("Cselnmaxlab2",
                     xmLabelWidgetClass,
                     cellnfldselform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, cselnminlab2,
                     XmNleftAttachment, XmATTACH_FORM,
                     XmNleftOffset, 20,
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
         XmStringFree(string); 

      cselnmaxtxt = XtVaCreateManagedWidget("Cselnmaxtxt",
                    xmTextFieldWidgetClass,
                    cellnfldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnminlab2,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cselnmaxlab2,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNmarginHeight, 0,
                    XmNmarginWidth, 1,
                    XmNmaxLength, 20,
                    XmNcolumns, 20,
                    XmNvalue, "",
                    NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      cselnreset = XtVaCreateManagedWidget("Cselnreset",
                   xmPushButtonGadgetClass,
                   cellnfldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cselnmaxtxt,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(cselnreset,XmNactivateCallback,cselnfldreset,
                       NULL);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, cselnreset);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      csnfldbox = XmCreateRadioBox(cellnfldselform,"Csnfldbox",wargs,i);
      XtManageChild(csnfldbox);
 
      string = XmStringCreate("Any Nodes",
               XmSTRING_DEFAULT_CHARSET);
      cselnfldany = XtVaCreateManagedWidget("Cselnfldany",
                    xmToggleButtonGadgetClass,
                    csnfldbox,
                    XmNlabelString, string,
                    XmNset, TRUE,
                    XmNspacing, 2,
                    XmNselectColor, yellowpix,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cselnfldany,XmNvalueChangedCallback,
                       cselnfldsetopt,(caddr_t) 0);
 
      string = XmStringCreate("All Nodes",
               XmSTRING_DEFAULT_CHARSET);
      cselnfldall = XtVaCreateManagedWidget("Cselnfldall",
                  xmToggleButtonGadgetClass,
                  csnfldbox,
                  XmNlabelString, string,
                  XmNset, FALSE,
                  XmNspacing, 2,
                  XmNselectColor, yellowpix,
                  NULL);
         XmStringFree(string);
         XtAddCallback(cselnfldall,XmNvalueChangedCallback,
                       cselnfldsetopt,(caddr_t) 1);
     }


   /*  Create Search Sphere selection menu.  */
   /*****************************************/

   string = XmStringCreate("Cell Select - Search Sphere",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellsphselform = XmCreateFormDialog(cellselform,"Cellsphselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   cssphclose = XtVaCreateManagedWidget("Cssphclose",
                xmPushButtonGadgetClass,
                cellsphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cssphclose,XmNactivateCallback,cselsphclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   cssphapply = XtVaCreateManagedWidget("Cssphapply",
                xmPushButtonGadgetClass,
                cellsphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cssphclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cssphapply,XmNactivateCallback,cselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   cselonbut2[3] = XtVaCreateManagedWidget("Cselonbut23",
                   xmToggleButtonWidgetClass,
                   cellsphselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cssphclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut2[3],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 3);

   string = XmStringCreate("X:",XmSTRING_DEFAULT_CHARSET);
   cssphxlab = XtVaCreateManagedWidget("Cssphxlab",
               xmLabelWidgetClass,
               cellsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cselonbut2[3],
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   cselsphxtxt = XtVaCreateManagedWidget("Cselsphxtxt",
                 xmTextFieldWidgetClass,
                 cellsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cselonbut2[3],
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cssphxlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Y:",XmSTRING_DEFAULT_CHARSET);
   cssphylab = XtVaCreateManagedWidget("Cssphylab",
               xmLabelWidgetClass,
               cellsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cssphxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   cselsphytxt = XtVaCreateManagedWidget("Cselsphytxt",
                 xmTextFieldWidgetClass,
                 cellsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cssphxlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cssphylab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Z:",XmSTRING_DEFAULT_CHARSET);
   cssphzlab = XtVaCreateManagedWidget("Cssphzlab",
               xmLabelWidgetClass,
               cellsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cssphylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   cselsphztxt = XtVaCreateManagedWidget("Cselsphztxt",
                 xmTextFieldWidgetClass,
                 cellsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cssphylab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cssphzlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("R:",XmSTRING_DEFAULT_CHARSET);
   cssphrlab = XtVaCreateManagedWidget("Cssphrlab",
               xmLabelWidgetClass,
               cellsphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, cssphzlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   cselsphrtxt = XtVaCreateManagedWidget("Cselsphrtxt",
                 xmTextFieldWidgetClass,
                 cellsphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, cssphzlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, cssphrlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cselsphrtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   cssphbox = XmCreateRadioBox(cellsphselform,"Cssphbox",wargs,i);
   XtManageChild(cssphbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   cselsphin = XtVaCreateManagedWidget("Cselsphin",
               xmToggleButtonGadgetClass,
               cssphbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(cselsphin,XmNvalueChangedCallback,
                   cselsphsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   cselsphout = XtVaCreateManagedWidget("Cselsphout",
                xmToggleButtonGadgetClass,
                cssphbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(cselsphout,XmNvalueChangedCallback,
                   cselsphsetinopt,(caddr_t) 0);


   /*  Create Search Box selection menu.  */
   /***************************************/

   string = XmStringCreate("Cell Select - Search Box",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellboxselform = XmCreateFormDialog(cellselform,"Cellboxselform",
                                       wargs,i);
      XmStringFree(string);

   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csboxclose = XtVaCreateManagedWidget("Csboxclose",
                xmPushButtonGadgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csboxclose,XmNactivateCallback,cselboxclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   csboxapply = XtVaCreateManagedWidget("Csboxapply",
                xmPushButtonGadgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, csboxclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csboxapply,XmNactivateCallback,cselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   cselonbut2[5] = XtVaCreateManagedWidget("Cselonbut25",
                   xmToggleButtonWidgetClass,
                   cellboxselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, csboxclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut2[5],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 5);

   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   csboxx1lab = XtVaCreateManagedWidget("Csboxx1lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselonbut2[5],
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxx1sl = XtVaCreateManagedWidget("Cselboxx1sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxx1lab,
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
      XtAddCallback(cselboxx1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 0);
      XtAddCallback(cselboxx1sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 6);

   cselboxx1txt = XtVaCreateManagedWidget("Cselboxx1txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxx1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxx1txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   csboxx2lab = XtVaCreateManagedWidget("Csboxx2lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselonbut2[5],
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cselboxx1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxx2sl = XtVaCreateManagedWidget("Cselboxx2sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxx2lab,
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
      XtAddCallback(cselboxx2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 1);
      XtAddCallback(cselboxx2sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 7);

   cselboxx2txt = XtVaCreateManagedWidget("Cselboxx2txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxx2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxx2txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   csboxy1lab = XtVaCreateManagedWidget("Csboxy1lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselboxx1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxy1sl = XtVaCreateManagedWidget("Cselboxy1sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxy1lab,
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
      XtAddCallback(cselboxy1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 2);
      XtAddCallback(cselboxy1sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 8);

   cselboxy1txt = XtVaCreateManagedWidget("Cselboxy1txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxy1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxy1txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   csboxy2lab = XtVaCreateManagedWidget("Csboxy2lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselboxx1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cselboxy1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxy2sl = XtVaCreateManagedWidget("Cselboxy2sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxy2lab,
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
      XtAddCallback(cselboxy2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 3);
      XtAddCallback(cselboxy2sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 9);

   cselboxy2txt = XtVaCreateManagedWidget("Cselboxy2txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxy2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxy2txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   csboxz1lab = XtVaCreateManagedWidget("Csboxz1lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselboxy1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxz1sl = XtVaCreateManagedWidget("Cselboxz1sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxz1lab,
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
      XtAddCallback(cselboxz1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 4);
      XtAddCallback(cselboxz1sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 10);

   cselboxz1txt = XtVaCreateManagedWidget("Cselboxz1txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxz1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxz1txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   csboxz2lab = XtVaCreateManagedWidget("Csboxz2lab",
                xmLabelWidgetClass,
                cellboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cselboxy1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, cselboxz1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   cselboxz2sl = XtVaCreateManagedWidget("Cselboxz2sl",
                 xmScaleWidgetClass,
                 cellboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, csboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, csboxz2lab,
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
      XtAddCallback(cselboxz2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 5);
      XtAddCallback(cselboxz2sl,XmNdragCallback,csboxsubsetsl,
                    (caddr_t) 11);

   cselboxz2txt = XtVaCreateManagedWidget("Cselboxz2txt",
                  xmTextFieldWidgetClass,
                  cellboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, csboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cselboxz2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(cselboxz2txt,XmNactivateCallback,
                    cselboxtxt,(caddr_t) 5);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cselboxz1txt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   csboxbox = XmCreateRadioBox(cellboxselform,"Csboxbox",wargs,i);
   XtManageChild(csboxbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   cselboxin = XtVaCreateManagedWidget("Cselboxin",
               xmToggleButtonGadgetClass,
               csboxbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(cselboxin,XmNvalueChangedCallback,
                   cselboxsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   cselboxout = XtVaCreateManagedWidget("Cselboxout",
                xmToggleButtonGadgetClass,
                csboxbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(cselboxout,XmNvalueChangedCallback,
                   cselboxsetinopt,(caddr_t) 0);


   /*  Create Cell Number selection menu.  */
   /****************************************/

   string = XmStringCreate("Cell Select - Cell Number",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   cellnumselform = XmCreateFormDialog(cellselform,"Cellnumselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   csnumclose = XtVaCreateManagedWidget("Csnumclose",
                xmPushButtonGadgetClass,
                cellnumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csnumclose,XmNactivateCallback,cselnumclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   csnumapply = XtVaCreateManagedWidget("Csnumapply",
                xmPushButtonGadgetClass,
                cellnumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, csnumclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(csnumapply,XmNactivateCallback,cselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   cselonbut2[4] = XtVaCreateManagedWidget("Cselonbut24",
                   xmToggleButtonWidgetClass,
                   cellnumselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, csnumclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(cselonbut2[4],XmNvalueChangedCallback,cselsetonbut,
                    (caddr_t) 4);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, cselonbut2[4]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 150);
   i++;  XtSetArg(wargs[i],XmNheight, 250);
   i++;
   csnumwin = XmCreateScrolledWindow(cellnumselform,"Csnumwin",wargs,i);
   XtManageChild(csnumwin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   csnumform = XmCreateForm(csnumwin,"Csnumform",wargs,i);

   string = XmStringCreate(" 1",XmSTRING_DEFAULT_CHARSET);
   csnumlab[0] = XtVaCreateManagedWidget("Csnumlab0",
                 xmLabelWidgetClass,
                 csnumform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   cselnumtxt[0] = XtVaCreateManagedWidget("Cselnumtxt0",
                   xmTextFieldWidgetClass,
                   csnumform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, csnumlab[0],
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
      sprintf(str,"Csnumlab%d",i);
      csnumlab[i] = XtVaCreateManagedWidget("str",
                    xmLabelWidgetClass,
                    csnumform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cselnumtxt[i-1],
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string); 

      sprintf(str,"Cselnumtxt%d",i);
      cselnumtxt[i] = XtVaCreateManagedWidget(str,
                      xmTextFieldWidgetClass,
                      csnumform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cselnumtxt[i-1],
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, csnumlab[i],
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
   csnumclrbut = XtVaCreateManagedWidget("Csnumclrbut",
                 xmPushButtonGadgetClass,
                 cellnumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, csnumwin,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(csnumclrbut,XmNactivateCallback,csnumclr,
                    (caddr_t) 0);

   XtManageChild(csnumform);


   /*  Create Cell Groups selection menu.  */
   /************************************************/

   if (ncellgrps > 0 || physedflag)
     {
      string = XmStringCreate("Cell Select - Cell Groups",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cellgrpselform = XmCreateFormDialog(cellselform,"Cellgrpselform",wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      cscgrpclose = XtVaCreateManagedWidget("Cscgrpclose",
                    xmPushButtonGadgetClass,
                    cellgrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cscgrpclose,XmNactivateCallback,cellgrpclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      cscgrpapply = XtVaCreateManagedWidget("Cscgrpapply",
                    xmPushButtonGadgetClass,
                    cellgrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cscgrpclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(cscgrpapply,XmNactivateCallback,cselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      cselonbut2[6] = XtVaCreateManagedWidget("Cselonbut26",
                      xmToggleButtonWidgetClass,
                      cellgrpselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, cscgrpclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(cselonbut2[6],XmNvalueChangedCallback,cselsetonbut,
                       (caddr_t) 6);

      for (i = 0; i < ncellgrps; i++)
         cellgrpitem[i] = XmStringLtoRCreate(cellgrpname[i],
                                             XmSTRING_DEFAULT_CHARSET);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, cselonbut2[6]);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNheight, 250);
      if (ncellgrps > 0)
        {
         i++;  XtSetArg(wargs[i],XmNitemCount, ncellgrps);
         i++;  XtSetArg(wargs[i],XmNitems, cellgrpitem);
        }
      i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmEXTENDED_SELECT);
      i++;
      cellgrplist = (Widget)XmCreateScrolledList(cellgrpselform,
                                                 "Cellgrplist",wargs,i);
      XtManageChild(cellgrplist);
      for (i = 0; i < ncellgrps; i++) 
         XmStringFree(cellgrpitem[i]);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, cellgrplist);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      csgrpbox = XmCreateRadioBox(cellgrpselform,"Csgrpbox",wargs,i);
      XtManageChild(csgrpbox);

      string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
      cselgrpin = XtVaCreateManagedWidget("Cselgrpin",
                  xmToggleButtonGadgetClass,
                  csgrpbox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
        XmStringFree(string);
        XtAddCallback(cselgrpin,XmNvalueChangedCallback,
                      cselgrpsetinopt,(caddr_t) 0);

      string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
      cselgrpout = XtVaCreateManagedWidget("Cselgrpout",
                   xmToggleButtonGadgetClass,
                   csgrpbox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
        XmStringFree(string);
        XtAddCallback(cselgrpout,XmNvalueChangedCallback,
                      cselgrpsetinopt,(caddr_t) 0);
     }


   /*  Create Face Groups selection menu.  */
   /************************************************/

   if (nfacegrps > 0)
     {
      string = XmStringCreate("Cell Select - Face Groups",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      facegrpselform = XmCreateFormDialog(cellselform,"Facegrpselform",wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      csfgrpclose = XtVaCreateManagedWidget("Csfgrpclose",
                    xmPushButtonGadgetClass,
                    facegrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(csfgrpclose,XmNactivateCallback,facegrpclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      csfgrpapply = XtVaCreateManagedWidget("Csfgrpapply",
                    xmPushButtonGadgetClass,
                    facegrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, csfgrpclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(csfgrpapply,XmNactivateCallback,cselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      cselonbut2[7] = XtVaCreateManagedWidget("Cselonbut27",
                      xmToggleButtonWidgetClass,
                      facegrpselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, csfgrpclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(cselonbut2[7],XmNvalueChangedCallback,cselsetonbut,
                       (caddr_t) 7);

      for (i = 0; i < nfacegrps; i++)
         facegrpitem[i] = XmStringLtoRCreate(facegrpname[i],
                                             XmSTRING_DEFAULT_CHARSET);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, cselonbut2[7]);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNheight, 250);
      i++;  XtSetArg(wargs[i],XmNitemCount, nfacegrps);
      i++;  XtSetArg(wargs[i],XmNitems, facegrpitem);
      i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmEXTENDED_SELECT);
      i++;
      facegrplist = (Widget)XmCreateScrolledList(facegrpselform,
                                                 "Facegrplist",wargs,i);
      XtManageChild(facegrplist);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, facegrplist);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      fsgrpbox = XmCreateRadioBox(facegrpselform,"Fsgrpbox",wargs,i);
      XtManageChild(fsgrpbox);

      string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
      fselgrpin = XtVaCreateManagedWidget("Fselgrpin",
                  xmToggleButtonGadgetClass,
                  fsgrpbox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
        XmStringFree(string);
        XtAddCallback(fselgrpin,XmNvalueChangedCallback,
                      fselgrpsetinopt,(caddr_t) 0);

      string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
      fselgrpout = XtVaCreateManagedWidget("Fselgrpout",
                   xmToggleButtonGadgetClass,
                   fsgrpbox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
        XmStringFree(string);
        XtAddCallback(fselgrpout,XmNvalueChangedCallback,
                      fselgrpsetinopt,(caddr_t) 0);
     }
}


void setcellsel()
{
  int i, j, k, l, nf, jcell, ii, jj, id, igrp, iface, jface;
  long ifirst, ilast, istride;
  short isel, jsel, nseln;
  double rad, dist, xd, yd, zd;
  double *cvalue, *nvalue, xb, yb, zb;
  double * readcellvalue(int ifld);
  double * readnodevalue(int ifld);

   /*  Allocate array and read field(s) for range selection, if needed.  */
   if (cellselcfld_on)
      cvalue = readcellvalue(cellselfield);
   if (cellselnfld_on)
      nvalue = readnodevalue(cellselnfield);

   /*  For search sphere test, square the sphere radius.  */
   rad = (double)cellselsph_r * (double)cellselsph_r;

   /*  Loop through cells and set selections.  */
   for (i = 0; i < ncells; i++)
     {
      cellselectflag[i] = 1;

      /*  Set material selections first.  */
      if (cellselmat_on)
        {
         cellselectflag[i] = 0;

         for (k = 0; k < maxcellmat; k++)
            if (cellmats[i]-1 == k && cellselflag[0][k] == 1)
                cellselectflag[i] = 1;
         if (maxcellmat < 1) cellselectflag[i] = 1;

         /*  Add cells with a 0 material no.  */
         if (cellmats[i] < 1) cellselectflag[i] = 1;

         /*  Now check selection flags.  */
         for (j = 1; j <= numflags_cell; j++)
           {
            nf = flagtypes_cell[j-1];
            jsel = 0;
            for (k = 0; k < nf; k++)
               if (cellflags[j-1][i]-1 == k && cellselflag[j][k] == 1)
                  jsel = 1;

            /*  And/or with previous select flag.  */
            isel = cellselectflag[i];
            cellselectflag[i] = 0;
            if (cellandorflag[j-1] == 0)
               if (isel == 1 && jsel == 1) cellselectflag[i] = 1;
            if (cellandorflag[j-1] == 1)
               if (isel == 1 || jsel == 1) cellselectflag[i] = 1;
           }
        }

      /*  Now check cell field range, if requested.  */
      isel = (int)cellselectflag[i];
      if (cellselcfld_on && isel)
        {
         jsel = 0;
         if (cfldselmin <= cvalue[i] && cvalue[i] <= cfldselmax)
            jsel = 1;

         cellselectflag[i] = jsel;
        }

      /*  If node field range or search sphere requested,  */
      /*  and structered grid, set cell data.              */
      jcell = i;
      if (structuredflag == 1 && (cellselnfld_on || cellselsph_on ||
          cellselbox_on))
        {
         jcell = 0;
         fillstructcell(i,0);
        }
      
      /*  Now check node field range, if requested.  */
      isel = (int)cellselectflag[i];
      if (cellselnfld_on && isel)
        {
         jsel = 0;
         nseln = 0;

         /*  Loop through cell nodes.  */
         for (j = 0; j < cells[jcell].ncnodes; j++)
           {
            k = cells[jcell].cnodes[j];
            if (cnfldselmin <= nvalue[k] && nvalue[k] <= cnfldselmax)
               nseln++;
           }
         if (cellselnfldopt == 0 && nseln > 0)
            jsel = 1;
         if (cellselnfldopt == 1 && nseln == cells[jcell].ncnodes) 
            jsel = 1;

         cellselectflag[i] = jsel;
        }

      /*  Now check search sphere, if requested.  */
      isel = (int)cellselectflag[i];
      if (cellselsph_on && isel)
        {
         jsel = 0;

         xd = (double)cells[jcell].xavg - (double)cellselsph_x;
         yd = (double)cells[jcell].yavg - (double)cellselsph_y;
         zd = (double)cells[jcell].zavg - (double)cellselsph_z;
         dist = xd*xd + yd*yd + zd*zd;
         if (cellselsph_in == 1 && dist <= rad)
            jsel = 1;
         if (cellselsph_in == 0 && dist >= rad)
            jsel = 1;

         cellselectflag[i] = jsel;
        }

      /*  Now check search box, if requested.  */
      isel = (int)cellselectflag[i];
      if (cellselbox_on && isel)
        {
         jsel = 0;

         xb = cells[jcell].xavg;
         yb = cells[jcell].yavg;
         zb = cells[jcell].zavg;

         if (cellselbox_in == 1)
           {
            if (cellselbox_xmin <= xb && xb <= cellselbox_xmax &&
                cellselbox_ymin <= yb && yb <= cellselbox_ymax &&
                cellselbox_zmin <= zb && zb <= cellselbox_zmax)
              jsel = 1;
           }
         if (cellselbox_in == 0)
           {
            if (xb <= cellselbox_xmin || cellselbox_xmax <= xb ||
                yb <= cellselbox_ymin || cellselbox_ymax <= yb ||
                zb <= cellselbox_zmin || cellselbox_zmax <= zb)
               jsel = 1;
           }

         cellselectflag[i] = jsel;
        }
    }

   if (cellselcfld_on) free(cvalue);
   if (cellselnfld_on) free(nvalue);

   /*  Check cell number selection, and with  */
   /*  current select flag for those cells.   */
   if (cellselnum_on)
     {

      /*  Loop through cells, and get a cell id.  */
      for (ii = 0; ii < ncells; ii++)
        {
         id = ii + 1;
         if (cellidflag) id = cellids[ii];

         /*  Loop through active first, last, stride sets.  */
         for (j = 0; j < ncellselnum; j++)
           {
            ifirst = cellselnum_f[j];
            ilast = cellselnum_l[j];
            istride = cellselnum_s[j];
            if (ifirst <= id && id <= ilast)
              {
               for (i = ifirst; i <= ilast; i+=istride)
                 {
                  if (i == id)
                    {
                     isel = (int)cellselectflag[ii];

                     /*  If previously selected, set value to 2.  */
                     if (isel) cellselectflag[ii] = 2;
                    }
                 }
              }
           }
        }

      /*  Now loop through cells, and keep those with a 2.  */
      for (i = 0; i < ncells; i++)
        {
         jsel = 0;
         isel = (int)cellselectflag[i];
         if (isel == 2) jsel = 1;
         cellselectflag[i] = jsel;
        }
     }

   /*  Check cell group selection, and with   */
   /*  current select flag for those cells.   */
   if (cellselcellgrp_on && ncellgrpselected > 0)
     {

      /*  Loop through cells, and get a cell id.  */
      for (ii = 0; ii < ncells; ii++)
        {
         id = ii + 1;
         if (cellidflag) id = cellids[ii];

         /*  If previously selected, loop through selected  */ 
         /*  cell groups and set selected cell to 2.        */
         isel = (int)cellselectflag[ii];
         if (isel)
           {
            if (cellselgrp_in == 0) cellselectflag[ii] = 2;
            for (j = 0; j < ncellgrpselected; j++)
              {
               igrp = cellgrpselected[j] - 1;
               k = numcellgrp[igrp];
               for (i = 0; i < k; i++)
                 {
                  jj = cellgrps[igrp][i];
                  if (jj == id)
                    {
                     if (cellselgrp_in == 1) cellselectflag[ii] = 2;
                     if (cellselgrp_in == 0) cellselectflag[ii] = 0;
                    }
                 }
              }
           }
        }

      /*  Now loop through cells, and keep those with a 2.  */
      for (i = 0; i < ncells; i++)
        {
         jsel = 0;
         isel = (int)cellselectflag[i];
         if (isel == 2) jsel = 1;
         cellselectflag[i] = jsel;
        }
     }

   /*  Check face group selection, and with   */
   /*  current select flag for those cells.   */
   if (cellselfacegrp_on && nfacegrpselected > 0)
     {

      /*  Loop through selected cells.  */
      for (ii = 0; ii < ncells; ii++)
        {

         if ((int)cellselectflag[ii])
           {

            /*  If structured flag, fill cell.  */
            jcell = ii;
            if (structuredflag == 1)
              {
               jcell = 0;
               fillstructcell(ii,1);
              }

            /*  Loop through cell faces.  */
            isel = 0;
            for (jj = 0; jj < cells[jcell].nfaces; jj++)
              {
               if (isel) break;
               iface = cells[jcell].facenos[jj];
               id = iface + 1;
               if (faceidflag) id = faceids[iface];

               /*  Loop through selected face groups.  */
               for (j = 0; j < nfacegrpselected; j++)
                 {
                  if (isel) break;
                  igrp = facegrpselected[j] - 1;
                  k = numfacegrp[igrp];
                  for (i = 0; i < k; i++)
                    {
                     if (isel) break;
                     jface = facegrps[igrp][i];
                     if (jface == id) isel = 1;
                    }
                 }
              }
            if (isel)
              {
               if (faceselgrp_in == 0) isel = 0;
              }
            cellselectflag[ii] = isel;
           }
        }
     }

   /*  Check ghost cell options.   */
   if (cellghostflag) drawghostcelloptions();
}


void cellselgetcfield(int i)
{
  char str[20];
  XmString string;

   cellselfield = i;

   string = XmStringCreate(cfieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselcfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",cfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselcminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",cfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselcmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cfldselmin = cfieldmin[i];
   cfldselmax = cfieldmax[i];
   sprintf(str,"%.11g",cfieldmin[i]);
   XmTextFieldSetString(cselcmintxt,str);
   sprintf(str,"%.11g",cfieldmax[i]);
   XmTextFieldSetString(cselcmaxtxt,str);
}


void cellselgetnfield(int i)
{
  char str[20];
  XmString string;

   cellselnfield = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselnfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselnminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cselnmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cnfldselmin = fieldmin[i];
   cnfldselmax = fieldmax[i];
   sprintf(str,"%.11g",fieldmin[i]);
   XmTextFieldSetString(cselnmintxt,str);
   sprintf(str,"%.11g",fieldmax[i]);
   XmTextFieldSetString(cselnmaxtxt,str);
}


void setcellselboxwidgets()
{
  int imin, imax, ival, iscale;
  float val;
  char str[20];

   XtRemoveCallback(cselboxx1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 0);
   XtRemoveCallback(cselboxx2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 1);
   XtRemoveCallback(cselboxy1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 2);
   XtRemoveCallback(cselboxy2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 3);
   XtRemoveCallback(cselboxz1sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 4);
   XtRemoveCallback(cselboxz2sl,XmNvalueChangedCallback,csboxsubsetsl,
                    (caddr_t) 5);
   XFlush(dpy);

   sprintf(str,"%.11g",cellselbox_xmin);
   XmTextFieldSetString(cselboxx1txt,str);
   val = (cellselbox_xmin - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxx1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",cellselbox_xmax);
   XmTextFieldSetString(cselboxx2txt,str);
   val = (cellselbox_xmax - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxx2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",cellselbox_ymin);
   XmTextFieldSetString(cselboxy1txt,str);
   val = (cellselbox_ymin - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxy1sl,XmNvalue,ival, NULL);

   sprintf(str,"%.11g",cellselbox_ymax);
   XmTextFieldSetString(cselboxy2txt,str);
   val = (cellselbox_ymax - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxy2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",cellselbox_zmin);
   XmTextFieldSetString(cselboxz1txt,str);
   val = (cellselbox_zmin - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxz1sl,XmNvalue,ival, NULL);

   sprintf(str,"%.11g",cellselbox_zmax);
   XmTextFieldSetString(cselboxz2txt,str);
   val = (cellselbox_zmax - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(cselboxz2sl, XmNvalue,ival, NULL);

   XFlush(dpy);
   XtAddCallback(cselboxx1sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 0);
   XtAddCallback(cselboxx2sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 1);
   XtAddCallback(cselboxy1sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 2);
   XtAddCallback(cselboxy2sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 3);
   XtAddCallback(cselboxz1sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 4);
   XtAddCallback(cselboxz2sl,XmNvalueChangedCallback,csboxsubsetsl,(caddr_t) 5);
   XFlush(dpy);
}


void setcellselwidgets()
{
  char str[20];

   setcellselboxwidgets();

   sprintf(str,"%.11g",cellselsph_x);
   XmTextFieldSetString(cselsphxtxt,str);

   sprintf(str,"%.11g",cellselsph_y);
   XmTextFieldSetString(cselsphytxt,str);

   sprintf(str,"%.11g",cellselsph_z);
   XmTextFieldSetString(cselsphztxt,str);

   sprintf(str,"%.11g",cellselsph_r);
   XmTextFieldSetString(cselsphrtxt,str);
}


void addcellselgrplist(int i)
{
  XmString item;

   item = XmStringLtoRCreate(cellgrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(cellgrplist,item,0);
}


void setcellselgrplist(int i)
{
  XmString item[2];

   item[0] = XmStringLtoRCreate(cellgrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(cellgrplist,item,1,i+1);
}
