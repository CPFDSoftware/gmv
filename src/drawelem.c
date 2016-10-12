
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
 
#include "gmvdata.h"
#include "lights.h"

#define MAXELEMS 10000
#define P_S_ARRAYSIZE 50100
float *p_s_data;
FILE *p_s_out;

 
void drawtriangles(int refl, int nlines, short nttypes[6], 
                   struct trielem *drawtris, int ntris)
{
  int i, j, k, ii, matnum, itype, icol[3];
  float n[3];
  double v[3], tx[3], ty[3], tz[3];
  int incr, ic1, fldflag, tricount, texture_on = 0;
  float colr, colg, colb, texcol;
  void getrgbcol(int ic1, float *colr, float *colg, float *colb);
  GLfloat rgba[4], specular[4] = {0.,0.,0.,1.}, 
          shine[1] = {0.};

   incr = 1;
   if (fastdraw == 1)
     {
      /* incr = 3 * drawincr; */
      incr = drawincr;
      if (drawincr == 1) incr = 1;
     }

   /*  If edges or contours are drawn, z-offset the face.  */
   if (poly_offset > 0 && nlines > 0)
     {
#if GL_VERSION_1_1
      if (poly_offset == 2)
         glEnable(GL_POLYGON_OFFSET_FILL);
#elif GL_EXT_polygon_offset
      if (poly_offset == 1)
         glEnable(GL_POLYGON_OFFSET_EXT);
#endif
     }

   /* shine[0] = 10.; */

   /*  Loop over the triangle types.  */
   for (ii = 0; ii < LIT_SMOOTH1SIDE+1; ii++)
     {

      /*  Skip types that are not drawn.  */
      if (nttypes[ii] == 0) continue;

      /*  Set up shade and lighting depending on itype.  */
      glShadeModel(GL_FLAT);
      if (ii == UNLIT_SMOOTH || ii == LIT_SMOOTH || LIT_SMOOTH1SIDE)
         glShadeModel(GL_SMOOTH);
      glDisable(GL_LIGHTING);
      if (ii > UNLIT_SMOOTH)
        {
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, specular);
         glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
         glEnable(GL_COLOR_MATERIAL);
         glEnable(GL_LIGHTING);
         if (ii == LIT_FLAT || ii == LIT_SMOOTH)
           {
            glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
           }
         else
            glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
         /* glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); */
        }

      /*  If smooth field data and texture  */ 
      /*  requested, enable texture.        */
      if (textureflag && (ii == UNLIT_SMOOTH || ii == LIT_SMOOTH ||
          ii == LIT_SMOOTH1SIDE))
        {
         glEnable(GL_TEXTURE_1D);
         if (ii == UNLIT_SMOOTH)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
         else
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
         texture_on = 1;
        }

      /*  Loop over the triangles.  */
      tricount = 0;
      glBegin(GL_TRIANGLES);
      for (i = 0; i < ntris; i += incr)
        {
         itype = drawtris[i].itype;
         if (itype == ii)
           {

            /*  Determine colors to use from icr1.  */
            ic1 = drawtris[i].iclr1;
            if (ic1 < MAXMATS) fldflag = 0;
            else if (ic1 >= MAXMATS + 300) fldflag = 2;
            else fldflag = 1;
            if (ic1 < 0) fldflag = ic1;

            /*  Set colors for flat lit images.  */
            if (itype == LIT_FLAT || itype == LIT_FLAT1SIDE)
              {

               /*  If used, set material color to iclr1.  */
               if (fldflag == 0)
                 {
                  matnum = ic1;
                  for (j = 0; j < 3; j++)
                    {
                     /* rgba[j] = matrgba[4*matnum+j] * .6; */
                     rgba[j] = matrgba[4*matnum+j];
                    }
                  rgba[3] = matrgba[4*matnum+3];
                 }

               /*  If used, set contour color to iclr1-200.  */
               if (fldflag == 1)
                 {
                  matnum = ic1 - MAXMATS - 200;
                  if (matnum == MAXCONTOURS+MAXMATS)
                     matnum = MAXCONTOURS;
                  if (matnum == MAXCONTOURS+MAXMATS + 1)
                     matnum = MAXCONTOURS + 1;

                  for (j = 0; j < 3; j++)
                    {
                     /* rgba[j] = conrgba[4*matnum+j] * .6; */
                     rgba[j] = conrgba[4*matnum+j];
                    }
                  rgba[3] = conrgba[4*matnum+3];
                 }

               /*  If used, set flat field color iclr1 - 400.  */
               if (fldflag == 2 || fldflag < 0)
                 {
                  getrgbcol(ic1, &colr, &colg, &colb);
                  rgba[0] = colr;
                  rgba[1] = colg;
                  rgba[2] = colb;
                  rgba[3] = 1.;
                 }
               glColor4fv(rgba);
              }

            /*  Set colors for unlit flat shading.  */
            if (itype == UNLIT_FLAT)
              {
               getrgbcol(ic1, &colr, &colg, &colb); 
               glColor3f(colr,colg,colb);
              }

            /*  If lit, get the triangle normal.  */
            if (itype > UNLIT_SMOOTH)
                 {     
               if (refl == 0) 
                 {
                  n[0] = drawtris[i].xnorm;
                  n[1] = drawtris[i].ynorm;
                  n[2] = drawtris[i].znorm;
                 } 
               else
                 {
                  n[0] = - drawtris[i].xnorm;
                  n[1] = - drawtris[i].ynorm;
                  n[2] = - drawtris[i].znorm;
                 } 
               glNormal3fv(n);
              }

            /*  Set up triangle arrays and loop over vertices.  */
            tx[0] = drawtris[i].x1;
            tx[1] = drawtris[i].x2;
            tx[2] = drawtris[i].x3;
            ty[0] = drawtris[i].y1;
            ty[1] = drawtris[i].y2;
            ty[2] = drawtris[i].y3;
            tz[0] = drawtris[i].z1;
            tz[1] = drawtris[i].z2;
            tz[2] = drawtris[i].z3;
            icol[0] = (int)drawtris[i].iclr1;
            icol[1] = (int)drawtris[i].iclr2;
            icol[2] = (int)drawtris[i].iclr3;
            for (k = 0; k < 3; k++)
              {
               v[0] = tx[k];
               v[1] = ty[k];
               v[2] = tz[k];

               /*  If surface is colored by      */
               /*  field values, set the color.  */
               if ((fldflag == 2) && 
                   (itype == UNLIT_SMOOTH || itype == LIT_SMOOTH ||
                   itype == LIT_SMOOTH1SIDE)) 
                 {
                  ic1 = icol[k];
                  if (texture_on)
                    {
                     if (itype == LIT_SMOOTH || itype == LIT_SMOOTH1SIDE)
                       {
                        rgba[0] = .9;
                        rgba[1] = .9;
                        rgba[2] = .9;
                        rgba[3] = 1.;
                        glColor4fv(rgba);
                       }
                     texcol = (float)(ic1 - MAXMATS - 400 + 1) / 255.;
                     glTexCoord1f(texcol);
                    }
                  else
                    {
                     getrgbcol(ic1, &colr, &colg, &colb);

                     if (itype == LIT_SMOOTH || itype == LIT_SMOOTH1SIDE)
                       {
                        rgba[0] = colr;
                        rgba[1] = colg;
                        rgba[2] = colb;
                        rgba[3] = 1.;
                        glColor4fv(rgba);
                       }
                     else glColor3f(colr,colg,colb);
                    }
                 }

               glVertex3dv(v);
              }
            tricount++;
            if (tricount > MAXELEMS)
              {
               tricount = 0;
               glEnd();
               glBegin(GL_TRIANGLES);
              } 
           }
        }
      glEnd(); 
      if (ii > UNLIT_SMOOTH)
         glDisable(GL_COLOR_MATERIAL);
      if (texture_on)
        {
         texture_on = 0;
         glDisable(GL_TEXTURE_1D);
        }
     }

   /*  Reset polgon z-offset.  */
   if (poly_offset > 0 && nlines > 0)
     {
#if GL_VERSION_1_1
      if (poly_offset == 2)
         glDisable(GL_POLYGON_OFFSET_FILL);
#elif GL_EXT_polygon_offset
      if (poly_offset == 1)
         glDisable(GL_POLYGON_OFFSET_EXT);
#endif
     }
}

 
void drawitplv(int npts, int nlines,  struct plvelem *drawplv, int nvects, 
               int nplv, int hiddenflag)
{
  int i, j, l, ii, itype, texture_on = 0;
  int  p_s_run, p_s_size;
  double v[3];
  int incr, ic1, ic2, linecount, nsphpts;
  float colr, colg, colb, colr2, colg2, colb2, texcol;
  float pt_size;
  double v1[3], fac, speed, maxspd, minspd, xv, yv, zv, uv, vv, wv, val;
  double a[4][3], xn, yn, zn, t, x1, y1, z1, vlogoffset;
  double x2, y2, z2, x3, y3, z3, theta, xa, ya, d, radius;
  double xns[500], yns[500], zns[500];
  double d90 = 1.570796327;
  long ival, ival2;
  char str[30];
  int nregpts, ntsphere, nnums, nlsolid, nlsmooth, nthist;
  static int nregpts0, ntsphere0, nnums0, nlsolid0, nlsmooth0, nthist0;
  static int nregpts1, ntsphere1, nnums1, nlsolid1, nlsmooth1, nthist1;
  void getrgbcol(int ic1, float *colr, float *colg, float *colb);
  void p_s_header(), p_s_doit();
  GLUquadric *myquad;
  GLfloat rgba[4], specular[4] = {.5,.5,.5,1.},
          shine[1] = {10.};
  GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

   incr = 1;
   if (fastdraw == 1)
     {
      incr = 3 * drawincr;
      if (drawincr == 1) incr = 1;
     }
   glShadeModel(GL_FLAT);
   glDisable(GL_LIGHTING);

   /*  First determine the number of the different types  */ 
   /*  of points, numbers, lines and vectors to draw.     */
   if (drawnew3dflag && hiddenflag == 0)
     {
      nregpts0 = 0;  ntsphere0 = 0;  nnums0 = 0;  nlsolid0 = 0;  nlsmooth0 = 0;
      nthist0 = 0;
      for (i = 0; i < nplv; i++)
        {
         itype = drawplv[i].itype;
         if (itype == PONLY || itype == PANDNUM || itype == TPOINT) nregpts0++;
         if (itype == SPHERE || itype == TSIZE) ntsphere0++;
         if (itype == PANDNUM || itype == NUMONLY || itype == TNUMVAL) nnums0++;
         if (itype == LSOLID) nlsolid0++;
         if (itype == LSMOOTH || itype == THIST) nlsmooth0++;
         if (itype == THIST) nthist0++;
        }
     }
   if (drawnew3dflag && hiddenflag == 1)
     {
      nregpts1 = 0;  ntsphere1 = 0;  nnums1 = 0;  nlsolid1 = 0;  nlsmooth1 = 0;
      nthist1 = 0;
      for (i = 0; i < nplv; i++)
        {
         itype = drawplv[i].itype;
         if (itype == PONLY || itype == PANDNUM || itype == TPOINT) nregpts1++;
         if (itype == SPHERE || itype == TSIZE) ntsphere1++;
         if (itype == PANDNUM || itype == NUMONLY || itype == TNUMVAL) nnums1++;
         if (itype == LSOLID) nlsolid1++;
         if (itype == LSMOOTH || itype == THIST) nlsmooth1++;
         if (itype == THIST) nthist1++;
        }
     }

   /*  Set the type counts depending on the hidden line flag.  */
   if (hiddenflag == 0)
     {
      nregpts = nregpts0;  ntsphere = ntsphere0;  nnums = nnums0;  
      nlsolid = nlsolid0;  nlsmooth = nlsmooth0;  nthist = nthist0;
     }
   if (hiddenflag == 1)
     {
      nregpts = nregpts1;  ntsphere = ntsphere1;  nnums = nnums1;  
      nlsolid = nlsolid1;  nlsmooth = nlsmooth1;  nthist = nthist1;
     }

   /*  Draw regular and large tracer history points.  */
   if (nregpts > 0 || nthist > 0)
     {
      glBegin(GL_POINTS);

      for (ii = 0; ii < 2; ii++)
        {
         if (ii == 0 && nregpts == 0) continue;
         if (ii == 1 && nthist == 0) continue;
         if (ii == 1)
           {
            pt_size = line_size + 3;
            glPointSize(pt_size);
           }
         for (i = 0; i < nplv; i += incr)
           {
            itype = drawplv[i].itype;
            if ((ii == 0 && (itype == PONLY || itype == PANDNUM || 
                             itype == TPOINT)) ||
                (ii == 1 && itype == THIST))
              {

               /*  Set colors.  */
               ic1 = drawplv[i].iclr1;
               getrgbcol(ic1, &colr, &colg, &colb);
               glColor3f(colr,colg,colb);

               v[0] = drawplv[i].x1;
               v[1] = drawplv[i].y1;
               v[2] = drawplv[i].z1;

               if (ii == 1 && itype == THIST)
                 {
                  v[0] = drawplv[i].x2;
                  v[1] = drawplv[i].y2;
                  v[2] = drawplv[i].z2;
                 }
               glVertex3dv(v);
              }
           }
         if (ii == 1)
           {
            pt_size = (float)pnt_size;
            glPointSize(pt_size);
           }
        }
      glEnd();
     }

   /*  Draw numbers.  */
   if (nnums > 0)
     {
      for (i = 0; i < nplv; i += incr)
        {
         itype = drawplv[i].itype;
         if (itype == PANDNUM || itype == NUMONLY || itype == TNUMVAL)
           {

            /*  Set colors.  */
            ic1 = drawplv[i].iclr1;
            getrgbcol(ic1, &colr, &colg, &colb);
            glColor3f(colr,colg,colb);

            v[0] = drawplv[i].x1;
            v[1] = drawplv[i].y1;
            v[2] = drawplv[i].z1;

            if (itype == TNUMVAL)
              {
               val = drawplv[i].x2;
               sprintf(str,"%g",val);
              }
            else
              {
               ival = drawplv[i].x2;
               sprintf(str,"%ld",ival);
              }
#ifdef MPI
            if (local_idflag && itype != TNUMVAL)
              {
               ival2 = drawplv[i].y2;
               sprintf(str,"%ld-%ld",ival,ival2);
              }
#endif
            glRasterPos3d(v[0], v[1], v[2]);  
            glPushAttrib (GL_LIST_BIT);
            glListBase(fontOffset);
            glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
            glPopAttrib ();
           }
        }
     }

   /*  Draw points as spheres.  */
   if (ntsphere > 0)
     {
      /*  Get sphere normals for a 10x10 sphere.  */
      nsphpts = getSphereNorms(10, xns, yns, zns);

      /*  Set up lighting and shading.  */
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, shine);
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, specular);
      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.);
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glShadeModel(GL_SMOOTH);

      val = sqrt(windowwidth*windowwidth + windowheight*windowheight);
      d = pnt_size * ((double)span / val);
      for (i = 0; i < nplv; i += incr)
        {
         itype = drawplv[i].itype;
         if (itype == SPHERE || itype == TSIZE)
           {

            /*  Set colors.  */
            ic1 = drawplv[i].iclr1;
            getrgbcol(ic1, &colr, &colg, &colb);
            //glColor3f(colr,colg,colb);
            rgba[0] = colr;
            rgba[1] = colg;
            rgba[2] = colb;
            rgba[3] = 1.;
            glColor4fv(rgba);

            v[0] = drawplv[i].x1;
            v[1] = drawplv[i].y1;
            v[2] = drawplv[i].z1;

            if (itype == SPHERE)
               radius = d;
            if (itype == TSIZE)
               radius = drawplv[i].y2;
            radius /= (double)magnify;
            //CreateSphere(v,radius,10);

            /*  Generate sphere points using normals and draw the sphere.  */
            glBegin(GL_TRIANGLE_STRIP);
            for (j = 0; j < nsphpts; j++)
              {
               xv = v[0] + radius * xns[j];
               yv = v[1] + radius * yns[j];
               zv = v[2] + radius * zns[j];
               glNormal3d(xns[j],yns[j],zns[j]);
               glVertex3d(xv,yv,zv);
              }
            glEnd();
           }
        }
      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);
     }

   p_s_run = 0;
   if (p_s_flag == 1 && (nlsolid > 0 || nlsmooth > 0))
     {
      p_s_run = 1;
      p_s_data = (float *)malloc(P_S_ARRAYSIZE*sizeof(float));
      glFeedbackBuffer(P_S_ARRAYSIZE, GL_2D, p_s_data);
      p_s_out = fopen(snapshotfilnam,"w");
      p_s_header();
      glRenderMode(GL_FEEDBACK);
     }

   /*  Draw solid lines.  */
   linecount = 0;
   if (nlsolid > 0)
     {
      glShadeModel(GL_FLAT);
      glBegin(GL_LINES);
      for (i = 0; i < nplv; i += incr)
        {
         itype = drawplv[i].itype;
         if (itype == LSOLID)
           {

            /*  Set colors.  */
            ic1 = drawplv[i].iclr1;
            getrgbcol(ic1, &colr, &colg, &colb);
            glColor3f(colr,colg,colb);

            v[0] = drawplv[i].x1;
            v[1] = drawplv[i].y1;
            v[2] = drawplv[i].z1;
            glVertex3dv(v);

            v[0] = drawplv[i].x2;
            v[1] = drawplv[i].y2;
            v[2] = drawplv[i].z2;
            glVertex3dv(v);

            linecount++;
            if (linecount > MAXELEMS)
              {
               glEnd();
               if (p_s_flag == 1)
                 {
                  p_s_size = glRenderMode(GL_RENDER);
                  p_s_doit(p_s_size);
                  glRenderMode(GL_FEEDBACK);
                 }
               linecount = 0;
               glBegin(GL_LINES);
              }
           }
        }
      glEnd();

      if (p_s_flag == 1 && linecount > 0)
        {
         p_s_size = glRenderMode(GL_RENDER);
         p_s_doit(p_s_size);
         glRenderMode(GL_FEEDBACK);
        }  
     }

   /*  Draw smooth lines.  */
   linecount = 0;
   if (nlsmooth > 0)
     {
      glShadeModel(GL_SMOOTH);

      /*  Set texure on if requested.  */
      if (textureflag)
        {
         glEnable(GL_TEXTURE_1D);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
         texture_on = 1;
        }
      glBegin(GL_LINES);
      for (i = 0; i < nplv; i += incr)
        {
         itype = drawplv[i].itype;
         if (itype == LSMOOTH || itype == THIST)
           {

            /*  Set color and vertex for first point.  */
            ic1 = drawplv[i].iclr1;
            if (texture_on)
              {
               texcol = (float)(ic1 - MAXMATS - 400 + 1) / 255.;
               glTexCoord1f(texcol);
              }
            else
              {
               getrgbcol(ic1, &colr, &colg, &colb);
               glColor3f(colr,colg,colb);
              }
            v[0] = drawplv[i].x1;
            v[1] = drawplv[i].y1;
            v[2] = drawplv[i].z1;
            glVertex3dv(v);

            /*  Set color and vertex for second point.  */
            ic2 = drawplv[i].iclr2;
            if (texture_on)
              {
               texcol = (float)(ic2 - MAXMATS - 400 + 1) / 255.;
               glTexCoord1f(texcol);
              }
            else
              {
               getrgbcol(ic2, &colr2, &colg2, &colb2);
               glColor3f(colr2,colg2,colb2);
              }
            v[0] = drawplv[i].x2;
            v[1] = drawplv[i].y2;
            v[2] = drawplv[i].z2;
            glVertex3dv(v);

            linecount++;
            if (linecount > MAXELEMS)
              {
               glEnd();
               if (p_s_flag == 1)
                 {
                  p_s_size = glRenderMode(GL_RENDER);
                  p_s_doit(p_s_size);
                  glRenderMode(GL_FEEDBACK);
                 }
               linecount = 0;
               glBegin(GL_LINES);
              }
           }
        }
      glEnd();

      if (texture_on)
        {
         texture_on = 0;
         glDisable(GL_TEXTURE_1D);
        }

      if (p_s_flag == 1 && linecount > 0)
        {
         p_s_size = glRenderMode(GL_RENDER);
         p_s_doit(p_s_size);
         glRenderMode(GL_FEEDBACK);
        }  
     }
       
   if (p_s_run == 1)
     {
      free(p_s_data);
      p_s_run = 0;
      p_s_flag = 0;
      fprintf(p_s_out,"%s\n"," stroke restore restore showpage");
      strcpy(str,"%%Trailer");
      fprintf(p_s_out,"%s\n",str);
      strcpy(str,"%%Pages:     1");
      fprintf(p_s_out,"%s\n",str);
      fclose(p_s_out);
      glRenderMode(GL_RENDER);
     }

   /*  Draw vectors.  */
   if (nvects > 0)
     {
      glShadeModel(GL_FLAT);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      d = span/(magnify*150.);

      for (i = 0; i < nplv; i += incr)
        {
         itype = drawplv[i].itype;
         if (itype == NVECTOR || itype == CVECTOR || itype == SVECTOR ||
             itype == FVECTOR)
           {

            /*  Set colors.  */
            ic1 = drawplv[i].iclr1;
            getrgbcol(ic1, &colr, &colg, &colb);
            glColor3f(colr,colg,colb);

            vlogoffset = 0;
            if (vectorlog)
              {
               if (itype == NVECTOR)
                  minspd = fieldmin[speedfld];
               else if (itype == CVECTOR)
                  minspd = cfieldmin[cspeedfld];
               else if (itype == FVECTOR)
                  minspd = ffieldmin[fspeedfld];
               else minspd = sfieldmin[sspeedfld];
               if (minspd < .00001) minspd = .00001;
               minspd = log10(minspd);
               if (minspd < 0) vlogoffset = fabs(minspd);
              }

            if (itype == NVECTOR)
               maxspd = maxspeed;
            else if (itype == CVECTOR)
               maxspd = cmaxspeed;
            else if (itype == FVECTOR)
               maxspd = fmaxspeed;
            else maxspd = smaxspeed;
            if (vectorlog) maxspd = log10(maxspd) + vlogoffset;
            fac = velscale*span/(maxspd*20.);

            xv = drawplv[i].x1;
            yv = drawplv[i].y1;
            zv = drawplv[i].z1;
            uv = drawplv[i].x2;
            vv = drawplv[i].y2;
            wv = drawplv[i].z2;

            speed = sqrt((uv*uv) + (vv*vv) + (wv*wv));
            if (vectorlog) speed = log10(speed) + vlogoffset;
            t = speed * fac;
            if (t > .00001) 
              {
               glBegin(GL_LINE_STRIP);
        
               v1[0] = xv;
               v1[1] = yv;
               v1[2] = zv;  

               glVertex3dv(v1);

               t = sqrt(uv*uv + vv*vv + wv*wv);
               xn = uv / t;  yn = vv / t;  zn = wv / t;
 
               v1[0] += xn * speed * fac;
               v1[1] += yn * speed * fac;
               v1[2] += zn * speed * fac;

               glVertex3dv(v1);
               if (vectorarrow)
                 {
                  glEnd();
                 }
        
               /*  Draw a pyramid as the arrowhead, if requested.  */
               if (vectorarrow)
                 {
                  x2 = v1[0];  y2 = v1[1];  z2 = v1[2];
                  x3 = x2 - xn * 2.5*d * velarrowsize;
                  y3 = y2 - yn * 2.5*d * velarrowsize;
                  z3 = z2 - zn * 2.5*d * velarrowsize;

                  /*  Project a normal line along the x-y plane.  */
                  if (fabs(y2-yv) > 1.0e-10)
                    {
                     theta = atan(-(x2-xv)/(y2-yv));
                     if (fabs(x2-xv) <= 1.0e-10) theta = 0;
                    }
                  else theta = d90;
                  xa = d*cos(theta) * velarrowsize;
                  ya = d*sin(theta) * velarrowsize;
                  a[0][0] = x3 - xa;
                  a[0][1] = y3 - ya;
                  a[0][2] = z3;
                  a[2][0] = x3 + xa;
                  a[2][1] = y3 + ya;
                  a[2][2] = z3;

                  /*  Now find a normal to the new plane.  */
                  x1 = a[0][0];  y1 = a[0][1];  z1= a[0][2];
                  xn = (y2-y1)*(z3-z1) - (y3-y1)*(z2-z1);
                  yn = (z2-z1)*(x3-x1) - (z3-z1)*(x2-x1);
                  zn = (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1);
                  t = sqrt(xn*xn + yn*yn + zn*zn);
                  xn /= t; 
                  yn /= t; 
                  zn /= t; 

                  a[1][0] = x3 + xn * d * velarrowsize;
                  a[1][1] = y3 + yn * d * velarrowsize;
                  a[1][2] = z3 + zn * d * velarrowsize;
                  a[3][0] = x3 - xn * d * velarrowsize;
                  a[3][1] = y3 - yn * d * velarrowsize;
                  a[3][2] = z3 - zn * d * velarrowsize;

                  v1[0] = x2;  v1[1] = y2;  v1[2] = z2; 
                  for (l = 0; l < 4; l++)
                    {
                     j = l + 1;
                     if (j == 4) j = 0;
                     glBegin(GL_POLYGON);
                     glVertex3dv(v1);
                     glVertex3dv(a[l]);
                     glVertex3dv(a[j]);
                     glEnd();
                    }

                  /*  Now draw the base of the pyramid in textcolor.  */
                  if (txtcolor < 256) glColor3f(0.,0.,0.);
                  else glColor3f(1.,1.,1.);
                  glBegin(GL_POLYGON);
                  for (l = 0; l < 4; l++)
                    {
                     glVertex3dv(a[l]);
                    }
                  glEnd();
                 }

               /*  Draw textcolor tip, if requested.  */
               if (!vectorarrow)
                 {
                  if (txtcolor < 256) glColor3f(0.,0.,0.);
                  else glColor3f(1.,1.,1.);

                  v1[0] += xn * speed * fac * 0.1;
                  v1[1] += yn * speed * fac * 0.1;
                  v1[2] += zn * speed * fac * 0.1;
                  glVertex3dv(v1);
                  glEnd();
                 }
              }
           }
        }
     }
}


void getrgbcol(int ic1, float *rcolr, float *rcolg, float *rcolb)
{
  int fldflag, matnum;
  float colr, colg, colb;

   if (ic1 < MAXMATS) fldflag = 0;
   else if (ic1 >= MAXMATS + 300) fldflag = 2;
   else fldflag = 1;
   if (ic1 < 0) fldflag = ic1;

   /*  If used, set material color to iclr1.  */
   if (fldflag == 0)
     {
      matnum = ic1;
      colr = matrgba[4*matnum];
      colg = matrgba[4*matnum+1];
      colb = matrgba[4*matnum+2];
     }

   /*  If used, set contour color to iclr1.  */
   if (fldflag == 1)
     {
      matnum = ic1 - MAXMATS - 200;
      colr = conrgba[4*matnum];
      colg = conrgba[4*matnum+1];
      colb = conrgba[4*matnum+2];
     }

   /*  If used, set field color to iclr1.  */
   if (fldflag == 2 || fldflag < 0)
     {
      if (fldflag == 2)
        {
         ic1 = ic1 - MAXMATS - 400;
         if (ic1 < 0)
           {
             ic1 = 0;
           }
         else if (ic1 > 255)
           {
             ic1 = 255;
           }
         colr = (float)(jcolor[ic1]>>24 & 255) / 255.;
         colg = (float)(jcolor[ic1]>>16 & 255) / 255.;
         colb = (float)(jcolor[ic1]>>8 & 255) / 255.;
        }
      else if (fldflag == -1) /* Black */
        {
         colr = colg = colb = 0.;
        }
      else if (fldflag == -2) /* White */
        {
         colr = colg = colb = 1.;
        }
      else if (fldflag == -3) /* Grey */
        {
         colr = colg = colb = .3;
        }
      else if (fldflag == -4) /* Red */
        {
         colr = 1.;
         colg = 0.;
         colb = 0.;
        }
      else if (fldflag == -5) /* Green */
        {
         colr = 0.;
         colg = 1.;
         colb = 0.;
        }
      else if (fldflag == -6) /* Yellow */
        {
         colr = 1.;
         colg = 1.;
         colb = 0.;
        }
      else if (fldflag == -7) /* Blue */
        {
         colr = 0.;
         colg = 0.;
         colb = 1.;
        }
      else if (fldflag == -10) /* Below scale */
        {
         colr = 0.392;
         colg = 0.1;
         colb = 0.79;
        }
      else if (fldflag == -11) /* Above scale */
        {
         colr = 1.;
         colg = 0.1;
         colb = 0.67;
        }
     }
   *rcolr = colr;
   *rcolg = colg;
   *rcolb = colb;
}

 
void p_s_header()
{
 
 char *ast, tempstr[100];
 time_t tt;
 
   strcpy(tempstr,"%! PS-Adobe-1.0");
   fprintf(p_s_out,"%s\n",tempstr);
   strcpy(tempstr,"%%Title: printfile");
   fprintf(p_s_out,"%s\n",tempstr);
   strcpy(tempstr,"%%Creator: Pzip");
   fprintf(p_s_out,"%s\n",tempstr);
   strcpy(tempstr,"%%CreationDate: ");
   tt = time(NULL);
   ast = (char *)asctime(localtime(&tt));
   strcat(tempstr,ast);
   fprintf(p_s_out,"%s",tempstr);
   strcpy(tempstr,"%%Pages: 1");
   fprintf(p_s_out,"%s\n",tempstr);
   strcpy(tempstr,"%%EndComments");
   fprintf(p_s_out,"%s\n",tempstr);
   fprintf(p_s_out,"%s\n"," /inch {72 mul} bind def /m1 matrix currentmatrix def /M /moveto load def /L");
   fprintf(p_s_out,"%s\n"," /lineto load def /R /rlineto load def /T /show load def /U {currentpoint stroke");
   fprintf(p_s_out,"%s\n"," moveto} bind def /AW {maxwid minwid sub mul minwid add xyscale div");
   fprintf(p_s_out,"%s\n"," setlinewidth} bind def /W {maxwid minwid sub mul minwid add 32767 mul pprmax");
   fprintf(p_s_out,"%s\n"," div setlinewidth} bind def /X {currentpoint currentlinewidth dup -2.0 div 0");
   fprintf(p_s_out,"%s\n"," rmoveto 0 rlineto moveto} bind def /AFONTDEF {xyscale div exch findfont exch");
   fprintf(p_s_out,"%s\n"," scalefont setfont} bind def /FONTDEF {32767 mul pprmax div exch findfont exch");
   fprintf(p_s_out,"%s\n"," scalefont setfont} bind def /TRANSFORM {xoffset yoffset translate xyscale");
   fprintf(p_s_out,"%s\n"," xyscale scale} bind def /LANDSCAPE {90 rotate 0 pprmin neg translate} bind def");
   fprintf(p_s_out,"%s\n"," /pprmin 8.5000 inch def /pprmax 11.0000 inch def /minwid 0.0034 inch def");
   fprintf(p_s_out,"%s\n"," /maxwid 0.0420 inch def /J {1 setlinejoin} bind def /colorimage where {pop /C");
   fprintf(p_s_out,"%s\n"," /setrgbcolor load def} {/C {2 copy eq {3 copy pop eq {setrgbcolor} {pop pop pop");
   fprintf(p_s_out,"%s\n"," 0 setgray} ifelse} {pop pop pop 0 setgray} ifelse} def } ifelse");
   strcpy(tempstr,"%%EndProlog");
   fprintf(p_s_out,"%s\n",tempstr);
   strcpy(tempstr,"%%Page ? 1");
   fprintf(p_s_out,"%s\n",tempstr);
   fprintf(p_s_out,"%s\n"," save J 0.00 0.00 0.00 C 0.00 0.00 0.00 C 0.00 W 0 0 M");
   fprintf(p_s_out,"%s\n"," save J 0.00 0.00 0.00 C 0.00 0.00 0.00 C 0.00 W /xoffset 0.6475 inch def");
   fprintf(p_s_out,"%s\n"," /yoffset 0.5000 inch def /xyscale 0.2289E-03 inch def m1 setmatrix LANDSCAPE");
   fprintf(p_s_out,"%s\n"," TRANSFORM /Courier 10 FONTDEF");
 
}
 
void p_s_doit(size)
 int size;
{
  int count;
  float token; 
  int i,ix,iy,ix2,iy2;
  char str1[80],str2[40];
 
   if (size < 0)
     {
      printf("Size < 0\n");
      return;
     }
   i = 0;
   count = size;
   while (count)
     {
      token = p_s_data[size-count]; count--;
      if (token == GL_PASS_THROUGH_TOKEN)
        {
	 count--;
	}
      else if (token == GL_POINT_TOKEN)
        {
         count -= 2;
	}
      else if (token == GL_LINE_TOKEN)
        {
         ix = (int)(p_s_data[size-count]*40.0);
         iy = (int)(p_s_data[size-count+1]*40.0);
         sprintf(str2," %d",ix);
         strcpy(str1,str2);
         sprintf(str2, " %d M",iy);
         strcat(str1,str2);
         fprintf(p_s_out,"%s",str1);
         ix2 = (int)(p_s_data[size-count+2]*40.0);
         iy2 = (int)(p_s_data[size-count+3]*40.0);
         sprintf(str2," %d",ix2-ix);
         strcpy(str1,str2);
         sprintf(str2, " %d R",iy2-iy);
         strcat(str1,str2);
         fprintf(p_s_out,"%s",str1);
         count -= 4;
	}
      else if (token == GL_LINE_RESET_TOKEN)
        {
         ix = (int)(p_s_data[size-count]*40.0);
         iy = (int)(p_s_data[size-count+1]*40.0);
         sprintf(str2," %d",ix);
         strcpy(str1,str2);
         sprintf(str2, " %d M",iy);
         strcat(str1,str2);
         fprintf(p_s_out,"%s",str1);
         ix2 = (int)(p_s_data[size-count+2]*40.0);
         iy2 = (int)(p_s_data[size-count+3]*40.0);
         sprintf(str2," %d",ix2-ix);
         strcpy(str1,str2);
         sprintf(str2, " %d R",iy2-iy);
         strcat(str1,str2);
         fprintf(p_s_out,"%s",str1);
         count -= 4;
	}
     i++;
     if (i%20 == 0) fprintf(p_s_out," U\n");
     else if (i%3 == 0) fprintf(p_s_out,"\n");
    }
}
