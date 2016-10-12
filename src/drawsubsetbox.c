
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>
#include "gmvdata.h"

void drawsubsetbox()

{

  /*  This routine draws the node, cell and tracer and surface subset box.  */

  float v1[3];

   glDisable(GL_LIGHTING);

   if (txtcolor < 256) glColor3f(0.,0.,0.);
   else glColor3f(1.,1.,1.);
   glBegin(GL_LINE_LOOP);
   v1[0] = xminfdval;
   v1[1] = yminfdval;
   v1[2] = zminfdval;
   glVertex3fv(v1);
   v1[0] = xmaxfdval;
   glVertex3fv(v1);
   v1[1] = ymaxfdval;
   glVertex3fv(v1);
   v1[0] = xminfdval;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINE_LOOP);
   v1[0] = xminfdval;
   v1[1] = yminfdval;
   v1[2] = zmaxfdval;
   glVertex3fv(v1);
   v1[0] = xmaxfdval;
   glVertex3fv(v1);
   v1[1] = ymaxfdval;
   glVertex3fv(v1);
   v1[0] = xminfdval;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xminfdval;
   v1[1] = yminfdval;
   v1[2] = zminfdval;
   glVertex3fv(v1);
   v1[2] = zmaxfdval;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xminfdval;
   v1[1] = ymaxfdval;
   v1[2] = zminfdval;
   glVertex3fv(v1);
   v1[2] = zmaxfdval;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmaxfdval;
   v1[1] = yminfdval;
   v1[2] = zminfdval;
   glVertex3fv(v1);
   v1[2] = zmaxfdval;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmaxfdval;
   v1[1] = ymaxfdval;
   v1[2] = zminfdval;
   glVertex3fv(v1);
   v1[2] = zmaxfdval;
   glVertex3fv(v1);
   glEnd();
   glEnable(GL_LIGHTING);

}



void drawselectbox(int iflag)

{

  /*  This routine draws the node or cell select box.   */
  /*  Input: iflag - data type, 0 = cells, 1 == nodes,  */
  /*         2 = surfaces, 3 = polygons, 4 = tracers.   */

  float v1[3];
  float xmin, xmax, ymin, ymax, zmin, zmax;

   if (iflag == 0)
     {
      xmin = cellselbox_xmin;
      xmax = cellselbox_xmax;
      ymin = cellselbox_ymin;
      ymax = cellselbox_ymax;
      zmin = cellselbox_zmin;
      zmax = cellselbox_zmax;
      glColor3f(1.,0.,0.);
     }
   if (iflag == 1)
     {
      xmin = nodeselbox_xmin;
      xmax = nodeselbox_xmax;
      ymin = nodeselbox_ymin;
      ymax = nodeselbox_ymax;
      zmin = nodeselbox_zmin;
      zmax = nodeselbox_zmax;
      glColor3f(0.,1.,0.);
     }

   if (iflag == 2)
     {
      xmin = surfselbox_xmin;
      xmax = surfselbox_xmax;
      ymin = surfselbox_ymin;
      ymax = surfselbox_ymax;
      zmin = surfselbox_zmin;
      zmax = surfselbox_zmax;
      glColor3f(0.,0.,1.);
     }

   if (iflag == 3)
     {
      xmin = xminbdval;
      xmax = xmaxbdval;
      ymin = yminbdval;
      ymax = ymaxbdval;
      zmin = zminbdval;
      zmax = zmaxbdval;
      glColor3f(1.,0.,1.);
     }

   if (iflag == 4)
     {
      xmin = traceselbox_xmin;
      xmax = traceselbox_xmax;
      ymin = traceselbox_ymin;
      ymax = traceselbox_ymax;
      zmin = traceselbox_zmin;
      zmax = traceselbox_zmax;
      glColor3f(0.,1.,0.);
     }

   glDisable(GL_LIGHTING);

   glBegin(GL_LINE_LOOP);
   v1[0] = xmin;
   v1[1] = ymin;
   v1[2] = zmin;
   glVertex3fv(v1);
   v1[0] = xmax;
   glVertex3fv(v1);
   v1[1] = ymax;
   glVertex3fv(v1);
   v1[0] = xmin;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINE_LOOP);
   v1[0] = xmin;
   v1[1] = ymin;
   v1[2] = zmax;
   glVertex3fv(v1);
   v1[0] = xmax;
   glVertex3fv(v1);
   v1[1] = ymax;
   glVertex3fv(v1);
   v1[0] = xmin;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmin;
   v1[1] = ymin;
   v1[2] = zmin;
   glVertex3fv(v1);
   v1[2] = zmax;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmin;
   v1[1] = ymax;
   v1[2] = zmin;
   glVertex3fv(v1);
   v1[2] = zmax;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmax;
   v1[1] = ymin;
   v1[2] = zmin;
   glVertex3fv(v1);
   v1[2] = zmax;
   glVertex3fv(v1);
   glEnd();
 
   glBegin(GL_LINES);
   v1[0] = xmax;
   v1[1] = ymax;
   v1[2] = zmin;
   glVertex3fv(v1);
   v1[2] = zmax;
   glVertex3fv(v1);
   glEnd();
   glEnable(GL_LIGHTING);

}
