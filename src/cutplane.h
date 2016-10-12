#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

#define MAXCUTPLANES 5
#define MAXCUTSPHERES 5
#define MAXCUTLINES 20

EXTERN int cutplanefieldactive, cutplanefieldforplane;
EXTERN int cutspherefieldactive, cutspherefieldforsphere;
EXTERN short cutplane_on[MAXCUTPLANES+MAXCUTSPHERES+2], currcp;
struct cutpln
        {
         float  aa;
         float  bb;
         float  cc;
         float  dd;
         float  norm[3];
         float  x1;
         float  y1;
         float  z1;
         float  x2;
         float  y2;
         float  z2;
         float  x3;
         float  y3;
         float  z3;
         float  heightfac;
         float  dist;
         short  cliponbox;
         short  cliponcellsel;
         short  heightflag;
         short  edgesflag;
         short  velflag;
         short  nodeflag;
         short  linesflag;
         short  nnumflag;
         short  cellflag;
         short  cvelflag;
         short  cnumflag;
         short  facesflag;
         short  *vertsperpoly;
         float  *polylist;
         int    npolys;
         int    ndnodes;
         int    *dnodes;
         int    ndcells;
         int    *dcells;
         float  *vect[3];
         float  *cvect[3];
        };
EXTERN struct cutpln cutplane[MAXCUTPLANES+MAXCUTSPHERES+2];

EXTERN int *vpcutpoly;
EXTERN float *cutlist, *cutval;

EXTERN short cutlineon[MAXCUTLINES], waveflag[MAXCUTLINES],
             cutlinenodeson[MAXCUTLINES], 
             cutlinenumson[MAXCUTLINES];
EXTERN int ncutlinepts[MAXCUTLINES], cutlinefieldactive, 
           cutlinefield[MAXCUTLINES], *cutlinenodes[MAXCUTLINES],
           *cutnodes, *cutlinenodesm[MAXCUTLINES];
EXTERN float ctlinx1[MAXCUTLINES], ctliny1[MAXCUTLINES], 
             ctlinz1[MAXCUTLINES], ctlinx2[MAXCUTLINES],
             ctliny2[MAXCUTLINES], ctlinz2[MAXCUTLINES],
             *cutlineval[MAXCUTLINES], *cutlinedist[MAXCUTLINES],
             wavefac[MAXCUTLINES], cutlineradius[MAXCUTLINES],
             clminradius, clmaxradius, *maxcellradius, *maxsurfradius,
             *cutlinenodesx[MAXCUTLINES], *cutlinenodesy[MAXCUTLINES], 
             *cutlinenodesz[MAXCUTLINES];

