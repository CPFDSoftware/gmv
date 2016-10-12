#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"
 
void drawpolylines(matnum)
 int matnum;
 
{
  int i, j, k, kk, m, incr, ic1, itype;
  long nv;
  double v1[3], v2[3];

   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */
   i = matnum;
 
   ic1 = matnum;
   if (polysflag == 1)
      ic1 = -2;

   j = 0;
   for (k = 0; k < npolyspermat[matnum]; k++)
     {
      j += (int)polygons[matnum][k].nverts;
     }
   j += 1000;
   if (numplv+j > plvarray_alloc)
     {
      plvarray_alloc += j;
      plvarray = (struct plvelem *)realloc(plvarray,
                  plvarray_alloc*sizeof(struct plvelem));
     }
 
   for (j = 0; j < npolyspermat[i]; j += incr)
     {
 
      if (polydrawchk[i][j] == 1)
        { 
         m = (int)polygons[i][j].nverts;
         itype = LSOLID;
         if (m == 1) itype = PONLY;
         nv = polygons[i][j].polyvertloc;
         for (k = 0; k < m; k++)
           {
            kk = k + 1;
            if (k == m-1) kk = 0;
            v1[0] = polyverts[i][k+nv].x + pexplodefact*matexpx[i];
            v1[1] = polyverts[i][k+nv].y + pexplodefact*matexpy[i];
            v1[2] = polyverts[i][k+nv].z + pexplodefact*matexpz[i];
            v2[0] = polyverts[i][kk+nv].x + pexplodefact*matexpx[i];
            v2[1] = polyverts[i][kk+nv].y + pexplodefact*matexpy[i];
            v2[2] = polyverts[i][kk+nv].z + pexplodefact*matexpz[i];

            plvarray[numplv].itype = itype;
            plvarray[numplv].x1 = v1[0];
            plvarray[numplv].y1 = v1[1];
            plvarray[numplv].z1 = v1[2];
            plvarray[numplv].x2 = v2[0];
            plvarray[numplv].y2 = v2[1];
            plvarray[numplv].z2 = v2[2];
            plvarray[numplv].iclr1 = ic1;
            plvarray[numplv].iclr2 = ic1;
            numplv++;
           }
        } 
     }
}
