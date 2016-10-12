#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <malloc.h>

#include "gmvdata.h"
#include "lights.h"

static int nfv, fieldactive, icell, jcell, itype, itype2;
static short matnum, medianedgeflag, medianflag, voronoiflag, 
       voronoiedgeflag, fldflag;
static char logflag;
static char *selectflag; 
static double tempf, *field, expx, expy, expz, fld_min, fld_max;
void cesetfldcolr(double fldval, double fldmin, double fldmax, double tempval, 
                 char logflag, int *icl1);

 
void drawcelledges(int drawtype) 
{
 
  /*  This routine plots only cell edges for:  */
  /*  regular cells (drawtype = 0)             */
  /*  grid analysis cells (drawtype = 1).      */
  /*  unselected cells (drawtype = 2).         */
 
  int i, j, k, l, gocell, ic1, iex, allvars, iedge, cactive, ic2, 
      id, id2, faceselflag, jface;
  double v1[3];
  double xpt, ypt, zpt;
  int incr;
  short faces_on, numflag, edgeflag, explodeflag, *edgemat;
  short drawedge, nnumflag, fnumflag, *matcolr;
  void drawaltedges(int meshtype);

   faceselflag = 0;

   /*  Set data for regular cells.  */
   if (drawtype == 0)
     {
      edgeflag = celledgesflag;
      fieldactive = cellfieldactive;
      selectflag = cellselectflag;
      field = cellfield;
      faces_on = cellsflag;
      numflag = cellnumflag;
      nnumflag = cellnodenumflag;
      fnumflag = cellfacenumflag;
      medianflag = 0;
      medianedgeflag = cellmedianedgeflag;
      voronoiflag = 0;
      voronoiedgeflag = 0;
     }

   /*  Set data for grid analysis cells.  */
   if (drawtype == 1)
     {
      edgeflag = gridedgesflag;
      fieldactive = gridfieldactive;
      selectflag = gridselectflag;
      field = gridfield;
      faces_on = gridsflag;
      numflag = gacellnumflag;
      nnumflag = ganodenumflag;
      fnumflag = gafacenumflag;
      medianflag = gridmedianflag;
      medianedgeflag = gridmedianedgeflag;
      voronoiflag = gridvoronoiflag;
      voronoiedgeflag = gridvoronoiedgeflag;
     }

   /*  Set data for unselected cells.  */
   if (drawtype == 2)
     {
      edgeflag = 1;
      fieldactive = -1;
      selectflag = cellselectflag;

      /*  If 3D mesh and coloring by cell group  */ 
      /*  reset selectflag to cellfacedraw.      */
      if (mesh2dflag == 0 && (cellfieldactive == -2 || cellfieldactive == -3))
         selectflag = cellfacedraw;

      /*  If 3D mesh and coloring by face group  */
      /*  or selected by face group, set flag.   */
      if (mesh2dflag == 0 && (cellfieldactive == -3 || cellselfacegrp_on))
         faceselflag = 1;
      field = 0;
      faces_on = 0;
      numflag = 0;
      nnumflag = 0;
      fnumflag = 0;
      medianflag = 0;
      medianedgeflag = 0;
      voronoiflag = 0;
      voronoiedgeflag = 0;
     }

   allvars = numvars + cnumvars + fnumvars;
   cactive = fieldactive - numvars;
   fldflag = 0;

   /*  Determine min and max scale values for field data (if any). */
   if (fieldactive > -1 && fieldactive < numvars)
     {
      fldflag = 1;
      logflag = nfld_log[fieldactive];
      fld_min = nfld_nice_min[fieldactive];
      fld_max = nfld_nice_max[fieldactive];
     }

   if (cactive > -1 && cactive < cnumvars)
     {
      fldflag = 2;
      logflag = cfld_log[cactive];
      fld_min = cfld_nice_min[cactive];
      fld_max = cfld_nice_max[cactive];
     }

   if (fldflag > 0)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (fieldactive == -2) matcolr = cellgrpcolr;
   else matcolr = cellmats;

   incr = 1;
   /* if (fastdraw == 1 && drawtype == 0) incr = drawincr; */

   /*  Set grey color.  */
   ic1 = -3;
   ic2 = -3;

   /*  Set black color for unselected cells.  */
   ic1 = -1;
   ic2 = -1;

   /*  Check for explode for these cells.  */
   explodeflag = 0;
   if (cexplodefact > 0.) explodeflag = 1;

   /*  Turn off edges if faces are drawn.  Face  */
   /*  edges will be drawn in drawcellfaces.     */
   if (faces_on == 1)
     {
      edgeflag = 0;
      if (medianflag == 0)  medianedgeflag = 0;
      if (voronoiflag == 0) voronoiedgeflag = 0;
     }

   /*  If edges are drawn, allocate     */
   /*  and initialize edge test array.  */
   if (edgeflag == 1)
     {
      edgemat = (short *)malloc(ncelledges*sizeof(short));
      for (i = 0; i < ncelledges; i++)
         edgemat[i] = -1;
     }

   /*  Set up Gouraud shading for field data.  */
   itype = LSOLID;
   itype2 = LSOLID;
   if (fldflag == 1)
     {
      itype = LSMOOTH;
      itype2 = LSMOOTH;
     }

   /*  Loop through selected cells.  */
   for (icell = 0; icell < ncells; icell += incr)
     {

      /*  See if the cell has been selected for display.  */
      gocell = (int)selectflag[icell];

      /*  Eliminate cells with material   */ 
      /*  or group < 1 unless requested.  */
      if (drawtype == 0 && cell0matflag == 0 && matcolr[icell] < 1) 
         gocell = 0;

      /*  Reset gocell for unselected cells.  */
      if (drawtype == 2)
        {
         gocell--;
         if (gocell < 0) gocell = 1;
        }

      /*  If only faces drawn, select all cells.  */
      if (faceselflag) gocell = 1;

      /*  See if the cell lies within the subset.  */
      if (drawtype != 1 && (int)cellsubsetflag[icell] == 0)
         gocell = 0;

      /*  Process selected cell.  */
      if (gocell == 1)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,3);
           }

         /*  Set material/flag color for this cell.  */
         matnum = matcolr[icell] - 1;
         if (fieldactive >= allvars)
           matnum = cellflags[fieldactive-allvars][icell] - 1;
         ic1 = matnum;
         ic2 = matnum;
         if (drawtype == 2)
           {
            if (txtcolor < 256) ic1 = -1;
            else ic1 = -2;
           }

         /*  Determine and set explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0;
         iex = matnum;
         if (explodeflag)
           {
            if (cellexpflag == -1) 
               iex = (int)cellmats[icell] - 1;
            else iex = (int)cellflags[cellexpflag][icell] - 1;
            expx = cexplodefact*cellexpx[iex];
            expy = cexplodefact*cellexpy[iex];
            expz = cexplodefact*cellexpz[iex];
           }

         if (faces_on == 0)
           {
            ic1 = matnum;
            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            if (drawtype == 2)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            if (medianedgeflag == 1 || voronoiedgeflag == 1)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
               itype = LSOLID;
              }
            ic2 = ic1;
           }

         /*  If a cell field is to be drawn, reset the color. */
         if (fldflag == 2 && medianedgeflag == 0 && voronoiedgeflag == 0)
           {
            cesetfldcolr(field[icell],fld_min,fld_max,tempf,logflag,&ic1);
            ic2 = ic1;
           }

         /*  Loop over the cell edges, if requested.  */
         if (edgeflag == 1)
           {
            for (i = 0; i < cells[jcell].nedges; i++)
              {

               /*  Draw edges once, unless explode is  */
               /*  on and iex differs from the last.  */
               iedge = cells[jcell].edgenos[i];
               drawedge = 0;
               if (edgemat[iedge] == -1)
                 {
                  drawedge = 1;
                  edgemat[iedge] = iex;
                 }
               if (explodeflag && iex != edgemat[iedge])
                 {
                  drawedge = 1;
                  edgemat[iedge] = iex;
                 }

               /*  Draw the selected edge.  */
               if (drawedge)
                 {
                  if (structuredflag == 1) iedge = i;
                  k = celledges[iedge].edgept1;
                  l = celledges[iedge].edgept2;

                  /*  If a node field is to be drawn,  */ 
                  /*  reset the color for each node.   */
                  ic2 = ic1;
                  if (fldflag == 1 && medianedgeflag == 0 && 
                      voronoiedgeflag == 0)
                    {
                     cesetfldcolr(field[k],fld_min,fld_max,tempf,
                                  logflag,&ic1);
                     cesetfldcolr(field[l],fld_min,fld_max,tempf,
                                  logflag,&ic2);
                    }

                  if (numplv+3 > plvarray_alloc)
                    {
                     plvarray_alloc += 10000;
                     plvarray = (struct plvelem *)realloc(plvarray,
                                 plvarray_alloc*sizeof(struct plvelem));
                    }
                  plvarray[numplv].itype = itype;
                  plvarray[numplv].x1 = nodes[k].x + expx;
                  plvarray[numplv].y1 = nodes[k].y + expy;
                  plvarray[numplv].z1 = nodes[k].z + expz;
                  plvarray[numplv].x2 = nodes[l].x + expx;
                  plvarray[numplv].y2 = nodes[l].y + expy;
                  plvarray[numplv].z2 = nodes[l].z + expz;
                  plvarray[numplv].iclr1 = ic1;
                  plvarray[numplv].iclr2 = ic2;
                  numplv++;
                 }
              } 
           }   /*  End of cell edges.  */

         /*  Draw median or voronoi mesh edges, if requested.  */
         if (medianedgeflag == 1) drawaltedges(0);
         if (voronoiedgeflag == 1) drawaltedges(1);

         /*  Draw cell, node and face numbers if requested.  */
         if (numflag == 1 || nnumflag == 1 || fnumflag == 1)
           {

            /*  Draw cell no. in material/flag color.  */
            i = cells[jcell].ncnodes + cells[jcell].nfaces + 1;
            if (numplv+i > plvarray_alloc)
              {
               plvarray_alloc += 10000;
               plvarray = (struct plvelem *)realloc(plvarray,
                           plvarray_alloc*sizeof(struct plvelem));
              }
            if (numflag == 1)
              {
               xpt = cells[jcell].xavg;
               ypt = cells[jcell].yavg; 
               zpt = cells[jcell].zavg;
               xpt = xpt + expx;
               ypt = ypt + expy;
               zpt = zpt + expz;
               id = icell + 1;
               if (cellidflag) id = cellids[icell];
               id2 = 0;

#ifdef MPI
               if (local_idflag)
                 {
                  id = cellpe[icell];
                  id2 = cellnuminpe[icell];
                 }
#endif

               /*  If Grid Analysis, draw the cell number in grey.  */
               if (drawtype == 1) ic1 = -3;

               if (numplv+3 > plvarray_alloc)
                 {
                  plvarray_alloc += 10000;
                  plvarray = (struct plvelem *)realloc(plvarray,
                              plvarray_alloc*sizeof(struct plvelem));
                 }
               plvarray[numplv].itype = NUMONLY;
               plvarray[numplv].x1 = xpt;
               plvarray[numplv].y1 = ypt;
               plvarray[numplv].z1 = zpt;
               plvarray[numplv].x2 = id;
               plvarray[numplv].y2 = id2;
               plvarray[numplv].z2 = 0.;
               plvarray[numplv].iclr1 = ic1;
               plvarray[numplv].iclr2 = ic1;
               numplv++;
              }

            /*  Draw node nos. in text color.  */
            if (nnumflag == 1)
              {

               /*  Loop over the nodes for the cell.  */
               i = cells[jcell].ncnodes;
               if (numplv+i+1 > plvarray_alloc)
                 {
                  plvarray_alloc += 10000;
                  plvarray = (struct plvelem *)realloc(plvarray,
                              plvarray_alloc*sizeof(struct plvelem));
                 }
               for (i = 0; i < cells[jcell].ncnodes; i++)
                 {
                  k = cells[jcell].cnodes[i];
                  v1[0] = nodes[k].x + expx;
                  v1[1] = nodes[k].y + expy;
                  v1[2] = nodes[k].z + expz;
                  id = k;
                  if (nodeidflag) id = nodeids[k];
                  id2 = 0;

#ifdef MPI
                  if (local_idflag)
                    {
                     id = nodepe[k];
                     id2 = nodenuminpe[k];
                    }
#endif

                  /*  Draw the node no. in text color.  */
                  if (txtcolor < 256) ic1 = -1;
                  else ic1 = -2;

                  plvarray[numplv].itype = NUMONLY;
                  plvarray[numplv].x1 = v1[0];
                  plvarray[numplv].y1 = v1[1];
                  plvarray[numplv].z1 = v1[2];
                  plvarray[numplv].x2 = id;
                  plvarray[numplv].y2 = id2;
                  plvarray[numplv].z2 = 0.;
                  plvarray[numplv].iclr1 = ic1;
                  plvarray[numplv].iclr2 = ic1;
                  numplv++;
                 }
              }


            /*  Draw face nos. in text color.  */
            if (fnumflag == 1)
              {

               /*  Loop over the faces for the cell.  */
               i = cells[jcell].nfaces;
               if (numplv+i+1 > plvarray_alloc)
                 {
                  plvarray_alloc += 10000;
                  plvarray = (struct plvelem *)realloc(plvarray,
                              plvarray_alloc*sizeof(struct plvelem));
                 }
               for (i = 0; i < cells[jcell].nfaces; i++)
                 {
                  k = cells[jcell].facenos[i];
		  jface = k;
		  if (structuredflag == 1) jface = i;

                  /*  Calculate face center.  */
                  xpt = 0.;  ypt = 0.;  zpt = 0.;
                  for (j = 0; j < cellfaces[jface].nverts; j++)
                    {
                     l = cellfaces[jface].fverts[j];
                     xpt += nodes[l].x;
                     ypt += nodes[l].y;
                     zpt += nodes[l].z;
                    }
                  xpt /= cellfaces[jface].nverts;
                  ypt /= cellfaces[jface].nverts;
                  zpt /= cellfaces[jface].nverts;
                  v1[0] = xpt + expx;
                  v1[1] = ypt + expy;
                  v1[2] = zpt + expz;
                  id = k + 1;
                  if (faceidflag) id = faceids[k];

                  /*  Draw the face no. in grey.  */
                  ic1 = -3;

                  plvarray[numplv].itype = NUMONLY;
                  plvarray[numplv].x1 = v1[0];
                  plvarray[numplv].y1 = v1[1];
                  plvarray[numplv].z1 = v1[2];
                  plvarray[numplv].x2 = id;
                  plvarray[numplv].y2 = 0.;
                  plvarray[numplv].z2 = 0.;
                  plvarray[numplv].iclr1 = ic1;
                  plvarray[numplv].iclr2 = ic1;
                  numplv++;
                 }
              }
           }   /*  End of numbers.  */
        }
     }

   /*  If edges are drawn, free edge test array.  */
   if (edgeflag == 1)
     {
      FREE(edgemat);
     }
}


void drawaltedges(int meshtype)
{
  int i, j, k, l, ic1, ic2, iface, jface;
  short meshflag;
  double centval, xc, yc, zc, xf, yf, zf;
  double v1[3], v2[3];
  void voronoipt(int jcell, double vpt[3]), 
       voronoifacept(int jface, double vpt[3]);

   /*  Draw median or voronoi mesh edges.  */
   /*  meshtype = 0 for median mesh,       */
   /*  meshtype = 1 for voronoi mesh.      */
   if (meshtype == 0) meshflag = medianflag;
   else meshflag = voronoiflag;

   /*  Reset material/flag color.  */
   ic1 = matnum;
   if (meshflag == 1) ic1 = -2;

   /*  If field is to be drawn, set the cell center color.  */
   if (fldflag > 0 && meshflag == 0)
     {

      /*  If node field, average nodes.  */
      if (fldflag == 1)
        {
         centval = 0;
         for (i = 0; i < cells[jcell].ncnodes; i++)
           {
            k = cells[jcell].cnodes[i];
            centval += field[k];
           }
         centval /= (double)cells[jcell].ncnodes;
         cesetfldcolr(centval,fld_min,fld_max,tempf,logflag,&ic1);
        }
      else
        {
         cesetfldcolr(field[icell],fld_min,fld_max,tempf,logflag,&ic1);
        }
     }
   ic2 = ic1;

   /*  Set median or voronoi mesh point.  */
   if (meshtype == 0)
     {
      xc = cells[jcell].xavg;
      yc = cells[jcell].yavg;
      zc = cells[jcell].zavg;
     }
   else
     {
      voronoipt(jcell, v1);
      xc = v1[0];
      yc = v1[1];
      zc = v1[2];
     }

   /*  Loop through cell faces to generate median or voronoi edges.  */
   for (i = 0; i < cells[jcell].nfaces; i++)
     {
      iface = cells[jcell].facenos[i];
      jface = iface;
      if (structuredflag == 1) jface = i;

      /*  Average face node values for node field data.  */
      /*  Get average face point, and set color.         */
      nfv = cellfaces[jface].nverts;
      xf = 0;  yf = 0;  zf = 0;  centval = 0; 
      if (meshtype == 0)
        {
         for (j = 0; j < nfv; j++)
           {
            k = cellfaces[jface].fverts[j];
            xf += nodes[k].x;
            yf += nodes[k].y;
            zf += nodes[k].z;
            if (fldflag == 1)
               centval += field[k];
           }
         xf /= (float)nfv;
         yf /= (float)nfv;
         zf /= (float)nfv;
        }
      else 
        {
         voronoifacept(jface, v1);
         xf = v1[0];
         yf = v1[1];
         zf = v1[2];
         if (fldflag == 1)
           {
            for (j = 0; j < nfv; j++)
              {
               k = cellfaces[jface].fverts[j];
               centval += field[k];
              }
           }
        }

      if (fldflag == 1 && meshflag == 0)
        {
         centval /= (float)nfv;
         cesetfldcolr(centval,fld_min,fld_max,tempf,logflag,&ic2);
        }

      /*  Draw line from cell center to face center.  */
      /*  Reset colors for node field data.           */
      if (numplv+nfv+1 > plvarray_alloc)
        {
         plvarray_alloc += 10000;
         plvarray = (struct plvelem *)realloc(plvarray,
                     plvarray_alloc*sizeof(struct plvelem));
        }
      plvarray[numplv].itype = itype2;
      plvarray[numplv].x1 = xc + expx;
      plvarray[numplv].y1 = yc + expy;
      plvarray[numplv].z1 = zc + expz;
      plvarray[numplv].x2 = xf + expx;
      plvarray[numplv].y2 = yf + expy;
      plvarray[numplv].z2 = zf + expz;
      plvarray[numplv].iclr1 = ic1;
      plvarray[numplv].iclr2 = ic2;
      numplv++;

      /*  Loop through face edges and draw lines  */
      /*  from face center to edge centers.       */
      v1[0] = xf + expx;
      v1[1] = yf + expy;
      v1[2] = zf + expz;
      for (j = 0; j < nfv; j++)
        {
         k = cellfaces[jface].fverts[j];
         if (j < nfv - 1)
            l = cellfaces[jface].fverts[j+1];
         else
            l = cellfaces[jface].fverts[0];
         v2[0] = 0.5*(nodes[k].x + nodes[l].x) + expx;
         v2[1] = 0.5*(nodes[k].y + nodes[l].y) + expy;
         v2[2] = 0.5*(nodes[k].z + nodes[l].z) + expz;
                  
         if (fldflag == 1 && meshflag == 0)
           {
            centval = 0.5*(field[k] + field[l]); 
            cesetfldcolr(centval,fld_min,fld_max,tempf,logflag,&ic2);
           }

         plvarray[numplv].itype = itype2;
         plvarray[numplv].x1 = v1[0];
         plvarray[numplv].y1 = v1[1];
         plvarray[numplv].z1 = v1[2];
         plvarray[numplv].x2 = v2[0];
         plvarray[numplv].y2 = v2[1];
         plvarray[numplv].z2 = v2[2];
         plvarray[numplv].iclr1 = ic1;
         plvarray[numplv].iclr2 = ic2;
         numplv++;
        }
     }
}  

void cesetfldcolr
#include "setfldcolr.h"
