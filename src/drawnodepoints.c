#include <math.h>
#include <stdlib.h>

#include "gmvdata.h"
#include "lights.h"

char sp[] = ".";

void drawnodepoints()

{

  /*  This routine plots node points.  */

  int ip, ic1, gopt, id, ncount, itype, id2;
  double tempf, fld_min, fld_max;
  int incr;
  short matnum, *matcolr, unselflag;
  char logflag;
  void nfsetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval, char logflag, int *icl);

   incr = 1;
   /* if (fastdraw == 1) incr = drawincr; */

   if (nodefieldactive > -1 && nodefieldactive < numvars)
     {
      logflag = nfld_log[nodefieldactive];
      fld_min = nfld_nice_min[nodefieldactive];
      fld_max = nfld_nice_max[nodefieldactive];
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (nodefieldactive == -2) matcolr = nodegrpcolr;
   else matcolr = imat;

   /*  Count the number of nodes selected.  */
   ncount = 0;
   for (ip = 1; ip < nnodes+1; ip += incr)
     {
      if ((int)nodeselectflag[ip] == 1)
        {
         ncount++;
         if (node0matflag == 0 && matcolr[ip] < 1) ncount--;
        }
     }
   if (drawunselnodes) ncount = nnodes;
   if (numplv+ncount > plvarray_alloc)
     {
      ncount += 1000;
      plvarray_alloc += ncount;
      plvarray = (struct plvelem *)realloc(plvarray,
                  plvarray_alloc*sizeof(struct plvelem));
     }

   for (ip = 1; ip < nnodes+1; ip += incr)
     { 

      /*  See if the point has been selected for display.  */
      gopt = (int)nodeselectflag[ip];
 
      /*  Eliminate nodes with material   */
      /*  or group < 1 unless requested.  */
      if (node0matflag == 0 && matcolr[ip] < 1) gopt = 0;
 
      /*  See if the point lies within the subset.  */
      if ((int)nodesubsetflag[ip] == 0)
         gopt = 0;

      /*  If drawing unselected nodes, reset gopt.  */
      unselflag = 0;
      if (drawunselnodes && gopt == 0)
        {
         unselflag = 1;
         gopt = 1;
        }

      if (gopt == 1)
        {

         /* Set material/flag color for this region. */
         if (nodefieldactive < 0 || nodefieldactive >= numvars)
           {
            matnum = matcolr[ip] - 1;
            if (nodefieldactive >= numvars)
               matnum = flags[nodefieldactive-numvars][ip] - 1;
            ic1 = matnum;

            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }

            if (unselflag)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
            } 

         /* If a field is to be colored, reset the color. */ 
         if (nodefieldactive > -1 && nodefieldactive < numvars && !unselflag)
           {
            nfsetfldcolr(nodefield[ip],fld_min,fld_max,tempf,logflag,&ic1);
           }
 
         itype = PONLY;
         if (nodesflag == 2) itype = SPHERE;
         if (unselflag) itype = PONLY;
         id = 0;
         id2 = 0;
         if (nodenumflag == 1 && !unselflag)
           {
            id = ip;
            if (nodeidflag) id = nodeids[ip];
            itype = PANDNUM;
           }

#ifdef MPI
         if (local_idflag)
           {
            id = nodepe[ip];
            id2 = nodenuminpe[ip];
           }
#endif

         plvarray[numplv].itype = itype;
         plvarray[numplv].x1 = nodes[ip].x;
         plvarray[numplv].y1 = nodes[ip].y;
         plvarray[numplv].z1 = nodes[ip].z;
         plvarray[numplv].x2 = id;
         plvarray[numplv].y2 = id2;
         plvarray[numplv].z2 = 0.;
         plvarray[numplv].iclr1 = ic1;
         plvarray[numplv].iclr2 = ic1;
         numplv++;
        }
     }
}

void nfsetfldcolr
#include "setfldcolr.h"
