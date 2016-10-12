#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#include <malloc.h>

#include "gmvdata.h"
#include "cutplane.h"

#define MSIZE 10000
#define CSIZE 10000
#define MAXVAL 9.99999e32

 
void makecutsphere(sx,sy,sz,sr,clipbox,clipcellsel,flddata,npolysout)
 float sx, sy,sz, sr;
 double *flddata;
 int *npolysout, clipbox, clipcellsel;

 /*  This routine interpolates cell edges from a cut-   */
 /*  sphere and generates polygons with interpolated    */
 /*  values along the cutsphere.                        */  
 /*  sx,sy,sz,sr - sphere center and radius.            */ 
 /*  clipbox - clip on subset box flag                  */ 
 /*  clipcellsel - clip on cell selection flag          */ 
 /*  flddata - pointer to the data to interpolate       */
 /*  npolys - no. of polygons generated.                */
 /*                                                     */
 /*  NOTE - Be sure to free vpcutpoly, cutlist and      */
 /*         cutval in calling routine.                  */

{
  int vertsallocated, polysallocated, valsallocated, icell, gocell;
  int i, j, k, npolys, np, nv, nptsv[CSIZE], totverts, jcell;
  int jj, ic, nodeid[CSIZE], iedge;
  float xv[CSIZE], yv[CSIZE], zv[CSIZE], vv[CSIZE], fv[CSIZE];
  double v1, v2, fact, xk, yk, zk, xj, yj, zj, r1, r2, rchk, rmin, rmax;
  double a, b, c, d, x1, y1, z1, x2, y2, z2, x3, y3, z3;
  void bndingpoly(int np,float xv[],float yv[],float zv[],
                  float xno,float yno,float zno,int *nvout,
                  int nptsv[]);

   npolys = 0;
   rchk = sr * sr;

   if ((cutlist = (float *)malloc(4*MSIZE*sizeof(float))) == NULL)
     {
      printf("Makecutsphere error, can't allocate any more memory.\n");
      gmvexit();
     }
   vertsallocated = 4*MSIZE;

   if ((cutval = (float *)malloc(MSIZE*sizeof(float))) == NULL)
     {
      printf("Makesphere error, can't allocate any more memory.\n");
      gmvexit();
     }
   valsallocated = MSIZE;

   if ((vpcutpoly = (int *)malloc(MSIZE*sizeof(int)/4)) == NULL)
     {
      printf("Makecutsphere error, can't allocate any more memory.\n");
      gmvexit();
     }
   polysallocated = MSIZE/4;

   /*  Loop over cells.  */
   totverts = 0;
   for (icell = 0; icell < ncells; icell++)
     {

      /*  If clipcellsel is turned on,        */ 
      /*  see if the cell has been selected.  */
      gocell = 1;
      if (clipcellsel == 1) gocell = (int)cellselectflag[icell];

      /*  Eliminate cells with material < 1.  */
      if (cellmats[icell] < 1) gocell = 0;

      /*  If cliponbox is turned on, see if  */
      /*  the cell lies within the subset.   */
      if (clipbox == 1 && (int)cellsubsetflag[icell] == 0)
         gocell = 0;

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,2);
           }

         /*  Loop over cell edges, calculate edge    */ 
         /*  intersections, then interpolate value.  */
         np = 0;
         for (i = 0; i < cells[jcell].nedges; i++)
           {
            iedge = cells[jcell].edgenos[i];
            if (structuredflag == 1) iedge = i;
            j = celledges[iedge].edgept1;
            k = celledges[iedge].edgept2;
            if (cutspherefieldforsphere < numvars && 
               cutspherefieldforsphere >=-1)
              {
               v1 = flddata[j];
               v2 = flddata[k];
              }
            else
              {
               v1 = flddata[icell];
               v2 = flddata[icell];
              }
            xj = nodes[j].x;  yj = nodes[j].y;  zj = nodes[j].z;
            xk = nodes[k].x;  yk = nodes[k].y;  zk = nodes[k].z;
            r1 = (xj-sx)*(xj-sx) + (yj-sy)*(yj-sy) + (zj-sz)*(zj-sz);
            r2 = (xk-sx)*(xk-sx) + (yk-sy)*(yk-sy) + (zk-sz)*(zk-sz);
            rmin = (r1 < r2) ? r1 : r2;
            rmax = (r1 > r2) ? r1 : r2;
            if (rmin <= rchk && rchk <= rmax)
              {
               r1 = sqrt(r1);
               r2 = sqrt(r2);
               fact = (r1 - sr) / (r1 - r2);
               xv[np] = xj + fact * (xk - xj);
               yv[np] = yj + fact * (yk - yj);
               zv[np] = zj + fact * (zk - zj);
               vv[np] = v1 + fact * (v2 - v1);
               if (fact <= .5) nodeid[np] = j;
               else nodeid[np] = k;
                  
               fv[np] = fact;

               np++;

               /*  If multiple interpolations at  */
               /*  cell vertices decrement np.    */
               ic = -1;
               if (fact == 0. || fact == 1.)
                 {
                  for (jj = 0; jj < np-1; jj++)
                     if ((fv[jj] == 0. || fv[jj] == 1.) &&
                         nodeid[jj] == nodeid[np-1]) ic = jj;
                 }
               if (ic > -1) np--;
               
               if (np > CSIZE)
                 {
                  printf("Makecutsphere error, more than %d "
                         "vertices.\n",CSIZE);
                  gmvexit();
                 } 
              }
           }

         /*  If this cell has intersections, call bndingpoly */
         /*  to get bounding polygon and average normal.     */
         /*  Then fill cutplane arrays.                      */
         if (np > 2)
           {

            /*  If three points, save in that order.  */
            if (np == 3)
              {
               nptsv[0] = 0; nptsv[1] = 1;  nptsv[2] = 2;
               nv = 3;
              }

            /*  If more than 3 points, calculate a normal  */
            /*  and set the point in a bounding polygon.   */
            else
              {
               x1 = xv[0];  y1 = yv[0];  z1 = zv[0];
               x2 = xv[1];  y2 = yv[1];  z2 = zv[1];
               x3 = xv[2];  y3 = yv[2];  z3 = zv[2];
               a = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
               b = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
               c = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
               d = sqrt(a*a + b*b + c*c);
               a /= d;  b /= d;  c /= d;
               bndingpoly(np,xv,yv,zv,a,b,c,&nv,nptsv);
              }

            /*  See if more memory needed.  */
            if (npolys+1 > polysallocated)
              {
               polysallocated += MSIZE/4;
               if ((vpcutpoly = (int *)realloc(vpcutpoly,
                    polysallocated*sizeof(int))) == NULL)
                 {
                  printf("Makecutsphere error, can't allocate "
                         "any more memory.\n");
                  gmvexit();
                 }
              }

            if (3*(totverts+nv) > vertsallocated)
              {
               vertsallocated += 4*MSIZE;
               if ((cutlist = (float *)realloc(cutlist,
                   vertsallocated*sizeof(float))) == NULL)
                 {
                  printf("Makecutsphere error, can't allocate "
                         "any more memory.\n");
                  gmvexit();
                 } 
              }

            if (totverts+nv > valsallocated)
              {
               valsallocated += MSIZE;
               if ((cutval = (float *)realloc(cutval,
                   valsallocated*sizeof(float))) == NULL)
                 {
                  printf("Makecutsphere error, can't allocate "
                         "any more memory.\n");
                  gmvexit();
                 } 
              }

            /*  Fill cutplane arrays.  */
            vpcutpoly[npolys] = nv;
            for (i = 0; i < nv; i++)
              {
               k = nptsv[i];
               cutlist[3*totverts+3*i] = xv[k];
               cutlist[3*totverts+3*i+1] = yv[k];
               cutlist[3*totverts+3*i+2] = zv[k];
               cutval[totverts+i] = vv[k];
              }
            totverts += nv;
            npolys++;
           }
        }  /* End of if gocell. */
     }  /* End of icell loop. */

   *npolysout = npolys;
}
