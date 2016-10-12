/*
 * Copyright (c) 2005 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Governement
 * retains certain rights in this software.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.  
 * 
 *     * Neither the name of Sandia Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/* $Id: create_mesh.c,v 1.3 2006/11/28 14:02:07 gdsjaar Exp $ */

#define HAVE_EXODUSII

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exodusII.h"

#define DEFAULT_FILE_NAME       "mesh"
#define DEFAULT_MAP_ORIGIN      1
#define DEFAULT_NUM_DOMAINS     1
#define DEFAULT_NUM_ELEMENTS    10000
#define DEFAULT_NUM_FIELDS      0
#define DEFAULT_NUM_TIMESTEPS   1

#define MAX_STRING_LEN          128
#define NUM_BYTES_PER_INT       4
#define NUM_NODES_PER_ELEM      4

#define EXODUSII_FILE_TYPE      ".e"

void get_file_name(const char* base, const char* ext, int rank, int nprocs,
                   const char* other,char *output);

/**
 * Mark Borgerding's algorithm...
 */

/* We need to do a sqrt to find the number of elements on each
 * side of the square, but don't want to link in -lm just for
 * a single square root.  Use this routine which is better than
 * a brute force approach, but not as good as some much larger
 * optimized versions.
 */
static int isqrt(int val) {
  int guess=0;
  int bit = 1 << 15;
  do {
    guess ^= bit;  
    /* check to see if we can set this bit without going over sqrt(val)... */
    if (guess * guess > val )
      guess ^= bit;  /* it was too much, unset the bit... */
  } while ((bit >>= 1) != 0);
  
  return guess;
}

/*
 *      Prototypes
 */

void create_rr_elem_map(
        int loc_num_elements,
        int *elem_map,
        int map_origin,
        int num_domains,
        int current_domain
);

void create_elem_map (
        int loc_num_elems, 
        int start, 
        int *elem_map,
        int map_origin
);

void create_local_connect(
        int *node_map,
        int len_node_map, 
        int len_connect,
        int *domain_connect, 
        int *loc_connect,
        int map_origin
);

void extract_connect(
                     int element_offset,
        int num_elem,
        int *elem_map,
        int *connect, 
        int *domain_connect,
        int map_origin
);

void make_mesh(
        float   *x,
        float   *y,
        int     *connect,
        int     map_origin,
        int     num_elements
);

void parse_input(
        int     argc,
        char    *argv[],
        int     *debug,
        int     *map_origin,
        int     *num_elements, 
        int     *num_domains,
        int     *num_nodal_fields, 
        int     *num_global_fields, 
        int     *num_element_fields, 
        int     *num_timesteps, 
        char    *device_name,
        char    *file_name,
        int     *exodus
);

void write_exo_mesh(
        int     debug,
        char    *file_name,
        int     map_origin,
        int     num_nodes,
        int     num_elements,
        int     num_domains,
        int     num_nodal_fields,
        int     num_global_fields,
        int     num_element_fields,
        int     num_timesteps,
        float   *x,
        float   *y, 
        int     *connect
);

void create_node_map (int len_map, int len_connect, int *domain_connect, int *node_map,
                      int *loc_num_nodes, int map_origin);

void sort_int(int n, int ra[]);

int bin_search2 (int value, int num, int List[]);

/***********************************************************************
 *
 *  Main function
 * 
 ***********************************************************************/

int
main( int argc, char *argv[] ) {
  int   *connect;
  int   debug =         FALSE;                  /* TRUE, display debug information; FALSE       */
  /* otherwise.                                 */
  static char device_name[MAX_STRING_LEN];
  static char file_name[MAX_STRING_LEN] =       DEFAULT_FILE_NAME;
  int   exodus =                TRUE;
  int   map_origin =            DEFAULT_MAP_ORIGIN;     
  int   num_domains =           DEFAULT_NUM_DOMAINS;
  int   num_elements_1d;
  int   num_elements =          DEFAULT_NUM_ELEMENTS;
  int   num_nodal_fields =      DEFAULT_NUM_FIELDS;
  int   num_global_fields =     DEFAULT_NUM_FIELDS;
  int   num_element_fields =    DEFAULT_NUM_FIELDS;
  int   num_timesteps =         DEFAULT_NUM_TIMESTEPS;
  int   num_nodes;

  float *x;
  float *y;     

   ex_opts (EX_VERBOSE | EX_ABORT);

  /* Parse Input */
  parse_input(argc, argv, &debug, &map_origin,
              &num_elements, &num_domains, &num_nodal_fields,
              &num_global_fields, &num_element_fields, &num_timesteps, 
              device_name,file_name, &exodus);
        
  /* Create Coordinates and Connectivity Array */
  num_elements_1d = isqrt(num_elements);
  num_nodes =   (num_elements_1d + 1) * (num_elements_1d + 1);
  x =           malloc( num_nodes * sizeof( float ) );
  y =           malloc( num_nodes * sizeof( float ) );

  num_elements = num_elements_1d * num_elements_1d;
  connect =     malloc( NUM_NODES_PER_ELEM * num_elements * sizeof( int ) );

  make_mesh(x, y, connect, map_origin, num_elements_1d);

  /*
   *    Write Out Mesh
   */

  if (exodus) {
    write_exo_mesh(
                   debug,
                   file_name,
                   map_origin,
                   num_nodes,
                   num_elements,
                   num_domains,
                   num_nodal_fields,
                   num_global_fields,
                   num_element_fields,
                   num_timesteps,
                   x,
                   y,
                   connect
                   );
  }

  free(x);
  free(y);
  free(connect);
  return 0;
} /* end of main() */


/***********************************************************************
 ***********************************************************************/
void parse_input(
                 int  argc,
                 char *argv[],
                 int  *debug,
                 int  *map_origin,
                 int  *num_elements_1d, 
                 int  *num_domains,
                 int  *num_nodal_fields, 
                 int  *num_global_fields, 
                 int  *num_element_fields, 
                 int  *num_timesteps, 
                 char *device_name,
                 char *file_name,
                 int  *exodus
                 ) {
  int arg = 0;  /* Argument index.      */

  while ( ++arg < argc ) {
    if ( strcmp( "-c", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_nodal_fields = atoi( argv[arg] );
    }
    else if ( strcmp( "-nv", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_nodal_fields = atoi( argv[arg] );
    }
    else if ( strcmp( "-gv", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_global_fields = atoi( argv[arg] );
    }
    else if ( strcmp( "-ev", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_element_fields = atoi( argv[arg] );
    }
    else if ( strcmp( "-t", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_timesteps = atoi( argv[arg] );
    }
    else if ( strcmp( "-d", argv[arg] ) == 0 ) {
      *debug = TRUE;
    }
    else if ( strcmp( "-f", argv[arg] ) == 0 ) { 
      if ( ++arg < argc ) strcpy( file_name, argv[arg] );
    }
    else if ( strcmp( "-m", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *map_origin = atoi( argv[arg] );
    }
    else if ( strcmp( "-n", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_elements_1d = atoi( argv[arg] );
    }
    else if ( strcmp( "-p", argv[arg] ) == 0 ) {
      if ( ++arg < argc ) *num_domains = atoi( argv[arg] );
    }
    else if ( strcmp( "-x", argv[arg] ) == 0 ) {
      *exodus = TRUE;
    }
    else if ( (strcmp( "-h", argv[arg] ) == 0) || (strcmp( "-u", argv[arg] ) == 0) ) {
      printf( "                                                                \n" );
      printf( "NAME                                                            \n" );
      printf( "                                                                \n" );
      printf( "create_mesh - creates a mesh file for performance benchmarking. \n" );
      printf( "                                                                \n" );
      printf( "SYNOPSIS                                                        \n" );
      printf( "                                                                \n" );
      printf( "create_mesh [-c fields] [-t timesteps] [-d] [-f file_name] \n" );
      printf( "            [-m map_origin] [-n elements] [-p domains]          \n" );
      printf( "            [-nv number] [-ev number] [-gv number] ");
      printf( "            [-r] [-u] [-h] ");
      printf( "[-x]" );
      printf( "                 \n" );
      printf( "                                                                \n" );
      printf( "DESCRIPTION                                                     \n" );
      printf( "                                                                \n" );
      printf( "This program creates a 2-D mesh for performance benchmarking.   \n" );
      printf( "The EXODUSII II database file(s) created by this       \n" );
      printf( "prrogram is/are read by the rd_wt_mesh program to perform the   \n" );
      printf( "actual benchmark.                                               \n" );
      printf( "                                                                \n" );
      printf( "OPTIONS                                                         \n" );
      printf( "                                                                \n" );
      printf( "-c  fields     number of nodal   fields. Default: %d            \n", DEFAULT_NUM_FIELDS   );
      printf( "-nv fields     number of nodal   fields. Default: %d            \n", DEFAULT_NUM_FIELDS   );
      printf( "-ev fields     number of element fields. Default: %d           \n", DEFAULT_NUM_FIELDS   );
      printf( "-gv fields     number of global  fields. Default: %d            \n", DEFAULT_NUM_FIELDS   );
      printf( "-t timesteps   number of timesteps. Default: %d                 \n", DEFAULT_NUM_TIMESTEPS);
      printf( "-d             display debug information.                         \n" );
      printf( "-f file_name   file name prefix for all created files:          \n" );
      printf( "                                                                \n" );
      printf( "                  'file_name'_n%s [EXODUSII II file]              \n", EXODUSII_FILE_TYPE );
      printf( "                                                                \n" );
      printf( "               where n varies from 0 to number of domains-1.  \n" );
      printf( "               Default: %s                                      \n", DEFAULT_FILE_NAME       );
      printf( "-h             display help/usage information.                  \n" );
      printf( "-m map_origin  element map origin. Default: %d                  \n", DEFAULT_MAP_ORIGIN    );
      printf( "-n elements    number of elements in mesh   \n" );
      printf( "               elements/file = elements/number_of_domains.      \n" );
      printf( "               Default: %d                                      \n", DEFAULT_NUM_ELEMENTS);
      printf( "-p domains     number of domains. Default: %d                   \n", DEFAULT_NUM_DOMAINS   );
      printf( "-u             display help/usage information.                  \n" );

      exit( 0 );
    }   
    else {
      fprintf(stderr,  "Unknown option: %s\n", argv[arg]                                    );
      fprintf(stderr,  "Enter create_mesh -h for description of valid options.\n"           );
                        
      exit( 0 );
    }
  }

  return;
}

/***********************************************************************
 *
 *  Create the coordinates and connectivity array for the mesh
 * 
 ***********************************************************************/

void make_mesh(
        float   *x,
        float   *y,
        int     *connect,
        int     map_origin,
        int     num_elements_1d
) {
  int i, j, k, base, cnt;

  /* create global coordinates */

  for (i=0, k=0; i < (num_elements_1d+1); i++)
  {
    for (j=0; j < (num_elements_1d+1); j++, k++) 
    {
      x[k] = (float)j;
      y[k] = (float)i;
    }
  }

  /* build connectivity array (node list) for mesh */

  for (i=0, k=0, cnt=0; i < num_elements_1d; i++)
  {
    for (j=0; j < num_elements_1d; j++, k++) 
    {
      base = k+i+map_origin;
      connect[cnt++] = base;
      connect[cnt++] = base+1;
      connect[cnt++] = base+num_elements_1d+2;
      connect[cnt++] = base+num_elements_1d+1;
    }
  }
} /* end of make_mesh() */

/***********************************************************************
 ***********************************************************************/
void write_exo_mesh(
                    int debug,
                    char        *file_name,
                    int map_origin,
                    int num_nodes,
                    int num_elements,
                    int num_domains,
                    int num_nodal_fields,
                    int num_global_fields,
                    int num_element_fields,
                    int num_timesteps,
                    float       *x,
                    float       *y, 
                    int *connect
                    ) {
  int CPU_word_size=0;
  int IO_word_size=4;
  int exoid, err, num_dim, num_elem_blk, num_node_sets, num_side_sets;
  int i, j, t, index, loc_num_elements, loc_num_nodes, len_connect;
  int *elem_map, *node_map, *domain_connect, *loc_connect;
  int *elem_var_tab;
  int accum_num_elements = 0;
  
  float *z, *loc_xcoords, *loc_ycoords, *globals = NULL;

  char temporary_name[MAX_STRING_LEN];
  char **var_name;

  accum_num_elements = 0;
  for (i=0; i<num_domains; i++) {

    /* create the EXODUSII file */
    get_file_name(file_name, "e", i, num_domains, NULL, temporary_name);
    exoid = ex_create (temporary_name, EX_CLOBBER, &CPU_word_size, &IO_word_size);

    if (exoid < 0) {
      fprintf(stderr, "after ex_create, error = %d\n", err);
      exit(-1);
    }

    /* Determine local number of elements */
    if (num_elements < num_domains) 
      {
        fprintf(stderr, "number of elements is less than number of domains.\n");
        if (i < num_elements) loc_num_elements = 1;
        else loc_num_elements = 0;
      } else {
      loc_num_elements = num_elements / num_domains;
      if (i < (num_elements % num_domains)) loc_num_elements++;
    }

    len_connect = NUM_NODES_PER_ELEM * loc_num_elements;

    /* malloc things we need */

    if (i == 0) {  /* first time through; max size arrays occur on
                      first iteration */
      elem_map =       malloc(loc_num_elements * sizeof(int));
      domain_connect = malloc(len_connect * sizeof(int));
      loc_connect =    malloc(len_connect * sizeof(int));
      node_map =       malloc(num_nodes * sizeof(int));
    }

    /* Create element local/global map */
    create_elem_map(
                    loc_num_elements,
                    accum_num_elements,
                    elem_map,
                    map_origin
                    );
    
    /* Extract current domain's connectivity, referencing global node ids */
    extract_connect(accum_num_elements,
                    loc_num_elements,
                    elem_map,
                    connect,
                    domain_connect,
                    map_origin);

    accum_num_elements += loc_num_elements;

    /* The local/global node map is just the current domain's connectivity,
       sorted with duplicate entries removed */
    create_node_map (num_nodes, len_connect, domain_connect, node_map, &loc_num_nodes, map_origin);

    /* Using local/global node map, convert the domain connectivity 
       (referencing global node ids) to local connectivity (referencing 
       local node ids) */

    create_local_connect(
                         node_map,
                         loc_num_nodes, 
                         len_connect,
                         domain_connect,
                         loc_connect,
                         map_origin
                         );

    if ( debug ) {
      fprintf(stderr, "\n\n\n");

      fprintf(stderr, "\n domain: %d\n", i);
      fprintf(stderr, "\n loc_num_elements: %d\n", loc_num_elements);
      fprintf(stderr, "\n loc_num_nodes: %d\n", loc_num_nodes);

      fprintf(stderr, "\n element map:\n");
      for (j=0; j<loc_num_elements; j++) fprintf(stderr, " %d,", elem_map[j]);

      fprintf(stderr, "\n domain connectivity:\n");
      for (j=0; j<len_connect; j++) fprintf(stderr, " %d,", domain_connect[j]);

      fprintf(stderr, "\n node map:\n");
      for (j=0; j<loc_num_nodes; j++) fprintf(stderr, " %d,", node_map[j]);

      fprintf(stderr, "\n local connectivity:\n");
      for (j=0; j<len_connect; j++) fprintf(stderr, " %d,", loc_connect[j]);
    }

    num_dim = 2;
    num_elem_blk = 1;
    num_node_sets = 0;
    num_side_sets = 0;

    err = ex_put_init (exoid, "This is an EXODUSII performance test.", num_dim, 
                       loc_num_nodes, loc_num_elements, num_elem_blk, 
                       num_node_sets, num_side_sets);

    if (err) {
      fprintf(stderr, "after ex_put_init, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    /* Extract the local x and y coordinates */

    if (i == 0) {  /* first time through; max size occurs on
                      first iteration */
      loc_xcoords = malloc(loc_num_nodes * sizeof(float));
      loc_ycoords = malloc(loc_num_nodes * sizeof(float));
    }

    for (j=0; j<loc_num_nodes; j++) {
      index = node_map[j] - map_origin;
      loc_xcoords[j] = x[index];
      loc_ycoords[j] = y[index];
    }

    err = ex_put_coord (exoid, loc_xcoords, loc_ycoords, z);

    if (err) {
      fprintf(stderr, "after ex_put_coord, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    err = ex_put_elem_block 
      (exoid, 10, "quad", loc_num_elements, NUM_NODES_PER_ELEM, 0);

    if (err) {
      fprintf(stderr, "after ex_put_elem_block, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    err = ex_put_elem_conn (exoid, 10, loc_connect);

    if (err) {
      fprintf(stderr, "after ex_put_elem_conn, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    /* write out element and node maps */

    err = ex_put_node_num_map (exoid, node_map);

    if (err) {
      fprintf(stderr, "after ex_put_node_num_map, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    err = ex_put_elem_num_map (exoid, elem_map);

    if (err) {
      fprintf(stderr, "after ex_put_elem_num_map, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    /* write out simulated results fields;
       we'll just write out the x coordinate field 'num_nodal_fields' times */
    if (loc_num_nodes < loc_num_elements) {
      fprintf(stderr, "INTERNAL ERROR: Programmer assumed number of nodes > number of elements, but that is not true.\n");
      ex_close (exoid);
      exit(-1);
    }

    if (num_element_fields > 0) {
      elem_var_tab = malloc(num_element_fields * sizeof(int));
      for (j = 0; j < num_element_fields; j++)
        elem_var_tab[j] = 1;
    } else {
      elem_var_tab = 0;
    }
    err = ex_put_all_var_param(exoid,
                               num_global_fields,
                               num_nodal_fields,
                               num_element_fields,
                               elem_var_tab,
                               0, 0, 0, 0);
    if (err) {
      fprintf(stderr, "after ex_put_all_var_param, error = %d\n", err);
      ex_close (exoid);
      exit(-1);
    }

    if (num_nodal_fields > 0) {

      var_name = malloc (num_nodal_fields * sizeof(char *));
      for (j=0; j<num_nodal_fields; j++) {
        var_name[j] = malloc ((MAX_STRING_LEN+1) * sizeof (char));
        sprintf (var_name[j], "node_field_%d", j+1);
      }
      err = ex_put_var_names (exoid, "n", num_nodal_fields, var_name);
      for (j=0; j<num_nodal_fields; j++) {
        free(var_name[j]);
      }
      free(var_name);
    }

    if (num_global_fields > 0) {
      globals = malloc(num_global_fields * sizeof(float));
      var_name = malloc (num_global_fields * sizeof(char *));
      for (j=0; j<num_global_fields; j++) {
        var_name[j] = malloc ((MAX_STRING_LEN+1) * sizeof (char));
        sprintf (var_name[j], "global_field_%d", j+1);
        globals[j] = j;
      }
      err = ex_put_var_names (exoid, "g", num_global_fields, var_name);
      for (j=0; j<num_global_fields; j++) {
        free(var_name[j]);
      }
      free(var_name);
    }

    if (num_element_fields > 0) {
      free(elem_var_tab);
      var_name = malloc (num_element_fields * sizeof(char *));
      for (j=0; j<num_element_fields; j++) {
        var_name[j] = malloc ((MAX_STRING_LEN+1) * sizeof (char));
        sprintf (var_name[j], "element_field_%d", j+1);
      }
      err = ex_put_var_names (exoid, "e", num_element_fields, var_name);
      for (j=0; j<num_element_fields; j++) {
        free(var_name[j]);
      }
      free(var_name);
    }

    if (num_nodal_fields+num_global_fields+num_element_fields > 0) {
      fprintf(stderr, "Domain %d/%d, Writing Timestep: ", i+1, num_domains);
      for (t=0; t<num_timesteps; t++) {
        float time = t;
        ex_put_time(exoid, t+1, &time);
        fprintf(stderr, "%d, ", t+1);
        if (num_global_fields > 0) {
          err = ex_put_glob_vars (exoid, t+1, num_global_fields, globals);
          if (err) {
            fprintf(stderr, "after ex_put_global_var, error = %d\n", err);
            ex_close (exoid);
            exit(-1);
          }
        }
        for (j=0; j<num_nodal_fields; j++) {
          err = ex_put_nodal_var (exoid, t+1, j+1, loc_num_nodes, loc_xcoords);
          if (err) {
            fprintf(stderr, "after ex_put_nodal_var, error = %d\n", err);
            ex_close (exoid);
            exit(-1);
          }
        }
        for (j=0; j<num_element_fields; j++) {
          err = ex_put_elem_var (exoid, t+1, j+1, 10, loc_num_elements, loc_xcoords);
          if (err) {
            fprintf(stderr, "after ex_put_element_var, error = %d\n", err);
            ex_close (exoid);
            exit(-1);
          }
        }
      }
      fprintf(stderr, "\n");
    }

    err = ex_close (exoid);

    if (err) {
      fprintf(stderr, "after ex_close, error = %d\n", err);
      exit(-1);
    }
  }

  /*
   * Free Memory
   */

  free( domain_connect );
  free( elem_map       );
  free( loc_connect    );
  free( loc_xcoords    );
  free( loc_ycoords    );
  free( node_map       );
  if (num_global_fields > 0)
    free(globals);
}

/***********************************************************************
 *
 * Create element local/global map
 *
 * This puts contiguous groups of elements in each domain.  This is
 * a somewhat reasonable map for a realistic application.
 *
 ***********************************************************************/
void create_elem_map (
        int loc_num_elems, 
        int elem_num, 
        int *elem_map,
        int map_origin)
{
  int i;

  for (i=0; i<loc_num_elems; i++) {
    elem_map[i] = map_origin + elem_num++;
  }

}

/***********************************************************************
 *
 * Extract current domain's connectivity, referencing global node ids
 *
 * This extracts the "domain connectivity," that is, the connectivity 
 * of the elements in the current domain.  The node ids in the domain 
 * connectivity reference global node ids.
 *
 ***********************************************************************/

void extract_connect(
                     int element_offset,
                     int num_elem,
                     int *elem_map,
                     int *connect, 
                     int *domain_connect,
                     int map_origin
                     ) {
  int i, j, k, m, offset;

  for (i=element_offset, j=0, m=0; j<num_elem; j++) {
    if (elem_map[j] == i+map_origin) {  /* extract this element */
      offset = (i * NUM_NODES_PER_ELEM);
      for (k=offset; k < offset+NUM_NODES_PER_ELEM; k++) {
        domain_connect[m++] = connect[k];
      }
      i++;
    }
  }
}

/***********************************************************************
 *
 * The local/global node map is just the current domain's connectivity,
 * sorted, with duplicate entries removed.  This isn't obvious, but
 * trust me.
 *
 ***********************************************************************/
void create_node_map (int len_map, int len_connect, int *domain_connect, int *node_map,
                      int *loc_num_nodes, int map_origin)
{
  int cnt, i;

  *loc_num_nodes = len_connect;

  for (i = 0; i < len_map; i++) {
    node_map[i] = 0;
  }

  for (i = 0; i < len_connect; i++) {
    node_map[domain_connect[i]-map_origin] = 1;
  }

  cnt = 0;
  for (i = 0; i < len_map; i++) {
    if (node_map[i] > 0) {
      node_map[cnt++] = i+map_origin;
    }
  }
  *loc_num_nodes = cnt;
}

/***********************************************************************
 *
 * Using local/global node map, convert the domain connectivity 
 * (referencing global node ids) to local connectivity (referencing 
 * local node ids).
 *
 * This requires inverting the local/global map, a relatively expensive
 * operation.  The procedure is:
 *
 *   for every entry in the domain connectivity
 *     search the node map until found
 *     set the value of the entry in the local connectivity to 
 *       the index of the located value in the node map
 *
 ***********************************************************************/
void create_local_connect(int *node_map,
                          int len_node_map, 
                          int len_connect,
                          int *domain_connect, 
                          int *loc_connect,
                          int map_origin)
{
  int i, index;

  for (i=0; i<len_connect; i++) {
    index = bin_search2 (domain_connect[i], len_node_map, node_map);
    if (index != -1) {  /* found */
      loc_connect[i] = index+map_origin;
    } else {
      fprintf (stderr, "error creating local connectivity; i = %d\n", i);
      exit (-1);
    }
  }
}



/*****************************************************************************
 *
 *       Numerical Recipies in C source code
 *       modified to have first argument an integer array
 *
 *       Sorts the array ra[0,..,(n-1)] in ascending numerical order using
 *       heapsort algorithm.
 *
 *****************************************************************************/

void sort_int(int n, int ra[])

{
  int   l, j, ir, i;
  int   rra;

  /*
   *  No need to sort if one or fewer items.
   */
  if (n <= 1) return;

  l=n >> 1;
  ir=n-1;
  for (;;) {
    if (l > 0)
      rra=ra[--l];
    else {
      rra=ra[ir];
      ra[ir]=ra[0];
      if (--ir == 0) {
        ra[0]=rra;
        return;
      }
    }
    i=l;
    j=(l << 1)+1;
    while (j <= ir) {
      if (j < ir && ra[j] < ra[j+1]) ++j;
      if (rra < ra[j]) {
        ra[i]=ra[j];
        j += (i=j)+1;
      }
      else j=ir+1;
    }
    ra[i]=rra;
  }
}


/*****************************************************************************
 *
 * Searches a monotonic list of values for the value, 'value'.
 * It returns the index (0-based) of the first position found, which 
 *   matches 'value'.
 * The list is assumed to be monotonic, and consist of elements 
 *   list[0], ..., list[n-1].
 * If no position in list matches value, it returns the value -1.
 *
 *****************************************************************************/

int bin_search2 (int value, int num, int List[])
{
  int top, bottom = 0, middle, g_mid;

  top = num - 1;
  while (bottom <= top) {
    middle = (bottom + top) >> 1;
    g_mid = List[middle];
    if (value < g_mid)
      top = middle - 1;
    else if (value > g_mid)
      bottom = middle + 1;
    else
      return middle;     /* found */
  }
  return -1;
} /* bin_search2 */

/*****************************************************************************/
void get_file_name(const char* base, const char* ext, int rank, int nprocs,
                   const char* other,char *output)
{
  int i1, iTemp1;
  int iMaxDigit=0, iMyDigit=0;
  char cTemp[128];

  output[0] = '\0';
  strcpy(output, base);
  strcat(output, ".");
  strcat(output, ext);
  if (other != NULL) {
    strcat(output, ".");
    strcat(output, other);
  }
  if (nprocs > 1) {
    /*
     * Find out the number of digits needed to specify the processor ID.
     * This allows numbers like 01-99, i.e., prepending zeros to the
     * name to preserve proper alphabetic sorting of the files.
     */

    iTemp1 = nprocs;
    do {
      iTemp1 /= 10;
      iMaxDigit++;
    }
    while(iTemp1 >= 1);
  
    iTemp1 = rank;
    do {
      iTemp1 /= 10;
      iMyDigit++;
    }
    while(iTemp1 >= 1);

    strcat(output, ".");
    sprintf(cTemp, "%d", nprocs);
    strcat(output, cTemp);
    strcat(output, ".");

    /*
     * Append the proper number of zeros to the filename.
     */
    for(i1=0; i1 < iMaxDigit-iMyDigit; i1++)
      strcat(output, "0");

    sprintf(cTemp, "%d", rank);
    strcat(output, cTemp);
  }
}
