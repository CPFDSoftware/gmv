#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

void drawtracers()

{

  /*  This routine plots tracers as either an icosa-  */
  /*  hedron, the tracer number or the tracer value.  */

  int i, ip, ic1, gopt, ic2, ih, itype, id, SIZEno;
  double v1[3], v2[3], tempf, xck, yck, zck, fval, fval2, fld_min, fld_max;
  char logflag;
  int incr;
  void trsetfldcolr(double fldval, double fldmin, double fldmax,
                    double tempval, char logflag, int *icl);

   incr = 1;
   /* if (fastdraw == 1 && ntracers > 2*incr) incr = drawincr; */

   logflag = trfld_log[tracefieldactive];
   fld_min = trfld_nice_min[tracefieldactive];
   fld_max = trfld_nice_max[tracefieldactive];
   tempf = fld_max - fld_min;
   if (logflag) tempf = log10(fld_max) - log10(fld_min);

   if (numplv+ntracers > plvarray_alloc)
     {
      plvarray_alloc += ntracers + 1000;
      plvarray = (struct plvelem *)realloc(plvarray,
                  plvarray_alloc*sizeof(struct plvelem));
      if (plvarray == NULL) memerr();
     }
   if (tracehistflag && ntracehist > 1)
     {
      i = ntracers * ntracehist;
      if (numplv+i > plvarray_alloc)
        {
         plvarray_alloc += i + 1000;
         plvarray = (struct plvelem *)realloc(plvarray,
                     plvarray_alloc*sizeof(struct plvelem));
         if (plvarray == NULL) memerr();
        }
     }

   SIZEno = -1;
   for (i = 0; i < numvartr; i++)
      if (strcmp(fieldtrname[i],"SIZE") == 0 ||
          strncmp(fieldtrname[i],"SIZE ",5) == 0) SIZEno = i;


   for (ip = 0; ip < ntracers; ip += incr)
     {
      gopt = (int) traceselectflag[ip]; 

      /*  See if the tracer point lies within the subset.  */
      xck = tracers[0].x[ip];
      yck = tracers[0].y[ip];
      zck = tracers[0].z[ip];
      if (xck > xmaxfdval) gopt = 0;
      if (xck < xminfdval) gopt = 0;
      if (yck > ymaxfdval) gopt = 0;
      if (yck < yminfdval) gopt = 0;
      if (zck > zmaxfdval) gopt = 0;
      if (zck < zminfdval) gopt = 0;

      if (gopt == 1)
        {

         /*  Set the trace field color.  */ 
         fval = tracers[0].field[tracefieldactive][ip];
         trsetfldcolr(fval,fld_min,fld_max,tempf,logflag,&ic1);

         /*  Set tracer drawing flags.  */
         if (tracerflag == 1) itype = TPOINT;

         /*  Draw an unshaded icosahedron at the point.  */
         if (tracerflag == 2) itype = SPHERE;

         /*  Draw the tracer no. or the value at the point.  */
         if (tracerflag == 3 || tracerflag == 4)
           {
            itype = TNUMVAL;
            if (tracerflag == 3) 
              {
               id = ip + 1;
               if (traceridflag) id = tracerids[ip];
               fval2 = id;
              }
            if (tracerflag == 4) fval2 = fval;
           }

         /*  Draw an unshaded icosahedron of radius SIZE at the point.  */
         if (tracerflag == 5)
           {
            itype = TSIZE;
            fval2 = fval;
           }

         /*  Draw a point at the tracer location.  */
         plvarray[numplv].itype = itype;
         plvarray[numplv].x1 = xck;
         plvarray[numplv].y1 = yck;
         plvarray[numplv].z1 = zck;
         plvarray[numplv].x2 = fval2;
         plvarray[numplv].y2 = 0;
         if (itype == TSIZE)
            plvarray[numplv].y2 = tracers[0].field[SIZEno][ip];
         plvarray[numplv].z2 = 0.;
         plvarray[numplv].iclr1 = ic1;
         plvarray[numplv].iclr2 = ic1;
         numplv++;

         /*  Draw Gouraud colored history lines if requested.  */
         if (tracehistflag && ntracehist > 1)
           {

            /*  Draw points along history lines.  */
            for (ih = 1; ih < ntracehist; ih++)
              {
               v1[0] = tracers[ih-1].x[ip];
               v1[1] = tracers[ih-1].y[ip];
               v1[2] = tracers[ih-1].z[ip];
               v2[0] = tracers[ih].x[ip];
               v2[1] = tracers[ih].y[ip];
               v2[2] = tracers[ih].z[ip];

               fval2 = tracers[ih].field[tracefieldactive][ip];
               trsetfldcolr(fval2,fld_min,fld_max,tempf,logflag,&ic2);

               /*  Draw the line.  */
               plvarray[numplv].itype = THIST;
               plvarray[numplv].x1 = v1[0];
               plvarray[numplv].y1 = v1[1];
               plvarray[numplv].z1 = v1[2];
               plvarray[numplv].x2 = v2[0];
               plvarray[numplv].y2 = v2[1];
               plvarray[numplv].z2 = v2[2];
               plvarray[numplv].iclr1 = ic1;
               plvarray[numplv].iclr1 = ic2;
               numplv++;

               ic1 = ic2;
              }
           }
        }
     }
}


void trsetfldcolr
#include "setfldcolr.h"
