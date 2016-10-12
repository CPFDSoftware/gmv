#include <Xm/Xm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include "gmvdata.h"
#include "formwidgets.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "image.h"

#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>

short rs[2048];
short gs[2048];
short bs[2048];

void savescreen(name,x1,x2,y1,y2,parray)
char *name;
int x1, x2, y1, y2;
unsigned int parray[];
{
  /*  jpeg stuff.  */
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * outjpeg;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  char *irow;                   /* one row of image data */

  /*  SGI RGB image stuff.  */
  IMAGE *oimage;

  int xsize, ysize;
  int xorg, yorg;
  int y, i, ii;

  int writeerr;
  char str[] = 
    "Error - unable to open rgb output file!\nCheck permissions.";
  char str2[] = 
    "Error - unable to open jpeg output file!\nCheck permissions.";
  XmString string;
  unsigned int *uiparray;

   uiparray = (unsigned int *)parray;

   xorg = (x1 < x2) ? x1 : x2;
   yorg = (y1 < y2) ? y1 : y2;
   if (xorg < 0) xorg = 0;
   if (yorg < 0) yorg = 0;
   xsize = x2 - x1;
   ysize = y2 - y1;
   if (xsize < 0) xsize = -xsize;
   if (ysize < 0) ysize = -ysize;
   xsize++;
   ysize++;

   /*  Open jpeg file, if requested.  */
   if (jpegflag)
     {
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_compress(&cinfo);
      if ((outjpeg = fopen(name,"wb")) == NULL)
        {
         if (!movieflag)
           {
            errorfile = 2;
            string = XmStringLtoRCreate(str2, XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
            XtManageChild(errorform);
            XmStringFree(string);
            return;
           }
         else
           {
            fprintf(stderr, "Error - unable to open jpeg output file!\n");
	    gmvexit();
           }
        }
      jpeg_stdio_dest(&cinfo,outjpeg);
      cinfo.image_width = xsize; 	
      cinfo.image_height = ysize;
      cinfo.input_components = 3;	
      cinfo.in_color_space = JCS_RGB;
      jpeg_set_defaults(&cinfo);
      jpeg_set_quality(&cinfo,jpeg_quality,TRUE);
      jpeg_start_compress(&cinfo, TRUE);
     }

   /*  Else open SGI RGB file.  */
   else
     {
      if ((oimage = iopen(name,"w",RLE(1),3,xsize,ysize,3)) == NULL)
        {
         if (!movieflag)
           {
            errorfile = 2;
            string = XmStringLtoRCreate(str, XmSTRING_DEFAULT_CHARSET);
            XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
            XtManageChild(errorform);
            XmStringFree(string);
            return;
           }
         else
           {
            fprintf(stderr, "Error - unable to open rgb output file!\n");
	    gmvexit();
           }
        }
     }

#if defined(LINUX) || defined(DEC) || defined(NT) || defined(IMAC)
   swapbytes(uiparray,4,xsize*ysize);
#endif

   /* screenspace(); */
   writeerr = 0;

   /*  Write jpeg scan lines.  */
   if (jpegflag)
     {
      irow = (char *)malloc((size_t) xsize * 3);
      row_pointer[0] = (JSAMPROW) &irow[0];
      for (y = ysize-1; y >= 0; y--)
        {
         for (i = 0; i < xsize; i++)
           {
            ii = (yorg+y)*xsize + xorg+i;
            *(irow+i*3+0) = (char) ((uiparray[ii] >> 24) & 255);
            *(irow+i*3+1) = (char) ((uiparray[ii] >> 16) & 255);
            *(irow+i*3+2) = (char) ((uiparray[ii] >> 8) & 255);
           }
         (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

      /*  Finish compression and close the file.  */
      free(irow);
      jpeg_finish_compress(&cinfo);
      fclose(outjpeg);
      jpeg_destroy_compress(&cinfo);
     }

   /*  Write SGI RGB scan lines.  */
   else
     {
      for (y = 0; y < ysize; y++)
        {
         for (i = 0; i < xsize; i++)
           {
            ii = (yorg+y)*xsize + xorg+i;
            rs[i] = (short) ((uiparray[ii] >> 24) & 255);
            gs[i] = (short) ((uiparray[ii] >> 16) & 255);
            bs[i] = (short) ((uiparray[ii] >> 8) & 255);
           }

         if (putrow(oimage,rs,y,0) != xsize) writeerr = 1;
         if (putrow(oimage,gs,y,1) != xsize) writeerr = 1;
         if (putrow(oimage,bs,y,2) != xsize) writeerr = 1;
        }

      /*  Close the SGI RGB file.  */
      if (iclose(oimage) < 0)
        {
         writeerr = 1;
         /* exit(1); */
        }
     }

   /*  Close the jpeg file.  */

}
