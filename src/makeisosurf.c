#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "contour.h"
 
#define MSIZE 10000
#define CSIZE 10000
#define MAXVAL 9.99999e32

static int polysallocated, npolys;
static float xv[CSIZE], yv[CSIZE], zv[CSIZE], vfact[CSIZE];
static int vpt1[CSIZE], vpt2[CSIZE];
static char *facelist;
 
 
void makeisosurf(cvalue,ctype,nodedata,npolysout)
 float cvalue;
 double *nodedata;
 int ctype, *npolysout;
 
 /*  This routine interpolates cell edges to create an  */
 /*  isosurface for the requested contour value.        */
 /*  cvalue - contour value                             */
 /*  ctype - contour type: (0=field, 1=material)        */
 /*  nodedata - pointer to the data to interpolate      */
 /*  npolys - no. of polygons generated.                */
 /*                                                     */
 /*  NOTE - Be sure to free conlist in                  */
 /*         calling routine.                            */
 
{
  int icell, gocell;
  int i, j, k, jj, np, ic, jcell, jface;
  int nodeid[CSIZE], ncn, iedge, ncontfaces;
  double v1, v2, fact, vmax, vmin;
  double xno, yno, zno, denom, denomck, diff;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3;
  double xj, yj, zj, xk, yk, zk;
  void chkfaces(float cvalue, int ctype, char *facelist, 
                double *nodedata, int *ncontfaces);
  void facecont(int jface, int ctype, float cvalue, 
                double *nodedata);
 
   npolys = 0;
 
   if ((conlist = (struct isocont *)malloc(MSIZE*
                       sizeof(struct isocont))) == NULL)
     {
      fprintf(stderr,
           "Makecontour error, can't allocate any more memory.\n");
      gmvexit();
     }
   polysallocated = MSIZE;

   /*  Allocate and initialize facelist to 0.  */
   if ((facelist = (char *)malloc(ncellfaces*sizeof(char))) == NULL)
     {
      fprintf(stderr,
           "Makecontour error, can't allocate any more memory.\n");
      gmvexit();
     }
   for (i = 0; i < ncellfaces; i++) facelist[i] = 0;

   /*  See if any faces create an outer shell.  */
   chkfaces(cvalue,ctype,facelist,nodedata,&ncontfaces);

   /*  Loop over cells.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  If cliponcellsel is turned on, see  */ 
      /*  if the cell has been selected.      */
      gocell = 1;
      if (cliponcellsel == 1) gocell = (int)cellselectflag[icell];

      /*  Eliminate cells with material < 1.  */
      if (cellmats[icell] < 1) gocell = 0;
 
      /*  If cliponbox is turned on, see if  */
      /*  the cell lies within the subset.   */
      if (cliponbox == 1 && (int)cellsubsetflag[icell] == 0)
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
         if (ncontfaces > 0)
           {
            for (j = 0; j < cells[jcell].nfaces; j++)
              {
               k = cells[jcell].facenos[j];
               jface = k;
               if (structuredflag == 1) jface = j;

               /*  If face is selected, add to the isosurf list.  */
               if ((int)facelist[k] == 1)
                  facecont(jface,ctype,cvalue,nodedata);
              }
           }

         /*  See if cell min.-max. value  */
         /*  spans the contour value.     */
         vmin = MAXVAL;  vmax = -MAXVAL;
         ncn = (int)cells[jcell].ncnodes;
         for (i = 0; i < ncn; i++)
           {
            j = cells[jcell].cnodes[i];
            v1 = nodedata[j];
            vmin = (v1 < vmin) ? v1 : vmin;
            vmax = (v1 > vmax) ? v1 : vmax;
           }
         if (cvalue < vmin || cvalue > vmax) gocell = 0;

         /*  Delete this cell if all     */
         /*  nodes have the same value.  */
         if (vmin == vmax) gocell = 0;
        }
 
      /*  Loop over cell edges, interpolate and get  */
      /*   x,y,z values for interpolated points.     */
      if (gocell == 1)
        {
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
            denom = v1 - v2;
            denomck = (float)(fabs((double)denom));
            fact = 1.1;
            if (denomck > 1.0e-20)
               fact = (v1 - cvalue) / denom;

            /*  Check values at vertices, and add  */
            /*  to list if not used previously.    */
            diff = (float)(fabs((double)(v1 - cvalue)));
            if (diff < myzero)
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
            diff = (float)(fabs((double)(v2 - cvalue)));
            if (diff < myzero)
              {
               ic = -1;
               for (jj = 0; jj < np; jj++)
                  if (nodeid[jj] == k) ic = jj;
               if (ic == -1)
                 {
                  xv[np] = xk;
                  yv[np] = yk;
                  zv[np] = zk;
                  nodeid[np] = k;
                  vfact[np] = 1.0;
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
                  "Makecontour error, more than %d vertices.\n",
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
                     if (npolys+1 > polysallocated)
                       {
                        polysallocated += MSIZE;
                        if ((conlist = (struct isocont *)realloc(conlist,
                                       polysallocated*sizeof(struct isocont)))
                             == NULL)
                          {
                           fprintf(stderr,"Makecontour error, can't "
                              "allocate any more memory.\n");
                           gmvexit();
                          }
                       }
 
                     /*  Fill contour structure.  */
                     conlist[npolys].vx[0] = xv[i];
                     conlist[npolys].vy[0] = yv[i];
                     conlist[npolys].vz[0] = zv[i];
                     conlist[npolys].vx[1] = xv[j];
                     conlist[npolys].vy[1] = yv[j];
                     conlist[npolys].vz[1] = zv[j];
                     conlist[npolys].vx[2] = xv[k];
                     conlist[npolys].vy[2] = yv[k];
                     conlist[npolys].vz[2] = zv[k];

                     /*  Calculate normal.  */
                     x1 = xv[i];  y1 = yv[i];  z1 = zv[i];
                     x2 = xv[j];  y2 = yv[j];  z2 = zv[j];
                     x3 = xv[k];  y3 = yv[k];  z3 = zv[k];
                     xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
                     yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
                     zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
                     denom = sqrt(xno*xno + yno*yno + zno*zno);
                     conlist[npolys].nx = xno / denom;
                     conlist[npolys].ny = yno / denom;
                     conlist[npolys].nz = zno / denom;

                     /*  Fill node point data to color surface.  */
                     conlist[npolys].fact[0] = vfact[i];
                     conlist[npolys].node1[0] = vpt1[i];
                     conlist[npolys].node2[0] = vpt2[i];
                     conlist[npolys].fact[1] = vfact[j];
                     conlist[npolys].node1[1] = vpt1[j];
                     conlist[npolys].node2[1] = vpt2[j];
                     conlist[npolys].fact[2] = vfact[k];
                     conlist[npolys].node1[2] = vpt1[k];
                     conlist[npolys].node2[2] = vpt2[k];

                     npolys++;
                    }  /*  End of k loop.  */
                 }  /*  End of j loop.  */
              }  /*  End of i loop.  */
           }  /*  End of if np > 2.  */
        }  /* End of if gocell. */
     }  /* End of icell loop. */
 
   *npolysout = npolys;
   free(facelist);
}
 
 
void chkfaces(float cvalue, int ctype, char *facelist, 
              double *nodedata, int *ncontfaces)
 
 /*  This routine finds the faces that creat the outer  */
 /*  shell for the isosurface.                          */
 /*  cvalue - contour value                             */
 /*  ctype - contour type: (0=field, 1=material)        */
 /*  facelist - list of selected faces                  */
 /*  nodedata - pointer to the data to interpolate      */
 /*  ncontfaces - no. of selected faces returned.       */

{
  int i, j, k, icell, jcell, iface, jface, ifound, nfacesel, nv;
  double chkval, val;

   /*  Set chkval to cvalue, for materials set chkval to 1.  */
   chkval = cvalue;
   if (ctype == 1) chkval = 1.;

   /*  Loop through the cells.  */
   for (icell = 0; icell < ncells; icell++)
     {

      /*  Fill cell structure if structured grid.  */
      jcell = icell;
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,1);
        }

      /*  Loop over cell faces and count the times  */
      /*  that the face values equal the isovalue.  */
      for (i = 0; i < cells[jcell].nfaces; i++)
        {
         iface = cells[jcell].facenos[i];
         jface = iface;
         if (structuredflag == 1) jface = i;

         /*  Loop over vertices in the face.     */
         /*  For fields, all face values must    */
         /*  equal the isovalue.  For materials  */ 
         /*  any face value must equal 1.        */
         /*  Skip faces with only 2 vertices.    */
         ifound = 0;
         nv = cellfaces[jface].nverts;
         if (nv > 2)
           {
            for (j = 0; j < nv; j++)
              {
               k = cellfaces[jface].fverts[j];
               val = nodedata[k];
               if (val == chkval) ifound++;
              }
           }
         if (ctype == 0 && ifound == nv)
            facelist[iface]++;
         if (ctype == 1 && ifound > 0) facelist[iface]++;
        }
     }

   /*  Loop over all faces, count no. faces selected.  */
   nfacesel = 0;
   for (i = 0; i < ncellfaces; i++)
     {
      if ((int)facelist[i] == 1) nfacesel++;
     }
   *ncontfaces = nfacesel;
}


void facecont(int jface, int ctype, float cvalue, double *nodedata)

 /*  This routine gets the face nodes and adds to conlist.  */
 /*  iface - face no.                                       */
 /*  ctype - contour type: (0=field, 1=material)            */
 /*  cvalue - contour value                                 */
 /*  nodedata - pointer to the data to interpolate          */

{
  int i, j, k, np, nv;
  double v1, v2, chkval, denom;
  double xno, yno, zno;
  double xj, yj, zj, xk, yk, zk;
  float x1, y1, z1, x2, y2, z2, x3, y3, z3;

   /*  If field isosurface, fill xv, yv, zv from nodes.  */
   if (ctype == 0)
     {
      np = cellfaces[jface].nverts;
      nv = cellfaces[jface].nverts;
      for (i = 0; i < np; i++)
        {
         j = cellfaces[jface].fverts[i];
         if (i < (nv-1)) 
           k = cellfaces[jface].fverts[i+1];
         else k =cellfaces[jface].fverts[0];
         xj = nodes[j].x;  yj = nodes[j].y;  zj = nodes[j].z;
         xk = nodes[j].x;  yk = nodes[k].y;  zk = nodes[k].z;
         xv[i] = xj;
         yv[i] = yj;
         zv[i] = zj;
         vfact[i] = 0.;
         vpt1[i] = j;
         vpt2[i] = k;
        }
     }

   /*  If material, interpolate if necessary.  */
   else
     {
      chkval = 1.;
      np = 0;
      nv = cellfaces[jface].nverts;
      for (i = 0; i < nv; i++)
        {
         j = cellfaces[jface].fverts[i];
         if (i < (nv-1)) 
           k = cellfaces[jface].fverts[i+1];
         else k =cellfaces[jface].fverts[0];
         xj = nodes[j].x;  yj = nodes[j].y;  zj = nodes[j].z;
         xk = nodes[j].x;  yk = nodes[k].y;  zk = nodes[k].z;
         v1 = nodedata[j];
         v2 = nodedata[k];
         if (v1 == chkval)
           {
            xv[np] = xj;
            yv[np] = yj;
            zv[np] = zj;
            vfact[np] = 0.;
            vpt1[np] = j;
            vpt2[np] = k;
            np++;
           }
         if (v1 != v2)
           {
            xv[np] = xj + cvalue * (xk - xj);
            yv[np] = yj + cvalue * (yk - yj);
            zv[np] = zj + cvalue * (zk - zj);
            vfact[np] = cvalue;
            vpt1[np] = j;
            vpt2[np] = k;
            np++;
           }
        }
     }

   /*  Make triangles of the nodes and fill conlist.  */
   if (np > 2)
     {
      for (i = 0; i < np-2; i++)
        {
                     
         /*  See if more memory needed.  */
         if (npolys+1 > polysallocated)
           {
            polysallocated += MSIZE;
            if ((conlist = (struct isocont *)realloc(conlist,
                           polysallocated*sizeof(struct isocont)))
                 == NULL)
              {
               fprintf(stderr,"Makecontour error, can't "
                  "allocate any more memory.\n");
               gmvexit();
              }
           }
 
         /*  Fill contour structure.  */
         j = i + 1;
         k = i + 2;
         conlist[npolys].vx[0] = xv[0];
         conlist[npolys].vy[0] = yv[0];
         conlist[npolys].vz[0] = zv[0];
         conlist[npolys].vx[1] = xv[j];
         conlist[npolys].vy[1] = yv[j];
         conlist[npolys].vz[1] = zv[j];
         conlist[npolys].vx[2] = xv[k];
         conlist[npolys].vy[2] = yv[k];
         conlist[npolys].vz[2] = zv[k];

         /*  Calculate normal.  */
         x1 = xv[0];  y1 = yv[0];  z1 = zv[0];
         x2 = xv[j];  y2 = yv[j];  z2 = zv[j];
         x3 = xv[k];  y3 = yv[k];  z3 = zv[k];
         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         denom = sqrt(xno*xno + yno*yno + zno*zno);
         conlist[npolys].nx = xno / denom;
         conlist[npolys].ny = yno / denom;
         conlist[npolys].nz = zno / denom;

         /*  Fill node point data to color surface.  */
         conlist[npolys].fact[0] = vfact[0];
         conlist[npolys].node1[0] = vpt1[0];
         conlist[npolys].node2[0] = vpt2[0];
         conlist[npolys].fact[1] = vfact[j];
         conlist[npolys].node1[1] = vpt1[j];
         conlist[npolys].node2[1] = vpt2[j];
         conlist[npolys].fact[2] = vfact[k];
         conlist[npolys].node1[2] = vpt1[k];
         conlist[npolys].node2[2] = vpt2[k];

         npolys++;
        }
     }

}
