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

#define NSELON 6
static Widget nodematselform, nodefldselform, nodesphselform,
              nodenumselform, nselonbut[NSELON], nselonbut2[NSELON],
              nodeboxselform, nodegrpselform;
static Widget nsellist[MAXFLAGS+1], nandbuts[MAXFLAGS+1],
              norbuts[MAXFLAGS+1], nselfldlab, nselminlab, nselmaxlab,
              nselmintxt, nselmaxtxt, nselerror, nselsphxtxt,
              nselsphytxt, nselsphztxt, nselsphrtxt, nselnumtxt[MAXNUMSEL],
              nselsphin, nselsphout, nselboxx1txt, nselboxy1txt, 
              nselboxz1txt, nselboxin, nselboxx2txt, nselboxy2txt, 
              nselboxz2txt, nselboxout, nselboxx1sl, nselboxx2sl, 
              nselboxy1sl, nselboxy2sl, nselboxz1sl, nselboxz2sl,
              nodegrplist, nselgrpin, nselgrpout;
static int fastboxcount = 0;


void nodeselformpop()
{
   XmToggleButtonSetState(nselonbut[0],nodeselmat_on,FALSE);
   XmToggleButtonSetState(nselonbut[1],nodeselfld_on,FALSE);
   XmToggleButtonSetState(nselonbut[2],nodeselsph_on,FALSE);
   XmToggleButtonSetState(nselonbut[3],nodeselnum_on,FALSE);
   XmToggleButtonSetState(nselonbut[4],nodeselbox_on,FALSE);
   XmToggleButtonSetState(nselonbut[5],nodeselnodegrp_on,FALSE);

   MyManageChild(nodeselform);
}


void nodematselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(nselonbut2[0],nodeselmat_on,FALSE);

   for (i = 0; i <= numflags_node; i++)
     {
      if (i == 0) k = maxcellmat;
      else k = flagtypes_node[i-1];
      XmListDeselectAllItems(nsellist[i]);
      for (j = 0; j < k; j++)
        {
         if (nodeselflag[i][j] == 1)
            XmListSelectPos(nsellist[i],j+1,FALSE);
        }
     }

   if (numflags_node > 0)
     {
      for (i = 0; i < numflags_node; i++)
        {
         if (nodeandorflag[i] == 0)
           {
            XmToggleButtonGadgetSetState(nandbuts[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(norbuts[i],FALSE,FALSE);
           }
         if (nodeandorflag[i] == 1)
           {
            XmToggleButtonGadgetSetState(norbuts[i],TRUE,FALSE);
            XmToggleButtonGadgetSetState(nandbuts[i],FALSE,FALSE);
           }
        }
     }

   /*  Calculate material pop-up postition from widgets.  */
   XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
   fx = wx;
   fy = wy + 50;
   XtVaSetValues(nodematselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(nodematselform);
}


void nodefldselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];
  XmString string;
  Position  wx, wy, fx, fy, p0 = 0;

   if (numvars > 0)
     {
      XmToggleButtonSetState(nselonbut2[1],nodeselfld_on,FALSE);

      i = nodeselfield;

      string = XmStringCreate(fieldname[i],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nselfldlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      sprintf(str,"%.11g",fieldmin[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nselminlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      sprintf(str,"%.11g",fieldmax[i]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nselmaxlab,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (nfldselmin == 0. && nfldselmax == 0.)
        {
         nfldselmin = fieldmin[i];
         nfldselmax = fieldmax[i];
        }
      sprintf(str,"%.11g",nfldselmin);
      XtVaSetValues(nselmintxt,XmNvalue,str,NULL);
      sprintf(str,"%.11g",nfldselmax);
      XtVaSetValues(nselmaxtxt,XmNvalue,str,NULL);

      /*  Calculate node range pop-up postition from widgets.  */
      XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
      fx = wx + 20;
      fy = wy + 50;
      XtVaSetValues(nodefldselform, XmNx,fx, XmNy,fy, NULL);

      MyManageChild(nodefldselform);
     }
}


void nodesphselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char str[20];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(nselonbut2[2],nodeselsph_on,FALSE);

   sprintf(str,"%.11g",nodeselsph_x);
   XmTextFieldSetString(nselsphxtxt,str);

   sprintf(str,"%.11g",nodeselsph_y);
   XmTextFieldSetString(nselsphytxt,str);

   sprintf(str,"%.11g",nodeselsph_z);
   XmTextFieldSetString(nselsphztxt,str);

   sprintf(str,"%.11g",nodeselsph_r);
   XmTextFieldSetString(nselsphrtxt,str);

   if (nodeselsph_in)
     {
      XmToggleButtonGadgetSetState(nselsphin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(nselsphout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(nselsphin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(nselsphout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
   fx = wx + 40;
   fy = wy + 50;
   XtVaSetValues(nodesphselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(nodesphselform);
}


void nodenumselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[30];
  Position  wx, wy, fx, fy, p0 = 0;

   XmToggleButtonSetState(nselonbut2[3],nodeselnum_on,FALSE);

   /*  Fill text lines with node number select data.  */
   if (nnodeselnum > 0)
     {
      for (i = 0; i < nnodeselnum; i++)
        {
         if (nodeselnum_f[i] == nodeselnum_l[i])
            sprintf(str,"%ld",nodeselnum_f[i]);
         else if (nodeselnum_s[i] == 1)
            sprintf(str,"%ld:%ld",nodeselnum_f[i],nodeselnum_l[i]);
         else
            sprintf(str,"%ld:%ld:%ld",nodeselnum_f[i],nodeselnum_l[i],
                                   nodeselnum_s[i]);
         XmTextFieldSetString(nselnumtxt[i],str);
        }
      for (i = nnodeselnum; i < MAXNUMSEL; i++)
         XmTextFieldSetString(nselnumtxt[i],"");
     }

   /*  Calculate number pop-up postition from widgets.  */
   XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(nodenumselform, XmNx,fx, XmNy,fy, NULL);
   XFlush(dpy);

   MyManageChild(nodenumselform);
}


void nodeboxselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;
  void setnodeselboxwidgets();

   XmToggleButtonSetState(nselonbut2[4],nodeselbox_on,FALSE);

   setnodeselboxwidgets();

   if (nodeselbox_in)
     {
      XmToggleButtonGadgetSetState(nselboxin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(nselboxout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(nselboxin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(nselboxout,TRUE,FALSE); 
     }

   /*  Calculate sphere pop-up postition from widgets.  */
   XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
   fx = wx + 60;
   fy = wy + 50;
   XtVaSetValues(nodeboxselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(nodeboxselform);
}


void nodegrpselformpop(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;
  Position  wx, wy, fx, fy, p0 = 0;

   if (nodeselgrp_in)
     {
      XmToggleButtonGadgetSetState(nselgrpin,TRUE,FALSE);         
      XmToggleButtonGadgetSetState(nselgrpout,FALSE,FALSE);                
     }
   else
     {
      XmToggleButtonGadgetSetState(nselgrpin,FALSE,FALSE);         
      XmToggleButtonGadgetSetState(nselgrpout,TRUE,FALSE); 
     }

   XmToggleButtonSetState(nselonbut2[5],nodeselnodegrp_on,FALSE);
   XmListDeselectAllItems(nodegrplist);

   for (j = 0; j < nnodegrpselected; j++)
     {
      i = nodegrpselected[j];
      XmListSelectPos(nodegrplist,i,FALSE);
     }

   /*  Calculate node group pop-up postition from widgets.  */
   XtTranslateCoords(nselerror, p0,p0, &wx,&wy); 
   fx = wx + 30;
   fy = wy + 60;
   XtVaSetValues(nodegrpselform, XmNx,fx, XmNy,fy, NULL);

   MyManageChild(nodegrpselform);
}


void nselmatclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodematselform);
}


void nselfldclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfieldform);
   XtUnmanageChild(nodefldselform);
}


void nselsphclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodesphselform);
}


void nselnumclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodenumselform);
}


void nselboxclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   nodeselbox_draw = 0;
   expose_redraw();
   XtUnmanageChild(nodeboxselform);
}


void nodegrpclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodegrpselform);
}


void nselclose(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nodematselform);
   if (numvars > 0)
      nselfldclose(NULL,NULL,NULL);
   XtUnmanageChild(nodesphselform);
   XtUnmanageChild(nodenumselform);
   nselboxclose(NULL,NULL,NULL);
   if (nnodegrps > 0 || physedflag)
      XtUnmanageChild(nodegrpselform);

   XtUnmanageChild(nodeselform);
}


void nselsphsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   nodeselsph_in = (short)XmToggleButtonGadgetGetState(nselsphin);
}


void nselboxsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   nodeselbox_in = (short)XmToggleButtonGadgetGetState(nselboxin);
}


void nselgrpsetinopt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   nodeselgrp_in = (short)XmToggleButtonGadgetGetState(nselgrpin);
}


void nselsetonbut(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      nodeselmat_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[0],nodeselmat_on,FALSE);
      XmToggleButtonSetState(nselonbut2[0],nodeselmat_on,FALSE);
     }
   if (arg == 1)
     {
      nodeselfld_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[1],nodeselfld_on,FALSE);
      XmToggleButtonSetState(nselonbut2[1],nodeselfld_on,FALSE);
     }
   if (arg == 2)
     {
      nodeselsph_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[2],nodeselsph_on,FALSE);
      XmToggleButtonSetState(nselonbut2[2],nodeselsph_on,FALSE);
     }
   if (arg == 3)
     {
      nodeselnum_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[3],nodeselnum_on,FALSE);
      XmToggleButtonSetState(nselonbut2[3],nodeselnum_on,FALSE);
     }
   if (arg == 4)
     {
      nodeselbox_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[4],nodeselbox_on,FALSE);
      XmToggleButtonSetState(nselonbut2[4],nodeselbox_on,FALSE);
     }
   if (arg == 5)
     {
      nodeselnodegrp_on = (short)XmToggleButtonGetState(w);
      XmToggleButtonSetState(nselonbut[5],nodeselnodegrp_on,FALSE);
      XmToggleButtonSetState(nselonbut2[5],nodeselnodegrp_on,FALSE);
     }
}


void nselapply(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k, ierr, tmpstrlen, *grpselected, *nselected, nlist;
  unsigned long num;
  char *tmpstr;
  float x1,y1,z1, x2,y2, z2;
  static char *token;
  static char *str1 = 
           "Error - minimum must be less than maximum.";
  static char *str2 = 
           "Error - search sphere radius must be greater than 0.";
  static char str4[55]; 
  XmString string;
  char *chk_num = "0123456789: ", *chk_blank = " ";
  void setnodesel();

   /*  Get and set the node material and node flags selection.  */
   for (i = 0; i <= numflags_node; i++)
     {
      j = XmListGetSelectedPos(nsellist[i],&nselected,&nlist);
      if (!j) nlist = 0;
      for (j = 0; j < MAXMATS; j++) nodeselflag[i][j] = 0;
      for (j = 0; j < nlist; j++)
        { 
         k = nselected[j] - 1;
         nodeselflag[i][k] = 1;
        }
      FREE(nselected);
     }

   /*  Read and check the range values.  */
   nodeselbox_draw = 0;
   ierr = 0;
   if (numvars > 0 && nodeselfld_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(nselmintxt);
      nfldselmin = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselmaxtxt);
      nfldselmax = atof(tmpstr);
      XtFree(tmpstr);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nselerror,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (nfldselmin > nfldselmax)
        {
         ierr = 1;
         string = XmStringCreate(str1,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(nselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check the search sphere values.  */
   if (nodeselsph_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(nselsphxtxt);
      nodeselsph_x = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselsphytxt);
      nodeselsph_y = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselsphztxt);
      nodeselsph_z = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselsphrtxt);
      nodeselsph_r = atof(tmpstr);
      XtFree(tmpstr);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nselerror,XmNlabelString,string,NULL);
      XmStringFree(string);

      if (nodeselsph_r <= 0)
        {
         ierr = 1;
         string = XmStringCreate(str2,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(nselerror,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
     }

   /*  Read and check node number values.  */
   if (nodeselnum_on && ierr == 0)
     {
      nnodeselnum = 0;
      i = 0;

      while(ierr == 0 && i < MAXNUMSEL)
        {
         tmpstr = XmTextFieldGetString(nselnumtxt[i]);
         tmpstrlen = strlen(tmpstr);
         if (tmpstrlen > 0)
           {

            /*  Check for invalid characters.  */
            if (strspn(tmpstr,chk_num) != tmpstrlen)
              {
               ierr = 1;
               sprintf(str4,"Error - invalid character in line %d",i+1);
               string = XmStringCreate(str4,XmSTRING_DEFAULT_CHARSET);
               XtVaSetValues(nselerror,XmNlabelString,string,NULL);
               XmStringFree(string);
              }

            /*  If not all blanks, extract data from the string.  */
            if (ierr == 0 && strspn(tmpstr,chk_blank) != tmpstrlen)
              {

               /*  Get the first number from the set.  */
               token = strtok(tmpstr,":");
               num = strtoul(token,NULL,10);
               if (num > nnodes && !nodeidflag) num = nnodes;
               nodeselnum_f[nnodeselnum] = num;

               /*  Get the last number from the set.  */
               nodeselnum_l[nnodeselnum] = num;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  if (num > nnodes && !nodeidflag) num = nnodes;

                  /*  If first greater than last, switch.  */
                  if (num >= nodeselnum_f[i])
                     nodeselnum_l[nnodeselnum] = num;
                  else
                    {
                     nodeselnum_l[i] = nodeselnum_f[i];
                     nodeselnum_f[i] = num;
                    }
                 }

               /*  Get the stride from the set.  */
               nodeselnum_s[nnodeselnum] = 1;
               token = strtok(NULL,":");
               if (token != NULL)
                 {
                  num = strtoul(token,NULL,10);
                  nodeselnum_s[nnodeselnum] = num;
                 }

               nnodeselnum++;
              }
           }
         XtFree(tmpstr);
         i++;
        }

     }

   /*  Read and set the search box values.  */
   if (nodeselbox_on && ierr == 0)
     {
      tmpstr = XmTextFieldGetString(nselboxx1txt);
      x1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselboxy1txt);
      y1 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselboxz1txt);
      z1 = atof(tmpstr);
      XtFree(tmpstr);

      tmpstr = XmTextFieldGetString(nselboxx2txt);
      x2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselboxy2txt);
      y2 = atof(tmpstr);
      XtFree(tmpstr);
      tmpstr = XmTextFieldGetString(nselboxz2txt);
      z2 = atof(tmpstr);
      XtFree(tmpstr);

      nodeselbox_xmin = (x1 < x2) ? x1 : x2;
      nodeselbox_ymin = (y1 < y2) ? y1 : y2;
      nodeselbox_zmin = (z1 < z2) ? z1 : z2;
      nodeselbox_xmax = (x1 > x2) ? x1 : x2;
      nodeselbox_ymax = (y1 > y2) ? y1 : y2;
      nodeselbox_zmax = (z1 > z2) ? z1 : z2;
     }

   /*  Read and set node group selections.  */
   nnodegrpselected = 0;
   if (nodeselnodegrp_on && ierr == 0)
     {
      j = XmListGetSelectedPos(nodegrplist,&grpselected,&nnodegrpselected);
      if (!j) nnodegrpselected = 0;
      for (i = 0; i < MAXGROUPS; i++) nodegrpselected[i] = 0;
      for (i = 0; i < nnodegrpselected; i++) 
         nodegrpselected[i] = grpselected[i];
      FREE(grpselected);
     }

   if (ierr == 0)
     {
      XDefineCursor(dpy,topwin,waitcsr);
      XFlush(dpy);

      setnodesel();
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}


void nsset_on(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, k;

   i = (long int)client_data;
   if (i == 0) k = maxcellmat;
   else k = flagtypes_node[i-1];

   XmListDeselectAllItems(nsellist[i]);

   for (j = 0; j < k; j++)
     {
      XmListSelectPos(nsellist[i],j+1,FALSE);
     }
}


void nsset_off(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;

   i = (long int)client_data;

   XmListDeselectAllItems(nsellist[i]);
}


void select_node_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j, arg;

   arg = (long int)client_data;
   i = arg / MAXMATS;
   j = arg - (i*MAXMATS);
   nodeselflag[i][j] = 
      (short)XmToggleButtonGetState(w);
}


void select_and_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) nodeandorflag[i] = 1;
   if (j == 1) nodeandorflag[i] = 0;
}


void select_or_flag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, j;

   i = (long int)client_data;
   j = (int)XmToggleButtonGetState(w);
   if (j == 0) nodeandorflag[i] = 0;
   if (j == 1) nodeandorflag[i] = 1;
}


void nselnfldfld(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ifld;
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Set current component flag.  */
   ifld = nodeselfield;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDNODESEL, ifld);
}


void nselfldreset(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[20];

   i = nodeselfield;

   nfldselmin = fieldmin[i];
   nfldselmax = fieldmax[i];

   sprintf(str,"%.11g",fieldmin[i]);
   XtVaSetValues(nselmintxt,XmNvalue,str,NULL);
   sprintf(str,"%.11g",fieldmax[i]);
   XtVaSetValues(nselmaxtxt,XmNvalue,str,NULL);
}


void nsboxsubsetsl(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int ival, arg;
  float val;
  char str[20];

   nodeselbox_draw = 1;
   arg = (long int)client_data;

   XmScaleGetValue(w,&ival);
   val = (float) ival / 1000.;
 
   if (arg == 0 || arg == 6)
     {
      nodeselbox_xmin = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",nodeselbox_xmin);
      XtVaSetValues(nselboxx1txt,XmNvalue,str,NULL);
      if (nodeselbox_xmin > nodeselbox_xmax)
        {
         XmScaleSetValue(nselboxx2sl,ival);
         XtVaSetValues(nselboxx2txt,XmNvalue,str,NULL);
         nodeselbox_xmax = nodeselbox_xmin;
        }
     }
   if (arg == 1 || arg == 7)
     {
      nodeselbox_xmax = (globxicmax-globxicmin) * val + globxicmin;
      sprintf(str,"%.11g",nodeselbox_xmax);
      XtVaSetValues(nselboxx2txt,XmNvalue,str,NULL);
      if (nodeselbox_xmin > nodeselbox_xmax)
        {
         XmScaleSetValue(nselboxx1sl,ival);
         XtVaSetValues(nselboxx1txt,XmNvalue,str,NULL);
         nodeselbox_xmin = nodeselbox_xmax;
        }
     }
   if (arg == 2 || arg == 8)
     {
      nodeselbox_ymin = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",nodeselbox_ymin);
      XtVaSetValues(nselboxy1txt,XmNvalue,str,NULL);
      if (nodeselbox_ymin > nodeselbox_ymax)
        {
         XmScaleSetValue(nselboxy2sl,ival);
         XtVaSetValues(nselboxy2txt,XmNvalue,str,NULL);
         nodeselbox_ymax = nodeselbox_ymin;
        }
     }
   if (arg == 3 || arg == 9)
     {
      nodeselbox_ymax = (globyicmax-globyicmin) * val + globyicmin;
      sprintf(str,"%.11g",nodeselbox_ymax);
      XtVaSetValues(nselboxy2txt,XmNvalue,str,NULL);
      if (nodeselbox_ymin > nodeselbox_ymax)
        {
         XmScaleSetValue(nselboxy1sl,ival);
         XtVaSetValues(nselboxy1txt,XmNvalue,str,NULL);
         nodeselbox_ymin = nodeselbox_ymax;
        }
     }
   if (arg == 4 || arg == 10)
     {
      nodeselbox_zmin = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",nodeselbox_zmin);
      XtVaSetValues(nselboxz1txt,XmNvalue,str,NULL);
      if (nodeselbox_zmin > nodeselbox_zmax)
        {
         XmScaleSetValue(nselboxz2sl,ival);
         XtVaSetValues(nselboxz2txt,XmNvalue,str,NULL);
         nodeselbox_zmax = nodeselbox_zmin;
        }
     }
   if (arg == 5 || arg == 11)
     {
      nodeselbox_zmax = (globzicmax-globzicmin) * val + globzicmin;
      sprintf(str,"%.11g",nodeselbox_zmax);
      XtVaSetValues(nselboxz2txt,XmNvalue,str,NULL);
      if (nodeselbox_zmin > nodeselbox_zmax)
        {
         XmScaleSetValue(nselboxz1sl,ival);
         XtVaSetValues(nselboxz1txt,XmNvalue,str,NULL);
         nodeselbox_zmin = nodeselbox_zmax;
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


void nselboxtxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  char *tmpstr, str[20];
  float val;
  void setnodeselboxwidgets();

   nodeselbox_draw = 1;
   arg = (long int)client_data;

   tmpstr = XmTextFieldGetString(w);
   val = atof(tmpstr);
 
   if (arg == 0 || arg == 1)
     {
      if (val < globxicmin) val = globxicmin;
      if (val > globxicmax) val = globxicmax;
      if (arg == 0)
        {
         nodeselbox_xmin = val;
         if (nodeselbox_xmin > nodeselbox_xmax) 
            nodeselbox_xmax = nodeselbox_xmin;
        }
      else
        {
         nodeselbox_xmax = val;
         if (nodeselbox_xmin > nodeselbox_xmax)
            nodeselbox_xmin = nodeselbox_xmax;
        }
     }
 
   if (arg == 2 || arg == 3)
     {
      if (val < globyicmin) val = globyicmin;
      if (val > globyicmax) val = globyicmax;
      if (arg == 2)
        {
         nodeselbox_ymin = val;
         if (nodeselbox_ymin > nodeselbox_ymax)
            nodeselbox_ymax = nodeselbox_ymin;
        }
      else
        {
         nodeselbox_ymax = val;
         if (nodeselbox_ymin > nodeselbox_ymax)
            nodeselbox_ymin = nodeselbox_ymax;
        }
     }
 
   if (arg == 4 || arg == 5)
     {
      if (val < globzicmin) val = globzicmin;
      if (val > globzicmax) val = globzicmax;
      if (arg == 4)
        {
         nodeselbox_zmin = val;
         if (nodeselbox_zmin > nodeselbox_zmax)
            nodeselbox_zmax = nodeselbox_zmin;
        }
      else
        {
         nodeselbox_zmax = val;
         if (nodeselbox_zmin > nodeselbox_zmax)
            nodeselbox_zmin = nodeselbox_zmax;
        }
     }

   XtFree(tmpstr);

   setnodeselboxwidgets();

   fastdraw = 0;
   fastboxcount = 0;
   drawfastbox(fastboxcount);
}


void nsnumclr(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char blank[1] = "";

   for (i = 0; i < MAXNUMSEL; i++)
     {
      XmTextFieldSetString(nselnumtxt[i],blank);
     }
}


void makenodeselform(parent)
 Widget parent;
{
  Widget nsclose, nsapply, nsmatbut, nsfldbut, nssphbut, nsmatclose,
         nstxt[MAXFLAGS+2], nandorbox[MAXFLAGS+1], nson[MAXFLAGS+1],
         nsoff[MAXFLAGS+1], nsfldclose, nselfldlab1, nselfldbut,
         nselminlab1, nselmaxlab1, nselrangelab, nselminlab2,  
         nselmaxlab2, nselreset, nssphclose, nssphxlab,  nssphylab,
         nssphzlab, nssphrlab, nsnumbut, nsnumclose, nsnumwin,
         nsnumform, nsnumlab[MAXNUMSEL], nsmatapply, nsfldapply, 
         nssphapply, nsnumapply, nssphbox, nsboxclose, nsboxapply,
         nsboxx1lab, nsboxx2lab, nsboxy1lab, nsboxy2lab,
         nsboxz1lab, nsboxz2lab, nsboxbox, nsboxbut, nsngrpclose,
         nsngrpapply, nsgrpbox, nsnumclrbut;
  Widget lastleft, nodeselswin, nswinform, nsngrpbut;
  int i, j, jj, k, kk, winw;
  char str[20];
  Arg wargs[MAXGROUPS+20];
  XmString string; 
  XmString nodematitem[MAXMATS];
  XmString nodegrpitem[MAXGROUPS];


   string = XmStringCreate("Node Select",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodeselform = XmCreateFormDialog(parent,"Node_Select",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nsclose = XtVaCreateManagedWidget("Nsclose",
             xmPushButtonGadgetClass,
             nodeselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(nsclose,XmNactivateCallback,nselclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   nsapply = XtVaCreateManagedWidget("Nsapply",
             xmPushButtonGadgetClass,
             nodeselform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, nsclose,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(nsapply,XmNactivateCallback,nselapply,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[0] = XtVaCreateManagedWidget("Nselonbut0",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nsclose,
                  XmNtopOffset, 10,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[0],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 0);

   string = XmStringLtoRCreate("Materials\nand Flags",
                               XmSTRING_DEFAULT_CHARSET);
   nsmatbut = XtVaCreateManagedWidget("Nsmatbut",
             xmPushButtonGadgetClass,
             nodeselform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, nsclose,
             XmNtopOffset, 10,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, nselonbut[0],
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(nsmatbut,XmNactivateCallback,nodematselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[1] = XtVaCreateManagedWidget("Nselonbut1",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nsmatbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[1],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 1);

   string = XmStringLtoRCreate("Node Field\nData Range",
                               XmSTRING_DEFAULT_CHARSET);
   nsfldbut = XtVaCreateManagedWidget("Nsfldbut",
              xmPushButtonGadgetClass,
              nodeselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nsmatbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, nselonbut[1],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(nsfldbut,XmNactivateCallback,nodefldselformpop,NULL);
   if (numvars == 0)
     {
      XtSetSensitive(nselonbut[1],FALSE);
      XtSetSensitive(nsfldbut,FALSE);
     }

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[2] = XtVaCreateManagedWidget("Nselonbut2",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nsfldbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[2],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 2);

   string = XmStringLtoRCreate("Search\nSphere",
                               XmSTRING_DEFAULT_CHARSET);
   nssphbut = XtVaCreateManagedWidget("Nssphbut",
               xmPushButtonGadgetClass,
               nodeselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nsfldbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, nselonbut[2],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nssphbut,XmNactivateCallback,nodesphselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[3] = XtVaCreateManagedWidget("Nselonbut3",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nssphbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[3],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 3);

   string = XmStringLtoRCreate("Node\nNumbers",
                               XmSTRING_DEFAULT_CHARSET);
   nsnumbut = XtVaCreateManagedWidget("Nsnumbut",
              xmPushButtonGadgetClass,
              nodeselform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nssphbut,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, nselonbut[3],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(nsnumbut,XmNactivateCallback,nodenumselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[4] = XtVaCreateManagedWidget("Nselonbut4",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nsnumbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[4],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 4);

   string = XmStringLtoRCreate("Search\nBox",
                               XmSTRING_DEFAULT_CHARSET);
   nsboxbut = XtVaCreateManagedWidget("Nsboxbut",
               xmPushButtonGadgetClass,
               nodeselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nsnumbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, nselonbut[4],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nsboxbut,XmNactivateCallback,nodeboxselformpop,NULL);

   string = XmStringLtoRCreate("Select\nOn",XmSTRING_DEFAULT_CHARSET);
   nselonbut[5] = XtVaCreateManagedWidget("Nselonbut5",
                  xmToggleButtonWidgetClass,
                  nodeselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nsboxbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNselectColor, yellowpix,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut[5],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 5);

   string = XmStringLtoRCreate("Node\nGroups",
                               XmSTRING_DEFAULT_CHARSET);
   nsngrpbut = XtVaCreateManagedWidget("Nsngrpbut",
               xmPushButtonGadgetClass,
               nodeselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nsboxbut,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, nselonbut[5],
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(nsngrpbut,XmNactivateCallback,nodegrpselformpop,NULL);
   if (nnodegrps == 0 && physedflag == 0)
     {
      XtSetSensitive(nselonbut[5],FALSE);
      XtSetSensitive(nsngrpbut,FALSE);
     }

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   nselerror = XtVaCreateManagedWidget("Nselerror",
               xmLabelWidgetClass,
               nodeselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nsngrpbut,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 


   /*  Create Materials and Flags selection menu.  */
   /************************************************/

   string = XmStringCreate("Node Select - Materials and Flags",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodematselform = XmCreateFormDialog(nodeselform,"Nodematselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nsmatclose = XtVaCreateManagedWidget("Nsmatclose",
                xmPushButtonGadgetClass,
                nodematselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsmatclose,XmNactivateCallback,nselmatclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   nsmatapply = XtVaCreateManagedWidget("Nsmatapply",
                xmPushButtonGadgetClass,
                nodematselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nsmatclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsmatapply,XmNactivateCallback,nselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   nselonbut2[0] = XtVaCreateManagedWidget("Nselonbut20",
                   xmToggleButtonWidgetClass,
                   nodematselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nsmatclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut2[0],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 0);

   winw = 180;
   if (numflags_node > 0) winw = 250;
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nselonbut2[0]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   /* i++;  XtSetArg(wargs[i],XmNwidth, winw); */
   i++;  XtSetArg(wargs[i],XmNheight, 320);
   i++;
   nodeselswin = XmCreateScrolledWindow(nodematselform,"Nodeselswin",wargs,i);
   XtManageChild(nodeselswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nswinform = XmCreateForm(nodeselswin,"Nswinform",wargs,i);

   /*  Create material selections.  */

   string = XmStringCreate("Materials",XmSTRING_DEFAULT_CHARSET);
   nstxt[0] = XtVaCreateManagedWidget("Nstxt0",
              xmLabelWidgetClass,
              nswinform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   nson[0] = XtVaCreateManagedWidget("Nson0",
             xmPushButtonGadgetClass,
             nswinform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, nstxt[0],
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(nson[0],XmNactivateCallback,nsset_on,
                    (caddr_t) 0);

   string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
   nsoff[0] = XtVaCreateManagedWidget("Nsoff0",
              xmPushButtonGadgetClass,
              nswinform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, nstxt[0],
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, nson[0],
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(nsoff[0],XmNactivateCallback,nsset_off,
                    (caddr_t) 0);

   for (i = 0; i < maxcellmat; i++)
      nodematitem[i] = XmStringLtoRCreate(mmatnames[i],
                                          XmSTRING_DEFAULT_CHARSET);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nson[0]);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNheight, 160);
   if (maxcellmat > 0)
     {
      i++;  XtSetArg(wargs[i],XmNitemCount, maxcellmat);
      i++;  XtSetArg(wargs[i],XmNitems, nodematitem);
     }
   i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
   i++;
   nsellist[0] = (Widget)XmCreateScrolledList(nswinform,
                                              "Nsellist0",wargs,i);
   //TJ 2013-12-18 crashing here for some reason with large grid/example file
   XtManageChild(nsellist[0]);
   for (i = 0; i < maxcellmat; i++) 
      XmStringFree(nodematitem[i]);
  
   /*  Create sets of and/or and flag selection buttons.  */
 
   lastleft = nsellist[0];
   if (numflags_node > 0)
     {
      for (j = 0; j < numflags_node; j++)
        {
         jj = j + 1;
         i=0; XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++; XtSetArg(wargs[i],XmNtopWidget, nson[j]);
         i++; XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++; XtSetArg(wargs[i],XmNleftWidget, lastleft);
         i++; XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++; XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;
         sprintf(str,"Nandorbox%d",j);
         nandorbox[j] = XmCreateRadioBox(nswinform,str,wargs,i);
         XtManageChild(nandorbox[j]);
         string = XmStringCreate("And",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Nandbuts%d",j);
         nandbuts[j] = XtVaCreateManagedWidget(str,
                       xmToggleButtonGadgetClass,
                       nandorbox[j],
                       XmNlabelString, string,
                       XmNspacing, 1,
                       XmNselectColor, yellowpix,
                       XmNmarginHeight, 0,
                       XmNmarginWidth, 0,
                       XmNset, TRUE,
                       NULL);
            XmStringFree(string);
            XtAddCallback(nandbuts[j],XmNvalueChangedCallback,
                          select_and_flag,(caddr_t) j);
         string = XmStringCreate("Or",
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Norbuts%d",j);
         norbuts[j] = XtVaCreateManagedWidget(str,
                      xmToggleButtonGadgetClass,
                      nandorbox[j],
                      XmNlabelString, string,
                      XmNspacing, 1,
                      XmNselectColor, yellowpix,
                      XmNmarginHeight, 0,
                      XmNmarginWidth, 0,
                      XmNset, FALSE,
                      NULL);
            XmStringFree(string);
            XtAddCallback(norbuts[j],XmNvalueChangedCallback,
                          select_or_flag,(caddr_t) j);

         string = XmStringCreate(flagname_node[j],
                                 XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Nstxt%d",jj);
         nstxt[jj] = XtVaCreateManagedWidget(str,
                     xmLabelWidgetClass,
                     nswinform,
                     XmNtopAttachment, XmATTACH_FORM,
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, nandorbox[j],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
            XmStringFree(string);

         string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Nson%d",jj);
         nson[jj] = XtVaCreateManagedWidget(str,
                    xmPushButtonGadgetClass,
                    nswinform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nstxt[jj],
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, nandorbox[j],
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
            XmStringFree(string);
            XtAddCallback(nson[jj],XmNactivateCallback,nsset_on,
                          (caddr_t) jj);

         string = XmStringCreate("Off",XmSTRING_DEFAULT_CHARSET);
         sprintf(str,"Nsoff%d",jj);
         nsoff[jj] = XtVaCreateManagedWidget(str,
                     xmPushButtonGadgetClass,
                     nswinform,
                     XmNtopAttachment, XmATTACH_WIDGET,
                     XmNtopWidget, nstxt[jj],
                     XmNleftAttachment, XmATTACH_WIDGET,
                     XmNleftWidget, nson[jj],
                     XmNrightAttachment, XmATTACH_NONE,
                     XmNbottomAttachment, XmATTACH_NONE,
                     XmNlabelString, string,
                     NULL);
            XmStringFree(string);
            XtAddCallback(nsoff[jj],XmNactivateCallback,nsset_off,
                          (caddr_t) jj);

         kk = flagtypes_node[j];
         for (i = 0; i < kk; i++)
            nodematitem[i] = XmStringLtoRCreate(flagnames_node[j][i],
                                                XmSTRING_DEFAULT_CHARSET);

         k = j + 1;
         i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNtopWidget, nson[jj]);
         i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_WIDGET);
         i++;  XtSetArg(wargs[i],XmNleftWidget, nandorbox[j]);
         i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
         i++;  XtSetArg(wargs[i],XmNheight, 160);
         if (kk > 0)
           {
            i++;  XtSetArg(wargs[i],XmNitemCount, kk);
            i++;  XtSetArg(wargs[i],XmNitems, nodematitem);
           }
         i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmMULTIPLE_SELECT);
         i++;
         sprintf(str,"Nsellist%d",k);
         nsellist[k] = (Widget)XmCreateScrolledList(nswinform,
                                                    str,wargs,i);
         XtManageChild(nsellist[k]);
         for (i = 0; i < kk; i++) 
            XmStringFree(nodematitem[i]);

         lastleft = nsellist[k];
        } 
     }
     
   XtManageChild(nswinform);


   /*  Create Node Field Data Range selection menu.  */
   /**************************************************/
 
   if (numvars > 0)
     {
      string = XmStringCreate("Node Select - Node Field Data Range",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      nodefldselform = XmCreateFormDialog(nodeselform,"Nodefldselform",
                                          wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      nsfldclose = XtVaCreateManagedWidget("Nsfldclose",
                    xmPushButtonGadgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(nsfldclose,XmNactivateCallback,nselfldclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      nsfldapply = XtVaCreateManagedWidget("Nsfldapply",
                   xmPushButtonGadgetClass,
                   nodefldselform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, nsfldclose,
                   XmNleftOffset, 10,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(nsfldapply,XmNactivateCallback,nselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      nselonbut2[1] = XtVaCreateManagedWidget("Nselonbut21",
                      xmToggleButtonWidgetClass,
                      nodefldselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, nsfldclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(nselonbut2[1],XmNvalueChangedCallback,nselsetonbut,
                       (caddr_t) 1);

      string = XmStringCreate("Field:",XmSTRING_DEFAULT_CHARSET);
      nselfldlab1 = XtVaCreateManagedWidget("Nselfldlab1",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselonbut2[1],
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      nselfldlab = XtVaCreateManagedWidget("Nselfldlab",
                   xmLabelWidgetClass,
                   nodefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nselonbut2[1],
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, nselfldlab1,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
            XmStringFree(string); 

      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      nselfldbut = XtVaCreateManagedWidget("Nselfldbut",
                   xmPushButtonGadgetClass,
                   nodefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nselfldlab1,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(nselfldbut,XmNactivateCallback,nselnfldfld,
                       NULL);

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      nselminlab1 = XtVaCreateManagedWidget("Nselminlab1",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselfldbut,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      nselminlab = XtVaCreateManagedWidget("Nselminlab",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselfldbut,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, nselminlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      nselmaxlab1 = XtVaCreateManagedWidget("Nselmaxlab1",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselminlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
      nselmaxlab = XtVaCreateManagedWidget("Nselmaxlab",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselminlab1,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, nselmaxlab1,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);  

      string = XmStringCreate("RANGE",XmSTRING_DEFAULT_CHARSET);
      nselrangelab = XtVaCreateManagedWidget("Nselrangelab1",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselmaxlab1,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      string = XmStringCreate("Min:",XmSTRING_DEFAULT_CHARSET);
      nselminlab2 = XtVaCreateManagedWidget("Nselminlab2",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselrangelab,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      nselmintxt = XtVaCreateManagedWidget("Nselmintxt",
                   xmTextFieldWidgetClass,
                   nodefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nselrangelab,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, nselminlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Max:",XmSTRING_DEFAULT_CHARSET);
      nselmaxlab2 = XtVaCreateManagedWidget("Nselmaxlab2",
                    xmLabelWidgetClass,
                    nodefldselform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselminlab2,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNleftOffset, 20,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string); 

      nselmaxtxt = XtVaCreateManagedWidget("Nselmaxtxt",
                   xmTextFieldWidgetClass,
                   nodefldselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nselminlab2,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, nselmaxlab2,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 1,
                   XmNmaxLength, 20,
                   XmNcolumns, 20,
                   XmNvalue, "",
                   NULL);

      string = XmStringCreate("Reset",XmSTRING_DEFAULT_CHARSET);
      nselreset = XtVaCreateManagedWidget("Nselreset",
                  xmPushButtonGadgetClass,
                  nodefldselform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nselmaxtxt,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(nselreset,XmNactivateCallback,nselfldreset,
                       NULL);
     }


   /*  Create Search Sphere selection menu.  */
   /*****************************************/

   string = XmStringCreate("Node Select - Search Sphere",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodesphselform = XmCreateFormDialog(nodeselform,"Nodesphselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nssphclose = XtVaCreateManagedWidget("Nssphclose",
                xmPushButtonGadgetClass,
                nodesphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nssphclose,XmNactivateCallback,nselsphclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   nssphapply = XtVaCreateManagedWidget("Nssphapply",
                xmPushButtonGadgetClass,
                nodesphselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nssphclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nssphapply,XmNactivateCallback,nselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   nselonbut2[2] = XtVaCreateManagedWidget("Nselonbut22",
                   xmToggleButtonWidgetClass,
                   nodesphselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nssphclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut2[2],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 2);

   string = XmStringCreate("X:",XmSTRING_DEFAULT_CHARSET);
   nssphxlab = XtVaCreateManagedWidget("Nssphxlab",
               xmLabelWidgetClass,
               nodesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nselonbut2[2],
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   nselsphxtxt = XtVaCreateManagedWidget("Nselsphxtxt",
                 xmTextFieldWidgetClass,
                 nodesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, nselonbut2[2],
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nssphxlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Y:",XmSTRING_DEFAULT_CHARSET);
   nssphylab = XtVaCreateManagedWidget("Nssphylab",
               xmLabelWidgetClass,
               nodesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nssphxlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   nselsphytxt = XtVaCreateManagedWidget("Nselsphytxt",
                 xmTextFieldWidgetClass,
                 nodesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, nssphxlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nssphylab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("Z:",XmSTRING_DEFAULT_CHARSET);
   nssphzlab = XtVaCreateManagedWidget("Nssphzlab",
               xmLabelWidgetClass,
               nodesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nssphylab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   nselsphztxt = XtVaCreateManagedWidget("Nselsphztxt",
                 xmTextFieldWidgetClass,
                 nodesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, nssphylab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nssphzlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   string = XmStringCreate("R:",XmSTRING_DEFAULT_CHARSET);
   nssphrlab = XtVaCreateManagedWidget("Nssphrlab",
               xmLabelWidgetClass,
               nodesphselform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, nssphzlab,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string); 

   nselsphrtxt = XtVaCreateManagedWidget("Nselsphrtxt",
                 xmTextFieldWidgetClass,
                 nodesphselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, nssphzlab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nssphrlab,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNmarginHeight, 0,
                 XmNmarginWidth, 1,
                 XmNmaxLength, 20,
                 XmNcolumns, 18,
                 XmNvalue, "",
                 NULL);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nselsphrtxt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   nssphbox = XmCreateRadioBox(nodesphselform,"Nssphbox",wargs,i);
   XtManageChild(nssphbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   nselsphin = XtVaCreateManagedWidget("Nselsphin",
               xmToggleButtonGadgetClass,
               nssphbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(nselsphin,XmNvalueChangedCallback,
                   nselsphsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   nselsphout = XtVaCreateManagedWidget("Nselsphout",
                xmToggleButtonGadgetClass,
                nssphbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(nselsphout,XmNvalueChangedCallback,
                   nselsphsetinopt,(caddr_t) 0);


   /*  Create Search Box selection menu.  */
   /***************************************/

   string = XmStringCreate("Node Select - Search Box",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodeboxselform = XmCreateFormDialog(nodeselform,"Nodeboxselform",
                                       wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nsboxclose = XtVaCreateManagedWidget("Nsboxclose",
                xmPushButtonGadgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsboxclose,XmNactivateCallback,nselboxclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   nsboxapply = XtVaCreateManagedWidget("Nsboxapply",
                xmPushButtonGadgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nsboxclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsboxapply,XmNactivateCallback,nselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   nselonbut2[4] = XtVaCreateManagedWidget("Nselonbut25",
                   xmToggleButtonWidgetClass,
                   nodeboxselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nsboxclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut2[4],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 4);

   string = XmStringCreate("xmin",XmSTRING_DEFAULT_CHARSET);
   nsboxx1lab = XtVaCreateManagedWidget("Nsboxx1lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselonbut2[4],
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxx1sl = XtVaCreateManagedWidget("Nselboxx1sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxx1lab,
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
      XtAddCallback(nselboxx1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 0);
      XtAddCallback(nselboxx1sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 6);

   nselboxx1txt = XtVaCreateManagedWidget("Nselboxx1txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxx1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxx1txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 0);

   string = XmStringCreate("xmax",XmSTRING_DEFAULT_CHARSET);
   nsboxx2lab = XtVaCreateManagedWidget("Nsboxx2lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselonbut2[4],
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nselboxx1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxx2sl = XtVaCreateManagedWidget("Nselboxx2sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxx1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxx2lab,
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
      XtAddCallback(nselboxx2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 1);
      XtAddCallback(nselboxx2sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 7);

   nselboxx2txt = XtVaCreateManagedWidget("Nselboxx2txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxx1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxx2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxx2txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 1);

   string = XmStringCreate("ymin",XmSTRING_DEFAULT_CHARSET);
   nsboxy1lab = XtVaCreateManagedWidget("Nsboxy1lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselboxx1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxy1sl = XtVaCreateManagedWidget("Nselboxy1sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxy1lab,
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
      XtAddCallback(nselboxy1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 2);
      XtAddCallback(nselboxy1sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 8);

   nselboxy1txt = XtVaCreateManagedWidget("Nselboxy1txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxy1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxy1txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 2);

   string = XmStringCreate("ymax",XmSTRING_DEFAULT_CHARSET);
   nsboxy2lab = XtVaCreateManagedWidget("Nsboxy2lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselboxx1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nselboxy1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxy2sl = XtVaCreateManagedWidget("Nselboxy2sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxy1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxy2lab,
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
      XtAddCallback(nselboxy2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 3);
      XtAddCallback(nselboxy2sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 9);

   nselboxy2txt = XtVaCreateManagedWidget("Nselboxy2txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxy1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxy2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxy2txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 3);

   string = XmStringCreate("zmin",XmSTRING_DEFAULT_CHARSET);
   nsboxz1lab = XtVaCreateManagedWidget("Nsboxz1lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselboxy1txt,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxz1sl = XtVaCreateManagedWidget("Nselboxz1sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxz1lab,
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
      XtAddCallback(nselboxz1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 4);
      XtAddCallback(nselboxz1sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 10);

   nselboxz1txt = XtVaCreateManagedWidget("Nselboxz1txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxz1sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxz1txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 4);

   string = XmStringCreate("zmax",XmSTRING_DEFAULT_CHARSET);
   nsboxz2lab = XtVaCreateManagedWidget("Nsboxz2lab",
                xmLabelWidgetClass,
                nodeboxselform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nselboxy1txt,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nselboxz1txt,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string); 

   nselboxz2sl = XtVaCreateManagedWidget("Nselboxz2sl",
                 xmScaleWidgetClass,
                 nodeboxselform,
                 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                 XmNtopWidget, nsboxz1lab,
                 XmNleftAttachment, XmATTACH_WIDGET,
                 XmNleftWidget, nsboxz2lab,
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
      XtAddCallback(nselboxz2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 5);
      XtAddCallback(nselboxz2sl,XmNdragCallback,nsboxsubsetsl,
                    (caddr_t) 11);

   nselboxz2txt = XtVaCreateManagedWidget("Nselboxz2txt",
                  xmTextFieldWidgetClass,
                  nodeboxselform,
                  XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNtopWidget, nsboxz1lab,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nselboxz2sl,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 18,
                  XmNvalue, "",
                  NULL);
      XtAddCallback(nselboxz2txt,XmNactivateCallback,
                    nselboxtxt,(caddr_t) 5);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nselboxz1txt);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;
   nsboxbox = XmCreateRadioBox(nodeboxselform,"nsboxbox",wargs,i);
   XtManageChild(nsboxbox);

   string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
   nselboxin = XtVaCreateManagedWidget("Nselboxin",
               xmToggleButtonGadgetClass,
               nsboxbox,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNmarginHeight, 0,
               XmNmarginWidth, 0,
               XmNset, FALSE,
               NULL);
     XmStringFree(string);
     XtAddCallback(nselboxin,XmNvalueChangedCallback,
                   nselboxsetinopt,(caddr_t) 0);

   string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
   nselboxout = XtVaCreateManagedWidget("Nselboxout",
                xmToggleButtonGadgetClass,
                nsboxbox,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNset, FALSE,
                NULL);
     XmStringFree(string);
     XtAddCallback(nselboxout,XmNvalueChangedCallback,
                   nselboxsetinopt,(caddr_t) 0);


   /*  Create Node Number selection menu.  */
   /****************************************/

   string = XmStringCreate("Node Select - Node Number",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 10);
   i++; XtSetArg(wargs[i], XmNy, 100);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nodenumselform = XmCreateFormDialog(nodeselform,"Nodenumselform",wargs,i);
      XmStringFree(string);
 
   string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
   nsnumclose = XtVaCreateManagedWidget("Nsnumclose",
                xmPushButtonGadgetClass,
                nodenumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsnumclose,XmNactivateCallback,nselnumclose,NULL);

   string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
   nsnumapply = XtVaCreateManagedWidget("Nsnumapply",
                xmPushButtonGadgetClass,
                nodenumselform,
                XmNtopAttachment, XmATTACH_FORM,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, nsnumclose,
                XmNleftOffset, 10,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nsnumapply,XmNactivateCallback,nselapply,NULL);

   string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
   nselonbut2[3] = XtVaCreateManagedWidget("Nselonbut23",
                   xmToggleButtonWidgetClass,
                   nodenumselform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nsnumclose,
                   XmNtopOffset, 10,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNselectColor, yellowpix,
                   XmNlabelString, string,
                   NULL);
      XmStringFree(string);
      XtAddCallback(nselonbut2[3],XmNvalueChangedCallback,nselsetonbut,
                    (caddr_t) 3);

   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, nselonbut2[3]);
   i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 150);
   i++;  XtSetArg(wargs[i],XmNheight, 250);
   i++;
   nsnumwin = XmCreateScrolledWindow(nodenumselform,"Nsnumwin",wargs,i);
   XtManageChild(nsnumwin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   nsnumform = XmCreateForm(nsnumwin,"Nsnumform",wargs,i);

   string = XmStringCreate(" 1",XmSTRING_DEFAULT_CHARSET);
   nsnumlab[0] = XtVaCreateManagedWidget("Nsnumlab0",
                 xmLabelWidgetClass,
                 nsnumform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string); 

   nselnumtxt[0] = XtVaCreateManagedWidget("nselnumtxt0",
                   xmTextFieldWidgetClass,
                   nsnumform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_WIDGET,
                   XmNleftWidget, nsnumlab[0],
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
      sprintf(str,"Nsnumlab%d",i);
      nsnumlab[i] = XtVaCreateManagedWidget("str",
                    xmLabelWidgetClass,
                    nsnumform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, nselnumtxt[i-1],
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
      XmStringFree(string); 

      sprintf(str,"Nselnumtxt%d",i);
      nselnumtxt[i] = XtVaCreateManagedWidget(str,
                      xmTextFieldWidgetClass,
                      nsnumform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, nselnumtxt[i-1],
                      XmNleftAttachment, XmATTACH_WIDGET,
                      XmNleftWidget, nsnumlab[i],
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
   nsnumclrbut = XtVaCreateManagedWidget("Nsnumclrbut",
                 xmPushButtonGadgetClass,
                 nodenumselform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, nsnumwin,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(nsnumclrbut,XmNactivateCallback,nsnumclr,
                    (caddr_t) 0);

   XtManageChild(nsnumform);


   /*  Create Node Groups selection menu.  */
   /************************************************/

   if (nnodegrps > 0 || physedflag)
     {
      string = XmStringCreate("Node Select - Node Groups",
                              XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 10);
      i++; XtSetArg(wargs[i], XmNy, 100);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      nodegrpselform = XmCreateFormDialog(nodeselform,"Nodegrpselform",wargs,i);
         XmStringFree(string);
 
      string = XmStringCreate("Close",XmSTRING_DEFAULT_CHARSET);
      nsngrpclose = XtVaCreateManagedWidget("Nsngrpclose",
                    xmPushButtonGadgetClass,
                    nodegrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_FORM,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(nsngrpclose,XmNactivateCallback,nodegrpclose,NULL);

      string = XmStringCreate("Apply",XmSTRING_DEFAULT_CHARSET);
      nsngrpapply = XtVaCreateManagedWidget("Nsngrpapply",
                    xmPushButtonGadgetClass,
                    nodegrpselform,
                    XmNtopAttachment, XmATTACH_FORM,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, nsngrpclose,
                    XmNleftOffset, 10,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);
         XtAddCallback(nsngrpapply,XmNactivateCallback,nselapply,NULL);

      string = XmStringLtoRCreate("Select On",XmSTRING_DEFAULT_CHARSET);
      nselonbut2[5] = XtVaCreateManagedWidget("Nselonbut25",
                      xmToggleButtonWidgetClass,
                      nodegrpselform,
                      XmNtopAttachment, XmATTACH_WIDGET,
                      XmNtopWidget, nsngrpclose,
                      XmNtopOffset, 10,
                      XmNleftAttachment, XmATTACH_FORM,
                      XmNrightAttachment, XmATTACH_NONE,
                      XmNbottomAttachment, XmATTACH_NONE,
                      XmNselectColor, yellowpix,
                      XmNlabelString, string,
                      NULL);
         XmStringFree(string);
         XtAddCallback(nselonbut2[5],XmNvalueChangedCallback,nselsetonbut,
                       (caddr_t) 5);

      for (i = 0; i < nnodegrps; i++)
         nodegrpitem[i] = XmStringLtoRCreate(nodegrpname[i],
                                             XmSTRING_DEFAULT_CHARSET);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, nselonbut2[5]);
      i++;  XtSetArg(wargs[i],XmNtopOffset, 10);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNheight, 250);
      if (nnodegrps > 0)
        {
         i++;  XtSetArg(wargs[i],XmNitemCount, nnodegrps);
         i++;  XtSetArg(wargs[i],XmNitems, nodegrpitem);
        }
      i++;  XtSetArg(wargs[i],XmNselectionPolicy, XmEXTENDED_SELECT);
      i++;
      nodegrplist = (Widget)XmCreateScrolledList(nodegrpselform,
                                                 "Nodegrplist",wargs,i);
      XtManageChild(nodegrplist);
      for (i = 0; i < nnodegrps; i++) 
         XmStringFree(nodegrpitem[i]);

      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, nodegrplist);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;
      nsgrpbox = XmCreateRadioBox(nodegrpselform,"Nsgrpbox",wargs,i);
      XtManageChild(nsgrpbox);

      string = XmStringCreate("Inside", XmSTRING_DEFAULT_CHARSET);
      nselgrpin = XtVaCreateManagedWidget("Nselgrpin",
                  xmToggleButtonGadgetClass,
                  nsgrpbox,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 0,
                  XmNset, FALSE,
                  NULL);
        XmStringFree(string);
        XtAddCallback(nselgrpin,XmNvalueChangedCallback,
                      nselgrpsetinopt,(caddr_t) 0);

      string = XmStringCreate("Outside", XmSTRING_DEFAULT_CHARSET);
      nselgrpout = XtVaCreateManagedWidget("Nselgrpout",
                   xmToggleButtonGadgetClass,
                   nsgrpbox,
                   XmNlabelString, string,
                   XmNspacing, 1,
                   XmNselectColor, yellowpix,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNset, FALSE,
                   NULL);
        XmStringFree(string);
        XtAddCallback(nselgrpout,XmNvalueChangedCallback,
                      nselgrpsetinopt,(caddr_t) 0);
     }
}


void setnodesel()
{
  int i, j, k, nf, ii, jj, id, igrp;
  long ifirst, ilast, istride;
  short isel, jsel;
  float xb, yb, zb;
  double rad, dist, xd, yd, zd;
  double *value;
  double * readnodevalue(int ifld);

   /*  Allocate array and read field for range selection, if needed.  */
   if (nodeselfld_on)
      value = readnodevalue(nodeselfield);

   /*  For search sphere test, square the sphere radius.  */
   rad = (double)nodeselsph_r * (double)nodeselsph_r;

   /*  Loop through nodes and set selections.  */
   for (i = 1; i < nnodes+1; i++)
     {
      nodeselectflag[i] = 1;

      /*  Set material selections first.  */
      if (nodeselmat_on)
        {
         nodeselectflag[i] = 0;

         for (k = 0; k < maxcellmat; k++)
            if (imat[i]-1 == k && nodeselflag[0][k] == 1)
                nodeselectflag[i] = 1;
         if (maxcellmat < 1) nodeselectflag[i] = 1;

         /*  Add nodes with a 0 material no.  */
         if (imat[i] < 1) nodeselectflag[i] = 1;

         /*  Now check selection flags.  */
         for (j = 1; j <= numflags_node; j++)
           {
            nf = flagtypes_node[j-1];
            jsel = 0;
            for (k = 0; k < nf; k++)
               if (flags[j-1][i]-1 == k && nodeselflag[j][k] == 1)
                  jsel = 1;

            /*  And/or with previous select flag.  */
            isel = nodeselectflag[i];
            nodeselectflag[i] = 0;
            if (nodeandorflag[j-1] == 0)
               if (isel == 1 && jsel == 1) nodeselectflag[i] = 1;
            if (nodeandorflag[j-1] == 1)
              if (isel == 1 || jsel == 1) nodeselectflag[i] = 1;
           } 
        }

      /*  Now check field range, if requested.  */
      isel = (int)nodeselectflag[i];
      if (nodeselfld_on && isel)
        {
         jsel = 0;
         if (nfldselmin <= value[i] && value[i] <= nfldselmax)
            jsel = 1;

         nodeselectflag[i] = jsel;
        }

      /*  Now check search sphere, if requested.  */
      isel = (int)nodeselectflag[i];
      if (nodeselsph_on && isel)
        {
         jsel = 0;

         xd = nodes[i].x - (double)nodeselsph_x;
         yd = nodes[i].y - (double)nodeselsph_y;
         zd = nodes[i].z - (double)nodeselsph_z;
         dist = xd*xd + yd*yd + zd*zd;
         if (nodeselsph_in == 1 && dist <= rad)
            jsel = 1;
         if (nodeselsph_in == 0 && dist >= rad)
            jsel = 1;

         nodeselectflag[i] = jsel;
        }

      /*  Now check search box, if requested.  */
      isel = (int)nodeselectflag[i];
      if (nodeselbox_on && isel)
        {
         jsel = 0;

         xb = nodes[i].x;
         yb = nodes[i].y;
         zb = nodes[i].z;

         if (nodeselbox_in == 1)
           {
            if (nodeselbox_xmin <= xb && xb <= nodeselbox_xmax &&
                nodeselbox_ymin <= yb && yb <= nodeselbox_ymax &&
                nodeselbox_zmin <= zb && zb <= nodeselbox_zmax)
              jsel = 1;
           }
         if (nodeselbox_in == 0)
           {
            if (xb <= nodeselbox_xmin || nodeselbox_xmax <= xb ||
                yb <= nodeselbox_ymin || nodeselbox_ymax <= yb ||
                zb <= nodeselbox_zmin || nodeselbox_zmax <= zb)
               jsel = 1;
           }

         nodeselectflag[i] = jsel;
        }
    }

   if (nodeselfld_on) free(value);

   /*  Check node number selection, and with  */
   /*  current select flag for those nodes.   */
   if (nodeselnum_on)
     {

      /*  Loop through nodes, and get a node id.  */
      for (ii = 1; ii < nnodes+1; ii++)
        {
         id = ii;
         if (nodeidflag) id = nodeids[ii];

         /*  Loop through active first, last, stride sets.  */
         for (j = 0; j < nnodeselnum; j++)
           {
            ifirst = nodeselnum_f[j];
            ilast = nodeselnum_l[j];
            istride = nodeselnum_s[j];

            if (ifirst <= id && id <= ilast)
              {
               for (i = ifirst; i <= ilast; i+=istride)
                 {
                  if (i == id)
                    {
                     isel = (int)nodeselectflag[ii];

                     /*  If previously selected, set value to 2.  */
                     if (isel) nodeselectflag[ii] = 2;
                    }
                 }
              }
           }
        }

      /*  Now loop through nodes, and keep those with a 2.  */
      for (i = 1; i < nnodes+1; i++)
        {
         jsel = 0;
         isel = (int)nodeselectflag[i];
         if (isel == 2) jsel = 1;
         nodeselectflag[i] = jsel;
        }
     }

   /*  Check node group selection, and with   */
   /*  current select flag for those nodes.   */
   if (nodeselnodegrp_on && nnodegrpselected > 0)
     {

      /*  Loop through nodes, and get a node id.  */
      for (ii = 0; ii < nnodes+1; ii++)
        {
         id = ii;
         if (nodeidflag) id = nodeids[ii];

         /*  If previously selected, loop through selected  */ 
         /*  node groups and set selected node to 2.        */
         isel = (int)nodeselectflag[ii];
         if (isel)
           {
            if (nodeselgrp_in == 0) nodeselectflag[ii] = 2;
            for (j = 0; j < nnodegrpselected; j++)
              {
               igrp = nodegrpselected[j] - 1;
               k = numnodegrp[igrp];
               for (i = 0; i < k; i++)
                 {
                  jj = nodegrps[igrp][i];
                  if (jj == id)
                    {
                     if (nodeselgrp_in == 1) nodeselectflag[ii] = 2;
                     if (nodeselgrp_in == 0) nodeselectflag[ii] = 0;
                    }
                 }
              }
           }
        }

      /*  Now loop through nodes, and keep those with a 2.  */
      for (i = 1; i < nnodes+1; i++)
        {
         jsel = 0;
         isel = (int)nodeselectflag[i];
         if (isel == 2) jsel = 1;
         nodeselectflag[i] = jsel;
        }
     }

   /*  Check ghost node options.  */
   if (nodeghostflag) drawghostnodeoptions();
}


void nodeselgetnfield(int i)
{
  char str[20];
  XmString string;

   nodeselfield = i;

   string = XmStringCreate(fieldname[i],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nselfldlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"%.11g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nselminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"%.11g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nselmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   nfldselmin = fieldmin[i];
   nfldselmax = fieldmax[i];
   sprintf(str,"%.11g",fieldmin[i]);
   XtVaSetValues(nselmintxt,XmNvalue,str,NULL);
   sprintf(str,"%.11g",fieldmax[i]);
   XtVaSetValues(nselmaxtxt,XmNvalue,str,NULL);
}


void setnodeselboxwidgets()
{
  int ival;
  double val;
  char str[20];

   XtRemoveCallback(nselboxx1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 0);
   XtRemoveCallback(nselboxx2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 1);
   XtRemoveCallback(nselboxy1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 2);
   XtRemoveCallback(nselboxy2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 3);
   XtRemoveCallback(nselboxz1sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 4);
   XtRemoveCallback(nselboxz2sl,XmNvalueChangedCallback,nsboxsubsetsl,
                    (caddr_t) 5);
   XFlush(dpy);

   sprintf(str,"%.11g",nodeselbox_xmin);
   XmTextFieldSetString(nselboxx1txt,str);
   val = (nodeselbox_xmin - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxx1sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",nodeselbox_xmax);
   XmTextFieldSetString(nselboxx2txt,str);
   val = (nodeselbox_xmax - globxicmin) / (globxicmax - globxicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxx2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",nodeselbox_ymin);
   XmTextFieldSetString(nselboxy1txt,str);
   val = (nodeselbox_ymin - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxy1sl,XmNvalue,ival, NULL);

   sprintf(str,"%.11g",nodeselbox_ymax);
   XmTextFieldSetString(nselboxy2txt,str);
   val = (nodeselbox_ymax - globyicmin) / (globyicmax-globyicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxy2sl, XmNvalue,ival, NULL);

   sprintf(str,"%.11g",nodeselbox_zmin);
   XmTextFieldSetString(nselboxz1txt,str);
   val = (nodeselbox_zmin - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxz1sl,XmNvalue,ival, NULL);

   sprintf(str,"%.11g",nodeselbox_zmax);
   XmTextFieldSetString(nselboxz2txt,str);
   val = (nodeselbox_zmax - globzicmin) / (globzicmax-globzicmin);
   ival = (int) (val * 1000.);
   XtVaSetValues(nselboxz2sl, XmNvalue,ival, NULL);

   XFlush(dpy);
   XtAddCallback(nselboxx1sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 0);
   XtAddCallback(nselboxx2sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 1);
   XtAddCallback(nselboxy1sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 2);
   XtAddCallback(nselboxy2sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 3);
   XtAddCallback(nselboxz1sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 4);
   XtAddCallback(nselboxz2sl,XmNvalueChangedCallback,nsboxsubsetsl,(caddr_t) 5);
   XFlush(dpy);
}

void setnodeselwidgets()
{
  char str[20];

   setnodeselboxwidgets();

   sprintf(str,"%.11g",nodeselsph_x);
   XmTextFieldSetString(nselsphxtxt,str);

   sprintf(str,"%.11g",nodeselsph_y);
   XmTextFieldSetString(nselsphytxt,str);

   sprintf(str,"%.11g",nodeselsph_z);
   XmTextFieldSetString(nselsphztxt,str);

   sprintf(str,"%.11g",nodeselsph_r);
   XmTextFieldSetString(nselsphrtxt,str);
}


void addnodeselgrplist(int i)
{
  XmString item;

   item = XmStringLtoRCreate(nodegrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListAddItemUnselected(nodegrplist,item,0);
}


void setnodeselgrplist(int i)
{
  XmString item[2];

   item[0] = XmStringLtoRCreate(nodegrpname[i],XmSTRING_DEFAULT_CHARSET);
   XmListReplaceItemsPos(nodegrplist,item,1,i+1);
}
