#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
//#include <malloc.h>
#include <math.h>

#include "gmvdata.h"
#include "gmvread.h"

#define CHAR 0
#define SHORT 1
#define INT 2
#define FLOAT 3
#define WORD 4

#define CHARSIZE 1
#define SHORTSIZE 2
#define INTSIZE 4
#define WORDSIZE 4
#define FLOATSIZE 4
#define LONGSIZE 4

#define MAXLEV 50
int nxc, nyc, nzc, numcell, numdim, maxnodealloc, maxfacealloc,
    maxedgealloc;
float xZ, yZ, zZ, Dx, Dy, Dz;
int *daughter, *mother, *lpoint, numpack, maxlevel,
    lopack[MAXLEV+1],hipack[MAXLEV+1], *levcell, *ltop,
    numtop, *lnxtlox, *lnxthix, *lnxtloy, *lnxthiy,
    *lnxtloz, *lnxthiz;
double *xc, *yc, *zc, *dxhalf, *dyhalf, *dzhalf;


void makegmvamr()
{
  /*                             */
  /*  Get and create amr cells.  */
  /*                             */
  int i, j, k, n;
  void bld_amr_mesh();

   /*  Get amr data.  */
   nxc = gmv_meshdata.nxv;
   nyc = gmv_meshdata.nyv;
   nzc = gmv_meshdata.nzv;
   xZ = gmv_meshdata.x[0];   Dx = gmv_meshdata.x[1];
   yZ = gmv_meshdata.y[0];   Dy = gmv_meshdata.y[1];
   zZ = gmv_meshdata.z[0];   Dz = gmv_meshdata.z[1];
   free(gmv_meshdata.x);
   free(gmv_meshdata.y);
   free(gmv_meshdata.z);
   
   /*  Calculate dimension.  */
   numdim = 3;
   if (nzc < 2)
     {
      numdim = 2;
      nzc = 1;
     }
   if (nyc < 2) numdim = 1;
   if (numdim < 2)
     {
      fprintf(stderr,"Error, one dimensional AMR.\n");
      gmvexit();
     }

   /*  Get cumulative no. and no. of top level cells,  */
   /*  then allocate cell data for top level cells.    */
   numtop = gmv_meshdata.ncells;
   numcell = gmv_meshdata.nfaces;

   /*  Allocate numtop cells.  */
   ncells = numtop;
   cells = (struct cell *)malloc(ncells*sizeof(struct cell));
   if (cells == NULL) memerr();

   /*  Loop through cells and allocate    */ 
   /*  cell nodes, faces and edges data.  */
   n = 8;  j = 6;  k = 12;
   if (numdim == 2)
     {
      n = 4;  j = 1;  k = 4;
     }
   for (i = 0; i < ncells; i++)
     {
      cells[i].cnodes = (int *)malloc(n*sizeof(int));
      if (cells[i].cnodes == NULL) memerr();
      cells[i].facenos = (int *)malloc(j*sizeof(int));
      if (cells[i].facenos == NULL) memerr();
      cells[i].edgenos = (int *)malloc(k*sizeof(int));
      if (cells[i].edgenos == NULL) memerr();
     }

   /*  Allocate x,y and z arrays to be filled in gen_nodes.  */
   maxnodealloc = (nxc+1) * (nyc+1) * (nzc+1);
   if (2*numcell > maxnodealloc) maxnodealloc = 2 * numcell;
   nodes = (struct node *)malloc(maxnodealloc*sizeof(struct node));
   if (nodes == NULL) memerr();

   /*  Allocate faces to be filled in gen_faces.  */
   maxfacealloc = 4 * numcell;
   if (numdim == 2) maxfacealloc = numtop;
   cellfaces = (struct cellface *)
                  malloc(maxfacealloc*sizeof(struct cellface));
   if (cellfaces == NULL) memerr();

   /*  Allocate edges to be filled in gen_edges.  */
   maxedgealloc = 4 * numcell;
   if (numdim == 2) maxedgealloc = 2* numcell;
   celledges = (struct celledge *)
                  malloc(maxedgealloc*sizeof(struct celledge));
   if (celledges == NULL) memerr();

   /*  Allocate and fill daughter list.  */
   daughter = (int *)malloc((numcell+1)*sizeof(int));
   if (daughter == NULL) memerr();
   
   for (i = 0; i < numcell; i++)
      daughter[i+1] = gmv_meshdata.celltoface[i];
   free(gmv_meshdata.celltoface);

   printf("GMV read %d AMR daughters.\n",numcell);

   /*  Build and save amr mesh.  */
   bld_amr_mesh();
}


void bld_amr_mesh()
{
  /*                     */
  /*  Create amr cells.  */
  /*                     */
  void bldmthr();
  void bldpck();
  void bldlvl();
  void bldtop();
  void bldcoord();
  void bldnbh();
  void gen_nodes();
  void gen_faces();
  void gen_edges();

   /*  Generate mother list.  */
   bldmthr();

   /*  Generate lpoint, lopack and hipack list.  */
   bldpck();

   /*  Generate cell level array.  */
   bldlvl();

   /*  Identify top level cells.  */
   bldtop();

   /*  Build cell coordinates.  */
   bldcoord();

   /*  Build neighbor lists.  */
   bldnbh();

   printf("GMV created %d AMR cells.\n",numtop);

   /*  Build and fill node coordinates and cell node lists.  */
   gen_nodes();

   printf("GMV created %d AMR nodes.\n",nnodes);

   /*  Build and fill cell face list and cell face nos.  */
   gen_faces();

   printf("GMV created %d AMR faces.\n",ncellfaces);

   /*  Build and fill cell edge list and cell edge nos.  */
   gen_edges();

   printf("GMV created %d AMR edges.\n",ncelledges);

   /*  Free mesh arrays.  */
   free(daughter);  free(mother);  free(levcell);  free(lpoint);
   free(lnxtlox);  free(lnxtloy);  free(lnxtloz);
   free(lnxthix);  free(lnxthiy);  free(lnxthiz);
   free(ltop);

}


void bldmthr()
{
  /*                                             */
  /*  Build amr mother list from daughter list.  */
  /*                                             */
  int i, i1, n, pwr2;

   mother = (int *)malloc((numcell+1)*sizeof(int));
   if (mother == NULL) memerr();

   for (i = 0; i <= numcell; i++)
     mother[i] = 0;

   pwr2 = 2;
   for (i = 0; i < numdim - 1; i++)
     pwr2 *= 2;

   for (i = 1; i <= numcell; i++)
     {
      if (daughter[i] > 0)
        {
         i1 = daughter[i];
         for (n = 0; n < pwr2; n++)
           mother[i1+n] = i;
        }
     }
}


void bldpck()
{
  /*                                              */
  /*  Build the lpoint, lopack and hipack lists.  */
  /*                                              */
  int level, lp, l, lpsave, lpm, lm, loop, loopmx, pwr2;

   lpoint = (int *)malloc((numcell+1)*sizeof(int));
   if (lpoint == NULL) memerr();

   for (l = 0; l <= numcell; l++)
      lpoint[l] = 0;

   pwr2 = 2;
   for (l = 0; l < numdim - 1; l++)
     pwr2 *= 2;
   loopmx = pwr2 - 1;
 
   lp = 0;
   for (l = 1; l <= numcell; l++)
     {
      if (mother[l] == 0)
        {
         lp++;
         lpoint[lp] = l;
        }
     }

   lopack[1] = 1;
   hipack[1] = lp;
   lp = hipack[1];
 
   level = 1;
 
   lpsave = lp - 1;
   while(lp > lpsave)
     {
      lpsave = lp;
      for (lpm = lopack[level]; lpm <= hipack[level]; lpm++)
        {
         lm = lpoint[lpm];
         l = daughter[lm];
         if (l > 0)
           {
            for (loop = 0; loop <= loopmx; loop++)
              {
               lp++;
               lpoint[lp] = l + loop;
              }
           }
        }

      if (lp > lpsave)
        {
         level++;
         lopack[level] = lpsave + 1;
         hipack[level] = lp;
        }
     }

   numpack = level;
   maxlevel = level;

   if (lp != numcell)
     {
      fprintf(stderr,"Error - AMR, lp != numcell.\n");
      gmvexit();
     }
}


void bldlvl()
{
  /*                          */
  /*  Build cell level list.  */
  /*                          */
  int level, lp, l;

   levcell = (int *)malloc((numcell+1)*sizeof(int));
   if (levcell == NULL) memerr();

   for (l = 0; l <= numcell; l++)
     levcell[l] = 0;

   for (level = 1; level <= numpack; level++)
     {
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         l = lpoint[lp];
         levcell[l] = level;
        }
     }
}


void bldtop()
{
  /*                          */
  /*  Build cell level list.  */
  /*                          */
  int l, ntop;

   ltop = (int *)malloc((numcell+1)*sizeof(int));
   if (ltop == NULL) memerr();

   for (l = 0; l <= numcell; l++)
     ltop[l] = 0;

   ntop = 0;
   for (l = 1; l <= numcell; l++)
     {
      if (daughter[l] == 0)
        {
         ntop++;
         ltop[ntop] = l;
        }
     }

   /*  Check that calculated numtop is same as input.  */
   if (numtop != ntop)
     {
      fprintf(stderr,"Error, calculated numtop different from input.\n");
      gmvexit();
     }
}


void bldcoord()
{
  /*                                                    */
  /*  Build all xc, yc, zc and dxhalf, dyhalf, dzhalf.  */
  /*                                                    */
  int i, j, k, l, lp, level;
  int l1, l2, l3, l4, l5, l6, l7, l8;
  double dxo4, dyo4, dzo4, dx[MAXLEV+1], dy[MAXLEV+1], dz[MAXLEV+1];
 
   xc = (double *)malloc((numcell+1)*sizeof(double));
   if (xc == NULL) memerr();
   yc = (double *)malloc((numcell+1)*sizeof(double));
   if (yc == NULL) memerr();
   zc = (double *)malloc((numcell+1)*sizeof(double));
   if (zc == NULL) memerr();
   dxhalf = (double *)malloc((numcell+1)*sizeof(double));
   if (dxhalf == NULL) memerr();
   dyhalf = (double *)malloc((numcell+1)*sizeof(double));
   if (dyhalf == NULL) memerr();
   dzhalf = (double *)malloc((numcell+1)*sizeof(double));
   if (dzhalf == NULL) memerr();
 
   dx[1] = (double)Dx;
   dy[1] = (double)Dy;
   dz[1] = (double)Dz;
   if (numdim == 2) dz[1] = 0.;
 
   for (level=2; level <= maxlevel; level++)
     {
      dx[level] = (double)0.5 * dx[level-1];
      dy[level] = (double)0.5 * dy[level-1];
      dz[level] = (double)0.5 * dz[level-1];
     }
 
   for (k = 1; k <= nzc; k++)
     {
      for (j = 1; j <= nyc; j++)
        {
         for (i = 1; i <= nxc; i++)
           {
            l = (k-1)*nxc*nyc + (j-1)*nxc + i;
            xc[l] = xZ + ((double)i-0.5)*dx[1];
            yc[l] = yZ + ((double)j-0.5)*dy[1];
            zc[l] = zZ + ((double)k-0.5)*dz[1];
            dxhalf[l] = (double)0.5 * dx[1];
            dyhalf[l] = (double)0.5 * dy[1];
            dzhalf[l] = (double)0.5 * dz[1];
           }
        }
     }
 
   for (level = 1; level <= numpack-1; level++)
     {
      dxo4 = (double)0.5 * dx[level+1];
      dyo4 = (double)0.5 * dy[level+1];
      dzo4 = (double)0.5 * dz[level+1];
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         l = lpoint[lp];
         if (daughter[l] > 0)
           {
            l1 = daughter[l];
            l2 = l1 + 1;
            l3 = l2 + 1;
            l4 = l3 + 1;
            if (numdim == 3)
              {
               l5 = l4 + 1;
               l6 = l5 + 1;
               l7 = l6 + 1;
               l8 = l7 + 1;
              }
 
            dxhalf[l1] = dxo4;
            dyhalf[l1] = dyo4;
            dzhalf[l1] = dzo4;
            xc[l1] = xc[l] - dxo4;
            yc[l1] = yc[l] - dyo4;
            zc[l1] = zc[l] - dzo4;
 
            dxhalf[l2] = dxo4;
            dyhalf[l2] = dyo4;
            dzhalf[l2] = dzo4;
            xc[l2] = xc[l] + dxo4;
            yc[l2] = yc[l] - dyo4;
            zc[l2] = zc[l] - dzo4;
 
 
            dxhalf[l3] = dxo4;
            dyhalf[l3] = dyo4;
            dzhalf[l3] = dzo4;
            xc[l3] = xc[l] - dxo4;
            yc[l3] = yc[l] + dyo4;
            zc[l3] = zc[l] - dzo4;
 
            dxhalf[l4] = dxo4;
            dyhalf[l4] = dyo4;
            dzhalf[l4] = dzo4;
            xc[l4] = xc[l] + dxo4;
            yc[l4] = yc[l] + dyo4;
            zc[l4] = zc[l] - dzo4;
 
            if (numdim == 3)
              {
               dxhalf[l5] = dxo4;
               dyhalf[l5] = dyo4;
               dzhalf[l5] = dzo4;
               xc[l5] = xc[l] - dxo4;
               yc[l5] = yc[l] - dyo4;
               zc[l5] = zc[l] + dzo4;
 
               dxhalf[l6] = dxo4;
               dyhalf[l6] = dyo4;
               dzhalf[l6] = dzo4;
               xc[l6] = xc[l] + dxo4;
               yc[l6] = yc[l] - dyo4;
               zc[l6] = zc[l] + dzo4;
 
               dxhalf[l7] = dxo4;
               dyhalf[l7] = dyo4;
               dzhalf[l7] = dzo4;
               xc[l7] = xc[l] - dxo4;
               yc[l7] = yc[l] + dyo4;
               zc[l7] = zc[l] + dzo4;
 
               dxhalf[l8] = dxo4;
               dyhalf[l8] = dyo4;
               dzhalf[l8] = dzo4;
               xc[l8] = xc[l] + dxo4;
               yc[l8] = yc[l] + dyo4;
               zc[l8] = zc[l] + dzo4;
              }
           }
        }
     }
}


void bldnbh()
{
  /*                        */
  /*  Build neighbor list.  */
  /*                       */
  int i, j, k, l, lc, lp, ln, ldn0, level;
  int ldc0, ldc1, ldc2, ldc3, ldc4, ldc5, ldc6, ldc7,
      i0, i1, i2, i3, i4, i5, i6, i7;

   /*  Allocate arrays.  */ 
   lnxtlox = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxtlox == NULL) memerr();
   lnxthix = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxthix == NULL) memerr();

   lnxtloy = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxtloy == NULL) memerr();
   lnxthiy = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxthiy == NULL) memerr();

   lnxtloz = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxtloz == NULL) memerr();
   lnxthiz = (int *)malloc((numcell+1)*sizeof(int));
   if (lnxthiz == NULL) memerr();

   /*  Build level 1 neighbor links */
   for (l=1; l <= nxc*nyc*nzc; l++)
     {
      lnxtlox[l] = l;
      lnxthix[l] = l;
      lnxtloy[l] = l;
      lnxthiy[l] = l;
      lnxtloz[l] = l;
      lnxthiz[l] = l;
     }
 
   for (k = 1; k <= nzc; k++)
     {
      for (j = 1; j <= nyc; j++)
        {
         for (i = 1; i <= nxc; i++)
           {
            l = (k-1)*nxc*nyc + (j-1)*nxc + i;
            if (i > 1) lnxtlox[l] = l - 1;
            if (i < nxc) lnxthix[l] = l + 1;
            if (j > 1) lnxtloy[l] = l - nxc;
            if (j < nyc) lnxthiy[l] =l + nxc;
            if (k > 1) lnxtloz[l] = l - nxc*nyc;
            if (k < nzc) lnxthiz[l] =l + nxc*nyc;
           }
        }
     }
  
   /*  Build interior neighbor links for level > 1  */
   for (level = 1; level <= numpack; level++)
     {
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         lc = lpoint[lp];
         ldc0 = daughter[lc];
         if (ldc0 > 0)
           {
            ldc1 = ldc0 + 1;
            ldc2 = ldc1 + 1;
            ldc3 = ldc2 + 1;
            ldc4 = ldc3 + 1;
            ldc5 = ldc4 + 1;
            ldc6 = ldc5 + 1;
            ldc7 = ldc6 + 1;
 
            lnxthiy[ldc0] = ldc2;
            lnxthiy[ldc1] = ldc3;
  
            lnxtloy[ldc2] = ldc0;
            lnxtloy[ldc3] = ldc1;
 
            lnxthix[ldc0] = ldc1;
            lnxthix[ldc2] = ldc3;
 
            lnxtlox[ldc1] = ldc0;
            lnxtlox[ldc3] = ldc2;

            if (numdim == 3)
              {
               lnxthiy[ldc4] = ldc6;
               lnxthiy[ldc5] = ldc7;

               lnxtloy[ldc6] = ldc4;
               lnxtloy[ldc7] = ldc5;

               lnxthix[ldc4] = ldc5;
               lnxthix[ldc6] = ldc7;

               lnxtlox[ldc5] = ldc4;
               lnxtlox[ldc7] = ldc6;

               lnxthiz[ldc0] = ldc4;
               lnxthiz[ldc1] = ldc5;
               lnxthiz[ldc2] = ldc6;
               lnxthiz[ldc3] = ldc7;
 
               lnxtloz[ldc4] = ldc0;
               lnxtloz[ldc5] = ldc1;
               lnxtloz[ldc6] = ldc2;
               lnxtloz[ldc7] = ldc3;
              }
           }
        }
     }
 
   /*  Build exterior neighboor links for level > 1  */
   for (level = 1; level <= numpack; level++)
     {
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         lc = lpoint[lp];
         ldc0 = daughter[lc];
         if (ldc0 > 0)
           {
            ldc1 = ldc0 + 1;
            ldc2 = ldc1 + 1;
            ldc3 = ldc2 + 1;
            ldc4 = ldc3 + 1;
            ldc5 = ldc4 + 1;
            ldc6 = ldc5 + 1;
            ldc7 = ldc6 + 1;
 
            lnxtlox[ldc0] = ldc0;
            lnxtlox[ldc2] = ldc2;
 
            lnxthix[ldc1] = ldc1;
            lnxthix[ldc3] = ldc3;
 
            lnxtloy[ldc0] = ldc0;
            lnxtloy[ldc1] = ldc1;

            lnxthiy[ldc2] = ldc2;
            lnxthiy[ldc3] = ldc3;
 
            if (numdim == 3)
              {
               lnxtlox[ldc4] = ldc4;
               lnxtlox[ldc6] = ldc6;

               lnxthix[ldc5] = ldc5;
               lnxthix[ldc7] = ldc7;

               lnxtloy[ldc4] = ldc4;
               lnxtloy[ldc5] = ldc5;

               lnxthiy[ldc6] = ldc6;
               lnxthiy[ldc7] = ldc7;

               lnxtloz[ldc0] = ldc0;
               lnxtloz[ldc1] = ldc1;
               lnxtloz[ldc2] = ldc2;
               lnxtloz[ldc3] = ldc3;

               lnxthiz[ldc4] = ldc4;
               lnxthiz[ldc5] = ldc5;
               lnxthiz[ldc6] = ldc6;
               lnxthiz[ldc7] = ldc7;
              }

            i0 = 0;  i1 = 1;  i2 = 2;  i3 = 3;
            i4 = 4;  i5 = 5;  i6 = 6;  i7 = 7;
            if (numdim == 2)
              {
               i4 = 0;  i5 = 1;  i6 = 2;  i7 = 3;
               ldc4 = ldc0;  ldc5 = ldc1;
               ldc6 = ldc2;  ldc7 = ldc3;
              }
 
            ln = lnxtlox[lc];
            if (ln != lc)
              {
               ldn0 = daughter[ln];
               if (ldn0 > 0)
                 {
                  lnxtlox[ldc0] = ldn0 + i1;
                  lnxtlox[ldc2] = ldn0 + i3;
                  lnxtlox[ldc4] = ldn0 + i5;
                  lnxtlox[ldc6] = ldn0 + i7;
                 }
               else
                 {
                  lnxtlox[ldc0] = ln;
                  lnxtlox[ldc2] = ln;
                  lnxtlox[ldc4] = ln;
                  lnxtlox[ldc6] = ln;
                 }
              }
 
            ln = lnxthix[lc];
            if (ln != lc)
              {
               ldn0=daughter[ln];
               if (ldn0 > 0)
                 {
                  lnxthix[ldc1] = ldn0 + i0;
                  lnxthix[ldc3] = ldn0 + i2;
                  lnxthix[ldc5] = ldn0 + i4;
                  lnxthix[ldc7] = ldn0 + i6;
                 }
               else
                 {
                  lnxthix[ldc1] = ln;
                  lnxthix[ldc3] = ln;
                  lnxthix[ldc5] = ln;
                  lnxthix[ldc7] = ln;
                 }
              }
 
            ln = lnxtloy[lc];
            if (ln != lc)
              {
               ldn0 = daughter[ln];
               if (ldn0 > 0)
                 {
                  lnxtloy[ldc0] = ldn0 + i2;
                  lnxtloy[ldc1] = ldn0 + i3;
                  lnxtloy[ldc4] = ldn0 + i6;
                  lnxtloy[ldc5] = ldn0 + i7;
                 }
               else
                 {
                  lnxtloy[ldc0] = ln;
                  lnxtloy[ldc1] = ln;
                  lnxtloy[ldc4] = ln;
                  lnxtloy[ldc5] = ln;
                 }
              }
 
            ln = lnxthiy[lc];
            if (ln != lc)
              {
               ldn0 = daughter[ln];
               if (ldn0 > 0)
                 {
                  lnxthiy[ldc2] = ldn0 + i0;
                  lnxthiy[ldc3] = ldn0 + i1;
                  lnxthiy[ldc6] = ldn0 + i4;
                  lnxthiy[ldc7] = ldn0 + i5;
                 }
               else
                 {
                  lnxthiy[ldc2] = ln;
                  lnxthiy[ldc3] = ln;
                  lnxthiy[ldc6] = ln;
                  lnxthiy[ldc7] = ln;
                 }
              }
 
            if (numdim == 3)
              {
               ln = lnxtloz[lc];
               if (ln != lc)
                 {
                  ldn0=daughter[ln];
                  if (ldn0 > 0)
                    {
                     lnxtloz[ldc0] = ldn0 + i4;
                     lnxtloz[ldc1] = ldn0 + i5;
                     lnxtloz[ldc2] = ldn0 + i6;
                     lnxtloz[ldc3] = ldn0 + i7;
                    }
                  else
                    {
                     lnxtloz[ldc0] = ln;
                     lnxtloz[ldc1] = ln;
                     lnxtloz[ldc2] = ln;
                     lnxtloz[ldc3] = ln;
                    }
                 }
 
               ln = lnxthiz[lc];
               if (ln != lc)
                 {
                  ldn0 = daughter[ln];
                  if (ldn0 > 0)
                    {
                     lnxthiz[ldc4] = ldn0 + i0;
                     lnxthiz[ldc5] = ldn0 + i1;
                     lnxthiz[ldc6] = ldn0 + i2;
                     lnxthiz[ldc7] = ldn0 + i3;
                    }
                  else
                    {
                     lnxthiz[ldc4] = ln;
                     lnxthiz[ldc5] = ln;
                     lnxthiz[ldc6] = ln;
                     lnxthiz[ldc7] = ln;
                    }
                 }
              }
           }
        }
     }
}
