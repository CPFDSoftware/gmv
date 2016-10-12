
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "gmvdata.h"
#include "lights.h"
#include "cutplane.h"

#define MAXVAL 9.99999e32

extern GLdouble wavdx1,wavdx2,wavdy1,wavdy2,wavdz1,wavdz2;


void drawcutline(int icut)
{
  int i, j, k, ic, incr, nclpts, jj, id;
  float fac, val1, val2, avg, wc, v1[3], v2[3],
        clx1, cly1, clz1, clx2, cly2, clz2;
  double a1, b1, c1, a2, b2, c2;
  double xnorm, ynorm, znorm, vlen;
  float x1, y1, z1, x2, y2, z2, s, colr, colg, colb;
  double fld_min, fld_max, tempf;
  char str[10];
  char logflag;
  static GLfloat wx1, wy1, wz1, wx2, wy2, wz2;
  void clsetfldcolr(double fldval, double fldmin, double fldmax, 
                    double tempval, char logflag, int *ic1);
  void getrgbcol(int ic1, float *colr, float *colg, float *colb);

   if (cutlinefield[icut] < numvars)
     {
      k = cutlinefield[icut];
      logflag = nfld_log[k];
      fld_min = nfld_nice_min[k];
      fld_max = nfld_nice_max[k];
     }
   else
     {
      k = cutlinefield[icut] - numvars;
      logflag = cfld_log[k];
      fld_min = cfld_nice_min[k];
      fld_max = cfld_nice_max[k];
     }

   tempf = fld_max - fld_min;
   if (logflag) tempf = log10(fld_max) - log10(fld_min);

   incr = 1;
   if (fastdraw == 1) incr = drawincr;

   nclpts = ncutlinepts[icut];

   clx1 = ctlinx1[icut];  cly1 = ctliny1[icut];  clz1 = ctlinz1[icut];
   clx2 = ctlinx2[icut];  cly2 = ctliny2[icut];  clz2 = ctlinz2[icut];
   a1 = (double)clx2 - (double)clx1;
   b1 = (double)cly2 - (double)cly1;
   c1 = (double)clz2 - (double)clz1;

   glDisable(GL_LIGHTING);

   /*  Draw waves.  */
   if (waveflag[icut] == 1 && incr == 1)
     {

      /*  Caluclate average line value.  */
      val1 = MAXVAL;  val2 = -MAXVAL;
      for (i = 0; i < nclpts; i++)
        {
         val1 = (val1 < cutlineval[icut][i]) ? 
                val1 : cutlineval[icut][i];
         val2 = (val2 > cutlineval[icut][i]) ? 
                val2 : cutlineval[icut][i];
        }
      avg = 0.5 * (val1 + val2);
      fac = span*wavefac[icut] / (fld_max - fld_min);

      /*  Get the points of a line normal to the screen.  */
      wx1 = (float) wavdx1;
      wy1 = (float) wavdy1;
      wz1 = (float) wavdz1;

      wx2 = (float) wavdx2;
      wy2 = (float) wavdy2;
      wz2 = (float) wavdz2;

      /*  Create a plane normal to the screen and along    */
      /*  the cutline so that the wave is always visible.  */
      a2 = (double)(wx2 - wx1);
      b2 = (double)(wy2 - wy1);
      c2 = (double)(wz2 - wz1);
      vlen = sqrt(a1*a1 + b1*b1 + c1*c1);
      if (vlen > 0.0)
        { a1 /= vlen;  b1 /= vlen;  c1 /= vlen; }
      vlen = sqrt(a2*a2 + b2*b2 + c2*c2);
      if (vlen > 0.0)
        { a2 /= vlen;  b2 /= vlen;  c2 /= vlen; }
      xnorm = b1*c2 - b2*c1;
      ynorm = c1*a2 - c2*a1;
      znorm = a1*b2 - a2*b1;
      vlen = xnorm*xnorm + ynorm*ynorm + znorm*znorm;

      if (vlen == 0.0)
        { xnorm = 0.;  ynorm = 0.;  znorm = 1.; }
      else
        {
         vlen = sqrt(vlen);
         xnorm /= vlen;
         ynorm /= vlen;
         znorm /= vlen;
        }

      if (txtcolor < 256) glColor3f(0.,0.,0.);
      else glColor3f(1.,1.,1.);

      for (i = 0; i < nclpts-1; i += incr)
        {
         val1 = cutlineval[icut][i];
         val2 = cutlineval[icut][i+1];

         glBegin(GL_LINES);

         s = cutlinedist[icut][i];
         x1 = clx2*s + clx1*(1.0 - s);
         y1 = cly2*s + cly1*(1.0 - s);
         z1 = clz2*s + clz1*(1.0 - s);
         s = cutlinedist[icut][i+1];
         x2 = clx2*s + clx1*(1.0 - s);
         y2 = cly2*s + cly1*(1.0 - s);
         z2 = clz2*s + clz1*(1.0 - s);

         wc = val1 - avg;  
         v1[0] = x1 + wc * xnorm * fac;
         v1[1] = y1 + wc * ynorm * fac;
         v1[2] = z1 + wc * znorm * fac;

         wc = val2 - avg;  
         v2[0] = x2 + wc * xnorm * fac;
         v2[1] = y2 + wc * ynorm * fac;
         v2[2] = z2 + wc * znorm * fac;

         glVertex3fv(v1);
         glVertex3fv(v2);

         glEnd();
        }
     }

   /*  Draw Gouraud colored line.  */
   glShadeModel(GL_SMOOTH);

   for (i = 0; i < nclpts-1; i += incr)
     {
      val1 = cutlineval[icut][i];
      val2 = cutlineval[icut][i+1];

      glBegin(GL_LINES);

      s = cutlinedist[icut][i];
      x1 = clx2*s + clx1*(1.0 - s);
      y1 = cly2*s + cly1*(1.0 - s);
      z1 = clz2*s + clz1*(1.0 - s);
      s = cutlinedist[icut][i+1];
      x2 = clx2*s + clx1*(1.0 - s);
      y2 = cly2*s + cly1*(1.0 - s);
      z2 = clz2*s + clz1*(1.0 - s);

      v1[0] = x1;  v1[1] = y1;  v1[2] = z1;
      clsetfldcolr(val1,fld_min,fld_max,tempf,logflag,&ic);
      getrgbcol(ic, &colr, &colg, &colb);
      glColor3f(colr,colg,colb);
      glVertex3fv(v1);

      v1[0] = x2;  v1[1] = y2;  v1[2] = z2;
      clsetfldcolr(val2,fld_min,fld_max,tempf,logflag,&ic);
      getrgbcol(ic, &colr, &colg, &colb);
      glColor3f(colr,colg,colb);
      glVertex3fv(v1);

      glEnd();
     }

   /*  Draw points and numbers in material color.  */
   if ((cutlinenodeson[icut] == 1 || cutlinenumson[icut] == 1) && 
      incr == 1)
     {

      for (j = 0; j < nclpts; j++)
        {

         i = cutlinenodesm[icut][j];
         if (i >= 0 && i < MAXMATS)
           {
            jj = 4 * i;
            colr = matrgba[jj+0];
            colg = matrgba[jj+1];
            colb = matrgba[jj+2];
            glColor3f(colr,colg,colb);
           }
         else
           {
            if (txtcolor < 256) glColor3f(0.,0.,0.);
            else glColor3f(1.,1.,1.);
           }

         v1[0] = cutlinenodesx[icut][j];
         v1[1] = cutlinenodesy[icut][j];
         v1[2] = cutlinenodesz[icut][j];
 
         if (cutlinenodeson[icut] == 1)
           {
            glBegin(GL_POINTS);
            glVertex3fv(v1);
            glEnd();
           }

         if (cutlinenumson[icut] == 1)
           {
            id = cutlinenodes[icut][j];
            sprintf(str,"%d",id);
            glRasterPos3f(v1[0], v1[1], v1[2]);  
            glPushAttrib (GL_LIST_BIT);
            glListBase(fontOffset);
            glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
            glPopAttrib ();
           }

        }
     }
  
   glEnable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);
}


void clsetfldcolr
#include "setfldcolr.h"
