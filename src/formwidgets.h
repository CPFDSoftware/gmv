
#ifndef HDATA_INIT
#define EXTERN extern
#else
#define EXTERN /**/
#endif

#define FIONUM 16
#ifdef HDATA_INIT
char  fspattern[FIONUM][80] = { {"*"}, {"*attr"}, {"*attr"}, {"*jpg"},
                                {"*jpg*"},  {"*jpg*"},  {"flightfile*"}, 
                                {"flightfile*"},  {"cutline*"}, {"*cmap"},
                                {"*"}, {"*"}, {"*ray*"}, {"*jpg"}, {"*query"},
				{"*vars"} };
short fsread[FIONUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
EXTERN char  fspattern[FIONUM][80];
EXTERN short fsread[FIONUM];
#endif

EXTERN Widget form, isosurfformf, subsetform, centerform, 
              animationform, cutplaneform, coloreditform,
              polysubsetform, snapshotform, scaleaxisform,
              matorderform, polygonform, nodeform, cellform,
              nodeselform, cellselform, cellexpform, traceform,
              gridanalform, plotboxform, errorform, msgform,
              fieldlimform, tracelimform, boundingboxform,
              animflyform, isosurfformm, cutlineform,
              fldcalcform, queryform, nfieldform, gmvmsgform,
              cfieldform, cfieldlimform, cfldcalcform,
              cutplaneoptform, isovolform, pntsizeform, autordform,
              stereoform, linesizeform, fcoloreditform,
              cutsphereform, backgrndform, vectctlform, surfform,
              sfieldform, sfieldlimform, sfldcalcform, surfselform, 
              surfexpform, numfilesform, rbzoomform, nfldavgform,
              cfldavgform, ffieldform, ffieldlimform, rayform,
              raylimform, concaveform, clrbartitleform, traceselform,
              trfieldform, matcolorbarform, distsclon, tfldcalcform;
 
EXTERN Widget objxrot, objyrot, objzrot, objxrottxt, objyrottxt,
              objzrottxt, objmagnify, objmagnifytxt, displistbut,
              rbzoombut, trackballbut, beepbut, texturebut;
EXTERN Widget obj2pts, objdist, objdistval, objaxes, objlight,
              objxrefl, objyrefl, objzrefl, o_axis_on;
EXTERN Widget file_sel, objerrlab, polysubset, psubset, gmvmsglab, file_sel2; 
EXTERN Widget colrbaron, colrpref0, colrpref1, colrpref2, colrpref3,
              colrpref4, colrpref5, colrpref6, colrpref7, colrpref8,
              colrpref9, colrtitl0, colrtitl1, view1, view2, view3, view4,
              view5, drawframe, numfilestxt, colrbarmat;
EXTERN Widget dispnodes, dispcells, disppolys, disptrace, calciso,
              calccutp, calcganal, fieldlimbut, tracelimbut, cfieldlimbut,
              calccuts, dispsurfs, sfieldlimbut, ffieldlimbut, dispray,
              raylimbut, nwrtvars, cwrtvars, twrtvars, swrtvars;
EXTERN Widget calccutl, ctln1pt, ctln2pt, calfcalc, calcisomats, calcisoflds, 
              calcquery, calcnfldcalc, calccfldcalc, calctfldcalc,
              calcisovol, calcsfldcalc, calcavgnfld, calcavgcfld,
              calcconcave;
EXTERN short autordflag, nprobeflag, cprobeflag, stereoflag, fileselflag,
             displistflag, drawnewlistflag, stereowindow, rbzoomflag,
             trackballflag, cennprobeflag, cencprobeflag, surfselprobopt,
             surfselpolyopt, cellmatselprobeflag, cellmatremprobeflag,
             surfmatselprobeflag, surfmatremprobeflag, polymatselprobeflag, 
             polymatremprobeflag, has_fullscreenstereo;
EXTERN float eyescale, convscale;
EXTERN char  fsdir[FIONUM][200];

EXTERN Widget PEcellfldeditform, PEcellfldselform, PEcellfldaddform, 
              PEnodefldeditform, PEnodefldselform, PEnodefldaddform,
              PEsurffldeditform, PEsurffldselform, PEsurffldaddform,
              PEcellgrpeditform, PEcellgrpselform, PEcellgrpaddform, 
              PEnodegrpeditform, PEnodegrpselform, PEnodegrpaddform,
              PEsurfgrpeditform, PEsurfgrpselform, PEsurfgrpaddform,
              PEcellflddelform, PEnodeflddelform, PEsurfflddelform,
              PEcellgrpdelform, PEnodegrpdelform, PEsurfgrpdelform,
              PEcellmatform, PEnodematform;
EXTERN int PEcfieldactive, PEnfieldactive, PEsfieldactive, PEffieldactive;
EXTERN int PEcgroupactive, PEngroupactive, PEsgroupactive, PEfgroupactive;
EXTERN unsigned int stwinw, stwinh;
EXTERN int nsurfpolyverts, surfpolyx[1000], surfpolyy[1000], surfpolyz[1000];
EXTERN float xrot_start, yrot_start, zrot_start, magnify_start;
EXTERN int num_navgmat, num_cavgmat;

#define YMAXSTEREO 491
#define YOFFSET 532

#define NFLDQUERY        0
#define NFLDVECTBLD      1
#define NFLDFLDLIM       2
#define NFLDNFIELD       3
#define NFLDCNFIELD      4
#define NFLDISOSURF      5
#define NFLDNCUTPLANE    6
#define NFLDGANFIELD     7
#define NFLDNCUTLINE     8
#define NFLDFLDCALCX     9
#define NFLDFLDCALCY    10
#define NFLDANIMISO     11
#define NFLDANIMCUTP    12
#define NFLDNODESEL     13
#define NFLDISOVOL      14
#define NFLDCELLSEL     15
#define NFLDISOVOLCLR   16
#define NFLDISOSURFCLR  17
#define NFLDISOSURFMCLR 18
#define NFLDNCUTSPHERE  19
#define NFLDANIMCUTS    20
#define NFLDPEFIELD     21
#define NFLDPEFIELDCP   22
#define NFLDSNFIELD     23
#define NFLDSURFSEL     24
#define NFLDAVG         25

#define CFLDQUERY      100
#define CFLDVECTBLD    101
#define CFLDFLDLIM     102
#define CFLDCCFIELD    103
#define CFLDCCUTPLANE  104
#define CFLDGACFIELD   105
#define CFLDCCUTLINE   106
#define CFLDFLDCALCX   108
#define CFLDFLDCALCY   109
#define CFLDANIMCUTP   110
#define CFLDCELLSEL    111
#define CFLDCCUTSPHERE 112
#define CFLDANIMCUTS   113
#define CFLDPEFIELD    114
#define CFLDPEFIELDCP  115
#define CFLDPEFIELDDEL 116
#define CFLDAVG        117

#define SFLDVECTBLD    200
#define SFLDFLDLIM     201
#define SFLDSSFIELD    202
#define SFLDFLDCALCX   203
#define SFLDFLDCALCY   204
#define SFLDSURFSEL    205
#define SFLDPEFIELD    206
#define SFLDPEFIELDCP  207
#define SFLDPEFIELDDEL 208

#define FFLDVECTBLD    300
#define FFLDFLDLIM     301
#define FFLDCFFIELD    302
#define FFLDGAFFIELD   303
#define FFLDPEFIELD    304
#define FFLDPEFIELDCP  305
#define FFLDPEFIELDDEL 306

#define TRFLDNFIELD    307
#define TRFLDFLDLIM    308
#define TRFLDNODESEL   309
#define TRFLDFLDCALCX  310
#define TRFLDFLDCALCY  311

#define GMVINPUT     0
#define GETATTR      1
#define PUTATTR      2
#define SNAPSHOT     3
#define ANIMSNAP     4
#define ANIMFLYSNAP  5
#define ANIMFLYSAV   6
#define ANIMFLYGET   7
#define CUTLINEOUT   8
#define COLORMAPIN   9
#define GMVOUTPUT   10
#define SDMOUTPUT   11
#define GMVRAY      12
#define MATBARSNAP  13
#define QUERYOUT    14
#define WRITEVARS   15

#define DLISTALL 1
#define DLISTINT 2
