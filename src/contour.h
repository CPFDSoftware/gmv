#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

EXTERN int contourfieldactive, cliponbox, cliponcellsel;
EXTERN int num_of_contour_polys[MAXCONTOURS+MAXMATS+2],
           contour_on[MAXCONTOURS+MAXMATS+2],
           contourfield_draw, mcontourfield_draw; 
struct isocont
         {
          float  vx[3];
          float  vy[3];
          float  vz[3];
          float  nx;
          float  ny;
          float  nz;
          float  fact[3];
          int    node1[3];
          int    node2[3];
         };
EXTERN struct isocont *contourlist[MAXCONTOURS+MAXMATS+2];
EXTERN float mconval[MAXCONTOURS+MAXMATS];
EXTERN int mconfield[MAXCONTOURS+MAXMATS];
EXTERN struct isocont *conlist;
EXTERN int isovol_on, num_of_isovol_polys, isovolfieldactive, 
       ivcliponbox, ivcliponcellsel, isovolfield_draw;
EXTERN float isovol_val1, isovol_val2;
EXTERN double *contourdrawfield, *mcontourdrawfield, *isovoldrawfield;
EXTERN short isomats_active[MAXMATS];
