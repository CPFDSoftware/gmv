
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


void drawboundingbox()

{

  /*  This routine draws a bounding box.  */

  float v1[3], v2[3];
  char str[10];

   /*  Draw the bounding box, set line width to 1.  */
   /* glLineWidth(1.0); */

   glDisable(GL_LIGHTING);

   /*  Draw first X line in red, rest in text color.  */
   glColor3f(1.0,0.0,0.0);

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   /*  Draw first Y line in green, rest in text color.  */
   glColor3f(0.0,1.0,0.0);

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zminbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zmaxbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   /*  Draw first Z line in blue, rest in text color.  */
   glColor3f(0.0,0.0,1.0);

   v1[0] = xminbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);

   v1[0] = xmaxbbval;
   v1[1] = yminbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = yminbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xminbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xminbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   v1[0] = xmaxbbval;
   v1[1] = ymaxbbval;
   v1[2] = zminbbval;
   v2[0] = xmaxbbval;
   v2[1] = ymaxbbval;
   v2[2] = zmaxbbval;
   glBegin(GL_LINES);
      glVertex3fv(v1);
      glVertex3fv(v2);
   glEnd();

   /*  Reset line width to 2.  */
   /* glLineWidth(2.0); */

   /*  Now draw the min and max values, if requested.  */

   if (bboxcoordflag)
     {
      if (txtcolor < 256) glColor3f(0.,0.,0.);
      else glColor3f(1.,1.,1.);
      glRasterPos3f(xminbbval,yminbbval,zminbbval);
      sprintf(str,"(%g,",xminbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
      sprintf(str," %g,",yminbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
      sprintf(str," %g)",zminbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);

      glRasterPos3f(xmaxbbval,yminbbval,zminbbval);
      sprintf(str,"%g",xmaxbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);

      glRasterPos3f(xminbbval,ymaxbbval,zminbbval);
      sprintf(str,"%g",ymaxbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);

      glRasterPos3f(xminbbval,yminbbval,zmaxbbval);
      sprintf(str,"%g",zmaxbbval);
      glPushAttrib (GL_LIST_BIT);
      glListBase(fontOffset);
      glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
     }

      glEnable(GL_LIGHTING);

}
