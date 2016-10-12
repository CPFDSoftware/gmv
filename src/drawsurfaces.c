#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

#define MAXVAL 9.99999e32
#define MAXFACES 1000

static double tempf, fld_min, fld_max;
static int  incr, fldflag, sactive, allvars;
static short matnum, explodeflag, contflag, *matcolr;
static char logflag, lineflag;
void sfsetfldcolr(double fldval, double fldmin, double fldmax, double tempval, 
                  char logflag, int *icl);


void drawsurfaces()
{
 
  /*  This routine draws colored contours accross surfaces.  */
 
  void drawsurflines();
  void drawfacets();

   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */

   allvars = numvars + snumvars;
   sactive = surffieldactive - numvars;

   fldflag = 0;
   if (surffieldactive > -1 && surffieldactive < numvars)
     {
      fldflag = 1;
      lineflag = nfld_nice[surffieldactive];
      logflag = nfld_log[surffieldactive];
      fld_min = nfld_nice_min[surffieldactive];
      fld_max = nfld_nice_max[surffieldactive];
     }
   if (sactive > -1 && sactive < snumvars)
     {
      fldflag = 2;
      lineflag = sfld_nice[sactive];
      logflag = sfld_log[sactive];
      fld_min = sfld_nice_min[sactive];
      fld_max = sfld_nice_max[sactive];
     }

   if (fldflag > 0)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (surffieldactive == -2) matcolr = surfgrpcolr;
   else matcolr = surfmats;

   /*  Check for explode for these surfaces.  */
   explodeflag = 0;
   if (sexplodefact > 0.) explodeflag = 1;

   /*  Reset contour flag if necessary.  */
   contflag = surfcontflag;
   if (incr > 1 || surffieldactive == -1 || surffieldactive >= numvars)
      contflag = 0;

   /*  Draw field contour lines and facet  */
   /*  edges on surfacess, if requested.   */
   if (contflag == 1 || surfedgesflag == 1)
      drawsurflines();
 
   /*  Draw selected faces of selected facets.  */
   if (surfflag == 1)
      drawfacets();
}


void drawsurflines()
{
 
  /*  This routine draws contour lines across   */
  /*  surface facets and/or facet edges.  This  */
  /*  routine will also draw the white probe    */
  /*  selection of surface facets.              */
 
  int j, k, isurf, gosurf, iex, ncl, nfverts, icol;
  float expx, expy, expz;
  double clv[1000][3];
  double fval, s, denom, vmin, vmax, intervals[100];
  double val1, val2, x1, y1, z1, x2, y2, z2;
  int nsets, ipt1, ipt2, m, nintervals;
  void getscale(double llimit, double ulimit,int max, char lineflag,
                char logflag, double *intervals, int *nintervals);

   /*  Call getscale to get the number of intervals.  */
   getscale(fld_min, fld_max, nset_contours, lineflag, logflag,
            intervals, &nintervals);
   nsets = nintervals;

   /*  Loop through surfaces and operate on selected facets.  */
   for (isurf = 0; isurf < nsurf; isurf += incr)
     {

      /*  See if the surface has been selected for display.  */
      gosurf = (int)surfselectflag[isurf];

      /*  Eliminate surfacess with material < 1  */ 
      /*  for regular unless requested.          */
      if (surf0matflag == 0 && matcolr[isurf] < 1) 
         gosurf = 0;

      /*  Process selected surface.  */
      if (gosurf == 1)
        {

         /*  Determine explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0.;
         iex = matnum;
         if (explodeflag)
           {
            if (surfexpflag == -1) 
               iex = (int)surfmats[isurf] - 1;
            else iex = (int)surfflags[surfexpflag][isurf] - 1;
            expx = sexplodefact*surfexpx[iex];
            expy = sexplodefact*surfexpy[iex];
            expz = sexplodefact*surfexpz[iex];
           }

         /*  Loop over suface nodes.  */
         nfverts = surfs[isurf].nverts;

         /*  Draw contour lines if requested.  */
         if (contflag == 1 && nfverts > 2)
           {
            if (txtcolor < 256) icol = -1;
            else icol = -2;

            /*  Loop over vertices in the facet to   */
            /*  get facet min. and max field value.  */
            vmin = MAXVAL;  vmax = -MAXVAL;
            for (j = 0; j < nfverts; j++)
              {
               k = surfs[isurf].fverts[j];
               val1 = surffield[k];
               vmin = (val1 < vmin) ? val1 : vmin;
               vmax = (val1 > vmax) ? val1 : vmax;
              }
           
            /*  Loop over nsets contour lines.  */
            for (m = 1; m < nsets; m++)
              {
               fval = intervals[m];
               ncl = 0;

               /*  If contour level lies between surface min.   */
               /*  and max. field value, loop over vertices in  */
               /*  the facet to draw the contour line.          */ 
               if (vmin <= fval && fval <= vmax && vmin < vmax)
                 {
                  for (j = 0; j < nfverts; j++)
                    {
                     ipt1 = surfs[isurf].fverts[j];
                     val1 = surffield[ipt1];
                     if (j < nfverts - 1)
                        ipt2 = surfs[isurf].fverts[j+1];
                     else
                        ipt2 = surfs[isurf].fverts[0];
                     val2 = surffield[ipt2];

                     if ((val1 <= fval && fval <= val2) ||
                         (val2 <= fval && fval <= val1))
                       {
                        x1 = nodes[ipt1].x + expx;
                        y1 = nodes[ipt1].y + expy;
                        z1 = nodes[ipt1].z + expz;
                        x2 = nodes[ipt2].x + expx;
                        y2 = nodes[ipt2].y + expy;
                        z2 = nodes[ipt2].z + expz;

                        denom = val1 - val2;
                        s = 1.1;
                        if (fabs((double)denom) > 1.0e-20)
                          {
                           s = (val1 - fval) / denom;
                           if (val1 == fval) s = 0.;
                              if (val2 == fval) s = 1.;
                          }
                        if (s >= 0. && s <= 1.)
                          {
                           clv[ncl][0] = x1 + s * (x2 - x1);
                           clv[ncl][1] = y1 + s * (y2 - y1);
                           clv[ncl][2] = z1 + s * (z2 - z1);
                           ncl++;
                          }
                       }
                    }
                 }
               if (ncl > 0)
                 {
                  if (numplv+ncl > plvarray_alloc)
                    {
                     plvarray_alloc += 10000;
                     plvarray = (struct plvelem *)realloc(plvarray,
                                 plvarray_alloc*sizeof(struct plvelem));
                    }
                  for (j = 0; j < ncl-1; j++)
                    {
                     plvarray[numplv].itype = LSOLID;
                     plvarray[numplv].x1 = clv[j][0];
                     plvarray[numplv].y1 = clv[j][1];
                     plvarray[numplv].z1 = clv[j][2];
                     plvarray[numplv].x2 = clv[j+1][0];
                     plvarray[numplv].y2 = clv[j+1][1];
                     plvarray[numplv].z2 = clv[j+1][2];
                     plvarray[numplv].iclr1 = icol;
                     plvarray[numplv].iclr2 = icol;
                     numplv++;
                    }
                 }
              }
           }
                  
         /*  Draw facet edges in black, or grey if contours on.  */
         if (surfedgesflag == 1)
           {
            if (contflag == 1) 
               icol = -3; 
            else
               icol = -1; 

            /*  Loop over facet vertices and draw edges.  */
            j = nfverts + 1;
            if (numplv+j > plvarray_alloc)
              {
               plvarray_alloc += 10000;
               plvarray = (struct plvelem *)realloc(plvarray,
                           plvarray_alloc*sizeof(struct plvelem));
              }
            for (j = 0; j < nfverts; j++)
              {
               ipt1 = surfs[isurf].fverts[j];
               if (j < nfverts-1) 
                  ipt2 = surfs[isurf].fverts[j+1];
               else
                  ipt2 = surfs[isurf].fverts[0];
               plvarray[numplv].itype = LSOLID;
               plvarray[numplv].x1 = nodes[ipt1].x + expx;
               plvarray[numplv].y1 = nodes[ipt1].y + expy;
               plvarray[numplv].z1 = nodes[ipt1].z + expz;
               plvarray[numplv].x2 = nodes[ipt2].x + expx;
               plvarray[numplv].y2 = nodes[ipt2].y + expy;
               plvarray[numplv].z2 = nodes[ipt2].z + expz;
               plvarray[numplv].iclr1 = icol;
               plvarray[numplv].iclr2 = icol;
               numplv++;
              }
           }
               
         /*  Draw white selection indicator from center to edges.  */
         if (surfselprobe_on == 1 && surfselectflagp[isurf] == 1)
           {
            icol = -2; 

            /*  Loop over facet vertices and      */
            /*  draw lines from center to edges.  */
            j = nfverts + 1;
            if (numplv+j > plvarray_alloc)
              {
               plvarray_alloc += 10000;
               plvarray = (struct plvelem *)realloc(plvarray,
                           plvarray_alloc*sizeof(struct plvelem));
              }
            for (j = 0; j < nfverts; j++)
              {
               ipt1 = surfs[isurf].fverts[j];
               plvarray[numplv].itype = LSOLID;
               plvarray[numplv].x1 = surfs[isurf].xavg + expx;
               plvarray[numplv].y1 = surfs[isurf].yavg + expy;
               plvarray[numplv].z1 = surfs[isurf].zavg + expz;
               plvarray[numplv].x2 = nodes[ipt1].x + expx;
               plvarray[numplv].y2 = nodes[ipt1].y + expy;
               plvarray[numplv].z2 = nodes[ipt1].z + expz;
               plvarray[numplv].iclr1 = icol;
               plvarray[numplv].iclr2 = icol;
               numplv++;
              }
           }
        }
     }
}


void drawfacets()
{
 
  /*  This routine draws surface facets.  */
 
  int j, k, k2, isurf, gosurf, iex, nfverts, itype, 
      ic1, ic2, ic3, transflag;
  double v1[3], v2[3], v3[3]; 
  float expx, expy, expz, norm[3];
  int jj;
  double fvalavg;

   for (isurf = 0; isurf < nsurf; isurf += incr)
     {

      /*  See if the surface has been selected for display.  */
      gosurf = (int)surfselectflag[isurf];

      /*  Eliminate surfaces with material  */ 
      /*  or group < 1 unless requested.    */
      if (surf0matflag == 0 && matcolr[isurf] < 1) 
         gosurf = 0;

      /*  Process selected facet.  */
      if (gosurf == 1)
        {

         /*  Determine explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0.;
         iex = matnum;
         if (explodeflag)
           {
            if (surfexpflag == -1) 
               iex = (int)surfmats[isurf] - 1;
            else iex = (int)surfflags[surfexpflag][isurf] - 1;
            expx = sexplodefact*surfexpx[iex];
            expy = sexplodefact*surfexpy[iex];
            expz = sexplodefact*surfexpz[iex];
           }

         /*  If drawing materials or flags, set colors.  */
         transflag = 0;
         if (fldflag == 0)
           {
            matnum = matcolr[isurf] - 1;
            if (surffieldactive >= allvars)
               matnum = surfflags[surffieldactive-allvars][isurf] - 1;
            ic1 = matnum;
            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            ic3 = ic2 = ic1;
            jj = 4 * ic1;
            if (matrgba[3+jj] < 1.) transflag = 1;
            itype = UNLIT_FLAT;
            if (surfshadeflag) itype = LIT_FLAT;
           }

         /*  If drawing node field data, set flags.  */
         if (fldflag == 1)
           {
            itype = UNLIT_SMOOTH;
            if (surfshadeflag) itype = LIT_SMOOTH;
           }

         /*  If drawing surface field data, set flags.  */
         if (fldflag == 2)
           {
            sfsetfldcolr(surffield[isurf],fld_min,fld_max,tempf,
                         logflag,&ic1);
            ic3 = ic2 = ic1;
            itype = UNLIT_FLAT;
            if (surfshadeflag) itype = LIT_FLAT;
           }

         /*  Loop over surface nodes.  */
         nfverts = surfs[isurf].nverts;

         /*  Set normals for shaded facets.  */
         norm[0] = norm[1] = norm[2] = 0.;
         if (surfshadeflag)
           {
            norm[0] = surfs[isurf].xnorm;
            norm[1] = surfs[isurf].ynorm;
            norm[2] = surfs[isurf].znorm;
           }

         /*  Begin facet vertices.  */

         /*  If face refinement not on, loop over  */
         /*  the face verices and make triangles.  */
         j = nfverts * 2;
         if (!transflag)
           {
            if (numtris+j > drawarray_alloc)
              {
               drawarray_alloc += 10000;
               drawarray = (struct trielem *)realloc(drawarray,
                           drawarray_alloc*sizeof(struct trielem));
              }
           }
         if (transflag)
           {
            if (numtris2+j > drawarray2_alloc)
              {
               drawarray2_alloc += 10000;
               drawarray2 = (struct trielem *)realloc(drawarray2,
                            drawarray2_alloc*sizeof(struct trielem));
              }
           }
         if (!surfrefineflag && nfverts > 2)
           {

            k = surfs[isurf].fverts[0];
            v1[0] = nodes[k].x + expx;
            v1[1] = nodes[k].y + expy;
            v1[2] = nodes[k].z + expz;

            /* If a node field is to be drawn, reset the color. */
            if (fldflag == 1)
              {
               sfsetfldcolr(surffield[k],fld_min,fld_max,tempf,logflag,
                            &ic1);
              }
            for (j = 1; j < nfverts-1; j++)
              {
               k = surfs[isurf].fverts[j];
               v2[0] = nodes[k].x + expx;
               v2[1] = nodes[k].y + expy;
               v2[2] = nodes[k].z + expz;

               /* If a node field is to be drawn, reset the color. */
               if (fldflag == 1)
                 {
                  sfsetfldcolr(surffield[k],fld_min,fld_max,tempf,logflag,
                               &ic2);
                 }

               k = surfs[isurf].fverts[j+1];
               v3[0] = nodes[k].x + expx;
               v3[1] = nodes[k].y + expy;
               v3[2] = nodes[k].z + expz;

               /* If a node field is to be drawn, reset the color. */
               if (fldflag == 1)
                 {
                  sfsetfldcolr(surffield[k],fld_min,fld_max,tempf,logflag,
                               &ic3);
                 }

               if (!transflag)
                 {
                  drawarray[numtris].itype = itype;
                  drawarray[numtris].x1 = v1[0];
                  drawarray[numtris].y1 = v1[1];
                  drawarray[numtris].z1 = v1[2];
                  drawarray[numtris].x2 = v2[0];
                  drawarray[numtris].y2 = v2[1];
                  drawarray[numtris].z2 = v2[2];
                  drawarray[numtris].x3 = v3[0];
                  drawarray[numtris].y3 = v3[1];
                  drawarray[numtris].z3 = v3[2];
                  drawarray[numtris].xnorm = norm[0];
                  drawarray[numtris].ynorm = norm[1];
                  drawarray[numtris].znorm = norm[2];
                  drawarray[numtris].iclr1 = ic1;
                  drawarray[numtris].iclr2 = ic2;
                  drawarray[numtris].iclr3 = ic3;
                  numtris++;
                 }

               if (transflag)
                 {
                  drawarray2[numtris2].itype = itype;
                  drawarray2[numtris2].x1 = v1[0];
                  drawarray2[numtris2].y1 = v1[1];
                  drawarray2[numtris2].z1 = v1[2];
                  drawarray2[numtris2].x2 = v2[0];
                  drawarray2[numtris2].y2 = v2[1];
                  drawarray2[numtris2].z2 = v2[2];
                  drawarray2[numtris2].x3 = v3[0];
                  drawarray2[numtris2].y3 = v3[1];
                  drawarray2[numtris2].z3 = v3[2];
                  drawarray2[numtris2].xnorm = norm[0];
                  drawarray2[numtris2].ynorm = norm[1];
                  drawarray2[numtris2].znorm = norm[2];
                  drawarray2[numtris2].iclr1 = ic1;
                  drawarray2[numtris2].iclr2 = ic2;
                  drawarray2[numtris2].iclr3 = ic3;
                  numtris2++;
                 }
              }
           }

         /*  If face refinement on, get face averages   */
         /*  and set first vertex to be at the center.  */
         if (surfrefineflag && nfverts > 2)
           {
            v1[0] = 0.;  v1[1] = 0.;  v1[2] = 0.;  fvalavg = 0.;
            for (j = 0; j < nfverts; j++)
              {
               k = surfs[isurf].fverts[j];
               v1[0] += nodes[k].x + expx;
               v1[1] += nodes[k].y + expy;
               v1[2] += nodes[k].z + expz;
               fvalavg += surffield[k];
              }
            v1[0] /= (float)nfverts;
            v1[1] /= (float)nfverts;
            v1[2] /= (float)nfverts;
            fvalavg /= (float)nfverts;

            /*  Set node field color for face average.  */
            if (fldflag == 1)
               sfsetfldcolr(fvalavg,fld_min,fld_max,tempf,logflag,&ic1);

            /*  Loop through face vertices.  */
            for (j = 0; j < nfverts; j++)
              {
               k = surfs[isurf].fverts[j];
               v2[0] = nodes[k].x + expx;
               v2[1] = nodes[k].y + expy;
               v2[2] = nodes[k].z + expz;

               if (fldflag == 1) sfsetfldcolr(surffield[k],fld_min,fld_max,
                                              tempf,logflag,&ic2);

               if (j < nfverts - 1)
                  k2 = surfs[isurf].fverts[j+1];
               else
                  k2 = surfs[isurf].fverts[0];
               v3[0] = nodes[k2].x + expx;
               v3[1] = nodes[k2].y + expy;
               v3[2] = nodes[k2].z + expz;

               if (fldflag == 1) sfsetfldcolr(surffield[k2],fld_min,fld_max,
                                              tempf,logflag,&ic3);

               drawarray[numtris].itype = itype;
               drawarray[numtris].x1 = v1[0];
               drawarray[numtris].y1 = v1[1];
               drawarray[numtris].z1 = v1[2];
               drawarray[numtris].x2 = v2[0];
               drawarray[numtris].y2 = v2[1];
               drawarray[numtris].z2 = v2[2];
               drawarray[numtris].x3 = v3[0];
               drawarray[numtris].y3 = v3[1];
               drawarray[numtris].z3 = v3[2];
               drawarray[numtris].xnorm = norm[0];
               drawarray[numtris].ynorm = norm[1];
               drawarray[numtris].znorm = norm[2];
               drawarray[numtris].iclr1 = ic1;
               drawarray[numtris].iclr2 = ic2;
               drawarray[numtris].iclr3 = ic3;
               numtris++;
             }
           }

         /*  If two vertices, draw the line.   */
         /*  and set first vertex to be at the center.  */
         if (numplv+1 > plvarray_alloc)
           {
            plvarray_alloc += 10000;
            plvarray = (struct plvelem *)realloc(plvarray,
                        plvarray_alloc*sizeof(struct plvelem));
           }
         if (nfverts == 2)
           {
            k = surfs[isurf].fverts[0];
            v1[0] = nodes[k].x + expx;
            v1[1] = nodes[k].y + expy;
            v1[2] = nodes[k].z + expz;

            if (fldflag == 1) sfsetfldcolr(surffield[k],fld_min,fld_max,
                                           tempf,logflag,&ic1);

            k = surfs[isurf].fverts[1];
            v2[0] = nodes[k].x + expx;
            v2[1] = nodes[k].y + expy;
            v2[2] = nodes[k].z + expz;

            if (fldflag == 1) sfsetfldcolr(surffield[k],fld_min,fld_max,
                                           tempf,logflag,&ic2);

            plvarray[numplv].itype = LSMOOTH;
            plvarray[numplv].x1 = v1[0];
            plvarray[numplv].y1 = v1[1];
            plvarray[numplv].z1 = v1[2];
            plvarray[numplv].x2 = v2[0];
            plvarray[numplv].y2 = v2[1];
            plvarray[numplv].z2 = v2[2];
            plvarray[numplv].iclr1 = ic1;
            plvarray[numplv].iclr2 = ic2;
            numplv++;
           }

        }
     }
}


void sfsetfldcolr
#include "setfldcolr.h"
