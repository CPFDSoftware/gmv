
#ifdef NT
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <malloc.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/CascadeB.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>
#include <Xm/PushBG.h>
#include <Xm/FileSB.h>
#include <Xm/DrawingA.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
 
#include "GLwMDrawA.h"
#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "xstuff.h"

Widget menu_pane8, cascade8;
void setview();
 
void init_motif()
{
  int i, scrn, maxclrs;
  Arg args[20];
  Widget quitbut, axisframe, animbut, centbut, subsetbut, pattrbut,
         gattrbut, sclaxbut, matedbut, snapbut, azlabel,  twlabel,
         elevlabel, maglabel, timebut, cyclebut, plotboxbut, 
         menu_bar, menu_pane1, rdgmvbut, cascade1, menu_pane2, cascade2, 
         menu_pane3, cascade3, menu_pane4, cascade4, menu_pane5, cascade5,  
         menu_pane6, cascade6, menu_pane7, cascade7, errokbut, subsetmenu, 
         othersubset, datalimmenu, clipbut, interactbut, lightbut,
         datalimbut, colrbarmenu, colrbarbut, colrprefmenu, colrprefbut,
         bboxbut, titlebut, calcisomenu, calcfcmenu, leftform,
         ptsizebut, rdgmvmenu, rdgmvnew, rdgmvsame, rdgmvsameg, rdgmvauto, 
         rdgmvautog, lnsizebut, coloreditmenu, fieldedbut, coloreditbut, 
         ncontbut, backgrndbut, gmvrcbut, vectbut, calcavg, calcavgmenu, 
         distscalebut, winsizebut, datalimpopbut, view6, raybut,
         colrtitlmenu, colrtitlbut, distscalemenu, distsetbut, axismenu,
         o_axis_but, axisbut, wrtvarmenu, writevars, topform;
  void quit(), axesswitch(), doanimation(), doorigin(), dosubset(),
       attributes(), dodistance(), docoloredit(), snapit(),
       clip_on_field_box(), reflections(), doisosurfformm(),
       docutplaneform(),  clipper(), rotator(), magnifier(), init_window(),
       init_axwindow(), draw_expose_callback(), do_resize(),
       input(), doscaleaxis(), getfilename(), rotatortxt(),
       magnifiertxt(), donodeform(), docellform(), dopolygonform(), 
       time_cycle(), dogridanal(), dotraceform(),  doplotbox(), 
       rdgmv(),filecancel(), errok(), expose_axwindow(), dopolysubset(), 
       dofieldlim(), dotracelim(), docolorbar(), docolorpref(), 
       docolortitle(), doboundingbox(), pop_title(), doisosurfformf(), 
       docutlineform(), doqueryform(), docfieldlim(), doptsize(), 
       donfldcalcform(), docfldcalcform(), doisovolform(), doautord(), 
       dostereo(), dolnsize(), rdangles(), dodisplist(), dofcoloredit(),
       pop_ncontours(), docutsphereform(), dobackground(), dotfldcalcform(),
       pop_vectorctl(), dosfieldlim(), dosfldcalcform(), savegmvrc(),
       dosurfform(), pop_rbzoom(), donfldavgform(), docfldavgform(),
       dotrackball(), beepswitch(), dodistscale(), pop_clip(),
       pop_interact(), pop_light(), pop_winsize(), dodatalimpop(),
       doffieldlim(), dotexture(), getfilenameg(), filecancelg(), rdmag(),
       reset_startangles(), rdray(), dorayform(), doraylim(), 
       doconcaveform(), dodistset(), do_o_axis(), do_o_axis_set(),
       donwrtvars(), docwrtvars(), dotwrtvars(), doswrtvars();

#ifdef PHYSED
  Widget writegmvbut, writesdmbut;
  void wrtgmv(), makePEmenu(Widget menubar), makePEforms(Widget poplevel),
       wrtsdm();
#endif

  XmString string;
  Pixmap  csource, cmask;
  GC    gcsource, gcmask;
  XColor cfore, cback;
  unsigned int x1, x2, y1, y2, xhot, yhot, cwidth, cheight;
  int scale_size;
  XGCValues values;
  XColor get_color, exact_color, *my_colors;
  Visual *visual;
  Status istat;
  unsigned long plane_mask[1];
 

   scrn = DefaultScreen(dpy);
   my_cmap = top_cmap;

   /*  Allocate white, black, red, green, blue and yellow colors. */
   whitepix = 0;  blackpix = 0;
   istat = XAllocNamedColor(dpy,my_cmap,"white",
                            &get_color,&exact_color);
   if (istat != 0) whitepix = get_color.pixel;
   istat = XAllocNamedColor(dpy,my_cmap,"black",
                            &get_color,&exact_color);
   if (istat != 0) blackpix = get_color.pixel;
   redpix = 0;  greenpix = 0;  bluepix = 0; yellowpix = 0;
   istat = XAllocNamedColor(dpy,my_cmap,"red",
                            &get_color,&exact_color);
   if (istat != 0) redpix = get_color.pixel;
   istat = XAllocNamedColor(dpy,my_cmap,"green",
                            &get_color,&exact_color);
   if (istat != 0) greenpix = get_color.pixel;
   istat = XAllocNamedColor(dpy,my_cmap,"blue",
                            &get_color,&exact_color);
   if (istat != 0) bluepix = get_color.pixel;
   istat = XAllocNamedColor(dpy,my_cmap,"yellow",
                            &get_color,&exact_color);
   if (istat != 0) yellowpix = get_color.pixel;

   mesacmap = 0;

#ifdef MESA
   /*  Check for PseudoColor visuals in Mesa,  */
   /*  if so, use Mesa's colormap only.        */
   if (vi->class == PseudoColor) mesacmap = 1;
#endif

   /*  Generate pixels for contour buttons.  */
   istat = 1;
   if (!truecolorcmap)
     {
      maxclrs = MAXCONTOURS + 3;
      if (mesacmap) maxclrs = 1;
      visual = DefaultVisual(dpy,scrn);
      istat = XAllocColorCells(dpy,my_cmap,FALSE,plane_mask,0,
                               mat_pixels,maxclrs);
     }

   /*  If not enough color cells to allocate for material  */
   /*  button colors, create a new colormap for toplevel.  */
   if (istat == 0 || mesacmap)
     {
      if (!mesacmap)
         printf("Cannot allocate enough colors for material buttons.\n");
      printf("New colormap created, color flashing may occur.\n");

      my_cmap = XCreateColormap(dpy,
                       RootWindowOfScreen(XtScreenOfObject(toplevel)),
                       visual,AllocNone);
      /*  Get the first 24 colors from the default  */
      /*  colormap to reduce color flashing.        */
      maxclrs = 24;
      my_colors = (XColor *) XtMalloc(maxclrs * sizeof(XColor));
      for (i = 0; i < maxclrs; i++)
         my_colors[i].pixel = i;
      XQueryColors(dpy,top_cmap,my_colors,maxclrs);
      istat = XAllocColorCells(dpy,my_cmap,FALSE,plane_mask,0,
                               mat_pixels,maxclrs);
      XStoreColors(dpy,my_cmap,my_colors,maxclrs);

      /*  Re-allocate pixels from new colormap.  */
      istat = XAllocNamedColor(dpy,my_cmap,"black",
                            &get_color,&exact_color);
      if (istat != 0) blackpix = get_color.pixel;
      istat = XAllocNamedColor(dpy,my_cmap,"white",
                            &get_color,&exact_color);
      if (istat != 0) whitepix = get_color.pixel;
      istat = XAllocNamedColor(dpy,my_cmap,"red",
                            &get_color,&exact_color);
      if (istat != 0) redpix = get_color.pixel;
      istat = XAllocNamedColor(dpy,my_cmap,"green",
                               &get_color,&exact_color);
      if (istat != 0) greenpix = get_color.pixel;
      istat = XAllocNamedColor(dpy,my_cmap,"blue",
                               &get_color,&exact_color);
      if (istat != 0) bluepix = get_color.pixel;
      istat = XAllocNamedColor(dpy,my_cmap,"yellow",
                               &get_color,&exact_color);
      if (istat != 0) yellowpix = get_color.pixel;

      /*  Generate pixels for material and contour buttons.  */
      maxclrs = MAXCONTOURS + 3;
      if (mesacmap) maxclrs = 1;
      istat = XAllocColorCells(dpy,my_cmap,FALSE,plane_mask,0,
                               mat_pixels,maxclrs);

      /*  Set new colormap to toplevel widget.  */
      XtUnrealizeWidget(toplevel);
      XtVaSetValues(toplevel,XmNcolormap,my_cmap,NULL);
      XtRealizeWidget(toplevel);
      dpy = XtDisplay(toplevel);
      topwin = XtWindow(toplevel);
     }

   /*  Create form widget.  */
   form = XmCreateForm(toplevel,"Form",NULL,0);
   XtVaSetValues(form, XmNtitle,"GMV", XmNx,0, XmNy,0, NULL);
   /* XtManageChild(form); */

   /*  Create menu bar.  */

   i=0; XtSetArg(args[i], XmNleftAttachment,XmATTACH_FORM);
   i++; XtSetArg(args[i], XmNrightAttachment,XmATTACH_NONE);
   i++;
   menu_bar = XmCreateMenuBar(form,"Menu_bar",args,i);

   /*  Create files menu to read gmv files, put and    */
   /*  get attributes, create snapshot file and quit.  */
   menu_pane1 = XmCreatePulldownMenu(menu_bar,"Menu_pane1",NULL,0);
 
   rdgmvmenu = XmCreatePulldownMenu(menu_pane1,"Rdgmvmenu",NULL,0);

   string = XmStringCreate("New Simulation",XmSTRING_DEFAULT_CHARSET);
   rdgmvnew = XtVaCreateManagedWidget("Rdgmvnew",
              xmPushButtonGadgetClass,
              rdgmvmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(rdgmvnew,XmNactivateCallback,rdgmv,(caddr_t) 0);

   string = XmStringCreate("Same Simulation",
                           XmSTRING_DEFAULT_CHARSET);
   rdgmvsame = XtVaCreateManagedWidget("Rdgmvsame",
               xmPushButtonGadgetClass,
               rdgmvmenu,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(rdgmvsame,XmNactivateCallback,rdgmv,(caddr_t) 1);

   string = XmStringCreate("Same Simulation, Same Cells",
                           XmSTRING_DEFAULT_CHARSET);
   rdgmvsameg = XtVaCreateManagedWidget("Rdgmvsameg",
                xmPushButtonGadgetClass,
                rdgmvmenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(rdgmvsameg,XmNactivateCallback,rdgmv,(caddr_t) 2);

   string = XmStringCreate("Auto Read - Same Simulation",
                           XmSTRING_DEFAULT_CHARSET);
   rdgmvauto = XtVaCreateManagedWidget("Rdgmvauto",
               xmPushButtonGadgetClass,
               rdgmvmenu,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(rdgmvauto,XmNactivateCallback,doautord,(caddr_t) 0);

   string = XmStringCreate("Auto Read - Same Simulation, Same Cells",
                           XmSTRING_DEFAULT_CHARSET);
   rdgmvautog = XtVaCreateManagedWidget("Rdgmvautog",
                xmPushButtonGadgetClass,
                rdgmvmenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(rdgmvautog,XmNactivateCallback,doautord,(caddr_t) 1);

   string = XmStringCreate("Read GMV File",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,rdgmvmenu);
   i++;
   rdgmvbut = XmCreateCascadeButton(menu_pane1,"Readgmvbut",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(rdgmvbut);
  
#ifdef PHYSED
   string = XmStringCreate("Write ASCII GMV file",XmSTRING_DEFAULT_CHARSET);
   writegmvbut = XtVaCreateManagedWidget("Writegmvbut",
                 xmPushButtonGadgetClass,
                 menu_pane1,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(writegmvbut,XmNactivateCallback,wrtgmv,0);

   string = XmStringCreate("Write Binary GMV file",XmSTRING_DEFAULT_CHARSET);
   writegmvbut = XtVaCreateManagedWidget("Writegmvbut",
                 xmPushButtonGadgetClass,
                 menu_pane1,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(writegmvbut,XmNactivateCallback,wrtgmv,1);

/*
   string = XmStringCreate("Write SMD file",XmSTRING_DEFAULT_CHARSET);
   writesdmbut = XtVaCreateManagedWidget("Writecmdbut",
                 xmPushButtonGadgetClass,
                 menu_pane1,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(writesdmbut,XmNactivateCallback,wrtsdm,NULL);
*/
#endif
 
   string = XmStringCreate("Put Attributes",
                            XmSTRING_DEFAULT_CHARSET);
   pattrbut = XtVaCreateManagedWidget("Pattrbut",
              xmPushButtonGadgetClass,
              menu_pane1,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(pattrbut,XmNactivateCallback,attributes,
                    (caddr_t) 0);
  
   string = XmStringCreate("Get Attributes",
                            XmSTRING_DEFAULT_CHARSET);
   gattrbut = XtVaCreateManagedWidget("Gattrbut",
              xmPushButtonGadgetClass,
              menu_pane1,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(gattrbut,XmNactivateCallback,attributes,
                    (caddr_t) 1);
  
   string = XmStringCreate("Read GMV RAY file",
                            XmSTRING_DEFAULT_CHARSET);
   raybut = XtVaCreateManagedWidget("Raybut",
            xmPushButtonGadgetClass,
            menu_pane1,
            XmNlabelString, string,
            NULL);
      XmStringFree(string);
      XtAddCallback(raybut,XmNactivateCallback,rdray,NULL);
 
   string = XmStringCreate("Snapshot",XmSTRING_DEFAULT_CHARSET);
   snapbut = XtVaCreateManagedWidget("Snapbut",
             xmPushButtonGadgetClass,
             menu_pane1,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(snapbut,XmNactivateCallback,snapit,NULL);
 
   string = XmStringCreate("Save gmvrc",XmSTRING_DEFAULT_CHARSET);
   gmvrcbut = XtVaCreateManagedWidget("Gmvrcbut",
              xmPushButtonGadgetClass,
              menu_pane1,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(gmvrcbut,XmNactivateCallback,savegmvrc,NULL);

   wrtvarmenu = XmCreatePulldownMenu(menu_pane1,"Wrtvarmenu",NULL,0);

   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   nwrtvars = XtVaCreateManagedWidget("Nwrtvars",
              xmPushButtonGadgetClass,
              wrtvarmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(nwrtvars,XmNactivateCallback,donwrtvars,NULL);

   string = XmStringCreate("Cells",
                           XmSTRING_DEFAULT_CHARSET);
   cwrtvars = XtVaCreateManagedWidget("Vwrtvars",
              xmPushButtonGadgetClass,
              wrtvarmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cwrtvars,XmNactivateCallback,docwrtvars,NULL);

   string = XmStringCreate("Particles",
                           XmSTRING_DEFAULT_CHARSET);
   twrtvars = XtVaCreateManagedWidget("Twrtvars",
              xmPushButtonGadgetClass,
              wrtvarmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(twrtvars,XmNactivateCallback,dotwrtvars,NULL);

   string = XmStringCreate("Surfaces",
                           XmSTRING_DEFAULT_CHARSET);
   swrtvars = XtVaCreateManagedWidget("Swrtvars",
                  xmPushButtonGadgetClass,
                  wrtvarmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(swrtvars,XmNactivateCallback,doswrtvars,NULL);

   string = XmStringCreate("Write Variables",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,wrtvarmenu);
   i++;
   writevars = XmCreateCascadeButton(menu_pane1,"Writevars",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(writevars);
 
   string = XmStringCreate("QUIT",XmSTRING_DEFAULT_CHARSET);
   quitbut = XtVaCreateManagedWidget("Quitbut",
             xmPushButtonGadgetClass,
             menu_pane1,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(quitbut,XmNactivateCallback,quit,NULL);

   string = XmStringCreate("File",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane1);
   i++;
   cascade1 = XmCreateCascadeButton(menu_bar,"Cascade1",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade1);

#ifdef PHYSED
   /*  Create PhysEd Edit buttons.  */
   makePEmenu(menu_bar);
#endif
 
   /*  Create display menu for nodes,  */
   /*  cells, polygons, and tracers.   */
   menu_pane2 = XmCreatePulldownMenu(menu_bar,"Menu_pane2",NULL,0);
 
   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   dispnodes = XtVaCreateManagedWidget("Dispnodes",
               xmPushButtonGadgetClass,
               menu_pane2,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(dispnodes,XmNactivateCallback,donodeform,NULL);
 
   string = XmStringCreate("Cells",XmSTRING_DEFAULT_CHARSET);
   dispcells = XtVaCreateManagedWidget("Dispcells",
               xmPushButtonGadgetClass,
               menu_pane2,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(dispcells,XmNactivateCallback,docellform,NULL);
 
   string = XmStringCreate("Polygons",XmSTRING_DEFAULT_CHARSET);
   disppolys = XtVaCreateManagedWidget("Disppolys",
               xmPushButtonGadgetClass,
               menu_pane2,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(disppolys,XmNactivateCallback,dopolygonform,NULL);
 
   string = XmStringCreate("Particles",XmSTRING_DEFAULT_CHARSET);
   disptrace = XtVaCreateManagedWidget("Disptrace",
               xmPushButtonGadgetClass,
               menu_pane2,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(disptrace,XmNactivateCallback,dotraceform,NULL);
 
   string = XmStringCreate("Surfaces",XmSTRING_DEFAULT_CHARSET);
   dispsurfs = XtVaCreateManagedWidget("Dispsurfs",
               xmPushButtonGadgetClass,
               menu_pane2,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(dispsurfs,XmNactivateCallback,dosurfform,NULL);
 
   string = XmStringCreate("Rays",XmSTRING_DEFAULT_CHARSET);
   dispray = XtVaCreateManagedWidget("Dispray",
             xmPushButtonGadgetClass,
             menu_pane2,
             XmNlabelString, string,
             XmNsensitive, FALSE,
             NULL);
      XmStringFree(string);
      XtAddCallback(dispray,XmNactivateCallback,dorayform,NULL);

   string = XmStringCreate("Display",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane2);
   i++;
   cascade2 = XmCreateCascadeButton(menu_bar,"Cascade2",
                                   args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade2);
 
   /*  Create calculate menu for contours, cutplanes, grid        */
   /*  analysis, distance, cutlines, field calc. and query data.  */
   menu_pane3 = XmCreatePulldownMenu(menu_bar,"Menu_pane3",NULL,0);

   calcavgmenu = XmCreatePulldownMenu(menu_pane3,"Calcavgmenu",NULL,0);

   string = XmStringCreate("Node Fields",XmSTRING_DEFAULT_CHARSET);
   calcavgnfld = XtVaCreateManagedWidget("Calcavgnfld",
                 xmPushButtonGadgetClass,
                 calcavgmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(calcavgnfld,XmNactivateCallback,donfldavgform,NULL);

   string = XmStringCreate("Cell Fields",
                           XmSTRING_DEFAULT_CHARSET);
   calcavgcfld = XtVaCreateManagedWidget("Calcavgcfld",
                 xmPushButtonGadgetClass,
                 calcavgmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(calcavgcfld,XmNactivateCallback,docfldavgform,NULL);

   string = XmStringCreate("Average",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,calcavgmenu);
   i++;
   calcavg = XmCreateCascadeButton(menu_pane3,"Calcavg",
                                  args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(calcavg);
  
   string = XmStringCreate("Cutlines",XmSTRING_DEFAULT_CHARSET);
   calccutl = XtVaCreateManagedWidget("Calccutl",
              xmPushButtonGadgetClass,
              menu_pane3,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(calccutl,XmNactivateCallback,docutlineform,NULL);
 
   string = XmStringCreate("Cutplanes",XmSTRING_DEFAULT_CHARSET);
   calccutp = XtVaCreateManagedWidget("Calccutp",
              xmPushButtonGadgetClass,
              menu_pane3,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(calccutp,XmNactivateCallback,docutplaneform,NULL);
 
   string = XmStringCreate("Cutspheres",XmSTRING_DEFAULT_CHARSET);
   calccuts = XtVaCreateManagedWidget("Calccuts",
              xmPushButtonGadgetClass,
              menu_pane3,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(calccuts,XmNactivateCallback,docutsphereform,NULL);
 
   string = XmStringCreate("Distance",XmSTRING_DEFAULT_CHARSET);
   objdist = XtVaCreateManagedWidget("Objdist",
             xmPushButtonGadgetClass,
             menu_pane3,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(objdist,XmNactivateCallback,dodistance,NULL);

   calcfcmenu = XmCreatePulldownMenu(menu_pane3,"Calcfcmenu",NULL,0);

   string = XmStringCreate("Node Fields",XmSTRING_DEFAULT_CHARSET);
   calcnfldcalc = XtVaCreateManagedWidget("Calcnfldcalc",
                  xmPushButtonGadgetClass,
                  calcfcmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(calcnfldcalc,XmNactivateCallback,donfldcalcform,NULL);

   string = XmStringCreate("Cell Fields",
                           XmSTRING_DEFAULT_CHARSET);
   calccfldcalc = XtVaCreateManagedWidget("Calccfldcalc",
                  xmPushButtonGadgetClass,
                  calcfcmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(calccfldcalc,XmNactivateCallback,docfldcalcform,NULL);

   string = XmStringCreate("Particle Fields",
                           XmSTRING_DEFAULT_CHARSET);
   calctfldcalc = XtVaCreateManagedWidget("Calctfldcalc",
                  xmPushButtonGadgetClass,
                  calcfcmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(calctfldcalc,XmNactivateCallback,dotfldcalcform,NULL);

   string = XmStringCreate("Surface Fields",
                           XmSTRING_DEFAULT_CHARSET);
   calcsfldcalc = XtVaCreateManagedWidget("Calcsfldcalc",
                  xmPushButtonGadgetClass,
                  calcfcmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(calcsfldcalc,XmNactivateCallback,dosfldcalcform,NULL);

   string = XmStringCreate("Field Calc.",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,calcfcmenu);
   i++;
   calfcalc = XmCreateCascadeButton(menu_pane3,"Calfcalc",
                                  args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(calfcalc);

   string = XmStringCreate("Grid Analysis",
                           XmSTRING_DEFAULT_CHARSET);
   calcganal = XtVaCreateManagedWidget("Calcganal",
               xmPushButtonGadgetClass,
               menu_pane3,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(calcganal,XmNactivateCallback,
                    dogridanal,NULL);
 
   calcisomenu = XmCreatePulldownMenu(menu_pane3,"Calcisomenu",NULL,0);

   string = XmStringCreate("Fields",XmSTRING_DEFAULT_CHARSET);
   calcisoflds = XtVaCreateManagedWidget("Calcisoflds",
                 xmPushButtonGadgetClass,
                 calcisomenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(calcisoflds,XmNactivateCallback,doisosurfformf,NULL);

   string = XmStringCreate("Materials",
                           XmSTRING_DEFAULT_CHARSET);
   calcisomats = XtVaCreateManagedWidget("Calcisomats",
                 xmPushButtonGadgetClass,
                 calcisomenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(calcisomats,XmNactivateCallback,doisosurfformm,NULL);

   string = XmStringCreate("Isosurfaces",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,calcisomenu);
   i++;
   calciso = XmCreateCascadeButton(menu_pane3,"Calciso",
                                   args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(calciso);
   
   string = XmStringCreate("Isovolume",XmSTRING_DEFAULT_CHARSET);
   calcisovol = XtVaCreateManagedWidget("Calcisovol",
                xmPushButtonGadgetClass,
                menu_pane3,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(calcisovol,XmNactivateCallback,doisovolform,NULL);
  
   string = XmStringCreate("Query Data",XmSTRING_DEFAULT_CHARSET);
   calcquery = XtVaCreateManagedWidget("Calcquery",
               xmPushButtonGadgetClass,
               menu_pane3,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(calcquery,XmNactivateCallback,doqueryform,NULL);
  
   string = XmStringCreate("Test Concave",XmSTRING_DEFAULT_CHARSET);
   calcconcave = XtVaCreateManagedWidget("Calconcave",
                 xmPushButtonGadgetClass,
                 menu_pane3,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(calcconcave,XmNactivateCallback,doconcaveform,NULL);

   string = XmStringCreate("Calculate",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane3);
   i++;
   cascade3 = XmCreateCascadeButton(menu_bar,"Cascade3",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade3);

   /*  Create first control menu for pull down menus.  */
   menu_pane4 = XmCreatePulldownMenu(menu_bar,"Menu_pane4",NULL,0);
 
   string = XmStringCreate("Animation",XmSTRING_DEFAULT_CHARSET);
   animbut = XtVaCreateManagedWidget("Animbut",
             xmPushButtonGadgetClass,
             menu_pane4,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(animbut,XmNactivateCallback,doanimation,NULL);

   axismenu = XmCreatePulldownMenu(menu_pane4,"Axis_menu",NULL,0);

   string = XmStringCreate("Origin Axis",XmSTRING_DEFAULT_CHARSET);
   objaxes = XtVaCreateManagedWidget("Objaxes",
               xmToggleButtonGadgetClass,
               axismenu,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               XmNset, TRUE,
               NULL);
      XmStringFree(string);
      XtAddCallback(objaxes,XmNvalueChangedCallback,axesswitch,
                    (caddr_t) 0);

   string = XmStringCreate("Orientation Axis",XmSTRING_DEFAULT_CHARSET);
   o_axis_on = XtVaCreateManagedWidget("O_axis_on",
               xmToggleButtonGadgetClass,
               axismenu,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               XmNset, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(o_axis_on,XmNvalueChangedCallback,do_o_axis,
                    (caddr_t) 0);

   string = XmStringCreate("Position Orientation Axis",
                           XmSTRING_DEFAULT_CHARSET);
   o_axis_but = XtVaCreateManagedWidget("O_axis_but",
                xmPushButtonGadgetClass,
                axismenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(o_axis_but,XmNactivateCallback,do_o_axis_set,NULL);

   string = XmStringCreate("Axes",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,axismenu);
   i++;
   axisbut = XmCreateCascadeButton(menu_pane4,"axisbut",
                                   args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(axisbut);
 
   string = XmStringCreate("Beep Sound",XmSTRING_DEFAULT_CHARSET);
   beepbut = XtVaCreateManagedWidget("Beepbut",
             xmToggleButtonGadgetClass,
             menu_pane4,
             XmNlabelString, string,
             XmNspacing, 1,
             XmNselectColor, yellowpix,
             XmNset, TRUE,
             NULL);
      XmStringFree(string);
      XtAddCallback(beepbut,XmNvalueChangedCallback,
                    beepswitch,NULL);
 
   string = XmStringCreate("Bounding Box",XmSTRING_DEFAULT_CHARSET);
   bboxbut = XtVaCreateManagedWidget("Bboxbut",
             xmPushButtonGadgetClass,
             menu_pane4,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(bboxbut,XmNactivateCallback,doboundingbox,NULL);

   string = XmStringCreate("Center",XmSTRING_DEFAULT_CHARSET);
   centbut = XtVaCreateManagedWidget("Centbut",
             xmPushButtonGadgetClass,
             menu_pane4,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(centbut,XmNactivateCallback,doorigin,NULL);

   string = XmStringCreate("Clip",XmSTRING_DEFAULT_CHARSET);
   clipbut = XtVaCreateManagedWidget("Clipbut",
             xmPushButtonGadgetClass,
             menu_pane4,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(clipbut,XmNactivateCallback,pop_clip,NULL);

   colrbarmenu = XmCreatePulldownMenu(menu_pane4,"Colrbarmenu",NULL,0);

   string = XmStringCreate("Material/Flags",
                           XmSTRING_DEFAULT_CHARSET);
   colrbarmat = XtVaCreateManagedWidget("Colrbarmat",
                xmPushButtonGadgetClass,
                colrbarmenu,
                XmNlabelString, string,
                XmNspacing, 2,
                NULL);
      XmStringFree(string);
      XtAddCallback(colrbarmat,XmNactivateCallback,docolorbar,
                    (caddr_t) 1);

   string = XmStringCreate("Fields",
                           XmSTRING_DEFAULT_CHARSET);
   colrbaron = XtVaCreateManagedWidget("Colrbaron",
               xmToggleButtonGadgetClass,
               colrbarmenu,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               XmNset, TRUE,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrbaron,XmNvalueChangedCallback,docolorbar,
                    (caddr_t) 0);

   string = XmStringCreate("Color Bar",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,colrbarmenu);
   i++;
   colrbarbut = XmCreateCascadeButton(menu_pane4,"Colrbarbut",
                                      args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(colrbarbut);

   i=0; XtSetArg(args[i], XmNradioBehavior,TRUE);
   i++;
   colrprefmenu = XmCreatePulldownMenu(colrbarmenu,"Colrprefmenu",
                                       args,(Cardinal) i);

   string = XmStringCreate("None",XmSTRING_DEFAULT_CHARSET);
   colrpref0 = XtVaCreateManagedWidget("Colrpref0",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNset, TRUE,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref0,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 0);

   string = XmStringCreate("Nodes",XmSTRING_DEFAULT_CHARSET);
   colrpref1 = XtVaCreateManagedWidget("Colrpref1",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref1,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 1);

   string = XmStringCreate("Particles",XmSTRING_DEFAULT_CHARSET);
   colrpref2 = XtVaCreateManagedWidget("Colrpref2",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref2,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 2);

   string = XmStringCreate("Cells",XmSTRING_DEFAULT_CHARSET);
   colrpref3 = XtVaCreateManagedWidget("Colrpref3",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref3,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 3);

   string = XmStringCreate("Cutplane",XmSTRING_DEFAULT_CHARSET);
   colrpref4 = XtVaCreateManagedWidget("Colrpref4",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref4,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 4);

   string = XmStringCreate("Cutsphere",XmSTRING_DEFAULT_CHARSET);
   colrpref5 = XtVaCreateManagedWidget("Colrpref5",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref5,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 5);

   string = XmStringCreate("Material Isosurface",XmSTRING_DEFAULT_CHARSET);
   colrpref6 = XtVaCreateManagedWidget("Colrpref6",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref6,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 6);

   string = XmStringCreate("Field Isosurface",XmSTRING_DEFAULT_CHARSET);
   colrpref7 = XtVaCreateManagedWidget("Colrpref7",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref7,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 7);

   string = XmStringCreate("Isovolume",XmSTRING_DEFAULT_CHARSET);
   colrpref8 = XtVaCreateManagedWidget("Colrpref8",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref8,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 8);

   string = XmStringCreate("Surfaces",XmSTRING_DEFAULT_CHARSET);
   colrpref9 = XtVaCreateManagedWidget("Colrpref9",
               xmToggleButtonGadgetClass,
               colrprefmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrpref9,XmNvalueChangedCallback,
                    docolorpref,(caddr_t) 9);

   string = XmStringCreate("Preference",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,colrprefmenu);
   i++;
   colrprefbut = XmCreateCascadeButton(colrbarmenu,"Colrprefbut",
                                       args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(colrprefbut);

   i=0; XtSetArg(args[i], XmNradioBehavior,TRUE);
   i++;
   colrtitlmenu = XmCreatePulldownMenu(colrbarmenu,"Colrtitlmenu",
                                       args,(Cardinal) i);

   string = XmStringCreate("Default",XmSTRING_DEFAULT_CHARSET);
   colrtitl0 = XtVaCreateManagedWidget("Colrtitl0",
               xmToggleButtonGadgetClass,
               colrtitlmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               XmNset, TRUE,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrtitl0,XmNvalueChangedCallback,
                    docolortitle,(caddr_t) 0);

   string = XmStringCreate("User Defined",XmSTRING_DEFAULT_CHARSET);
   colrtitl1 = XtVaCreateManagedWidget("Colrtitl1",
               xmToggleButtonGadgetClass,
               colrtitlmenu,
               XmNlabelString, string,
               XmNspacing, 1,
               XmNselectColor, yellowpix,
               NULL);
      XmStringFree(string);
      XtAddCallback(colrtitl1,XmNvalueChangedCallback,
                    docolortitle,(caddr_t) 1);

   string = XmStringCreate("Label",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,colrtitlmenu);
   i++;
   colrtitlbut = XmCreateCascadeButton(colrbarmenu,"Colrtitlbut",
                                       args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(colrtitlbut);
 
   coloreditmenu = XmCreatePulldownMenu(menu_pane4,"Coloreditmenu",NULL,0);

   string = XmStringCreate("Materials, Isosurfaces, Isovolume",
                           XmSTRING_DEFAULT_CHARSET);
   matedbut = XtVaCreateManagedWidget("Matedbut",
              xmPushButtonGadgetClass,
              coloreditmenu,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(matedbut,XmNactivateCallback,docoloredit,NULL);

   string = XmStringCreate("Field Data Color Map",XmSTRING_DEFAULT_CHARSET);
   fieldedbut = XtVaCreateManagedWidget("Fieldedbut",
                xmPushButtonGadgetClass,
                coloreditmenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(fieldedbut,XmNactivateCallback,dofcoloredit,NULL);

   string = XmStringCreate("Background Color",XmSTRING_DEFAULT_CHARSET);
   backgrndbut = XtVaCreateManagedWidget("Backgrndbut",
                 xmPushButtonGadgetClass,
                 coloreditmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(backgrndbut,XmNactivateCallback,dobackground,NULL);

   string = XmStringCreate("Coloredit",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,coloreditmenu);
   i++;
   coloreditbut = XmCreateCascadeButton(menu_pane4,"Coloreditbut",
                                        args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(coloreditbut);
  
   string = XmStringCreate("Contour Levels",XmSTRING_DEFAULT_CHARSET);
   ncontbut = XtVaCreateManagedWidget("Ncontbut",
              xmPushButtonGadgetClass,
              menu_pane4,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(ncontbut,XmNactivateCallback,pop_ncontours,NULL);

   string = XmStringCreateLtoR("Ctl-1",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane4);
   i++;
   cascade4 = XmCreateCascadeButton(menu_bar,"Cascade4",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade4);

   /*  Create control menu2.  */
   menu_pane5 = XmCreatePulldownMenu(menu_bar,"Menu_pane5",NULL,0);

   string = XmStringCreate("Cycle",XmSTRING_DEFAULT_CHARSET);
   cyclebut = XtVaCreateManagedWidget("Cyclebut",
              xmPushButtonGadgetClass,
              menu_pane5,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(cyclebut,XmNactivateCallback,time_cycle,
                    (caddr_t) 1);
 
   datalimmenu = XmCreatePulldownMenu(menu_pane5,"Datalimmenu",NULL,0);

   string = XmStringCreate("Node Fields",
                           XmSTRING_DEFAULT_CHARSET);
   fieldlimbut = XtVaCreateManagedWidget("Fieldlimbut",
                 xmPushButtonGadgetClass,
                 datalimmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(fieldlimbut,XmNactivateCallback,dofieldlim,NULL);

   string = XmStringCreate("Cell Fields",
                           XmSTRING_DEFAULT_CHARSET);
   cfieldlimbut = XtVaCreateManagedWidget("Cfieldlimbut",
                  xmPushButtonGadgetClass,
                  datalimmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(cfieldlimbut,XmNactivateCallback,docfieldlim,NULL);

   string = XmStringCreate("Particles",XmSTRING_DEFAULT_CHARSET);
   tracelimbut = XtVaCreateManagedWidget("Tracelimbut",
                 xmPushButtonGadgetClass,
                 datalimmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(tracelimbut,XmNactivateCallback,dotracelim,NULL);

   string = XmStringCreate("Surface Fields",
                           XmSTRING_DEFAULT_CHARSET);
   sfieldlimbut = XtVaCreateManagedWidget("Sfieldlimbut",
                  xmPushButtonGadgetClass,
                  datalimmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(sfieldlimbut,XmNactivateCallback,dosfieldlim,NULL);

   string = XmStringCreate("Face Fields",
                           XmSTRING_DEFAULT_CHARSET);
   ffieldlimbut = XtVaCreateManagedWidget("Ffieldlimbut",
                  xmPushButtonGadgetClass,
                  datalimmenu,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(ffieldlimbut,XmNactivateCallback,doffieldlim,NULL);

   string = XmStringCreate("Rays",XmSTRING_DEFAULT_CHARSET);
   raylimbut = XtVaCreateManagedWidget("Raylimbut",
               xmPushButtonGadgetClass,
               datalimmenu,
               XmNlabelString, string,
               XmNsensitive, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(raylimbut,XmNactivateCallback,doraylim,NULL);

   string = XmStringCreate("Auto Menu Popup",
                           XmSTRING_DEFAULT_CHARSET);
   datalimpopbut = XtVaCreateManagedWidget("Datalimpopbut",
                   xmToggleButtonGadgetClass,
                   datalimmenu,
                   XmNlabelString, string,
                   XmNset, FALSE,
                   XmNspacing, 2,
                   XmNselectColor, yellowpix,
                   NULL);
      XmStringFree(string);
      XtAddCallback(datalimpopbut,XmNvalueChangedCallback,dodatalimpop,NULL);

   string = XmStringCreate("Data Limits",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,datalimmenu);
   i++;
   datalimbut = XmCreateCascadeButton(menu_pane5,"Datalimbut",
                                      args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(datalimbut);

   distscalemenu = XmCreatePulldownMenu(menu_pane5,"Distscalemenu",NULL,0);

   string = XmStringCreate("On",XmSTRING_DEFAULT_CHARSET);
   distsclon = XtVaCreateManagedWidget("Distsclon",
               xmToggleButtonGadgetClass,
               distscalemenu,
               XmNlabelString, string,
               XmNspacing, 2,
               XmNselectColor, yellowpix,
               XmNset, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(distsclon,XmNvalueChangedCallback,dodistscale,
                    (caddr_t) 0);

   string = XmStringCreate("Set Distance Scale",
                           XmSTRING_DEFAULT_CHARSET);
   distsetbut = XtVaCreateManagedWidget("Distsetbut",
                xmPushButtonGadgetClass,
                distscalemenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(distsetbut,XmNactivateCallback,dodistset,NULL);

   string = XmStringCreate("Distance Scale",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,distscalemenu);
   i++;
   distscalebut = XmCreateCascadeButton(menu_pane5,"Distscalebut",
                                        args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(distscalebut);

   string = XmStringCreate("Interactivity",XmSTRING_DEFAULT_CHARSET);
   interactbut = XtVaCreateManagedWidget("Interactbut",
                  xmPushButtonGadgetClass,
                  menu_pane5,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(interactbut,XmNactivateCallback,pop_interact,NULL);

   string = XmStringCreate("Light",XmSTRING_DEFAULT_CHARSET);
   lightbut = XtVaCreateManagedWidget("Lightbut",
                  xmPushButtonGadgetClass,
                  menu_pane5,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);
      XtAddCallback(lightbut,XmNactivateCallback,pop_light,NULL);
 
   string = XmStringCreate("Line Width",XmSTRING_DEFAULT_CHARSET);
   lnsizebut = XtVaCreateManagedWidget("Lnsizebut",
               xmPushButtonGadgetClass,
               menu_pane5,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(lnsizebut,XmNactivateCallback,dolnsize,NULL);
 
   string = XmStringCreate("Point Size",XmSTRING_DEFAULT_CHARSET);
   ptsizebut = XtVaCreateManagedWidget("Ptsizebut",
               xmPushButtonGadgetClass,
               menu_pane5,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(ptsizebut,XmNactivateCallback,doptsize,NULL);
 
   string = XmStringCreate("Plot Box",XmSTRING_DEFAULT_CHARSET);
   plotboxbut = XtVaCreateManagedWidget("Plotboxbut",
                xmPushButtonGadgetClass,
                menu_pane5,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(plotboxbut,XmNactivateCallback,doplotbox,NULL);
  
   string = XmStringCreate("Scale Axes",XmSTRING_DEFAULT_CHARSET);
   sclaxbut = XtVaCreateManagedWidget("Sclaxbut",
              xmPushButtonGadgetClass,
              menu_pane5,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(sclaxbut,XmNactivateCallback,doscaleaxis,NULL);

   string = XmStringCreateLtoR("Ctl-2",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane5);
   i++;
   cascade5 = XmCreateCascadeButton(menu_bar,"Cascade5",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade5);

   /*  Create control menu3.  */
   menu_pane6 = XmCreatePulldownMenu(menu_bar,"Menu_pane6",NULL,0);
 
   subsetmenu = XmCreatePulldownMenu(menu_pane6,"Subsetmenu",NULL,0);

   string = XmStringCreate("Nodes, Cells, Particles",
                           XmSTRING_DEFAULT_CHARSET);
   othersubset = XtVaCreateManagedWidget("Othersubset",
                 xmPushButtonGadgetClass,
                 subsetmenu,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);
      XtAddCallback(othersubset,XmNactivateCallback,dosubset,NULL);

   string = XmStringCreate("Polygons",XmSTRING_DEFAULT_CHARSET);
   polysubset = XtVaCreateManagedWidget("Polysubset",
                xmPushButtonGadgetClass,
                subsetmenu,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(polysubset,XmNactivateCallback,dopolysubset,NULL);

   string = XmStringCreate("Subset",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,subsetmenu);
   i++;
   subsetbut = XmCreateCascadeButton(menu_pane6,"Subsetbut",
                                     args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(subsetbut);

   string = XmStringCreate("Texture Smoothing",XmSTRING_DEFAULT_CHARSET);
   texturebut = XtVaCreateManagedWidget("Texturebut",
                xmToggleButtonGadgetClass,
                menu_pane6,
                XmNlabelString, string,
                XmNspacing, 1,
                XmNselectColor, yellowpix,
                XmNset, TRUE,
                NULL);
      XmStringFree(string);
      XtAddCallback(texturebut,XmNvalueChangedCallback,
                    dotexture,NULL);
 
   string = XmStringCreate("Time",XmSTRING_DEFAULT_CHARSET);
   timebut = XtVaCreateManagedWidget("Timebut",
             xmPushButtonGadgetClass,
             menu_pane6,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(timebut,XmNactivateCallback,time_cycle,
                    (caddr_t) 0);
 
   string = XmStringCreate("Title",XmSTRING_DEFAULT_CHARSET);
   titlebut = XtVaCreateManagedWidget("Titlebut",
              xmPushButtonGadgetClass,
              menu_pane6,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(titlebut,XmNactivateCallback,pop_title,
                    (caddr_t) 0);

   string = XmStringCreate("Use Display List",XmSTRING_DEFAULT_CHARSET);
   displistbut = XtVaCreateManagedWidget("Displistbut",
                 xmToggleButtonGadgetClass,
                 menu_pane6,
                 XmNlabelString, string,
                 XmNspacing, 1,
                 XmNselectColor, yellowpix,
                 XmNset, FALSE,
                 NULL);
      XmStringFree(string);
      XtAddCallback(displistbut,XmNvalueChangedCallback,
                    dodisplist,NULL);

   string = XmStringCreate("Use Virtual Trackball",XmSTRING_DEFAULT_CHARSET);
   trackballbut = XtVaCreateManagedWidget("Trackballbut",
                  xmToggleButtonGadgetClass,
                  menu_pane6,
                  XmNlabelString, string,
                  XmNspacing, 1,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
      XmStringFree(string);
      XtAddCallback(trackballbut,XmNvalueChangedCallback,
                    dotrackball,NULL);
 
   string = XmStringCreate("Vector Controls",XmSTRING_DEFAULT_CHARSET);
   vectbut = XtVaCreateManagedWidget("Vectbut",
             xmPushButtonGadgetClass,
             menu_pane6,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
      XtAddCallback(vectbut,XmNactivateCallback,pop_vectorctl,NULL);
 
   string = XmStringCreate("Window Size",XmSTRING_DEFAULT_CHARSET);
   winsizebut = XtVaCreateManagedWidget("Winsizebut",
                xmPushButtonGadgetClass,
                menu_pane6,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(winsizebut,XmNactivateCallback,pop_winsize,NULL);
  
   string = XmStringCreate("Zoom (Rubberband)",XmSTRING_DEFAULT_CHARSET);
   rbzoombut = XtVaCreateManagedWidget("Rbzoombut",
               xmPushButtonGadgetClass,
               menu_pane6,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
      XtAddCallback(rbzoombut,XmNactivateCallback,pop_rbzoom,NULL);

   string = XmStringCreateLtoR("Ctl-3",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane6);
   i++;
   cascade6 = XmCreateCascadeButton(menu_bar,"Cascade6",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade6);

   /*  Create reflection menu.  */
   menu_pane7 = XmCreatePulldownMenu(menu_bar,"Menu_pane7",NULL,0);
 
   string = XmStringCreate("X Reflect",XmSTRING_DEFAULT_CHARSET);
   objxrefl = XtVaCreateManagedWidget("Objxrefl",
              xmToggleButtonGadgetClass,
              menu_pane7,
              XmNlabelString, string,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(objxrefl,XmNvalueChangedCallback,reflections,
                    (caddr_t) 0);
 
   string = XmStringCreate("Y Reflect",XmSTRING_DEFAULT_CHARSET);
   objyrefl = XtVaCreateManagedWidget("Objyrefl",
              xmToggleButtonGadgetClass,
              menu_pane7,
              XmNlabelString, string,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(objyrefl,XmNvalueChangedCallback,reflections,
                    (caddr_t) 1);
 
   string = XmStringCreate("Z Reflect",XmSTRING_DEFAULT_CHARSET);
   objzrefl = XtVaCreateManagedWidget("Objzrefl",
              xmToggleButtonGadgetClass,
              menu_pane7,
              XmNlabelString, string,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
      XmStringFree(string);
      XtAddCallback(objzrefl,XmNvalueChangedCallback,reflections,
                    (caddr_t) 2);

   string = XmStringCreate("Reflections",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane7);
   i++;
   cascade7 = XmCreateCascadeButton(menu_bar,"Cascade7",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade7);

   /*  Create view menu.  */
   i=0; XtSetArg(args[i], XmNradioBehavior,TRUE);
   i++;
   menu_pane8 = XmCreatePulldownMenu(menu_bar,"Menu_pane8",
                                     args,(Cardinal) i);
 
   string = XmStringCreate("Orthographic",
                           XmSTRING_DEFAULT_CHARSET);
   view1 = XtVaCreateManagedWidget("View1",
           xmToggleButtonGadgetClass,
           menu_pane8,
           XmNlabelString, string,
           XmNspacing, 2,
           XmNselectColor, yellowpix,
           XmNset, TRUE,
           NULL);
      XmStringFree(string);
      XtAddCallback(view1,XmNvalueChangedCallback,setview,
                    (caddr_t) 0);

   string = XmStringCreate("Perspective",
                           XmSTRING_DEFAULT_CHARSET);
   view2 = XtVaCreateManagedWidget("View2",
           xmToggleButtonGadgetClass,
           menu_pane8,
           XmNlabelString, string,
           XmNspacing, 2,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(view2,XmNvalueChangedCallback,setview,
                    (caddr_t) 1);

   string = XmStringCreate("Flight",
                           XmSTRING_DEFAULT_CHARSET);
   view3 = XtVaCreateManagedWidget("View3",
           xmToggleButtonGadgetClass,
           menu_pane8,
           XmNlabelString, string,
           XmNspacing, 2,
           XmNselectColor, yellowpix,
           NULL);
      XmStringFree(string);
      XtAddCallback(view3,XmNvalueChangedCallback,setview,
                    (caddr_t) 2);

   string = XmStringLtoRCreate("Reset Original\nAngles & Mag.",
                           XmSTRING_DEFAULT_CHARSET);
   view6 = XtVaCreateManagedWidget("View6",
           xmPushButtonGadgetClass,
           menu_pane8,
           XmNlabelString, string,
           XmNspacing, 2,
           NULL);
      XmStringFree(string);
      XtAddCallback(view6,XmNactivateCallback,reset_startangles,
                    (caddr_t) 0);

   string = XmStringCreate("View",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNlabelString,string);
   i++; XtSetArg(args[i], XmNsubMenuId,menu_pane8);
   i++;
   cascade8 = XmCreateCascadeButton(menu_bar,"Cascade8",
                                    args,(Cardinal) i);
      XmStringFree(string);
      XtManageChild(cascade8);

   XtManageChild(menu_bar);

   /*  Create form widget for axis and sliders.  */
      
   topform = XtVaCreateManagedWidget("Topform",
             xmFormWidgetClass,
             form,
             XmNtopAttachment, XmATTACH_WIDGET,
	     XmNtopWidget, menu_bar,
	     XmNtopOffset, 5,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             NULL);
      XtManageChild(topform); 


   /*  Create axis window.  */
 
   axisframe = XtVaCreateManagedWidget("Axisframe",
               xmFrameWidgetClass,
               topform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
	       XmNresizable, False,
               XmNheight, 82,
               XmNwidth, 82,
               NULL);
      XtManageChild(axisframe); 

   i = 0;
   XtSetArg(args[i], XmNheight,80);i++;
   XtSetArg(args[i], XmNwidth,80);i++;
   XtSetArg(args[i], GLwNvisualInfo, vi);i++;
   if (mesacmap)
     { XtSetArg(args[i], XmNcolormap, my_cmap); i++; }
   axglw = GLwCreateMDrawingArea(axisframe,"Axglw",args,i); 
      XtManageChild (axglw); 
      XtAddCallback(axglw,GLwNexposeCallback,expose_axwindow,
                    (caddr_t) 0);
      XtAddCallback(axglw,GLwNginitCallback,init_axwindow,
                    (caddr_t) 0);
 
   /*  Create information label.  */

   string = XmStringCreate("GMV initializing.                              ",
                           XmSTRING_DEFAULT_CHARSET);
   gmvmsglab = XtVaCreateManagedWidget("Gmvmsglab",
             xmLabelWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_FORM,
             XmNleftAttachment, XmATTACH_WIDGET,
	     XmNleftWidget, axisframe,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_FORM,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             XmNforeground, redpix,
	     // XmNalignment, XmALIGNMENT_BEGINNING,
             NULL);
      XmStringFree(string);
 
   /*  Create drawing window top control sliders.  */

   /* scale_size = windowwidth - 200; */
   scale_size = 250;
 
   string = XmStringCreate("Twist",XmSTRING_DEFAULT_CHARSET);
   twlabel = XtVaCreateManagedWidget("Twlabel",
             xmLabelWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, gmvmsglab,
             /* XmNtopOffset, 10, */
             XmNleftAttachment, XmATTACH_WIDGET,
	     XmNleftWidget, axisframe,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   objzrot = XtVaCreateManagedWidget("Objzrot",
             xmScaleWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, twlabel,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, twlabel,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, -200,
             XmNmaximum, 200,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, 0,
             XmNscaleWidth, scale_size,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(objzrot,XmNvalueChangedCallback,rotator,
                    (caddr_t) 2);
      XtAddCallback(objzrot,XmNdragCallback,rotatortxt,
                    (caddr_t) 2);
   objzrottxt = XtVaCreateManagedWidget("Objzrottxt",
                xmTextFieldWidgetClass,
                topform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, objzrot,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objzrot,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNmaxLength, 10,
                XmNcolumns, 6,
                XmNvalue, "  0.0",
                NULL);
      XtAddCallback(objzrottxt,XmNactivateCallback,
                    rdangles,(caddr_t) 2);
 
   string = XmStringCreate("Elev.",XmSTRING_DEFAULT_CHARSET);
   elevlabel = XtVaCreateManagedWidget("Elevlabel",
               xmLabelWidgetClass,
               topform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, objzrottxt,
               XmNleftAttachment, XmATTACH_WIDGET,
	       XmNleftWidget, axisframe,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
   objyrot = XtVaCreateManagedWidget("Objyrot",
             xmScaleWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, elevlabel,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objzrot,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, -200,
             XmNmaximum, 200,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, 0,
             XmNscaleWidth, scale_size,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(objyrot,XmNvalueChangedCallback,rotator,
                    (caddr_t) 1);
      XtAddCallback(objyrot,XmNdragCallback,rotatortxt,
                    (caddr_t) 1);
   objyrottxt = XtVaCreateManagedWidget("Objyrottxt",
                xmTextFieldWidgetClass,
                topform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, objyrot,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objyrot,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNmaxLength, 10,
                XmNcolumns, 6,
                XmNvalue, "  0.0",
                NULL);
      XtAddCallback(objyrottxt,XmNactivateCallback,
                    rdangles,(caddr_t) 1);
 
   string = XmStringCreate("Azim.",XmSTRING_DEFAULT_CHARSET);
   azlabel = XtVaCreateManagedWidget("Azlabel",
             xmLabelWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, objyrottxt,
             XmNleftAttachment, XmATTACH_WIDGET,
	     XmNleftWidget, axisframe,
             XmNleftOffset, 10,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);
   objxrot = XtVaCreateManagedWidget("Objxrot",
             xmScaleWidgetClass,
             topform,
             XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNtopWidget, azlabel,
             XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
             XmNleftWidget, objyrot,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNminimum, -200,
             XmNmaximum, 200,
             XmNorientation, XmHORIZONTAL,
             XmNprocessingDirection, XmMAX_ON_RIGHT,
             XmNvalue, 0,
             XmNscaleWidth, scale_size,
             XmNscaleMultiple, 1,
             NULL);
      XtAddCallback(objxrot,XmNvalueChangedCallback,rotator,
                    (caddr_t) 0);
      XtAddCallback(objxrot,XmNdragCallback,rotatortxt,
                    (caddr_t) 0);
   objxrottxt = XtVaCreateManagedWidget("Objxrottxt",
                xmTextFieldWidgetClass,
                topform,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, objxrot,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, objxrot,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 0,
                XmNmaxLength, 10,
                XmNcolumns, 6,
                XmNvalue, "  0.0",
                NULL);
      XtAddCallback(objxrottxt,XmNactivateCallback,
                    rdangles,(caddr_t) 0);
 
   string = XmStringCreate("       ",XmSTRING_DEFAULT_CHARSET);
   objdistval = XtVaCreateManagedWidget("Objdistval",
               xmLabelWidgetClass,
               topform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, gmvmsglab,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, objzrottxt,
               XmNleftOffset, 10,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   /*  Create form widget for mag. sliders and text.  */
      
   leftform = XtVaCreateManagedWidget("Topform",
              xmFormWidgetClass,
              form,
              XmNtopAttachment, XmATTACH_WIDGET,
	      XmNtopWidget, topform,
	      XmNtopOffset, 5,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              NULL);
      XtManageChild(leftform); 

   string = XmStringCreate(" Mag. ",XmSTRING_DEFAULT_CHARSET);
   maglabel = XtVaCreateManagedWidget("Maglabel",
              xmLabelWidgetClass,
              leftform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNtopWidget, topform,
              /* XmNtopOffset, 10, */
              XmNleftAttachment, XmATTACH_FORM,
              /* XmNleftOffset, 20, */
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
   objmagnify = XtVaCreateManagedWidget("Objmagnify",
                xmScaleWidgetClass,
                leftform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, maglabel,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 20,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNminimum, -400,
                XmNmaximum, 600,
                XmNvalue, 0,
                XmNorientation, XmVERTICAL,
                XmNprocessingDirection, XmMAX_ON_TOP,
                XmNscaleHeight, scale_size,
                XmNscaleMultiple, 2,
                NULL);
      XtAddCallback(objmagnify,XmNvalueChangedCallback,magnifier,
                    NULL);
      XtAddCallback(objmagnify,XmNdragCallback,magnifiertxt,NULL);
   objmagnifytxt = XtVaCreateManagedWidget("Objmagnifytxt",
                   xmTextFieldWidgetClass,
                   leftform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, objmagnify,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNmarginHeight, 0,
                   XmNmarginWidth, 0,
                   XmNmaxLength, 10,
                   XmNcolumns, 5,
                   XmNvalue, " 1.00",
                   NULL);
      XtAddCallback(objmagnifytxt,XmNactivateCallback,
                    rdmag,(caddr_t) 0);

   /*  Create OpenGL drawing window frame.  */
 
   drawframe = XmCreateFrame(form, "Drawframe", NULL, 0);
   XtVaSetValues(drawframe,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, topform,
	       XmNtopOffset, 5,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, leftform,
               XmNrightAttachment, XmATTACH_FORM,
               XmNbottomAttachment, XmATTACH_FORM,
               XmNheight, windowheight+4,
               XmNwidth, windowwidth+4,
               NULL);
      XtManageChild (drawframe);
   
   /*  Create OpenGL drawing window.  */
   i = 0;
   XtSetArg(args[i], XmNheight,windowheight);i++;
   XtSetArg(args[i], XmNwidth,windowwidth);i++;
   XtSetArg(args[i], GLwNvisualInfo, vi);i++;
   if (mesacmap)
     { XtSetArg(args[i], XmNcolormap, my_cmap);i++; }
   glw = GLwCreateMDrawingArea(drawframe,"Glw",args,i);
      XtManageChild (glw); 
  
      XtAddCallback(glw,GLwNexposeCallback,draw_expose_callback,
                    (caddr_t) 0);
      XtAddCallback(glw,GLwNresizeCallback,do_resize,
                    (caddr_t) 0);
      XtAddCallback(glw,GLwNginitCallback,init_window,
                    (caddr_t) 0);
      XtAddCallback(glw,GLwNinputCallback,input,(caddr_t) 0);

   /*  Create crosshair cursor.  */
 
   XQueryBestCursor(dpy,RootWindow(dpy,scrn),windowwidth,
                    windowheight,&cwidth,&cheight);
   if (cwidth > 32) cwidth = 32;
   if (cheight > 32)  cheight = 32;
   csource = XCreatePixmap(dpy,RootWindow(dpy,scrn),
                           cwidth,cheight,1);
   gcsource = XCreateGC(dpy,csource,0,0);
   cmask = XCreatePixmap(dpy,RootWindow(dpy,scrn),
                         cwidth,cheight,1);
   gcmask = XCreateGC(dpy,cmask,0,0);
 
   XSetForeground(dpy,gcsource,0);
   XFillRectangle(dpy,csource,gcsource,0,0,cwidth,cheight);
   XSetForeground(dpy,gcsource,1);
   x1 = 0; x2 = cwidth; y1 = y2 = cheight/2;
   XDrawLine(dpy,csource,gcsource,x1,y1,x2,y2);
   x1 = x2 = cwidth/2; y1 = 0; y2 = cheight;
   XDrawLine(dpy,csource,gcsource,x1,y1,x2,y2);
 
   XSetForeground(dpy,gcmask,0);
   XFillRectangle(dpy,cmask,gcmask,0,0,cwidth,cheight);
   XSetForeground(dpy,gcmask,1);
   x1 = 0; x2 = cwidth; y1 = y2 = cheight/2;
   XDrawLine(dpy,cmask,gcmask,x1,y1+1,x2,y2+1);
   XDrawLine(dpy,cmask,gcmask,x1,y1,x2,y2);
   XDrawLine(dpy,cmask,gcmask,x1,y1-1,x2,y2-1);
   x1 = x2 = cwidth/2; y1 = 0; y2 = cheight;
   XDrawLine(dpy,cmask,gcmask,x1+1,y1,x2+1,y2);
   XDrawLine(dpy,cmask,gcmask,x1,y1,x2,y2);
   XDrawLine(dpy,cmask,gcmask,x1-1,y1,x2-1,y2);
 
   cback.red = 65535; cback.green = 65535; cback.blue = 65535;
   cfore.red = 65535; cfore.green = 0; cfore.blue = 0;
   xhot = cwidth/2; yhot = cheight/2;
   crosshair = XCreatePixmapCursor(dpy,csource,cmask,&cfore,
                                   &cback,xhot,yhot);

   /*  Create a widget used as the popup parents.  */

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   poplevel = XtVaCreateManagedWidget("Poplevel",
              xmLabelWidgetClass,
              form,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, menu_bar,
              XmNleftAttachment, XmATTACH_NONE,
              XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
              XmNrightWidget, drawframe,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);

   string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
   poplevel2 = XtVaCreateManagedWidget("Poplevel2",
               xmLabelWidgetClass,
               form,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, menu_bar,
               XmNleftAttachment, XmATTACH_WIDGET,
               XmNleftWidget, objdistval,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);

   /*  Create file selection popups.  */

   string = XmStringCreate("Read GMV File",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNfileTypeMask,XmFILE_REGULAR);
   i++;
   file_sel = XmCreateFileSelectionDialog(form,"Filesel",
                                          args,(Cardinal) i);
      XmStringFree(string);
      XtAddCallback(file_sel,XmNokCallback,getfilenameg,NULL);
      XtAddCallback(file_sel,XmNcancelCallback,filecancelg,NULL); 

   string = XmStringCreate("Read Attributes File",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNfileTypeMask,XmFILE_REGULAR);
   i++;
   file_sel2 = XmCreateFileSelectionDialog(form,"Filesel2",
                                          args,(Cardinal) i);
      XmStringFree(string);
      XtAddCallback(file_sel2,XmNokCallback,getfilename,NULL);
      XtAddCallback(file_sel2,XmNcancelCallback,filecancel,NULL); 

   /*  Create error popup form.  */
 
   string = XmStringCreate("Error Message",
                           XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(args[i], XmNdialogTitle,string);
   i++; XtSetArg(args[i], XmNx, 500);
   i++; XtSetArg(args[i], XmNy, 10);
   i++; XtSetArg(args[i], XmNautoUnmanage, FALSE);
   i++;
   errorform = XmCreateFormDialog(form,"Errorform",
                                  args,(Cardinal) i);
      XmStringFree(string);
 
   string = XmStringCreate("   ",
                           XmSTRING_DEFAULT_CHARSET);
   objerrlab = XtVaCreateManagedWidget("Objerrlab",
               xmLabelWidgetClass,
               errorform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               NULL);
      XmStringFree(string);
 
   string = XmStringCreate("Ok",XmSTRING_DEFAULT_CHARSET);
   errokbut = XtVaCreateManagedWidget("Errokbut",
              xmPushButtonGadgetClass,
              errorform,
              XmNtopAttachment, XmATTACH_WIDGET,
              XmNtopWidget, objerrlab,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(errokbut,XmNactivateCallback,errok,NULL);


   /*  Create misc. control popup forms (clip        */
   /*  interactivity, title, contour levels, light.  */
   makemisc_ctlform(poplevel);

   /*  Create snapshotform popup form.  */
   makesnapshotform(poplevel);

   /*  Create animflyform popup form.  */
   makeanimflyform(poplevel);

   /*  Create scaleaxisform popup form.  */
   makescaleaxisform(poplevel);

   /*  Create node, cell and tracer subset popup form.  */
   makesubsetform(poplevel);

   /*  Create bounding box popup form.  */
   makeboundingboxform(poplevel);

   /*  Create plot box popup form.  */
   makeplotboxform(poplevel);

   /*  Create center popup form.  */
   makecenterform(poplevel);

   /*  Create point size control popup form.  */
   makepntsizeform(poplevel);

   /*  Create auto read form.  */
   makeautordform(poplevel);

   /*  Create line width control popup form.  */
   makelinesizeform(poplevel);

   /*  Create field color map edit form.  */
   makefcoloreditform(poplevel);

   /*  Create cutline form.  */
   makecutlineform(poplevel);

   /*  Create cutplane form.  */
   makecutplaneform(poplevel);

   /*  Create cutsphere form.  */
   makecutsphereform(poplevel);

   /*  Create isovolume form.  */
   makeisovolform(poplevel);

   /*  Create background form.  */
   makebackgrndform(poplevel);

   /*  Create vector control form.  */
   makevectctlform(poplevel);

   /*  Create zoom form.  */
   makerbzoomform(poplevel);

   /*  Create node field average form.  */
   makenfldavgform(poplevel2);

   /*  Create cell field average form.  */
   makecfldavgform(poplevel2);

   /*  Create window size form.  */
   makewindowsizeform(poplevel2);

   /*  Create material/flag color bar form.  */
   makematcolorbarform(poplevel2);

#ifdef PHYSED
   /*  Create PhysEd forms.  */
   makePEforms(poplevel);
#endif

   /*  Add window decoration exit button.  */
   SetWMCloseProtocol(toplevel);

   values.background = whitepix;
   values.foreground = redpix;
   lpgc = XtGetGC(objlight, GCForeground | GCBackground, &values);
}


void addstereobuttons()
{
  XmString string;

   if (has_fullscreenstereo || stereowindow)
     {
      string = XmStringLtoRCreate
              ("Stereo Perspective - \nPress x key to exit stereo.",
                           XmSTRING_DEFAULT_CHARSET);
      view4 = XtVaCreateManagedWidget("View4",
              xmToggleButtonGadgetClass,
              menu_pane8,
              XmNlabelString, string,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
         XmStringFree(string);
         XtAddCallback(view4,XmNvalueChangedCallback,setview,
                       (caddr_t) 3);

      string = XmStringLtoRCreate
              ("Stereo Flight - \nPress x key to exit stereo.",
                           XmSTRING_DEFAULT_CHARSET);
      view5 = XtVaCreateManagedWidget("View5",
              xmToggleButtonGadgetClass,
              menu_pane8,
              XmNlabelString, string,
              XmNspacing, 2,
              XmNselectColor, yellowpix,
              NULL);
         XmStringFree(string);
         XtAddCallback(view5,XmNvalueChangedCallback,setview,
                       (caddr_t) 4);
     }
}
