#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gmvdata.h"
#include "gmvread.h"

#include <sys/types.h>
//#include <malloc.h>
#include <math.h>

#define MSIZE 1000
#define MAXVAL 9.99999e32

static char *type_str[4] = {"cells","nodes","faces","surfaces"};

void do_header(void);
void do_nodes(void);
void do_nodev(void);
void do_cells(void);
void do_faces(void);
void do_vfaces(void);
void do_xfaces(void);
void do_materials(void);
void do_velocity(void);
void do_variables(void);
void do_flags(void);
void do_polygons(void);
void do_tracers(void);
void do_probtime(void);
void do_cycleno(void);
void do_nodeids(void);
void do_cellids(void);
void do_surface(void);
void do_surfmats(void);
void do_surfvel(void);
void do_surfvars(void);
void do_surfflag(void);
void do_units(void);
void do_vinfo(void);
void do_traceids(void);
void do_faceids(void);
void do_surfids(void);
void do_groups(void);
void do_subvars(void);
void do_ghosts(void);
void do_vects(void);


void readgmvinfo()
{
  /*                         */
  /*  Read a GMV input file  */
  /*  and print file info.   */
  /*                         */
  int iend, i, ierr, nodes_read;

   ierr = gmvread_open(filnam);

   if (ierr > 0)
      {
       fprintf(stderr,"Error reading gmv file %s\n",filnam);
       gmvexit();
      }
   printf("GMV reading file %s\n",filnam);

   gmvread_printoff();

   /*  Loop through file until GMVEND.  */
   nodes_read = 0;
   iend = 0;
   while (iend == 0)
     {
      gmvread_data();

      /*  Check for GMVEND.  */
      if (gmv_data.keyword == GMVEND)
        {
         iend = 1;
         gmvread_close();
        }

      /*  Check for GMVERROR, if nodes not read, quit.  */
      if (gmv_data.keyword == GMVERROR && !nodes_read) gmvexit();

      /*  Check for GMVERROR, if nodes read, stop  */
      /*  reading at this point and continue GMV   */
      if (gmv_data.keyword == GMVERROR && nodes_read)
        {
         iend = 1;
         gmvread_close();
        }

      /*  Process the data.  */
      switch (gmv_data.keyword)
        {
         case(NODES):
            nodes_read = 1;
            do_nodes();
            break;
         case(CELLS):
            nodes_read = 1;
            do_cells();
            break;
         case(FACES):
            do_faces();
            break;
         case(VFACES):
            do_vfaces();
            break;
         case(XFACES):
            do_xfaces();
            break;
         case(MATERIAL):
            do_materials();
            break;
         case(VELOCITY):
            do_velocity();
            break;
         case(VARIABLE):
            do_variables();
            break;
         case(FLAGS):
            do_flags();
            break;
         case(POLYGONS):
            do_polygons();
            break;
         case(TRACERS):
            do_tracers();
            break;
         case(PROBTIME):
            printf(" Simulation time = %lf\n",gmv_data.doubledata1[0]);
            break;
         case(CYCLENO):
            printf(" Cycle no. = %ld\n",gmv_data.num);
            break;
         case(NODEIDS):
            printf(" Reading node ids.\n");
            break;
         case(CELLIDS):
            printf(" Reading cell ids.\n");
            break;
         case(FACEIDS):
            printf(" Reading face ids.\n");
            break;
         case(TRACEIDS):
            printf(" Reading Particle ids.\n");
            break;
         case(UNITS):
            do_units();
            break;
         case(SURFACE):
            do_surface();
            break;
         case(SURFMATS):
            printf(" Read surface materials.\n");
            break;
         case(SURFVEL):
            do_surfvel();
            break;
         case(SURFVARS):
            do_surfvars();
            break;
         case(SURFFLAG):
            do_surfflag();
            break;
         case(SURFIDS):
            printf(" Read surface ids.\n");
            break;
         case(GROUPS):
            do_groups();
            break;
         case(VINFO):
            break;
         case(SUBVARS):
            do_subvars();
            break;
         case(GHOSTS):
            do_ghosts();
            break;
         case(VECTORS):
            do_vects();
            break;
         case(GMVEND):
            break;
        }
     }
}


void do_nodes(void)
{
  int nx, ny, nz, num;
  int nnodes, i;
  double minx, maxx, miny, maxy, minz, maxz, val;

   if (gmv_data.datatype == UNSTRUCT ||
       gmv_data.datatype == LOGICALLY_STRUCT)
     {
      nnodes = gmv_data.num;
      printf(" Read %d nodes\n",nnodes);
      if (nnodes > 0)
        {
         minx = MAXVAL;  maxx = -MAXVAL;
         miny = MAXVAL;  maxy = -MAXVAL;
         minz = MAXVAL;  maxz = -MAXVAL;
         for (i = 0; i < nnodes; i++)
           {
            val = gmv_data.doubledata1[i];
            minx = (minx < val) ? minx : val;
            maxx = (maxx > val) ? maxx : val;
            val = gmv_data.doubledata2[i];
            miny = (miny < val) ? miny : val;
            maxy = (maxy > val) ? maxy : val;
            val = gmv_data.doubledata3[i];
            minz = (minz < val) ? minz : val;
            maxz = (maxz > val) ? maxz : val;
           }
         printf("   min. x = %lf,  max. x = %lf\n",minx, maxx);
         printf("   min. y = %lf,  max. y = %lf\n",miny, maxy);
         printf("   min. z = %lf,  max. z = %lf\n",minz, maxz);
        }
      if (gmv_data.datatype == LOGICALLY_STRUCT)
        {
         nx = gmv_data.ndoubledata1;
         ny = gmv_data.ndoubledata2;
         nz = gmv_data.ndoubledata3;
         num = (nx - 1) * (ny -1 ) * (nz - 1);
         printf(" Mesh is logically structured with %d cells\n",num);
        }
     }

   else if (gmv_data.datatype == STRUCT)
     {
      nx = gmv_data.ndoubledata1;
      ny = gmv_data.ndoubledata2;
      nz = gmv_data.ndoubledata3;
      
      nnodes = nx * ny * nz;
      num = (nx - 1) * (ny -1 ) * (nz - 1);
      printf(" Read %d nodes\n",nnodes);
      minx = gmv_data.doubledata1[0];
      maxx = gmv_data.doubledata1[nx-1];
      miny = gmv_data.doubledata2[0];
      maxy = gmv_data.doubledata2[ny-1];
      minz = gmv_data.doubledata3[0];
      maxz = gmv_data.doubledata3[nz-1];
      printf("   min. x = %lf,  max. x = %lf\n",minx, maxx);
      printf("   min. y = %lf,  max. y = %lf\n",miny, maxy);
      printf("   min. z = %lf,  max. z = %lf\n",minz, maxz);
      printf(" Mesh is rectangular structured with %d cells\n",num);
     }
}


void do_cells(void)
{
  static int ifirst = 1;
  int ncells, i;

   if (ifirst)
     {
      ifirst = 0;
      ncells = gmv_data.num;
      printf(" The mesh is unstructured with %d cells\n",ncells);
     }
}


void do_faces(void)
{
  static int ifirst = 1;
  int numfaces, numcells, i;
  int nverts, *verts, cellid1, cellid2;

   if (ifirst)
     {
      ifirst = 0;
      numfaces = gmv_data.num;
      numcells = gmv_data.num2;
      printf(" The mesh is defined by faces with %d faces and %d cells.\n",
               numfaces, numcells);
     }

   if (gmv_data.datatype == ENDKEYWORD) return;
}


void do_vfaces(void)
{
  static int ifirst = 1;
  int numfaces, numcells, i;

   if (ifirst)
     {
      ifirst = 0;
      numfaces = gmv_data.num;
      printf(" There are %d vfaces.\n",numfaces);
     }

   if (gmv_data.datatype == ENDKEYWORD) return;
}


void do_xfaces(void)
{
  static int ifirst = 1, numfaces;

   if (ifirst)
     {
      ifirst = 0;
      printf("Converting xfaces.\n");
      numfaces = gmv_data.num;
      printf(" There are %d xfaces.\n",numfaces);
     }

   if (gmv_data.datatype == ENDKEYWORD) return;
}


void do_materials(void)
{
  int n_mats, d_type;
  int nmatdata, i, maxmatin;
  char matname[40];

   n_mats = gmv_data.num;
   if (gmv_data.datatype == CELL)
     {
      d_type = 0;
      printf( " There are %d cell materials.\n",n_mats);
     }
   if (gmv_data.datatype == NODE)
     {
      d_type = 1;
      printf( " There are %d node materials.\n",n_mats);
     }
   for (i = 0; i < n_mats; i++)
     {
      strncpy(matname,&gmv_data.chardata1[i*33],32);
      *(matname+32) = (char) 0;
      printf("  %s\n",matname);
     }

   /*  Check for more than MAXMAT materials.  */
   if (n_mats > MAXMATS)
      printf("   Warning, there are more than %d materials.\n", MAXMATS);

   /*  Check for material nos. > n_mats.  */
   nmatdata = gmv_data.nlongdata1;
   maxmatin = 0;
   if (d_type == 0)
     {
      for (i = 0; i < nmatdata; i++)
        {
         if (gmv_data.longdata1[i] > n_mats)
            printf("Error, material for cell %d is greater than %d\n",
                    i+1,n_mats);
        }
     }
   if (d_type == 1)
     {
      for (i = 0; i < nmatdata; i++)
        {
         if (gmv_data.longdata1[i] > n_mats)
            printf("Error, material for node %d is greater than %d\n",
                    i+1,n_mats);
        }
     }
}


void do_velocity(void)
{
  int d_type;
  int nvel, i;
  double minu, maxu, minv, maxv, minw, maxw, val;

   if (gmv_data.datatype == CELL) d_type = 0;
   if (gmv_data.datatype == NODE) d_type = 1;
   if (gmv_data.datatype == FACE) d_type = 2;
   nvel = gmv_data.num;
   minu = MAXVAL;  maxu = -MAXVAL;
   minv = MAXVAL;  maxv = -MAXVAL;
   minw = MAXVAL;  maxw = -MAXVAL;
   for (i = 0; i < nvel; i++)
     {
      val = gmv_data.doubledata1[i];
      minu = (minu < val) ? minu : val;
      maxu = (maxu > val) ? maxu : val;
      val = gmv_data.doubledata2[i];
      minv = (minv < val) ? minv : val;
      maxv = (maxv > val) ? maxv : val;
      val = gmv_data.doubledata3[i];
      minw = (minw < val) ? minw : val;
      maxw = (maxw > val) ? maxw : val;
     }
   printf(" Read %s velocities.\n",type_str[d_type]);
   printf("   min. u = %lf,  max. u = %lf\n",minu, maxu);
   printf("   min. v = %lf,  max. v = %lf\n",minv, maxv);
   printf("   min. w = %lf,  max. w = %lf\n",minw, maxw);
}


void do_variables(void)
{
  char varname[40];
  int d_type;
  int i, numdat;
  static int ifirst = 1;
  double min, max, val;

   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE ||
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading variables.\n");
        }
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;
      if (gmv_data.datatype == FACE) d_type = 2;
      numdat = gmv_data.num;
      strcpy(varname,gmv_data.name1);
      min = MAXVAL;  max = -MAXVAL;
      for (i = 0; i < numdat; i++)
        {
         val = gmv_data.doubledata1[i];
         min = (min < val) ? min : val;
         max = (max > val) ? max : val;
        }
      printf("   %s (%s)  min = %lf, max = %lf\n",varname,type_str[d_type],
                                              min,max);
     }
}


void do_flags(void)
{
  char flagname[33], fnames[33];
  int d_type, numtypes;
  int i, numdat;
  static int ifirst = 1;

   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading flags.\n");
        }
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;
      numdat = gmv_data.num;
      strcpy(flagname,gmv_data.name1);
      numtypes = gmv_data.num2;
      printf("  %s (%s) has %d types\n",flagname,type_str[d_type],numtypes);
      
      for (i = 0; i < numtypes; i++)
        {
         strcpy(fnames,&gmv_data.chardata1[i*33]);
         printf("    %s\n",fnames);
        }
     }
}


void do_polygons(void)
{
  static int first = 1, npolys;

   if (gmv_data.datatype == REGULAR)
     {
      if (first)
        {
         first = 0;
         npolys = 0;
        }
      npolys++;
     }

   if (gmv_data.datatype == ENDKEYWORD)
      printf(" Read %d polygons.\n",npolys);
}


void do_tracers(void)
{
  int ntrace, i;
  double min, max, val;
  char word1[40];

   if (gmv_data.datatype == XYZ)
     {
      ntrace = gmv_data.num;
      printf(" Reading %d Particles.\n", ntrace);
      return;
     }

   if (gmv_data.datatype == TRACERDATA)
     {
      ntrace = gmv_data.num;
      strcpy(word1,gmv_data.name1);
      min = MAXVAL;  max = -MAXVAL;
      for (i = 0; i < ntrace; i++)
        {
         val = gmv_data.doubledata1[i];
         min = (min < val) ? min : val;
         max = (max > val) ? max : val;
        }
      printf("   %s  min = %lf, max = %lf\n",word1,min,max);
      return;
     }
}


void do_units()
{
  int i;
  static int ifirst = 1;
  int lnumunits_node, lnumunits_cell;
  char varname[40], unitname[40];

   if (ifirst)
     {
      ifirst = 0;
      printf(" Reading units.\n");
     }

   /*  Save XYZ units.  */
   if (gmv_data.datatype == XYZ)
      printf("   XYZ units = %s/n",gmv_data.chardata2);

   /*  Save velocity units.  */
   if (gmv_data.datatype == VEL)
      printf("   Velocity units = %s/n",gmv_data.chardata2);

   /*  Allocate and save node units.  */
   if (gmv_data.datatype == NODE)
     {
      lnumunits_node = gmv_data.num;

      printf("  Node variable units:\n");
      for (i = 0; i < lnumunits_node; i++)
        {
         strcpy(varname,&gmv_data.chardata1[i*33]);
         strcpy(unitname,&gmv_data.chardata2[i*33]);
         printf("     %s = %s\n",varname,unitname);
        }
     }

   /*  Allocate and save cell units.  */
   if (gmv_data.datatype == CELL)
     {
      lnumunits_cell = gmv_data.num;
      units.ncells = lnumunits_cell;

      for (i = 0; i < lnumunits_cell; i++)
        {
         strcpy(units.cfldname[i],&gmv_data.chardata1[i*33]);
         strcpy(units.cfldunit[i],&gmv_data.chardata2[i*33]);
        }
     }

   if (gmv_data.datatype == ENDKEYWORD) ifirst = 1;
}


void do_surface(void)
{
  static int ifirst = 1, numsurf;
  int nverts, *verts;

   if (gmv_data.datatype == REGULAR) 
     {
      if (ifirst)
        {
         ifirst = 0;
         numsurf = gmv_data.num;
        }
     }

   if (gmv_data.datatype == ENDKEYWORD)
     {
      printf(" Read %d surface elements.\n",numsurf);
     }
}


void do_surfvel(void)
{
  int nvel, i;
  double minu, maxu, minv, maxv, minw, maxw, val;

   nvel = gmv_data.num;
   minu = MAXVAL;  maxu = -MAXVAL;
   minv = MAXVAL;  maxv = -MAXVAL;
   minw = MAXVAL;  maxw = -MAXVAL;
   for (i = 0; i < nvel; i++)
     {
      val = gmv_data.doubledata1[i];
      minu = (minu < val) ? minu : val;
      maxu = (maxu > val) ? maxu : val;
      val = gmv_data.doubledata2[i];
      minv = (minv < val) ? minv : val;
      maxv = (maxv > val) ? maxv : val;
      val = gmv_data.doubledata3[i];
      minw = (minw < val) ? minw : val;
      maxw = (maxw > val) ? maxw : val;
     }
   printf(" Read Surface velocities.\n");
   printf("   min. u = %lf,  max. u = %lf\n",minu, maxu);
   printf("   min. v = %lf,  max. v = %lf\n",minv, maxv);
   printf("   min. w = %lf,  max. w = %lf\n",minw, maxw);
}


void do_surfvars(void)
{
  char varname[40];
  int i, numdat;
  static int ifirst = 1;
  double min, max, val;

   if (gmv_data.datatype == REGULAR)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading surface variables.\n");
        }
      numdat = gmv_data.num;
      strcpy(varname,gmv_data.name1);
      min = MAXVAL;  max = -MAXVAL;
      for (i = 0; i < numdat; i++)
        {
         val = gmv_data.doubledata1[i];
         min = (min < val) ? min : val;
         max = (max > val) ? max : val;
        }
      printf("   %s  min = %lf, max = %lf\n",varname,min,max);
     }
}


void do_surfflag(void)
{
  char flagname[33], fnames[33];
  int numtypes;
  int i, numdat;
  static int ifirst = 1;

   if (gmv_data.datatype == REGULAR)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading surface flags.\n");
        }
      numdat = gmv_data.num;
      strcpy(flagname,gmv_data.name1);
      numtypes = gmv_data.num2;
      printf("   %s has %d types\n",flagname,numtypes);
      
      for (i = 0; i < numtypes; i++)
        {
         strcpy(fnames,&gmv_data.chardata1[i*33]);
         printf("     %s\n",fnames);
        }
     }
}

void do_groups(void)
{
  char groupname[33];
  int d_type, numgroup, i;
  static int ifirst = 1;

   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE ||
        gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading groups.\n");
        }
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;
      if (gmv_data.datatype == FACE) d_type = 2;
      if (gmv_data.datatype == SURF) d_type = 3;
      numgroup = gmv_data.num;
      strcpy(groupname,gmv_data.name1);
      printf("   Group %s for %s has %d elements.\n",groupname,
                                            type_str[d_type],numgroup);
     }
}


void do_subvars(void)
{
  char varname[40];
  int d_type;
  int i, numdat;
  static int ifirst = 1;
  double min, max, val;

   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE ||
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         printf(" Reading subvars.\n");
        }
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;
      if (gmv_data.datatype == FACE) d_type = 2;
      numdat = gmv_data.num;
      strcpy(varname,gmv_data.name1);
      min = MAXVAL;  max = -MAXVAL;
      for (i = 0; i < numdat; i++)
        {
         val = gmv_data.doubledata1[i];
         min = (min < val) ? min : val;
         max = (max > val) ? max : val;
        }
      printf("   %s %s  min = %lf, max = %lf\n",varname,type_str[d_type],
                                              min,max);
     }
}


void do_ghosts()
{
  int i, j, d_type, nelem;

   /*  Process node or cell ghost info. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE)
     {

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;

      nelem = gmv_data.nlongdata1;
      printf("GMV reading %d ghost %s.\n",nelem,type_str[d_type]);

     }
}


void do_vects()
{
  int i, d_type, ncomps, ic;
  long nvarin;
  char varname[33], vectname[33];
  static int ifirst = 1;

   /*  Process node, cell or face vector data. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE || 
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf(" Reading vectors.\n");
        }

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) d_type = 0;
      if (gmv_data.datatype == NODE) d_type = 1;
      if (gmv_data.datatype == FACE) d_type = 2;

      /*  Allocate and print vector name.  */
      strcpy(vectname,gmv_data.name1);
      ncomps = gmv_data.num2;
      printf("    %s for %s has %d components.\n",vectname,type_str[d_type],
                                                  ncomps);
     }
}
