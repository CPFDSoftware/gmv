#ifndef _GMVWRITEH_
#define _GMVWRITEH_
#endif


/*  C, C++ prototypes.  */

extern "C"
{

void gmvwrite_openfile(const char filenam[]);

void gmvwrite_openfile_ascii(const char filenam[]);

void gmvwrite_openfile_ir(const char filenam[], int isize, int rsize);

void gmvwrite_openfile_ir_ascii(const char filenam[], int isize, int rsize);

void gmvwrite_closefile();

void gmvwrite_nodes_fromfile(const char *filename, long nndes);

void gmvwrite_node_data(void *nndes, void *x, void *y, void *z);

void gmvwrite_node_data_struct(void *nxv, void *nyv, void *nzv,
			       void *x,  void *y, void *z);

void gmvwrite_node_data_lstruct(void *nxv, void *nyv, void *nzv,
			        void *x, void *y, void *z);

void gmvwrite_node_data_amr(int nxc, int nyc, int nzc, void *x0, void *y0,
		            void *z0, void *dx, void *dy, void *dz);

void gmvwrite_nodev_fromfile(const char *filename, long nndes);

void gmvwrite_nodev_data(void *nndes, void *x, void *y, void *z);

void gmvwrite_nodev_data_lstruct(void *nxv, void *nyv, void *nzv,
			        void *x, void *y, void *z);

void gmvwrite_cells_amr(void *numcells, void *numtop, void *daughters);

void gmvwrite_cells_fromfile(const char *filename, long nclls);

void gmvwrite_cell_header(void *ncells);

void gmvwrite_cell_type(const char cell_type[], int nverts, void *nodes);

void gmvwrite_general_cell_type(const char cell_type[], int nverts[], int nfaces,
			        void *nodeids);

void gmvwrite_faces_fromfile(const char *filename, long nclls);

void gmvwrite_face_header(void *nfaces, void *ncells);

void gmvwrite_face_data(int nverts, void *nodeids, void *cellid1, 
                   void *cellid2);

void gmvwrite_vfaces_fromfile(const char *filename, long nclls);

void gmvwrite_vface_header(void *nfaces);

void gmvwrite_vface_data(int nverts, int facepe, void *oppface, 
                         int oppfacepe, void *cellid, void *nodeids);

void gmvwrite_xfaces_fromfile(const char *filename, long nfces, long nclls);

void gmvwrite_xface_header(void *nfaces);

void gmvwrite_xface_data(long totverts, void *nverts, void *nodeids, 
                         void *cellid, void *oppface, void *facepe,
                         void *oppfacepe);

void gmvwrite_material_fromfile(const char *filename);

void gmvwrite_material_header(int nmats, int data_type);

void gmvwrite_material_name(const char matname[]);

void gmvwrite_material_ids(int matids[], int data_type);

void gmvwrite_velocity_data(int data_type, void *u, void *v, void *w);

void gmvwrite_variable_header();

void gmvwrite_variable_name_data(int data_type, const char varname[], void *vids);

void gmvwrite_variable_endvars();

void gmvwrite_flag_fromfile(const char *filename);

void gmvwrite_flag_header();

void gmvwrite_flag_name(const char flagname[], int numtypes, int data_type);

void gmvwrite_flag_subname(const char subname[]);

void gmvwrite_flag_data(int data_type, int flag_data[]);

void gmvwrite_flag_endflag();

void gmvwrite_polygons_fromfile(const char *filename);

void gmvwrite_polygons_header();

void gmvwrite_polygons_data(int nverts, int matnum, void *x, void *y, void *z);

void gmvwrite_polygons_endpoly();

void gmvwrite_tracers_header(int ntracers, void *x, void *y, void *z);

void gmvwrite_tracers_name_data(int ntracers, const char tracername[], void *data);

void gmvwrite_tracers_endtrace();

void gmvwrite_probtime(double ptime);

void gmvwrite_cycleno(int cyclenum);

void gmvwrite_nodeids(void *nodeids);

void gmvwrite_nodeids_fromfile(const char *filename);

void gmvwrite_cellids(void *cellids);

void gmvwrite_cellids_fromfile(const char *filename);

void gmvwrite_surface_header(void *nsurf);

void gmvwrite_surface_data(int nverts, void *nodeids);

void gmvwrite_surface_fromfile(const char *filename, long nsrf);

void gmvwrite_surfmats(int matids[]);

void gmvwrite_surfvel( void *u, void *v, void *w);

void gmvwrite_surfvars_header();

void gmvwrite_surfvars_name_data(const char varname[], void *vids);

void gmvwrite_surfvars_endsvar();

void gmvwrite_surfflag_header();

void gmvwrite_surfflag_name(const char flagname[], int numtypes);

void gmvwrite_surfflag_subname(const char subname[]);

void gmvwrite_surfflag_data(int flag_data[]);

void gmvwrite_units_fromfile(const char *filename);

void gmvwrite_units_header();

void gmvwrite_units_typehdr(int data_type, int numtypes);

void gmvwrite_units_name(const char fldname[], const char unitname[]);

void gmvwrite_units_endunit();

void gmvwrite_vinfo_header();

void gmvwrite_vinfo_name_data(int nelem, int nlines, const char varname[], void *vids);

void gmvwrite_vinfo_endvinfo();

void gmvwrite_traceids(int ntracers, void *traceids);

void gmvwrite_traceids_fromfile(const char *filename);

void gmvwrite_faceids(void *faceids);

void gmvwrite_faceids_fromfile(const char *filename);

void gmvwrite_group_fromfile(const char *filename);

void gmvwrite_group_header();

void gmvwrite_group_data(const char groupname[], int data_type, int numgrp, 
     void *group_data);

void gmvwrite_group_endgroup();

void gmvwrite_surfids(void *surfids);

void gmvwrite_codename(const char codename[]);

void gmvwrite_codever(const char codever[]);

void gmvwrite_simdate(const char simdate[]);

void gmvwrite_subvars_header();

void gmvwrite_subvars_name_data(int data_type, int numelem, const char varname[], 
                              void *vids, void *vdata);

void gmvwrite_subvars_endsubv();

void gmvwrite_ghosts(int data_type, int numghst, void *ghost_data);

}
