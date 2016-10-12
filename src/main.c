/*
    This program was prepared by the Regents of the University
    of California at Los Alamos National Laboratory under 
    Contract No. W-7405-ENG-36 with the U.S. Department of 
    Energy.  The University has certain right in the program 
    pusuant to the contract and the program should not be
    copied or distributed outside your organization.  All rights
    in the program are reserved by the DOE and the University.
    Neither the U.S. Government nor the University makes any 
    warranty, express or implied, or assumes any liability or 
    responsibility for the use of this software.
 
    Software Author:  Frank A. Ortega, XCM (505) 665-0588
                      fao@lanl.gov
    The General Mesh Viewer (GMV) control number is LA-CC-95-5

****************************************************************

    Update as of 2016-10-12 by Tony Jones, CPFD Software, LLC.:

    Tony Jones worked with Frank Ortega over the years to 
    provide enhancements, fixes, and other changes to GMV,
    including porting of GMV to work via Cygwin in Windows.  
    Frank Ortega passed away suddenly on December 11, 2012.  
    Tony Jones has since been the sole maintainer for GMV on 
    behalf of CPFD Software, and has worked with LANL to provide 
    a path forward for GMV.
    LANL agreed to open source GMV under GPL v3 in April, 2013,
    allowing CPFD Software to maintain the source code and 
    distribute GMV.
    LANL License Agreement Number: 09-C02641
    
    Since then, Tony Jones has made mostly fixes and some
    improvements to GMV.  The current state of the code is 
    stable and GMV is still used heavily by CPFD Software.

    Please note that GMV is now protected under the GPL v3 
    license, and is copyright of Los Alamos National Laboratory.  
    All software is provided "AS IS", without warranty of any 
    kind.  CPFD Software cannot provide support for GMV.

    Code was previously kept in an internal CPFD Software 
    subversion repository, and sent via zipped source packages
    per GPL v3 requirements.  It is now being hosted on GitHub.
*/

/*#ifdef NT
#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <glx.h>
#endif

//#include <GLwMDrawA.h>

#ifdef LINUX*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
//#endif

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/FileSB.h>
#include <Xm/DialogS.h>
#include <Xm/Label.h>
#include <Xm/BulletinB.h>
#include <Xm/PushB.h>
 
#include <stdlib.h>
#include <math.h>
//#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>
#include <time.h>
#include <errno.h>
#include <locale.h>


#ifdef BATCH
#include <GL/osmesa.h>
OSMesaContext ctx;
short draw3dflag;
void *buffera;
#endif
char batchfilnam[256];
short writenodes_flag;
char  writenodes_filename[256];
short writecells_flag;
char  writecells_filename[256];
short writeparticles_flag;
char  writeparticles_filename[256];
short writesurfaces_flag;
char  writesurfaces_filename[256];

FILE *fieldfile, *cfieldfile, *polyfile, *sfieldfile, *ffieldfile;
char *fldfilenam, *cfldfilenam, *polyfilenam, *sfldfilenam, *ffldfilenam;

#define HDATA_INIT
#include "main.h"
#include "gmvdata.h"
#include "lights.h"
#include "xstuff.h"
#include "formwidgets.h"
#include "contour.h"
#include "cutplane.h"

#ifdef IRSTEREO
#include <X11/extensions/XSGIvc.h>
#endif

/*
 * Define function prototyping style
 */
#ifdef __STDC__
#define FUNCProto(p) p
#else
#define FUNCProto(p) ()
#endif

/*extern "C" {
  void readgmvinfo();
  void initcolors();
  void initglobals();
  void addstereobuttons();
  void makestereoform(Widget parent);
  void initfont();
  void getfile(int inpfileflag);
  void autoread();
  int  gmvread_checkfile(char *filnam);
  int  x3dread_checkfile(char *filnam);
  int  exoread_checkfile(char *filename);
  void readexodus();
  void readx3d();
}*/

XtAppContext app;
Widget startbt;
Visual *top_vis;
int top_depth;

int fileinfoflag, st_flag;

/* 
static String fallback_resources[] = {
    "*frame*shadowType: SHADOW_IN",
    "*fontList: -adobe-courier-bold-r-*-*-14-*",
    NULL
    };
*/
 
static int stereoBuf[] = 
   {
    GLX_STEREO, GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16,
    GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,  
    None
   };
 
static int dblBuf[] = 
   {
    GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16,
    GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,  
    None
   };
static int *snglBuf = &dblBuf[1];
GLboolean    doubleBuffer = GL_TRUE, hasStereo = GL_FALSE; 
GLXContext glx_glw_context, glx_axglw_context, glx_stglw_context;
static short infileflag = 0;
char monstat[40];
static Bool WaitForNotify(Display *d, XEvent *e, char *arg)
   {
    return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
   }

void drawit(), readgmv(), get_statistics(), initstuff(), 
     compute_boundary_triangle_unit_normals(),
     init_motif(Widget toplevel), getattributes(), gmvexit();

/*  Set up signal processing to clean up temp. files.  */
void die(int signo)
{
  gmvexit();
}


//TJ this is used for the license error to exit after blocking
void exitCallback(Widget dialog, XtPointer client_data, XtPointer call_data)
{
  XtUnmanageChild (dialog);
  exit(0);
}


int main(int argc,char *argv[])
{
  void commandline(int argc, char *argv[], char infile[256],
                   char datafile[80]);
  void main_motif(int argc, char *argv[]);
  void main_movie();
  char infile[256], datafile[80];
  void usageinfo(void);

   /*  Get and write GMV usage information.  */
   //usageinfo();

   /*  Set the gmvflag or physedflag.  */
#ifdef PHYSED
   gmvflag = 0;  physedflag = 1;
#else
   gmvflag = 1;  physedflag = 0;
#endif

   /*  Set up signal processing to clean up temp. files.  */
   signal(SIGINT, die);

   movieflag = 0;  snapshotflag = 0;  windowflag = 0;  readattributes = 0;
   readattributes_found_width_height = 0;
   windowxmin = 250;  windowymin = 50;  windowwidth = 900;  
   windowheight = 700;  debugflag = 0;  rdgmvcount = 0;  gridonlyflag = 0;
   noprintflag = 0;  fileinfoflag = 0;  shmemflag = 0;  st_flag = 0;
   strcpy(batchfilnam,"AzsnapgmvAz");
   writenodes_flag = 0;
   strcpy(writenodes_filename, "AzNodesAz");
   writecells_flag = 0;
   strcpy(writecells_filename, "AzCellsAz");
   writeparticles_flag = 0;
   strcpy(writeparticles_filename, "AzParticlesAz");
   writesurfaces_flag = 0;
   strcpy(writesurfaces_filename, "AzSurfacesAz");

   if (argc > 1)
      commandline(argc,argv,infile,datafile);

   /*  If the file was specified in the command line, save the name.  */
   if (infileflag == 1) strcpy(&filnam[0],infile);

   /*  If fileinfoflag and infileflag are on, get fileinfo and exit.  */
   if (fileinfoflag && infileflag)
     {
      readgmvinfo();
      exit(0);
     }

   //TJ fixes issue with gmv batch crash due to change in width/height apparently
   //   must call before initcolors...
   //   Also fixes issue with jarring resize of GMV window on launch
   if (readattributes == 1) getattributes_windowwidth_windowheight();

   /*  Initialize colors and flags.  */
    initcolors();
    initglobals();

 
   /*  Open field database file.  */
   fldfilenam = tempnam(NULL,pfxn);
   fieldfile = fopen(fldfilenam,"w+");   
   cfldfilenam = tempnam(NULL,pfxc);
   cfieldfile = fopen(cfldfilenam,"w+");   
   polyfilenam = tempnam(NULL,pfxp);
   polyfile = fopen(polyfilenam,"w+");   
   sfldfilenam = tempnam(NULL,pfxs);
   sfieldfile = fopen(sfldfilenam,"w+");   
   ffldfilenam = tempnam(NULL,pfxf);
   ffieldfile = fopen(ffldfilenam,"w+");   
   unlink(fldfilenam); 
   unlink(cfldfilenam); 
   unlink(polyfilenam);
   unlink(sfldfilenam);
   unlink(ffldfilenam);
   if (fieldfile == NULL || cfieldfile == NULL || polyfile == NULL ||
       sfieldfile == NULL || ffieldfile == NULL)
      {
       fprintf(stderr,"%s cannot open field database files.\n", appname);
       fprintf(stderr,
          "Set the environmental variable TMPDIR to a useable directory.\n");
       fprintf(stderr,"eg.: setenv TMPDIR mydirectory\n");
       exit(0);
      }


#ifdef BATCH
   fprintf(stdout,"Batch %s\n\n", appversionstring);
   movieflag = 1;
   snapshotflag = 1;
#endif
   if (movieflag == 0) main_motif(argc,argv);
   else main_movie();
   
   return 0;
}
 
 
void commandline(int argc,char *argv[],char infile[256],
                 char datafile[80])
{
  int i;
  char tmpfile[256], *cwd;

   i = 1;
   while ( i < argc )
     {
      if (strncmp(argv[i],"-m",2) == 0) movieflag = 1;
      else if (strcmp(argv[i],"-s") == 0)
        {
         snapshotflag = 1;
         if (i+1 < argc)
           {
            if (strncmp(argv[i+1],"-",1) == 0)
                i--;
            else
               strcpy(batchfilnam,argv[i+1]);
           }
         i++;
        }
      else if (strcmp(argv[i],"-writenodes") == 0)
        {
         writenodes_flag = 1;
         if (i+1 < argc)
           {
            if (strncmp(argv[i+1],"-",1) != 0)
            {
               strcpy(writenodes_filename,argv[i+1]);
               i++;
            }
           }
        }
      else if (strcmp(argv[i],"-writecells") == 0)
        {
         writecells_flag = 1;
         if (i+1 < argc)
           {
            if (strncmp(argv[i+1],"-",1) != 0)
            {
               strcpy(writecells_filename,argv[i+1]);
               i++;
            }
           }
        }
      else if (strcmp(argv[i],"-writeparticles") == 0)
        {
         writeparticles_flag = 1;
         if (i+1 < argc)
           {
            if (strncmp(argv[i+1],"-",1) != 0)
            {
               strcpy(writeparticles_filename,argv[i+1]);
               i++;
            }
           }
        }
      else if (strcmp(argv[i],"-writesurfaces") == 0)
        {
         writesurfaces_flag = 1;
         if (i+1 < argc)
           {
            if (strncmp(argv[i+1],"-",1) != 0)
            {
               strcpy(writesurfaces_filename,argv[i+1]);
               i++;
            }
           }
        }
      else if (strncmp(argv[i],"-w",2) == 0)
        {
         if( (i+4) < argc ) {
           windowflag = 1;
           windowxmin = atol(argv[i+1]);
           windowymin = atol(argv[i+2]);
           windowwidth = atol(argv[i+3]);
           windowheight = atol(argv[i+4]);
           i += 4;
         } else {
           fprintf(stdout,"Error: Incorrect number of arguments for -w option\n");
           fprintf(stdout,"Expected:  -w #XMIN #YMIN #XSIZE #YSIZE\n");
           exit(1);
         }
        }
      else if (strncmp(argv[i],"-i",2) == 0)
        {
         infileflag = 1;
         strcpy(tmpfile,argv[i+1]);
         if (strncmp(tmpfile,"/",1) == 0)
           {
            strcpy(infile,tmpfile);
           }
         else
           {
            cwd = getcwd(NULL,200);
            if (cwd == NULL)
              {
               strcpy(infile,tmpfile);
              }
            else
              {
               sprintf(infile,"%s/%s",cwd,tmpfile);
               free(cwd);
              }
           }
         i++;
        }
      else if (strncmp(argv[i],"-a",2) == 0)
        {
         readattributes = 1;
         strcpy(attribsfilnam,argv[i+1]);
         i++;
        }
      else if (strncmp(argv[i],"-gridonly",9) == 0) gridonlyflag = 1;
      else if (strncmp(argv[i],"-debug",6) == 0) debugflag = 1;
      else if (strncmp(argv[i],"-noprint",8) == 0) noprintflag = 1;
      else if (strncmp(argv[i],"-fileinfo",9) == 0) fileinfoflag = 1;
      else if (strncmp(argv[i],"-shmem",6) == 0) shmemflag = 1;
      else if (strncmp(argv[i],"-stereo",7) == 0) st_flag = 1;
      else {
        fprintf(stdout,"Error: Unknown command line option\n");
        fprintf(stdout,"----------------------\n");
        fprintf(stdout,"GMV Command Line Usage\n");
        fprintf(stdout,"----------------------\n");
        fprintf(stdout,"  -i FILE                         Read in Gmv data file\n");
        fprintf(stdout,"  -a FILE                         Read in Gmv attribute file\n");
        fprintf(stdout,"  -w #XMIN #YMIN #XSIZE #YSIZE    Window starting size\n");
        fprintf(stdout,"  -s FILE                         Snapshot file name prefix (default: AzsnapgmvAz)\n");
        fprintf(stdout,"  -m                              Movie mode (GMV Batch automatically uses this)\n");
        fprintf(stdout,"  -writenodes FILE                Write variables for nodes to FILE (requires '-m')\n");
        fprintf(stdout,"  -writecells FILE                Write variables for cells to FILE (requires '-m')\n");
        fprintf(stdout,"  -writeparticles FILE            Write variables for particles to FILE (requires '-m')\n");
        fprintf(stdout,"  -writesurfaces FILE             Write variables for surfaces to FILE (requires '-m')\n");
        fprintf(stdout,"  -gridonly                       Show grid only\n");
        fprintf(stdout,"  -debug                          Debug mode\n");
        fprintf(stdout,"  -noprint                        Do not print to console as much\n");
        fprintf(stdout,"  -fileinfo                       File info\n");
        fprintf(stdout,"  -shmem                          Enable shared memory file\n");
        fprintf(stdout,"  -stereo                         Enable stereo image pairs\n");
        
        exit(1);
      }
 
      i++;
     }
}
 
void main_motif(int argc,char *argv[])
{
  XmString string;
  XEvent event;
  int scrn, filequit, rdevent, nwin, i, j, k;
  Arg args[20];
  Window windows[4];

#ifdef IRSTEREO 
  XSGIvcChannelInfo *cinfo_return;
  char *mstr;
  int channel;
#endif
  void SetBackingStoreG(Widget w,
                        void (*cb),
                        XtPointer data),
       SetBackingStore(), lpexpose(), 
       draw_expose_callback(), expose_axwindow();
  void getTrueColor(int scrn);

   /*  Initialize the app.  */
   XtToolkitInitialize();
   app = XtCreateApplicationContext();

   dpy = XtOpenDisplay(app,0,appstring,appname,(XrmOptionDescList)NULL, 0,
                        (Cardinal *)&argc, (String *)argv);

   /*  Get a TrueColor visual, if available.  */
   scrn = DefaultScreen(dpy);
   getTrueColor(scrn);

   /*  Get the maximum screen size.  */
   windowxmax = DisplayWidth(dpy,scrn);
   windowymax = DisplayHeight(dpy,scrn);

   i=0; XtSetArg(args[i], XmNcolormap,top_cmap);
   i++; XtSetArg(args[i], XmNvisual,top_vis);
   i++; XtSetArg(args[i], XmNdepth,top_depth);
   i++;

   toplevel = XtAppCreateShell(NULL,appname,applicationShellWidgetClass,
                               dpy,args,i);

   XtVaSetValues(toplevel,
                 XtNtitle,appversionstring,
                 XtNx, 0, XtNy, 0,
                 XtNheight,900, XtNwidth, 1000, NULL);

//TJ fixes weird 50 pixel issue on load without resizing window, now fixes cygwin as well
//#ifdef LINUX
   string = XmStringCreate(appinitstring, XmSTRING_DEFAULT_CHARSET);

   startbt = XtVaCreateManagedWidget("Startbt",xmLabelWidgetClass,
             toplevel,
             XmNx, 10,
             XmNy, 10,
             XmNlabelString, string,
             NULL);
   XmStringFree(string);
//#endif

   /*  Get window. */
   XtRealizeWidget(toplevel);
   topwin = XtWindow(toplevel);

   vi = NULL;

   /*  See if stereo in a window available.  */
   vi = glXChooseVisual(dpy, DefaultScreen(dpy), stereoBuf);
   if (vi != NULL) stereowindow = 1;

   /*  For sgi, check that the X server has full screen stereo.  */
   has_fullscreenstereo = 0;
   if (XQueryExtension(dpy,"SGIFullScreenStereo",&i,&j,&k))
      has_fullscreenstereo = 1;
#if __sgi
   strcpy(monstat,"72HZ");
#endif

#ifdef IRSTEREO
   /*  Get monitor settings.  */
   if (stereowindow == 1)
     {
      channel = 0;
      XSGIvcQueryChannelInfo(dpy,scrn,channel,&cinfo_return);
      mstr = strdup(cinfo_return->vfinfo.name);
      strcpy(monstat,mstr);
      XFree(cinfo_return);
      j = strlen(monstat);
      for (i = 0; i < j; i++)
        {
         if (strncmp(monstat+i,".",1) == 0) *(monstat+i) = (char)0;
        }
     }
#endif
 
   /*  Find an OpenGL-capable RGB visual with depth buffer.  */
   if (vi == NULL)
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
   if (vi == NULL)
     {
      printf("DoubleBuffer visual not available.  "
             "Trying SingleBuffer.\n");
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
      if (vi == NULL)
          XtAppError(app, "no RGB visual with depth buffer");
      doubleBuffer = GL_FALSE;
     } 
 
   waitcsr = XCreateFontCursor(dpy,XC_watch);

   init_motif(toplevel);

   XtManageChild(form);
   XSync(dpy,0);
   XFlush(dpy);

   /*  Install Colormaps.  */
   windows[0] = XtWindow(toplevel);
   windows[1] = XtWindow(glw);
   windows[2] = XtWindow(axglw);
   nwin = 3;
   XSetWMColormapWindows(dpy,topwin,windows,nwin);
   XFlush(dpy);

   glGetBooleanv(GL_STEREO,&hasStereo);
   if (hasStereo) stereowindow = 1;

   /*  Add stereo options to View menu,  */
   /*  if stereo available.              */
   if (stereowindow || has_fullscreenstereo) addstereobuttons();

#ifdef __sgi
   /*  Create stereo form for sgi.  */
   if (has_fullscreenstereo || stereowindow)
      makestereoform(toplevel);
#else
   /*  Create stereoform, if stereo in a window available.  */
   if (stereowindow) makestereoform(toplevel);
#endif

   /* SetBackingStore(objlight,lpexpose,NULL); */
   SetBackingStoreG(glw,draw_expose_callback,NULL);
   SetBackingStoreG(axglw,expose_axwindow,NULL);

   initfont();
   /* initfont2(winaxisid,glx_axglw_context); */

   if (infileflag == 1)
        {
         XMapRaised(dpy,topwin);
         XSync(dpy,0);
         XFlush(dpy);
         getfile(1);
         XSync(dpy,0);
         XFlush(dpy);
        }
   else if (rdgmvcount == 0)  XtManageChild(file_sel);

   filequit = 1;
   while (filequit)
     {
      if (autordflag == 1)
        {
         autoread();
         rdevent = 1;
         if (XtAppPending(app) == 0) rdevent = 0;
         if (rdevent)
           {
            XtAppNextEvent(app, &event);
            XtDispatchEvent(&event);
           }
        }
      else
        {
         XtAppNextEvent(app, &event);
         XtDispatchEvent(&event);
        }
     }
 
}
 
void main_movie()
 
{
#ifdef BATCH
  int bsize;
#else
  Colormap cmap;
  XSetWindowAttributes swa;
  int xwin, ywin, scrn;
  int *pes_selected, totalpes;
  unsigned int scrn_height, scrn_width;
  XSizeHints hints;
  XWMHints wmhints;
  XClassHint classhints;
  XTextProperty windowName, iconName;
  XEvent event;
#endif
   int gmverr, exoerr, x3derr, i;
   char snapsuffix[10];
 
   /*  Check that the file is a valid GMV input file.  */
   gmverr = 1;  exoerr = 1;  x3derr = 1;
   gmvinflag = 0;  exodusinflag = 0;  x3dinflag = 0;
   gmverr = gmvread_checkfile(filnam);
   if (gmverr > 0 && exoerr > 0)
      x3derr = x3dread_checkfile(filnam);
   if (gmverr > 0 && x3derr > 0)
      exoerr = exoread_checkfile(filnam);

   if (gmverr > 0 && exoerr > 0 && x3derr > 0)
     {
      printf("Error - invalid input file %s:\n",filnam);
      exit(0);
     }

   /*  Set the input type flag.  */
   if (gmverr == 0) gmvinflag = 1;
   if (exoerr == 0) exodusinflag = 1;
   if (x3derr == 0) x3dinflag = 1;

   if (gmvinflag) readgmv();
   if (exodusinflag) readexodus();
   if (x3dinflag) readx3d();

   get_statistics();

#ifdef BATCH
   ctx = OSMesaCreateContext(GL_RGBA, NULL);
   bsize = windowwidth * windowheight;
   buffera = (unsigned int *)malloc(bsize*sizeof(unsigned int));
   OSMesaMakeCurrent(ctx,buffera,GL_UNSIGNED_BYTE,windowwidth,windowheight);
#else
   /*  For OpenGL, open an X window to draw into.  */
   dpy = XOpenDisplay(0);
   if (dpy == NULL)
     {
      printf("Error, cannot open X display.\n");
      exit(0);
     }

   /*  Find an OpenGL-capable RGB visual with depth buffer.  */
   vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
   if (vi == NULL)
     {
      printf("DoubleBuffer visual not available.  "
             "Trying SingleBuffer.\n");
      vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);

      if (vi == NULL)
         XtAppError(app, "no RGB visual with depth buffer");
      doubleBuffer = GL_FALSE;
     }

   glx_glw_context = glXCreateContext(dpy,vi,0,GL_TRUE);
   cmap = XCreateColormap(dpy,RootWindow(dpy, vi->screen),
                          vi->visual,AllocNone);

   swa.colormap = cmap;
   swa.border_pixel = 0;
   swa.event_mask = StructureNotifyMask;
   scrn = DefaultScreen(dpy);
   scrn_width = DisplayWidth(dpy,scrn);
   scrn_height = DisplayHeight(dpy,scrn);
   windowxmax = scrn_width;
   windowymax = scrn_height;
   xwin = windowxmin;
   if (xwin+windowwidth > scrn_width)
      xwin = scrn_width - windowwidth;
   ywin = scrn_height - windowymin - windowheight;
   if (ywin+windowheight > scrn_height)
      ywin = scrn_height - windowheight;
   windowid = XCreateWindow(dpy,RootWindow(dpy, vi->screen),xwin,ywin,
                            windowwidth+4,windowheight+4,0,vi->depth,
                            InputOutput,vi->visual,
                            CWBorderPixel | CWColormap | CWEventMask,
                            &swa);
   if (windowid == NULL)
     {
      printf("Error, cannot create an X window.\n");
      exit(0);
     }

   hints.x = xwin;
   hints.y = ywin;
   hints.width = windowwidth;
   hints.height = windowheight;
   hints.max_width = scrn_width;
   hints.max_height = scrn_height;
   hints.flags = PPosition | PSize | PMaxSize;
   wmhints.initial_state = NormalState;
   wmhints.input = False;
   wmhints.flags = StateHint | InputHint;

   classhints.res_name = (char *) malloc(strlen(appname)+1); 
   strcpy(classhints.res_name, appname);
   classhints.res_class= (char *) malloc(strlen(appname)+1); 
   strcpy(classhints.res_class, appname);

   XStringListToTextProperty(&appname,1,&windowName);
   XStringListToTextProperty(&appname,1,&iconName);

   XSetWMProperties(dpy,windowid,&windowName,&iconName,NULL,0,&hints,
                     &wmhints,&classhints); 

   XMapWindow(dpy,windowid);
   XIfEvent(dpy,&event,WaitForNotify,(char *)windowid);
   glXMakeCurrent(dpy,windowid,glx_glw_context);
#endif

   initstuff();
   initfont();
   compute_boundary_triangle_unit_normals();
   if (readattributes == 1) getattributes();

#ifdef BATCH
   /*  If asking for batch stereo images, set flags.  */
   if (st_flag)
     {
      stereoflag = 1;
      viewflag = 1;
     }
#endif

   /*  Set the image flag depending on the image file suffix.  */
   jpegflag = 1;
   i = strlen(batchfilnam) - 4;
   strcpy(snapsuffix,batchfilnam+i);
   if (strcmp(snapsuffix,".rgb") == 0) jpegflag = 0;
   if (strcmp(snapsuffix+1,".ps") == 0 || strcmp(snapsuffix,".eps") == 0)
     {
      snapshotflag = 0;
      jpegflag = 0;
      p_s_flag = 1;
      strcpy(snapshotfilnam,batchfilnam);
     }

   drawnew3dflag = 1;
   fastdraw = 0;
   drawit();
}
 
 
void gmvexit()
{
  char retmon[80];

   /*  Close and remove field database files, then exit.  */
   /* free_user_mem(); */

   /*  For OpenGL, close the X window.  */
   glFlush();
   XSync(dpy, 0);
   XCloseDisplay(dpy);
/*
   fclose(fieldfile);
   fclose(cfieldfile);
   fclose(polyfile);
   fclose(sfieldfile);
   fclose(ffieldfile);
   remove(fldfilenam);
   remove(cfldfilenam);
   remove(polyfilenam);   
   remove(sfldfilenam);
   remove(ffldfilenam);
*/
#ifdef __sgi
   if (stereoflag)
     {
      sprintf(retmon,"/usr/gfx/setmon -n %s",monstat);
      system(retmon);
      /* system("/usr/gfx/setmon -n 72HZ"); */
     }
#endif
   exit(0);
}



/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
** SetBackingStore
**
** This function sets the backing store attribute if applicable, otherwise it
** sets the exposure callback.
** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
*/
void SetBackingStoreG(Widget w, void (*cb), XtPointer data)
{
  XSetWindowAttributes attrs;
  unsigned long valuemask;

   if (XDoesBackingStore(XtScreen(w)) != NotUseful)
     {
      valuemask = CWBackingStore;
      attrs.backing_store = Always;
      XChangeWindowAttributes(XtDisplay(w),XtWindow(w), valuemask, &attrs);

/*
      if(cb)
         XtRemoveCallback(w, GLwNexposeCallback, cb, data);
*/
     }
}



/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
** SetBackingStore
**
** This function sets the backing store attribute if applicable, otherwise it
** sets the exposure callback.
** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
*/
void SetBackingStore(Widget w, void (*cb) FUNCProto((Widget, XtPointer, XtPointer)), XtPointer data)
{
  XSetWindowAttributes attrs;
  unsigned long valuemask;

   if (XDoesBackingStore(XtScreen(w)) != NotUseful)
     {
      valuemask = CWBackingStore;
      attrs.backing_store = Always;
      XChangeWindowAttributes(XtDisplay(w),XtWindow(w), valuemask, &attrs);

      /* if(cb)
            XtRemoveCallback(w, XmNexposeCallback, cb, data);  */
     }
}


void getTrueColor(int scrn)
{
  int depth;
  Status status;
  XVisualInfo vis;


   /*  First, check to see if the default visual is TrueColor.  */
   top_vis = DefaultVisual(dpy,scrn);
   top_cmap = DefaultColormap(dpy,scrn);
   top_depth = DefaultDepth(dpy,scrn);
   truecolorcmap = 0;
   if ( top_vis->class == TrueColor )
     {
      truecolorcmap = 1;
      return;
     }

#ifdef MESA
   return;
#endif

#ifdef __sgi
   /*  If default is not Truecolor, get the highest  */
   /*  depth TrueColor visual, if one exists.        */
   for (depth = 24; depth >= 8; depth--)
     {
      status = XMatchVisualInfo(dpy,scrn,depth,TrueColor,&vis);
      if (status)
        {
         truecolorcmap = 1;
         top_vis = vis.visual;
         top_depth = depth;
        }
      if (truecolorcmap) break;
     }

   /*  If TrueColor visual found, generate new colormap.  */
   if (truecolorcmap)
      top_cmap = XCreateColormap(dpy,RootWindow(dpy,scrn),top_vis,AllocNone);
#endif
}


void usageinfo(void)
{
  char progname[20], sysname[10], hostname[50], homeprefix[50],
       homefile[200], datechar[50], *username;
  char * homeenv;
  int i, len, len2;
  time_t now;
#ifdef LINUX
  char *mail_command = "/bin/mail -s 'GMV run' gmv@lanl.gov";
#elif MACX
  char *mail_command = "/usr/bin/mail -s 'GMV run' gmv@lanl.gov";
#elif DEC
  char *mail_command = "/usr/bin/mailx -s 'GMV run' gmv@lanl.gov";
#elif __sgi
  char *mail_command = "/usr/sbin/mailx -s 'GMV run' gmv@lanl.gov";
#else
  char *mail_command = "/usr/bin/mail -tw gmv@lanl.gov";
#endif
  struct utsname systeminfo;
  FILE *usagefile, *mailpipe;

   /*  Gather and write usage information, if the file can be written to.  */
   /*  Also send information as an email to gmv@lanl.gov.                  */

   /*  Get the home prefix.  */
   homeenv = getenv("HOME");
   len = strlen(homeenv);
   len2 = 1;
   homeprefix[0] = homeenv[0];
   for (i = 1; i < len; i++)
     {
      homeprefix[i] = homeenv[i];
      len2++;
      if (homeenv[i] == '/') break;
     }
   homeprefix[len2] = '\0';
   strcpy(homefile,homeprefix);
   strcat(homefile,"fao/usageinfo/gmvusage");
   usagefile = fopen(homefile,"a");

   sprintf(progname,"gmv %s    ",gmv_version_no);
#ifdef MESA
   sprintf(progname,"gmvMesa %s ",gmv_version_no);
#endif
#ifdef BATCH
   sprintf(progname,"gmvBatch %s",gmv_version_no);
#endif
   
   strcpy(sysname,"Unknown");
   i = uname(&systeminfo);
   if (i > -1)
     {
      strcpy(sysname,systeminfo.sysname);
     }
   i = gethostname(hostname,50);
   if (i < 0) strcpy(hostname,"Unkown");

   now = time(NULL);
   strcpy(datechar,ctime(&now));

   username = getenv("USER");

   if (usagefile != NULL)
     {
      fprintf(usagefile,"%s %s %s %s %s", progname, username, sysname, 
              hostname, datechar);
      fclose(usagefile);
     }

#ifdef NT
   return;
#endif

   /*  Send email message.  */
/*
   mailpipe = popen(mail_command,"w");
   if (mailpipe == NULL)
     {
      return;
     }


#ifdef __sun	
   fprintf(mailpipe,"Subject: GMV run \n");
#endif
   fprintf(mailpipe,"%s %s %s %s %s", progname, username, sysname, 
           hostname, datechar);
   fprintf(mailpipe,""".\n");
   pclose(mailpipe);
*/
}


