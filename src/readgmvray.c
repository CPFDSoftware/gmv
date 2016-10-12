#include <Xm/Xm.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gmvdata.h"
#include "gmvread.h"
#include "gmvrayread.h"
#include "formwidgets.h"
#include "xstuff.h"

#include <sys/types.h>
//#include <malloc.h>
#include <math.h>

#define MAXVAL 9.99999e32

static char rayfilnam[256];


void rdray(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string, string2, string3;

   fileselflag = GMVRAY;
   string = XmStringCreate("Read GMV RAY file",
                           XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[GMVRAY],
                            XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[GMVRAY],
                            XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                            XmNpattern, string2, 
                            XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);

   if (fsread[GMVRAY])
     {
      string = XmStringCreate(rayfilnam,
                                    XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
      XmStringFree(string);
     }

   MyManageChild(file_sel2);
}


int readrayfile()
{
  int iray, i, j, nvar, ierr;
  double fmin, fmax, f;

   /*  Clear previous ray data, if any.  */
   if (nrays > 0)
     {

      /*  Reset ray data.  */
      for (iray = 0; iray < nrays; iray++)
        {
         FREE(rays[iray].x);
         FREE(rays[iray].y);
         FREE(rays[iray].z);
         for (i = 0; i < rnumvars; i++)
           FREE(rays[iray].field[i]);
         rays[iray].npts = 0;
        }
      /* FREE(rays); */
      nrays = 0;
      rnumvars = 0;
     }
   rayidflag = 0;

   /*  Read ray file.  */
   gmvrayread_data();

   /*  Check for errors.  */
   if (gmvray_data.nvars == -1) return 1;

   /*  Save ray data.  */
   nrays = gmvray_data.nrays;
   rnumvars = gmvray_data.nvars;
   for (i = 0; i < rnumvars; i++)
     {
      rfieldname[i]=(char *)malloc(33*sizeof(char));
      if (rfieldname[i] == NULL) memerr();
      strncpy(rfieldname[i],&gmvray_data.varnames[i*33],32);
      *(rfieldname[i]+32) = (char) 0;
      rayvartype[i] = gmvray_data.vartype[i];
     }
   rays = gmvray_data.gmvrays;

   rayidflag = 1;
   rayids = gmvray_data.rayids;

   /*  Get the ray field mins. and maxs.  */
   for (i = 0; i < rnumvars; i++)
     {
      fmin = MAXVAL;
      fmax = -MAXVAL;
      for (iray = 0; iray < nrays; iray++)
        {
         nvar = rays[iray].npts;
         if (rayvartype[i] == 1) nvar--;
         for (j = 0; j < nvar; j++)
           {
            fmin = (rays[iray].field[i][j] < fmin) ? 
                    rays[iray].field[i][j] : fmin;
            fmax = (rays[iray].field[i][j] > fmax) ? 
                    rays[iray].field[i][j] : fmax;
           }
        }
      rfieldmin[i] = fmin;
      rfieldmax[i] = fmax;
      globfieldrmin[i] = rfieldmin[i];
      globfieldrmax[i] = rfieldmax[i];
      if (globfieldrmax[i] <= globfieldrmin[i])
        { 
         f = fabs(globfieldrmin[i]);
         if (f == 0.) f = 1.;
         globfieldrmax[i] = globfieldrmin[i] + f*0.00001;
         rfieldmax[i] = globfieldrmax[i];
        }
      rfld_nice_min[i] = globfieldrmin[i];   
      rfld_nice_max[i] = globfieldrmax[i];   
     }

   rayselectflag = (char *)malloc(nrays*sizeof(char));
   if (rayselectflag == NULL) memerr();
   for (i = 0; i < nrays; i++)
      rayselectflag[i] = 1;
   rayselnum_on = 0;
   for (i = 0; i < MAXRAYFIELDS; i++) rfieldlimauto[i] = 0;
   return 0;
}


void readgmvray(char *fname)
{
  /*                              */
  /*  Read a GMV ray input file.  */
  /*                              */
  int ierr;
  XmString string, string1, string2;
  char str[] = 
   "Error - this is not a GMV ray file.\nTry another file.";
  char str2[] = 
   "Error - cannot open file: ";
  char str3[] = 
   "Error - rayend not found.\nTry another file.";

   strcpy(rayfilnam,fname);

   /*  Check that the file is a valid GMV ray file.  */
   ierr = gmvrayread_open(rayfilnam);

   if (ierr > 0)
     {
      fprintf(stderr,"Error reading gmv ray file %s\n",rayfilnam);
      gmvrayread_close();

      if (ierr == 1)
        {
         errorfile = 2;
         string1 = XmStringLtoRCreate(str2,
                                     XmSTRING_DEFAULT_CHARSET);
         string2 = XmStringLtoRCreate(rayfilnam,
                                     XmSTRING_DEFAULT_CHARSET);
         string = XmStringConcat(string1,string2);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         XmStringFree(string1);
         XmStringFree(string2);
        }
      if (ierr == 2)
        {
         errorfile = 2;
         string = XmStringLtoRCreate(str,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
      if (ierr == 3)
        {
         errorfile = 2;
         string = XmStringLtoRCreate(str3,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
        }
      MyManageChild(errorform);
      /*  MyManageChild(file_sel2);  */
      return;
     }

   printf("GMV reading ray file %s\n",rayfilnam);

   /*  Reset drawing flags and widgets.  */
   if (nrays > 0)
     {

      raysflag = 0;  drawrayflag = 0;
      XUndefineCursor(dpy,topwin);
      XtUnmanageChild(rayform);
      XtUnmanageChild(raylimform);
      XtDestroyWidget(rayform);
      XtDestroyWidget(raylimform);
      XtSetSensitive(dispray,FALSE);
      XtSetSensitive(raylimbut,FALSE);
     }

   /*  Clear drawing and selection flags.  */
   drawrayflag = 0;
   rayflag = 0;
   rayfieldactive = 0;
   nrayselnum = 0;
   rayselnum_on = 0;

   /*  Read the ray file.  */
   ierr = readrayfile();
   if (ierr == 1) return;

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();

   /*  Build new ray widgets and display the ray menu.  */
   if (nrays > 0)
     {
      XtSetSensitive(dispray,TRUE);
      XtSetSensitive(raylimbut,TRUE);
      makerayform(poplevel);
      makeraylimform(poplevel2);
     }
   rayformpop();
}
