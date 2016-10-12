
#ifdef NT
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "xstuff.h"
extern short noresizeflag;
extern GLXContext glx_glw_context;


/*-----------------------------------------------------------------*/
/*   Write a gmvrc file			  			   */
/*-----------------------------------------------------------------*/
void writegmvrc()
{
  FILE *out;
  char str1[10], str2[10], str3[10];
  int mr, mg, mb, titlelen;
  float redcol, greencol, bluecol;
  char str[] = 
    "Error - unable to open gmvrc output file!\nCheck permissions.";
  XmString string;

   /* Write out all the ascii data at once. */

   out = fopen("gmvrc", "w");
   if (out == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str, XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      return;
     }

   fprintf(out, "gmvrc\n");

   /*  Write out the display angles and magnification.  */
   fprintf(out, "azim %f   elev %f   twist %f\n",xrot,yrot,zrot);
   fprintf(out, "mag %f\n",magnify);

   /*  Write out display options.  */
   strcpy(str1,"off");  strcpy(str2,"off");
   if (nodesflag) strcpy(str1,"on");
   if (nodenumflag) strcpy(str2,"on");
   fprintf(out,"nodes %s   nodenumbers %s\n",str1,str2);
   if (nodesflag) strcpy(str1,"on");
   if (nodenumflag) strcpy(str2,"on");

   strcpy(str1,"off");  strcpy(str2,"off");  strcpy(str3,"off");
   if (cellsflag) strcpy(str1,"on");
   if (celledgesflag) strcpy(str2,"on");
   if (cellnumflag) strcpy(str3,"on");\
   fprintf(out,"cellfaces %s   celledges %s   cellnumbers %s\n",
           str1,str2,str3);

   strcpy(str1,"off");  strcpy(str2,"off");
   if (polysflag) strcpy(str1,"on");
   if (linesflag) strcpy(str2,"on");
   fprintf(out,"polygons %s   polygonlines %s\n",str1,str2);

   /*  Write out axis, time and cycle preference.  */
   strcpy(str1,"off");  strcpy(str2,"off");  strcpy(str3,"off");
   if (axisflag) strcpy(str1,"on");
   if (timeflag) strcpy(str2,"on");
   if (cycleflag) strcpy(str3,"on");
   fprintf(out,"axis %s   time %s   cycle %s\n",str1,str2,str3);

   /*  Write out line and point size.  */
   strcpy(str1,"regular");
   if (line_type) strcpy(str1,"smooth");
   fprintf(out,"linesize %d   linetype %s\n",line_size,str1);

   strcpy(str1,"square");
   if (pnt_shape) strcpy(str1,"round");
   fprintf(out,"pointsize %d   pointshape %s\n",pnt_size,str1);

   /*  Write out number of contour levels and vector arrow.  */
   strcpy(str1,"off");
   if (vectorarrow) strcpy(str1,"on");
   fprintf(out,"ncontours %d   vector_arrow %s\n",nset_contours,str1);

   /*  Write out reflection preferences.  */
   strcpy(str1,"off");  strcpy(str2,"off");  strcpy(str3,"off");
   if (xreflect) strcpy(str1,"on");
   if (yreflect) strcpy(str2,"on");
   if (zreflect) strcpy(str3,"on");
   fprintf(out,"xreflect %s   yreflect %s   zreflect %s\n",str1,str2,str3);

   /*  Write out axis scale numbers.  */
   fprintf(out,"xscaleaxis %f   yscaleaxis %f   zscaleaxis %f\n",
           xscaleaxis,yscaleaxis,zscaleaxis);

   /*  Write out bounding box preferences.  */
   strcpy(str1,"off");  strcpy(str2,"off");
   if (boundingboxflag) strcpy(str1,"on");
   if (bboxcoordflag) strcpy(str2,"on");
   fprintf(out,"boundingbox %s   boundingboxcoords %s\n",str1,str2);

   /*  Write out background color elements.  */
   mr = (int) (backgroundcolor & 255);
   mg = (int) ((backgroundcolor >> 8) & 255);
   mb = (int) ((backgroundcolor >> 16) & 255);
   redcol = (float)mr/255.0;
   greencol = (float)mg/255.0;
   bluecol = (float)mb/255.0;
   fprintf(out,
      "background_red %f   background_green %f   background_blue %f\n",
       redcol,greencol,bluecol);

   /*  Write out display list preference.  */
   strcpy(str1,"off");  
   if (displistflag) strcpy(str1,"on");
   fprintf(out,"display_list %s\n",str1);

   /*  Write out top and bottom titles, if any.  */
   if (titleflag == 1)
     {
      titlelen = strlen(titlestr);
      if (titlelen > 0)
        {
         fprintf(out,"title_top %s\n",titlestr);
        }
      titlelen = strlen(titlestr2);
      if (titlelen > 0)
        {
         fprintf(out,"title_bot %s\n",titlestr2);
        }
     }

   /*  Write out virtual tracball preference.  */
   strcpy(str1,"off");  
   if (trackballflag) strcpy(str1,"on");
   fprintf(out,"trackball %s\n",str1);

   /*  Write out beep sound preference.  */
   strcpy(str1,"off");  
   if (beepflag) strcpy(str1,"on");
   fprintf(out,"beep %s\n",str1);

   /*  Write out distance scale preference.  */
   strcpy(str1,"off");  
   if (distscaleflag) strcpy(str1,"on");
   fprintf(out,"distscale %s\n",str1);

   /*  Write out window size values.  */
   fprintf(out,"windowwidth %ld  windowheight %ld\n",windowwidth,windowheight);

   /*  Write out texture map preference.  */
   strcpy(str1,"off");  
   if (textureflag) strcpy(str1,"on");
   fprintf(out,"texture %s\n",str1);

   /*  Write out the interactivity value.  */
   fprintf(out,"interactivity %d\n",drawincr);

   /*  Write out the JPEG or SGI RGB option.  */
   if (jpegflag) fprintf(out,"imagetype jpg\n");
   else fprintf(out,"imagetype rgb\n");

   /*  Write out orientation axis status and position.  */
   strcpy(str1,"off");
   if (o_axisflag) strcpy(str1,"on");
   fprintf(out,"orient_axis %s   orient_axis_x %d   orient_axis_y %d\n",
           str1,o_axis_x,o_axis_y);

   /*  Write out end_gmvrc and close the file.  */
   fprintf(out,"end_gmvrc\n");
   fclose(out);
}


/*---------------------------------------------------------------------*/
/*   Read a gmvrc file and set flags.	         		       */
/*---------------------------------------------------------------------*/


void readgmvrc()
{
  char keyword[50], old_key[50], str[50], *home, homerc[105], 
       temptitle[83], atrfn[256];
  FILE *fp;
  int mr, mg, mb, chkattr, titlelen;
  long windowwidth_in = -1, windowheight_in = -1;
  float redcol, greencol, bluecol;
  void gmvrc_set_data();

   fp = fopen("gmvrc", "r");
   if (fp == NULL)
     {
      home = getenv("HOME");
      sprintf(homerc,"%s/gmvrc",home);
      fp = fopen(homerc, "r");
      if (fp == NULL) return;
     }

   fscanf(fp, "%s", keyword);
   if (strcmp("gmvrc", keyword))
     {
      fprintf(stderr, "Warning...invalid GMV gmvrc file!\n");
      return;
     }

   redcol = 1.;  greencol = 1.;  bluecol = 1.;
   while (9)
     {

      /*  Read keyword, break at end of file.  */
      strcpy(old_key, keyword); /* this is for debugging */
      if (fscanf(fp, "%s", keyword) == EOF || 
          !strcmp(keyword, "end_gmvrc"))
         break;

      /*  Compare keyword, read and set flags.  */

      else if (!strcmp(keyword, "azim"))
         fscanf(fp, "%f", &xrot);

      else if (!strcmp(keyword, "elev"))
         fscanf(fp, "%f", &yrot);

      else if (!strcmp(keyword, "twist"))
         fscanf(fp, "%f", &zrot);

      else if (!strcmp(keyword, "mag"))
         fscanf(fp, "%f", &magnify);

      else if (!strcmp(keyword, "nodes"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) nodesflag = 1;
         else nodesflag = 0;
        }

      else if (!strcmp(keyword, "nodenumbers"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) nodenumflag = 1;
         else nodenumflag = 0;
        }

      else if (!strcmp(keyword, "cellfaces"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) cellsflag = 1;
         else cellsflag = 0;
        }

      else if (!strcmp(keyword, "celledges"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) celledgesflag = 1;
         else celledgesflag = 0;
        }

      else if (!strcmp(keyword, "cellnumbers"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) cellnumflag = 1;
         else cellnumflag = 0;
        }

      else if (!strcmp(keyword, "polygons"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) polysflag = 1;
         else polysflag = 0;
        }

      else if (!strcmp(keyword, "polygonlines"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) linesflag = 1;
         else linesflag = 0;
        }

      else if (!strcmp(keyword, "axis"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) axisflag = 1;
         else axisflag = 0;
        }

      else if (!strcmp(keyword, "time"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) timeflag = 1;
         else timeflag = 0;
        }

      else if (!strcmp(keyword, "cycle"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) cycleflag = 1;
         else cycleflag = 0;
        }

      else if (!strcmp(keyword, "linesize"))
        {
         fscanf(fp, "%hd", &line_size);
         if (line_size < 1) line_size = 1;
         if (line_size > 3) line_size = 3;
        }

      else if (!strcmp(keyword, "linetype"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"smooth") == 0) line_type = 1;
         else line_type = 0;
        }

      else if (!strcmp(keyword, "pointsize"))
        {
         fscanf(fp, "%hd", &pnt_size);
         if (pnt_size < 2) pnt_size = 2;
         if (pnt_size > 8) pnt_size = 8;
         if ((pnt_size%2) == 1) pnt_size += 1;
        }

      else if (!strcmp(keyword, "pointshape"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"round") == 0) pnt_shape = 1;
         else pnt_shape = 0;
        }

      else if (!strcmp(keyword, "ncontours"))
        {
         fscanf(fp, "%d", &nset_contours);
        }

      else if (!strcmp(keyword, "vector_arrow"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) vectorarrow = 1;
         else vectorarrow = 0;
        }

      else if (!strcmp(keyword, "xreflect"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) xreflect = 1;
         else xreflect = 0;
        }

      else if (!strcmp(keyword, "yreflect"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) yreflect = 1;
         else yreflect = 0;
        }

      else if (!strcmp(keyword, "zreflect"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) zreflect = 1;
         else zreflect = 0;
        }

      else if (!strcmp(keyword, "xscaleaxis"))
        {
         fscanf(fp, "%f", &xscaleaxis);
        }

      else if (!strcmp(keyword, "yscaleaxis"))
        {
         fscanf(fp, "%f", &yscaleaxis);
        }

      else if (!strcmp(keyword, "zscaleaxis"))
        {
         fscanf(fp, "%f", &zscaleaxis);
        }

      else if (!strcmp(keyword, "boundingbox"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) boundingboxflag = 1;
         else boundingboxflag = 0;
        }

      else if (!strcmp(keyword, "boundingboxcoords"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) bboxcoordflag = 1;
         else bboxcoordflag = 0;
        }

      else if (!strcmp(keyword, "background_red"))
        {
         fscanf(fp, "%f", &redcol);
        }

      else if (!strcmp(keyword, "background_green"))
        {
         fscanf(fp, "%f", &greencol);
        }

      else if (!strcmp(keyword, "background_blue"))
        {
         fscanf(fp, "%f", &bluecol);
        }

      else if (!strcmp(keyword, "display_list"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) displistflag = 1;
         else displistflag = 0;
        }

      else if (!strcmp(keyword, "title_top"))
        {
         fgets(temptitle,83,fp);
         titlelen = strlen(temptitle);
         strncpy(titlestr,temptitle+1,titlelen-2);
         titleflag = 1;
        }

      else if (!strcmp(keyword, "title_bot"))
        {
         fgets(temptitle,83,fp);
         titlelen = strlen(temptitle);
         strncpy(titlestr2,temptitle+1,titlelen-2);
         titleflag = 1;
        }

      else if (!strcmp(keyword, "trackball"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) trackballflag = 1;
         else trackballflag = 0;
        }

      else if (!strcmp(keyword, "beep"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) beepflag = 1;
         else beepflag = 0;
        }

      else if (!strcmp(keyword, "distscale"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) distscaleflag = 1;
         else distscaleflag = 0;
        }

      else if (!strcmp(keyword, "windowwidth"))
        {
         fscanf(fp, "%ld", &windowwidth_in);
        }

      else if (!strcmp(keyword, "windowheight"))
        {
         fscanf(fp, "%ld", &windowheight_in);
        }

      else if (!strcmp(keyword, "texture"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) textureflag = 1;
         else textureflag = 0;
        }

      else if (!strcmp(keyword, "interactivity"))
        {
         fscanf(fp, "%d", &drawincr);
        }

      else if (!strcmp(keyword, "imagetype"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"jpg") == 0) jpegflag = 1;
         else jpegflag = 0;
        }

      else if (!strcmp(keyword, "orient_axis"))
        {
         fscanf(fp, "%s", str);
         if (strcmp(str,"on") == 0) o_axisflag = 1;
         else o_axisflag = 0;
        }

      else if (!strcmp(keyword, "orient_axis_x"))
        {
         fscanf(fp, "%d", &o_axis_x);
        }

      else if (!strcmp(keyword, "orient_axis_y"))
        {
         fscanf(fp, "%d", &o_axis_y);
        }

      else if (!strcmp(keyword, "attributes"))
        {
         fscanf(fp, "%s", atrfn);
         strcpy(attribsfilnam,atrfn);
         chkattr = check_attrib_version();
         if (chkattr >= 0 && chkattr < 12) getattributes();
        }

      else
        {
         fprintf(stderr, "Invalid gmvrc keyword - %s \n",keyword);
        }
     }
   fclose(fp);

   /*  Reset bacground color.  */
   mr = (int)(redcol * 255.);
   mg = (int)(greencol * 255.);
   mb = (int)(bluecol * 255.);
   backgroundcolor = ((mb << 16) | (mg << 8) | mr);

   /*  Check if the window needs resizing.  */
   if (windowwidth_in > 0 || windowheight_in > 0)
   {
      if (windowwidth_in == -1)  windowwidth_in = windowwidth;
      if (windowheight_in == -1)  windowheight_in = windowheight;
      if (windowwidth_in != windowwidth || windowheight_in != windowheight)
      {
         windowwidth = windowwidth_in;
         if (windowwidth > windowxmax) windowwidth = windowxmax;
         windowheight = windowheight_in;
         if (windowheight > windowymax) windowheight = windowymax;
         //TJ attributes now main width/height supplier.
         //   If this is not wrapped causes events pushed that get sorted later
         //   incorrectly resizing back to gmvrc instead of attribute file
         if( readattributes_found_width_height == 0 )
         {
           noresizeflag = 1;
           winresize();
         }
      }
   }

   /*  Set data and widgets.  */
   gmvrc_set_data();
}


void gmvrc_set_data()
{
  int rr, gg, bb;
  double dmin, dmax, dbig;
  float lnsize, ptsize;
  void readpolyfile();

   /*  If polygons are drawn, get polygon data from disk.  */
   if (polysin && (polysflag || linesflag))
      readpolyfile();

   /*  Scale axes.  */
   setscaleaxis(0);

   /*  Set text color color.  */
   rr = (int) (backgroundcolor & 255);
   gg = (int) ((backgroundcolor >> 8) & 255);
   bb = (int) ((backgroundcolor >> 16) & 255);
   txtcolor = 0xffffffff;
   if (gg > 150 | (rr > 220 & bb > 220)) txtcolor = 0x000000ff;

   /*  Set point size and shape.  */
   glXMakeCurrent(dpy,windowid,glx_glw_context);
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

   /*  Set motif widgets.  */

   /*  First pop down all forms.  */
   popdownall();

   /*  Now fill in widgets with global access.  */
   setmag();
   setangles();

   /*  Set new mins and maxs for reflections.  */
   if (xreflect == 1) 
     {
      dmin = fabs(globxicmin);
      dmax = fabs(globxicmax);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      globxicmax = dbig;
      globxicmin = -globxicmax;
      xrange = 2.0*globxicmax;
      xtrans = 0.0;
      dmin = fabs(xminbbvalo);
      dmax = fabs(xmaxbbvalo);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      xmaxbbvalo = dbig;
      xminbbvalo = -xmaxbbvalo;
      xminbbval = xminbbvalo;
      xmaxbbval = xmaxbbvalo;
     } 
   if (yreflect == 1) 
     {
      dmin = fabs(globyicmin);
      dmax = fabs(globyicmax);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      globyicmax = dbig;
      globyicmin = -globyicmax;
      yrange = 2.0*globyicmax;
      ytrans = 0.0;
      dmin = fabs(yminbbvalo);
      dmax = fabs(ymaxbbvalo);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      ymaxbbvalo = dbig;
      yminbbvalo = -ymaxbbvalo;
      yminbbval = yminbbvalo;
      ymaxbbval = ymaxbbvalo;
     } 
   if (zreflect == 1) 
     {
      dmin = fabs(globzicmin);
      dmax = fabs(globzicmax);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      globzicmax = dbig;
      globzicmin = -globzicmax;
      zrange = 2.0*globzicmax;
      ztrans = 0.0;
      dmin = fabs(zminbbvalo);
      dmax = fabs(zmaxbbvalo);
      dbig = dmax; if (dmin > dmax) dbig = dmin;
      zmaxbbvalo = dbig;
      zminbbvalo = -zmaxbbvalo;
      zminbbval = zminbbvalo;
      zmaxbbval = zmaxbbvalo;
     }

   /*  Set reflection buttons.  */
   XmToggleButtonGadgetSetState(objxrefl,xreflect,FALSE);
   XmToggleButtonGadgetSetState(objyrefl,yreflect,FALSE);
   XmToggleButtonGadgetSetState(objzrefl,zreflect,FALSE);

   if (xreflect || yreflect || zreflect)
     {
      setcenterwidgets();
      setsubsetwidgets();
      setboundingboxwidgets();
     }

   /*  Set display list button.  */
   if (displistflag)
     {
      drawnewlistflag = 1;
      XmToggleButtonGadgetSetState(displistbut,TRUE,FALSE);
     }
   else XmToggleButtonGadgetSetState(displistbut,FALSE,FALSE);

   /*  Set virtual trackball button.  */
   if (trackballflag)
      XmToggleButtonGadgetSetState(trackballbut,TRUE,FALSE);
   else XmToggleButtonGadgetSetState(trackballbut,FALSE,FALSE);

   /*  Set beep sound button.  */
   if (beepflag)
      XmToggleButtonGadgetSetState(beepbut,TRUE,FALSE);
   else XmToggleButtonGadgetSetState(beepbut,FALSE,FALSE);

   /*  Set texture field button.  */
   if (textureflag)
      XmToggleButtonGadgetSetState(texturebut,TRUE,FALSE);
   else XmToggleButtonGadgetSetState(texturebut,FALSE,FALSE);

   /*  Check that drawing flags are correct.  */
   if (polysin == 0 && (polysflag || linesflag))
     {
      polysflag = 0;  linesflag = 0;
     }

   if (ncells == 0 && (cellsflag || celledgesflag || cellnumflag))
     {
      cellsflag = 0;  celledgesflag = 0;  cellnumflag = 0;
     }

   if (polysin == 0 && ncells == 0)
     {
      if (nodesflag == 0 && nodenumflag == 0) nodesflag = 1;
     }

   if (polysin && (polysflag || linesflag)) polygonformpop();
   if (cellsflag || celledgesflag || cellnumflag) cellformpop();
   if (nodesflag || nodenumflag) nodeformpop();

   /*  Set axes toggles.  */
   XmToggleButtonGadgetSetState(objaxes,axisflag,FALSE);
   XmToggleButtonGadgetSetState(o_axis_on,o_axisflag,FALSE);   
}
