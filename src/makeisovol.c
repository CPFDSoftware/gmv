#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "contour.h"
 
#define MSIZE 10000
#define CSIZE 10000
#define MAXVAL 9.99999e32

static int vpolysallocated, nvpolys;
static float xv[CSIZE], yv[CSIZE], zv[CSIZE], vfact[CSIZE];
static int vpt1[CSIZE], vpt2[CSIZE];
static char *vfacelist;
 
 
void makeisovol(volval1,volval2,nodedata,nvpolysout)
 float volval1, volval2;
 double *nodedata;
 int *nvpolysout;
 
 /*  This routine interpolates cell edges to create an  */
 /*  isovolume for the requested contour value.         */
 /*  volval1 - first isovolume value                    */
 /*  volval2 - second isovolume value                   */
 /*  nodedata - pointer to the data to interpolate      */
 /*  nvpolysout - no. of polygons generated.            */
 /*                                                     */
 /*  NOTE - Be sure to free conlist in                  */
 /*         calling routine.                            */
 
{
  int icell, gocell;
  int i, j, k, jj, np, ic, jcell, jface, iv;
  int nodeid[CSIZE], ncn, iedge, nvolfaces;
  float volmin, volmax, cvalue;
  double v1, v2, fact, vmax, vmin;
  double xno, yno, zno, denom, denomck, diff;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double xj, yj, zj, xk, yk, zk;
  void chkvolfaces(float volmin, float volmax, char *vfacelist, 
                   double *nodedata, int *nvolfaces);
  void facevol(int jface, float volmin, float volmax, 
               double *nodedata);
 
   nvpolys = 0;
 
   if ((conlist = (struct isocont *)malloc(MSIZE*
                       sizeof(struct isocont))) == NULL)
     {
      fprintf(stderr,
           "Makeisovolume conlist error, can't allocate any more memory.\n");
      gmvexit();
     }
   vpolysallocated = MSIZE;

   /*  Allocate and initialize vfacelist to 0.  */
   if ((vfacelist = (char *)malloc(ncellfaces*sizeof(char))) == NULL)
     {
      fprintf(stderr,
           "Makeisovolume vfacelist error, can't allocate any more memory.\n");
      gmvexit();
     }
   for (i = 0; i < ncellfaces; i++) vfacelist[i] = 0;

   /*  Order the isovolume range values.  */
   volmin = (volval1 < volval2) ? volval1 : volval2;
   volmax = (volval1 > volval2) ? volval1 : volval2;

   /*  See if any faces create an outer shell.  */
   chkvolfaces(volmin,volmax,vfacelist,nodedata,&nvolfaces);

   /*  Loop over cells.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  If ivcliponcellsel is turned on,    */ 
      /*  see if the cell has been selected.  */
      gocell = 1;
      if (ivcliponcellsel == 1) gocell = (int)cellselectflag[icell];

      /*  Eliminate cells with material < 1.  */
      if (cellmats[icell] < 1) gocell = 0;
 
      /*  If ivcliponbox is turned on, see     */
      /*  if the cell lies within the subset.  */
      if (ivcliponbox == 1 && (int)cellsubsetflag[icell] == 0)
         gocell = 0;

      /*  Proccess selected cells.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,3);
           }

         /*  Loop through faces and check face selection.  */
         if (nvolfaces > 0)
           {
            for (j = 0; j < cells[jcell].nfaces; j++)
              {
               k = cells[jcell].facenos[j];
               jface = k;
               if (structuredflag == 1) jface = j;

               /*  If face is selected, add to the isovol list.  */
               if ((int)vfacelist[k] == 1)
                  facevol(jface,volmin,volmax,nodedata);
              }
           }
 
         /*  See if cell min.-max. value  */
         /*  spans the isovolume values.  */
         vmin = MAXVAL;  vmax = -MAXVAL;
         ncn = (int)cells[jcell].ncnodes;
         for (i = 0; i < ncn; i++)
           {
            j = cells[jcell].cnodes[i];
            v1 = nodedata[j];
            vmin = (v1 < vmin) ? v1 : vmin;
            vmax = (v1 > vmax) ? v1 : vmax;
           }
         if (vmax < volmin || vmin > volmax) gocell = 0;
         if (volmin <= vmin && vmax <= volmax) gocell = 0;

         /*  Delete this cell if all     */
         /*  nodes have the same value.  */
         if (vmin == vmax) gocell = 0;
        }
 
      /*  Loop over cell edges, interpolate and get  */
      /*  Get x,y,z values for interpolated points.  */
      if (gocell == 1)
        {

         /*  Process cell twice, once for each value.  */
         for (iv = 0; iv < 2; iv++)
           {
            if (iv == 0) cvalue = volmin;
            else cvalue = volmax;

            np = 0;
            for (i = 0; i < cells[jcell].nedges; i++)
              {
               iedge = cells[jcell].edgenos[i];
               if (structuredflag == 1) iedge = i;
               j = celledges[iedge].edgept1;
               k = celledges[iedge].edgept2;
               xj = nodes[j].x;  yj = nodes[j].y;  zj = nodes[j].z;
               xk = nodes[k].x;  yk = nodes[k].y;  zk = nodes[k].z;
               v1 = nodedata[j];
               v2 = nodedata[k];
               vmin = (v1 <= v2) ? v1 : v2;
               vmax = (v1 >= v2) ? v1 : v2;
               denom = v1 - v2;
               denomck = (float)(fabs((double)denom));
               fact = 1.1;
               if (denomck > 1.0e-10)
                  fact = (v1 - cvalue) / denom;

               /*  Check values at vertices, and add  */
               /*  to list if not used previously.    */
               diff = (float)(fabs((double)(v1 - cvalue)));
               if (diff < 1.0e-7)
                 {
                  ic = -1;
                  for (jj = 0; jj < np; jj++)
                     if (nodeid[jj] == j) ic = jj;
                  if (ic == -1)
                    {
                     xv[np] = xj;
                     yv[np] = yj;
                     zv[np] = zj;
                     nodeid[np] = j;
                     vfact[np] = 0.;
                     vpt1[np] = j;
                     vpt2[np] = k;
                     np++;
                    }
                 }
               diff = (fabs((double)(v2 - cvalue)));
               if (diff < 1.0e-7)
                 {
                  ic = -1;
                  for (jj = 0; jj < np; jj++)
                     if (nodeid[jj] == k) ic = jj;
                  if (ic == -1)
                    {
                     xv[np] = xk;
                     yv[np] = yk;
                     zv[np] = zk;
                     vfact[np] = 1.0;
                     nodeid[np] = k;
                     vpt1[np] = j;
                     vpt2[np] = k;
                     np++;
                    }
                 }

               /*  If not at a vertex, interpolate point.  */
               if (fact > 0. && fact < 1.)
                 {
                  xv[np] = xj + fact * (xk - xj);
                  yv[np] = yj + fact * (yk - yj);
                  zv[np] = zj + fact * (zk - zj);
                  nodeid[np] = -1;
                  vfact[np] = fact;
                  vpt1[np] = j;
                  vpt2[np] = k;
                  np++;
                 }

               if (np > CSIZE)
                 {
                  fprintf(stderr,
                     "Makeisovolume error, more than %d vertices.\n",
                     CSIZE);
                  gmvexit();
                 }
             }
 
            /*  If this cell has intersections,  */
            /*  group them into triangles.       */
            if (np > 2)
              {
               for (i = 0; i < np-2; i++)
                 {
                  for (j = i+1; j < np-1; j++)
                    {
                     for (k = j+1; k < np; k++)
                       {
                     
                        /*  See if more memory needed.  */
                        if (nvpolys+1 > vpolysallocated)
                          {
                           vpolysallocated += MSIZE;
                           if ((conlist = (struct isocont *)realloc(conlist,
                                vpolysallocated*sizeof(struct isocont)))
                                == NULL)
                             {
                              fprintf(stderr,"Makeisovolume conlist2 error, can't "
                                 "allocate any more memory.\n");
                              gmvexit();
                             }
                          }
 
                        /*  Fill contour structure.  */
                        conlist[nvpolys].vx[0] = xv[i];
                        conlist[nvpolys].vy[0] = yv[i];
                        conlist[nvpolys].vz[0] = zv[i];
                        conlist[nvpolys].vx[1] = xv[j];
                        conlist[nvpolys].vy[1] = yv[j];
                        conlist[nvpolys].vz[1] = zv[j];
                        conlist[nvpolys].vx[2] = xv[k];
                        conlist[nvpolys].vy[2] = yv[k];
                        conlist[nvpolys].vz[2] = zv[k];

                        /*  Calculate normal.  */
                        x1 = xv[i];  y1 = yv[i];  z1 = zv[i];
                        x2 = xv[j];  y2 = yv[j];  z2 = zv[j];
                        x3 = xv[k];  y3 = yv[k];  z3 = zv[k];
                        xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
                        yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
                        zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
                        denom = sqrt(xno*xno + yno*yno + zno*zno);
                        conlist[nvpolys].nx = xno / denom;
                        conlist[nvpolys].ny = yno / denom;
                        conlist[nvpolys].nz = zno / denom;

                        /*  Fill node point data to color surface.  */
                        conlist[nvpolys].fact[0] = vfact[i];
                        conlist[nvpolys].node1[0] = vpt1[i];
                        conlist[nvpolys].node2[0] = vpt2[i];
                        conlist[nvpolys].fact[1] = vfact[j];
                        conlist[nvpolys].node1[1] = vpt1[j];
                        conlist[nvpolys].node2[1] = vpt2[j];
                        conlist[nvpolys].fact[2] = vfact[k];
                        conlist[nvpolys].node1[2] = vpt1[k];
                        conlist[nvpolys].node2[2] = vpt2[k];

                        nvpolys++;
                       }  /*  End of k loop.  */
                    }  /*  End of j loop.  */
                 }  /*  End of i loop.  */
              }  /*  End of if np > 2.  */
           }  /*  End of iv loop.  */
        }  /* End of if gocell. */
     }  /* End of icell loop. */
 
   *nvpolysout = nvpolys;
   free(vfacelist);
}
 
 
void chkvolfaces(float volmin, float volmax, char *vfacelist, 
                 double *nodedata, int *nvolfaces)
 
 /*  This routine finds the faces that creat the outer  */
 /*  shell for the isovolume.                           */
 /*  volval1 - first isovolume value                    */
 /*  volval2 - second isovolume value                   */
 /*  vfacelist - list of selected faces                 */
 /*  nodedata - pointer to the data to interpolate      */
 /*  nvolfaces - no. of selected faces returned.        */

{
  int i, j, k, icell, jcell, iface, jface, ifound, nfacesel, nv;
  int gocell;
  float val, vmin, vmax;

   /*  Loop through the cells.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  If ivcliponcellsel is turned on,    */ 
      /*  see if the cell has been selected.  */
      gocell = 1;
      if (ivcliponcellsel == 1) gocell = (int)cellselectflag[icell];

      /*  Eliminate cells with material < 1.  */
      if (cellmats[icell] < 1) gocell = 0;
 
      /*  If ivcliponbox is turned on, see     */
      /*  if the cell lies within the subset.  */
      if (ivcliponbox == 1 && (int)cellsubsetflag[icell] == 0)
         gocell = 0;

      /*  Process cell if selected.  */
      if (gocell)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Loop over cell faces and count the times that     */
         /*  the face values lie between the isovolume range.  */
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];
            jface = iface;
            if (structuredflag == 1) jface = i;

            /*  Loop over vertices in the face.  If any vertex  */
            /*  value lies between the isovolume range, or      */
            /*  the face min and max spans the isovolume range, */ 
            /*  add to the face count.                          */
            /*  Skip faces with only 2 vertices.                */
            vmin = MAXVAL;  vmax = -MAXVAL;
            ifound = 0;
            nv = cellfaces[jface].nverts;
            if (nv > 2)
              {
               for (j = 0; j < nv; j++)
                 {
                  k = cellfaces[jface].fverts[j];
                  val = nodedata[k];
                  if (volmin <= val && val <= volmax) ifound++;
                  vmin = (val < vmin) ? val : vmin;
                  vmax = (val > vmax) ? val : vmax;
                 }
              }
            if (vmin < volmin && volmax < vmax) ifound++;
            if (ifound > 0) vfacelist[iface]++;
           }
        }
     }

   /*  Loop over all faces, count no. faces selected.  */
   nfacesel = 0;
   for (i = 0; i < ncellfaces; i++)
     {
      if ((int)vfacelist[i] == 1) nfacesel++;
     }
   *nvolfaces = nfacesel;
}


void facevol(int jface, float volmin, float volmax, double *nodedata)

 /*  This routine gets the face nodes and adds to conlist.  */
 /*  iface - face no.                                       */
 /*  volmin - first isovolume value                         */
 /*  volmax - second isovolume value                        */
 /*  nodedata - pointer to the data to interpolate          */

{
  int i, j, k, np, nv, goedge;
  double v1, v2, fact, vmax, vmin;
  double xno, yno, zno, denom;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double xj, yj, zj, xk, yk, zk;

   /*  Interpolate face if necessary.  */
   np = 0;
   nv = cellfaces[jface].nverts;
   for (i = 0; i < nv; i++)
     {
      j = cellfaces[jface].fverts[i];
      if (i < (nv-1)) 
        k = cellfaces[jface].fverts[i+1];
      else k =cellfaces[jface].fverts[0];
      v1 = nodedata[j];
      v2 = nodedata[k];
      vmin = (v1 < v2) ? v1 : v2;
      vmax = (v1 > v2) ? v1 : v2;
      xj = nodes[j].x;  yj = nodes[j].y;  zj = nodes[j].z;
      xk = nodes[k].x;  yk = nodes[k].y;  zk = nodes[k].z;

      /*  Eliminate face edges whose values  */
      /*  lie outside the isovolume values.  */
      goedge = 1;
      if (vmax < volmin || vmin > volmax) goedge = 0;
      if (goedge)
        {
         if (volmin <= v1 && v1 <= volmax)
           {
            xv[np] = xj;
            yv[np] = yj;
            zv[np] = zj;
            vfact[np] = 0.;
            vpt1[np] = j;
            vpt2[np] = k;
            np++;
           }
         if (v1 < volmin || v1 > volmax)
           {
            denom = v1 - v2;
            if (v1 < volmin)
               fact = (v1 - volmin) / denom;
            else
               fact = (v1 - volmax) / denom;
            if (fact > 0. && fact < 1.)
              {
               xv[np] = xj + fact * (xk - xj);
               yv[np] = yj + fact * (yk - yj);
               zv[np] = zj + fact * (zk - zj);
               vfact[np] = fact;
               vpt1[np] = j;
               vpt2[np] = k;
               np++;
              }
           }
         if (v2 < volmin || v2 > volmax)
           {
            denom = v1 - v2;
            if (v2 < volmin)
               fact = (v1 - volmin) / denom;
            else
               fact = (v1 - volmax) / denom;
            if (fact > 0. && fact < 1.)
              {
               xv[np] = xj + fact * (xk - xj);
               yv[np] = yj + fact * (yk - yj);
               zv[np] = zj + fact * (zk - zj);
               vfact[np] = fact;
               vpt1[np] = j;
               vpt2[np] = k;
               np++;
              }
           }
        }
     }

   /*  Make triangles of the nodes and fill conlist.  */
   if (np > 2)
     {
      for (i = 0; i < np-2; i++)
        {
                     
         /*  See if more memory needed.  */
         if (nvpolys+1 > vpolysallocated)
           {
            vpolysallocated += MSIZE;
            if ((conlist = (struct isocont *)realloc(conlist,
                           vpolysallocated*sizeof(struct isocont)))
                 == NULL)
              {
               fprintf(stderr,"Makeisovolume conlist3 error, can't "
                  "allocate any more memory.\n");
               gmvexit();
              }
           }
 
         /*  Fill contour structure.  */
         j = i + 1;
         k = i + 2;
         conlist[nvpolys].vx[0] = xv[0];
         conlist[nvpolys].vy[0] = yv[0];
         conlist[nvpolys].vz[0] = zv[0];
         conlist[nvpolys].vx[1] = xv[j];
         conlist[nvpolys].vy[1] = yv[j];
         conlist[nvpolys].vz[1] = zv[j];
         conlist[nvpolys].vx[2] = xv[k];
         conlist[nvpolys].vy[2] = yv[k];
         conlist[nvpolys].vz[2] = zv[k];

         /*  Calculate normal.  */
         x1 = xv[0];  y1 = yv[0];  z1 = zv[0];
         x2 = xv[j];  y2 = yv[j];  z2 = zv[j];
         x3 = xv[k];  y3 = yv[k];  z3 = zv[k];
         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         denom = sqrt(xno*xno + yno*yno + zno*zno);
         conlist[nvpolys].nx = xno / denom;
         conlist[nvpolys].ny = yno / denom;
         conlist[nvpolys].nz = zno / denom;

         /*  Fill node point data to color surface.  */
         conlist[nvpolys].fact[0] = vfact[0];
         conlist[nvpolys].node1[0] = vpt1[0];
         conlist[nvpolys].node2[0] = vpt2[0];
         conlist[nvpolys].fact[1] = vfact[j];
         conlist[nvpolys].node1[1] = vpt1[j];
         conlist[nvpolys].node2[1] = vpt2[j];
         conlist[nvpolys].fact[2] = vfact[k];
         conlist[nvpolys].node1[2] = vpt1[k];
         conlist[nvpolys].node2[2] = vpt2[k];

         nvpolys++;
        }
     }

}
