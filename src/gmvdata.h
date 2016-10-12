#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

#define MAXMATS 1000
#define MAXFIELDS 500
#define MAXCONTOURS 20
#define MAXFLAGS 50
#define MAXFLDCALC 10
#define MAXTRHIST 250
#define MAXTRFIELDS 500
#define MAXNUMSEL 50
#define MAXGROUPS 1000
#define MAXRAYFIELDS 20

#define FREE(a) { if (a) free(a); a = NULL; }

EXTERN short physedflag, gmvflag, gridonlyflag, gmvinflag,
             exodusinflag, x3dinflag, noprintflag, shmemflag;
EXTERN int   nnodes, ncells, mmats, maxmatno, maxcellmat, numvars, 
             ntracers, numvartr, numflags_node, numflags_cell, 
             cnumvars, ncelledges, ncellfaces, ntracehist, nodevarsin,
             nset_contours, tracehiststop, tracehiststride, nsurf,
             snumvars, numflags_surf, maxsurfmat, fnumvars, nrays,
             rnumvars, ncell2dfaces, n_exo_time_steps, exo_time_step;
struct node
         {
          double  x;
          double  y;
          double  z;
         };
EXTERN struct node *nodes;
struct cell
         {
          short   nfaces;
          short   nedges;
          short   ncnodes;
          double  xavg;
          double  yavg;
          double  zavg;
          int     *facenos;   
          int     *edgenos;
          int     *cnodes;
         };
EXTERN struct cell *cells;
struct celledge
         {
          int  edgept1;
          int  edgept2;
         };
EXTERN struct celledge *celledges;
struct cellface
        {
         int  nverts;
         int  *fverts;
         int  cell1;
         int  cell2;
         int  oppface;
        };
EXTERN struct cellface *cellfaces;
struct surface
         {
          int     nverts;
          double  xavg;
          double  yavg;
          double  zavg;
          float   xnorm;
          float   ynorm;
          float   znorm;
          int     *fverts;
         };
EXTERN struct surface *surfs;
struct polystruct
         {
          short   nverts;
          long    polyvertloc;
          float   xnorm;
          float   ynorm;
          float   znorm;
          double  xavg;
          double  yavg;
          double  zavg;
          double  maxradius;
         };
EXTERN struct polystruct *polygons[MAXMATS];
struct polyvert
         {
          double  x;
          double  y;
          double  z;
         };
EXTERN struct polyvert *polyverts[MAXMATS];
struct Units
         {
          char  xyz[17];
          char  uvw[17];
          int   nnodes;
          char  nfldname[MAXFIELDS][33];
          char  nfldunit[MAXFIELDS][17];
          int   ncells;
          char  cfldname[MAXFIELDS][33];
          char  cfldunit[MAXFIELDS][17];
          int   nsurfs;
          char  sfldname[MAXFIELDS][33];
          char  sfldunit[MAXFIELDS][17];
         };
EXTERN struct Units units;
EXTERN short *imat, flagtypes_node[MAXFLAGS], *flags[MAXFLAGS],
             *cellmats, *cellflags[MAXFLAGS],
             flagtypes_cell[MAXFLAGS], *surfmats, *surfflags[MAXFLAGS],
             flagtypes_surf[MAXFLAGS];
EXTERN char *polydrawchk[MAXMATS], *nodeselectflag, *cellselectflag,
            *gridselectflag, *nodesubsetflag, *cellsubsetflag,
            *traceselectflag, *surfselectflag, *surfsubsetflag, *facesel,
            *surfselectflagp, local_idflag, *rayselectflag;
EXTERN char  *mmatnames[MAXMATS], *fieldname[MAXFIELDS],
             *flagname_node[MAXFLAGS], *fieldtrname[MAXFIELDS],
             *flagnames_node[MAXFLAGS][MAXMATS],
             *flagname_cell[MAXFLAGS],
             *flagnames_cell[MAXFLAGS][MAXMATS],
             *cfieldname[MAXFIELDS], *ffieldname[MAXFIELDS],
             *flagname_surf[MAXFLAGS],
             *flagnames_surf[MAXFLAGS][MAXMATS],
             *sfieldname[MAXFIELDS], *cellgrpname[MAXGROUPS],
             *nodegrpname[MAXGROUPS], *facegrpname[MAXGROUPS],
             *surfacegrpname[MAXGROUPS], *rfieldname[MAXRAYFIELDS];
EXTERN int npolyspermat[MAXMATS], vectors, polysin, cvectors, svectors,
           fvectors;
EXTERN long npolyvertspermat[MAXMATS];
EXTERN double *nodefield, *cellfield, *gridfield, ptime, *surffield;
EXTERN short mat_has_active_pts[MAXMATS], active_mat[MAXMATS], 
             mat_has_active_polys[MAXMATS];
EXTERN float matexpx[MAXMATS], matexpy[MAXMATS], matexpz[MAXMATS],
             cellexpx[MAXMATS],cellexpy[MAXMATS],cellexpz[MAXMATS],
             surfexpx[MAXMATS],surfexpy[MAXMATS],surfexpz[MAXMATS];
EXTERN double *uic, *vic, *wic, *cuic, *cvic, *cwic, *suic, *svic, *swic,
              *fuic, *fvic, *fwic;
EXTERN long xorig, yorig, xsize, ysize;
EXTERN double fieldmin[MAXFIELDS], fieldmax[MAXFIELDS],
              fieldtrmin[MAXTRFIELDS], fieldtrmax[MAXTRFIELDS],
              cfieldmin[MAXFIELDS], cfieldmax[MAXFIELDS],
              sfieldmin[MAXFIELDS], sfieldmax[MAXFIELDS],
              ffieldmin[MAXFIELDS], ffieldmax[MAXFIELDS],
              rfieldmin[MAXRAYFIELDS], rfieldmax[MAXRAYFIELDS];
EXTERN int ncellgrps, nnodegrps, nfacegrps, nsurfacegrps,
           numcellgrp[MAXGROUPS], *cellgrps[MAXGROUPS],
           numnodegrp[MAXGROUPS], *nodegrps[MAXGROUPS],
           numfacegrp[MAXGROUPS], *facegrps[MAXGROUPS],
           numsurfacegrp[MAXGROUPS], *surfacegrps[MAXGROUPS];
EXTERN float xrot, yrot, zrot, magnify;
EXTERN short axisflag, linesflag, polysflag, nodesflag, cellsflag,
             celledgesflag, timeflag, cycleflag, nodenumflag,
             cellnumflag, ganodenumflag, gacellnumflag, gridanalflag,
             colorbarflag, colorbarpref, boundingboxflag,
             gridedgesflag, gridsflag, viewflag, structuredflag,
             cellcontflag, gridcontflag, bboxcoordflag, node0matflag,
             cell0matflag, tracehistflag, cellshadeflag, facerefineflag,
             cellmedianedgeflag, gridmedianflag, gridmedianedgeflag,
             cellnodenumflag, polystiplflag, struct2dflag, tracehistpoints,
             surfflag, surfcontflag, surfshadeflag, surfrefineflag, 
             surfedgesflag, surfnumflag, surfnodenumflag, surf0matflag,
             cellnormflag, gridvoronoiflag, gridvoronoiedgeflag,
             drawnew3dflag, distscaleflag, datalimpopflag, textureflag,
             cellfacenumflag, gafacenumflag, raysflag, drawrayflag,
             mesh2dflag, drawghosts, drawghostface, drawnodeghosts,
             drawunselnodes, drawunselcells, cellhiddenedgesflag,
             distscalesetflag, o_axisflag, o_axissetflag;
EXTERN short snapshotsize, cellselmat_on, cellselcfld_on, 
             cellselnfld_on, cellselnfldopt, cellselsph_on,
             cellselsph_in, cellselbox_on, cellselbox_in,
             cellselnum_on, ncellselnum,  nodeselmat_on,
             nodeselfld_on, nodeselsph_on, nodeselsph_in,
             nodeselbox_on, nodeselbox_in, nodeselnum_on,
             nnodeselnum, ntraceselnum,
             nodeselbox_draw, cellselbox_draw, surfselmat_on, 
             surfselsfld_on, surfselnfld_on, surfselnfldopt, surfselsph_on,
             surfselsph_in, surfselbox_on, surfselbox_in, subsetbox_draw,
             surfselnum_on, nsurfselnum, surfselbox_draw, polyselbox_draw,
             cellselcellgrp_on, cellselfacegrp_on, nodeselnodegrp_on,
             surfselsurfgrp_on, surfselprobe_on, nodeselgrp_in, 
             cellselgrp_in, faceselgrp_in, surfselgrp_in, traceselbox_draw,
             traceselfld_on, traceselsph_on, traceselsph_in,
             traceselbox_on, traceselbox_in, traceselnum_on,
             rayselnum_on, nrayselnum, jpegflag;
EXTERN float textures[768];
EXTERN unsigned long jcolor[256], backgroundcolor, matcolor[MAXMATS],
                     txtcolor;
EXTERN int nodefieldactive, xreflect, yreflect, zreflect,
           cellfieldactive, cellexpflag, cycleno,
           tracefieldactive, gridfieldactive, surffieldactive,
           surfexpflag, rayfieldactive;
EXTERN double cellsxmin,cellsxmax, cellsymin,cellsymax, cellszmin,cellszmax;
EXTERN double allxmin,allxmax, allymin,allymax, allzmin,allzmax;
EXTERN double globxicmin,globxicmax, globyicmin,globyicmax,
              globzicmin,globzicmax;
EXTERN double globxicmino,globxicmaxo ,globyicmino,globyicmaxo,
              globzicmino,globzicmaxo;
EXTERN double probxicmin,probxicmax, probyicmin,probyicmax,
              probzicmin, probzicmax;
EXTERN double globfieldmin[MAXFIELDS],globfieldmax[MAXFIELDS],
              globfieldtrmin[MAXTRFIELDS],globfieldtrmax[MAXTRFIELDS],
              globfieldcmin[MAXFIELDS],globfieldcmax[MAXFIELDS],
              globfieldsmin[MAXFIELDS],globfieldsmax[MAXFIELDS],
              globfieldfmin[MAXFIELDS],globfieldfmax[MAXFIELDS],
              globfieldrmin[MAXRAYFIELDS],globfieldrmax[MAXRAYFIELDS];
EXTERN char nfld_nice[MAXFIELDS], cfld_nice[MAXFIELDS], sfld_nice[MAXFIELDS],
            ffld_nice[MAXFIELDS], trfld_nice[MAXTRFIELDS], 
            rfld_nice[MAXRAYFIELDS], nfld_log[MAXFIELDS], cfld_log[MAXFIELDS],
            sfld_log[MAXFIELDS], ffld_log[MAXFIELDS], trfld_log[MAXTRFIELDS], 
            rfld_log[MAXRAYFIELDS];
EXTERN double nfld_nice_min[MAXFIELDS], nfld_nice_max[MAXFIELDS],
              cfld_nice_min[MAXFIELDS], cfld_nice_max[MAXFIELDS],
              sfld_nice_min[MAXFIELDS], sfld_nice_max[MAXFIELDS],
              ffld_nice_min[MAXFIELDS], ffld_nice_max[MAXFIELDS],
              trfld_nice_min[MAXTRFIELDS], trfld_nice_max[MAXTRFIELDS],
              rfld_nice_min[MAXRAYFIELDS], rfld_nice_max[MAXRAYFIELDS];
EXTERN float xrange, yrange, zrange, span, aspect, xpan, ypan, zpan,
             xtrans, ytrans, ztrans, flypos[3];
EXTERN short rdgmvflag, rdgmvcount;
EXTERN char filnam[256], attribsfilnam[256], animflyfilnam[256],
            titlestr[80], titlestr2[80], snapshotfilnam[256], 
            animfilnam[256], infilnam[256], clrbartitlestr[100],
            matbarfilnam[256];
EXTERN float near_clip, far_clip;
EXTERN short fastdraw, readattributes, readattributes_found_width_height;
EXTERN short lightchanged, vectorflag, polysubsetflag,
             tracerflag, titleflag, cvectorflag, svectorflag, 
             fvectorflag, rayflag, rayvartype[MAXRAYFIELDS],
             colorbartitleflag;
EXTERN double xminfdval,xmaxfdval, yminfdval,ymaxfdval,
              zminfdval,zmaxfdval;
EXTERN double xminbdval,xmaxbdval, yminbdval,ymaxbdval,
              zminbdval,zmaxbdval;
EXTERN double xminbbval,xmaxbbval, yminbbval,ymaxbbval,
              zminbbval,zmaxbbval;
EXTERN double xminbbvalo,xmaxbbvalo, yminbbvalo,ymaxbbvalo,
              zminbbvalo,zmaxbbvalo;
EXTERN short movieflag, windowflag, snapshotflag,
             debugflag;
EXTERN short anim_snapshotsflag, animationflag, animvelflowflag, 
             animfly_snapflag, animationmenu_up, animflymenu_up;
EXTERN long windowxmin, windowymin, windowwidth, windowheight,
            windowxmax, windowymax;
EXTERN int nrefl_loops, anim_iterations, animation_count, drawincr,
           animfly_count;
EXTERN float xscale[8], yscale[8], zscale[8], xanimrot, yanimrot,
             zanimrot, xanimcen, yanimcen, zanimcen, animmag, velscale,
             velarrowsize;
EXTERN float  frontclipvalue, backclipvalue;
EXTERN double maxspeed, cmaxspeed, smaxspeed, fmaxspeed;
EXTERN int animfadematflag[MAXMATS],animfadecontflag[MAXCONTOURS];
EXTERN float xscaleaxis, yscaleaxis, zscaleaxis,
             xscaleaxiso, yscaleaxiso, zscaleaxiso;
EXTERN int mat_order[MAXMATS], nmat_order;
EXTERN float pexplodefact, cexplodefact, sexplodefact;
EXTERN char nodeselflag[MAXFLAGS+1][MAXMATS],
            nodeandorflag[MAXFLAGS+1],
            cellselflag[MAXFLAGS+1][MAXMATS],
            cellandorflag[MAXFLAGS+1],
            surfselflag[MAXFLAGS+1][MAXMATS],
            surfandorflag[MAXFLAGS+1];
EXTERN short vectfldx, vectfldy, vectfldz, speedfld,
             cvectfldx, cvectfldy, cvectfldz, cspeedfld,
             svectfldx, svectfldy, svectfldz, sspeedfld,
             fvectfldx, fvectfldy, fvectfldz, fspeedfld;
EXTERN int nxv, nyv, nzv;
struct fldcalc
         {
          short xfldno;
          short yfldno;
          short oper;
          float cval;
          char  name[33];
         };
EXTERN struct fldcalc fieldcalc[MAXFLDCALC];
EXTERN struct fldcalc cfieldcalc[MAXFLDCALC];
EXTERN struct fldcalc sfieldcalc[MAXFLDCALC];
EXTERN struct fldcalc tfieldcalc[MAXFLDCALC];
EXTERN short fldcalcbeg, cfldcalcbeg, fldcalc_has_data, cfldcalc_has_data,
             sfldcalcbeg, sfldcalc_has_data, tfldcalcbeg;
EXTERN short pnt_size, pnt_shape, errorfile, line_size, line_type,
             vectorarrow, vectorlog;
struct tracer
         {
          int    ntracers;
          double *x;
          double *y;
          double *z;
          int    numvar;
          double *field[MAXTRFIELDS];
         };
EXTERN struct tracer tracers[MAXTRHIST];

struct ray
         {
          int    npts;
          double *x;
          double *y;
          double *z;
          double *field[MAXRAYFIELDS];
         };
EXTERN struct ray *rays;

EXTERN short *cellgrpcolr, *nodegrpcolr, *surfgrpcolr, *facegrpcolr;
EXTERN char *cellfacedraw;
EXTERN float *facenormx, *facenormy, *facenormz;
EXTERN short nodeselfield, cellselfield, cellselnfield, surfselfield, 
             surfselnfield, traceselfield;
EXTERN float cellselsph_x, cellselsph_y, cellselsph_z, cellselsph_r,
             cellselbox_xmin, cellselbox_ymin, cellselbox_zmin,
             cellselbox_xmax, cellselbox_ymax, cellselbox_zmax,
             nodeselsph_x, nodeselsph_y, nodeselsph_z, nodeselsph_r,
             nodeselbox_xmin, nodeselbox_ymin, nodeselbox_zmin,
             nodeselbox_xmax, nodeselbox_ymax, nodeselbox_zmax,
             surfselsph_x, surfselsph_y, surfselsph_z, surfselsph_r,
             surfselbox_xmin, surfselbox_ymin, surfselbox_zmin,
             surfselbox_xmax, surfselbox_ymax, surfselbox_zmax,
             traceselsph_x, traceselsph_y, traceselsph_z, traceselsph_r,
             traceselbox_xmin, traceselbox_ymin, traceselbox_zmin,
             traceselbox_xmax, traceselbox_ymax, traceselbox_zmax;
EXTERN double nfldselmin, nfldselmax, cfldselmin, cfldselmax,
              cnfldselmin, cnfldselmax, sfldselmin, sfldselmax, 
              snfldselmin, snfldselmax, 
              traceselfields[MAXTRFIELDS],
              trfldselmin[MAXTRFIELDS], trfldselmax[MAXTRFIELDS];
EXTERN long cellselnum_f[MAXNUMSEL],  cellselnum_l[MAXNUMSEL], 
            cellselnum_s[MAXNUMSEL],  nodeselnum_f[MAXNUMSEL],  
            nodeselnum_l[MAXNUMSEL],  nodeselnum_s[MAXNUMSEL],
            surfselnum_f[MAXNUMSEL],  surfselnum_l[MAXNUMSEL], 
            surfselnum_s[MAXNUMSEL];
EXTERN int traceselnum_f[MAXNUMSEL], traceselnum_l[MAXNUMSEL],
           traceselnum_s[MAXNUMSEL];
EXTERN int rayselnum_f[MAXNUMSEL], rayselnum_l[MAXNUMSEL],
           rayselnum_s[MAXNUMSEL];
EXTERN short nfieldlimauto[MAXFIELDS], cfieldlimauto[MAXFIELDS],
             trfieldlimauto[MAXTRFIELDS], sfieldlimauto[MAXFIELDS],
             ffieldlimauto[MAXFIELDS], rfieldlimauto[MAXRAYFIELDS];
EXTERN short nodeidflag, cellidflag, faceidflag, traceridflag, surfidflag,
             rayidflag, cellghostflag, nodeghostflag, ghostcellflagid;
EXTERN int *nodeids, *cellids, *faceids, *tracerids, *surfids, *rayids,
           *cellpe, *cellnuminpe, *nodepe, *nodenuminpe, *surfpe, *surfnuminpe,
           ncellghosts, *cellghosts, nnodeghosts, *nodeghosts;

EXTERN int fontOffset, fontOffset2, axfontOffset, matfontOffset;
EXTERN int p_s_flag, poly_offset;
EXTERN short structured_in, mats_on_nodes, mats_on_cells, nvectin, cvectin,
             nvars_to_write[MAXFIELDS], cvars_to_write[MAXFIELDS], faces_in,
             svectin, svars_to_write[MAXFIELDS],
             fvectin, fvars_to_write[MAXFIELDS],
             nsubvarsin[MAXFIELDS], csubvarsin[MAXFIELDS], 
             fsubvarsin[MAXFIELDS];
EXTERN double *xin, *yin, *zin, myzero;
EXTERN int cellgrpselected[MAXGROUPS], ncellgrpselected,
           facegrpselected[MAXGROUPS], nfacegrpselected,
           nodegrpselected[MAXGROUPS], nnodegrpselected,
           surfgrpselected[MAXGROUPS], nsurfgrpselected;
EXTERN int cencellid, cennodeid, centracerid;
EXTERN int ncellsinpe[5], nnodesinpe[5];
EXTERN int distscale_x, distscale_y, jpeg_quality;
EXTERN short sort_trans;
EXTERN int o_axis_x, o_axis_y;

/*  Triangle drawing element structure.  */
struct trielem
         {
          int     itype;
          double  x1;
          double  y1;
          double  z1;
          double  x2;
          double  y2;
          double  z2;
          double  x3;
          double  y3;
          double  z3;
          float   xnorm;
          float   ynorm;
          float   znorm;
          int     iclr1;
          int     iclr2;
          int     iclr3;
         };
EXTERN struct trielem *drawarray;
EXTERN struct trielem *drawarray2;
EXTERN struct trielem *drawarray3;
EXTERN int numtris, numtris2, numtris3, drawarray_alloc, drawarray2_alloc,
           drawarray3_alloc;

/*  Triangle drawing types.  */
#define UNLIT_FLAT      0
#define UNLIT_SMOOTH    1
#define LIT_FLAT        2
#define LIT_SMOOTH      3
#define LIT_FLAT1SIDE   4
#define LIT_SMOOTH1SIDE 5

/*  Point, line, vector (plv) drawing element structure.  */
struct plvelem
         {
          int     itype;
          double  x1;
          double  y1;
          double  z1;
          double  x2;
          double  y2;
          double  z2;
          int     iclr1;
          int     iclr2;
         };
EXTERN struct plvelem *plvarray;
EXTERN struct plvelem *plvarray3;
EXTERN int numplv, plvarray_alloc, numplv3 , plvarray3_alloc;

/*  Point, line, vector drawing types.  */
#define PONLY      10
#define PANDNUM    11
#define NUMONLY    12
#define TPOINT     13
#define SPHERE     14
#define TNUMVAL    15
#define LSOLID     16
#define LSMOOTH    17
#define NVECTOR    18
#define CVECTOR    19
#define SVECTOR    20
#define FVECTOR    21
#define THIST      22
#define TSIZE      23
