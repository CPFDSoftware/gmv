#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

 
void drawsurfedges() 
{
 
  /*  This routine plots surface edges.  */
 
  int i, k, l, isurf, gosurf, ic1, iex, allvars, sactive, id, id2,
      ic2, itype;
  double v1[3], tempf, fld_min, fld_max;
  double xpt, ypt, zpt, expx, expy, expz;
  char logflag;
  int incr;
  short matnum, explodeflag, fldflag, edgeflag, *matcolr;
  void sesetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval, char logflag, int *icl);
 
   /*  Turn off edges if surface faces are drawn.  */
   /*  Face edges will be drawn in drawsurfaces.   */
   edgeflag = surfedgesflag;
   if (surfflag == 1)
     {
      edgeflag = 0;

      /*  If not drawing anything else, return.  */
      if (surfnumflag == 0 && surfnodenumflag == 0)
         return;
     }

   allvars = numvars + snumvars;
   sactive = surffieldactive - numvars;

   /*  Determine min and max scale values for field data (if any). */
   fldflag = 0;
   if (surffieldactive > -1 && surffieldactive < numvars)
     {
      fldflag = 1;
      logflag = nfld_log[surffieldactive];
      fld_min = nfld_nice_min[surffieldactive];
      fld_max = nfld_nice_max[surffieldactive];
     }
   if (sactive > -1 && sactive < snumvars)
     {
      fldflag = 2;
      logflag = sfld_log[sactive];
      fld_min = sfld_nice_min[sactive];
      fld_max = sfld_nice_max[sactive];
     }

   if (fldflag > 0)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (surffieldactive == -2) matcolr = surfgrpcolr;
   else matcolr = surfmats;

   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */

   /*  Set grey color.  */
   ic1 = -3;
   ic2 = -3;

   /*  Check for explode for these surfaces.  */
   explodeflag = 0;
   if (sexplodefact > 0.) explodeflag = 1;

   /*  Set up Gouraud shading for field data.  */
   itype = LSOLID;
   if (fldflag == 1) itype = LSMOOTH;

   /*  Loop through selected surfaces.  */ 
   for (isurf = 0; isurf < nsurf; isurf += incr)
     {

      /*  See if the surface has been selected for display.  */
      gosurf = (int)surfselectflag[isurf];

      /*  Eliminate surfaces with material  */ 
      /*  or group < 1 unless requested.    */
      if (surf0matflag == 0 && matcolr[isurf] < 1) 
         gosurf = 0;
 
      /*  See if the surface lies within the subset.  */
      /*  if ((int)surfsubsetflag[isurf] == 0)  */
      /*     gosurf = 0;  */

      /*  Process selected surface.  */
      if (gosurf == 1)
        {

         /*  Set material/flag color for this surface.  */
         matnum = matcolr[isurf] - 1;
         if (surffieldactive >= allvars)
           matnum = surfflags[surffieldactive-allvars][isurf] - 1;
         ic1 = matnum;
         ic2 = matnum;

         /*  Determine and set explode factor type.  */
         expx = 0.;  expy = 0.;  expz = 0;
         iex = matnum;
         if (explodeflag)
           {
            if (surfexpflag == -1) 
               iex = (int)surfmats[isurf] - 1;
            else iex = (int)surfflags[surfexpflag][isurf] - 1;
            expx = sexplodefact*surfexpx[iex];
            expy = sexplodefact*surfexpy[iex];
            expz = sexplodefact*surfexpz[iex];
           }

         if (surfflag == 0)
           {
            ic1 = matnum;
            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            ic2 = ic1;
           }

         /* If a surface field is to be drawn, reset the color. */
         if (fldflag == 2)
           {
            sesetfldcolr(surffield[isurf],fld_min,fld_max,tempf,
                         logflag,&ic1);
            ic2 = ic1;
           }

         /*  Loop over the surface edges, if requested.  */
         if (edgeflag == 1)
           {
            for (i = 0; i < surfs[isurf].nverts; i++)
              {
               k = surfs[isurf].fverts[i];
               if (i < surfs[isurf].nverts-1)
                  l = surfs[isurf].fverts[i+1];
               else 
                  l = surfs[isurf].fverts[0];

               /*  If a node field is to be drawn,  */ 
               /*  reset the color for each node.   */
               if (fldflag == 1)
                 {
                  sesetfldcolr(surffield[k],fld_min,fld_max,tempf,
                                  logflag,&ic1);
                  sesetfldcolr(surffield[l],fld_min,fld_max,tempf,
                                  logflag,&ic2);
                 }

               v1[0] = nodes[k].x + expx;
               v1[1] = nodes[k].y + expy;
               v1[2] = nodes[k].z + expz;

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
           }   /*  End of surface edges.  */

         /*  Draw surface and node numbers if requested.  */
         if (surfnumflag == 1 || surfnodenumflag == 1)
           {
            i = surfs[isurf].nverts + 1;
            if (numplv+i > plvarray_alloc)
              {
               plvarray_alloc += 10000;
               plvarray = (struct plvelem *)realloc(plvarray,
                           plvarray_alloc*sizeof(struct plvelem));
              }

            /*  Draw surface no. in material/flag color.  */
            if (surfnumflag == 1)
              {
               xpt = surfs[isurf].xavg;
               ypt = surfs[isurf].yavg; 
               zpt = surfs[isurf].zavg;
               xpt = xpt + expx;
               ypt = ypt + expy;
               zpt = zpt + expz;
               id = isurf + 1;
               if (surfidflag) id = surfids[isurf];
               id2 = 0;

#ifdef MPI
               if (local_idflag)
                 {
                  id = surfpe[isurf];
                  id2 = surfnuminpe[isurf];
                 }
#endif

               plvarray[numplv].itype = NUMONLY;
               plvarray[numplv].x1 = xpt;
               plvarray[numplv].y1 = ypt;
               plvarray[numplv].z1 = zpt;
               plvarray[numplv].x2 = id;
               plvarray[numplv].y2 = id2;
               plvarray[numplv].z2 = 0.;
               plvarray[numplv].iclr1 = ic1;
               plvarray[numplv].iclr1 = ic1;
               numplv++;
              }

            /*  Draw nodes in material colors  */
            /*  and node nos. in text color.   */
            if (surfnodenumflag == 1)
              {

               /*  Loop over the nodes for the surface.  */
               for (i = 0; i < surfs[isurf].nverts; i++)
                 {
                  k = surfs[isurf].fverts[i];
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
                  plvarray[numplv].y2 = 0.;
                  plvarray[numplv].z2 = 0.;
                  plvarray[numplv].iclr1 = ic1;
                  plvarray[numplv].iclr1 = ic1;
                  numplv++;
                 }
              }
           }   /*  End of numbers.  */
        }
     }
}


void sesetfldcolr
#include "setfldcolr.h"
