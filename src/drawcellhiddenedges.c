#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "gmvdata.h"

static double tempf, *field, fld_min, fld_max;
static int fieldactive, fldflag, cactive, allvars, factive;
static char logflag;
static short matnum, *matcolr;
void chfsetfldcolr(double fldval, double fldmin, double fldmax, double tempval, 
                  char logflag, int *icl);

struct cellface *strcellfaces;
struct cellface *chkcellfaces;


void drawcellhiddenedges()
{
 
  /*  This routine generates triangles and lines  */
  /*  for cell hidden line edges                  */

  int i, j, icell, jcell, iface;
  void gethiddenfaces();
  void gethiddenedges();
 
   fieldactive = cellfieldactive;
   field = cellfield;

   allvars = numvars + cnumvars + fnumvars;
   cactive = fieldactive - numvars;
   factive = fieldactive - (numvars + cnumvars);
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

   if (factive > -1 && factive < fnumvars)
     {
      fldflag = 3;
      logflag = ffld_log[factive];
      fld_min = ffld_nice_min[factive];
      fld_max = ffld_nice_max[factive];
     }

   if (fldflag > 0)
     {
      tempf = fld_max - fld_min;
      if (logflag) tempf = log10(fld_max) - log10(fld_min);
     }

   /*  Set to material color or group color.  */
   if (fieldactive == -1) matcolr = cellmats;
   if (fieldactive == -2) matcolr = cellgrpcolr;

   /*  Set fldflag for face group color.  */
   if (fieldactive == -3) fldflag = 4;

   /*  Set fldflag for hidden line text color.  */
   if (fieldactive == -4) fldflag = -1;

   /*  If structured mesh, build cellfaces.  */
   if (structuredflag)
     {
      strcellfaces = (struct cellface *)
                     malloc(ncellfaces*sizeof(struct cellface));

      if (strcellfaces == NULL)
        {
         printf("Warning, not enough memory for hidden edges.\n");
         return;
        }

      for (icell = 0; icell < ncells; icell++)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }
         for (i = 0; i < cells[jcell].nfaces; i++)
           {
            iface = cells[jcell].facenos[i];
            strcellfaces[iface].nverts = cellfaces[i].nverts;
            strcellfaces[iface].cell1 = cellfaces[i].cell1;
            strcellfaces[iface].cell2 = cellfaces[i].nverts;
            strcellfaces[iface].oppface = cellfaces[i].oppface;
            strcellfaces[iface].fverts = (int *)malloc(4*sizeof(int));
            for (j = 0; j < 4; j++)
               strcellfaces[iface].fverts[j] = cellfaces[i].fverts[j];
           }
        }
     }

   if (structuredflag) chkcellfaces = strcellfaces;
   else chkcellfaces = cellfaces;
 
   /*  Get selected faces of selected cells.  */
   gethiddenfaces();
 
   /*  Get edges of selected faces of selected cells.  */
   gethiddenedges();

   if (structuredflag) free(strcellfaces);
}


void gethiddenfaces()
{
 
  /*  This gets the exterior faces of the mesh (opposite face < 0).  */
 
  int i, j, k, iface, nfverts, itype, icol;
  double v1[3], v2[3], v3[3];
  float norm[3];


   /*  Set color to text color.  */
   if (txtcolor < 256) icol = -1;
      else icol = -2;

   /*  Loop over the faces.  */
   itype = UNLIT_FLAT;
   for (iface = 0; iface < ncellfaces; iface++)
     {

      /*  Select this face if it has no opposite face.  */
      if (chkcellfaces[iface].oppface < 0)
        {

         nfverts = chkcellfaces[iface].nverts;

         /*  Begin face vertices, allocate drawarray3.  */
         j = nfverts * 2;
         if (numtris3+j > drawarray3_alloc)
           {
            drawarray3_alloc += 10000;
            drawarray3 = (struct trielem *)realloc(drawarray3,
                         drawarray3_alloc*sizeof(struct trielem));
           }

         norm[1] = norm[2] = norm[3] = 0.;

         k = chkcellfaces[iface].fverts[0];
         v1[0] = nodes[k].x;
         v1[1] = nodes[k].y;
         v1[2] = nodes[k].z;

         for (j = 1; j < nfverts-1; j++)
           {
            k = chkcellfaces[iface].fverts[j];
            v2[0] = nodes[k].x;
            v2[1] = nodes[k].y;
            v2[2] = nodes[k].z;

            k = chkcellfaces[iface].fverts[j+1];
            v3[0] = nodes[k].x;
            v3[1] = nodes[k].y;
            v3[2] = nodes[k].z;

            drawarray3[numtris3].itype = itype;
            drawarray3[numtris3].x1 = v1[0];
            drawarray3[numtris3].y1 = v1[1];
            drawarray3[numtris3].z1 = v1[2];
            drawarray3[numtris3].x2 = v2[0];
            drawarray3[numtris3].y2 = v2[1];
            drawarray3[numtris3].z2 = v2[2];
            drawarray3[numtris3].x3 = v3[0];
            drawarray3[numtris3].y3 = v3[1];
            drawarray3[numtris3].z3 = v3[2];
            drawarray3[numtris3].xnorm = norm[0];
            drawarray3[numtris3].ynorm = norm[1];
            drawarray3[numtris3].znorm = norm[2];
            drawarray3[numtris3].iclr1 = icol;
            drawarray3[numtris3].iclr2 = icol;
            drawarray3[numtris3].iclr3 = icol;
            numtris3++;
           }
        }
     }
}


void gethiddenedges()
{
 
  /*  This routine gets the outside faces edge lines.  */
 
  int j, icell, iface, nfverts, ic1, ic2, itype;
  int ipt1, ipt2;

   /*  Set up Gouraud shading for field data.  */
   itype = LSOLID;
   if (fldflag == 1)
      itype = LSMOOTH;

   /*  Loop through faces.  */
   for (iface = 0; iface < ncellfaces; iface++)
     {

      /*  Select face if no opposite face.  */
      if (chkcellfaces[iface].oppface < 0)
        {

         /*  Get primary cell for this face.  */
         icell = chkcellfaces[iface].cell1;

         /*  Set material/flag color for this cell.  */
         if (fldflag == 0)
           {
            matnum = matcolr[icell] - 1;
            if (fieldactive >= allvars)
              matnum = cellflags[fieldactive-allvars][icell] - 1;
            ic1 = matnum;
            if (ic1 < 0 || ic1 > MAXMATS)
              {
               if (txtcolor < 256) ic1 = -1;
               else ic1 = -2;
              }
           }
         ic2 = ic1;

         /*  If hidden edges colored in textcolor, set the color.  */
         if (fldflag == -1)
           {
            if (txtcolor < 256) ic1 = -1;
            else ic1 = -2;
           }
         ic2 = ic1;

         /*  If a cell field is to be drawn, reset the color. */
         if (fldflag == 2)
           {
            chfsetfldcolr(field[icell],fld_min,fld_max,tempf,logflag,&ic1);
            ic2 = ic1;
           }

         nfverts = chkcellfaces[iface].nverts;

         /*  Loop over face vertices and draw edges.  */
         j = nfverts + 1;
         if (numplv3+j > plvarray3_alloc)
           {
            plvarray3_alloc += 10000;
            plvarray3 = (struct plvelem *)realloc(plvarray3,
                                 plvarray3_alloc*sizeof(struct plvelem));
           }

         for (j = 0; j < nfverts; j++)
           {
            ipt1 = chkcellfaces[iface].fverts[j];
            if (j < nfverts-1) 
               ipt2 = chkcellfaces[iface].fverts[j+1];
            else
               ipt2 = chkcellfaces[iface].fverts[0];
  
            /*  If a node field is to be drawn,  */ 
            /*  reset the color for each node.   */
            ic2 = ic1;
            if (fldflag == 1)
              {
               cesetfldcolr(field[ipt1],fld_min,fld_max,tempf,
                            logflag,&ic1);
               cesetfldcolr(field[ipt2],fld_min,fld_max,tempf,
                            logflag,&ic2);
              }
            plvarray3[numplv3].itype = itype;
            plvarray3[numplv3].x1 = nodes[ipt1].x;
            plvarray3[numplv3].y1 = nodes[ipt1].y;
            plvarray3[numplv3].z1 = nodes[ipt1].z;
            plvarray3[numplv3].x2 = nodes[ipt2].x;
            plvarray3[numplv3].y2 = nodes[ipt2].y;
            plvarray3[numplv3].z2 = nodes[ipt2].z;
            plvarray3[numplv3].iclr1 = ic1;
            plvarray3[numplv3].iclr2 = ic2;
            numplv3++;
           }
        }
     }
}


void chfsetfldcolr
#include "setfldcolr.h"
