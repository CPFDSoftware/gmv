#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "lights.h"

void drawvectors(int vtype)

{
  int iv, i, j, k, govect, ii, jj, ibeg, iend, jcell, ncount, itype, nvects;
  double fac, speed, maxspd, xv, yv, zv, uv, vv, wv;
  double *field;
  double tempf, fld_min, fld_max;
  char logflag;
  float expx, expy, expz;
  int fieldactive, nvars, iex, faceid[1000], nfverts;
  int incr;
  short matnum, *mats, *vflags, mat0flag;
  char *selectflag, *subsetflag;
  void vlsetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval,  char logflag, int *icl);

   /*  Set variables for vector type:  */
   /*     cells (vtype=0)              */
   /*     nodes (vtype=1)              */
   /*     surfaces (vtype=2)           */
   /*     faces (vtype=3)              */
   if (vtype == 0)
     {
      itype = CVECTOR;
      maxspd = cmaxspeed;
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
      if (fieldactive > cnumvars + fnumvars)
         vflags = cellflags[fieldactive - (cnumvars + fnumvars)];
     }
   else if (vtype == 1)
     {
      itype = NVECTOR;
      maxspd = maxspeed;
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
      itype = SVECTOR;
      maxspd = smaxspeed;
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
      itype = FVECTOR;
      maxspd = fmaxspeed;
      ibeg = 0;
      iend = ncells;
      selectflag = cellfacedraw;
      if (cellfacedraw == NULL)
         selectflag = cellselectflag;
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
   /* if (fastdraw == 1) incr = drawincr; */

   /*  If no drawable vectors exist (maxspd = 0) quit.  */
   if (maxspd == 0) return;

   fac = velscale*span/(maxspd*20.);

   /*  Count the number of vectors selected.  */
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

   /*  Loop over vectors.  */
   for (iv = ibeg; iv < iend; iv+=incr)
     {

      /*  See if the point has been selected for display.  */
      govect = (int)selectflag[iv];
 
      /*  Eliminate nodes with material < 1 unless requested.  */
      if (mat0flag == 0)
        {
         if (mats[iv] < 1) govect = 0;
        }
 
      /*  See if the point lies within the subset.  */
      if (vtype < 3 && (int)subsetflag[iv] == 0)
         govect = 0;

      /*  Process selected vector.  */
      if (govect)
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
            if (vtype == 0)
              {
               nvects = 1;
               uv = cuic[iv];  vv = cvic[iv];  wv = cwic[iv];
              }
            else 
              {
               nvects = 0;
               for (i = 0; i < cells[jcell].nfaces; i++)
                 {
                  jj = cells[iv].facenos[i];
                  if (facesel == NULL)
                    {
                     faceid[nvects] = jj;
                     nvects++;                     
                    }
                  else if ((int)facesel[jj] == 1)
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

            xv = surfs[iv].xavg + expx;
            yv = surfs[iv].yavg + expy;
            zv = surfs[iv].zavg + expz;
            uv = suic[iv];  vv = svic[iv];  wv = swic[iv];
            nvects = 1;
           }

         /* Set material/flag number. */
         matnum = mats[iv] - 1;
         if (fieldactive > nvars)
            matnum = vflags[iv] - 1;

         /*  Set a vector in material, flag or field color.  */
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
               uv = fuic[jj];  vv = fvic[jj];  wv = fwic[jj];
              }

            speed = sqrt((uv*uv) + (vv*vv) + (wv*wv));
            speed *= fac;
            if (speed > .00001) 
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

               plvarray[numplv].itype = itype;
               plvarray[numplv].x1 = xv;
               plvarray[numplv].y1 = yv;
               plvarray[numplv].z1 = zv;
               plvarray[numplv].x2 = uv;
               plvarray[numplv].y2 = vv;
               plvarray[numplv].z2 = wv;
               plvarray[numplv].iclr1 = i;
               plvarray[numplv].iclr2 = i;
               numplv++;
              }
           }
        }
     }  
}


void vlsetfldcolr
#include "setfldcolr.h"
