#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "gmvdata.h"
#include "x3dread.h"
#include "gmvread.h"

#define MSIZE 1000
#define MAXVAL 9.99999e32
#define direct  dirent

char save_dir[200];
extern char *fldfilenam, *cfldfilenam, *polyfilenam, *sfldfilenam, *ffldfilenam;
extern FILE *fieldfile, *cfieldfile, *polyfile, *sfieldfile, *ffieldfile;

void readx3d_error(char *filename);
void readx3dfaces(int nfacesin);
void readx3dcells(int nfacesin);
void bldcellfaces(int nfacesin);
void getx3dfaces(int nfacesin);
void getx3dcells();
void getx3dslaves(void);
void getx3dghosts(void);
void getx3dcelldata(void);
void getx3dnodedata(void);
void getx3dBdyfiles(void);
void getx3dRegfiles(void);

int has_suffix;
char x3dprefix[200];
char x3dsuffix[200];


void readx3d_error(char *filename)
{
   fprintf(stderr,"Error reading x3d file %s\n",filename);
   gmvexit();
}


void readx3d()
{
  /*                          */
  /*  Read a X3D input file.  */
  /*                          */
  int iend, i, j, ierr, nodes_read, ndims, nfacesin, foundx3d, len;
  char *tok, filename[200], tstchar;

   ierr = x3dread_open(filnam);

   if (ierr > 0) readx3d_error(filnam);
   printf("GMV reading file %s\n",filnam);

   /*  Extract the directory path from the file name.  */
   /*  Find the last /, then copy the directory name.  */
   len = strlen(filnam);
   for (i = 0; i < len; i++)
     {
      sscanf(&filnam[i], "%c", &tstchar);
      if (tstchar == '/') j = i;
     }
   strncpy(save_dir,filnam,j+1);

   /*  Eliminate the directory portion of the filename.  */
   i = strlen(save_dir);
   strcpy(filename,&filnam[i]);

   /*  Extract file prefix and suffix for Bdy and Rgn file search.  */
   tok =strtok(filename,".");
   strcpy(x3dprefix,tok);
   foundx3d = 0;
   while (!foundx3d)
     {
      tok = strtok(NULL,".");
      if (tok == NULL) break;
      if (strcmp(tok,"x3d") == 0) foundx3d = 1;
     }
   has_suffix = 0;
   if (foundx3d)
     {
      tok = strtok(NULL,".");
      if (tok != NULL)
        {
         has_suffix = 1;
         strcpy(x3dsuffix,tok);
        }
     }
   

   /*  Initialize data and reopen database files.  */
   readgmv_init();

   /*  Read and save the X3D header data.  */
   ierr = x3dread_header();
   if (ierr > 0) readx3d_error(filnam);
   if (!noprintflag) printf("GMV read X3d file header.\n");
   ndims = x3d_header.numdim;
   mmats = x3d_header.nummats;
   nnodes = x3d_header.numnodes;
   nfacesin = x3d_header.numfaces;
   ncells = x3d_header.numcells;

   /*  Read the material names.  */
   ierr = x3dread_matnames();
   if (ierr > 0) readx3d_error(filnam);

   /*  Allocate and save 32 character material names.  */
   if (!noprintflag) printf("GMV read %d cell materials.\n",mmats);
   for (i = 0; i < mmats; i++)
     {
      mmatnames[i] = (char *)malloc(mmats*33*sizeof(char));
      strncpy(mmatnames[i],x3dmatnames[i],32);
      if (!noprintflag) printf("    %s\n",mmatnames[i]);
     }
   mats_on_cells = 1;

   /*  Skip over mateos and matopc.  */
   if (x3dread_mateos() > 0) readx3d_error(filnam);
   if (x3dread_matopc() > 0) readx3d_error(filnam);

   /*  Read node coordinates.  */
   if (x3dread_nodes() > 0) readx3d_error(filnam);
   nodes = (struct node *)malloc((nnodes+1)*sizeof(struct node));
   if (nodes == NULL) memerr();
   nodes[0].x = 0.;  nodes[0].y = 0.;  nodes[0].z = 0.;
   for (i = 1; i < nnodes+1; i++)
     {
      nodes[i].x = x3d_x[i-1];
      nodes[i].y = x3d_y[i-1];
      nodes[i].z = x3d_z[i-1];
     }
   FREE(x3d_x);  FREE(x3d_y);  FREE(x3d_z);
   if (!noprintflag) printf("GMV read %d nodes.\n",nnodes);

   /*  Read faces.  */
   getx3dfaces(nfacesin);
   if (!noprintflag) printf("GMV read %d faces.\n",nfacesin);

   /*  Read cells.  */
   getx3dcells();
   if (!noprintflag) printf("GMV read %d cells.\n",ncells);

   /*  Complete face and cell info.  */
   bldcellfaces(nfacesin);

   /*  Read slave and master nodes and save as a node flag.  */
   getx3dslaves();
   if (!noprintflag) printf("GMV read %d slaves.\n",x3d_header.numslaves);

   /*  Read ghost nodes and save as a node group.  */
   getx3dghosts();
   if (!noprintflag) printf("GMV read %d ghosts.\n",x3d_header.numslaves);

   /*  Read cell data and save.  */
   getx3dcelldata();
   if (!noprintflag) 
      printf("GMV read %d cell data fields.\n",x3d_header.num_cell_fields);

   /*  Read node data and skip.  */
   getx3dnodedata();
   if (!noprintflag) 
      printf("GMV read %d node data fields.\n",x3d_header.num_node_fields);

   /*  Close the x3d file.  */
   ierr = x3dread_close();

   /*  Get bdy file data and save as node groups.  */
   getx3dBdyfiles();

   /*  Get rgn file data and save as cell groups.  */
   getx3dRegfiles();
}


void getx3dfaces(int nfacesin)
{
 /*                                                                  */
 /*  Read and save the X3D faces data in the faces structure array.  */
 /*                                                                  */
  int i, j, nfaces, nv;

   /*  Allocate the faces array, note, if a 2D problem, allocate  */
   /*  another ncells faces to be used as the cell drawing face.  */
   nfaces = nfacesin;
   if (x3d_header.numdim == 2) nfaces = nfaces + ncells;
   cellfaces = (struct cellface *)malloc(nfaces*sizeof(struct cellface));

   /*  Read the X3D face header, data, and trailer.  */
   if (x3dread_face_header() > 0) readx3d_error(filnam);
   for (i = 0; i < nfacesin; i++)
     {
      if (x3dread_faces() > 0) readx3d_error(filnam);

      /*  Save the data in the faces structure.  */
      cellfaces[i].nverts = x3d_face.nverts;
      cellfaces[i].nverts = nv = x3d_face.nverts;
      cellfaces[i].cell1 = -1;
      cellfaces[i].cell2 = -1;
      cellfaces[i].oppface = x3d_face.opp_face - 1;
      cellfaces[i].fverts = (int *)malloc(nv * sizeof(int));
      for (j = 0; j < nv; j++)
        {
         cellfaces[i].fverts[j] = x3d_face.verts[j];
        }
     }
   if (x3dread_face_end() > 0) readx3d_error(filnam);
}


void getx3dcells()
{
 /*                                                                 */
 /*  Read and save the X3D cells data in the cell structure array.  */
 /*                                                                 */
  int i, j, nf;

   /*  Allocate the cells array.  */
   cells = (struct cell *)malloc(ncells * sizeof(struct cell));

   /*  Read the X3D cell header, data, and trailer.  */
   if (x3dread_cell_header() > 0) readx3d_error(filnam);
   for (i = 0; i < ncells; i++)
     {
      if (x3dread_cells() > 0) readx3d_error(filnam);

      /*  Save the data in the faces structure.  */
      nf = x3d_cell.nfaces;
      if (x3d_header.numdim == 2) nf++;
      cells[i].facenos = (int *)malloc(nf * sizeof(int));
      nf = x3d_cell.nfaces;
      cells[i].nfaces = nf;
      for (j = 0; j < nf; j++)
        {
         cells[i].facenos[j] = x3d_cell.faces[j] - 1;
        }
     }
   if (x3dread_cell_end() > 0) readx3d_error(filnam);
}


void bldcellfaces(int nfacesin)
{
 /*                                           */
 /*  Finish building cell and cellface info.  */
 /*                                           */
  int i, j, k, l, n, nv, nf, ncnodes, cnodes[100];
  short *tstnodes;

   /*  Get the cellface cell 1 data from the cells.  */
   for (i = 0; i < ncells; i++)
     {
      nf = cells[i].nfaces;
      for (j = 0; j < nf; j++)
        {
         k = cells[i].facenos[j];
         cellfaces[k].cell1 = i;
        }
     }

   /*  Get the cellface cell 2 data from the opposite face info.  */
   for (i = 0; i < nfacesin; i++)
     {
      k = cellfaces[i].oppface;
      if (k > -1)
        {
         j = cellfaces[k].cell1;
         cellfaces[i].cell2 = j;
        }
     }

   /*  If 2D, build the cell drawing faces.  */
   ncellfaces = nfacesin;
   if (x3d_header.numdim == 2)
     {
      mesh2dflag = 1;
      ncellfaces = nfacesin + ncells;
      ncell2dfaces = nfacesin;
      for (i = 0; i < ncells; i++)
        {
         nf = cells[i].nfaces;
         cellfaces[nfacesin+i].nverts = nf;
         cellfaces[nfacesin+i].fverts = (int *)malloc(nf * sizeof(int));
         cellfaces[nfacesin+i].cell1 = i;
         cellfaces[nfacesin+i].cell2 = -1;
         cellfaces[nfacesin+i].oppface = -1;
         for (j = 0; j < nf; j++)
           {
            k = cells[i].facenos[j];
            l = cellfaces[k].fverts[0];
            cellfaces[nfacesin+i].fverts[j] = l;
           }
         cells[i].nfaces++;
         cells[i].facenos[nf] = nfacesin + i;
        }
     }

   /*  Generate the nodes for each cell from the face vertices.  */
   tstnodes = (short *)malloc((nnodes+1)*sizeof(short));
   if (tstnodes == NULL) memerr();
   for (i = 0; i < nnodes+1; i++) tstnodes[i] = 0;
   for (i = 0; i < ncells; i++)
     {
      for (j = 0; j < cells[i].nfaces; j++)
        {
         k = cells[i].facenos[j];
         for (l = 0; l < cellfaces[k].nverts; l++)
           {
            n = cellfaces[k].fverts[l];
            tstnodes[n] = 1;
           }
        }

      /*  Save cell nodes.  */
      ncnodes = 0;
      for (j = 0; j < nnodes+1; j++)
         {
          if (tstnodes[j])
            {
             cnodes[ncnodes] = j;
             ncnodes++;
             tstnodes[j] = 0;
            }
         }
      cells[i].ncnodes = ncnodes;
      cells[i].cnodes = (int *)malloc(ncnodes*sizeof(int));
      if (cells[i].cnodes == NULL) memerr();
      for (j = 0; j < ncnodes; j++)
        {
         cells[i].cnodes[j] = l = cnodes[j];
        }
     }

   /*  Call getcelledges to generate cell edge data.  */
   if (x3d_header.numdim == 2) gmv_meshdata.intype = VFACES2D;
   else gmv_meshdata.intype = VFACES3D;
   getcelledges();
}


void getx3dslaves()
{
 /*                              */
 /*  Get the slave/master data.  */
 /*                              */
  int i, j, *flag_data, ntypes;

   /*  Read the header and loop through the data.  */
   if (x3dread_slave_header() > 0) readx3d_error(filnam);

   if (x3d_header.numslaves > 0)
     {

      /*  Save slaves as a node flag, slave_type,  */ 
      /*  with none, slave and master types.       */

      flag_data = (int *)malloc((nnodes+1) * sizeof(int));
      for (i = 0; i < nnodes+1; i++) flag_data[i] = 4;

      /*  Read a slave lines from the x3d file and set flag_data.  */
      for (i = 0; i < x3d_header.numslaves; i++)
        {
         if (x3dread_slaves() > 0) readx3d_error(filnam);
         flag_data[x3d_slave.node_no] = 2;
         for (j = 0; j < x3d_slave.nmasters; j++)
           {
            flag_data[x3d_slave.masters[j]] = 1;
           }
        }

      /*  Save node flag name.  */
      flagname_node[numflags_node] = (char *)malloc(33*sizeof(char));
      if (flagname_node[numflags_node] == NULL) memerr();
      strcpy(flagname_node[numflags_node],"slave_types");
      ntypes = 4;
      flagtypes_node[numflags_node] = (short) ntypes;
      for (j = 0; j < ntypes; j++)
         {
          flagnames_node[numflags_node][j] = (char *)malloc(33*sizeof(char));
          if (j == 0) strcpy(flagnames_node[numflags_node][j],"master");
          if (j == 1) strcpy(flagnames_node[numflags_node][j],"slave");
          if (j == 2) strcpy(flagnames_node[numflags_node][j],"-");
          if (j == 3) strcpy(flagnames_node[numflags_node][j],"none");
         }

      /*  Save node flag data.  */
      flags[numflags_node] = (short *)malloc((nnodes+1)*sizeof(short));
      if (flags[numflags_node] == NULL) memerr();
      for (i = 0; i < nnodes+1; i++)
         flags[numflags_node][i] = (short)flag_data[i];

      numflags_node++;

      free(flag_data);
     }

   /*  Read the slave trailer.  */
   if (x3dread_slave_end() > 0) readx3d_error(filnam);
}


void getx3dghosts()
{
 /*                       */
 /*  Get the ghost node.  */
 /*                       */
  int i, j, *ghosts, nghosts;

   /*  Read the header and loop through the data.  */
   if (x3dread_ghost_header() > 0) readx3d_error(filnam);

   if (x3d_header.numghosts > 0)
     {

      /*  Save ghosts as a node group.  */
      nghosts = x3d_header.numghosts;
      ghosts = (int *)malloc(nghosts * sizeof(int));

      /*  Read a ghostline lines from the x3d file and add to ghosts.  */
      for (i = 0; i < x3d_header.numghosts; i++)
        {
         if (x3dread_ghosts() > 0) readx3d_error(filnam);
         ghosts[i] = x3d_ghost.node_no;
        }

      /*  Save node group name.  */
      nodegrpname[nnodegrps] = (char *)malloc(33*sizeof(char));
      if (nodegrpname[nnodegrps] == NULL) memerr();
      strcpy(nodegrpname[nnodegrps],"ghosts");

      /*  Save the number and data for this node group.  */
      numnodegrp[nnodegrps] = nghosts;

      nodegrps[nnodegrps] = (int *)malloc(nghosts * sizeof(int));
      if (nodegrps[nnodegrps] == NULL) memerr();
      for (i = 0; i < nghosts; i++)
         nodegrps[nnodegrps][i] = ghosts[i];

      nnodegrps++;

      free(ghosts);
     }

   /*  Read the ghost trailer.  */
   if (x3dread_ghost_end() > 0) readx3d_error(filnam);
}


void getx3dcelldata()
{
 /*                                          */
 /*  Get cell data, only save material ids.  */
 /*                                          */
  int i, j, ii;
  char name[40];

   /*  Read the header and loop through the number of data fields.  */
   if (x3dread_cell_data_header() > 0) readx3d_error(filnam);

   for (ii = 0; ii < x3d_header.num_cell_fields; ii++)
     {

      /*  Read the cell field, process only the mat_ids.  */
      if (x3d_read_cell_data() > 0) readx3d_error(filnam);
      if (strcmp(x3d_cell_data.name,"matid") == 0)
        {
         cellmats=(short *)malloc((ncells)*sizeof(short));
         if (cellmats == NULL) memerr();
         for (i = 0; i < ncells; i++)
            cellmats[i] = (short)x3d_cell_data.int_data[i];

         /*  Check for invalid material data.  */
         matvalchk(ncells,mmats,0,cellmats);

         /*  Calculate node materials.  */
         imat=(short *)malloc((nnodes+1)*sizeof(short));
         if (imat == NULL) memerr();
         bldnodemats();         
        }

      /*  Free cell field data.  */
      if (x3d_cell_data.typedata == 0) free(x3d_cell_data.int_data);
      else free(x3d_cell_data.double_data);
     }

   /*  Read the trailer.  */
   if (x3dread_cell_data_header_end() > 0) readx3d_error(filnam);
}


void getx3dnodedata()
{
 /*                                 */
 /*  Get and skip node field data.  */
 /*                                 */
  int i, j, ii;
  char name[40];

   /*  Read the header and loop through the number of data fields.  */
   if (x3dread_node_data_header() > 0) readx3d_error(filnam);

   for (ii = 0; ii < x3d_header.num_node_fields; ii++)
     {

      /*  Read the node field, process nodeid, if any.  */
      if (x3d_read_node_data() > 0) readx3d_error(filnam);
      if (strcmp(x3d_node_data.name,"nodeid") == 0)
        {
         nodeids = (int *)malloc((nnodes+1)*sizeof(int));
         if (nodeids == NULL) memerr();
         nodeids[0] = 0;
         for (i = 0; i < nnodes; i++)
            nodeids[i+1] = (short)x3d_node_data.int_data[i];

         nodeidflag = 1;
        }

      /*  Free node field data.  */
      if (x3d_node_data.typedata == 0) free(x3d_node_data.int_data);
      else free(x3d_node_data.double_data);
     }

   /*  Read the trailer.  */
   if (x3dread_node_data_header_end() > 0) readx3d_error(filnam);
}


void getx3dBdyfiles()
{
 /*                                                    */
 /*  Get data from Bdy files and save as node groups.  */
 /*                                                    */
  int i, j, ii, ngrp, return_code, ifound, len, foundTag;
  //char dir_buf[sizeof(struct dirent) + 1024];
  struct dirent *entry;
  DIR *dir;
  FILE *bdyfile;
  char bdyfilename[256], *tok, tagname[200], *ptr, tmpname[256];

   /*  Open the current directory.  */
   if ((dir = opendir(save_dir)) == NULL)
     {
      printf("Boundary file error, could not open directory %s\n",save_dir);
      return;
     }

   /*  Loop through the files in the directory and get relevant Bdy files.  */
   while ((entry = readdir(dir)) != NULL)
     {

      /*  Skip . and .. .  */
      ifound = 0;
      if ((strcmp(entry->d_name, ".")== 0) ||
          (strcmp(entry->d_name, "..") == 0)) continue;

      /*  Check that this file is a good Bdy file.  */
      ptr = strstr(entry->d_name,"Bdy");
      if (ptr == NULL) continue;

      /*  Check that prefix matches.  */
      len = strlen(x3dprefix);
      if (strncmp(entry->d_name,x3dprefix,len) != 0) continue;

      /*  Check suffix, if any.  */
      ifound = 1;
      if (has_suffix)
        {
         ptr = (strstr(entry->d_name,x3dsuffix));
         if (ptr == NULL) ifound = 0;;
        }
      if (ifound == 0) continue;      

      /*  Found a matching Bdy file.  */
      sprintf(bdyfilename,"%s%s",save_dir,entry->d_name);

      /*  Extract the group name from the file name.  */
      sprintf(tmpname,"%s",entry->d_name);
      tok = strtok(entry->d_name,".");
      foundTag = 0;
      while (!foundTag)
        {
         tok = strtok(NULL,".");
         if (tok == NULL) break;
         if (strcmp(tok,"Bdy") == 0) foundTag = 1;
         else
           strcpy(tagname,tok);
        }

      /*  Read the boundary data and save.  */
      if (x3d_read_Bdy_file(bdyfilename) > 0) readx3d_error(filnam);

      /*  Save node group name and data, if any.  */
      if (x3d_Bdy_data.num_entries > 0)
        {
         printf("GMV read %d node group entries from Bdy file %s\n",
                x3d_Bdy_data.num_entries,tmpname);

         nodegrpname[nnodegrps] = (char *)malloc(33*sizeof(char));
         if (nodegrpname[nnodegrps] == NULL) memerr();
         strcpy(nodegrpname[nnodegrps],tagname);

         /*  Save the number and data for this node group.  */
         ngrp = x3d_Bdy_data.num_entries;
         numnodegrp[nnodegrps] = ngrp;
         nodegrps[nnodegrps] = (int *)malloc((ngrp)*sizeof(int));
         if (nodegrps[nnodegrps] == NULL) memerr();
         for (i = 0; i < ngrp; i++)
            nodegrps[nnodegrps][i] = x3d_Bdy_data.int_data[i];
         FREE(x3d_Bdy_data.int_data);
         nnodegrps++;
        }
     }
   closedir(dir);
}


void getx3dRegfiles()
{
 /*                                                   */
 /*  Get data from Reg files and save as cell groups  */
 /*  or as material volume fraction subvar data.      */
 /*                                                   */
  int i, j, ii, ngrp, nelem, ifound, len, matid, foundTag;
  double fmin, fmax, fillval, *vfield;
  struct dirent *entry;
  DIR *dir;
  FILE *regfile;
  char regfilename[256], *tok, tagname[200], *ptr, tmpname[256];

   /*  Open the current directory.  */
   if ((dir = opendir(save_dir)) == NULL)
     {
      printf("Reg file error, could not open directory %s\n",save_dir);
      return;
     }

   /*  Loop through the files in the directory and get relevant Reg files.  */
   vfield = (double *)malloc(ncells * sizeof(double));
   while ((entry = readdir(dir)) != NULL)
     {

      /*  Skip . and .. .  */
      ifound = 0;
      if ((strcmp(entry->d_name, ".")== 0) ||
          (strcmp(entry->d_name, "..") == 0)) continue;

      /*  Check that this file is a good Reg file.  */
      ptr = strstr(entry->d_name,"Reg");
      if (ptr == NULL) continue;

      /*  Check that prefix matches.  */
      len = strlen(x3dprefix);
      if (strncmp(entry->d_name,x3dprefix,len) != 0) continue;

      /*  Check suffix, if any.  */
      ifound = 1;
      if (has_suffix)
        {
         ptr = (strstr(entry->d_name,x3dsuffix));
         if (ptr == NULL) ifound = 0;;
        }
      if (ifound == 0) continue;

      /*  Found a matching Reg file.  */
      sprintf(regfilename,"%s%s",save_dir,entry->d_name);

      /*  Extract the name from the file name.  */
      sprintf(tmpname,"%s",entry->d_name);
      tok = strtok(entry->d_name,".");
      foundTag = 0;
      while (!foundTag)
        {
         tok = strtok(NULL,".");
         if (tok == NULL) break;
         if (strcmp(tok,"Reg") == 0) foundTag = 1;
         else
           strcpy(tagname,tok);
        }
       
      /*  Read the region data and save.  */
      if (x3d_read_Reg_file(regfilename) > 0) readx3d_error(filnam);

      /*  Save cell group name or subvar name and data, if any.  */
      if (x3d_Reg_data.num_entries > 0)
        {

         if (x3d_Reg_data.type == 0)  // Tag to group.
           {
            printf("GMV read %d cell group entries from Reg file %s\n",
                   x3d_Reg_data.num_entries,tmpname);

            /*  Save tagname.  */
            cellgrpname[ncellgrps] = (char *)malloc(33*sizeof(char));
            if (cellgrpname[ncellgrps] == NULL) memerr();
            strcpy(cellgrpname[ncellgrps],tagname);

            /*  Save the number and data for this cell group.  */
            ngrp = x3d_Reg_data.num_entries;
            numcellgrp[ncellgrps] = ngrp;
            cellgrps[ncellgrps] = (int *)malloc((ngrp)*sizeof(int));
            if (cellgrps[ncellgrps] == NULL) memerr();
            for (i = 0; i < ngrp; i++)
               cellgrps[ncellgrps][i] = x3d_Reg_data.int_data[i];
            FREE(x3d_Reg_data.int_data);
            ncellgrps++;
           }

         if (x3d_Reg_data.type == 1)  // Mat vol. fraction to subvar.
           {
            printf("GMV read %d cell subvar entries from Reg file %s\n",
                   x3d_Reg_data.num_entries,tmpname);

            /*  Save subvar name.  */
            cfieldname[cnumvars]=(char *)malloc(33*sizeof(char));
            if (cfieldname[cnumvars] == NULL) memerr();
            strcpy(cfieldname[cnumvars],tagname);
            cvars_to_write[cnumvars] = 0;
            csubvarsin[cnumvars] = 1;

            /*  If the material name does not exist, add it.  */
            matid = 0;
            for (i = 0; i < mmats; i++)
               {
                if (strcmp(tagname,mmatnames[i]) == 0) matid = i + 1;
               }
            if (matid == 0)
              {
               mmatnames[mmats] = (char *)malloc(mmats*33*sizeof(char));
               strncpy(mmatnames[i],tagname,32);
               matid = mmats;
               mmats++;
              }

            /*  Calculate min. and max.  */
            nelem = x3d_Reg_data.num_entries;
            fmin = MAXVAL;
            fmax = -MAXVAL;
            for (i = 0; i < nelem; i++)
              {
               fmin = (x3d_Reg_data.double_data[i] < fmin) ? 
                         x3d_Reg_data.double_data[i] : fmin;
               fmax = (x3d_Reg_data.double_data[i] > fmax) ? 
                         x3d_Reg_data.double_data[i] : fmax;
              }
            if (fmin >= 0.) fillval = 0;
            else fillval = -MAXVAL;

            /*  Fill array and write.  */
            for (i = 0; i < ncells; i++)
               vfield[i] = fillval;
            for (i = 0; i < nelem; i++)
              {
               j = x3d_Reg_data.int_data[i] - 1;
               if (j >= 0 && j < ncells)
                  vfield[j] = x3d_Reg_data.double_data[i];
               else
                  printf("Warning, subvars - cell id %d is out of range.\n",
                         j+1);
             }
            writecellfld(vfield);
            cfieldmin[cnumvars] = fmin;
            cfieldmax[cnumvars] = fmax;
            cnumvars++;

            /*  Replace cell materials with the current material  */
            /*  if the volume fraction is greater than .5.        */

            /*  Get the material id from the name.  */
            matid = 0;
            for (i = 0; i < mmats; i++)
               {
                if (strcmp(tagname,mmatnames[i]) == 0) matid = i + 1;
               }
            if (matid > 0)
              {
               for (i = 0; i < nelem; i++)
                 {
                  if (x3d_Reg_data.double_data[i] > .5)
                    {
                     j = x3d_Reg_data.int_data[i] - 1;
                     cellmats[j] = matid;
                    }
                 } 
              } 

            FREE(x3d_Reg_data.int_data);
            FREE(x3d_Reg_data.double_data);
           }
        }
     }
   closedir(dir);
   free(vfield);
}
