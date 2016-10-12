#include <stdlib.h>
#include <sys/types.h>
//#include <malloc.h>
#include <math.h>

#include "gmvdata.h"

#define MAXLEV 50
extern int nxc, nyc, nzc, numcell, numdim, maxnodealloc,
           maxfacealloc, maxedgealloc;
extern float xZ, yZ, zZ, Dx, Dy, Dz;
extern int *daughter, *mother, *lpoint, numpack, maxlevel,
           lopack[MAXLEV+1],hipack[MAXLEV+1], *levcell, *ltop,
           numtop, *lnxtlox, *lnxthix, *lnxtloy, *lnxthiy,
           *lnxtloz, *lnxthiz;
extern double *xc, *yc, *zc, *dxhalf, *dyhalf, *dzhalf;
int *node_index[8];


void gen_nodes()
{
/*                                                          */
/*  Build the node coordinates and fill the cell node nos.  */
/*                                                          */
  int i, j, icell, np;
  void gen_nodes_2d(), gen_nodes_3d();

   /*  Build the nodes for all cells,  */
   /*  and save the node coordinates.  */
   if (numdim == 2) gen_nodes_2d();
   if (numdim == 3) gen_nodes_3d();

   /*  Save the cell node numbers for the top level cells.  */
   np = 4;
   if (numdim == 3) np = 8;
   for (i = 1; i <= numtop; i++)
     {
      icell = ltop[i];
      cells[i-1].ncnodes = np;
      for (j = 0; j < np; j++)
         cells[i-1].cnodes[j] = node_index[j][icell];
     }

   /*  Release coordinate arrays.  */
   free(dxhalf);  free(dyhalf);  free(dzhalf);
   for (i = 0; i < np; i++)
      free(node_index[i]);
   if (numdim == 2)
     {  free(xc);  free(yc);  free(zc);  }
}

void gen_nodes_3d()
{
/*
 ======================================================================
 
    Purpose -
     Generates the 3D nodes and the node index for each cell.

        Order of the nodes (3D):


              2------------1
              |\           |\
              | \          | \
              |  \         |  \
              |   6--------+---5               +z (z into page)
              |   |        |   |                 \    +y
              3---+--------4   |                  \   |  
               \  |         \  |                   \  | 
                \ |          \ |                    \ |
                 \|           \|                     \|
                 7------------8                       ----------- +x
 
 ====================================================================== 
*/
 
  int i, j, k, l, ln, n, ix, iy,iz;
  int level, lp, lm, ldaut;
  double xmax, ymax, zmax;
  short new_node;
  short *cell_done;
  double epsilon = 1.0e-10;


   /*  Allocate cell node_index array.  */
   for (i = 0; i < 8; i++)
     {
      node_index[i] = (int *)malloc((numcell+1)*sizeof(int));
     }

   /*  Allocate cell_done check array.  */
   cell_done = (short *)malloc((numcell+1)*sizeof(short));

   for (l = 0; l <= numcell; l++)
      cell_done[l] = 0;

   for (n = 0; n < 8; n++)
     {
      for (l = 0; l <= numcell; l++)
         node_index[n][l] = -1;
     }

   /*  Create nodes for the level one mesh   */
   /*  (assumed to be logically connected).  */

   for (k = 1; k <= nzc+1; k++)
     {
      for (j = 1; j <= nyc+1; j++)
        {
         for (i = 1; i <= nxc+1; i++)
           {
            ln = i + (nxc+1)*(j-1) + (nxc+1)*(nyc+1)*(k-1);
            nodes[ln].x = xZ + (double)(i-1) * Dx;
            nodes[ln].y = yZ + (double)(j-1) * Dy;
            nodes[ln].z = zZ + (double)(k-1) * Dz;
           }
        }
     }

   for (k = 2; k <= nzc+1; k++)
     {
      for (j = 2; j <= nyc+1; j++)
        {
         for (i = 2; i <= nxc+1; i++)
           {
            l = (i-1) + nxc*(j-2)  + nxc*nyc*(k-2);
            ln = i + (nxc+1)*(j-1) + (nxc+1)*(nyc+1)*(k-1);
            cell_done[l] = 1;
            node_index[0][l] = ln;
            node_index[1][l] = ln - 1;
            node_index[2][l] = ln - 1 - (nxc+1);
            node_index[3][l] = ln - (nxc+1);
            node_index[4][l] = ln - (nxc+1)*(nyc+1);
            node_index[5][l] = ln - (nxc+1)*(nyc+1) - 1;
            node_index[6][l] = ln - (nxc+1)*(nyc+1) - 1 - (nxc+1);
            node_index[7][l] = ln - (nxc+1)*(nyc+1) - (nxc+1);
           }
        }
     }

   ln = (nxc+1) * (nyc+1) * (nzc+1);

   xmax = xZ + (double)nxc * Dx;
   ymax = yZ + (double)nyc * Dy;
   zmax = zZ + (double)nzc * Dz;

   /*  Now do all higher levels, starting from the bottom.  */
   for (level = 1; level <= numpack-1; level++)
     {
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         lm = lpoint[lp];
         l = daughter[lm];

         if (l > 0)
           {

            /*  Allocate more memory for x,y,z if needed.  */
            if ((ln+31) > maxnodealloc)
              {
               maxnodealloc += numcell;
               nodes = (struct node *)
                       realloc(nodes,maxnodealloc*sizeof(struct node));
              }


            /*  First child.  */

            /*  First possible new node.  */
            cell_done[l] = 1;
            ln = ln + 1;
            nodes[ln].x = xc[lm];
            nodes[ln].y = yc[lm];
            nodes[ln].z = zc[lm];
            node_index[0][l] = ln;
            node_index[6][l] = node_index[6][lm];

            /*  Second possible new node.  */
            new_node = 0;
            ix = lnxtlox[l];
            if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
               new_node = 1;
            else if (levcell[ix] < levcell[l])
               new_node = 1;
            else if (!cell_done[ix])
               new_node = 1;
            else
               node_index[1][l] = node_index[0][ix]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm];
               node_index[1][l] = ln;
              }

            /*  Third possible new node.  */
            new_node = 0;
            iy = lnxtloy[l];
            if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
               new_node = 1;
            else if (levcell[iy] < levcell[l])
               new_node = 1;
            else if (!cell_done[iy])
               new_node = 1;
            else
               node_index[3][l] = node_index[0][iy]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm]; 
               nodes[ln].y= yc[lm] - dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[3][l] = ln;
              }

            /*  Fourth possible new node.  */
            new_node = 0;
            iz = lnxtloz[l];
            if (fabs(zc[l]-dzhalf[l] - zZ) < epsilon)
               new_node = 1;
            else if (levcell[iz] < levcell[l])
               new_node = 1;
            else if (!cell_done[iz])
               new_node = 1;
            else
              node_index[4][l] = node_index[0][iz]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm] - dzhalf[lm];
               node_index[4][l] = ln;
              }

            /*  Fifth possible new node.  */
            new_node = 0;
            ix = lnxtlox[l];
            iy = lnxtloy[l];
            if ((fabs(xc[l]-dxhalf[l] - xZ) < epsilon) &&
                 (fabs(yc[l]-dyhalf[l] - yZ) < epsilon))
               new_node = 1;
            else if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[2][l] = node_index[3][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[2][l] = node_index[1][iy]; 
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[2][l] = node_index[3][ix];
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[2][l] = node_index[1][iy]; 
            else if ((levcell[lnxtlox[iy]] >= levcell[l]) && 
                     (cell_done[lnxtlox[iy]]))
               node_index[2][l] = node_index[0][lnxtlox[iy]]; 
            else if ((levcell[lnxtloy[ix]] >= levcell[l]) && 
                     (cell_done[lnxtloy[ix]]))
               node_index[2][l] = node_index[0][lnxtloy[ix]]; 
            else if (daughter[lnxtloy[ix]] > 0)
              {
               ldaut = daughter[lnxtloy[ix]];
               if (cell_done[ldaut+3])
                  node_index[2][l] = node_index[0][ldaut+3];
               else if (cell_done[ldaut+7])
                  node_index[2][l] = node_index[4][ldaut+7];
               else
                  new_node = 1;
              }
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm] - dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[2][l] = ln;
              }

            /*  Sixth possible new node.  */
            new_node = 0;
            ix = lnxtlox[l];
            iz = lnxtloz[l];
            if ((fabs(xc[l]-dxhalf[l] - xZ) < epsilon) &&
                 (fabs(zc[l]-dzhalf[l] - zZ) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]-dzhalf[l] - zZ) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[5][l] = node_index[4][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[5][l] = node_index[1][iz]; 
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[5][l] = node_index[4][ix]; 
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[5][l] = node_index[1][iz];
            else if ((levcell[lnxtlox[iz]] >= levcell[l]) && 
                     (cell_done[lnxtlox[iz]]))
               node_index[5][l] = node_index[0][lnxtlox[iz]]; 
            else if ((levcell[lnxtloz[ix]] >= levcell[l]) && 
                     (cell_done[lnxtloz[ix]]))
               node_index[5][l] = node_index[0][lnxtloz[ix]];
            else if (daughter[lnxtloz[ix]] > 0)
              {
               ldaut = daughter[lnxtloz[ix]];
               if (cell_done[ldaut+5])
                  node_index[5][l] = node_index[0][ldaut+5];
               else if (cell_done[ldaut+7]) 
                  node_index[5][l] = node_index[3][ldaut+7];
               else
                 new_node = 1;
              } 
            else
              new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z= zc[lm] - dzhalf[lm];
               node_index[5][l] = ln;
              }

            /*  Seventh possible new node.  */
            new_node = 0;
            iy = lnxtloy[l];
            iz = lnxtloz[l];
            if ((fabs(yc[l]-dyhalf[l] - yZ) < epsilon) &&
                 (fabs(zc[l]-dzhalf[l] - zZ) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]-dzhalf[l] - zZ) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[7][l] = node_index[4][iy]; 
               else
                  new_node = 1;
              }
            else if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[7][l] = node_index[3][iz];
               else
                  new_node = 1;
              }
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[7][l] = node_index[4][iy];
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[7][l] = node_index[3][iz];
            else if ((levcell[lnxtloy[iz]] >= levcell[l]) && 
                     (cell_done[lnxtloy[iz]]))
               node_index[7][l] = node_index[0][lnxtloy[iz]]; 
            else if ((levcell[lnxtloz[iy]] >= levcell[l]) && 
                     (cell_done[lnxtloz[iy]]))
               node_index[7][l] = node_index[0][lnxtloz[iy]];
            else if (daughter[lnxtloz[iy]] > 0)
              {
               ldaut = daughter[lnxtloz[iy]];
               if (cell_done[ldaut+6])
                  node_index[7][l] = node_index[0][ldaut+6];
               else if (cell_done[ldaut+7])
                  node_index[7][l] = node_index[1][ldaut+7];
               else
                  new_node = 1;
              }  
            else
              new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] - dyhalf[lm];
               nodes[ln].z = zc[lm] - dzhalf[lm];
               node_index[7][l] = ln;
              }


            /*  Second child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[5][l] = node_index[4][l-1];
            node_index[6][l] = node_index[7][l-1];
            node_index[7][l] = node_index[7][lm];

            /*  First possible new node.  */
            new_node = 0;
            ix = lnxthix[l];
            if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
               new_node = 1;
            else if (levcell[ix] < levcell[l])
               new_node = 1;
            else if (!cell_done[ix])
               new_node = 1;
            else
               node_index[0][l] = node_index[1][ix]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm];
               node_index[0][l] = ln;
              }

            /*  Second possible new node.  */
            new_node = 0;
            ix = lnxthix[l];
            iy = lnxtloy[l];
            if ((fabs(xc[l]+dxhalf[l] - xmax) < epsilon) &&
                 (fabs(yc[l]-dyhalf[l] - yZ) < epsilon))
               new_node = 1;
            else if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[3][l] = node_index[2][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[3][l] = node_index[0][iy];
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[3][l] = node_index[2][ix]; 
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[3][l] = node_index[0][iy]; 
            else if ((levcell[lnxthix[iy]] >= levcell[l]) && 
                     (cell_done[lnxthix[iy]]))
               node_index[3][l] = node_index[1][lnxthix[iy]]; 
            else if ((levcell[lnxtloy[ix]] >= levcell[l]) && 
                     (cell_done[lnxtloy[ix]]))
               node_index[3][l] = node_index[1][lnxtloy[ix]];
            else if (daughter[lnxtloy[ix]] > 0)
              {
               ldaut = daughter[lnxtloy[ix]];
               if (cell_done[ldaut+2])
                  node_index[3][l] = node_index[1][ldaut+2];
               else if (cell_done[ldaut+6])
                  node_index[3][l] = node_index[5][ldaut+6];
               else
                  new_node = 1;
              }   
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm] - dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[3][l] = ln;
              }

            /*  Third possible new node.  */
            new_node = 0;
            ix = lnxthix[l];
            iz = lnxtloz[l];
            if ((fabs(xc[l]+dxhalf[l] - xmax) < epsilon) &&
                 (fabs(zc[l]-dzhalf[l] - zZ) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]-dzhalf[l] - zZ) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[4][l] = node_index[5][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[4][l] = node_index[0][iz];
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[4][l] = node_index[5][ix]; 
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[4][l] = node_index[0][iz]; 
            else if ((levcell[lnxthix[iz]] >= levcell[l]) && 
                     (cell_done[lnxthix[iz]]))
                node_index[4][l] = node_index[1][lnxthix[iz]]; 
            else if ((levcell[lnxtloz[ix]] >= levcell[l]) && 
                     (cell_done[lnxtloz[ix]]))
               node_index[4][l] = node_index[1][lnxtloz[ix]];
            else if (daughter[lnxtloz[ix]] > 0)
              {
               ldaut = daughter[lnxtloz[ix]];
               if (cell_done[ldaut+4])
                  node_index[4][l] = node_index[1][ldaut+4];
               else if (cell_done[ldaut+6])
                  node_index[4][l] = node_index[2][ldaut+6];
               else
                  new_node = 1;
              }    
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm] - dzhalf[lm];
               node_index[4][l] = ln;
              }


            /*  Third child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[2][l] = node_index[1][l-2];
            node_index[3][l] = node_index[0][l-2];
            node_index[5][l] = node_index[5][lm];
            node_index[6][l] = node_index[5][l-2];
            node_index[7][l] = node_index[4][l-2];

            /*  Frist possible new node.  */
            new_node = 0;
            iy = lnxthiy[l];
            if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
               new_node = 1;
            else if (levcell[iy] < levcell[l])
               new_node = 1;
            else if (!cell_done[iy])
               new_node = 1;
            else
               node_index[0][l] = node_index[3][iy]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[0][l] = ln;
              }

            /*  Second possible new node.  */
            new_node = 0;
            ix = lnxtlox[l];
            iy = lnxthiy[l];
            if ((fabs(xc[l]-dxhalf[l] - xZ) < epsilon) &&
                 (fabs(yc[l]+dyhalf[l] - ymax) < epsilon))
               new_node = 1;
            else if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[1][l] = node_index[0][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[1][l] = node_index[2][iy]; 
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[1][l] = node_index[0][ix]; 
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[1][l] = node_index[2][iy]; 
            else if ((levcell[lnxtlox[iy]] >= levcell[l]) && 
                     (cell_done[lnxtlox[iy]]))
               node_index[1][l] = node_index[3][lnxtlox[iy]]; 
            else if ((levcell[lnxthiy[ix]] >= levcell[l]) && 
                     (cell_done[lnxthiy[ix]]))
               node_index[1][l] = node_index[3][lnxthiy[ix]];
            else if (daughter[lnxthiy[ix]] > 0)
              {
               ldaut = daughter[lnxthiy[ix]];
               if (cell_done[ldaut+1])
                  node_index[1][l] = node_index[3][ldaut+1];
               else if (cell_done[ldaut+5])
                  node_index[1][l] = node_index[7][ldaut+5];
               else
                  new_node = 1;
              }  
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[1][l] = ln;
              }

            /*  Third possible new node. */
            new_node = 0;
            iy = lnxthiy[l];
            iz = lnxtloz[l];
            if ((fabs(yc[l]+dyhalf[l] - ymax) < epsilon) &&
                 (fabs(zc[l]-dzhalf[l] - zZ) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]-dzhalf[l] - zZ) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[4][l] = node_index[7][iy]; 
               else
                  new_node = 1;
              }
            else if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[4][l] = node_index[0][iz];
               else
                  new_node = 1;
              }
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[4][l] = node_index[7][iy];
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[4][l] = node_index[0][iz]; 
            else if ((levcell[lnxthiy[iz]] >= levcell[l]) && 
                     (cell_done[lnxthiy[iz]]))
               node_index[4][l] = node_index[3][lnxthiy[iz]]; 
            else if ((levcell[lnxtloz[iy]] >= levcell[l]) && 
                     (cell_done[lnxtloz[iy]]))
               node_index[4][l] = node_index[3][lnxtloz[iy]];
            else if (daughter[lnxtloz[iy]] > 0)
              {
               ldaut = daughter[lnxtloz[iy]];
               if (cell_done[ldaut+4])
                  node_index[4][l] = node_index[3][ldaut+4];
               else if (cell_done[ldaut+5])
                  node_index[4][l] = node_index[2][ldaut+5];
               else
                  new_node = 1;
              }   
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               nodes[ln].z = zc[lm] - dzhalf[lm];
               node_index[4][l] = ln;
              }


            /*  Fourth child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[3][l] = node_index[0][l-2];
            node_index[4][l] = node_index[4][lm];
            node_index[5][l] = node_index[4][l-1];
            node_index[6][l] = node_index[7][l-1];
            node_index[7][l] = node_index[4][l-2];

            /*  First node.  */
            new_node = 0;
            ix = lnxthix[l];
            iy = lnxthiy[l];
            if ((fabs(xc[l]+dxhalf[l] - xmax) < epsilon) &&
                 (fabs(yc[l]+dyhalf[l] - ymax) < epsilon))
               new_node = 1;
            else if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[0][l] = node_index[1][ix];
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[0][l] = node_index[3][iy];
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[0][l] = node_index[1][ix]; 
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[0][l] = node_index[3][iy];
            else if ((levcell[lnxthix[iy]] >= levcell[l]) && 
                     (cell_done[lnxthix[iy]]))
               node_index[0][l] = node_index[2][lnxthix[iy]]; 
            else if ((levcell[lnxthiy[ix]] >= levcell[l]) && 
                     (cell_done[lnxthiy[ix]]))
               node_index[0][l] = node_index[2][lnxthiy[ix]];
            else if (daughter[lnxthiy[ix]] > 0)
              {
               ldaut = daughter[lnxthiy[ix]];
               if (cell_done[ldaut+0])
                  node_index[0][l] = node_index[2][ldaut+0];
               else if (cell_done[ldaut+4])
                  node_index[0][l] = node_index[6][ldaut+4];
               else
                  new_node = 1;
              } 
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               nodes[ln].z = zc[lm];
               node_index[0][l] = ln;
              }


            /*  Fifth child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[2][l] = node_index[2][lm];
            node_index[4][l] = node_index[0][l-4];
            node_index[5][l] = node_index[1][l-4];
            node_index[6][l] = node_index[2][l-4];
            node_index[7][l] = node_index[3][l-4];

            /*  First node.  */
            new_node = 0;
            iy = lnxthiy[l];
            if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              new_node = 1;
            else if (levcell[iy] < levcell[l]) 
                new_node = 1;
            else if (!cell_done[iy])
               new_node = 1;
            else
               node_index[0][l] = node_index[3][iy]; 

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm] + dzhalf[lm];
               node_index[0][l] = ln;
              }

            /*  Second node.  */
            new_node = 0;
            ix = lnxtlox[l];
            iz = lnxthiz[l];
            if ((fabs(xc[l]-dxhalf[l] - xZ) < epsilon) &&
                 (fabs(zc[l]+dzhalf[l] - zmax) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]+dzhalf[l] - zmax) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[1][l] = node_index[0][ix]; 
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[1][l] = node_index[5][iz]; 
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[1][l] = node_index[0][ix]; 
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[1][l] = node_index[5][iz];
            else if ((levcell[lnxtlox[iz]] >= levcell[l]) && 
                     (cell_done[lnxtlox[iz]]))
               node_index[1][l] = node_index[4][lnxtlox[iz]]; 
            else if ((levcell[lnxthiz[ix]] >= levcell[l]) && 
                     (cell_done[lnxthiz[ix]]))
               node_index[1][l] = node_index[4][lnxthiz[ix]];
            else if (daughter[lnxthiz[ix]] > 0)
              {
               ldaut = daughter[lnxthiz[ix]];
               if (cell_done[ldaut+1])
                  node_index[1][l] = node_index[4][ldaut+1];
               else if (cell_done[ldaut+3])
                  node_index[1][l] = node_index[7][ldaut+3];
               else
                  new_node = 1;
              }  
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm] + dzhalf[lm];
               node_index[1][l] = ln;
              }

            /*  Third node.  */
            new_node = 0;
            iy = lnxtloy[l];
            iz = lnxthiz[l];
            if ((fabs(yc[l]-dyhalf[l] - yZ) < epsilon) &&
                 (fabs(zc[l]+dzhalf[l] - zmax) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]+dzhalf[l] - zmax) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[3][l] = node_index[0][iy]; 
               else
                  new_node = 1;
              }
            else if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[3][l] = node_index[7][iz];
               else
                  new_node = 1;
              }
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[3][l] = node_index[0][iy];
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[3][l] = node_index[7][iz];
            else if ((levcell[lnxtloy[iz]] >= levcell[l]) && 
                     (cell_done[lnxtloy[iz]]))
               node_index[3][l] = node_index[4][lnxtloy[iz]];
            else if ((levcell[lnxthiz[iy]] >= levcell[l]) && 
                     (cell_done[lnxthiz[iy]]))
               node_index[3][l] = node_index[4][lnxthiz[iy]];
            else if (daughter[lnxthiz[iy]] > 0)
              {
               ldaut = daughter[lnxthiz[iy]];
               if (cell_done[ldaut+2])
                  node_index[3][l] = node_index[4][ldaut+2];
               else if (cell_done[ldaut+3])
                  node_index[3][l] = node_index[5][ldaut+3];
               else
                  new_node = 1;
              }   
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] - dyhalf[lm];
               nodes[ln].z = zc[lm] + dzhalf[lm];
               node_index[3][l] = ln;
              }


            /*  Sixth child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[3][l] = node_index[3][lm];
            node_index[4][l] = node_index[0][l-4];
            node_index[5][l] = node_index[1][l-4];
            node_index[6][l] = node_index[2][l-4];
            node_index[7][l] = node_index[3][l-4];

            /*  First node.  */
            new_node = 0;
            ix = lnxthix[l];
            iz = lnxthiz[l];
            if ((fabs(xc[l]+dxhalf[l] - xmax) < epsilon) &&
                 (fabs(zc[l]+dzhalf[l] - zmax) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]+dzhalf[l] - zmax) < epsilon)
              {
               if (levcell[ix] < levcell[l])
                  new_node = 1;
               else if (cell_done[ix])
                  node_index[0][l] = node_index[1][ix];
               else
                  new_node = 1;
              }
            else if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[0][l] = node_index[4][iz]; 
               else
                  new_node = 1;
              }
            else if ((levcell[ix] >= levcell[l]) && 
                     (cell_done[ix]))
               node_index[0][l] = node_index[1][ix]; 
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[0][l] = node_index[4][iz];
            else if ((levcell[lnxthix[iz]] >= levcell[l]) && 
                     (cell_done[lnxthix[iz]]))
               node_index[0][l] = node_index[5][lnxthix[iz]];
            else if ((levcell[lnxthiz[ix]] >= levcell[l]) && 
                     (cell_done[lnxthiz[ix]]))
               node_index[0][l] = node_index[5][lnxthiz[ix]];
            else if (daughter[lnxthiz[ix]] > 0)
              {
               ldaut = daughter[lnxthiz[ix]];
               if (cell_done[ldaut+0])
                 node_index[0][l] = node_index[5][ldaut+0];
               else if (cell_done[ldaut+2])
                 node_index[0][l] = node_index[6][ldaut+2];
               else
                 new_node = 1;
              }  
            else
              new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               nodes[ln].z = zc[lm] + dzhalf[lm];
               node_index[0][l] = ln;
              }


            /*  Seventh child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[1][lm];
            node_index[2][l] = node_index[1][l-2];
            node_index[3][l] = node_index[0][l-2];
            node_index[4][l] = node_index[0][l-4];
            node_index[5][l] = node_index[1][l-4];
            node_index[6][l] = node_index[2][l-4];
            node_index[7][l] = node_index[3][l-4];

            /*  First node.  */
            new_node = 0;
            iy = lnxthiy[l];
            iz = lnxthiz[l];
            if ((fabs(yc[l]+dyhalf[l] - ymax) < epsilon) &&
                 (fabs(zc[l]+dzhalf[l] - zmax) < epsilon))
               new_node = 1;
            else if (fabs(zc[l]+dzhalf[l] - zmax) < epsilon)
              {
               if (levcell[iy] < levcell[l])
                  new_node = 1;
               else if (cell_done[iy])
                  node_index[0][l] = node_index[3][iy];
               else
                  new_node = 1;
              }
            else if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              {
               if (levcell[iz] < levcell[l])
                  new_node = 1;
               else if (cell_done[iz])
                  node_index[0][l] = node_index[4][iz]; 
               else
                  new_node = 1;
              }
            else if ((levcell[iy] >= levcell[l]) && 
                     (cell_done[iy]))
               node_index[0][l] = node_index[3][iy]; 
            else if ((levcell[iz] >= levcell[l]) && 
                     (cell_done[iz]))
               node_index[0][l] = node_index[4][iz]; 
            else if ((levcell[lnxthiy[iz]] >= levcell[l]) && 
                     (cell_done[lnxthiy[iz]]))
               node_index[0][l] = node_index[7][lnxthiy[iz]]; 
            else if ((levcell[lnxthiz[iy]] >= levcell[l]) && 
                     (cell_done[lnxthiz[iy]]))
              {
               node_index[0][l] = node_index[7][lnxthiz[iy]];
               node_index[0][l] = node_index[5][lnxthiz[lnxthix[l]]];
              }
            else if (daughter[lnxthiz[iy]] > 0)
              {
               ldaut = daughter[lnxthiz[iy]];
               if (cell_done[ldaut+0])
                  node_index[0][l] = node_index[7][ldaut+0];
               else if (cell_done[ldaut+1])
                  node_index[0][l] = node_index[6][ldaut+1];
               else
                 new_node = 1;
              }   
            else
               new_node = 1;

            if (new_node)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               nodes[ln].z = zc[lm] + dzhalf[lm];
               node_index[0][l] = ln;
              }


            /*  Eighth child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[0][l] = node_index[0][lm];
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[3][l] = node_index[0][l-2];
            node_index[4][l] = node_index[0][l-4];
            node_index[5][l] = node_index[1][l-4];
            node_index[6][l] = node_index[2][l-4];
            node_index[7][l] = node_index[3][l-4];

          }
       }
   }

   free(cell_done);

   /*  Save the number of nodes and reallocate x,y,z arrays.  */
   nnodes = ln;
   nodes = (struct node *)realloc(nodes,(nnodes+1)*sizeof(struct node));
}


void gen_nodes_2d()
{
/*
 ======================================================================
 
   Purpose -
    Generates the 2D nodes and the node index for each cell.

       Order of the nodes (2D):

                                    +y (z)
             2------------1          |
             |            |          |
             |            |          |
             |            |          |
             3------------4          +----------  +x (r)
 
 ======================================================================
 */ 
 
  int i, j, l, ln, n;
  int level, lp, lm;
  float xmax, ymax;
  short *cell_done;
  double epsilon = 1.0e-10;


   /*  Allocate cell node_index array.  */
   for (i = 0; i < 4; i++)
     {
      node_index[i] = (int *)malloc((numcell+1)*sizeof(int));
     }

   /*  Allocate cell_done check array.  */
   cell_done = (short *)malloc((numcell+1)*sizeof(short));

   for (l = 0; l <= numcell; l++)
      cell_done[l] = 0;

   for (n = 0; n < 4; n++)
     {
      for (l = 0; l <= numcell; l++)
         node_index[n][l] = -1;
     }

   /*  Create nodes for the level one mesh   */
   /*  (assumed to be logically connected).  */

   for (j = 1; j <= nyc+1; j++)
     {
      for (i = 1; i <= nxc+1; i++)
        {
         ln = i + (nxc+1)*(j-1);
         nodes[ln].x = xZ + (double)(i-1) * Dx;
         nodes[ln].y = yZ + (double)(j-1) * Dy;
        }
     }

   for (j = 2; j <= nyc+1; j++)
     {
      for (i = 2; i <= nxc+1; i++)
        {
         l = (i-1) + nxc*(j-2);
         ln = i + (nxc+1)*(j-1);
         cell_done[l] = 1;
         node_index[0][l] = ln;
         node_index[1][l] = ln - 1;
         node_index[2][l] = ln - nxc - 2;
         node_index[3][l] = ln - nxc - 1;
        }
     }

   ln = (nxc+1) * (nyc+1);

   xmax = xZ + (double)(nxc) * Dx;
   ymax = yZ + (double)(nyc) * Dy;

   /*  Now do all higher levels, starting from the bottom.  */
   for (level = 1; level <= numpack-1; level++)
     {
      for (lp = lopack[level]; lp <= hipack[level]; lp++)
        {
         lm = lpoint[lp];
         l  = daughter[lm];

         if (l > 0)
           {


            /*  First child.  */

            cell_done[l] = 1;
            ln = ln + 1;
            nodes[ln].x = xc[lm];
            nodes[ln].y = yc[lm];
            node_index[0][l] = ln;
            node_index[2][l] = node_index[2][lm];

            if (fabs(xc[l]-dxhalf[l] - xZ) < epsilon)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[1][l] = ln;
              }
            else if (levcell[lnxtlox[l]] < levcell[l])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[1][l] = ln;
              }
            else if (!cell_done[lnxtlox[l]])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] - dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[1][l] = ln;
              }
            else
               node_index[1][l] = node_index[0][lnxtlox[l]]; 

            if (fabs(yc[l]-dyhalf[l] - yZ) < epsilon)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] ;
               nodes[ln].y = yc[lm] - dyhalf[lm];
               node_index[3][l] = ln;
              }
            else if (levcell[lnxtloy[l]] < levcell[l])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm]; 
               nodes[ln].y = yc[lm] - dyhalf[lm];
               node_index[3][l] = ln;
              }
            else if (!cell_done[lnxtloy[l]])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] - dyhalf[lm];
               node_index[3][l] = ln;
              }
            else
               node_index[3][l] = node_index[0][lnxtloy[l]]; 


            /*  Second child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[3][l] = node_index[3][lm];

            if (fabs(xc[l]+dxhalf[l] - xmax) < epsilon)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[0][l] = ln;
              }
            else if (levcell[lnxthix[l]] < levcell[l])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[0][l] = ln;
              }
            else if (!cell_done[lnxthix[l]])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm] + dxhalf[lm];
               nodes[ln].y = yc[lm];
               node_index[0][l] = ln;
              }
            else
               node_index[0][l] = node_index[1][lnxthix[l]]; 


            /* Third child.  */
            l = l + 1;

            cell_done[l] = 1;
            node_index[1][l] = node_index[1][lm];
            node_index[2][l] = node_index[1][l-2];
            node_index[3][l] = node_index[0][l-2];

            if (fabs(yc[l]+dyhalf[l] - ymax) < epsilon)
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               node_index[0][l] = ln;
              }
            else if (levcell[lnxthiy[l]] < levcell[l])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               node_index[0][l] = ln;
              }
            else if (!cell_done[lnxthiy[l]])
              {
               ln = ln + 1;
               nodes[ln].x = xc[lm];
               nodes[ln].y = yc[lm] + dyhalf[lm];
               node_index[0][l] = ln;
              }
            else
               node_index[0][l] = node_index[3][lnxthiy[l]]; 


            /*  Fourth child. */
            l = l + 1;

            cell_done[l] = 1;
            node_index[0][l] = node_index[0][lm];
            node_index[1][l] = node_index[0][l-1];
            node_index[2][l] = node_index[3][l-1];
            node_index[3][l] = node_index[0][l-2];

           }
        }
     }

   free(cell_done);

   /*  Save the number of nodes and reallocate x,y,z arrays.  */
   nnodes = ln;
   nodes = (struct node *)realloc(nodes,(nnodes+1)*sizeof(struct node));

   /*  Fill z array.  */
   for (i = 1; i <= nnodes; i++)
     nodes[i].z = zZ;
}


void gen_faces()
{
  /*                                    */
  /*  Generate and save the face list.  */
  /*                                    */
  void gen_faces_2d();
  void gen_faces_3d();

   if (numdim == 2) gen_faces_2d();
   if (numdim == 3) gen_faces_3d();
}


void gen_faces_2d()
{
  /*                                            */
  /*  Generate and save face list for 2D mesh.  */
  /*  Note: 2D top level cells are the faces.   */
  /*                                            */
  int i, j, n;

   for (i = 0; i < numtop; i++)
     {
      n = ltop[i+1];
      cellfaces[i].nverts = 4;
      cellfaces[i].fverts = (int *)malloc(4*sizeof(int));
      for (j = 0; j < 4; j++)
         cellfaces[i].fverts[j] = cells[i].cnodes[j];

      cells[i].nfaces = 1;
      cells[i].facenos[0] = i;
      cellfaces[i].cell1 = i;
      cellfaces[i].cell2 = -1;
     }

   /*  Save the number of faces and reallocate faces.  */
   ncellfaces = numtop;
}


void gen_faces_3d()
{
/* 
 ======================================================================
 
   Purpose -
    Generates the 3D faces and the face index for each cell.

       Face
         1      HiX
         2      LoX
         3      HiY
         4      LoY
         5      HiZ
         6      LoZ

       Order of the nodes (3D):


             2------------1
             |\           |\
             | \          | \
             |  \         |  \
             |   6--------+---5               +z (z into page)
             |   |        |   |                 \    +y
             3---+--------4   |                  \   |  
              \  |         \  |                   \  | 
               \ |          \ |                    \ |
                \|           \|                     \|
                 7------------8                       ----------- +x
 
 ======================================================================
*/
  int i, l, n, numf, nf, lnext, nfnext, nindx, pf;
  int *cell_face[6], nodeindex[8];
  static int face_node_index[6][4] = { { 1, 4, 8, 5 },
                                       { 2, 3, 7, 6 },
                                       { 1, 2, 6, 5 },
                                       { 4, 3, 7, 8 },
                                       { 1, 2, 3, 4 },
                                       { 5, 6, 7, 8 } };


   /*  Allocate and fill cell face test array.  */
   for (i = 0; i < 6; i++)
     {
      cell_face[i] = (int *)malloc((numcell+1)*sizeof(int));
     }
   for (i = 0; i < 6; i++)
     {
      for (l = 0; l <= numcell; l++)
        cell_face[i][l] = -1;
     }

   /*  Generate faces for top level cells.  */
   numf = 0;
   for (n = 1; n <= numtop; n++)
     {
      l = ltop[n];

      /*  Save cell node list.  */
      for (i = 0; i < 8; i++)
         nodeindex[i] = cells[n-1].cnodes[i];

      /*  Build the six cell faces as needed.  */
      for (nf = 0; nf < 6; nf++)
        {

         /*  If this face has been built,  */
         /*  set cell2 for the face.       */
         if (cell_face[nf][l] > 0)
           {
            pf = cell_face[nf][l];
            cellfaces[pf].cell2 = n - 1;
           }

         /*  If the face has not been built, build it.  */
         if (cell_face[nf][l] < 0)
           {

            /*  Allocate more memory for faces if needed.  */
            if ((numf + 1) > maxfacealloc)
              {
               maxfacealloc += numcell;
               cellfaces = (struct cellface *)realloc(cellfaces,
                              maxfacealloc*sizeof(struct cellface));
              }

            /*  Generate a new face.  */
            cellfaces[numf].nverts = 4;
            cellfaces[numf].fverts = (int *)malloc(4*sizeof(int));
            for (i = 0; i < 4; i++)
              {
               nindx = face_node_index[nf][i] - 1;
               cellfaces[numf].fverts[i] = nodeindex[nindx];
              }
            cellfaces[numf].cell1 = n - 1;
            cellfaces[numf].cell2 = -1;

            /*  Set the cell face list to face index.  */
            cell_face[nf][l] = numf;

            /*  Check the neighbor cell across the face.  */
            if (nf == 0)
              {
               lnext  = lnxthix[l];
               nfnext = 1;
              }
            else if (nf == 1)
              {
               lnext  = lnxtlox[l];
               nfnext = 0;
              }
            else if (nf == 2)
              {
               lnext  = lnxthiy[l];
               nfnext = 3;
              }
            else if (nf == 3)
              {
               lnext  = lnxtloy[l];
               nfnext = 2;
              }
            else if (nf == 4)
              {
               lnext  = lnxthiz[l];
               nfnext = 5;
              }
            else if (nf == 5)
              {
               lnext  = lnxtloz[l];
               nfnext = 4;
              }

            /*  Set shared faces in cell face list.  */
            if (l != lnext)
              {
               if (levcell[l] == levcell[lnext])
               cell_face[nfnext][lnext] = numf;
               cellfaces[numf].cell2 = lnext;
              }

            numf++;
           }
        }
     }

   /*  Save the number of faces and reallocate faces.  */
   ncellfaces = numf;
   cellfaces = (struct cellface *)realloc(cellfaces,
                              ncellfaces*sizeof(struct cellface));

   /*  Fill cell face numbers.  */
   for (n = 1; n <= numtop; n++)
     {
      cells[n-1].nfaces = 6;

      l = ltop[n];
      
      for (i = 0; i < 6; i++)
         cells[n-1].facenos[i] = cell_face[i][l];
     }

   for (i = 0; i < 6; i++)
      free(cell_face[i]); 
}


void gen_edges()
{
  /*                                    */
  /*  Generate and save the edge list.  */
  /*                                    */
  void gen_edges_2d();
  void gen_edges_3d();

   if (numdim == 2) gen_edges_2d();
   if (numdim == 3) gen_edges_3d();
}


void gen_edges_2d()
{
/*
 ======================================================================
 
   Purpose -
    Generates the 2D edges and the edge index for each cell.

       Edge
         1      HiX
         2      LoX
         3      HiY
         4      LoY

       Order of the nodes (2D):

                                    +y (z)
             2------------1          |
             |            |          |
             |            |          |
             |            |          |
             3------------4          +----------  +x (r)
 
 ======================================================================
*/
  int l, n, nume, ne, lnext, nenext, i;
  int nodeindex[4], nindx1, nindx2;
  int *cell_edge[4];
  int edge_node_index[4][2] = { { 1, 4 },
                                { 2, 3 },
                                { 1, 2 },
                                { 4, 3 } };


   /*  Allocate and fill cell edge test array.  */
   for (i = 0; i < 4; i++)
     {
      cell_edge[i] = (int *)malloc((numcell+1)*sizeof(int));
     }
   for (i = 0; i < 4; i++)
     {
      for (l = 0; l <= numcell; l++)
        cell_edge[i][l] = -1;
     }
   
   /*  Generate edges for top level cells.  */
   nume = 0;
   for (n = 1; n <= numtop; n++)
     {
      l = ltop[n];

      /*  Fill node index for this cell.  */
      for (i = 0; i < 4; i++)
         nodeindex[i] = cells[n-1].cnodes[i];

      /*  Build the 4 cell edges as needed.  */
      for (ne = 0; ne < 4; ne++)
        {
         if (cell_edge[ne][l] < 0)
           {

            /*  Allocate more memory for edges if needed.  */
            if ((nume + 1) > maxedgealloc)
              {
               maxedgealloc += numcell;
               celledges = (struct celledge *)realloc(celledges,
                               maxedgealloc*sizeof(struct celledge));
              }

            /*  Generate a new edge.  */
            nindx1 = edge_node_index[ne][0] - 1;
            nindx2 = edge_node_index[ne][1] - 1;
            celledges[nume].edgept1 = nodeindex[nindx1];
            celledges[nume].edgept2 = nodeindex[nindx2];

            /*  Set the cell to edge index.  */
            cell_edge[ne][l] = nume;

            /*  Check the neighbor cell across edge.  */
            if (ne == 0)
              {
               nenext = 1;
               lnext  = lnxthix[l];
              }
            else if (ne == 1)
              {
               nenext = 0;
               lnext  = lnxtlox[l];
              }
            else if (ne == 2)
              {
               nenext = 3;
               lnext  = lnxthiy[l];
              }
            else if (ne == 3)
              {
               nenext = 2;
               lnext  = lnxtloy[l];
              }

            /*  Set shared edges.  */
            if (l != lnext)
              {
               if (daughter[lnext] == 0)
                 {
                  if (levcell[l] == levcell[lnext])
                     cell_edge[nenext][lnext] = nume;
                 }
              }
         
            nume++;
           }
        }
     }

   /*  Save the number of edges and reallocate edge list.  */
   ncelledges = nume;  
   celledges = (struct celledge *)realloc(celledges,
                              ncelledges*sizeof(struct celledge));

   /*  Fill cell edge numbers.  */
   for (n = 1; n <= numtop; n++)
     {
      cells[n-1].nedges = 4;

      l = ltop[n];
      
      for (i = 0; i < 4; i++)
         cells[n-1].edgenos[i] = cell_edge[i][l];
     }

   for (i = 0; i < 4; i++)
      free(cell_edge[i]); 
}


void gen_edges_3d()
{
/*
 ======================================================================
 
   Purpose -
    Generates the 3D edges and the edge index for each cell.

       Edge
         1      HiX-HiZ
         2      LoX-HiZ
         3      HiY-HiZ
         4      LoY-HiZ
         5      HiX-LoZ
         6      LoX-LoZ
         7      HiY-LoZ
         8      LoY-LoZ
         9      HiX-HiY
        10      HiX-LoY
        11      LoX-HiY
        12      LoX-LoY

       Order of the nodes (3D):


             2------------1
             |\           |\
             | \          | \
             |  \         |  \
             |   6--------+---5               +z (z into page)
             |   |        |   |                 \    +y
             3---+--------4   |                  \   |  
              \  |         \  |                   \  | 
               \ |          \ |                    \ |
                \|           \|                     \|
                 7------------8                       ----------- +x
 
 ======================================================================
*/
  int l, n, nume, ne, i;
  int nodeindex[8], nindx1, nindx2;
  int lnext1, lnext2, lnext3;
  int nenext1, nenext2, nenext3;
  int *cell_edge[12];
  void find_daut_index(int l, int ne, int lnext);
  int edge_node_index[12][2] = { { 1, 4 },
                                 { 2, 3 },
                                 { 1, 2 },
                                 { 4, 3 },
                                 { 5, 8 },
                                 { 6, 7 },
                                 { 5, 6 },
                                 { 8, 7 },
                                 { 1, 5 },
                                 { 4, 8 },
                                 { 2, 6 },
                                 { 3, 7 } };


   /*  Allocate and fill cell edge test array.  */
   for (i = 0; i < 12; i++)
     {
      cell_edge[i] = (int *)malloc((numcell+1)*sizeof(int));
     }
   for (i = 0; i < 12; i++)
     {
      for (l = 0; l <= numcell; l++)
        cell_edge[i][l] = -1;
     }
   
   /*  Generate edges for top level cells.  */
   nume = 0;
   for (n = 1; n <= numtop; n++)
     {
      l = ltop[n];

      /*  Fill node index for this cell.  */
      for (i = 0; i < 8; i++)
         nodeindex[i] = cells[n-1].cnodes[i];

      /*  Build the 12 cell edges as needed.  */
      for (ne = 0; ne < 12; ne++)
        {
         if (cell_edge[ne][l] < 0)
           {

            /*  Allocate more memory for edges if needed.  */
            if ((nume + 1) > maxedgealloc)
              {
               maxedgealloc += numcell;
               celledges = (struct celledge *)realloc(celledges,
                               maxedgealloc*sizeof(struct celledge));
              }

            /*  Generate a new edge.  */
            nindx1 = edge_node_index[ne][0] - 1;
            nindx2 = edge_node_index[ne][1] - 1;
            celledges[nume].edgept1 = nodeindex[nindx1];
            celledges[nume].edgept2 = nodeindex[nindx2];

            /*  Set the cell to edge index.  */
            cell_edge[ne][l] = nume;

            /*  Check the neighbor cell across edge.  */
            lnext3 = -1;

            if (ne == 0)
              {
               lnext1 = lnxthix[l];
               lnext2 = lnxthiz[l];
               if (lnext1 != l && lnxthiz[l] != l)
                  lnext3 = lnxthix[lnxthiz[l]];
               else if (lnext2 != l && lnxthix[l] != l)
                  lnext3 = lnxthiz[lnxthix[l]];
               nenext1 = 2;
               nenext2 = 5;
               nenext3 = 6;
              }
            else if (ne == 1)
              {
               lnext1 = lnxtlox[l];
               lnext2 = lnxthiz[l];
               if (lnext1 != l && lnxthiz[l] != l)
                  lnext3 = lnxtlox[lnxthiz[l]];
               else if (lnext2 != l && lnxtlox[l] != l)
                  lnext3 = lnxthiz[lnxtlox[l]];
               nenext1 = 1;
               nenext2 = 6;
               nenext3 = 5;
              }
            else if (ne == 2)
              {
               lnext1 = lnxthiy[l];
               lnext2 = lnxthiz[l];
               if (lnext1 != l && lnxthiz[l] != l)
                  lnext3 = lnxthiy[lnxthiz[l]];
               else if (lnext2 != l && lnxthiy[l] != l)
                  lnext3 = lnxthiz[lnxthiy[l]];
               nenext1 = 4;
               nenext2 = 7;
               nenext3 = 8;
              }
            else if (ne == 3)
              {
               lnext1 = lnxtloy[l];
               lnext2 = lnxthiz[l];
               if (lnext1 != l && lnxthiz[l] != l)
                  lnext3 = lnxtloy[lnxthiz[l]];
                else if (lnext2 != l && lnxtloy[l] != l)
                 lnext3 = lnxthiz[lnxtloy[l]];
               nenext1 = 3;
               nenext2 = 8;
               nenext3 = 7;
              }
            else if (ne == 4)
              {
               lnext1 = lnxthix[l];
               lnext2 = lnxtloz[l];
               if (lnext1 != l && lnxtloz[l] != l)
                  lnext3 = lnxthix[lnxtloz[l]];
               else if (lnext2 != l && lnxthix[l] != l)
                  lnext3 = lnxtloz[lnxthix[l]];
               nenext1 = 6;
               nenext2 = 1;
               nenext3 = 2;
              }
            else if (ne == 5)
              {
               lnext1 = lnxtlox[l];
               lnext2 = lnxtloz[l];
               if (lnext1 != l && lnxtloz[l] != l)
                  lnext3 = lnxtlox[lnxtloz[l]];
               else if (lnext2 != l && lnxtlox[l] != l)
                  lnext3 = lnxtloz[lnxtlox[l]];
               nenext1 = 5;
               nenext2 = 2;
               nenext3 = 1;
              }
            else if (ne == 6)
              {
               lnext1 = lnxthiy[l];
               lnext2 = lnxtloz[l];
               if (lnext1 != l && lnxtloz[l] != l)
                  lnext3 = lnxthiy[lnxtloz[l]];
               else if (lnext2 != l && lnxthiy[l] != l)
                  lnext3 = lnxtloz[lnxthiy[l]];
               nenext1 = 8;
               nenext2 = 3;
               nenext3 = 4;
              }
            else if (ne == 7)
              {
               lnext1 = lnxtloy[l];
               lnext2 = lnxtloz[l];
               if (lnext1 != l && lnxtloz[l] != l)
                  lnext3 = lnxtloy[lnxtloz[l]];
               else if (lnext2 != l && lnxtloy[l])
                  lnext3 = lnxtloz[lnxtloy[l]];
               nenext1 = 7;
               nenext2 = 4;
               nenext3 = 3;
              }
            else if (ne == 8)
              {
               lnext1 = lnxthix[l];
               lnext2 = lnxthiy[l];
               if (lnext1 != l && lnxthiy[l] != l)
                  lnext3 = lnxthix[lnxthiy[l]];
               else if (lnext2 != l && lnxthix[l] != l)
                 lnext3 = lnxthiy[lnxthix[l]];
               nenext1 = 11;
               nenext2 = 10;
               nenext3 = 12;
              }
            else if (ne == 9)
              {
               lnext1 = lnxthix[l];
               lnext2 = lnxtloy[l];
               if (lnext1 != l && lnxtloy[l] != l)
                  lnext3 = lnxthix[lnxtloy[l]];
               else if (lnext2 != l && lnxthix[l] != l)
                  lnext3 = lnxtloy[lnxthix[l]];
               nenext1 = 12;
               nenext2 = 9;
               nenext3 = 11;
              }
            else if (ne == 10)
              {
               lnext1 = lnxtlox[l];
               lnext2 = lnxthiy[l];
               if (lnext1 != l && lnxthiy[l] != l)
                 lnext3 = lnxtlox[lnxthiy[l]];
               else if (lnext2 != l && lnxtlox[l] != l)
                 lnext3 = lnxthiy[lnxtlox[l]];
               nenext1 = 9;
               nenext2 = 12;
               nenext3 = 10;
              }
            else if (ne == 11)
              {
               lnext1 = lnxtlox[l];
               lnext2 = lnxtloy[l];
               if (lnext1 != l && lnxtloy[l] != l)
                  lnext3 = lnxtlox[lnxtloy[l]];
               else if (lnext2 != l && lnxtlox[l] != l)
                  lnext3 = lnxtloy[lnxtlox[l]];
               nenext1 = 10;
               nenext2 = 11;
               nenext3 = 9;
              }

            if (lnext3 == lnext1 || lnext3 == lnext2)
               lnext3 = -1;

            /*  Set shared edges. */
            nenext1--;
            nenext2--;
            nenext3--;
            if (l != lnext1)
              {
               if (daughter[lnext1] == 0)
                 {
                  if (levcell[l] == levcell[lnext1])
                     cell_edge[nenext1][lnext1] = nume;
                 }
              }

            if (l != lnext2)
              {
               if (daughter[lnext2] == 0)
                 {
                  if (levcell[l] == levcell[lnext2])
                     cell_edge[nenext2][lnext2] = nume;
                 }
              }

            if (lnext3 > 0)
              {
               if (daughter[lnext3] == 0)
                 {
                  if (levcell[l] == levcell[lnext3])
                     cell_edge[nenext3][lnext3] = nume;
                 }
               else if (levcell[l] == levcell[lnext3]+1)
                 {
                  find_daut_index(l, ne, lnext3);
                  if (lnext3 > 0)
                     cell_edge[nenext3][lnext3] = nume;
                 }
              }
         
            nume++;
           }
        }
     }

   /*  Save the number of edges and reallocate edge list.  */
   ncelledges = nume;  
   celledges = (struct celledge *)realloc(celledges,
                              ncelledges*sizeof(struct celledge));

   /*  Fill cell edge numbers.  */
   for (n = 1; n <= numtop; n++)
     {
      cells[n-1].nedges = 12;

      l = ltop[n];
      
      for (i = 0; i < 12; i++)
         cells[n-1].edgenos[i] = cell_edge[i][l];
     }   

   free(xc);  free(yc);  free(zc);
   for (i = 0; i < 12; i++)
      free(cell_edge[i]);
}


void find_daut_index(int l, int ne, int lnext)
{
  /*                               */
  /*  Returns the daughter index.  */
  /*                               */
  int lm, index, offset;
  int neighbor[8][12] = { 
                   { -1, -1, -1, -1, -1,  5, -1,  6, -1, -1, -1,  3 },
                   { -1, -1, -1, -1,  4, -1, -1,  7, -1,  2, -1, -1 },
                   { -1, -1, -1, -1, -1,  7,  4, -1, -1, -1,  1, -1 },
                   { -1, -1, -1, -1,  6, -1,  5, -1,  0, -1, -1, -4 },
                   { -1,  1, -1,  2, -1, -1, -1, -1, -1, -1, -1,  7 },
                   {  0, -1, -1,  3, -1, -1, -1, -1, -1,  6, -1, -1 },
                   { -1,  3,  0, -1, -1, -1, -1, -1, -1, -1,  5, -1 },
                   {  2, -1,  1, -1, -1, -1, -1, -1,  4, -1, -1, -1 } };


   lm = mother[l];

   if (l < 1 || l > numcell || lm < 1 || lm > numcell) gmvexit();

   if (zc[l] < zc[lm])
     {
      if (yc[l] < yc[lm])
        {
         if (xc[l] < xc[lm])
            index = 0;
         else
            index = 1;
        }
      else
        {
         if (xc[l] < xc[lm])
            index = 2;
         else
            index = 3;
        }
     }

   else
     {
      if (yc[l] < yc[lm])
        {
         if (xc[l] < xc[lm])
            index = 4;
         else
            index = 5;
        }
      else
        {
         if (xc[l] < xc[lm])
            index = 6;
         else
            index = 7;
        }
     }

   offset = neighbor[index][ne];
   if (offset < 0)
      lnext = -1;
   else
      lnext = daughter[lnext] + offset;

}
