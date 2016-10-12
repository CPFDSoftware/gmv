
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "gmvdata.h"
#include "contour.h"
#include "cutplane.h"

 
void drawcolorbar()

{
 
/* This routine plots a colorbar. */
 
  int ifld, ifld2, ifld3, nsets, i, icol, ipref, cutplaneon, misosurfon,
      isosurfon, isovolon, cutsphereon, nintervals;
  double fmin, fmax, f1, f2, intervals[100];
  float v1[3], x1, y1, x2, y2;
  float dy, y, yp1, yp2;
  double fcol, df, flab;
  float  colr, colg, colb;
  char title1[12], title2[33], str[100], str2[80], niceflag, logflag;
  void getscale(double llimit, double ulimit,int max, char lineflag,
                char logflag, double *intervals, int *nintervals);
 
   /*  Choose a field to display.  */
   ifld = -1;  ifld2 = -1;
   ipref = 0;
   niceflag = 0;  logflag = 0;
   if (nodesflag > 0 || vectorflag == 1 || nodenumflag)
     {
      ifld2 = nodefieldactive;
      if (ifld2 >= numvars) ifld2 = -1;
      if (ifld2 >= 0)
        {
         strcpy(title1,"Nodes");
         strcpy(title2,fieldname[ifld2]);
         fmin = globfieldmin[ifld2];
         fmax = globfieldmax[ifld2];
         if (nfld_nice[ifld2] || nfld_log[ifld2])
           {
            niceflag = nfld_nice[ifld2];
            logflag = nfld_log[ifld2];
            fmin = nfld_nice_min[ifld2];
            fmax = nfld_nice_max[ifld2];
           }
         if (colorbarpref == 1) ipref = 1;
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   if (tracerflag > 0 && ipref == 0 && ntracers > 0)
     {
      ifld2 = tracefieldactive;
      strcpy(title1,"Particles");
      strcpy(title2,fieldtrname[ifld2]);
      fmin = globfieldtrmin[ifld2];
      fmax = globfieldtrmax[ifld2];
      if (trfld_nice[ifld2] || trfld_log[ifld2])
        {
         niceflag = trfld_nice[ifld2];
         logflag = trfld_log[ifld2] ;
         fmin = trfld_nice_min[ifld2];
         fmax = trfld_nice_max[ifld2];
        }
      if (colorbarpref == 2) ipref = 1;
     }
   if (ifld2 >= 0) ifld = ifld2;
   if ((cellsflag || celledgesflag || cellmedianedgeflag || cvectorflag ||
        fvectorflag || cellnumflag || cellhiddenedgesflag) && ipref == 0)
     {
      ifld2 = cellfieldactive;
      if (ifld2 >= numvars + cnumvars + fnumvars) ifld2 = -1;
      if (cvectorflag && (ifld2 > -1 && ifld2 < numvars)) ifld2 = -1;
      if (fvectorflag && (ifld2 > -1 && ifld2 < numvars + cnumvars)) ifld2 = -1;
      if (ifld2 >= 0)
        {
         strcpy(title1,"Cells");
         if (colorbarpref == 3) ipref = 1;
         if (ifld2 < numvars)
           {
            strcpy(title2,fieldname[ifld2]);
            fmin = globfieldmin[ifld2];
            fmax = globfieldmax[ifld2];
            if (nfld_nice[ifld2] || nfld_log[ifld2])
              {
               niceflag = nfld_nice[ifld2];
               logflag = nfld_log[ifld2];
               fmin = nfld_nice_min[ifld2];
               fmax = nfld_nice_max[ifld2];
              }
           }
         else if (ifld2 < numvars + cnumvars)
           {
            ifld3 = ifld2 - numvars;
            strcpy(title2,cfieldname[ifld3]);
            fmin = globfieldcmin[ifld3];
            fmax = globfieldcmax[ifld3];
            if (cfld_nice[ifld3] || cfld_log[ifld3])
              {
               niceflag = cfld_nice[ifld3];
               logflag = cfld_log[ifld3];
               fmin = cfld_nice_min[ifld3];
               fmax = cfld_nice_max[ifld3];
              }
           }
         else
           {
            ifld3 = ifld2 - (numvars + cnumvars);
            strcpy(title2,ffieldname[ifld3]);
            fmin = globfieldfmin[ifld3];
            fmax = globfieldfmax[ifld3];
            if (ffld_nice[ifld3] || ffld_log[ifld3])
              {
               niceflag = ffld_nice[ifld3];
               logflag = ffld_log[ifld3];
               fmin = ffld_nice_min[ifld3];
               fmax = ffld_nice_max[ifld3];
              }
           }
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   cutplaneon = 0;
   for (i = 0; i < MAXCUTPLANES+1; i++)
     {
      if (cutplane_on[i] == 1) cutplaneon = 1;
     }
   if (cutplaneon == 1 && cutplanefieldforplane < 0)
      cutplaneon = 0;
   if (cutplaneon == 1 && ipref == 0)
     {
      ifld2 = cutplanefieldforplane;
      strcpy(title1,"Cutplane");
      if (colorbarpref == 4) ipref = 1;
      if (ifld2 < numvars)
        {
         strcpy(title2,fieldname[ifld2]);
         fmin = globfieldmin[ifld2];
         fmax = globfieldmax[ifld2];
         if (nfld_nice[ifld2] || nfld_log[ifld2])
           {
            niceflag = nfld_nice[ifld2];
            logflag = nfld_log[ifld2];
            fmin = nfld_nice_min[ifld2];
            fmax = nfld_nice_max[ifld2];
           }
        }
      else
        {
         ifld3 = ifld2 - numvars;
         strcpy(title2,cfieldname[ifld3]);
         fmin = globfieldcmin[ifld3];
         fmax = globfieldcmax[ifld3];
         if (cfld_nice[ifld3] || cfld_log[ifld3])
           {
            niceflag = cfld_nice[ifld3];
            logflag = cfld_log[ifld3];
            fmin = cfld_nice_min[ifld3];
            fmax = cfld_nice_max[ifld3];
           }
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   cutsphereon = 0;
   for (i = MAXCUTPLANES+1; i < MAXCUTPLANES+MAXCUTSPHERES+2; i++)
     {
      if (cutplane_on[i] == 1) cutsphereon = 1;
     }
   if (cutsphereon == 1 && cutspherefieldforsphere < 0)
      cutsphereon = 0;
   if (cutsphereon == 1 && ipref == 0)
     {
      ifld2 = cutspherefieldforsphere;
      strcpy(title1,"Cutsphere");
      if (colorbarpref == 5) ipref = 1;
      if (ifld2 < numvars)
        {
         strcpy(title2,fieldname[ifld2]);
         fmin = globfieldmin[ifld2];
         fmax = globfieldmax[ifld2];
         if (nfld_nice[ifld2] || nfld_log[ifld2])
           {
            niceflag = nfld_nice[ifld2];
            logflag = nfld_log[ifld2];
            fmin = nfld_nice_min[ifld2];
            fmax = nfld_nice_max[ifld2];
           }
        }
      else
        {
         ifld3 = ifld2 - numvars;
         strcpy(title2,cfieldname[ifld3]);
         fmin = globfieldcmin[ifld3];
         fmax = globfieldcmax[ifld3];
         if (cfld_nice[ifld3] || cfld_log[ifld3])
           {
            niceflag = cfld_nice[ifld3];
            logflag = cfld_log[ifld3];
            fmin = cfld_nice_min[ifld3];
            fmax = cfld_nice_max[ifld3];
           }
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   misosurfon = 0;
   for (i = MAXCONTOURS; i < MAXCONTOURS+MAXMATS; i++)
     {
      if (contour_on[i] == 1 && mcontourfield_draw > -1) 
         misosurfon = 1;
     }
   if (misosurfon == 1 && ipref == 0)
     {
      ifld2 = mcontourfield_draw;
      strcpy(title1,"MIsosurface");
      if (colorbarpref == 6) ipref = 1;
      strcpy(title2,fieldname[ifld2]);
      fmin = globfieldmin[ifld2];
      fmax = globfieldmax[ifld2];
      if (nfld_nice[ifld2] || nfld_log[ifld2])
        {
         niceflag = nfld_nice[ifld2];
         logflag = nfld_log[ifld2];
         fmin = nfld_nice_min[ifld2];
         fmax = nfld_nice_max[ifld2];
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   isosurfon = 0;
   for (i = 0; i < MAXCONTOURS; i++)
     {
      if (contour_on[i] == 1 && contourfield_draw > -1) 
         isosurfon = 1;
     }
   if (isosurfon == 1 && ipref == 0)
     {
      ifld2 = contourfield_draw;
      strcpy(title1,"Isosurface");
      if (colorbarpref == 7) ipref = 1;
      strcpy(title2,fieldname[ifld2]);
      fmin = globfieldmin[ifld2];
      fmax = globfieldmax[ifld2];
      if (nfld_nice[ifld2] || nfld_log[ifld2])
        {
         niceflag = nfld_nice[ifld2];
         logflag = nfld_log[ifld2];
         fmin = nfld_nice_min[ifld2];
         fmax = nfld_nice_max[ifld2];
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   isovolon = 0;
   if (isovol_on == 1 && isovolfield_draw > -1) 
      isovolon = 1;
   if (isovolon == 1 && ipref == 0)
     {
      ifld2 = isovolfield_draw;
      strcpy(title1,"Isovolume");
      if (colorbarpref == 8) ipref = 1;
      strcpy(title2,fieldname[ifld2]);
      fmin = globfieldmin[ifld2];
      fmax = globfieldmax[ifld2];
      if (nfld_nice[ifld2] || nfld_log[ifld2])
        {
         niceflag = nfld_nice[ifld2];
         logflag = nfld_log[ifld2];
         fmin = nfld_nice_min[ifld2];
         fmax = nfld_nice_max[ifld2];
        }
     }
   if (ifld2 >= 0) ifld = ifld2;
   ifld2 = -1;

   if ((surfflag || surfedgesflag || svectorflag || surfnumflag)
       && ipref == 0)
     {
      ifld2 = surffieldactive;
      if (ifld2 >= numvars + snumvars) ifld2 = -1;
      if (svectorflag && (ifld2 > -1 && ifld2 < numvars)) ifld2 = -1;
      if (ifld2 >= 0)
        {
         strcpy(title1,"Surfaces");
         if (colorbarpref == 9) ipref = 1;
         if (ifld2 < numvars)
           {
            strcpy(title2,fieldname[ifld2]);
            fmin = globfieldmin[ifld2];
            fmax = globfieldmax[ifld2];
            if (nfld_nice[ifld2] || nfld_log[ifld2])
              {
               niceflag = nfld_nice[ifld2];
               logflag = nfld_log[ifld2];
               fmin = nfld_nice_min[ifld2];
               fmax = nfld_nice_max[ifld2];
              }
           }
         else
           {
            ifld3 = ifld2 - numvars;
            strcpy(title2,sfieldname[ifld3]);
            fmin = globfieldsmin[ifld3];
            fmax = globfieldsmax[ifld3];
            if (sfld_nice[ifld3] || sfld_log[ifld3])
              {
               niceflag = sfld_nice[ifld3];
               logflag = sfld_log[ifld3];
               fmin = sfld_nice_min[ifld3];
               fmax = sfld_nice_max[ifld3];
              }
           }
        }
     }
   if (ifld2 >= 0) ifld = ifld2;

   if (ifld < 0) return;

   glShadeModel(GL_SMOOTH);
 
   /*  Draw the colorbar rectangles.  */
   x1 = 10.;  x2 = 25.;
   y1 = 20.;  y2 = ysize - 150.;
   y2 = ysize * .7858;

/*
   if ((animationflag == 1 && anim_snapshotsflag == 1) ||
       snapshotsize == 2 || movieflag == 1)
     {
      x1 += 20;
      x2 += 20;
     }
*/

   nsets = 254;
   dy = (y2 - y1) / (float)nsets;
   fcol = (float)nsets / (fmax - fmin);
   df = (fmax - fmin) / (float)nsets;

   /*  First draw the lower boundary color.  */
   colr = (float)(jcolor[0]>>24 & 255) / 255.;
   colg = (float)(jcolor[0]>>16 & 255) / 255.;
   colb = (float)(jcolor[0]>>8 & 255) / 255.;
   glColor3f(colr,colg,colb);
   glBegin(GL_POLYGON);
   yp1 = y1;  yp2 = y1 - dy*3.;
   v1[0] = x1;
   v1[1] = yp1;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x2;
   v1[1] = yp1;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x2;
   v1[1] = yp2;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x1;
   v1[1] = yp2;
   v1[2] = 0.;
   glVertex3fv(v1);
   glEnd();

   for (i = 0; i < nsets+1; i++)
     {
      yp1 = y1 + (dy * i);
      yp2 = y1 + (dy * (i + 1));
      f1 = fmin + (df * i);
      f2 = fmin + (df * (i + 1));
      if (i == nsets)
        {
         f2 = fmax;
         yp2 = y2;
        }
      glBegin(GL_POLYGON);

      icol = (int) ((f1 - fmin) * fcol);
      if (i == 0) icol = 0;
      icol++;
      v1[0] = x1;
      v1[1] = yp1;
      v1[2] = 0.;
      colr = (float)(jcolor[icol]>>24 & 255) / 255.;
      colg = (float)(jcolor[icol]>>16 & 255) / 255.;
      colb = (float)(jcolor[icol]>>8 & 255) / 255.;
      glColor3f(colr,colg,colb);
      glVertex3fv(v1);

      v1[0] = x2;
      v1[1] = yp1;
      v1[2] = 0.;

      glVertex3fv(v1);

      icol = (int) ((f2 - fmin) * fcol) + 1;
      if (i == nsets) icol = 254;
      v1[0] = x2;
      v1[1] = yp2;
      v1[2] = 0.;

      colr = (float)(jcolor[icol]>>24 & 255) / 255.;
      colg = (float)(jcolor[icol]>>16 & 255) / 255.;
      colb = (float)(jcolor[icol]>>8 & 255) / 255.;
      glColor3f(colr,colg,colb);

      glVertex3fv(v1);

      v1[0] = x1;
      v1[1] = yp2;
      v1[2] = 0.;

      colr = (float)(jcolor[icol]>>24 & 255) / 255.;
      colg = (float)(jcolor[icol]>>16 & 255) / 255.;
      colb = (float)(jcolor[icol]>>8 & 255) / 255.;
      glColor3f(colr,colg,colb);
      glVertex3fv(v1);
      glEnd();
     }

   /*  Now draw the upper boundary color.  */
   colr = (float)(jcolor[255]>>24 & 255) / 255.;
   colg = (float)(jcolor[255]>>16 & 255) / 255.;
   colb = (float)(jcolor[255]>>8 & 255) / 255.;
   glColor3f(colr,colg,colb);
   glBegin(GL_POLYGON);
   yp1 = y2;  yp2 = y2 + dy*3.;
   v1[0] = x1;
   v1[1] = yp1;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x2;
   v1[1] = yp1;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x2;
   v1[1] = yp2;
   v1[2] = 0.;
   glVertex3fv(v1);
   v1[0] = x1;
   v1[1] = yp2;
   v1[2] = 0.;
   glVertex3fv(v1);
   glEnd();

   /*  Annotate the colorbar data.  */
   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   /*  Call getscale to get the number of intervals.  */
   getscale(fmin, fmax, nset_contours, niceflag, logflag,
            intervals, &nintervals);
   nsets = nintervals;
   dy = (y2 - y1) / (float)(nsets-1);

   for (i = 0; i < nsets; i++)
     {
      y = y1 + (dy * i);
      flab = intervals[i];
      if (i == nsets-1)
        {
         flab = fmax;
         y = y2;
        }
      sprintf(str,"%.7g",flab);
      v1[0] = x2;
      v1[1] = y;
      v1[2] = 0.;
      glBegin(GL_LINE_STRIP);
      glVertex3fv(v1);
      v1[0] = x2 + 10.;
      glVertex3fv(v1);
      glEnd();
      glRasterPos3f(x2+12,y-5.,0.);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
     }

   /*  Annotate the colorbar title.  */
   if (colorbartitleflag == 0)
     {
      strcpy(str,title1);
      strcpy(str2," ");
      strcat(str2,title2);
      if (logflag) sprintf(str2," log %s",title2);
      strcat(str,str2);
     }
   else strcpy(str,clrbartitlestr);
   glRasterPos3f(x1-5.,y2+27.,0.);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset);
   glCallLists(strlen(str),GL_UNSIGNED_BYTE,(GLubyte *)str);
   glRasterPos3f(x1-5.,y2+8.,0.);
   glPopAttrib ();

   glShadeModel(GL_FLAT);
   glShadeModel(GL_SMOOTH);
}
