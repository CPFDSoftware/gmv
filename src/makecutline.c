#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "cutplane.h"

#define MSIZE 1000
#define MAXVAL 9.99999e32

 
void makecutline(xp1,yp1,zp1,xp2,yp2,zp2,cldist,clfld,flddata,nclpts)
 float xp1, yp1, zp1, xp2, yp2, zp2, cldist;
 double *flddata; 
 int clfld, *nclpts;

 /*  This routine gathers nodes within cldist radius of  */
 /*  the line between (xp1,yp1,zp1) and (xp2,yp2,zp2).   */
 /*  The nodes are then sorted by distance factors to    */  
 /*  the first point of the line.                        */
 /*  xp1,yp1,zp1 - line end 1                            */
 /*  xp2,yp2,zp2 - line end 2                            */
 /*  cldist - search radius                              */
 /*  clfld - current cutline field selected              */
 /*  flddata - pointer to the selected field values      */
 /*  nclpts - no. of cut plane points created            */
 /*                                                      */
 /*  The cutline values are stored in cutval, the node   */
 /*  distance factors are stored in cutlist and the      */
 /*  selected nodes are stored in cutnode.               */
 /*                                                      */
 /*  NOTE - Be sure to free cutval, cutlist and cutnode  */
 /*         in the calling routine.                      */

{
  int gonode, i, j, k, *nodesfound, nallocated, npts;
  double *vals, xn, yn, zn;
  float *ds, mindist;
  double ax, ay, az, bx, by, bz, s, xp, yp, zp, dist;
  int istrt, nloop, icell, jcell;


   /*  Allocate temporary arrays.  */
   if ((vals = (double *)malloc(MSIZE*sizeof(double))) == NULL)
     {
      printf("Makecutline error, can't allocate any more memory.\n");
      gmvexit();
     }
   if ((ds = (float *)malloc(MSIZE*sizeof(float))) == NULL)
     {
      printf("Makecutline error, can't allocate any more memory.\n");
      gmvexit();
     }
   if ((nodesfound = (int *)malloc(MSIZE*sizeof(int))) == NULL)
     {
      printf("Makecutline error, can't allocate any more memory.\n");
      gmvexit();
     }
   nallocated = MSIZE;

   /*  Initialize cut line point data.  */
   npts = 0;
   ax = (double)xp2 - (double)xp1;
   ay = (double)yp2 - (double)yp1;
   az = (double)zp2 - (double)zp1;

   /*  Loop over all nodes or cells.  */
   istrt = 1;
   nloop = nnodes + 1;
   if (clfld >= numvars)
     {
      istrt = 0;
      nloop = ncells;
     }
   for (i = istrt; i < nloop; i++)
     {

      /*  Eliminate nodes, cells with material < 1.  */
      gonode = 1;
      if (clfld < numvars)
         { if (imat[i] < 1) gonode = 0; }
      else
         { if (cellmats[i] < 1) gonode = 0; }

      /*  Calculate projection point of node or cell avg. on the line.  */
      if (gonode == 1)
        {
         if (clfld < numvars)
           { xn = nodes[i].x;  yn = nodes[i].y;  zn = nodes[i].z; }
         else
           {
            icell = i;
            jcell = icell;
            if (structuredflag == 1)
              {
               jcell = 0;
               fillstructcell(icell,0);
              }
            xn = cells[jcell].xavg;
            yn = cells[jcell].yavg;
            zn = cells[jcell].zavg;
           }
         bx = (double)xn - (double)(xp1);
         by = (double)yn - (double)(yp1);
         bz = (double)zn - (double)(zp1);
         s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
         if ((s - 1.) > 0. && (s - 1.) <= myzero) s = 1.;
         if (fabs(s) < myzero) s = 0.;
         xp = s*xp2 + xp1*(1.0 - s);
         yp = s*yp2 + yp1*(1.0 - s);
         zp = s*zp2 + zp1*(1.0 - s);
         dist = sqrt((xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + 
                     (zp-zn)*(zp-zn));
        }
      if (gonode == 1 && dist <= cldist && s >= 0. && s <= 1.)
        {

         /*  See if more memory needed.  */
         if (npts+1 > nallocated)
           {
            nallocated += MSIZE;
            if ((vals = (double *)realloc(vals,
                nallocated*sizeof(double))) == NULL)
              {
               printf("Makecutline error, can't allocate "
                      "any more memory.\n");
               gmvexit();
              } 
            if ((ds = (float *)realloc(ds,
                nallocated*sizeof(float))) == NULL)
              {
               printf("Makecutline error, can't allocate "
                      "any more memory.\n");
               gmvexit();
              } 
            if ((nodesfound = (int *)realloc(nodesfound,
                 nallocated*sizeof(int))) == NULL)
              {
               printf("Makecutline error, can't allocate "
                     "any more memory.\n");
               gmvexit();
              }
           }

         vals[npts] = flddata[i];
         ds[npts] = s;
         nodesfound[npts] = i;
         npts++;
        }
     }

   /*  Allocate arrays used in calling routine.  */
   if (npts > 0)
     {
      if ((cutval = (float *)malloc(npts*sizeof(float))) == NULL)
        {
         printf("Makecutline error, can't allocate any more memory.\n");
         gmvexit();
        }
      if ((cutlist = (float *)malloc(npts*sizeof(float))) == NULL)
        {
         printf("Makecutline error, can't allocate any more memory.\n");
         gmvexit();
        }
      if ((cutnodes = (int *)malloc(npts*sizeof(int))) == NULL)
        {
         printf("Makecutline error, can't allocate any more memory.\n");
         gmvexit();
        }
     }

   /*  Sort the nodes by projected line distance.  */
   for (i = 0; i < npts; i++)
     { 
      mindist = MAXVAL;
      for (j = 0; j < npts; j++)
        {
         if (ds[j] < mindist)
           {
            k = j;
            mindist = ds[j];
           }
        } 
      cutval[i] = vals[k];
      cutlist[i] = mindist;
      cutnodes[i] = nodesfound[k];
      ds[k] = MAXVAL;
     }

   free(vals);  free(ds);  free(nodesfound);
   *nclpts = npts;

}
