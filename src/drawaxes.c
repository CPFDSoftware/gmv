
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
 
void drawaxes(int itype)

{
  float v0[3], v1[3], xlen, ylen, zlen;
  float txtr, txtg, txtb;
  char s[3];
  int fOffset;
 
   v0[0] = v0[1] = v0[2] = 0.0;
   glDisable(GL_LIGHTING);

   txtr = 1.;  txtg = 1.;  txtb = 1.;
   if (txtcolor < 256)
     {
      txtr = 0.;  txtg = 0.;  txtb = 0.;
     }

   if (itype == 1)
     {
      xlen = (float)(fabs((double)(1.01 * globxicmax)));
      ylen = (float)(fabs((double)(1.01 * globyicmax)));
      zlen = (float)(fabs((double)(1.01 * globzicmax)));
      fOffset = fontOffset;
     }
   else if (itype == 2)
     {
      xlen = ylen = zlen = span/2.5;
      fOffset = axfontOffset;
     }
   else if (itype == 3)
     {
      xlen = ylen = zlen = 50.;
      fOffset = fontOffset;
     }
   else return;

   glColor3f(1.0,0.0,0.0);
   if (itype == 3) glColor3f(txtr,txtg,txtb);
   v1[0] = v0[0] + xlen;
   v1[1] = v0[1];
   v1[2] = v0[2];
   glBegin(GL_LINES);
      glVertex3fv(v0);
      glVertex3fv(v1);
   glEnd();
   glColor3f(txtr,txtg,txtb);
   /* glColor4ubv((GLubyte *)&txtcolor); */
   glRasterPos3f(v1[0], v1[1], v1[2]);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fOffset);
   strcpy(s,"X");
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();

   glColor3f(0.0,1.0,0.0);
   if (itype == 3) glColor3f(txtr,txtg,txtb);
   v1[0] = v0[0];
   v1[1] = v0[1] + ylen;
   glBegin(GL_LINES);
      glVertex3fv(v0);
      glVertex3fv(v1);
   glEnd();
   glColor3f(txtr,txtg,txtb);
   /* glColor4ubv((GLubyte *)&txtcolor); */
   glRasterPos3f(v1[0], v1[1], v1[2]);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fOffset);
   strcpy(s,"Y");
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();

   glColor3f(0.0,0.0,1.0);
   if (itype == 3) glColor3f(txtr,txtg,txtb);
   v1[1] = v0[0];
   v1[2] = v0[2] + zlen;
   glBegin(GL_LINES);
      glVertex3fv(v0);
      glVertex3fv(v1);
   glEnd();
   glColor3f(txtr,txtg,txtb);
   /* glColor4ubv((GLubyte *)&txtcolor); */
   glRasterPos3f(v1[0], v1[1], v1[2]);
   glPushAttrib (GL_LIST_BIT);
   glListBase(fOffset);
   strcpy(s,"Z");
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();

   glEnable(GL_LIGHTING);

}
 
