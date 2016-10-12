#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"

FILE *varsout;
char varfilename[300];

extern FILE *fieldfile, *cfieldfile, *sfieldfile;

double *field;
int vartype = -1;

void varsoutput(void);
/*  Write variables for specified type.  */
void writenodevars(void);
void writecellvars(void);
void writetracevars(void);
void writesurfvars(void);


void wrtvarbrws() 
{
  /*  Open file browser for write variables.  */
  /*  input:                                  */
  /*    vartype - type of data to get         */
  /*              0 = nodes, 1 = cells,       */
  /*              2 = tracers, 3 = surfacees. */
  XmString string, string2, string3;

   fileselflag = WRITEVARS;
   
   if (vartype == 0)
      string = XmStringCreate("Write Selected Node Variables",
                              XmSTRING_DEFAULT_CHARSET);
   if (vartype == 1)
      string = XmStringCreate("Write Selected Cell Variables",
                              XmSTRING_DEFAULT_CHARSET);
   if (vartype == 2)
      string = XmStringCreate("Write Selected Particle Variables",
                              XmSTRING_DEFAULT_CHARSET);
   if (vartype == 3)
      string = XmStringCreate("Write Selected Surface Variables",
                              XmSTRING_DEFAULT_CHARSET);
   string2 = XmStringCreate(fspattern[WRITEVARS],
                           XmSTRING_DEFAULT_CHARSET);
   string3 = XmStringCreate(fsdir[WRITEVARS],
                           XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                           XmNpattern, string2,
                           XmNdirectory, string3, NULL);
   XmStringFree(string);
   XmStringFree(string2);
   XmStringFree(string3);

   if (fsread[WRITEVARS])
     {
      string = XmStringCreate(varfilename, XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
      XmStringFree(string);
     }

   MyManageChild(file_sel2);
}


void varswritefile(char *fname)
{
  char str[] = 
    "Error - unable to open output file!\nCheck permissions.";
  XmString string;

   strcpy(varfilename,fname);
   /*  Open the file and write the variables.  */
   if ((varsout = fopen(varfilename,"w")) == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      vartype = -1;
      return;
     }
     
   varsoutput();
}


void setVartypeAndWrite(int argVartype, char * filename )
{
   vartype = argVartype;
   /*  Open the file and write the variables.  */
   if ((varsout = fopen(filename,"w")) == NULL)
   {
     errorfile = 2;
     vartype = -1;
     printf("\nError - unable to open output file: %s!\nCheck permissions.\n",
            filename);
     return;
   }

   /*  Write filename and time.  */
   fprintf(varsout,"# %s\n",filnam);
   fprintf(varsout,"# simulation time = %10.7e\n",ptime);

   if      (vartype == 0) writenodevars();
   else if (vartype == 1) writecellvars();
   else if (vartype == 2) writetracevars();
   else if (vartype == 3) writesurfvars();

   /*  Close the file.  */
   fclose(varsout);
}


void nwritevars()
{
   /*  Set vartype for nodes and call file browser.  */
   vartype = 0;
   wrtvarbrws();   
}

void cwritevars()
{
   /*  Set vartype for cells and call file browser.  */
   vartype = 1;
   wrtvarbrws();   
}

void twritevars()
{
   /*  Set vartype for tracers and call file browser.  */
   vartype = 2;
   wrtvarbrws();   
}

void swritevars()
{
   /*  Set vartype for surface and call file browser.  */
   vartype = 3;
   wrtvarbrws();   
}


void varsoutput()
{
   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);
  
  /*  Write filename and time.  */
  fprintf(varsout,"# %s\n",filnam);
  fprintf(varsout,"# simulation time = %10.7e\n",ptime);
  
   if (vartype == 0) writenodevars();
   else if (vartype == 1) writecellvars();
   else if (vartype == 2) writetracevars();
   else if (vartype == 3) writesurfvars();
   
   /*  Close the file.  */
   fclose(varsout);
   
   XUndefineCursor(dpy,topwin);
   XFlush(dpy); 
}


void writenodevars(void)
{
  /*  Write node variables for selected nodes.  */
  
  int nodeid, id, i, ii, loc, colno, gonode;
  double vvv;
  
  /*  Write header, node variable names and velocity header, if any.  */
  colno = 1;
  fprintf(varsout,"#%d node id\n",colno);
  for (i = 0; i < nodevarsin; i++)
    {
     colno++;
     fprintf(varsout,"#%d %s\n",colno,fieldname[i]);
    }

  //  Write node vectors, if any.
  if (vectors)
    {
     fprintf(varsout,"#%d x-velocity\n#%d y-velocity\n#%d z-velocity\n",
             colno+1, colno+2, colno+3);
     colno += 3;
    }
    
  //  Print coordinate headers.
  fprintf(varsout,"#%d x\n#%d y\n#%d z\n", colno+1, colno+2, colno+3);
  
  /*  Print node variables for selected nodes.  */
  for (i = 1; i < nnodes+1; i++)
    { 
     gonode = (int)nodeselectflag[i];
     if ((int)nodesubsetflag[i] == 0) gonode = 0;

     if (gonode)
       {
        id = i;
	if (nodeidflag) id = nodeids[i];
	fprintf(varsout,"%10d",id);
	
	/*  Get and print variables.  */
	for (ii = 0; ii < nodevarsin; ii++)
	  {
           loc = ii * (nnodes+1) + i;
           fseek(fieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,fieldfile);
           fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write vectors, if any.  */
	if (vectors)
	  {
	   ii = speedfld - 3;
           loc = ii * (nnodes+1) + i;
           fseek(fieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,fieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = speedfld - 2;
           loc = ii * (nnodes+1) + i;
           fseek(fieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,fieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = speedfld - 1;
           loc = ii * (nnodes+1) + i;
           fseek(fieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,fieldfile);
	   fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write node coordinates.  */
	fprintf(varsout," %12.8g %12.8g %12.8g",nodes[i].x, nodes[i].y,
	                                         nodes[i].z);	
	fprintf(varsout,"\n");
       }
    }
}


void writecellvars(void)
{
  /*  Write cell variables for selected cells.  */
  
  int cellid, id, i, ii, loc, cellvarsin, colno, gocell;
  double vvv;
  
  /*  Write header, cell variable names and velocity header, if any.  */
  if (cvectors) cellvarsin = cspeedfld - 3;
  else cellvarsin = cnumvars - MAXFLDCALC;
  
  colno = 1;
  fprintf(varsout,"#%d cell id\n",colno);
  for (i = 0; i < cellvarsin; i++)
    {
     colno++;
     fprintf(varsout,"#%d %s\n",colno,cfieldname[i]);
    }
  if (cvectors)
    {
     fprintf(varsout,"#%d x-velocity\n#%d y-velocity\n#%d z-velocity\n",
                     colno+1, colno+2, colno+3);
     colno += 3;
    }
    
  /*  Write cell centers.  */
  fprintf(varsout,"#%d x-center\n#%d y-center\n#%d z-center\n",
                  colno+1, colno+2, colno+3);
  
  /*  Print cell variables for selected cells.  */
  for (i = 0; i < ncells; i++)
    {
     gocell = (int)cellselectflag[i];      
     if ((int)cellsubsetflag[i] == 0) gocell = 0;
     
     if (gocell)
       {
        id = i + 1;
	if (cellidflag) id = cellids[i];
	fprintf(varsout,"%10d",id);
	
	/*  Get and print variables.  */
	for (ii = 0; ii < cellvarsin; ii++)
	  {
           loc = ii * ncells + i;
           fseek(cfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,cfieldfile);
           fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write vectors, if any.  */
	if (cvectors)
	  {
	   ii = cspeedfld - 3;
           loc = ii * ncells + i;
           fseek(cfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,cfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = cspeedfld - 2;
           loc = ii * ncells + i;
           fseek(cfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,cfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = cspeedfld - 1;
           loc = ii * ncells + i;
           fseek(cfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,cfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write centers  */
	fprintf(varsout," %12.8g %12.8g %12.8g",cells[i].xavg, cells[i].yavg,
	                                        cells[i].zavg);	
	fprintf(varsout,"\n");
       }
    }
}


void writesurfvars(void)
{
  /*  Write surface variables for selected surfaces.  */
  
  int surfid, id, i, ii, loc, surfvarsin, colno, gosurf;
  double vvv;
  
  /*  Write header, surf variable names and velocity header, if any.  */
  if (svectors) surfvarsin = sspeedfld - 3;
  else surfvarsin = snumvars - MAXFLDCALC;
  colno = 1;
  fprintf(varsout,"#%d surface id\n",colno);
  for (i = 0; i < surfvarsin; i++)
    {
     colno++;
     fprintf(varsout,"#%d %s\n",colno, sfieldname[i]);
    }
  if (svectors)
    {
     fprintf(varsout,"#%d x-velocity\n#%d y-velocity\n#%d z-velocity\n",
                    colno+1, colno+2, colno+3);
     colno += 3;
    }
    
  /*  Write surface centers.  */
  fprintf(varsout,"#%d x-center\n#%d y-center\n#%d z-center\n",
                  colno+1, colno+2, colno+3);  
  
  /*  Print surf variables for selected surfs.  */
  for (i = 0; i < nsurf; i++)
    {
     gosurf = surfselectflag[i];
     if ((int)surfsubsetflag[i] == 0) gosurf = 0;
     
     if (gosurf)
       {
        id = i + 1;
	if (surfidflag) id = surfids[i];
	fprintf(varsout,"%10d",id);
	
	/*  Get and print variables.  */
	for (ii = 0; ii < surfvarsin; ii++)
	  {
           loc = ii * nsurf + i;
           fseek(sfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,sfieldfile);
           fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write vectors, if any.  */
	if (svectors)
	  {
	   ii = sspeedfld - 3;
           loc = ii * nsurf + i;
           fseek(sfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,sfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = sspeedfld - 2;
           loc = ii * nsurf + i;
           fseek(sfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,sfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	   
	   ii = sspeedfld - 1;
           loc = ii * nsurf + i;
           fseek(sfieldfile, loc*sizeof(double), 0);
           fread(&vvv,sizeof(double),1,sfieldfile);
	   fprintf(varsout," %12.8g",vvv);
	  }
	/*  Write centers  */
	fprintf(varsout," %12.8g %12.8g %12.8g",surfs[i].xavg, surfs[i].yavg,
	                                        surfs[i].zavg);	
	fprintf(varsout,"\n");
       }
    }
}


void writetracevars(void)
{
  /*  Write tracer variables for selected tracers.  */
  
  int surfid, id, i, ii, tracevarsin, colno, gotracer;
  double xck, yck, zck;
  double vvv;
  
  /*  Write header, tracer variable names.  */
  tracevarsin = tfldcalcbeg;
  colno = 1;
  fprintf(varsout,"#%d Particle id\n",colno);
  for (i = 0; i < tracevarsin; i++)
    {
     colno++;
     fprintf(varsout,"#%d %s\n",colno,fieldtrname[i]);
    }
    
  /*  Write tracer coordintates.  */
  fprintf(varsout,"#%d x\n#%d y\n#%d z\n",colno+1, colno+2, colno+3);  
  
  /*  Print tracer variables for selected tracers.  */
  for (i = 0; i < ntracers; i++)
    {
     gotracer = (int) traceselectflag[i]; 

     /*  See if the tracer point lies within the subset.  */
     xck = tracers[0].x[i];
     yck = tracers[0].y[i];
     zck = tracers[0].z[i];
     if (xck > xmaxfdval) gotracer = 0;
     if (xck < xminfdval) gotracer = 0;
     if (yck > ymaxfdval) gotracer = 0;
     if (yck < yminfdval) gotracer = 0;
     if (zck > zmaxfdval) gotracer = 0;
     if (zck < zminfdval) gotracer = 0;
     
     if (gotracer)
       {
        id = i + 1;
	if (traceridflag) id = tracerids[i];
	fprintf(varsout,"%10d",id);
	
	/*  Get and print variables.  */
	for (ii = 0; ii < tracevarsin; ii++)
	  {
           vvv = tracers[0].field[ii][i];
           fprintf(varsout," %12.8g",vvv);
	  }
	  
	/*  Write tracer coordinates.  */
	fprintf(varsout," %12.8g %12.8g %12.8g",tracers[0].x[i],
	                 tracers[0].y[i], tracers[0].z[i]);	
	fprintf(varsout,"\n");
       }
    }
}
