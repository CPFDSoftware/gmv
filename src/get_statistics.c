#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "gmvdata.h"
#include "contour.h"

#define MAXVAL 9.99999e32

extern void memerr();
extern void fillstructcell(int icell, int iopt);
extern void writefacefld(double *value);
extern void drawghostcelloptions();
extern void setfacesel();
extern void readpolyfile();

short alltetmesh;
int maxpolyverts, maxfaceverts;

void get_statistics()
{
  int i, j, k, l, m, npolys, *count, ii, jj, ifound, numfaces;
  long nv;
  short *gnodechk;
  double xp, yp, zp, f, fval;
  double xsum, ysum, zsum;
  double xdif, ydif, zdif;
  double *field, *cfield;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3, xno, yno, zno, t;
  void writenodefld(double *value);
  void writecellfld(double *value);
  void writesurffld(double *value);
  void fillnodevalue(int ifld, double *value);
  void fillcellvalue(int ifld, double *value);
  void fillfacevalue(int ifld, double *value);
  void fillsurfvalue(int ifld, double *value);
  void matexpvects();
  void savenodecoords();

   /*  Get x,y,z min and max. from nodes.  */
   globxicmin = MAXVAL;  globxicmax = -MAXVAL;
   globyicmin = MAXVAL;  globyicmax = -MAXVAL;
   globzicmin = MAXVAL;  globzicmax = -MAXVAL;
   for (i = 1; i < nnodes+1; i++)
     {
      xp = nodes[i].x;  yp = nodes[i].y;  zp = nodes[i].z;
      globxicmin = (globxicmin < xp) ? globxicmin : xp;
      globxicmax = (globxicmax > xp) ? globxicmax : xp;
      globyicmin = (globyicmin < yp) ? globyicmin : yp;
      globyicmax = (globyicmax > yp) ? globyicmax : yp;
      globzicmin = (globzicmin < zp) ? globzicmin : zp;
      globzicmax = (globzicmax > zp) ? globzicmax : zp;
     }

   /*  Assume cell bounds are node bounds.  */
   cellsxmin = globxicmin;  cellsxmax = globxicmax;
   cellsymin = globyicmin;  cellsymax = globyicmax;
   cellszmin = globzicmin;  cellszmax = globzicmax;

   /*  Set tracer select flags.  */
   if (ntracers > 0)
     {
      traceselectflag = (char *)malloc(ntracers*sizeof(char));
      if (traceselectflag == NULL) memerr();
      for (i = 0; i < ntracers; i++)
         traceselectflag[i] = 1;
      traceselnum_on = 0;
      for (i = 0; i < MAXTRFIELDS; i++) trfieldlimauto[i] = 0;
     }

   /*  Check x,y,z min and max. from tracers, if any.  */
   for (i = 0; i < ntracers; i++)
     {
      xp = tracers[0].x[i];
      yp = tracers[0].y[i];
      zp = tracers[0].z[i];
      globxicmin = (globxicmin < xp) ? globxicmin : xp;
      globxicmax = (globxicmax > xp) ? globxicmax : xp;
      globyicmin = (globyicmin < yp) ? globyicmin : yp;
      globyicmax = (globyicmax > yp) ? globyicmax : yp;
      globzicmin = (globzicmin < zp) ? globzicmin : zp;
      globzicmax = (globzicmax > zp) ? globzicmax : zp;
     }

   /*  If polygon material nos. > mmats, set mat names.  */
   maxcellmat = mmats;
   if (maxmatno > mmats)
     {
      for (j = mmats; j < maxmatno; j++)
        {
         sprintf(mmatnames[j],"mat%d",j+1);
        }
      mmats = maxmatno;
     }

   /*  Loop through materials and check    */  
   /*  mins and maxs of polygon vertices.  */
   maxpolyverts = 0;
   if (polysin == 1)
     {
      for (i = 0; i < mmats; i++)
        {
         for (j = 0; j < npolyspermat[i]; j++)
           {
            m = (int)polygons[i][j].nverts;
            nv = polygons[i][j].polyvertloc;
            for (k = 0; k < m; k++)
              {
               xp = polyverts[i][k+nv].x;
               yp = polyverts[i][k+nv].y;
               zp = polyverts[i][k+nv].z;
               globxicmin = (globxicmin < xp) ? globxicmin : xp;
               globxicmax = (globxicmax > xp) ? globxicmax : xp;
               globyicmin = (globyicmin < yp) ? globyicmin : yp;
               globyicmax = (globyicmax > yp) ? globyicmax : yp;
               globzicmin = (globzicmin < zp) ? globzicmin : zp;
               globzicmax = (globzicmax > zp) ? globzicmax : zp;
              } 
            /*  Determine maximum number of polygon vertices.  */
            if (m > maxpolyverts) maxpolyverts = m;       
           }
        }
     }

   /*  Save problem mins. and maxs. for bounding box.  */
   xminbbval = globxicmin; xmaxbbval = globxicmax;
   yminbbval = globyicmin; ymaxbbval = globyicmax;
   zminbbval = globzicmin; zmaxbbval = globzicmax;   
   xminbbvalo = globxicmin; xmaxbbvalo = globxicmax;
   yminbbvalo = globyicmin; ymaxbbvalo = globyicmax;
   zminbbvalo = globzicmin; zmaxbbvalo = globzicmax;

   /*  Save original mins and maxs.  */ 
   allxmin = globxicmin; allxmax = globxicmax;
   allymin = globyicmin; allymax = globyicmax;
   allzmin = globzicmin; allzmax = globzicmax;   

   /*  Extend the problem mins. and maxs.     */
   /*  to ensure enclosure of all drawables.  */
   xdif = fabs(globxicmax - globxicmin);
   ydif = fabs(globyicmax - globyicmin);
   zdif = fabs(globzicmax - globzicmin);
   globxicmin = globxicmin - .01 * xdif;
   globyicmin = globyicmin - .01 * ydif;
   globzicmin = globzicmin - .01 * zdif;
   globxicmax = globxicmax + .01 * xdif;
   globyicmax = globyicmax + .01 * ydif;
   globzicmax = globzicmax + .01 * zdif;

   /*  Set initial cell box, node box and surface box selection data.  */
   cellselbox_xmin = globxicmin;
   cellselbox_xmax = globxicmax;
   cellselbox_ymin = globyicmin;
   cellselbox_ymax = globyicmax;
   cellselbox_zmin = globzicmin;
   cellselbox_zmax = globzicmax;
   nodeselbox_xmin = globxicmin;
   nodeselbox_xmax = globxicmax;
   nodeselbox_ymin = globyicmin;
   nodeselbox_ymax = globyicmax;
   nodeselbox_zmin = globzicmin;
   nodeselbox_zmax = globzicmax;
   surfselbox_xmin = globxicmin;
   surfselbox_xmax = globxicmax;
   surfselbox_ymin = globyicmin;
   surfselbox_ymax = globyicmax;
   surfselbox_zmin = globzicmin;
   surfselbox_zmax = globzicmax;
   traceselbox_xmin = globxicmin;
   traceselbox_xmax = globxicmax;
   traceselbox_ymin = globyicmin;
   traceselbox_ymax = globyicmax;
   traceselbox_zmin = globzicmin;
   traceselbox_zmax = globzicmax;

   /*  Get cell center(avg).  */
   if (structuredflag == 0)
     {
      for (i = 0; i < ncells; i++)
        {
         xsum = ysum = zsum = 0;
         for (j = 0; j < cells[i].ncnodes; j++)
           {
            m = cells[i].cnodes[j];
            xsum += nodes[m].x;
            ysum += nodes[m].y;
            zsum += nodes[m].z;
           }
         cells[i].xavg = xsum / (float)cells[i].ncnodes;
         cells[i].yavg = ysum / (float)cells[i].ncnodes;
         cells[i].zavg = zsum / (float)cells[i].ncnodes;
        }
     }

   /*  If no materials entered, allocate and  */
   /*  set all to one then generate a name.   */
   if (maxcellmat == 0)
     {
      maxcellmat = 1;
      if (mmats == 0) mmats = 1;
      imat=(short *)malloc((nnodes+1)*sizeof(short));
      if (imat == NULL) memerr();
      for (i = 0; i < nnodes+1; i++) imat[i] = 1;
      sprintf(mmatnames[0],"mat_1");
      if (ncells > 0)
        {
         cellmats=(short *)malloc((ncells)*sizeof(short));
         if (cellmats == NULL) memerr();
         for (i = 0; i < ncells; i++) cellmats[i] = 1;
        }
     }

   /*  Get maximum node/cell material no.  */
   maxcellmat = 0;
   for (i = 1; i < nnodes+1; i++)
      if (imat[i] > maxcellmat) maxcellmat = imat[i];
   for (i = 0; i < ncells; i++)
      if (cellmats[i] > maxcellmat) maxcellmat = cellmats[i];

   /*  If surface polygons exist, allocate  */
   /*  and set polygon draw flag.           */
   if (polysin == 1)
     {
      for (i = 0; i < maxmatno; i++)
        {
         npolys = npolyspermat[i];
         if (npolys > 0)
           {
            polydrawchk[i] = (char *)malloc(npolys*sizeof(char));
            if (polydrawchk[i] == NULL) memerr();
            for (j = 0; j < npolys; j++) polydrawchk[i][j] = 1;
           }
        }
     }

   /*  If cell fields entered, and no field name entered,  */
   /*  average to nodes and add to node field list.        */
   nodevarsin = numvars;
   if (gmvflag && cnumvars > 0)
     {
      field=(double *)malloc((nnodes+1)*sizeof(double)); 
      cfield=(double *)malloc((ncells)*sizeof(double)); 
      count = (int *)malloc((nnodes+1)*sizeof(int));
      if (field == NULL || cfield == NULL || count == NULL) memerr();
  
      for (l = 0; l < cnumvars; l++)
        {

         /*  See if this field name exists.  */
         ifound = 0;
         for (i = 0; i < numvars; i++)
           {
            if (strcmp(cfieldname[l],fieldname[i]) == 0) ifound = 1;
           }
         if (!ifound)
           {
            fieldname[numvars]=(char *)malloc(33*sizeof(char));
            if (fieldname[numvars] == NULL) memerr();
            strcpy(fieldname[numvars],cfieldname[l]);

            for (i = 0; i < nnodes+1; i++)
              { field[i] = 0;  count[i] = 0; }
            fillcellvalue(l, cfield);

            for (jj = 0; jj < ncells; jj++)
              {
               if (cellmats[jj] > 0)
                 {
                  ii = jj;
                  if (structuredflag == 1)
                    {
                     ii = 0;
                     fillstructcell(jj,0);
                    }
                  for (j = 0; j < cells[ii].ncnodes; j++)
                    {
                     k = cells[ii].cnodes[j];
                     field[k] += cfield[jj];
                     count[k]++;
                    }
                 }
              }
            for (i = 1; i < nnodes+1; i++)
              {
               if(count[i])
                 field[i] = field[i] /
                            ((float)count[i]);
              } 
            writenodefld(field);

            /*  If cell field data was from subvars,        */
            /*  set node field min and max from cellfield.  */
            if (csubvarsin[l] == 1)
              {
               nsubvarsin[numvars] = 1;
               fieldmin[numvars] = cfieldmin[l];
               fieldmax[numvars] = cfieldmax[l];
              }

            numvars++;
           }
        }
      free(cfield);  free(count);  free(field);
     }

   /*  If cell velocities entered, average to nodes.   */
   if (gmvflag && cvectors == 1 && vectors == 0)
     {
      vectors = 1;
      uic = (double *)malloc((nnodes+1)*sizeof(double));
      vic = (double *)malloc((nnodes+1)*sizeof(double));
      wic = (double *)malloc((nnodes+1)*sizeof(double));
      if (uic == NULL || vic == NULL || wic == NULL) memerr();

      count = (int *)malloc((nnodes+1)*sizeof(int));
      if (count == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
        { uic[i] = 0.;  vic[i] = 0.;  wic[i] = 0.; count[i] = 0; }

      for (i = 0; i < ncells; i++)
        {
         if (cellmats[i] > 0)
           {
            ii = i;
            if (structuredflag == 1)
              {
               ii = 0;
               fillstructcell(i,0);
              }
            for (j = 0; j < cells[ii].ncnodes; j++)
              {
               k = cells[ii].cnodes[j];
               uic[k] += cuic[i];
               vic[k] += cvic[i];
               wic[k] += cwic[i];
               count[k]++;
              }
           }
        }

      for (i = 1; i < nnodes+1; i++)
        if(count[i])
          { 
           uic[i] = uic[i] / ((float) count[i]);
           vic[i] = vic[i] / ((float) count[i]);
           wic[i] = wic[i] / ((float) count[i]);
          }
      free(count);
     }

   /*  Save node velocity components as    */ 
   /*  field data with the names U, V, W.  */
   if (vectors == 1 && (numvars+3) < MAXFIELDS)
     {
      fieldname[numvars] = (char *)malloc(33*sizeof(char));
      if (fieldname[numvars] == NULL) memerr();
      strcpy(fieldname[numvars],"U       ");
      writenodefld(uic);
      vectfldx = numvars;
      numvars++;

      fieldname[numvars] = (char *)malloc(33*sizeof(char));
      if (fieldname[numvars] == NULL) memerr();
      strcpy(fieldname[numvars],"V       ");
      writenodefld(vic);
      vectfldy = numvars;
      numvars++;

      fieldname[numvars] = (char *)malloc(33*sizeof(char));
      if (fieldname[numvars] == NULL) memerr();
      strcpy(fieldname[numvars],"W       ");
      writenodefld(wic);
      vectfldz = numvars;
      numvars++;
     }

   /*  Make node speed field.  */
   if (numvars+1 < MAXFIELDS)
     {
      fieldname[numvars] = (char *)malloc(33*sizeof(char));
      if (fieldname[numvars] == NULL) memerr();
      strcpy(fieldname[numvars],"Vect Mag");

      field=(double *)malloc((nnodes+1)*sizeof(double)); 
      memset(field,nnodes+1, sizeof(double));
      if (field == NULL) memerr();  
      for (i = 1; i < nnodes+1; i++) field[i] = 0;

      /*  If velocities present, calc. speed and get max.  */
      maxspeed = 0;
      if (vectors > 0)
        {
         for (i = 1; i < nnodes+1; i++)
           {
            field[i] = sqrt((uic[i] * uic[i]) + 
                            (vic[i] * vic[i]) +
                            (wic[i] * wic[i]));
            maxspeed = (field[i] > maxspeed) ? 
                        field[i] : maxspeed;
           }
         FREE(uic);  FREE(vic);  FREE(wic);
        } 
     
      writenodefld(field);
      speedfld = numvars;
      numvars++;
      free(field); 
     }

   /*  Make node field calc. fields.  */
   fldcalcbeg = numvars;
   if (numvars+1 < MAXFIELDS)
     {
      field=(double *)malloc((nnodes+1)*sizeof(double));
      if (field == NULL) memerr();   

      if (physedflag) fldcalc_has_data = 1;   

      for (i = 0; i < nnodes; i++) field[i] = 0; 

      for (i = 0; i < MAXFLDCALC; i++)
        {
         fieldcalc[i].xfldno = -1;
         fieldcalc[i].yfldno = -1;
         fieldcalc[i].oper = -1;
         fieldcalc[i].cval = 0.;
         sprintf(fieldcalc[i].name,"FldCalc%d",i+1);

         if (numvars < MAXFIELDS)
           {
            fieldname[numvars] = (char *)malloc(33*sizeof(char));
            if (fieldname[numvars] == NULL) memerr();
            strcpy(fieldname[numvars],fieldcalc[i].name);

            writenodefld(field);

            numvars++;
           }
        } 

     free(field);
     }      

   /*  Save node coordinates as node field data.  */
   savenodecoords();

   /*  Save cell velocity components as    */ 
   /*  field data with the names U, V, W.  */
   if (cvectors == 1 && (cnumvars+3) < MAXFIELDS)
     {
      cfieldname[cnumvars] = (char *)malloc(33*sizeof(char));
      if (cfieldname[cnumvars] == NULL) memerr();
      strcpy(cfieldname[cnumvars],"U       ");
      writecellfld(cuic);
      cvectfldx = cnumvars;
      cnumvars++;

      cfieldname[cnumvars] = (char *)malloc(33*sizeof(char));
      if (cfieldname[cnumvars] == NULL) memerr();
      strcpy(cfieldname[cnumvars],"V       ");
      writecellfld(cvic);
      cvectfldy = cnumvars;
      cnumvars++;

      cfieldname[cnumvars] = (char *)malloc(33*sizeof(char));
      if (cfieldname[cnumvars] == NULL) memerr();
      strcpy(cfieldname[cnumvars],"W       ");
      writecellfld(cwic);
      cvectfldz = cnumvars;
      cnumvars++;
     }

   /*  If more than one field exists, make cell speed field.  */
   if ((cnumvars > 1 || physedflag) && cnumvars < MAXFIELDS)
     {
      cfieldname[cnumvars] = (char *)malloc(33*sizeof(char));
      if (cfieldname[cnumvars] == NULL) memerr();
      strcpy(cfieldname[cnumvars],"Vect Mag");

      field=(double *)malloc((ncells)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (i = 0; i < ncells; i++) field[i] = 0;

      /*  If velocities present, calc. speed and get max.  */
      cmaxspeed = 0;
      if (cvectors > 0)
        {
         for (i = 0; i < ncells; i++)
           {
            field[i] = sqrt((cuic[i] * cuic[i]) + 
                            (cvic[i] * cvic[i]) +
                            (cwic[i] * cwic[i]));
            cmaxspeed = (field[i] > cmaxspeed) ? 
                         field[i] : cmaxspeed;
           }
         FREE(cuic);  FREE(cvic);  FREE(cwic);
        } 
     
      writecellfld(field);
      cspeedfld = cnumvars;
      cnumvars++;
      free(field); 
     }      

   /*  If at least one field exists, make cell field calc. fields.  */
   cfldcalcbeg = cnumvars;
   if (cnumvars > 0 || physedflag)
     {
      field=(double *)malloc((ncells)*sizeof(double)); 
      if (field == NULL) memerr();  

      if (physedflag) cfldcalc_has_data = 1;

      for (i = 0; i < ncells; i++) field[i] = 0;

      for (i = 0; i < MAXFLDCALC; i++)
        {
         cfieldcalc[i].xfldno = -1;
         cfieldcalc[i].yfldno = -1;
         cfieldcalc[i].oper = -1;
         cfieldcalc[i].cval = 0.;
         sprintf(cfieldcalc[i].name,"FldCalc%d",i+1);

         if (cnumvars < MAXFIELDS)
           {
            cfieldname[cnumvars] = (char *)malloc(33*sizeof(char));
            if (cfieldname[cnumvars] == NULL) memerr();
            strcpy(cfieldname[cnumvars],cfieldcalc[i].name);

            if (physedflag) writecellfld(field);

            cnumvars++;
           }
        } 
     
      free(field); 
     }  

   /*  Save face velocity components as    */ 
   /*  field data with the names U, V, W.  */
   if (fvectors == 1 && (fnumvars+3) < MAXFIELDS)
     {
      ffieldname[fnumvars] = (char *)malloc(33*sizeof(char));
      if (ffieldname[fnumvars] == NULL) memerr();
      strcpy(ffieldname[fnumvars],"U       ");
      writefacefld(fuic);
      fvectfldx = fnumvars;
      fnumvars++;

      ffieldname[fnumvars] = (char *)malloc(33*sizeof(char));
      if (ffieldname[fnumvars] == NULL) memerr();
      strcpy(ffieldname[fnumvars],"V       ");
      writefacefld(fvic);
      fvectfldy = fnumvars;
      fnumvars++;

      ffieldname[fnumvars] = (char *)malloc(33*sizeof(char));
      if (ffieldname[fnumvars] == NULL) memerr();
      strcpy(ffieldname[fnumvars],"W       ");
      writefacefld(fwic);
      fvectfldz = fnumvars;
      fnumvars++;
     }

   /*  If more than one field exists, make face speed field.  */
   if ((fnumvars > 1 || physedflag) && fnumvars < MAXFIELDS)
     {
      ffieldname[fnumvars] = (char *)malloc(33*sizeof(char));
      if (ffieldname[fnumvars] == NULL) memerr();
      strcpy(ffieldname[fnumvars],"Vect Mag");

      field=(double *)malloc((ncellfaces)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (i = 0; i < ncellfaces; i++) field[i] = 0;

      /*  If velocities present, calc. speed and get max.  */
      fmaxspeed = 0;
      numfaces = ncellfaces;
      if (mesh2dflag > 0) numfaces = ncell2dfaces;
      if (fvectors > 0)
        {
         for (i = 0; i < numfaces; i++)
           {
            field[i] = sqrt((fuic[i] * fuic[i]) + 
                            (fvic[i] * fvic[i]) +
                            (fwic[i] * fwic[i]));
            fmaxspeed = (field[i] > fmaxspeed) ? 
                         field[i] : fmaxspeed;
           }
         FREE(fuic);  FREE(fvic);  FREE(fwic);
        } 
     
      writefacefld(field);
      fspeedfld = fnumvars;
      fnumvars++;
      free(field); 
     }    

   /*  Allocate and set node and cell selection flags.  */
   if (nnodes > 0)
     {
      nodeselectflag = (char *)malloc((nnodes+1)*sizeof(char));
      if (nodeselectflag == NULL) memerr();
      nodesubsetflag = (char *)malloc((nnodes+1)*sizeof(char));
      if (nodesubsetflag == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
        {
         nodeselectflag[i] = 1;
         nodesubsetflag[i] = 1;
        }
      for (i = 0; i <= MAXFLAGS; i++)
        {
         for (j = 0; j < MAXMATS; j++)
            nodeselflag[i][j] = 1;
         if (i < MAXFLAGS) nodeandorflag[i] = 0;
        }
      if (numvars > 0) nodeandorflag[numflags_node] = 0;
      nodeselmat_on = 0;  nodeselfld_on = 0;  nodeselsph_on = 0;
      nodeselnum_on = 0;
      if (numvars > 0)
        {
         nodeselfield = 0;
         nfldselmin = fieldmin[0];
         nfldselmax = fieldmax[0];
        }
     }
   maxfaceverts = 0;
   if (ncells > 0)
     {
      cellselectflag = (char *)malloc(ncells*sizeof(char));
      if (cellselectflag == NULL) memerr();
      cellsubsetflag = (char *)malloc(ncells*sizeof(char));
      if (cellsubsetflag == NULL) memerr();
      for (i = 0; i < ncells; i++)
        {
         cellselectflag[i] = 1;
         cellsubsetflag[i] = 1;
        }
      for (i = 0; i <= MAXFLAGS; i++)
        {
         for (j = 0; j < MAXMATS; j++)
            cellselflag[i][j] = 1;
         if (i < MAXFLAGS) cellandorflag[i] = 0;
        }
      cellselmat_on = 0;  cellselcfld_on = 0;  cellselnfld_on = 0;
      cellselsph_on = 0;  cellselnum_on = 0;
      if (cnumvars > 0)
        {
         cellselfield = 0;
         cfldselmin = cfieldmin[0];
         cfldselmax = cfieldmax[0];
        }
      if (numvars > 0)
        {
         cellselnfield = 0;
         cellselnfldopt = 0;
         cnfldselmin = fieldmin[0];
         cnfldselmax = fieldmax[0];
        }

      /*  Determine maximum number of cell face vertices.  */
      if (structuredflag == 1) maxfaceverts = 6;
      else
        {
         for (i = 0; i < ncellfaces; i++)
           {
            if (cellfaces[i].nverts > maxfaceverts) 
               maxfaceverts = cellfaces[i].nverts;
           }
        }
     }

   /*  Calculate field min and max (variable data only).  */
   field=(double *)malloc((nnodes+1)*sizeof(double)); 
   if (field == NULL) memerr();  
   for (l = 0; l < numvars; l++)
     {

      if (nsubvarsin[l] == 0)
        {

         /*  Initialize node field min and max and get data. */
         fieldmin[l] = MAXVAL;
         fieldmax[l] = -MAXVAL;
         fillnodevalue(l, field);

         for (i = 1; i < nnodes+1; i++)
           {
            if (imat[i] > 0)
              {
               fieldmin[l] = (field[i] < fieldmin[l]) ? 
                                field[i] : fieldmin[l];
               fieldmax[l] = (field[i] > fieldmax[l]) ? 
                                field[i] : fieldmax[l];
              }
           }
        }
     }
   free(field);

   for (l = 0; l < numvars; l++)
     {
      globfieldmin[l] = fieldmin[l];
      globfieldmax[l] = fieldmax[l];
      if (globfieldmax[l] <= globfieldmin[l])
        { 
         f = fabs(globfieldmin[l]);
         if (f == 0.) f = 1.;
         globfieldmax[l] = globfieldmin[l] + f*0.00001;
         fieldmax[l] = globfieldmax[l];
        }
      nfld_nice_min[l] = globfieldmin[l];   
      nfld_nice_max[l] = globfieldmax[l];   
     }

   /*  Calculate cell field min and max (variable data only). */
   if (cnumvars > 0)
     {
      field=(double *)malloc((ncells)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (l = 0; l < cfldcalcbeg; l++)
        {

         if (csubvarsin[l] == 0)
           {

            /*  Initialize cell field min and max and get data. */
            cfieldmin[l] = MAXVAL;
            cfieldmax[l] = -MAXVAL;
            fillcellvalue(l, field);

            for (i = 0; i < ncells; i++)
              {
               if (cellmats[i] > 0)
                 {
                  cfieldmin[l] = (field[i] < cfieldmin[l]) ? 
                                   field[i] : cfieldmin[l];
                  cfieldmax[l] = (field[i] > cfieldmax[l]) ? 
                                   field[i] : cfieldmax[l];
                 }
              }
           }
        }
      free(field);

      for (l = 0; l < cfldcalcbeg; l++)
        {
         globfieldcmin[l] = cfieldmin[l];
         globfieldcmax[l] = cfieldmax[l];
         if (globfieldcmax[l] <= globfieldcmin[l])
           { 
            f = fabs(globfieldcmin[l]);
            if (f == 0.) f = 1.;
            globfieldcmax[l] = globfieldcmin[l] + f*0.00001;
            cfieldmax[l] = globfieldcmax[l];
           }
         cfld_nice_min[l] = globfieldcmin[l];   
         cfld_nice_max[l] = globfieldcmax[l];   
        }
     }

   /*  Calculate face field min and max (variable data only). */
   if (fnumvars > 0)
     {
      field=(double *)malloc((ncellfaces)*sizeof(double)); 
      if (field == NULL) memerr();
      numfaces = ncellfaces;
      if (mesh2dflag > 0) numfaces = ncell2dfaces; 
      for (l = 0; l < fnumvars; l++)
        {

         if (fsubvarsin[l] == 0)
           {

            /*  Initialize face field min and max and get data. */
            ffieldmin[l] = MAXVAL;
            ffieldmax[l] = -MAXVAL;
            fillfacevalue(l, field);

            for (i = 0; i < numfaces; i++)
              {
               ffieldmin[l] = (field[i] < ffieldmin[l]) ? 
                                field[i] : ffieldmin[l];
               ffieldmax[l] = (field[i] > ffieldmax[l]) ? 
                                field[i] : ffieldmax[l];
              }
           }
        }
      free(field);

      for (l = 0; l < fnumvars; l++)
        {
         globfieldfmin[l] = ffieldmin[l];
         globfieldfmax[l] = ffieldmax[l];
         if (globfieldfmax[l] <= globfieldfmin[l])
           { 
            f = fabs(globfieldfmin[l]);
            if (f == 0.) f = 1.;
            globfieldfmax[l] = globfieldfmin[l] + f*0.00001;
            ffieldmax[l] = globfieldfmax[l];
           }
         ffld_nice_min[l] = globfieldfmin[l];   
         ffld_nice_max[l] = globfieldfmax[l];   
        }

      /*  Generate face groups if SLIP variable exists.  */
      ii = -1;
      for (l = 0; l < fnumvars; l++)
         if (strncmp(ffieldname[l],"SLIP",4) == 0) ii = l;
      if (ii > -1)
        {
         field=(double *)malloc((ncellfaces)*sizeof(double)); 
         if (field == NULL) memerr();
         fillfacevalue(ii, field);
         for (i = 1; i < ffieldmax[ii]+1; i++)
           {
            m = 0;
            for (j = 0; j < ncellfaces; j++)
              if (field[j] == i) m++;
            if (m > 0)
              {

               /*  Save face group name.  */
               facegrpname[nfacegrps] = (char *)malloc(33*sizeof(char));
               if (facegrpname[nfacegrps] == NULL) memerr();
               sprintf(facegrpname[nfacegrps],"SLIP%d",i);

               /*  Save the number and data for this face group.  */
               numfacegrp[nfacegrps] = m;

               facegrps[nfacegrps] = (int *)malloc(m*sizeof(int));
               if (facegrps[nfacegrps] == NULL) memerr();
               jj =0;
               for (j = 0; j < ncellfaces; j++)
                 {
                  if (field[j] == i)
                    {
                     facegrps[nfacegrps][jj] = j + 1;
                     jj++;
                    }
                 }
               nfacegrps++;
              }
           }
         free(field);
        }
     }

   /*  If ghost cells exist, generate cell ghost flag.  */
   if (cellghostflag)
     {
      flagname_cell[numflags_cell] = (char *)malloc(33*sizeof(char));
      if (flagname_cell[numflags_cell] == NULL) memerr();
      strcpy(flagname_cell[numflags_cell],"GHOST");
      flagtypes_cell[numflags_cell] = 2;

      flagnames_cell[numflags_cell][0] = (char *)malloc(33*sizeof(char));
      strcpy(flagnames_cell[numflags_cell][0],"No");
      flagnames_cell[numflags_cell][1] = (char *)malloc(33*sizeof(char));
      strcpy(flagnames_cell[numflags_cell][1],"Yes");

      cellflags[numflags_cell] = (short *)malloc((ncells)*sizeof(short));
      if (cellflags[numflags_cell] == NULL) memerr();
      for (i = 0; i < ncells; i++) cellflags[numflags_cell][i] = 1;
      for (i = 0; i < ncellghosts; i++)
        {
         l = cellghosts[i];
         cellflags[numflags_cell][l] = 2;

         /*  Deselect ghost cells.  */
         cellselectflag[l] = 0;
        }
      ghostcellflagid = numflags_cell;
      numflags_cell++;
     }

   /*  If ghost cells entered, but not ghost nodes, determine ghost nodes.  */
   if (cellghostflag && !nodeghostflag)
     {
      gnodechk = (short *)calloc((nnodes+1),sizeof(short));

      /*  First, set the nodes in ghost cells to 1.  */
      for (i = 0; i < ncellghosts; i++)
        {
         l = cellghosts[i];
         for (j = 0; j < cells[l].ncnodes; j++)
           {
            m = cells[l].cnodes[j];
            gnodechk[m] = 1;
           }
        }

      /*  Now set nodes in non-ghost cells to 2.  */
      for (i = 0; i < ncells; i++)
        {
         if (cellflags[ghostcellflagid][i] == 1)
           {
            for (j = 0; j < cells[i].ncnodes; j++)
              {
               m = cells[i].cnodes[j];
               gnodechk[m] = 2;
              }
           }
        }

      /*  Count the nodes with a 1 value and set as ghost nodes.  */
      nnodeghosts = 0;
      for (i = 1; i < nnodes+1; i++)
        {
         if (gnodechk[i] == 1) nnodeghosts++;
        }
      if (nnodeghosts > 0)
        {
         nodeghostflag = 1;
         nodeghosts = (int *)malloc(nnodeghosts*sizeof(int));
         if (nodeghosts == NULL) memerr();
         k = 0;
         for (i = 1; i < nnodes+1; i++)
           {
            if (gnodechk[i] == 1)
              {
               nodeghosts[k] = i;
               k++;
              }
           }
        }
      free(gnodechk);
     }

   /*  If ghost nodes exist, generate node ghost flag.  */
   if (nodeghostflag)
     {
      flagname_node[numflags_node] = (char *)malloc(33*sizeof(char));
      if (flagname_node[numflags_node] == NULL) memerr();
      strcpy(flagname_node[numflags_node],"GHOST");
      flagtypes_node[numflags_node] = 2;

      flagnames_node[numflags_node][0] = (char *)malloc(33*sizeof(char));
      strcpy(flagnames_node[numflags_node][0],"No");
      flagnames_node[numflags_node][1] = (char *)malloc(33*sizeof(char));
      strcpy(flagnames_node[numflags_node][1],"Yes");

      flags[numflags_node] = (short *)malloc((nnodes+1)*sizeof(short));
      if (flags[numflags_node] == NULL) memerr();
      for (i = 1; i < nnodes+1; i++) flags[numflags_node][i] = 1;
      for (i = 0; i < nnodeghosts; i++)
        {
         l = nodeghosts[i];
         flags[numflags_node][l] = 2;

         /*  Deselect ghost nodes.  */
         nodeselectflag[l] = 0;
        }
      numflags_node++;
     }

   /*  If cell ghosts exist, reset the cell center to ghost node.  */
   if (cellghostflag)
     {
      for (i = 0; i < ncellghosts; i++)
        {
         l = cellghosts[i];
         k = 0;
         for (j = 0; j < cells[l].ncnodes; j++)
           {
            m = cells[l].cnodes[j];
            if (flags[numflags_node-1][m] == 2)
              {
               ii = m;
               k++;
              }
            if (k == 1)
              {
               cells[l].xavg = nodes[ii].x;
               cells[l].yavg = nodes[ii].y;
               cells[l].zavg = nodes[ii].z;
              }
           }
        }
     }

   /*  Set material data.  */
   for (j = 0; j < MAXMATS; j++)
     {
      mat_has_active_polys[j] = 0;
      active_mat[j] = 0;
      mat_has_active_pts[j] = 0;
      isomats_active[j] = 0;
     }
   for (j = 0; j < mmats; j++)
     {
      if (npolyspermat[j] > 0) mat_has_active_polys[j] = 1;
      if (mat_has_active_polys[j] > 0) active_mat[j] = 1;
     }

   /*  Set active materials/flags by node. */
   for (i = 1; i < nnodes+1; i++)
     {

      /*  Materials.  */
      m = (int) imat[i] - 1;
      if (m >= 0)
        {
         isomats_active[m] = 1;
         mat_has_active_pts[m] = 1;
        }

      /*  Flags.  */
      for (j = 0; j < numflags_node; j++)
        {
         for (k = 0; k < flagtypes_node[j]; k++)
           {
            m = (int)flags[j][i] - 1;
            if (m >= 0) mat_has_active_pts[m] = 1;
           }
        }
     }

   /*  Set active materials/flags by cell. */
   for (i = 0; i < ncells; i++)
     {

      /*  Materials.  */
      m = (int) cellmats[i] - 1;
      if (m >= 0)
        {
         isomats_active[m] = 1;
         mat_has_active_pts[m] = 1;
        }

      /*  Flags.  */
      for (j = 0; j < numflags_cell; j++)
        {
         for (k = 0; k < flagtypes_cell[j]; k++)
           {
            m = (int)cellflags[j][i] - 1;
            if (m >= 0) mat_has_active_pts[m] = 1;
           }
        }
     }

   /*  Check that the mesh is all tets for voronoi calculation.  */
   alltetmesh = 1;
   if (structuredflag) alltetmesh = 0;
   else
     {
      for (i = 0; i < ncells; i++)
        {
         if (cells[i].nfaces != 4 || cells[i].ncnodes != 4) alltetmesh = 0;
         if (alltetmesh == 0) break;
        }
     }

   /*  If at least one field exists, make trace field calc. fields.  */
   tfldcalcbeg = numvartr;
   if (numvartr > 0 || physedflag)
     {

      for (i = 0; i < MAXFLDCALC; i++)
        {
         tfieldcalc[i].xfldno = -1;
         tfieldcalc[i].yfldno = -1;
         tfieldcalc[i].oper = -1;
         tfieldcalc[i].cval = 0.;
         sprintf(tfieldcalc[i].name,"FldCalc%d",i+1);

         if (numvartr < MAXTRFIELDS)
           {
            FREE(tracers[0].field[numvartr]);
            tracers[0].field[numvartr] = (double *)malloc((ntracers)*sizeof(double)); 
            if (tracers[0].field[numvartr] == NULL) memerr();  

            for (j = 0; j < ntracers; j++) tracers[0].field[numvartr][j] = 0;
	    
            fieldtrname[numvartr] = (char *)malloc(33*sizeof(char));
            if (fieldtrname[numvartr] == NULL) memerr();
            strcpy(fieldtrname[numvartr],tfieldcalc[i].name);

            numvartr++;
           }
        } 
     }  

   /*  Get tracer fields mins and maxes.  */
   for (l = 0; l < numvartr; l++)
     {
      fieldtrmin[l] = MAXVAL;
      fieldtrmax[l] = -MAXVAL;
      for (i = 0; i < ntracers; i++)
        {
         fval = tracers[0].field[l][i];
         fieldtrmin[l] = (fieldtrmin[l] < fval) ? 
                          fieldtrmin[l] : fval;
         fieldtrmax[l] = (fieldtrmax[l] > fval) ? 
                          fieldtrmax[l] : fval;
        }        
     }
   for (l = 0; l < numvartr; l++)
     {
      globfieldtrmin[l] = fieldtrmin[l];
      globfieldtrmax[l] = fieldtrmax[l];
      if (globfieldtrmax[l] <= globfieldtrmin[l])
        {
         f = fabs(globfieldtrmin[l]);
         if (f == 0.) f = 1.;
         globfieldtrmax[l] = globfieldtrmin[l] + f*0.00001;
         fieldtrmax[l] = globfieldtrmax[l];
        }
      trfld_nice_min[l] = globfieldtrmin[l];   
      trfld_nice_max[l] = globfieldtrmax[l];
      trfldselmin[l] = fieldtrmin[l];
      trfldselmax[l] = fieldtrmax[l];
     }

   /*  Process surface data.  */
   if (nsurf > 0)
     {

      /*  If no surface materials entered, allocate and set to one.  */
      if (surfmats == NULL) maxsurfmat = 0;
      if (maxsurfmat == 0)
        {
         surfmats=(short *)malloc((nsurf)*sizeof(short));
         if (surfmats == NULL) memerr();
         for (i = 0; i < nsurf; i++) surfmats[i] = 1;
        }

      /*  If surface material nos. > mmats, set mat names.  */
      maxsurfmat = 0;
      for (i = 0; i < nsurf; i++)
         if (surfmats[i] > maxsurfmat) maxsurfmat = surfmats[i];
      if (maxsurfmat > mmats)
        {
         for (j = mmats; j < maxsurfmat; j++)
           {
            sprintf(mmatnames[j],"mat %d",j+1);
           }
        }

      /*  Get surface center(avg) and normal.  */
      for (i = 0; i < nsurf; i++)
        {
         xsum = ysum = zsum = 0;
         for (j = 0; j < surfs[i].nverts; j++)
           {
            m = surfs[i].fverts[j];
            xsum += nodes[m].x;
            ysum += nodes[m].y;
            zsum += nodes[m].z;
           }
         surfs[i].xavg = xsum / (float)surfs[i].nverts;
         surfs[i].yavg = ysum / (float)surfs[i].nverts;
         surfs[i].zavg = zsum / (float)surfs[i].nverts;

         /*  Calculate surface normal.  */
         x1 = surfs[i].xavg;
         y1 = surfs[i].yavg;
         z1 = surfs[i].zavg;
         k = surfs[i].fverts[0];
         x2 = nodes[k].x;  y2 = nodes[k].y;  z2 = nodes[k].z;
         k = surfs[i].fverts[1];
         x3 = nodes[k].x;  y3 = nodes[k].y;  z3 = nodes[k].z;
         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         t = sqrt(xno*xno + yno*yno + zno*zno);

         surfs[i].xnorm = xno/t;
         surfs[i].ynorm = yno/t;
         surfs[i].znorm = zno/t;
        }

      /*  Save surface velocity components as  */ 
      /*  field data with the names U, V, W.   */
      if (svectors == 1 && (snumvars+3) < MAXFIELDS)
        {
         sfieldname[snumvars] = (char *)malloc(33*sizeof(char));
         if (sfieldname[snumvars] == NULL) memerr();
         strcpy(sfieldname[snumvars],"U       ");
         writesurffld(suic);
         svectfldx = snumvars;
         snumvars++;

         sfieldname[snumvars] = (char *)malloc(33*sizeof(char));
         if (sfieldname[snumvars] == NULL) memerr();
         strcpy(sfieldname[snumvars],"V       ");
         writesurffld(svic);
         svectfldy = snumvars;
         snumvars++;

         sfieldname[snumvars] = (char *)malloc(33*sizeof(char));
         if (sfieldname[snumvars] == NULL) memerr();
         strcpy(sfieldname[snumvars],"W       ");
         writesurffld(swic);
         svectfldz = snumvars;
         snumvars++;
        }

      /*  If more than one field exists, make surface speed field.  */
      if ((snumvars > 1 || physedflag) && snumvars < MAXFIELDS)
        {
         sfieldname[snumvars] = (char *)malloc(33*sizeof(char));
         if (sfieldname[snumvars] == NULL) memerr();
         strcpy(sfieldname[snumvars],"Vect Mag");

         field=(double *)malloc((nsurf)*sizeof(double)); 
         if (field == NULL) memerr();  
         for (i = 0; i < nsurf; i++) field[i] = 0;

         /*  If velocities present, calc. speed and get max.  */
         smaxspeed = 0;
         if (svectors > 0)
           {
            for (i = 0; i < nsurf; i++)
              {
               field[i] = sqrt((suic[i] * suic[i]) + 
                               (svic[i] * svic[i]) +
                               (swic[i] * swic[i]));
               smaxspeed = (field[i] > smaxspeed) ? 
                            field[i] : smaxspeed;
              }
            FREE(suic);  FREE(svic);  FREE(swic);
           } 
     
         writesurffld(field);
         sspeedfld = snumvars;
         snumvars++;
         free(field); 
        }      

      /*  If at least one field exists, make surface field calc. fields.  */
      sfldcalcbeg = snumvars;
      if (snumvars > 0 || physedflag)
        {
         field=(double *)malloc((nsurf)*sizeof(double)); 
         if (field == NULL) memerr();  

         if (physedflag) sfldcalc_has_data = 1;

         for (i = 0; i < nsurf; i++) field[i] = 0;

         for (i = 0; i < MAXFLDCALC; i++)
           {
            sfieldcalc[i].xfldno = -1;
            sfieldcalc[i].yfldno = -1;
            sfieldcalc[i].oper = -1;
            sfieldcalc[i].cval = 0.;
            sprintf(sfieldcalc[i].name,"FldCalc%d",i+1);

            if (snumvars < MAXFIELDS)
              {
               sfieldname[snumvars] = (char *)malloc(33*sizeof(char));
               if (sfieldname[snumvars] == NULL) memerr();
               strcpy(sfieldname[snumvars],sfieldcalc[i].name);

               if (physedflag) writesurffld(field);

               snumvars++;
              }
           } 
     
         free(field); 
        }

      /*  Allocate and set surface selection flags.  */
      surfselectflag = (char *)malloc(nsurf*sizeof(char));
      if (surfselectflag == NULL) memerr();
      surfsubsetflag = (char *)malloc(nsurf*sizeof(char));
      if (surfsubsetflag == NULL) memerr();
      for (i = 0; i < nsurf; i++)
        {
         surfselectflag[i] = 1;
         surfsubsetflag[i] = 1;
        }
      for (i = 0; i <= MAXFLAGS; i++)
        {
         for (j = 0; j < MAXMATS; j++)
            surfselflag[i][j] = 1;
         if (i < MAXFLAGS) surfandorflag[i] = 0;
        }
      surfselmat_on = 0;  surfselsfld_on = 0;  surfselnfld_on = 0;
      surfselsph_on = 0;  surfselnum_on = 0;
      if (snumvars > 0)
        {
         surfselfield = 0;
         sfldselmin = sfieldmin[0];
         sfldselmax = sfieldmax[0];
        }
      if (numvars > 0)
        {
         surfselnfield = 0;
         surfselnfldopt = 0;
         snfldselmin = fieldmin[0];
         snfldselmax = fieldmax[0];
        }
     
      /*  Initialize surface field min and max. */
      for (i = 0; i < sfldcalcbeg; i++)
        {
         sfieldmin[i] = MAXVAL;
         sfieldmax[i] = -MAXVAL;
        }
      field=(double *)malloc((nsurf)*sizeof(double)); 
      if (field == NULL) memerr();  
      for (l = 0; l < sfldcalcbeg; l++)
        {
         fillsurfvalue(l, field);

         for (i = 0; i < nsurf; i++)
           {
            if (surfmats[i] > 0)
              {
               sfieldmin[l] = (field[i] < sfieldmin[l]) ? 
                                field[i] : sfieldmin[l];
               sfieldmax[l] = (field[i] > sfieldmax[l]) ? 
                                field[i] : sfieldmax[l];
              }
           }
        }
      free(field);

      for (l = 0; l < sfldcalcbeg; l++)
        {
         globfieldsmin[l] = sfieldmin[l];
         globfieldsmax[l] = sfieldmax[l];
         if (globfieldsmax[l] <= globfieldsmin[l])
           { 
            f = fabs(globfieldsmin[l]);
            if (f == 0.) f = 1.;
            globfieldsmax[l] = globfieldsmin[l] + f*0.00001;
            sfieldmax[l] = globfieldsmax[l];
           }
         sfld_nice_min[l] = globfieldsmin[l];   
         sfld_nice_max[l] = globfieldsmax[l];   
        }

      /*  Set active materials by surface. */
      for (i = 0; i < nsurf; i++)
        {

         /*  Materials.  */
         m = (int) surfmats[i] - 1;
         if (m >= 0)
            mat_has_active_pts[m] = 1;

         /*  Flags.  */
         for (j = 0; j < numflags_surf; j++)
           {
            for (k = 0; k < flagtypes_surf[j]; k++)
              {
               m = (int)surfflags[j][i] - 1;
               if (m >= 0) mat_has_active_pts[m] = 1;
              }
           }
        }
     }


   /*  Set display limits.  */
   xrange = globxicmax - globxicmin;
   yrange = globyicmax - globyicmin;
   zrange = globzicmax - globzicmin;
   span = sqrt(xrange*xrange + yrange*yrange + zrange*zrange);
   globxicmino = globxicmin; globxicmaxo = globxicmax;
   globyicmino = globyicmin; globyicmaxo = globyicmax;
   globzicmino = globzicmin; globzicmaxo = globzicmax;
   probxicmin = globxicmin; probxicmax = globxicmax;
   probyicmin = globyicmin; probyicmax = globyicmax;
   probzicmin = globzicmin; probzicmax = globzicmax;

   near_clip = 1000.0*span;
   far_clip = -0.1*span;

   xtrans = -0.5*(globxicmax+globxicmin);
   ytrans = -0.5*(globyicmax+globyicmin);
   ztrans = -0.5*(globzicmax+globzicmin);

   /*  Caclulate center of mass vectors for each material.  */
   /*  These vectors are used for explode directions.       */

   if (polysin == 1) matexpvects();


   /*  Initialize material order.  */

   nmat_order = 0;
   for (i = 0; i < MAXMATS; i++)
      mat_order[i] = 0;
   for (i = 0; i < MAXMATS; i++)
     {
      animfadematflag[i] = 0;
      if (mat_has_active_polys[i] > 0)
        {
         mat_order[nmat_order] = i;
         nmat_order++;
        }
     }

   /*  Initialize animation fade contour flag.  */

   for (i = 0; i < MAXCONTOURS; i++) animfadecontflag[i] = 0;

   /*  Initialize values for subsets.  */
   
   xminfdval = globxicmin; xmaxfdval = globxicmax;
   yminfdval = globyicmin; ymaxfdval = globyicmax;
   zminfdval = globzicmin; zmaxfdval = globzicmax;

   xminbdval = globxicmin; xmaxbdval = globxicmax;
   yminbdval = globyicmin; ymaxbdval = globyicmax;
   zminbdval = globzicmin; zmaxbdval = globzicmax;

   /*  Set field limits auto scale flags off.  */
   for (i = 0; i < MAXFIELDS; i++)
     {
      nfieldlimauto[i] = 0;
      cfieldlimauto[i] = 0;
      sfieldlimauto[i] = 0;
      ffieldlimauto[i] = 0;
     }

   /*  Start up with cell faces and edges showing.  */
   if (ncells > 0)
     {
      cellsflag = 1;
      celledgesflag = 0;     
      if (cellghostflag) drawghostcelloptions();
      setfacesel();
     }
   else if (nnodes > 1) nodesflag = 1;

   /*  If no cells or nodes, start with polygons.  */
   if (polysin == 1 && cellsflag == 0 && nodesflag == 0) polysflag = 1;
}


void matexpvects()

{
  int i, j, k, count[MAXMATS], nverts, tcount, polysactive;
  long nv;
  double xt, yt, zt, xsum[MAXMATS], ysum[MAXMATS], zsum[MAXMATS];
  double xav, yav, zav, adj;
  int numpaths = 16, ip, kp;
  int path[16][3] = { {1,0,1}, {0,1,1}, {-1,0,1}, {0,-1,1},
                      {1,0,-1}, {0,1,-1}, {-1,0,-1}, {0,-1,-1},
                      {1,1,1}, {1,-1,1}, {-1,1,1}, {-1,-1,1},
                      {1,1,-1}, {1,-1,-1}, {-1,1,-1}, {-1,-1,-1} };

   /*  Check to see if polygons are available.  */
   polysactive = 0;
   for (i = 0; i < mmats; i++)
     {
      if (polygons[i] != NULL) polysactive = 1;
     }

   /*  If polygons are not available, get them.  */
   if (!polysactive) readpolyfile();

   /*  Loop through materials and sum x,y,z's  */  
   /*  of the average vertex of each polygon.  */
   xt = 0;  yt = 0;  zt = 0;
   tcount = 0;
   for (i = 0; i < mmats; i++)
     {
      count[i] = 0;  nv = 0;
      xsum[i] = 0;  ysum[i] = 0;  zsum[i] = 0;
      for (j = 0; j < npolyspermat[i]; j++)
        {
         xav = 0;  yav = 0;  zav = 0;
         nverts = (int)polygons[i][j].nverts;
         nv = polygons[i][j].polyvertloc;
         for (k = 0; k < nverts; k++)
           {
            xav += polyverts[i][k+nv].x;
            yav += polyverts[i][k+nv].y;
            zav += polyverts[i][k+nv].z;
           }         

         xav /= (float)nverts;
         yav /= (float)nverts;
         zav /= (float)nverts;
         xsum[i] += xav;
         ysum[i] += yav;
         zsum[i] += zav;
         count[i]++;
         xt += xav;
         yt += yav;
         zt += zav;
         tcount++;
        }
     }

   /*  Get average x,y,z for all materials.  */
   if (tcount > 0)
     {
      xt /= (float)tcount;
      yt /= (float)tcount;
      zt /= (float)tcount;
     }
   if (xreflect == 1) xt = 0;
   if (yreflect == 1) yt = 0;
   if (zreflect == 1) zt = 0;

   /*  Get average x,y,z for each material     */
   /*  and generate material average vectors.  */
   for (i = 0; i < mmats; i++)
     {
      if (count[i] > 0)
        {
         xsum[i] /= (float)count[i];
         ysum[i] /= (float)count[i];
         zsum[i] /= (float)count[i];
        }
      matexpx[i] = xsum[i] - xt;
      matexpy[i] = ysum[i] - yt;
      matexpz[i] = zsum[i] - zt;
     }

   /*  Look for duplicates, and adjust the vector.  */
   adj = span * 0.05;
   tcount = 0;
   for (i = 1; i < mmats; i++)
     {
      for (j = 0;  j < i; j++)
        {
         if (matexpx[i] == matexpx[j] && 
             matexpy[i] == matexpy[j] && 
             matexpz[i] == matexpz[j])
           {
            ip = tcount % numpaths;
            kp = tcount / numpaths + 1;
            matexpx[i] += adj * kp * path[ip][0];
            matexpy[i] += adj * kp * path[ip][1];
            matexpz[i] += adj * kp * path[ip][2];
            tcount++;
           }
        } 
     }

   /*  Free polygons if not active.  */
   if (!polysactive)
     {
      for (i = 0; i < MAXMATS; i++)
        {
         FREE(polygons[i]);
         FREE(polyverts[i]);
        }
     }
}


void savenodecoords()
{
  int i;
  double *vfield;
  void writenodefld(double *value);

   /*  Save nodes coordinates as node variables.  */
   vfield=(double *)malloc((nnodes+1)*sizeof(double));
   if (vfield == NULL) memerr();
   vfield[0] = 0.;

   /*  X coordinates.  */
   fieldname[numvars]=(char *)malloc(33*sizeof(char));
   if (fieldname[numvars] == NULL) memerr();
   strncpy(fieldname[numvars],"X-Coords",8);
   *(fieldname[numvars]+8) = (char) 0;
   nvars_to_write[numvars] = 0;

   for (i = 1; i < nnodes+1; i++)
      vfield[i] = nodes[i].x;
   writenodefld(vfield);
   nsubvarsin[numvars] = 0;
   numvars++;

   /*  Y coordinates.  */
   fieldname[numvars]=(char *)malloc(33*sizeof(char));
   if (fieldname[numvars] == NULL) memerr();
   strncpy(fieldname[numvars],"Y-Coords",8);
   *(fieldname[numvars]+8) = (char) 0;
   nvars_to_write[numvars] = 0;

   for (i = 1; i < nnodes+1; i++)
      vfield[i] = nodes[i].y;
   writenodefld(vfield);
   nsubvarsin[numvars] = 0;
   numvars++;

   /*  Z coordinates.  */
   fieldname[numvars]=(char *)malloc(33*sizeof(char));
   if (fieldname[numvars] == NULL) memerr();
   strncpy(fieldname[numvars],"Z-Coords",8);
   *(fieldname[numvars]+8) = (char) 0;
   nvars_to_write[numvars] = 0;

   for (i = 1; i < nnodes+1; i++)
      vfield[i] = nodes[i].z;
   writenodefld(vfield);
   nsubvarsin[numvars] = 0;
   numvars++;

   free(vfield);
}
