#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmvdata.h"

#include <sys/types.h>
#include <sys/stat.h>
//#include <malloc.h>
#include <math.h>

#include <exodusII.h>

#define MAXSIZE 1000
#define MAXVAL 9.99999e32

extern char *fldfilenam, *cfldfilenam, *polyfilenam, *sfldfilenam, *ffldfilenam;
extern FILE *fieldfile, *cfieldfile, *polyfile, *sfieldfile, *ffieldfile;

int exoid;
static long long nnodes_last, ncells_last, nfaces_alloc;
static long long maxnodes = 0, maxcells = 0;
static int numcells, numnodes, numdim, max_elem_per_blk;
static int nvars_node, nvars_cell, num_blks, num_node_sets, num_side_sets;

FILE *tmpnfieldfile, *tmpcfieldfile;
char *tmpnfldnam, *tmpcfldnam;
char pfxtn[5] = "GMVTN", pfxtc[5] = "GMVTC";

double *nfield_data[MAXFIELDS], *cfield_data[MAXFIELDS];
long int last_nfield_loc[MAXFIELDS], last_cfield_loc[MAXFIELDS];

int *i_data;
double *d_data1, *d_data2, *d_data3;
static int mesh_dim, nnodevects, vect_var;

int exoread_checkfile(char *filename)
{
  /*                           */
  /*  Check for Exodus input.  */
  /*                           */

  int ierr, i, j, comp_ws = 8, io_ws = 0;
  float version, *fdum;
  char *cdum;

   ierr = 0;
   i = ex_open(filename,EX_READ,&comp_ws,&io_ws,&version);
   if (i < 0) ierr = 1;

   if (ierr) printf("This is not an Exodus file.\n");

   /*  If this is a good exodus file, get the number of time steps.  */
   n_exo_time_steps = 0;
   if (ierr == 0)
     j = ex_inquire(i, EX_INQ_TIME, &n_exo_time_steps, &fdum, cdum);
   exo_time_step = 0;

   /*  Close the file.  */
   j = ex_close(i);
   return ierr;
}


void readexodus()
{
  int ierr, i;
  int comp_ws = 8, io_ws = 0;
  float version;
  char exotitle[MAX_LINE_LENGTH+1];
  void readgmv_init(void);
  void exo_get_mesh(void);
  void exo_get_data(void);

   /*  Open the exodus file.  */
   exoid = ex_open(filnam,EX_READ,&comp_ws,&io_ws,&version);

   /*  Initialize data and reopen database files.  */
   readgmv_init();
   if (!noprintflag)
      printf("GMV reading exodus file %s\n",filnam);

   /*  Get inititial data.  */
   ierr = ex_get_init(exoid, exotitle, &numdim, &numnodes, &numcells, 
                      &num_blks, &num_node_sets, &num_side_sets);
   
   /*  Open and read mesh data from exodus files.  */
   exo_get_mesh();

   /*  Read/set attributes and variables for this mesh.  */
   exo_get_data();

   /*  Close the exodus file.  */
   ierr = ex_close(exoid);
}


void exo_get_mesh(void)
{ 
  void bld_exo_mesh();
  void getcellfaces_exo(void);
  void getcelledges_exo(void);

   /*  Build mesh if new simulation (always same cells).  */
   if (rdgmvflag == 0)
     {
      if (!noprintflag)
         printf("GMV reading %d nodes and %d cells.\n",numnodes,numcells);
      bld_exo_mesh();

      /* Find duplicate faces and set cell numbers per face.  */
      getcellfaces_exo();

      /*  Calculate and save compressed cell edges.  */
      getcelledges_exo();
     }
}


void bld_exo_mesh(void)
{
  int i, j, ib, cellno, ierr, *blk_ids, nelem_in_blk, nnodes_per_elem, nattr;
  int *elem_map, *connect, node_list[30], np, icell;
  double *x, *y, *z;
  char elem_type[MAX_STR_LENGTH+1], gmv_reg[9];
  void convert_exo2gmv(char exo_reg[MAX_STR_LENGTH+1], int np, char gmv_reg[9]);
  void regcell_exo(long icell, char *gmv_reg, int np, int node_list[30]);
                               
   /*  Allocate cell and node structures.  */
   ncells = numcells;
   nnodes = numnodes;

   cells = (struct cell *)malloc(numcells*sizeof(struct cell));
   nodes = (struct node *)malloc((numnodes+1)*sizeof(struct node));
   for (i = 0; i < numnodes+1; i++)
     {
      nodes[i].x = 0.;
      nodes[i].y = 0.;
      nodes[i].z = 0.;
     }

   /*  Approximate number of faces and allocate face structure.  */
   cellfaces = (struct cellface *)malloc(numcells*6*sizeof(struct cellface));
   nfaces_alloc = numcells * 6;
   ncellfaces = 0;

   /*  Get the node coordinates.  */
   x = (double *)malloc((nnodes) * sizeof(double));
   y = (double *)malloc((nnodes) * sizeof(double));
   z = (double *)malloc((nnodes) * sizeof(double));
   for (i = 0; i < nnodes; i++) z[i] = 0.;
   if (numdim == 2) ex_get_coord(exoid, x, y, 0);
   else ex_get_coord(exoid, x, y, z);
   for (i = 0; i < nnodes; i++)
     {
      nodes[i+1].x = x[i];
      nodes[i+1].y = y[i];;
      nodes[i+1].z = z[i];
     }
   free(x);  free(y);  free(z);

   /*  Get an element order map.  */
   elem_map = (int *)malloc(numcells * sizeof(int));
   ierr = ex_get_map(exoid, elem_map);

   /*  Read element block ids.  */
   blk_ids = (int *)malloc(num_blks * sizeof(int));
   ierr = ex_get_elem_blk_ids(exoid, blk_ids);

   /*  Loop through the element blocks.  */
   cellno = 0;   max_elem_per_blk = 0;
   for (ib = 0; ib < num_blks; ib++)
     {

      /*  Get the element type, number of elements in the block, */
      /*  the number of nodes per element in the block,          */
      /*  and the number of attributes in the block.             */
      ierr = ex_get_elem_block(exoid, blk_ids[ib], elem_type, &nelem_in_blk,
                               &nnodes_per_elem, &nattr);

      /*  Determine max. no. of elements per block.  */
      if (nelem_in_blk > max_elem_per_blk) max_elem_per_blk = nelem_in_blk;

      /*  Convert from exodus type to gmv type.  */
      convert_exo2gmv(elem_type, nnodes_per_elem, gmv_reg);

      /*  Get the connectivity array.  */
      connect = (int *)malloc(nnodes_per_elem * nelem_in_blk * sizeof(int));
      ierr = ex_get_elem_conn(exoid, blk_ids[ib], connect);

      /*  Loop through the connectivity array to fill GMV cell structure.  */
      for (j = 0; j < nelem_in_blk; j++)
        {
         for (i = 0; i < nnodes_per_elem; i++)
           {
            node_list[i] = connect[j * nnodes_per_elem + i];
           }
         icell = elem_map[cellno] - 1;
         regcell_exo(icell, gmv_reg, nnodes_per_elem, node_list);
         cellno++;
        }
      FREE(connect);
     }
   FREE(elem_map);  FREE(blk_ids);
}


void exo_get_data()
{
  int ierr, i, j, k, *tstit;
  int *elem_map, *blk_ids, nelem_in_blk, nnodes_per_elem, nattr, icell, jcell;
  char elem_type[MAX_STR_LENGTH+1];
  void bld_mats_exo(int *elem_map, int *blk_ids);
  void bld_bdy_exo(), bld_vars_exo(int *elem_map, int *blk_ids);

   /*  Get an element order map and block ids.  */
   elem_map = (int *)malloc(numcells * sizeof(int));
   ierr = ex_get_map(exoid, elem_map);
   blk_ids = (int *)malloc(num_blks * sizeof(int));
   ierr = ex_get_elem_blk_ids(exoid, blk_ids);

   /*  Get the time for the time step, if any.  */
   ptime = 0.;
   if (exo_time_step > 0)
     {
      ierr = ex_get_time(exoid, exo_time_step, &ptime);
      if (ierr |= 0) ptime = 0.;
     }

   /*  Generate materials info.  */
   bld_mats_exo(elem_map, blk_ids);

   /*  Generate node boundary groups/flags from node and side sets.  */
   bld_bdy_exo();

   /*  Get variable for the current time step, if any.  */
   if (exo_time_step > 0)
      bld_vars_exo(elem_map, blk_ids);

   FREE(elem_map);  FREE(blk_ids);  
}


void bld_mats_exo(int *elem_map,int *blk_ids)
{
  int i, j, k, ierr;
  int nelem_in_blk, nnodes_per_elem, nattr, icell, jcell;
  char **matnames_exo;
  char elem_type[MAX_STR_LENGTH+1];
  short *matcomp, *blkmat;
  void bldnodemats(void);

   /*  Use block names, if any, as material names.     */
   /*  Else, build material names with block numbers.  */
   matnames_exo = (char **)malloc(num_blks * sizeof(* matnames_exo));
   for (i = 0; i < num_blks; i++)
      matnames_exo[i] = (char *)malloc(MAX_STR_LENGTH + 1);
   blkmat = (short *)malloc(num_blks * sizeof(short));
   for (j = 0; j < num_blks; j++) blkmat[j] = j;
   matcomp = (short *)malloc(num_blks * sizeof(short));
   for (j = 0; j < num_blks; j++) matcomp[j] = 1;
   ierr = ex_get_names(exoid, EX_ELEM_BLOCK, matnames_exo);
   mmats = num_blks;
   j =strlen(matnames_exo[0]);
   if (j < 1) ierr = 1;

   /*  Loop through block names and look for duplicate names.  */
   if (ierr == 0 && num_blks > 1)
     {
      for (j = 0; j < num_blks-1; j++)
        {
         for (k = j+1; k < num_blks; k++)
           {
            if (strcmp(matnames_exo[j],matnames_exo[k]) == 0)
              {
               matcomp[k] = 0;
               blkmat[k] = j;
              }
           }
        }
      mmats = 0;
      for (j = 0; j < num_blks; j++) mmats += matcomp[j];
      for (j = 0; j < num_blks; j++)
        {
         if (blkmat[j] > mmats) blkmat[j] = mmats;
        }
     }

   /*  Fill material names from block names, if any.           */
   /*  If no block names, generate names using block numbers.  */ 
   if (ierr == 0)
     {
      if (mmats > 0)
        {
         if (mmats > MAXMATS) mmats = MAXMATS;
         j = 0;
         for (i = 0; i < num_blks; i++)
           {
            if (matcomp[i] && j < MAXMATS)
              {
               strncpy(mmatnames[j],matnames_exo[i],32);
               *(mmatnames[j]+33) = (char) 0;
               j++;
              }
           }
        }
     }
   else
     {
      for (i = 0; i < num_blks; i++)
         sprintf(mmatnames[i],"mat%d",i+1);
     }
   maxcellmat = mmats;

   free(matcomp);
   for (i = 0; i < num_blks; i++) free(matnames_exo[i]);
   free(matnames_exo);

   /*  Fill cell materials from blocks.  */
   mats_on_cells = 1;  mats_on_nodes = 0;
   imat=(short *)malloc((nnodes+1)*sizeof(short));
   if (imat == NULL) memerr();
   cellmats=(short *)malloc((ncells)*sizeof(short));
   if (cellmats == NULL) memerr();
   jcell = 0;
   for (i = 0; i < num_blks; i++)
     {
      ierr = ex_get_elem_block(exoid, blk_ids[i], elem_type, &nelem_in_blk,
                               &nnodes_per_elem, &nattr);
      for (j = 0; j < nelem_in_blk; j++)
        {
         icell = elem_map[jcell] - 1;
         cellmats[icell] = blkmat[i] + 1;
         jcell++;
        }
     }
   free(blkmat);

   /*  Calculate node materials.  */
   bldnodemats();
}



void bld_bdy_exo()
{
  int ierr, i, j, k, l, *flags_in, *elem_list, *face_list, icell, iface;
  int *ids, *node_list, *fctr, *cctr, nnodes_in_set, ndist_in_set, ntypes, 
      nsides, max_sides;
  int i1, quad_1[4] = {0,1,2,3}, tri_1[3] = {0,1,2};
  int i2, quad_2[4] = {1,2,3,0}, tri_2[3] = {1,2,0};
  float version, *fdum;
  char *cdum;
  char tmpname[MAX_STR_LENGTH+1];

   /*  Build node boundary groups/flags from node and side sets.  */
   /*  From cubit, node sets contain nodes at all boundaries and  */ 
   /*  side sets contain external boundary info.                  */
   /*  Also, build surface elements from side sets.               */

   flags_in = (int *)malloc((nnodes+1) * sizeof(int));
   for (i = 0; i < nnodes+1; i++) flags_in[i] = 1;

   /*  Get the node set ids.  */
   if (num_node_sets > 0)
     {      
      ids = (int *)malloc(num_node_sets * sizeof(int));
      ierr = ex_get_node_set_ids(exoid, ids);
   
      /*  Loop through node sets, if any.  */
      if (ierr == 0)
        {
         for (i = 0; i < num_node_sets; i++)
           {

            /*  Get the name for this set.  */
            ierr = ex_get_name(exoid, EX_NODE_SET, ids[i], tmpname);
            if (ierr != 0) sprintf(tmpname,"node_set_%d",i+1);
            j = strlen(tmpname);
            if (j == 0) sprintf(tmpname,"node_set_%d",i+1);

            /*  Get the node list for this set.  */
            ierr = ex_get_node_set_param(exoid, ids[i], &nnodes_in_set,
                                         &ndist_in_set);
            node_list = (int *)malloc(nnodes_in_set * sizeof(int));
            ierr = ex_get_node_set(exoid, ids[i], node_list);

            /*  Loop through the nodes in the set and set to interface.  */
            l = 0;
            for (j = 0; j < nnodes_in_set; j++)
              {
               k = node_list[j];
               if (k <= nnodes && k >= 0)
                 {
                  flags_in[k] = 2;
                  l++;
                 }
              }

            /*  Save node group, if good set.  */
            if (l > 0)
              {

               /*  Allocate and set a node group for this set.  */
               nodegrpname[nnodegrps] = (char *)malloc(33*sizeof(char));
               if (nodegrpname[nnodegrps] == NULL) memerr();
               strcpy(nodegrpname[nnodegrps],tmpname);
               *(nodegrpname[nnodegrps]+32) = (char) 0;

               /*  Save the number and data for this node group.  */
               numnodegrp[nnodegrps] = nnodes_in_set;

               nodegrps[nnodegrps] = (int *)malloc((nnodes_in_set)*sizeof(int));
               if (nodegrps[nnodegrps] == NULL) memerr();
               for (j = 0; j < nnodes_in_set; j++)
                 {
                  k = node_list[j];
                  nodegrps[nnodegrps][j] = k;
                 }
         
               nnodegrps++;
              }
            FREE(node_list);
           }
        }
      free(ids);
     }

   /*  Get the side set ids.  */
   if (num_side_sets > 0)
     {

      /*  Get the total number of sides and allocat surface data.  */
      ierr = ex_inquire(exoid, EX_INQ_SS_ELEM_LEN, &max_sides, &fdum, cdum);
      surfs = (struct surface *) malloc(max_sides*sizeof(struct surface));
      if (surfs == NULL) memerr();
      surfmats=(short *)malloc((max_sides)*sizeof(short));
      if (surfmats == NULL) memerr();
      nsurf = 0;

      ids = (int *)malloc(num_side_sets * sizeof(int));
      ierr = ex_get_side_set_ids(exoid, ids);
   
      /*  Loop through side sets, if any.  */
      if (ierr == 0)
        {
         cctr = (int *)malloc(ncells * sizeof(int));

         for (i = 0; i < num_side_sets; i++)
           {

            /*  Get the name for this set.  */
            ierr = ex_get_name(exoid, EX_SIDE_SET, ids[i], tmpname);
            if (ierr != 0) sprintf(tmpname,"side_set_%d",i+1);
            j = strlen(tmpname);
            if (j == 0) sprintf(tmpname,"side_set_%d",i+1);

            /*  Save the name as a new material name.  */
            strncpy(mmatnames[mmats],tmpname,32);
            *(mmatnames[mmats]+33) = (char) 0;

            /*  Get the node ids for this set.  */
            ierr = ex_get_side_set_param(exoid, ids[i], &nsides, 
                                         &nnodes_in_set);
            fctr = (int *)malloc(nsides * sizeof(int));
            ierr = ex_get_side_set_node_list_len(exoid, ids[i], &nnodes_in_set);
            node_list = (int *)malloc(nnodes_in_set * sizeof(int));
            ierr = ex_get_side_set_node_list(exoid, ids[i], fctr, node_list);

            /*  Loop through the nodes in the set and set to exterior.  */
            for (j = 0; j < nnodes_in_set; j++)
              {
               k = node_list[j];
               flags_in[k] = 3;
              }

            /*  Allocate and set a node group for this set.  */
            nodegrpname[nnodegrps] = (char *)malloc(33*sizeof(char));
            if (nodegrpname[nnodegrps] == NULL) memerr();
            strcpy(nodegrpname[nnodegrps],tmpname);
            *(nodegrpname[nnodegrps]+32) = (char) 0;

            /*  Save the number and data for this node group.  */
            numnodegrp[nnodegrps] = nnodes_in_set;

            nodegrps[nnodegrps] = (int *)malloc((nnodes_in_set)*sizeof(int));
            if (nodegrps[nnodegrps] == NULL) memerr();
            for (j = 0; j < nnodes_in_set; j++)
              {
               k = node_list[j];
               nodegrps[nnodegrps][j] = k;
              }
         
            nnodegrps++;
            FREE(node_list);
            FREE(fctr);

            /*  Generate a cell and face group for this side set.  */

            /*  Save cell group name.  */
            cellgrpname[ncellgrps] = (char *)malloc(33*sizeof(char));
            if (cellgrpname[ncellgrps] == NULL) memerr();
            strcpy(cellgrpname[ncellgrps],tmpname);
            *(cellgrpname[ncellgrps]+32) = (char) 0;

            /*  Get the cells and faces for this side.  */
            elem_list = (int *)malloc(nsides*sizeof(int));
            face_list = (int *)malloc(nsides*sizeof(int));
            ierr = ex_get_side_set(exoid, ids[i], elem_list, face_list);

            /*  Find duplicates in elem_list.  */
            for (j = 0; j < ncells; j++) cctr[j] = 0;
            for (j = 0; j < nsides; j++)
              {
               icell = elem_list[j] - 1;
               cctr[icell] = 1;
              }

            /*  Save cell groups.  */
            k = 0;
            for (j = 0; j < ncells; j++) k += cctr[j];
            numcellgrp[ncellgrps] = k;
            cellgrps[ncellgrps] = (int *)malloc((k)*sizeof(int));
            if (cellgrps[ncellgrps] == NULL) memerr();
            k = 0;
            for (j = 0; j < ncells; j++)
              {
               if (cctr[j])
                 {
                  cellgrps[ncellgrps][k] = j+1;
                  k++;
                 }
              }
            ncellgrps++;

            /*  Save the face groups and make surface elements.  */
            
            l = 0;
            for (j = 0; j < nsides; j++)
              {

               /*  Get the cell no.  */
               icell = elem_list[j] - 1;

               /*  Get the face no. and save for 3D cell.  */
               if (cells[icell].nfaces > 1) l++;

               /*  Save the face as a surface element.  */
               k = face_list[j] - 1;

               /*  Save face of 3D object.  */
               if (cells[icell].nfaces > 1)
                 {
                  iface = cells[icell].facenos[k];
                  k = cellfaces[iface].nverts;
                  surfs[nsurf].nverts = k;
                  surfs[nsurf].fverts = (int *)malloc(k*sizeof(int));
                  for (l = 0; l < k; l++)
                    {
                     surfs[nsurf].fverts[l] = cellfaces[iface].fverts[l];
                    }
                 }

               /*  Save edge of a 2D object.  */
               if (cells[icell].nfaces == 1)
                 {
                  iface = cells[icell].facenos[0];
                  surfs[nsurf].nverts = 2;
                  surfs[nsurf].fverts = (int *)malloc(2*sizeof(int));
                  if (cellfaces[iface].nverts == 1)
                    {
                     i1 = i2 = 0;
                    }
                  else if (cellfaces[iface].nverts == 2)
                    {
                     i1 = 0; i2 = 1;
                    }
                  else if (cellfaces[iface].nverts == 3)
                    {
                     i1 = tri_1[k];  i2 = tri_2[k];
                    }
                  else
                    {
                     i1 = quad_1[k];  i2 = quad_2[k];
                    }
                  surfs[nsurf].fverts[0] = cellfaces[iface].fverts[i1];
                  surfs[nsurf].fverts[1] = cellfaces[iface].fverts[i2];
                 }

               /*  Save material data.  */
               surfmats[nsurf] = mmats + 1;
               nsurf++;
              }
            if (l > 0)
              {

               /*  Save face group name.  */
               facegrpname[nfacegrps] = (char *)malloc(33*sizeof(char));
               if (facegrpname[nfacegrps] == NULL) memerr();
               strcpy(facegrpname[nfacegrps],tmpname);
               *(facegrpname[nfacegrps]+32) = (char) 0;
 
               /*  Save the number and data for this face group.  */
               numfacegrp[nfacegrps] = nsides;
               facegrps[nfacegrps] = (int *)malloc((nsides)*sizeof(int));
               if (facegrps[nfacegrps] == NULL) memerr();

               l = 0;
               for (j = 0; j < nsides; j++)
                 {

                  /*  Get the cell no.  */
                  icell = elem_list[j] - 1;

                  /*  Get the face no. and save for 3D cell.  */
                  if (cells[icell].nfaces > 1)
                    {
                     k = face_list[j] - 1;
                     iface = cells[icell].facenos[k];
                     facegrps[nfacegrps][l] = iface + 1;
                     l++;
                    }
                 }
               nfacegrps++;

              }
            free(elem_list);  free(face_list);
            mmats++;
           }
         free(cctr);
        }
      maxsurfmat = mmats;
      free(ids);
     }



   /*  Make a boundary flag.  */
   ntypes = 3;
   flagname_node[numflags_node] = (char *)malloc(33*sizeof(char));
   if (flagname_node[numflags_node] == NULL) memerr();
   strcpy(flagname_node[numflags_node],"boundary");
   *(flagname_node[numflags_node]+32) = (char) 0;
   flagtypes_node[numflags_node] = (short) ntypes;

   for (i = 0; i < ntypes; i++)
      {
       flagnames_node[numflags_node][i]=
          (char *)malloc(33*sizeof(char));
       if (i == 0) strcpy(flagnames_node[numflags_node][i],"none");
       if (i == 1) strcpy(flagnames_node[numflags_node][i],"interface");
       if (i == 2) strcpy(flagnames_node[numflags_node][i],"exterior");
       *(flagnames_node[numflags_node][i]+32) = (char) 0;
      }

   flags[numflags_node] = (short *)malloc((nnodes+1)*sizeof(short));
   if (flags[numflags_node] == NULL) memerr();
   for (i = 0; i < nnodes+1; i++)
      flags[numflags_node][i] = (short)flags_in[i];

   numflags_node++;

   FREE(flags_in);
}


void bld_vars_exo(int *elem_map, int *blk_ids)
{
  /*  Build node and cell variable field data from Exodus var data.  */

  int ierr, i, j, k, l;
  int maxvars, nvars, *num_map, nelem_blk;
  short *ctest;
  double *vfield, *efld, fmin, fmax, fillval;
  char **tmpname;

   /*  Deterine max. no. of node or cell variables.  */
   ierr = ex_get_var_param(exoid, "n", &i);
   ierr = ex_get_var_param(exoid, "e", &j);
   maxvars = (i > j) ? i : j;

   if ((i+j) <= 0) return;

   if (!noprintflag) printf("GMV reading variables.\n");

   /*  Allocate name array and field data array.  */
   tmpname = (char **)malloc(maxvars * sizeof(* tmpname));
   for (i = 0; i < maxvars; i++)
      tmpname[i] = (char *)malloc(MAX_STR_LENGTH + 1);

   i = nnodes + 1;
   if (ncells > i) i = nnodes;
   vfield = (double *)malloc(i*sizeof(double));
   if (vfield == NULL) memerr();

   /*  Get node field data.  */
   ierr = ex_get_var_param(exoid, "n", &nvars);
   if (nvars > 0)
     {

      /*  Get the variable names.  */
      ierr = ex_get_var_names(exoid, "n", nvars, tmpname);

      /*  Loop through node variables.  */
      for (i = 0; i < nvars; i++)
        {

         /*  Set the variable name.  */ 
         fieldname[numvars]=(char *)malloc(33*sizeof(char));
         if (fieldname[numvars] == NULL) memerr();
         strncpy(fieldname[numvars],tmpname[i],32);
         *(fieldname[numvars]+32) = (char) 0;

         if (!noprintflag) printf("    %s  (nodes)\n",fieldname[numvars]);

         /*  Get node variable data for this time step and write.  */
         ierr = ex_get_nodal_var(exoid, exo_time_step, i, nnodes, vfield);
         writenodefld(vfield);
         numvars++;
        }
     }

   /*  Clear tmpname.  */
   for (i = 0; i < maxvars; i++)  strcpy(tmpname[i]," ");

   /*  Get the cell variables, if any.  */
   ierr = ex_get_var_param(exoid, "e", &nvars);
   if (nvars > 0)
     {

      /*  Get the variable names.  */
      ierr = ex_get_var_names(exoid, "e", nvars, tmpname);

      /*  Allocate array to hold variable for a block.  */
      efld = (double *)malloc(max_elem_per_blk * sizeof(double));

      /*  Allocate array to determine if data is available for all cells.  */
      ctest = (short *)malloc(ncells * sizeof(int));

      /*  Loop through cell variables.  */
      for (i = 0; i < nvars; i++)
        {

         /*  Set the variable name.  */ 
         cfieldname[cnumvars]=(char *)malloc(33*sizeof(char));
         if (cfieldname[cnumvars] == NULL) memerr();
         strncpy(fieldname[cnumvars],tmpname[i],32);
         *(cfieldname[cnumvars]+32) = (char) 0;

         if (!noprintflag) printf("    %s  (cells)\n",cfieldname[cnumvars]);

         /*  Loop through the blocks.  */     
         for (j = 0; j < ncells; j++) ctest[j] = 1;
         fmin = MAXVAL;
         fmax = -MAXVAL;
         for (j = 0; j < num_blks; j++)
           {
            ierr = ex_get_elem_var(exoid, exo_time_step, i, blk_ids[j],
                                   nelem_blk, efld);
            if (ierr == 0)
              {
               for (k = 0; k < nelem_blk; k++)
                 {
                  fmin = (efld[k] < fmin) ? efld[k] : fmin;
                  fmax = (efld[k] > fmax) ? efld[k] : fmax;
                  l = elem_map[k] - 1;
                  vfield[l] = efld[k];
                  ctest[l] = 0;
                 }
              }
           }

         /*  See if any data is missing for this variable.  */
         /*  If so, treat the variable as a subvar.         */
         l = 0;
         for (k = 0; k < ncells; k++) l += ctest[k];
         if (l > 0)
           {
            csubvarsin[cnumvars] = 1;
            if (fmin >= 1.) fillval = 0;
            else fillval = -MAXVAL;
            for (j = 0; j < ncells; j++) if (ctest[j]) vfield[j] = fillval;
            cfieldmin[cnumvars] = fmin;
            cfieldmax[cnumvars] = fmax;
           }

         writecellfld(vfield);
         cnumvars++;
        }
      FREE(ctest);  FREE(efld);
     }

   FREE(vfield);
   for (i = 0; i < maxvars; i++)  FREE(tmpname[i]);
   FREE(tmpname);

   /*  Get the node and cell ids, if available.  */
   i = nnodes + 1;
   if (ncells > i) i = nnodes;
   num_map = (int *)malloc(i*sizeof(int));

   /*  Get node ids.  */
   ierr = ex_get_node_num_map(exoid, num_map);
   if (ierr == 0)
     {
      if (!noprintflag) printf("GMV reading node ids.\n");

      /*  Allocate node ids.  */
      nodeids = (int *)malloc((nnodes+1)*sizeof(int));
      if (nodeids == NULL) memerr();

      /*  Set the node ids.  */
      nodeids[0] = 0;
      for (i = 0; i < nnodes; i++)
         nodeids[i+1] = num_map[i];

      nodeidflag = 1;
     }

   /*  Get cell ids.  */
   ierr = ex_get_elem_num_map(exoid, num_map);
   if (ierr == 0)
     {
      if (!noprintflag) printf("GMV reading cell ids.\n");

      /*  Allocate cell ids.  */
      cellids = (int *)malloc((ncells)*sizeof(int));
      if (cellids == NULL) memerr();

      /*  Set the cell ids.  */
      for (i = 0; i < ncells; i++)
         cellids[i] = num_map[i];

      cellidflag = 1;
     }
   
}


void convert_exo2gmv(char exo_reg[MAX_STR_LENGTH+1], int np, char gmv_reg[9])
{
  int ifound;

   ifound = 0;

   if (strncmp(exo_reg,"TRUSS",5) == 0 || strncmp(exo_reg,"BEAM",4) == 0 || 
       strncmp(exo_reg,"SHELL",5) == 0 || strncmp(exo_reg,"truss",5) == 0 || 
       strncmp(exo_reg,"beam",4) == 0 || strncmp(exo_reg,"shell",5) == 0)
     {
      if (np == 2) strcpy(gmv_reg,"line");
      if (np == 3) strcpy(gmv_reg,"3line");
      ifound = 1;
     }
   if (strncmp(exo_reg,"QUAD",4) == 0 || strncmp(exo_reg,"SHELL",5) == 0 || 
       strncmp(exo_reg,"quad",4) == 0 || strncmp(exo_reg,"shell",5) == 0) 
     {
      if (np == 4) strcpy(gmv_reg,"quad");
      if (np >= 8) strcpy(gmv_reg,"8quad");
      ifound = 1;
     }
   if (strncmp(exo_reg,"TRI",3) == 0 || strncmp(exo_reg,"tri",3) == 0)
     {
      if (np == 3) strcpy(gmv_reg,"tri");
      if (np >= 6) strcpy(gmv_reg,"6tri");
      ifound = 1;
     }
   if (strncmp(exo_reg,"PYRAMID",7) == 0 || strncmp(exo_reg,"pyramid",7) == 0)
     {
      if (np == 5) strcpy(gmv_reg,"ppyrmd5");
      if (np == 13) strcpy(gmv_reg,"ppyrmd10");
      ifound = 1;
     }
   if (strncmp(exo_reg,"TETRA",5) == 0 || strncmp(exo_reg,"tetra",5) == 0)
     {
      if (np == 4) strcpy(gmv_reg,"ptet4");
      if (np >= 10) strcpy(gmv_reg,"ptet10");
      ifound = 1;
     }
   if (strncmp(exo_reg,"WEDGE",5) == 0 || strncmp(exo_reg,"wedge",5) == 0)
     {
      if (np == 6) strcpy(gmv_reg,"pprism6");
      if (np >= 15) strcpy(gmv_reg,"pprism15");
      ifound = 1;
     }
   if (strncmp(exo_reg,"HEX",3) == 0 || strncmp(exo_reg,"hex",3) == 0)
     {
      if (np == 8) strcpy(gmv_reg,"phex8");
      if (np >= 20) strcpy(gmv_reg,"phex20");
      ifound = 1;
     }

   if (!ifound)
     {
      printf("Error, cannot identify Exodus element type %s\n",exo_reg);
      gmvexit();
     }
}


void regcell_exo(long icell, char *gmv_reg, int np, int node_list[30])
{
  /*                                                       */
  /*  Read and set data for a regular cell.                */
  /*  input:                                               */
  /*    icell - cell number                                */
  /*    gmv_reg - cell type                                */
  /*    np - number of vertices                            */
  /*    node_list - list of nodes                          */
  /*                                                       */
  /*  icelltype indicates the cell type,                   */
  /*    1-tri (triangle), 2-quad,  3-tet, 4-pyramid,       */
  /*    5-prism, 6-hex, 7-line, 8-phex8, 9-phex20,         */
  /*    10-pprymd5, 11-pprymd13, 12-pprism6, 13-pprism15,  */
  /*    14-ptet4, 15-ptet10, 16-6tri, 17-8quad,            */
  /*    18-3line, 19-phex27, 20-pentagon, 21-pentprsm,     */
  /*    22-ppentprsm                                       */
  /*                                                       */
  long i, j, k, cnodes[30], fverts[145], l1, l2, i1; 
  int nfaces, nverts[144], totverts,  dupflag, ncnodes,
      dupverts[145], dupnverts[145], ndup, nf;
  int icelltype;
  char ckeyword[9];
  short trinverts[1] = {3};
  short trifverts[3] = {1,2,3};
  short quadnverts[1] = {4};
  short quadfverts[4] = {1,2,3,4};
  short tetnverts[4] = {3,3,3,3};
  short tetfverts[12] = {1,2,3, 1,3,4, 1,4,2, 4,3,2};
  short pyrnverts[5] = {3,3,3,3,4};
  short pyrfverts[16] = {1,2,3, 1,3,4, 1,4,5, 1,5,2, 5,4,3,2};
  short prsnverts[5] = {3,4,4,4,3};
  short prsfverts[18] = {1,2,3, 1,4,5,2 ,2,5,6,3, 1,3,6,4, 6,5,4};
  short hexnverts[6] = {4,4,4,4,4,4};
  short hexfverts[24] = {1,2,3,4, 1,5,6,2, 2,6,7,3, 3,7,8,4,
                         4,8,5,1, 8,7,6,5};
  short linenverts[1] = {2};
  short linefverts[2] = {1,2};
  short phex8nverts[6] = {4,4,4,4,4,4};
  short phex8fverts[24] = {1,2,6,5, 2,3,7,6, 3,4,8,7, 4,1,5,8,
                           1,4,3,2, 5,6,7,8};
  short phex20nverts[6] = {8,8,8,8,8,8};
  short phex20fverts[48] = {1,9,2,14,6,17,5,13, 2,10,3,15,7,18,6,14,
                            3,11,4,16,8,19,7,15, 4,12,1,13,5,20,8,16,
                            1,12,4,11,3,10,2,9, 5,17,6,18,7,19,8,20};
  short pyr5nverts[5] = {3,3,3,3,4};
  short pyr5fverts[16] = {1,2,5, 2,3,5, 3,4,5, 4,1,5, 1,4,3,2};
  short pyr13nverts[5] = {6,6,6,6,8};
  short pyr13fverts[32] = {1,6,2,11,5,10, 2,7,3,12,5,11, 
                           3,8,4,13,5,12, 4,9,1,10,5,13, 1,9,4,8,3,7,2};
  short prs6nverts[5] = {4,4,4,3,3};
  short prs6fverts[18] = {1,2,5,4 ,2,3,6,5, 1,4,6,3, 1,3,2, 4,5,6};
  short prs15nverts[5] = {8,8,8,6,6};
  short prs15fverts[36] = {1,7,2,11,5,13,4,10, 2,8,3,12,6,14,5,11,
                           3,9,1,10,4,15,6,12, 1,9,3,8,2,7, 4,13,5,14,6,15};
  short tet4nverts[4] = {3,3,3,3};
  short tet4fverts[12] = {1,2,4, 2,3,4, 3,1,4, 1,3,2,};
  short tet10nverts[4] = {6,6,6,6};
  short tet10fverts[24] = {1,5,2,9,4,8, 2,6,3,10,4,9, 
                           3,7,1,8,4,10, 1,7,3,6,2,5};
  short quad8nverts[1] = {8};
  short quad8fverts[8] = {1,5,2,6,3,7,4,8};
  short tri6nverts[1] = {6};
  short tri6fverts[6] = {1,4,2,5,3,6};
  short line3nverts[2] = {2,2};
  short line3fverts[4] = {1,2, 2,3};
  short phex27nverts[48] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                            3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
  short phex27fverts[144] = {1,9,21,  9,2,21,  2,18,21, 18,6,21, 
                             6,13,21, 13,5,21, 5,17,21, 17,1,21,
                             2,10,22, 10,3,22, 3,19,22, 19,7,22,
                             7,14,22, 14,6,22, 6,18,22, 18,2,22,
                             3,11,23, 11,4,23, 4,20,23, 20,8,23,
                             8,15,23, 15,7,23, 7,19,23, 19,3,23,
                             4,12,24, 12,1,24, 1,17,24, 17,5,24,
                             5,16,24, 16,8,24, 8,20,24, 20,4,24,
                             1,12,25, 12,4,25, 4,11,25, 11,3,25,
                             3,10,25, 10,2,25, 2,9,25,  9,1,25,
                             5,13,26, 13,6,26, 6,14,26, 14,7,26,
                             7,15,26, 15,8,26, 8,16,26, 16,5,26};
  short pentnverts[1] = {5};
  short pentfverts[5] = {1,2,3,4,5};
  short pentprsnverts[7] = {5,4,4,4,4,4,5};
  short pentprsfverts[30] = {1,2,3,4,5, 1,6,7,2, 2,7,8,3, 3,8,9,4, 
                             4,9,10,5, 5,10,6,1, 6,10,9,8,7};
  short ppentprsnverts[7] = {5,4,4,4,4,4,5};
  short ppentprsfverts[30] = {1,5,4,3,2, 1,2,7,6, 2,3,8,7, 3,4,9,8,
                              4,5,10,9, 5,1,6,10, 6,7,8,9,10};
  short *nv, *fv;

   /*  Determine cell type.  */
   strcpy(ckeyword,gmv_reg);
   if (strncmp(ckeyword,"tri",3) == 0) icelltype = 1;
   else if (strncmp(ckeyword,"quad",4) == 0) icelltype = 2;
   else if (strncmp(ckeyword,"tet",3) == 0) icelltype = 3;
   else if (strncmp(ckeyword,"hex",3) == 0) icelltype = 4;
   else if (strncmp(ckeyword,"prism",5) == 0) icelltype = 5;
   else if (strncmp(ckeyword,"pyramid",7) == 0) icelltype = 6;
   else if (strncmp(ckeyword,"line",4) == 0) icelltype = 7;
   else if (strncmp(ckeyword,"phex8",5) == 0) icelltype = 8;
   else if (strncmp(ckeyword,"phex20",6) == 0) icelltype = 9;
   else if (strncmp(ckeyword,"ppyrmd5",7) == 0) icelltype = 10;
   else if (strncmp(ckeyword,"ppyrmd13",8) == 0) icelltype = 11;
   else if (strncmp(ckeyword,"pprism6",7) == 0) icelltype = 12;
   else if (strncmp(ckeyword,"pprism15",8) == 0) icelltype = 13;
   else if (strncmp(ckeyword,"ptet4",5) == 0) icelltype = 14;
   else if (strncmp(ckeyword,"ptet10",6) == 0) icelltype = 15;
   else if (strncmp(ckeyword,"6tri",4) == 0) icelltype = 16;
   else if (strncmp(ckeyword,"8quad",5) == 0) icelltype = 17;
   else if (strncmp(ckeyword,"3line",5) == 0) icelltype = 18;
   else if (strncmp(ckeyword,"phex27",6) == 0) icelltype = 19;
   else if (strncmp(ckeyword,"pentagon",8) == 0) icelltype = 20;
   else if (strncmp(ckeyword,"pentprsm",7) == 0) icelltype = 21;
   else if (strncmp(ckeyword,"ppentprsm",8) == 0) icelltype = 22;

   /*  Set face information according to cell type.  */
   switch (icelltype)
     {
      case 1: nfaces = 1;    /* tri */
              totverts = 3;
              nv = trinverts;
              fv = trifverts;
              break;
      case 2: nfaces = 1;    /* quad */
              totverts = 4;
              nv = quadnverts;
              fv = quadfverts;
              break;
      case 3: nfaces = 4;    /* tet */
              totverts = 12;
              nv = tetnverts;
              fv = tetfverts;
              break;
      case 4: nfaces = 6;    /* hex */
              totverts = 24;
              nv = hexnverts;
              fv = hexfverts;
              break;
      case 5: nfaces = 5;    /* prism */
              totverts = 18;
              nv = prsnverts;
              fv = prsfverts;
              break;
      case 6: nfaces = 5;    /* pyramid */
              totverts = 16;
              nv = pyrnverts;
              fv = pyrfverts;
              break;
      case 7: nfaces = 1;    /* line */
              totverts = 2;
              nv = linenverts;
              fv = linefverts;
              break;
      case 8: nfaces = 6;    /* phex8 */
              totverts = 24;
              nv = phex8nverts;
              fv = phex8fverts;
              break;
      case 9: nfaces = 6;    /* phex20 */
              totverts = 48;
              nv = phex20nverts;
              fv = phex20fverts;
              break;
      case 10: nfaces = 5;    /* ppyrmd5, patran 5 point pyramid */
               totverts = 16;
               nv = pyr5nverts;
               fv = pyr5fverts;
               break;
      case 11: nfaces = 5;    /* ppyrmd13, patran 13 point pyramid */
               totverts = 32;
               nv = pyr13nverts;
               fv = pyr13fverts;
               break;
      case 12: nfaces = 5;    /* pprism6, patran 6 point prism */
               totverts = 18;
               nv = prs6nverts;
               fv = prs6fverts;
               break;
      case 13: nfaces = 5;    /* pprism15, patran 15 point prism */
               totverts = 36;
               nv = prs15nverts;
               fv = prs15fverts;
               break;
      case 14: nfaces = 4;    /* ptet4, patran tet */
               totverts = 12;
               nv = tet4nverts;
               fv = tet4fverts;
               break;
      case 15: nfaces = 4;    /* ptet10, patran tet */
               totverts = 24;
               nv = tet10nverts;
               fv = tet10fverts;
               break;
      case 16: nfaces = 1;    /* 6tri */
               totverts = 6;
               nv = tri6nverts;
               fv = tri6fverts;
               break;
      case 17: nfaces = 1;    /* 8quad */
               totverts = 8;
               nv = quad8nverts;
               fv = quad8fverts;
               break;
      case 18: nfaces = 2;    /* 3line, 3 point line */
               totverts = 4;
               nv = line3nverts;
               fv = line3fverts;
               break;
      case 19: nfaces = 48;    /* phex27, 27 point hex */
               totverts = 144;
               nv = phex27nverts;
               fv = phex27fverts;
               break;
      case 20: nfaces = 1;    /* pentagon */
               totverts = 5;
               nv = pentnverts;
               fv = pentfverts;
               break;
      case 21: nfaces = 7;    /* pentagon prism */
               totverts = 10;
               nv = pentprsnverts;
               fv = pentprsfverts;
               break;
      case 22: nfaces = 7;    /* patran pentagon prism */
               totverts = 10;
               nv = ppentprsnverts;
               fv = ppentprsfverts;
               break;
      default: break;
     }

   /*  Get cell nodes.  */
   ncnodes = np;
   for (i = 0; i < ncnodes; i++)
      cnodes[i] =  node_list[i];;

   /*  Check for duplicate nodes.  */
   dupflag = 0;
   for (i = 0; i < ncnodes-1; i++)
     {
      for (j = i+1; j < ncnodes; j++)
        {
         if (cnodes[i] == cnodes[j])
            dupflag = 1;
         }
     }

   /*  Build face information, realloc if needed.  */
   if (ncellfaces + nfaces > nfaces_alloc)
     {
      nfaces_alloc += 1000;
      cellfaces = (struct cellface *)realloc(cellfaces, nfaces_alloc *
                                             sizeof(struct cellface));
     }
   for (i = 0; i < nfaces; i++)
      nverts[i] = nv[i];

   /*  Build face vertices.  */
   for (i = 0; i < totverts; i++)
     {
      j = fv[i] - 1;
      fverts[i] = cnodes[j];
     }

   /*  If duplicate nodes exist, check   */ 
   /*  and adjust for degenerate faces.  */
   if (dupflag)
     {
      ndup = 0;
      k = 0;
      nf = 0;
      for (i = 0; i < nfaces; i++)
        {
         if (i > 0) k += nverts[i-1];
         dupnverts[nf] = nverts[i];
         for (j = 0; j < nverts[i]; j++)
           {
            l1 = k + j;
            l2 = l1 + 1;
            if (j == nverts[i] - 1) l2 = k;
            if (fverts[l1] != fverts[l2])
              {
               dupverts[ndup] = fverts[l1];
               ndup++;
              }
            else
              {
               dupnverts[nf]--;
              }
           }
         if (dupnverts[nf] > 2) nf++;
         else ndup -= dupnverts[nf];
        }

      /*  If cell consists of all duplicate nodes, set to first node.  */
      if (ndup <= 0)
        {
         dupverts[0] = fverts[0];
         dupnverts[0] = 1;
        }

      nfaces = nf;
      if (nfaces == 0) nfaces = 1;
      totverts = 0;
      for (i = 0; i < nfaces; i++)
       {
        nverts[i] = dupnverts[i];
        totverts += dupnverts[i];
       }
      for (i = 0; i < totverts; i++)
        fverts[i] = dupverts[i];
     }

   /*  Save cell data.  */
   cells[icell].ncnodes = ncnodes;
   cells[icell].cnodes = (int *)malloc(ncnodes*sizeof(int));
   if (cells[icell].cnodes == NULL) memerr();
   for (j = 0; j < ncnodes; j++)
     {
      cells[icell].cnodes[j] = cnodes[j];
     }
   cells[icell].nfaces = nfaces;
   cells[icell].facenos = (int *)malloc(nfaces*sizeof(int));
   if (cells[icell].facenos == NULL) memerr();
   for (j = 0; j < nfaces; j++)
     {
      cells[icell].facenos[j] = ncellfaces + j;
     }

   /*  Save face data.  */
   i1 = 0;
   for (i = 0; i < nfaces; i++)
     {
      k = nverts[i];
      cellfaces[ncellfaces].nverts = k;
      cellfaces[ncellfaces].fverts = (int *)malloc(k*sizeof(int));
      if (cellfaces[ncellfaces].fverts == NULL) memerr();
      for (j = 0; j < k; j++)
         cellfaces[ncellfaces].fverts[j] = fverts[i1+j];
      cellfaces[ncellfaces].cell1 = icell;
      cellfaces[ncellfaces].cell2 = -1;
      cellfaces[ncellfaces].oppface = -1;
      ncellfaces++;
      i1 += k;
     }
}


void getcellfaces_exo()
{
  int i, ii, j, k, l, n, kk, ibin, jface, kface, jverts, kverts;
  int  nverts, jsum, ksum, *oppface, newno, binavg, *binno, *nfacesperbin;
  unsigned int **bins, nbins;
  unsigned int nfacesin;
  short *test, dupface;

   /*                                 */
   /*  Fill cell and face structures  */
   /*  and find duplicate faces.      */
   /*                                 */
   if (!noprintflag)
      printf("GMV searching for duplicate faces.\n");

   nfacesin = ncellfaces;

   /*  Calculate the number of bins to use, based on the number of nodes.  */
   if (nnodes < 65536) nbins = nnodes + 1;
   else if (nnodes/4 < 65536) nbins = 65536;
   else nbins = nnodes / 4 + 1;
   bins = (unsigned int **)malloc(nbins*sizeof(unsigned int *));
   if (bins == NULL) memerr();
   
   /*  Allocate and calculate bin no for each face.    */
   /*  Bin no. is calculated by multiplying a modulo   */
   /*  of the face numbers then taking the mod(nbins)  */
   /*  of the product.  Then count faces per bin.      */
   nfacesperbin = (int *)malloc(nbins*sizeof(int));
   if (nfacesperbin == NULL) memerr();
   for (i = 0; i < nbins; i++) nfacesperbin[i] = 0;
   oppface = (int *)malloc(nfacesin*sizeof(int));
   binno = (int *)malloc(nfacesin*sizeof(int));
   if (binno == NULL || oppface == NULL) memerr();
   for (i = 0; i < nfacesin; i++)
     {
      ibin = nnodes + 1;
      nverts = cellfaces[i].nverts;
      for (j = 0; j < nverts; j++)
        {
         l = cellfaces[i].fverts[j];
         if (ibin > l) ibin = l;
        }
      ibin = ibin % nbins;
      binno[i] = ibin;
      nfacesperbin[ibin]++;
     }

   /*  Get average faces per bin.  */
   jsum = 0;
   for (i = 0; i < nbins; i++)
     {
      k = nfacesperbin[i];
      if (k > 0) jsum++;
     }
   binavg = nfacesin / jsum;
   for (ii = 0; ii < nbins; ii++)
     {
      kk = nfacesperbin[ii];
      if (kk > 20*binavg)
        {
         nfacesperbin[ii] = 0;
         for (i = 0; i < nfacesin; i++)
           {
            if (binno[i] == ii)
              {
               ibin = 0;
               nverts = cellfaces[i].nverts;
               for (j = 0; j < nverts; j++)
                 {
                  l = cellfaces[i].fverts[j];
                  if (ibin < l) ibin = l;
                 }
               ibin = ibin % nbins;
               binno[i] = ibin;
               nfacesperbin[ibin]++;
              }
           }
        }
     }  

   /*  Allocate  and fill bins.  Then use  */
   /*  binno array as new face no. list.   */
   for (i = 0; i < nbins; i++)
     {
      k = nfacesperbin[i];
      if (k > 0)
        {
         bins[i] = (unsigned int *)malloc(k*sizeof(unsigned int));
         if (bins[i] == NULL) memerr();
         nfacesperbin[i] = 0;
        }
     }
   for (j = 0; j < nfacesin; j++)
     {
      i = binno[j];
      l = nfacesperbin[i];
      bins[i][l] = j;
      nfacesperbin[i]++;
      binno[j] = -1;
      oppface[j] = -1;
     }

   /*  Allocate and initialize node size test array.  */
   test = (short *)malloc((nnodes+1)*sizeof(short));
   if (test == NULL) memerr();
   for (i = 0; i < nnodes+1; i++) test[i] = 0;

   /*  Loop through bins and faces per bins to find duplicates.  */
   ncellfaces = 0;
   for (i = 0; i < nbins; i++)
     {
      kk = nfacesperbin[i];
      for (j = 0; j < kk; j++)
        {
         jface = bins[i][j];
         newno = binno[jface];

         /*  If this face has not been altered,  */
         /*  add to the list and continue.       */
         if (newno == -1)
           {
            binno[jface] = ncellfaces;
            ncellfaces++;

            /*  Fill test array node nos. from this  */ 
            /*  face with 1 and sum face nodes.     */
            jverts = cellfaces[jface].nverts;
            jsum = 0;
            for (l = 0; l < jverts; l++)
              {
               n = cellfaces[jface].fverts[l];
               test[n] = 1;
               jsum += n;
              }

            /*  Now loop through the rest of the      */
            /*  faces in the bin to find duplicates.  */
            /*  Quit the loop if a duplicate found.   */
            dupface = 0;
            k = j + 1;
            while (dupface == 0 && k < kk)
              {
               kface = bins[i][k];
               newno = binno[kface];
               dupface = 0;

               /*  If this face has not been reset, check no. of verts.  */
               if (newno == -1)
                 {
                  kverts = cellfaces[kface].nverts;

                  /*  If this face has the same no. of vertices and has  */
                  /*  not been reset, then check for duplicate nodes.    */
                  if (jverts == kverts && newno == -1)
                    {
                     dupface = 1;
                     l = 0;
                     ksum = 0;
                     while (dupface == 1 && l < kverts)
                       {
                        n = cellfaces[kface].fverts[l];
                        if (test[n] == 0) dupface = 0;
                        ksum += n;
                        l++;
                       }

                     /*  If face nodes match, check sums.  */
                     if (dupface == 1 && (jsum != ksum))
                        dupface = 0;

                     /*  If face nodes match, set new face no.   */
                     /*  in binno array and set opposite faces.  */
                     if (dupface == 1)
                      {
                        binno[kface] = ncellfaces - 1;
                        oppface[jface] = kface;
                        oppface[kface] = jface;
                       }
                    }
                 }
               k++;
              }

            /*  Reset test array node nos. to 0.  */ 
            jverts = cellfaces[jface].nverts;
            for (l = 0; l < jverts; l++)
              {
               n = cellfaces[jface].fverts[l];
               test[n] = 0;
              }
           }
        }
     }
   if (!noprintflag)
      printf("GMV found %d unique faces.\n",ncellfaces);

   free(test);  free(binno);

   for (i = 0; i < nbins; i++)
     {
      if (nfacesperbin[i] > 0) free(bins[i]);
     }
   free(nfacesperbin);
   free(bins);

   /*  Set cellno2 for faces.  */
   ncellfaces = nfacesin;
   for (i = 0; i < ncellfaces; i++)
     {
      if (oppface[i] > -1)
        {
         k = oppface[i];
         cellfaces[i].cell2 = cellfaces[k].cell1;
         cellfaces[i].oppface = oppface[i];
        }
     }
   free(oppface);
}


void getcelledges_exo()
{
  int i, j, k, l, m, n, ii, kk, nedge, totverts, jpt1, jpt2, kpt1, kpt2;
  int *faceedges, *nedgesperface, *edgept1, *edgept2;
  int  ibin, *oldnos, newno, celledgenos[1000], binavg, isum, jsum, 
       *binno, *nedgesperbin;
  unsigned int **bins, nbins;
  int nverts, fverti, fvertj, jedge, kedge;
  char *tstedges;
  short *newface;

   /*                                                      */
   /*  Extract a set of edges from face data.              */
   /*  (The first edge point is always the smaller node.)  */
   /*  Then check for duplicates against the edge list.    */
   /*                                                      */
   if (!noprintflag)
      printf("GMV generating cell edges list.\n");

   /*  Determine max. no. of edges and allocate temporary arrays.  */
   newface = (short *)malloc(ncellfaces*sizeof(short));
   if (newface == NULL) memerr();
   for (i = 0; i < ncellfaces; i++)
      newface[i] = 1;

   for (i = 0; i < ncellfaces; i++)
     {
      if (newface[i] >= 0)
        {
         j = cellfaces[i].oppface;
         k = -1;
         if (j >= 0)
            k = cellfaces[j].oppface;
         if (j >= 0 && j > i && i == k) newface[j] = -1;
        }
     }
   totverts = 0;
   for (i = 0; i < ncellfaces; i++)
     {
      if (newface[i] >= 0)
         totverts += cellfaces[i].nverts;
     }
   faceedges = (int *)malloc(ncellfaces*sizeof(int));
   nedgesperface = (int *)malloc(ncellfaces*sizeof(int));
   if (faceedges == NULL || nedgesperface == NULL) memerr();
   oldnos = (int *)malloc(totverts*sizeof(int));
   binno = (int *)malloc(totverts*sizeof(int));
   edgept1 = (int *)malloc(totverts*sizeof(int));
   edgept2 = (int *)malloc(totverts*sizeof(int));
   if (oldnos == NULL || edgept1 == NULL || edgept2 == NULL)
      memerr();

   /*  Calculate the number of bins to use, based on the number of nodes.  */
   if (nnodes < 65536) nbins = nnodes + 1;
   else if (nnodes/4 < 65536) nbins = 65536;
   else nbins = nnodes / 4 + 1;
   bins = (unsigned int **)malloc(nbins*sizeof(unsigned int *));

   /*  Fill edgept arrays and calculate bin for each edge.  */
   nedgesperbin = (int *)malloc(nbins*sizeof(int));
   if (nedgesperbin == NULL) memerr();
   for (i = 0; i < nbins; i++) nedgesperbin[i] = 0;
   nedge = 0;
   for (k = 0; k < ncellfaces; k++)
     {
      if (newface[k] >= 0)
        {
         nverts = cellfaces[k].nverts;
         faceedges[k] = nedge;
         nedgesperface[k] = nverts;
         j = cellfaces[k].oppface;
         if (j >= 0 && newface[j] == -1)
           {
            faceedges[j] = nedge;
            nedgesperface[j] = nverts;
           }
         for (i = 0; i < nverts; i++)
           {
            j = i + 1;
            if (j == nverts) j = 0;
            fverti = cellfaces[k].fverts[i];
            fvertj = cellfaces[k].fverts[j];
            edgept1[nedge] = (fverti < fvertj) ? fverti : fvertj;
            edgept2[nedge] = (fverti > fvertj) ? fverti : fvertj;
            ibin = edgept1[nedge] % nbins;
            /* ibin = (fverti + fvertj) % nbins; */
            binno[nedge] = ibin;
            nedgesperbin[ibin]++;
            nedge++;
           }
        }
     }
   free(newface);

   /*  Get average edges per bin.  */
   jsum = 0;
   for (i = 0; i < nbins; i++)
     {
      k = nedgesperbin[i];
      if (k > 0) jsum++;
     }
   binavg = nedge / jsum;
   for (ii = 0; ii < nbins; ii++)
     {
      kk = nedgesperbin[ii];
      if (kk > 20*binavg)
        {
         nedgesperbin[ii] = 0;
         for (i = 0; i < nedge; i++)
           {
            if (binno[i] == ii)
              {
               ibin = edgept2[i] % nbins;
               binno[i] = ibin;
               nedgesperbin[ibin]++;
              }
           }
        }
     }  

   /*  Allocate  and fill bins.  Then use  */
   /*  binno array as new edge no. list.   */
   for (i = 0; i < nbins; i++)
     {
      k = nedgesperbin[i];
      if (k > 0)
        {
         bins[i] = (unsigned int *)malloc(k*sizeof(unsigned int));
         if (bins[i] == NULL) memerr();
         nedgesperbin[i] = 0;
        }
     }
   for (j = 0; j < nedge; j++)
     {
      i = binno[j];
      l = nedgesperbin[i];
      bins[i][l] = j;
      nedgesperbin[i]++;
      binno[j] = -1;
     }

   /*  Loop through bins and edges per bins to find duplicates.  */
   ncelledges = 0;
   for (i = 0; i < nbins; i++)
     {
      kk = nedgesperbin[i];
      for (j = 0; j < kk; j++)
        {
         jedge = bins[i][j];
         newno = binno[jedge];

         /*  If this edge has not been altered,  */
         /*  add to the list and continue.       */
         if (newno == -1)
           {
            oldnos[ncelledges] = jedge;
            binno[jedge] = ncelledges;
            ncelledges++;

            /*  Now loop through the rest of the      */
            /*  edges in the bin to find duplicates.  */
            jpt1 = edgept1[jedge];
            jpt2 = edgept2[jedge];
            for (k = j+1; k < kk; k++)
              {
               kedge = bins[i][k];
               newno = binno[kedge];

               /*  If this edge has not been reset, check edge points.  */
               if (newno == -1)
                 {
                  kpt1 = edgept1[kedge];
                  kpt2 = edgept2[kedge];

                  if (jpt1 == kpt1 && jpt2 == kpt2)
                    {

                     /*  If edge points match, reset  */
                     /*  face no. in binno array.     */
                     binno[kedge] = ncelledges - 1;
                    }
                 }
              }
           }
        }
     }

   for (i = 0; i < nbins; i++)
     {
      if (nedgesperbin[i] > 0) free(bins[i]);
     }
   free(nedgesperbin);
   free(bins);

   /*  Allocate and fill celledges structure.  */
   celledges = (struct celledge *)
                  malloc(ncelledges*sizeof(struct celledge));
   if (celledges == NULL) memerr();

   if (!noprintflag)
      printf("GMV found %d unique edges.\n",ncelledges);

   for (i = 0; i < ncelledges; i++)
     {
      j = oldnos[i];
      celledges[i].edgept1 = edgept1[j];
      celledges[i].edgept2 = edgept2[j];
     }
   free(edgept1);  free(edgept2);  free(oldnos);

   /*  Fill cell structure with edge nos. and node nos. */
   tstedges = (char *)malloc(ncelledges*sizeof(char));
   if (tstedges == NULL) memerr();
   for (i = 0; i < ncelledges; i++) tstedges[i] = 0;
   for (i = 0; i < ncells; i++)
     {
      nedge = 0;
      for (j = 0; j < cells[i].nfaces; j++)
        {
         k = cells[i].facenos[j];
         for (l = 0; l < nedgesperface[k]; l++)
           {
            m = faceedges[k] + l;
            n = binno[m];
            if (nedge == 0)
              {
               celledgenos[0] = n;
               tstedges[n] = 1;
               nedge++;
              }
            else
              {
/*
               iflg = 0;
               for (ii = 0; ii < nedge; ii++)
                  if (n == celledgenos[ii]) iflg = 1;
               if (iflg == 0)
*/
               if ((int)tstedges[n] == 0)
                 {
                  celledgenos[nedge] = n;
                  tstedges[n] = 1;
                  nedge++;
                 }
              }
           }
        }
      cells[i].nedges = nedge;
      cells[i].edgenos = (int *)malloc(nedge*sizeof(int));
      if (cells[i].edgenos == NULL) memerr();
      for (j = 0; j < nedge; j++)
        {
         cells[i].edgenos[j] = l = celledgenos[j];
         tstedges[l] = 0;
        }
     }
   free(faceedges);  free(nedgesperface);  free(binno);  free(tstedges);
}
