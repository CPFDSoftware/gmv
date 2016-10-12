#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
//#include <malloc.h>
#include <stdio.h>

extern FILE *fieldfile, *cfieldfile, *sfieldfile, *ffieldfile;
extern int nnodes, ncells, nsurf, ncellfaces;


void writenodefld(double *value)
{
  long int nfldwrt;

   fseek(fieldfile, 0, 2);
   nfldwrt = fwrite(value,sizeof(double),nnodes+1,fieldfile);
   if (nfldwrt != nnodes+1) fldwrterr();
}


void writenodefldloc(int ifld, double *value)
{
  long int loc, nfldwrt;

   loc = ifld * (nnodes + 1);
   fseek(fieldfile, loc*sizeof(double), 0);
   nfldwrt = fwrite(value,sizeof(double),nnodes+1,fieldfile);
   if (nfldwrt != nnodes+1) fldwrterr();
}


double * readnodevalue(int ifld)
{
  long int loc, nfldrd;
  double *value;

   value=(double *)malloc((nnodes+1)*sizeof(double));
   if (value == NULL) memerr();
   loc = ifld * (nnodes+1);
   fseek(fieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), nnodes+1, fieldfile);
   if (nfldrd != nnodes+1)
      printf("Warning, reading field data, expected %d, read %ld\n",
             nnodes+1,nfldrd);
   return value;
}


void fillnodevalue(int ifld, double *value)
{
  long int loc, nfldrd;

   loc = ifld * (nnodes+1);
   fseek(fieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), nnodes+1, fieldfile);
   if (nfldrd != nnodes+1)
      printf("Warning, reading field data, expected %d, read %ld\n",
             nnodes+1,nfldrd);
}


void writecellfld(double *value)
{
  long int nfldwrt;

   fseek(cfieldfile, 0, 2);
   nfldwrt = fwrite(value,sizeof(double),ncells,cfieldfile);
   if (nfldwrt != ncells) fldwrterr();
}


void writecellfldloc(int ifld, double *value)
{
  long int loc, nfldwrt;

   loc = ifld * ncells;
   fseek(cfieldfile, loc*sizeof(double), 0);
   nfldwrt = fwrite(value,sizeof(double),ncells,cfieldfile);
   if (nfldwrt != ncells) fldwrterr();
}


double * readcellvalue(int ifld)
{
  long int loc, nfldrd;
  double *value;

   value=(double *)malloc(ncells*sizeof(double));
   if (value == NULL) memerr();
   loc = ifld * ncells;
   fseek(cfieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), ncells, cfieldfile);
   if (nfldrd != ncells)
      printf("Warning, reading field data, expected %d, read %ld\n",
             ncells,nfldrd);
   return value;
}


void fillcellvalue(int ifld, double *value)
{
  long int loc, nfldrd;

   loc = ifld * ncells;
   fseek(cfieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), ncells, cfieldfile);
   if (nfldrd != ncells)
      printf("Warning, reading field data, expected %d, read %ld\n",
             ncells,nfldrd);
}


void writesurffld(double *value)
{
  long int nfldwrt;

   fseek(sfieldfile, 0, 2);
   nfldwrt = fwrite(value,sizeof(double),nsurf,sfieldfile);
   if (nfldwrt != nsurf) fldwrterr();
}


void writesurffldloc(int ifld, double *value)
{
  long int loc, nfldwrt;

   loc = ifld * nsurf;
   fseek(sfieldfile, loc*sizeof(double), 0);
   nfldwrt = fwrite(value,sizeof(double),nsurf,sfieldfile);
   if (nfldwrt != nsurf) fldwrterr();
}


double * readsurfvalue(int ifld)
{
  long int loc, nfldrd;
  double *value;

   value=(double *)malloc(nsurf*sizeof(double));
   if (value == NULL) memerr();
   loc = ifld * nsurf;
   fseek(sfieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), nsurf, sfieldfile);
   if (nfldrd != nsurf)
      printf("Warning, reading field data, expected %d, read %ld\n",
             nsurf,nfldrd);
   return value;
}


void fillsurfvalue(int ifld, double *value)
{
  long int loc, nfldrd;

   loc = ifld * nsurf;
   fseek(sfieldfile, loc*sizeof(double), 0);
   nfldrd = fread(value, sizeof(double), nsurf, sfieldfile);
   if (nfldrd != nsurf)
      printf("Warning, reading field data, expected %d, read %ld\n",
             nsurf,nfldrd);
}


void writefacefld(double *value)
{
  long int ffldwrt;

   fseek(ffieldfile, 0, 2);
   ffldwrt = fwrite(value,sizeof(double),ncellfaces,ffieldfile);
   if (ffldwrt != ncellfaces) fldwrterr();
}


void writefacefldloc(int ifld, double *value)
{
  long int loc, ffldwrt;

   loc = ifld * ncellfaces;
   fseek(ffieldfile, loc*sizeof(double), 0);
   ffldwrt = fwrite(value,sizeof(double),ncellfaces,ffieldfile);
   if (ffldwrt != ncellfaces) fldwrterr();
}


double * readfacevalue(int ifld)
{
  long int loc, ffldrd;
  double *value;

   value=(double *)malloc(ncellfaces*sizeof(double));
   if (value == NULL) memerr();
   loc = ifld * ncellfaces;
   fseek(ffieldfile, loc*sizeof(double), 0);
   ffldrd = fread(value, sizeof(double), ncellfaces, ffieldfile);
   if (ffldrd != ncellfaces)
      printf("Warning, reading field data, expected %d, read %ld\n",
             ncellfaces,ffldrd);
   return value;
}


void fillfacevalue(int ifld, double *value)
{
  long int loc, ffldrd;

   loc = ifld * ncellfaces;
   fseek(ffieldfile, loc*sizeof(double), 0);
   ffldrd = fread(value, sizeof(double), ncellfaces, ffieldfile);
   if (ffldrd != ncellfaces)
      printf("Warning, reading field data, expected %d, read %ld\n",
             ncellfaces,ffldrd);
}
