
#ifdef NT
#include <windows.h>
#endif

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Protocols.h>
#include <Xm/TextF.h>
#include <Xm/DialogS.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include <stdlib.h>

#include "GLwMDrawA.h"
#include "gmvdata.h"
#include "lights.h"
#include "xstuff.h"
#include "formwidgets.h"
#include "contour.h"
#include "cutplane.h"

#define MYPI 3.14159265358979323846

/* Locates the shell that contains a given widget */
#define GETSHELL(w,s)  { for(w = s; \
                             !XmIsDialogShell(w) && !XtIsTopLevelShell(w); \
                             w = XtParent(w)); \
                       }
 
extern GLXContext glx_glw_context, glx_axglw_context, glx_stglw_context;
extern GLboolean    doubleBuffer;

/*  Virtual Trackball variables.  */
enum QuatPart {X, Y, Z, W, QuatLen};
typedef struct {float x, y, z, w;} Quat;
typedef Quat HVect;
typedef float HMatrix[QuatLen][QuatLen];
typedef enum AxisSet{NoAxes, CameraAxes, BodyAxes, OtherAxes, NSets} AxisSet;
typedef float *ConstraintSet;
extern Quat qOne;
 
typedef struct
   {
    HVect center;
    double radius;
    Quat qNow, qDown, qDrag;
    HVect vNow, vDown, vFrom, vTo, vrFrom, vrTo;
    HMatrix mNow, mDown;
    Bool showResult, dragging;
    ConstraintSet sets[NSets];
    int setSizes[NSets];
    AxisSet axisSet;
    int axisIndex;
   } BallData;
 
static BallData ball;
GLfloat Ballmatrix[4][4] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
extern Quat qOne;
HVect vNow;
int interactivemouse = 0;

XColor get_colors[256];

int omx = 0, mx = 0, omy = 0, my = 0, opnx = 0, pnx = 0, opny= 0,
    pny = 0, oscl = 1000, scl = 1000,  mode = 0, 
    rbx1 = 0, rbx2 = 0, rby1 = 0, rby2 = 0, spx, spy;
float rbwx1, rbwx2, rbwy1, rbwy2, rbwz1, rbwz2;
int ncrosspts, olxpix = 50, olypix = 50, opxpix = 50, opypix = 50;
char dot[] = {2,3};
int gmv_data_in;
short lastviewflag = -1, cp2ptflag = 0, distanceflag = 0,
      valueflag = 0, cl1ptflag = 0, cl2ptflag = 0, noresizeflag = 0;
char save_dir[200];

/* Modes the program can be in */
#define NOTHING 0
#define ORIENT 1
#define CROSSHAIR 2
#define RBZOOM 3
#define POLY 4

extern float fclip, bclip;

void MyManageChild(Widget w);

/*  Variables needed to save attributes info.  */
static float sav_xrange, sav_yrange, sav_zrange, sav_span, sav_velscale,
             sav_velarrowsize, sav_xtrans, sav_ytrans, sav_ztrans, 
             sav_cfldselmin, sav_cfldselmax, sav_cnfldselmin, 
             sav_cnfldselmax, sav_nfldselmin, sav_nfldselmax,
             sav_cellselsph_x, sav_cellselsph_y, sav_cellselsph_z,
             sav_cellselsph_r, sav_cellselbox_xmin, sav_cellselbox_xmax,
             sav_cellselbox_ymin, sav_cellselbox_ymax,
             sav_cellselbox_zmin, sav_cellselbox_zmax,
             sav_nodeselsph_x, sav_nodeselsph_y,  sav_nodeselsph_z,
             sav_nodeselsph_r, sav_nodeselbox_xmin, sav_nodeselbox_xmax,
             sav_nodeselbox_ymin, sav_nodeselbox_ymax,
             sav_nodeselbox_zmin, sav_nodeselbox_zmax,
             sav_cutlineradius[MAXCUTLINES], sav_sfldselmin, 
             sav_sfldselmax, sav_snfldselmin,  sav_snfldselmax,            
             sav_surfselsph_x, sav_surfselsph_y, sav_surfselsph_z,
             sav_surfselsph_r, sav_surfselbox_xmin, sav_surfselbox_xmax,
             sav_surfselbox_ymin, sav_surfselbox_ymax,
             sav_surfselbox_zmin, sav_surfselbox_zmax,
             sav_traceselfields[MAXTRFIELDS],
             sav_trfldselmin[MAXTRFIELDS], sav_trfldselmax[MAXTRFIELDS], 
             sav_traceselsph_x, sav_traceselsph_y, sav_traceselsph_z,
             sav_traceselsph_r, sav_traceselbox_xmin, sav_traceselbox_xmax,
             sav_traceselbox_ymin, sav_traceselbox_ymax,
             sav_traceselbox_zmin, sav_traceselbox_zmax;
static double sav_globxicmin, sav_globxicmax, sav_globyicmin, sav_globyicmax, 
              sav_globzicmin, sav_globzicmax, sav_globxicmino, 
              sav_globxicmaxo, sav_globyicmino, sav_globyicmaxo, 
              sav_globzicmino, sav_globzicmaxo, sav_globfieldmin[MAXFIELDS],
              sav_globfieldmax[MAXFIELDS], sav_globfieldtrmin[MAXTRFIELDS],
              sav_globfieldtrmax[MAXTRFIELDS], sav_globfieldcmin[MAXFIELDS],
              sav_globfieldcmax[MAXFIELDS], sav_xminbdval, sav_yminbdval,
              sav_zminbdval, sav_xmaxbdval, sav_ymaxbdval, sav_zmaxbdval,
              sav_xminbbval, sav_yminbbval, sav_zminbbval, sav_xmaxbbval,
              sav_ymaxbbval, sav_zmaxbbval, sav_xminbbvalo, sav_yminbbvalo,
              sav_zminbbvalo, sav_xmaxbbvalo, sav_ymaxbbvalo, sav_zmaxbbvalo,
              sav_xminfdval, sav_yminfdval, sav_zminfdval, sav_xmaxfdval, 
              sav_ymaxfdval, sav_zmaxfdval, sav_maxspeed, sav_cmaxspeed,
              sav_nfld_nice_min[MAXFIELDS], sav_nfld_nice_max[MAXFIELDS],
              sav_cfld_nice_min[MAXFIELDS], sav_cfld_nice_max[MAXFIELDS],
              sav_sfld_nice_min[MAXFIELDS], sav_sfld_nice_max[MAXFIELDS],
              sav_ffld_nice_min[MAXFIELDS], sav_ffld_nice_max[MAXFIELDS],
              sav_trfld_nice_min[MAXTRFIELDS], sav_trfld_nice_max[MAXTRFIELDS],
              sav_fmaxspeed;
static int sav_active_mat[MAXMATS], sav_nmat_order, sav_mat_order[MAXMATS],
           sav_vectfldx, sav_vectfldy, sav_vectfldz, sav_speedfld, 
           sav_cvectfldx, sav_cvectfldy, sav_cvectfldz, sav_cspeedfld,
           sav_fvectfldx, sav_fvectfldy, sav_fvectfldz, sav_fspeedfld,
           sav_svectfldx, sav_svectfldy, sav_svectfldz, sav_sspeedfld,
           sav_contourfield_draw, sav_mcontourfield_draw, 
           sav_isovolfield_draw;
static short sav_polysflag, sav_linesflag, sav_celledgesflag, sav_nodesflag, 
             sav_vectorflag, sav_cvectorflag, sav_cellselmat_on, 
             sav_cellselcfld_on, sav_cellselnfld_on, sav_cellselnfldopt,
             sav_cellselsph_on, sav_cellselsph_in, sav_cellselbox_on,
             sav_cellselbox_in, sav_cellselfield, sav_cellselnfield,
             sav_cellselnum_on, sav_ncellselnum, sav_cellsflag,
             sav_nodeselmat_on, sav_nodeselfld_on, sav_nodeselsph_on, 
             sav_nodeselsph_in, sav_nodeselbox_on, sav_nodeselbox_in,
             sav_nodeselfield, sav_nodeselnum_on, sav_nnodeselnum,
             sav_traceselnum_on, sav_ntraceselnum, sav_timeflag,
             sav_cycleflag, sav_nfieldlimauto[MAXFIELDS],
             sav_cfieldlimauto[MAXFIELDS], sav_trfieldlimauto[MAXTRFIELDS],
             sav_svectorflag,sav_surfselmat_on,  sav_surfselsfld_on,
             sav_surfselnfld_on, sav_surfselnfldopt, sav_surfselsph_on,
             sav_surfselsph_in, sav_surfselbox_on, sav_surfselbox_in,
             sav_surfselfield, sav_surfselnfield, sav_surfselnum_on, 
             sav_nsurfselnum, sav_sfieldlimauto[MAXFIELDS],
             sav_ffieldlimauto[MAXFIELDS], sav_fvectorflag,
             sav_nodeselnodegrp_on, sav_nnodegrpselected, sav_nodeselgrp_in,
             sav_cellselcellgrp_on, sav_ncellgrpselected, sav_cellselgrp_in,
             sav_cellselfacegrp_on, sav_nfacegrpselected, sav_faceselgrp_in,
             sav_surfselsurfgrp_on, sav_nsurfgrpselected, sav_surfselgrp_in,
             sav_drawghosts, sav_drawghostface, sav_drawnodeghosts, 
             sav_cellhiddenedgesflag, sav_timeflag,
             sav_traceselfld_on, sav_traceselsph_on, sav_traceselsph_in,
sav_traceselbox_on, sav_traceselbox_in, sav_traceselfield,
             sav_traceselnum_on, sav_ntraceselnum;
          
static char sav_nodeselflag[MAXFLAGS+1][MAXMATS], 
            sav_nodeandorflag[MAXFLAGS+1],
            sav_cellselflag[MAXFLAGS+1][MAXMATS], 
            sav_cellandorflag[MAXFLAGS+1],
            sav_surfselflag[MAXFLAGS+1][MAXMATS], 
            sav_surfandorflag[MAXFLAGS+1],
            sav_nfld_nice[MAXFIELDS], sav_nfld_log[MAXFIELDS],
            sav_cfld_nice[MAXFIELDS], sav_cfld_log[MAXFIELDS],
            sav_sfld_nice[MAXFIELDS], sav_sfld_log[MAXFIELDS],
            sav_ffld_nice[MAXFIELDS], sav_ffld_log[MAXFIELDS],
            sav_trfld_nice[MAXTRFIELDS], sav_trfld_log[MAXTRFIELDS];
static struct fldcalc sav_fieldcalc[MAXFLDCALC];
static struct fldcalc sav_cfieldcalc[MAXFLDCALC];
static struct fldcalc sav_sfieldcalc[MAXFLDCALC];

void setangles(), setmag();



void rdgmv(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   rdgmvflag = (long int)client_data;

   fileselflag = GMVINPUT;

   MyManageChild(file_sel);
}


void getfilenameg(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *fname, *pattern, *dir;
  void getfile(int inpfileflag);

   XmFileSelectionBoxCallbackStruct *fcb = 
      (XmFileSelectionBoxCallbackStruct *) call_data;
   XmStringGetLtoR(fcb->value,XmSTRING_DEFAULT_CHARSET,&fname);
   XmStringGetLtoR(fcb->pattern,XmSTRING_DEFAULT_CHARSET,&pattern);
   XmStringGetLtoR(fcb->dir,XmSTRING_DEFAULT_CHARSET,&dir);
   XtUnmanageChild(w);
   XFlush(dpy);

   strcpy(filnam,fname);
   strcpy(save_dir,dir);
   strcpy(fspattern[GMVINPUT],pattern);
   getfile(0);
}


void getfilename(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *fname, *pattern, *dir;

   XmFileSelectionBoxCallbackStruct *fcb = 
      (XmFileSelectionBoxCallbackStruct *) call_data;
   XmStringGetLtoR(fcb->value,XmSTRING_DEFAULT_CHARSET,&fname);
   XmStringGetLtoR(fcb->pattern,XmSTRING_DEFAULT_CHARSET,&pattern);
   XmStringGetLtoR(fcb->dir,XmSTRING_DEFAULT_CHARSET,&dir);
   XtUnmanageChild(w);
   XFlush(dpy);

   switch(fileselflag)
     {

      case(GMVINPUT):  /*  GMV input file.  */
/*
         strcpy(filnam,fname);
         strcpy(fspattern[GMVINPUT],pattern);
         strcpy(fsdir[GMVINPUT],dir);
         getfile(0);
         fsread[GMVINPUT] = 1;
*/
         break;

      case(GETATTR):  /*  Get attributes file.  */
         strcpy(attribsfilnam,fname);
         strcpy(fspattern[GETATTR],pattern);
         strcpy(fsdir[GETATTR],dir);
         fsread[GETATTR] = 1;
         XDefineCursor(dpy,topwin,waitcsr);

         getattributes();

         XUndefineCursor(dpy,topwin);
         if (displistflag) drawnewlistflag = 1;
         drawnew3dflag = 1;
         fastdraw = 0;
         drawit();
   
         break;

      case(PUTATTR):  /*  Put attributes file.  */
         strcpy(attribsfilnam,fname);
         strcpy(fspattern[PUTATTR],pattern);
         strcpy(fsdir[PUTATTR],dir);
         fsread[PUTATTR] = 1;
         putattributes();
         break;

      case(SNAPSHOT):  /*  Save snapshot.  */
         strcpy(snapshotfilnam,fname);
         strcpy(fspattern[SNAPSHOT],pattern);
         strcpy(fsdir[SNAPSHOT],dir);
         fsread[SNAPSHOT] = 1;
         XFlush(dpy);
         glXWaitX();

         if (p_s_flag == 2) p_s_flag = 1;
         if (snapshotsize == 3) snapshotsize = 1;

         fastdraw = 0;
         drawit();

         fastdraw = 0;
         drawit();
         break;

      case(ANIMSNAP):  /*  Animation snapshot file prefix.  */
         strcpy(animfilnam,fname);
         strcpy(fspattern[ANIMSNAP],pattern);
         strcpy(fsdir[ANIMSNAP],dir);
         fsread[ANIMSNAP] = 1;
         XFlush(dpy);
         glXWaitX();
         animstart();
         break;

      case(ANIMFLYSNAP):  /*  Flight animation snapshot file prefix.  */
         strcpy(animflyfilnam,fname);
         strcpy(fspattern[ANIMFLYSNAP],pattern);
         strcpy(fsdir[ANIMFLYSNAP],dir);
         fsread[ANIMFLYSNAP] = 1;
         XFlush(dpy);
         glXWaitX();
         animflystrt();
         break;

      case(ANIMFLYSAV):  /*  Save flight animation control data.  */
         strcpy(fspattern[ANIMFLYSAV],pattern);
         strcpy(fsdir[ANIMFLYSAV],dir);
         fsread[ANIMFLYSAV] = 1;
         savflightfile(fname);
         break;

      case(ANIMFLYGET):  /*  Get flight animation control data.  */
         strcpy(fspattern[ANIMFLYGET],pattern);
         strcpy(fsdir[ANIMFLYGET],dir);
         getflightfile(fname);
         break;

      case(CUTLINEOUT):  /*  Save cutlines.  */
         strcpy(fspattern[CUTLINEOUT],pattern);
         strcpy(fsdir[CUTLINEOUT],dir);
         fsread[CUTLINEOUT] = 1;
         clwritefile(fname);
         break;

      case(COLORMAPIN):  /*  Read field colormap.  */
         strcpy(fspattern[COLORMAPIN],pattern);
         strcpy(fsdir[COLORMAPIN],dir);
         fsread[COLORMAPIN] = 1;
         readfcolormap(fname);
         break;

      case(GMVRAY):  /*  Read GMV RAY file.  */
         strcpy(fspattern[GMVRAY],pattern);
         strcpy(fsdir[GMVRAY],dir);
         fsread[GMVRAY] = 1;
         readgmvray(fname);
         break;

      case(MATBARSNAP):  /*  Material/Flag colorbar snapshot.  */
         strcpy(matbarfilnam,fname);
         strcpy(fspattern[MATBARSNAP],pattern);
         strcpy(fsdir[MATBARSNAP],dir);
         fsread[MATBARSNAP] = 1;
         XFlush(dpy);
         glXWaitX();
         drawmatcolorbar(0);
         XFlush(dpy);
         glXWaitX();
         drawmatcolorbar(1);
         break;

      case(QUERYOUT):  /*  Save query data.  */
         strcpy(fspattern[QUERYOUT],pattern);
         strcpy(fsdir[QUERYOUT],dir);
         fsread[QUERYOUT] = 1;
         qdwritefile(fname);
         break;

      case(WRITEVARS):  /*  Save query data.  */
         strcpy(fspattern[WRITEVARS],pattern);
         strcpy(fsdir[WRITEVARS],dir);
         fsread[WRITEVARS] = 1;
         varswritefile(fname);
         break;

#ifdef PHYSED
      case(GMVOUTPUT):  /*  GMV output file.  */
         strcpy(filnam,fname);
         strcpy(fspattern[GMVOUTPUT],pattern);
         strcpy(fsdir[GMVOUTPUT],dir);
         XDefineCursor(dpy,topwin,waitcsr);
         writegmv(filnam);
         fsread[GMVOUTPUT] = 1;
         XUndefineCursor(dpy,topwin);
         break;
#endif
     }

   XtFree(fname);
   XtFree(pattern);
   XtFree(dir);
   XFlush(dpy);
}


void getfile(int infileflag)
{
  int gmverr, exoerr, x3derr,  i, len, j;
  char tstchar;
  XmString string, string1, string2;
  char str[] = 
   "Error - this is not a GMV input file.\nTry another file.";
  char str2[] = 
   "Error - cannot open file: ";
  char str3[] = 
   "Error - gmvend not found.\nTry another file.";
  char str4[] = 
   "Error - this is not a valid GMV file.\nTry another file.";
  void readfile();

   /*  Check that the file is a valid GMV input file.  */
   gmverr = 1;  exoerr = 1;  x3derr = 1;
   gmvinflag = 0;  exodusinflag = 0;  x3dinflag = 0;
   gmverr = gmvread_checkfile(filnam);
   if (gmverr > 0)
      x3derr = x3dread_checkfile(filnam);
   if (gmverr > 0 && x3derr)
      exoerr = exoread_checkfile(filnam);
   if (gmverr > 0 && exoerr > 0 && x3derr > 0)
     {
      if (gmverr == 1 && exoerr == 1 && x3derr > 0)
        {
         errorfile = 0;
         string1 = XmStringLtoRCreate(str2,
                                     XmSTRING_DEFAULT_CHARSET);
         string2 = XmStringLtoRCreate(filnam,
                                     XmSTRING_DEFAULT_CHARSET);
         string = XmStringConcat(string1,string2);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         XmStringFree(string1);
         XmStringFree(string2);
         MyManageChild(errorform);
         return;
        }
      if (gmverr == 2)
        {
         errorfile = 0;
         string = XmStringLtoRCreate(str,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         MyManageChild(errorform);
         return;
        }
      if (gmverr == 3)
        {
         errorfile = 0;
         string = XmStringLtoRCreate(str3,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         MyManageChild(errorform);
         return;
        }
      if (gmverr > 3)
        {
         errorfile = 0;
         string = XmStringLtoRCreate(str4,
                                     XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
         XmStringFree(string);
         MyManageChild(errorform);
         return;
        }
     }

   /*  Set the input type flag.  */
   if (gmverr == 0) gmvinflag = 1;
   if (exoerr == 0) exodusinflag = 1;
   if (x3derr == 0) x3dinflag = 1;
 
   /*  Save input file name in case writegmv needs it later.  */
   strcpy(infilnam, filnam);

   /*  If the file was input with the command line option,  */
   /*  extract the directory path for the file menu.        */
   if (infileflag)
     {
      /*  Find the last /, then copy the directory name.  */
      len = strlen(filnam);
      for (i = 0; i < len; i++)
        {
         sscanf(&filnam[i], "%c", &tstchar);
         if (tstchar == '/') j = i;
        }
      strncpy(save_dir,filnam,j+1);
      string = XmStringLtoRCreate(filnam,XmSTRING_DEFAULT_CHARSET);
      string1 = XmStringLtoRCreate(save_dir,XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringLtoRCreate(fspattern[GMVINPUT], 
                                   XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel, XmNdirectory,string1, NULL);
      XtVaSetValues(file_sel, XmNdirSpec,string, NULL);
      XtVaSetValues(file_sel, XmNpattern,string2, NULL);
      XmStringFree(string);  XmStringFree(string1);
     }

   /*  If a good file, set the directory for all file types.  */
   for (i = 0; i < FIONUM; i++)
     {
      strcpy(fsdir[i],save_dir);
     }

   readfile();
}


void readfile()
{
  XmString string1, string2, msgstr;
  int i, j, matpixid, mr, mg, mb, fnsize;
  int *pes_selected, totalpes;
  char tmpfilnam[65];
  float redcol,greencol,bluecol;
  double xthick,ythick,zthick;
  unsigned long matpix;
  void free_user_mem(), del_widgets(), save_attr_data(),
        makewidgets(), get_attr_data();
  unsigned long getbestcolor(float r, float g, float b);
  void clminradcalc();

   XDefineCursor(dpy,topwin,waitcsr);

   fnsize = strlen(filnam);
   if (fnsize < 65) strcpy(tmpfilnam,filnam);
   else strcpy(tmpfilnam,filnam+(fnsize-64));

#ifdef PHYSED
   string1 = XmStringLtoRCreate("PHYSED reading file: ",
                               XmSTRING_DEFAULT_CHARSET);
#else
   string1 = XmStringLtoRCreate("GMV reading file: ",
                               XmSTRING_DEFAULT_CHARSET);
#endif
   string2 = XmStringLtoRCreate(tmpfilnam,
                                XmSTRING_DEFAULT_CHARSET);
   msgstr = XmStringConcat(string1,string2);
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(string1);
   XmStringFree(string2);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);

   /*  Clear display.  */
   gmv_data_in = 0; 

   XmUpdateDisplay(glw);
   XmUpdateDisplay(axglw);

   XFlush(dpy);
   XSync(dpy,0);

   /*  Increment read count.  */
   rdgmvcount++;

   /*  If Grid Analysis is active and reading an input file  */
   /*  for a different simulation, exit Grid Analysis.       */
   if (rdgmvcount > 1 && rdgmvflag == 0)
     {
      if (gridedgesflag == 1 || gacellnumflag == 1 || gridsflag == 1 ||  
          gridmedianflag == 1 || gridmedianedgeflag == 1)
         gridexit(NULL,NULL,NULL);
     }

   /*  If more than 1 file read and reading an input file  */
   /*  from the same simulation, save attributes data.     */
   if (rdgmvcount > 1 && rdgmvflag > 0) save_attr_data();

   /*  If more than 1 file read, free memory.  */
   if (rdgmvcount > 1) free_user_mem();

   /*  If more than 1 file read and reading an input     */
   /*  file for a different simulation, delete widgets.  */
   if (rdgmvcount > 1 && rdgmvflag == 0) del_widgets();

   XUndefineCursor(dpy,windowid);
   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);
   XSync(dpy,0);

   if (gmvinflag) readgmv();
   if (exodusinflag) readexodus();
   if (x3dinflag) readx3d();
   get_statistics();
   compute_boundary_triangle_unit_normals();

   /*  Calculate default cutline, query and search sphere radius.  */
   clminradcalc();

   /*  After first read, create material  */
   /*  and contour button colors.         */
   if (rdgmvcount == 1)
     {

      /*  If using Mesa colormap, get color array.  */
      if (mesacmap)
        {
         for (i = 0; i < 256; i++)
            get_colors[i].pixel = i;
         XQueryColors(dpy,my_cmap,get_colors,256);
        }

      /*  Create Motif popup menus and add material */
      /*  and contour colors to X color table.      */
      for (i = 0; i < MAXMATS; i++)
        {
         j = i * 4;
         mr = (int)(matrgba[0+j] * 255.);
         mg = (int)(matrgba[1+j] * 255.);
         mb = (int)(matrgba[2+j] * 255.);
         matcolor[i] = ((mr<<24) | (mg << 16) | (mb << 8) | (255));
        }

      for (i = 0; i < MAXCONTOURS+2; i++)
        {
         j = i * 4;
         mr = (int)(conrgba[0+j] * 255.);
         mg = (int)(conrgba[1+j] * 255.);
         mb = (int)(conrgba[2+j] * 255.);

         redcol = (float)mr/255.0;
         greencol = (float)mg/255.0;
         bluecol = (float)mb/255.0;
         matpixid = i + 1;
         if (!mesacmap)
           {
            matpix = mat_pixels[matpixid];
            mat_color.pixel = matpix;
            mat_color.flags = DoRed | DoGreen | DoBlue;
            mat_color.red = (unsigned short)(redcol * 65535.);
            mat_color.green = (unsigned short)(greencol * 65535.);
            mat_color.blue = (unsigned short)(bluecol * 65535.);
            if (!truecolorcmap)   
               XStoreColor(dpy,my_cmap,&mat_color);
            else
              {
               XAllocColor(dpy,my_cmap,&mat_color);
               matpix = mat_color.pixel;
               mat_pixels[matpixid] = matpix;
              }
           }
         else
           {
            matpix = getbestcolor(redcol,greencol,bluecol);
            mat_pixels[matpixid] = matpix;
           }
        }
     }

   /*  If new problem, or first file, make custom widgets.  */
   //TJ makewidgets causes the slowdown at the startup of GMV, must be Motif
   //   or similar issue.  Frank mentioned this at some point in an email...
   if (rdgmvcount == 1 || rdgmvflag == 0)
   {
     printf("Creating custom widgets...\n");
     fflush(stdout);
     makewidgets();
     printf("Custom widgets completed.\n");
   }

   /*  If new problem, or first file, reset angles for.  */
   if (rdgmvcount == 1 || rdgmvflag == 0)
     {
      xrot = 0.;  yrot = 0.;  zrot = 0.;

      /*  If a 2D or thin problem, reset angles.  */
      xthick = globxicmax - globxicmin;
      ythick = globyicmax - globyicmin;
      zthick = globzicmax - globzicmin;
      if (xthick > ythick * 3.)
         xrot = -90.;
      if (xthick > zthick*3. && ythick > zthick*3.) 
        { 
         xrot = -90;
         yrot = 90.;
        }
      setangles();
     }

   /*  If new problem, or first file, read gmvrc, if any.  */
   if (rdgmvcount == 1 || rdgmvflag == 0) readgmvrc();

   /*  If more than 1 file read and reading an input file  */
   /*  from the same simulation, apply attributes data.    */
   if (rdgmvcount > 1 && rdgmvflag > 0) get_attr_data();

   msgstr = XmStringLtoRCreate(" ",
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);

   string1 = XmStringLtoRCreate("Current file: ",
                               XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringLtoRCreate(tmpfilnam,
                                XmSTRING_DEFAULT_CHARSET);
   msgstr = XmStringConcat(string1,string2);
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(string1);
   XmStringFree(string2);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);
   if (beepflag) XBell(dpy,100);
   XFlush(dpy);

   gmv_data_in = 1;

   /*  If attributes file specified on the command line,  */
   /*  get and set attributes.  (First time only.         */
   if (readattributes == 1)
     {
      getattributes();
      readattributes = 0;
     }

   /*  Save starting angles and magnification.  */
   if (rdgmvcount == 1 || rdgmvflag == 0)
     {
      xrot_start = xrot;
      yrot_start = yrot;
      zrot_start = zrot;
      magnify_start = magnify;
     }

   if (displistflag) drawnewlistflag = 1;
   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
   XFlush(dpy);
}  

void filecancelg(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(w);
   XFlush(dpy);
}

void filecancel(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  char *pattern, *dir;

   XmFileSelectionBoxCallbackStruct *fcb = 
      (XmFileSelectionBoxCallbackStruct *) call_data;
   XmStringGetLtoR(fcb->pattern,XmSTRING_DEFAULT_CHARSET,&pattern);
   XmStringGetLtoR(fcb->dir,XmSTRING_DEFAULT_CHARSET,&dir);
   XtUnmanageChild(w);
   XFlush(dpy);

   switch(fileselflag)
     {

      case(GMVINPUT):  /*  GMV input file.  */
         strcpy(fspattern[GMVINPUT],pattern);
         strcpy(fsdir[GMVINPUT],dir);
         break;

      case(GETATTR):  /*  Get attributes file.  */
         strcpy(fspattern[GETATTR],pattern);
         strcpy(fsdir[GETATTR],dir);
         break;

      case(PUTATTR):  /*  Put attributes file.  */
         strcpy(fspattern[PUTATTR],pattern);
         strcpy(fsdir[PUTATTR],dir);
         break;

      case(SNAPSHOT):  /*  Save snapshot.  */
         strcpy(fspattern[SNAPSHOT],pattern);
         strcpy(fsdir[SNAPSHOT],dir);
         snapshotsize = 0;
         break;

      case(ANIMSNAP):  /*  Animation snapshot file prefix.  */
         strcpy(fspattern[ANIMSNAP],pattern);
         strcpy(fsdir[ANIMSNAP],dir);
         break;

      case(ANIMFLYSNAP):  /*  Flight animation snapshot file prefix.  */
         strcpy(fspattern[ANIMFLYSNAP],pattern);
         strcpy(fsdir[ANIMFLYSNAP],dir);
         break;

      case(ANIMFLYSAV):  /*  Save flight animation control data.  */
         strcpy(fspattern[ANIMFLYSAV],pattern);
         strcpy(fsdir[ANIMFLYSAV],dir);
         break;

      case(ANIMFLYGET):  /*  Get flight animation control data.  */
         strcpy(fspattern[ANIMFLYGET],pattern);
         strcpy(fsdir[ANIMFLYGET],dir);
         break;

      case(CUTLINEOUT):  /*  Save cutlines.  */
         strcpy(fspattern[CUTLINEOUT],pattern);
         strcpy(fsdir[CUTLINEOUT],dir);
         break;

      case(COLORMAPIN):  /*  Read colormap.  */
         strcpy(fspattern[COLORMAPIN],pattern);
         strcpy(fsdir[COLORMAPIN],dir);
         break;

      case(GMVRAY):  /*  Read GMV RAY file.  */
         strcpy(fspattern[GMVRAY],pattern);
         strcpy(fsdir[GMVRAY],dir);
         break;

      case(QUERYOUT):  /*  Save query data.  */
         strcpy(fspattern[QUERYOUT],pattern);
         strcpy(fsdir[QUERYOUT],dir);
         qdquitfile();
         break;

#ifdef PHYSED
      case(GMVOUTPUT):  /*  GMV input file.  */
         strcpy(fspattern[GMVINPUT],pattern);
         strcpy(fsdir[GMVINPUT],dir);
         break;
#endif
     }
   free(pattern);
   free(dir);
}
  
void quit(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   gmvexit();
}

void SetWMCloseProtocol(w)
  Widget w;
{
 Widget shell;
 Atom wm_close_atom;

   /*  This function sets the WM_DELETE_WINDOW protocol to call     */
   /*  the WMCloseWindowProtocol callback of the specified widget.  */
   GETSHELL(shell, w);
   wm_close_atom = XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW", False);
   XmAddWMProtocols(shell, &wm_close_atom, 1);
   XmAddWMProtocolCallback(shell, wm_close_atom, quit, NULL);
}

void axesswitch(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   axisflag =! axisflag;
   XmToggleButtonGadgetSetState(objaxes,axisflag,FALSE);
   fastdraw = 0;
   drawit();
}

void do_o_axis(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   o_axisflag =! o_axisflag;
   XmToggleButtonGadgetSetState(o_axis_on,o_axisflag,FALSE);

   /* if (displistflag) drawnewlistflag = 1; */
   fastdraw = 0;
   drawit();
}

void do_o_axis_set(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void get_1_point(Widget w, XtPointer client_data, XtPointer call_data);

   o_axissetflag = 1;
   get_1_point(NULL, (caddr_t) 3, NULL);
}

void time_cycle(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  
   arg = (long int)client_data;
   if (arg == 0) timeflag =! timeflag;
   if (arg == 1) cycleflag =! cycleflag;
   /* if (displistflag) drawnewlistflag = 1; */
   fastdraw = 0;
   drawit();
}

void dodistscale(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   distscaleflag =! distscaleflag;
   if (viewflag > 0) distscaleflag = 0;
   XmToggleButtonGadgetSetState(distsclon,distscaleflag,FALSE);

   /* if (displistflag) drawnewlistflag = 1; */
   fastdraw = 0;
   drawit();
}

void dodistset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void get_1_point(Widget w, XtPointer client_data, XtPointer call_data);

   distscalesetflag = 1;
   get_1_point(NULL, (caddr_t) 3, NULL);
}
  
void doanimation(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   if (viewflag < 2)
      setanimationform();
   if (viewflag == 2)
      setanimflyform();
}

void doorigin(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   centerformpop();
}

void dosubset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   subsetformpop();
}

void doboundingbox(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   boundingboxpop();
}

void dofieldlim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   fieldlimformpop();
}

void docfieldlim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cfieldlimformpop();
}

void dosfieldlim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   sfieldlimformpop();
}

void doffieldlim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   ffieldlimformpop();
}

void dodatalimpop(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   datalimpopflag = (int)XmToggleButtonGadgetGetState(w);
}

void dotracelim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   tracelimformpop();
}

void doraylim(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   raylimformpop();
}

void doscaleaxis(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   scaleaxisformpop();
}

void pop_vectorctl(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   vectorctlformpop();
}

void pop_rbzoom(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   rbzoomformpop();
}

void attributes(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string, string2, string3;

   arg = (long int)client_data;
   if (arg == 0)
     {
      fileselflag = PUTATTR;
      string = XmStringCreate("Put Attributes",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[PUTATTR],
                               XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[PUTATTR],
                               XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2, 
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[PUTATTR])
        {
         string = XmStringCreate(attribsfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }

   if (arg == 1)
     {
      fileselflag = GETATTR;
      string = XmStringCreate("Get Attributes",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[GETATTR],
                               XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[GETATTR],
                               XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2, 
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[GETATTR])
        {
         string = XmStringCreate(attribsfilnam,
                              XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }
}

void dodistance(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   distanceflag = 1;
   ncrosspts = 0;
   glXMakeCurrent(dpy,windowid,glx_glw_context);
   XDefineCursor(dpy,windowid,crosshair);
}

void get_2_points(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) cp2ptflag = 1;
   if (arg == 1) cl2ptflag = 1;
   ncrosspts = 0;
   glXMakeCurrent(dpy,windowid,glx_glw_context);
   XDefineCursor(dpy,windowid,crosshair);
}

void dopolysubset(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   polysubsetformpop();
}

void get_1_point(Widget w, XtPointer client_data, XtPointer call_data)
{
  int arg;

   arg = (long int)client_data;

   if (arg == 0) valueflag = 1;
   if (arg == 1) cl1ptflag = 1;
   /* if (arg == 2) nprobeflag = 1; */
   /* if (arg == 3) cprobeflag = 1; */
   if (arg == 4) valueflag = 2;
   ncrosspts = 0;
   glXMakeCurrent(dpy,windowid,glx_glw_context);
   XDefineCursor(dpy,windowid,crosshair);
}

void docoloredit(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   coloreditformpop();
}

void dofcoloredit(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   fcoloreditformpop();
}

void dobackground(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   backgrndformpop();
}


void snapit(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   snapshotformpop();
}


void savegmvrc(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   writegmvrc();
}


void donwrtvars(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   nwritevars();
}

void docwrtvars(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cwritevars();
}

void dotwrtvars(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   twritevars();
}

void doswrtvars(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   swritevars();
}


void doplotbox(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   plotboxformpop();
}

void doptsize(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   pntsizeformpop();
}

void dolnsize(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   linesizeformpop();
}

void docolorbar(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;
   if (arg == 0)
     {
      colorbarflag = (short)XmToggleButtonGadgetGetState(w);
      /* if (displistflag) drawnewlistflag = 1; */
      fastdraw = 0;
      drawit();
     }
   if (arg == 1) matcolorbarformpop();
}

void docolorpref(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, cbflag;

   arg = (long int)client_data;
   cbflag = (int)XmToggleButtonGadgetGetState(w);
   if (cbflag == 1)
     {
      colorbarpref = arg;
      /* if (displistflag) drawnewlistflag = 1; */
      fastdraw = 0;
      drawit();
     }
}

void docolortitle(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg, cbflag;
  void pop_clrbartitle();

   arg = (long int)client_data;
   cbflag = (int)XmToggleButtonGadgetGetState(w);
   if (cbflag == 1)
     {
      colorbartitleflag = arg;
      if (colorbartitleflag == 1) pop_clrbartitle(),
      /* if (displistflag) drawnewlistflag = 1; */
      fastdraw = 0;
      drawit();
     }
}

void doautord(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;

   arg = (long int)client_data;

   autordformpop(arg);
}

void setreflections()
{
  int reflectmask;

   span = sqrt(xrange*xrange + yrange*yrange + zrange*zrange);
   setcenterwidgets();
   setboundingboxwidgets();
 
   reflectmask = xreflect | (yreflect << 1) | (zreflect << 2);
 
   /*   reflectmask      buttons on
	   0		   none
	   1		   x
	   2		   y
	   3		   x and y
	   4		   z
	   5		   x and z
	   6		   y and z
	   7		   x, y, and z
   */
 
   switch (reflectmask) 
     {
 
      case 0:
        nrefl_loops = 1;
        break;
 
      case 1:
        nrefl_loops = 2;
        xscale[1] = -1.; yscale[1] = 1.; zscale[1] = 1.;
        break;
 
      case 2:
        nrefl_loops = 2;
        xscale[1] = 1.; yscale[1] = -1.; zscale[1] = 1.;
        break;
 
      case 3:
        nrefl_loops = 4;
        xscale[1] = -1.; yscale[1] = 1.; zscale[1] = 1.;
        xscale[2] = 1.; yscale[2] = -1.; zscale[2] = 1.;
        xscale[3] = -1.; yscale[3] = -1.; zscale[3] = 1.;
        break;
 
      case 4:
        nrefl_loops = 2;
        xscale[1] = 1.; yscale[1] = 1.; zscale[1] = -1.;
        break;
 
      case 5:
        nrefl_loops = 4;
        xscale[1] = -1.; yscale[1] = 1.; zscale[1] = 1.;
        xscale[2] = 1.; yscale[2] = 1.; zscale[2] = -1.;
        xscale[3] = -1.; yscale[3] = 1.; zscale[3] = -1.;
        break;
 
      case 6:
        nrefl_loops = 4;
        xscale[1] = 1.; yscale[1] = -1.; zscale[1] = 1.;
        xscale[2] = 1.; yscale[2] = 1.; zscale[2] = -1.;
        xscale[3] = 1.; yscale[3] = -1.; zscale[3] = -1.; 
        break;
 
      case 7:
        nrefl_loops = 8;
        xscale[1] = -1.; yscale[1] = 1.; zscale[1] = 1.;
        xscale[2] = 1.; yscale[2] = 1.; zscale[2] = -1.;
        xscale[3] = -1.; yscale[3] = 1.; zscale[3] = -1.;
        xscale[4] = 1.; yscale[4] = -1.; zscale[4] = -1.;
        xscale[5] = -1.; yscale[5] = -1.; zscale[5] = 1.;
        xscale[6] = 1.; yscale[6] = -1.; zscale[6] = 1.;
        xscale[7] = -1.; yscale[7] = -1.; zscale[7] = -1.; 
        break; 
     }

   if (polysin == 1) matexpvects();
   if (cexplodefact > 0.) cellexpvects();
}

void reflections(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  double dmin, dmax, dbig;

   arg = (long int)client_data;
   
   if (arg == 0) 
     {
      xreflect = (int)XmToggleButtonGadgetGetState(objxrefl);
      if (xreflect == 1) 
        {
         dmin = fabs(globxicmin);
         dmax = fabs(globxicmax);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         globxicmax = dbig;
         globxicmin = -globxicmax;
         xrange = 2.0*globxicmax;
         xtrans = 0.0;
         dmin = fabs(xminbbvalo);
         dmax = fabs(xmaxbbvalo);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         xmaxbbvalo = dbig;
         xminbbvalo = -xmaxbbvalo;
         xminbbval = xminbbvalo;
         xmaxbbval = xmaxbbvalo;
        } 
      else 
        {
         globxicmax = globxicmaxo; globxicmin = globxicmino;
         xminbbvalo = globxicmin;  xmaxbbvalo = globxicmax;
         xminbbval = xminbbvalo;   xmaxbbval = xmaxbbvalo;
         xrange = globxicmax - globxicmin;
         xtrans = -0.5*(globxicmax+globxicmin);
        }
     } 

   if (arg == 1) 
     {
      yreflect = (int)XmToggleButtonGadgetGetState(objyrefl);
      if (yreflect == 1) 
        {
         dmin = fabs(globyicmin);
         dmax = fabs(globyicmax);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         globyicmax = dbig;
         globyicmin = -globyicmax;
         yrange = 2.0*globyicmax;
         ytrans = 0.0;
         dmin = fabs(yminbbvalo);
         dmax = fabs(ymaxbbvalo);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         ymaxbbvalo = dbig;
         yminbbvalo = -ymaxbbvalo;
         yminbbval = yminbbvalo;
         ymaxbbval = ymaxbbvalo;
        } 
      else 
        {
         globyicmax = globyicmaxo;  globyicmin = globyicmino;
         ymaxbbvalo = globyicmax;   yminbbvalo = globyicmin;
         yminbbval = yminbbvalo;    ymaxbbval = ymaxbbvalo;
         yrange = globyicmax - globyicmin;
         ytrans = -0.5*(globyicmax+globyicmin);
        }
     } 

   if (arg == 2) 
     {
      zreflect = (int)XmToggleButtonGadgetGetState(objzrefl);
      if (zreflect == 1) 
        {
         dmin = fabs(globzicmin);
         dmax = fabs(globzicmax);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         globzicmax = dbig;
         globzicmin = -globzicmax;
         zrange = 2.0*globzicmax;
         ztrans = 0.0;
         dmin = fabs(zminbbvalo);
         dmax = fabs(zmaxbbvalo);
         dbig = dmax; if (dmin > dmax) dbig = dmin;
         zmaxbbvalo = dbig;
         zminbbvalo = -zmaxbbvalo;
         zminbbval = zminbbvalo;
         zmaxbbval = zmaxbbvalo;
        }
      else 
        {
         globzicmax = globzicmaxo;  globzicmin = globzicmino;
         zminbbvalo = globzicmin;   zmaxbbvalo = globzicmax;
         zminbbval = zminbbvalo;    zmaxbbval = zmaxbbvalo;
         zrange = globzicmax - globzicmin;
         ztrans = -0.5*(globzicmax+globzicmin);
        }
     }

   setreflections();
   if (displistflag) drawnewlistflag = 1;
   fastdraw = 0;
   drawit();

}

void doisosurfformm(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   isosurfformmpop();
}

void doisosurfformf(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   isosurfformfpop();
}

void docutplaneform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   if (ncells > 0 && numvars > 0)
      cutplaneformpop();
}

void docutsphereform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   if (ncells > 0 && numvars > 0)
      cutsphereformpop();
}

void docutlineform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   if (numvars > 0)
      cutlineformpop();
}

void dogridanal(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   gridanalformpop();
}

void dopolygonform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void polygonformpop();

   polygonformpop();
}

void donodeform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void nodeformpop();

   nodeformpop();
}

void docellform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void cellformpop();

   cellformpop();
}

void dosurfform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void surfformpop();

   surfformpop();
}

void dotraceform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void traceformpop();

   traceformpop();
}

void dorayform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  void rayformpop();

   rayformpop();
}

void donfldcalcform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   fldcalcformpop();
}

void docfldcalcform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cfldcalcformpop();
}

void dotfldcalcform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   tfldcalcformpop();
}

void dosfldcalcform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   sfldcalcformpop();
}

void doqueryform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   queryformpop();
}

void doisovolform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   isovolformpop();
}

void donfldavgform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   nfldavgformpop();
}

void docfldavgform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   cfldavgformpop();
}

void doconcaveform(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   concaveformpop();
}

void rotator(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ival;
  char str[20];
  float val;

   i = (long int)client_data;
   XmScaleGetValue(w,&ival);
   val = (float)ival;
   sprintf(str,"%6.1f",val);

   if (i == 0)
     {
      xrot = (float)ival;
      XtVaSetValues(objxrottxt,XmNvalue,str,NULL);
     }
   if (i == 1)
     {
      yrot = (float)ival;
      XtVaSetValues(objyrottxt,XmNvalue,str,NULL);
     }
   if (i == 2)
     {
      zrot = (float)ival;
      XtVaSetValues(objzrottxt,XmNvalue,str,NULL);
     }

   fastdraw = 0;
   if (gmv_data_in > 0) drawit();
}

void rdangles(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr = NULL;
  float val = 0;

   tmpstr = XmTextFieldGetString(objxrottxt);
   sscanf(tmpstr,"%f",&val);
   XtFree(tmpstr);

   while (val > 360.) { val -= 360.; }
   while (val < -360.) { val += 360.; }
   if (val < -180.) val += 360.;
   else if (val > 180.) val -= 360.;

   xrot = val;

   tmpstr = XmTextFieldGetString(objyrottxt);
   sscanf(tmpstr,"%f",&val);
   XtFree(tmpstr);

   while (val > 360.) { val -= 360.; }
   while (val < -360.) { val += 360.; }
   if (val < -180.) val += 360.;
   else if (val > 180.) val -= 360.;

   yrot = val;

   tmpstr = XmTextFieldGetString(objzrottxt);
   sscanf(tmpstr,"%f",&val);
   XtFree(tmpstr);

   while (val > 360.) { val -= 360.; }
   while (val < -360.) { val += 360.; }
   if (val < -180.) val += 360.;
   else if (val > 180.) val -= 360.;

   zrot = val;

   setangles();

   fastdraw = 0;
   drawit();
}

void reset_startangles(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   xrot = xrot_start;
   yrot = yrot_start;
   zrot = zrot_start;
   magnify = magnify_start;

   setangles();
   setmag();

   fastdraw = 0;
   drawit();
}

void setview(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, onflag, retviewflag;
  double xa, ya, za, ra;

   i = (long int)client_data;
   onflag = (int)XmToggleButtonGadgetGetState(w);
   if (!onflag) return;
   viewflag = i;
   if (viewflag > 0)
     {
      distscaleflag = 0;
      XmToggleButtonGadgetSetState(distsclon,distscaleflag,FALSE);
     }

   if (viewflag == 0)
     {
      XtSetSensitive(objdist,TRUE);
      if (numvars > 0)
        {
         XtSetSensitive(ctln1pt,TRUE);
         XtSetSensitive(ctln2pt,TRUE);
        }
      if (ncells > 0 && numvars > 0)
         XtSetSensitive(obj2pts,TRUE);
      if (polysin > 0)
        {
         XtSetSensitive(polysubset,TRUE);
         XtSetSensitive(psubset,TRUE);
        }
      XtSetSensitive(rbzoombut,TRUE);
     }
   else
     {
      XtSetSensitive(objdist,FALSE);
      if (numvars > 0)
        {
         XtSetSensitive(ctln1pt,FALSE);
         XtSetSensitive(ctln2pt,FALSE);
        }
      if (ncells > 0 && numvars > 0)
         XtSetSensitive(obj2pts,FALSE);
      if (polysin > 0)
        {
         XtSetSensitive(polysubset,FALSE);
         XtSetSensitive(psubset,FALSE);
        }
      XtSetSensitive(rbzoombut,FALSE);
      rzclose(NULL,NULL,NULL);
     }

   if (nnodes > 0) setqueryprobe();

   if ((viewflag == 2 || viewflag == 4) && lastviewflag < 2)
     {
      xa = (double)xrot / ((double)180./3.14592654);
      ya = (double)(90.-yrot) / ((double)180./3.14592654); 
      flypos[0] = span*sin(ya)*cos(xa) - xtrans;
      flypos[1] = span*sin(ya)*sin(xa) - ytrans;
      flypos[2] = span*cos(ya) - ztrans;
      xrot = 180. - xrot;
      if (xrot < -180.) xrot += 360.;
      else if (xrot > 180.) xrot -= 360.;

      yrot = 90. - yrot;
      if (yrot < -180.) yrot += 360.;
      else if (yrot > 180.) yrot -= 360.;

      setangles();

      if (animationmenu_up == 1)
        {
         animationflag = 0;
         animation_count = 0;
         animationmenu_up = 0;
         XtUnmanageChild(animationform);
         setanimflyform();
        }

      xpan = 0.;  ypan = 0.;  zpan = 0.;
     }

   if (viewflag < 2 && (lastviewflag == 2 || lastviewflag == 4))
     {
      xa = (double)flypos[0] + (double)xtrans;
      ya = (double)flypos[1] + (double)ytrans;
      za = (double)flypos[2] + (double)ztrans;
      ra = sqrt(xa*xa + ya*ya + za*za);

      xrot = 90. - (180./3.14592654) * atan2(xa,ya);
      if (xrot < -180.) xrot += 360.;
      else if (xrot > 180.) xrot -= 360.;

      yrot = 90. - (180./3.14592654) * acos((double)(za/ra));
      if (yrot < -180.) yrot += 360.;
      else if (yrot > 180.) yrot -= 360.;

      setangles();

      xpan = 0.;  ypan = 0.;  zpan = 0.;

      if (animflymenu_up == 1)
        {
         animflymenu_up = 0;
         animfly_snapflag = 0;
         animfly_count = 0;
         XtUnmanageChild(animflyform);
         setanimationform();
        }
     }

   if (viewflag < 3)
     {
      if (lastviewflag != viewflag)
        {
         if (viewflag == 2 || lastviewflag == 2)
            if (displistflag) drawnewlistflag = 1;
         lastviewflag = viewflag;
         fastdraw = 0;
         drawit();
        }
     }
   else
     {
      retviewflag = lastviewflag;
      lastviewflag = viewflag;
#ifdef __sgi
      stereoformpop(retviewflag);
#else
     if (stereowindow)
        stereoformpop(retviewflag);
#endif
     }
}

void rotatortxt(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int i, ival;
  char str[20];
  float val;

   i = (long int)client_data;
   XmScaleGetValue(w,&ival);
   val = (float)ival;
   sprintf(str,"%6.1f",val);

   if (i == 0)
     {
      xrot = (float)ival;
      XtVaSetValues(objxrottxt,XmNvalue,str,NULL);
     }
   if (i == 1)
     {
      yrot = (float)ival;
      XtVaSetValues(objyrottxt,XmNvalue,str,NULL);
     }
   if (i == 2)
     {
      zrot = (float)ival;
      XtVaSetValues(objzrottxt,XmNvalue,str,NULL);
     }

}

void magnifier(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval;
  double mag;

   XmScaleGetValue(w,&inval);
   mag = (double) (inval) / 200.;
   mag = pow(10.0,mag);
   magnify = (mag > 1000. ? 1000. : (float)mag);
   setmag();

   fastdraw = 0;
   if (gmv_data_in > 0) drawit();
}

void magnifiertxt(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int inval;
  double mag;
  char str[20];

   XmScaleGetValue(w,&inval);
   mag = (double) (inval) / 200.;
   mag = pow(10.0,mag);
   magnify = (mag > 1000. ? 1000. : (float)mag);
   sprintf(str,"%4.2f",magnify);
   XtVaSetValues(objmagnifytxt,XmNvalue,str,NULL);
}

void rdmag(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  char *tmpstr;
  float val;

   tmpstr = XmTextFieldGetString(w);
   sscanf(tmpstr,"%f",&val);
   XtFree(tmpstr);

   if (val > 1000.) val = 1000.;
   if (val < .01) val = .01;

   magnify = val;

   setmag();

   fastdraw = 0;
   drawit();
}

void errok(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(errorform);

   if (errorfile > 0)
      MyManageChild(file_sel2);
   else
      MyManageChild(file_sel);
}

void dodisplist(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int dispflag;

   dispflag = (int)XmToggleButtonGadgetGetState(w);
   if (dispflag == 1)
     {
      displistflag = 1;
      drawnewlistflag = 1;
      fastdraw = 0;
      drawit();
     }
   else
     {
      displistflag = 0;
      drawnewlistflag = 0;
      glDeleteLists(DLISTALL,2);
      fastdraw = 0;
      drawit();
     }
}

void dotrackball(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   trackballflag = (int)XmToggleButtonGadgetGetState(w);
}

void beepswitch(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   beepflag = (int)XmToggleButtonGadgetGetState(w);
}

void dotexture(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
   textureflag = (int)XmToggleButtonGadgetGetState(w);
   drawnewlistflag = 1;
   fastdraw = 0;
   drawit();
}


/*  GLX callbacks.  */

void init_axwindow(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 XtPointer call_data;
{
  int i;
  void expose_axwindow();

   winaxisid = XtWindow(w);
   glx_axglw_context = glXCreateContext(XtDisplay(w), vi, 
                                        glx_glw_context, GL_TRUE);
   glXMakeCurrent(dpy,winaxisid,glx_axglw_context);

   initaxfont(winaxisid,glx_axglw_context);
 
   glMatrixMode(GL_MODELVIEW);
   glLineWidth(2.);
}
 
void expose_axwindow(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 caddr_t call_data;
{
   if (gmv_data_in > 0) drawwinaxis();
   else
     {
      glXMakeCurrent(dpy,winaxisid,glx_axglw_context);
      glClearDepth(1.0);
      glClearColor(1.0,1.0,1.0,1.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winaxisid);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, winaxisid);
     }
}
 
void draw_scene_callback(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 caddr_t call_data;
{
   if (rdgmvcount > 0) drawit();
}
 
void draw_expose_callback(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 caddr_t call_data;
{
   fastdraw = 0;
   if (gmv_data_in > 0)
     {
      expose_redraw();

      /*  Redraw zoombox if needed.  */
      if (rbzoomflag == 2)
         drawrbzoom(rbx1,rby1,rbx2,rby2);

      /*  Redraw select polygon if needed.  */
      if (surfselpolyopt > 0)
         drawselpoly(0);
     }
   else
     {
      glXMakeCurrent(dpy,windowid,glx_glw_context);
      glClearDepth(1.0);
      glClearColor(0.0,0.0,0.0,1.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, windowid);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, windowid);
      glClearColor(1.0,1.0,1.0,1.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, windowid);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      if (doubleBuffer == GL_TRUE) glXSwapBuffers(dpy, windowid);
     }
}

void init_window(w, client_data, call_data)
 Widget w;
 caddr_t client_data;
 XtPointer call_data;
{
   glx_glw_context = glXCreateContext(XtDisplay(w), vi, 
                                      glx_axglw_context, GL_TRUE);
   windowid = XtWindow(glw);
   XFlush(dpy);
   glXMakeCurrent(dpy,windowid,glx_glw_context);
   initstuff();

   /*  Initialize Trackball.  */
   Ball_Init(&ball);
   Ball_Place(&ball, qOne, 0.75);
   Ball_UseSet(&ball,NoAxes);
}
 
void do_resize(w, client_data, call_data)
 Widget w;
 XtPointer client_data;
 XtPointer call_data;
{
  GLwDrawingAreaCallbackStruct *call;
/*
   if (noresizeflag)
     {
      noresizeflag = 0;
      return;
     }
*/
   call = (GLwDrawingAreaCallbackStruct *)call_data;
 
   windowwidth = (GLint)call->width;
   windowheight = (GLint)call->height;
   if (windowwidth > windowxmax) windowwidth = windowxmax;
   if (windowheight > windowymax) windowheight = windowymax;
   xsize = windowwidth;
   ysize = windowheight;
   aspect = (float)xsize / (float)ysize;
   fill_winsize();

   XFlush(dpy);
   glXWaitX();
   fastdraw = 0;
   draw_scene_callback(w,NULL,NULL);
}
 
void update_scene() 
{
  void orient();
 
   switch (mode)
     {
      case ORIENT:
         orient();
         break;
     }
}
 
 
void orient() 
{
  GLfloat btheta, bphi, bpsi;
  double costheta, sintheta, cosphi, sinphi, cospsi, sinpsi, arg;

   /*  Compute the rotation angles from the quaternion matrix.  */
   if (interactivemouse == 1)
     {
      cosphi = (double)Ballmatrix[2][2];
      sinphi = sqrt(1.-cosphi*cosphi);
      costheta = Ballmatrix[1][2]/sinphi;
      sintheta = -Ballmatrix[0][2]/sinphi;
      cospsi = Ballmatrix[2][1]/sinphi;
      sinpsi = Ballmatrix[2][0]/sinphi;
  
      bphi = (float)acos((double)Ballmatrix[2][2]);
      arg = (double)Ballmatrix[2][1]/sinphi;

      /*  have to make sure that the argument isn't > 1.0  */
      if ( arg > 1.0 ) arg = 1.0;
      else if ( arg < -1.0 ) arg = -1.0;
      bpsi = (float)acos(arg);
      arg = 0.;
      arg = (double)Ballmatrix[1][2]/sinphi;
      if ( arg > 1.0 ) arg = 1.0;
      else if ( arg < -1.0 ) arg = -1.0;
      btheta = -(float)acos(arg);

      bphi = 180.*bphi/MYPI;
      bpsi = 180.*bpsi/MYPI;
      btheta = 180.*btheta/MYPI;
      if ( (sintheta > 0.0) && (costheta > 0.0) ) btheta = -btheta;
      if ( (sintheta > 0.0) && (costheta < 0.0) ) btheta = -btheta - 360.0;

      if ( Ballmatrix[2][0] < 0.0 ) bpsi = -bpsi;
      xrot = btheta+90.;
      yrot = -(bphi-90.);   
      zrot = bpsi;
     }

   if (viewflag < 2)
     {
      magnify *= (double)pow(10.0,
                             0.1*(double)(4.0*(double)(oscl-scl))
                             / 1000.);
      if (magnify > 1000.) magnify = 1000.;
      setmag();
     }

   if (viewflag == 2)
     zpan = zpan - span*(oscl - scl)/windowheight;

   xpan = xpan + span*((float)(pnx-opnx))/
                       (windowwidth*magnify);
   ypan = ypan + span*((float)(opny-pny))/
                       (windowheight*magnify);
 
   if (interactivemouse == 0) 
     { 
      xrot += 0.5*(double)(omx-mx);
      if (xrot < -180.) xrot += 360.;
      else if (xrot > 180.) xrot -= 360.;

      yrot += 0.5*(double)(my-omy);
      if (yrot < -180.) yrot += 360.;
      else if (yrot > 180.) yrot -= 360.;
     }

   setangles();
}
 
 
/*  Process all Input callbacks.  */

void input(w, client_data, call_data)
 Widget w; 
 caddr_t client_data;
 GLwDrawingAreaCallbackStruct *call_data;
{
  char buffer[1];
  KeySym keysym;
  void docrosshair();
  int i, j, docross;
  void getquatsfromangles();
 
   docross = 0;
   if (distanceflag > 0 || cp2ptflag > 0 || polysubsetflag > 0 || 
       valueflag > 0 || cl1ptflag > 0 || cl2ptflag > 0 ||
       nprobeflag > 0 || cprobeflag > 0 || cennprobeflag > 0 || 
       cencprobeflag > 0 || surfselprobopt > 0 || distscalesetflag > 0 ||
       cellmatselprobeflag > 0 || cellmatremprobeflag > 0 ||
       surfmatselprobeflag > 0 || surfmatremprobeflag > 0 ||
       polymatselprobeflag > 0 || polymatremprobeflag > 0 || o_axissetflag > 0)
      docross = 1;
   switch(call_data->event->type)
     {
      case KeyRelease:
        printf("keyboard\n");
	if (XLookupString(call_data->event,buffer,1,&keysym,NULL)
            == 1 &&
	   keysym == (KeySym)XK_Escape)
	   gmvexit();
	break;
      case ButtonPress:
        fastdraw = 1;
	switch(call_data->event->xbutton.button)
	  {
	   case Button1:
	      mode = ORIENT;
	      omx = mx = call_data->event->xbutton.x;
	      omy = my = call_data->event->xbutton.y;
              if (docross)
                 mode = CROSSHAIR;
              if (rbzoomflag == 1)
                {  mode = RBZOOM; rbx1 = omx;  rby1 = omy;  }
              if (surfselpolyopt == 1)
                {
                 mode = POLY;
                 surfpolyx[nsurfpolyverts] = spx = mx;
                 surfpolyy[nsurfpolyverts] = spy = my;
                 drawselpoly(1);
                }

              interactivemouse = 0;
              if (viewflag < 2 && mode == ORIENT && trackballflag)
                {
                 interactivemouse = 1;
                 getquatsfromangles();
	         omy = my = windowheight - call_data->event->xbutton.y;
                 vNow.x = 2.0*(((float)mx/(float)windowwidth)) - 1.0;
                 vNow.y = 2.0*(((float)my/(float)windowheight)) - 1.0;
                 Ball_Mouse(&ball, vNow); 
                 Ball_Update(&ball);
                 Ball_BeginDrag(&ball);
                }
	      break;
	   case Button2:
	      mode = ORIENT;
              if (docross || surfselpolyopt > 0) mode = NOTHING;
	      opnx = pnx = call_data->event->xbutton.x;
	      opny = pny = call_data->event->xbutton.y;
	      break;
	   case Button3:
	      mode = ORIENT;
              if (docross || surfselpolyopt > 0) mode = NOTHING;
	      oscl = scl = call_data->event->xbutton.y;
	      break;
	  }
	break;
      case ButtonRelease:
        fastdraw = 0;
	switch(call_data->event->xbutton.button)
	  {
	   case Button1:
              interactivemouse = 0;
	      mode = NOTHING; mx = omx; my = omy;
              if (docross)
                 docrosshair();
              else if (rbzoomflag == 1)
                 {
                  rbzoomflag = 2;
                  XUndefineCursor(dpy,windowid);
                  XFlush(dpy);
                  drawrbzoom(rbx1,rby1,rbx2,rby2);
                 }
              else if (surfselpolyopt == 1)
                {
                 surfpolyx[nsurfpolyverts] = spx;
                 surfpolyy[nsurfpolyverts] = spy;
                 nsurfpolyverts++;
                 drawselpoly(0);
                }
              else 
	         draw_scene_callback(w,NULL,NULL);
	      break;
	   case Button2:
	      mode = NOTHING; pnx = opnx; pny = opny;
              if (docross == 0 || surfselpolyopt == 0)
	         draw_scene_callback(w,NULL,NULL);
	      break;
	   case Button3:
	      mode = NOTHING;scl = oscl;
              if (docross == 0 || surfselpolyopt == 0)
	         draw_scene_callback(w,NULL,NULL);
	      break;
	  }
	break;
      case MotionNotify:
	if (mode == ORIENT && call_data->event->xmotion.state & 
            Button1Mask)
	  {
	   omx = mx;
	   omy = my;
	   mx = call_data->event->xbutton.x;
	   my = call_data->event->xbutton.y;
           if (interactivemouse == 1)
             {
	      my = windowheight - call_data->event->xbutton.y;
              vNow.x = 2.0*(((float)mx/(float)windowwidth)) - 1.0;
              vNow.y = 2.0*(((float)my/(float)windowheight)) - 1.0;
              Ball_Mouse(&ball, vNow); 
              Ball_Update(&ball);

              for ( i = 0; i < 4; i++ )
                {
                 for ( j = 0; j < 4; j++ )
                   {
                    Ballmatrix[i][j] = ball.mNow[i][j];
                   }
                }
             }
           update_scene();
           draworientbox();
	   /* draw_scene_callback(w,NULL,NULL); */
	  }

	if (mode == RBZOOM && call_data->event->xmotion.state & 
            Button1Mask)
	  {
	   rbx2 = call_data->event->xbutton.x;
	   rby2 = call_data->event->xbutton.y;
           drawrbzoom(rbx1,rby1,rbx2,rby2);
      	  }
 
	if (mode == POLY && surfselpolyopt == 1 && nsurfpolyverts > 0 &&
            call_data->event->xmotion.state & Button1Mask)
	  {
           spx = call_data->event->xbutton.x;
           spy = call_data->event->xbutton.y;
           surfpolyx[nsurfpolyverts] = spx;
           surfpolyy[nsurfpolyverts] = spy;
           drawselpoly(1);
	  }
 
	if (mode == ORIENT && call_data->event->xmotion.state & 
            Button2Mask)
	  {
	   opnx = pnx;
	   opny = pny;
	   pnx = call_data->event->xbutton.x;
	   pny = call_data->event->xbutton.y;
           update_scene();
           draworientbox();
	   /* draw_scene_callback(w,NULL,NULL); */
	  }
 
	if (mode == ORIENT && call_data->event->xmotion.state & 
            Button3Mask)
	  {
	   oscl = scl;
	   scl = call_data->event->xbutton.y;
	   update_scene();
           draworientbox();
	   /* draw_scene_callback(w,NULL,NULL); */
	  }
	break;
    }
	
}


void docrosshair()
{
  float distance1, params[4];
  GLuint sx, sy;
  static float wx1, wy1, wz1, wx2, wy2, wz2, wx3, wy3, wz3,
               wx4, wy4, wz4;
  GLubyte pval[4];
  unsigned long val;
  char str[10];
  XmString string;
  GLfloat projmatrix[16], modelvmatrix[16];
  GLdouble dprojmatrix[16], dmodelvmatrix[16];
  GLint vport[4];
  GLdouble dwx1, dwy1, dwz1, dwx2, dwy2, dwz2;
  int i;
  GLfloat amb[4], diff[4], spec[4], shin[1];
 
   ncrosspts++;
   sx = (GLuint) mx + 2;
   sy = (GLuint) (windowheight - my - 3);
   glPushMatrix();
   if (viewflag == 0)
      glScalef(magnify,magnify,magnify);
   if (viewflag == 1)
      glScalef(1.0,1.0,1.0);
   glTranslatef(xpan, ypan, 0.);
   glTranslatef(0., 0., -((GLfloat)span));
   glRotatef( -( (GLfloat)(zrot)), 0., 0., 1.);
   glRotatef( -((GLfloat)(-yrot+90.)), 1., 0., 0.);
   glRotatef( -( (GLfloat)(xrot+90.)), 0., 0., 1);
   glTranslatef(xtrans, ytrans, ztrans);
   glFlush();
   vport[0] = 0;
   vport[1] = 0;
   vport[2] = windowwidth+1;
   vport[3] = windowheight+1;
   glGetFloatv(GL_PROJECTION_MATRIX,projmatrix);
   glGetFloatv(GL_MODELVIEW_MATRIX,modelvmatrix);
   for ( i = 0; i < 16; i++ )
     {
      dmodelvmatrix[i] = (GLdouble)modelvmatrix[i];
      dprojmatrix[i] = (GLdouble)projmatrix[i];
     }
   gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)0.0,
                dmodelvmatrix,dprojmatrix,vport,&dwx1,&dwy1,&dwz1);
   gluUnProject((GLdouble)sx,(GLdouble)sy,(GLdouble)1.0,
                dmodelvmatrix,dprojmatrix,vport,&dwx2,&dwy2,&dwz2);
   if (ncrosspts == 1)
     {
      wx1 = (float) dwx1;
      wy1 = (float) dwy1;
      wz1 = (float) dwz1;
      wx2 = (float) dwx2;
      wy2 = (float) dwy2;
      wz2 = (float) dwz2;
 
      /*  Draw a sphere as a marker for the first point.  */
 
      if (distanceflag > 0 || cp2ptflag > 0 || cl2ptflag > 0)
        {
         if (doubleBuffer == GL_TRUE) glDrawBuffer(GL_FRONT);
         glDisable(GL_DEPTH_TEST);
         amb[0] = amb[1] = amb[2] = .5; amb[3] = 1.;
         diff[0] = diff[1] = diff[2] = .5; diff[3] = 1.;
         spec[0] = spec[1] = spec[2] = .5; spec[3] = 1.;
         shin[0] = 10.;
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
         glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);

         params[0] = wx1+(wx2-wx1)*.01;
         params[1] = wy1+(wy2-wy1)*.01;
         params[2] = wz1+(wz2-wz1)*.01;
         params[3] = span/(magnify*200.);
 
         spheredraw(params);
 
         if (doubleBuffer == GL_TRUE) glDrawBuffer(GL_BACK);
         glEnable(GL_DEPTH_TEST);
         glPopMatrix();
        }
 
      /*  Get cutplane or cutsphere color value.  */
 
      if (valueflag > 0)
        {
         if (doubleBuffer == GL_TRUE) glReadBuffer(GL_FRONT);
         glReadPixels(sx, sy,1,1,GL_RGB,GL_UNSIGNED_BYTE,pval);
         if (doubleBuffer == GL_TRUE) glReadBuffer(GL_BACK);
         glPopMatrix();
 
         val = ((int)(pval[0]))<<24 | ((int)(pval[1]))<<16 |
               ((int)(pval[2]))<<8;
 
         if (valueflag == 1)
            get_value(val);
         if (valueflag == 2)
            get_value2(val);
         valueflag = 0;
         XUndefineCursor(dpy,windowid);
         ncrosspts = 0;
        }

      /*  Create cutline from one point.  */

      if (cl1ptflag > 0)
        {
         cl1ptflag = 0;
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         doitcutline1(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
        }

      /*  Create query node probe line.  */

      if (nprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         doitnprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 2, NULL);
        }

      /*  Create center node probe line.  */

      if (cennprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         cennprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 2, NULL);
        }

      /*  Create query cell probe line.  */

      if (cprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         doitcprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 3, NULL);
        }

      /*  Create center cell probe line.  */

      if (cencprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         cencprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 3, NULL);
        }

      /*  Create surface select probe line.  */

      if (surfselprobopt > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         doitsprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         /* glPopMatrix(); */
         /* get_1_point(NULL, (caddr_t) 3, NULL); */
        }

      /*  Create cell material select probe line.  */

      if (cellmatselprobeflag > 0 || cellmatremprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         cmatprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 3, NULL);
        }

      /*  Create surface material select probe line.  */

      if (surfmatselprobeflag > 0 || surfmatremprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         smatprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 3, NULL);
        }

      /*  Create polygon material select probe line.  */

      if (polymatselprobeflag > 0 || polymatremprobeflag > 0)
        {
         ncrosspts = 0;
         XUndefineCursor(dpy,windowid);
         pmatprobe(wx1,wy1,wz1, wx2,wy2,wz2);
         glPopMatrix();
         get_1_point(NULL, (caddr_t) 3, NULL);
        }

      /*  Generate rubberband zoom center.  */

      if (rbzoomflag > 0)
        {
         ncrosspts = 0;
         rbwx1 = wx1;
         rbwy1 = wy1;
         rbwz1 = wz1;
         rbwx2 = wx2;
         rbwy2 = wy2;
         rbwz2 = wz2;
         glPopMatrix();
        }
 
      /*  Get pixel location to set distance scale.  */
 
      if (distscalesetflag > 0)
        {
         glPopMatrix();
         distscale_x = sx - 1;  distscale_y = sy + 12;
         XUndefineCursor(dpy,windowid);
         distscalesetflag = 0;
         ncrosspts = 0;
         fastdraw = 0;
         drawit();
        }
 
      if (o_axissetflag > 0)
        {
         glPopMatrix();
         o_axis_x = sx - 1;  o_axis_y = sy;
         XUndefineCursor(dpy,windowid);
         o_axissetflag = 0;
         ncrosspts = 0;
         fastdraw = 0;
         drawit();
        }
     }
 
   if (ncrosspts == 2)
     {
      wx3 = (float) dwx1;
      wy3 = (float) dwy1;
      wz3 = (float) dwz1;
      wx4 = (float) dwx2;
      wy4 = (float) dwy2;
      wz4 = (float) dwz2;
      if (distanceflag > 0)
        {
         distance1 = (float)sqrt(
                        (double)(wx1-wx3)*(double)(wx1-wx3) +
                        (double)(wy1-wy3)*(double)(wy1-wy3) +
                        (double)(wz1-wz3)*(double)(wz1-wz3) );
         sprintf(str,"%7.4g",distance1);
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objdistval,XmNlabelString,string,NULL);
         XmStringFree(string);
         distanceflag = 0;
        }
 
      if (cp2ptflag > 0)
        {
         doitcutplane2(wx1,wy1,wz1,wx2,wy2,wz2,wx3,wy3,wz3);
         cp2ptflag = 0;
        }

      if (cl2ptflag > 0)
        {
         doitcutline2(wx1,wy1,wz1,wx2,wy2,wz2,
                      wx3,wy3,wz3,wx4,wy4,wz4);
         cl2ptflag = 0;
        } 
 
      glPopMatrix();
      XUndefineCursor(dpy,windowid);
      ncrosspts = 0;
      fastdraw = 0;
      drawit();
     }
 
}


/*  Light callbacks.  */

void drawlightgrid()
{
  int i;
  Window win = XtWindow(objlight);

   /*  Draw light window grid.  */
   XClearWindow(dpy,win);
   XSetForeground(dpy,lpgc,bluepix);
   XSetDashes(dpy,lpgc,0,dot,2);
   XSetLineAttributes(dpy,lpgc,0,LineOnOffDash,CapButt,JoinMiter);
   for (i = 0; i < 9; i++)
      {
       XDrawLine(dpy,win,lpgc,0,(i*10+10),100,(i*10+10));
       XDrawLine(dpy,win,lpgc,(i*10+10),0,(i*10+10),100);
      }
   XSetLineAttributes(dpy,lpgc,0,LineSolid,CapButt,JoinMiter);
   XDrawLine(dpy,win,lpgc,50,0,50,100);
   XDrawLine(dpy,win,lpgc,0,50,100,50);
   XSetForeground(dpy,lpgc,blackpix);
   XDrawString(dpy,win,lpgc,5,95,"Light",strlen("Light"));
}

void drawlightpos(int xpix, int ypix)
{
  Window win = XtWindow(objlight);

   /*  Draw light window light position crosshair.  */
   XSetForeground(dpy,lpgc,redpix);
   XDrawLine(dpy,win,lpgc,0,ypix,100,ypix);
   XDrawLine(dpy,win,lpgc,xpix,0,xpix,100);
}

void lpexpose(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int xpix, ypix;
  Window win = XtWindow(w);

   /*  Draw grid.  */
   drawlightgrid();

   xpix = (int) ((100.*(lightx + 1.)/2.) + .5);
   ypix = (int) ((100.*(1. - lighty)/2.) + .5);
   olxpix = xpix;
   olypix = ypix;

   /*  Draw light position on grid.  */
   drawlightpos(xpix,ypix);
}


void lpbpress(w,client_data,event) 
 Widget w;
 XtPointer client_data;
 XEvent *event;
{
  int xpix, ypix;
  Window win = XtWindow(w);

   XDefineCursor(dpy,win,crosshair);

   /*  Draw grid.  */
   drawlightgrid();

   xpix = event->xbutton.x;
   ypix = event->xbutton.y;

   xpix = olxpix;
   ypix = olypix;
   lightx = -1. + 2.*((float)xpix)/100.;
   lighty = 1. - 2.*((float)ypix)/100.;

   lightchanged = 1;
   fastdraw = 0;
   drawit();
}


void lpmotion(w,client_data,event) 
 Widget w;
 XtPointer client_data;
 XEvent *event;
{
  int xpix, ypix;

   xpix = event->xbutton.x;
   ypix = event->xbutton.y;

   lightx = -1. + 2.*((float)xpix)/100.;
   lighty = 1. - 2.*((float)ypix)/100.;

   lightchanged = 1;
   fastdraw = 0;
   drawit();
}


void lpbrelease(w,client_data,event) 
 Widget w;
 XtPointer client_data;
 XEvent *event;
{
  Window win = XtWindow(objlight);
  int xpix, ypix;

   xpix = event->xbutton.x;
   ypix = event->xbutton.y;

   lightx = -1. + 2.*((float)xpix)/100.;
   lighty = 1. - 2.*((float)ypix)/100.;

   lightchanged = 1;
   fastdraw = 0;
   olxpix = xpix;
   olypix = ypix;

   /*  Draw light position on grid.  */
   drawlightpos(xpix,ypix);

   XUndefineCursor(dpy,win);
   drawit();
}


void popdownall()
{
   XUndefineCursor(dpy,topwin);

   XtUnmanageChild(subsetform);
   XtUnmanageChild(scaleaxisform);
   XtUnmanageChild(centerform);
   XtUnmanageChild(plotboxform);
   XtUnmanageChild(boundingboxform);
   XtUnmanageChild(pntsizeform);
   XtUnmanageChild(linesizeform);
   XtUnmanageChild(vectctlform);
   XtUnmanageChild(matcolorbarform);
   XtUnmanageChild(nfldavgform);
   navgclearwin();
   XtUnmanageChild(cfldavgform);
   cavgclearwin();

   XtUnmanageChild(animationform);
   XtUnmanageChild(animflyform);
   if (ncells > 0)
     {
      XtUnmanageChild(cellform);
      if (maxcellmat > 1)
         XtUnmanageChild(isosurfformm);
      if (numvars > 0)
        {
         XtUnmanageChild(isosurfformf);
         XtUnmanageChild(cutplaneoptform);
         XtUnmanageChild(cutplaneform);
         XtUnmanageChild(cutsphereform);
         XtUnmanageChild(isovolform);
        }
      XtUnmanageChild(gridanalform);
      XtUnmanageChild(concaveform);
     }
   XtUnmanageChild(coloreditform);
   XtUnmanageChild(fcoloreditform);
   if (nnodes > 0)
     {
      XtUnmanageChild(nodeform);
      XtUnmanageChild(queryform);
     }
   if (polysin > 0)
     {
      XtUnmanageChild(polygonform);
      XtUnmanageChild(polysubsetform);
     }
   if (ntracers > 0)
     {
      XtUnmanageChild(traceform);
      XtUnmanageChild(tracelimform);
     }
   if (numvars > 0)
     {
      XtUnmanageChild(nfieldform);
      XtUnmanageChild(fieldlimform);
      XtUnmanageChild(cutlineform);
      XtUnmanageChild(fldcalcform);
     }
   if (cnumvars > 0)
     {
      XtUnmanageChild(cfieldform);
      XtUnmanageChild(cfieldlimform);
      XtUnmanageChild(cfldcalcform);
     }
   if (nsurf > 0)
      XtUnmanageChild(surfform);
   if (snumvars > 0)
     {
      XtUnmanageChild(sfieldform);
      XtUnmanageChild(sfieldlimform);
      XtUnmanageChild(sfldcalcform);
     }
   if (fnumvars > 0)
     {
      XtUnmanageChild(ffieldform);
      XtUnmanageChild(ffieldlimform);
     }

#ifdef PHYSED
   popdownPEforms();
#endif
}


void free_user_mem()
{
  int i, j, ih;

   /*  Free display lists, if used.  */
   if (displistflag) glDeleteLists(DLISTALL,2);

   /*  Free memory.  */
   for (i = 0; i < MAXMATS; i++)
     {
      FREE(polygons[i]);
      FREE(polyverts[i]);
      FREE(mmatnames[i]);
     }
   if (polysin == 1)
     {
      for (i = 0; i < maxmatno; i++)
        {
         FREE(polydrawchk[i]);
        }
     }
   for (i = 0; i < MAXMATS; i++)
     {
      mat_has_active_pts[i] = 0;
      mat_has_active_polys[i] = 0;
      active_mat[i] = 0;
      npolyspermat[i] = 0;
     }
   for (i = 0; i < numvars; i++)
     {
      FREE(fieldname[i]);
     }
   for (i = 0; i < cnumvars; i++)
     {
      FREE(cfieldname[i]);
     }
   for (i = 0; i < fnumvars; i++)
     {
      FREE(ffieldname[i]);
     }
   for (i = 0; i < numflags_cell; i++)
     {
      for (j = 0; j < flagtypes_cell[i]; j++) 
         FREE(flagnames_cell[i][j]);
      FREE(flagname_cell[i]);
      FREE(cellflags[i]);
      flagtypes_cell[i] = 0;
     }
   for (i = 0; i < numflags_node; i++)
     {
      for (j = 0; j < flagtypes_node[i]; j++) 
         FREE(flagnames_node[i][j]);
      FREE(flagname_node[i]);
      FREE(flags[i]);
      flagtypes_node[i] = 0;
     }
   for (i = 0; i < nnodegrps; i++)
     {
      FREE(nodegrpname[i]);
     }
   for (i = 0; i < ncellgrps; i++)
     {
      FREE(cellgrpname[i]);
     }
   for (i = 0; i < nfacegrps; i++)
     {
      FREE(facegrpname[i]);
     }
   if (nsurf > 0)
     {
      FREE(surffield);
      FREE(surfselectflag);
      FREE(surfselectflagp);
      FREE(surfsubsetflag);
      FREE(surfmats);
      for (i = 0; i < nsurf; i++)
        {
         FREE(surfs[i].fverts);
        }
      FREE(surfs);
      for (i = 0; i < numflags_surf; i++)
        {
         for (j = 0; j < flagtypes_surf[i]; j++) 
            FREE(flagnames_surf[i][j]);
         FREE(flagname_surf[i]);
         FREE(surfflags[i]);
         flagtypes_surf[i] = 0;
        }
      for (i = 0; i < snumvars; i++)
        {
         FREE(sfieldname[i]);
        }
      if (svectors)
        {
         FREE(suic);  FREE(svic);  FREE(swic);
        }
      for (i = 0; i < nsurfacegrps; i++)
        {
         FREE(surfacegrpname[i]);
        }
     }
   if (ntracers > 0)
     {
      FREE(traceselectflag);
      for (i = 0; i < numvartr; i++)
         FREE(fieldtrname[i]);
      for (ih = 0; ih < ntracehist; ih++)
        {
         FREE(tracers[ih].x);
         FREE(tracers[ih].y);
         FREE(tracers[ih].z);
         tracers[ih].ntracers = 0;
         for (i = 0; i < tracers[ih].numvar; i++)
            FREE(tracers[ih].field[i]);
         tracers[ih].numvar = 0;
        }
     }

   if (rdgmvflag == 0) reset_isos_cuts();

   if (ncells > 0)
     {
      FREE(cellselectflag);
      FREE(cellsubsetflag);
      if (rdgmvflag < 2)
        {
         FREE(celledges);
         ncelledges = 0;
        }
     }
   if (ncells > 0 && structuredflag == 0 && rdgmvflag < 2)
     {
      for (i = 0; i < ncells; i++)
        {
         FREE(cells[i].cnodes);
         FREE(cells[i].edgenos);
         FREE(cells[i].facenos);
        }
      FREE(cells);
      for (i = 0; i < ncellfaces; i++)
         FREE(cellfaces[i].fverts);
      FREE(cellfaces);
      ncellfaces = 0;
     }
   if (structuredflag == 1 && rdgmvflag < 2)
     {
      FREE(cells[0].cnodes);
      FREE(cells[0].edgenos);
      FREE(cells[0].facenos);
      FREE(cells);
      j = 6;
      if (struct2dflag) j = 1;
      for (i = 0; i < j; i++)
         FREE(cellfaces[i].fverts);
      FREE(cellfaces);
     }

   FREE(nodes);
   FREE(nodefield);  FREE(cellfield);
   FREE(imat);  FREE(cellmats);  FREE(facesel);
   FREE(facenormx);  FREE(facenormx);  FREE(facenormx);
   FREE(cellfacedraw);  FREE(isovoldrawfield);
   FREE(contourdrawfield);  FREE(mcontourdrawfield); 
   FREE(cellgrpcolr);  FREE(nodegrpcolr);  FREE(surfgrpcolr); 
   if (vectors)
     {
      FREE(uic);  FREE(vic);  FREE(wic);
     }
   vectors = 0;  vectorflag = 0;
   if (cvectors)
     {
      FREE(cuic);  FREE(cvic);  FREE(cwic);
     }
   cvectors = 0;  cvectorflag = 0;
   if (fvectors)
     {
      FREE(fuic);  FREE(fvic);  FREE(fwic);
     }
   fvectors = 0;  fvectorflag = 0;
   svectors = 0;  svectorflag = 0;
   FREE(nodeselectflag);  FREE(nodesubsetflag);
   FREE(nodeids);  FREE(cellids);  FREE(tracerids);  FREE(surfids);
   if (structured_in == -1)
     {  FREE(xin);  FREE(yin);  FREE(zin);  }
   if (nodeghostflag) FREE(nodeghosts);
   nodeghostflag = 0;  nnodeghosts = 0;
   if (cellghostflag) FREE(cellghosts);
   cellghostflag = 0;  ncellghosts = 0;  ghostcellflagid = 0;
}


void del_widgets()
{

   /*  Reset drawing flags and widgets.  */
   linesflag = 0;  polysflag = 0;  nodesflag = 0;  cellsflag = 0;
   cellcontflag = 0;  celledgesflag = 0;  cellnumflag = 0;  
   nodenumflag = 0;  ganodenumflag = 0;  cellshadeflag = 0;
   facerefineflag = 0; cellmedianedgeflag = 0;  cellnodenumflag = 0;
   cellnormflag = 0; gacellnumflag = 0;  gridanalflag = 0;
   cellhiddenedgesflag = 0;  cellfacenumflag = 0;  gafacenumflag = 0; 
   nodefieldactive = -1;  contourfieldactive = 0;  
   cellfieldactive = -1;  cellexpflag = -1; isovolfieldactive = 0;
   tracefieldactive = 0;  animflymenu_up = 0;
   xreflect = 0;  yreflect = 0;  zreflect = 0;  nrefl_loops = 1;
   gridfieldactive = -1;  gridsflag = 0;  gridedgesflag = 0;
   gridmedianflag = 0;  gridmedianedgeflag = 0; 
   vectfldx = -1;  vectfldy = -1;  vectfldz = -1;  speedfld = -1;
   cvectfldx = -1;  cvectfldy = -1;  cvectfldz = -1;  cspeedfld = -1;
   fvectfldx = -1;  fvectfldy = -1;  fvectfldz = -1;  fspeedfld = -1;
   contourfield_draw = -1;  mcontourfield_draw = -1;  surfflag = 0;
   isovolfield_draw = -1;  surffieldactive = -1;  surfexpflag = -1;
   surfcontflag = 0;  surfedgesflag = 0;  surfnumflag = 0;  
   surfshadeflag = 0;  surfrefineflag = 0;  surfnodenumflag = 0;
   svectfldx = -1;  svectfldy = -1;  svectfldz = -1;  sspeedfld = -1;
   xtrans = 0;  ytrans = 0;  ztrans = 0;
   xpan = 0;  ypan = 0;
   lightchanged = 0;  cp2ptflag = 0;  distanceflag = 0;
   valueflag = 0;  polysubsetflag = 0;  tracerflag = 0;
   cliponbox = 0;  animationflag = 0;  animvelflowflag = 0;
   cl1ptflag = 0;  cl2ptflag = 0;  velscale = 1.; velarrowsize = 1.;
   xscaleaxis = 1.;  yscaleaxis = 1.;  zscaleaxis = 1.;
   xscaleaxiso = 1.;  yscaleaxiso = 1.;  zscaleaxiso = 1.;
   pexplodefact = 0.;  cexplodefact = 0.; structuredflag = 0;
   struct2dflag = 0; nprobeflag = 0;  cprobeflag = 0;  
   nodeselfield = 0;  nfldselmin = 0.;  nfldselmax = 0.;
   nodeselsph_on = 0;  nodeselsph_in = 1;  
   nodeselsph_x = 0;  nodeselsph_y = 0;  nodeselsph_z = 0;
   nodeselsph_r = 0;  nodeselbox_on = 0;
   nodeselbox_xmin = 0; nodeselbox_xmax = 0;
   nodeselbox_ymin = 0; nodeselbox_ymax = 0;
   nodeselbox_zmin = 0; nodeselbox_zmax = 0; nodeselbox_in = 1;
   cellselfield = 0;  cellselnfield = 0;
   cfldselmin = 0.;  cfldselmax = 0.;
   cnfldselmin = 0.;  cnfldselmax = 0.;
   cellselsph_on = 0;  cellselsph_in = 1;  
   cellselsph_x = 0;  cellselsph_y = 0;  cellselsph_z = 0;
   cellselsph_r = 0;  cellselbox_on = 0;
   cellselbox_xmin = 0; cellselbox_xmax = 0;
   cellselbox_ymin = 0; cellselbox_ymax = 0;
   cellselbox_zmin = 0; cellselbox_zmax = 0; cellselbox_in = 0;
   surfselfield = 0;  surfselnfield = 0;
   sfldselmin = 0.;  sfldselmax = 0.;
   snfldselmin = 0.;  snfldselmax = 0.;
   surfselsph_on = 0;  surfselsph_in = 1;  
   surfselsph_x = 0;  surfselsph_y = 0;  surfselsph_z = 0;
   surfselsph_r = 0;  surfselbox_on = 0;
   surfselbox_xmin = 0; surfselbox_xmax = 0;
   surfselbox_ymin = 0; surfselbox_ymax = 0;
   surfselbox_zmin = 0; surfselbox_zmax = 0; surfselbox_in = 0;
   fldcalc_has_data = 0;  cfldcalc_has_data = 0;
   node0matflag = 0;  cell0matflag = 0;  surf0matflag = 0;
   polystiplflag = 0;  drawghosts = 0;  drawghostface = 1;
   drawnodeghosts = 0;  drawunselnodes= 0;  drawunselcells = 0;

   traceselsph_on = 0;  traceselsph_in = 1;  
   traceselsph_x = 0;  traceselsph_y = 0;  traceselsph_z = 0;
   traceselsph_r = 0;  traceselbox_on = 0;
   traceselbox_xmin = 0; traceselbox_xmax = 0;
   traceselbox_ymin = 0; traceselbox_ymax = 0;
   traceselbox_zmin = 0; traceselbox_zmax = 0; traceselbox_in = 1;

   XmToggleButtonGadgetSetState(objxrefl,FALSE,FALSE);
   XmToggleButtonGadgetSetState(objyrefl,FALSE,FALSE);
   XmToggleButtonGadgetSetState(objzrefl,FALSE,FALSE);

   /*  Unmanage all form widgets before re-generating forms.  */
   popdownall();

   /*  Destroy form widgets before re-generating forms.  */
   XtDestroyWidget(animationform);
   if (ncells > 0)
     {
      XtDestroyWidget(cellform);
      if (maxcellmat > 1)
         XtDestroyWidget(isosurfformm);
      if (numvars > 0)
        {
         XtDestroyWidget(isosurfformf);
        }
      XtDestroyWidget(gridanalform);
      XtDestroyWidget(concaveform);
     }
   XtDestroyWidget(coloreditform);
   if (nnodes > 0)
     {
      XtDestroyWidget(nodeform);
      XtDestroyWidget(queryform);
     }
   if (polysin > 0)
     {
      XtDestroyWidget(polygonform);
      XtDestroyWidget(polysubsetform);
     }
   if (ntracers > 0)
     {
      XtDestroyWidget(traceform);
      XtDestroyWidget(tracelimform);
     }
   if (numvars > 0)
     {
      XtDestroyWidget(nfieldform);
      XtDestroyWidget(fieldlimform);
      /* XtDestroyWidget(cutlineform); */
      XtDestroyWidget(fldcalcform);
     }
   if (cnumvars > 0)
     {
      XtDestroyWidget(cfieldform);
      XtDestroyWidget(cfieldlimform);
      XtDestroyWidget(cfldcalcform);
     }
   if (nsurf > 0)
      XtDestroyWidget(surfform);
   if (snumvars > 0)
     {
      XtDestroyWidget(sfieldform);
      XtDestroyWidget(sfieldlimform);
      XtDestroyWidget(sfldcalcform);
     }
   if (fnumvars > 0)
     {
      XtDestroyWidget(ffieldform);
      XtDestroyWidget(ffieldlimform);
     }

#ifdef PHYSED
   destroyPEforms();
#endif

   XFlush(dpy);
}

void makewidgets()
{
  XmString msgstr;
  void polygonformpop(), cellformpop(), nodeformpop(), surfformpop();


#ifdef PHYSED
   msgstr = XmStringLtoRCreate("PHYSED generating custom menus: Display",
                           XmSTRING_DEFAULT_CHARSET);
#else
   msgstr = XmStringLtoRCreate("GMV generating custom menus: Display",
                           XmSTRING_DEFAULT_CHARSET);
#endif
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);
   XFlush(dpy);
   XSync(dpy, 0);

   /*  Make Display menus.  */
   if (polysin > 0)
      makepolygonform(poplevel);
   if (nnodes > 0)
      makenodeform(poplevel);
   if (ncells > 0)
      makecellform(poplevel);
   if (nsurf > 0)
      makesurfform(poplevel);
   if (ntracers > 0)
      maketraceform(poplevel);
   if (numvars > 0)
      makenfieldform(poplevel);
   if (cnumvars > 0)
      makecfieldform(poplevel);
   if (snumvars > 0)
      makesfieldform(poplevel);
   if (fnumvars > 0)
      makeffieldform(poplevel);
   if (numvartr > 0)
      maketrfieldform(poplevel);

#ifdef PHYSED
   msgstr = XmStringLtoRCreate("PHYSED generating custom menus: Calculate",
                           XmSTRING_DEFAULT_CHARSET);
#else
   msgstr = XmStringLtoRCreate("GMV generating custom menus: Calculate",
                           XmSTRING_DEFAULT_CHARSET);
#endif
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);
   XFlush(dpy);
   XSync(dpy, 0);

   /*  Make Calculate menus.  */
   if (ncells > 0)
     {
      makeisosurfform(poplevel);
/*
      if (ncells > 0 && numvars > 0)
        {
         makecutplaneform(poplevel);
         makecutsphereform(poplevel);
         makeisovolform(poplevel);
        }
*/
      makegridanalform(poplevel);
      makeconcaveform(poplevel);
     }
   if (numvars > 0)
     {
      /* makecutlineform(poplevel); */
      makefldcalcform(poplevel);
     }
   if (cnumvars > 0)
      makecfldcalcform(poplevel);
   if (snumvars > 0)
      makesfldcalcform(poplevel);
   if (nnodes > 0)
      makequeryform(poplevel);
   if (numvartr > 0)
      maketfldcalcform(poplevel);

#ifdef PHYSED
   msgstr = XmStringLtoRCreate("PHYSED generating custom menus: Controls",
                           XmSTRING_DEFAULT_CHARSET);
#else
   msgstr = XmStringLtoRCreate("GMV generating custom menus: Controls",
                           XmSTRING_DEFAULT_CHARSET);
#endif
   XtVaSetValues(gmvmsglab,XmNlabelString,msgstr,NULL);
   XmStringFree(msgstr);
   XmUpdateDisplay(gmvmsglab);
   XFlush(dpy);
   XSync(dpy, 0);

   /*  Make Control menus.  */
   makeanimationform(poplevel);
   makecoloreditform(poplevel);
   if (ntracers > 0)
      maketracelimform(poplevel2);
   if (numvars > 0)
      makefieldlimform(poplevel2);
   if (cnumvars > 0)
      makecfieldlimform(poplevel2);
   if (fnumvars > 0)
      makeffieldlimform(poplevel2);
   if (snumvars > 0)
      makesfieldlimform(poplevel2);

   /*  Set widgets for new data. */
   XUndefineCursor(dpy,topwin);
/*
   if (numvars > 0)
     {
      XtManageChild(nfieldform);
      XtUnmanageChild(nfieldform);
     }

   if (cnumvars > 0)
     {
      XtManageChild(cfieldform);
      XtUnmanageChild(cfieldform);
     }

   if (snumvars > 0)
     {
      XtManageChild(sfieldform);
      XtUnmanageChild(sfieldform);
     }

   if (fnumvars > 0)
     {
      XtManageChild(ffieldform);
      XtUnmanageChild(ffieldform);
     }
*/
   if (polysflag == 1) polygonformpop();
   if (cellsflag == 1 || celledgesflag == 1) cellformpop();
   if (nodesflag == 1) nodeformpop();
   if (surfflag == 1 || surfedgesflag == 1) surfformpop();

   if (ntracers <= 0) XtSetSensitive(disptrace,FALSE);
   else XtSetSensitive(disptrace,TRUE);

   if (polysin == 0)
     {
      XtSetSensitive(disppolys,FALSE);
      XtSetSensitive(polysubset,FALSE);
     }
   else
     {
      XtSetSensitive(disppolys,TRUE);
      XtSetSensitive(polysubset,TRUE);
     }

   if (numvars <= 0)
     {
      XtSetSensitive(fieldlimbut,FALSE);
      XtSetSensitive(calccutl,FALSE);
      XtSetSensitive(calcnfldcalc,FALSE);
      XtSetSensitive(calcavgnfld,FALSE);
      XtSetSensitive(nwrtvars,FALSE);
     }
   else
     {
      XtSetSensitive(fieldlimbut,TRUE);
      XtSetSensitive(calccutl,TRUE);
      XtSetSensitive(calcnfldcalc,TRUE);
      XtSetSensitive(calcavgnfld,TRUE);
      XtSetSensitive(nwrtvars,TRUE);
     }

   if (cnumvars <= 0)
     {
      XtSetSensitive(cfieldlimbut,FALSE);
      XtSetSensitive(calccfldcalc,FALSE);
      XtSetSensitive(calcavgcfld,FALSE);
      XtSetSensitive(cwrtvars,FALSE);
     }
   else
     {
      XtSetSensitive(cfieldlimbut,TRUE);
      XtSetSensitive(calccfldcalc,TRUE);
      XtSetSensitive(calcavgcfld,TRUE);
      XtSetSensitive(cwrtvars,TRUE);
     }

   if (numvartr <= 0)
     {
      XtSetSensitive(calctfldcalc,FALSE);
      XtSetSensitive(twrtvars,FALSE);
     }
   else
     {
      XtSetSensitive(calctfldcalc,TRUE);
      XtSetSensitive(twrtvars,TRUE);
     }

   if (snumvars <= 0)
     {
      XtSetSensitive(sfieldlimbut,FALSE);
      XtSetSensitive(calcsfldcalc,FALSE);
      XtSetSensitive(swrtvars,FALSE);
     }
   else
     {
      XtSetSensitive(sfieldlimbut,TRUE);
      XtSetSensitive(calcsfldcalc,TRUE);
      XtSetSensitive(swrtvars,TRUE);
     }

   if (fnumvars <= 0)
     {
      XtSetSensitive(ffieldlimbut,FALSE);
     }
   else
     {
      XtSetSensitive(ffieldlimbut,TRUE);
     }

   if (numvars <= 0 && cnumvars <=0) 
      XtSetSensitive(calfcalc,FALSE);
   else
      XtSetSensitive(calfcalc,TRUE);

   if (ntracers <= 0) XtSetSensitive(tracelimbut,FALSE);
   else XtSetSensitive(tracelimbut,TRUE);

   if (ncells <= 0)
     {
      XtSetSensitive(dispcells,FALSE);
      XtSetSensitive(calciso,FALSE);
      XtSetSensitive(calccutp,FALSE);
      XtSetSensitive(calccuts,FALSE);
      XtSetSensitive(calcganal,FALSE);
      XtSetSensitive(calcisovol,FALSE);
     }
   else
     {
      XtSetSensitive(dispcells,TRUE);
      XtSetSensitive(calciso,TRUE);
      XtSetSensitive(calccutp,TRUE);
      XtSetSensitive(calccuts,TRUE);
      XtSetSensitive(calcganal,TRUE);
      XtSetSensitive(calcisoflds,TRUE);
      XtSetSensitive(calcisomats,TRUE);
      XtSetSensitive(calcisovol,TRUE);
     }

   if (nnodes <= 0)
     {
      XtSetSensitive(dispnodes,FALSE);
      XtSetSensitive(calcquery,FALSE);
     }
   else
     {
      XtSetSensitive(dispnodes,TRUE);
      XtSetSensitive(calcquery,TRUE);
     }

   if (ncells > 0 && numvars <= 0)
     {
      XtSetSensitive(calccutp,FALSE);
      XtSetSensitive(calccuts,FALSE);
      XtSetSensitive(calcisovol,FALSE);
     }

   if (ncells > 0)
     {
      if (numvars <= 0 && maxcellmat < 2)
         XtSetSensitive(calciso,FALSE);
      else
        {
         if (numvars <= 0) 
            XtSetSensitive(calcisoflds,FALSE);
         /* if (maxcellmat < 2) 
            XtSetSensitive(calcisomats,FALSE);  */
        }
     }

   if (nsurf <= 0) XtSetSensitive(dispsurfs,FALSE);
   else XtSetSensitive(dispsurfs,TRUE);

#ifdef PHYSED
   /*  Make PhysEd material edit menus.  */
   /* makePEmatforms(); */
#endif
   XFlush(dpy);
}


void save_attr_data()
{
  int i, j;

   /*  When reading another sequence from the same problem,     */
   /*  save attributes information that will be changed in the  */
   /*  get_statistics routine.                                  */

   sav_xrange = xrange;  sav_yrange = yrange;  sav_zrange = zrange;
   sav_xtrans = xtrans;  sav_ytrans = ytrans;  sav_ztrans = ztrans;
   sav_span = span;  
   sav_globxicmin = globxicmin;  sav_globxicmax = globxicmax;  
   sav_globyicmin = globyicmin;  sav_globyicmax = globyicmax;
   sav_globzicmin = globzicmin;  sav_globzicmax = globzicmax;
   sav_globxicmino = globxicmino;  sav_globxicmaxo = globxicmaxo;
   sav_globyicmino = globyicmino;  sav_globyicmaxo = globyicmaxo;
   sav_globzicmino = globzicmino;  sav_globzicmaxo = globzicmaxo;
   sav_xminfdval = xminfdval;  sav_xmaxfdval = xmaxfdval;
   sav_yminfdval = yminfdval;  sav_ymaxfdval = ymaxfdval;
   sav_zminfdval = zminfdval;  sav_zmaxfdval = zmaxfdval;

   sav_xminbdval = xminbdval;  sav_xmaxbdval = xmaxbdval;
   sav_yminbdval = yminbdval;  sav_ymaxbdval = ymaxbdval;
   sav_zminbdval = zminbdval;  sav_zmaxbdval = zmaxbdval;

   sav_xminbbval = xminbbval;  sav_xmaxbbval = xmaxbbval;
   sav_yminbbval = yminbbval;  sav_ymaxbbval = ymaxbbval;
   sav_zminbbval = zminbbval;  sav_zmaxbbval = zmaxbbval;
   sav_xminbbvalo = xminbbvalo;  sav_xmaxbbvalo = xmaxbbvalo;
   sav_yminbbvalo = yminbbvalo;  sav_ymaxbbvalo = ymaxbbvalo;
   sav_zminbbvalo = zminbbvalo;  sav_zmaxbbvalo = zmaxbbvalo;

   for (i = 0; i < MAXFIELDS; i++)
     {
      sav_globfieldmin[i] = globfieldmin[i];
      sav_globfieldmax[i] = globfieldmax[i];
      sav_globfieldcmin[i] = globfieldcmin[i];
      sav_globfieldcmax[i] = globfieldcmax[i];
      sav_nfieldlimauto[i] = nfieldlimauto[i];
      sav_cfieldlimauto[i] = cfieldlimauto[i];
      sav_sfieldlimauto[i] = sfieldlimauto[i];
      sav_ffieldlimauto[i] = ffieldlimauto[i];
      sav_nfld_nice_min[i] = nfld_nice_min[i];
      sav_cfld_nice_min[i] = cfld_nice_min[i];
      sav_sfld_nice_min[i] = sfld_nice_min[i];
      sav_ffld_nice_min[i] = ffld_nice_min[i];
      sav_nfld_nice_max[i] = nfld_nice_max[i];
      sav_cfld_nice_max[i] = cfld_nice_max[i];
      sav_sfld_nice_max[i] = sfld_nice_max[i];
      sav_ffld_nice_max[i] = ffld_nice_max[i];
      sav_nfld_nice[i] = nfld_nice[i];
      sav_cfld_nice[i] = cfld_nice[i];
      sav_sfld_nice[i] = sfld_nice[i];
      sav_ffld_nice[i] = ffld_nice[i];
      sav_nfld_log[i] = nfld_log[i];
      sav_cfld_log[i] = cfld_log[i];
      sav_sfld_log[i] = sfld_log[i];
      sav_ffld_log[i] = ffld_log[i];
     }
   for (i = 0; i < MAXTRFIELDS; i++)
     {
      sav_globfieldtrmin[i] = globfieldtrmin[i];
      sav_globfieldtrmax[i] = globfieldtrmax[i];
      sav_trfieldlimauto[i] = trfieldlimauto[i];
      sav_trfld_nice_min[i] = trfld_nice_min[i];
      sav_trfld_nice_max[i] = trfld_nice_max[i];
      sav_trfld_nice[i] = trfld_nice[i];
      sav_trfld_log[i] = trfld_log[i];
     }

   sav_nmat_order = nmat_order;
   for (i = 0; i < nmat_order; i++)
      sav_mat_order[i] = mat_order[i];
   for (i = 0; i < MAXMATS; i++)
      sav_active_mat[i] = active_mat[i];
   sav_vectfldx = vectfldx;
   sav_vectfldy = vectfldy;
   sav_vectfldz = vectfldz;
   sav_speedfld = speedfld;
   sav_cvectfldx = cvectfldx;
   sav_cvectfldy = cvectfldy;
   sav_cvectfldz = cvectfldz;
   sav_cspeedfld = cspeedfld;
   sav_fvectfldx = fvectfldx;
   sav_fvectfldy = fvectfldy;
   sav_fvectfldz = fvectfldz;
   sav_fspeedfld = fspeedfld;
   sav_svectfldx = svectfldx;
   sav_svectfldy = svectfldy;
   sav_svectfldz = svectfldz;
   sav_sspeedfld = sspeedfld;
   sav_velscale = velscale;
   sav_velarrowsize = velarrowsize;
   sav_maxspeed = maxspeed;
   sav_cmaxspeed = cmaxspeed;
   sav_fmaxspeed = fmaxspeed;

   sav_polysflag = polysflag;
   sav_linesflag = linesflag;
   sav_cellsflag = cellsflag;
   sav_celledgesflag = celledgesflag;
   sav_cellhiddenedgesflag = cellhiddenedgesflag;
   sav_nodesflag = nodesflag;
   sav_vectorflag = vectorflag;
   sav_cvectorflag = cvectorflag;
   sav_fvectorflag = fvectorflag;
   sav_svectorflag = svectorflag;
   sav_timeflag = timeflag;
   sav_cycleflag = cycleflag;

   sav_cellselmat_on = cellselmat_on;
   sav_cellselcfld_on = cellselcfld_on;
   sav_cellselnfld_on = cellselnfld_on;
   sav_cellselsph_on = cellselsph_on;
   sav_cellselsph_in = cellselsph_in;
   sav_cellselbox_on = cellselbox_on;
   sav_cellselbox_in = cellselbox_in;
   sav_cellselnum_on = cellselnum_on;
   sav_cellselfield = cellselfield;
   sav_cellselnfield = cellselnfield;
   sav_cellselnfldopt = cellselnfldopt;
   sav_cfldselmin = cfldselmin;
   sav_cfldselmax = cfldselmax;
   sav_cnfldselmin = cnfldselmin;
   sav_cnfldselmax = cnfldselmax;
   sav_cellselsph_x = cellselsph_x;
   sav_cellselsph_y = cellselsph_y;
   sav_cellselsph_z = cellselsph_z;
   sav_cellselsph_r = cellselsph_r;
   sav_cellselbox_xmin = cellselbox_xmin;
   sav_cellselbox_xmax = cellselbox_xmax;
   sav_cellselbox_ymin = cellselbox_ymin;
   sav_cellselbox_ymax = cellselbox_ymax;
   sav_cellselbox_zmin = cellselbox_zmin;
   sav_cellselbox_zmax = cellselbox_zmax;
   sav_ncellselnum = ncellselnum;
   sav_cellselcellgrp_on = cellselcellgrp_on;
   sav_ncellgrpselected = ncellgrpselected;
   sav_cellselgrp_in = cellselgrp_in;
   sav_cellselfacegrp_on = cellselfacegrp_on;
   sav_nfacegrpselected = nfacegrpselected;
   sav_faceselgrp_in = faceselgrp_in;

   sav_nodeselmat_on = nodeselmat_on;
   sav_nodeselfld_on = nodeselfld_on;
   sav_nodeselsph_on = nodeselsph_on;
   sav_nodeselsph_in = nodeselsph_in;
   sav_nodeselbox_on = nodeselbox_on;
   sav_nodeselbox_in = nodeselbox_in;
   sav_nodeselnum_on = nodeselnum_on;
   sav_nodeselfield = nodeselfield;
   sav_nfldselmin = nfldselmin;
   sav_nfldselmax = nfldselmax;
   sav_nodeselsph_x = nodeselsph_x;
   sav_nodeselsph_y = nodeselsph_y;
   sav_nodeselsph_z = nodeselsph_z;
   sav_nodeselsph_r = nodeselsph_r;
   sav_nodeselbox_xmin = nodeselbox_xmin;
   sav_nodeselbox_xmax = nodeselbox_xmax;
   sav_nodeselbox_ymin = nodeselbox_ymin;
   sav_nodeselbox_ymax = nodeselbox_ymax;
   sav_nodeselbox_zmin = nodeselbox_zmin;
   sav_nodeselbox_zmax = nodeselbox_zmax;
   sav_nnodeselnum = nnodeselnum;
   sav_nodeselnodegrp_on = nodeselnodegrp_on;
   sav_nnodegrpselected = nnodegrpselected;
   sav_nodeselgrp_in = nodeselgrp_in;

   sav_traceselfld_on = traceselfld_on;
   sav_traceselsph_on = traceselsph_on;
   sav_traceselsph_in = traceselsph_in;
   sav_traceselbox_on = traceselbox_on;
   sav_traceselbox_in = traceselbox_in;
   sav_traceselnum_on = traceselnum_on;
   sav_traceselfield = traceselfield;
   for (i = 0; i < numvartr; i++)
     {
      sav_traceselfields[i] = traceselfields[i];
      sav_trfldselmin[i] = trfldselmin[i];
      sav_trfldselmax[i] = trfldselmax[i];
     }
   sav_traceselsph_x = traceselsph_x;
   sav_traceselsph_y = traceselsph_y;
   sav_traceselsph_z = traceselsph_z;
   sav_traceselsph_r = traceselsph_r;
   sav_traceselbox_xmin = traceselbox_xmin;
   sav_traceselbox_xmax = traceselbox_xmax;
   sav_traceselbox_ymin = traceselbox_ymin;
   sav_traceselbox_ymax = traceselbox_ymax;
   sav_traceselbox_zmin = traceselbox_zmin;
   sav_traceselbox_zmax = traceselbox_zmax;
   sav_ntraceselnum = ntraceselnum;

   sav_surfselmat_on = surfselmat_on;
   sav_surfselsfld_on = surfselsfld_on;
   sav_surfselnfld_on = surfselnfld_on;
   sav_surfselsph_on = surfselsph_on;
   sav_surfselsph_in = surfselsph_in;
   sav_surfselbox_on = surfselbox_on;
   sav_surfselbox_in = surfselbox_in;
   sav_surfselnum_on = surfselnum_on;
   sav_surfselfield = surfselfield;
   sav_surfselnfield = surfselnfield;
   sav_surfselnfldopt = surfselnfldopt;
   sav_sfldselmin = sfldselmin;
   sav_sfldselmax = sfldselmax;
   sav_snfldselmin = snfldselmin;
   sav_snfldselmax = snfldselmax;
   sav_surfselsph_x = surfselsph_x;
   sav_surfselsph_y = surfselsph_y;
   sav_surfselsph_z = surfselsph_z;
   sav_surfselsph_r = surfselsph_r;
   sav_surfselbox_xmin = surfselbox_xmin;
   sav_surfselbox_xmax = surfselbox_xmax;
   sav_surfselbox_ymin = surfselbox_ymin;
   sav_surfselbox_ymax = surfselbox_ymax;
   sav_surfselbox_zmin = surfselbox_zmin;
   sav_surfselbox_zmax = surfselbox_zmax;
   sav_nsurfselnum = nsurfselnum;
   sav_surfselsurfgrp_on = surfselsurfgrp_on;
   sav_nsurfgrpselected = nsurfgrpselected;
   sav_surfselgrp_in = surfselgrp_in;
   sav_drawghosts = drawghosts;
   sav_drawghostface = drawghostface;
   sav_drawnodeghosts = drawnodeghosts;

   for (i = 0; i < MAXFLAGS+1; i++)
     {
      sav_nodeandorflag[i] = nodeandorflag[i];
      sav_cellandorflag[i] = cellandorflag[i];
      sav_surfandorflag[i] = surfandorflag[i];
      for (j = 0; j < MAXMATS; j++)
        {
         sav_nodeselflag[i][j] = nodeselflag[i][j];
         sav_cellselflag[i][j] = cellselflag[i][j];
         sav_surfselflag[i][j] = surfselflag[i][j];
        }
     }

   sav_traceselnum_on = traceselnum_on;
   sav_ntraceselnum = ntraceselnum;

   sav_contourfield_draw = contourfield_draw;
   sav_mcontourfield_draw = mcontourfield_draw;
   sav_isovolfield_draw = isovolfield_draw;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      sav_fieldcalc[i] = fieldcalc[i];
      sav_cfieldcalc[i] = cfieldcalc[i];
      sav_sfieldcalc[i] = sfieldcalc[i];
     }

   for (i = 0; i < MAXCUTLINES; i++)
      sav_cutlineradius[i] = cutlineradius[i];
}


void get_attr_data()
{
  int i, j;

   /*  When reading another sequence from the same problem, get  */
   /*  saved attributes information and set the attributes for   */
   /*  the new file.                                             */

   xrange = sav_xrange;  yrange = sav_yrange;  zrange = sav_zrange;
   xtrans = sav_xtrans;  ytrans = sav_ytrans;  ztrans = sav_ztrans;
   span = sav_span;  
   globxicmin = sav_globxicmin;  globxicmax = sav_globxicmax;  
   globyicmin = sav_globyicmin;  globyicmax = sav_globyicmax;
   globzicmin = sav_globzicmin;  globzicmax = sav_globzicmax;
   globxicmino = sav_globxicmino;  globxicmaxo = sav_globxicmaxo;
   globyicmino = sav_globyicmino;  globyicmaxo = sav_globyicmaxo;
   globzicmino = sav_globzicmino;  globzicmaxo = sav_globzicmaxo;
   xminfdval = sav_xminfdval;  xmaxfdval = sav_xmaxfdval;
   yminfdval = sav_yminfdval;  ymaxfdval = sav_ymaxfdval;
   zminfdval = sav_zminfdval;  zmaxfdval = sav_zmaxfdval;

   xminbdval = sav_xminbdval;  xmaxbdval = sav_xmaxbdval;
   yminbdval = sav_yminbdval;  ymaxbdval = sav_ymaxbdval;
   zminbdval = sav_zminbdval;  zmaxbdval = sav_zmaxbdval;

   xminbbval = sav_xminbbval;  xmaxbbval = sav_xmaxbbval;
   yminbbval = sav_yminbbval;  ymaxbbval = sav_ymaxbbval;
   zminbbval = sav_zminbbval;  zmaxbbval = sav_zmaxbbval;
   xminbbvalo = sav_xminbbvalo;  xmaxbbvalo = sav_xmaxbbvalo;
   yminbbvalo = sav_yminbbvalo;  ymaxbbvalo = sav_ymaxbbvalo;
   zminbbvalo = sav_zminbbvalo;  zmaxbbvalo = sav_zmaxbbvalo;

   for (i = 0; i < MAXFIELDS; i++)
     {
      globfieldmin[i] = sav_globfieldmin[i];
      globfieldmax[i] = sav_globfieldmax[i];
      globfieldcmin[i] = sav_globfieldcmin[i];
      globfieldcmax[i] = sav_globfieldcmax[i];
      nfieldlimauto[i] = sav_nfieldlimauto[i];
      cfieldlimauto[i] = sav_cfieldlimauto[i];
      sfieldlimauto[i] = sav_sfieldlimauto[i];
      ffieldlimauto[i] = sav_ffieldlimauto[i];
      nfld_nice_min[i] = sav_nfld_nice_min[i];
      cfld_nice_min[i] = sav_cfld_nice_min[i];
      sfld_nice_min[i] = sav_sfld_nice_min[i];
      ffld_nice_min[i] = sav_ffld_nice_min[i];
      nfld_nice_max[i] = sav_nfld_nice_max[i];
      cfld_nice_max[i] = sav_cfld_nice_max[i];
      sfld_nice_max[i] = sav_sfld_nice_max[i];
      ffld_nice_max[i] = sav_ffld_nice_max[i];
      nfld_nice[i] = sav_nfld_nice[i];
      cfld_nice[i] = sav_cfld_nice[i];
      sfld_nice[i] = sav_sfld_nice[i];
      ffld_nice[i] = sav_ffld_nice[i];
      nfld_log[i] = sav_nfld_log[i];
      cfld_log[i] = sav_cfld_log[i];
      sfld_log[i] = sav_sfld_log[i];
      ffld_log[i] = sav_ffld_log[i];
     }
   for (i = 0; i < MAXTRFIELDS; i++)
     {
      globfieldtrmin[i] = sav_globfieldtrmin[i];
      globfieldtrmax[i] = sav_globfieldtrmax[i];
      trfieldlimauto[i] = sav_trfieldlimauto[i];
      trfld_nice_min[i] = sav_trfld_nice_min[i];
      trfld_nice_max[i] = sav_trfld_nice_max[i];
      trfld_nice[i] = sav_trfld_nice[i];
      trfld_log[i] = sav_trfld_log[i];
     }

   for (i = 0; i < MAXFIELDS; i++)
     {
      if (nfieldlimauto[i])
        {
         globfieldmin[i] = fieldmin[i];
         globfieldmax[i] = fieldmax[i];
         nfld_nice_min[i] = fieldmin[i];
         nfld_nice_max[i] = fieldmax[i];
        }
      if (cfieldlimauto[i])
        {
         globfieldcmin[i] = cfieldmin[i];
         globfieldcmax[i] = cfieldmax[i];
         cfld_nice_min[i] = cfieldmin[i];
         cfld_nice_max[i] = cfieldmax[i];
        }
      if (sfieldlimauto[i])
        {
         globfieldsmin[i] = sfieldmin[i];
         globfieldsmax[i] = sfieldmax[i];
         sfld_nice_min[i] = sfieldmin[i];
         sfld_nice_max[i] = sfieldmax[i];
        }
      if (ffieldlimauto[i])
        {
         globfieldfmin[i] = ffieldmin[i];
         globfieldfmax[i] = ffieldmax[i];
         ffld_nice_min[i] = ffieldmin[i];
         ffld_nice_max[i] = ffieldmax[i];
        }
     }
   for (i = 0; i < MAXTRFIELDS; i++)
     {
      if (trfieldlimauto[i])
        {
         globfieldtrmin[i] = fieldtrmin[i];
         globfieldtrmax[i] = fieldtrmax[i];
         trfld_nice_min[i] = fieldtrmin[i];
         trfld_nice_max[i] = fieldtrmax[i];
        }
     }

   nmat_order = sav_nmat_order;
   for (i = 0; i < nmat_order; i++)
      mat_order[i] = sav_mat_order[i];
   for (i = 0; i < MAXMATS; i++)
      active_mat[i] = sav_active_mat[i];
   vectfldx = sav_vectfldx;
   vectfldy = sav_vectfldy;
   vectfldz = sav_vectfldz;
   speedfld = sav_speedfld;
   cvectfldx = sav_cvectfldx;
   cvectfldy = sav_cvectfldy;
   cvectfldz = sav_cvectfldz;
   cspeedfld = sav_cspeedfld;
   fvectfldx = sav_fvectfldx;
   fvectfldy = sav_fvectfldy;
   fvectfldz = sav_fvectfldz;
   fspeedfld = sav_fspeedfld;
   svectfldx = sav_svectfldx;
   svectfldy = sav_svectfldy;
   svectfldz = sav_svectfldz;
   sspeedfld = sav_sspeedfld;
   velscale = sav_velscale;
   velarrowsize = sav_velarrowsize;
   maxspeed = sav_maxspeed;
   cmaxspeed = sav_cmaxspeed;
   fmaxspeed = sav_fmaxspeed;

   polysflag = sav_polysflag;
   linesflag = sav_linesflag;
   cellsflag = sav_cellsflag;
   celledgesflag = sav_celledgesflag;
   cellhiddenedgesflag = sav_cellhiddenedgesflag;
   nodesflag = sav_nodesflag;
   vectorflag = sav_vectorflag;
   cvectorflag = sav_cvectorflag;
   fvectorflag = sav_fvectorflag;
   svectorflag = sav_svectorflag;
   timeflag = sav_timeflag;
   cycleflag = sav_cycleflag;

   cellselmat_on = sav_cellselmat_on;
   cellselcfld_on = sav_cellselcfld_on;
   cellselnfld_on = sav_cellselnfld_on;
   cellselsph_on = sav_cellselsph_on;
   cellselsph_in = sav_cellselsph_in;
   cellselbox_on = sav_cellselbox_on;
   cellselbox_in = sav_cellselbox_in;
   cellselnum_on = sav_cellselnum_on;
   cellselfield = sav_cellselfield;
   cfldselmin = sav_cfldselmin;
   cfldselmax = sav_cfldselmax;
   cellselnfield = sav_cellselnfield;
   cellselnfldopt = sav_cellselnfldopt;
   cnfldselmin = sav_cnfldselmin;
   cnfldselmax = sav_cnfldselmax;
   cellselsph_x = sav_cellselsph_x;
   cellselsph_y = sav_cellselsph_y;
   cellselsph_z = sav_cellselsph_z;
   cellselsph_r = sav_cellselsph_r;
   cellselbox_xmin = sav_cellselbox_xmin;
   cellselbox_xmax = sav_cellselbox_xmax;
   cellselbox_ymin = sav_cellselbox_ymin;
   cellselbox_ymax = sav_cellselbox_ymax;
   cellselbox_zmin = sav_cellselbox_zmin;
   cellselbox_zmax = sav_cellselbox_zmax;
   ncellselnum = sav_ncellselnum;
   cellselcellgrp_on = sav_cellselcellgrp_on;
   ncellgrpselected = sav_ncellgrpselected;
   cellselgrp_in = sav_cellselgrp_in;
   cellselfacegrp_on = sav_cellselfacegrp_on;
   nfacegrpselected = sav_nfacegrpselected;
   faceselgrp_in = sav_faceselgrp_in;

   nodeselmat_on = sav_nodeselmat_on;
   nodeselfld_on = sav_nodeselfld_on;
   nodeselsph_on = sav_nodeselsph_on;
   nodeselsph_in = sav_nodeselsph_in;
   nodeselbox_on = sav_nodeselbox_on;
   nodeselbox_in = sav_nodeselbox_in;
   nodeselnum_on = sav_nodeselnum_on;
   nodeselfield = sav_nodeselfield;
   nfldselmin = sav_nfldselmin;
   nfldselmax = sav_nfldselmax;
   nodeselsph_x = sav_nodeselsph_x;
   nodeselsph_y = sav_nodeselsph_y;
   nodeselsph_z = sav_nodeselsph_z;
   nodeselsph_r = sav_nodeselsph_r;
   nodeselbox_xmin = sav_nodeselbox_xmin;
   nodeselbox_xmax = sav_nodeselbox_xmax;
   nodeselbox_ymin = sav_nodeselbox_ymin;
   nodeselbox_ymax = sav_nodeselbox_ymax;
   nodeselbox_zmin = sav_nodeselbox_zmin;
   nodeselbox_zmax = sav_nodeselbox_zmax;
   nnodeselnum = sav_nnodeselnum;
   nodeselnodegrp_on = sav_nodeselnodegrp_on;
   nnodegrpselected = sav_nnodegrpselected;
   nodeselgrp_in = sav_nodeselgrp_in;

   traceselfld_on = sav_traceselfld_on;
   traceselsph_on = sav_traceselsph_on;
   traceselsph_in = sav_traceselsph_in;
   traceselbox_on = sav_traceselbox_on;
   traceselbox_in = sav_traceselbox_in;
   traceselnum_on = sav_traceselnum_on;
   traceselfield = sav_traceselfield;
   for (i = 0; i < numvartr; i++)
     {
      traceselfields[i] = sav_traceselfields[i];
      trfldselmin[i] = sav_trfldselmin[i];
      trfldselmax[i] = sav_trfldselmax[i];
     }
   traceselsph_x = sav_traceselsph_x;
   traceselsph_y = sav_traceselsph_y;
   traceselsph_z = sav_traceselsph_z;
   traceselsph_r = sav_traceselsph_r;
   traceselbox_xmin = sav_traceselbox_xmin;
   traceselbox_xmax = sav_traceselbox_xmax;
   traceselbox_ymin = sav_traceselbox_ymin;
   traceselbox_ymax = sav_traceselbox_ymax;
   traceselbox_zmin = sav_traceselbox_zmin;
   traceselbox_zmax = sav_traceselbox_zmax;
   ntraceselnum = sav_ntraceselnum;

   surfselmat_on = sav_surfselmat_on;
   surfselsfld_on = sav_surfselsfld_on;
   surfselnfld_on = sav_surfselnfld_on;
   surfselsph_on = sav_surfselsph_on;
   surfselsph_in = sav_surfselsph_in;
   surfselbox_on = sav_surfselbox_on;
   surfselbox_in = sav_surfselbox_in;
   surfselnum_on = sav_surfselnum_on;
   surfselfield = sav_surfselfield;
   sfldselmin = sav_sfldselmin;
   sfldselmax = sav_sfldselmax;
   snfldselmin = sav_snfldselmin;
   snfldselmax = sav_snfldselmax;
   surfselnfield = sav_surfselnfield;
   surfselnfldopt = sav_surfselnfldopt;
   cnfldselmin = sav_cnfldselmin;
   cnfldselmax = sav_cnfldselmax;
   surfselsph_x = sav_surfselsph_x;
   surfselsph_y = sav_surfselsph_y;
   surfselsph_z = sav_surfselsph_z;
   surfselsph_r = sav_surfselsph_r;
   surfselbox_xmin = sav_surfselbox_xmin;
   surfselbox_xmax = sav_surfselbox_xmax;
   surfselbox_ymin = sav_surfselbox_ymin;
   surfselbox_ymax = sav_surfselbox_ymax;
   surfselbox_zmin = sav_surfselbox_zmin;
   surfselbox_zmax = sav_surfselbox_zmax;
   nsurfselnum = sav_nsurfselnum;
   surfselsurfgrp_on = sav_surfselsurfgrp_on;
   nsurfgrpselected = sav_nsurfgrpselected;
   surfselgrp_in = sav_surfselgrp_in;
   drawghosts = sav_drawghosts;
   drawghostface = sav_drawghostface;
   drawnodeghosts = sav_drawnodeghosts;

   for (i = 0; i < MAXFLAGS+1; i++)
     {
      nodeandorflag[i] = sav_nodeandorflag[i];
      cellandorflag[i] = sav_cellandorflag[i];
      surfandorflag[i] = sav_surfandorflag[i];
      for (j = 0; j < MAXMATS; j++)
        {
         nodeselflag[i][j] = sav_nodeselflag[i][j];
         cellselflag[i][j] = sav_cellselflag[i][j];
         surfselflag[i][j] = sav_surfselflag[i][j];
        }
     }

   traceselnum_on = sav_traceselnum_on;
   ntraceselnum = sav_ntraceselnum;

   contourfield_draw = sav_contourfield_draw;
   mcontourfield_draw = sav_mcontourfield_draw;
   isovolfield_draw = sav_isovolfield_draw;

   for (i = 0; i < MAXFLDCALC; i++)
     {
      fieldcalc[i] = sav_fieldcalc[i];
      cfieldcalc[i] = sav_cfieldcalc[i];
      sfieldcalc[i] = sav_sfieldcalc[i];
     }

   for (i = 0; i < MAXCUTLINES; i++)
      cutlineradius[i] = sav_cutlineradius[i];

   /*  If grid analysis, get gridfield data.  */
   if (gridedgesflag == 1 || gacellnumflag == 1 || gridsflag == 1 ||  
       gridmedianflag == 1 || gridmedianedgeflag == 1)
      gridfillfield();   

   getattr_set_data();

   /*  If node or cell query probe, set crosshair cursor.  */
   if (nprobeflag || cprobeflag)
      XDefineCursor(dpy,windowid,crosshair);
}



unsigned long getbestcolor(float red, float green, float blue)
{
  float dist, mindist, rr, gg, bb;
  int i, j;

   /*  Get the closest color from the  */
   /*  current colormap by distance.   */
   mindist = 9.99999e32;
   for (i = 0; i < 256; i++)
     {
      rr = (float)get_colors[i].red / 65535.;
      gg = (float)get_colors[i].green / 65535.;
      bb = (float)get_colors[i].blue / 65535.;
      dist = (red-rr)*(red-rr) + (green-gg)*(green-gg) + 
             (blue-bb)*(blue-bb);
      if (dist < mindist)
        {
         j = i;
         mindist = dist;
        }
     }

   return j;
}


void clminradcalc()
{
  double dist, mindist, a, b, c, maxdist, maxrdist, maxrad;
  int icell, jcell, nloop, j, l, m, iedge, isurf;

   /*  If no cells, use the diagonal * e-5.  */
   if (ncells == 0)
     {
      a = globxicmax - globxicmin;
      b = globyicmax - globyicmin;
      c = globzicmax - globzicmin;
      clminradius = sqrt(a*a + b*b + c*c) * 1.0e-5;
      maxdist = clminradius * 100.;
     }

   /*  Otherwise use half of minimum distance  */ 
   /*  from cell averages to cell vertices.    */
   else
     {
      FREE(maxcellradius);
      maxcellradius = (float *)malloc(ncells*sizeof(float));
      if (maxcellradius == NULL) memerr();
      nloop = ncells;
      /* if (structuredflag == 1) nloop = 1; */

      /*  Loop through the cells and get      */
      /*  minimum distance along cell edges.  */
      mindist = 9.99999e32;
      maxdist = -9.99999e32;
      maxrdist = -9.99999e32;
      for (icell = 0; icell < nloop; icell++)
        {
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(jcell,2);
           }
         for (j = 0; j < cells[jcell].nedges; j++)
           {
            iedge = cells[jcell].edgenos[j];
            if (structuredflag == 1) iedge = j;
            l = celledges[iedge].edgept1;
            m = celledges[iedge].edgept2;
            a = nodes[l].x - nodes[m].x;
            b = nodes[l].y - nodes[m].y;
            c = nodes[l].z - nodes[m].z;
            dist = a*a + b*b + c*c;
            if (dist < mindist && dist > myzero)
               mindist = dist;
            if (dist > maxdist && dist > myzero)
               maxdist = dist;
           }

         /*  Get maximum distance from cell center to nodes.  */
         maxrad = 0;
         for (j = 0; j < cells[jcell].ncnodes; j++)
           {
            l = cells[jcell].cnodes[j];
            a = cells[jcell].xavg - nodes[l].x;
            b = cells[jcell].yavg - nodes[l].y;
            c = cells[jcell].zavg - nodes[l].z;
            dist = a*a + b*b + c*c;
            if (dist > maxrad && dist > myzero)
               maxrad = dist;
           }
         if (maxrad > maxrdist) maxrdist = maxrad;
         maxcellradius[icell] = sqrt(maxrad);
        }
      clminradius = 0.5 * sqrt(mindist);
     }

   /*  Calculate surface radius.  */
   if (nsurf > 0)
     {
      FREE(maxsurfradius);
      maxsurfradius = (float *)malloc(nsurf*sizeof(float));
      if (maxsurfradius == NULL) memerr();

      for (isurf = 0; isurf < nsurf; isurf++)
        {

         /*  Get maximum distance from surface center to nodes.  */
         maxrdist = -9.99999e32;
         maxrad = 0;
         for (j = 0; j < surfs[isurf].nverts; j++)
           {
            l = surfs[isurf].fverts[j];
            a = surfs[isurf].xavg - nodes[l].x;
            b = surfs[isurf].yavg - nodes[l].y;
            c = surfs[isurf].zavg - nodes[l].z;
            dist = a*a + b*b + c*c;
            if (dist > maxrad && dist > myzero)
               maxrad = dist;
           }
         if (maxrad > maxrdist) maxrdist = maxrad;
         maxsurfradius[isurf] = sqrt(maxrad);
        }
     }

   /*  Set cutline radius defaults.  */
   for (j = 0; j < MAXCUTLINES; j++)
      cutlineradius[j] = clminradius;

   /*  Set node and cell select coordinate    */
   /*  search data to the maximum cell edge.  */
   if (maxrdist > maxdist) maxdist = maxrdist;
   clmaxradius = sqrt(maxdist);
   nodeselsph_x = -xtrans;
   nodeselsph_y = -ytrans;
   nodeselsph_z = -ztrans;
   nodeselsph_r = clmaxradius;
   cellselsph_x = -xtrans;
   cellselsph_y = -ytrans;
   cellselsph_z = -ztrans;
   cellselsph_r = clmaxradius;
   surfselsph_x = -xtrans;
   surfselsph_y = -ytrans;
   surfselsph_z = -ztrans;
   surfselsph_r = clmaxradius;
}

 
void MyManageChild(Widget w)
{
  Widget ww;

   XtManageChild(w);
   /* GETSHELL(ww, w); */
   for(ww = w;
       !XmIsDialogShell(ww) && !XtIsTopLevelShell(ww);
       ww = XtParent(ww));
  XMapRaised(XtDisplay(ww), XtWindow(ww));
}


void setangles()
{
  char str[20];
  int ival;

   XtRemoveCallback(objxrot,XmNvalueChangedCallback,rotator,(caddr_t) 0);
   XtRemoveCallback(objyrot,XmNvalueChangedCallback,rotator,(caddr_t) 1);
   XtRemoveCallback(objzrot,XmNvalueChangedCallback,rotator,(caddr_t) 2);
   XFlush(dpy);

   ival = (int)(xrot + .5);
   if (xrot < 0) ival = (int)(xrot - .5);
   XmScaleSetValue(objxrot,ival);
   sprintf(str,"%6.1f",xrot);
   XtVaSetValues(objxrottxt,XmNvalue,str,NULL);

   ival = (int)(yrot + .5);
   if (yrot < 0) ival = (int)(yrot - .5);
   XmScaleSetValue(objyrot,ival);
   sprintf(str,"%6.1f",yrot);
   XtVaSetValues(objyrottxt,XmNvalue,str,NULL);

   ival = (int)(zrot + .5);
   if (zrot < 0) ival = (int)(zrot - .5);
   XmScaleSetValue(objzrot,ival);
   sprintf(str,"%6.1f",zrot);
   XtVaSetValues(objzrottxt,XmNvalue,str,NULL);

   XFlush(dpy);
   XtAddCallback(objxrot,XmNvalueChangedCallback,rotator,(caddr_t) 0);
   XtAddCallback(objyrot,XmNvalueChangedCallback,rotator,(caddr_t) 1);
   XtAddCallback(objzrot,XmNvalueChangedCallback,rotator,(caddr_t) 2);

   /*  If angles have changed, then set flag to z sort transparent objects.  */
   sort_trans = 1;
}


void setmag()
{
  char str[20];
  int ival;

   XtRemoveCallback(objmagnify,XmNvalueChangedCallback,magnifier,NULL);
   sprintf(str,"%4.2f",magnify);
   XtVaSetValues(objmagnifytxt,XmNvalue,str,NULL);
   ival = (int) (200.*log10(magnify));
   XmScaleSetValue(objmagnify,ival);
   XtAddCallback(objmagnify,XmNvalueChangedCallback,magnifier,NULL);
}


void calcrbzoom()
{
  int rbxcen, rbycen;
  float winasp, boxasp, magfac;

   /*  Generate new zoom center.  */
   rbxcen = (rbx2 + rbx1) / 2.;
   rbycen = (rby2 + rby1) / 2.;
   mx = rbxcen;  my = rbycen;
   ncrosspts = 0;
   docrosshair();
   mx = omx;  my = omy;

   if (fabs(rbwx2-rbwx1) < 1.e-6) rbwx2 += 1.e-6;
   if (fabs(rbwy2-rbwy1) < 1.e-6) rbwy2 += 1.e-6;
   if (fabs(rbwz2-rbwz1) < 1.e-6) rbwz2 += 1.e-6;

   xtrans = (rbwx2+rbwx1)*0.5;
   ytrans = (rbwy2+rbwy1)*0.5;
   ztrans = (rbwz2+rbwz1)*0.5;
   if (xtrans < globxicmin) xtrans = globxicmin;
   if (xtrans > globxicmax) xtrans = globxicmax;
   if (ytrans < globyicmin) ytrans = globyicmin;
   if (ytrans > globyicmax) ytrans = globyicmax;
   if (ztrans < globzicmin) ztrans = globzicmin;
   if (ztrans > globzicmax) ztrans = globzicmax;
   xtrans = -xtrans;
   ytrans = -ytrans;
   ztrans = -ztrans;

   /*  Calculate new magnification.  */
   winasp = (float)windowheight / (float)windowwidth;
   boxasp = (float)(rby2-rby1) / (float)(rbx2-rbx1);
   if (rbx2 != rbx1) 
     {
      if (winasp > boxasp)
        {
 
         /*  window is tall relative to box  */
         magfac = (float)windowwidth/fabs((float)(rbx2-rbx1));
        } 
      else
        {
         magfac = (float)windowheight/fabs((float)(rby2-rby1));
        }
      magnify *= magfac;
     }
}


/*  The following function computes a quaternion from the    */
/*  rotation angles.  It also updates the rotation matrices  */
/*  for quaternions.  */
void getquatsfromangles()
{
 GLfloat btheta, bphi, bpsi;
 double costheta, sintheta, cosphi, sinphi, cospsi, sinpsi;
 double xrot2, yrot2, zrot2;
 double W, X, Y, Z, S, T;

   /*  Check for 90 degree rotation angles, they cause problems     */
   /*  with the quaternion to angle computation in orient.  Change  */
   /*  90 degree angles to 89.6 degree angles.                      */
   xrot2 = xrot;
   if (xrot == 90.) xrot2 = 89.96;
   if (xrot == -90.) xrot2 = -89.96;
   yrot2 = yrot;
   if (yrot == 90.) yrot2 = 89.96;
   if (yrot == -90.) yrot2 = -89.96;
   zrot2 = zrot;
   if (zrot == 90.) zrot2 = 89.96;
   if (zrot == -90.) zrot2 = -89.96;

   btheta = MYPI*(xrot2 - 270.0)/180.;
   bphi = MYPI*(90.0 - yrot2)/180.;
   bpsi = MYPI*zrot2/180.;
   costheta = cos((double)btheta);
   sintheta = sin((double)btheta);
   cosphi = cos((double)bphi);
   sinphi = sin((double)bphi);
   cospsi = cos((double)bpsi);
   sinpsi = sin((double)bpsi);

   Ballmatrix[0][0] = ball.mNow[0][0] = 
    costheta*cospsi - sintheta*cosphi*sinpsi;
   Ballmatrix[0][1] = ball.mNow[0][1] = 
    -costheta*sinpsi - sintheta*cosphi*cospsi;
   Ballmatrix[0][2] = ball.mNow[0][2] = 
    sintheta*sinphi;
   Ballmatrix[1][0] = ball.mNow[1][0] = 
    sintheta*cospsi + costheta*cosphi*sinpsi;
   Ballmatrix[1][1] = ball.mNow[1][1] = 
    -sintheta*sinpsi + costheta*cosphi*cospsi;
   Ballmatrix[1][2] = ball.mNow[1][2] = 
    -costheta*sinphi;
   Ballmatrix[2][0] = ball.mNow[2][0] = 
    sinphi*sinpsi;
   Ballmatrix[2][1] = ball.mNow[2][1] = 
    sinphi*cospsi;
   Ballmatrix[2][2] = ball.mNow[2][2] = 
    cosphi;

   T = Ballmatrix[0][0] + Ballmatrix[1][1] + Ballmatrix[2][2] + 1.0;
   if (T > 0.0)
     {
      S = 0.5/sqrt(T);
      W = 0.25/S;
      X = ( Ballmatrix[1][2] - Ballmatrix[2][1] ) * S;
      Y = ( Ballmatrix[2][0] - Ballmatrix[0][2] ) * S;
      Z = ( Ballmatrix[0][1] - Ballmatrix[1][0] ) * S;
     }
   ball.qNow.x = (float)X;
   ball.qNow.y = (float)Y;
   ball.qNow.z = (float)Z;
   ball.qNow.w = (float)W;

   /*  Tell the quat data structure about this.  */
   Ball_EndDrag(&ball);
}

 
#ifdef PHYSED

extern int write_binary_flag;

void wrtgmv(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int arg;
  XmString string, string2, string3;

   arg = (long int)client_data;
   write_binary_flag = 1;
   if (arg == 1) write_binary_flag = 0;
 
   fileselflag = GMVOUTPUT;
   string = XmStringCreate("Write GMV File",
                           XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[GMVOUTPUT],
                           XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[GMVOUTPUT],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string,
                           XmNpattern, string2, 
                           XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);
   if (fsread[GMVOUTPUT])
     {
      string = XmStringCreate(filnam,
                           XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
      XmStringFree(string);
     }
 
   MyManageChild(file_sel2);
} 


void wrtsdm(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  XmString string, string2, string3;
  
   fileselflag = SDMOUTPUT;
   string = XmStringCreate("Write SDM File",
                           XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[SDMOUTPUT],
                           XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[SDMOUTPUT],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string,
                           XmNpattern, string2, 
                           XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);
   if (fsread[SDMOUTPUT])
     {
      string = XmStringCreate(filnam,
                           XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
      XmStringFree(string);
     }
 
   MyManageChild(file_sel2);
}
#endif
