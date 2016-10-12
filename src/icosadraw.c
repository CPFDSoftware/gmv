
#ifdef NT
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>

#define X .525731112119133606
#define Z .850650808352039932

static float vdata[12][3] = {
                 {-X,0.0,Z}, {X,0.0,Z}, {-X,0.0,-Z}, {X,0.0,-Z},
                 {0.0,Z,X}, {0.0,Z,-X}, {0.0,-Z,X}, {0.0,-Z,-X},
                 {Z,X,0.0}, {-Z,X,0.0}, {Z,-X,0.0}, {-Z,-X,0.0}};
static int tindices[20][3] = {
               {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
               {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
               {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
               {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}};
float xoff, yoff, zoff, radius;
void drawtriangle(float *v1, float *v2, float *v3);


void icosadraw(float xyzr[4])
{

  /*  This routine draws a Gouraud shade icosahedron.  */

  int i;

   xoff = xyzr[0];  yoff = xyzr[1];  zoff = xyzr[2];
   radius = xyzr[3];

   /*  Draw the icosahedron.  */
   for (i = 0; i < 20; i++)
     {
      drawtriangle(&vdata[tindices[i][0]][0],
                   &vdata[tindices[i][1]][0],
                   &vdata[tindices[i][2]][0]);
     }
}


void icosaflat(float xyzr[4])
{

  /*  This routine draws an unshaded icoahedron.  */

  int i;
  float vmove[12][3];

   xoff = xyzr[0];  yoff = xyzr[1];  zoff = xyzr[2];
   radius = xyzr[3];

   /*  Multiply icosahedron points by radius then    */
   /*  translate icoshedron point about new center.  */
   for (i = 0; i < 12; i++)
     {
      vmove[i][0] = vdata[i][0] * radius + xoff;
      vmove[i][1] = vdata[i][1] * radius + yoff;
      vmove[i][2] = vdata[i][2] * radius + zoff;
     }

   /*  Draw the icosahedron.  */
   for (i = 0; i < 20; i++)
     {
      glBegin(GL_TRIANGLES);

      glVertex3fv(&vmove[tindices[i][0]][0]);
      glVertex3fv(&vmove[tindices[i][1]][0]);
      glVertex3fv(&vmove[tindices[i][2]][0]);

      glEnd();

     }
}


void spheredraw(float xyzr[4])
{

  /*  This routine builds and draws a Gouroud shaded sphere.  */

  int i;
  void subdivide(float *v1, float *v2, float *v3, int depth);

   xoff = xyzr[0];  yoff = xyzr[1];  zoff = xyzr[2];
   radius = xyzr[3];

   /*  Subdivide the icosahedron twice for a sphere.  */
   for (i = 0; i < 20; i++)
     {
      subdivide(&vdata[tindices[i][0]][0],
                &vdata[tindices[i][1]][0],
                &vdata[tindices[i][2]][0],2);
     }
}


void subdivide(float *v1, float *v2, float *v3, int depth)
{
  float v12[3], v23[3], v31[3], d;
  int i;
  void drawtrinagle(float *v1, float *v2, float *v3);

   /*  Draw this triangle when depth is zero.  */
   if (depth == 0)
     {
      drawtriangle(v1,v2,v3);
      return;
     }

   /*  Normalize the midpoints of each triangle side.  */
   for (i = 0; i < 3; i++)
     {
      v12[i] = v1[i] + (v2[i]-v1[i])/2.;
      v23[i] = v2[i] + (v3[i]-v2[i])/2.;
      v31[i] = v3[i] + (v1[i]-v3[i])/2.;
     }
   d = sqrt(v12[0]*v12[0] + v12[1]*v12[1] + v12[2]*v12[2]);
   if (d > 0.0)
     { v12[0] /= d; v12[1] /= d; v12[2] /= d; }
   d = sqrt(v23[0]*v23[0] + v23[1]*v23[1] + v23[2]*v23[2]);
   if (d > 0.0)
     { v23[0] /= d; v23[1] /= d; v23[2] /= d; }
   d = sqrt(v31[0]*v31[0] + v31[1]*v31[1] + v31[2]*v31[2]);
   if (d > 0.0)
     { v31[0] /= d; v31[1] /= d; v31[2] /= d; }

   /*  Recursively subdivide the new triangles.  */
   subdivide(v1,v12,v31,depth-1);
   subdivide(v2,v23,v12,depth-1);
   subdivide(v3,v31,v23,depth-1);
   subdivide(v12,v23,v31,depth-1);
}


void drawtriangle(float *v1, float *v2, float *v3)
{

  /*  This routine draws a triangle scaled to the  */
  /*  radius and transformed to xoff,yoff,zoff.    */

  float vm1[3], vm2[3], vm3[3];
  float vn1[3], vn2[3], vn3[3];

   vm1[0] = v1[0] * radius + xoff;
   vm1[1] = v1[1] * radius + yoff;
   vm1[2] = v1[2] * radius + zoff;
   vm2[0] = v2[0] * radius + xoff;
   vm2[1] = v2[1] * radius + yoff;
   vm2[2] = v2[2] * radius + zoff;
   vm3[0] = v3[0] * radius + xoff;
   vm3[1] = v3[1] * radius + yoff;
   vm3[2] = v3[2] * radius + zoff;

   /*  Draw the triangle.  */
   vn1[0] = -v1[0];  vn1[1] = -v1[1];  vn1[2] = -v1[2];
   vn2[0] = -v2[0];  vn2[1] = -v2[1];  vn2[2] = -v2[2];
   vn3[0] = -v3[0];  vn3[1] = -v3[1];  vn3[2] = -v3[2];

   glBegin(GL_TRIANGLES);

   glNormal3fv(vn1);  glVertex3fv(vm1);
   glNormal3fv(vn2);  glVertex3fv(vm2);
   glNormal3fv(vn3);  glVertex3fv(vm3);

   glEnd();
}


#define MYPI 3.14159265358979323846
#define TWOPI 6.2831853071795862
#define PID2 1.57079632679489661923

void CreateSphere(double c[3], double r, int n)
{
/*
   Create a sphere centered at c, with radius r, and precision n
*/
  int i, j;
  double theta1, theta2, theta3;
  double xn, yn, zn, x, y, z;
static short ifirst = 1;

   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 4 || r <= 0)
     {
      glBegin(GL_POINTS);
      glVertex3f(c[0],c[1],c[2]);
      glEnd();
      return;
     }

   for (j=0; j < n/2; j++)
     {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

      //glBegin(GL_QUAD_STRIP);
      glBegin(GL_TRIANGLE_STRIP);
      for (i=0; i <= n; i++)
        {
         theta3 = i * TWOPI / n;

         xn = cos(theta2) * cos(theta3);
         yn = sin(theta2);
         zn = cos(theta2) * sin(theta3);
         x = c[0] + r * xn;
         y = c[1] + r * yn;
         z = c[2] + r * zn;

         glNormal3d(xn,yn,zn);
         //glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3d(x,y,z);

         xn = cos(theta1) * cos(theta3);
         yn = sin(theta1);
         zn = cos(theta1) * sin(theta3);
         x = c[0] + r * xn;
         y = c[1] + r * yn;
         z = c[2] + r * zn;

         glNormal3d(xn,yn,zn);
         //glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3d(x,y,z);
        }
      glEnd();
     }
}


int getSphereNorms(int n, double *xn, double *yn, double *zn)
{
/*
   Create a sphere centered at c, with radius r, and precision n
*/
  int i, j, npts;
  double theta1, theta2, theta3, dn;

   npts = 0;
   dn = n;
   for (j=0; j < n/2; j++)
     {
      theta1 = j * TWOPI / dn - PID2;
      theta2 = (j + 1) * TWOPI / dn - PID2;

      for (i=0; i <= n; i++)
        {
         theta3 = i * TWOPI / dn;

         xn[npts] = cos(theta2) * cos(theta3);
         yn[npts] = sin(theta2);
         zn[npts] = cos(theta2) * sin(theta3);
         npts++;

         xn[npts] = cos(theta1) * cos(theta3);
         yn[npts] = sin(theta1);
         zn[npts] = cos(theta1) * sin(theta3);
         npts++;
        }
     }
   return npts;
}
