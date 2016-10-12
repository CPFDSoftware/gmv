#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gmvdata.h"
#include "gmvread.h"

#include <sys/types.h>
//#include <malloc.h>
#include <math.h>

#define MSIZE 1000
#define MAXVERTS 10000
#define MAXFACES 10000
#define MAXVAL 9.99999e32

extern char *fldfilenam, *cfldfilenam, *polyfilenam, *sfldfilenam, *ffldfilenam;
extern FILE *fieldfile, *cfieldfile, *polyfile, *sfieldfile, *ffieldfile;

extern void gmvexit();

static int nnodemats = 0, ncellmats = 0;
static char *type_str[4] = {"cells","nodes","faces","surfaces"};
static short hasphex27 = 0;
static char *isphex27 = NULL;

void makemesh(),
     savemats(),
     savevels(),
     savevars(),
     saveflags(),
     savepolys(),
     savetracers(),
     savenodeids(),
     savecellids(),
     savefaceids(),
     savetracerids(),
     saveunits(),
     savesurface(),
     savesurfvel(),
     savesurfmats(),
     savesurfvars(),
     savesurfflag(),
     savesurfids(),
     savegroups(),
     savesubvars(),
     saveghosts(),
     savevects(),
     memerr();


void readgmv_init()
{
  int i;

   /*  Initialize data.  */
   numvars = 0;  numflags_node = 0;  mmats = 0;  numvartr = 0;
   cycleno = 0;  cycleflag = 0;  ptime = 0;  timeflag = 0;
   numflags_cell = 0; polysin = 0;  ntracers = 0;  cnumvars = 0;
   maxmatno = 0;  ntracehist = 0;  nnodemats = 0;  ncellmats = 0;
   nodeidflag = 0;  cellidflag = 0;  faceidflag = 0;
   faces_in = 0;  mats_on_cells = 0;  nsurf = 0;  fnumvars = 0;
   mats_on_nodes = 0;  nvectin = -1;  cvectin = -1;  svectin = -1;
   fvectin = -1;  snumvars = 0;  traceridflag = 0;
   ncellgrps = 0;  nnodegrps = 0;  nfacegrps = 0;  nsurfacegrps = 0;
   for (i = 0; i < MAXMATS; i++)
     {
      mmatnames[i]=(char *)malloc(33*sizeof(char));
      if (mmatnames[i] == NULL) memerr();
      strcpy(mmatnames[i],"                                ");
     }
   for (i = 0; i < MAXFIELDS; i++)
     {
      nvars_to_write[i] = 0;
      cvars_to_write[i] = 0;
      svars_to_write[i] = 0;
      nfld_nice[i] = 0;  nfld_log[i] = 0;
      cfld_nice[i] = 0;  cfld_log[i] = 0;
      sfld_nice[i] = 0;  sfld_log[i] = 0;
      nfld_nice[i] = 0;  nfld_log[i] = 0;
      nsubvarsin[i] = 0;
      csubvarsin[i] = 0;
      fsubvarsin[i] = 0;
     }
   for (i = 0; i < MAXTRFIELDS; i++)
     {
      trfld_nice[i] = 0;  trfld_log[i] = 0;
     }
   strcpy(units.xyz,"        ");
   strcpy(units.uvw,"        ");

   /*  Reopen the field database files.  */
   fieldfile = freopen(fldfilenam,"w+",fieldfile);
   cfieldfile = freopen(cfldfilenam,"w+",cfieldfile);
   polyfile = freopen(polyfilenam,"w+",polyfile);
   sfieldfile = freopen(sfldfilenam,"w+",sfieldfile);
   ffieldfile = freopen(ffldfilenam,"w+",ffieldfile);
   unlink(fldfilenam); 
   unlink(cfldfilenam); 
   unlink(polyfilenam);
   unlink(sfldfilenam);
   unlink(ffldfilenam);
}


void readgmv()
{
  /*                          */
  /*  Read a GMV input file.  */
  /*                          */
  int iend, i, ierr, nodes_read;

   ierr = gmvread_open(filnam);

   if (ierr > 0)
      {
       fprintf(stderr,"Error reading gmv file %s\n",filnam);
       gmvexit();
      }
   printf("GMV reading file %s\n",filnam);

   if (noprintflag) gmvread_printoff();
   else gmvread_printon();

   /*  Initialize data and reopen database files.  */
   readgmv_init();

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
            gmvread_mesh();

            /*  Check for GMVERROR.  */
            if (gmv_meshdata.intype == GMVERROR) gmvexit();

            /*  Fill GMV mesh data structures.  */
            makemesh();
            break;
         case(MATERIAL):
            savemats();
            if (gridonlyflag)
              {
               iend = 1;
               gmvread_close();
              }
            break;
         case(VELOCITY):
            if (!gridonlyflag)
               savevels();
            break;
         case(VARIABLE):
            if (!gridonlyflag)
               savevars();
            break;
         case(FLAGS):
            if (!gridonlyflag)
               saveflags();
            break;
         case(POLYGONS):
            if (!gridonlyflag)
               savepolys();
            break;
         case(TRACERS):
            if (!gridonlyflag)
               savetracers();
            break;
         case(PROBTIME):
            ptime = gmv_data.doubledata1[0];
            timeflag = 1;
            break;
         case(CYCLENO):
            cycleno = gmv_data.num;
            cycleflag = 1;
            break;
         case(NODEIDS):
            if (!gridonlyflag)
               savenodeids();
            break;
         case(CELLIDS):
            if (!gridonlyflag)
               savecellids();
            break;
         case(FACEIDS):
            if (!gridonlyflag)
               savefaceids();
            break;
         case(TRACEIDS):
            if (!gridonlyflag)
               savetracerids();
            break;
         case(UNITS):
            if (!gridonlyflag)
               saveunits();
            break;
         case(SURFACE):
            if (!gridonlyflag)
               savesurface();
            break;
         case(SURFMATS):
            if (!gridonlyflag)
               savesurfmats();
            break;
         case(SURFVEL):
            if (!gridonlyflag)
               savesurfvel();
            break;
         case(SURFVARS):
            if (!gridonlyflag)
               savesurfvars();
            break;
         case(SURFFLAG):
            if (!gridonlyflag)
               savesurfflag();
            break;
         case(SURFIDS):
            if (!gridonlyflag)
               savesurfids();
            break;
         case(GROUPS):
            if (!gridonlyflag)
               savegroups();
            break;
         case(VINFO):
            break;
         case(SUBVARS):
            if (!gridonlyflag)
               savesubvars();
            break;
         case(GHOSTS):
            if (!gridonlyflag)
               saveghosts();
            break;
         case(VECTORS):
            if (!gridonlyflag)
               savevects();
            break;
         case(GMVEND):
            break;
        }
     }
}


void getcellfaces()
{
  int i, ii, j, k, l, m, n, kk, ibin, jface, kface, jverts, kverts;
  int  nverts, jsum, ksum, newno, binavg, *binno, *nfacesperbin;
  unsigned int **bins, nbins;
  long *faceptr, *facenodes, *cellfaceno;
  unsigned int nfacesin;
  short *test, dupface;

   /*                                 */
   /*  Fill cell and face structures  */
   /*  and find duplicate faces.      */
   /*                                 */
   if (!noprintflag) printf("GMV searching for duplicate faces.\n");


   /*  Set face data pointers to gmvread_mesh data.  */
   cellfaceno = gmv_meshdata.celltoface;
   nfacesin = gmv_meshdata.totfaces;
   faceptr = gmv_meshdata.facetoverts;
   facenodes = gmv_meshdata.faceverts;
   FREE(gmv_meshdata.cellfaces);

   /*  Allocate and fill cellfaces structure.  */
   ncellfaces = nfacesin;
   cellfaces = (struct cellface *)
                  malloc(ncellfaces*sizeof(struct cellface));
   if (cellfaces == NULL) memerr();

   for (i = 0; i < ncellfaces; i++)
     {
      jverts = faceptr[i+1] - faceptr[i];
      k = faceptr[i];
      cellfaces[i].nverts = jverts;
      cellfaces[i].fverts = (int *)malloc(jverts*sizeof(int));
      if (cellfaces[i].fverts == NULL) memerr();
      for (j = 0; j < jverts; j++)
         cellfaces[i].fverts[j] = facenodes[k+j];
      cellfaces[i].cell1 = -1;
      cellfaces[i].cell2 = -1;
      cellfaces[i].oppface = -1;
     }
   free(facenodes);  free(faceptr);

   /*  Fill cell structure with face nos. */
   for (i = 0; i < ncells; i++)
     {
      k = cellfaceno[i+1] - cellfaceno[i];
      cells[i].nfaces = k;
      cells[i].facenos = (int *)malloc(k*sizeof(int));
      if (cells[i].facenos == NULL) memerr();
      l = cellfaceno[i];
      for (j = 0; j < k; j++)
        {
         kface = l + j;
         cells[i].facenos[j] = kface;
         cellfaces[kface].cell1 = i;
        }
     }
   free(cellfaceno);

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
   binno = (int *)malloc(nfacesin*sizeof(int));
   if (binno == NULL) memerr();
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
                        cellfaces[jface].oppface = kface;
                        cellfaces[kface].oppface = jface;
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
   if (!noprintflag) printf("GMV found %d unique faces.\n",ncellfaces);

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
      if (cellfaces[i].oppface > -1)
        {
         k = cellfaces[i].oppface;
         cellfaces[i].cell2 = cellfaces[k].cell1;
        }
     }
}


void getcelledges()
{
  int i, j, k, l, m, n, ii, kk, nedge, totverts, jpt1, jpt2, kpt1, kpt2;
  int *faceedges, *nedgesperface, *edgept1, *edgept2, numfaces;
  int  ibin, *oldnos, newno, celledgenos[MAXFACES], binavg, isum, jsum, 
       *binno, *nedgesperbin, ncfaces;
  unsigned int **bins, nbins;
  int nverts, fverti, fvertj, jedge, kedge;
  char *tstedges;
  short *newface;

   /*                                                      */
   /*  Extract a set of edges from face data.              */
   /*  (The first edge point is always the smaller node.)  */
   /*  Then check for duplicates against the edge list.    */
   /*                                                      */
   if (!noprintflag) printf("GMV generating cell edges list.\n");

   /*  Determine max. no. of edges and allocate temporary arrays.  */
   numfaces = ncellfaces;
   if (mesh2dflag > 0) numfaces = ncell2dfaces;
   newface = (short *)malloc(numfaces*sizeof(short));
   if (newface == NULL) memerr();
   for (i = 0; i < numfaces; i++)
      newface[i] = 1;
   for (i = 0; i < numfaces; i++)
     {
      if (newface[i] >= 0)
        {
         j = cellfaces[i].oppface;
         k = -1;
         if (j >= 0)
            k = cellfaces[j].oppface;
         if (j >= 0 && j > i && i == k)
           {
            isum = 0;  jsum = 0;
            if (gmv_meshdata.intype == VFACES3D ||
                gmv_meshdata.intype == VFACES2D)
              {
               for (l = 0; l < cellfaces[i].nverts; l++)
                  isum += cellfaces[i].fverts[l];
               for (l = 0; l < cellfaces[j].nverts; l++)
                  jsum += cellfaces[j].fverts[l];
              }
            if (isum == jsum) newface[j] = -1;
           }
        }
     }
   totverts = 0;
   for (i = 0; i < numfaces; i++)
     {
      if (newface[i] >= 0)
         totverts += cellfaces[i].nverts;
     }
   faceedges = (int *)malloc(numfaces*sizeof(int));
   nedgesperface = (int *)malloc(numfaces*sizeof(int));
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
   for (k = 0; k < numfaces; k++)
     {
      if (newface[k] >= 0)
        {
         nverts = cellfaces[k].nverts;

         /*  If face is part of a phex27, set nverts to 2  */
         /*  to exclude edges to the center of the faces.  */
         if (hasphex27)
           {
            j = cellfaces[k].cell1;
            if ((int)isphex27[j] == 1)
               nverts = 2;
           }
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

   if (!noprintflag) printf("GMV found %d unique edges.\n",ncelledges);

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
      ncfaces = cells[i].nfaces;
      if (mesh2dflag > 0) ncfaces -= 1;
      for (j = 0; j < ncfaces; j++)
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



void makestructmesh(int lstructuredflag)
{
  int i, ncfaces, ncnodes, ncedges;

   nxv = gmv_meshdata.nxv;
   nyv = gmv_meshdata.nyv;
   nzv = gmv_meshdata.nzv;

   /*  Check for 2D structured (nzv = 1). */
   if (nzv == 1)
      struct2dflag = 1;

   /*  Set ncells for structured grids   */
   /*  and allocate one cell structure.  */
   if (rdgmvflag < 2)
     {
      ncells = (nxv-1) * (nyv-1) * (nzv-1);
      ncfaces = 6;
      ncnodes = 8;
      ncedges = 12;
      if (struct2dflag)
        {
         ncells = (nxv-1) * (nyv-1);
         ncfaces = 1;
         ncnodes = 4;
         ncedges = 4;
        }
      cells = (struct cell *)malloc(1*sizeof(struct cell));
      cells[0].ncnodes = ncnodes;
      cells[0].cnodes = (int *)malloc(ncnodes*sizeof(int));
      if (cells[0].cnodes == NULL) memerr();
      cells[0].nfaces = ncfaces;
      cells[0].facenos = (int *)malloc(ncfaces*sizeof(int));
      if (cells[0].facenos == NULL) memerr();
      cells[0].nedges = ncedges;
      cells[0].edgenos = (int *)malloc(ncedges*sizeof(int));
      if (cells[0].edgenos == NULL) memerr();
      if (!noprintflag) printf("GMV generating %d cells.\n",ncells);

      /*  Allocate 6 face structures with 4 verts each.  */
      cellfaces = (struct cellface *)
                  malloc(6*sizeof(struct cellface));
      ncellfaces = (nxv*(nyv-1))*(nzv-1) + (nyv*(nxv-1))*(nzv-1) +
                   (nxv-1)*(nyv-1)*nzv;
      if (!noprintflag) printf("GMV generating %d cell faces.\n",ncellfaces);
      for (i = 0; i < 6; i++)
         cellfaces[i].fverts = (int *)malloc(4*sizeof(int));

      /*  Allocate 12 edge structures.  */
      celledges = (struct celledge *)
                  malloc(12*sizeof(struct celledge));
      ncelledges = (nxv*(nyv-1))*nzv + (nyv*(nxv-1))*nzv +
                   (nxv*nyv)*(nzv-1);
      if (!noprintflag) printf("GMV generating %d cells edges.\n",ncelledges);
     }
}


void makemesh()
{
  int i, j, l, n, k1, lstructuredflag, lncells;
  int cellnodenos[MAXVERTS], ncnodes, nodesfilled;
  long totcellnodes;
  void makegmvamr(), setcellfaces(), setcellvfaces2d(), setcellvfaces3d();
  char *tstnodes;

   /*  Check for structured or logically rectangular mesh.  */
   lstructuredflag = 0;
   if (gmv_meshdata.intype == STRUCT) lstructuredflag = 1;
   if (gmv_meshdata.intype == LOGICALLY_STRUCT) lstructuredflag = 2;

   /*  If amr mesh generate amr grid.  */
   if (gmv_meshdata.intype == AMR)
     {
      makegmvamr();
      return;
     }

   /*  Save node data for structured or general mesh.  */
   nnodes = gmv_meshdata.nnodes;
   if (!noprintflag)
     {
      printf("GMV is generating the mesh.\n");
      printf("GMV read %d nodes.\n",nnodes);
     }
   nodes = (struct node *)malloc((nnodes+1)*sizeof(struct node));
   if (nodes == NULL) memerr();
   nodes[0].x = 0.;  nodes[0].y = 0.;  nodes[0].z = 0.;
   for (i = 1; i < nnodes+1; i++)
     {
      nodes[i].x = gmv_meshdata.x[i-1];
      nodes[i].y = gmv_meshdata.y[i-1];
      nodes[i].z = gmv_meshdata.z[i-1];
     }

   FREE(gmv_meshdata.x);
   FREE(gmv_meshdata.y);
   FREE(gmv_meshdata.z);

   /*  If reading same cells, check that ncells are the same.  */
   lncells = gmv_meshdata.ncells;
   if (rdgmvflag == 2 && (ncells == lncells))
     {

      /*  If reading same cells, free mesh data arrays.  */
      FREE(gmv_meshdata.celltoface);
      FREE(gmv_meshdata.cellfaces);
      FREE(gmv_meshdata.facetoverts);
      FREE(gmv_meshdata.faceverts);
      FREE(gmv_meshdata.facecell1);
      FREE(gmv_meshdata.facecell2);
      FREE(gmv_meshdata.vfacepe);
      FREE(gmv_meshdata.vfaceoppface);
      FREE(gmv_meshdata.vfaceoppfacepe);
      return;
     }
   mesh2dflag = 0;  ncell2dfaces = 0;

   /*  If structured grid generate structured mesh.  */
   if (lstructuredflag > 0)
     {
      makestructmesh(lstructuredflag);
      structuredflag = 1;
      return;
     }

   /*  Allocate cell structure.  */
   ncells = gmv_meshdata.ncells;
   if (ncells == 0) return;
   cells = (struct cell *)malloc(ncells*sizeof(struct cell));
   if (cells == NULL) memerr();

   /*  If cells read, fill structures and find duplicate faces. */
   if (gmv_meshdata.intype == CELLS)
     {
      if (!noprintflag) printf("GMV read %d cells.\n",ncells);
      getcellfaces();
     }

   /*  If faces read, set cell data.  */
   if (gmv_meshdata.intype == FACES) setcellfaces();

   /*  If vfaces read, eliminate duplicate faces and set cell data. */
   if (gmv_meshdata.intype == VFACES3D) setcellvfaces3d();
   if (gmv_meshdata.intype == VFACES2D) setcellvfaces2d();

   /*  Allocate and set cell nodes.  */
   totcellnodes = 0;
   tstnodes = (char *)malloc((nnodes+1)*sizeof(char));
   if (tstnodes == NULL) memerr();
   for (i = 0; i < nnodes+1; i++) tstnodes[i] = 0;
   isphex27 = (char *)malloc(ncells*sizeof(char));
   if (isphex27 == NULL) memerr();
   for (i = 0; i < ncells; i++) isphex27[i] = 0;
   hasphex27 = 0;
   for (i = 0; i < ncells; i++)
     {
      nodesfilled = 0;

      /*  Save cell nodes for regular cells.  */
      if (gmv_meshdata.intype == CELLS && gmv_meshdata.cellnnode != NULL)
        {
         if (gmv_meshdata.cellnnode[i] > 0)
           {
            ncnodes = gmv_meshdata.cellnnode[i];
            for (l = 0; l < ncnodes; l++)
               cellnodenos[l] = gmv_meshdata.cellnodes[totcellnodes+l];
            totcellnodes += ncnodes;
            nodesfilled = 1;
            if (ncnodes == 27)
              {
               hasphex27 = 1;
               isphex27[i] = 1;
              }
           }
        }

      /*  Else gather nodes from face vertices and eliminate duplicates.  */
      if (nodesfilled == 0)
        {
         ncnodes = 0;
         k1 = cells[i].facenos[0];
         for (j = 0; j < cells[i].nfaces; j++)
           {
            k1 = cells[i].facenos[j];
            for (l = 0; l < cellfaces[k1].nverts; l++)
              {
               n = cellfaces[k1].fverts[l];
               if (tstnodes[n] == 0)
                 {
                  cellnodenos[ncnodes] = n;
                  tstnodes[n] = 1;
                  ncnodes++;
                 }
              }
           }
        }

      /*  Save cell nodes.  */
      cells[i].ncnodes = ncnodes;
      cells[i].cnodes = (int *)malloc(ncnodes*sizeof(int));
      if (cells[i].cnodes == NULL) memerr();
      for (j = 0; j < ncnodes; j++)
        {
         cells[i].cnodes[j] = l = cellnodenos[j];
         tstnodes[l] = 0;
         if (cellnodenos[j] < 1 || cellnodenos[j] > nnodes)
           {
            fprintf(stderr,
                 "Error: Cell %d has a node no. %d out of range.\n",
                 i+1,cellnodenos[j]);
            gmvexit();
           }
        }
     }
   if (gmv_meshdata.intype == CELLS)
     {
      FREE(gmv_meshdata.cellnnode);
      FREE(gmv_meshdata.cellnodes);
     }
   free(tstnodes);

   /*  Calculate and save compressed cell edges.  */
   getcelledges();

   free(isphex27);
}


void setcellfaces()
{
  int i, j, j1, j2, k1, k2, nverts;

   /*  Set number of faces read.  */
   faces_in = 1;
   ncellfaces = gmv_meshdata.nfaces;
   if (!noprintflag) printf("GMV read %d faces.\n",ncellfaces);

   /*  Allocate face structures, then read face     */
   /*  data and fill structure data for each face.  */
   if (ncellfaces > 0)
     {
      cellfaces = (struct cellface *)
                     malloc(ncellfaces*sizeof(struct cellface));
      if (cellfaces == NULL) memerr();
     }

   /*  Set face vertices and cell info for each face.  */
   for (i = 0; i < ncellfaces; i++)
     {

      /*  Allocate and fill face vertex numbers.  */
      if (i < ncellfaces - 1) 
         nverts = gmv_meshdata.facetoverts[i+1] - gmv_meshdata.facetoverts[i];
      else nverts = gmv_meshdata.totverts - gmv_meshdata.facetoverts[i];
      cellfaces[i].nverts = nverts;
      cellfaces[i].fverts = (int *)malloc(nverts*sizeof(int));
      j1 = gmv_meshdata.facetoverts[i];
      for (j = 0; j < nverts; j++)
        {
         j2 = gmv_meshdata.faceverts[j1+j];
         cellfaces[i].fverts[j] = j2;
         if (j2 < 1 || j2 > nnodes)
           {
            fprintf(stderr,
                 "Error: face %d has a node no. %d out of range.\n",
                 i+1,j2);
           }
        }

      /*  Set cell no. by face and count cell faces.  */
      cellfaces[i].cell1 = gmv_meshdata.facecell1[i] - 1;
      cellfaces[i].cell2 = gmv_meshdata.facecell2[i] - 1;
      if (cellfaces[i].cell2 >= 0)
         cellfaces[i].oppface = i;
      else cellfaces[i].oppface = -1;
     }
   FREE(gmv_meshdata.facetoverts);
   FREE(gmv_meshdata.faceverts);
   FREE(gmv_meshdata.facecell1);
   FREE(gmv_meshdata.facecell2);

   /*  Allocate and fill cell face nos.  */
   for (i = 0; i < ncells; i++)
     {
      if (i < ncells - 1) 
         j1 = gmv_meshdata.celltoface[i+1] - gmv_meshdata.celltoface[i];
      else
         j1 = gmv_meshdata.totfaces - gmv_meshdata.celltoface[i];
      cells[i].nfaces = j1;
      cells[i].facenos = (int *)malloc(j1*sizeof(int));
      k1 = gmv_meshdata.celltoface[i];
      for (j = 0; j < j1; j++)
        {
         k2 = gmv_meshdata.cellfaces[k1+j];
         cells[i].facenos[j] = k2;
        }
     }
   FREE(gmv_meshdata.celltoface);
   FREE(gmv_meshdata.cellfaces);
   if (!noprintflag) printf("GMV generated %d cells.\n",ncells);
}


void setcellvfaces3d()
{
  int i, ii, j, j1, j2, k1, k2, k3, nfacesin, nverts;
  long *newface;

   /*  Set number of faces read.  */
   nfacesin = gmv_meshdata.nfaces;
   if (!noprintflag) printf("GMV read %d vfaces.\n",nfacesin);

   /*  Use facecell info. to determine duplicate faces.  */
   ncellfaces = nfacesin;
   newface = (long*)malloc(nfacesin*sizeof(long));
   for (i = 0; i < nfacesin; i++)
      newface[i] = i;
   for (i = 0; i < nfacesin; i++)
     {
      if (newface[i] >= 0)
        {
         j = gmv_meshdata.vfaceoppface[i];
         if (j >= 0) newface[j] = -1;
         ncellfaces--;
        }
     }
/*
   for (i = 0; i < nfacesin; i++)
     {
      if (gmv_meshdata.facecell2[i] > 0)
        {
         j = gmv_meshdata.vfaceoppface[i];
         gmv_meshdata.facecell1[j] = 0;
         gmv_meshdata.facecell2[j] = 0;
         gmv_meshdata.cellfaces[j] = i;
         ncellfaces--;
        }
     }
*/
   FREE(gmv_meshdata.vfacepe);
   FREE(gmv_meshdata.vfaceoppfacepe);

   if (!noprintflag) printf("GMV found %d unique faces.\n",ncellfaces);

   /*  Allocate face structures, then read face     */
   /*  data and fill structure data for each face.  */
   ncellfaces = nfacesin;
   if (ncellfaces > 0)
     {
      cellfaces = (struct cellface *)
                     malloc(ncellfaces*sizeof(struct cellface));
      if (cellfaces == NULL) memerr();
     }

   /*  Set face vertices and cell info for each face.  */
   ii = 0;
   for (i = 0; i < nfacesin; i++)
     {
      newface[i] = -1;
      if (gmv_meshdata.facecell1[i] > 0)
        {

         /*  Allocate and fill face vertex numbers.  */
         nverts = gmv_meshdata.facetoverts[i+1] - 
                  gmv_meshdata.facetoverts[i];
         cellfaces[ii].nverts = nverts;
         cellfaces[ii].fverts = (int *)malloc(nverts*sizeof(int));
         j1 = gmv_meshdata.facetoverts[i];
         for (j = 0; j < nverts; j++)
           {
            j2 = gmv_meshdata.faceverts[j1+j];
            cellfaces[ii].fverts[j] = j2;
            if (j2 < 1 || j2 > nnodes)
              {
               fprintf(stderr,
                    "Error: face %d has a node no. %d out of range.\n",
                    i+1,j2);
              }
           }

         /*  Set cell no. by face and count cell faces.  */
         cellfaces[ii].cell1 = gmv_meshdata.facecell1[i] - 1;
         cellfaces[ii].cell2 = gmv_meshdata.facecell2[i] - 1;
         if (cellfaces[ii].cell2 >= 0)
            cellfaces[ii].oppface = gmv_meshdata.vfaceoppface[i];
         else cellfaces[ii].oppface = -1;
         newface[i] = ii;
         ii++;
        }
     }
   FREE(gmv_meshdata.facetoverts);
   FREE(gmv_meshdata.faceverts);
   FREE(gmv_meshdata.facecell1);
   FREE(gmv_meshdata.facecell2);
   FREE(gmv_meshdata.vfaceoppface);

   /*  Allocate and fill cell face nos.  */
   for (i = 0; i < ncells; i++)
     {
      j1 = gmv_meshdata.celltoface[i+1] - gmv_meshdata.celltoface[i];
      cells[i].nfaces = j1;
      cells[i].facenos = (int *)malloc(j1*sizeof(int));
      k1 = gmv_meshdata.celltoface[i];
      for (j = 0; j < j1; j++)
        {
         k2 = gmv_meshdata.cellfaces[k1+j];
         k3 = newface[k2];
         cells[i].facenos[j] = k3;
        }
     }
   FREE(gmv_meshdata.celltoface);
   FREE(gmv_meshdata.cellfaces);
   free(newface);
   if (!noprintflag) printf("GMV generated %d cells.\n",ncells);
}


void setcellvfaces2d()
{
  int i, ii, j, j1, j2, k1, k2, k3, nfacesin, nverts,
      facenodenos[MAXVERTS], nfnodes;
  long *newface;

   /*  For VFACES2D, the faces are edges and the cells are the faces.  */

   /*  Free unneeded data.  */
   FREE(gmv_meshdata.vfacepe);
   FREE(gmv_meshdata.vfaceoppfacepe);

   /*  Set number of faces read.  */
   mesh2dflag = 1;
   nfacesin = gmv_meshdata.nfaces;
   if (!noprintflag) printf("GMV read %d vfaces.\n",nfacesin);

   /*  Use facecell info. to determine duplicate faces.  */
   ncell2dfaces = nfacesin;
   newface = (long*)malloc(nfacesin*sizeof(long));
   ncellfaces = nfacesin + ncells;
   for (i = 0; i < nfacesin; i++)
      newface[i] = i;
   for (i = 0; i < nfacesin; i++)
     {
      if (newface[i] >= 0)
        {
         j = gmv_meshdata.vfaceoppface[i];
         if (j >= 0) newface[j] = -1;
         ncellfaces--;
        }
     }
   if (!noprintflag) printf("GMV found %d unique faces.\n",ncellfaces);

   /*  Allocate and fill faces structure with  */
   /*  1D faces, add ncell faces for 2D faces. */
   ncellfaces = nfacesin + ncells;
   cellfaces = (struct cellface *)
                  malloc(ncellfaces*sizeof(struct cellface));
   if (cellfaces == NULL) memerr();

   /*  Set face vertices and cell info for each face.  */
   ii = 0;
   for (i = 0; i < nfacesin; i++)
     {
      newface[i] = -1;
      if (gmv_meshdata.facecell1[i] > 0)
        {

         /*  Allocate and fill face vertex numbers.  */
         nverts = gmv_meshdata.facetoverts[i+1] - 
                  gmv_meshdata.facetoverts[i];
         cellfaces[ii].nverts = nverts;
         cellfaces[ii].fverts = (int *)malloc(nverts*sizeof(int));
         j1 = gmv_meshdata.facetoverts[i];
         for (j = 0; j < nverts; j++)
           {
            j2 = gmv_meshdata.faceverts[j1+j];
            cellfaces[ii].fverts[j] = j2;
            if (j2 < 1 || j2 > nnodes)
              {
               fprintf(stderr,
                    "Error: face %d has a node no. %d out of range.\n",
                    i+1,j2);
              }
           }

         /*  Set cell no. by face and count cell faces.  */
         cellfaces[ii].cell1 = gmv_meshdata.facecell1[i] - 1;
         cellfaces[ii].cell2 = gmv_meshdata.facecell2[i] - 1;
         if (cellfaces[ii].cell2 >= 0)
            cellfaces[ii].oppface = gmv_meshdata.vfaceoppface[i];
         else cellfaces[ii].oppface = -1;
         newface[i] = ii;
         ii++;
        }
     }
   FREE(gmv_meshdata.facecell1);
   FREE(gmv_meshdata.facecell2);
   FREE(gmv_meshdata.vfaceoppface);

   /*  Allocate and fill cell face  */
   /*  nos. and 2D cellfaces data.  */
   for (i = 0; i < ncells; i++)
     {
      j1 = gmv_meshdata.celltoface[i+1] - gmv_meshdata.celltoface[i];
      cells[i].nfaces = j1 + 1;
      cells[i].facenos = (int *)malloc((j1+1)*sizeof(int));
      k1 = gmv_meshdata.celltoface[i];
      nfnodes = 0;
      for (j = 0; j < j1; j++)
        {
         k2 = gmv_meshdata.cellfaces[k1+j];
         k3 = newface[k2];
         cells[i].facenos[j] = k3;
         nverts = gmv_meshdata.facetoverts[k2+1] - 
                  gmv_meshdata.facetoverts[k2];
         j2 = gmv_meshdata.facetoverts[k2];
         for (ii = 0; ii < nverts-1; ii++)
           {
            facenodenos[nfnodes] = gmv_meshdata.faceverts[j2+ii];
            nfnodes++;
           }
        }
      cells[i].facenos[j1] = ncell2dfaces + i;
      cellfaces[ncell2dfaces+i].nverts = nfnodes;
      cellfaces[ncell2dfaces+i].fverts = (int *)malloc(nfnodes*sizeof(int));
      for (j = 0; j < nfnodes; j++)
        {
         cellfaces[ncell2dfaces+i].fverts[j] = facenodenos[j];
        }
      cellfaces[ncell2dfaces+i].cell1 = i;
      cellfaces[ncell2dfaces+i].cell2 = -1;
      cellfaces[ncell2dfaces+i].oppface = -1;
     }
   FREE(gmv_meshdata.celltoface);
   FREE(gmv_meshdata.cellfaces);
   FREE(gmv_meshdata.faceverts);
   FREE(gmv_meshdata.facetoverts);
   free(newface);
   if (!noprintflag) printf("GMV generated %d cells.\n",ncells);
}


void matvalchk(int nmatin, int maxmatin, int data_type, short *matdata)
{
  int i, modcount;

   /*  Check for more than MAXMAT materials.  */
   modcount = 0;
   if (maxmatin > MAXMATS)
     {
      fprintf(stderr,"Warning, there are more than %d materials.\n",
              MAXMATS);
      fprintf(stderr,"   Note, materials > %d will be set to mod %d\n",
              MAXMATS,MAXMATS);
      maxmatin = MAXMATS;
      for (i = 0; i < nmatin; i++)
        {
         if (matdata[i] > maxmatin)
           {
            matdata[i] = matdata[i] % MAXMATS;
            if (matdata[i] == 0)
               matdata[i] = MAXMATS;
            modcount++;
           }
        }
     }
   if (modcount > 0)
     {
      fprintf(stderr,"Warning, there are %d elements with material > %d.\n",
              modcount,MAXMATS);
     }


   /*  Check for material nos. > mmats.  */
   if (data_type == 0)
     {
      for (i = 0; i < nmatin; i++)
        {
         if (matdata[i] > mmats)
            printf("Error, material for cell %d is greater than %d\n",
                    i+1,mmats);
        }
     }
   if (data_type == 1)
     {
      for (i = 1; i < nmatin+1; i++)
        {
         if (matdata[i] > mmats)
            printf("Error, material for node %d is greater than %d\n",
                    i,mmats);
        }
     }
}


void bldcellmats()
{
  int i, ii, j, k, l, maxsum, matsum[MAXMATS+1];
  void fillstructcell(int icell, int iopt);

   /*  Build cell material from node materials.  */
   for (i = 0; i < ncells; i++)
     {
      for (j = 0; j < MAXMATS; j++) matsum[j] = 0;

      ii = i;
      if (structuredflag == 1)
        {
         ii = 0;
         fillstructcell(i,0);
        }
      for (j = 0; j < cells[ii].ncnodes; j++)
        {
         k = cells[ii].cnodes[j];
         l = imat[k];
         matsum[l]++;
        }
      maxsum = matsum[0];
      k = 0;
      for (j = 1; j < MAXMATS; j++)
        {
         if (maxsum < matsum[j]) k = j;
         maxsum = matsum[k];
        }
      cellmats[i] = k;
     }            
}


void bldnodemats()
{
  int i, ii, j, k, l, maxmat, icell;
  unsigned int matcount[mmats];
  unsigned int **node2cells, *nnode2cells, *n2c_alloc;
  void fillstructcell(int icell, int iopt);

   /*  Allocate cells in node arrays.  */
   ii = 10;
   node2cells = (unsigned int **)malloc((nnodes + 1) * sizeof(unsigned int *));
   nnode2cells = (unsigned int *)malloc((nnodes + 1) * sizeof(unsigned int));
   n2c_alloc = (unsigned int *)malloc((nnodes + 1) * sizeof(unsigned int));
   for (i = 0; i < nnodes+1; i++)
     {
      node2cells[i] = (unsigned int*)malloc(10 * sizeof(unsigned int));
      nnode2cells[i] = 0;
      n2c_alloc[i] = 10; 
     }

   /*  Loop through cells to fill node2cells array.  */
   for (icell = 0; icell < ncells; icell++)
     {
      ii = icell;
      if (structuredflag == 1)
        {
         ii = 0;
         fillstructcell(icell,0);
        }

      for (j = 0; j < cells[ii].ncnodes; j++)
        {
         k = cells[ii].cnodes[j];
         if (nnode2cells[k] + 1 > n2c_alloc[k])
           {
            n2c_alloc[k] += 10;
            l = n2c_alloc[k];          
            node2cells[k] = (unsigned int *)realloc(node2cells[k], 
                                                 l * sizeof(unsigned int));
           }
         l = nnode2cells[k];
         node2cells[k][l] = icell;
         nnode2cells[k]++;       
        } 
     }
   free(n2c_alloc);

   /*  Build node material from cell materials.  */
   for (i = 1; i < nnodes+1; i++)
     {
      imat[i] = -1;
      for (j = 0; j < mmats; j++) matcount[j] = 0;
      for (j = 0; j < nnode2cells[i]; j++)
        {
         icell = node2cells[i][j];
         l = cellmats[icell] - 1;
         if (l >= 0) matcount[l]++;
        }

      /*  Determine max. no. of materials per node.  */
      maxmat = 0;   ii = -1;
      for (k = 0; k < mmats; k++)
        {
         if (matcount[k] > maxmat)
           {
            maxmat = matcount[k];
            ii = k;
           }
        }
      if (ii >= 0)
         imat[i] = ii + 1;
     }

   for (i = 0; i < nnodes+1; i++)
      free(node2cells[i]);
   free(node2cells);   free(nnode2cells);
}


void savemats()
{
  /*                               */
  /*  Get and set material data.   */
  /*                               */
  int i, data_type, lnmatin, mmatin;

   /*  Get the number of materials and determine data type (cells or nodes).  */
   mmatin = (short)gmv_data.num;
   if (gmv_data.datatype == CELL) data_type = 0;
   if (gmv_data.datatype == NODE) data_type = 1;
   if (data_type == 0) lnmatin = ncells;
   if (data_type == 1) lnmatin = nnodes;

   /*  Check for materials > MAXMATS.  */
   mmats = mmatin;
   if (mmatin > MAXMATS) mmats = MAXMATS;

   if (data_type == 0)
     {
      mats_on_cells = 1;
      if (!noprintflag) printf("GMV read %d cell materials.\n",mmats);
     }
   if (data_type == 1)
     {
      mats_on_nodes = 1;
      if (!noprintflag) printf("GMV read %d node materials.\n",mmats);
     }

   /*  Allocate and save 32 character material names.  */
   for (i = 0; i < mmats; i++)
     {
      strncpy(mmatnames[i],&gmv_data.chardata1[i*33],32);
      mmatnames[i][32] = (char) 0;
      if (!noprintflag) printf("    %s\n",mmatnames[i]);
     }

   /*  Allocate and fill material nos.  */
   if (imat == NULL)
     {
      imat=(short *)malloc((nnodes+1)*sizeof(short));
      if (imat == NULL) memerr();
     }
   if (ncells > 0 && cellmats == NULL)
     {
      cellmats=(short *)malloc((ncells)*sizeof(short));
      if (cellmats == NULL) memerr();
     }

   if (data_type == 0) ncellmats = mmats;
   else nnodemats = mmats;

   /*  Fill imat array if node data and get a cell material.  */
   if (data_type == 1)
     {
      for (i = 0; i < nnodes; i++)
         imat[i+1] = (short)gmv_data.longdata1[i];

      /*  Check for invalid material data.  */
      matvalchk(nnodes,mmatin,data_type,imat);

      /*  Calculate cell materials if not already read.  */
      if (ncellmats == 0) bldcellmats();
     }

   /*  If cell data, create node data from cell data,  */
   /*  by using the material of the max. cell mats.    */
   if (data_type == 0)
     {

      /*  Fill cell materials.  */
      for (i = 0; i < ncells; i++)
         cellmats[i] = (short)gmv_data.longdata1[i];

      /*  Check for invalid material data.  */
      matvalchk(ncells,mmatin,data_type,cellmats);

      /*  Calculate node materials if not already read.  */
      if (nnodemats == 0) bldnodemats();
     }    

   mmats = (nnodemats > ncellmats) ? nnodemats : ncellmats;
}


void savevels()
{
  /*                               */
  /*  Get and set velocity data.  */
  /*                               */
  int i, data_type, nvelin;

   /*  Determine data type (cells or nodes).  */
   if (gmv_data.datatype == CELL) data_type = 0;
   if (gmv_data.datatype == NODE) data_type = 1;
   if (gmv_data.datatype == FACE) data_type = 2;

   /*  Allocate and save velocity data.  */
   if (data_type == 0) nvelin = ncells;
   if (data_type == 1) nvelin = nnodes;
   if (data_type == 2) nvelin = ncellfaces;
   if (data_type == 2 && mesh2dflag > 0)
     {
      nvelin = ncell2dfaces;
      mesh2dflag = 2;
     }

   if (data_type == 0)
     {
      cuic = (double *)malloc(ncells*sizeof(double));
      cvic = (double *)malloc(ncells*sizeof(double));
      cwic = (double *)malloc(ncells*sizeof(double));
      if (cuic == NULL || cvic == NULL || cwic == NULL) memerr();
      cvectin = 0;
     }
   if (data_type == 1)
     {
      uic = (double *)malloc((nnodes+1)*sizeof(double));
      vic = (double *)malloc((nnodes+1)*sizeof(double));
      wic = (double *)malloc((nnodes+1)*sizeof(double));
      if (uic == NULL || vic == NULL || wic == NULL) memerr();
      nvectin = 0;
      }
   if (data_type == 2)
     {
      fuic = (double *)malloc(ncellfaces*sizeof(double));
      fvic = (double *)malloc(ncellfaces*sizeof(double));
      fwic = (double *)malloc(ncellfaces*sizeof(double));
      if (fuic == NULL || fvic == NULL || fwic == NULL) memerr();
      fvectin = 0;
     }

   if (!noprintflag)
     {
      if (data_type == 0) 
         printf("GMV reading %d cell velocities.\n",nvelin);
      if (data_type == 1) 
         printf("GMV reading %d node velocities.\n",nvelin);
      if (data_type == 2) 
         printf("GMV reading %d face velocities.\n",nvelin);
     }

   /*  Fill arrays.  */
   if (data_type == 0)
     {
      cvectors = 1;
      for (i = 0; i < ncells; i++)
        {
         cuic[i] = gmv_data.doubledata1[i];
         cvic[i] = gmv_data.doubledata2[i];
         cwic[i] = gmv_data.doubledata3[i];
        }
     }
   if (data_type == 1)
     {
      vectors = 1;
      for (i = 0; i < nnodes; i++)
        {
         uic[i+1] = gmv_data.doubledata1[i];
         vic[i+1] = gmv_data.doubledata2[i];
         wic[i+1] = gmv_data.doubledata3[i];
        }
     }
   if (data_type == 2)
     {
      fvectors = 1;
      for (i = 0; i < nvelin; i++)
        {
         fuic[i] = gmv_data.doubledata1[i];
         fvic[i] = gmv_data.doubledata2[i];
         fwic[i] = gmv_data.doubledata3[i];
        }
     }
}


void savevars()
{
  /*                                     */
  /*  Get and set variable field data.  */
  /*                                     */
  int i, data_type;
  long nvarin;
  static double *vfield;
  char varname[33];
  static int ifirst = 1;
  void writenodefld(double *value);
  void writecellfld(double *value);
  void writefacefld(double *value);

   /*  Process node or cell variable data. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE || 
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading variables.\n");

         /*  Allocate a double to hold variable data.  */
         i = nnodes + 1;
         if (ncells > i) i = ncells;
         if (ncellfaces > i) i = ncellfaces;
         vfield = (double *)malloc(i*sizeof(double));
         if (vfield == NULL) memerr();
        }

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;
      if (gmv_data.datatype == FACE) data_type = 2;

      /*  Allocate and save variable name.  */
      strcpy(varname,gmv_data.name1);
      if (data_type == 0)
        {
         cfieldname[cnumvars]=(char *)malloc(33*sizeof(char));
         if (cfieldname[cnumvars] == NULL) memerr();
         strncpy(cfieldname[cnumvars],varname,32);
         *(cfieldname[cnumvars]+32) = (char) 0;
         cvars_to_write[cnumvars] = 1;
        }
      if (data_type == 1)
        {
         fieldname[numvars]=(char *)malloc(33*sizeof(char));
         if (fieldname[numvars] == NULL) memerr();
         strncpy(fieldname[numvars],varname,32);
         *(fieldname[numvars]+32) = (char) 0;
         nvars_to_write[numvars] = 1;
        }
      if (data_type == 2)
        {
         ffieldname[fnumvars]=(char *)malloc(33*sizeof(char));
         if (ffieldname[fnumvars] == NULL) memerr();
         strncpy(ffieldname[fnumvars],varname,32);
         *(ffieldname[fnumvars]+32) = (char) 0;
         fvars_to_write[fnumvars] = 1;
        }

      if (!noprintflag) printf("    %s  (%s)\n",varname,type_str[data_type]);

      /*  If cell data, fill array and write.  */
      if (data_type == 0)
        {
         for (i = 0; i < ncells; i++)
            vfield[i] = gmv_data.doubledata1[i];
         writecellfld(vfield);
         cnumvars++;
        }

      /*  If node data, fill array and write.  */
      if (data_type == 1)
        {
         for (i = 0; i < nnodes; i++)
            vfield[i+1] = gmv_data.doubledata1[i];
         writenodefld(vfield);
         numvars++;
        }

      /*  If face data, fill array and write.  */
      if (data_type == 2)
        {
         nvarin = ncellfaces;
         if (mesh2dflag > 0)
           {
            nvarin = ncell2dfaces;
            mesh2dflag = 2;
           }
         for (i = 0; i < ncellfaces; i++) vfield[i] = 0.;
         for (i = 0; i < nvarin; i++)
            vfield[i] = gmv_data.doubledata1[i];
         writefacefld(vfield);
         fnumvars++;
        }

      if (numvars >= MAXFIELDS || cnumvars >= MAXFIELDS || 
          fnumvars >= MAXFIELDS)
        {
         fprintf(stderr,"Error, there are more than %d variables.\n",
                 MAXFIELDS);
         gmvexit();
        }
     }

   /*  Return if ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      free(vfield);
      return;
     }
}


void saveflags()
{
  /*                                     */
  /*  Read and set selection flag data.  */
  /*                                     */
  int i, data_type, ntypes;
  static int ifirst = 1, modcount;

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      if (modcount > 0)
        {
         fprintf(stderr,"Warning, there are %d elements with a flag > %d.\n",
                 modcount, MAXMATS);
        }
      return;
     }

   /*  Process cell or node flag data.  */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading flags.\n");
         modcount = 0;
        }

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;

      /*  Check that the number of cell flags is less than MAXFLAGS.  */
      if (data_type == 0)
        {
          if (numflags_cell + 1 >= MAXFLAGS)
           {
            printf("Error, there are more than %d cell flags.\n",MAXFLAGS);
            printf("   The rest of the cell flags will not be processed.\n");
            return;
           }
        }

      /*  Check that the number of node flags is less than MAXFLAGS.  */
      if (data_type == 1)
        {
          if (numflags_node + 1 >= MAXFLAGS)
           {
            printf("Error, there are more than %d node flags.\n",MAXFLAGS);
            printf("   The rest of the node flags will not be processed.\n");
            return;
           }
        }

      /*  Get number of flag types.  */
      ntypes = gmv_data.num2;
      if (ntypes > MAXMATS)
        {
         if (modcount == 0)
           {
            fprintf(stderr,"Warning, there are more than %d flag types.\n",
                    MAXMATS);
            fprintf(stderr,"   Note, flag types > %d will be set to mod %d\n",
                    MAXMATS,MAXMATS);
           }
         ntypes = MAXMATS;
         for (i = 0; i < gmv_data.num; i++)
           {
            if (gmv_data.longdata1[i] > ntypes)
              {
               gmv_data.longdata1[i] = gmv_data.longdata1[i] % MAXMATS;
               if (gmv_data.longdata1[i] == 0) 
                  gmv_data.longdata1[i] = MAXMATS;
               modcount++;
              }
           }
        }

      /*  Save data depending on type.  */
      if (data_type == 0)
        {

         /*  Save cell flag names.  */
         flagname_cell[numflags_cell] = (char *)malloc(33*sizeof(char));
         if (flagname_cell[numflags_cell] == NULL) memerr();
         strncpy(flagname_cell[numflags_cell],gmv_data.name1,32);
         *(flagname_cell[numflags_cell]+32) = (char) 0;
         flagtypes_cell[numflags_cell] = (short) ntypes;

         for (i = 0; i < ntypes; i++)
            {
             flagnames_cell[numflags_cell][i] =
                (char *)malloc(33*sizeof(char));
             strncpy(flagnames_cell[numflags_cell][i],
                     &gmv_data.chardata1[i*33],32);
             *(flagnames_cell[numflags_cell][i]+32) = (char) 0;
            }

         /*  Save cell flag data.  */
         cellflags[numflags_cell] = (short *)malloc((ncells)*sizeof(short));
         if (cellflags[numflags_cell] == NULL) memerr();
         for (i = 0; i < ncells; i++)
            cellflags[numflags_cell][i] = (short)gmv_data.longdata1[i];

         numflags_cell++;
        }

      if (data_type == 1)
        {

         /*  Save node flag names.  */
         flagname_node[numflags_node] = (char *)malloc(33*sizeof(char));
         if (flagname_node[numflags_node] == NULL) memerr();
         strncpy(flagname_node[numflags_node],gmv_data.name1,32);
         *(flagname_node[numflags_node]+32) = (char) 0;
         flagtypes_node[numflags_node] = (short) ntypes;

         for (i = 0; i < ntypes; i++)
            {
             flagnames_node[numflags_node][i]=
                (char *)malloc(33*sizeof(char));
             strncpy(flagnames_node[numflags_node][i],
                     &gmv_data.chardata1[i*33],32);
             *(flagnames_node[numflags_node][i]+32) = (char) 0;
            }

         /*  Save node flag data.  */
         flags[numflags_node] = (short *)malloc((nnodes+1)*sizeof(short));
         if (flags[numflags_node] == NULL) memerr();
         for (i = 0; i < nnodes; i++)
            flags[numflags_node][i+1] = (short)gmv_data.longdata1[i];

         numflags_node++;
        }
     }
}


void savepolys()
{
  /*                                             */
  /*  Read and set interface/boundary polygons.  */
  /*                                             */
  int i, k, limatno, nvertsin;
  static int vertsalloc[MAXMATS], nvertsalloc[MAXMATS];
  static int ifirst = 1, totpolys, mincount, modcount;

   /*  Initialize control arrays.  */
   if (ifirst)
     {
      ifirst = 0;
      for (i = 0; i < MAXMATS; i++)
        {
         vertsalloc[i] = 0;
         nvertsalloc[i] = 0;
         npolyvertspermat[i] = 0;
         npolyspermat[i] = 0;
        }
      totpolys = 0;
      mincount = 0;
      modcount = 0;
      if (!noprintflag) printf("GMV reading polygons.\n");
     }

   /*  Save polygon data.  */
   if (gmv_data.datatype == REGULAR)
     {

      /*  Get and check material number, max. mat. and no. of vertices.  */
      limatno = gmv_data.num;
      if (limatno < 1)
        {
         if (mincount == 0)
           {
            fprintf(stderr,"Warning, polygon material no. is less than 1\n");
            fprintf(stderr,"  Note polygon material no. will be set to 1\n");
           }
         mincount++;
         limatno = 1;
        }
      if (limatno > MAXMATS)
        {
         if (modcount == 0)
           {
            fprintf(stderr,"Warning, polygon material no. is greater than %d\n",
                    MAXMATS);
            fprintf(stderr,"  Note material no. will be set to mod %d\n",
                    MAXMATS);
           }
         modcount++;
         limatno = limatno % MAXMATS;
         if (limatno == 0) limatno = MAXMATS;
        }
      maxmatno = (limatno > maxmatno) ? limatno : maxmatno;
      nvertsin = gmv_data.ndoubledata1;
      
      /*  If this is the first polygon for this material,   */
      /*  allocate arrays.  Also, check for reallocations.  */
      limatno--;
      if (npolyspermat[limatno] == 0)
        {
         polygons[limatno] = (struct polystruct *)malloc(MSIZE*
                              sizeof(struct polystruct));
         nvertsalloc[limatno] = MSIZE;
         polyverts[limatno] = (struct polyvert *)malloc(MSIZE*6*
                               sizeof(struct polyvert));
         vertsalloc[limatno] = MSIZE * 6;
         if (polygons[limatno] == NULL ||
             polyverts[limatno] == NULL) memerr();
        }
      else
        {
         if (npolyspermat[limatno] + 1 > nvertsalloc[limatno])
           {
            k = nvertsalloc[limatno] + MSIZE;
            polygons[limatno] = (struct polystruct *)realloc
                        (polygons[limatno],k*sizeof(struct polystruct));
            nvertsalloc[limatno] = k;
            if (polygons[limatno] == NULL) memerr();
           }
         if (npolyvertspermat[limatno] + nvertsin > vertsalloc[limatno])
           {
            k = vertsalloc[limatno] + MSIZE * 6;
            polyverts[limatno] = (struct polyvert *)realloc
                       (polyverts[limatno],k*sizeof(struct polyvert));
            vertsalloc[limatno] = k;
            if (polyverts[limatno] == NULL) memerr();
           }
        }  

      /* Save vertices and set data.  */
      npolyspermat[limatno]++;
      totpolys++;
      k = npolyspermat[limatno] - 1;
      polygons[limatno][k].nverts = (short) nvertsin;
      polygons[limatno][k].polyvertloc = npolyvertspermat[limatno];
      k = npolyvertspermat[limatno];
      npolyvertspermat[limatno] += nvertsin;
      for (i = 0; i < nvertsin; i++)
        {
         polyverts[limatno][k+i].x = gmv_data.doubledata1[i];
         polyverts[limatno][k+i].y = gmv_data.doubledata2[i];
         polyverts[limatno][k+i].z = gmv_data.doubledata3[i];
        }
     }

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD) 
     {
      /*  Reallocate arrays to actual size.  */
      for (i = 0; i < maxmatno; i++)
        {
         if (npolyspermat[i] > 0)
           {
            k = npolyspermat[i];
            //TJ 20150317 valgrind issue on this line, can't fix with memset on polygons[i] when it's realloc
            polygons[i] = (struct polystruct *)realloc
                          (polygons[i],k*sizeof(struct polystruct));
            k = npolyvertspermat[i];
            polyverts[i] = (struct polyvert *)realloc
                           (polyverts[i],k*sizeof(struct polyvert));
           }
        }
      polysin = 1;

      if (!noprintflag) printf("GMV read %d polygons.\n",totpolys);
      ifirst = 1;

      if (mincount > 0)
         fprintf(stderr,"Warning, there are %d polygons with material < 1\n",
                 mincount);
      if (modcount > 0)
         fprintf(stderr,"Warning, there are %d polygons with material > %d\n",
                 modcount,MAXMATS);
     }
}


void savetracers()
{
  /*                             */
  /*  Read and set tracer data.  */
  /*                             */
  int lntracers, i;
  static int lnumvartr;

   /*  Get tracer xyz data.  */
   if (gmv_data.datatype == XYZ)
     {

      /*  Get no of tracers.  */
      lntracers =  gmv_data.num;
      if (!noprintflag) printf("GMV reading %d Particles.\n",lntracers);

      /*  Allocate and save tracer xyz.  */
      if (lntracers > 0)
        {
         tracers[ntracehist].x = (double *)malloc(sizeof(double) * lntracers);
         tracers[ntracehist].y = (double *)malloc(sizeof(double) * lntracers);
         tracers[ntracehist].z = (double *)malloc(sizeof(double) * lntracers);
         for (i = 0; i < lntracers; i++)
           {
            tracers[ntracehist].x[i] = gmv_data.doubledata1[i];
            tracers[ntracehist].y[i] = gmv_data.doubledata2[i];
            tracers[ntracehist].z[i] = gmv_data.doubledata3[i];
            tracers[ntracehist].ntracers = lntracers;
           }
        }

      if (ntracehist == 0) ntracers = lntracers;
      lnumvartr = 0;
     }

   /*  Get tracer field data.  */
   if (gmv_data.datatype == TRACERDATA)
     {
      lntracers = gmv_data.num;

      /*  Allocate and save tracer file name and data.  */
      if (ntracehist == 0)
        {
  
         /*  Save name.  */
         fieldtrname[lnumvartr] = (char *)malloc(33*sizeof(char));
         if (fieldtrname[lnumvartr] == NULL) memerr();
         strncpy(fieldtrname[lnumvartr],gmv_data.name1,32);
         *(fieldtrname[lnumvartr]+32) = (char)0;
        }

      /*  Save data.  */
      if (lntracers > 0)
        {
         tracers[ntracehist].field[lnumvartr] = 
                         (double *)malloc(sizeof(double) * lntracers);
         for (i = 0; i < lntracers; i++)
           {
            tracers[ntracehist].field[lnumvartr][i] = gmv_data.doubledata1[i];
           }
        }

      lnumvartr++;
      if (lnumvartr >= MAXTRFIELDS)
        {
         fprintf(stderr,"Error, there are more than %d Particle fields.\n",
                 MAXTRFIELDS);
         gmvexit();
        }
     }

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      tracers[ntracehist].numvar = lnumvartr;
      if (ntracehist == 0)
         numvartr = lnumvartr;
      ntracehist++;
     }

}


void savenodeids()
{
  /*                                              */
  /*  Get and set alternate node numbers (ids).  */
  /*                                              */
  int i;

   if (!noprintflag) printf("GMV reading node ids.\n");

   /*  Allocate node ids.  */
   nodeids = (int *)malloc((nnodes+1)*sizeof(int));
   if (nodeids == NULL) memerr();

   /*  Set the node ids.  */
   nodeids[0] = 0;
   for (i = 0; i < nnodes; i++)
      nodeids[i+1] = gmv_data.longdata1[i];

   nodeidflag = 1;
}


void savecellids()
{
  /*                                              */
  /*  Get and set alternate cell numbers (ids).   */
  /*                                              */
  int i;

   if (!noprintflag) printf("GMV reading cell ids.\n");

   /*  Allocate cell ids.  */
   cellids = (int *)malloc((ncells)*sizeof(int));
   if (cellids == NULL) memerr();

   /*  Set the cell ids.  */
   for (i = 0; i < ncells; i++)
      cellids[i] = gmv_data.longdata1[i];

   cellidflag = 1;
}


void savefaceids()
{
  /*                                              */
  /*  Get and set alternate face numbers (ids).   */
  /*                                              */
  int i, numfaces;

   if (!noprintflag) printf("GMV reading face ids.\n");

   /*  Allocate face ids.  */
   faceids = (int *)malloc((ncellfaces)*sizeof(int));
   if (faceids == NULL) memerr();
   for (i = 0; i < ncellfaces; i++) faceids[i] = 0;

   /*  Set the face ids.  */
   numfaces = ncellfaces;
   if (mesh2dflag > 0) numfaces = ncell2dfaces;
   for (i = 0; i < numfaces; i++)
      faceids[i] = gmv_data.longdata1[i];

   faceidflag = 1;
}


void savetracerids()
{
  /*                                               */
  /*  Get and set alternate tracer numbers (ids).  */
  /*                                               */
  int i;

   if (!noprintflag) printf("GMV reading Particle ids.\n");
   if (ntracers == 0) return;

   /*  Allocate tracer ids.  */
   tracerids = (int *)malloc((ntracers)*sizeof(int));
   if (tracerids == NULL) memerr();

   /*  Set the tracer ids.  */
   for (i = 0; i < ntracers; i++)
      tracerids[i] = gmv_data.longdata1[i];

   traceridflag = 1;
}


void saveunits()
{
  /*                                   */
  /*  Get and set units information.  */
  /*                                   */
  int i;
  static int ifirst = 1;
  int lnumunits_node, lnumunits_cell;

   if (ifirst)
     {
      ifirst = 0;
      if (!noprintflag) printf("GMV reading units.\n");
     }

   /*  Save XYZ units.  */
   if (gmv_data.datatype == XYZ)
      strcpy(units.xyz,gmv_data.chardata2);

   /*  Save velocity units.  */
   if (gmv_data.datatype == VEL)
      strcpy(units.uvw,gmv_data.chardata2);

   /*  Allocate and save node units.  */
   if (gmv_data.datatype == NODE)
     {
      lnumunits_node = gmv_data.num;
      units.nnodes = lnumunits_node;

      for (i = 0; i < lnumunits_node; i++)
        {
         strcpy(units.nfldname[i],&gmv_data.chardata1[i*33]);
         strcpy(units.nfldunit[i],&gmv_data.chardata2[i*33]);
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


void savesurface()
{
  int j, k, nverts;
  static int ifirst = 1, isurf;

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      return;
     }

   /*  Get number of surface facets to read.  */
   if (ifirst)
     {
      ifirst = 0;
      nsurf = gmv_data.num;
      if (!noprintflag) printf("GMV reading %d surfaces.\n",nsurf);

      /*  Allocate surface structures.  */
      if (nsurf > 0)
        {
         surfs = (struct surface *)
                        malloc(nsurf*sizeof(struct surface));
         if (surfs == NULL) memerr();
        }
      isurf = 0;
     }

   /*  Save surface facet vertices.  */
   nverts = gmv_data.nlongdata1;
   if (nverts > MAXVERTS)
     {
      fprintf(stderr,
              "Error, surface %d has more than 10000 vertices.\n",isurf);
      gmvexit();
     }

   surfs[isurf].nverts = nverts;
   surfs[isurf].fverts = (int *)malloc(nverts*sizeof(int));
   for (j = 0; j < nverts; j++)
     {
      k =  gmv_data.longdata1[j];
      surfs[isurf].fverts[j] = k;
      if (k < 1 || k > nnodes)
        {
         fprintf(stderr,
                 "Error: surface %d has a node no. %d out of range.\n",
                  isurf+1,k);
        }
     }
   isurf++;
}


void savesurfmats()
{
  /*                                       */
  /*  Get and set surface material data.  */
  /*                                       */
  int i;

   /*  If surface materials have been read with 0 surfaces, return.  */
   if (nsurf == 0) return;

   if (!noprintflag) printf("GMV reading %d surfmats.\n",nsurf);

   /*  Allocate and save surface material data.  */
   surfmats=(short *)malloc((nsurf)*sizeof(short));
   if (surfmats == NULL) memerr();
   for (i = 0; i < nsurf; i++) 
      surfmats[i] = gmv_data.longdata1[i];

   /*  Determine max. surf material no.  */
   maxsurfmat = 0;
   for (i = 0; i < nsurf; i++)
     {
      if (surfmats[i] > maxsurfmat)  maxsurfmat = surfmats[i];
     }
   if (maxsurfmat > MAXMATS)
     {
      printf("Warning - surface material is greater than %d\n",MAXMATS);
      fprintf(stderr,"   Note, materials > %d will be set to mod %d\n",
              MAXMATS,MAXMATS);
      for (i = 0; i < nsurf; i++)
        {
         if (surfmats[i] > MAXMATS)
           {
            surfmats[i] = surfmats[i] % MAXMATS;
            if (surfmats[i] == 0) surfmats[i] = MAXMATS;
           }
        }
     }

   /*  Check for material nos. > mmats.  */
   if (mmats > 0)
     {
      for (i = 0; i < nsurf; i++)
        {
         if (surfmats[i] > mmats)
           {
            printf("Warning, material for surface %d is greater than %d\n",
                    i-1,mmats);
           }
        }
     }
}


void savesurfvel()
{
  /*                                       */
  /*  Get and set surface velocity data.  */
  /*                                       */
  int i;

   /*  If surface has been read with 0 surfaces, return.  */
   if (nsurf == 0) return;

   if (!noprintflag) printf("GMV reading %d surface velocities.\n",nsurf);

   /*  Allocate and save velocity data.  */
   suic = (double *)malloc((nsurf)*sizeof(double));
   svic = (double *)malloc((nsurf)*sizeof(double));
   swic = (double *)malloc((nsurf)*sizeof(double));
   if (suic == NULL || svic == NULL || swic == NULL) memerr();
   svectin = 0;

   /*  Fill arrays.  */
   svectors = 1;
   for (i = 0; i < nsurf; i++)
     {
      suic[i] = gmv_data.doubledata1[i];
      svic[i] = gmv_data.doubledata2[i];
      swic[i] = gmv_data.doubledata3[i];
     }
}


void savesurfvars()
{
  /*                                             */
  /*  Get and set surface variable field data.  */
  /*                                             */
  int i;
  static double *varin;
  void writesurffld(double *value);
  static int ifirst = 1;

   /*  If surface vaiables have been read with 0 surfaces return.  */
   if (nsurf == 0) return;


   if (gmv_data.datatype == REGULAR)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading surface variables.\n");

         /*  Allocate temporary array.  */
         varin = (double *)malloc(nsurf*sizeof(double));
         if (varin == NULL) memerr();
        }

      /*  Save name.  */
      sfieldname[snumvars]=(char *)malloc(33*sizeof(char));
      if (sfieldname[snumvars] == NULL) memerr();
      strncpy(sfieldname[snumvars],gmv_data.name1,32);
      *(sfieldname[snumvars]+32) = (char) 0;
      svars_to_write[snumvars] = 1;

      if (!noprintflag) printf("    %s\n",gmv_data.name1);

      /*  Get surface field data.  */
      for (i = 0; i < nsurf; i++)
         varin[i] = gmv_data.doubledata1[i];

      /*  Write out variable data to surface field database file.  */
      writesurffld(varin);

      snumvars++;
      if (snumvars >= MAXFIELDS)
        {
         fprintf(stderr,"Error, there are more than %d surface variables.\n",
                 MAXFIELDS);
         gmvexit();
        }
     }

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      if (ifirst == 0) free(varin);
      ifirst = 1;
     }
}


void savesurfflag()
{
  /*                                     */
  /*  Read and set selection flag data.  */
  /*                                     */
  int i, ntypes;
  static int ifirst = 1, lnumflags_surf;


   /*  If surface has been read with 0      */
   /*  surfaces, read endsflag and return.  */
   if (nsurf == 0) return;

   /*  Check for endkeyword.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      return;
     }

   if (ifirst)
     {
      ifirst = 0;
      lnumflags_surf = 0;
      if (!noprintflag) printf("GMV reading surface flags.\n");
     }

   /*  Check that the number of surface flags is less than MAXFLAGS.  */
   if (lnumflags_surf + 1 >= MAXFLAGS)
     {
      printf("Error, there are more than %d surface flags.\n",MAXFLAGS);
      printf("   The rest of the surface flags will not be processed.\n");
      return;
     }

   /*  Get flag types.  */
   ntypes = gmv_data.num2;
   if (ntypes > MAXMATS)
     {
      fprintf(stderr,"Warning, there are more than %d flag types.\n",
              MAXMATS);
      fprintf(stderr,"  Note, flags types > %d will be set to mod %d.\n",
              MAXMATS,MAXMATS);
      ntypes = MAXMATS;
      for (i = 0; i < gmv_data.num; i++)
        {
         if (gmv_data.longdata1[i] > ntypes)
           {
            gmv_data.longdata1[i] = gmv_data.longdata1[i] % MAXMATS;
            if (gmv_data.longdata1[i] == 0)
               gmv_data.longdata1[i] = MAXMATS;
           }
        }
     }

   /*  Save name, type and flag names.  */
   flagname_surf[lnumflags_surf] = (char *)malloc(33*sizeof(char));
   if (flagname_surf[lnumflags_surf] == NULL) memerr();
   strncpy(flagname_surf[lnumflags_surf],gmv_data.name1,32);
   *(flagname_surf[lnumflags_surf]+32) = (char) 0;
   flagtypes_surf[lnumflags_surf] = (short) ntypes;

   for (i = 0; i < ntypes; i++)
     {
      flagnames_surf[lnumflags_surf][i]=
             (char *)malloc(33*sizeof(char));
      strncpy(flagnames_surf[lnumflags_surf][i],&gmv_data.chardata1[i*33],32);
      *(flagnames_surf[lnumflags_surf][i]+32) = (char) 0;
     }


   /*  Allocate and save flag data.  */
   surfflags[lnumflags_surf] = (short *)malloc((nsurf)*sizeof(short));
   if (surfflags[lnumflags_surf] == NULL) memerr();
   for (i = 0; i < nsurf; i++)
      surfflags[lnumflags_surf][i] = (short)gmv_data.longdata1[i];
   
   /*  Increment flag counters.  */
   lnumflags_surf++;
   numflags_surf = lnumflags_surf;
}


void savesurfids()
{
  /*                                                 */
  /*  Get and set alternate surface numbers (ids).   */
  /*                                                 */
  int i;

   if (!noprintflag) printf("GMV reading surface ids.\n");

   /*  Allocate surface ids.  */
   surfids = (int *)malloc((nsurf)*sizeof(int));
   if (surfids == NULL) memerr();

   /*  Set the surface ids.  */
   for (i = 0; i < nsurf; i++)
      surfids[i] = gmv_data.longdata1[i];

   surfidflag = 1;
}


void savegroups()
{
  /*                            */
  /*  Read and set group data.  */
  /*                            */
  int i, data_type, ngrp;
  static int ifirst = 1;

   /*  Check for ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      return;
     }

   /*  Process cell, node or face group data.  */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE ||
       gmv_data.datatype == FACE || gmv_data.datatype == SURF)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading groups.\n");
        }

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;
      if (gmv_data.datatype == FACE) data_type = 2;
      if (gmv_data.datatype == SURF) data_type = 3;

      if (!noprintflag) 
         printf("    %s  (%s)\n",gmv_data.name1,type_str[data_type]);

      /*  Get the number of elements in the group.  */
      ngrp = gmv_data.num;

      /*  Save data depending on type.  */
      if (data_type == 0)
        {

         /*  Save cell group name.  */
         cellgrpname[ncellgrps] = (char *)malloc(33*sizeof(char));
         if (cellgrpname[ncellgrps] == NULL) memerr();
         strncpy(cellgrpname[ncellgrps],gmv_data.name1,32);
         *(cellgrpname[ncellgrps]+32) = (char) 0;

         /*  Save the number and data for this cell group.  */
         numcellgrp[ncellgrps] = ngrp;

         cellgrps[ncellgrps] = (int *)malloc((ngrp)*sizeof(int));
         if (cellgrps[ncellgrps] == NULL) memerr();
         for (i = 0; i < ngrp; i++)
            cellgrps[ncellgrps][i] = (int)gmv_data.longdata1[i];

         ncellgrps++;
        }

      if (data_type == 1)
        {

         /*  Save node group name.  */
         nodegrpname[nnodegrps] = (char *)malloc(33*sizeof(char));
         if (nodegrpname[nnodegrps] == NULL) memerr();
         strncpy(nodegrpname[nnodegrps],gmv_data.name1,32);
         *(nodegrpname[nnodegrps]+32) = (char) 0;

         /*  Save the number and data for this node group.  */
         numnodegrp[nnodegrps] = ngrp;

         nodegrps[nnodegrps] = (int *)malloc((ngrp)*sizeof(int));
         if (nodegrps[nnodegrps] == NULL) memerr();
         for (i = 0; i < ngrp; i++)
            nodegrps[nnodegrps][i] = (int)gmv_data.longdata1[i];

         nnodegrps++;
        }

      if (data_type == 2)
        {

         /*  Save face group name.  */
         facegrpname[nfacegrps] = (char *)malloc(33*sizeof(char));
         if (facegrpname[nfacegrps] == NULL) memerr();
         strncpy(facegrpname[nfacegrps],gmv_data.name1,32);
         *(facegrpname[nfacegrps]+32) = (char) 0;

         /*  Save the number and data for this face group.  */
         numfacegrp[nfacegrps] = ngrp;

         facegrps[nfacegrps] = (int *)malloc((ngrp)*sizeof(int));
         if (facegrps[nfacegrps] == NULL) memerr();
         for (i = 0; i < ngrp; i++)
            facegrps[nfacegrps][i] = (int)gmv_data.longdata1[i];

         nfacegrps++;
        }

      if (data_type == 3)
        {

         /*  Save surface group name.  */
         surfacegrpname[nsurfacegrps] = (char *)malloc(33*sizeof(char));
         if (surfacegrpname[nsurfacegrps] == NULL) memerr();
         strncpy(surfacegrpname[nsurfacegrps],gmv_data.name1,32);
         *(surfacegrpname[nsurfacegrps]+32) = (char) 0;

         /*  Save the number and data for this face group.  */
         numsurfacegrp[nsurfacegrps] = ngrp;

         surfacegrps[nsurfacegrps] = (int *)malloc((ngrp)*sizeof(int));
         if (surfacegrps[nsurfacegrps] == NULL) memerr();
         for (i = 0; i < ngrp; i++)
            surfacegrps[nsurfacegrps][i] = (int)gmv_data.longdata1[i];

         nsurfacegrps++;
        }
     }
}


void savesubvars()
{
  /*                                   */
  /*  Get and set subvars field data.  */
  /*                                   */
  int i, j, data_type, nelem;
  double fmin, fmax, fillval;
  static double *vfield;
  char varname[33];
  static int ifirst = 1;
  void writenodefld(double *value);
  void writecellfld(double *value);
  void writefacefld(double *value);

   /*  Process node or cell variable data. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE || 
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading subvars.\n");

         /*  Allocate a double to hold variable data.  */
         i = nnodes + 1;
         if (ncells > i) i = ncells;
         if (ncellfaces > i) i = ncellfaces;
         vfield = (double *)malloc(i*sizeof(double));
         if (vfield == NULL) memerr();
        }

      /*  If no data, return.
      nelem = gmv_data.num;
      if (nelem == 0) return;

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;
      if (gmv_data.datatype == FACE) data_type = 2;

      /*  Allocate and save variable name.  */
      strcpy(varname,gmv_data.name1);
      if (data_type == 0)
        {
         cfieldname[cnumvars]=(char *)malloc(33*sizeof(char));
         if (cfieldname[cnumvars] == NULL) memerr();
         strncpy(cfieldname[cnumvars],varname,32);
         *(cfieldname[cnumvars]+32) = (char) 0;
         cvars_to_write[cnumvars] = 0;
         csubvarsin[cnumvars] = 1;
        }
      if (data_type == 1)
        {
         fieldname[numvars]=(char *)malloc(33*sizeof(char));
         if (fieldname[numvars] == NULL) memerr();
         strncpy(fieldname[numvars],varname,32);
         *(fieldname[numvars]+32) = (char) 0;
         nvars_to_write[numvars] = 0;
         nsubvarsin[numvars] = 1;
        }
      if (data_type == 2)
        {
         ffieldname[fnumvars]=(char *)malloc(33*sizeof(char));
         if (ffieldname[fnumvars] == NULL) memerr();
         strncpy(ffieldname[fnumvars],varname,32);
         *(ffieldname[fnumvars]+32) = (char) 0;
         fvars_to_write[fnumvars] = 0;
         fsubvarsin[fnumvars] = 1;
        }

      if (!noprintflag) printf("    %s  (%s)\n",varname,type_str[data_type]);

      /*  Calculate min and max values.  */
      nelem = gmv_data.num;
      fmin = MAXVAL;
      fmax = -MAXVAL;
      for (i = 0; i < nelem; i++)
        {
         fmin = (gmv_data.doubledata1[i] < fmin) ? 
                   gmv_data.doubledata1[i] : fmin;
         fmax = (gmv_data.doubledata1[i] > fmax) ? 
                   gmv_data.doubledata1[i] : fmax;
        }
      if (fmin > 0.) fillval = 0;
      else fillval = -MAXVAL;

      /*  If cell data, fill array and write.  */
      if (data_type == 0)
        {
         for (i = 0; i < ncells; i++)
            vfield[i] = fillval;
         for (i = 0; i < nelem; i++)
           {
            j = gmv_data.longdata1[i] - 1;
            if (j >= 0 && j < ncells)
               vfield[j] = gmv_data.doubledata1[i];
            else
               printf("Warning, subvars - cell id %d is out of range.\n",
                      j+1);
          }
         writecellfld(vfield);
         cfieldmin[cnumvars] = fmin;
         cfieldmax[cnumvars] = fmax;
         cnumvars++;
        }

      /*  If node data, fill array and write.  */
      if (data_type == 1)
        {
         for (i = 0; i < nnodes; i++)
            vfield[i+1] = fillval;
         for (i = 0; i < nelem; i++)
           {
            j = gmv_data.longdata1[i];
            if (j > 0 && j <= nnodes)
                vfield[j] = gmv_data.doubledata1[i];
            else
               printf("Warning, subvars - node id %d is out of range.\n",
                      j);
           }
         writenodefld(vfield);
         fieldmin[numvars] = fmin;
         fieldmax[numvars] = fmax;
         numvars++;
        }

      /*  If face data, fill array and write.  */
      if (data_type == 2)
        {
         if (mesh2dflag > 0) mesh2dflag = 2;
         for (i = 0; i < ncellfaces; i++)
            vfield[i] =  fillval;
         for (i = 0; i < nelem; i++)
           {
            j = gmv_data.longdata1[i] - 1;
            if (j >= 0 && j < ncellfaces)
               vfield[j] = gmv_data.doubledata1[i];
            else
               printf("Warning, subvars - face id %d is out of range.\n",
                      j+1);
           }
         writefacefld(vfield);
         ffieldmin[fnumvars] = fmin;
         ffieldmax[fnumvars] = fmax;
         fnumvars++;
        }

      if (numvars >= MAXFIELDS || cnumvars >= MAXFIELDS || 
          fnumvars >= MAXFIELDS)
        {
         fprintf(stderr,"Error, subvars - there are more than %d variables.\n",
                 MAXFIELDS);
         gmvexit();
        }
     }

   /*  Return if ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      free(vfield);
      return;
     }
}


void saveghosts()
{
  /*                           */
  /*  Get and set ghost data.  */
  /*                           */
  int i, j, data_type, nelem;

   /*  Process node or cell ghost info. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE)
     {

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;

      if (data_type == 0 && !noprintflag) printf("GMV reading ghost cells.\n");
      if (data_type == 1 && !noprintflag) printf("GMV reading ghost nodes.\n");

      nelem = gmv_data.nlongdata1;

      /*  If cell save ids.  */
      if (data_type == 0)
        {
         cellghosts = (int *)malloc(nelem * sizeof(int));
         if (cellghosts == NULL) memerr();
         cellghostflag = 1;
         ncellghosts = 0;
         for (i = 0; i < nelem; i++)
           {
            j = gmv_data.longdata1[i] - 1;
            if (j >= 0 && j < ncells)
              {
               cellghosts[ncellghosts] = j;
               ncellghosts++;
              }
            else
               printf("Warning, ghosts - cell id %d is out of range.\n",
                      j+1);
           }
        }

      /*  If node save ids.  */
      if (data_type == 1)
        {
         nodeghosts = (int *)malloc(nelem * sizeof(int));
         if (nodeghosts == NULL) memerr();
         nnodeghosts = 0;
         nodeghostflag = 1;
         for (i = 0; i < nelem; i++)
           {
            j = gmv_data.longdata1[i];
            if (j > 0 && j <= nnodes)
              {
               nodeghosts[ncellghosts] = j;
               nnodeghosts++;
              }
            else
               printf("Warning, ghosts - node id %d is out of range.\n",
                      j);
           }
        }
     }
}


void savevects()
{
  /*                                  */
  /*  Get and set vector field data.  */
  /*                                  */
  int i, data_type, ncomps, ic;
  long nvarin;
  static double *vfield;
  char varname[33], vectname[33];
  static int ifirst = 1;
  void writenodefld(double *value);
  void writecellfld(double *value);
  void writefacefld(double *value);

   /*  Process node, cell or face vector data. */
   if (gmv_data.datatype == CELL || gmv_data.datatype == NODE || 
       gmv_data.datatype == FACE)
     {
      if (ifirst)
        {
         ifirst = 0;
         if (!noprintflag) printf("GMV reading vectors.\n");

         /*  Allocate a double to hold vector data by component.  */
         i = nnodes + 1;
         if (ncells > i) i = ncells;
         if (ncellfaces > i) i = ncellfaces;
         vfield = (double *)malloc(i*sizeof(double));
         if (vfield == NULL) memerr();
        }

      /*  Determine data type.  */
      if (gmv_data.datatype == CELL) data_type = 0;
      if (gmv_data.datatype == NODE) data_type = 1;
      if (gmv_data.datatype == FACE) data_type = 2;

      /*  Allocate and save vector name.  */
      strcpy(vectname,gmv_data.name1);
      if (!noprintflag) printf("    %s  (%s)\n",vectname,type_str[data_type]);

      /*  Loop throuh the number of components and save each    */
      /*  component as a scalar field with the component name.  */
      ncomps = gmv_data.num2;
      for (ic = 0; ic < ncomps; ic++)
        {
         strncpy(varname,&gmv_data.chardata1[ic*33],32);
         if (data_type == 0)
           {
            cfieldname[cnumvars]=(char *)malloc(33*sizeof(char));
            if (cfieldname[cnumvars] == NULL) memerr();
            strncpy(cfieldname[cnumvars],varname,32);
            *(cfieldname[cnumvars]+32) = (char) 0;
            cvars_to_write[cnumvars] = 1;
           }
         if (data_type == 1)
           {
            fieldname[numvars]=(char *)malloc(33*sizeof(char));
            if (fieldname[numvars] == NULL) memerr();
            strncpy(fieldname[numvars],varname,32);
            *(fieldname[numvars]+32) = (char) 0;
            nvars_to_write[numvars] = 1;
           }
         if (data_type == 2)
           {
            ffieldname[fnumvars]=(char *)malloc(33*sizeof(char));
            if (ffieldname[fnumvars] == NULL) memerr();
            strncpy(ffieldname[fnumvars],varname,32);
            *(ffieldname[fnumvars]+32) = (char) 0;
            fvars_to_write[fnumvars] = 1;
           }

         /*  If cell data, fill array and write.  */
         if (data_type == 0)
           {
            for (i = 0; i < ncells; i++)
               vfield[i] = gmv_data.doubledata1[(ic*ncells)+i];
            writecellfld(vfield);
            cnumvars++;
           }

         /*  If node data, fill array and write.  */
         if (data_type == 1)
           {
            for (i = 0; i < nnodes; i++)
               vfield[i+1] = gmv_data.doubledata1[(ic*nnodes)+i];
            writenodefld(vfield);
            numvars++;
           }

         /*  If face data, fill array and write.  */
         if (data_type == 2)
           {
            nvarin = ncellfaces;
            if (mesh2dflag > 0)
              {
               nvarin = ncell2dfaces;
               mesh2dflag = 2;
              }
            for (i = 0; i < ncellfaces; i++) vfield[i] = 0.;
            for (i = 0; i < nvarin; i++)
               vfield[i] = gmv_data.doubledata1[(ic*ncellfaces)+i];
            writefacefld(vfield);
            fnumvars++;
           }

         if (numvars >= MAXFIELDS || cnumvars >= MAXFIELDS || 
             fnumvars >= MAXFIELDS)
          {
            fprintf(stderr,"Error, there are more than %d vectors.\n",
                    MAXFIELDS);
            gmvexit();
           }
        }
     }

   /*  Return if ENDKEYWORD.  */
   if (gmv_data.datatype == ENDKEYWORD)
     {
      ifirst = 1;
      free(vfield);
      return;
     }
}


void memerr()
{
  /*                 */
  /*  Memory error.  */
  /*                 */

   fprintf(stderr,"Not enough memory to run this problem.\n");
   gmvexit();
}


void fldwrterr()
{
  /*                                */
  /*  Field data base write error.  */
  /*                                */

   fprintf(stderr,"Not enough disk space for field database file.\n");
       fprintf(stderr,
          "Set the environmental variable TMPDIR to a directory "
          "with more disk space.\n");
       fprintf(stderr,"eg.: setenv TMPDIR mydirectroy\n");
   gmvexit();
}


void fillstructcell(int icell, int iopt)
{
  int ncnodes, itemp[8], ii, jj, j, kk, nfaces, nedges, 
      nxyv, nxyc, ixc, iyc, izc, ixy, nxc, nyc, icell2;
  int nfxs, nfys, nxfaces, nyfaces,
      xface, yface, zface, iface[6];
  int nexs, neys, nxedges, nyedges, 
      xedge, yedge, zedge, iedge[12];
  short hexfverts[24] = {1,4,3,2, 1,2,6,5, 2,3,7,6, 3,4,8,7,
                         4,1,5,8, 5,6,7,8};
  short edge1[12] = {1,2,3,4, 1,2,3,4, 5,6,7,8};
  short edge2[12] = {2,3,4,1, 5,6,7,8, 6,7,8,5};

  /*  Fill structured cell data for cell icell.  */
  /*  icell - cell no.                           */
  /*  iopt - level of information needed:        */
  /*         0 - fill nodes and averages         */
  /*         1 - fill nodes, averages and faces  */
  /*         2 - fill nodes, averages and edges  */
  /*         3 - fill all cell data              */

   nfaces = 6;
   ncnodes = 8;
   nedges = 12;
   if (struct2dflag)
     {
      nfaces = 1;
      ncnodes = 4;
      nedges = 4;
     }
   nxyv = nxv * nyv;
   nxc = nxv - 1;
   nyc = nyv - 1;
   nxyc = nxc * nyc;

   /*  Determine cell vertices.  */
   izc = icell / nxyc;
   ixy = icell - izc * nxyc;
   iyc = ixy / nxc;
   ixc = ixy - iyc * nxc;

   itemp[0] = ixc + iyc*nxv + izc*nxyv + 1;
   itemp[1] = itemp[0] + 1;
   itemp[2] = itemp[1] + nxv;
   itemp[3] = itemp[0] + nxv;
   itemp[4] = itemp[0] + nxyv;
   itemp[5] = itemp[1] + nxyv;
   itemp[6] = itemp[2] + nxyv;
   itemp[7] = itemp[3] + nxyv;

   /*  Fill node list.  */
   for (ii = 0; ii < ncnodes; ii++)
       cells[0].cnodes[ii] = itemp[ii];

   /*  Fill center point.  */
   cells[0].xavg = 0;
   cells[0].yavg = 0;
   cells[0].zavg = 0;
   for (ii = 0; ii < ncnodes; ii++)
     {
      jj = itemp[ii];
      cells[0].xavg += nodes[jj].x;
      cells[0].yavg += nodes[jj].y;
      cells[0].zavg += nodes[jj].z;
     }
   cells[0].xavg /= (float)ncnodes;
   cells[0].yavg /= (float)ncnodes;
   cells[0].zavg /= (float)ncnodes;

   /*  Fill face information.  */
   if (iopt == 1 || iopt == 3)
     {
      nfxs = nxv * (nyv-1);
      nfys = nyv * (nxv-1);
      nxfaces = nfxs * (nzv-1);
      nyfaces = nfys * (nzv-1);
      xface = iyc*nxv + ixc + nfxs*izc;
      yface = iyc*nxc + ixc + nfys*izc + nxfaces;
      zface = icell + nxfaces + nyfaces;
      iface[0] = zface;
      iface[1] = yface;
      iface[2] = xface + 1;
      iface[3] = yface + nxc;
      iface[4] = xface;
      iface[5] = zface + nxyc;
      for (ii = 0; ii < nfaces; ii++)
        {
         cells[0].facenos[ii] = iface[ii];
         cellfaces[ii].nverts = 4;
         for (j = 0; j < 4; j++)
           {
            kk = (ii*4) + j;
            jj = hexfverts[kk] - 1;
            cellfaces[ii].fverts[j] = itemp[jj];
           }
         cellfaces[ii].cell1 = icell;
         switch (ii)
           {
            case 0: icell2 = icell - nxyc;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) + 4;
                    if (izc == 0)
                       cellfaces[ii].cell2 = -1;
                    break;
            case 1: icell2 = icell - nxc;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) + 2;
                    if (iyc == 0)
                       cellfaces[ii].cell2 = -1;
                    break;
            case 2: icell2 = icell + 1;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) + 3;
                    if (ixc+1 == nxc)
                       cellfaces[ii].cell2 = -1;
                    break;
            case 3: icell2 = icell + nxc;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) + 0;
                    if (iyc+1 == nyc)
                       cellfaces[ii].cell2 = -1;
                    break;
            case 4: icell2 = icell - 1;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) + 1;
                    if (ixc == 0)
                       cellfaces[ii].cell2 = -1;
                    break;
            case 5: icell2 = icell + nxyc;
                    cellfaces[ii].cell2 = icell2;
                    cellfaces[ii].oppface = (icell2 * nfaces) - 1;
                    if (icell+nxyc >= ncells)
                       cellfaces[ii].cell2 = -1;
                    break;
           }
         if (struct2dflag) cellfaces[ii].cell2 = -1;
         if (cellfaces[ii].cell2 == -1) cellfaces[ii].oppface = -1;
        }
     }

   /*  Fill unique edges.  */
   if (iopt == 2 || iopt == 3)
     {
      nexs = nxv * (nyv-1);
      neys = nyv * (nxv-1);
      nxedges = nexs * nzv;
      nyedges = neys * nzv;
      xedge = iyc*nxv + ixc + nexs*izc;
      yedge = iyc*nxc + ixc + neys*izc + nxedges;
      zedge = itemp[0] + nxedges + nyedges - 1;
      iedge[0] = yedge;
      iedge[1] = xedge + 1;
      iedge[2] = yedge + nxc;
      iedge[3] = xedge;
      iedge[4] = zedge;
      iedge[5] = zedge + 1;
      iedge[6] = zedge + nxv + 1;
      iedge[7] = zedge + nxv;
      iedge[8] = yedge + neys;
      iedge[9] = xedge + nexs + 1;
      iedge[10] = iedge[2] + neys;
      iedge[11] = xedge + nexs;
      for (ii = 0; ii < nedges; ii++)
        {
         cells[0].edgenos[ii] = iedge[ii];
         jj = edge1[ii] - 1;
         celledges[ii].edgept1 = itemp[jj];
         jj = edge2[ii] - 1;
         celledges[ii].edgept2 = itemp[jj];
        }
     }
}


void writepolyfile()
{
  int i, j, nfldwrt;

   rewind(polyfile);
   for (i = 0; i < mmats; i++)
     {
      j = npolyspermat[i];
      if (j > 0)
        {
         nfldwrt = fwrite(polygons[i],sizeof(struct polystruct),j,polyfile);
         if (nfldwrt != j) fldwrterr();
         FREE(polygons[i]);
        }
     }

   for (i = 0; i < mmats; i++)
     {
      j = npolyvertspermat[i];
      if (j > 0)
        {
         nfldwrt = fwrite(polyverts[i],sizeof(struct polyvert),j,polyfile);
         if (nfldwrt != j) fldwrterr();
         FREE(polyverts[i]);
        }
     }
}


void readpolyfile()
{
  int i, j, nfldrd;

   rewind(polyfile);
   for (i = 0; i < mmats; i++)
     {
      j = npolyspermat[i];
      if (j > 0)
        {
         polygons[i] = (struct polystruct *)malloc(j*
                              sizeof(struct polystruct));
         nfldrd = fread(polygons[i],sizeof(struct polystruct),j,polyfile);
         if (nfldrd != j)
            printf("Warning, reading polygon data, expected %d, read %d\n",
                   j,nfldrd);
        }
     }

   for (i = 0; i < mmats; i++)
     {
      j = npolyvertspermat[i];
      if (j > 0)
        {
         polyverts[i] = (struct polyvert *)malloc(j*
                              sizeof(struct polyvert));
         nfldrd = fread(polyverts[i],sizeof(struct polyvert),j,polyfile);
         if (nfldrd != j)
            printf("Warning, reading polygon data, expected %d, read %d\n",
                   j,nfldrd);
        }
     }
}
