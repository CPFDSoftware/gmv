#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

void drawrays()

{

  /*  This routine plots rays.  */

  int ii, ip, ic1, goray, ic2, itype, fldflag, npts;
  double tempf, fval, fld_min, fld_max;
  char logflag;
  int incr;
  void raysetfldcolr(double fldval, double fldmin, double fldmax,
                     double tempval, char logflag, int *icl);

   incr = 1;
   /* if (fastdraw == 1 && nrays > 2*incr) incr = drawincr; */

   logflag = rfld_log[rayfieldactive];
   fld_min = rfld_nice_min[rayfieldactive];
   fld_max = rfld_nice_max[rayfieldactive];
   tempf = fld_max - fld_min;
   if (logflag) tempf = log10(fld_max) - log10(fld_min);

   /*  If the ray is colored by a solid color, */
   /*  set the solid color for all rays.       */
   fldflag = 1;
   if (rayflag < 0)
     {
      itype = LSOLID;
      ic1 = rayflag;
      ic2 = ic1;
      fldflag = -1;
     }

   /*  Determine if the ray field is Material.  */
   if (rayflag > 0)
     {
      if (strcmp(rfieldname[rayfieldactive],"Material") == 0)
         fldflag = 0;
     }

   for (ii = 0; ii < nrays; ii++)
     {
      goray = (int) rayselectflag[ii]; 

      if (goray == 1)
        {

         if (numplv+rays[ii].npts > plvarray_alloc)
           {
            plvarray_alloc += nrays + 1000;
            plvarray = (struct plvelem *)realloc(plvarray,
                        plvarray_alloc*sizeof(struct plvelem));
           }

         npts = rays[ii].npts - 1;
         //if (rayvartype[ii] == 1) npts--;

         for (ip = 0; ip < npts; ip += incr)
           {

            /*  Set the ray field color for this segment.  */
            if (fldflag == 0)
              {
               itype = LSOLID;
               ic1 = rays[ii].field[rayfieldactive][ip] - 1;
               ic2 = ic1;
              }
            if (fldflag == 1)
              {
               fval = rays[ii].field[rayfieldactive][ip];
               raysetfldcolr(fval,fld_min,fld_max,tempf,logflag,&ic1);
               if (rayvartype[rayflag] == 1)
                 {
                  itype = LSOLID;
                  ic2 = ic1;
                 }
               else
                 {
                  itype = LSMOOTH;
                  fval = rays[ii].field[rayfieldactive][ip+1];
                  raysetfldcolr(fval,fld_min,fld_max,tempf,logflag,&ic2);
                 }
              }

            /*  Set the line segment.  */
            plvarray[numplv].itype = itype;
            plvarray[numplv].x1 = rays[ii].x[ip];
            plvarray[numplv].y1 = rays[ii].y[ip];
            plvarray[numplv].z1 = rays[ii].z[ip];
            plvarray[numplv].x2 = rays[ii].x[ip+1];
            plvarray[numplv].y2 = rays[ii].y[ip+1];
            plvarray[numplv].z2 = rays[ii].z[ip+1];
            plvarray[numplv].iclr1 = ic1;
            plvarray[numplv].iclr2 = ic2;
            numplv++;
           }
        }
     }
}


void raysetfldcolr
#include "setfldcolr.h"
