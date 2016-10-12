#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

static double tempf, *field, fld_min, fld_max;
static int incr, fieldactive, fldflag, cactive, allvars, factive;
static char logflag, lineflag;
static short matnum, contflag, explodeflag, edgeflag, medianflag,
             medianedgeflag, voronoiflag, voronoiedgeflag, *matcolr;
static int drawtype;
static char *selectflag;
void cfsetfldcolr(double fldval, double fldmin, double fldmax, double tempval, 
                  char logflag, int *icl);
void voronoipt(int jcell, double vpt[3]), 
     voronoifacept(int jface, double vpt[3]);


void drawcellfaces(int drawtypein)
{
 
  /*  This routine draws colored contours accross cell    */
  /*  faces for regular cells (drawtype = 0) or for grid  */
  /*  analysis cells (drawtype = 1).                      */
 
  void drawfacelines();
  /* void drawmedianlines(); */
  void drawfaces();
  void drawaltfaces(int meshtype);
  void draw2dunsel();
 
   drawtype = drawtypein;

   incr = 1;
   /* if (fastdraw == 1 && drawtype == 0) incr = drawincr; */

   if (drawtype == 0)
     {
      fieldactive = cellfieldactive;
      selectflag = cellfacedraw;
      field = cellfield;
      contflag = cellcontflag;
      edgeflag = celledgesflag;
      medianflag = 0;
      medianedgeflag = cellmedianedgeflag;
      voronoiflag = 0;
      voronoiedgeflag = 0;
     }
   if (drawtype == 1)
     {
      fieldactive = gridfieldactive;
      selectflag = gridselectflag;
      field = gridfield;
      contflag = gridcontflag;
      edgeflag = gridedgesflag;
      medianflag = gridmedianflag;
      medianedgeflag = gridmedianedgeflag;
      voronoiflag = gridvoronoiflag;
      voronoiedgeflag = gridvoronoiedgeflag;
     }

   allvars = numvars + cnumvars + fnumvars;
   cactive = fieldactive - numvars;
   factive = fieldactive - (numvars + cnumvars);
   fldflag = 0;

   /*  Determine min and max scale values for field data (if any). */
   if (fieldactive > -1 && fieldactive < numvars)
     {
      fldflag = 1;
      lineflag = nfld_nice[fieldactive];
      logflag = nfld_log[fieldactive];
      fld_min = nfld_nice_min[fieldactive];
      fld_max = nfld_nice_max[fieldactive];
     }

   if (cactive > -1 && cactive < cnumvars)
     {
      fldflag = 2;
      lineflag = cfld_nice[cactive];
      logflag = cfld_log[cactive];
      fld_min = cfld_nice_min[cactive];
      fld_max = cfld_nice_max[cactive];
     }

   if (factive > -1 && factive < fnumvars)
     {
      fldflag = 3;
      lineflag = ffld_nice[factive];
      logflag = ffld_log[factive];
      fld_min = ffld_nice_min[factive];
      fld_max = ffld_nice_max[factive];
     }

   if (fldflag > 0)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (fieldactive == -1 || fieldactive == allvars) matcolr = cellmats;
   if (fieldactive == -2) matcolr = cellgrpcolr;

   /*  Set fldflag for face group color.  */
   if (fieldactive == -3) fldflag = 4;

   /*  Check for explode for these cells.  */
   explodeflag = 0;
   if (cexplodefact > 0.) explodeflag = 1;

   /*  Reset contour flag if necessary.  */
   if (incr > 1 || fieldactive == -1 || fieldactive >= numvars)
      contflag = 0;

   /*  Draw field contour lines, cell edges, median mesh edges  */
   /*  or voronoi mesh edges on cell faces, if requested.       */
   if ((medianflag == 0 && voronoiflag == 0) && (contflag == 1 || 
       edgeflag == 1 || medianedgeflag == 1 || voronoiedgeflag == 1))
      drawfacelines();
 
   /*  Draw selected faces of selected cells.  */
   if (cellsflag == 1 || gridsflag == 1)
      drawfaces();

   /*  If 2D mesh, and draw unselected flag on, draw unselected faces.  */
   if (cellsflag == 1 && mesh2dflag > 0 && drawunselcells)
      draw2dunsel();
 
   /*  Draw median faces of selected cells.  */
   if (medianflag == 1)
      drawaltfaces(0);
 
   /*  Draw voronoi faces of selected cells.  */
   if (voronoiflag == 1)
      drawaltfaces(1);
}


void drawfacelines()
{
 
  /*  This routine draws contour lines across cell faces  */
  /*  and/or cell edges for regular cells (drawtype = 0)  */
  /*  or for grid analysis cells (drawtype = 1).          */
 
  int i, j, k, icell, jcell, gocell, iex, iface, jface, ncl, nfverts,
      icol;
  float expx, expy, expz;
  double v1[3], v2[3], clv[1000][3];
  double fval, s, denom, vmin, vmax, intervals[100];
  double val1, val2, x1, y1, z1, x2, y2, z2;
  int nsets, ipt1, ipt2, m, nintervals;
  void getscale(double llimit, double ulimit,int max, char lineflag,
                char logflag, double *intervals, int *nintervals);

   /*  Call getscale to get the number of intervals.  */
   getscale(fld_min, fld_max, nset_contours, lineflag, logflag,
            intervals, &nintervals);
   nsets = nintervals;

   /*  Loop through cells and operate on selected cells.  */
   for (icell = 0; icell < ncells; icell += incr)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)selectflag[icell];

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Determine explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0.;
         if (explodeflag)
           {
            if (cellexpflag == -1) 
               iex = (int)cellmats[icell] - 1;
            else iex = (int)cellflags[cellexpflag][icell] - 1;
            expx = cexplodefact*cellexpx[iex];
            expy = cexplodefact*cellexpy[iex];
            expz = cexplodefact*cellexpz[iex];
           }

         /*  Loop over cell faces.  */
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];

            /*  Process face if selected.  */
            k = 0;
            if (drawtype == 1) k = 1;
            else
              {
               if ((int)facesel[iface] == 1) k = 1;
              }
            if (k == 1)
              {
               jface = iface;
               if (structuredflag == 1) jface = i;

               nfverts = cellfaces[jface].nverts;

               /*  Draw contour lines if requested.  */
               if (contflag == 1 && nfverts > 2)
                 {
                  if (txtcolor < 256) icol = -1;
                  else icol = -2;

                  /*  Loop over vertices in the face to   */
                  /*  get face min. and max field value.  */
                  vmin = MAXVAL;  vmax = -MAXVAL;
                  for (j = 0; j < nfverts; j++)
                    {
                     k = cellfaces[jface].fverts[j];
                     val1 = field[k];
                     vmin = (val1 < vmin) ? val1 : vmin;
                     vmax = (val1 > vmax) ? val1 : vmax;
                    }
           
                  /*  Loop over nsets contour lines.  */
                  for (m = 1; m < nsets; m++)
                    {
                     fval = intervals[m];
                     ncl = 0;

                     /*  If contour level lies between cell face min.  */
                     /*  and max. field value, loop over vertices in   */
                     /*  the face to draw the contour line.            */ 
                     if (vmin <= fval && fval <= vmax && vmin < vmax)
                       {
                        for (j = 0; j < nfverts; j++)
                          {
                           ipt1 = cellfaces[jface].fverts[j];
                           val1 = field[ipt1];
                           if (j < nfverts - 1)
                              ipt2 = cellfaces[jface].fverts[j+1];
                           else
                              ipt2 = cellfaces[jface].fverts[0];
                           val2 = field[ipt2];

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
                  
               /*  Draw face edges in black, or grey if contours on.  */
               if (edgeflag == 1)
                 {
                  if (contflag == 1) 
                     icol = -3; 
                  else
                     icol = -1;

                  /*  Loop over face vertices and draw edges.  */
                  j = nfverts + 1;
                  if (numplv+j > plvarray_alloc)
                    {
                     plvarray_alloc += 10000;
                     plvarray = (struct plvelem *)realloc(plvarray,
                                 plvarray_alloc*sizeof(struct plvelem));
                    }
                  for (j = 0; j < nfverts; j++)
                    {
                     ipt1 = cellfaces[jface].fverts[j];
                     if (j < nfverts-1) 
                        ipt2 = cellfaces[jface].fverts[j+1];
                     else
                        ipt2 = cellfaces[jface].fverts[0];
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

               /*  Draw median or voronoi mesh    */ 
               /*  edges in white, if requested.  */
               if (((medianflag == 0 && medianedgeflag == 1) ||
                   (voronoiflag == 0 && voronoiedgeflag == 1)) &&
                   nfverts > 2)
                 {
                  icol = -2;

                  /*  For median mesh, calculate    */
                  /*  face center (average) point.  */
                  if (medianedgeflag)
                    {
                     for (j = 0; j <= nfverts; j++)
                       {
                        v1[0] = 0.;  v1[1] = 0.;  v1[2] = 0.;
                        for (j = 0; j < nfverts; j++)
                          {
                           k = cellfaces[jface].fverts[j];
                           v1[0] += nodes[k].x + expx;
                           v1[1] += nodes[k].y + expy;
                           v1[2] += nodes[k].z + expz;
                          }
                        v1[0] /= (float)nfverts;
                        v1[1] /= (float)nfverts;
                        v1[2] /= (float)nfverts;
                       }

                     /*  Loop over face vertices and draw median mesh.  */
                     j = nfverts + 1;
                     if (numplv+j > plvarray_alloc)
                       {
                        plvarray_alloc += 10000;
                        plvarray = (struct plvelem *)realloc(plvarray,
                                    plvarray_alloc*sizeof(struct plvelem));
                       }
                     for (j = 0; j < nfverts; j++)
                       {
                        ipt1 = cellfaces[jface].fverts[j];
                        if (j < nfverts - 1)
                           ipt2 = cellfaces[jface].fverts[j+1];
                        else
                           ipt2 = cellfaces[jface].fverts[0];
                        v2[0] = 0.5*(nodes[ipt1].x + nodes[ipt2].x) + expx;
                        v2[1] = 0.5*(nodes[ipt1].y + nodes[ipt2].y) + expy;
                        v2[2] = 0.5*(nodes[ipt1].z + nodes[ipt2].z) + expz;

                        plvarray[numplv].itype = LSOLID;
                        plvarray[numplv].x1 = v1[0];
                        plvarray[numplv].y1 = v1[1];
                        plvarray[numplv].z1 = v1[2];
                        plvarray[numplv].x2 = v2[0];
                        plvarray[numplv].y2 = v2[1];
                        plvarray[numplv].z2 = v2[2];
                        plvarray[numplv].iclr1 = icol;
                        plvarray[numplv].iclr2 = icol;
                        numplv++;
                       }
                    }

                  /*  For voronoi mesh, calculate  */
                  /*  face voronoi point.          */
                  if (voronoiedgeflag)
                    {
                     voronoifacept(jface,v1);
                     v1[0] += expx;
                     v1[1] += expy;
                     v1[2] += expz;

                     /*  Loop over face vertices and draw voronoi mesh.  */
                     j = nfverts + 1;
                     if (numplv+j > plvarray_alloc)
                       {
                        plvarray_alloc += 10000;
                        plvarray = (struct plvelem *)realloc(plvarray,
                                    plvarray_alloc*sizeof(struct plvelem));
                       }
                     for (j = 0; j < nfverts; j++)
                       {
                        ipt1 = cellfaces[jface].fverts[j];
                        if (j < nfverts - 1)
                           ipt2 = cellfaces[jface].fverts[j+1];
                        else
                           ipt2 = cellfaces[jface].fverts[0];
                        v2[0] = 0.5*(nodes[ipt1].x + nodes[ipt2].x) + expx;
                        v2[1] = 0.5*(nodes[ipt1].y + nodes[ipt2].y) + expy;
                        v2[2] = 0.5*(nodes[ipt1].z + nodes[ipt2].z) + expz;

                        plvarray[numplv].itype = LSOLID;
                        plvarray[numplv].x1 = v1[0];
                        plvarray[numplv].y1 = v1[1];
                        plvarray[numplv].z1 = v1[2];
                        plvarray[numplv].x2 = v2[0];
                        plvarray[numplv].y2 = v2[1];
                        plvarray[numplv].z2 = v2[2];
                        plvarray[numplv].iclr1 = icol;
                        plvarray[numplv].iclr2 = icol;
                        numplv++;
                       }
                    }
                 }
              }
           }
        }
     }
}


void drawfaces()
{
 
  /*  This routine draws cell faces.  */
 
  int i, j, k, k2, icell, jcell, gocell, iex, iface, jface, nfverts,
      transflag, itype, ic1, ic2, ic3;
  double v1[3], v2[3], v3[3]; 
  float expx, expy, expz, norm[3];
  int jj;
  double fvalavg;

   for (icell = 0; icell < ncells; icell += incr)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)selectflag[icell];

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Determine explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0.;
         if (explodeflag)
           {
            if (cellexpflag == -1) 
               iex = (int)cellmats[icell] - 1;
            else iex = (int)cellflags[cellexpflag][icell] - 1;
            expx = cexplodefact*cellexpx[iex];
            expy = cexplodefact*cellexpy[iex];
            expz = cexplodefact*cellexpz[iex];
           }

         /*  If drawing materials or flags , set flags and colors.  */
         transflag = 0;
         if (fldflag == 0 || fldflag == 4)
           {
            matnum = matcolr[icell] - 1;
            if (fieldactive >= allvars)
               matnum = cellflags[fieldactive-allvars][icell] - 1;
            ic1 = matnum;
            ic3 = ic2 = ic1;
            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            if (ic1 >= 0 && cellshadeflag)
              {
               jj = 4 * ic1;
               if (matrgba[3+jj] < 1.) transflag = 1;
              }
            itype = UNLIT_FLAT;
            if (cellshadeflag) itype = LIT_FLAT;
            if (cellnormflag) itype = LIT_FLAT1SIDE;
           }

         /*  If drawing node field data, set flags.  */
         if (fldflag == 1)
           {
            itype = UNLIT_SMOOTH;
            if (cellshadeflag) itype = LIT_SMOOTH;
            if (cellnormflag) itype = LIT_SMOOTH1SIDE;
           }

         /*  If drawing cell field data, set colors.  */
         if (fldflag == 2)
           {
            cfsetfldcolr(field[icell],fld_min,fld_max,tempf,logflag,&ic1);
            ic3 = ic2 = ic1;
            itype = UNLIT_FLAT;
            if (cellshadeflag) itype = LIT_FLAT;
            if (cellnormflag) itype = LIT_FLAT1SIDE;
           }

         /*  If drawing face field data, set flags.  */
         if (fldflag == 3)
           {
            itype = UNLIT_FLAT;
            if (cellshadeflag) itype = LIT_FLAT;
            if (cellnormflag) itype = LIT_FLAT1SIDE;
           }

         /*  Loop over cell faces.  */
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];

            /*  Draw face if selected.  */
            k = 0;
            if (drawtype == 1) k = 1;
            else
              {
               if ((int)facesel[iface] == 1) k = 1;
              }
            if (k == 1)
              {
               jface = iface;
               if (structuredflag == 1) jface = i;

               nfverts = cellfaces[jface].nverts;

               /*  If drawing face field data, set colors.  */
               if (fldflag == 3)
                 {
                  cfsetfldcolr(field[iface],fld_min,fld_max,tempf,logflag,&ic1);
                  ic3 = ic2 = ic1;
                 }

               /*  If face group colors selected, set colors.  */
               if (fldflag == 4)
                 {
                  matnum = facegrpcolr[iface] - 1;
                  ic1 = matnum;
                  if (ic1 < 0 || ic1 > MAXMATS)
                    {
                     if (txtcolor < 256) ic1 = -1;
                     else ic1 = -2;
                    }
                  transflag = 0;
                  if (ic1 >= 0 && cellshadeflag)
                    {
                     jj = 4 * ic1;
                     if (matrgba[3+jj] < 1.) transflag = 1;
                    }
                  ic3 = ic2 = ic1;
                 }

               /*  Set normals for shaded faces.  */
               norm[0] = norm[1] = norm[2] = 0.;
               if (cellshadeflag || cellnormflag)
                 {
                  norm[0] = facenormx[iface];
                  norm[1] = facenormy[iface];
                  norm[2] = facenormz[iface];
                 }

               /*  Begin face vertices, allocate drawarray.  */
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

               /*  If face refinement not on, loop over  */
               /*  the face verices and make triangles.  */
               if (!facerefineflag && nfverts > 2)
                 {

                  k = cellfaces[jface].fverts[0];
                  v1[0] = nodes[k].x + expx;
                  v1[1] = nodes[k].y + expy;
                  v1[2] = nodes[k].z + expz;

                  /* If a node field is to be drawn, reset the color. */
                  if (fldflag == 1)
                    {
                     cfsetfldcolr(field[k],fld_min,fld_max,tempf,logflag,&ic1);
                    }

                  for (j = 1; j < nfverts-1; j++)
                    {
                     k = cellfaces[jface].fverts[j];
                     v2[0] = nodes[k].x + expx;
                     v2[1] = nodes[k].y + expy;
                     v2[2] = nodes[k].z + expz;

                     /* If a node field is to be drawn, reset the color. */
                     if (fldflag == 1)
                       {
                        cfsetfldcolr(field[k],fld_min,fld_max,tempf,
                                     logflag,&ic2);
                       }

                     k = cellfaces[jface].fverts[j+1];
                     v3[0] = nodes[k].x + expx;
                     v3[1] = nodes[k].y + expy;
                     v3[2] = nodes[k].z + expz;

                     /* If a node field is to be drawn, reset the color. */
                     if (fldflag == 1)
                       {
                        cfsetfldcolr(field[k],fld_min,fld_max,tempf,
                                     logflag,&ic3);
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
               if (facerefineflag && nfverts > 2)
                 {
                  v1[0] = 0.;  v1[1] = 0.;  v1[2] = 0.;  fvalavg = 0.;
                  for (j = 0; j < nfverts; j++)
                    {
                     k = cellfaces[jface].fverts[j];
                     v1[0] += nodes[k].x + expx;
                     v1[1] += nodes[k].y + expy;
                     v1[2] += nodes[k].z + expz;
                     if (fldflag == 1) fvalavg += field[k]; 
                    }
                  v1[0] /= (float)nfverts;
                  v1[1] /= (float)nfverts;
                  v1[2] /= (float)nfverts;
                  if (fldflag == 1) fvalavg /= (float)nfverts;

                  /*  Set node field color for face average.  */
                  if (fldflag == 1)
                     cfsetfldcolr(fvalavg,fld_min,fld_max,tempf,logflag,&ic1);

                  /*  Loop through face vertices.  */
                  for (j = 0; j < nfverts; j++)
                    {
                     k = cellfaces[jface].fverts[j];
                     v2[0] = nodes[k].x + expx;
                     v2[1] = nodes[k].y + expy;
                     v2[2] = nodes[k].z + expz;

                     if (fldflag == 1)
                        cfsetfldcolr(field[k],fld_min,fld_max,tempf,
                                     logflag,&ic2);

                     if (j < nfverts - 1)
                        k2 = cellfaces[jface].fverts[j+1];
                     else
                        k2 = cellfaces[jface].fverts[0];
                     v3[0] = nodes[k2].x + expx;
                     v3[1] = nodes[k2].y + expy;
                     v3[2] = nodes[k2].z + expz;

                     if (fldflag == 1)
                        cfsetfldcolr(field[k2],fld_min,fld_max,tempf,
                                     logflag,&ic3);

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

               /*  If two vertices, draw the line.   */
               /*  and set first vertex to be at the center.  */
               if (nfverts == 2)
                 {
                  itype = LSOLID;
                  if (fldflag == 1) itype = LSMOOTH;
                  if (numplv+1 > plvarray_alloc)
                    {
                     plvarray_alloc += 10000;
                     plvarray = (struct plvelem *)realloc(plvarray,
                                 plvarray_alloc*sizeof(struct plvelem));
                    }
                  k = cellfaces[jface].fverts[0];
                  v1[0] = nodes[k].x + expx;
                  v1[1] = nodes[k].y + expy;
                  v1[2] = nodes[k].z + expz;

                  if (fldflag == 1)
                     cfsetfldcolr(field[k],fld_min,fld_max,tempf,logflag,&ic1);

                  k = cellfaces[jface].fverts[1];
                  v2[0] = nodes[k].x + expx;
                  v2[1] = nodes[k].y + expy;
                  v2[2] = nodes[k].z + expz;

                  if (fldflag == 1)
                     cfsetfldcolr(field[k],fld_min,fld_max,tempf,logflag,&ic2);

                  plvarray[numplv].itype = itype;
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
     }
}


void drawaltfaces(int meshtype)
{
 
  /*  This routine creates and draws median or voronoi mesh faces.   */
  /*  meshtype determines the mesh to draw, 0 = median, 1 = voronoi  */
 
  int i, j, k, k1, k2, icell, jcell, gocell, iex, iface, jface, 
      nfv, ic1, itype;
  float expx, expy, expz, norm[3];
  double v1[3], centval, vf;
  double x1,y1,z1, x2,y2,z2, x3,y3,z3, t, xno, yno, zno;
  int jj, transflag;

   for (icell = 0; icell < ncells; icell += incr)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)selectflag[icell];

      /*  Deselect if cell is a line.  */
      if (!structuredflag)
        {
         if (cells[icell].ncnodes < 3) gocell = 0;
        }

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,3);
           }

         /*  Determine explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0.;
         if (explodeflag)
           {
            if (cellexpflag == -1) 
               iex = (int)cellmats[icell] - 1;
            else iex = (int)cellflags[cellexpflag][icell] - 1;
            expx = cexplodefact*cellexpx[iex];
            expy = cexplodefact*cellexpy[iex];
            expz = cexplodefact*cellexpz[iex];
           }

         /*  If drawing materials, flags, or     */ 
         /*  cell groups, set colors and flags.  */
         if (fldflag == 0)
           {
            matnum = matcolr[icell] - 1;
            if (fieldactive >= allvars)
               matnum = cellflags[fieldactive-allvars][icell] - 1;
            ic1 = matnum;
            jj = 4 * ic1;
            transflag = 0;
            if (matrgba[3+jj] < 1.) transflag = 1;
            itype = UNLIT_FLAT;
            if (cellshadeflag) itype = LIT_FLAT;
            if (cellnormflag) itype = LIT_FLAT1SIDE;
           }

         /*  If drawing cell field data, set colors.  */
         if (fldflag == 2)
           {
            itype = UNLIT_FLAT;
            cfsetfldcolr(field[icell],fld_min,fld_max,tempf,logflag,&ic1);
          }

         if (fldflag == 1)
            itype = UNLIT_SMOOTH;

         /*  If node data, average node field data for center.  */
         centval = 0;
         if (fldflag == 1)
           {
            for (i = 0; i < cells[jcell].ncnodes; i++)
              {
               k = cells[jcell].cnodes[i];
               centval += field[k];
              }
            centval /= (float)cells[jcell].ncnodes;
           }

         /*  Get mesh center point.  */
         if (meshtype == 0)
           {
            x1 = cells[jcell].xavg + expx;
            y1 = cells[jcell].yavg + expy;
            z1 = cells[jcell].zavg + expz;
           }
         else
           {
            voronoipt(jcell, v1);
            x1 = v1[0] + expx;
            y1 = v1[1] + expy;
            z1 = v1[2] + expz;
           }

         /*  Loop over cell faces.  */
         for (i = 0; i < cells[jcell].nfaces;  i++)
           {
            x2 = 0.;  y2 = 0.;  z2 = 0.;  vf = 0.;
            iface = cells[jcell].facenos[i];
            jface = iface;
            if (structuredflag == 1) jface = i;
            nfv = cellfaces[jface].nverts;

            /*  Get median mesh face point and data average.  */
            if (meshtype == 0)
              {
               for (j = 0; j < nfv; j++)
                 {
                  k = cellfaces[jface].fverts[j];
                  x2 += nodes[k].x;
                  y2 += nodes[k].y;
                  z2 += nodes[k].z;
                  if (fldflag == 1) vf += field[k];
                 }
               x2 /= (float)nfv;
               y2 /= (float)nfv;
               z2 /= (float)nfv;
               if (fldflag == 1) vf /= (float)nfv;
              }

            /*  Get voronoi mesh face point and data average.  */
            if (meshtype == 1)
              {
               voronoifacept(jface,v1);
               x2 = v1[0];
               y2 = v1[1];
               z2 = v1[2];
               if (fldflag == 1)
                 {
                  for (j = 0; j < nfv; j++) vf += field[k];
                  vf /= (float)nfv;
                 }
              }
            x2 += expx;
            y2 += expy;
            z2 += expz;

            /*  Loop over face edges and draw triangles from  */
            /*  center point to face point to edge center.    */
            for (j = 0; j < nfv; j++)
              {

               /*  Get the two edge points.  */
               k1 = cellfaces[jface].fverts[j];
               if (j < nfv - 1)
                  k2 = cellfaces[jface].fverts[j+1];
               else
                  k2 = cellfaces[jface].fverts[0];

               x3 = 0.5*(nodes[k1].x + nodes[k2].x) + expx;
               y3 = 0.5*(nodes[k1].y + nodes[k2].y) + expy;
               z3 = 0.5*(nodes[k1].z + nodes[k2].z) + expz;

               /*  If shading on, calculate and set the face normal.   */
               norm[0] = norm[1] = norm[2] = 0.;
               if (cellshadeflag || cellnormflag)
                 {
                  xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
                  yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
                  zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
                  t = sqrt(xno*xno + yno*yno + zno*zno);
                  norm[0] = xno / t;
                  norm[1] = yno / t;
                  norm[2] = zno / t;
                 }

               /*  Now draw the triangle, changing colors for node data.  */
               if (fldflag == 1)
                 {
                  cfsetfldcolr(centval,fld_min,fld_max,tempf,logflag,&ic1);
                 }

               if (!transflag)
                 {
                  if (numtris+2 > drawarray_alloc)
                    {
                     drawarray_alloc += 10000;
                     drawarray = (struct trielem *)realloc(drawarray,
                                 drawarray_alloc*sizeof(struct trielem));
                    }
                 }
               if (transflag)
                 {
                  if (numtris2+2 > drawarray_alloc)
                    {
                     drawarray2_alloc += 10000;
                     drawarray2 = (struct trielem *)realloc(drawarray2,
                                  drawarray2_alloc*sizeof(struct trielem));
                    }
                 }

               drawarray[numtris].itype = itype;
               drawarray[numtris].x1 = x1;
               drawarray[numtris].y1 = y1;
               drawarray[numtris].z1 = z1;
               drawarray[numtris].x2 = x2;
               drawarray[numtris].y2 = y2;
               drawarray[numtris].z2 = z2;
               drawarray[numtris].x3 = x3;
               drawarray[numtris].y3 = y3;
               drawarray[numtris].z3 = z3;
               drawarray[numtris].xnorm = norm[0];
               drawarray[numtris].ynorm = norm[1];
               drawarray[numtris].znorm = norm[2];
               drawarray[numtris].iclr1 = ic1;

               if (fldflag == 1)
                 {
                  cfsetfldcolr(vf,fld_min,fld_max,tempf,logflag,&ic1);
                 }
               drawarray[numtris].iclr2 = ic1;

               if (fldflag == 1)
                 {
                  cfsetfldcolr(0.5*(field[k1]+field[k2]),fld_min,fld_max,
                               tempf,logflag,&ic1);
                 }
               drawarray[numtris].iclr3 = ic1;
               numtris++;
              }
           }
        }
     }
}



#define CROSX(A,B,C,D,E,F) B*F-C*E
#define CROSY(A,B,C,D,E,F) C*D-A*F
#define CROSZ(A,B,C,D,E,F) A*E-B*D


void voronoipt(int jcell, double vpt[3])
{
  int i, k, npts;
  double xic[4], yic[4], zic[4];
  double xa,ya,za, xb,yb,zb, xd,yd,zd, xn,yn,zn;
  double xl,yl,zl, xl1,yl1,zl1, xl2,yl2,zl2, xl3,yl3,zl3, xl4,yl4,zl4,
         x2,y2,z2, xc,yc,zc, q, dvor, 
        qvor2;

   /*  Get the vertices for this cell.  */
   npts = cells[jcell].ncnodes;
   for (i = 0; i < npts; i++)
     {
      k = cells[jcell].cnodes[i];
      xic[i] = nodes[k].x;
      yic[i] = nodes[k].y;
      zic[i] = nodes[k].z;
     }

   xl1 = xic[0];
   yl1 = yic[0];
   zl1 = zic[0];
   xl2 = xic[1];
   yl2 = yic[1];
   zl2 = zic[1];
   xl3 = xic[2];
   yl3 = yic[2];
   zl3 = zic[2];
   xl4 = xic[3];
   yl4 = yic[3];
   zl4 = zic[3];
   xa = xl2;
   ya = yl2;
   za = zl2;
   xb = xl3-xa;
   yb = yl3-ya;
   zb = zl3-za;
   xc = xl4-xa;
   yc = yl4-ya;
   zc = zl4-za;
   xd = xl1-xa;
   yd = yl1-ya;
   zd = zl1-za;
   xn =   yb*zc-yc*zb;
   yn = -(xb*zc-xc*zb);
   zn =   xb*yc-xc*yb;
   x2 =   yn*zb-yb*zn;
   y2 = -(xn*zb-xb*zn);
   z2 =   xn*yb-xb*yn;
   q = -0.5*(xc*xb+yc*yb+zc*zb-xc*xc-yc*yc-zc*zc)/
            (x2*xc+y2*yc+z2*zc+1.0e-30);
   xl = q*x2+0.5*xb;
   yl = q*y2+0.5*yb;
   zl = q*z2+0.5*zb;
   dvor = -0.5*(xd*xd+yd*yd+zd*zd);
   qvor2 = -(xd*xl+yd*yl+zd*zl+dvor)/(xd*xn+yd*yn+zd*zn+1.0E-30);
   vpt[0] = qvor2*xn+xl+xa;
   vpt[1] = qvor2*yn+yl+ya;
   vpt[2] = qvor2*zn+zl+za;
}  

void voronoifacept(int jface, double vpt[3])
{
  int i, k;
  double xic[3], yic[3], zic[3];
  double xa,ya,za, xb,yb,zb, xd,yd,zd, xn1,yn1,zn1, rn, xn,yn,zn, dotb3, 
         dot3, rb3, ql, xl,yl,zl;
  double epsilonl;

   /*  Get the vertices for this face.  */
   epsilonl = 1.0E-8 * clminradius;
   for (i = 0; i < cellfaces[jface].nverts; i++)
     {
      k = cellfaces[jface].fverts[i];
      xic[i] = nodes[k].x;
      yic[i] = nodes[k].y;
      zic[i] = nodes[k].z;
     }
   xa = xic[0];
   ya = yic[0];
   za = zic[0];
   xb = xic[1] - xa;
   yb = yic[1] - ya;
   zb = zic[1] - za;
   xd = xic[2] - xa;
   yd = yic[2] - ya;
   zd = zic[2] - za;
   xn1 = CROSX(xb,yb,zb,xd,yd,zd);
   yn1 = CROSY(xb,yb,zb,xd,yd,zd);
   zn1 = CROSZ(xb,yb,zb,xd,yd,zd);
   xn = CROSX(xb,yb,zb,xn1,yn1,zn1);
   yn = CROSY(xb,yb,zb,xn1,yn1,zn1);
   zn = CROSZ(xb,yb,zb,xn1,yn1,zn1);
   rn = 1.0/sqrt(xn*xn+yn*yn+zn*zn);
   xn = xn*rn;
   yn = yn*rn;
   zn = zn*rn;
   dotb3 = xb*xd+yb*yd+zb*zd;
   dot3 = dotb3/(xd*xd+yd*yd+zd*zd);
   rb3 = 1.0/(xb*xb+yb*yb+zb*zb);
   ql = (1.0-dot3)/(1.0-dot3*dotb3*rb3+epsilonl);
   xl = 0.5*(ql*(xd-dotb3*rb3*xb)+xb);
   yl = 0.5*(ql*(yd-dotb3*rb3*yb)+yb);
   zl = 0.5*(ql*(zd-dotb3*rb3*zb)+zb);
   vpt[0] = xl+xa;
   vpt[1] = yl+ya;
   vpt[2] = zl+za;
}


void draw2dunsel()
{
  int i, j, k, iface, itype, ic1, ic2;
  double v1[3], v2[3], v3[3];
  float expx, expy, expz;

   /*  First, check to see if there are any unselected cells.  */
   j = 0;
   for (i = 0; i < ncells; i++)
     if ((int)cellselectflag[i] == 0) j++;
   if (cellfieldactive == -2 || cellfieldactive == -3) j = 1;
   if (j == 0) return;

   /*  Draw unselected faces for 2D mesh.  */
   j = 0;
   for (i = 0; i < ncell2dfaces; i++)
     if ((int)facesel[i] == 0) j++;

   if (numtris+j > drawarray_alloc)
     {
      drawarray_alloc += j + 1000;
      drawarray = (struct trielem *)realloc(drawarray,
                   drawarray_alloc*sizeof(struct trielem));
     }

   ic1 = -1;  ic2 = -1;  itype = LSOLID;
   expx = 0.;  expy = 0.;  expz = 0.;
   for (iface = 0; iface < ncell2dfaces; iface++)
     {
      j = 1;
      if ((int)facesel[iface] == 1) j = 0;
      if (j == 1)
        {
         k = cellfaces[iface].fverts[0];
         v1[0] = nodes[k].x + expx;
         v1[1] = nodes[k].y + expy;
         v1[2] = nodes[k].z + expz;

         k = cellfaces[iface].fverts[1];
         v2[0] = nodes[k].x + expx;
         v2[1] = nodes[k].y + expy;
         v2[2] = nodes[k].z + expz;

         plvarray[numplv].itype = itype;
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


void cfsetfldcolr
#include "setfldcolr.h"
