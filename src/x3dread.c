#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define X3D_INIT
#include "x3dread.h"

#include <sys/types.h>
#include <math.h>

#define FREE(a) { if (a) free(a); a = NULL; }

FILE *x3din;


int x3dread_checkfile(char *filnam)
{
  /*                           */
  /*  Check a GMV input file.  */
  /*                           */
  int i, chkend;
  char magic[20], filetype[9];
  char rdend[21];
  FILE *x3dchk;

   x3dchk = fopen(filnam,"r");

   if (x3dchk == NULL)
     {
      fprintf(stderr,"Cannot open X3D input file %s\n",filnam);
      return 1;
     }
    
   /*  Read header. */
   fscanf(x3dchk,"%s",magic);
   if (strncmp(magic,"x3dtoflag",9) != 0)
     {
      fprintf(stderr,"This is not a X3D input file.\n");
      fclose(x3dchk);
      return 2;
     }

   /*  Check that x3d input file has "end_dump".  */
   if (strncmp(magic,"x3dtoflag",8) == 0)
     {

      /*  Read the last 20 characters of the file.  */
      fseek(x3dchk, -20L, 2);
      fread(rdend,sizeof(char), 20, x3dchk);

      /*  Check the 20 characters for end_dump.  */
      chkend = 0;
      for (i = 0; i < 15; i++)
         if (strncmp((rdend+i),"end_dump",8) == 0) chkend = 1;
      if (!chkend)
        {
         fprintf(stderr,"Error - end_dump not found.\n");
         fclose(x3dchk);
         return 3;
        }
     }
   fclose(x3dchk);
   return 0;
}


int x3dread_open(char *filnam)
{
  /*                                     */
  /*  Open and check an X3D input file.  */
  /*                                     */
  int ierr;
  char magic[20], filetype[20];

   ierr = x3dread_checkfile(filnam);
   if (ierr > 0) return ierr;

   x3din = fopen(filnam,"r");

   /*  Read past the first header.  */
   fscanf(x3din,"%s%s",magic,filetype);
   return 0;
}


void x3dread_close()
{
   fclose(x3din);
   // vfaceflag = 0;
}


int x3dread_header()
{
 /*                             */
 /*  Read the X3D header data.  */
 /*                             */
  char keyword[20];
  int indata, ierr;

   /*  Read the header header.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"header") != 0)
     {
      printf("x3d read error, header not found.\n");
      return 1;
     }

   /*  Read the header data.  */
   ierr = 0;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"process") == 0) x3d_header.process = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"numdim") == 0) x3d_header.numdim = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"materials") == 0) x3d_header.nummats = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"nodes") == 0) x3d_header.numnodes = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"faces") == 0) x3d_header.numfaces = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"elements") == 0) x3d_header.numcells = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"ghost_nodes") == 0) x3d_header.numghosts = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"slaved_nodes") == 0) x3d_header.numslaves = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"nodes_per_slave") == 0) 
      x3d_header.nodes_per_slave = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"nodes_per_face") == 0) 
      x3d_header.nodes_per_face = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"faces_per_cell") == 0) 
      x3d_header.faces_per_cell = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"node_data_fields") == 0) 
      x3d_header.num_node_fields = indata;
   else ierr = 1;
   fscanf(x3din,"%s%d",keyword,&indata);
   if (strcmp(keyword,"cell_data_fields") == 0) 
      x3d_header.num_cell_fields = indata;
   else ierr = 1;

   /*  Check for read error.  */
   if (ierr)
     {
      printf("x3d read error, header data incorrect.\n");
      return 1;
     }

   /*  Read header trailer.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_header") != 0) 
     {
      printf("x3d read error, header end_header not found.\n");
      return 1;
     }  
   return 0;
}


int x3dread_matnames()
{
 /*                                */
 /*  Read list of material names.  */
 /*                                */
  int i, j, tmpin;
  char keyword[40];

   /*  Read material names header.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"matnames") != 0) 
     {
      printf("x3d read error, material header not found.\n");
      return 1;
     }

   /*  If no materials, read trailer and quit.  */
   if (x3d_header.nummats == 0)
     {
      fscanf(x3din,"%s",keyword);
      if (strcmp(keyword,"end_matnames") != 0) 
        {
         printf("x3d read error, end_matnames not found.\n");
         return 1;
        }
      return 0;
     }

   /*  Allocate material name array.  */
   x3dmatnames = (char **)malloc(x3d_header.nummats * sizeof(* x3dmatnames));
   for (i = 0; i < x3d_header.nummats; i++)
     {
      fscanf(x3din,"%d%s",&tmpin,keyword);
      j = strlen(keyword) + 1;
      x3dmatnames[i] = (char *)malloc(j * sizeof(char));
      strcpy(x3dmatnames[i],keyword);
     }

   /*  Read trailer.  */   
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_matnames") != 0) 
     {
      printf("x3d read error, end_matnames not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_mateos()
{
 /*                                   */
 /*  Read material eos (usually -1).  */
 /*                                   */
  int i, j, tmpin;
  double eosin;
  char keyword[40];

   /*  Read material eso header.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"mateos") != 0) 
     {
      printf("x3d read error, material eos header not found.\n");
      return 1;
     }

   /*  If no materials, read trailer and quit.  */
   if (x3d_header.nummats == 0)
     {
      fscanf(x3din,"%s",keyword);
      if (strcmp(keyword,"end_mateos") != 0) 
        {
         printf("x3d read error, end_mateos not found.\n");
         return 1;
        }
      return 0;
     }

   /*  Allocate material eos array.  */
   mateos = (double *)malloc(x3d_header.nummats * sizeof(double));
   for (i = 0; i < x3d_header.nummats; i++)
     {
      fscanf(x3din,"%d%lf",&tmpin,&eosin);
      mateos[i] = eosin;
     }

   /*  Read trailer.  */   
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_mateos") != 0) 
     {
      printf("x3d read error, end_mateos not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_matopc()
{
 /*                                   */
 /*  Read material opc (usually -1).  */
 /*                                   */
  int i, j, tmpin;
  double opcin;
  char keyword[40];

   /*  Read material eso header.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"matopc") != 0) 
     {
      printf("x3d read error, material opc header not found.\n");
      return 1;
     }

   /*  If no materials, read trailer and quit.  */
   if (x3d_header.nummats == 0)
     {
      fscanf(x3din,"%s",keyword);
      if (strcmp(keyword,"end_matopc") != 0) 
        {
         printf("x3d read error, end_matopc not found.\n");
         return 1;
        }
      return 0;
     }

   /*  Allocate material opc array.  */
   matopc = (double *)malloc(x3d_header.nummats * sizeof(double));
   for (i = 0; i < x3d_header.nummats; i++)
     {
      fscanf(x3din,"%d%lf",&tmpin,&opcin);
      matopc[i] = opcin;
     }

   /*  Read trailer.  */   
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_matopc") != 0) 
     {
      printf("x3d read error, end_matopc not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_nodes()
{
 /*                          */
 /*  Read node coordinates.  */
 /*                          */
  int i, tmpin;
  double xin, yin, zin;
  char keyword[40];

   /*  Read node coordinates header.  */
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"nodes") != 0) 
     {
      printf("x3d read error, node coordinate header not found.\n");
      return 1;
     }

   /*  Loop through the coordindate and save.  */
   x3d_x = (double *)malloc(x3d_header.numnodes * sizeof(double));
   x3d_y = (double *)malloc(x3d_header.numnodes * sizeof(double));
   x3d_z = (double *)malloc(x3d_header.numnodes * sizeof(double));
   for (i = 0; i < x3d_header.numnodes; i++)
     {
      fscanf(x3din,"%d%lf%lf%lf",&tmpin,&xin,&yin,&zin);
      x3d_x[i] = xin;
      x3d_y[i] = yin;
      x3d_z[i] = zin;
     }
   
   /*  Read trailer.  */   
   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_nodes") != 0) 
     {
      printf("x3d read error, end_nodes not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_face_header()
{
 /*                          */
 /*  Read x3d faces header.  */
 /*                          */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"faces") != 0) 
     {
      printf("x3d read error, face header not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_faces()
{
 /*                     */
 /*  Read an x3d face.  */
 /*                     */
  int i, tmpnum, nverts, verts[20], proc_no, opp_pe, opp_face, tmpin;

   /*  Read face number, and number of vertices.  */
   fscanf(x3din,"%d%d",&tmpnum,&nverts);

   /*  Loop through vertices.  */
   for (i = 0; i < nverts; i++)
     {
      fscanf(x3din,"%d",&verts[i]);
     }

   /*  Read processor number, opposite face pe and opposite face.  */
   fscanf(x3din,"%d%d%d",&proc_no,&opp_pe,&opp_face);

   /*  Read the rest of the line, five ones.  */
   for (i = 0; i < 5; i++)
     {
      fscanf(x3din,"%d",&tmpin);
     }

   /*  Set the input data into the structure and return.  */
   x3d_face.number = tmpnum;
   x3d_face.nverts = nverts;
   for (i = 0; i < nverts; i++) x3d_face.verts[i] = verts[i];
   x3d_face.proc_no = proc_no;
   x3d_face.opp_pe = opp_pe;
   x3d_face.opp_face = opp_face;
   return 0;
}


int x3dread_face_end()
{
 /*                          */
 /*  Read x3d end_faces.  */
 /*                          */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_faces") != 0) 
     {
      printf("x3d read error, end_faces not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_cell_header()
{
 /*                          */
 /*  Read x3d cells header.  */
 /*                          */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"cells") != 0) 
     {
      printf("x3d read error, cell header not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_cells()
{
 /*                     */
 /*  Read an x3d cell.  */
 /*                     */
  int i, tmpnum, nfaces, faces[20];

   /*  Read cell number, and number of faces.  */
   fscanf(x3din,"%d%d",&tmpnum,&nfaces);

   /*  Loop through faces.  */
   for (i = 0; i < nfaces; i++)
     {
      fscanf(x3din,"%d",&faces[i]);
     }

   /*  Set the input data into the structure and return.  */
   x3d_cell.number = tmpnum;
   x3d_cell.nfaces = nfaces;
   for (i = 0; i < nfaces; i++) x3d_cell.faces[i] = faces[i];
   return 0;
}


int x3dread_cell_end()
{
 /*                          */
 /*  Read x3d end_cells.  */
 /*                          */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_cells") != 0) 
     {
      printf("x3d read error, end_cells not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_slave_header()
{
 /*                          */
 /*  Read x3d slave header.  */
 /*                          */
  int tmpin;
  char keyword[40];

   fscanf(x3din,"%s%d",keyword,&tmpin);
   if (strcmp(keyword,"slaved_nodes") != 0) 
     {
      printf("x3d read error, slave header not found.\n");
      return 1;
     }
   nslaves = tmpin;
   return 0;
}


int x3dread_slaves()
{
 /*                      */
 /*  Read an x3d slave.  */
 /*                      */
  int i, tmpnum, slave_id, num_masters, masters[20];

   /*  Read slave number, slave node number, and number of masters.  */
   fscanf(x3din,"%d%d%d",&tmpnum,&slave_id,&num_masters);

   /*  Loop through masters.  */
   for (i = 0; i < num_masters; i++)
     {
      fscanf(x3din,"%d",&masters[i]);
     }

   /*  Set the input data into the structure and return.  */
   x3d_slave.number = tmpnum;
   x3d_slave.node_no = slave_id;
   x3d_slave.nmasters = num_masters;
   for (i = 0; i < num_masters; i++) x3d_slave.masters[i] = masters[i];
   return 0;
}


int x3dread_slave_end()
{
 /*                              */
 /*  Read x3d end_slaved_nodes.  */
 /*                              */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_slaved_nodes") != 0) 
     {
      printf("x3d read error, end_slaved_nodes not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_ghost_header()
{
 /*                          */
 /*  Read x3d ghost header.  */
 /*                          */
  int tmpin;
  char keyword[40];

   fscanf(x3din,"%s%d",keyword,&tmpin);
   if (strcmp(keyword,"ghost_nodes") != 0) 
     {
      printf("x3d read error, ghost header not found.\n");
      return 1;
     }
   nghosts = tmpin;
  return 0;
}


int x3dread_ghosts()
{
 /*                                */
 /*  Read an x3d ghost.  */
 /*                                 */
  int i, tmpnum, ghost_id, own_proc, opp_ghost;

   /*  Read ghost number, ghost node no., processor and opposite ghost.  */
   fscanf(x3din,"%d%d%d%d",&ghost_id,&own_proc,&opp_ghost,&tmpnum);

   /*  Set the input data into the structure and return.  */
   x3d_ghost.number = tmpnum;
   x3d_ghost.node_no = ghost_id;
   x3d_ghost.own_proc = own_proc;
   x3d_ghost.opp_ghost = opp_ghost;
   return 0;
}


int x3dread_ghost_end()
{
 /*                              */
 /*  Read x3d end_ghosts_nodes.  */
 /*                              */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_ghost_nodes") != 0) 
     {
      printf("x3d read error, end_ghosts not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_cell_data_header()
{
 /*                              */
 /*  Read x3d cell data header.  */
 /*                              */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"cell_data") != 0) 
     {
      printf("x3d read error, cell_data header not found.\n");
      return 1;
     }
   return 0;
}


int x3d_read_cell_data()
{
 /*                       */
 /*  Read x3d cell data.  */
 /*                       */
  char name[40], tmpchar[11];
  int i, j, k, nc, tmpint;
  double tmpdouble;

   /*  Read the name, if the name is matid, generate integer id set.  */
   /*  Otherwise, save integer values, note, no double values exist.  */
   fscanf(x3din,"%s",name);
   strcpy(x3d_cell_data.name,name);
   nc = x3d_header.numcells;

   if (strcmp(name,"matid") == 0 || strcmp(name,"partelm") == 0)
     {
      x3d_cell_data.typedata = 0;
      x3d_cell_data.int_data = (int *)malloc(nc * sizeof(int));
     }
   else
     {
      x3d_cell_data.typedata = 1;
      x3d_cell_data.double_data = (double *)malloc(nc * sizeof(double));
     }

   if (strcmp(name,"matid") == 0)
     {
      for (i = 0; i < nc; i++)
        {
         fscanf(x3din,"%s",tmpchar);
         k = 0;
         for (j = 0; j < x3d_header.nummats; j++)
           {
            if (strcmp(tmpchar,x3dmatnames[j]) == 0) k = j;
           }
         x3d_cell_data.int_data[i] = k + 1;
        }
     }
   else if (x3d_cell_data.typedata == 0)
     {
      for (i = 0; i < nc; i++)
        {
         fscanf(x3din,"%d",&tmpint);
         x3d_cell_data.int_data[i] = tmpint;
        }
     }
   else if (x3d_cell_data.typedata == 1)
     {
      for (i = 0; i < nc; i++)
        {
         fscanf(x3din,"%lf",&tmpdouble);
         x3d_cell_data.double_data[i] = tmpdouble;
        }
     }
 
   /*  Read end of named data string.  */
   fscanf(x3din,"%s",name);
   return 0;
}


int x3dread_cell_data_header_end()
{
 /*                           */
 /*  Read x3d end_cell_data.  */
 /*                           */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_cell_data") != 0) 
     {
      printf("x3d read error, end_cell_data not found.\n");
      return 1;
     }
   return 0;
}


int x3dread_node_data_header()
{
 /*                              */
 /*  Read x3d node data header.  */
 /*                              */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"node_data") != 0) 
     {
      printf("x3d read error, node_data header not found.\n");
      return 1;
     }
   return 0;
}


int x3d_read_node_data()
{
 /*                       */
 /*  Read x3d node data.  */
 /*                       */
  char name[40], tmpchar[11];
  int i, j, k, nc, tmpint;
  double tmpdouble;

   /*  Read the name, if the name is node, generate integer id set.   */
   /*  Otherwise, save integer values, note, no double values exist.  */
   fscanf(x3din,"%s",name);
   strcpy(x3d_node_data.name,name);
   nc = x3d_header.numnodes;

   if (strcmp(name,"nodeid") == 0)
     {
      x3d_node_data.typedata = 0;
      x3d_node_data.int_data = (int *)malloc(nc * sizeof(int));
     }
   else
     {
      x3d_node_data.typedata = 1;
      x3d_node_data.double_data = (double *)malloc(nc * sizeof(double));
     }

   if (strcmp(name,"nodeid") == 0)
     {
      for (i = 0; i < nc; i++)
        {
         fscanf(x3din,"%s",tmpchar);
         k = 0;
         for (j = 0; j < x3d_header.nummats; j++)
           {
            if (strcmp(tmpchar,x3dmatnames[j]) == 0) k = j;
           }
         x3d_node_data.int_data[i] = k;
        }
     }
   else if (x3d_node_data.typedata = 1)
     {
      for (i = 0; i < nc; i++)
        {
         fscanf(x3din,"%lf",&tmpdouble);
         x3d_node_data.double_data[i] = tmpdouble;
        }
     }
 
   /*  Read end of named data string.  */
   fscanf(x3din,"%s",name);
   return 0;
}


int x3dread_node_data_header_end()
{
 /*                           */
 /*  Read x3d end_node_data.  */
 /*                           */
  char keyword[40];

   fscanf(x3din,"%s",keyword);
   if (strcmp(keyword,"end_node_data") != 0) 
     {
      printf("x3d read error, end_node_data not found.\n");
      return 1;
     }
   return 0;
}


int x3d_read_Bdy_file(char *filename)
{
 /*                    */
 /*  Read a Bdy file.  */
 /*                    */
  int i, id, *data, ieof, count;
  FILE *Bdy;

   /*  Open the file.  */
   Bdy = fopen(filename,"r");
   if (Bdy == NULL)
     {
      fprintf(stderr,"Cannot open Bdy file %s\n",filename);
      return 1;
     }

   /*  Read the nodes.  */
   data = (int *)malloc(x3d_header.numnodes * sizeof(int));
   i = 0;
   ieof = 0;
   while(!ieof)
      {
       fscanf(Bdy,"%d",&id);
       if (feof(Bdy) == 0 && ferror(Bdy) == 0)
         {
          data[i] = id;
          i++;
         }
       else ieof = 1;
      }
   fclose(Bdy);

   /*  Fill the Bdy structure.  */
   x3d_Bdy_data.num_entries = count = i;
   if (count < 1) return 0;
   x3d_Bdy_data.int_data = (int *)malloc(count * sizeof(int));
   for (i = 0; i < count; i++)
     {
      x3d_Bdy_data.int_data[i] = data[i];
     }
   free(data);

   return 0;
}


int x3d_read_Reg_file(char *filename)
{
 /*                    */
 /*  Read a Reg file.  */
 /*                    */
  int i, id, *data, ieof, count, itype, rdint;
  double *ddata;
  char linein[200], *result;
  FILE *Reg;

   /*  Open the file.  */
   Reg = fopen(filename,"r");
   if (Reg == NULL)
     {
      fprintf(stderr,"Cannot open Reg file %s\n",filename);
      return 1;
     }

   /*  Read the cells until end or until a blank line is found.  */
   data = (int *)malloc(x3d_header.numcells * sizeof(int));
   i = 0;
   ieof = 0;
   rdint = 1;
   x3d_Reg_data.type = 0;
   while(!ieof)
      {
       result = fgets(linein,200,Reg);

       /*  Check for blank line in a volume fraction Reg file.  */
       if (result == NULL || feof(Reg) != 0 || ferror(Reg) != 0) ieof = 1;
       else
         {
          if (rdint)
            {
             id = 0;
             sscanf(linein,"%d",&id);
             if (id > 0)
               {
                data[i] = id;
                i++;
               }
             else
               {
                count = i;
                i = 0;
                rdint = 0;
                x3d_Reg_data.type = 1;
                ddata = (double *)malloc(x3d_header.numcells * sizeof(double));
               }
            }
          else
            {
             sscanf(linein,"%lf",&ddata[i]);
             i++;
            }
         }
      }
   fclose(Reg);

   /*  Fill the Reg structure.  */
   if (rdint) count = i;
   x3d_Reg_data.num_entries = count;
   if (count < 1) return 0;
   x3d_Reg_data.int_data = (int *)malloc(count * sizeof(int));
   for (i = 0; i < count; i++)
     {
      x3d_Reg_data.int_data[i] = data[i];
     }
   free(data);

   if (x3d_Reg_data.type == 1)
     {
      x3d_Reg_data.double_data = (double *)malloc(count * sizeof(double));
      for (i = 0; i < count; i++)
        {
         x3d_Reg_data.double_data[i] = ddata[i];
        }
      free(ddata);
     }

   return 0;
}
