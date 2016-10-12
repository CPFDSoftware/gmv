#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#include <malloc.h>

#include "gmvdata.h"
#include "cutplane.h"

#define MSIZE 10000
#define CSIZE 10000
#define MAXVAL 9.99999e32

static int vertsallocated, polysallocated, valsallocated, npolys;

 
void makecutplane(a,b,c,d,clipbox,clipcellsel,flddata,npolysout)
 float a, b, c, d;
 double *flddata;
 int *npolysout, clipbox, clipcellsel;

 /*  This routine interpolates cell edges from a cut-   */
 /*  plane and generates polygons with interpolated     */
 /*  values along the cutplane.                         */  
 /*  a,b,c,d - plane equation coefficients              */ 
 /*  clipbox - clip on subset box flag                  */ 
 /*  clipcellsel - clip on cell selection flag          */ 
 /*  flddata - pointer to the data to interpolate       */
 /*  npolys - no. of polygons generated.                */
 /*                                                     */
 /*  NOTE - Be sure to free vpcutpoly, cutlist and      */
 /*         cutval in calling routine.                  */

{
  int icell, gocell, totverts;
  int i, j, k, np, nv, nptsv[CSIZE], jcell;
  int jj, ic, nodeid[CSIZE], iedge;
  int iface, jface, facefound, ff;
  float xv[CSIZE], yv[CSIZE], zv[CSIZE], vv[CSIZE], fv[CSIZE];
  double v1, v2, fact, denom, denomck, xk, yk, zk, xj, yj, zj;
  double denomf, dist, mindist, maxdist;
  short *chkface;
  void bndingpoly(int np,float xv[],float yv[],float zv[],
                  float xno,float yno,float zno,int *nvout,
                  int nptsv[]);
  void chkcpmem(int nv, int totverts);

   npolys = 0;

   if ((cutlist = (float *)malloc(4*MSIZE*sizeof(float))) == NULL)
     {
      printf("Makecutplane error, can't allocate any more memory.\n");
      gmvexit();
     }
   vertsallocated = 4*MSIZE;

   if ((cutval = (float *)malloc(MSIZE*sizeof(float))) == NULL)
     {
      printf("Makecutplane error, can't allocate any more memory.\n");
      gmvexit();
     }
   valsallocated = MSIZE;

   if ((vpcutpoly = (int *)malloc(MSIZE*sizeof(int)/4)) == NULL)
     {
      printf("Makecutplane error, can't allocate any more memory.\n");
      gmvexit();
     }
   polysallocated = MSIZE/4;

   /*  Allocate a face size array for face check.  */
   if ((chkface = (short *)malloc(ncellfaces*sizeof(short))) == NULL)
     {
      printf("Makecutplane error, can't allocate any more memory.\n");
      gmvexit();
     }
   for (i = 0; i < ncellfaces; i++) chkface[i] = 0;
   denomf = sqrt(a*a + b*b + c*c);

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

      /*  Loop over cell nodes to see if the cell spans the cutplane.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,3);
           }
         mindist = MAXVAL;
         maxdist = -MAXVAL;
         for (i = 0; i < cells[jcell].ncnodes; i++)
           {
            k = cells[jcell].cnodes[i];
            dist = (a*nodes[k].x + b*nodes[k].y + 
                    c*nodes[k].z + d) / denomf;
            mindist = (dist < mindist) ? dist : mindist;
            maxdist = (dist > maxdist) ? dist : maxdist;
           }
         if (mindist > 9.0e-7 || maxdist < -9.0e-7) gocell = 0;
        }

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Loop over cell faces to see if a face lies    */
         /*  on the cutplane.  If so, set and skip edges.  */
         facefound = -1;
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];
            jface = iface;
            if (structuredflag == 1) jface = i;
            ff = 1;

            /*  Skip face if it only has 2 points.  */
            nv = cellfaces[jface].nverts;
            if (nv < 3) ff = 0;
            else
              {
               for (j = 0; j < cellfaces[jface].nverts; j++)
                 {
                  k = cellfaces[jface].fverts[j];
                  dist = (a*nodes[k].x + b*nodes[k].y + 
                          c*nodes[k].z + d) / denomf;
                  if (dist < 0.) dist = -dist;
                  if (dist > 9.0e-7) ff = 0;
                  if (ff == 0) break;
                 }
              }

            /*  If a new face is found, fill cutplane array.  */
            if (ff == 1 && chkface[iface] == 0)
              {
               chkface[iface] = 1;
               np = cellfaces[jface].nverts;

               /*  Allocate more memory, if needed.  */
               chkcpmem(np,totverts);

               vpcutpoly[npolys] = np;
               for (j = 0; j < np; j++)
                 {
                  k = cellfaces[jface].fverts[j];
                  cutlist[3*totverts+3*j] = nodes[k].x;
                  cutlist[3*totverts+3*j+1] = nodes[k].y;
                  cutlist[3*totverts+3*j+2] = nodes[k].z;
                  if (cutplanefieldforplane < numvars && 
                     cutplanefieldforplane >=-1)
                     v1 = flddata[k];
                  else
                     v1 = flddata[icell];
                  cutval[totverts+j] = v1;
                 }
               totverts += np;
               npolys++;
               facefound = 1;
              }
           }

         /*  Loop over cell edges, calculate edge    */ 
         /*  intersections, then interpolate value.  */
         if (facefound == -1 || (mindist < -9.0e-7 && maxdist > 9.0e-7))
           {
            np = 0;
            for (i = 0; i < cells[jcell].nedges; i++)
              {
               iedge = cells[jcell].edgenos[i];
               if (structuredflag == 1) iedge = i;
               j = celledges[iedge].edgept1;
               k = celledges[iedge].edgept2;
               if (cutplanefieldforplane < numvars && 
                  cutplanefieldforplane >=-1)
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
               denom = a*(xk-xj) + b*(yk-yj) + 
                       c*(zk-zj);
               denomck = (float)(fabs((double)denom));
               fact = 1.1;
               if (denomck > 1.0e-10)
                  fact = -(a*xj + b*yj + c*zj + d) /
                          denom;
               if (fabs((double)fact) < 9.0e-7) fact = 0.;
               denomck = fact - 1.;
               if (fabs((double)denomck) < 9.0e-7) fact = 1.;
               if (fact >= 0. && fact <= 1.)
                 {
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
                     printf("Makecutplane error, more than %d "
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
               bndingpoly(np,xv,yv,zv,a,b,c,&nv,nptsv);

               /*  See if more memory needed.  */
               chkcpmem(nv,totverts);

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
           }
        }  /* End of if gocell. */
     }  /* End of icell loop. */

   free(chkface);
   *npolysout = npolys;
}
 
 
void bndingpoly(int np,float xv[],float yv[],float zv[],
                float xno,float yno,float zno,int *nvout,
                int nptsv[])
 
 /*  This routine takes a list of points, projects them  */
 /*  onto an average plane and determines the bounding   */
 /*  convex polygon around the points.                   */
 /*  np - no. of points to process.                      */
 /*  xv,yv,zv - x,y,z values of the points.              */
 /*  xno,yno,zno - plane normals.                        */
 /*  nvout - no. of vertices of the bounding polygon.    */
 /*  nptsv - ordered list of polygon vertices.           */
 
{
  int i, j, selpt, icnt, nv;
  float x1, y1, z1, t, xn, yn, zn;
  float xp[CSIZE], yp[CSIZE], zp[CSIZE], angles[CSIZE];
  float dist, maxdist, maxcos;
 
   /*  If 3 points, return points.  */
   if (np == 3)
     {
      nptsv[0] = 0; nptsv[1] = 1;  nptsv[2] = 2;
      nv = 2;
     }
 
   /*  If more than three points, determine a point order  */
   /*  that defines a bounding polyon in the plane.        */
   else
     {
 
      /*  Use the plane normals to define a plane that passes   */
      /*  through the origin and get projected x,y,z values.     */
      for (i = 0; i < np; i++)
        {
         x1 = xv[i];  y1 = yv[i];  z1 = zv[i];
         dist = (xno*x1 + yno*y1 + zno*z1);
         xp[i] = x1 - dist * xno;
         yp[i] = y1 - dist * yno;
         zp[i] = z1 - dist * zno;
        }
 
      /*  Find the projected point farthest from the origin.  */
      maxdist = -MAXVAL;
      for (i = 0; i < np; i++)
        {
         dist = sqrt(xp[i]*xp[i] + yp[i]*yp[i] + zp[i]*zp[i]);
         if (dist > maxdist)
           {
            selpt = i;
            maxdist = dist;
           }
        }
 
      /*  Use the max pt. as the first polygon point, find  */
      /*  the line that is perpendicular to the line from   */
      /*  the origin to the point that lies on the plane.   */
      nv = 0;
      nptsv[nv] = selpt;
      x1 = xp[selpt];  y1 = yp[selpt];  z1 = zp[selpt];
      xn = y1*zno - yno*z1;
      yn = z1*xno - zno*x1;
      zn = x1*yno - xno*y1;
 
      /*  Calcualte the angles from the first point to all the  */
      /*  other points.  The next vertex is the point with the  */
      /*  smallest angle.  Continue until the first point is    */
      /*  selected.  Note, smallest angle is largest cosine.    */
      selpt = -1;
      icnt = 0;
      while (selpt != nptsv[0])
        {
         icnt++;
         if (icnt > np) break;
         j = nptsv[nv];
         for (i = 0; i < np; i++)
           {
            if (i == j) angles[i] = -1.;
            else
              {
               x1 = xp[i] - xp[j];
               y1 = yp[i] - yp[j];
               z1 = zp[i] - zp[j];
               t = sqrt(xn*xn + yn*yn + zn*zn) *
                   sqrt(x1*x1 + y1*y1 + z1*z1);
               if (t < 1.0e-30) angles[i] = -1.;
               else angles[i] = (xn*x1 + yn*y1 + zn*z1) / t;
              }
            if (nv > 0 && i == nptsv[nv-1]) angles[i] = -1.;
           }
         maxcos = -MAXVAL;
         for (i = 0; i < np; i++)
           {
            if (angles[i] > maxcos)
              {
               maxcos = angles[i];
               selpt = i;
              }
           }
 
         /*  If selpt not a point quit loop.  */
         if (selpt < 0 || selpt >= np) break;

         /*  See if more than one point has this angle.  */
         x1 = xp[selpt] - xp[j];
         y1 = yp[selpt] - yp[j];
         z1 = zp[selpt] - zp[j];
         maxdist = sqrt(x1*x1 + y1*y1 + z1*z1);
         for (i = 0; i < np; i++)
           {
            if (fabs((double)(maxcos-angles[i])) < 1.0e-10)
              {
               x1 = xp[i] - xp[j];
               y1 = yp[i] - yp[j];
               z1 = zp[i] - zp[j];
               dist = sqrt(x1*x1 + y1*y1 + z1*z1);
               if (dist > maxdist)
                 {
                  selpt = i;
                  maxdist = dist;
                 }
              }
           }
 
         /*  Save next point and vector.  */
         if (selpt != nptsv[0] && nv < np)
           {
            nv++;
            nptsv[nv] = selpt;
            xn = xp[selpt] - xp[j];
            yn = yp[selpt] - yp[j];
            zn = zp[selpt] - zp[j];
           }
        }
     }
 
   *nvout = nv + 1;
}


void chkcpmem(int nv, int totverts)
{
   if (npolys+1 > polysallocated)
     {
      polysallocated += MSIZE/4;
      if ((vpcutpoly = (int *)realloc(vpcutpoly,
                       polysallocated*sizeof(int))) == NULL)
        {
         printf("Makecutplane error, can't allocate "
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
         printf("Makecutplane error, can't allocate "
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
         printf("Makecutplane error, can't allocate "
                "any more memory.\n");
         gmvexit();
        } 
     }
}
