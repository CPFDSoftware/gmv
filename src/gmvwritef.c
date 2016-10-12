/*
    Copyright, 1991, The Regents of the University of
    California.  This software was produced under a U. S.
    Government contract (W-7405-ENG-36) by the Los Alamos
    National Laboratory, which is operated by the
    University of California for the U. S. Department of
    Energy.  The U. S. Government is licensed to use,
    reproduce, and distribute this software.  Permission
    is granted to the public to copy and use this software
    without charge, provided that this Notice and any statement
    of authorship are reproduced on all copies.  Neither the
    Government nor the University makes any warranty, express
    or implied, or assumes any liability or responsibility for
    the use of this software.


     Software Author: Kevin L. Bolling

     Numerous changes made by Jeff Hinrichs
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gmvwrite.c"

char charbuffer[90];

void *fortchartoc(char* filnam);
void *fortchartocfield(char* filnam,int length);

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_openfile(char* filnam)
#elif unicos
FGMVWRITE_OPENFILE(char* filnam)
#else
fgmvwrite_openfile_(char* filnam)
#endif
{
   gmvwrite_openfile(fortchartoc(filnam));
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_openfile_ascii(char* filnam)
#elif unicos
FGMVWRITE_OPENFILE_ASCII(char* filnam)
#else
fgmvwrite_openfile_ascii_(char* filnam)
#endif
{
   gmvwrite_openfile_ascii(fortchartoc(filnam));
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_openfile_ir(char *filnam, int *isize, int *rsize)
#elif unicos
FGMVWRITE_OPENFILE_ir(char *filnam, int *isize, int *rsize)
#else
fgmvwrite_openfile_ir_(char *filnam, int *isize, int *rsize)
#endif
{
   gmvwrite_openfile_ir(fortchartoc(filnam), *isize, *rsize);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_openfile_cxir(char *filnam, int *isize, int *rsize)
#elif unicos
FGMVWRITE_OPENFILE_cxir(char *filnam, int *isize, int *rsize)
#else
fgmvwrite_openfile_cxir_(char *filnam, int *isize, int *rsize)
#endif
{
   gmvwrite_openfile_cxir(fortchartoc(filnam), *isize, *rsize);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_openfile_ir_ascii(char *filnam, int *isize, int *rsize)
#elif unicos
FGMVWRITE_OPENFILE_IR_ASCII(char *filnam, int *isize, int *rsize)
#else
fgmvwrite_openfile_ir_ascii_(char *filnam, int *isize, int *rsize)
#endif
{
   gmvwrite_openfile_ir_ascii(fortchartoc(filnam), *isize, *rsize);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_closefile()
#elif unicos
FGMVWRITE_CLOSEFILE()
#else
fgmvwrite_closefile_()
#endif
 {
    gmvwrite_closefile();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodes_fromfile(char filnam[], long nndes)
#elif unicos
FGMVWRITE_NODES_FROMFILE(char filnam[], long nndes)
#else
fgmvwrite_nodes_fromfile_(char filnam[], long nndes)
#endif
{
   gmvwrite_nodes_fromfile(fortchartoc(filnam), nndes);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_node_data(char *nndes, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_NODE_DATA(char *nndes, char *x, char *y, char *z)
#else
fgmvwrite_node_data_(char *nndes, char *x, char *y, char *z)
#endif
 {
    gmvwrite_node_data(nndes, x, y, z);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_node_data_struct(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_NODE_DATA_STRUCT(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#else
fgmvwrite_node_data_struct_(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#endif
 {
  gmvwrite_node_data_struct(nxv, nyv, nzv, x, y, z);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_node_data_lstruct(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_NODE_DATA_LSTRUCT(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#else
fgmvwrite_node_data_lstruct_(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#endif
{
 gmvwrite_node_data_lstruct(nxv, nyv, nzv, x, y, z);
}   

/* ---------------------------------------------------------------- */

void
#if defined hp || defined ibm
fgmvwrite_node_data_amr(int *nxc, int *nyc, int *nzc, char *x0, char *y0,
		        char *z0, char *dx, char *dy, char *dz)
#elif unicos
FGMVWRITE_NODE_DATA_AMR(int *nxc, int *nyc, int *nzc, char *x0, char *y0,
			char *z0, char *dx, char *dy, char *dz)
#else
fgmvwrite_node_data_amr_(int *nxc, int *nyc, int *nzc, char *x0, char *y0,
			 char *z0, char *dx, char *dy, char *dz)
#endif
{
  gmvwrite_node_data_amr(*nxc, *nyc, *nzc, x0, y0, z0, dx, dy, dz);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodev_fromfile(char filnam[], long nndes)
#elif unicos
FGMVWRITE_NODEv_FROMFILE(char filnam[], long nndes)
#else
fgmvwrite_nodev_fromfile_(char filnam[], long nndes)
#endif
{
   gmvwrite_nodev_fromfile(fortchartoc(filnam), nndes);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodev_data(char *nndes, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_NODEV_DATA(char *nndes, char *x, char *y, char *z)
#else
fgmvwrite_nodev_data_(char *nndes, char *x, char *y, char *z)
#endif
 {
    gmvwrite_nodev_data(nndes, x, y, z);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodev_data_lstruct(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_NODEV_DATA_LSTRUCT(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#else
fgmvwrite_nodev_data_lstruct_(char *nxv, char *nyv, char *nzv, char *x, char *y, char *z)
#endif
{
 gmvwrite_nodev_data_lstruct(nxv, nyv, nzv, x, y, z);
}   

/* ---------------------------------------------------------------- */

void
#if defined hp || defined ibm
fgmvwrite_cells_amr(char *numcells, char *numtop, char *daughters)
#elif unicos
FGMVWRITE_CELLS_AMR(char *numcells, char *numtop, char *daughters)
#else
fgmvwrite_cells_amr_(char *numcells, char *numtop, char *daughters)
#endif
{
  gmvwrite_cells_amr(numcells, numtop, daughters);
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cell_header(char *ncells)
#elif unicos
FGMVWRITE_CELL_HEADER(char *ncells)
#else
fgmvwrite_cell_header_(char *ncells)
#endif
 {
  gmvwrite_cell_header(ncells);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cells_fromfile(char filnam[], long ncells)
#elif unicos
FGMVWRITE_CELLS_FROMFILE(char filnam[], long ncells)
#else
fgmvwrite_cells_fromfile_(char filnam[], long ncells)
#endif
{

 gmvwrite_cells_fromfile(fortchartoc(filnam),ncells);

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cell_type(char cell_type[], int *nverts, char *nodes)
#elif unicos
FGMVWRITE_CELL_TYPE(char cell_type[], int *nverts, char *nodes)
#else
fgmvwrite_cell_type_(char cell_type[], int *nverts, char *nodes)
#endif
 {

  gmvwrite_cell_type(fortchartocfield(cell_type,8), *nverts, nodes);

 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_general_cell_type(char cell_type[], int nverts[], int *nfaces, char *nodeids)
#elif unicos
FGMVWRITE_GENERAL_CELL_TYPE(char cell_type[], int nverts[], int *nfaces, char *nodeids)
#else
fgmvwrite_general_cell_type_(char cell_type[], int nverts[], int *nfaces, char *nodeids)
#endif
 {

  gmvwrite_general_cell_type(fortchartocfield(cell_type,8), nverts, *nfaces, nodeids);

 }                  

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_faces_fromfile(char filnam[], long nfaces)
#elif unicos
FGMVWRITE_FACES_FROMFILE(char filnam[], long nfaces)
#else
fgmvwrite_faces_fromfile_(char filnam[], long nfaces)
#endif
{

 gmvwrite_faces_fromfile(fortchartoc(filnam), nfaces);

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_face_header(char *nfaces, char *ncells)
#elif unicos
FGMVWRITE_FACE_HEADER(char *nfaces, char *ncells)
#else
fgmvwrite_face_header_(char *nfaces, char *ncells)
#endif
 {
  gmvwrite_face_header(nfaces, ncells);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_face_data(int *nverts, char *nodeids, char *cellid1, char *cellid2)
#elif unicos
FGMVWRITE_FACE_DATA(int *nverts, char *nodeids, char *cellid1, char *cellid2)
#else
fgmvwrite_face_data_(int *nverts, char *nodeids, char *cellid1, char *cellid2)
#endif
 {

  gmvwrite_face_data(*nverts, nodeids, cellid1, cellid2);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vfaces_fromfile(char filnam[], long nfaces)
#elif unicos
FGMVWRITE_VFACES_FROMFILE(char filnam[], long nfaces)
#else
fgmvwrite_vfaces_fromfile_(char filnam[], long nfaces)
#endif
{

 gmvwrite_vfaces_fromfile(fortchartoc(filnam), nfaces);

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vface_header(char *nfaces)
#elif unicos
FGMVWRITE_VFACE_HEADER(char *nfaces)
#else
fgmvwrite_vface_header_(char *nfaces)
#endif
 {
  gmvwrite_vface_header(nfaces);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vface_data(int *nverts, int *facepe, char *oppface, 
                     int *oppfacepe, char *cellid, char *nodeids)
#elif unicos
FGMVWRITE_VFACE_DATA(int *nverts, int *facepe, char *oppface, 
                     int *oppfacepe, char *cellid, char *nodeids)
#else
fgmvwrite_vface_data_(int *nverts, int *facepe, char *oppface, 
                      int *oppfacepe, char *cellid, char *nodeids)
#endif
 {
  gmvwrite_vface_data(*nverts, *facepe, oppface, *oppfacepe, cellid, nodeids);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_material_fromfile(char filnam[])
#elif unicos
FGMVWRITE_MATERIAL_FROMFILE(char filnam[])
#else
fgmvwrite_material_fromfile_(char filnam[])
#endif
{

 gmvwrite_material_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_material_header(int *nmats, int *data_type)
#elif unicos
FGMVWRITE_MATERIAL_HEADER(int *nmats, int *data_type)
#else
fgmvwrite_material_header_(int *nmats, int *data_type)
#endif
 {
  gmvwrite_material_header(*nmats, *data_type);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_material_name(char matname[])
#elif unicos
FGMVWRITE_MATERIAL_NAME(char matname[])
#else
fgmvwrite_material_name_(char matname[])
#endif
 {
  char name[10];
  /* teminate matname with null character */
  strncpy(name, matname,8);
  *(name + 8) = '\0';
  gmvwrite_material_name(name);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_material_ids(int matids[], int *data_type)
#elif unicos
FGMVWRITE_MATERIAL_IDS(int matids[], int *data_type)
#else
fgmvwrite_material_ids_(int matids[], int *data_type)
#endif
 {
  gmvwrite_material_ids(matids, *data_type);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_velocity_data(int *data_type, char *u, char *v, char *w)
#elif unicos
FGMVWRITE_VELOCITY_DATA(int *data_type, char *u, char *v, char *w)
#else
fgmvwrite_velocity_data_(int *data_type, char *u, char *v, char *w)
#endif
 {
  gmvwrite_velocity_data(*data_type, u, v, w);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_variable_header()
#elif unicos
FGMVWRITE_VARIABLE_HEADER()
#else
fgmvwrite_variable_header_()
#endif
 {
  gmvwrite_variable_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_variable_name_data(int *data_type, char varname[], char *vids)
#elif unicos
FGMVWRITE_VARIABLE_NAME_DATA(int *data_type, char varname[], char *vids)
#else
fgmvwrite_variable_name_data_(int *data_type, char varname[], char *vids)
#endif
 {
  char name[10];
  /*  terminate varname with null character  */
  strncpy(name, varname, 8);
  *(name+8) = '\0';
  gmvwrite_variable_name_data(*data_type, name, vids);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_variable_endvars()
#elif unicos
FGMVWRITE_VARIABLE_ENDVARS()
#else
fgmvwrite_variable_endvars_()
#endif
 {
  gmvwrite_variable_endvars();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_fromfile(char filnam[])
#elif unicos
FGMVWRITE_FLAG_FROMFILE(char filnam[])
#else
fgmvwrite_flag_fromfile_(char filnam[])
#endif
{

 gmvwrite_flag_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_header()
#elif unicos
FGMVWRITE_FLAG_HEADER()
#else
fgmvwrite_flag_header_()
#endif
{
  gmvwrite_flag_header();
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_name(char flagname[], int *numtypes, int *data_type)
#elif unicos
FGMVWRITE_FLAG_NAME(char flagname[], int *numtypes, int *data_type)
#else
fgmvwrite_flag_name_(char flagname[], int *numtypes, int *data_type)
#endif
 {
  char name[10];
  strncpy(name, flagname, 8);
  /*  terminate flagname with null character  */
  *(name+8) = '\0';
  gmvwrite_flag_name(name, *numtypes, *data_type);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_subname(char subname[])
#elif unicos
FGMVWRITE_FLAG_SUBNAME(char subname[])
#else
fgmvwrite_flag_subname_(char subname[])
#endif
 {
  char type[10];
  strncpy(type, subname, 8);
  /*  terminate subname with null character  */
  *(type+8) = '\0';
  gmvwrite_flag_subname(type);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_data(int *data_type,int flagdata[])
#elif unicos
FGMVWRITE_FLAG_DATA(int *data_type,int flagdata[])
#else
fgmvwrite_flag_data_(int *data_type, int flagdata[])
#endif
 {
  gmvwrite_flag_data(*data_type, flagdata);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_flag_endflag()
#elif unicos
FGMVWRITE_FLAG_ENDFLAG()
#else
fgmvwrite_flag_endflag_()
#endif
{
  gmvwrite_flag_endflag();
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_polygons_fromfile(char filnam[])
#elif unicos
FGMVWRITE_POLYGONS_FROMFILE(char filnam[])
#else
fgmvwrite_polygons_fromfile_(char filnam[])
#endif
{

 gmvwrite_polygons_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_polygons_header()
#elif unicos
FGMVWRITE_POLYGONS_HEADER()
#else
fgmvwrite_polygons_header_()
#endif
 {
  gmvwrite_polygons_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_polygons_data(int *nverts, int *matnum, char *x, char *y, char *z)
#elif unicos
FGMVWRITE_POLYGONS_DATA(int *nverts, int *matnum, char *x, char *y, char *z)
#else 
fgmvwrite_polygons_data_(int *nverts, int *matnum, char *x, char *y, char *z)
#endif
 {  
  gmvwrite_polygons_data(*nverts, *matnum, x, y, z);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_polygons_endpoly()
#elif unicos
FGMVWRITE_POLYGONS_ENDPOLY()
#else
fgmvwrite_polygons_endpoly_()
#endif
 {
  gmvwrite_polygons_endpoly();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_tracers_header(int *ntracers, char *u, char *y, char *z)
#elif unicos
FGMVWRITE_TRACERS_HEADER(int *ntracers, char *u, char *y, char *z)
#else
fgmvwrite_tracers_header_(int *ntracers, char *u, char *y, char *z)
#endif
 {
  gmvwrite_tracers_header(*ntracers, u, y, z);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_tracers_name_data(int *ntracers, char tracername[], char *data)
#elif unicos
FGMVWRITE_TRACERS_NAME_DATA(int *ntracers, char tracername[], char *data)
#else
fgmvwrite_tracers_name_data_(int *ntracers, char tracername[], char *data)
#endif
 {
  char name[10];
  strncpy(name, tracername,8);
  /*  terminate tracername with null character  */
  *(name+8) = '\0';
  gmvwrite_tracers_name_data(*ntracers, name, data);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_tracers_endtrace()
#elif unicos
FGMVWRITE_TRACERS_ENDTRACE()
#else
fgmvwrite_tracers_endtrace_()
#endif
 {
  gmvwrite_tracers_endtrace();
 }
  
/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_probtime(double *ptime)
#elif unicos
FGMVWRITE_PROBTIME(double *ptime)
#else
fgmvwrite_probtime_(double *ptime)
#endif
 {
  gmvwrite_probtime(*ptime);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cycleno(int *number)
#elif unicos
FGMVWRITE_CYCLENO(int *number)
#else
fgmvwrite_cycleno_(int *number)
#endif
 {
  gmvwrite_cycleno(*number);
 }

/* ---------------------------------------------------------------- */

void *fortchartoc(char* filnam){

 int i;


 /*  teminate fname with null character */

 i = 0;
 while ( *(filnam+i) != ' ')
  {
   charbuffer[i] = *(filnam+i);
   i++;
  }
 charbuffer[i] = '\0';

 return charbuffer;

}

/* ---------------------------------------------------------------- */

void *fortchartocfield(char* filnam,int length){

 int i;


 /*  teminate fname with null character */

 for(i = 0; (i < length) && (*(filnam+i) != ' '); i++)
    charbuffer[i] = *(filnam+i);

 for(; i < length; i++)
    charbuffer[i] = ' ';

 charbuffer[++i] = '\0';

 return charbuffer;

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodeids(char *nodeids)
#elif unicos
FGMVWRITE_NODEIDS(char *nodeids)
#else
fgmvwrite_nodeids_(char *nodeids)
#endif
 {

  gmvwrite_nodeids(nodeids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_nodeids_fromfile(char filnam[])
#elif unicos
FGMVWRITE_NODEIDS_FROMFILE(char filnam[])
#else
fgmvwrite_nodeids_fromfile_(char filnam[])
#endif
{

 gmvwrite_nodeids_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cellids(char *cellids)
#elif unicos
FGMVWRITE_CELLIDS(char *cellids)
#else
fgmvwrite_cellids_(char *cellids)
#endif
 {

  gmvwrite_cellids(cellids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_cellids_fromfile(char filnam[])
#elif unicos
FGMVWRITE_CELLIDS_FROMFILE(char filnam[])
#else
fgmvwrite_cellids_fromfile_(char filnam[])
#endif
{

 gmvwrite_cellids_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surface_header(char *nsurf)
#elif unicos
FGMVWRITE_SURFACE_HEADER(char *nsurf)
#else
fgmvwrite_surface_header(char *nsurf)
#endif
 {
  gmvwrite_surface_header(nsurf);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surface_data(int *nverts, char *nodeids)
#elif unicos
FGMVWRITE_SURFACE_DATA(int *nverts, char *nodeids2)
#else
fgmvwrite_surface_data_(int *nverts, char *nodeids)
#endif
 {

  gmvwrite_surface_data(*nverts, nodeids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surface_fromfile(char filnam[], long nsrf)
#elif unicos
FGMVWRITE_surface_FROMFILE(char filnam[], long nsrf)
#else
fgmvwrite_surface_fromfile_(char filnam[], long nsrf)
#endif
{

 gmvwrite_surface_fromfile(fortchartoc(filnam), nsrf);

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfmats(int matids[])
#elif unicos
FGMVWRITE_SURFMATS(int matids[])
#else
fgmvwrite_surfmats_(int matids[])
#endif
 {
  gmvwrite_surfmats(matids);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfvel(char *u, char *v, char *w)
#elif unicos
FGMVWRITE_SURFVEL(char *u, char *v, char *w)
#else
fgmvwrite_surfvel(char *u, char *v, char *w)
#endif
 {
  gmvwrite_surfvel(u, v, w);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfvars_header()
#elif unicos
FGMVWRITE_SURFVARS_HEADER()
#else
fgmvwrite_surfvars_header_()
#endif
 {
  gmvwrite_surfvars_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfvars_name_data(char varname[], char *vids)
#elif unicos
FGMVWRITE_SURFVARS_NAME_DATA(char varname[], char *vids)
#else
fgmvwrite_surfvars_name_data_(char varname[], char *vids)
#endif
 {
  char name[10];
  /*  terminate varname with null character  */
  strncpy(name, varname, 8);
  *(name+8) = '\0';
  gmvwrite_surfvars_name_data(name, vids);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfvars_endsvar()
#elif unicos
FGMVWRITE_SURFVARS_ENDSVAR()
#else
fgmvwrite_surfvars_endsvar_()
#endif
 {
  gmvwrite_surfvars_endsvar();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfflag_header()
#elif unicos
FGMVWRITE_SURFFLAG_HEADER()
#else
fgmvwrite_surfflag_header_()
#endif
{
  gmvwrite_surfflag_header();
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfflag_name(char flagname[], int *numtypes)
#elif unicos
FGMVWRITE_SURFFLAG_NAME(char flagname[], int *numtypes)
#else
fgmvwrite_surfflag_name_(char flagname[], int *numtypes)
#endif
 {
  char name[10];
  strncpy(name, flagname, 8);
  /*  terminate flagname with null character  */
  *(name+8) = '\0';
  gmvwrite_surfflag_name(name, *numtypes);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfflag_subname(char subname[])
#elif unicos
FGMVWRITE_SURFFLAG_SUBNAME(char subname[])
#else
fgmvwrite_surfflag_subname_(char subname[])
#endif
 {
  char type[10];
  strncpy(type, subname, 8);
  /*  terminate subname with null character  */
  *(type+8) = '\0';
  gmvwrite_surfflag_subname(type);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfflag_data(int flagdata[])
#elif unicos
FGMVWRITE_SURFFLAG_DATA(int flagdata[])
#else
fgmvwrite_surfflag_data_(int flagdata[])
#endif
 {
  gmvwrite_surfflag_data(flagdata);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfflag_endflag()
#elif unicos
FGMVWRITE_SURFFLAG_ENDFLAG()
#else
fgmvwrite_surfflag_endflag_()
#endif
{
  gmvwrite_surfflag_endflag();
}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_units_fromfile(char filnam[])
#elif unicos
FGMVWRITE_UNITS_FROMFILE(char filnam[])
#else
fgmvwrite_units_fromfile_(char filnam[])
#endif
{

 gmvwrite_units_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_units_header()
#elif unicos
FGMVWRITE_UNITS_HEADER()
#else
fgmvwrite_units_header_()
#endif
 {
  gmvwrite_units_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_units_typehdr(int *data_type, int numtypes)
#elif unicos
FGMVWRITE_UNITS_TYPEHDR(int *data_type, int numtypes)
#else
fgmvwrite_units_typehdr_(int *data_type, int numtypes)
#endif
 {
  gmvwrite_units_typehdr(*data_type, numtypes);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_units_name(char fldname[], char unitname[])
#elif unicos
FGMVWRITE_UNITS_NAME(char fldname[], char unitname[])
#else
fgmvwrite_units_name_(char fldname[], char unitname[])
#endif
 {
  char type1[10], type2[20];
  strncpy(type1, fldname, 8);
  strncpy(type2, unitname, 16);
  /*  terminate with null character  */
  *(type1+8) = '\0';
  *(type2+16) = '\0';
  gmvwrite_units_name(type1,type2);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_units_endunit()
#elif unicos
FGMVWRITE_UNITS_ENDUNIT()
#else
fgmvwrite_units_endunit_()
#endif
 {
  gmvwrite_units_endunit();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vinfo_header()
#elif unicos
FGMVWRITE_VINFO_HEADER()
#else
fgmvwrite_vinfo_header_()
#endif
 {
  gmvwrite_vinfo_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vinfo_name_data(int nelem, int nlines, char varname[], char *vids)
#elif unicos
FGMVWRITE_VINFO_NAMEDATA(int nelem, int nlines, char varname[], char *vids)
#else
fgmvwrite_vinfo_name_data_(int nelem, int nlines, char varname[], char *vids)
#endif
 {
  char name[10];
  /*  terminate varname with null character  */
  strncpy(name, varname, 8);
  *(name+8) = '\0';
  gmvwrite_vinfo_name_data(nelem, nlines, name, vids);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_vinfo_endvinfo()
#elif unicos
FGMVWRITE_VINFO_ENDVINFO()
#else
fgmvwrite_vinfo_endvinfo_()
#endif
 {
  gmvwrite_vinfo_endvinfo();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_traceids(int ntracers, char *traceids)
#elif unicos
FGMVWRITE_FACEIDS(int ntracers, char *traceids)
#else
fgmvwrite_traceids_(int ntracers, char *traceids)
#endif
 {

  gmvwrite_traceids(ntracers, traceids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_traceids_fromfile(char filnam[])
#elif unicos
FGMVWRITE_NODEIDS_FROMFILE(char filnam[])
#else
fgmvwrite_traceids_fromfile_(char filnam[])
#endif
{

 gmvwrite_traceids_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_faceids(char *faceids)
#elif unicos
FGMVWRITE_FACEIDS(char *faceids)
#else
fgmvwrite_faceids_(char *faceids)
#endif
 {

  gmvwrite_faceids(faceids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_faceids_fromfile(char filnam[])
#elif unicos
FGMVWRITE_NODEIDS_FROMFILE(char filnam[])
#else
fgmvwrite_faceids_fromfile_(char filnam[])
#endif
{

 gmvwrite_faceids_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_group_fromfile(char filnam[])
#elif unicos
FGMVWRITE_GROUP_FROMFILE(char filnam[])
#else
fgmvwrite_group_fromfile_(char filnam[])
#endif
{

 gmvwrite_group_fromfile(fortchartoc(filnam));

}

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_group_header()
#elif unicos
FGMVWRITE_GROUP_HEADER()
#else
fgmvwrite_group_header_()
#endif
 {
  gmvwrite_group_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_group_data(char groupname[], int data_type, int numgrp, 
                     char *group_data)
#elif unicos
FGMVWRITE_GROUP_DATA(char groupname[], int data_type, int numgrp, 
                     char *group_data)
#else
fgmvwrite_group_data_(char groupname[], int data_type, int numgrp, 
                      char *group_data)
#endif
 {
  char name[10];
  /*  terminate groupname with null character  */
  strncpy(name, groupname, 8);
  *(name+8) = '\0';
  gmvwrite_group_data(name, data_type, numgrp, group_data);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_group_endgroup()
#elif unicos
FGMVWRITE_GROUP_ENDGROUP()
#else
fgmvwrite_group_endgroup_()
#endif
 {
  gmvwrite_group_endgroup();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_surfids(char *surfids)
#elif unicos
FGMVWRITE_SURFIDS(char *surfids)
#else
fgmvwrite_surfids_(char *surfids)
#endif
 {

  gmvwrite_surfids(surfids);

 }     

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_codename(char codename[])
#elif unicos
FGMVWRITE_CODENAME(char codename[])
#else
fgmvwrite_codename_(char codename[])
#endif
 {
  char name[10];
  /*  terminate codename with null character  */
  strncpy(name, codename, 8);
  *(name+8) = '\0';
  gmvwrite_codename(name);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_codever(char codever[])
#elif unicos
FGMVWRITE_CODEVER(char codever[])
#else
fgmvwrite_codever_(char codever[])
#endif
 {
  char name[10];
  /*  terminate codever with null character  */
  strncpy(name, codever, 8);
  *(name+8) = '\0';
  gmvwrite_codever(name);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_simdate(char simdate[])
#elif unicos
FGMVWRITE_SIMDATE(char simdate[])
#else
fgmvwrite_simdate_(char simdate[])
#endif
 {
  char name[10];
  /*  terminate simdate with null character  */
  strncpy(name, simdate, 8);
  *(name+8) = '\0';
  gmvwrite_simdate(name);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_subvars_header()
#elif unicos
FGMVWRITE_SUBVARS_HEADER()
#else
fgmvwrite_subvars_header_()
#endif
 {
  gmvwrite_subvars_header();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_subvars_name_data(int data_type, int numelem, char varname[], 
                          void *vids, void *vdata)
#elif unicos
FGMVWRITE_SUBVARS_NAME_DATA(int data_type, int numelem, char varname[], 
                          void *vids, void *vdata)
#else
fgmvwrite_subvars_name_data_(int data_type, int numelem, char varname[], 
                           void *vids, void *vdata)
#endif
 {
  char name[10];
  /*  terminate varname with null character  */
  strncpy(name, varname, 8);
  *(name+8) = '\0';
  gmvwrite_subvars_name_data(data_type, *numelem, name, vids, vdata);
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_subvars_endsubv()
#elif unicos
FGMVWRITE_SUBVARS_ENDSUBV()
#else
fgmvwrite_subvars_endsubv_()
#endif
 {
  gmvwrite_subvars_endsubv();
 }

/* ---------------------------------------------------------------- */

void
#ifdef hp
fgmvwrite_ghosts(int data_type, int numghst, char *ghost_data)
#elif unicos
FGMVWRITE_GHOSTS(int data_type, int numghst, char *ghost_data)
#else
fgmvwrite_ghosts_(int data_type, int numghst, char *ghost_data)
#endif
 {
  gmvwrite_ghosts(data_type, numghst, ghost_data);
 }

/* ---------------------------------------------------------------- */
