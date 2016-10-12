#ifndef _X3DREADH_
#define _X3DREADH_

#ifndef X3D_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif


EXTERN struct
  {
   int process;
   int numdim;
   int nummats;
   int numnodes;
   int numfaces;
   int numcells;
   int numghosts;
   int numslaves;
   int nodes_per_slave;
   int nodes_per_face;
   int faces_per_cell;
   int num_node_fields;
   int num_cell_fields;
  }  x3d_header;

EXTERN char **x3dmatnames;

EXTERN double *mateos, *matopc;

EXTERN double *x3d_x, *x3d_y, *x3d_z;   // Note, need to free after use.

EXTERN struct
  {
   int number;
   int nverts;
   int verts[20];
   int proc_no;
   int opp_pe;
   int opp_face;
  }  x3d_face;

EXTERN struct
  {
   int number;
   int nfaces;
   int faces[20];
  }  x3d_cell;

EXTERN int nslaves;

EXTERN struct
  {
   int number;
   int node_no;
   int nmasters;
   int masters[20];
  } x3d_slave;

EXTERN int nghosts;

EXTERN struct
  {
   int number;
   int node_no;
   int own_proc;
   int opp_ghost;
  } x3d_ghost;

EXTERN struct
  {
   char name[40];
   int typedata;  // 0 = integer, 1 = double.
   int *int_data; // Note, need to free after each use.
   double *double_data; // Note, need to free after each use.
  } x3d_cell_data;

EXTERN struct
  {
   char name[40];
   int typedata;  // 0 = integer, 1 = double.
   int *int_data; // Note, need to free after each use.
   double *double_data; // Note, need to free after each use.
  } x3d_node_data;

EXTERN struct
  {
   int num_entries;
   int *int_data; // Note, need to free after each use.
  } x3d_Bdy_data;

EXTERN struct
  {
   int type; // 0 = tag name, 1 = volume fraction.
   int num_entries;
   int *int_data; // Note, need to free after each use.
   double *double_data; // Note, need to free after each use.
  } x3d_Reg_data;

#endif
