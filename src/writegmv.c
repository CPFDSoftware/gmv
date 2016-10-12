/*  This file, writegmv.c - has been modified from the original
    starting August 26, 1999.  It incorporates new functionality
    for writing out the original mesh information directly from
    the input file rather than writing out mesh information from
    memory in the generalized GMV form.

    Genevieve Barrett  genevieve@lanl.gov
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>

#include "gmvdata.h"
#include "gmvread.h"
#include "formwidgets.h"

void do_nodes(void);
void do_cells(void);
void do_faces(void);
void do_vfaces(void);

int write_binary_flag;

static int newwriteflag = 1;
static int cfirst = 1;
static int ffirst = 1;
static int vfirst = 1;

void writegmv(char *outfilnam)
{
  int i, j, k, itemp, nv, nverts, maxchar;
  int flag=-1;
  int ierr=0, iend=0;
  int    *intdata;
  double *doubledata, *x, *y, *z;
  double *u, *v, *w;

   /*------------------------------------------------------------------------*/
  /* Set flags for state of write. Upon initial enter to writegmv, set to 1. */
  /* During write, the cell, face, and vfaces flags will be set to 0.  The   */
  /* extra newwrite flag allows for global control of those individual flags.*/
   /*------------------------------------------------------------------------*/
  if (newwriteflag)
    {
     cfirst = 1; 
     ffirst = 1; 
     vfirst = 1; 
     newwriteflag = 0;
    }

  if (strcmp(infilnam, outfilnam) == 0)
    {
     printf("ERROR: Current filename and output filename are the same!\n");
     printf("       Cannot write out new data to original input file name.\n");
     printf("       Please make sure filename for output file is different.\n");
     return;
    }

   /*  Determine whether to write 8 or 32 bit character data.  */
   maxchar = 8;
   for (i = 0; i < mmats; i++)
      if (strlen(mmatnames[i]) > maxchar) maxchar = 32;
   if (maxchar == 8)
     {
      for (i = 0; i < numvars; i++)
         if (strlen(fieldname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < cnumvars; i++)
         if (strlen(cfieldname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < snumvars; i++)
         if (strlen(sfieldname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < fnumvars; i++)
         if (strlen(ffieldname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < numvartr; i++)
         if (strlen(fieldtrname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < numvars; i++)
         if (strlen(fieldname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (j = 0; j < numflags_cell; j++)
        {
         if (strlen(flagname_cell[j]) > maxchar) maxchar = 32;
         k = flagtypes_cell[j];
         for (i = 0; i < k; i++)
            if (strlen(flagnames_cell[j][i]) > maxchar) maxchar = 32;
        }
     }
   if (maxchar == 8)
     {
      for (j = 0; j < numflags_node; j++)
        {
         if (strlen(flagname_node[j]) > maxchar) maxchar = 32;
         k = flagtypes_node[j];
         for (i = 0; i < k; i++)
            if (strlen(flagnames_node[j][i]) > maxchar) maxchar = 32;
        }
     }
   if (maxchar == 8)
     {
      for (i = 0; i < ncellgrps; i++)
         if (strlen(cellgrpname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < nnodegrps; i++)
         if (strlen(nodegrpname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < nfacegrps; i++)
         if (strlen(facegrpname[i]) > maxchar) maxchar = 32;
     }
   if (maxchar == 8)
     {
      for (i = 0; i < nsurfacegrps; i++)
         if (strlen(surfacegrpname[i]) > maxchar) maxchar = 32;
     }

   /* Open file for data output */
   if (write_binary_flag)
     {
      if (maxchar == 8)
         gmvwrite_openfile_ir(outfilnam, 4, 8);
      else
         gmvwrite_openfile_cxir(outfilnam, 4, 8);
     }
   else
      gmvwrite_openfile_ir_ascii(outfilnam, 4, 8);

   printf("Opened file for writing:%s\n", outfilnam);

   /*------------------------------------------------------------------------*/
   /* Re-Open original file/get mesh data (anything <= VFACES ), write it out*/
   /* all other information will come from the data stored in GMV memory     */
   /*------------------------------------------------------------------------*/
   ierr = gmvread_open_fromfileskip(infilnam);
 
   if (ierr > 0)
     {
      printf("Error re-opening original input file, %s\n", infilnam);
      gmvexit();
     }
   else
     {
      printf("Re-opened original input file: %s\n"\
             "    to get mesh data.\n",infilnam);
     }

   while (iend == 0)
     {
      
      gmvread_data();
      
      /*  Check for GMVEND.  */
      if (gmv_data.keyword == GMVEND)
        {
         iend = 1;
        }

      /*  Write out the data from the original file if <= VFACES.  */
      if (gmv_data.keyword > VFACES)
        {
         iend = 1;
        /* Finished getting data from original input file, close it. */
         gmvread_close();
        }
      else
        {
         switch (gmv_data.keyword)
           {
            case(NODES):
               do_nodes();
               break;
            case(CELLS):
               do_cells();
               break;
            case(FACES):
               do_faces();
               break;
            case(VFACES):
               do_vfaces();
               break;
           }
        }
     }
   
/*------------------------------------------------------------------------*/
/* Ok, all of the <= VFACES information is written to the new file, now   */
/* write out all the information that is stored in GMV memory             */
/* with the routines from writegmv.                                       */
/*------------------------------------------------------------------------*/

   /*  Write out materials.  */

   if (mats_on_cells > 0)
     {
      printf("Writing cell materials.\n");
      flag = 0;

      gmvwrite_material_header(mmats, flag);

      for (i = 0; i < mmats; i++) 
        {
         gmvwrite_material_name(mmatnames[i]);
        }

      intdata = (int *) malloc(sizeof(int) * ncells);

      for (j = 0; j < ncells; j++)
        {                           
         intdata[j] = (int)cellmats[j];
        }                                     
      gmvwrite_material_ids(intdata, flag);

      free(intdata);
     } 

   if (mats_on_nodes > 0)
     {
      printf("Writing node materials.\n");
      flag = 1;

      gmvwrite_material_header(mmats, flag);

      for (i = 0; i < mmats; i++) 
        {
         gmvwrite_material_name(mmatnames[i]);
        }

      intdata = (int *) malloc(sizeof(int) * nnodes);

      for (j = 1; j < nnodes+1; j++)
        {
         intdata[j-1] = (int)imat[j];
        }                                     
      gmvwrite_material_ids(intdata, flag);

      free(intdata);
     } 



   /*  Write velocities.  */
   flag = -1;

   if (nvectin > -1)  /* nodal velocities */
     {
      flag = 1;
     }
   else if (cvectin > -1) /* cell velocities */
     {
      flag = 0;
     }
    
   if (flag > -1)
     {
      printf("Writing velocities.\n");
      if (flag == 1)
        {
         u = (double *)malloc((nnodes+1)*sizeof(double));
         v = (double *)malloc((nnodes+1)*sizeof(double));
         w = (double *)malloc((nnodes+1)*sizeof(double));

         fillnodevalue(vectfldx, u);
         fillnodevalue(vectfldy, v);
         fillnodevalue(vectfldz, w);

         gmvwrite_velocity_data(flag, u+1, v+1, w+1);

         free(u);
         free(v);
         free(w);
        }

      else
        {
         u = (double *)malloc((ncells+1)*sizeof(double));
         v = (double *)malloc((ncells+1)*sizeof(double));
         w = (double *)malloc((ncells+1)*sizeof(double));

         fillcellvalue(cvectfldx, u);
         fillcellvalue(cvectfldy, v);
         fillcellvalue(cvectfldz, w);

         gmvwrite_velocity_data(flag, u, v, w);

         free(u);
         free(v);
         free(w);
        }  
      }


   /*  Write out fields.  */
   for (i = 0; i < cnumvars; i++) 
     {
      if (strncmp(cfieldname[i],"Deleted",7) == 0) cvars_to_write[i] = 0; 
     }
   for (i = 0; i < numvars; i++) 
     {
      if (strncmp(fieldname[i],"Deleted",7) == 0) nvars_to_write[i] = 0;
     }
   flag = 0;
   for (i = 0; i < MAXFIELDS; i++) 
     {
      flag += cvars_to_write[i] + nvars_to_write[i];
     }

   if (flag > 0)
     {
      k = ((nnodes+1) > ncells) ? (nnodes+1) : ncells;
      doubledata = (double *)malloc(k*sizeof(double));

      gmvwrite_variable_header();

      flag = 0; 
      for (i = 0; i < MAXFIELDS; i++) 
        {
         flag += nvars_to_write[i];
        }

      if (flag > 0) 
        {
         flag = 1;
         printf("Writing node field variables.\n");

         for (i = 0; i < MAXFIELDS; i++)
           {
            if (nvars_to_write[i])
              {
               fillnodevalue(i,doubledata);
               gmvwrite_variable_name_data(flag, fieldname[i], doubledata+1);
              }
           }
        }

      flag = 0; 
      for (i = 0; i < MAXFIELDS; i++) 
        {
         flag += cvars_to_write[i];
        }

      if (flag > 0) 
        {
         flag = 0;
         printf("Writing cell field variables.\n");

         for (i = 0; i < MAXFIELDS; i++)
           {
            if (cvars_to_write[i])
              {
               fillcellvalue(i,doubledata);
               gmvwrite_variable_name_data(flag, cfieldname[i], doubledata);
              }
           }
        }

      gmvwrite_variable_endvars();

      free(doubledata); 
     }


   /*  Write units.  */
   if (units.nnodes > 0 || units.ncells > 0 || 
       strncmp(units.xyz,"  ",2) != 0 ||
       strncmp(units.uvw,"  ",2) != 0)
     {
      printf("Writing units.\n");

      gmvwrite_units_header();

      if (strncmp(units.xyz,"  ",2) != 0)
        {
         gmvwrite_units_name("xyz",units.xyz);
        }
      if (strncmp(units.uvw,"  ",2) != 0)
        {
         gmvwrite_units_name("velocity",units.uvw);
        }

      if (units.nnodes > 0)
        {
         for (i = 0; i < units.nnodes; i++)
           {
            gmvwrite_units_name(units.nfldname[i],units.nfldunit[i]);
           }
        }
      if (units.ncells > 0)
        {
         for (i = 0; i < units.ncells; i++)
           {
            gmvwrite_units_name(units.cfldname[i],units.cfldunit[i]);
           }
        }

      gmvwrite_units_endunit();

     }



   /*  Write node flags.  */
   if ((numflags_node > 0) || (numflags_cell > 0))       
     {
      gmvwrite_flag_header();

      printf("Writing flags.\n");

      for (i = 0; i < numflags_node; i++)
        {
         flag = 1;

         gmvwrite_flag_name(flagname_node[i], flagtypes_node[i], flag);

         for (j = 0; j < flagtypes_node[i]; j++)
           {
            gmvwrite_flag_subname(flagnames_node[i][j]);
           }

         intdata = (int *) malloc(sizeof(int) * nnodes);

         for (j = 0; j < nnodes; j++ )
           {
            intdata[j] = (int)flags[i][j+1];
           }
         gmvwrite_flag_data(flag, intdata);

         free(intdata);

        }

   /*  Write cell flags.  */

      for (i = 0; i < numflags_cell; i++) 
        {
         flag = 0;

         gmvwrite_flag_name(flagname_cell[i], flagtypes_cell[i], flag);

         for (j = 0; j < flagtypes_cell[i]; j++) 
           {
            gmvwrite_flag_subname(flagnames_cell[i][j]);
           } 

         intdata = (int *) malloc(sizeof(int) * ncells);

         for (j = 0; j < ncells; j++ )
           {
            intdata[j] = (int)cellflags[i][j+1];
           } 
         gmvwrite_flag_data(flag, intdata);

         free(intdata);

        }

      gmvwrite_flag_endflag();

     }


   /*  Write polygons.  */
   flag = 0;
   for (i = 0; i < mmats; i++)
     {
      if (npolyspermat[i] > 0 )
        {
         flag = 1;
         break;
        }
     }
   if (flag == 1)
     {
      if (polysflag == 0 && linesflag == 0)
        {
         readpolyfile();
        }

      printf("Writing polygons.\n");

      gmvwrite_polygons_header();

      for (i = 0; i < mmats; i++)
        {
         for (j = 0; j < npolyspermat[i]; j++)
           {
            itemp = i+1;  /* material number */
            nverts = (int)polygons[i][j].nverts;
            nv = polygons[i][j].polyvertloc;

            x = (double *) malloc(sizeof(double) * nverts);
            y = (double *) malloc(sizeof(double) * nverts);
            z = (double *) malloc(sizeof(double) * nverts);

            for (k = 0; k < nverts; k++)
              {
               x[k] = polyverts[i][k+nv].x;
               y[k] = polyverts[i][k+nv].y;
               z[k] = polyverts[i][k+nv].z;
              }
            gmvwrite_polygons_data(nverts, itemp, x, y, z);

            free(x);
            free(y);
            free(z);

           }     
        }

      gmvwrite_polygons_endpoly();

      if (polysflag == 0 && linesflag == 0)
        {
         for (i = 0; i < MAXMATS; i++)
           { /* free arrays filled in readpolyfile */
            FREE(polygons[i]);
            FREE(polyverts[i]);
           }
        }
     }


   /*  Write out nodeids  */
   if (nodeidflag != 0) 
     {
      printf("Writing nodeids.\n");
      gmvwrite_nodeids(nodeids+1);
      printf("nodeids = %d\n", nodeids+1);
     }

   /*  Write out cellids  */
   if (cellidflag != 0)
     {
      printf("Writing cellids.\n");
      gmvwrite_cellids(cellids);
     }

   /*  Write out tracers  */
   if (ntracers > 0)
     {
      printf("Writing Particles.\n");

      x = (double *) malloc(sizeof(double) * ntracers);
      y = (double *) malloc(sizeof(double) * ntracers);
      z = (double *) malloc(sizeof(double) * ntracers);

      for (i = 0; i < ntracers; i++) 
        {
         x[i] = tracers[0].x[i];
         y[i] = tracers[0].y[i];
         z[i] = tracers[0].z[i];
        }
      gmvwrite_tracers_header(ntracers, x, y, z);

      free(x);
      free(y);
      free(z);

      for (k = 0; k < numvartr; k++)
        {
         gmvwrite_tracers_name_data(ntracers, fieldtrname[k], 
                                              tracers[0].field[k]);
        }
      gmvwrite_tracers_endtrace();

      /*  Write out traceids  */
      if (traceridflag != 0)
        {
         printf("Writing traceids.\n");
         gmvwrite_traceids(ntracers,tracerids);
        }
     }


   /* Write out cycleno */
   if (cycleno > 0) 
     {
      printf("Writing cycle number.\n");
      gmvwrite_cycleno(cycleno);
     }

   /* Write out problem time */
   if (ptime != 0) 
     {
      printf("Writing problem time.\n");
      gmvwrite_probtime(ptime);
     }


   /*  Write out surfaces.  */
   if (nsurf > 0)
     {
      printf("Writing surfaces.\n");

      gmvwrite_surface_header(&nsurf);

      /*  Write out surface facets.  */
      for (i = 0; i < nsurf; i++)
        {
         nverts = surfs[i].nverts;

         intdata = (int *) malloc(sizeof(int) * nverts);

         for (j = 0; j < nverts; j++)
           {
            intdata[j] =  surfs[i].fverts[j];
           }
         gmvwrite_surface_data(surfs[i].nverts, intdata);

         free(intdata);
        }

      /*  Write out surface material nos.  */
      printf("Writing surface materials.\n");
      intdata = (int *) malloc(sizeof(int) * nsurf);
      for (i = 0; i < nsurf; i++)
        {
         intdata[i] = (int)surfmats[i];
        }
      gmvwrite_surfmats(intdata);
   
      free(intdata);


      /*  Write out surface velocities.  */
      if (svectin > -1)
        {
         printf("Writing surface velocities.\n");
         x = (double *)malloc((nsurf)*sizeof(double));
         y = (double *)malloc((nsurf)*sizeof(double));
         z = (double *)malloc((nsurf)*sizeof(double));

         fillsurfvalue(svectfldx, x);
         fillsurfvalue(svectfldy, y);
         fillsurfvalue(svectfldz, z);

         gmvwrite_surfvel(u, v, w);

         free(x);
         free(y);
         free(z);
        }

      /*  Write out surface fields.  */
     for (i = 0; i < snumvars; i++) 
        {
         if (strncmp(sfieldname[i],"Deleted",7) == 0) svars_to_write[i] = 0; 
        }
      flag = 0;
      for (i = 0; i < MAXFIELDS; i++) 
        {
         flag += svars_to_write[i];
        }

      if (flag > 0)
        {
         gmvwrite_surfvars_header();
         printf("Writing surface variables.\n");

         doubledata = (double *)malloc(nsurf*sizeof(double));

         for (i = 0; i < MAXFIELDS; i++)
           {
            if (svars_to_write[i])
              {
               fillsurfvalue(i,doubledata);
               gmvwrite_surfvars_name_data(sfieldname[i], doubledata);
              }
           }
         gmvwrite_surfvars_endsvar();

         free(doubledata); 
        }

      /*  Write surface flags.  */
      if (numflags_surf > 0)
        {
         printf("Writing surface flags.\n");
         gmvwrite_surfflag_header();

         for (i = 0; i < numflags_surf; i++) 
           {
            gmvwrite_surfflag_name(flagname_surf[i], flagtypes_surf[i]);

            for (j = 0; j < flagtypes_surf[i]; j++) 
              {
               gmvwrite_surfflag_subname(flagnames_surf[i][j]);
              }

            intdata = (int *) malloc(sizeof(int) * nsurf);

            for (j = 0; j < nsurf; j++ )
              {
               intdata[j] = (int)surfflags[i][j];
              } 
            gmvwrite_surfflag_data(intdata);

            free(intdata);
           }

         gmvwrite_surfflag_endflag();
        }
     }

   /*  Write out group data.  */
   if (ncellgrps > 0 || nnodegrps > 0 || nfacegrps > 0 || nsurfacegrps > 0)
     {
      gmvwrite_group_header();
      if (ncellgrps > 0)
        {
         flag = 0;
         for (i = 0; i < ncellgrps; i++)
           {
            if (strncmp(cellgrpname[i],"Deleted",7) != 0)
               gmvwrite_group_data(cellgrpname[i], flag, numcellgrp[i],
                                   cellgrps[i]);
           }
        }
      if (nnodegrps > 0)
        {
         flag = 1;
         for (i = 0; i < nnodegrps; i++)
           {
            if (strncmp(nodegrpname[i],"Deleted",7) != 0)
               gmvwrite_group_data(nodegrpname[i], flag, numnodegrp[i],
                                   nodegrps[i]);
           }
        }
      if (nfacegrps > 0)
        {
         flag = 2;
         for (i = 0; i < nfacegrps; i++)
           {
            if (strncmp(facegrpname[i],"Deleted",7) != 0)
              gmvwrite_group_data(facegrpname[i], flag, numfacegrp[i],
                                  facegrps[i]);
           }
        }
      if (nsurfacegrps > 0)
        {
         flag = 3;
         for (i = 0; i < nsurfacegrps; i++)
           {
            if (strncmp(surfacegrpname[i],"Deleted",7) != 0)
            gmvwrite_group_data(surfacegrpname[i], flag, numsurfacegrp[i],
                                surfacegrps[i]);
           }
        }
      gmvwrite_group_endgroup();
     }


   gmvwrite_closefile();

   printf("Finished writing output file: %s\n", outfilnam);

   flag = -1;
   newwriteflag = 1;
}

/* --------------------------------------------------------------- */

void do_nodes(void)
{
  char ffname[200];
  int nx, ny, nz, num;
  int nnodes, i;
  long ffnnodes;
  double *x, *y, *z;
  double x0, y0, z0, dx, dy, dz;

   printf("Writing nodes.\n");

   if (gmv_data.datatype == FROMFILE)
     {
      strcpy(ffname,gmv_data.chardata1);
      ffnnodes = gmv_data.num;
      gmvwrite_nodes_fromfile(ffname, ffnnodes);
      nnodes = ffnnodes;
     }

   else if (gmv_data.datatype == UNSTRUCT ||
            gmv_data.datatype == LOGICALLY_STRUCT)
     {
      nnodes = gmv_data.num;
      if (nnodes > 0)
        {
         x = (double *) malloc(sizeof(double) * nnodes);
         y = (double *) malloc(sizeof(double) * nnodes);
         z = (double *) malloc(sizeof(double) * nnodes);
         for (i = 0; i < nnodes; i++)
           {
            x[i] = gmv_data.doubledata1[i];
            y[i] = gmv_data.doubledata2[i];
            z[i] = gmv_data.doubledata3[i];
           }
        }
      if (gmv_data.datatype == LOGICALLY_STRUCT)
        {
         nx = gmv_data.ndoubledata1;
         ny = gmv_data.ndoubledata2;
         nz = gmv_data.ndoubledata3;
         gmvwrite_node_data_lstruct(&nx, &ny, &nz, x, y, z);
        }
      else
        {
         num = nnodes;
         gmvwrite_node_data(&num, x, y, z);
        }

      free(x);
      free(y);  
      free(z);
     }

   else if (gmv_data.datatype == STRUCT)
     {
      nx = gmv_data.ndoubledata1;
      ny = gmv_data.ndoubledata2;
      nz = gmv_data.ndoubledata3;

      x = (double *) malloc(sizeof(double) * nx);
      y = (double *) malloc(sizeof(double) * ny);
      z = (double *) malloc(sizeof(double) * nz);
      for (i = 0; i < nx; i++)
         x[i] = gmv_data.doubledata1[i];
      for (i = 0; i < ny; i++)
         y[i] = gmv_data.doubledata2[i];
      for (i = 0; i < nz; i++)
         z[i] = gmv_data.doubledata3[i];
      gmvwrite_node_data_struct(&nx, &ny, &nz, x, y, z);

      free(x);
      free(y);
      free(z);
     }

   else if (gmv_data.datatype == AMR)
     {
      nx = gmv_data.num2;
      ny = gmv_data.nlongdata1;
      nz = gmv_data.nlongdata2;
      x0 = gmv_data.doubledata1[0];
      y0 = gmv_data.doubledata1[1];
      z0 = gmv_data.doubledata1[2];
      dx = gmv_data.doubledata2[0];
      dy = gmv_data.doubledata2[1];
      dz = gmv_data.doubledata2[2];
      gmvwrite_node_data_amr(nx, ny, nz, &x0, &y0, &z0,
                             &dx, &dy, &dz);
     }
}

/* --------------------------------------------------------------- */

void do_cells(void)
{
  char cellname[10], ffname[200];
  int ncells, i;
  long ffncells;
  int *daughters, n_faces, *n_verts, *verts, num_verts, sum_verts;

   if (gmv_data.datatype == FROMFILE)
     {
      strcpy(ffname,gmv_data.chardata1);
      ffncells = gmv_data.num;
      gmvwrite_cells_fromfile(ffname,ffncells);
      ncells = gmv_data.num;
     }
  
   else if (gmv_data.datatype == AMR)
     {
      ncells = gmv_data.num;
      daughters = (int *) malloc(sizeof(int) * ncells);
      for (i = 0; i < ncells; i++)
         daughters[i] = gmv_data.longdata1[i];
      gmvwrite_cells_amr(&ncells, &ncells, daughters);
      free(daughters);
     }

   else if (gmv_data.datatype == ENDKEYWORD)
     {
      if (gmv_data.num == 0)
        {
         printf("Writing cell header. (at ENDKEYWORD)\n");
         ncells = gmv_data.num;
         gmvwrite_cell_header(&ncells);
        }
      return;
     }

   else
     {
      if (cfirst)
        {
         cfirst = 0;
         printf("Writing cells.\n");
         ncells = gmv_data.num;
         gmvwrite_cell_header(&ncells);
        }

      if (gmv_data.datatype == GENERAL)
        {
         n_faces = gmv_data.num2;
         n_verts = (int *) malloc(sizeof(int) * n_faces);
         for (i = 0; i < n_faces; i++)
            n_verts[i] = gmv_data.longdata1[i];
         sum_verts = gmv_data.nlongdata2;
         verts = (int *) malloc(sizeof(int) * sum_verts);
         for (i = 0; i < sum_verts; i++)
            verts[i] = gmv_data.longdata2[i];
         gmvwrite_general_cell_type("general ", n_verts, n_faces, verts);
         free(n_verts);  free(verts);
        }
      if (gmv_data.datatype == REGULAR || gmv_data.datatype == VFACE3D ||
          gmv_data.datatype == VFACE2D)
        {
         strncpy(cellname,gmv_data.name1,8);
         *(cellname+8) = (char) 0;
         num_verts = gmv_data.num2;
         verts = (int *) malloc(sizeof(int) * num_verts);
         for (i = 0; i < num_verts; i++)
            verts[i] = gmv_data.longdata1[i];
         gmvwrite_cell_type(cellname, num_verts, verts);
         free(verts);
        }
     }
}

/* --------------------------------------------------------------- */

void do_faces(void)
{
  char ffname[200];
  int numfaces, numcells, i;
  long ffncells;
  int nverts, *verts, cellid1, cellid2;

   if (gmv_data.datatype == REGULAR)
     {
      if (ffirst)
        {
         ffirst = 0;
         printf("Writing faces.\n");
         numfaces = gmv_data.num;
         numcells = gmv_data.num2;
         gmvwrite_face_header(&numfaces, &numcells);
        }
      nverts = gmv_data.nlongdata1 - 2;
      verts = (int *) malloc(sizeof(int) * nverts);
      for (i = 0; i < nverts; i++)
         verts[i] = gmv_data.longdata1[i];
      cellid1 = gmv_data.longdata1[nverts];
      cellid2 = gmv_data.longdata1[nverts+1];
      gmvwrite_face_data(nverts, verts, &cellid1, &cellid2);
      free(verts);
     }

   if (gmv_data.datatype == FROMFILE)
     {
      printf("Writing faces.\n");
      strcpy(ffname,gmv_data.chardata1);
      numfaces = gmv_data.num;
      numcells = gmv_data.num2;
      ffncells = gmv_data.num2;
      gmvwrite_faces_fromfile(ffname,ffncells);
     }

   if (gmv_data.datatype == ENDKEYWORD) 
     {
      return;
     }
}

/* --------------------------------------------------------------- */

void do_vfaces(void)
{
  char ffname[200];
  int numfaces, i;
  long ffncells;
  int nverts, *verts, facepe, oppface, oppfacepe, cellid;

   if (gmv_data.datatype == REGULAR)
     {
      if (vfirst)
        {
         vfirst = 0;
         printf("Writing vfaces.\n");
         numfaces = gmv_data.num;
         gmvwrite_vface_header(&numfaces);
        }
      nverts = gmv_data.nlongdata1;
      verts = (int *) malloc(sizeof(int) * nverts);
      for (i = 0; i < nverts; i++)
         verts[i] = gmv_data.longdata1[i];
      facepe = gmv_data.longdata2[0];
      oppface = gmv_data.longdata2[1];
      oppfacepe = gmv_data.longdata2[2];
      cellid = gmv_data.longdata2[3];
      gmvwrite_vface_data(nverts,facepe,&oppface,oppfacepe,&cellid,verts);
      free(verts);
     }

   if (gmv_data.datatype == FROMFILE)
     {
      printf("Writing vfaces.\n");
      strcpy(ffname,gmv_data.chardata1);
      numfaces = gmv_data.num;
      ffncells = gmv_data.num2;
      gmvwrite_vfaces_fromfile(ffname,ffncells);
     }

   if (gmv_data.datatype == ENDKEYWORD) 
     {
      return;
     }
}


/* --------------------------------------------------------------- */

