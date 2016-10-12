#include "gmvdata.h"
#include "lights.h"

#include <stdlib.h>
#include <math.h>

int incr, matnum, transflg;
struct trielem *drawpoly;


void drawpolygons(int matnumin)
{
  int i, j, k;
  void drawpolys();

   matnum = matnumin;
   k = matnum * 4;
   transflg = 0;
   if (matrgba[3+k] < 1.) transflg = 1;

   j = 0;
   for (i = 0; i < npolyspermat[matnum]; i++)
     {
      j += (int)polygons[matnum][i].nverts - 2;
     }
   j += 1000;
   if (transflg == 0)
     {
      if (numtris+j > drawarray_alloc)
        {
         drawarray_alloc += j;
         drawarray = (struct trielem *)realloc(drawarray,
                      drawarray_alloc*sizeof(struct trielem));
        }
      drawpoly = drawarray;
     }
   if (transflg == 1)
     {
      if (numtris2+j > drawarray2_alloc)
        {
         drawarray2_alloc += j;
         drawarray2 = (struct trielem *)realloc(drawarray2,
                       drawarray2_alloc*sizeof(struct trielem));
        }
      drawpoly = drawarray2;
     }

   drawpolys();
}


void drawpolys()
{
  int i, j, k, m, nump;
  float n[3];
  double v1[3], v2[3], v3[3];
  long nv;

   incr = 1;    
   /* if (fastdraw == 1) incr = drawincr; */
   i = matnum;

   for (j = 0; j < npolyspermat[i]; j += incr)
     {

      if (polydrawchk[i][j] == 1)
        {
         n[0] = polygons[i][j].xnorm; 
         n[1] = polygons[i][j].ynorm; 
         n[2] = polygons[i][j].znorm;

         m = (int)polygons[i][j].nverts;
         nv = polygons[i][j].polyvertloc;
         v1[0] = polyverts[i][nv].x + pexplodefact*matexpx[i];
         v1[1] = polyverts[i][nv].y + pexplodefact*matexpy[i];
         v1[2] = polyverts[i][nv].z + pexplodefact*matexpz[i];
         for (k = 1; k < m-1; k++)
           {
            v2[0] = polyverts[i][k+nv].x + pexplodefact*matexpx[i];
            v2[1] = polyverts[i][k+nv].y + pexplodefact*matexpy[i];
            v2[2] = polyverts[i][k+nv].z + pexplodefact*matexpz[i];
            v3[0] = polyverts[i][k+1+nv].x + pexplodefact*matexpx[i];
            v3[1] = polyverts[i][k+1+nv].y + pexplodefact*matexpy[i];
            v3[2] = polyverts[i][k+1+nv].z + pexplodefact*matexpz[i];
            if (transflg == 0) nump = numtris;
            else nump = numtris2;
            drawpoly[nump].itype = LIT_FLAT;
            drawpoly[nump].x1 = v1[0];
            drawpoly[nump].y1 = v1[1];
            drawpoly[nump].z1 = v1[2];
            drawpoly[nump].x2 = v2[0];
            drawpoly[nump].y2 = v2[1];
            drawpoly[nump].z2 = v2[2];
            drawpoly[nump].x3 = v3[0];
            drawpoly[nump].y3 = v3[1];
            drawpoly[nump].z3 = v3[2];
            drawpoly[nump].xnorm = n[0];
            drawpoly[nump].ynorm = n[1];
            drawpoly[nump].znorm = n[2];
            drawpoly[nump].iclr1 = i;
            drawpoly[nump].iclr2 = i;
            drawpoly[nump].iclr3 = i;
            if (transflg == 0) numtris++;
            else numtris2++;
           }         
        }
     }
}
