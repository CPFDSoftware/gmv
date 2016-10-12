#include <math.h>

#include "gmvdata.h"

#define MAXVAL 9.99999e32


void compute_boundary_triangle_unit_normals()
{
/*  This function takes the list of point numbers pointed to by 
    bdry, 3 per triangle, and computes a list of unit normal
    components, 3 per triangle, which it leaves in the list pointed
    to by bdrynorm.  Both bdry and bdrynorm are arrays of pointers,
    one per each material number.  The point sequences in bdry are
    supposed to be pre-arranged so that these are all outward-
    pointing unit normals.  */

  int i, j, k, ierr, npv;
  long nv;
  double x1,y1,z1,x2,y2,z2,x3,y3,z3,xno,yno,zno, t,
         dist, epslon = 1.0e-10, a, b, c, maxrad;
  void writepolyfile();
     
   if (polysin == 0) return;

   for (i = 0; i < mmats; i++)
     {

      for (j = 0; j < npolyspermat[i]; j++ )
        {

         /*  Calculate polygon average location to  */
         /*  use as the first point of the normal.  */
         x1 = 0.;  y1 = 0.;  z1 = 0.;
         npv = polygons[i][j].nverts;
         nv = polygons[i][j].polyvertloc;
         for (k = 0; k < npv; k++)
           {
            x1 += polyverts[i][k+nv].x;
            y1 += polyverts[i][k+nv].y;
            z1 += polyverts[i][k+nv].z;
           }
         x1 /= (double)npv;
         y1 /= (double)npv;
         z1 /= (double)npv;

         x2 = polyverts[i][1+nv].x;
         y2 = polyverts[i][1+nv].y;
         z2 = polyverts[i][1+nv].z;
         x3 = polyverts[i][2+nv].x;
         y3 = polyverts[i][2+nv].y;
         z3 = polyverts[i][2+nv].z;

         xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
         yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
         zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
         t = sqrt(xno*xno + yno*yno + zno*zno);

         /*  Check that we have a valid normal.  */
         if (t == 0)
           {

            /*  Find a second point.  */
            ierr = 0;
            k = 1;
            dist = 0.;
            while (dist < epslon && k < npv)
              {
               x2 = polyverts[i][k+nv].x;
               y2 = polyverts[i][k+nv].y;
               z2 = polyverts[i][k+nv].z;
               dist = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) +
                      (z2-z1)*(z2-z1);
               k++;
              }
            if (k == npv) ierr = 1;

            /*  Find a third non-linear point.  */
            dist = 0.;
            while (dist < epslon && k < npv)
              {
               x3 = polyverts[i][k+nv].x;
               y3 = polyverts[i][k+nv].y;
               z3 = polyverts[i][k+nv].z;
               xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
               yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
               zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
               dist =  xno*xno + yno*yno + zno*zno;
               k++;
              }
            if (k == npv && dist < epslon) ierr = 1;

            /*  Recalculate the normal components.  */
            if (ierr == 0)
              {
               xno = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
               yno = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
               zno = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
               t = sqrt(xno*xno + yno*yno + zno*zno);
              }
            else
              {
               xno = 1.;  yno = 1.;  zno = 1.;  t = 1.;
              }
           }

         /*  Save the normal and average location.  */
         polygons[i][j].xnorm = xno/t;
         polygons[i][j].ynorm = yno/t;
         polygons[i][j].znorm = zno/t;
         polygons[i][j].xavg = x1;
         polygons[i][j].yavg = y1;
         polygons[i][j].zavg = z1;

         /*  Calculate the maximum radius from the  */
         /*  average location to the vertices.      */
         maxrad = 0;
         for (k = 0; k < npv; k++)
           {
            a = x1 - polyverts[i][k+nv].x;
            b = y1 - polyverts[i][k+nv].y;
            c = z1 - polyverts[i][k+nv].z;
            dist = a*a + b*b + c*c;
            if (dist > maxrad && dist > myzero)
               maxrad = dist;
           }
         polygons[i][j].maxradius = sqrt(maxrad);
        }
     }

   /*  Write out polygon data and release memory.  */
   writepolyfile();

   /*  If polysflag is on, fill polygon data.  */
   if (polysflag || linesflag) readpolyfile();
}


#define  FALSE  0
#define   TRUE  1
#define   ZERO  0.0
#define    ONE  1.0
#define    TEN 10.0
#define     P4  0.9999
#define     P5  0.99999
#define   XCON  2.30258 /* XCON cannot be 0!! */
#define    EPS  0.001  

void nicelog(double llimit, double ulimit, int max, double *val, int *nret)
{   

  /*  This routine generates a nice log scale.   */
  /*  Note, do NOT send values <= 0.               */
  /*  Variables:                                   */
  /*    llimit   - lower limit of the array        */
  /*    ulimit   - upper limit of the array        */
  /*    max      - maximum numbers allowed         */
  /*    val[max] - returned array of nice numbers  */
  /*    nret     - number of returned values       */
  /*    nexp     - an exponent such that val[i]/10**nexp is an integer  */

  int i, j, k, ij, mul, icy, nexp, nrt; 
  double xxl, xxu;

  double lind[5]    = {9, 5, 3, 2, 1};
  double seed[9][5] = {1, 1, 1, 1, 1, 
                       2, 2, 2, 3, 0, 
                       3, 4, 5, 0, 0, 
                       4, 6, 0, 0, 0, 
                       5, 8, 0, 0, 0, 
                       6, 0, 0, 0, 0, 
                       7, 0, 0, 0, 0,  
                       8, 0, 0, 0, 0, 
                       9, 0, 0, 0, 0};

   nrt = 0;

   if ( llimit <= ZERO || ulimit <= ZERO )
     {
       return;
     }           

   xxl = llimit;
   xxu = ulimit;
 
   if ( xxu < xxl)
     {
      xxl = ulimit;
      xxu = llimit; 
     }
      
   xxl = log (xxl) / P5 / XCON;
   if ( xxl < 0 )
     {
      xxl = xxl * P4 - ONE;
     }

   ij  = xxl;
   mul = ij;
   xxl = pow(10, (double)ij);

   xxu = log (xxu) / P5 / XCON;
   if ( xxu > 0 )  
     {
      xxu = xxu * P4 + ONE;
     }

   icy = xxu;
   icy = icy - ij;

   
   for ( k = 0; k < 5; k++ )
     {
       nrt = icy*lind[k] + 1;
 
       if (nrt <= max )  
         {
          break;
         }
       if ( k == 4 )
         {
           k = -1;
           icy--;
         }
     }


   ij = -1;
   for ( i = 0; i < icy; i++ )
      {
      for ( j = 0; j < lind[k] ; j++ )
         {
         ij++;
         val[ij] = seed[j][k] * xxl * pow(TEN, (double)(i)); 
         }
      }

   val[nrt-1] = xxl * pow(TEN, (double)icy);

   nexp = mul;
   *nret = nrt; 
}


void nicelog_new(double llimit, double ulimit, int max, double *val, int *nret)
{   

  /*  This routine generates a nice log scale.   */
  /*  Note, do NOT send values <= 0.               */
  /*  Variables:                                   */
  /*    llimit   - lower limit of the array        */
  /*    ulimit   - upper limit of the array        */
  /*    max      - maximum numbers allowed         */
  /*    val[max] - returned array of nice numbers  */
  /*    nret     - number of returned values       */
  /*    nexp     - an exponent such that val[i]/10**nexp is an integer  */

  int i, j, nexp, imin, imax; 

   imin = log10(llimit) - .5;
   imax = log10(ulimit) + .5;
   nexp = imax - imin + 1;
   if (nexp > max) 
     {
      j = nexp - max;
      imin = imin + j;
      nexp = max;
     }
   
   for (i = 0; i < nexp; i++)
     {
      val[i] = pow(10., (double)(imin+i));
     }

   *nret = nexp;
}


void nicelin(double llimit, double ulimit, int max, double *val, int *nret)
{   

  /*  This routine generates a nice linear scale.  */
  /*  Note, do NOT send values <= 0.               */
  /*  Variables:                                   */
  /*    llimit   - lower limit of the array        */
  /*    ulimit   - upper limit of the array        */
  /*    max      - maximum numbers allowed         */
  /*    val[max] - returned array of nice numbers  */
  /*    nret     - number of returned values       */
  /*    nexp     - an exponent such that val[i]/10**nexp is an integer  */

  int    i, j, ij, ik, nexp, nrt;
  int    nf0, ijof, mul, ikof;  
  int    done;
  double y, cy, xxu, xxl, del;
  double xncr[6] = { 1.0, 2.0, 2.5, 5.0, 10.0, 0.0};

   nrt = 0;

   if ( max != 0 )
      {
       y = (ulimit-llimit)/max;
      }
   else 
      {
       y = 0;
      }

   xxl = llimit;
   xxu = ulimit;
   nf0 = 1;

   if ((max <= 1) || (y == 0))
      {
       nrt     = 2;
       val[1] = xxl;
       *nret = nrt;
       return;
      }

   if (y < ZERO )
      {
       xxu = llimit;
       xxl = ulimit;
       y   = -y;  
      }

   cy  = log(y)/XCON;
   mul = cy;  
   
   if (cy < ONE)
      {
       mul = mul - 1;
      }

   if (xxl > ZERO)
      {
       xxl  = xxl/P4;
       ijof = 0;
       
       if (max == 3)
          {
           nf0 = 0;
          }
      }
   else if (xxl == ZERO)
      {
       ijof = 0;
      }
   else 
      {
       xxl = xxl * P4;
       ijof = 1;
      }
   
   if (xxu > ZERO)
      {
       xxu  = xxu*P4;
       ikof = 1;
      }
   else if (xxu == ZERO)
      {
       ikof = 0;
      }
   else 
      {
       xxu = xxu/P4;
       ikof = 0;
      }

   del = 0;

   done = FALSE;

   for (j = 0; j < max; j++)
      {
       for (i = 0; i < 4; i++)
          {
           del  = xncr[i] * pow(TEN, (double)mul);
           if (del == 0)  /* prevent a divide by 0 error */
              {
               ij = -ijof;
               ik =  ikof;
              }
           else
              {
               ij   = xxl/del - ijof;
               ik   = xxu/del + ikof;
              }

           nrt = ik - ij;
    
           if ((nrt+nf0) <= max)
              {
               done = TRUE;
               break;            /* break out of the inner loop */ 
               }
          }

       if (done == TRUE) break;  /* break out of the outer loop */ 

       mul++;

      }

   if (nrt <= 0)
     {
      nrt = 2;
      val[0] = llimit;
      val[1] = ulimit;
      *nret = nrt;
      return;
     }

   if ( i == 2 ) 
      {
       mul--;
      }

   val[0] = ij * del;

   for (j = 0; j < nrt ; j++)
      {
       val[j+1] = val[j] + del;
      }

   nexp = mul;

   (nrt)++;
   *nret = nrt;
}   


void getscale(double llimit, double ulimit,int max, char lineflag, 
              char logflag, double *retvals, int *nret)
{   

  /*  This routine generates a linear, nice linear or log scale.  */
  /*  Variables:                                   */
  /*    llimit   - lower data limit                */
  /*    ulimit   - upper data limit                */
  /*    max      - maximum numbers allowed         */
  /*    val[max] - returned array of nice numbers  */
  /*    nret     - number of returned values       */
  /*    tempf    - 255.0/(max - min)               */

  int i, nrt;
  double df;

   /*  Determine scale values.  */
   nrt = max + 1;
   df = (ulimit - llimit) / (float)max;
   for (i = 0; i < nrt-1; i++)
      retvals[i] = llimit + (df * i);
   retvals[nrt-1] = ulimit;

   if (logflag)
     {
      nicelog(llimit,ulimit,max,retvals,&nrt);
     }

   if (lineflag && !logflag)
     {
      nicelin(llimit,ulimit,max,retvals,&nrt);
     }

   *nret = nrt;
}


void fldrange(int ifld, int itype, char lineflag, char logflag)
{   

  /*  This routine determines and sets the field min and max   */
  /*  for display purposes depending on scale type requested.  */
  /*  Variables:                                   */
  /*    ifld     - current active field            */
  /*    itype    - field data type:                */
  /*               0 = node data                   */
  /*               1 = cell data                   */
  /*               2 = surface data                */
  /*               3 = tracer data                 */
  /*               4 = ray data                    */

  int i, nrt, max;
  double df, llimit, ulimit, retvals[100];

   max = nset_contours;
 
   /*  Get initial min and max, depending on type.  */
   if (itype == 0)
     {
      llimit = globfieldmin[ifld];
      ulimit = globfieldmax[ifld];
     }
   else if (itype == 1)
     {
      llimit = globfieldcmin[ifld];
      ulimit = globfieldcmax[ifld];
     }
   else if (itype == 2)
     {
      llimit = globfieldsmin[ifld];
      ulimit = globfieldsmax[ifld]; 
     }
   else if (itype == 3)
     {
      llimit = globfieldtrmin[ifld];
      ulimit = globfieldtrmax[ifld]; 
     }
   else if (itype == 4)
     {
      llimit = globfieldrmin[ifld];
      ulimit = globfieldrmax[ifld]; 
     }

   /*  Determine scale values.  */
   nrt = max + 1;
   df = (ulimit - llimit) / (float)max;
   for (i = 0; i < nrt-1; i++)
      retvals[i] = llimit + (df * i);
   retvals[nrt-1] = ulimit;

   if (logflag)
     {
      nicelog(llimit,ulimit,max,retvals,&nrt);
      llimit = retvals[0];
      ulimit = retvals[nrt-1];
     }

   if (lineflag && !logflag)
     {
      nicelin(llimit,ulimit,max,retvals,&nrt);
      llimit = retvals[0];
      ulimit = retvals[nrt-1];
     }

   /*  Reset nice min and max data, depending on type.  */
   if (lineflag || logflag)
     {
      if (itype == 0)
        {
         nfld_nice_min[ifld] = llimit;
         nfld_nice_max[ifld] = ulimit;
        }
      if (itype == 1)
        {
         cfld_nice_min[ifld] = llimit;
         cfld_nice_max[ifld] = ulimit;
        }
      if (itype == 2)
        {
         sfld_nice_min[ifld] = llimit;
         sfld_nice_max[ifld] = ulimit;
        }
      if (itype == 3)
        {
         trfld_nice_min[ifld] = llimit;
         trfld_nice_max[ifld] = ulimit;
        }
      if (itype == 4)
        {
         rfld_nice_min[ifld] = llimit;
         rfld_nice_max[ifld] = ulimit;
        }
     }
}
