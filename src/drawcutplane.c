#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "gmvdata.h"
#include "lights.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

static double fld_min, fld_max, tempf, fac, valmin;
static float xno, yno, zno;
static char logflag, lineflag;
static int incr, icp, cpfield, cuttype;
void cpsetfldcolr(double fldval, double fldmin, double fldmax, double tempval, 
                 char logflag, int *icl);

void drawcutplane(int icpin)
{
  int k, i, j, ind;
  float val1;
  void drawcpcontours();
  void drawcpedges();
  void drawcpdnodes();
  void drawcpvects(int vtype);
  void drawcpdcells();
  void drawcppolys();

   icp = icpin;

   cuttype = 1;
   if (icp > MAXCUTPLANES) cuttype = 0;

   fac = span*cutplane[icp].heightfac / 255.;

   if (cuttype)
     {
      if (icp < MAXCUTPLANES) cpfield = cutplanefieldactive;
      else cpfield = cutplanefieldforplane;
     }
   else
     {
      i = icp - MAXCUTPLANES - 1;
      if (i < MAXCUTSPHERES) cpfield = cutspherefieldactive;
      else cpfield = cutspherefieldforsphere;
     }

   if (cpfield < numvars && cpfield >= 0)
     {
      k = cpfield;
      lineflag = nfld_nice[k];
      logflag = nfld_log[k];
      fld_min = nfld_nice_min[k];
      fld_max = nfld_nice_max[k];
     }
   else if (cpfield >= 0)
     {
      k = cpfield - numvars;
      lineflag = cfld_nice[k];
      logflag = cfld_log[k];
      fld_min = cfld_nice_min[k];
      fld_max = cfld_nice_max[k];
     }
   tempf = fld_max - fld_min;
   if (logflag) tempf = log10(fld_max) - log10(fld_min);

   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */

   /*  Calculate minimum field data to clamp to cutplane.  */
   ind = 0;
   valmin = MAXVAL;
   for (i = 0; i < cutplane[icp].npolys; i++)
     {
      for (j = 0; j < cutplane[icp].vertsperpoly[i]; j++)
        {
         val1 = cutplane[icp].polylist[ind];
         valmin = (val1 < valmin) ? val1 : valmin;
         ind += 4;
        }
     }

   /*  Set cutplane normal components.  */
   xno = cutplane[icp].norm[0];
   yno = cutplane[icp].norm[1];
   zno = cutplane[icp].norm[2];

   /*  Draw contour lines.  */
   if (cutplane[icp].linesflag == 1 && incr == 1 && 
       cpfield < numvars && cpfield >= 0)
      drawcpcontours();

   /*  Draw edges.  */
   if (cutplane[icp].edgesflag == 1 && incr == 1)
      drawcpedges();

   /*  Draw cutplane nodes and/or numbers, if requested.  */
   if (cutplane[icp].ndnodes > 0 && 
       (cutplane[icp].nodeflag == 1 || cutplane[icp].nnumflag == 1))
      drawcpdnodes(); 

   /*  Draw cutplane vectors, if any.  */
   if (cutplane[icp].ndnodes > 0 && cutplane[icp].velflag == 1) 
      drawcpvects(1);
   if (cutplane[icp].ndcells > 0 && cutplane[icp].cvelflag == 1) 
      drawcpvects(0);

   /*  Draw cutplane cells and/or numbers, if requested.  */
   if (cutplane[icp].ndcells > 0 && 
       (cutplane[icp].cellflag == 1 || cutplane[icp].cnumflag == 1))
      drawcpdcells(); 

   /*  Draw cutplane polys.  */
   if (cutplane[icp].facesflag == 1) drawcppolys();
}


void drawcpcontours()
{
  int ind, i, j, m, ic, ncl, ic1;
  float fval, s, denom, vmin, vmax;
  float val1, val2, x1, y1, z1, x2, y2, z2, v1[3], cv;
  double clv[1000][3];
  double intervals[100];
  int nsets, ipt2, ind2, nintervals;
  void getscale(double llimit, double ulimit,int max, char lineflag,
                char logflag, double *intervals, int *nintervals);

   /*  Call getscale to get the number of intervals.  */
   getscale(fld_min, fld_max, nset_contours, lineflag, logflag,
            intervals, &nintervals);
   nsets = nintervals;

   ind = 0;

   if (txtcolor < 256) ic1 = -1;
   else ic1 = -2;

   for (i = 0; i < cutplane[icp].npolys; i += incr)
     {

      vmin = MAXVAL;  vmax = -MAXVAL;
      for (j = 0; j < cutplane[icp].vertsperpoly[i]; j++)
        {
         val1 = cutplane[icp].polylist[ind];
         vmin = (val1 < vmin) ? val1 : vmin;
         vmax = (val1 > vmax) ? val1 : vmax;
         ind += 4;
        }
           
      for (m = 1; m < nsets; m++)
        {
         fval = intervals[m];
         ncl = 0;

         if (vmin <= fval && fval <= vmax && vmin < vmax)
           {
            ind2 = ind - 4 * cutplane[icp].vertsperpoly[i];
            for (j = 0; j < cutplane[icp].vertsperpoly[i]; j++)
              {
               val1 = cutplane[icp].polylist[ind2];
               x1 = cutplane[icp].polylist[ind2+1];
               y1 = cutplane[icp].polylist[ind2+2];
               z1 = cutplane[icp].polylist[ind2+3];
               ipt2 = ind2 + 4;
               if (j == cutplane[icp].vertsperpoly[i] - 1)
                  ipt2 = ind - 4 * cutplane[icp].vertsperpoly[i];
               val2 = cutplane[icp].polylist[ipt2];
               x2 = cutplane[icp].polylist[ipt2+1];
               y2 = cutplane[icp].polylist[ipt2+2];
               z2 = cutplane[icp].polylist[ipt2+3];

               if ((val1 <= fval && fval <= val2) ||
                   (val2 <= fval && fval <= val1))
                 {
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
                     v1[0] = x1 + s * (x2 - x1);
                     v1[1] = y1 + s * (y2 - y1);
                     v1[2] = z1 + s * (z2 - z1);

                     if (cutplane[icp].heightflag)
                       {
                        cv = fval;
                        ic = (int) ((cv - valmin)*(255./tempf));  
                        ic = (ic < 0) ? 0 : ic;
                        ic = (ic > 255) ? 255 : ic;

                        /*  If cutsphere, recalculate normals.  */
                        if (cuttype == 0)
                          {
                           xno = v1[0] - cutplane[icp].aa;
                           yno = v1[1] - cutplane[icp].bb;
                           zno = v1[2] - cutplane[icp].cc;
                           denom = sqrt(xno*xno + yno*yno + zno*zno);
                           xno /= denom;
                           yno /= denom;
                           zno /= denom;
                          }

                        v1[0] += (float)ic * xno * fac;
                        v1[1] += (float)ic * yno * fac;
                        v1[2] += (float)ic * zno * fac;
                       }

                     clv[ncl][0] = v1[0];
                     clv[ncl][1] = v1[1];
                     clv[ncl][2] = v1[2];
                     ncl++;
                    }
                 }
               ind2 += 4;
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
               plvarray[numplv].iclr1 = ic1;
               plvarray[numplv].iclr2 = ic1;
               numplv++;
              }
           }
        }
 
      /* ind += 4 * cutplane[icp].vertsperpoly[i]; */
     }
}


void drawcpedges()
{
  int ind, i, j, l, ic, ncl, ic1;
  float cv, v1[3], denom;
  double clv[1000][3];

   ind = 0;
   if (cutplane[icp].linesflag == 1 && cpfield < numvars && cpfield >= 0)      
      ic1 = -3;
   else
      ic1 = -1;
   for (i = 0; i < cutplane[icp].npolys; i += incr)
     {
      ncl = 0;
      for (j = 0; j < cutplane[icp].vertsperpoly[i]; j++)
        {
         v1[0] = cutplane[icp].polylist[ind+1];
         v1[1] = cutplane[icp].polylist[ind+2];
         v1[2] = cutplane[icp].polylist[ind+3];

         if (cutplane[icp].heightflag)
           {
            cv = cutplane[icp].polylist[ind];
            ic = (int) ((cv - valmin)*(255./tempf));  
            /* ic = (int) ((cv)*tempf);  */
            ic = (ic < 0) ? 0 : ic;
            ic = (ic > 255) ? 255 : ic;

            /*  If cutsphere, recalculate normals.  */
            if (cuttype == 0)
              {
               xno = v1[0] - cutplane[icp].aa;
               yno = v1[1] - cutplane[icp].bb;
               zno = v1[2] - cutplane[icp].cc;
               denom = sqrt(xno*xno + yno*yno + zno*zno);
               xno /= denom;
               yno /= denom;
               zno /= denom;
              }

            v1[0] += (float)ic * xno * fac;
            v1[1] += (float)ic * yno * fac;
            v1[2] += (float)ic * zno * fac;
           }

         clv[ncl][0] = v1[0];
         clv[ncl][1] = v1[1];
         clv[ncl][2] = v1[2];
         ncl++;

         ind += 4;
        }

      if (ncl > 0)
        {
         if (numplv+ncl > plvarray_alloc)
           {
            plvarray_alloc += 10000;
            plvarray = (struct plvelem *)realloc(plvarray,
                        plvarray_alloc*sizeof(struct plvelem));
           }
         for (j = 0; j < ncl; j++)
           {
            l = j + 1;
            if (j == ncl-1) l = 0;
            plvarray[numplv].itype = LSOLID;
            plvarray[numplv].x1 = clv[j][0];
            plvarray[numplv].y1 = clv[j][1];
            plvarray[numplv].z1 = clv[j][2];
            plvarray[numplv].x2 = clv[l][0];
            plvarray[numplv].y2 = clv[l][1];
            plvarray[numplv].z2 = clv[l][2];
            plvarray[numplv].iclr1 = ic1;
            plvarray[numplv].iclr2 = ic1;
            numplv++;
           }
        }
     }
}


void drawcpdnodes()
{
  int i, ic1, num, id, id2, itype;
  short matnum;

   for (i = 0; i < cutplane[icp].ndnodes; i += incr)
     {
      num = cutplane[icp].dnodes[i];

      /*  Draw a node in material color.     */
      /*  Set material color for this node.  */
      matnum = imat[num] - 1;
      ic1 = matnum;
      if (ic1 < 0 || ic1 > mmats) 
        {
         if (txtcolor < 256) ic1 = -1;
         else ic1 = -2;
        }

      if (cutplane[icp].nodeflag == 1) itype = PONLY;
      id = 0;  id2 = 0;
      if (cutplane[icp].nnumflag == 1)
        {
         itype = PANDNUM;
         id = num;
         if (nodeidflag) id = nodeids[num];

#ifdef MPI
         if (local_idflag)
           {
            id = nodepe[num];
            id2 = nodenuminpe[num];
           }
#endif
        }
      if (numplv+1 > plvarray_alloc)
        {
         plvarray_alloc += 10000;
         plvarray = (struct plvelem *)realloc(plvarray,
                     plvarray_alloc*sizeof(struct plvelem));
        }
      plvarray[numplv].itype = itype;
      plvarray[numplv].x1 = nodes[num].x;
      plvarray[numplv].y1 = nodes[num].y;
      plvarray[numplv].z1 = nodes[num].z;
      plvarray[numplv].x2 = id;
      plvarray[numplv].y2 = id2;
      plvarray[numplv].z2 = 0.;
      plvarray[numplv].iclr1 = ic1;
      plvarray[numplv].iclr2 = ic1;
      numplv++;
     } 
}


void drawcpvects(int vtype)
{
  int i, j, num, jcell, iend, itype;
  double vfac, speed, maxspd;
  double xv, yv, zv, uv, vv, wv;
  short matnum, *mats;
  int *cpnums;


   /*  Set variables for vector type,       */
   /*  cells (vtype=0) or nodes (vtype=1).  */
   if (vtype)
     {
      itype = NVECTOR;
      maxspd = maxspeed;
      iend = cutplane[icp].ndnodes;
      mats = imat;
      cpnums = cutplane[icp].dnodes;
     }
   else
     {
      itype = CVECTOR;
      maxspd = cmaxspeed;
      iend = cutplane[icp].ndcells;
      mats = cellmats;
      cpnums = cutplane[icp].dcells;
     }

   vfac = velscale*span / (maxspd*20.);

   if (numplv+iend > plvarray_alloc)
     {
      plvarray_alloc += iend + 1000;
      plvarray = (struct plvelem *)realloc(plvarray,
                  plvarray_alloc*sizeof(struct plvelem));
     }

   for (i = 0; i < iend; i += incr)
     {
      num = cpnums[i];

      /*  Set x,y,z, and u,v,w.  */
      if (vtype)
        {
         xv = nodes[num].x;  yv = nodes[num].y;  zv = nodes[num].z;
         uv = cutplane[icp].vect[0][i];
         vv = cutplane[icp].vect[1][i];
         wv = cutplane[icp].vect[2][i];
        }
      else
        {
         jcell = num;

         /*  Fill cell structure if structured grid.  */
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(num,0);
           }
         xv = cells[jcell].xavg;
         yv = cells[jcell].yavg;
         zv = cells[jcell].zavg;
         uv = cutplane[icp].cvect[0][i];
         vv = cutplane[icp].cvect[1][i];
         wv = cutplane[icp].cvect[2][i];
        }

      speed = sqrt((uv * uv) + (vv * vv) + (wv * wv));
      speed *= vfac;

      /*  Draw a vector in material color.  */
      if (speed > .00001) 
        {

         /*  Set material color for this node.  */
         matnum = mats[num] - 1;
         j = matnum;

         if (j < 0 || j > mmats) 
           {
            if (txtcolor < 256) j = -1;
            else j = -2;
           }

         plvarray[numplv].itype = itype;
         plvarray[numplv].x1 = xv;
         plvarray[numplv].y1 = yv;
         plvarray[numplv].z1 = zv;
         plvarray[numplv].x2 = uv;
         plvarray[numplv].y2 = vv;
         plvarray[numplv].z2 = wv;
         plvarray[numplv].iclr1 = j;
         plvarray[numplv].iclr2 = j;
         numplv++;
        }
     }   
}


void drawcpdcells()
{
  int i, k, ic1, num, icell, jcell, iedge, id, id2;
  short matnum;

   for (icell = 0; icell < cutplane[icp].ndcells; icell += incr)
     {
      num = cutplane[icp].dcells[icell];

      /*  Draw a cell in material color.     */
      /*  Set material color for this node.  */
      matnum = cellmats[num] - 1;
      ic1 = matnum;
      if (ic1 < 0 || ic1 > mmats) 
        {
         if (txtcolor < 256) ic1 = -1;
         else ic1 = -2;
        }
      jcell = num;

      /*  Fill cell structure if structured grid.  */
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(num,2);
        }

      /*  Loop over the cell edges and draw the edge.  */
      i = cells[jcell].nedges + 1;
      if (numplv+i > plvarray_alloc)
        {
         plvarray_alloc += 10000;
         plvarray = (struct plvelem *)realloc(plvarray,
                     plvarray_alloc*sizeof(struct plvelem));
        }
      for (i = 0; i < cells[jcell].nedges; i++)
        {
         iedge = cells[jcell].edgenos[i];
         if (structuredflag == 1) iedge = i;

         if (numplv+2 > plvarray_alloc)
           {
            plvarray_alloc += 10000;
            plvarray = (struct plvelem *)realloc(plvarray,
                        plvarray_alloc*sizeof(struct plvelem));
           }

         k = celledges[iedge].edgept1;
         plvarray[numplv].itype = LSOLID;
         plvarray[numplv].x1 = nodes[k].x;
         plvarray[numplv].y1 = nodes[k].y;
         plvarray[numplv].z1 = nodes[k].z;
         k = celledges[iedge].edgept2;
         plvarray[numplv].x2 = nodes[k].x;
         plvarray[numplv].y2 = nodes[k].y;
         plvarray[numplv].z2 = nodes[k].z;
         plvarray[numplv].iclr1 = ic1;
         plvarray[numplv].iclr2 = ic1;
         numplv++;
        } 

      /*  Draw cell number if requested.  */
      id = 0;  id2 = 0;
      if (cutplane[icp].cnumflag == 1)
        {
         id = num +1;
         if (cellidflag) id = cellids[num];

#ifdef MPI
         if (local_idflag)
           {
            id = cellpe[num];
            id2 = cellnuminpe[num];
           }
#endif
         plvarray[numplv].itype = NUMONLY;
         plvarray[numplv].x1 = cells[jcell].xavg;
         plvarray[numplv].y1 = cells[jcell].yavg;
         plvarray[numplv].z1 = cells[jcell].zavg;
         plvarray[numplv].x2 = id;
         plvarray[numplv].y2 = id2;
         plvarray[numplv].z2 = 0.;
         plvarray[numplv].iclr1 = ic1;
         plvarray[numplv].iclr2 = ic1;
         numplv++;
        }

     }
}


void drawcppolys()
{
  int i, j, ind, ic, ic1, ic2, ic3, itype;
  float cv, denom, x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double xn, yn, zn, t;

   ind = 0;

   for (i = 0; i < cutplane[icp].npolys; i+= incr)
     {
      cv = cutplane[icp].polylist[ind];
      x1 = cutplane[icp].polylist[ind+1];
      y1 = cutplane[icp].polylist[ind+2];
      z1 = cutplane[icp].polylist[ind+3];
      ind += 4;

      if (cpfield >= 0)
        {
         cpsetfldcolr(cv,fld_min,fld_max,tempf,logflag,&ic1);
        }
      else
        {
         ic1 = (int)(cv - 1);
        }

      if (cutplane[icp].heightflag)
        {
         ic = (int) ((cv - valmin)*(255./tempf)); 
         ic = (ic < 0) ? 0 : ic;
         ic = (ic > 255) ? 255 : ic;

         /*  If cutsphere, recalculate normals.  */
         if (cuttype == 0)
           {
            xno = x1 - cutplane[icp].aa;
            yno = y1 - cutplane[icp].bb;
            zno = z1 - cutplane[icp].cc;
            denom = sqrt(xno*xno + yno*yno + zno*zno);
            xno /= denom;
            yno /= denom;
            zno /= denom;
           }
          x1 += (float)ic * xno * fac;
          y1 += (float)ic * yno * fac;
          z1 += (float)ic * zno * fac;
         }

      j = cutplane[icp].vertsperpoly[i];
      if (numtris+j > drawarray_alloc)
        {
         drawarray_alloc += 10000;
         drawarray = (struct trielem *)realloc(drawarray,
                     drawarray_alloc*sizeof(struct trielem));
        }
      for (j = 1; j < cutplane[icp].vertsperpoly[i]-1; j++)
        {
         cv = cutplane[icp].polylist[ind];
         x2 = cutplane[icp].polylist[ind+1];
         y2 = cutplane[icp].polylist[ind+2];
         z2 = cutplane[icp].polylist[ind+3];
         ind += 4;

         if (cpfield >= 0)
           {
            cpsetfldcolr(cv,fld_min,fld_max,tempf,logflag,&ic2);
           }
         else
           {
            ic2 = (int)(cv - 1);
           }

         if (cutplane[icp].heightflag)
           {
            ic = (int) ((cv - valmin)*(255./tempf)); 
            ic = (ic < 0) ? 0 : ic;
            ic = (ic > 255) ? 255 : ic;

            /*  If cutsphere, recalculate normals.  */
            if (cuttype == 0)
              {
               xno = x2 - cutplane[icp].aa;
               yno = y2 - cutplane[icp].bb;
               zno = z2 - cutplane[icp].cc;
               denom = sqrt(xno*xno + yno*yno + zno*zno);
               xno /= denom;
               yno /= denom;
               zno /= denom;
              }
             x2 += (float)ic * xno * fac;
             y2 += (float)ic * yno * fac;
             z2 += (float)ic * zno * fac;
            }
      
         cv = cutplane[icp].polylist[ind];
         x3 = cutplane[icp].polylist[ind+1];
         y3 = cutplane[icp].polylist[ind+2];
         z3 = cutplane[icp].polylist[ind+3];
         /* ind += 4; */

         if (cpfield >= 0)
           {
            cpsetfldcolr(cv,fld_min,fld_max,tempf,logflag,&ic3);
           }
         else
           {
            ic3 = (int)(cv - 1);
           }

         if (cutplane[icp].heightflag)
           {
            ic = (int) ((cv - valmin)*(255./tempf)); 
            ic = (ic < 0) ? 0 : ic;
            ic = (ic > 255) ? 255 : ic;

            /*  If cutsphere, recalculate normals.  */
            if (cuttype == 0)
              {
               xno = x3 - cutplane[icp].aa;
               yno = y3 - cutplane[icp].bb;
               zno = z3 - cutplane[icp].cc;
               denom = sqrt(xno*xno + yno*yno + zno*zno);
               xno /= denom;
               yno /= denom;
               zno /= denom;
              }
             x3 += (float)ic * xno * fac;
             y3 += (float)ic * yno * fac;
             z3 += (float)ic * zno * fac;
            }

         itype = UNLIT_SMOOTH;
         if (cpfield < 0)
           {
            itype = UNLIT_FLAT;
            if (ic2 == ic3) ic1 = ic2;
            ic2 = ic1;
            ic3 = ic1;
           }

         /*  If height selected and node field displayed,  */
         /*  turn on light model and shading.              */
         xn = 0.;  yn = 0.;   zn = 0.;
         if (cutplane[icp].heightflag && cpfield < numvars && cpfield >= 0)
           {
            itype = LIT_SMOOTH;
            xn = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
            yn = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
            zn = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
            t = sqrt(xn*xn + yn*yn + zn*zn);
            xn /= t;
            yn /= t;
            zn /= t;
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
         drawarray[numtris].xnorm = (float)xn;
         drawarray[numtris].ynorm = (float)yn;
         drawarray[numtris].znorm = (float)zn;
         drawarray[numtris].iclr1 = ic1;
         drawarray[numtris].iclr2 = ic2;
         drawarray[numtris].iclr3 = ic3;
         numtris++;
        }
 
      ind += 4;
     }
}


void cpsetfldcolr
#include "setfldcolr.h"
