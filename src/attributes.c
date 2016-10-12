
#ifdef NT
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <malloc.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/TextF.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#define NUM_WORDS 93

#include "gmvdata.h"
#include "formwidgets.h"
#include "lights.h"
#include "contour.h"
#include "cutplane.h"
#include "xstuff.h"

/* function prototypes */
void putattributes(void);
void put_doubles(double *data, int number, FILE *fp);
void put_floats(float *data, int number, FILE *fp);
void put_ints(int *data, int number, FILE *fp);
void put_uints(unsigned long *data, int number, FILE *fp);
void put_chars(char *data, int number, FILE *fp);
void put_struct_fldcalc(struct fldcalc *data, int number, int itype, FILE *fp);
void put_shorts(short *data, int number, FILE *fp);
void cpywblanks(char tmpname[50], char * inname);

void getattributes(void);
void getattributes_new(void);
void getattributes_new2(void);
void getattributes_old(void);
void getattr_set_defaults(void);
void getattr_set_data(void);
void getattr_set_widgets();
void scan_doubles(double *data, int number, FILE *fp);
void scan_floats(float *data, int number, FILE *fp);
void scan_ints(int *data, int number, FILE *fp);
void scan_uints(unsigned long *data, int number, FILE *fp);
void scan_shorts(short *data, int number, FILE *fp);
void scan_chars(char *data, int number, FILE *fp);
void scan_struct_fldcalc(struct fldcalc *data, int number, FILE *fp);
void scan_struct_fldcalc2(struct fldcalc *data, int itype, FILE *fp);
int check_attrib_version(void);

/* global vars */
extern short noresizeflag;
int attribsversion;

/*-----------------------------------------------------------------*/
/*		THE PUT ATTRIBUTES SECTION			   */
/*-----------------------------------------------------------------*/
void putattributes(void)
{
  FILE *out;
  int maxmats, i, j, k, chkcont, mr, mg, mb, nmats;
  char tmpfilnam[256], tmpname[50], tmptype[10];
  char tmpname1[33], tmpname2[33], tmpname3[33];
  char str[] = 
    "Error - unable to open attributes output file!\nCheck permissions.";
  XmString string;

   maxmats = MAXMATS;

   /* Write all the ascii data at once. */

   out = fopen(attribsfilnam, "w");
   if (out == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str, XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      return;
     }

   fprintf(out, "gmv_attributes2\n\n");
   strcpy(tmpfilnam,filnam);
   j = strlen(tmpfilnam);
   for (i = 0; i < j; i++)
     {
      if (isspace(*(tmpfilnam+i)))
         *(tmpfilnam+i) = '-';
     }
   fprintf(out, "filename %s\n\n", tmpfilnam);

   /*  Write the material color values.  */
   nmats = mmats;
   if (maxcellmat >= mmats) nmats = maxcellmat;
   fprintf(out, "matrgba %d ", 4*nmats);
   put_floats(matrgba, 4*nmats, out);

   /*  Write out window size values.  */
   fprintf(out,"windowwidth %ld  windowheight %ld\n",windowwidth,windowheight);

   /*  Write polygon material selection data.  */
   for (i = 0; i < nmats; i++)
     {
      if (active_mat[i])
        {
         cpywblanks(tmpname,mmatnames[i]);
         fprintf(out, "active_mat %d %s\n", i, tmpname);
        }
     }
   fprintf(out, "\n");

   fprintf(out, "angles %e %e %e\n\n", xrot, yrot, zrot);
   fprintf(out, "magnify %e\n\n", magnify);
   fprintf(out, "range %e %e %e\n\n", xrange, yrange, zrange);
   fprintf(out, "span %e\n\n", span);
   fprintf(out, "aspect %e\n\n", aspect);
   fprintf(out, "pan %e %e\n\n", xpan, ypan);
   fprintf(out, "translate %e %e %e\n\n", xtrans, ytrans, ztrans);
   fprintf(out, "plotbox_mins %e %e %e\n\n", 
                 globxicmin, globyicmin, globzicmin);
   fprintf(out, "plotbox_maxs %e %e %e\n\n", 
                 globxicmax, globyicmax, globzicmax);

   /*  Field calculation information must be written out     */
   /*  so that getattributes_new2 can place the variable     */
   /*  names in the appropriate variable names for possible  */
   /*  selection by other functions.                         */

   /*  Write node field calculation structure.  */
   if (numvars > 0)
     {
      put_struct_fldcalc(fieldcalc, MAXFLDCALC, 1, out);
     }

   /*  Write cell field calculation structure.  */
   if (cnumvars > 0)
     {
      put_struct_fldcalc(cfieldcalc, MAXFLDCALC, 0, out);
     }

   if (snumvars > 0)
     {
      put_struct_fldcalc(sfieldcalc, MAXFLDCALC, 2, out);
     }

   if (numvartr > 0)
     {
      put_struct_fldcalc(tfieldcalc, MAXFLDCALC, 3, out);
     }

   /*  Write all cutplane or cutsphere   */
   /*  info for each existing cutplane.  */
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      if (cutplane[i].npolys > 0)
        {
         fprintf(out, "cutplane %d ",i);
         if (i < MAXCUTPLANES)
           {
            if (cutplanefieldactive == -2)
              {
               strcpy(tmptype,"Cell");
               strcpy(tmpname,"MATERIAL");
              }
            else if (cutplanefieldactive == -1)
              {
               strcpy(tmptype,"Node");
               strcpy(tmpname,"MATERIAL");
              }
            else if (cutplanefieldactive > numvars)
              {
               strcpy(tmptype,"Cell");
               j = cutplanefieldactive - numvars;
               cpywblanks(tmpname,cfieldname[j]);
              }
            else
              {
               strcpy(tmptype,"Node");
               j = cutplanefieldactive;
               cpywblanks(tmpname,fieldname[j]);
              }
           }
         else
           {
            if (cutspherefieldactive == -2)
              {
               strcpy(tmptype,"Cell");
               strcpy(tmpname,"MATERIAL");
              }
            else if (cutspherefieldactive == -1)
              {
               strcpy(tmptype,"Node");
               strcpy(tmpname,"MATERIAL");
              }
            else if (cutspherefieldactive > numvars)
              {
               strcpy(tmptype,"Cell");
               j = cutplanefieldactive - numvars;
               cpywblanks(tmpname,cfieldname[j]);
              }
            else
              {
               strcpy(tmptype,"Node");
               j = cutplanefieldactive;
               cpywblanks(tmpname,fieldname[j]);
              }
           }
         fprintf(out, "%d %d %s %s\n", cutplane_on[i], cutplane[i].npolys,    
                 tmptype, tmpname);
         fprintf(out, "     %e %e %e %e\n", cutplane[i].aa, cutplane[i].bb, 
                 cutplane[i].cc, cutplane[i].dd);
         fprintf(out, "     %e %e %e\n", cutplane[i].x1, cutplane[i].x2, 
                 cutplane[i].x3);
         fprintf(out, "     %e %e %e\n", cutplane[i].y1, cutplane[i].y2, 
                 cutplane[i].y3);
         fprintf(out, "     %e %e %e\n", cutplane[i].z1, cutplane[i].z2, 
                 cutplane[i].z3);
         fprintf(out, "     %e %e %e\n", cutplane[i].norm[0], 
                 cutplane[i].norm[1], cutplane[i].norm[2]);
         fprintf(out, "     %d\n", 10);  /* there are currently 10 cutplane flags */
         fprintf(out, "      %d %e %e %d %d %d %d %d %d %d\n\n", 
                 cutplane[i].heightflag, cutplane[i].heightfac, 
                 cutplane[i].dist,  cutplane[i].edgesflag, 
                 cutplane[i].velflag, cutplane[i].nodeflag, 
                 cutplane[i].cliponbox, cutplane[i].cliponcellsel, 
                 cutplane[i].linesflag, cutplane[i].facesflag);
        }
     }

   /*  Write drawing flags.  */
   fprintf(out, "explodefact %e %e %e\n\n", cexplodefact, pexplodefact, 
                                            sexplodefact);
   fprintf(out, "velscale %e\n\n", velscale);
   fprintf(out, "velarrowsize %e\n\n", velarrowsize);
   fprintf(out, "axisflag %d\n\n", axisflag);
   fprintf(out, "nodesflag %d\n\n", nodesflag);
   fprintf(out, "linesflag %d\n\n", linesflag);
   fprintf(out, "polysflag %d\n\n", polysflag);
   fprintf(out, "cellsflag %d\n\n", cellsflag);
   fprintf(out, "celledgesflag %d\n\n", celledgesflag);
   fprintf(out, "cellhiddenedgesflag %d\n\n", cellhiddenedgesflag);
   fprintf(out, "cellexpflag %d\n\n", cellexpflag);
   fprintf(out, "timeflag %d\n\n", timeflag);
   fprintf(out, "cycleflag %d\n\n", cycleflag);
   fprintf(out, "distscaleflag %d\n\n", distscaleflag);
   fprintf(out, "textureflag %d\n\n", textureflag);
   fprintf(out, "tracerflag %d\n\n", tracerflag);

   /*  Write active field names.  */
   j = nodefieldactive;
   strcpy(tmpname," ");
   if (j == -1) strcpy(tmptype,"MATERIAL");
   else if (j == -2) strcpy(tmptype,"GROUP");
   else if (j < numvars)
     {
      strcpy(tmptype,"Node");
      cpywblanks(tmpname,fieldname[j]);
     }
   else 
     {
      i = j - numvars;
      strcpy(tmptype,"Flags");
      cpywblanks(tmpname,flagname_node[i]);
     }
   fprintf(out, "nodefieldactive %s %s\n\n", tmptype, tmpname);

   j = cellfieldactive;
   strcpy(tmpname," ");
   if (j == -1) strcpy(tmptype,"MATERIAL");
   else if (j == -2) strcpy(tmptype,"Cell_GROUP");
   else if (j == -3) strcpy(tmptype,"Face_GROUP");
   else if (j == -4) strcpy(tmptype,"Hidden");
   else if (j < numvars)
     {
      strcpy(tmptype,"Node");
      cpywblanks(tmpname,fieldname[j]);
     }
   else if (j < numvars + cnumvars)
     {
      i = j - numvars;
      strcpy(tmptype,"Cell");
      cpywblanks(tmpname,cfieldname[i]);
     }
   else if (j < numvars + cnumvars + fnumvars)
     {
      i = j - (numvars + cnumvars);
      strcpy(tmptype,"Face");
      cpywblanks(tmpname,ffieldname[i]);
     }
   else 
     {
      i = j - (numvars + cnumvars + fnumvars);
      strcpy(tmptype,"Flags");
      cpywblanks(tmpname,flagname_cell[i]);
     }
   fprintf(out, "cellfieldactive %s %s\n\n", tmptype, tmpname);

   if (numvartr > 0)
     {
      cpywblanks(tmpname,fieldtrname[tracefieldactive]);
      fprintf(out, "tracefieldactive %s\n\n", tmpname);
     }

   /*  Write field isosurface information for active contours.  */
   k = 0;
   for (i = 0; i < MAXCONTOURS; i++)
     {
      if (num_of_contour_polys[i] > 0)
        {
         fprintf(out, "isosurf %d",i);
         j = mconfield[i];
         cpywblanks(tmpname,fieldname[j]);
         fprintf(out,"   %s %d %e\n",tmpname,contour_on[i],mconval[i]);
         k = 1;
        }
     }
   if (k) fprintf(out,"\n");

   /*  Write material isosurface information for active contours.  */
   k = 0;
   for (i = MAXCONTOURS; i < MAXCONTOURS+maxmats; i++)
     {
      if (num_of_contour_polys[i] > 0)
        {
         fprintf(out, "isosurf %d",i);
         j = i - MAXCONTOURS;
         cpywblanks(tmpname,mmatnames[j]);
         fprintf(out,"   %s %d %e\n",tmpname,contour_on[i],mconval[i]);
        }
     }
   if (k) fprintf(out,"\n");

   /*  Write tracer information, if tracers exist.  */
   if (ntracers > 0)
     {
      fprintf(out, "tracefield_limits %d\n", numvartr);
      for (i = 0; i < numvartr; i++)
        {
         cpywblanks(tmpname,fieldtrname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,globfieldtrmin[i],
                                               globfieldtrmax[i]);
        }

      fprintf(out, "tracefield_nice_limits %d\n",numvartr);
      for (i = 0; i < numvartr; i++)
        {
         cpywblanks(tmpname,fieldtrname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,trfld_nice_min[i],
                                               trfld_nice_max[i]);
        }
      fprintf(out, "tracefield_lim_flags %d ", numvartr);
      fprintf(out," \n");

      for (i = 0; i < numvartr; i++)
        {
         cpywblanks(tmpname,fieldtrname[i]);
         fprintf(out,"     %s %d %d %d\n",tmpname,(int)trfieldlimauto[i],
                                       (int)trfld_nice[i],(int)trfld_log[i]);
        }
      fprintf(out," \n");
      
      fprintf(out, "tracefield_select_limits %d\n", numvartr);
      for (i = 0; i < numvartr; i++)
        {
         cpywblanks(tmpname,fieldtrname[i]);
         fprintf(out,"     %s %.11g %.11g\n",tmpname,trfldselmin[i],
                                               trfldselmax[i]);
        }
      fprintf(out," \n");
     }

   /*  Extract the background color as r,g,b values and write.  */
   mr = (int) (backgroundcolor & 255);
   mg = (int) ((backgroundcolor >> 8) & 255);
   mb = (int) ((backgroundcolor >> 16) & 255);
   fprintf(out, "backgroundcolor %d %d %d\n\n", mr, mg, mb);

   /*  Write the light location.  */
   fprintf(out, "light  %e %e\n\n", lightx, lighty);

   /*  Write the x, y, z values for reflections.  */
   fprintf(out, "xscale ");
      put_floats(xscale, 8, out);
   fprintf(out, "yscale ");
      put_floats(yscale, 8, out);
   fprintf(out, "zscale ");
      put_floats(zscale, 8, out);

   /*  Write the x, y, z values for the old (original)  */
   /*  scale axis and the current scale axis.               */
   /* fprintf(out, "scaleaxiso %e %e %e\n\n",  */
   /*               xscaleaxiso, yscaleaxiso, zscaleaxiso); */
   fprintf(out, "scaleaxis %e %e %e\n\n", 
                 xscaleaxis, yscaleaxis, zscaleaxis);

   /*  Write the global mins and maxs for the node    */
   /*  variables, as well as the nice and log flags.  */
   fprintf(out, "nodefield_limits %d\n", numvars);
   for (i = 0; i < numvars; i++)
     {
      cpywblanks(tmpname,fieldname[i]);
      fprintf(out,"     %s %e %e\n",tmpname,globfieldmin[i],globfieldmax[i]);
      }

   fprintf(out, "nodefield_nice_limits %d\n",numvars);
   for (i = 0; i < numvars; i++)
     {
      cpywblanks(tmpname,fieldname[i]);
      fprintf(out,"     %s %e %e\n",tmpname,nfld_nice_min[i],nfld_nice_max[i]);
     }
   fprintf(out, "nodefield_lim_flags %d ", numvars);
   fprintf(out," \n");

   for (i = 0; i < numvars; i++)
     {
      cpywblanks(tmpname,fieldname[i]);
      fprintf(out,"     %s %d %d %d\n",tmpname,(int)nfieldlimauto[i],
                                    (int)nfld_nice[i],(int)nfld_log[i]);
     }
   fprintf(out," \n");

   /*  Write the global mins and maxs for the cell    */
   /*  variables, as well as the nice and log flags.  */
   fprintf(out, "cellfield_limits %d\n", cnumvars);
   for (i = 0; i < cnumvars; i++)
     {
      cpywblanks(tmpname,cfieldname[i]);
      fprintf(out,"     %s %e %e\n",tmpname,globfieldcmin[i],globfieldcmax[i]);
      }
   fprintf(out," \n");

   fprintf(out, "cellfield_nice_limits %d\n",cnumvars);
   for (i = 0; i < cnumvars; i++)
     {
      cpywblanks(tmpname,cfieldname[i]);
      fprintf(out,"     %s %e %e\n",tmpname,cfld_nice_min[i],cfld_nice_max[i]);
     }
   fprintf(out, "cellfield_lim_flags %d ", cnumvars);
   fprintf(out," \n");

   for (i = 0; i < cnumvars; i++)
     {
      cpywblanks(tmpname,cfieldname[i]);
      fprintf(out,"     %s %d %d %d\n",tmpname,(int)cfieldlimauto[i],
                                    (int)cfld_nice[i],(int)cfld_log[i]);
     }
   fprintf(out," \n");

   /*  Write the old (original) global x, y, z mins and maxs.  */
   fprintf(out, "plotbox_mins_prev %e %e %e\n\n", 
                 globxicmino, globyicmino, globzicmino);
   fprintf(out, "plotbox_maxs_prev %e %e %e\n\n", 
                 globxicmaxo, globyicmaxo, globzicmaxo);

   /*  Write the isosurface colors, as r, g, b floating point  */
   /*  componets for isosurfaces that will be drawn.           */
   for (i = 0; i < MAXCONTOURS; i++)
     {
      if (contour_on[i])
       {
        fprintf(out, "conrgba %d ", i);
        put_floats(&(conrgba[4*i]), 4, out);
        k = 1;
       }
     }

   /*  Write the isovolume information, if it exists.  */
   if (isovol_on)
     {
      cpywblanks(tmpname,fieldname[isovolfieldactive]);
      fprintf(out, "isovol 0 %e %e %s 2 %d %d\n\n",
              isovol_val1,isovol_val2,
              tmpname,ivcliponbox,ivcliponcellsel);
      i = MAXCONTOURS + 1;
      fprintf(out, "conrgba %d ", i);
      put_floats(&(conrgba[4*i]), 4, out);
     }

   /*  Write the field to color isovolumes with.  */
   if (isovol_on)
     {
      j = isovolfield_draw;
      if (j < 0) strcpy(tmpname,"NONE");
      else cpywblanks(tmpname,fieldname[j]);
      fprintf(out, "isovolfield_draw %s\n\n", tmpname);
     }

   /*  Write flags.  */
   fprintf(out, "cliponbox %d\n\n", cliponbox);
   fprintf(out, "nrefl_loops %d\n\n", nrefl_loops);
   fprintf(out, "reflect %d %d %d\n\n", xreflect, yreflect, zreflect);
   fprintf(out, "clipvalue %e %e\n\n", frontclipvalue, backclipvalue);

   /*  Write the polygon subset box mins and maxs.  */
   fprintf(out, "polysubset_mins %e %e %e\n\n", 
                 xminbdval, yminbdval, zminbdval);
   fprintf(out, "polysubset_maxs %e %e %e\n\n", 
                 xmaxbdval, ymaxbdval, zmaxbdval);

   /*  Write the node, cell, tracer subset box mins and maxs.  */
   fprintf(out, "subset_mins %e %e %e\n\n", 
                 xminfdval, yminfdval, zminfdval);
   fprintf(out, "subset_maxs %e %e %e\n\n", 
                 xmaxfdval, ymaxfdval, zmaxfdval);

   /*  Write the material preference order.  */
   fprintf(out, "mat_order %d ", nmat_order);
      put_ints(mat_order, nmat_order, out);

   /*  Write the node material and flag selection data.  */
   /*  First, see how many materials were selected.      */
   k = 0;
   for (j = 0; j < maxcellmat; j++)
     {
      if (nodeselflag[0][j] == 1) k++;
     }
   if (k == maxcellmat)
     {
      fprintf(out,"nodeselflag Material ALL\n");
     }
   else if (k > 0)
     {
      fprintf(out,"nodeselflag Material %d\n",k);
      for (j = 0; j < maxcellmat; j++)
        {
         if (nodeselflag[0][j] == 1)
           {
            cpywblanks(tmpname,mmatnames[j]);
            fprintf(out,"                  %s\n",tmpname);
           }
        }
     }
 
   /*  Write node flag selection data.  */
   for (i = 0; i < numflags_node; i++)
     {
      k = 0;
      for (j = 0; j < flagtypes_node[i]; j++)
        {
         if (nodeselflag[i+1][j] == 1) k++;
        }
      if (k == flagtypes_node[i])
        {
         cpywblanks(tmpname,flagname_node[i]);
         fprintf(out,"nodeselflag Flag %s ALL\n",tmpname);
        }
      else if (k > 0)
        {
         cpywblanks(tmpname,flagname_node[i]);
         fprintf(out,"nodeselflag Flag %s  %d\n",tmpname,k);
         for (j = 0; j < flagtypes_node[i]; j++)
           {
            if (nodeselflag[i+1][j] == 1)
              {   
               cpywblanks(tmpname,flagnames_node[i][j]);
               fprintf(out,"                  %s\n",tmpname);
              }
           }
        }
     }

   /*  Write node material and flag and/or flags.  */
   fprintf(out,"\n");
   fprintf(out, "nodeandorflag %d ", numflags_node+1);
      put_chars(nodeandorflag, numflags_node+1, out);

   /*  Write the cell material and flag selection data.  */
   /*  First, see how many materials were selected.      */
   k = 0;
   for (j = 0; j < maxcellmat; j++)
     {
      if(cellselflag[0][j] == 1) k++;
     }
   if (k == maxcellmat)
     {
      fprintf(out,"cellselflag Material ALL\n");
     }
   else if (k > 0)
     {
      fprintf(out,"cellselflag Material %d\n",k);
      for (j = 0; j < maxcellmat; j++)
        {
         if (cellselflag[0][j] == 1)
           {
            cpywblanks(tmpname,mmatnames[j]);
            fprintf(out,"                  %s\n",tmpname);
           }
        }
     }
 
   /*  Write cell flag selection data.  */
   for (i = 0; i < numflags_cell; i++)
     {
      k = 0;
      for (j = 0; j < flagtypes_cell[i]; j++)
        {
         if (cellselflag[i+1][j] == 1) k++;
        }
      if (k == flagtypes_cell[i])
        {
         cpywblanks(tmpname,flagname_cell[i]);
         fprintf(out,"cellselflag Flag %s ALL\n",tmpname);
        }
      else if (k > 0)
        {
         cpywblanks(tmpname,flagname_cell[i]);
         fprintf(out,"cellselflag Flag %s  %d\n",tmpname,k);
         for (j = 0; j < flagtypes_cell[i]; j++)
           {
            if (cellselflag[i+1][j] == 1)
              {
               cpywblanks(tmpname,flagnames_cell[i][j]);
               fprintf(out,"                  %s\n",tmpname);
              }
           }
        }
     }

   /*  Write cell material and flag and/or flags.  */
   fprintf(out,"\n");
   fprintf(out, "cellandorflag %d ", numflags_cell+1);
      put_chars(cellandorflag, numflags_cell+1, out);

   /*  Write colorbar and bounding box flags.  */
   fprintf(out, "colorbarflag %d\n\n", colorbarflag);
   fprintf(out, "colorbarpref %d\n\n", colorbarpref);
   fprintf(out, "boundingboxflag %d\n\n", boundingboxflag);

   /*  Write the bounding box mins and maxs.  */
   fprintf(out, "boundingbox_mins %e %e %e\n\n", 
                 xminbbval, yminbbval, zminbbval);
   fprintf(out, "boundingbox_maxs %e %e %e\n\n", 
                 xmaxbbval, ymaxbbval, zmaxbbval);
   fprintf(out, "boundingbox_mins_prev %e %e %e\n\n", 
                 xminbbvalo, yminbbvalo, zminbbvalo);
   fprintf(out, "boundingbox_maxs_prev %e %e %e\n\n", 
                 xmaxbbvalo, ymaxbbvalo, zmaxbbvalo);

   /*  Write the view and fly position flags.  */
   fprintf(out, "viewflag %d\n\n", viewflag);
   fprintf(out, "flypos ");
      put_floats(flypos, 3, out);

   /*  Write more flags and data.  */
   fprintf(out, "cellcontflag %d\n\n", cellcontflag);
   fprintf(out, "cliponcellsel %d\n\n", cliponcellsel);

   /*  Write node vector info.  */
   if (numvars > 0)
     {
      j  = vectfldx;
      if (j >= 0) cpywblanks(tmpname1,fieldname[j]);
      else strcpy(tmpname1,"NONE");
      j = vectfldy;
      if (j >= 0) cpywblanks(tmpname2,fieldname[j]);
      else strcpy(tmpname2,"NONE");
      j = vectfldz;
      if (j >= 0) cpywblanks(tmpname3,fieldname[j]);
      else strcpy(tmpname3,"NONE");
      j = speedfld;
      if (j >= 0) cpywblanks(tmpname,fieldname[j]);
      else strcpy(tmpname,"NONE");
      fprintf(out, "vectfld_ %s %s %s\n\n", 
                    tmpname1, tmpname2, tmpname3);
      /* fprintf(out, "speedfld %s\n\n", tmpname); */
      fprintf(out, "maxspeed %e\n\n", maxspeed);
      fprintf(out, "vectorflag %d\n\n", vectorflag);
      fprintf(out, "vectorarrow %d\n\n", vectorarrow);
      fprintf(out, "vectorlog %d\n\n", vectorlog);
     }

   /*  Write cell vector info.  */
   if (cnumvars > 0)
     {
      j = cvectfldx;
      if (j >= 0) cpywblanks(tmpname1,cfieldname[j]);
      else strcpy(tmpname1,"NONE");
      j = cvectfldy;
      if (j >= 0) cpywblanks(tmpname2,cfieldname[j]);
      else strcpy(tmpname2,"NONE");
      j = cvectfldz;
      if (j >= 0) cpywblanks(tmpname3,cfieldname[j]);
      else strcpy(tmpname3,"NONE");
      j = cspeedfld;
      if (j >= 0) cpywblanks(tmpname,cfieldname[j]);
      else strcpy(tmpname,"NONE");
      fprintf(out, "cvectfld_ %s %s %s\n\n", 
                    tmpname1, tmpname2, tmpname3);
      /* fprintf(out, "cspeedfld %s\n\n", tmpname); */
      fprintf(out, "cmaxspeed %e\n\n", cmaxspeed);
      fprintf(out, "cvectorflag %d\n\n", cvectorflag);
     }

   /*  Write title flag and data, if used.  */
   fprintf(out, "titleflag %d\n\n", titleflag);
   if (titleflag)
     {
      fprintf(out, "titlestr ");
      fprintf(out, "%s\n\n", titlestr);
      fprintf(out, "titlestr2 ");
      fprintf(out, "%s\n\n", titlestr2);
     }

   /*  Write cutline data for active cutlines.  */
   for (i = 0; i < MAXCUTLINES; i++)
     {
      if (ncutlinepts[i] > 0)
        { 
         j = cutlinefield[i];
         if (j < numvars)
           {
            strcpy(tmptype,"Node");
            cpywblanks(tmpname,fieldname[j]);
           }
         else
           {
            strcpy(tmptype,"Cell");
            cpywblanks(tmpname,cfieldname[j-numvars]);
           }
         fprintf(out, "cutline %d %d ",i,cutlineon[i]);
         fprintf(out, "%s %s %e\n", tmptype, tmpname, cutlineradius[i]);
         fprintf(out, "%e %e %e\n", ctlinx1[i], ctliny1[i], ctlinz1[i]);
         fprintf(out, "%e %e %e\n", ctlinx2[i], ctliny2[i], ctlinz2[i]);
         fprintf(out, "%d ", 4);  /* there are 4 additional options */
         fprintf(out, "%d %d %d %e\n\n", cutlinenodeson[i], cutlinenumson[i],
	         waveflag[i], wavefac[i]);
        }
     }

   /*  Write node selection flags and data.  */
   fprintf(out, "nodeselmat_on %d\n\n", nodeselmat_on);
   if (nodeselfld_on)
     {
      cpywblanks(tmpname,fieldname[nodeselfield]);
      fprintf(out, "nodeselfld_on %d\n", nodeselfld_on);
      fprintf(out, "  nodeselfield %s\n", tmpname);
      fprintf(out, "  nfldselmin %e\n", nfldselmin);
      fprintf(out, "  nfldselmax %e\n\n", nfldselmax);
     }
   if (nodeselsph_on)
     {
      fprintf(out, "nodeselsph_on %d\n", nodeselsph_on);
      fprintf(out, "  nodeselsph_x %e\n", nodeselsph_x);
      fprintf(out, "  nodeselsph_y %e\n", nodeselsph_y);
      fprintf(out, "  nodeselsph_z %e\n", nodeselsph_z);
      fprintf(out, "  nodeselsph_r %e\n", nodeselsph_r);
      fprintf(out, "nodeselsph_in %d\n\n", nodeselsph_in);
     }
   if (nodeselbox_on)
     {
      fprintf(out, "nodeselbox_on %d\n", nodeselbox_on);
      fprintf(out, "  nodeselbox_xmin %e\n", nodeselbox_xmin);
      fprintf(out, "  nodeselbox_xmax %e\n", nodeselbox_xmax);
      fprintf(out, "  nodeselbox_ymin %e\n", nodeselbox_ymin);
      fprintf(out, "  nodeselbox_ymax %e\n", nodeselbox_ymax);
      fprintf(out, "  nodeselbox_zmin %e\n", nodeselbox_zmin);
      fprintf(out, "  nodeselbox_zmax %e\n", nodeselbox_zmax);
      fprintf(out, "nodeselbox_in %d\n\n", nodeselbox_in);
     }
   if (nodeselnum_on)
     {
      fprintf(out, "nodeselnum_on %d\n", nodeselnum_on);
      fprintf(out, "  nnodeselnum %d\n", nnodeselnum);
      for (i = 0; i < nnodeselnum; i++)
        {
         fprintf(out, "    %ld %ld %ld\n", 
                 nodeselnum_f[i], nodeselnum_l[i], nodeselnum_s[i]);
        }
      fprintf(out," \n");
     }

   /*  Write cell selection flags and data.  */
   fprintf(out, "cellselmat_on %d\n\n", cellselmat_on);
   if (cellselcfld_on)
     {
      cpywblanks(tmpname,cfieldname[cellselfield]);
      fprintf(out, "cellselcfld_on %d\n", cellselcfld_on);
      fprintf(out, "  cellselfield %s\n", tmpname); 
      fprintf(out, "  cfldselmin %e\n", cfldselmin);
      fprintf(out, "  cfldselmax %e\n\n", cfldselmax);
     }
   if (cellselnfld_on)
     {
      cpywblanks(tmpname,fieldname[cellselnfield]);
      fprintf(out, "cellselnfld_on %d\n", cellselnfld_on);
      fprintf(out, "  cellselnfield %s\n", tmpname);
      fprintf(out, "  cellselnfldopt %d\n", cellselnfldopt);
      fprintf(out, "  cnfldselmin %e\n", cnfldselmin);
      fprintf(out, "  cnfldselmax %e\n", cnfldselmax);
     }
   if (cellselsph_on)
     {
      fprintf(out, "cellselsph_on %d\n", cellselsph_on);
      fprintf(out, "  cellselsph_x %e\n", cellselsph_x);
      fprintf(out, "  cellselsph_y %e\n", cellselsph_y);
      fprintf(out, "  cellselsph_z %e\n", cellselsph_z);
      fprintf(out, "  cellselsph_r %e\n", cellselsph_r);
      fprintf(out, "cellselsph_in %d\n\n", cellselsph_in);
     }
   if (cellselbox_on)
     {
      fprintf(out, "cellselbox_on %d\n", cellselbox_on);
      fprintf(out, "  cellselbox_xmin %e\n", cellselbox_xmin);
      fprintf(out, "  cellselbox_xmax %e\n", cellselbox_xmax);
      fprintf(out, "  cellselbox_ymin %e\n", cellselbox_ymin);
      fprintf(out, "  cellselbox_ymax %e\n", cellselbox_ymax);
      fprintf(out, "  cellselbox_zmin %e\n", cellselbox_zmin);
      fprintf(out, "  cellselbox_zmax %e\n", cellselbox_zmax);
      fprintf(out, "cellselbox_in %d\n\n", cellselbox_in);
     }
  if (cellselnum_on)
     {
      fprintf(out, "cellselnum_on %d\n", cellselnum_on);
      fprintf(out, "  ncellselnum %d\n", ncellselnum);
      for (i = 0; i < ncellselnum; i++)
        {
         fprintf(out, "    %ld %ld %ld\n", 
                 cellselnum_f[i], cellselnum_l[i], cellselnum_s[i]);
        } 
      fprintf(out," \n");
     }

   /*  Write tracer selection flags and data.  */
   if (traceselfld_on)
     {
      fprintf(out, "traceselfld_on %d\n", traceselfld_on);
      for(j = 0; j < numvartr; j++)
      {
          if(traceselfields[j] == 1)
          {
            cpywblanks(tmpname,fieldtrname[j]);
            fprintf(out, "  traceselfield %s\n", tmpname);
          }
      }
     }
   if (traceselsph_on)
     {
      fprintf(out, "traceselsph_on %d\n", traceselsph_on);
      fprintf(out, "  traceselsph_x %e\n", traceselsph_x);
      fprintf(out, "  traceselsph_y %e\n", traceselsph_y);
      fprintf(out, "  traceselsph_z %e\n", traceselsph_z);
      fprintf(out, "  traceselsph_r %e\n", traceselsph_r);
      fprintf(out, "traceselsph_in %d\n\n", traceselsph_in);
     }
   if (traceselbox_on)
     {
      fprintf(out, "traceselbox_on %d\n", traceselbox_on);
      fprintf(out, "  traceselbox_xmin %e\n", traceselbox_xmin);
      fprintf(out, "  traceselbox_xmax %e\n", traceselbox_xmax);
      fprintf(out, "  traceselbox_ymin %e\n", traceselbox_ymin);
      fprintf(out, "  traceselbox_ymax %e\n", traceselbox_ymax);
      fprintf(out, "  traceselbox_zmin %e\n", traceselbox_zmin);
      fprintf(out, "  traceselbox_zmax %e\n", traceselbox_zmax);
      fprintf(out, "traceselbox_in %d\n\n", traceselbox_in);
     }
   if (traceselnum_on)
     {
      fprintf(out, "traceselnum_on %d\n", traceselnum_on);
      fprintf(out, "  ntraceselnum %d\n", ntraceselnum);
      for (i = 0; i < ntraceselnum; i++)
        {
         fprintf(out, "    %d %d %d\n",
                 traceselnum_f[i], traceselnum_l[i], traceselnum_s[i]);
        }
      fprintf(out," \n");
     }

   /*  Write bounding box coordinate flag.  */
   fprintf(out, "bboxcoordflag %d\n\n", bboxcoordflag);

   /*  Write cellnum and nodenum flags.  */
   fprintf(out, "nodenumflag %d\n\n", nodenumflag);
   fprintf(out, "cellnumflag %d\n\n", cellnumflag);

   /*  Write point size, line size, and more flags and data.  */
   fprintf(out, "pnt_size %d %d\n\n", pnt_size, pnt_shape);
   fprintf(out, "line_size %d %d\n\n", line_size, line_type);
   fprintf(out, "node0matflag %d\n\n", node0matflag);
   fprintf(out, "cell0matflag %d\n\n", cell0matflag);
   fprintf(out, "tracehistflag %d\n\n", tracehistflag);
   fprintf(out, "tracehiststop %d\n\n", tracehiststop);
   fprintf(out, "tracehiststride %d\n\n", tracehiststride);
   fprintf(out, "tracehistpoints %d\n\n", tracehistpoints);
   fprintf(out, "cellshadeflag %d\n\n", cellshadeflag);

   /*  Write the field to color field isosurfaces with.  */
   chkcont = 0;
   for (i = 0; i < MAXCONTOURS; i++)
     {
      if (contour_on[i]) chkcont = 1;
     }
   if (chkcont)
     {
      j = contourfield_draw;
      if (j < 0) strcpy(tmpname,"NONE");
      else cpywblanks(tmpname,fieldname[j]);
      fprintf(out, "contourfield_draw %s\n\n", tmpname);
     }

   /*  Write the field to color material isosurfaces with.  */
   chkcont = 0;
   for (i = MAXCONTOURS; i < MAXCONTOURS+maxmats; i++)
     {
      if (contour_on[i]) chkcont = 1;
     }
   if (chkcont) 
     {
      j = mcontourfield_draw;
      if (j < 0) strcpy(tmpname,"NONE");
      else cpywblanks(tmpname,fieldname[j]);
      fprintf(out, "mcontourfield_draw %s\n\n", tmpname);
     }

   /*  Write the field colormap to use, and special map rgb, if any.  */
   fprintf(out, "fieldcolortype %d\n\n", fieldcolortype);
   if (fieldcolortype == -1)
     {
      fprintf(out, "jcolor\n");
      for (i = 0; i < 256; i++)
        {
         mr = jcolor[i]>>24 & 255;
         mg = jcolor[i]>>16 & 255;
         mb = jcolor[i]>>8 & 255;
         fprintf(out, "   %d %d %d\n", mr, mg, mb);
        }
      fprintf(out,"\n");
     }

   /*  Write more flags.  */
   fprintf(out, "nset_contours %d\n\n", nset_contours);
   fprintf(out, "facerefineflag %d\n\n", facerefineflag);
   fprintf(out, "cellnormflag %d\n\n", cellnormflag);
   fprintf(out, "cellmedianedgeflag %d\n\n", cellmedianedgeflag);
   fprintf(out, "cellnodenumflag %d\n\n", cellnodenumflag);
   fprintf(out, "cellfacenumflag %d\n\n", cellfacenumflag);
   fprintf(out, "polystiplflag %d\n\n", polystiplflag);

   /*  Write surface specific attributes, if surface exists.  */
   if (nsurf > 0)
     {
      fprintf(out, "surfflag %d\n\n", surfflag);
      fprintf(out, "surfedgesflag %d\n\n", surfedgesflag);
      fprintf(out, "surfexpflag %d\n\n", surfexpflag);

      j = surffieldactive;
      strcpy(tmpname," ");
      if (j == -1) strcpy(tmptype,"MATERIAL");
      else if (j == -2) strcpy(tmptype,"Surf_GROUP");
      else if (j < numvars)
        {
         strcpy(tmptype,"Node");
         cpywblanks(tmpname,fieldname[j]);
        }
      else if (j < numvars + snumvars)
        {
         i = j - numvars;
         strcpy(tmptype,"Surf");
         cpywblanks(tmpname,sfieldname[i]);
        }
      else 
        {
         i = j - (numvars + snumvars);
         strcpy(tmptype,"Flags");
         cpywblanks(tmpname,flagname_surf[i]);
        }
      fprintf(out, "surffieldactive %s %s\n\n", tmptype, tmpname);

      fprintf(out, "surfnumflag %d\n\n", surfnumflag);
      fprintf(out, "surfnodenumflag %d\n\n", surfnodenumflag);
      fprintf(out, "surfrefineflag %d\n\n", surfrefineflag);
      fprintf(out, "surfcontflag %d\n\n", surfcontflag);
      fprintf(out, "surfshadeflag %d\n\n", surfshadeflag);
      fprintf(out, "surf0matflag %d\n\n", surf0matflag);

      /*  Write surface vector info.  */
      if (snumvars > 0)
        {
         j = svectfldx;
         if (j >= 0) cpywblanks(tmpname1,sfieldname[j]);
         else strcpy(tmpname1,"NONE");
         j = svectfldy;
         if (j >= 0) cpywblanks(tmpname2,sfieldname[j]);
         else strcpy(tmpname2,"NONE");
         j = svectfldz;
         if (j >= 0) cpywblanks(tmpname3,sfieldname[j]);
         else strcpy(tmpname3,"NONE");
         j = sspeedfld;
         if (j >= 0) cpywblanks(tmpname,sfieldname[j]);
         else strcpy(tmpname,"NONE");
         fprintf(out, "svectfld_ %s %s %s\n\n",
                       tmpname1, tmpname2, tmpname3);
         /* fprintf(out, "sspeedfld %s\n\n", tmpname); */
         fprintf(out, "smaxspeed %e\n\n", smaxspeed);
         fprintf(out, "svectorflag %d\n\n", svectorflag);
        }

      /*  Write suface field limits information.  */
      fprintf(out, "surffield_limits %d\n", snumvars);
      for (i = 0; i < snumvars; i++)
        {
         cpywblanks(tmpname,sfieldname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,globfieldsmin[i],
                                               globfieldsmax[i]);
         }
      fprintf(out," \n");

      fprintf(out, "surffield_nice_limits %d\n",snumvars);
      for (i = 0; i < snumvars; i++)
        {
         cpywblanks(tmpname,sfieldname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,
                                       sfld_nice_min[i],sfld_nice_max[i]);
        }
      fprintf(out, "surffield_lim_flags %d ", snumvars);
      fprintf(out," \n");

      for (i = 0; i < snumvars; i++)
        {
         cpywblanks(tmpname,sfieldname[i]);
         fprintf(out,"     %s %d %d %d\n",tmpname,(int)sfieldlimauto[i],
                                       (int)sfld_nice[i],(int)sfld_log[i]);
        }
      fprintf(out," \n");

      /*  Write surface material and flag selection data.  */
      /*  First, see how many materials were selected.     */
      k = 0;
      for (j = 0; j < maxsurfmat; j++)
        {
         if (surfselflag[0][j] == 1) k++;
        }
      if (k == maxsurfmat)
        {
         fprintf(out,"surfselflag Material ALL\n");
        }
      else if (k > 0)
        {
         fprintf(out,"surfselflag Material %d\n",k);
         for (j = 0; j < maxsurfmat; j++)
           {
            if (surfselflag[0][j] == 1)
              {
               cpywblanks(tmpname,mmatnames[j]);
               fprintf(out,"                  %s",tmpname);
              }
           }
        }
 
      /*  Write surface flag selection data.  */
      for (i = 0; i < numflags_surf; i++)
        {
         k = 0;
         for (j = 0; j < flagtypes_surf[i]; j++)
           {
            if (surfselflag[i+1][j] == 1) k++;
           }
         if (k == flagtypes_surf[i])
           {
            fprintf(out,"surfselflag Flag %s ALL\n",flagname_surf[i]);
           }
         else if (k > 0)
           {
            fprintf(out,"surfselflag Flag %s  %d\n",flagname_surf[i],k);
            for (j = 0; j < flagtypes_surf[i]; j++)
              {
               if (surfselflag[i+1][j] == 1)
                 {
                  fprintf(out,"                  %s\n",flagnames_surf[i][j]);
                 }
              }
           }
        }

      /*  Write the surface material and flag and/or flags.  */
      fprintf(out, "\n\n");
      fprintf(out, "surfandorflag %d ", numflags_surf+1);
      put_chars(surfandorflag, numflags_surf+1, out);

      /*  Write surface selection data.  */
      fprintf(out, "surfselmat_on %d\n\n", surfselmat_on);
      if (surfselsfld_on)
        {
         cpywblanks(tmpname,sfieldname[surfselfield]);
         fprintf(out, "surfselsfld_on %d\n", surfselsfld_on);
         fprintf(out, "  surfselfield %s\n", tmpname);
         fprintf(out, "  sfldselmin %e\n", sfldselmin);
         fprintf(out, "  sfldselmax %e\n\n", sfldselmax);
        }
      if (surfselnfld_on)
        {
         cpywblanks(tmpname,fieldname[surfselnfield]);
         fprintf(out, "surfselnfld_on %d\n", surfselnfld_on);
         fprintf(out, "  surfselnfield %s\n", tmpname);
         fprintf(out, "  surfselnfldopt %d\n", surfselnfldopt);
         fprintf(out, "  snfldselmin %e\n", snfldselmin);
         fprintf(out, "  snfldselmax %e\n", snfldselmax);
        }
      if (surfselsph_on)
        {
         fprintf(out, "surfselsph_on %d\n", surfselsph_on);
         fprintf(out, "  surfselsph_x %e\n", surfselsph_x);
         fprintf(out, "  surfselsph_y %e\n", surfselsph_y);
         fprintf(out, "  surfselsph_z %e\n", surfselsph_z);
         fprintf(out, "  surfselsph_r %e\n", surfselsph_r);
         fprintf(out, "surfselsph_in %d\n\n", surfselsph_in);
        }
      if (surfselbox_on)
        {
         fprintf(out, "surfselbox_on %d\n", surfselbox_on);
         fprintf(out, "  surfselbox_xmin %e\n", surfselbox_xmin);
         fprintf(out, "  surfselbox_xmax %e\n", surfselbox_xmax);
         fprintf(out, "  surfselbox_ymin %e\n", surfselbox_ymin);
         fprintf(out, "  surfselbox_ymax %e\n", surfselbox_ymax);
         fprintf(out, "  surfselbox_zmin %e\n", surfselbox_zmin);
         fprintf(out, "  surfselbox_zmax %e\n", surfselbox_zmax);
         fprintf(out, "surfselbox_in %d\n\n", surfselbox_in);
        }
     if (surfselnum_on)
        {
         fprintf(out, "surfselnum_on %d\n", surfselnum_on);
         fprintf(out, "  nsurfselnum %d\n", nsurfselnum);
         for (i = 0; i < nsurfselnum; i++)
           {
            fprintf(out, "    %ld %ld %ld\n", 
                    surfselnum_f[i], surfselnum_l[i], surfselnum_s[i]);
           } 
         fprintf(out," \n");
        }
     }

   /*  Write face variable info., if they exist.  */
   if (fnumvars > 0)
     {
      fprintf(out, "flagfield_limits %d\n", fnumvars);
      for (i = 0; i < fnumvars; i++)
        {
         cpywblanks(tmpname,ffieldname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,globfieldfmin[i],
                                               globfieldfmax[i]);
         }
      fprintf(out," \n");

      fprintf(out, "flagfield_nice_limits %d\n",fnumvars);
      for (i = 0; i < fnumvars; i++)
        {
         cpywblanks(tmpname,ffieldname[i]);
         fprintf(out,"     %s %e %e\n",tmpname,ffld_nice_min[i],
                                               ffld_nice_max[i]);
        }
      fprintf(out, "flagfield_lim_flags %d ", fnumvars);
      fprintf(out," \n");

      for (i = 0; i < fnumvars; i++)
        {
         cpywblanks(tmpname,ffieldname[i]);
         fprintf(out,"     %s %d %d %d\n",tmpname,(int)ffieldlimauto[i],
                                       (int)ffld_nice[i],(int)ffld_log[i]);
        }
      fprintf(out," \n");

      /*  Write face vector info.  */
      if (fnumvars > 0)
        {
         j = fvectfldx;
         if (j >= 0) cpywblanks(tmpname1,ffieldname[j]);
         else strcpy(tmpname1,"NONE");
         j = fvectfldy;
         if (j >= 0) cpywblanks(tmpname2,ffieldname[j]);
         else strcpy(tmpname2,"NONE");
         j = fvectfldz;
         if (j >= 0) cpywblanks(tmpname3,ffieldname[j]);
         else strcpy(tmpname3,"NONE");
         j = fspeedfld;
         if (j >= 0) cpywblanks(tmpname,ffieldname[j]);
         else strcpy(tmpname,"NONE");
         fprintf(out, "fvectfld_ %s %s %s\n\n",
                      tmpname1, tmpname2, tmpname3);
         /* fprintf(out, "fspeedfld %s\n\n", tmpname); */
         fprintf(out, "fmaxspeed %e\n\n", fmaxspeed);
         fprintf(out, "fvectorflag %d\n\n", fvectorflag);
        }
     }

   /*  Write node group selection data, if any.  */
   if (nodeselnodegrp_on && nnodegrpselected > 0)
     {
      fprintf(out, "nodeselnodegrp_on %d\n", nodeselnodegrp_on);
      fprintf(out, "nodeselgrp_in %d\n", nodeselgrp_in);
      fprintf(out, "  nnodegrpselected %d ", nnodegrpselected);
      for (i = 0; i < nnodegrpselected; i++)
        {
         j = nodegrpselected[i] - 1;
         cpywblanks(tmpname,nodegrpname[j]);
         fprintf(out, "%s ", tmpname);
        } 
      fprintf(out," \n\n");
     }

   /*  Write cell group selection data, if any.  */
   if (cellselcellgrp_on && ncellgrpselected > 0)
     {
      fprintf(out, "cellselcellgrp_on %d\n", cellselcellgrp_on);
      fprintf(out, "cellselgrp_in %d\n", cellselgrp_in);
      fprintf(out, "  ncellgrpselected %d ", ncellgrpselected);
      for (i = 0; i < ncellgrpselected; i++)
        {
         j = cellgrpselected[i] - 1;
         cpywblanks(tmpname,cellgrpname[j]);
         fprintf(out, "%s ", tmpname);
        } 
      fprintf(out," \n\n");
     }

   /*  Write cell face group selection data, if any.  */
   if (cellselfacegrp_on && nfacegrpselected > 0)
     {
      fprintf(out, "cellselfacegrp_on %d\n", cellselfacegrp_on);
      fprintf(out, "faceselgrp_in %d\n", faceselgrp_in);
      fprintf(out, "  nfacegrpselected %d ", nfacegrpselected);
      for (i = 0; i < nfacegrpselected; i++)
        {
         j = facegrpselected[i] - 1;
         cpywblanks(tmpname,facegrpname[j]);
         fprintf(out, "%s ", tmpname);
        } 
      fprintf(out," \n\n");
     }


   /*  Write surface group selection data, if any.  */
   if (surfselsurfgrp_on && nsurfgrpselected > 0)
     {
      fprintf(out, "surfselsurfgrp_on %d\n", surfselsurfgrp_on);
      fprintf(out, "surfselgrp_in %d\n", surfselgrp_in);
      fprintf(out, "  nsurfgrpselected %d ", nsurfgrpselected);
      for (i = 0; i < nsurfgrpselected; i++)
        {
         j = surfgrpselected[i] - 1;
         cpywblanks(tmpname,surfacegrpname[j]);
         fprintf(out, "%s ", tmpname);
        } 
      fprintf(out," \n\n");
     }

   /*  Write draw ghost options, if ghosts exist.  */
   if (cellghostflag || nodeghostflag)
     {
      fprintf(out, "drawghosts %d\n", drawghosts);
      fprintf(out, "drawghostface %d\n", drawghostface);
      fprintf(out, "drawnodeghosts %d\n", drawnodeghosts);
     }

   /*  Write draw unselected options.  */
   fprintf(out, "drawunselnodes %d\n\n", drawunselnodes);
   fprintf(out, "drawunselcells %d\n\n", drawunselcells);

   /*  Write center on cell, node or tracer option.  */
   if (cencellid > -1)
      fprintf(out, "cencellid %d\n\n", cencellid);
   if (cennodeid > -1)
      fprintf(out, "cennodeid %d\n\n", cennodeid);
   if (centracerid > -1)
      fprintf(out, "centracerid %d\n\n", centracerid);

   /*  Write colorbar label flag and string, if any.  */
   if (colorbartitleflag > 0)
     {
      fprintf(out, "colorbartitleflag %d\n", colorbartitleflag);
      fprintf(out, "clrbartitlestr %s\n\n", clrbartitlestr);
     }

   /*  Write distance scale start points.  */
   fprintf(out, "distscale_x %d\n", distscale_x);
   fprintf(out, "distscale_y %d\n", distscale_y);

   /*  Write orientation axis flag and position.  */
   fprintf(out, "orient_axisflag %d\n", o_axisflag);
   fprintf(out, "orient_axis_x %d\n", o_axis_x);
   fprintf(out, "orient_axis_y %d\n", o_axis_y);

   fprintf(out, "end_attributes\n\n");

   fclose(out);
}


void put_doubles(double *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%e ", data[i]);
     }
   fprintf(fp, "\n\n");
}


void put_floats(float *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%e ", data[i]);
     }
   fprintf(fp, "\n\n");
}


void put_ints(int *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%d ", data[i]);
     }
   fprintf(fp, "\n\n");
}


void put_uints(unsigned long *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%lu ", data[i]);
     }
   fprintf(fp, "\n\n");
}


void put_chars(char *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%d ", data[i]);
     }
    fprintf(fp, "\n\n");
}


void cpywblanks(char tmpname[50], char * inname)
{
  /*  Copy the inname string to tmpname, if there are blanks  */
  /*  in inname, add ^ to the beginning and end of tmpname.   */
  int len, i, hasblanks;

   len = strlen(inname);
   hasblanks = 0;
   for (i = 0; i < len; i++)
     {
      if (inname[i] == ' ') hasblanks = 1;
     }
   if (hasblanks)
     {
      tmpname[0] = '^';
      for (i = 0; i < len; i++)
         tmpname[i+1] = inname[i];
      tmpname[len+1] = '^';
      tmpname[len+2] = '\0';
     }
   else strcpy(tmpname,inname);  
}


void put_struct_fldcalc(struct fldcalc *data, int number, int itype, FILE *fp)
{

  /*  Write field calc data.                                       */
  /*  Input:                                                       */
  /*     data - Pointer to selected field calc. structure.         */
  /*     number - the maximum number of structures available.      */
  /*     itype - the field type, 0 = cell, 1 = node, 2 = surface   */
  /*             3 = tracer.                                       */
  /*     fp - the file to write to.                                */
  int i, j, k;
  char **fldname, tmpname1[33], tmpname2[33], kname[12];

   /*  Set field names.  */
   if (itype == 0)
     {
      fldname = cfieldname;
      strcpy(kname,"cfieldcalc");
     }
   else if (itype == 1)
     {
      fldname = fieldname;
      strcpy(kname,"fieldcalc");
     }
   else if (itype == 2)
     {
      fldname = sfieldname;
      strcpy(kname,"sfieldcalc");
     }
   else if (itype == 3)
     {
      fldname = fieldtrname;
      strcpy(kname,"tfieldcalc");
     }
   else return;

   /*  Loop through the calc fields and output data, if needed.  */
   k = 0;
   for (i = 0; i < number; i++)
     {
      if (data[i].xfldno > -1 || data[i].yfldno > -1)
        {
         if (data[i].xfldno > -1)
           {
            j = data[i].xfldno;
            cpywblanks(tmpname1,fldname[j]);
           }
         else (strcpy(tmpname1,"NONE"));

         if (data[i].yfldno > -1)
           {
            j = data[i].yfldno;
            cpywblanks(tmpname2,fldname[j]);
           }
         else (strcpy(tmpname2,"NONE"));

         fprintf(fp,"%s %d %s %s %d %e %s\n", kname, i, tmpname1, tmpname2,
	         data[i].oper, data[i].cval, data[i].name);
        }
     }
   if (k) fprintf(fp, "\n");
}


void put_shorts(short *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
     {
      fprintf(fp, "%d ", data[i]);
     }
   fprintf(fp, "\n\n");
}


/*---------------------------------------------------------------------*/
/*		THE GET ATTRIBUTES SECTION			       */
/*---------------------------------------------------------------------*/

int check_attrib_version(void)
{
  /*  Returns version number of binary attributes file OR   */
  /*  zero if an ascii file OR -1 if something went wrong.  */

  FILE *fp;
  int readme;
  char magic[20];

   fp = fopen(attribsfilnam, "r");
   if (fp == NULL)
     {
      fprintf(stderr, "Error...cannot open file %s!\n", attribsfilnam);
      return(-1);
     }
   fread(magic,sizeof(char), 15, fp);
   if (strncmp(magic,"gmv_attributes2",15) == 0) 
     {
      fclose(fp);
      return(1);
     }
   if (strncmp(magic,"gmv_attributes",14) == 0) 
     {
      fclose(fp);
      return(0);
     }

   rewind(fp);
   fread(&readme, sizeof(int), 1, fp);
   fclose(fp);
   if (readme >= 2 && readme <= 11)
      return(readme);
   else
      return(-1);
}



void getattributes(void)
{
  int chkattr;
  XmString string;

   /*  Check for valid attributes file.  */
   chkattr = check_attrib_version();

   /*  If valid attributes file, set defaults,  */
   /*  read attributes, and set data.           */
   if (chkattr >= 0 && chkattr < 12)
     {
      getattr_set_defaults();
      if (chkattr == 0)
         getattributes_new();
      else if (chkattr == 1)
         getattributes_new2();
      else
         getattributes_old();
      getattr_set_data();
      getattr_set_widgets();
     }

   /*  Handle error for invalid attributes file.  */
   else
     {
      if (!movieflag)
        {
         errorfile = 1;
         string = XmStringCreate("Error - invalid GMV attributes file!",
                		  XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
         XtManageChild(errorform);
         XmStringFree(string);
         return;
        }
      else
        {
         fprintf(stderr, "Error - invalid GMV attributes file!\n");
         gmvexit();
        }
    }
}



//TJ added to read only the windowwidth and windowheight from attr file.
//   Had to add this due to GMV Batch bugs that crashed due to changes to the
//   width/height once initialization was done.
void getattributes_windowwidth_windowheight(void)
{
  /*  Second version of getattributes.                                   */
  /*  This version reads the gmv_attributes2 ASCII files, but only scans */
  /*  the file for the windowwidth and windowheight keywords.            */

  char keyword[2048];
  FILE *fp;
  //TJ added 2012-08-23 to support window resizing through attributes file
  long windowwidth_in = -1, windowheight_in = -1;

  fp = fopen(attribsfilnam, "r");
  if (fp == NULL)
  {
      fprintf(stderr, "Error...cannot open attributes file!\n");
      gmvexit();
  }

  fscanf(fp, "%s", keyword);
  if (strcmp("gmv_attributes2", keyword))
  {
      fprintf(stderr, "Error...invalid GMV attributes file!\n");
      gmvexit();
  }

  while (9)
  {
      /*  Read keyword, break at end of file.  */
      if (fscanf(fp, "%s", keyword) == EOF ||
          !strcmp(keyword, "end_attributes"))
         break;

      /*  Compare keyword, and read data.  */

      //TJ added 2012-08-23 to support window resizing through attributes file
      if (!strcmp(keyword, "windowwidth"))
      {
        fscanf(fp, "%ld", &windowwidth_in);
      }
      else if (!strcmp(keyword, "windowheight"))
      {
        fscanf(fp, "%ld", &windowheight_in);
      }
      else
      {
         //TJ ignore anything else while we grab width and height
      }
  }
  fclose(fp);

  /*  Check if the window sizes need updating.  */
  if (windowwidth_in > 0 || windowheight_in > 0)
  {
     readattributes_found_width_height = 1;
     if (windowwidth_in == -1)  windowwidth_in = windowwidth;
     if (windowheight_in == -1)  windowheight_in = windowheight;
     windowwidth = windowwidth_in;
     windowheight = windowheight_in;
  }
}



void getattributes_new2(void)
{

  /*  Second version of getattributes.                                  */
  /*  This version reads the gmv_attributes2 ASCII files.  These files  */
  /*  write out names (material, flags, field names, etc.) instead of   */
  /*  the number that represents the input order of the named data.     */

  char keyword[2048], old_key[2048], tempchar;
  char tmpname[50], tmpname1[50], tmpname2[50], tmpname3[50];
  char tmpchar[50], tmpchar1[50], tmpchar2[50], tmpchar3[50], tmptype[30];
  char junk[2048];
  FILE *fp;
  int temp, loop, number, number2, nopt, tmp1, tmp2, tmp3;
  int i, j, k, mr, mg, mb, nodeselfirst, cellselfirst, surfselfirst;
  float tempf, tempf2;
  XmString string;
  void stripname(FILE * fp,char namein[50], char nameout[50]);
  int get_name_id(int n, char **names, char *name);
  //TJ added 2012-08-23 to support window resizing through attributes file
  long windowwidth_in = -1, windowheight_in = -1;

   fp = fopen(attribsfilnam, "r");
   if (fp == NULL)
     {
      if (!movieflag)
        {
         string = XmStringCreate("Error...cannot open attribute file!\n",
	                         XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
         XtManageChild(errorform);
         XmStringFree(string);
         return;
        }
      else
        {
         fprintf(stderr, "Error...cannot open attributes file!\n");
         gmvexit();
        }
     }

   cencellid = -1;  cennodeid = -1;  centracerid = -1;
   nodeselfirst = 1;  cellselfirst = 1;  surfselfirst = 1;

   fscanf(fp, "%s", keyword);
   if (strcmp("gmv_attributes2", keyword))
     {
      if (!movieflag)
        {
         string = XmStringCreate("Error...invalid GMV attributes file!",
	                         XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
         XtManageChild(errorform);
         XmStringFree(string);
         return;
        }
      else
        {
         fprintf(stderr, "Error...invalid GMV attributes file!\n");
         gmvexit();
        }
     }

   while (9)
     {
      /*  Read keyword, break at end of file.  */
      strcpy(old_key, keyword); /* this is for debugging */
      if (fscanf(fp, "%s", keyword) == EOF || 
          !strcmp(keyword, "end_attributes"))
         break;

      /*  Compare keyword, and read data.  */

      /*  Read material color values.  */
      if (!strcmp(keyword, "matrgba"))
        {
         fscanf(fp, "%d", &number);
         scan_floats(matrgba, number, fp);
        }

      //TJ added 2012-08-23 to support window resizing through attributes file
      else if (!strcmp(keyword, "windowwidth"))
      {
        fscanf(fp, "%ld", &windowwidth_in);
      }
      else if (!strcmp(keyword, "windowheight"))
      {
        fscanf(fp, "%ld", &windowheight_in);
      }

      /*  Read Polygon material selection data.  */
      else if (!strcmp(keyword, "active_mat"))
             {
              fscanf(fp, "%d %s", &number, tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(mmats, mmatnames, tmpname);
              if (k > -1) active_mat[k] = 1;
             }

      else if (!strcmp(keyword, "angles"))
              fscanf(fp, "%f %f %f", &xrot, &yrot, &zrot);

      else if (!strcmp(keyword, "magnify"))
              fscanf(fp, "%f", &magnify);

      else if (!strcmp(keyword, "range"))
              fscanf(fp, "%f %f %f", &xrange, &yrange, &zrange);

      else if (!strcmp(keyword, "span"))
              fscanf(fp, "%f", &span);

      else if (!strcmp(keyword, "aspect"))
              fscanf(fp, "%f", &aspect);

      else if (!strcmp(keyword, "pan"))
              fscanf(fp, "%f %f", &xpan, &ypan);

      else if (!strcmp(keyword, "translate"))
              fscanf(fp, "%f %f %f", &xtrans, &ytrans, &ztrans);

      else if (!strcmp(keyword, "plotbox_mins"))
              fscanf(fp, "%lf %lf %lf", &globxicmin, &globyicmin, &globzicmin);

      else if (!strcmp(keyword, "plotbox_maxs"))
              fscanf(fp, "%lf %lf %lf", &globxicmax, &globyicmax, &globzicmax);

      /*  Read cutplane and cutsphere info for  */
      /*  each existing cutplane to display.    */
      else if (!strcmp(keyword, "cutplane"))
             {
              fscanf(fp, "%d %d", &i, &temp);
              cutplane_on[i] = temp;
	      fscanf(fp, "%d %s %s", &cutplane[i].npolys, tmptype, tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmptype,"Node"))
                {
                 if (!strcmp(tmpname,"MATERIAL")) number = -1;
                 else 
                   {
                    number = get_name_id(numvars, fieldname, tmpname);
                   }
                }
              if (!strcmp(tmptype,"Cell"))
                {
                 if (!strcmp(tmpname,"MATERIAL")) number = -2;
                 else 
                   {
                    k = get_name_id(cnumvars, cfieldname, tmpname);
                    number = -2;
                    if (k > -1) number = k + numvars;
                   }
                }
              if (i < MAXCUTPLANES)
                 cutplanefieldactive = number;
              else
                 cutspherefieldactive = number;
	      fscanf(fp, "%f %f %f %f", &cutplane[i].aa, &cutplane[i].bb, 
                     &cutplane[i].cc, &cutplane[i].dd);
	      fscanf(fp, "%f %f %f", &cutplane[i].x1, &cutplane[i].x2, 
                     &cutplane[i].x3);
	      fscanf(fp, "%f %f %f", &cutplane[i].y1, &cutplane[i].y2, 
                     &cutplane[i].y3);
	      fscanf(fp, "%f %f %f", &cutplane[i].z1, &cutplane[i].z2, 
                     &cutplane[i].z3);
	      scan_floats(cutplane[i].norm, 3, fp);
	      fscanf(fp, "%d", &number);
	      for (loop = 0; loop < number; loop++)
	         {
		  switch(loop)
		    {
		     case 0: fscanf(fp, "%d", &temp);
                             cutplane[i].heightflag = (short)temp;
			     break;
		     case 1: fscanf(fp, "%e", &cutplane[i].heightfac);
			     break;
		     case 2: fscanf(fp, "%e", &cutplane[i].dist);
			     break;
		     case 3: fscanf(fp, "%d", &temp);
                             cutplane[i].edgesflag = (short)temp;
			     break;
		     case 4: fscanf(fp, "%d", &temp);
                             cutplane[i].velflag = (short)temp;
			     break;
		     case 5: fscanf(fp, "%d", &temp);
                             cutplane[i].nodeflag = (short)temp;
			     break;
		     case 6: fscanf(fp, "%d", &temp);
                             cutplane[i].cliponbox = (short)temp;
			     break;
		     case 7: fscanf(fp, "%d", &temp);
                             cutplane[i].cliponcellsel = (short)temp;
			     break;
		     case 8: fscanf(fp, "%d", &temp);
                             cutplane[i].linesflag = (short)temp;
			     break;
		     case 9: fscanf(fp, "%d", &temp);
                             cutplane[i].facesflag = (short)temp;
			     break;
		     /* add additional flags here */
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
             }

      /*  Read drawing flags.  */
      else if (!strcmp(keyword, "explodefact"))
              fscanf(fp, "%f %f %f", &cexplodefact, &pexplodefact,
                                     &sexplodefact);

      else if (!strcmp(keyword, "velscale"))
              fscanf(fp, "%f", &velscale);

      else if (!strcmp(keyword, "velarrowsize"))
              fscanf(fp, "%f", &velarrowsize);

      else if (!strcmp(keyword, "axisflag"))
             {
              fscanf(fp, "%d", &temp);
              axisflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "nodesflag"))
             {
              fscanf(fp, "%d", &temp);
              nodesflag = (short)temp;
             }

      else if (!strcmp(keyword, "linesflag"))
             {
              fscanf(fp, "%d", &temp);
              linesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "polysflag"))
             {
              fscanf(fp, "%d", &temp);
              polysflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cellsflag"))
             {
              fscanf(fp, "%d", &temp);
              cellsflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "celledgesflag"))
             {
              fscanf(fp, "%d", &temp);
              celledgesflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cellhiddenedgesflag"))
             {
              fscanf(fp, "%d", &temp);
              cellhiddenedgesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "cellexpflag"))
             {
              fscanf(fp, "%d", &temp);
              cellexpflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "timeflag"))
             {
              fscanf(fp, "%d", &temp);
              timeflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cycleflag"))
             {
              fscanf(fp, "%d", &temp);
              cycleflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "distscaleflag"))
             {
              fscanf(fp, "%d", &temp);
              distscaleflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "textureflag"))
             {
              fscanf(fp, "%d", &temp);
              textureflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "tracerflag"))
             {
              fscanf(fp, "%d", &temp);
              tracerflag = (short)temp;
             }

      /*  Read active field names.  */
      else if (!strcmp(keyword, "nodefieldactive"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"MATERIAL")) k = -1;
              else if (!strcmp(tmpname,"GROUP")) k = -2;
              else if (!strcmp(tmpname,"Flags"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id((int)flagtypes_node,flagname_node,tmpname1);
                 if (j > -1) k = j + numvars;
                 else k = -1;
                }
              else
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 k = get_name_id(numvars,fieldname,tmpname1);
                }
              nodefieldactive = k;
             }
	      
      else if (!strcmp(keyword, "cellfieldactive"))
             {
              k = -1;
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"MATERIAL")) k = -1;
              else if (!strcmp(tmpname,"Cell_GROUP")) k = -2;
              else if (!strcmp(tmpname,"Face_GROUP")) k = -3;
              else if (!strcmp(tmpname,"Hidden")) k = -4;
              else if (!strcmp(tmpname,"Flags"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id((int)flagtypes_cell,flagname_cell,tmpname1);
                 if (j > -1) k = j + numvars + cnumvars + fnumvars;
                 else k = -1;
                }
              else if (!strcmp(tmpname,"Node"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id(numvars,fieldname,tmpname1);
                 if (j > -1) k = j;
                 else k = -1;
                }
              else if (!strcmp(tmpname,"Cell"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id(cnumvars,cfieldname,tmpname1);
                 if (j > -1) k = j + numvars;
                 else k = -1;
                }
              else if (!strcmp(tmpname,"Face"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id(fnumvars,ffieldname,tmpname1);
                 if (j > -1) k = j + numvars + cnumvars;
                 else k = -1;
                }
              cellfieldactive = k;
             }
	      
      else if (!strcmp(keyword, "tracefieldactive"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvartr,fieldtrname,tmpname);
              tracefieldactive = k;
             }

      /*  Read isosurface data for isosurfaces.  */    
      else if (!strcmp(keyword, "isosurf"))
             {
              fscanf(fp, "%d", &number);
              fscanf(fp, "%s",tmpchar);
              stripname(fp,tmpchar,tmpname);
              fscanf(fp, "%d %e",&temp, &tempf);
              if (number < MAXCONTOURS)
                 k = get_name_id(numvars, fieldname, tmpname);
              else
                 k = get_name_id(mmats, mmatnames, tmpname);
	      if (k > -1)
                {
                 contour_on[number] = temp;
                 mconfield[number] = k;
                 mconval[number] = tempf;
                }
	     }
	      
      /*  Read tracer limits information.  */
      else if (!strcmp(keyword, "tracefield_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(numvartr, fieldtrname, tmpname);
                 if (k > -1)
                   {
                    globfieldtrmin[k] = tempf;
                    globfieldtrmax[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "tracefield_nice_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(numvartr, fieldtrname, tmpname);
                 if (k > -1)
                   {
                    trfld_nice_min[k] = tempf;
                    trfld_nice_max[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "tracefield_lim_flags"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%d %d %d",&tmp1, &tmp2, &tmp3);
                 k = get_name_id(numvartr, fieldtrname, tmpname);
                 if (k > -1)
                   {
                    trfieldlimauto[k] = (short)tmp1;
                    trfld_nice[k] = (short)tmp2;
                    trfld_log[k] = (short)tmp3;
                   }
                }
             }
	      
      /*  Read tracer field select min, max.  */
      else if (!strcmp(keyword, "tracefield_select_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(numvartr, fieldtrname, tmpname);
                 if (k > -1)
                   {
                    trfldselmin[k] = tempf;
                    trfldselmax[k] = tempf2;
                   }
                }
             }

      else if (!strcmp(keyword, "isovol"))
             {
              fscanf(fp,"%d %f %f %s", &temp,
                     &isovol_val1, &isovol_val2, tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvars,fieldname,tmpname);
              fscanf(fp,"%d",&nopt);
              isovol_on = 1;
	      contour_on[MAXCONTOURS+MAXMATS+1] = 1;
              isovolfieldactive = k;
	      for (loop = 0; loop < nopt; loop++)
	         {
		  fscanf(fp, "%d", &number);
		  switch(loop)
		    {
		     case 0: ivcliponbox = (short) number;
			     break;
		     case 1: ivcliponcellsel = (short) number;
			     break;
		     /* add additional flags here */
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
	     }

      else if (!strcmp(keyword, "isovolfield_draw"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"NONE")) k = -1;
              else k = get_name_id(numvars,fieldname,tmpname);
              isovolfield_draw = k;
             }

      /*  Read and set the background color.  */
      else if (!strcmp(keyword, "backgroundcolor"))
             {
              fscanf(fp, "%d %d %d", &mr, &mg, &mb);
              backgroundcolor = ((mb << 16) | (mg << 8) | mr);
             }

      /*  Read the light location.  */   
      else if (!strcmp(keyword, "light"))
              fscanf(fp, "%f %f", &lightx, &lighty);

      /*  Read the x, y, z values for reflections.  */   
      else if (!strcmp(keyword, "xscale"))
              scan_floats(xscale, 8, fp);
	     
      else if (!strcmp(keyword, "yscale"))
              scan_floats(yscale, 8, fp);
	    
      else if (!strcmp(keyword, "zscale"))
              scan_floats(zscale, 8, fp);

      /*  Read the x, y, z values for the old (original)  */
      /*  scale axis and the current scale axis.          */
      else if (!strcmp(keyword, "scaleaxiso"))
              fscanf(fp, "%f %f %f", 
                     &xscaleaxiso, &yscaleaxiso, &zscaleaxiso);
	     
      else if (!strcmp(keyword, "scaleaxis"))
              fscanf(fp, "%f %f %f", 
                     &xscaleaxis, &yscaleaxis, &zscaleaxis);
	    
      /*  Read the global mins and maxs for the node     */
      /*  variables, as well as the nice and log flags.  */
      else if (!strcmp(keyword, "nodefield_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(numvars, fieldname, tmpname);
                 if (k > -1)
                   {
                    globfieldmin[k] = tempf;
                    globfieldmax[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "nodefield_nice_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(numvars, fieldname, tmpname);
                 if (k > -1)
                   {
                    nfld_nice_min[k] = tempf;
                    nfld_nice_max[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "nodefield_lim_flags"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%d %d %d",&tmp1, &tmp2, &tmp3);
                 k = get_name_id(numvars, fieldname, tmpname);
                 if (k > -1)
                   {
                    nfieldlimauto[k] = (short)tmp1;
                    nfld_nice[k] = (short)tmp2;
                    nfld_log[k] = (short)tmp3;
                   }
                }
             }
	     
      /*  Read the global mins and maxs for the cell     */
      /*  variables, as well as the nice and log flags.  */
      else if (!strcmp(keyword, "cellfield_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(cnumvars, cfieldname, tmpname);
                 if (k > -1)
                   {
                    globfieldcmin[k] = tempf;
                    globfieldcmax[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "cellfield_nice_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(cnumvars, cfieldname, tmpname);
                 if (k > -1)
                   {
                    cfld_nice_min[k] = tempf;
                    cfld_nice_max[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "cellfield_lim_flags"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%d %d %d",&tmp1, &tmp2, &tmp3);
                 k = get_name_id(cnumvars, cfieldname, tmpname);
                 if (k > -1)
                   {
                    cfieldlimauto[k] = (short)tmp1;
                    cfld_nice[k] = (short)tmp2;
                    cfld_log[k] = (short)tmp3;
                   }
                }
             }

      /*  Read the old (original) global x, y, z mins and maxs.  */
      else if (!strcmp(keyword, "plotbox_mins_prev"))
              fscanf(fp, "%lf %lf %lf", 
                     &globxicmino, &globyicmino, &globzicmino);
	      
      else if (!strcmp(keyword, "plotbox_maxs_prev"))
              fscanf(fp, "%lf %lf %lf", 
                     &globxicmaxo, &globyicmaxo, &globzicmaxo);
	      
      /*  Read the isosurface colors, as r, g, b floating point  */
      /*  componets for isosurfaces that will be drawn.          */
      else if (!strcmp(keyword, "conrgba"))
             {
              fscanf(fp, "%d", &number);
	      scan_floats(&(conrgba[4*number]), 4, fp);
             }
	    
      else if (!strcmp(keyword, "cliponbox"))
             {
              fscanf(fp, "%d", &temp);
              cliponbox = (short)temp;
             }
	      
      else if (!strcmp(keyword, "nrefl_loops"))
              fscanf(fp, "%d", &nrefl_loops);
	      
      else if (!strcmp(keyword, "reflect"))
              fscanf(fp, "%d %d %d", &xreflect, &yreflect, &zreflect);
	     
      else if (!strcmp(keyword, "clipvalue"))
              fscanf(fp, "%f %f", &frontclipvalue, &backclipvalue);
             
      /*  Read the polygon subset box mins and maxs.  */
      else if (!strcmp(keyword, "polysubset_mins"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbdval, &yminbdval, &zminbdval);
	     
      else if (!strcmp(keyword, "polysubset_maxs"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbdval, &ymaxbdval, &zmaxbdval);
	      
      else if (!strcmp(keyword, "subset_mins"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminfdval, &yminfdval, &zminfdval);
	      
      else if (!strcmp(keyword, "subset_maxs"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxfdval, &ymaxfdval, &zmaxfdval);
	      
      /*  Read the material preference order.  */
      else if (!strcmp(keyword, "mat_order"))
             {
              fscanf(fp, "%d", &nmat_order);
	      scan_ints(mat_order, nmat_order, fp);
             }
	      
      /*  Read the node material and flag selection data.  */
      else if (!strcmp(keyword, "nodeselflag"))
             {
              if (nodeselfirst)
                {
                 for (i = 0; i <= MAXFLAGS; i++)
                   {
                    for (j = 0; j < MAXMATS; j++)
                       nodeselflag[i][j] = 0;
                   }
                 nodeselfirst = 0;
                }
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              fscanf(fp, "%s", tmpchar);
              if (!strcmp(tmpname1,"Material"))   /* Material names. */
                {
                 stripname(fp,tmpchar,tmpname2);
                 if (!strcmp(tmpname2,"ALL"))
                   {
                    for (j = 0; j < maxcellmat; j++)
                       nodeselflag[0][j] = 1;
                   }
                 else
                   {
                    number = atoi(tmpchar);
                    for (i = 0; i < number; i++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname3);
                       k = get_name_id(mmats,mmatnames,tmpname3);
                       if (k > -1) nodeselflag[0][k] = 1;
                      }
                   }
                }
              else  /*  Flag names. */
                { 
                 stripname(fp,tmpchar,tmpname2);
                 i = get_name_id(numflags_node,flagname_node,tmpname2);
                 fscanf(fp,"%s",tmpchar);
                 stripname(fp,tmpchar,tmpname3);
                 if (!strcmp(tmpname3,"ALL") && i >= 0)
                   {
                    for (j = 0; j < flagtypes_node[i]; j++)
                          nodeselflag[i+1][j] = 1;
                   }               
                 else
                   {
                    number = atoi(tmpchar);
                    for (j = 0; j < number; j++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname);
                       if (i > -1)
                         {
                          k = get_name_id((int)flagtypes_node[i],
                                          flagnames_node[i],tmpname);
                          if (k > -1) nodeselflag[i+1][k] = 1;
                         }
                      }
                   }
                }
             }
	     
      else if (!strcmp(keyword, "nodeandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(nodeandorflag, number, fp);
             }
	      
      /*  Read the cell material and flag selection data.  */
      else if (!strcmp(keyword, "cellselflag"))
             {
              if (cellselfirst)
                {
                 for (i = 0; i <= MAXFLAGS; i++)
                   {
                    for (j = 0; j < MAXMATS; j++)
                       cellselflag[i][j] = 0;
                   }
                cellselfirst = 0;
               }
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              fscanf(fp, "%s", tmpchar);
              if (!strcmp(tmpname1,"Material"))   /* Material names. */
                {
                 stripname(fp,tmpchar,tmpname2);
                 if (!strcmp(tmpname2,"ALL"))
                   {
                    for (j = 0; j < maxcellmat; j++)
                       cellselflag[0][j] = 1;
                   }
                 else
                   {
                    number = atoi(tmpchar);
                    for (i = 0; i < number; i++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname3);
                       k = get_name_id(mmats,mmatnames,tmpname3);
                       if (k > -1) cellselflag[0][k] = 1;
                      }
                   }
                }
              else  /*  Flag names. */
                { 
                 stripname(fp,tmpchar,tmpname2);
                 i = get_name_id(numflags_cell,flagname_cell,tmpname2);
                 fscanf(fp,"%s",tmpchar);
                 stripname(fp,tmpchar,tmpname3);
                 if (!strcmp(tmpname3,"ALL") && i >= 0)
                   {
                    for (j = 0; j < flagtypes_cell[i]; j++)
                          cellselflag[i+1][j] = 1;
                   }               
                 else
                   {
                    number = atoi(tmpchar);
                    for (j = 0; j < number; j++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname);
                       if (i > -1)
                         {
                          k = get_name_id((int)flagtypes_cell[i],
                                          flagnames_cell[i],tmpname);
                          if (k > -1) cellselflag[i+1][k] = 1;
                         }
                      }
                   }
                }
             }
	    
      else if (!strcmp(keyword, "cellandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(cellandorflag, number, fp);
             }

      /*  Read colorbar and bounding box flags.  */
      else if (!strcmp(keyword, "colorbarflag"))
             {
              fscanf(fp, "%d", &temp);
              colorbarflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "colorbarpref"))
             {
              fscanf(fp, "%d", &temp);
              colorbarpref = (short)temp;
             }
	     
      else if (!strcmp(keyword, "boundingboxflag"))
             {
              fscanf(fp, "%d", &temp);
              boundingboxflag = (short)temp;
             }
	      
      /*  Read the bounding box mins and maxs.  */
      else if (!strcmp(keyword, "boundingbox_mins"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbbval, &yminbbval, &zminbbval);
	      
      else if (!strcmp(keyword, "boundingbox_maxs"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbbval, &ymaxbbval, &zmaxbbval);
	     
      else if (!strcmp(keyword, "boundingbox_mins_prev"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbbvalo, &yminbbvalo, &zminbbvalo);
	        
      else if (!strcmp(keyword, "boundingbox_maxs_prev"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbbvalo, &ymaxbbvalo, &zmaxbbvalo);
	          
      /*  Read the view and fly position flags.  */
      else if (!strcmp(keyword, "viewflag"))
             {
              fscanf(fp, "%d", &temp);
              viewflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "flypos"))
              scan_floats(flypos, 3, fp);
	      
      /*  Read more flags and data.  */	     
      else if (!strcmp(keyword, "cellcontflag"))
             {
              fscanf(fp, "%d", &temp);
              cellcontflag = (short)temp;
             }
	    
      else if (!strcmp(keyword, "cliponcellsel"))
             {
              fscanf(fp, "%d", &temp);
              cliponcellsel = (short)temp;
             }

      /*  Read node vector info.  */
      else if (!strcmp(keyword, "vectfld_"))
             {
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              if (!strcmp(tmpname1,"NONE")) vectfldx = -1;
              else
                {
                 k = get_name_id(numvars,fieldname,tmpname1);
                 vectfldx = k;
                }
              fscanf(fp, "%s", tmpchar2);
              stripname(fp,tmpchar2,tmpname2);
              if (!strcmp(tmpname2,"NONE")) vectfldy = -1;
              else
                {
                 k = get_name_id(numvars,fieldname,tmpname2);
                 vectfldy = k;
                }
              fscanf(fp, "%s", tmpchar3);
              stripname(fp,tmpchar3,tmpname3);
              if (!strcmp(tmpname3,"NONE")) vectfldz = -1;
              else
                {
                 k = get_name_id(numvars,fieldname,tmpname3);
                 vectfldz = k;
                }
             }
	   
      else if (!strcmp(keyword, "speedfld"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvars,fieldname,tmpname);
              speedfld = k;
             }
	      
      else if (!strcmp(keyword, "maxspeed"))
              fscanf(fp, "%lf", &maxspeed);
	     
      else if (!strcmp(keyword, "vectorflag"))
             {
              fscanf(fp, "%d", &temp);
              vectorflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "vectorarrow"))
             {
              fscanf(fp, "%d", &temp);
              vectorarrow = (short)temp;
             }
	     
      else if (!strcmp(keyword, "vectorlog"))
             {
              fscanf(fp, "%d", &temp);
              vectorlog = (short)temp;
             }

      /*  Read cell cvector info.  */
      else if (!strcmp(keyword, "cvectfld_"))
             {
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              if (!strcmp(tmpname1,"NONE")) cvectfldx = -1;
              else
                {
                 k = get_name_id(cnumvars,cfieldname,tmpname1);
                 cvectfldx = k;
                }
              fscanf(fp, "%s", tmpchar2);
              stripname(fp,tmpchar2,tmpname2);
              if (!strcmp(tmpname2,"NONE")) cvectfldy = -1;
              else
                {
                 k = get_name_id(cnumvars,cfieldname,tmpname2);
                 cvectfldy = k;
                }
              fscanf(fp, "%s", tmpchar3);
              stripname(fp,tmpchar3,tmpname3);
              if (!strcmp(tmpname3,"NONE")) cvectfldz = -1;
              else
                {
                 k = get_name_id(cnumvars,cfieldname,tmpname3);
                 cvectfldz = k;
                }
             }
	   
      else if (!strcmp(keyword, "cspeedfld"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(cnumvars,cfieldname,tmpname);
              cspeedfld = k;
             }
	      
      else if (!strcmp(keyword, "cmaxspeed"))
              fscanf(fp, "%lf", &cmaxspeed);
	     
      else if (!strcmp(keyword, "cvectorflag"))
             {
              fscanf(fp, "%d", &temp);
              cvectorflag = (short)temp;
             }
	     
      /*  Read title flag and data.  */
      else if (!strcmp(keyword, "titleflag"))
             {
              fscanf(fp, "%d", &temp);
              titleflag = (short)temp;
             }
	          
      else if (!strcmp(keyword, "titlestr"))
             {
              fscanf(fp, "%c", &tempchar);
	      temp = 0;
	      while (9)
	        {
	         fscanf(fp, "%c", &tempchar);
	         if (tempchar == '\n')
	            break;
	         titlestr[temp] = tempchar;
	         temp++;
	        }
	      titlestr[temp] = '\0';
             }
	          
      else if (!strcmp(keyword, "titlestr2"))
             {
              fscanf(fp, "%c", &tempchar);
	      temp = 0;
	      while (9)
	        {
	         fscanf(fp, "%c", &tempchar);
	         if (tempchar == '\n')
	            break;
	         titlestr2[temp] = tempchar;
	         temp++;
	        }
	      titlestr2[temp] = '\0';
             }
	 
      /*  Read cutline data for active cutlines.  */
      else if (!strcmp(keyword, "cutline"))
             { 
              fscanf(fp, "%d", &temp);
              j = temp;
              fscanf(fp, "%d", &temp);
	      cutlineon[j] = temp;
              fscanf(fp, "%s %s",tmptype,tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmptype,"Node"))
                {
                 k = get_name_id(numvars,fieldname,tmpname);
                 if (k < 0) cutlineon[j] = 0;
                 else cutlinefield[j] = k;
                }
              else
                {
                 k = get_name_id(cnumvars,cfieldname,tmpname);
                 if (k < 0) cutlineon[j] = 0;
                 else cutlinefield[j] = k + numvars;
                }
	      fscanf(fp, "%f", &cutlineradius[j]);
	      fscanf(fp, "%f %f %f", &ctlinx1[j], &ctliny1[j], &ctlinz1[j]);
	      fscanf(fp, "%f %f %f", &ctlinx2[j], &ctliny2[j],  &ctlinz2[j]);
	      fscanf(fp, "%d", &number);
	      for (loop = 0; loop < number; loop++)
		 {
		  switch(loop)
		    {
		     case 0: fscanf(fp, "%d", &number2);
			     cutlinenodeson[j] = (short) number2;
			     break;
		     case 1: fscanf(fp, "%d", &number2);
			     cutlinenumson[j] = (short) number2;
			     break;
		     case 2: fscanf(fp, "%d", &number2);
			     waveflag[j] = (short) number2;
			     break;
		     case 3: fscanf(fp, "%f", &wavefac[j]);
			     break;
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
             }


      /*  Read node selection flags and data.  */
      else if (!strcmp(keyword, "nodeselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "nodeselfld_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselfield"))
             {
              fscanf(fp, "%s",tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvars,fieldname,tmpname);
              if (k < 0) nodeselfld_on = 0;
              nodeselfield = (short)k;
             }
	
      else if (!strcmp(keyword, "nfldselmin"))
              fscanf(fp, "%lf", &nfldselmin);
	
      else if (!strcmp(keyword, "nfldselmax"))
              fscanf(fp, "%lf", &nfldselmax);
     
      else if (!strcmp(keyword, "nodeselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "nodeselsph_x"))
              fscanf(fp, "%f", &nodeselsph_x);
	
      else if (!strcmp(keyword, "nodeselsph_y"))
              fscanf(fp, "%f", &nodeselsph_y);
	
      else if (!strcmp(keyword, "nodeselsph_z"))
              fscanf(fp, "%f", &nodeselsph_z);
	
      else if (!strcmp(keyword, "nodeselsph_r"))
              fscanf(fp, "%f", &nodeselsph_r);
     
      else if (!strcmp(keyword, "nodeselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "nodeselbox_xmin"))
              fscanf(fp, "%f", &nodeselbox_xmin);
	
      else if (!strcmp(keyword, "nodeselbox_xmax"))
              fscanf(fp, "%f", &nodeselbox_xmax);
	
      else if (!strcmp(keyword, "nodeselbox_ymin"))
              fscanf(fp, "%f", &nodeselbox_ymin);
	
      else if (!strcmp(keyword, "nodeselbox_ymax"))
              fscanf(fp, "%f", &nodeselbox_ymax);
	
      else if (!strcmp(keyword, "nodeselbox_zmin"))
              fscanf(fp, "%f", &nodeselbox_zmin);
	
      else if (!strcmp(keyword, "nodeselbox_zmax"))
              fscanf(fp, "%f", &nodeselbox_zmax);
     
      else if (!strcmp(keyword, "nodeselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              nodeselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "nnodeselnum"))
             {
              fscanf(fp, "%d", &temp);
              nnodeselnum = (short)temp;
              for (i = 0; i < nnodeselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &nodeselnum_f[i],
                       &nodeselnum_l[i], &nodeselnum_s[i]);
                }
             }
	  
      /*  Read cell selection flags and data.  */
      else if (!strcmp(keyword, "cellselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "cellselcfld_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselcfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselfield"))
             {
              fscanf(fp, "%s",tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(cnumvars,cfieldname,tmpname);
              if (k < 0) cellselcfld_on = 0;
              cellselfield = (short)k;
             }
	
      else if (!strcmp(keyword, "cfldselmin"))
              fscanf(fp, "%lf", &cfldselmin);
	
      else if (!strcmp(keyword, "cfldselmax"))
              fscanf(fp, "%lf", &cfldselmax);
     
      else if (!strcmp(keyword, "cellselnfld_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselnfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselnfield"))
             {
              fscanf(fp, "%s",tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvars,fieldname,tmpname);
              if (k < 0) cellselnfld_on = 0;
              cellselnfield = (short)k;
             }

      else if (!strcmp(keyword, "cellselnfldopt"))
             {
              fscanf(fp, "%d", &temp);
              cellselnfldopt = (short)temp;
             }
	
      else if (!strcmp(keyword, "cnfldselmin"))
              fscanf(fp, "%lf", &cnfldselmin);
	
      else if (!strcmp(keyword, "cnfldselmax"))
              fscanf(fp, "%lf", &cnfldselmax);
     
      else if (!strcmp(keyword, "cellselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellselsph_x"))
              fscanf(fp, "%f", &cellselsph_x);
	
      else if (!strcmp(keyword, "cellselsph_y"))
              fscanf(fp, "%f", &cellselsph_y);
	
      else if (!strcmp(keyword, "cellselsph_z"))
              fscanf(fp, "%f", &cellselsph_z);
	
      else if (!strcmp(keyword, "cellselsph_r"))
              fscanf(fp, "%f", &cellselsph_r);
     
      else if (!strcmp(keyword, "cellselsph_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselsph_in = (short)temp;
             }
     
      else if (!strcmp(keyword, "cellselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellselbox_xmin"))
              fscanf(fp, "%f", &cellselbox_xmin);
	
      else if (!strcmp(keyword, "cellselbox_xmax"))
              fscanf(fp, "%f", &cellselbox_xmax);
	
      else if (!strcmp(keyword, "cellselbox_ymin"))
              fscanf(fp, "%f", &cellselbox_ymin);
	
      else if (!strcmp(keyword, "cellselbox_ymax"))
              fscanf(fp, "%f", &cellselbox_ymax);
	
      else if (!strcmp(keyword, "cellselbox_zmin"))
              fscanf(fp, "%f", &cellselbox_zmin);
	
      else if (!strcmp(keyword, "cellselbox_zmax"))
              fscanf(fp, "%f", &cellselbox_zmax);
     
      else if (!strcmp(keyword, "cellselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "cellselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "ncellselnum"))
             {
              fscanf(fp, "%d", &temp);
              ncellselnum = (short)temp;
              for (i = 0; i < ncellselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &cellselnum_f[i],
                        &cellselnum_l[i],&cellselnum_s[i]);
                }
             }

      /*  Read tracer selection flags and data.  */     
      else if (!strcmp(keyword, "traceselfld_on"))
             {
              fscanf(fp, "%d", &temp);
              traceselfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "traceselfield"))
             {
              fscanf(fp, "%s",tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvartr,fieldtrname,tmpname);
              if (k < 0)
                traceselfld_on = 0; //turn off tracer select, no name match
              else
              {
                traceselfield = k;
                traceselfields[k] = 1; //turn selection on for tracer field
              }
             }
	
      else if (!strcmp(keyword, "trfldselmin"))
              fscanf(fp, "%lf", &trfldselmin[traceselfield]);
      else if (!strcmp(keyword, "trfldselmax"))
              fscanf(fp, "%lf", &trfldselmax[traceselfield]);
      else if (!strcmp(keyword, "traceselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              traceselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "traceselsph_x"))
              fscanf(fp, "%f", &traceselsph_x);
	
      else if (!strcmp(keyword, "traceselsph_y"))
              fscanf(fp, "%f", &traceselsph_y);
	
      else if (!strcmp(keyword, "traceselsph_z"))
              fscanf(fp, "%f", &traceselsph_z);
	
      else if (!strcmp(keyword, "traceselsph_r"))
              fscanf(fp, "%f", &traceselsph_r);
     
      else if (!strcmp(keyword, "traceselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              traceselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "traceselbox_xmin"))
              fscanf(fp, "%f", &traceselbox_xmin);
	
      else if (!strcmp(keyword, "traceselbox_xmax"))
              fscanf(fp, "%f", &traceselbox_xmax);
	
      else if (!strcmp(keyword, "traceselbox_ymin"))
              fscanf(fp, "%f", &traceselbox_ymin);
	
      else if (!strcmp(keyword, "traceselbox_ymax"))
              fscanf(fp, "%f", &traceselbox_ymax);
	
      else if (!strcmp(keyword, "traceselbox_zmin"))
              fscanf(fp, "%f", &traceselbox_zmin);
	
      else if (!strcmp(keyword, "traceselbox_zmax"))
              fscanf(fp, "%f", &traceselbox_zmax);
     
      else if (!strcmp(keyword, "traceselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              traceselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "traceselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              traceselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "ntraceselnum"))
             {
              fscanf(fp, "%d", &temp);
              ntraceselnum = (short)temp;
              for (i = 0; i < ntraceselnum; i++)
                {
                 fscanf(fp,"%d %d %d", &traceselnum_f[i],
                       &traceselnum_l[i], &traceselnum_s[i]);
                }
             }
	          
      /*  Read a node field calculation structure.  */
      else if (!strcmp(keyword, "fieldcalc"))
             {
	      scan_struct_fldcalc2(fieldcalc, 1, fp);
             }
     	
      /*  Read a cell field calculation structure.  */
      else if (!strcmp(keyword, "cfieldcalc"))
             {
              scan_struct_fldcalc2(cfieldcalc, 0, fp);
             }
     	
      /*  Read a tracer field calculation structure.  */
      else if (!strcmp(keyword, "tfieldcalc"))
             {
              scan_struct_fldcalc2(tfieldcalc, 3, fp);
             }
	
      /*  Read bounding box coordinate flag.  */
      else if (!strcmp(keyword, "bboxcoordflag"))
             {
              fscanf(fp, "%d", &temp);
              bboxcoordflag = (short)temp;
             }

      /*  Read cellnum, nodenum and surfnum flags.  */
      else if (!strcmp(keyword, "nodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              nodenumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellnumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfnumflag"))
             {
              fscanf(fp, "%d", &temp);
              surfnumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "filename"))
              fscanf(fp, "%s", junk);
	
      /*  Read point size, line size, and more flags and data.  */
      else if (!strcmp(keyword, "pnt_size"))
             {
              fscanf(fp, "%d %d", &tmp1, &tmp2);
              pnt_size = (short) tmp1;
              pnt_shape = (short) tmp2;
             }
	
      else if (!strcmp(keyword, "line_size"))
             {
              fscanf(fp, "%d %d", &tmp1, &tmp2);
              line_size = (short) tmp1;
              line_type = (short) tmp2;
             }

      else if (!strcmp(keyword, "node0matflag"))
             {
              fscanf(fp, "%d", &temp);
              node0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "cell0matflag"))
             {
              fscanf(fp, "%d", &temp);
              cell0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "surf0matflag"))
             {
              fscanf(fp, "%d", &temp);
              surf0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "tracehistflag"))
             {
              fscanf(fp, "%d", &temp);
              tracehistflag = (short)temp;
             }

      else if (!strcmp(keyword, "tracehiststop"))
             {
              fscanf(fp, "%d", &temp);
              tracehiststop = (short)temp;
             }

      else if (!strcmp(keyword, "tracehiststride"))
             {
              fscanf(fp, "%d", &temp);
              tracehiststride = (short)temp;
             }

      else if (!strcmp(keyword, "tracehistpoints"))
             {
              fscanf(fp, "%d", &temp);
              tracehistpoints = (short)temp;
             }

      else if (!strcmp(keyword, "cellshadeflag"))
             {
              fscanf(fp, "%d", &temp);
              cellshadeflag = (short)temp;
             }

      /*  Read the field to color field isovolume with.  */
      else if (!strcmp(keyword, "isovolfield_draw"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"NONE")) k = -1;
              else k = get_name_id(numvars,fieldname,tmpname);
              isovolfield_draw = k;
             }

      /*  Read the field to color field isosurfaces with.  */
      else if (!strcmp(keyword, "contourfield_draw"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"NONE")) k = -1;
              else k = get_name_id(numvars,fieldname,tmpname);
              contourfield_draw = k;
             }

      /*  Read the field to color material isosurfaces with.  */
      else if (!strcmp(keyword, "mcontourfield_draw"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"NONE")) k = -1;
              else k = get_name_id(numvars,fieldname,tmpname);
              mcontourfield_draw = k;
             }
	     
      /*  Read the field colormap to use, and special map rgb, if any.  */
      else if (!strcmp(keyword, "fieldcolortype"))
             {
              fscanf(fp, "%d", &temp);
              fieldcolortype = (short)temp;
             }

      else if (!strcmp(keyword, "jcolor"))
             {
              for (i = 0; i < 256; i++)
                {
                 fscanf(fp, "%d %d %d", &mr, &mg, &mb);
                 jcolor[i] = (mr << 24) |(mg << 16 ) | (mb << 8 ) | 255;
                }
              resettexture();
             }

      /*  Read more flags.  */
      else if (!strcmp(keyword, "nset_contours"))
              fscanf(fp, "%d", &nset_contours);

      else if (!strcmp(keyword, "facerefineflag"))
             {
              fscanf(fp, "%d", &temp);
              facerefineflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellnormflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnormflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellmedianedgeflag"))
             {
              fscanf(fp, "%d", &temp);
              cellmedianedgeflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellnodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnodenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellfacenumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellfacenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "polystiplflag"))
             {
              fscanf(fp, "%d", &temp);
              polystiplflag = (short)temp;
             }

      /*  Surface attributes follow.  */

      else if (!strcmp(keyword, "surfflag"))
             {
              fscanf(fp, "%d", &temp);
              surfflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfedgesflag"))
             {
              fscanf(fp, "%d", &temp);
              surfedgesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfexpflag"))
             {
              fscanf(fp, "%d", &temp);
              surfexpflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "surffieldactive"))
             {
              k = -1;
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              if (!strcmp(tmpname,"MATERIAL")) k = -1;
              else if (!strcmp(tmpname,"Surf_GROUP")) k = -2;
              else if (!strcmp(tmpname,"Flags"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id((int)flagtypes_surf,flagname_surf,tmpname1);
                 if (j > -1) k = j + numvars + cnumvars + fnumvars;
                 else k = -1;
                }
              else if (!strcmp(tmpname,"Node"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id(numvars,fieldname,tmpname1);
                 if (j > -1) k = j;
                 else k = -1;
                }
              else if (!strcmp(tmpname,"Surf"))
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname1);
                 j = get_name_id(snumvars,sfieldname,tmpname1);
                 if (j > -1) k = j + numvars;
                 else k = -1;
                }
              surffieldactive = k;
             }

      else if (!strcmp(keyword, "surfshadeflag"))
             {
              fscanf(fp, "%d", &temp);
              surfshadeflag = (short)temp;
             }

      else if (!strcmp(keyword, "surfnodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              surfnodenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "surfrefineflag"))
             {
              fscanf(fp, "%d", &temp);
              surfrefineflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfcontflag"))
             {
              fscanf(fp, "%d", &temp);
              surfcontflag = (short)temp;
             }

      else if (!strcmp(keyword, "surf0matflag"))
             {
              fscanf(fp, "%d", &temp);
              surf0matflag = (short)temp;
             }
	    
      /*  Read surface vector info.  */ 
      else if (!strcmp(keyword, "svectfld_"))
             {
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              if (!strcmp(tmpname1,"NONE")) svectfldx = -1;
              else
                {
                 k = get_name_id(snumvars,sfieldname,tmpname1);
                 svectfldx = k;
                }
              fscanf(fp, "%s", tmpchar2);
              stripname(fp,tmpchar2,tmpname2);
              if (!strcmp(tmpname2,"NONE")) svectfldy = -1;
              else
                {
                 k = get_name_id(snumvars,sfieldname,tmpname2);
                 svectfldy = k;
                }
              fscanf(fp, "%s", tmpchar3);
              stripname(fp,tmpchar3,tmpname3);
              if (!strcmp(tmpname3,"NONE")) svectfldz = -1;
              else
                {
                 k = get_name_id(snumvars,sfieldname,tmpname3);
                 svectfldz = k;
                }
             }
	   
      else if (!strcmp(keyword, "sspeedfld"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(snumvars,sfieldname,tmpname);
              sspeedfld = k;
             }
	      
      else if (!strcmp(keyword, "smaxspeed"))
              fscanf(fp, "%lf", &smaxspeed);
	     
      else if (!strcmp(keyword, "svectorflag"))
             {
              fscanf(fp, "%d", &temp);
              svectorflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "surffield_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(snumvars, sfieldname, tmpname);
                 if (k > -1)
                   {
                    globfieldsmin[k] = tempf;
                    globfieldsmax[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "surffield_nice_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(snumvars, sfieldname, tmpname);
                 if (k > -1)
                   {
                    sfld_nice_min[k] = tempf;
                    sfld_nice_max[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "surffield_lim_flags"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%d %d %d",&tmp1, &tmp2, &tmp3);
                 k = get_name_id(snumvars, sfieldname, tmpname);
                 if (k > -1)
                   {
                    sfieldlimauto[k] = (short)tmp1;
                    sfld_nice[k] = (short)tmp2;
                    sfld_log[k] = (short)tmp3;
                   }
                }
             }
	
      else if (!strcmp(keyword, "sfieldcalc"))
             {
              fscanf(fp, "%d", &number);
              scan_struct_fldcalc2(sfieldcalc, number, fp);
             }
	      
      /*  Read the surface material and flag selection data.  */
      else if (!strcmp(keyword, "surfselflag"))
             {
              if (surfselfirst)
                {
                 for (i = 0; i <= MAXFLAGS; i++)
                   {
                    for (j = 0; j < MAXMATS; j++)
                       surfselflag[i][j] = 0;
                   }
                surfselfirst = 0;
               }
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              fscanf(fp, "%s", tmpchar);
              if (!strcmp(tmpname1,"Material"))   /* Material names. */
                {
                 stripname(fp,tmpchar,tmpname2);
                 if (!strcmp(tmpname2,"ALL"))
                   {
                    for (j = 0; j < maxsurfmat; j++)
                       surfselflag[0][j] = 1;
                   }
                 else
                   {
                    number = atoi(tmpchar);
                    for (i = 0; i < number; i++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname3);
                       k = get_name_id(maxsurfmat,mmatnames,tmpname3);
                       if (k > -1) surfselflag[0][k] = 1;
                      }
                   }
                }
              else  /*  Flag names. */
                { 
                 stripname(fp,tmpchar,tmpname2);
                 i = get_name_id(numflags_surf,flagname_surf,tmpname2);
                 fscanf(fp,"%s",tmpchar);
                 stripname(fp,tmpchar,tmpname3);
                 if (!strcmp(tmpname3,"ALL") && i >= 0)
                   {
                    for (j = 0; j < flagtypes_surf[i]; j++)
                          surfselflag[i+1][j] = 1;
                   }               
                 else
                   {
                    number = atoi(tmpchar);
                    for (j = 0; j < number; j++)
                      {
                       fscanf(fp, "%s", tmpchar);
                       stripname(fp,tmpchar,tmpname);
                       if (i > -1)
                         {
                          k = get_name_id((int)flagtypes_surf[i],
                                          flagnames_surf[i],tmpname);
                          if (k > -1) surfselflag[i+1][k] = 1;
                         }
                      }
                   }
                }
             }
	     
      /*  Read the surface material and flag and/or flags.  */
      else if (!strcmp(keyword, "surfandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(surfandorflag, number, fp);
             }

      /*  Read surface selection data.  */
      else if (!strcmp(keyword, "surfselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "surfselsfld_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselfield"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(snumvars,sfieldname,tmpname);
              if (k < 0) surfselsfld_on = 0;
              else surfselfield = (short)k;
             }
	
      else if (!strcmp(keyword, "sfldselmin"))
              fscanf(fp, "%lf", &sfldselmin);
	
      else if (!strcmp(keyword, "sfldselmax"))
              fscanf(fp, "%lf", &sfldselmax);
     
      else if (!strcmp(keyword, "surfselnfld_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselnfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselnfield"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(numvars,fieldname,tmpname);
              if (k < 0) surfselnfld_on = 0;
              else surfselnfield = (short)k;
             }

      else if (!strcmp(keyword, "surfselnfldopt"))
             {
              fscanf(fp, "%d", &temp);
              surfselnfldopt = (short)temp;
             }
	
      else if (!strcmp(keyword, "snfldselmin"))
              fscanf(fp, "%lf", &snfldselmin);
	
      else if (!strcmp(keyword, "snfldselmax"))
              fscanf(fp, "%lf", &snfldselmax);
     
      else if (!strcmp(keyword, "surfselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfselsph_x"))
              fscanf(fp, "%f", &surfselsph_x);
	
      else if (!strcmp(keyword, "surfselsph_y"))
              fscanf(fp, "%f", &surfselsph_y);
	
      else if (!strcmp(keyword, "surfselsph_z"))
              fscanf(fp, "%f", &surfselsph_z);
	
      else if (!strcmp(keyword, "surfselsph_r"))
              fscanf(fp, "%f", &surfselsph_r);
     
      else if (!strcmp(keyword, "surfselsph_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselsph_in = (short)temp;
             }
     
      else if (!strcmp(keyword, "surfselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfselbox_xmin"))
              fscanf(fp, "%f", &surfselbox_xmin);
	
      else if (!strcmp(keyword, "surfselbox_xmax"))
              fscanf(fp, "%f", &surfselbox_xmax);
	
      else if (!strcmp(keyword, "surfselbox_ymin"))
              fscanf(fp, "%f", &surfselbox_ymin);
	
      else if (!strcmp(keyword, "surfselbox_ymax"))
              fscanf(fp, "%f", &surfselbox_ymax);
	
      else if (!strcmp(keyword, "surfselbox_zmin"))
              fscanf(fp, "%f", &surfselbox_zmin);
	
      else if (!strcmp(keyword, "surfselbox_zmax"))
              fscanf(fp, "%f", &surfselbox_zmax);
     
      else if (!strcmp(keyword, "surfselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "surfselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "nsurfselnum"))
             {
              fscanf(fp, "%d", &temp);
              nsurfselnum = (short)temp;
              for (i = 0; i < nsurfselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &surfselnum_f[i],
                        &surfselnum_l[i],&surfselnum_s[i]);
                }
             }

      /*  Read face variable info.  */
      else if (!strcmp(keyword, "facefield_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(fnumvars, ffieldname, tmpname);
                 if (k > -1)
                   {
                    globfieldfmin[k] = tempf;
                    globfieldfmax[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "facefield_nice_limits"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%e %e",&tempf, &tempf2);
                 k = get_name_id(fnumvars, ffieldname, tmpname);
                 if (k > -1)
                   {
                    ffld_nice_min[k] = tempf;
                    ffld_nice_max[k] = tempf2;
                   }
                }
             }
	    
      else if (!strcmp(keyword, "facefield_lim_flags"))
             {
              fscanf(fp, "%d", &number);
              for (i = 0; i < number; i++)
                {
                 fscanf(fp, "%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 fscanf(fp, "%d %d %d",&tmp1, &tmp2, &tmp3);
                 k = get_name_id(fnumvars, ffieldname, tmpname);
                 if (k > -1)
                   {
                    ffieldlimauto[k] = (short)tmp1;
                    ffld_nice[k] = (short)tmp2;
                    ffld_log[k] = (short)tmp3;
                   }
                }
             }
	    
      /*  Read face vector info.  */ 
      else if (!strcmp(keyword, "fvectfld_"))
             {
              fscanf(fp, "%s", tmpchar1);
              stripname(fp,tmpchar1,tmpname1);
              if (!strcmp(tmpname1,"NONE")) fvectfldx = -1;
              else
                {
                 k = get_name_id(fnumvars,ffieldname,tmpname1);
                 fvectfldx = k;
                }
              fscanf(fp, "%s", tmpchar2);
              stripname(fp,tmpchar2,tmpname2);
              if (!strcmp(tmpname2,"NONE")) fvectfldy = -1;
              else
                {
                 k = get_name_id(fnumvars,ffieldname,tmpname2);
                 fvectfldy = k;
                }
              fscanf(fp, "%s", tmpchar3);
              stripname(fp,tmpchar3,tmpname3);
              if (!strcmp(tmpname3,"NONE")) fvectfldz = -1;
              else
                {
                 k = get_name_id(fnumvars,ffieldname,tmpname3);
                 fvectfldz = k;
                }
             }
	   
      else if (!strcmp(keyword, "fspeedfld"))
             {
              fscanf(fp, "%s", tmpchar);
              stripname(fp,tmpchar,tmpname);
              k = get_name_id(fnumvars,ffieldname,tmpname);
              fspeedfld = k;
             }
	     
      else if (!strcmp(keyword, "fvectorflag"))
             {
              fscanf(fp, "%d", &temp);
              fvectorflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "fmaxspeed"))
              fscanf(fp, "%lf", &fmaxspeed);

      /*  Read node group selection data, if any.  */
      else if (!strcmp(keyword, "nodeselnodegrp_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselnodegrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              nodeselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nnodegrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nnodegrpselected = (int)temp;
              for (i = 0; i < nnodegrpselected; i++)
                {
                 fscanf(fp,"%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 k = get_name_id(nnodegrps,nodegrpname,tmpname);
                 if (k > -1) nodegrpselected[i] = k + 1;
                }
             }

      /*  Read cell group selection data.  */
      else if (!strcmp(keyword, "cellselcellgrp_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselcellgrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "ncellgrpselected"))
             {
              fscanf(fp, "%d", &temp);
              ncellgrpselected = (int)temp;
              for (i = 0; i < ncellgrpselected; i++)
                {
                 fscanf(fp,"%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 k = get_name_id(ncellgrps,cellgrpname,tmpname);
                 if (k > -1) cellgrpselected[i] = k + 1;
                }
             }

      /*  Read cell face group selection data.  */
      else if (!strcmp(keyword, "cellselfacegrp_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselfacegrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "faceselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              faceselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nfacegrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nfacegrpselected = (int)temp;
              for (i = 0; i < nfacegrpselected; i++)
                {
                 fscanf(fp,"%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 k = get_name_id(nfacegrps,facegrpname,tmpname);
                 if (k > -1) facegrpselected[i] = k + 1;
                }
             }

      /*  Read surface group selection data.  */
      else if (!strcmp(keyword, "surfselsurfgrp_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsurfgrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nsurfgrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nsurfgrpselected = (int)temp;
              for (i = 0; i < nsurfgrpselected; i++)
                {
                 fscanf(fp,"%s", tmpchar);
                 stripname(fp,tmpchar,tmpname);
                 k = get_name_id(nsurfacegrps,surfacegrpname,tmpname);
                 if (k > -1) surfgrpselected[i] = k + 1;
                }
             }

      /*  Read draw ghost options.  */
      else if (!strcmp(keyword, "drawghosts"))
             {
              fscanf(fp, "%d", &temp);
              drawghosts = (short)temp;
             }

      else if (!strcmp(keyword, "drawghostface"))
             {
              fscanf(fp, "%d", &temp);
              drawghostface = (short)temp;
             }

      else if (!strcmp(keyword, "drawnodeghosts"))
             {
              fscanf(fp, "%d", &temp);
              drawnodeghosts = (short)temp;
             }

      /*  Read draw unselected options.  */
      else if (!strcmp(keyword, "drawunselnodes"))
             {
              fscanf(fp, "%d", &temp);
              drawunselnodes = (short)temp;
             }

      else if (!strcmp(keyword, "drawunselcells"))
             {
              fscanf(fp, "%d", &temp);
              drawunselcells = (short)temp;
             }

      /*  Read center on cell, node or tracer option.  */
      else if (!strcmp(keyword, "cencellid"))
             {
              fscanf(fp, "%d", &cencellid);
             }

      
      else if (!strcmp(keyword, "cennodeid"))
             {
              fscanf(fp, "%d", &cennodeid);
             }
      
      else if (!strcmp(keyword, "centracerid"))
             {
              fscanf(fp, "%d", &centracerid);
             }
      
      else if (!strcmp(keyword, "colorbartitleflag"))
             {
              fscanf(fp, "%hd", &colorbartitleflag);
             }
      
      else if (!strcmp(keyword, "clrbartitlestr"))
             {
              fscanf(fp, "%c", &tempchar);
	      temp = 0;
	      while (9)
	        {
	         fscanf(fp, "%c", &tempchar);
	         if (tempchar == '\n')
	            break;
	         clrbartitlestr[temp] = tempchar;
	         temp++;
	        }
	      clrbartitlestr[temp] = '\0';
             }
      
      else if (!strcmp(keyword, "distscale_x"))
             {
              fscanf(fp, "%d", &distscale_x);
             }
      
      else if (!strcmp(keyword, "distscale_y"))
             {
              fscanf(fp, "%d", &distscale_y);
             }
      
      else if (!strcmp(keyword, "orient_axisflag"))
             {
              fscanf(fp, "%hd", &o_axisflag);
             }
      
      else if (!strcmp(keyword, "orient_axis_x"))
             {
              fscanf(fp, "%d", &o_axis_x);
             }
      
      else if (!strcmp(keyword, "orient_axis_y"))
             {
              fscanf(fp, "%d", &o_axis_y);
             }

      else
        {
         fprintf(stderr, "Invalid Attributes Keyword - %s\n",keyword);
         gmvexit();
        }
     }
   fclose(fp);


   /*  Check if the window needs resizing.  */
   if (windowwidth_in > 0 || windowheight_in > 0)
   {
     if (windowwidth_in == -1)  windowwidth_in = windowwidth;
     if (windowheight_in == -1)  windowheight_in = windowheight;
     windowwidth = windowwidth_in;
     windowheight = windowheight_in;
     //TJ wrapped with if check because movieflag doesn't initialize windowxmax
     //   and windowymax correctly, and resize depends on motif/x11 calls,
     //   causing segfaults
     if( movieflag == 0 )
     {
       if (windowwidth > windowxmax)
         windowwidth = windowxmax;
       if (windowheight > windowymax)
         windowheight = windowymax;
       noresizeflag = 1;
       winresize();
     }
   }
}


void rmblanks(char inname[50],char charbuffer[50])
{
  int i, len, j;

   /*  Remove trailing blanks from inname.  */
   len = strlen(inname);
   i = len - 1;
   j = -1;
   while ( *(inname+i) == ' ' && i >= 0)
     {
      j = i;
      i--;
     }

   if (j == -1) j = len;
   for (i = 0; i < j; i++)
     {
      charbuffer[i] = inname[i];
     }
   charbuffer[j] = '\0';
}


void stripname(FILE * fp,char inname[50], char charbuffer[50])
{
  int i, len, j;
  char tstchar;

   /*  Remove the ^ first and last characters from inname.  */
   len = strlen(inname);
   if (inname[0] != '^')
     {

      /*  String does not need to be stripped.  */
      for (i = 0; i < len; i++)
         charbuffer[i] = inname[i];
      charbuffer[len] = '\0';
     }
   else if (inname[len-1] == '^')
     {

      /*  Copy everything between < ^s.  */
      for (i = 1; i < len-1; i++)
         charbuffer[i-1] = inname[i];
      charbuffer[len-2] = '\0';
     }
   else
     {
      
      /*  There must blanks between ^s., fill char buffer with  */ 
      /*  first part of the string, and look for trailing ^.    */
      for (i = 1; i < len; i++)
         charbuffer[i-1] = inname[i];
      j = len-1;
      fscanf(fp, "%c", &tstchar);
      while (tstchar != '^')
        {
         charbuffer[j] = tstchar;
         fscanf(fp, "%c", &tstchar);
         j++;
        }
      charbuffer[j] = '\0';
     }
}


int get_name_id(int n, char **names, char *name)
{
  int i, j, len1, len2;
  char smallname[50], tstname[50], tstname2[50];

   /*  Get the length of the name to check.  */
   len1 = strlen(name);
   strcpy(tstname,name);

   /*  Loop through the names and return position.  */
   j = -1;
   for (i = 0; i < n; i++)
     {

      /*  Get the length of the variable name.  */
      len2 = strlen(names[i]);

      /*  If string lengths are the same, compare.  */
      if (len1 == len2)
        {
         if (strcmp(names[i],tstname) == 0) j = i;
        }

      /*  Otherwise, check for trailing blanks.  */ 
      else
        {
         strcpy(tstname2,names[i]);

         /*  Remove trailing blanks from the variable name and test.  */
         rmblanks(tstname2,smallname);
         if (strcmp(smallname,tstname) == 0) j = i;
        }

      if (j > -1) break;
     }
   return j;
}


void getattributes_new(void)
{
  char keyword[50], old_key[50], tempchar;
  char junk[2048];
  FILE *fp;
  int temp, loop, number, number2, nopt, tmp1, tmp2, tmp3;
  int i, k;
  unsigned long unsigned_in;
  float *lmmat, lmcon[25];
  XmString string;

   fp = fopen(attribsfilnam, "r");
   if (fp == NULL)
     {
      if (!movieflag)
        {
         string = XmStringCreate("Error...cannot open attribute file!\n",
	                         XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
         XtManageChild(errorform);
         XmStringFree(string);
         return;
        }
      else
        {
         fprintf(stderr, "Error...cannot open attributes file!\n");
         gmvexit();
        }
     }

   cencellid = -1;  cennodeid = -1;  centracerid = -1;

   fscanf(fp, "%s", keyword);
   if (strcmp("gmv_attributes", keyword))
     {
      if (!movieflag)
        {
         string = XmStringCreate("Error...invalid GMV attributes file!",
	                         XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
         XtManageChild(errorform);
         XmStringFree(string);
         return;
        }
      else
        {
         fprintf(stderr, "Error...invalid GMV attributes file!\n");
         gmvexit();
        }
     }

   while (9)
     {

      /*  Read keyword, break at end of file.  */
      strcpy(old_key, keyword); /* this is for debugging */
      if (fscanf(fp, "%s", keyword) == EOF || 
          !strcmp(keyword, "end_attributes"))
         break;

      /*  Compare keyword, and read data.  */

      if (!strcmp(keyword, "matrgba"))
        {
         fscanf(fp, "%d", &number);
         scan_floats(matrgba, number, fp);
        }

      else if (!strcmp(keyword, "lmmat"))
        {
         fscanf(fp, "%d", &number);
         lmmat = (float *)malloc((number)*sizeof(float));
         scan_floats(lmmat, number, fp);
         k = number / 25;
         for (i = 0; i < k; i++)
           {
            matrgba[4*i+0] = lmmat[25*i+1]/.3;
            matrgba[4*i+1] = lmmat[25*i+2]/.3;
            matrgba[4*i+2] = lmmat[25*i+3]/.3;
            matrgba[4*i+3] = lmmat[25*i+15];
           }
         free(lmmat);
        }

      else if (!strcmp(keyword, "active_mat"))
             {
              fscanf(fp, "%d", &number);
              scan_shorts(active_mat, number, fp);
             }

      else if (!strcmp(keyword, "angles"))
              fscanf(fp, "%f %f %f", &xrot, &yrot, &zrot);

      else if (!strcmp(keyword, "magnify"))
              fscanf(fp, "%f", &magnify);

      else if (!strcmp(keyword, "range"))
              fscanf(fp, "%f %f %f", &xrange, &yrange, &zrange);

      else if (!strcmp(keyword, "span"))
              fscanf(fp, "%f", &span);

      else if (!strcmp(keyword, "aspect"))
              fscanf(fp, "%f", &aspect);

      else if (!strcmp(keyword, "pan"))
              fscanf(fp, "%f %f", &xpan, &ypan);

      else if (!strcmp(keyword, "translate"))
              fscanf(fp, "%f %f %f", &xtrans, &ytrans, &ztrans);

      else if (!strcmp(keyword, "glob_icmin"))
              fscanf(fp, "%lf %lf %lf", &globxicmin, &globyicmin, &globzicmin);

      else if (!strcmp(keyword, "glob_icmax"))
              fscanf(fp, "%lf %lf %lf", &globxicmax, &globyicmax, &globzicmax);

      else if (!strcmp(keyword, "explodefact"))
              fscanf(fp, "%f %f", &cexplodefact, &pexplodefact);

      else if (!strcmp(keyword, "velscale"))
              fscanf(fp, "%f", &velscale);

      else if (!strcmp(keyword, "velarrowsize"))
              fscanf(fp, "%f", &velarrowsize);

      else if (!strcmp(keyword, "axisflag"))
             {
              fscanf(fp, "%d", &temp);
              axisflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "nodesflag"))
             {
              fscanf(fp, "%d", &temp);
              nodesflag = (short)temp;
             }

      else if (!strcmp(keyword, "linesflag"))
             {
              fscanf(fp, "%d", &temp);
              linesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "polysflag"))
             {
              fscanf(fp, "%d", &temp);
              polysflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cellsflag"))
             {
              fscanf(fp, "%d", &temp);
              cellsflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "celledgesflag"))
             {
              fscanf(fp, "%d", &temp);
              celledgesflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cellhiddenedgesflag"))
             {
              fscanf(fp, "%d", &temp);
              cellhiddenedgesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "cellexpflag"))
             {
              fscanf(fp, "%d", &temp);
              cellexpflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "timeflag"))
             {
              fscanf(fp, "%d", &temp);
              timeflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cycleflag"))
             {
              fscanf(fp, "%d", &temp);
              cycleflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "distscaleflag"))
             {
              fscanf(fp, "%d", &temp);
              distscaleflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "textureflag"))
             {
              fscanf(fp, "%d", &temp);
              textureflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "tracerflag"))
             {
              fscanf(fp, "%d", &temp);
              tracerflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "nodefieldactive"))
              fscanf(fp, "%d", &nodefieldactive);
	      
      else if (!strcmp(keyword, "cellfieldactive"))
              fscanf(fp, "%d", &cellfieldactive);
	      
      else if (!strcmp(keyword, "tracefieldactive"))
              fscanf(fp, "%d", &tracefieldactive);
	      
      else if (!strcmp(keyword, "isosurf_on"))
             {
              fscanf(fp, "%d", &number);
	      scan_ints(contour_on, number, fp);
	     }

      else if (!strcmp(keyword, "isosurf_val"))
             {
              fscanf(fp, "%d", &number);
	      scan_floats(mconval, number, fp);
	     }
	     
      else if (!strcmp(keyword, "isosurf_field"))
             {
              fscanf(fp, "%d", &number);
	      scan_ints(mconfield, number, fp);
             }

      else if (!strcmp(keyword, "isovol"))
             {
              fscanf(fp,"%d %f %f %d", &temp,
                     &isovol_val1, &isovol_val2, &nopt);
              isovol_on = 1;
	      contour_on[MAXCONTOURS+MAXMATS+1] = 1;
	      for (loop = 0; loop < nopt; loop++)
	         {
		  fscanf(fp, "%d", &number);
		  switch(loop)
		    {
		     case 0: isovolfieldactive = number;
			     break;
		     case 1: ivcliponbox = (short) number;
			     break;
		     case 2: ivcliponcellsel = (short) number;
			     break;
		     /* add additional flags here */
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
	     }
	     
      else if (!strcmp(keyword, "backgroundcolor"))
             {
              fscanf(fp, "%lu", &unsigned_in);
              backgroundcolor = unsigned_in;
             }
	    
      else if (!strcmp(keyword, "globfieldmin"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldmin, number, fp);
              for (i = 0; i < number; i++)
                 nfld_nice_min[i] = globfieldmin[i];
             }
	    
      else if (!strcmp(keyword, "globfieldmax"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldmax, number, fp);
              for (i = 0; i < number; i++)
                 nfld_nice_max[i] = globfieldmax[i];
             }
	    
      else if (!strcmp(keyword, "nfld_nice_min"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(nfld_nice_min, number, fp);
             }
	    
      else if (!strcmp(keyword, "nfld_nice_max"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(nfld_nice_max, number, fp);
             }
	    
      else if (!strcmp(keyword, "nfieldlimauto"))
             {
              fscanf(fp, "%d", &number);
	      scan_shorts(nfieldlimauto, number, fp);
             }
	      
      else if (!strcmp(keyword, "nfld_nice"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(nfld_nice, number, fp);
             }
	      
      else if (!strcmp(keyword, "nfld_log"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(nfld_log, number, fp);
             }
	      
      else if (!strcmp(keyword, "globfieldtrmin"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldtrmin, number, fp);
              for (i = 0; i < number; i++)
                 trfld_nice_min[i] = globfieldtrmin[i];
             }
	     
      else if (!strcmp(keyword, "globfieldtrmax"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldtrmax, number, fp);
              for (i = 0; i < number; i++)
                 trfld_nice_max[i] = globfieldtrmax[i];
             }
	    
      else if (!strcmp(keyword, "trfld_nice_min"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(trfld_nice_min, number, fp);
             }
	    
      else if (!strcmp(keyword, "trfld_nice_max"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(trfld_nice_max, number, fp);
             }
	      
      else if (!strcmp(keyword, "trfld_nice"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(trfld_nice, number, fp);
             }
	      
      else if (!strcmp(keyword, "trfld_log"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(trfld_log, number, fp);
             }
	    
      else if (!strcmp(keyword, "trfieldlimauto"))
             {
              fscanf(fp, "%d", &number);
	      scan_shorts(trfieldlimauto, number, fp);
             }
	     
      else if (!strcmp(keyword, "light"))
              fscanf(fp, "%f %f", &lightx, &lighty);
	     
      else if (!strcmp(keyword, "glob_icmino"))
              fscanf(fp, "%lf %lf %lf", 
                     &globxicmino, &globyicmino, &globzicmino);
	      
      else if (!strcmp(keyword, "glob_icmaxo"))
              fscanf(fp, "%lf %lf %lf", 
                     &globxicmaxo, &globyicmaxo, &globzicmaxo);
	     
      else if (!strcmp(keyword, "xscale"))
              scan_floats(xscale, 8, fp);
	     
      else if (!strcmp(keyword, "yscale"))
              scan_floats(yscale, 8, fp);
	    
      else if (!strcmp(keyword, "zscale"))
              scan_floats(zscale, 8, fp);
	      
      else if (!strcmp(keyword, "nrefl_loops"))
              fscanf(fp, "%d", &nrefl_loops);
	      
      else if (!strcmp(keyword, "reflect"))
              fscanf(fp, "%d %d %d", &xreflect, &yreflect, &zreflect);
	      
      else if (!strcmp(keyword, "conrgba"))
             {
              fscanf(fp, "%d", &number);
	      scan_floats(&(conrgba[4*number]), 4, fp);
             }
	      
      else if (!strcmp(keyword, "lmcon"))
             {
              fscanf(fp, "%d", &number);
	      scan_floats(lmcon, 25, fp);
              conrgba[4*number+0] = lmcon[1]/.3;
              conrgba[4*number+1] = lmcon[2]/.3;
              conrgba[4*number+2] = lmcon[3]/.3;
              conrgba[4*number+3] = lmcon[15];
             }
	    
      else if (!strcmp(keyword, "cliponbox"))
             {
              fscanf(fp, "%d", &temp);
              cliponbox = (short)temp;
             }
	    
      else if (!strcmp(keyword, "cliponcellsel"))
             {
              fscanf(fp, "%d", &temp);
              cliponcellsel = (short)temp;
             }
	     
      else if (!strcmp(keyword, "clipvalue"))
              fscanf(fp, "%f %f", &frontclipvalue, &backclipvalue);
	      
      else if (!strcmp(keyword, "_minbdval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbdval, &yminbdval, &zminbdval);
	     
      else if (!strcmp(keyword, "_maxbdval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbdval, &ymaxbdval, &zmaxbdval);
	      
      else if (!strcmp(keyword, "_minfdval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminfdval, &yminfdval, &zminfdval);
	      
      else if (!strcmp(keyword, "_maxfdval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxfdval, &ymaxfdval, &zmaxfdval);
	    
      else if (!strcmp(keyword, "scaleaxiso"))
              fscanf(fp, "%f %f %f", 
                     &xscaleaxiso, &yscaleaxiso, &zscaleaxiso);
	     
      else if (!strcmp(keyword, "scaleaxis"))
              fscanf(fp, "%f %f %f", 
                     &xscaleaxis, &yscaleaxis, &zscaleaxis);
	      
      else if (!strcmp(keyword, "mat_order"))
             {
              fscanf(fp, "%d", &nmat_order);
	      scan_ints(mat_order, nmat_order, fp);
             }
	      
      else if (!strcmp(keyword, "nodeselflag"))
             {
              fscanf(fp, "%d %d", &number, &number2);
	      for (i = 0; i < number; i++)
	          scan_chars(nodeselflag[i], number2, fp);
             }
	     
      else if (!strcmp(keyword, "nodeandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(nodeandorflag, number, fp);
/*
              if (numflags_node > 0)
                {
                 if (nodeandorflag[numflags_node] == 0)
                    nodeselfld_on = 1;
                }
*/
             }
	     
      else if (!strcmp(keyword, "cellselflag"))
             {
              fscanf(fp, "%d %d", &number, &number2);
	      for (i = 0; i < number; i++)
	          scan_chars(cellselflag[i], number2, fp);
             }
	    
      else if (!strcmp(keyword, "cellandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(cellandorflag, number, fp);
/*
              if (numflags_cell > 0)
                {
                 if (cellandorflag[numflags_cell] == 0)
                    cellselcfld_on = 1;
                }
*/
             }
	    
      else if (!strcmp(keyword, "colorbarflag"))
             {
              fscanf(fp, "%d", &temp);
              colorbarflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "colorbarpref"))
             {
              fscanf(fp, "%d", &temp);
              colorbarpref = (short)temp;
             }
	     
      else if (!strcmp(keyword, "boundingboxflag"))
             {
              fscanf(fp, "%d", &temp);
              boundingboxflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "_minbbval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbbval, &yminbbval, &zminbbval);
	      
      else if (!strcmp(keyword, "_maxbbval"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbbval, &ymaxbbval, &zmaxbbval);
	     
      else if (!strcmp(keyword, "_minbbvalo"))
              fscanf(fp, "%lf %lf %lf", 
                     &xminbbvalo, &yminbbvalo, &zminbbvalo);
	        
      else if (!strcmp(keyword, "_maxbbvalo"))
              fscanf(fp, "%lf %lf %lf", 
                     &xmaxbbvalo, &ymaxbbvalo, &zmaxbbvalo);
	          
      else if (!strcmp(keyword, "viewflag"))
             {
              fscanf(fp, "%d", &temp);
              viewflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "flypos"))
              scan_floats(flypos, 3, fp);
	      
      else if (!strcmp(keyword, "vectfld_"))
             {
              fscanf(fp, "%d %d %d", &tmp1, &tmp2, &tmp3);
              vectfldx = tmp1;
              vectfldy = tmp2;
              vectfldz = tmp3;
             }
	   
      else if (!strcmp(keyword, "speedfld"))
             {
              fscanf(fp, "%d", &temp);
              speedfld = (short)temp;
             }
	     
      else if (!strcmp(keyword, "vectorflag"))
             {
              fscanf(fp, "%d", &temp);
              vectorflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "vectorarrow"))
             {
              fscanf(fp, "%d", &temp);
              vectorarrow = (short)temp;
             }
	     
      else if (!strcmp(keyword, "vectorlog"))
             {
              fscanf(fp, "%d", &temp);
              vectorlog = (short)temp;
             }
	      
      else if (!strcmp(keyword, "cvectfld_"))
             {
              fscanf(fp, "%d %d %d", &tmp1, &tmp2, &tmp3);
              cvectfldx = tmp1;
              cvectfldy = tmp2;
              cvectfldz = tmp3;
             }
	   
      else if (!strcmp(keyword, "cspeedfld"))
             {
              fscanf(fp, "%d", &temp);
              cspeedfld = (short)temp;
             }
	     
      else if (!strcmp(keyword, "cvectorflag"))
             {
              fscanf(fp, "%d", &temp);
              cvectorflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "titleflag"))
             {
              fscanf(fp, "%d", &temp);
              titleflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "maxspeed"))
              fscanf(fp, "%lf", &maxspeed);
	      
      else if (!strcmp(keyword, "cmaxspeed"))
              fscanf(fp, "%lf", &cmaxspeed);
	          
      else if (!strcmp(keyword, "titlestr"))
             {
              fscanf(fp, "%c", &tempchar);
	      temp = 0;
	      while (9)
	        {
	         fscanf(fp, "%c", &tempchar);
	         if (tempchar == '\n')
	            break;
	         titlestr[temp] = tempchar;
	         temp++;
	        }
	      titlestr[temp] = '\0';
             }
	          
      else if (!strcmp(keyword, "titlestr2"))
             {
              fscanf(fp, "%c", &tempchar);
	      temp = 0;
	      while (9)
	        {
	         fscanf(fp, "%c", &tempchar);
	         if (tempchar == '\n')
	            break;
	         titlestr2[temp] = tempchar;
	         temp++;
	        }
	      titlestr2[temp] = '\0';
             }
	     
      else if (!strcmp(keyword, "cellcontflag"))
             {
              fscanf(fp, "%d", &temp);
              cellcontflag = (short)temp;
             }
	        
      else if (!strcmp(keyword, "fieldcalc"))
             {
              fscanf(fp, "%d", &number);
	      scan_struct_fldcalc(fieldcalc, number, fp);
             }
     
      else if (!strcmp(keyword, "nodeselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "nodeselfld_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselfield"))
             {
              fscanf(fp, "%d", &temp);
              nodeselfield = (short)temp;
             }
	
      else if (!strcmp(keyword, "nfldselmin"))
              fscanf(fp, "%lf", &nfldselmin);
	
      else if (!strcmp(keyword, "nfldselmax"))
              fscanf(fp, "%lf", &nfldselmax);
     
      else if (!strcmp(keyword, "nodeselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "nodeselsph_x"))
              fscanf(fp, "%f", &nodeselsph_x);
	
      else if (!strcmp(keyword, "nodeselsph_y"))
              fscanf(fp, "%f", &nodeselsph_y);
	
      else if (!strcmp(keyword, "nodeselsph_z"))
              fscanf(fp, "%f", &nodeselsph_z);
	
      else if (!strcmp(keyword, "nodeselsph_r"))
              fscanf(fp, "%f", &nodeselsph_r);
     
      else if (!strcmp(keyword, "nodeselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "nodeselbox_xmin"))
              fscanf(fp, "%f", &nodeselbox_xmin);
	
      else if (!strcmp(keyword, "nodeselbox_xmax"))
              fscanf(fp, "%f", &nodeselbox_xmax);
	
      else if (!strcmp(keyword, "nodeselbox_ymin"))
              fscanf(fp, "%f", &nodeselbox_ymin);
	
      else if (!strcmp(keyword, "nodeselbox_ymax"))
              fscanf(fp, "%f", &nodeselbox_ymax);
	
      else if (!strcmp(keyword, "nodeselbox_zmin"))
              fscanf(fp, "%f", &nodeselbox_zmin);
	
      else if (!strcmp(keyword, "nodeselbox_zmax"))
              fscanf(fp, "%f", &nodeselbox_zmax);
     
      else if (!strcmp(keyword, "nodeselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              nodeselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "nnodeselnum"))
             {
              fscanf(fp, "%d", &temp);
              nnodeselnum = (short)temp;
              for (i = 0; i < nnodeselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &nodeselnum_f[i],
                       &nodeselnum_l[i], &nodeselnum_s[i]);
                }
             }
	  
      else if (!strcmp(keyword, "cellselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "cellselcfld_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselcfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselfield"))
             {
              fscanf(fp, "%d", &temp);
              cellselfield = (short)temp;
             }
	
      else if (!strcmp(keyword, "cfldselmin"))
              fscanf(fp, "%lf", &cfldselmin);
	
      else if (!strcmp(keyword, "cfldselmax"))
              fscanf(fp, "%lf", &cfldselmax);
     
      else if (!strcmp(keyword, "cellselnfld_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselnfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselnfield"))
             {
              fscanf(fp, "%d", &temp);
              cellselnfield = (short)temp;
             }

      else if (!strcmp(keyword, "cellselnfldopt"))
             {
              fscanf(fp, "%d", &temp);
              cellselnfldopt = (short)temp;
             }
	
      else if (!strcmp(keyword, "cnfldselmin"))
              fscanf(fp, "%lf", &cnfldselmin);
	
      else if (!strcmp(keyword, "cnfldselmax"))
              fscanf(fp, "%lf", &cnfldselmax);
     
      else if (!strcmp(keyword, "cellselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellselsph_x"))
              fscanf(fp, "%f", &cellselsph_x);
	
      else if (!strcmp(keyword, "cellselsph_y"))
              fscanf(fp, "%f", &cellselsph_y);
	
      else if (!strcmp(keyword, "cellselsph_z"))
              fscanf(fp, "%f", &cellselsph_z);
	
      else if (!strcmp(keyword, "cellselsph_r"))
              fscanf(fp, "%f", &cellselsph_r);
     
      else if (!strcmp(keyword, "cellselsph_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselsph_in = (short)temp;
             }
     
      else if (!strcmp(keyword, "cellselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellselbox_xmin"))
              fscanf(fp, "%f", &cellselbox_xmin);
	
      else if (!strcmp(keyword, "cellselbox_xmax"))
              fscanf(fp, "%f", &cellselbox_xmax);
	
      else if (!strcmp(keyword, "cellselbox_ymin"))
              fscanf(fp, "%f", &cellselbox_ymin);
	
      else if (!strcmp(keyword, "cellselbox_ymax"))
              fscanf(fp, "%f", &cellselbox_ymax);
	
      else if (!strcmp(keyword, "cellselbox_zmin"))
              fscanf(fp, "%f", &cellselbox_zmin);
	
      else if (!strcmp(keyword, "cellselbox_zmax"))
              fscanf(fp, "%f", &cellselbox_zmax);
     
      else if (!strcmp(keyword, "cellselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "cellselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "ncellselnum"))
             {
              fscanf(fp, "%d", &temp);
              ncellselnum = (short)temp;
              for (i = 0; i < ncellselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &cellselnum_f[i],
                        &cellselnum_l[i],&cellselnum_s[i]);
                }
             }

      else if (!strcmp(keyword, "traceselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              traceselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "ntraceselnum"))
             {
              fscanf(fp, "%d", &temp);
              ntraceselnum = (short)temp;
              for (i = 0; i < ntraceselnum; i++)
                {
                 fscanf(fp,"%d %d %d", &traceselnum_f[i],
                        &traceselnum_l[i],&traceselnum_s[i]);
                }
             }
	
      else if (!strcmp(keyword, "cnfldselmin"))
              fscanf(fp, "%lf", &cnfldselmin);
	
      else if (!strcmp(keyword, "cnfldselmax"))
              fscanf(fp, "%lf", &cnfldselmax);
	
      else if (!strcmp(keyword, "cfieldcalc"))
             {
              fscanf(fp, "%d", &number);
              scan_struct_fldcalc(cfieldcalc, number, fp);
             }
	
      else if (!strcmp(keyword, "bboxcoordflag"))
             {
              fscanf(fp, "%d", &temp);
              bboxcoordflag = (short)temp;
             }

      else if (!strcmp(keyword, "nodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              nodenumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "cellnumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfnumflag"))
             {
              fscanf(fp, "%d", &temp);
              surfnumflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "globfieldcmax"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldcmax, number, fp);
              for (i = 0; i < number; i++)
                 cfld_nice_max[i] = globfieldcmax[i];
             }
	
      else if (!strcmp(keyword, "globfieldcmin"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldcmin, number, fp);
              for (i = 0; i < number; i++)
                 cfld_nice_min[i] = globfieldcmin[i];
             }
	
      else if (!strcmp(keyword, "cfld_nice_min"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(cfld_nice_min, number, fp);
             }
	
      else if (!strcmp(keyword, "cfld_nice_max"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(cfld_nice_max, number, fp);
             }
	    
      else if (!strcmp(keyword, "cfieldlimauto"))
             {
              fscanf(fp, "%d", &number);
	      scan_shorts(cfieldlimauto, number, fp);
             }
	      
      else if (!strcmp(keyword, "cfld_nice"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(cfld_nice, number, fp);
             }
	      
      else if (!strcmp(keyword, "cfld_log"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(cfld_log, number, fp);
             }
	
      else if (!strcmp(keyword, "filename"))
              fscanf(fp, "%s", junk);
	
      else if (!strcmp(keyword, "cutplane"))
             {
              fscanf(fp, "%d", &i);
              cutplane_on[i] = 1;
	      fscanf(fp, "%d %d", &cutplane[i].npolys, &number);
              if (i < MAXCUTPLANES)
                 cutplanefieldactive = number;
              else
                 cutspherefieldactive = number;
	      fscanf(fp, "%f %f %f %f", &cutplane[i].aa, &cutplane[i].bb, 
                     &cutplane[i].cc, &cutplane[i].dd);
	      fscanf(fp, "%f %f %f", &cutplane[i].x1, &cutplane[i].x2, 
                     &cutplane[i].x3);
	      fscanf(fp, "%f %f %f", &cutplane[i].y1, &cutplane[i].y2, 
                     &cutplane[i].y3);
	      fscanf(fp, "%f %f %f", &cutplane[i].z1, &cutplane[i].z2, 
                     &cutplane[i].z3);
	      scan_floats(cutplane[i].norm, 3, fp);
	      fscanf(fp, "%d", &number);
	      for (loop = 0; loop < number; loop++)
	         {
		  switch(loop)
		    {
		     case 0: fscanf(fp, "%d", &temp);
                             cutplane[i].heightflag = (short)temp;
			     break;
		     case 1: fscanf(fp, "%e", &cutplane[i].heightfac);
			     break;
		     case 2: fscanf(fp, "%e", &cutplane[i].dist);
			     break;
		     case 3: fscanf(fp, "%d", &temp);
                             cutplane[i].edgesflag = (short)temp;
			     break;
		     case 4: fscanf(fp, "%d", &temp);
                             cutplane[i].velflag = (short)temp;
			     break;
		     case 5: fscanf(fp, "%d", &temp);
                             cutplane[i].nodeflag = (short)temp;
			     break;
		     case 6: fscanf(fp, "%d", &temp);
                             cutplane[i].cliponbox = (short)temp;
			     break;
		     case 7: fscanf(fp, "%d", &temp);
                             cutplane[i].cliponcellsel = (short)temp;
			     break;
		     case 8: fscanf(fp, "%d", &temp);
                             cutplane[i].linesflag = (short)temp;
			     break;
		     case 9: fscanf(fp, "%d", &temp);
                             cutplane[i].facesflag = (short)temp;
			     break;
		     /* add additional flags here */
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
             }
	 
      else if (!strcmp(keyword, "cutline"))
             {
              fscanf(fp, "%d", &temp);
	      cutlineon[temp] = 1;
	      fscanf(fp, "%d %f", &cutlinefield[temp], 
                     &cutlineradius[temp]);
	      fscanf(fp, "%f %f %f", &ctlinx1[temp], &ctliny1[temp],
		     &ctlinz1[temp]);
	      fscanf(fp, "%f %f %f", &ctlinx2[temp], &ctliny2[temp],
		     &ctlinz2[temp]);
	      fscanf(fp, "%d", &number);
	      for (loop = 0; loop < number; loop++)
		 {
		  switch(loop)
		    {
		     case 0: fscanf(fp, "%d", &number2);
			     cutlinenodeson[temp] = (short) number2;
			     break;
		     case 1: fscanf(fp, "%d", &number2);
			     cutlinenumson[temp] = (short) number2;
			     break;
		     case 2: fscanf(fp, "%d", &number2);
			     waveflag[temp] = (short) number2;
			     break;
		     case 3: fscanf(fp, "%f", &wavefac[temp]);
			     break;
		     default: fprintf(stderr, "This doesn't happen!\n");
		    }
		 }
             }
	
      else if (!strcmp(keyword, "pnt_size"))
             {
              fscanf(fp, "%d %d", &tmp1, &tmp2);
              pnt_size = (short) tmp1;
              pnt_shape = (short) tmp2;
             }
	
      else if (!strcmp(keyword, "line_size"))
             {
              fscanf(fp, "%d %d", &tmp1, &tmp2);
              line_size = (short) tmp1;
              line_type = (short) tmp2;
             }

      else if (!strcmp(keyword, "node0matflag"))
             {
              fscanf(fp, "%d", &temp);
              node0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "cell0matflag"))
             {
              fscanf(fp, "%d", &temp);
              cell0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "surf0matflag"))
             {
              fscanf(fp, "%d", &temp);
              surf0matflag = (short)temp;
             }

      else if (!strcmp(keyword, "tracehistflag"))
             {
              fscanf(fp, "%d", &temp);
              tracehistflag = (short)temp;
             }

      else if (!strcmp(keyword, "tracehiststop"))
             {
              fscanf(fp, "%d", &temp);
              tracehiststop = (short)temp;
             }

      else if (!strcmp(keyword, "tracehiststride"))
             {
              fscanf(fp, "%d", &temp);
              tracehiststride = (short)temp;
             }

      else if (!strcmp(keyword, "tracehistpoints"))
             {
              fscanf(fp, "%d", &temp);
              tracehistpoints = (short)temp;
             }

      else if (!strcmp(keyword, "cellshadeflag"))
             {
              fscanf(fp, "%d", &temp);
              cellshadeflag = (short)temp;
             }

      else if (!strcmp(keyword, "isovolfield_draw"))
              fscanf(fp, "%d", &isovolfield_draw);

      else if (!strcmp(keyword, "contourfield_draw"))
              fscanf(fp, "%d", &contourfield_draw);

      else if (!strcmp(keyword, "mcontourfield_draw"))
              fscanf(fp, "%d", &mcontourfield_draw);
	     
      else if (!strcmp(keyword, "fieldcolortype"))
             {
              fscanf(fp, "%d", &temp);
              fieldcolortype = (short)temp;
             }

      else if (!strcmp(keyword, "jcolor"))
             {
              scan_uints(jcolor, 256, fp);
              resettexture();
             }

      else if (!strcmp(keyword, "nset_contours"))
              fscanf(fp, "%d", &nset_contours);

      else if (!strcmp(keyword, "facerefineflag"))
             {
              fscanf(fp, "%d", &temp);
              facerefineflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellnormflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnormflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellmedianedgeflag"))
             {
              fscanf(fp, "%d", &temp);
              cellmedianedgeflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellnodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellnodenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "cellfacenumflag"))
             {
              fscanf(fp, "%d", &temp);
              cellfacenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "polystiplflag"))
             {
              fscanf(fp, "%d", &temp);
              polystiplflag = (short)temp;
             }

      /*  Surface attributes follow.  */

      else if (!strcmp(keyword, "surfflag"))
             {
              fscanf(fp, "%d", &temp);
              surfflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfedgesflag"))
             {
              fscanf(fp, "%d", &temp);
              surfedgesflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfexpflag"))
             {
              fscanf(fp, "%d", &temp);
              surfexpflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "surffieldactive"))
              fscanf(fp, "%d", &surffieldactive);

      else if (!strcmp(keyword, "surfshadeflag"))
             {
              fscanf(fp, "%d", &temp);
              surfshadeflag = (short)temp;
             }

      else if (!strcmp(keyword, "surfnodenumflag"))
             {
              fscanf(fp, "%d", &temp);
              surfnodenumflag = (short)temp;
             }

      else if (!strcmp(keyword, "surfrefineflag"))
             {
              fscanf(fp, "%d", &temp);
              surfrefineflag = (short)temp;
             }
	     
      else if (!strcmp(keyword, "surfcontflag"))
             {
              fscanf(fp, "%d", &temp);
              surfcontflag = (short)temp;
             }

      else if (!strcmp(keyword, "surf0matflag"))
             {
              fscanf(fp, "%d", &temp);
              surf0matflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "svectfld_"))
             {
              fscanf(fp, "%d %d %d", &tmp1, &tmp2, &tmp3);
              svectfldx = tmp1;
              svectfldy = tmp2;
              svectfldz = tmp3;
             }
	   
      else if (!strcmp(keyword, "sspeedfld"))
             {
              fscanf(fp, "%d", &temp);
              sspeedfld = (short)temp;
             }
	      
      else if (!strcmp(keyword, "smaxspeed"))
              fscanf(fp, "%lf", &smaxspeed);
	     
      else if (!strcmp(keyword, "svectorflag"))
             {
              fscanf(fp, "%d", &temp);
              svectorflag = (short)temp;
             }
	
      else if (!strcmp(keyword, "globfieldsmax"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldsmax, number, fp);
              for (i = 0; i < number; i++)
                 sfld_nice_max[i] = globfieldsmax[i];
             }
	
      else if (!strcmp(keyword, "globfieldsmin"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldsmin, number, fp);
              for (i = 0; i < number; i++)
                 sfld_nice_min[i] = globfieldsmin[i];
             }
	
      else if (!strcmp(keyword, "sfld_nice_min"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(sfld_nice_min, number, fp);
             }
	
      else if (!strcmp(keyword, "sfld_nice_max"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(sfld_nice_max, number, fp);
             }
	    
      else if (!strcmp(keyword, "sfieldlimauto"))
             {
              fscanf(fp, "%d", &number);
	      scan_shorts(sfieldlimauto, number, fp);
             }
	      
      else if (!strcmp(keyword, "sfld_nice"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(sfld_nice, number, fp);
             }
	      
      else if (!strcmp(keyword, "sfld_log"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(sfld_log, number, fp);
             }
	
      else if (!strcmp(keyword, "sfieldcalc"))
             {
              fscanf(fp, "%d", &number);
              scan_struct_fldcalc(sfieldcalc, number, fp);
             }
	     
      else if (!strcmp(keyword, "surfselflag"))
             {
              fscanf(fp, "%d %d", &number, &number2);
	      for (i = 0; i < number; i++)
	          scan_chars(surfselflag[i], number2, fp);
             }
	    
      else if (!strcmp(keyword, "surfandorflag"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(surfandorflag, number, fp);
/*
              if (numflags_surf > 0)
                {
                 if (surfandorflag[numflags_surf] == 0)
                    surfselsfld_on = 1;
                }
*/
             }
	  
      else if (!strcmp(keyword, "surfselmat_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselmat_on = (short)temp;
             }
     
      else if (!strcmp(keyword, "surfselsfld_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselfield"))
             {
              fscanf(fp, "%d", &temp);
              surfselfield = (short)temp;
             }
	
      else if (!strcmp(keyword, "sfldselmin"))
              fscanf(fp, "%lf", &sfldselmin);
	
      else if (!strcmp(keyword, "sfldselmax"))
              fscanf(fp, "%lf", &sfldselmax);
     
      else if (!strcmp(keyword, "surfselnfld_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselnfld_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselnfield"))
             {
              fscanf(fp, "%d", &temp);
              surfselnfield = (short)temp;
             }

      else if (!strcmp(keyword, "surfselnfldopt"))
             {
              fscanf(fp, "%d", &temp);
              surfselnfldopt = (short)temp;
             }
	
      else if (!strcmp(keyword, "snfldselmin"))
              fscanf(fp, "%lf", &snfldselmin);
	
      else if (!strcmp(keyword, "snfldselmax"))
              fscanf(fp, "%lf", &snfldselmax);
     
      else if (!strcmp(keyword, "surfselsph_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsph_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfselsph_x"))
              fscanf(fp, "%f", &surfselsph_x);
	
      else if (!strcmp(keyword, "surfselsph_y"))
              fscanf(fp, "%f", &surfselsph_y);
	
      else if (!strcmp(keyword, "surfselsph_z"))
              fscanf(fp, "%f", &surfselsph_z);
	
      else if (!strcmp(keyword, "surfselsph_r"))
              fscanf(fp, "%f", &surfselsph_r);
     
      else if (!strcmp(keyword, "surfselsph_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselsph_in = (short)temp;
             }
     
      else if (!strcmp(keyword, "surfselbox_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselbox_on = (short)temp;
             }
	
      else if (!strcmp(keyword, "surfselbox_xmin"))
              fscanf(fp, "%f", &surfselbox_xmin);
	
      else if (!strcmp(keyword, "surfselbox_xmax"))
              fscanf(fp, "%f", &surfselbox_xmax);
	
      else if (!strcmp(keyword, "surfselbox_ymin"))
              fscanf(fp, "%f", &surfselbox_ymin);
	
      else if (!strcmp(keyword, "surfselbox_ymax"))
              fscanf(fp, "%f", &surfselbox_ymax);
	
      else if (!strcmp(keyword, "surfselbox_zmin"))
              fscanf(fp, "%f", &surfselbox_zmin);
	
      else if (!strcmp(keyword, "surfselbox_zmax"))
              fscanf(fp, "%f", &surfselbox_zmax);
     
      else if (!strcmp(keyword, "surfselbox_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselbox_in = (short)temp;
             }

      else if (!strcmp(keyword, "surfselnum_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselnum_on = (short)temp;
             }

      else if (!strcmp(keyword, "nsurfselnum"))
             {
              fscanf(fp, "%d", &temp);
              nsurfselnum = (short)temp;
              for (i = 0; i < nsurfselnum; i++)
                {
                 fscanf(fp,"%ld %ld %ld", &surfselnum_f[i],
                        &surfselnum_l[i],&surfselnum_s[i]);
                }
             }
	
      else if (!strcmp(keyword, "globfieldfmax"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldfmax, number, fp);
              for (i = 0; i < number; i++)
                 ffld_nice_max[i] = globfieldfmax[i];
             }
	
      else if (!strcmp(keyword, "globfieldfmin"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(globfieldfmin, number, fp);
              for (i = 0; i < number; i++)
                 ffld_nice_min[i] = globfieldfmin[i];
             }
	
      else if (!strcmp(keyword, "ffld_nice_min"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(ffld_nice_min, number, fp);
             }
	
      else if (!strcmp(keyword, "ffld_nice_max"))
             {
              fscanf(fp, "%d", &number);
	      scan_doubles(ffld_nice_max, number, fp);
             }
	    
      else if (!strcmp(keyword, "ffieldlimauto"))
             {
              fscanf(fp, "%d", &number);
	      scan_shorts(ffieldlimauto, number, fp);
             }
	      
      else if (!strcmp(keyword, "ffld_nice"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(ffld_nice, number, fp);
             }
	      
      else if (!strcmp(keyword, "ffld_log"))
             {
              fscanf(fp, "%d", &number);
	      scan_chars(ffld_log, number, fp);
             }
	      
      else if (!strcmp(keyword, "fvectfld_"))
             {
              fscanf(fp, "%d %d %d", &tmp1, &tmp2, &tmp3);
              fvectfldx = tmp1;
              fvectfldy = tmp2;
              fvectfldz = tmp3;
             }
	   
      else if (!strcmp(keyword, "fspeedfld"))
             {
              fscanf(fp, "%d", &temp);
              fspeedfld = (short)temp;
             }
	     
      else if (!strcmp(keyword, "fvectorflag"))
             {
              fscanf(fp, "%d", &temp);
              fvectorflag = (short)temp;
             }
	      
      else if (!strcmp(keyword, "fmaxspeed"))
              fscanf(fp, "%lf", &fmaxspeed);

      else if (!strcmp(keyword, "nodeselnodegrp_on"))
             {
              fscanf(fp, "%d", &temp);
              nodeselnodegrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "nodeselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              nodeselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nnodegrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nnodegrpselected = (int)temp;
              for (i = 0; i < nnodegrpselected; i++)
                {
                 fscanf(fp,"%d", &nodegrpselected[i]);
                }
             }

      else if (!strcmp(keyword, "cellselcellgrp_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselcellgrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "cellselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              cellselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "ncellgrpselected"))
             {
              fscanf(fp, "%d", &temp);
              ncellgrpselected = (int)temp;
              for (i = 0; i < ncellgrpselected; i++)
                {
                 fscanf(fp,"%d", &cellgrpselected[i]);
                }
             }

      else if (!strcmp(keyword, "cellselfacegrp_on"))
             {
              fscanf(fp, "%d", &temp);
              cellselfacegrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "faceselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              faceselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nfacegrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nfacegrpselected = (int)temp;
              for (i = 0; i < nfacegrpselected; i++)
                {
                 fscanf(fp,"%d", &facegrpselected[i]);
                }
             }

      else if (!strcmp(keyword, "surfselsurfgrp_on"))
             {
              fscanf(fp, "%d", &temp);
              surfselsurfgrp_on = (short)temp;
             }

      else if (!strcmp(keyword, "surfselgrp_in"))
             {
              fscanf(fp, "%d", &temp);
              surfselgrp_in = (short)temp;
             }

      else if (!strcmp(keyword, "nsurfgrpselected"))
             {
              fscanf(fp, "%d", &temp);
              nsurfgrpselected = (int)temp;
              for (i = 0; i < nsurfgrpselected; i++)
                {
                 fscanf(fp,"%d", &surfgrpselected[i]);
                }
             }

      else if (!strcmp(keyword, "drawghosts"))
             {
              fscanf(fp, "%d", &temp);
              drawghosts = (short)temp;
             }

      else if (!strcmp(keyword, "drawghostface"))
             {
              fscanf(fp, "%d", &temp);
              drawghostface = (short)temp;
             }

      else if (!strcmp(keyword, "drawnodeghosts"))
             {
              fscanf(fp, "%d", &temp);
              drawnodeghosts = (short)temp;
             }

      else if (!strcmp(keyword, "drawunselnodes"))
             {
              fscanf(fp, "%d", &temp);
              drawunselnodes = (short)temp;
             }

      else if (!strcmp(keyword, "drawunselcells"))
             {
              fscanf(fp, "%d", &temp);
              drawunselcells = (short)temp;
             }

      else if (!strcmp(keyword, "cencellid"))
             {
              fscanf(fp, "%d", &cencellid);
             }

      
      else if (!strcmp(keyword, "cennodeid"))
             {
              fscanf(fp, "%d", &cennodeid);
             }
      
      else if (!strcmp(keyword, "centracerid"))
             {
              fscanf(fp, "%d", &centracerid);
             }

      else
        {
         fprintf(stderr, "Invalid Attributes Keyword - %s\n",keyword);
	 gmvexit();
        }
     }
   fclose(fp);
}


void scan_floats(float *data, int number, FILE *fp)
{
  int i;
  char junk[50];

   for (i = 0; i < number; i++)
     {
      if (!fscanf(fp, "%f", &data[i]))
        {
         fscanf(fp, "%s", junk);
         if (!strcmp(junk, "inf") || !strcmp(junk, "Inf"))
            data[i] = 9.99999e37;
         else if (!strcmp(junk, "nan") || !strcmp(junk, "NaN"))
            data[i] = 0.0;
         else
           {
            fprintf(stderr, 
                    "Error...unrecognized symbol in attribute file!\n");
            gmvexit();
           }
         data[i] = 9.99999e37;
        }
     }
}


void scan_doubles(double *data, int number, FILE *fp)
{
  int i;
  char junk[50];

   for (i = 0; i < number; i++)
     {
      if (!fscanf(fp, "%lf", &data[i]))
        {
         fscanf(fp, "%s", junk);
         if (!strcmp(junk, "inf") || !strcmp(junk, "Inf"))
            data[i] = 9.99999e37;
         else if (!strcmp(junk, "nan") || !strcmp(junk, "NaN"))
            data[i] = 0.0;
         else
           {
            fprintf(stderr, 
                    "Error...unrecognized symbol in attribute file!\n");
            gmvexit();
           }
         data[i] = 9.99999e37;
        }
     }
}


void scan_ints(int *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
      fscanf(fp, "%d", &data[i]);
}


void scan_uints(unsigned long *data, int number, FILE *fp)
{
  int i;

   for (i = 0; i < number; i++)
      fscanf(fp, "%lu", &data[i]);
}


void scan_shorts(short *data, int number, FILE *fp)
{
  int i, temp;

   for (i = 0; i < number; i++)
     {
      fscanf(fp, "%d", &temp);
      data[i] = (short) temp;
     }
}


void scan_chars(char *data, int number, FILE *fp)
{
  int i, temp;

   for (i = 0; i < number; i++)
     {
      fscanf(fp, "%d", &temp);
      data[i] = (char) temp;
     }
}


void scan_struct_fldcalc(struct fldcalc *data, int number, FILE *fp)
{
  int i, j, nops, tmpi;
  float tmpf;

   for (i = 0; i < number; i++)
     {
      fscanf(fp, "%d",&nops);
      for (j = 0; j < nops; j++)
        {
         switch(j)
           {
	    case 0: fscanf(fp, "%d", &tmpi);
                    data[i].xfldno = (short) tmpi;
	            break;
	    case 1: fscanf(fp, "%d", &tmpi);
                    data[i].yfldno = (short) tmpi;
		    break;
            case 2: fscanf(fp, "%d", &tmpi);
                    data[i].oper = (short) tmpi;
	            break;
            case 3: fscanf(fp, "%f", &tmpf);
                    data[i].cval = tmpf;
	            break;
            default: fprintf(stderr, "This doesn't happen!\n");
           }
        }
      fscanf(fp, "%s", data[i].name);
     }
}


void scan_struct_fldcalc2(struct fldcalc *data, int itype, FILE *fp)
{

  /*  Red field calc data.                                         */
  /*  Input:                                                       */
  /*     data - Pointer to selected field calc. structure.         */
  /*     itype - the field type, 0 = cell, 1 = node, 2 = surface   */
  /*             3 = tracer.                                       */
  /*     fp - the file to read from.                               */
  int i, k, nvars, fbegin, ivar, oper;
  float cval;
  char **fldname, tmpname[50], tmpname1[50], tmpname2[50];
  char tmpchar1[50], tmpchar2[50], tmpchar3[50];

   /*  Set field names.  */
   if (itype == 0)
     {
      fldname = cfieldname;
      nvars = cnumvars;
      fbegin = cfldcalcbeg;
     }
   else if (itype == 1)
     {
      fldname = fieldname;
      nvars = numvars;
      fbegin = fldcalcbeg;
     }
   else if (itype == 2)
     {
      fldname = sfieldname;
      nvars = snumvars;
      fbegin = sfldcalcbeg;
     }
   else if (itype == 3)
     {
      fldname = fieldtrname;
      nvars = numvartr;
      fbegin = tfldcalcbeg;
     }
   else return;

   fscanf(fp, "%d %s", &i, tmpchar1);
   stripname(fp,tmpchar1,tmpname1);
   fscanf(fp, "%s",tmpchar2);
   stripname(fp,tmpchar2,tmpname2);
   if (!strcmp(tmpname1,"NONE")) data[i].xfldno = -1;
   else
     {
      k = get_name_id(nvars,fldname,tmpname1);
      data[i].xfldno = (short)k;
     }
   if (!strcmp(tmpname2,"NONE")) data[i].yfldno = -1;
   else
     {
      k = get_name_id(nvars,fldname,tmpname2);
      data[i].yfldno = (short)k;
     }

   fscanf(fp, "%d %e %s",&oper,&cval,tmpchar3);
   stripname(fp,tmpchar3,tmpname);
   data[i].oper = (short)oper;
   data[i].cval = cval;
   strcpy(data[i].name,tmpname);

   /*  Move the new variable name into the appropriate field name.  */
   ivar = fbegin + i;
   strcpy(fldname[ivar],data[i].name);
}


void getattributes_old(void)
{
  FILE *in;
  float a[200];
  int i,j, s[100], maxmats, maxflds, maxtrflds, nvals;
  short m[100], titlelen, flg1[MAXFLAGS+1][MAXMATS], flg2[MAXFLAGS];
  float mmatfrac[MAXCONTOURS+MAXMATS], *lmmat, *lmcon;
  struct oldfldcalc
                {
                 short fldno;
                 short oper;
                 float cval;
                 char  name[9];
                };
  struct oldfldcalc oldfieldcalc[MAXFLDCALC];

   maxmats = MAXMATS;
   maxflds = MAXFIELDS;
   maxtrflds = MAXTRFIELDS;
   if ((in = fopen(attribsfilnam,"r")) == NULL) return;
   
   fread(&attribsversion,sizeof(int),1,in);
   if (attribsversion < 4) maxflds = 40;
   if (attribsversion < 9) maxmats = 64;
   lmmat = (float *)malloc((maxmats*25)*sizeof(float));
   fread(lmmat,sizeof(float),25*maxmats,in);
   for (i = 0; i < maxmats; i++)
     {
      matrgba[4*i+0] = lmmat[25*i+1]/.3;
      matrgba[4*i+1] = lmmat[25*i+2]/.3;
      matrgba[4*i+2] = lmmat[25*i+3]/.3;
      matrgba[4*i+3] = lmmat[25*i+15];
     }
   free(lmmat);
   fread(active_mat,sizeof(int),maxmats,in);

   fread(a,sizeof(float),29,in);
   xrot = a[0]; yrot = a[1]; zrot = a[2]; magnify = a[3];
   xrange = a[4]; yrange = a[5]; zrange = a[6]; span = a[7];
   aspect = a[8]; xpan = a[9]; ypan = a[10];
   xtrans = a[11]; ytrans = a[12]; ztrans = a[13];
   globxicmin = a[14]; globxicmax = a[15]; globyicmin = a[16];
   globyicmax = a[17]; globzicmin = a[18]; globzicmax = a[19];
   cutplane[0].aa = a[20]; cutplane[0].bb = a[21]; 
   cutplane[0].cc = a[22]; cutplane[0].dd = a[23];
   cutplane[0].heightfac = a[24]; cutplane[0].dist = a[25]; 
   pexplodefact = a[26]; velscale = a[27]; cexplodefact = a[28];

   fread(m,sizeof(short),15,in);
   axisflag = m[0]; nodesflag = m[1]; linesflag = m[2]; 
   polysflag = m[3]; cellsflag = m[4]; celledgesflag = m[5];
   cutplane[0].edgesflag = m[6]; cutplane[0].heightflag = m[7]; 
   cutplane[0].velflag = m[8]; cutplane[0].nodeflag = m[9]; 
   cutplane[0].cliponbox = m[10]; cellexpflag = m[11];
   timeflag = m[12]; cycleflag = m[13]; tracerflag = m[14];

   if (numflags_cell == 0) cellexpflag = -1;

   fread(s,sizeof(int),5,in);
   nodefieldactive = s[0]; cellfieldactive = s[1],
   cutplanefieldactive = s[2]; cutplane[0].npolys = s[3];
   tracefieldactive = s[4];
   if (cutplane[0].npolys > 0) cutplane_on[0] = 1;

   fread(contour_on,sizeof(int),MAXCONTOURS+maxmats,in);
   fread(mconval,sizeof(float),MAXCONTOURS+maxmats,in);
   fread(mmatfrac,sizeof(float),MAXCONTOURS+maxmats,in);
   for (i = MAXCONTOURS; i < MAXCONTOURS+maxmats; i++)
      mconval[i] = mmatfrac[i];
   fread(mconfield,sizeof(int),MAXCONTOURS+maxmats,in);
   fread(&backgroundcolor,sizeof(unsigned long),1,in);
   fread(a,sizeof(float),maxflds,in);
   for (i = 0; i < maxflds; i++) globfieldmin[i] = a[i];
   fread(a,sizeof(float),maxflds,in);
   for (i = 0; i < maxflds; i++) globfieldmax[i] = a[i];
   fread(cutplane[0].norm,sizeof(float),3,in);
   fread(a,sizeof(float),maxflds,in);
   for (i = 0; i < maxtrflds; i++) globfieldtrmin[i] = a[i];
   fread(a,sizeof(float),maxflds,in);
   for (i = 0; i < maxtrflds; i++) globfieldtrmax[i] = a[i];

   fread(a,sizeof(float),8,in); 
   lightx = a[0]; lighty = a[1]; globxicmino = a[2]; 
   globxicmaxo = a[3]; globyicmino = a[4];
   globyicmaxo = a[5]; globzicmino = a[6]; globzicmaxo = a[7];
   fread(xscale,sizeof(float),8,in);
   fread(yscale,sizeof(float),8,in);
   fread(zscale,sizeof(float),8,in);

   fread(s,sizeof(int),4,in);
   nrefl_loops = s[0]; xreflect = s[1]; yreflect = s[2];
   zreflect = s[3];
   if (polysin == 1 && (xreflect+yreflect+zreflect > 0))
      matexpvects();

   lmcon = (float *)malloc((MAXCONTOURS*25)*sizeof(float));
   fread(lmcon,sizeof(float),25*MAXCONTOURS,in);
   for (i = 0; i < MAXCONTOURS; i++)
     {
      conrgba[4*i+0] = lmcon[25*i+1]/.3;
      conrgba[4*i+1] = lmcon[25*i+2]/.3;
      conrgba[4*i+2] = lmcon[25*i+3]/.3;
      conrgba[4*i+3] = lmcon[25*i+15];
     }
   free(lmcon);
   fread(&cliponbox,sizeof(short),1,in);

   fread(a,sizeof(float),2,in);
   frontclipvalue = a[0]; backclipvalue = a[1];

   fread(a,sizeof(float),12,in);
   xminbdval = a[0]; xmaxbdval = a[1];
   yminbdval = a[2]; ymaxbdval = a[3];
   zminbdval = a[4]; zmaxbdval = a[5];
   xminfdval = a[6]; xmaxfdval = a[7];
   yminfdval = a[8]; ymaxfdval = a[9];
   zminfdval = a[10]; zmaxfdval = a[11];

   fread(a,sizeof(float),3,in);
   xscaleaxiso = xscaleaxis;
   yscaleaxiso = yscaleaxis;
   zscaleaxiso = zscaleaxis;
   xscaleaxis = a[0]; 
   yscaleaxis = a[1];
   zscaleaxis = a[2]; 

   fread(&nmat_order,sizeof(int),1,in);
   fread (mat_order,sizeof(int),nmat_order,in);

   nvals = (MAXFLAGS + 1) * maxmats;
   if (attribsversion < 11)
     {
      fread(flg1,sizeof(short),nvals,in);
      for (i = 0; i < MAXFLAGS+1; i++)
        {
         for (j = 0; j < maxmats; j++)
            nodeselflag[i][j] = flg1[i][j];
        }
      fread(flg2,sizeof(short),MAXFLAGS,in);
      for (i = 0; i < MAXFLAGS; i++)
          nodeandorflag[i] = flg2[i];
      fread(flg1,sizeof(short),nvals,in);
      for (i = 0; i < MAXFLAGS+1; i++)
        {
         for (j = 0; j < maxmats; j++)
            cellselflag[i][j] = flg1[i][j];
        }
      fread(flg2,sizeof(short),MAXFLAGS,in);
      for (i = 0; i < MAXFLAGS; i++)
          cellandorflag[i] = flg2[i];
     }
   else
     {
      fread(nodeselflag,sizeof(char),nvals,in);
      fread(nodeandorflag,sizeof(char),MAXFLAGS,in);
      fread(cellselflag,sizeof(char),nvals,in);
      fread(cellandorflag,sizeof(char),MAXFLAGS,in);
     }

   /*  Read version 2 data.  */

   if (attribsversion > 0)
     {
      fread(m,sizeof(short),3,in);
      colorbarflag = m[0]; colorbarpref = m[1]; 
      boundingboxflag = m[2]; 

      fread(a,sizeof(float),12,in);
      xminbbval = a[0]; xmaxbbval = a[1];
      yminbbval = a[2]; ymaxbbval = a[3];
      zminbbval = a[4]; zmaxbbval = a[5];
      xminbbvalo = a[6]; xmaxbbvalo = a[7];
      yminbbvalo = a[8]; ymaxbbvalo = a[9];
      zminbbvalo = a[10]; zmaxbbvalo = a[11];
     }

   /*  Read version 3 data.  */

   if (attribsversion > 2)
     {
      fread(&viewflag,sizeof(short),1,in);
      fread(flypos,sizeof(float),3,in);
     }
 
   /*  Read version 4 data.  */

   if (attribsversion > 3)
     {
      fread(m,sizeof(short),9,in);
      cliponcellsel = m[0];  cutplane[0].cliponcellsel = m[1];
      vectfldx = m[2];  vectfldy = m[3];  vectfldz = m[4];
      speedfld = m[5];  vectorflag = m[6];  titleflag = m[7];
      titlelen = m[8];
      fread(a,sizeof(float),1,in);
      maxspeed = a[0];
      strcpy(titlestr," ");
      if (titlelen > 0)
         fread(titlestr,sizeof(char),titlelen,in);
     }
 
   /*  Read version 5 data.  */

   if (attribsversion > 4)
     {
      fread(m,sizeof(short),1,in);
      cutplane[0].linesflag = m[0];
     }
 
   /*  Read version 6 data.  */

   if (attribsversion > 5)
      fread(&cellcontflag,sizeof(short),1,in);
 
   /*  Read version 7 data.  */

   if (attribsversion > 6)
     {
      fread(a,sizeof(float),9,in);
      cutplane[0].x1 = a[0];  cutplane[0].y1 = a[1];  cutplane[0].z1 = a[2];
      cutplane[0].x2 = a[3];  cutplane[0].y2 = a[4];  cutplane[0].z2 = a[5];
      cutplane[0].x3 = a[6];  cutplane[0].y3 = a[7];  cutplane[0].z3 = a[8];
      fread(cutlineon,sizeof(short),MAXCUTLINES,in);
      fread(cutlinefield,sizeof(int),MAXCUTLINES,in);
      fread(cutlinenodeson,sizeof(short),MAXCUTLINES,in);
      fread(waveflag,sizeof(short),MAXCUTLINES,in);
      fread(wavefac,sizeof(float),MAXCUTLINES,in);
      fread(ctlinx1,sizeof(float),MAXCUTLINES,in);
      fread(ctliny1,sizeof(float),MAXCUTLINES,in);
      fread(ctlinz1,sizeof(float),MAXCUTLINES,in);
      fread(ctlinx2,sizeof(float),MAXCUTLINES,in);
      fread(ctliny2,sizeof(float),MAXCUTLINES,in);
      fread(ctlinz2,sizeof(float),MAXCUTLINES,in);
      fread(cutlineradius,sizeof(float),MAXCUTLINES,in);
      if (attribsversion > 7)
         fread(fieldcalc,sizeof(struct fldcalc),MAXFLDCALC,in);
      else
        {
         fread(oldfieldcalc,sizeof(struct oldfldcalc),MAXFLDCALC,in);
         for (i = 0; i < MAXFLDCALC; i++)
           {
            fieldcalc[i].xfldno = oldfieldcalc[i].fldno;
            fieldcalc[i].yfldno = -1;
            fieldcalc[i].oper = oldfieldcalc[i].oper;
            fieldcalc[i].cval = oldfieldcalc[i].cval;
            strcpy(fieldcalc[i].name,oldfieldcalc[i].name);
           }
        }
     }
 
   /*  Read version 9 data.  */
   if (attribsversion > 8)
     {
      fread(s,sizeof(short),1,in);
      cutplane[0].linesflag = m[0];
     }

   /*  Read version 10 data.  */
   if (attribsversion > 9)
     {
      fread(&nodeselfield,sizeof(short),1,in);
      fread(&nfldselmin,sizeof(float),1,in);
      fread(&nfldselmax,sizeof(float),1,in);
      if (attribsversion < 11)
        {
         if (numflags_node == MAXFLAGS) 
            fread(&nodeandorflag[MAXFLAGS],sizeof(short),1,in);
        }
      else
        {
         if (numflags_node == MAXFLAGS) 
            fread(&nodeandorflag[MAXFLAGS],sizeof(char),1,in);
        }
     }

   /*  Read version 11 data.  */
   if (attribsversion > 10)
     {
      fread(&bboxcoordflag,sizeof(short),1,in);
      fread(&cellselfield,sizeof(short),1,in);
      fread(&cfldselmin,sizeof(float),1,in);
      fread(&cfldselmax,sizeof(float),1,in);
      if (numflags_cell == MAXFLAGS) 
         fread(&cellandorflag[MAXFLAGS],sizeof(char),1,in);
      fread(cfieldcalc,sizeof(struct fldcalc),MAXFLDCALC,in);
     }
  
   fclose(in);
}


void reset_isos_cuts()
{
  int i;

   /*  Set isosurfaces defaults.  */
   cliponbox = 0;
   cliponcellsel = 0;
   for (i = 0; i < MAXCONTOURS+MAXMATS+2; i++)
     {
      FREE(contourlist[i]);
      contour_on[i] = 0;
      num_of_contour_polys[i] = 0;
     }

   /*  Set isovolume defaults.  */
   isovol_on = 0;  ivcliponbox = 0;  ivcliponcellsel = 0;
   num_of_isovol_polys = 0;

   /*  Set cutplanes defaults.  */
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      cutplane_on[i] = 0;
      cutplane[i].npolys = 0;
      cutplane[i].cliponbox = 0;
      cutplane[i].cliponcellsel = 0;
      cutplane[i].heightflag = 0;
      cutplane[i].edgesflag = 0;
      cutplane[i].velflag = 0;
      cutplane[i].nodeflag = 0;
      cutplane[i].linesflag = 0;
      cutplane[i].cellflag = 0;
      cutplane[i].cvelflag = 0;
      cutplane[i].cnumflag = 0;
      cutplane[i].facesflag = 1;
      cutplane[i].dist = 0.;
      cutplane[i].ndnodes = 0;
      cutplane[i].ndcells = 0;
      cutplane[i].x1 = 0;
      cutplane[i].y1 = 0;
      cutplane[i].z1 = 0;
      cutplane[i].x2 = 0;
      cutplane[i].y2 = 0;
      cutplane[i].z2 = 0;
      cutplane[i].x3 = 0;
      cutplane[i].y3 = 0;
      cutplane[i].z3 = 0;
      FREE(cutplane[i].vertsperpoly);
      FREE(cutplane[i].polylist);
      FREE(cutplane[i].dnodes);
      FREE(cutplane[i].vect[0]);
      FREE(cutplane[i].vect[1]);
      FREE(cutplane[i].vect[2]);
      FREE(cutplane[i].dcells);
      FREE(cutplane[i].cvect[0]);
      FREE(cutplane[i].cvect[1]);
      FREE(cutplane[i].cvect[2]);
     }

   for (i = 0; i < MAXCUTLINES; i++)
     {
      ncutlinepts[i] = 0.;
      cutlineon[i] = 0;
      cutlinefield[i] = 0;
      cutlinenodeson[i] = 0;
      cutlineradius[i] = clminradius;
      waveflag[i] = 0;
      wavefac[i] = 0.;
      ctlinx1[i] = 0.;  ctliny1[i] = 0.;  ctlinz1[i] = 0;
      ctlinx2[i] = 0.;  ctliny2[i] = 0.;  ctlinz2[i] = 0;
      FREE(cutlineval[i]);
      FREE(cutlinedist[i]);
      FREE(cutlinenodes[i]);
     }
}


void getattr_set_defaults()
{

  int i, j;

   viewflag = 0;  cliponcellsel = 0;
   cellcontflag = 0;  bboxcoordflag = 1;
   nodeselfield = 0;
   nfldselmin = fieldmin[0];
   nfldselmax = fieldmax[0];
   nodeandorflag[numflags_node] = 0;
   cellselfield = 0;
   cfldselmin = cfieldmin[0];
   cfldselmax = cfieldmax[0];
   cellandorflag[numflags_cell] = 0;
   pnt_size = 2;  pnt_shape = 0;  line_size = 2;  line_type = 0;
   cellselmat_on = 0;  cellselcfld_on = 0; 
   cellselnfld_on = 0;  cellselnfldopt = 0;  cellselsph_on = 0;
   cellselnum_on = 0;  ncellselnum = 0;  cellselsph_in = 1;
   cellselbox_on = 0;  cellselbox_in = 1;
   nodeselmat_on = 0;  nodeselfld_on = 0;  nodeselsph_on = 0;
   nodeselnum_on = 0;  nnodeselnum = 0;  nodeselsph_in = 1;
   nodeselbox_on = 0;  nodeselbox_in = 0;
   traceselnum_on = 0;  ntraceselnum = 0; fieldcolortype = 0, 
   facerefineflag = 0;  nset_contours = 10; cellnormflag = 0;
   surfselfield = 0;
   sfldselmin = sfieldmin[0];
   sfldselmax = sfieldmax[0];
   surfandorflag[numflags_surf] = 0;
   surfselmat_on = 0;  surfselsfld_on = 0; 
   surfselnfld_on = 0;  surfselnfldopt = 0;  surfselsph_on = 0;
   surfselnum_on = 0;  nsurfselnum = 0;  surfselsph_in = 1;
   surfselbox_on = 0;  surfselbox_in = 1;  surfrefineflag = 0;
   nodeselnodegrp_on = 0;  nnodegrpselected = 0;  nodeselgrp_in = 1;
   cellselcellgrp_on = 0;  ncellgrpselected = 0;  cellselgrp_in = 1;
   cellselfacegrp_on = 0;  nfacegrpselected = 0;  faceselgrp_in = 1;
   surfselsurfgrp_on = 0;  nsurfgrpselected = 0;  surfselgrp_in = 1;
   drawghosts = 0;  drawghostface = 1;  drawnodeghosts = 0;
   drawnodeghosts = 0;  drawunselnodes= 0;

   /*  Set material data.  */
   for (i = 0; i < MAXMATS; i++) active_mat[i] = 0;

   /*  Set node, cell, and surface material and flag selection data.  */
   for (i = 0; i <= MAXFLAGS; i++)
     {
      for (j = 0; j < MAXMATS; j++)
        {
         nodeselflag[i][j] = 0;
         cellselflag[i][j] = 0;
         surfselflag[i][j] = 0;
        }
      if (i < MAXFLAGS)
        {
         nodeandorflag[i] = 0;
         cellandorflag[i] = 0;
         surfandorflag[i] = 0;
       }
     }

   /* Reset tracer selections */
   for(i = 0; i < MAXTRFIELDS; i++)
   {
       traceselfields[i] = 0;
   }

   /*  Reset isosurfaces, isovolume, cutplanes and cutlines.  */
   reset_isos_cuts();

   /*  Reset axis scales to 1, if necessary.  */
   if (xscaleaxis != 1.0 || yscaleaxis != 1.0 || zscaleaxis != 1.0)
     {
      xscaleaxiso = xscaleaxis;
      yscaleaxiso = yscaleaxis;
      zscaleaxiso = zscaleaxis;
      xscaleaxis = 1.0;
      yscaleaxis = 1.0;
      zscaleaxis = 1.0;
      setscaleaxis(0);
     }
}


void getattr_set_data()
{
  int i, j, fldno, ierr, nalloc, chkcont, chkmcont;
  int cpon;
  int nclpts, ctype;
  double *value, sav_fmin, sav_fmax, sav_speed, sav_fmin2, sav_fmax2;
  float cpaa, cpbb, cpcc, cpdd;

  int donfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  int docfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  int dotfldcalc(short oper, float cval, short xfld, 
                 short yfld, int fldno);
  double * readnodevalue(int ifld);
  double * readcellvalue(int ifld);
  void fillnodevalue(int ifld, double *value);
  void fillcellvalue(int ifld, double *value);
  void fillfacevalue(int ifld, double *value);
  void fillsurfvalue(int ifld, double *value);
  void calcisosurf(int i, int ctype, float isoval, double *value);
  void calccutplane(int i, float aa, float bb, float cc, 
                    float dd, double *value);
  void calccutsphere(int i, float aa, float bb, float cc, 
                     float dd, double *value);
  void calcisovolume(float isoval1, float isoval2, double *value);
  void nodespeedfill();
  void cellspeedfill();
  void facespeedfill();
  void surfspeedfill();
  void readpolyfile();

   /*  If polygons are drawn, get polygon data from disk.  */
   if (polysflag || linesflag)
      readpolyfile();

   /*  Scale axes.  */
   xscaleaxiso = 1.;
   yscaleaxiso = 1.;
   zscaleaxiso = 1.;
   setscaleaxis(1);

   if (cellfieldactive == -3 && 
       (cellsflag || celledgesflag || cvectorflag|| cellmedianedgeflag ||
        cellnumflag || cellhiddenedgesflag))
      fillfacegrpcolr();

   /*  Set node, cell and surface subset flags.  */
   setnodecellsubset();

   /*  Set polygon subset.  */
   if (polysin) setpolysubset();

   /*  Calculate node field calc.  */
   if (numvars > 3)
     {
      for (i = 0; i < MAXFLDCALC; i++)
        {
         fldno = fldcalcbeg + i;   
         if (fldno < numvars && fieldcalc[i].xfldno > -1)
           {

            /*  Save current fieldcalc field min., max.  */
            sav_fmin = globfieldmin[fldno];
            sav_fmax = globfieldmax[fldno];
            sav_fmin2 = nfld_nice_min[fldno];
            sav_fmax2 = nfld_nice_max[fldno];

            /*  Calculate new field.  */
            ierr = donfldcalc(fieldcalc[i].oper,fieldcalc[i].cval,
                              fieldcalc[i].xfldno,fieldcalc[i].yfldno,
                              fldno);

            strcpy(fieldname[fldno],fieldcalc[i].name);

            /*  Reset current fieldcalc field min., max.  */
            globfieldmin[fldno] = sav_fmin;
            globfieldmax[fldno] = sav_fmax;
            nfld_nice_min[fldno] = sav_fmin2;
            nfld_nice_max[fldno] = sav_fmax2;

            /*  Set node field button sensitivity on.  */
            if (movieflag == 0)
               setnfldsensitive(fldno,TRUE);
           }
        }
     }

   /*  Calculate cell field calc.  */

   if (cnumvars > 0)
     {
      for (i = 0; i < MAXFLDCALC; i++)
        {
         fldno = cfldcalcbeg + i;   
         if (fldno < cnumvars && cfieldcalc[i].xfldno > -1)
           {

            /*  Save current fieldcalc field min., max.  */
            sav_fmin = globfieldcmin[fldno];
            sav_fmax = globfieldcmax[fldno];
            sav_fmin2 = cfld_nice_min[fldno];
            sav_fmax2 = cfld_nice_max[fldno];

            /*  Calculate new field.  */
            ierr = docfldcalc(cfieldcalc[i].oper,cfieldcalc[i].cval,
                              cfieldcalc[i].xfldno,cfieldcalc[i].yfldno,
                              fldno);

            strcpy(cfieldname[fldno],cfieldcalc[i].name);

            /*  Reset current fieldcalc field min., max.  */
            globfieldcmin[fldno] = sav_fmin;
            globfieldcmax[fldno] = sav_fmax;
            cfld_nice_min[fldno] = sav_fmin2;
            cfld_nice_max[fldno] = sav_fmax2;

            /*  Set cell field button sensitivity on.  */
            if (movieflag == 0)
               setcfldsensitive(fldno,TRUE);
           }
        }
     }

   /*  Calculate tracer field calc.  */

   if (numvartr > 0)
     {
      for (i = 0; i < MAXFLDCALC; i++)
        {
         fldno = tfldcalcbeg + i;   
         if (fldno < numvartr && tfieldcalc[i].xfldno > -1)
           {

            /*  Save current fieldcalc field min., max.  */
            sav_fmin = globfieldtrmin[fldno];
            sav_fmax = globfieldtrmax[fldno];
            sav_fmin2 = trfld_nice_min[fldno];
            sav_fmax2 = trfld_nice_max[fldno];

            /*  Calculate new field.  */
            ierr = dotfldcalc(tfieldcalc[i].oper,tfieldcalc[i].cval,
                              tfieldcalc[i].xfldno,tfieldcalc[i].yfldno,
                              fldno);

            strcpy(fieldtrname[fldno],tfieldcalc[i].name);

            /*  Reset current fieldcalc field min., max.  */
            globfieldtrmin[fldno] = sav_fmin;
            globfieldtrmax[fldno] = sav_fmax;
            trfld_nice_min[fldno] = sav_fmin2;
            trfld_nice_max[fldno] = sav_fmax2;

            /*  Set cell field button sensitivity on.  */
            if (movieflag == 0)
               settrfldsensitive(fldno,TRUE);
           }
        }
     }

   /*  Calculate surface field calc.  */

   if (nsurf > 0 && snumvars > 0)
     {
      for (i = 0; i < MAXFLDCALC; i++)
        {
         fldno = sfldcalcbeg + i;   
         if (fldno < snumvars && sfieldcalc[i].xfldno > -1)
           {

            /*  Save current fieldcalc field min., max.  */
            sav_fmin = globfieldsmin[fldno];
            sav_fmax = globfieldsmax[fldno];
            sav_fmin2 = sfld_nice_min[fldno];
            sav_fmax2 = sfld_nice_max[fldno];

            /*  Calculate new field.  */
            ierr = dosfldcalc(sfieldcalc[i].oper,sfieldcalc[i].cval,
                              sfieldcalc[i].xfldno,sfieldcalc[i].yfldno,
                              fldno);

            strcpy(sfieldname[fldno],sfieldcalc[i].name);

            /*  Reset current fieldcalc field min., max.  */
            globfieldsmin[fldno] = sav_fmin;
            globfieldsmax[fldno] = sav_fmax;
            sfld_nice_min[fldno] = sav_fmin2;
            sfld_nice_max[fldno] = sav_fmax2;

            /*  Set surface field button sensitivity on.  */
            if (movieflag == 0)
               setsfldsensitive(fldno,TRUE);
           }
        }
     }

   /*  Set node, cell and surface selections.  */
   setnodesel();
   setcellsel();
   if (cellsflag) setfacesel(); 
   setsurfsel();

   /*  If node vectors drawn, fill node vector arrays.  */
   if (vectorflag)
      fillnodevects();

   /*  Calculate and write node vector speed.        */
   /*  Be sure to save and reset speed min and max.  */
   if (speedfld > 0)
     {
      sav_fmin = globfieldmin[speedfld];
      sav_fmax = globfieldmax[speedfld];
      sav_fmin2 = nfld_nice_min[speedfld];
      sav_fmax2 = nfld_nice_max[speedfld];
      sav_speed = maxspeed;
      if (vectfldx >= 0 || vectfldy >= 0 || vectfldz >= 0)
         nodespeedfill();
      globfieldmin[speedfld] = sav_fmin;
      globfieldmax[speedfld] = sav_fmax;
      nfld_nice_min[speedfld] = sav_fmin2;
      nfld_nice_max[speedfld] = sav_fmax2;
      maxspeed = sav_speed;
     }

   /*  If cell vectors drawn, fill cell vector arrays.  */
   if (cvectorflag)
      fillcellvects();

   /*  Calculate and write cell vector speed.        */
   /*  Be sure to save and reset speed min and max.  */
   if (cspeedfld > 0)
     {
      sav_fmin = globfieldcmin[cspeedfld];
      sav_fmax = globfieldcmax[cspeedfld];
      sav_fmin2 = cfld_nice_min[cspeedfld];
      sav_fmax2 = cfld_nice_max[cspeedfld];
      sav_speed = cmaxspeed;
      if (cvectfldx >= 0 || cvectfldy >= 0 || cvectfldz >= 0)
         cellspeedfill();
      globfieldcmin[cspeedfld] = sav_fmin;
      globfieldcmax[cspeedfld] = sav_fmax;
      cfld_nice_min[cspeedfld] = sav_fmin2;
      cfld_nice_max[cspeedfld] = sav_fmax2;
      cmaxspeed = sav_speed;
     }

   /*  If face vectors drawn, fill face vector arrays.  */
   if (fvectorflag)
      fillfacevects();

   /*  Calculate and write face vector speed.        */
   /*  Be sure to save and reset speed min and max.  */
   if (fspeedfld > 0)
     {
      sav_fmin = globfieldfmin[fspeedfld];
      sav_fmax = globfieldfmax[fspeedfld];
      sav_fmin2 = ffld_nice_min[speedfld];
      sav_fmax2 = ffld_nice_max[speedfld];
      sav_speed = fmaxspeed;
      if (fvectfldx >= 0 || fvectfldy >= 0 || fvectfldz >= 0)
         facespeedfill();
      globfieldfmin[fspeedfld] = sav_fmin;
      globfieldfmax[fspeedfld] = sav_fmax;
      ffld_nice_min[fspeedfld] = sav_fmin2;
      ffld_nice_max[fspeedfld] = sav_fmax2;
      fmaxspeed = sav_speed;
     }

   if (nsurf > 0)
     {
      /*  If surface vectors drawn, fill surface vector arrays.  */
      if (svectorflag)
         fillsurfvects();

      /*  Calculate and write surface vector speed.     */
      /*  Be sure to save and reset speed min and max.  */
      if (sspeedfld > 0)
        {
         sav_fmin = globfieldsmin[sspeedfld];
         sav_fmax = globfieldsmax[sspeedfld];
         sav_fmin2 = sfld_nice_min[sspeedfld];
         sav_fmax2 = sfld_nice_max[sspeedfld];
         sav_speed = smaxspeed;
         if (svectfldx >= 0 || svectfldy >= 0 || svectfldz >= 0)
            surfspeedfill();
         globfieldsmin[sspeedfld] = sav_fmin;
         globfieldsmax[sspeedfld] = sav_fmax;
         sfld_nice_min[sspeedfld] = sav_fmin2;
         sfld_nice_max[sspeedfld] = sav_fmax2;
         smaxspeed = sav_speed;
        }
     }

   /*  Reset field mins and maxs for auto scale.  */
   for (i = 0; i < numvars; i++)
     {
      if (nfieldlimauto[i])
        {
         globfieldmin[i] = fieldmin[i];
         globfieldmax[i] = fieldmax[i];
         nfld_nice_min[i] = fieldmin[i];
         nfld_nice_max[i] = fieldmax[i];
        }
     }
   if (speedfld > 0)
     {
      if (nfieldlimauto[speedfld])
         maxspeed = fieldmax[speedfld];
     }
   for (i = 0; i < cnumvars; i++)
     {
      if (cfieldlimauto[i])
        {
         globfieldcmin[i] = cfieldmin[i];
         globfieldcmax[i] = cfieldmax[i];
         cfld_nice_min[i] = cfieldmin[i];
         cfld_nice_max[i] = cfieldmax[i];
        }
     }
   if (cspeedfld > 0)
     {
      if (cfieldlimauto[cspeedfld])
         cmaxspeed = cfieldmax[cspeedfld];
     }
   if (nsurf > 0)
     {
      for (i = 0; i < snumvars; i++)
        {
         if (sfieldlimauto[i])
           {
            globfieldsmin[i] = sfieldmin[i];
            globfieldsmax[i] = sfieldmax[i];
            sfld_nice_min[i] = sfieldmin[i];
            sfld_nice_max[i] = sfieldmax[i];
           }
        }
      if (sspeedfld > 0)
        {
         if (sfieldlimauto[sspeedfld])
            smaxspeed = sfieldmax[sspeedfld];
        }
     }
   if (ntracers > 0)
     {
      for (i = 0; i < numvartr; i++)
        {
         if (trfieldlimauto[i])
           {
            globfieldtrmin[i] = fieldtrmin[i];
            globfieldtrmax[i] = fieldtrmax[i];
            trfld_nice_min[i] = fieldtrmin[i];
            trfld_nice_max[i] = fieldtrmax[i];
           }
        }
     }
   for (i = 0; i < fnumvars; i++)
     {
      if (ffieldlimauto[i])
        {
         globfieldfmin[i] = ffieldmin[i];
         globfieldfmax[i] = ffieldmax[i];
         ffld_nice_min[i] = ffieldmin[i];
         ffld_nice_max[i] = ffieldmax[i];
        }
     }
   if (fspeedfld > 0)
     {
      if (fnumvars > 0 && ffieldlimauto[fspeedfld])
         fmaxspeed = ffieldmax[fspeedfld];
     }

   /*  Read node and cell active fields.  */
   if (nodefieldactive > -1 && nodefieldactive < numvars &&
       (nodesflag > 0 || vectorflag || nodenumflag))
     {
      if (nodefield == NULL)
         nodefield = readnodevalue(nodefieldactive);
      else
         fillnodevalue(nodefieldactive,nodefield);
     }

   if (nodefieldactive == -2 && 
       (nodesflag > 0 || vectorflag || nodenumflag))
      fillnodegrpcolr();

   if (cellfieldactive > -1 && cellfieldactive < numvars + cnumvars + fnumvars
       &&
       (cellsflag || celledgesflag || cvectorflag|| cellmedianedgeflag ||
        cellnumflag || cellhiddenedgesflag))
     {
      if (cellfield == NULL)
        {
         nalloc = (nnodes >= ncells) ? nnodes+1 : ncells;
         if (fnumvars > 0)
           {
            if (ncellfaces > nalloc) nalloc = ncellfaces;
           } 
         cellfield=(double *)malloc((nalloc)*sizeof(double));
         if (cellfield == NULL) memerr();
        }
      if (cellfieldactive < numvars)
         fillnodevalue(cellfieldactive,cellfield);
      else if (cellfieldactive < numvars + cnumvars)
         fillcellvalue(cellfieldactive-numvars,cellfield);
      else
         fillfacevalue(cellfieldactive-(numvars+cnumvars),cellfield);
     }

   if (cellfieldactive == -2 && 
       (cellsflag || celledgesflag || cvectorflag|| cellmedianedgeflag ||
        cellnumflag || cellhiddenedgesflag))
      fillcellgrpcolr();

   /*  Read surface active fields.  */
   if (nsurf > 0)
     {
      if (surffieldactive > -1 && surffieldactive < numvars + snumvars &&
          (surfflag || surfedgesflag || svectorflag || surfnumflag))
        {
         if (surffield == NULL)
           {
            nalloc = (nnodes >= nsurf) ? nnodes+1 : nsurf;
            surffield=(double *)malloc((nalloc)*sizeof(double));
            if (surffield == NULL) memerr();
           }
         if (surffieldactive < numvars)
            fillnodevalue(surffieldactive,surffield);
         else
            fillsurfvalue(surffieldactive-numvars,surffield);
        }

      if (surffieldactive == -2 && 
          (surfflag || surfedgesflag || svectorflag || surfnumflag))
         fillsurfgrpcolr();
     }

   /*  Calculate contours, cutplanes and cutspheres.  */
   if (ncells > 0)
     {
      chkcont = 0;
      chkmcont = 0;
      for (i = 0; i < MAXCONTOURS+mmats; i++)
        { 
         if (contour_on[i] == 1 )
           {
            contourfieldactive = mconfield[i];
            if (i < MAXCONTOURS)
              {
               value = readnodevalue(contourfieldactive);
               ctype = 0;
               chkcont = 1;
              }
            else
              {
               value = (double *) malloc((nnodes+1)*sizeof(double));
               for (j = 1; j < nnodes+1; j++)
                  value[j] = (imat[j] == i-MAXCONTOURS+1) ? 1. : 0.;
               ctype = 1;
               chkmcont = 1;
              }

            calcisosurf(i,ctype,mconval[i],value);

            free(value);
           }
         if (chkcont) isosurfgetfldclr();
         if (chkmcont) misosurfgetfldclr();
        }  

      cpon = 0;
      for (i = 0; i < MAXCUTPLANES; i++)
         if (cutplane_on[i]) cpon = 1;
      if (cpon)
        {
         cutplanefieldforplane = cutplanefieldactive;
         if (cutplanefieldforplane < numvars)
            value = readnodevalue(cutplanefieldforplane);
         else
            value = readcellvalue(cutplanefieldforplane-numvars);

         for (i = 0; i < MAXCUTPLANES; i++)
           {
            if (cutplane_on[i])
              {
               cpaa = cutplane[i].aa;  cpbb = cutplane[i].bb;
               cpcc = cutplane[i].cc;  cpdd = cutplane[i].dd;
               calccutplane(i, cpaa, cpbb, cpcc, cpdd, value);
              }
           }

         free(value);
        }

      cpon = 0;
      for (j = 0; j < MAXCUTSPHERES; j++)
        {
         i = MAXCUTPLANES + 1 + j;
         if (cutplane_on[i]) cpon = 1;
        }
      if (cpon)
        {
         cutspherefieldforsphere = cutspherefieldactive;
         if (cutspherefieldforsphere < numvars)
            value = readnodevalue(cutspherefieldforsphere);
         else
            value = readcellvalue(cutspherefieldforsphere-numvars);

         for (j = 0; j < MAXCUTSPHERES; j++)
           {
            i = MAXCUTPLANES + 1 + j;
            if (cutplane_on[i])
              {
               cpaa = cutplane[i].aa;  cpbb = cutplane[i].bb;
               cpcc = cutplane[i].cc;  cpdd = cutplane[i].dd;
               calccutsphere(i, cpaa, cpbb, cpcc, cpdd, value);
              }
           }

         free(value);
        }
     }

   /*  Calculate isovolume.  */
   if (ncells > 0)
     {
      if (isovol_on == 1)
        {
         value = readnodevalue(isovolfieldactive);

         calcisovolume(isovol_val1,isovol_val2,value);

         free(value);

         isovolgetfldclr();
        }  
     }

   /*  Calculate cutlines.  */
   for (i = 0; i < MAXCUTLINES; i++)
     { 
      /*  If a cutline already exists, free memory.  */
      if (ncutlinepts[i] > 0)
        {
         free(cutlineval[i]);
         free(cutlinedist[i]);
         free(cutlinenodes[i]);
        }
      ncutlinepts[i] = 0;
      if (cutlineon[i] == 1)
        {
         if (cutlinefield[i] < numvars)
            value = readnodevalue(cutlinefield[i]);
         else
            value = readcellvalue(cutlinefield[i]-numvars);

         makecutline(ctlinx1[i],ctliny1[i],ctlinz1[i],
                     ctlinx2[i],ctliny2[i],ctlinz2[i],
                     cutlineradius[i],cutlinefield[i],
                     value,&nclpts);
         free(value);

         /*  Fill cutline arrays from makecutline arrays.  */
         cutlineon[i] = 0;
         if (nclpts > 0)
           {
            cutlineon[i] = 1;
            ncutlinepts[i] = nclpts;
            cutlineval[i] = (float *)malloc(nclpts*sizeof(float));
            cutlinedist[i] = (float *)malloc(nclpts*sizeof(float));
            cutlinenodes[i] = (int *)malloc(nclpts*sizeof(int));
            for (j = 0; j < nclpts; j++)
              {
               cutlineval[i][j] = cutval[j];
               cutlinedist[i][j] = cutlist[j];
               cutlinenodes[i][j] = cutnodes[j];
              }
           }
         free(cutnodes);  free(cutlist);  free(cutval);
        }
     } 

   /*  Set cell explosion vectors.  */
   if (cexplodefact > 0.) cellexpvects();

   /*  Set surface explosion vectors.  */
   if (nsurf > 0 && sexplodefact > 0.) surfexpvects();

   /*  Set tracer selections and read      */
   /*  trace history files, if requested.  */
   if (ntracers > 0)
     {
      checkSIZEflag();
      settracesel();
      if (tracehistflag)
        {
         if (ntracehist > 1)
           {
            for (i = 1; i < ntracehist; i++)
              {
               for (j = 0; j < tracers[i].numvar; j++)
                 {
                  free(tracers[i].field[j]);
                 }
               free(tracers[i].x);
               free(tracers[i].y);
               free(tracers[i].z);
               tracers[i].ntracers = 0;
              }
            ntracehist = 1;
           }
         rdtracehist();
        }
     }

   /*  Set animation cutplane limits.  */
   getanimcplims();

   /*  Set field color map.  */
   if (fieldcolortype >= 0)
      setfieldcolors();

   /*  Set center to cell, node or tracer, if requested.  */
   if (cencellid > -1)
     {
      i = cencell_set();
     }
   if (cennodeid > -1)
     {
      i = cennode_set();
     }
   if (centracerid > -1)
     {
      i = centracer_set();
     }
}


void getattr_set_widgets()
{
  int i, j, fldno;
  int rr, gg, bb, maxmats = MAXMATS;
  float redcol, greencol, bluecol, ptsize, lnsize;
  int matpixid;
  unsigned long matpix;
  unsigned long getbestcolor(float red, float green, float blue);

   /*  Set text color color.  */
   rr = (int) (backgroundcolor & 255);
   gg = (int) ((backgroundcolor >> 8) & 255);
   bb = (int) ((backgroundcolor >> 16) & 255);
   txtcolor = 0xffffffff;
   if (gg > 150 | (rr > 220 & bb > 220)) txtcolor = 0x000000ff;

   /*  Set point size and shape.  */
   ptsize = (float)pnt_size;
   glPointSize(ptsize);
   if (pnt_shape == 1)
      glEnable(GL_POINT_SMOOTH);
   else
      glDisable(GL_POINT_SMOOTH);

   /*  Set line_width.  */
   lnsize = (float)line_size;
   glLineWidth(lnsize);
   if (line_type == 1)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   /*  Set colors.  */
   j = 0;
   for (i = 1; i < maxmats+1; i++)
     { 
      rr = (int) (255.0*matrgba[j+0]);
      gg = (int) (255.0*matrgba[j+1]);
      bb = (int) (255.0*matrgba[j+2]);
      matcolor[i-1] = ((rr<<24) | (gg << 16) | (bb << 8) | (255));

      j += 4;          
     }  

   j = 0;
   for (i = 1; i < MAXCONTOURS+3; i++)
     { 
      rr = (int) (255.0*conrgba[j+0]);
      gg = (int) (255.0*conrgba[j+1]);
      bb = (int) (255.0*conrgba[j+2]);
   
      if (movieflag == 0)
        {
         redcol = (float)rr/255.0;
         greencol = (float)gg/255.0;
         bluecol = (float)bb/255.0;
         matpixid = i;
         if (!mesacmap && !truecolorcmap)
           {
            matpix = mat_pixels[matpixid];
            mat_color.pixel = matpix;
            mat_color.flags = DoRed | DoGreen | DoBlue;
            mat_color.red = (unsigned short)(redcol * 65535.);
            mat_color.green = (unsigned short)(greencol * 65535.);
            mat_color.blue = (unsigned short)(bluecol * 65535.);
            XStoreColor(dpy,my_cmap,&mat_color);
           }
         else
           {
            if (mesacmap)
               matpix = getbestcolor(redcol,greencol,bluecol);
            if (truecolorcmap)
              {
               mat_color.pixel = matpix;
               mat_color.flags = DoRed | DoGreen | DoBlue;
               mat_color.red = (unsigned short)(redcol * 65535.);
               mat_color.green = (unsigned short)(greencol * 65535.);
               mat_color.blue = (unsigned short)(bluecol * 65535.);
               XAllocColor(dpy,my_cmap,&mat_color);
               matpix = mat_color.pixel;
              }
            mat_pixels[matpixid] = matpix;
            setcoloreditisoclr(i-1,matpix);
            if (i < MAXCONTOURS+1)
              {
               setanimisoclr(i-1,matpix);
               if (ncells > 0)
                  setisoisoclr(i-1,matpix);
              }
            if (i == MAXCONTOURS+2)
               setisovolclr(matpix);
           }
        }

      j += 4;
     }

   /*  Set motif widgets.  */
   if (movieflag == 0 )
     {

      /*  First pop down all forms.  */

      popdownall();

      /*  Now fill in widgets with global access.  */

      setmag();
      setangles();

      XmToggleButtonGadgetSetState(objxrefl,xreflect,FALSE);
      XmToggleButtonGadgetSetState(objyrefl,yreflect,FALSE);
      XmToggleButtonGadgetSetState(objzrefl,zreflect,FALSE);

      setcenterwidgets();
      setsubsetwidgets();
      setboundingboxwidgets();

      if (polysin > 0)
         setpolysubsetwidgets();

      XmToggleButtonGadgetSetState(colrbaron,colorbarflag,FALSE);
      XmToggleButtonGadgetSetState(colrpref0,FALSE,FALSE);
      XmToggleButtonGadgetSetState(colrpref1,FALSE,FALSE);
      XmToggleButtonGadgetSetState(colrpref2,FALSE,FALSE);
      XmToggleButtonGadgetSetState(colrpref3,FALSE,FALSE);
      XmToggleButtonGadgetSetState(colrpref4,FALSE,FALSE);
      switch(colorbarpref)
        {
         case 0: XmToggleButtonGadgetSetState(colrpref0,TRUE,FALSE);
                 break;
         case 1: XmToggleButtonGadgetSetState(colrpref1,TRUE,FALSE);
                 break;
         case 2: XmToggleButtonGadgetSetState(colrpref2,TRUE,FALSE);
                 break;
         case 3: XmToggleButtonGadgetSetState(colrpref3,TRUE,FALSE);
                 break;
         case 4: XmToggleButtonGadgetSetState(colrpref4,TRUE,FALSE);
        }

      XmToggleButtonGadgetSetState(view1,FALSE,FALSE);
      XmToggleButtonGadgetSetState(view2,FALSE,FALSE);
      XmToggleButtonGadgetSetState(view3,FALSE,FALSE);
      switch(viewflag)
        {
         case 0: XmToggleButtonGadgetSetState(view1,TRUE,FALSE);
                 break;
         case 1: XmToggleButtonGadgetSetState(view2,TRUE,FALSE);
                 break;
         case 2: XmToggleButtonGadgetSetState(view3,TRUE,FALSE);
        }

      /*  Reset node field calc button name for all menus.  */
      if (numvars > 0)
        {
         for (i = 0; i < MAXFLDCALC; i++)
           {
            fldno = fldcalcbeg + i;   
            setnfieldname(fldno, fieldname[fldno]);
           }
       }

      /*  Reset cell field calc button name for all menus.  */
      if (cnumvars > 0)
        {
         for (i = 0; i < MAXFLDCALC; i++)
           {
            fldno = cfldcalcbeg + i;   
            setcfieldname(fldno, cfieldname[fldno]);
           }
       }

      /*  Reset surface field calc button name for all menus.  */
      if (nsurf > 0 && snumvars > 0)
        {
         for (i = 0; i < MAXFLDCALC; i++)
           {
            fldno = sfldcalcbeg + i;   
            setsfieldname(fldno, sfieldname[fldno]);
           }
       }

      /*  Reset tracer field calc button name for all menus.  */
      if (numvartr > 0)
        {
         for (i = 0; i < MAXFLDCALC; i++)
           {
            fldno = tfldcalcbeg + i;   
            settrfieldname(fldno, fieldtrname[fldno]);
            settrfldlab(fldno);
            settrfldlimlab(fldno);
           }
       }
      
     }

   /*  Reset selection box flags.  */
   subsetbox_draw = 0;
   cellselbox_draw = 0;
   nodeselbox_draw = 0;
   surfselbox_draw = 0;
   polyselbox_draw = 0;
   traceselbox_draw = 0;

   lt[0] = GL_POSITION;
   lt[1] = lightx;
   lt[2] = lighty;
   lt[3] = 1.0;
   lt[4] = 0.0;
   lightchanged = 1;
}
