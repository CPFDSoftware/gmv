
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

void drawanimvelflow(int vtype)

{
  int iv, i, j, k, govect, ii, jj, ibeg, iend, jcell, ncount, itype, nvects;
  double  tempf, fac, speed, maxspd, minspd, xv, yv, zv, uv, vv, wv;
  double xbeg, ybeg, zbeg, xend, yend, zend, vlogoffset, t;
  double xv2, yv2, zv2, fact2, *field, fld_min, fld_max, xn, yn, zn;
  char logflag;
  float expx, expy, expz;
  int incr;
  int fieldactive, nvars, iex, faceid[1000], nfverts;
  short matnum, *mats, *vflags, mat0flag;
  char  *selectflag, *subsetflag;
  void avsetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval,  char logflag, int *ic1);

   glDisable(GL_LIGHTING);
   glShadeModel(GL_FLAT);

   /*  Set variables for vector type:  */
   /*    cells (vtype=0)               */
   /*    nodes (vtype=1)               */
   /*    surfaces (vtype=2)            */
   /*    faces (vtype=3)               */
   if (vtype == 0)
     {
      maxspd = cmaxspeed;
      minspd = cfieldmin[cspeedfld];
      ibeg = 0;
      iend = ncells;
      selectflag = cellselectflag;
      subsetflag = cellsubsetflag;
      mat0flag = cell0matflag;
      mats = cellmats;
      if (cellfieldactive == -2) mats = cellgrpcolr;
      nvars = cnumvars;
      field = cellfield;
      fieldactive = cellfieldactive - numvars;
      if (cellfieldactive > -1 && cellfieldactive < numvars)
         fieldactive = -1;
      if (fieldactive > -1 && fieldactive < cnumvars)
        {
         logflag = cfld_log[fieldactive];
         fld_min = cfld_nice_min[fieldactive];
         fld_max = cfld_nice_max[fieldactive];
        }
      if (fieldactive > cnumvars)
         vflags = cellflags[fieldactive - cnumvars];
     }
   else if (vtype == 1)
     {
      maxspd = maxspeed;
      minspd = fieldmin[speedfld];
      ibeg = 1;
      iend = nnodes + 1;
      selectflag = nodeselectflag;
      subsetflag = nodesubsetflag;
      mat0flag = node0matflag;
      mats = imat;
      if (nodefieldactive == -2) mats = nodegrpcolr;
      nvars = numvars;
      field = nodefield;
      fieldactive = nodefieldactive;
      if (fieldactive > -1 && fieldactive < numvars)
        {
         logflag = nfld_log[fieldactive];
         fld_min = nfld_nice_min[fieldactive];
         fld_max = nfld_nice_max[fieldactive];
        }
      if (fieldactive > numvars)
         vflags = flags[fieldactive - numvars];
     }
   else if (vtype == 2)
     {
      maxspd = smaxspeed;
      minspd = sfieldmin[sspeedfld];
      ibeg = 0;
      iend = nsurf;
      selectflag = surfselectflag;
      subsetflag = surfsubsetflag;
      mat0flag = surf0matflag;
      mats = surfmats;
      if (surffieldactive == -2) mats = surfgrpcolr;
      nvars = snumvars;
      field = surffield;
      fieldactive = surffieldactive - numvars;
      if (surffieldactive > -1 && surffieldactive < numvars)
         fieldactive = -1;
      if (fieldactive > -1 && fieldactive < snumvars)
        {
         logflag = sfld_log[fieldactive];
         fld_min = sfld_nice_min[fieldactive];
         fld_max = sfld_nice_max[fieldactive];
        }
      if (fieldactive > snumvars)
         vflags = surfflags[fieldactive - snumvars];
     }
   else if (vtype == 3)
     {
      maxspd = fmaxspeed;
      minspd = ffieldmin[fspeedfld];
      ibeg = 0;
      iend = ncells;
      selectflag = cellfacedraw;
      subsetflag = cellsubsetflag;
      mat0flag = 1;
      mats = cellmats;
      if (cellfieldactive == -2) mats = cellgrpcolr;
      nvars = fnumvars;
      field = cellfield;
      fieldactive = cellfieldactive - (numvars + cnumvars);
      if (cellfieldactive > -1 && cellfieldactive < numvars)
         fieldactive = -1;
      if (fieldactive > -1 && fieldactive < fnumvars)
        {
         logflag = ffld_log[fieldactive];
         fld_min = ffld_nice_min[fieldactive];
         fld_max = ffld_nice_max[fieldactive];
        }
      if (fieldactive > cnumvars + fnumvars)
         vflags = cellflags[fieldactive - (cnumvars + fnumvars)];
     }

   if (fieldactive >=0 && fieldactive < nvars)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Draw vectors.  */
   incr = 1;
   itype = LSOLID;
   /* if (fastdraw == 1) incr = drawincr; */

   vlogoffset = 0;
   if (vectorlog)
     {
      if (minspd < .00001) minspd = .00001;
      minspd = log10(minspd);
      if (minspd < 0) vlogoffset = fabs(minspd);
      maxspd = log10(maxspd) + vlogoffset;
     }
   fac = velscale*span/(maxspd*20.);
   /* fact1 = (float)(animation_count-1) / (float)anim_iterations; */
   fact2 = (double)animation_count / (double)anim_iterations;

   /*  Count the vectors to draw.  */
   ncount = 0;
   for (iv = ibeg; iv < iend; iv+=incr)
     {
      ncount += (int)selectflag[iv];
     }
   if (numplv+ncount > plvarray_alloc)
     {
      ncount += ncount + 1000;
      plvarray_alloc += ncount;
      plvarray = (struct plvelem *)realloc(plvarray,
                  plvarray_alloc*sizeof(struct plvelem));
     }

   for (iv = ibeg; iv < iend; iv+=incr)
     {

      /*  See if the point has been selected for display.  */
      govect = (int)selectflag[iv];

      /*  Eliminate points with material < 1.  */
      if (mat0flag == 0 && mats[iv] < 1) govect = 0;

      /*  See if the point lies within the subset.  */
      if ((int)subsetflag[iv] == 0) 
         govect = 0;

      /*  Process selected vectors.  */
      if (govect == 1)
        {

         /*  Set x,y,z, and u,v,w.  */
         if (vtype == 1)
           {
            xv = nodes[iv].x;  yv = nodes[iv].y;  zv = nodes[iv].z;
            uv = uic[iv];  vv = vic[iv];  wv = wic[iv];
            nvects = 1;
           }
         else if (vtype == 0 || vtype == 3)
           {
            jcell = iv;

            /*  Fill cell structure if structured grid.  */
            if (govect == 1 && structuredflag == 1)
              {
               jcell = 0;
               fillstructcell(iv,0);
              }

            /*  Determine explode factor type.  */
            expx = 0.;  expy = 0.;  expz = 0.;
            if (cexplodefact > 0.)
              {
               if (cellexpflag == -1) 
                  iex = (int)cellmats[iv] - 1;
               else iex = (int)cellflags[cellexpflag][iv] - 1;
               expx = cexplodefact*cellexpx[iex];
               expy = cexplodefact*cellexpy[iex];
               expz = cexplodefact*cellexpz[iex];
              }
            xv = cells[jcell].xavg + expx;
            yv = cells[jcell].yavg + expy;
            zv = cells[jcell].zavg + expz;
            uv = cuic[iv];  vv = cvic[iv];  wv = cwic[iv];
            if (vtype == 0) nvects = 1;
            else 
              {
               nvects = 0;
               for (i = 0; i < cells[jcell].nfaces; i++)
                 {
                  jj = cells[iv].facenos[i];
                  if ((int)facesel[jj] == 1)
                    {
                     faceid[nvects] = jj;
                     nvects++;
                    }
                 }
              }
           }
         else if (vtype == 2)
           {

            /*  Determine explode factor type.  */
            expx = 0.;  expy = 0.;  expz = 0.;
            if (sexplodefact > 0.)
              {
               if (surfexpflag == -1) 
                  iex = (int)surfmats[iv] - 1;
               else iex = (int)surfflags[surfexpflag][iv] - 1;
               expx = sexplodefact*surfexpx[iex];
               expy = sexplodefact*surfexpy[iex];
               expz = sexplodefact*surfexpz[iex];
              }
            xv = surfs[jcell].xavg + expx;
            yv = surfs[jcell].yavg + expy;
            zv = surfs[jcell].zavg + expz;
            uv = suic[iv];  vv = svic[iv];  wv = swic[iv];
            nvects = 1;
           }
 
         /* Set material/flag number. */
         matnum = mats[iv] - 1;
         if (fieldactive > nvars)
            matnum = vflags[iv] - 1;

         /*  Draw a vector in material,  */
         /*  flag or field color.        */
         for (ii = 0; ii < nvects; ii++)
           {

            /*  Calculate face center for face vertices.  */
            if (vtype == 3)
              {
               jj = faceid[ii];
               nfverts = cellfaces[jj].nverts;
               xv = 0.;  yv = 0.;  zv = 0;
               for (j = 0; j < nfverts; j++)
                 {
                  k = cellfaces[jj].fverts[j];
                  xv += nodes[k].x + expx;
                  yv += nodes[k].y + expy;
                  zv += nodes[k].z + expz;
                 }
               xv /= (float)nfverts;
               yv /= (float)nfverts;
               zv /= (float)nfverts;
               uv = fuic[iv];  vv = fvic[iv];  wv = fwic[iv];
              }

            speed = sqrt((uv*uv) + (vv*vv) + (wv*wv));
            if (vectorlog) speed = log10(speed) + vlogoffset;
            t = speed * fac;
            if (t > .00001) 
              {

               /*  If material or flag color selected set color.  */
               if (fieldactive < 0 || fieldactive >= nvars)
                 {
                  i = matnum;
                  if (i < 0 || i > MAXMATS) 
                    {
                     if (txtcolor < 256) i = -1;
                     else i = -2;
                    }
                 }

               /*  Set field color, if selected.  */
               else
                 {
                  avsetfldcolr(field[iv],fld_min,fld_max,tempf,logflag,
                               &i);
                 }
               if (vtype == 3) i = -3;

               /*  Determine begining and end points of vector.  */
               t = sqrt(uv*uv + vv*vv + wv*wv);
               xn = uv / t;  yn = vv / t;  zn = wv / t;
               xbeg = xv;
               ybeg = yv;
               zbeg = zv;
               xend = xv + xn * speed * fac;
               yend = yv + yn * speed * fac;
               zend = zv + zn * speed * fac;

               /*  Create fractional part of vector  */ 
               /*  for this animation sequence.      */
               xv2 = xbeg + fact2 * (xend - xbeg);
               yv2 = ybeg + fact2 * (yend - ybeg);
               zv2 = zbeg + fact2 * (zend - zbeg);

               plvarray[numplv].itype = itype;
               plvarray[numplv].x1 = xbeg;
               plvarray[numplv].y1 = ybeg;
               plvarray[numplv].z1 = zbeg;
               plvarray[numplv].x2 = xv2;
               plvarray[numplv].y2 = yv2;
               plvarray[numplv].z2 = zv2;
               plvarray[numplv].iclr1 = i;
               plvarray[numplv].iclr2 = i;
               numplv++;
              }
           }
        }
     }  
}

void avsetfldcolr
#include "setfldcolr.h"

