#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"
#include "contour.h"

 
void drawisosurface(int i, int transflag)
{
  int j, k, matnum, field_draw, ipt1, ipt2, itype, numiso;
  float v[3], n[3], fact;
  double *field, tempf, fld_min, fld_max, val1, val2, val;
  char logflag;
  int incr, ic1;
  struct trielem *drawiso;
  void issetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval, char logflag, int *icl);
 
   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */

   /*  Determine whether to draw contour/material colors  */
   /*  or field value colors mapped on the surface.       */
   field_draw = -1;
   if (i < MAXCONTOURS)
     {
      field_draw = contourfield_draw;
      field = contourdrawfield;
     }
   else if (i < MAXCONTOURS+MAXMATS)
     {
      field_draw = mcontourfield_draw;
      field = mcontourdrawfield;
     }
   else if (i == MAXCONTOURS+MAXMATS+1)
     {
      field_draw = isovolfield_draw;
      field = isovoldrawfield;
     }

   /*  Set contour/material colors.  */
   if (field_draw == -1)
     {
      if (i < MAXCONTOURS || i >= MAXCONTOURS+MAXMATS)
        { 
         matnum = i;
         if (i == MAXCONTOURS+MAXMATS) matnum = MAXCONTOURS;
         if (i == MAXCONTOURS+MAXMATS + 1) matnum = MAXCONTOURS + 1;
         ic1 = matnum + 200 + MAXMATS;
        }
      else
        {
         matnum = i - MAXCONTOURS;
         ic1 = matnum;         
        }
      itype = LIT_FLAT;
     }

   if (field_draw > -1)
     {
      logflag = nfld_log[field_draw];
      fld_min = nfld_nice_min[field_draw];
      fld_max = nfld_nice_max[field_draw];
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
      itype = LIT_SMOOTH;
     }

   /*  Loop through the triangles for this surface.  */
   j = num_of_contour_polys[i];
   j += 1000;
   if (transflag == 0)
     {
      if (numtris+j > drawarray_alloc)
        {
         drawarray_alloc += j;
         drawarray = (struct trielem *)realloc(drawarray,
                      drawarray_alloc*sizeof(struct trielem));
        }
      drawiso = drawarray;
     }
   if (transflag == 1)
     {
      if (numtris2+j > drawarray2_alloc)
        {
         drawarray2_alloc += j;
         drawarray2 = (struct trielem *)realloc(drawarray2,
                       drawarray2_alloc*sizeof(struct trielem));
        }
      drawiso = drawarray2;
     }
   for (j = 0; j < num_of_contour_polys[i]; j += incr)
     {
 
      n[0] = contourlist[i][j].nx;
      n[1] = contourlist[i][j].ny;
      n[2] = contourlist[i][j].nz;
  
      for (k = 0; k < 3; k++)
        {
         v[0] = contourlist[i][j].vx[k];
         v[1] = contourlist[i][j].vy[k];
         v[2] = contourlist[i][j].vz[k];

         /*  If surface is colored by      */
         /*  field values, set the color.  */
         if (field_draw > -1)
           {
            ipt1 = contourlist[i][j].node1[k];
            ipt2 = contourlist[i][j].node2[k];
            fact = contourlist[i][j].fact[k];
            val1 = field[ipt1];
            val2 = field[ipt2];
            val = val1 + fact*(val2 - val1);
            issetfldcolr(val,fld_min,fld_max,tempf,logflag,&ic1);
           }

         if (k == 0)
           {
            if (transflag == 0) numiso = numtris;
            else numiso = numtris2;
            drawiso[numiso].itype = itype;
            drawiso[numiso].x1 = v[0];
            drawiso[numiso].y1 = v[1];
            drawiso[numiso].z1 = v[2];
            drawiso[numiso].xnorm = n[0];
            drawiso[numiso].ynorm = n[1];
            drawiso[numiso].znorm = n[2];  
            drawiso[numiso].iclr1 = ic1;
           }

         if (k == 1)
           {
            drawiso[numiso].x2 = v[0];
            drawiso[numiso].y2 = v[1];
            drawiso[numiso].z2 = v[2];
            drawiso[numiso].iclr2 = ic1;
           }

         if (k == 2)
           {
            drawiso[numiso].x3 = v[0];
            drawiso[numiso].y3 = v[1];
            drawiso[numiso].z3 = v[2];
            drawiso[numiso].iclr3 = ic1;
            if (transflag == 0) numtris++;
            else numtris2++;
           }
        }
     }
}


void issetfldcolr
#include "setfldcolr.h"
