
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleBG.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#include "gmvdata.h"
#include "formwidgets.h"
#include "xstuff.h"
#include "cutplane.h"

extern FILE *fieldfile, *cfieldfile;

/* extern XtAppContext app; */
extern int maxfaceverts;

#define MAXVAL 9.99999e32

static Widget valntxt, valdispnlab[MAXFIELDS+MAXFLAGS+1],
              valctxt, valdispclab[MAXFIELDS+MAXFLAGS+1], 
              valcnodeids[10000], nprobebut, cprobebut, vfilebut,
              nfldvalform, nfvnodelab, nfvminlab, nfvminlab1, nfvminlab2, 
              nfvmaxlab, nfvmaxlab1, nfvmaxlab2, nfvvaltxt, nfvselbut,
              nfvmatchlab, nfvvallab, cfldvalform, cfvcelllab, cfvminlab,
              cfvminlab1, cfvminlab2, cfvmaxlab, cfvmaxlab1, cfvmaxlab2, 
              cfvvaltxt, cfvselbut, cfvmatchlab, cfvvallab, valndistlab;
static int valnodeno, valcellno, maxnode, querynfield, querycfield,
           valndist1 = 0, valndist2 = 0;
static double nx1, ny1, nz1, nx2, ny2, nz2;
short query_file_flag;
static char qdsavfilnam[256];
FILE *qout;

/* 
   The shared memory is allocated in page-blocks (4096 bytes). If this 
   application opens shared memory, then it requests SHARED_MEM_SIZE 
   bytes of memory. If the shared memory exists, this 
   application simply points to the existing shared memory. Another 
   application may have opened the shared memory. The shared memory is 
   local to the directory, and many shared memories can be opened in 
   different directories. 
  
   Int, float and char pointers are defined. All three point to the same 
   memory space.  The int memory location iBuf[NEWDATA] is a flag set to 
   1 by this application and set to 0 by the user's application after 
   accessing the memory.  GMV does not monitor this flag but always sets 
   it to 1 after new data are loaded. This allows the user to monitor if 
   new data are in shared memory.

   There are FIXEDMEM int (or 4 byte parcels) reserved at the start of  
   shared memory.  These currently are not used.

   At this time, the shared memory is a keyword (8 bytes) followed by a 
   float (4 bytes) or an int (4 bytes) starting at memory location FIXEDMEM. 
   A single variable uses 12 bytes of memory.  The terminator "end", 
   designates the end of the GMV data.
*/

#define  nul       '\0'
#define  MAPFILE   "gmv.share"      /* shared memory "file" */
static int    membytes;             /* the number bytes in shared memory */
char *mapData;
char *mpnew(int);
char *mpold(void);
static int OpenSharedMem();
static const int SHARED_MEM_SIZE = 4096; /*  size of shared memory  */ 
                                         /*  in bytes.  One page.   */
static const int SHARED_INT      = 1024; /*  size of shared memory  */
                                         /*  in int (4 bytes).      */
static const int NBYTE           = 4;    /*  the byte length of a   */
                                         /*  float and int          */
static const int NEWDATA         = 0;    /*  new data in shared     */
                                         /*  mem flag               */ 
static const int FIXEDMEM        = 8;    /*  length (int) of reserved  */
                                         /*  fixed memory.             */

/*  Pointers to access shared memory. int,  */
/*  float and char point to  same memory.   */
static int   *iBufMem;
static float *fBufMem;
static char  *cBufMem;
static int   cLocMem;
static int   useSharedMem; /* 1=using shared mem. 0=not using shared memory */



void setqueryprobe()
{
  int i, cp_nodes, cp_cells;

   /*  Check for cutplane nodes and cells.  */
   cp_nodes = 0;  cp_cells = 0;
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES; i++)
     {
      if (cutplane_on[i] > 0)
        {
         if (cutplane[i].ndnodes > 0 && 
             (cutplane[i].nodeflag == 1 || cutplane[i].nnumflag == 1  ||
              cutplane[i].velflag == 1))
            cp_nodes = 1;
         if (cutplane[i].ndcells > 0 && 
             (cutplane[i].cellflag == 1 || cutplane[i].cnumflag == 1 ||
              cutplane[i].cvelflag))
            cp_cells = 1;
        }
     }

   if (nodesflag > 0 || vectorflag == 1 || nodenumflag == 1 || cp_nodes)
      XtSetSensitive(nprobebut,TRUE);
   else
     {
      XtSetSensitive(nprobebut,FALSE);
      XmToggleButtonGadgetSetState(nprobebut,FALSE,TRUE);
     }

   if (ncells > 0)
     {
      if (celledgesflag == 1 || cellsflag == 1 || cellnumflag == 1 ||
          gridanalflag == 1 || cp_cells)
         XtSetSensitive(cprobebut,TRUE);
      else
        {
         XtSetSensitive(cprobebut,FALSE);
         XmToggleButtonGadgetSetState(cprobebut,FALSE,TRUE);
        }
     }

   if (viewflag > 0)
     {
      XtSetSensitive(nprobebut,FALSE);
      XmToggleButtonGadgetSetState(nprobebut,FALSE,TRUE);
      if (ncells > 0)
        {
         XtSetSensitive(cprobebut,FALSE);
         XmToggleButtonGadgetSetState(cprobebut,FALSE,TRUE);
        }
     }
   XmUpdateDisplay(nprobebut);
   if (ncells > 0) XmUpdateDisplay(cprobebut);

   setcenterprobe();
}


/*
* ******************************************************************************
*     Connect to or open shared memory
*
*     Entry -
*        none
*      
*     Exit -
*        <return>  :0=error. 1=ok.
* ******************************************************************************
*/
static int OpenSharedMem()
{
   /*  Connect to existing mmap memory if it exists.  */
   iBufMem = (int *) mpold();

   /*  If mmap does not exist, open new.  */
   if (iBufMem == NULL)
     {
      iBufMem = (int *) mpnew(SHARED_MEM_SIZE);
      if (iBufMem == NULL)
        {
         return 0;                   /*  Could not aquire shared mem.  */
        }
     }

   /*  Same buffer for float, int, and char (remember char  */
   /*  are ref on byte. real and int are ref on 4*byte).    */
   fBufMem = (float *) iBufMem;
   cBufMem = (char  *) iBufMem;

   return 1;
}


void queryformpop()
{
   int i;

   /*  Open or attach to shared memory. Don't  */ 
   /*  crash GMV if shared memory fails.       */
   /*  If useSharedMem = 1, success else if    */
   /*  0, shared memory failed.                */
   useSharedMem = 0;
   if (shmemflag)
     {
      useSharedMem = OpenSharedMem();

      /*  Initialize the fixed memory section.  */
      for (i = 0; i < FIXEDMEM; i++)
        {
         iBufMem[i]=0;
        }

      /*  Set the start location to load shared memory (int).  */
      cLocMem = FIXEDMEM;
     }

   query_file_flag = 0;

   setqueryprobe();

   MyManageChild(queryform);
}


void valquit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   if (numvars > 0)
     {
      XtUnmanageChild(nfieldform);
      XtUnmanageChild(nfldvalform);
     }
   if (cnumvars > 0)
     {
      XtUnmanageChild(cfieldform);
      XtUnmanageChild(cfldvalform);
      cprobeflag = 0;
      XmToggleButtonGadgetSetState(cprobebut,FALSE,FALSE);
     }
   nprobeflag = 0;
   XmToggleButtonGadgetSetState(nprobebut,FALSE,FALSE);
   cprobeflag = 0;
   if (ncells > 0)
      XmToggleButtonGadgetSetState(cprobebut,FALSE,FALSE);
   XUndefineCursor(dpy,windowid);

   if (query_file_flag == 1)
     {
      query_file_flag = 0;
      fclose(qout);
      XmToggleButtonGadgetSetState(vfilebut,FALSE,FALSE);
     }  

   XtUnmanageChild(queryform);
}


void qdwrite(w,client_data,call_data) 
 Widget w;
 XtPointer client_data, call_data;
{
  int qwriteflag;
  XmString string, string2, string3;

   qwriteflag = (int)XmToggleButtonGadgetGetState(w);

   if (qwriteflag)
     {
      fileselflag = QUERYOUT;
      string = XmStringCreate("Write Query Data to a File",
                              XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringCreate(fspattern[QUERYOUT],
                              XmSTRING_DEFAULT_CHARSET);
      string3 = XmStringCreate(fsdir[QUERYOUT],
                              XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(file_sel2, XmNdialogTitle,string, 
                              XmNpattern, string2,
                              XmNdirectory, string3, NULL);
      XmStringFree(string);
      XmStringFree(string2);
      XmStringFree(string3);

      if (fsread[QUERYOUT])
        {
         string = XmStringCreate(qdsavfilnam, XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(file_sel2, XmNdirSpec,string, NULL);
         XmStringFree(string);
        }

      MyManageChild(file_sel2);
     }

   else
     {
      query_file_flag = 0;
      fclose(qout);
     }
}


void qdwritefile(char *fname)
{
  char str[] = 
    "Error - unable to open output file!\nCheck permissions.";
  XmString string;

   strcpy(qdsavfilnam,fname);
   /*  Open the file and set the query_file_flag.  */
   if ((qout = fopen(qdsavfilnam,"w")) == NULL)
     {
      errorfile = 2;
      string = XmStringLtoRCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(objerrlab, XmNlabelString, string, NULL);
      XtManageChild(errorform);
      XmStringFree(string);
      query_file_flag = 0;
      return;
     }
   query_file_flag = 1;
}


void qdquitfile()
{
   query_file_flag = 0;
   XmToggleButtonGadgetSetState(vfilebut,FALSE,FALSE);
}


void probe_nodes(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   nprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (nprobeflag)
     {
      get_1_point(NULL, (caddr_t) 2, NULL);
      cprobeflag = 0;
      if (ncells > 0)
         XmToggleButtonGadgetSetState(cprobebut,FALSE,FALSE);
     }
   else XUndefineCursor(dpy,windowid);
}


int getnodeprobe(a,b,c,d,e,f)
 float a,b,c,d,e,f;
{
  int i, j, k, l, nodeid, ibeg, iend;
  int ncpck, cpck[MAXCUTPLANES+MAXCUTSPHERES], icp;
  short gonode, *matchk;
  double xl1, yl1, zl1, xl2, yl2, zl2, xn, yn, zn;
  double epsln, dist, mindist, mins;
  double ax, ay, az, bx, by, bz, s, xp, yp, zp;

   /*  Set the points.  */
   xl1 = (double)a;  yl1 = (double)b;  zl1 = (double)c;
   xl2 = (double)d;  yl2 = (double)e;  zl2 = (double)f;

   /*  Set line distance elements.  */
   ax = (double)xl2 - (double)xl1;
   ay = (double)yl2 - (double)yl1;
   az = (double)zl2 - (double)zl1;

   /*  Set to material or group color.  */
   if (nodefieldactive == -2) matchk = nodegrpcolr;
   else matchk = imat;

   /*  Check for cutplane or cutsphere nodes.  */
   ncpck = 0;
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES; i++)
     {
      cpck[i] = 0;
      if (cutplane_on[i] > 0)
        {
         if (cutplane[i].ndnodes > 0 && 
             (cutplane[i].nodeflag == 1 || cutplane[i].nnumflag == 1  ||
              cutplane[i].velflag == 1))
           {
            cpck[ncpck] = i;
            ncpck++;
           }
        }
     }

   /*  Loop over all nodes.  */
   nodeid = 0;
   epsln = myzero;
   mindist = MAXVAL;
   mins = MAXVAL;
   ibeg = 1;  iend = nnodes + 1;

   for (j = ibeg; j < iend; j++)
     {

      /*  Check node material, subset and select.  */
      i = j;

      /*  See if the point has been selected for display.  */
      gonode = (int)nodeselectflag[i];

      /*  Eliminate nodes with material < 1.  */
      if (node0matflag == 0 && matchk[i] < 1) gonode = 0;
 
      /*  See if the point lies within the subset.  */
      if ((int)nodesubsetflag[i] == 0) gonode = 0;

      /*  Deactivate node if node and node numbers NOT drawn.  */
      if (nodesflag == 0 && nodenumflag == 0) gonode = 0;

      /*  If node not selected, check cutplane or cutsphere nodes.  */
      if (gonode == 0 && ncpck > 0)
        {
         for (k = 0; k < ncpck; k++)
           {
            icp = cpck[k];
            for (l = 0; l < cutplane[icp].ndnodes; l++)
              {
               if (i == cutplane[icp].dnodes[l]) gonode = 1;
              }
           }
        }

      /*  If active node, check distance to line.  */
      if (gonode)
        {

         /*  Calculate projection point of node on the line and    */
         /*  the square distance from the node to the projection.  */
         xn = nodes[i].x;  yn = nodes[i].y;  zn = nodes[i].z;
         bx = xn - (xl1);
         by = yn - (yl1);
         bz = zn - (zl1);
         s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
         xp = s*xl2 + xl1*(1.0 - s);
         yp = s*yl2 + yl1*(1.0 - s);
         zp = s*zl2 + zl1*(1.0 - s);
         dist = (xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + (zp-zn)*(zp-zn);

         /*  Save node id if closest to the line,        */
         /*  for ties, use node closest to first point.  */
         if (dist <= mindist+epsln)
           {
            if (dist+epsln < mindist)
              {
               nodeid = i;
               mindist = dist;
               mins = s;
              }
            else if (s < mins)
              {
               nodeid = i;
               mindist = dist;
               mins = s;
              }
           } 
        }
     }
   return nodeid;
}


void doitnprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int nodeid, id;
  char str[10];
  void getvalntxt(Widget w, XtPointer client_data, XtPointer call_data);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Get the node number.  */
   nodeid = getnodeprobe(pa,pb,pc,pd,pe,pf);

   /*  Set nodeid in text.  */
   id = nodeid;
   if (nodeidflag) id = nodeids[nodeid];
   sprintf(str,"%d",id);
   XtVaSetValues(valntxt,XmNvalue,str,NULL);
   XmUpdateDisplay(valntxt);
   XUndefineCursor(dpy,topwin);
   XFlush(dpy);

   getvalntxt(NULL,NULL,NULL);
}

void getvalntxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  double vvv, nodedist;
  char str[90], matnm[33];
  XmString string;
  char *tmpstr;
  int i, ii, ic, nv, fldbeg, loc, id, m, n, maxflagchar, maxfldchar;

   /*  Calculate max field name char. size.  */
   maxfldchar = 8;
   for (i = 0; i < numvars; i++)
     {
      ii = strlen(fieldname[i]);
      if (ii > maxfldchar) maxfldchar = ii;
     } 

   /*  Calculate max flag name char size.  */
   maxflagchar = 8;
   for (i = 0; i < numflags_node; i++)
     {
      ii = strlen(flagname_node[i]);
      if (ii > maxflagchar) maxflagchar = ii;
     }

   tmpstr = XmTextFieldGetString(valntxt);
   id = atoi(tmpstr);
   XtFree(tmpstr);
   valnodeno = id;
   if (nodeidflag)
     {
      for (i = 1; i < nnodes+1; i++)
        {
         if (id == nodeids[i]) valnodeno = i;
        }
     }

   if (valnodeno > 0 && valnodeno <= nnodes)
     {

      /*  If shared memory was succesfully opened,  */
      /*  store data to shared memory.              */
      if (useSharedMem)
        {

         /*  Set the start location to load shared memory (int).  */
         cLocMem = FIXEDMEM;

         /*  Store in shared memory the node number picked.  */
         strcpy(&(cBufMem[cLocMem]),"node    ");
         cLocMem = cLocMem + 8;  /*  Increase memory loc 8 bytes.  */
         iBufMem[cLocMem] = (int)id;  /*  Store the cell number.  */
         cLocMem = cLocMem + 4;  /*  Increase memory loc 4 bytes.  */

         /*  Store in shared memory the node location.  */
        strcpy(&(cBufMem[cLocMem]),"X      "); 
        cLocMem = cLocMem + 8;
        fBufMem[cLocMem] = (float)nodes[valnodeno].x;  /*  Store the float.  */
        cLocMem = cLocMem + 4;   /*  Increse memory loc 4 bytes. */

        strcpy(&(cBufMem[cLocMem]),"Y       ");
        cLocMem = cLocMem + 8; 
        fBufMem[cLocMem] = (float)nodes[valnodeno].y;
        cLocMem = cLocMem + 4;
 
        strcpy(&(cBufMem[cLocMem]),"Z       ");
        cLocMem = cLocMem + 8;
        fBufMem[cLocMem] = (float)nodes[valnodeno].z;
        cLocMem = cLocMem + 4; 

         /*  Store in shared memory the number of variables.  */
         strcpy(&(cBufMem[cLocMem]),"numVar  ");
         cLocMem = cLocMem + 8;
         iBufMem[cLocMem] = (int)numvars;
         cLocMem = cLocMem + 4;

         /*  Store the variable data in shared memory.  */
         for (i = 0; i < numvars; i++)
           {

            /*  Store variable names and values.  */
            ic = 0;
            for (i = 0; ic < 8; ic++)
              {
               /*  Blank fill 8 bytes of shared memory.  */
               cBufMem[cLocMem + ic] = ' ';
              } 

            /*  Get the length of the variable name,  */
            /*  if greater than 8, truncate to 8.     */
            ic = strlen(fieldname[i]);
            if (ic > 8) ic = 8;

            /*  Copy the name (max 8 bytes) and the data.  */                   
            strncpy(&(cBufMem[cLocMem]),fieldname[i],ic);
            cLocMem = cLocMem + 8; 
            ii = i - fldbeg;
            loc = ii * (nnodes+1) + valnodeno;
            fseek(fieldfile, loc*sizeof(double), 0);
            fread(&vvv,sizeof(double),1,fieldfile);
            fBufMem[cLocMem] = (float) vvv;
            cLocMem = cLocMem + 4;
           }

         /*  Store the end keyword and increase the memory location 8 bytes.  */
         strcpy(&(cBufMem[cLocMem]),"end     ");
         cLocMem = cLocMem + 8;
         iBufMem[cLocMem] = 0;

         /* store 0 (dummy) and increse memory loc 4 bytes */
         cLocMem = cLocMem + 4;

         /*  Set flag that new data loaded.  */
         iBufMem[NEWDATA]=1;
        }

      /*  If writing to a file, write the node number.  */
      if (query_file_flag)
        {
         fprintf(qout,"node %d\n",valnodeno);
        }

      nv = 3 + numvars + 1 + numflags_node;
      fldbeg = 3;
      for (i = 0; i < nv; i++)
        {
         vvv = 0.;
         if (i == 0) 
            sprintf(str,"X %12.8g",nodes[valnodeno].x);
         if (i == 1)
            sprintf(str,"Y %12.8g",nodes[valnodeno].y);
         if (i == 2)
            sprintf(str,"Z %12.8g",nodes[valnodeno].z);
         ii = i - fldbeg;
         if (ii >= 0 && ii < numvars)
           {
            loc = ii * (nnodes+1) + valnodeno;
            fseek(fieldfile, loc*sizeof(double), 0);
            fread(&vvv,sizeof(double),1,fieldfile);
            sprintf(str,"%-*s %12.8g",maxfldchar,fieldname[ii],vvv);
            // sprintf(str,"%-*s %16.12g",maxfldchar,fieldname[ii],vvv);
           }
         if (ii == numvars)
           {
            m = imat[valnodeno];
            if (m < 1)
               sprintf(matnm,"None");
            else
               sprintf(matnm,"%s",mmatnames[m-1]);
            sprintf(str,"Material %4d %s",m,matnm);
           }
         if (ii > numvars)
           {
            n = ii-numvars-1;
            m = flags[n][valnodeno];
            if (m < 1)
               sprintf(matnm,"None");
            else
               sprintf(matnm,"%s",flagnames_node[n][m-1]);
            sprintf(str,"%-*s %4d %s",maxflagchar,flagname_node[n],m,matnm);
           }
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(valdispnlab[i],XmNlabelString,string,NULL);
         XmStringFree(string);
         XmUpdateDisplay(valdispnlab[i]);

         /*  If writing to a file, write the line with a couple of spaces.  */
         if (query_file_flag) fprintf(qout,"  %s\n",str);
        }

      /*  If writing to a file, flush the buffer.  */
      if (query_file_flag) fflush(qout);

      valndist1 = valndist2;
      nx1 = nx2;  ny1 = ny2;  nz1 = nz2;
      valndist2 = valnodeno;
      nx2 = nodes[valnodeno].x;
      ny2 = nodes[valnodeno].y;
      nz2 = nodes[valnodeno].z;
      strcpy(str," ");
      if (valndist1 > 0)
        {
         nodedist = sqrt((nx2-nx1)*(nx2-nx1) + (ny2-ny1)*(ny2-ny1) +
                         (nz2-nz1)*(nz2-nz1));
         sprintf(str,"%12.8g",nodedist);
        }
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(valndistlab,XmNlabelString,string,NULL);
      XmStringFree(string);      
      XmUpdateDisplay(valndistlab);
     }
   else 
     {
      string = XmStringCreate("error",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(valdispnlab[0],XmNlabelString,string,NULL);
      XmStringFree(string);
      string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
      nv = 3 + numvars + 1 + numflags_node;
      for (i = 1; i < nv; i++)
         XtVaSetValues(valdispnlab[i],XmNlabelString,string,NULL);
      XtVaSetValues(valndistlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(valdispnlab[0]);
     }
}


void probe_cells(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  void get_1_point(Widget widg, XtPointer client, XtPointer call);

   cprobeflag = (int)XmToggleButtonGadgetGetState(w);
   if (cprobeflag)
     {
      get_1_point(NULL, (caddr_t) 3, NULL);
      nprobeflag = 0;
      XmToggleButtonGadgetSetState(nprobebut,FALSE,FALSE);
     }
   else 
     {
      XUndefineCursor(dpy,windowid);
     }
}


#define INSIDE 1
#define OUTSIDE 2
#define ON 567

int getcellprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int i, j, k, icell, jcell, cellid, iface, jface, itst;
  int ncpck, cpck[MAXCUTPLANES+MAXCUTSPHERES], icp;
  short gocell, *matchk;
  double xl1, yl1, zl1, xl2, yl2, zl2, xn, yn, zn;
  double aa, bb, cc, dd, x1, y1, z1, denomck;
  double denom, dist, mins;
  double ax, ay, az, bx, by, bz, s, xp, yp, zp;
  struct node xyzs[1000];
  int ptinface(double xip, double yip, double zip, int nv, struct node *xyzs,
               double A, double B, double C);

   /*  Set the points.  */
   xl1 = (double)pa;  yl1 = (double)pb;  zl1 = (double)pc;
   xl2 = (double)pd;  yl2 = (double)pe;  zl2 = (double)pf;

   /*  Set line end distance elements.  */
   ax = xl2 - xl1;
   ay = yl2 - yl1;
   az = zl2 - zl1;

   /*  Set to material or group color.  */
   if (cellfieldactive == -2) matchk = cellgrpcolr;
   else matchk = cellmats;

   /*  Check for cutplane or cutsphere cells.  */
   ncpck = 0;
   for (i = 0; i < MAXCUTPLANES+MAXCUTSPHERES; i++)
     {
      cpck[i] = 0;
      if (cutplane_on[i] > 0)
        {
         if (cutplane[i].ndcells > 0 && 
             (cutplane[i].cellflag == 1 || cutplane[i].cnumflag == 1 ||
              cutplane[i].cvelflag))
           {
            cpck[ncpck] = i;
            ncpck++;
           }
        }
     }

   /*  Loop over selected cells to find the shortest  */ 
   /*  distance from the cell center to the viewer.   */
   mins = MAXVAL;
   cellid = -1;
   for (icell = 0; icell < ncells; icell++)
     {

      /*  Check cell material, subset and select.  */
      gocell = 0;
      if (celledgesflag == 1 || cellsflag == 1 || cellnumflag == 1)
        {
         gocell = (int)cellselectflag[icell];

         /*  Eliminate cells with material < 1.  */
         if (cell0matflag == 0 && matchk[icell] < 1) gocell = 0;
 
         /*  See if the cell lies within the subset.  */
         if ((int)cellsubsetflag[icell] == 0) gocell = 0;
        }

      /*  Check grid analysis cells.  */
      if (gridanalflag == 1 && gocell == 0)
         gocell = (int)gridselectflag[icell];

      /*  If cell not selected, check cutplane or cutsphere cells.  */
      if (gocell == 0 && ncpck > 0)
        {
         for (k = 0; k < ncpck; k++)
           {
            icp = cpck[k];
            for (i = 0; i < cutplane[icp].ndcells; i++)
              {
               if (icell == cutplane[icp].dcells[i]) gocell = 1;
              }
           }
        }

      /*  If active cell, check cell ave. distance to line.  */
      if (gocell)
        {

         /*  Fill cell structure if structured grid.  */
         jcell = icell;
         if (structuredflag == 1)
           {
            jcell = 0;
            fillstructcell(icell,1);
           }

         /*  Calculate projection point of cell on the line     */
         /*  and the distance from the cell to the projection.  */
         xn = cells[jcell].xavg;
         yn = cells[jcell].yavg;
         zn = cells[jcell].zavg;
         bx = xn - xl1;
         by = yn - yl1;
         bz = zn - zl1;
         s = (ax*bx + ay*by + az*bz) / (ax*ax + ay*ay + az*az);
         xp = s*xl2 + xl1*(1.0 - s);
         yp = s*yl2 + yl1*(1.0 - s);
         zp = s*zl2 + zl1*(1.0 - s);
         dist = sqrt((xp-xn)*(xp-xn) + (yp-yn)*(yp-yn) + (zp-zn)*(zp-zn));

         /*  If the cell center is within clmaxradius of the line,  */
         /*  check that the line is within the max cell radius.     */
         if (dist <= clmaxradius && dist <= maxcellradius[icell])
           {

            /*  Now calculate the distance from   */
            /*  the line to the cell faces.       */
            for (i = 0; i < cells[jcell].nfaces; i++)
              {
               iface = cells[jcell].facenos[i];
               jface = iface;
               if (structuredflag == 1) jface = i;

               /*  Get the face normal and calculate the        */
               /*  intersection of the probe line to the face.  */ 
               getfacenorm(jface, &aa, &bb, &cc);
               k = cellfaces[jface].fverts[0];
               x1 = nodes[k].x;
               y1 = nodes[k].y;
               z1 = nodes[k].z;
               dd = -x1*aa - y1*bb - z1*cc;
               denom = aa*ax + bb*ay + cc*az;
               denomck = (float)(fabs((double)denom));
               s = 1.1;
               if (denomck > 1.0e-10)
                  s = -(aa*xl1 + bb*yl1 + cc*zl1 + dd) / denom;
               if (fabs((double)s) < 9.0e-7) s = 0.;
               denomck = s - 1.;
               if (fabs((double)denomck) < 9.0e-7) s = 1.;
               if (s >= 0. && s <= 1.)
                 {
                  xn = xl1 + s * (xl2 - xl1);
                  yn = yl1 + s * (yl2 - yl1);
                  zn = zl1 + s * (zl2 - zl1);

                  /*  Check that the intersection point lies in  */
                  /*  the face, if so, save closest to viewer.   */
                  for (j = 0; j < cellfaces[jface].nverts; j++)
                     {
                      k = cellfaces[jface].fverts[j];
                      xyzs[j].x = nodes[k].x;
                      xyzs[j].y = nodes[k].y;
                      xyzs[j].z = nodes[k].z;
                     }
                  itst = ptinface(xn, yn, zn, cellfaces[jface].nverts,
                         xyzs, aa, bb, cc);
                  if (itst != OUTSIDE)
                    {              
                     if (s < mins)
                       {
                        cellid = icell;
                        mins = s;
                       }
                    }
                 }
              }
           } 
        }
     }
   return cellid;
}


void doitcprobe(pa,pb,pc,pd,pe,pf)
 float pa,pb,pc,pd,pe,pf;
{
  int cellid, id;
  char str[10];
  void getvalctxt(Widget w, XtPointer client_data, XtPointer call_data);

   XDefineCursor(dpy,topwin,waitcsr);
   XFlush(dpy);

   /*  Get the cell number.  */
   cellid = getcellprobe(pa,pb,pc,pd,pe,pf);

   /*  Set cellid in text.  */
   id = cellid + 1;
   if (cellidflag) id = cellids[cellid];
   sprintf(str,"%d",id);
   XtVaSetValues(valctxt,XmNvalue,str,NULL);
   XmUpdateDisplay(valctxt);
   XUndefineCursor(dpy,topwin);
   XFlush(dpy);

   getvalctxt(NULL,NULL,NULL);
}


void getvalctxt(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int icell, jcell, id;
  char str[90], matnm[33];
  double vvv;
  XmString string;
  char *tmpstr;
  int i, ii, nv, fldbeg, loc, m, n, maxflagchar, maxfldchar;

   /*  Calculate max field name char. size.  */
   maxfldchar = 8;
   for (i = 0; i < cnumvars; i++)
     {
      ii = strlen(cfieldname[i]);
      if (ii > maxfldchar) maxfldchar = ii;
     } 

   /*  Calculate max flag name char size.  */
   maxflagchar = 8;
   for (i = 0; i < numflags_cell; i++)
     {
      ii = strlen(flagname_cell[i]);
      if (ii > maxflagchar) maxflagchar = ii;
     }

   tmpstr = XmTextFieldGetString(valctxt);
   id = atoi(tmpstr);
   XtFree(tmpstr);
   valcellno = id;
   if (cellidflag)
     {
      for (i = 0; i < ncells; i++)
        {
         if (id == cellids[i]) valcellno = i+1;
        }
     }

   if (valcellno > 0 && valcellno <= ncells)
     {
      icell = valcellno - 1;
      jcell = icell;
      if (structuredflag == 1)
        {
         jcell = 0;
         fillstructcell(icell,0);
        }

      /*  If shared memory was succesfully opened,  */
      /*  store data to shared memory.              */
      if (useSharedMem)
        {

         /*  Set the start location to load shared memory (int).  */
         cLocMem = FIXEDMEM;

         /*  Store in shared memory the cell number picked.  */
         strcpy(&(cBufMem[cLocMem]),"cell    ");
         cLocMem = cLocMem + 8;  /*  Increase memory loc 8 bytes.  */
         iBufMem[cLocMem] = (int)id;  /*  Store the cell number.  */
         cLocMem = cLocMem + 4;  /*  Increase memory loc 4 bytes.  */

         /*  Store in shared memory the cell center location.  */
        strcpy(&(cBufMem[cLocMem]),"xCen    "); 
        cLocMem = cLocMem + 8;
        fBufMem[cLocMem] = (float)cells[jcell].xavg;  /*  Store the float.  */
        cLocMem = cLocMem + 4;   /*  Increse memory loc 4 bytes. */

        strcpy(&(cBufMem[cLocMem]),"yCen    ");
        cLocMem = cLocMem + 8; 
        fBufMem[cLocMem] = (float)cells[jcell].yavg;
        cLocMem = cLocMem + 4;
 
        strcpy(&(cBufMem[cLocMem]),"zCen    ");
        cLocMem = cLocMem + 8;
        fBufMem[cLocMem] = (float)cells[jcell].zavg;
        cLocMem = cLocMem + 4; 

         /*  Store in shared memory the number of variables.  */
         strcpy(&(cBufMem[cLocMem]),"numVar  ");
         cLocMem = cLocMem + 8;
         iBufMem[cLocMem] = (int)cnumvars;
         cLocMem = cLocMem + 4;

         /*  Store the variable data in shared memory.  */
         for (i = 0; i < cnumvars; i++)
           {

            /*  Store variable names and values.  */
            for (ii = 0; ii < 8; ii++)
              {
               /*  Blank fill 8 bytes of shared memory.  */
               cBufMem[cLocMem + ii] = ' ';
              } 

            /*  Get the length of the variable name,  */
            /*  if greater than 8, truncate to 8.     */
            ii = strlen(cfieldname[i]);
            if (ii > 8) ii = 8;

            /*  Copy the name (max 8 bytes) and the data.  */                   
            strncpy(&(cBufMem[cLocMem]),cfieldname[i],ii);
            cLocMem = cLocMem + 8; 
            loc = i * ncells + icell;  
            fseek(cfieldfile, loc * sizeof(double), 0);
            fread(&vvv,sizeof(double),1,cfieldfile);
            fBufMem[cLocMem] = (float) vvv;
            cLocMem = cLocMem + 4;
           }

         /*  Store the end keyword and increase the memory location 8 bytes.  */
         strcpy(&(cBufMem[cLocMem]),"end     ");
         cLocMem = cLocMem + 8;
         iBufMem[cLocMem] = 0;

         /* store 0 (dummy) and increse memory loc 4 bytes */
         cLocMem = cLocMem + 4;

         /*  Set flag that new data loaded.  */
         iBufMem[NEWDATA]=1;
        }

      /*  If writing to a file, write the cell number.  */
      if (query_file_flag)
        {
         fprintf(qout,"cell %d\n",valcellno);
        }

      nv = 3 + cnumvars + 1 + numflags_cell;
      fldbeg = 3;
      for (i = 0; i < nv; i++)
        {
         vvv = 0.;
         if (i == 0) 
            sprintf(str,"X Cen. %12.8g",cells[jcell].xavg);
         if (i == 1)
            sprintf(str,"Y Cen. %12.8g",cells[jcell].yavg);
         if (i == 2)
            sprintf(str,"Z Cen. %12.8g",cells[jcell].zavg);
         ii = i - fldbeg;
         if (ii >= 0 && ii < cnumvars)
           {
            loc = ii * ncells + icell;
            fseek(cfieldfile, loc*sizeof(double), 0);
            fread(&vvv,sizeof(double),1,cfieldfile);
            sprintf(str,"%-*s %12.8g",maxfldchar,cfieldname[ii],vvv);
           }
         if (ii == cnumvars)
           {
            m = cellmats[icell];
            if (m < 1)
               sprintf(matnm,"None");
            else
               sprintf(matnm,"%s",mmatnames[m-1]);
            sprintf(str,"Material %4d %s",m,matnm);
           }
         if (ii > cnumvars)
           {
            n = ii-cnumvars-1;
            m = cellflags[n][icell];
            if (m < 1)
               sprintf(matnm,"None");
            else
               sprintf(matnm,"%s",flagnames_cell[n][m-1]);
            sprintf(str,"%-*s %4d %s",maxflagchar,flagname_cell[n],m,matnm);
           }
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(valdispclab[i],XmNlabelString,string,NULL);
         XmStringFree(string);

         /*  If writing to a file, write the line with a couple of spaces.  */
         if (query_file_flag) fprintf(qout,"  %s\n",str);
        }

      /*  If writing to a file, flush the buffer.  */
      if (query_file_flag) fflush(qout);

      for (i = 0; i < cells[jcell].ncnodes; i++)
        {
         id = cells[jcell].cnodes[i];
         if (nodeidflag) id = nodeids[id];
         sprintf(str,"%8d",id);
         string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(valcnodeids[i],XmNlabelString,string,NULL);
         XmStringFree(string);
        }
      for (i = cells[jcell].ncnodes; i < maxnode; i++)
        {
         string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
         XtVaSetValues(valcnodeids[i],XmNlabelString,string,NULL);
         XmStringFree(string);
         XmUpdateDisplay(valcnodeids[i]);
        }
     }
   else
     {
      string = XmStringCreate("error",XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(valdispclab[0],XmNlabelString,string,NULL);
      XmStringFree(string);
      string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
      nv = 3 + cnumvars + 1 + numflags_cell;
      for (i = 1; i < nv; i++)
         XtVaSetValues(valdispclab[i],XmNlabelString,string,NULL);
      for (i = 0; i < maxnode; i++)
         XtVaSetValues(valcnodeids[i],XmNlabelString,string,NULL);
      XmStringFree(string);
      XmUpdateDisplay(valdispclab[0]);
     }
}


void popnfldval(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[40];
  XmString string;
  void nfvgetminmax();

   i = querynfield;
   sprintf(str,"Field: %s",fieldname[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvnodelab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"Min: %10.7g",fieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max %10.7g",fieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   nfvgetminmax();

   MyManageChild(nfldvalform);
}


void popcfldval(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i;
  char str[40];
  XmString string;
  void cfvgetminmax();

   i = querycfield;
   sprintf(str,"Field: %s",cfieldname[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvcelllab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"Min: %10.7g",cfieldmin[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max %10.7g",cfieldmax[i]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cfvgetminmax();

   MyManageChild(cfldvalform);
}


void nfvalquit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(nfldvalform);
   XtUnmanageChild(nfieldform);
}


void cfvalquit(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
   XtUnmanageChild(cfldvalform);
   XtUnmanageChild(cfieldform);
}


void nfvfldsel(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(nfieldform, XmNx,fx, XmNy,fy, NULL);
 
   nfieldformpop(NFLDQUERY, querynfield);
}


void cfvfldsel(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  Position  wx, wy, fx, fy, p0 = 0;

   /*  Calculate field pop-up postition from widgets.  */
   XtTranslateCoords(w, p0,p0, &wx,&wy); 
   fx = wx + 10;
   fy = wy + 30;
   XtVaSetValues(cfieldform, XmNx,fx, XmNy,fy, NULL);
 
   cfieldformpop(CFLDQUERY, querycfield);
}


void nfvapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, icount, nodeid, id;
  double *value;
  float fval;
  double mindiff, diff;
  char str[30], *tmpstr;
  XmString string;
  double * readnodevalue(int ifld);

   /*  Get value.  */

   tmpstr = XmTextFieldGetString(nfvvaltxt);
   fval = atof(tmpstr); 
   XtFree(tmpstr);
  
   /*  Get values for the selected node field.  */
   value = readnodevalue(querynfield);

   /*  Loop through nodes to find closest node to selected value.  */
   icount = 0;
   nodeid = 0;
   mindiff = MAXVAL;
   for (i = 1; i < nnodes+1; i++)
     {
      diff = fabs(value[i] - fval);
      if (diff <= mindiff)
        {
         if (diff != mindiff) nodeid = i;
         mindiff = diff;
         if (diff == 0) icount++;
        }
     }

   /*  Set nodeid in text.  */
   id = nodeid;
   if (nodeidflag) id = nodeids[nodeid];
   sprintf(str," %d",id);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvmatchlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   XFlush(dpy);

   /*  Show exact match count.  */
   if (icount > 0)
     {
      sprintf(str,"Matches: %d",icount);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nfvvallab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   /*  Otherwise, show nearest value.  */
   else
     {
      sprintf(str,"Node Value: %10.7g",value[nodeid]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(nfvvallab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   
   free(value);

}


void cfvapply(w,client_data,call_data)
 Widget w;
 XtPointer client_data, call_data;
{
  int i, icount, cellid, id;
  double *value;
  float fval;
  double mindiff, diff;
  char str[30], *tmpstr;
  XmString string;
  double * readcellvalue(int ifld);

   /*  Get value.  */

   tmpstr = XmTextFieldGetString(cfvvaltxt);
   fval = atof(tmpstr); 
   XtFree(tmpstr);
  
   /*  Get values for the selected cell field.  */
   value = readcellvalue(querycfield);

   /*  Loop through cells to find closest cell to selected value.  */
   icount = 0;
   cellid = -2;
   mindiff = MAXVAL;
   for (i = 0; i < ncells; i++)
     {
      diff = fabs((double) (value[i] - fval));
      if (diff <= mindiff)
        {
         if (diff != mindiff) cellid = i+1;
         mindiff = diff;
         if (diff == 0) icount++;
        }
     }

   /*  Set cellid in text.  */
   id = cellid;
   if (cellidflag) id = cellids[cellid];
   sprintf(str," %d",id);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvmatchlab,XmNlabelString,string,NULL);
   XFlush(dpy);

   /*  Show exact match count.  */
   if (icount > 0)
     {
      sprintf(str,"Matches: %d",icount);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfvvallab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }

   /*  Otherwise, show nearest value.  */
   else
     {
      sprintf(str,"Cell Value: %10.7g",value[cellid-1]);
      string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
      XtVaSetValues(cfvvallab,XmNlabelString,string,NULL);
      XmStringFree(string);
     }
   
   free(value);

}


void makequeryform(parent)
 Widget parent;
{
  Widget lastwidget, vquitbut, valnlab, valntxtlab, valnoutbut, 
         valcnodelab, valctxtlab, valswin, valsep, valclab,
         valcoutbut, valswinform, nfldvalbut, nfvquitbut, nfvlab1, 
         nfvappbut, cfldvalbut, cvalswin, cvalsep, cvalswinform,
         cfvquitbut, cfvlab1, cfvappbut, valndistlab1, cnodeswin,
         cnodeswinform, cnodesep; 
  int i, ii, nv;
  Arg wargs[20];
  char str[20];
  XmString string;

   string = XmStringCreate("Query Data",XmSTRING_DEFAULT_CHARSET);
   i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
   i++; XtSetArg(wargs[i], XmNx, 500);
   i++; XtSetArg(wargs[i], XmNy, 10);
   i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   queryform = XmCreateFormDialog(parent,"Query",wargs,i);
      XmStringFree(string);
  
   string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
   vquitbut = XtVaCreateManagedWidget("Vquitbut",
              xmPushButtonGadgetClass,
              queryform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_FORM,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              NULL);
      XmStringFree(string);
      XtAddCallback(vquitbut,XmNactivateCallback,valquit,NULL);

   string = XmStringCreate("Write to File",XmSTRING_DEFAULT_CHARSET);
   vfilebut = XtVaCreateManagedWidget("Vfilebut",
              xmToggleButtonGadgetClass,
              queryform,
              XmNtopAttachment, XmATTACH_FORM,
              XmNleftAttachment, XmATTACH_WIDGET,
              XmNleftWidget, vquitbut,
              XmNleftOffset, 20,
              XmNrightAttachment, XmATTACH_NONE,
              XmNbottomAttachment, XmATTACH_NONE,
              XmNlabelString, string,
              XmNselectColor, yellowpix,
              XmNset, FALSE,
              NULL);
      XmStringFree(string);
      XtAddCallback(vfilebut,XmNvalueChangedCallback,qdwrite,NULL);

   /*  Create node input widgets.  */
   string = XmStringCreate("Node Values",XmSTRING_DEFAULT_CHARSET);
   valnlab = XtVaCreateManagedWidget("Valnlab",
             xmLabelGadgetClass,
             queryform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, vquitbut,
             XmNleftAttachment, XmATTACH_FORM,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNlabelString, string,
             NULL);
      XmStringFree(string);

   string = XmStringCreate("Node:",XmSTRING_DEFAULT_CHARSET);
   valntxtlab = XtVaCreateManagedWidget("Valntxtlab",
                xmLabelGadgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, valnlab,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);

   valntxt = XtVaCreateManagedWidget("Valntxt",
             xmTextFieldWidgetClass,
             queryform,
             XmNtopAttachment, XmATTACH_WIDGET,
             XmNtopWidget, valnlab,
             XmNleftAttachment, XmATTACH_WIDGET,
             XmNleftWidget, valntxtlab,
             XmNrightAttachment, XmATTACH_NONE,
             XmNbottomAttachment, XmATTACH_NONE,
             XmNmarginHeight, 0,
             XmNmarginWidth, 1,
             XmNmaxLength, 8,
             XmNcolumns, 8,
             XmNvalue, "",
             NULL);
      XtAddCallback(valntxt,XmNactivateCallback,getvalntxt,NULL);

   string = XmStringLtoRCreate("Get Node Data\nFrom Screen",
                               XmSTRING_DEFAULT_CHARSET);
   nprobebut = XtVaCreateManagedWidget("Nprobebut",
               xmToggleButtonGadgetClass,
               queryform,
               XmNtopAttachment, XmATTACH_WIDGET,
               XmNtopWidget, valntxt,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNlabelString, string,
               XmNselectColor, yellowpix,
               XmNset, FALSE,
               NULL);
      XmStringFree(string);
      XtAddCallback(nprobebut,XmNvalueChangedCallback,
                    probe_nodes,NULL);
  
   string = XmStringLtoRCreate("Get Node No.\nFrom Field Value",
                               XmSTRING_DEFAULT_CHARSET);
   nfldvalbut = XtVaCreateManagedWidget("Nfldvalbut",
                xmPushButtonGadgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nprobebut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(nfldvalbut,XmNactivateCallback,popnfldval,NULL);    
      querynfield = 0;
      if (numvars < 1) XtSetSensitive(nfldvalbut,FALSE);

   string = XmStringCreate("Get Values",XmSTRING_DEFAULT_CHARSET);
   valnoutbut = XtVaCreateManagedWidget("Valnoutbut",
                xmPushButtonGadgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nfldvalbut,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(valnoutbut,XmNactivateCallback,getvalntxt,NULL);

   /*  Create node value labels.  */
   i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
   i++;  XtSetArg(wargs[i],XmNtopWidget, valnoutbut);
   i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_FORM);
   i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
   i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
   i++;  XtSetArg(wargs[i],XmNwidth, 260);
   i++;  XtSetArg(wargs[i],XmNheight, 200);
   i++;
   valswin = XmCreateScrolledWindow(queryform,"Valswin",wargs,i);
   XtManageChild(valswin);

   i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
   i++;
   valswinform = XmCreateForm(valswin,"Valswinform",wargs,i);
   XtManageChild(valswinform);

   valsep = XtVaCreateManagedWidget("Valsep",
            xmSeparatorGadgetClass,
            valswinform,
            XmNtopAttachment, XmATTACH_FORM,
            XmNleftAttachment, XmATTACH_FORM,
            XmNrightAttachment, XmATTACH_NONE,
            XmNbottomAttachment, XmATTACH_NONE,
            XmNseparatorType, XmNO_LINE,
            NULL);

   if (gmvflag)
      nv = 3 + numvars + 1 + numflags_node;
   if (physedflag)
      nv = 3 + MAXFIELDS + 1 + MAXFLAGS;
   lastwidget = valsep;
   for (i = 0; i < nv; i++)
     {
      sprintf(str,"Valdispnlab%d",i);
      string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
      valdispnlab[i] = XtVaCreateManagedWidget(str,
                       xmLabelGadgetClass,
                       valswinform,
                       XmNtopAttachment, XmATTACH_WIDGET,
                       XmNtopWidget, lastwidget,
                       XmNleftAttachment, XmATTACH_FORM,
                       XmNrightAttachment, XmATTACH_NONE,
                       XmNbottomAttachment, XmATTACH_NONE,
                       XmNlabelString, string,
                       NULL);
         XmStringFree(string);
      lastwidget = valdispnlab[i];
     }

   string = XmStringLtoRCreate("Distance between\nlast 2 nodes:",
                           XmSTRING_DEFAULT_CHARSET);
   valndistlab1 = XtVaCreateManagedWidget("Valndistlab1",
                  xmLabelGadgetClass,
                  queryform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, valswin,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
      XmStringFree(string);

   string = XmStringLtoRCreate(" ",XmSTRING_DEFAULT_CHARSET);
   valndistlab = XtVaCreateManagedWidget("Valndistlab",
                 xmLabelGadgetClass,
                 queryform,
                 XmNtopAttachment, XmATTACH_WIDGET,
                 XmNtopWidget, valndistlab1,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNlabelString, string,
                 NULL);
      XmStringFree(string);


   /*  Create cell input widgets.  */
   if (ncells > 0)
     {
      string = XmStringCreate("Cell Info",XmSTRING_DEFAULT_CHARSET);
      valclab = XtVaCreateManagedWidget("Valclab",
                xmLabelGadgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, vquitbut,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, valswin,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);

      string = XmStringCreate("Cell:",XmSTRING_DEFAULT_CHARSET);
      valctxtlab = XtVaCreateManagedWidget("Valctxtlab",
                   xmLabelGadgetClass,
                   queryform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, valclab,
                   XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNleftWidget, valclab,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);

      valctxt = XtVaCreateManagedWidget("Valctxt",
                xmTextFieldWidgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, valclab,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, valctxtlab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNmarginHeight, 0,
                XmNmarginWidth, 1,
                XmNmaxLength, 8,
                XmNcolumns, 8,
                XmNvalue, "",
                NULL);
         XtAddCallback(valctxt,XmNactivateCallback,getvalctxt,NULL);

      string = XmStringLtoRCreate("Get Cell Data\nFrom Screen",
                                  XmSTRING_DEFAULT_CHARSET);
      cprobebut = XtVaCreateManagedWidget("Cprobebut",
                  xmToggleButtonGadgetClass,
                  queryform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, valctxt,
                  XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                  XmNleftWidget, valclab,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  XmNselectColor, yellowpix,
                  XmNset, FALSE,
                  NULL);
         XmStringFree(string);
         XtAddCallback(cprobebut,XmNvalueChangedCallback,
                       probe_cells,NULL);
  
   string = XmStringLtoRCreate("Get Cell No.\nFrom Field Value",
                               XmSTRING_DEFAULT_CHARSET);
   cfldvalbut = XtVaCreateManagedWidget("Cfldvalbut",
                xmPushButtonGadgetClass,
                queryform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cprobebut,
                XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNleftWidget, valclab,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
      XmStringFree(string);
      XtAddCallback(cfldvalbut,XmNactivateCallback,popcfldval,NULL);
      querycfield = 0;
      if (cnumvars < 1) XtSetSensitive(cfldvalbut,FALSE);

      string = XmStringCreate("Cell Values",XmSTRING_DEFAULT_CHARSET);
      valcoutbut = XtVaCreateManagedWidget("Valcoutbut",
                   xmPushButtonGadgetClass,
                   queryform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cfldvalbut,
                   XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                   XmNleftWidget, valclab,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(valcoutbut,XmNactivateCallback,getvalctxt,NULL);

      /*  Create cell value labels.  */
      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, valnoutbut);
      i++;  XtSetArg(wargs[i],XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET);
      i++;  XtSetArg(wargs[i],XmNleftWidget, valclab);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
      i++;  XtSetArg(wargs[i],XmNwidth, 260);
      i++;  XtSetArg(wargs[i],XmNheight, 200);
      i++;
      cvalswin = XmCreateScrolledWindow(queryform,"Cvalswin",wargs,i);
      XtManageChild(cvalswin);

      i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cvalswinform = XmCreateForm(cvalswin,"Cvalswinform",wargs,i);
      XtManageChild(cvalswinform);

      cvalsep = XtVaCreateManagedWidget("Cvalsep",
               xmSeparatorGadgetClass,
               cvalswinform,
               XmNtopAttachment, XmATTACH_FORM,
               XmNleftAttachment, XmATTACH_FORM,
               XmNrightAttachment, XmATTACH_NONE,
               XmNbottomAttachment, XmATTACH_NONE,
               XmNseparatorType, XmNO_LINE,
               NULL);

      if (gmvflag)
         nv = 3 + cnumvars + 1 + numflags_cell;
      if (physedflag)
         nv = 3 + MAXFIELDS + 1 + MAXFLAGS;
      lastwidget = cvalsep;
      for (i = 0; i < nv; i++)
        {
         sprintf(str,"Valdispclab%d",i);
         string = XmStringCreate("          ",XmSTRING_DEFAULT_CHARSET);
         valdispclab[i] = XtVaCreateManagedWidget(str,
                          xmLabelGadgetClass,
                          cvalswinform,
                          XmNtopAttachment, XmATTACH_WIDGET,
                          XmNtopWidget, lastwidget,
                          XmNleftAttachment, XmATTACH_FORM,
                          XmNrightAttachment, XmATTACH_NONE,
                          XmNbottomAttachment, XmATTACH_NONE,
                          XmNlabelString, string,
                          NULL);
            XmStringFree(string);
         lastwidget = valdispclab[i];
        }

      /*  Create cell node labels.  */
      string = XmStringCreate("Cell Node Ids",
                              XmSTRING_DEFAULT_CHARSET);
      valcnodelab = XtVaCreateManagedWidget("Valcnodelab",
                    xmLabelGadgetClass,
                    queryform,
                    XmNtopAttachment, XmATTACH_WIDGET,
                    XmNtopWidget, cvalswin,
                    XmNtopOffset, 10,
                    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
                    XmNleftWidget, valclab,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_NONE,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);

      /*  Create cell node scrolled window.  */
      i=0;  XtSetArg(wargs[i],XmNtopAttachment, XmATTACH_WIDGET);
      i++;  XtSetArg(wargs[i],XmNtopWidget, valcnodelab);
      i++;  XtSetArg(wargs[i],XmNleftAttachment,  XmATTACH_OPPOSITE_WIDGET);
      i++;  XtSetArg(wargs[i],XmNleftWidget, valclab);
      i++;  XtSetArg(wargs[i],XmNrightAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNbottomAttachment, XmATTACH_NONE);
      i++;  XtSetArg(wargs[i],XmNscrollingPolicy, XmAUTOMATIC);
      i++;  XtSetArg(wargs[i],XmNwidth, 260);
      i++;  XtSetArg(wargs[i],XmNheight, 150);
      i++;
      cnodeswin = XmCreateScrolledWindow(queryform,"Cnodeswin",wargs,i);
      XtManageChild(cnodeswin);

      i=0; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cnodeswinform = XmCreateForm(cnodeswin,"Cnodeswinform",wargs,i);
      XtManageChild(cnodeswinform);

      cnodesep = XtVaCreateManagedWidget("Valsep",
                 xmSeparatorGadgetClass,
                 cnodeswinform,
                 XmNtopAttachment, XmATTACH_FORM,
                 XmNleftAttachment, XmATTACH_FORM,
                 XmNrightAttachment, XmATTACH_NONE,
                 XmNbottomAttachment, XmATTACH_NONE,
                 XmNseparatorType, XmNO_LINE,
                 NULL);

      /*  Determine the max. no. of nodes in cells.  */
      maxnode = 0;
      if (structuredflag == 1) maxnode = 8;
      else
        {
         for (i = 0; i < ncells; i++)
           {
            ii = i;
            maxnode = (cells[ii].ncnodes > maxnode) ? 
                      cells[ii].ncnodes : maxnode;
           }
        }
      lastwidget = valcnodelab;
      for (i = 0; i < maxnode; i++)
        {
         sprintf(str,"Valcnodeids%d",i);
         string = XmStringCreate("        ",XmSTRING_DEFAULT_CHARSET);
         valcnodeids[i] = XtVaCreateManagedWidget(str,
                          xmLabelGadgetClass,
                          cnodeswinform,
                          XmNtopAttachment, XmATTACH_WIDGET,
                          XmNtopWidget, lastwidget,
                          XmNleftAttachment, XmATTACH_FORM,
                          XmNrightAttachment, XmATTACH_NONE,
                          XmNbottomAttachment, XmATTACH_NONE,
                          XmNlabelString, string,
                          NULL);
            XmStringFree(string);
         lastwidget = valcnodeids[i];
        }
     }   

   /*  Create Field Value Form.  */

   if (numvars > 0)
     {
      string = XmStringCreate("Get Node by Field Value",
                               XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 500);
      i++; XtSetArg(wargs[i], XmNy, 10);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      nfldvalform = XmCreateFormDialog(queryform,"Nfldvalform",wargs,i);
         XmStringFree(string);
  
      string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
      nfvquitbut = XtVaCreateManagedWidget("Nfvquitbut",
                   xmPushButtonGadgetClass,
                   nfldvalform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(nfvquitbut,XmNactivateCallback,nfvalquit,NULL);

      string = XmStringCreate("Field:        ",XmSTRING_DEFAULT_CHARSET);
      nfvnodelab = XtVaCreateManagedWidget("Nfvnodelab",
                   xmLabelGadgetClass,
                   nfldvalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, nfvquitbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
  
      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      nfvselbut = XtVaCreateManagedWidget("Nfvselbut",
                  xmPushButtonGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvnodelab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(nfvselbut,XmNactivateCallback,nfvfldsel,NULL);

      string = XmStringCreate("Min:          ",XmSTRING_DEFAULT_CHARSET);
      nfvminlab = XtVaCreateManagedWidget("Nfvminlab",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvselbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Node:    ",XmSTRING_DEFAULT_CHARSET);
      nfvminlab1 = XtVaCreateManagedWidget("Nfvminlab1",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvminlab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Matches:    ",XmSTRING_DEFAULT_CHARSET);
      nfvminlab2 = XtVaCreateManagedWidget("Nfvminlab2",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvminlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Max:          ",XmSTRING_DEFAULT_CHARSET);
      nfvmaxlab = XtVaCreateManagedWidget("Nfvmaxlab",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvminlab2,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Node:    ",XmSTRING_DEFAULT_CHARSET);
      nfvmaxlab1 = XtVaCreateManagedWidget("Nfvmaxlab1",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvmaxlab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Matches:    ",XmSTRING_DEFAULT_CHARSET);
      nfvmaxlab2 = XtVaCreateManagedWidget("Nfvmaxlab2",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvmaxlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Match Value:",XmSTRING_DEFAULT_CHARSET);
      nfvlab1 = XtVaCreateManagedWidget("Nfvlab4",
                xmLabelGadgetClass,
                nfldvalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, nfvmaxlab2,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);

      nfvvaltxt = XtVaCreateManagedWidget("Nfvvaltxt",
                  xmTextFieldWidgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvmaxlab2,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, nfvlab1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 12,
                  XmNvalue, "",
                  NULL);
  
      string = XmStringCreate("Get Node",XmSTRING_DEFAULT_CHARSET);
      nfvappbut = XtVaCreateManagedWidget("Nfvappbut",
                  xmPushButtonGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(nfvappbut,XmNactivateCallback,nfvapply,NULL);

      string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
      nfvmatchlab = XtVaCreateManagedWidget("Nfvmatchlab",
                    xmLabelGadgetClass,
                    nfldvalform,
                    XmNtopAttachment, XmATTACH_NONE,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, nfvappbut,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
                    XmNbottomWidget, nfvappbut,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      nfvvallab = XtVaCreateManagedWidget("Nfvvallab",
                  xmLabelGadgetClass,
                  nfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, nfvappbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
   }

   /*  Create Cell Field Value Form.  */

   if (cnumvars > 0)
     {
      string = XmStringCreate("Get Cell by Field Value",
                               XmSTRING_DEFAULT_CHARSET);
      i=0; XtSetArg(wargs[i], XmNdialogTitle,string);
      i++; XtSetArg(wargs[i], XmNx, 500);
      i++; XtSetArg(wargs[i], XmNy, 10);
      i++; XtSetArg(wargs[i], XmNautoUnmanage, FALSE);
      i++;
      cfldvalform = XmCreateFormDialog(queryform,"Cfldvalform",wargs,i);
         XmStringFree(string);
  
      string = XmStringCreate(" Close ",XmSTRING_DEFAULT_CHARSET);
      cfvquitbut = XtVaCreateManagedWidget("Cfvquitbut",
                   xmPushButtonGadgetClass,
                   cfldvalform,
                   XmNtopAttachment, XmATTACH_FORM,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
         XtAddCallback(cfvquitbut,XmNactivateCallback,cfvalquit,NULL);

      string = XmStringCreate("Field:        ",XmSTRING_DEFAULT_CHARSET);
      cfvcelllab = XtVaCreateManagedWidget("Cfvcelllab",
                   xmLabelGadgetClass,
                   cfldvalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cfvquitbut,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);
  
      string = XmStringCreate("New Field",XmSTRING_DEFAULT_CHARSET);
      cfvselbut = XtVaCreateManagedWidget("Cfvselbut",
                  xmPushButtonGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvcelllab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(cfvselbut,XmNactivateCallback,cfvfldsel,NULL);

      string = XmStringCreate("Min:          ",XmSTRING_DEFAULT_CHARSET);
      cfvminlab = XtVaCreateManagedWidget("cfvminlab",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvselbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Cell:    ",XmSTRING_DEFAULT_CHARSET);
      cfvminlab1 = XtVaCreateManagedWidget("Cfvminlab1",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvminlab,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Matches:    ",XmSTRING_DEFAULT_CHARSET);
      cfvminlab2 = XtVaCreateManagedWidget("Cfvminlab2",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvminlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Max:          ",XmSTRING_DEFAULT_CHARSET);
      cfvmaxlab = XtVaCreateManagedWidget("Cfvmaxlab",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvminlab2,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Cell:    ",XmSTRING_DEFAULT_CHARSET);
      cfvmaxlab1 = XtVaCreateManagedWidget("Cfvmaxlab1",
                   xmLabelGadgetClass,
                   cfldvalform,
                   XmNtopAttachment, XmATTACH_WIDGET,
                   XmNtopWidget, cfvmaxlab,
                   XmNleftAttachment, XmATTACH_FORM,
                   XmNleftOffset, 20,
                   XmNrightAttachment, XmATTACH_NONE,
                   XmNbottomAttachment, XmATTACH_NONE,
                   XmNlabelString, string,
                   NULL);
         XmStringFree(string);

      string = XmStringCreate("Matches:    ",XmSTRING_DEFAULT_CHARSET);
      cfvmaxlab2 = XtVaCreateManagedWidget("Cfvmaxlab2",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvmaxlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 20,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);

      string = XmStringCreate("Match Value:",XmSTRING_DEFAULT_CHARSET);
      cfvlab1 = XtVaCreateManagedWidget("Cfvlab4",
                xmLabelGadgetClass,
                cfldvalform,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, cfvmaxlab2,
                XmNleftAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_NONE,
                XmNbottomAttachment, XmATTACH_NONE,
                XmNlabelString, string,
                NULL);
         XmStringFree(string);

      cfvvaltxt = XtVaCreateManagedWidget("Cfvvaltxt",
                  xmTextFieldWidgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvmaxlab2,
                  XmNleftAttachment, XmATTACH_WIDGET,
                  XmNleftWidget, cfvlab1,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNmarginHeight, 0,
                  XmNmarginWidth, 1,
                  XmNmaxLength, 20,
                  XmNcolumns, 12,
                  XmNvalue, "",
                  NULL);
  
      string = XmStringCreate("Get Cell",XmSTRING_DEFAULT_CHARSET);
      cfvappbut = XtVaCreateManagedWidget("cfvappbut",
                  xmPushButtonGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvlab1,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
         XtAddCallback(cfvappbut,XmNactivateCallback,cfvapply,NULL);

      string = XmStringCreate("     ",XmSTRING_DEFAULT_CHARSET);
      cfvmatchlab = XtVaCreateManagedWidget("Cfvmatchlab",
                    xmLabelGadgetClass,
                    cfldvalform,
                    XmNtopAttachment, XmATTACH_NONE,
                    XmNleftAttachment, XmATTACH_WIDGET,
                    XmNleftWidget, cfvappbut,
                    XmNrightAttachment, XmATTACH_NONE,
                    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
                    XmNbottomWidget, cfvappbut,
                    XmNlabelString, string,
                    NULL);
         XmStringFree(string);

      string = XmStringCreate(" ",XmSTRING_DEFAULT_CHARSET);
      cfvvallab = XtVaCreateManagedWidget("Cfvvallab",
                  xmLabelGadgetClass,
                  cfldvalform,
                  XmNtopAttachment, XmATTACH_WIDGET,
                  XmNtopWidget, cfvappbut,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNleftOffset, 10,
                  XmNrightAttachment, XmATTACH_NONE,
                  XmNbottomAttachment, XmATTACH_NONE,
                  XmNlabelString, string,
                  NULL);
         XmStringFree(string);
   }

}



void nfvgetminmax()
{
  int i, mincount, maxcount, minid, maxid;
  double *value, fmin, fmax;
  char str[20];
  XmString string;
  double * readnodevalue(int ifld);

   /*  Get values for the selected field.  */
   value = readnodevalue(querynfield);

   /*  Loop through nodes to find node with min & max values.  */
   mincount = 0;   maxcount = 0;
   minid = 0;   maxid = 0;
   fmin = fieldmin[querynfield];
   fmax = fieldmax[querynfield];
   for (i = 1; i < nnodes+1; i++)
     {
      if (value[i] == fmin)
        {
         mincount++;
         if (mincount == 1) minid = i;
        }
      else if (value[i] == fmax)
        {
         maxcount++;
         if (maxcount == 1) maxid = i;
        }
     }

   /*  Set min node and count widgets.  */
   sprintf(str,"Node: %d",minid);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvminlab1,XmNlabelString,string,NULL);
   XFlush(dpy);
 
   sprintf(str,"Matches: %d",mincount);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvminlab2,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Set max node and count widgets.  */
   sprintf(str,"Node: %d",maxid);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvmaxlab1,XmNlabelString,string,NULL);
   XFlush(dpy);
 
   sprintf(str,"Matches: %d",maxcount);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvmaxlab2,XmNlabelString,string,NULL);
   XmStringFree(string);

   free(value);

}


void cfvgetminmax()
{
  int i, mincount, maxcount, minid, maxid;
  double *value, fmin, fmax;
  char str[20];
  XmString string;
  double * readcellvalue(int ifld);

   /*  Get values for the selected field.  */
   value = readcellvalue(querycfield);

   /*  Loop through cells to find node with min & max values.  */
   mincount = 0;   maxcount = 0;
   minid = 0;   maxid = 0;
   fmin = cfieldmin[querycfield];
   fmax = cfieldmax[querycfield];
   for (i = 0; i < ncells; i++)
     {
      if (value[i] == fmin)
        {
         mincount++;
         if (mincount == 1) minid = i+1;
        }
      else if (value[i] == fmax)
        {
         maxcount++;
         if (maxcount == 1) maxid = i+1;
        }
     }

   /*  Set min cell and count widgets.  */
   sprintf(str,"Cell: %d",minid);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvminlab1,XmNlabelString,string,NULL);
   XFlush(dpy);
 
   sprintf(str,"Matches: %d",mincount);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvminlab2,XmNlabelString,string,NULL);
   XmStringFree(string);

   /*  Set max node and count widgets.  */
   sprintf(str,"Cell: %d",maxid);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvmaxlab1,XmNlabelString,string,NULL);
   XFlush(dpy);
 
   sprintf(str,"Matches: %d",maxcount);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvmaxlab2,XmNlabelString,string,NULL);
   XmStringFree(string);

   free(value);

}


void querygetnfield(int ifld)
{
  char str[50];
  XmString string;

   querynfield = ifld;
   sprintf(str,"Field: %s",fieldname[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvnodelab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"Min: %10.7g",fieldmin[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max: %10.7g",fieldmax[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(nfvmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   nfvgetminmax();
}


void querygetcfield(int ifld)
{
  char str[50];
  XmString string;

   querycfield = ifld;
   sprintf(str,"Field: %s",cfieldname[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvcelllab,XmNlabelString,string,NULL);
   XmStringFree(string);

   sprintf(str,"Min: %10.7g",cfieldmin[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvminlab,XmNlabelString,string,NULL);
   XmStringFree(string);
   sprintf(str,"Max: %10.7g",cfieldmax[ifld]);
   string = XmStringCreate(str,XmSTRING_DEFAULT_CHARSET);
   XtVaSetValues(cfvmaxlab,XmNlabelString,string,NULL);
   XmStringFree(string);

   cfvgetminmax();
}


#define EPSILONC 1.0e-16

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**ptinface
**
**Determines whether a point is INSIDE, ON, or OUTSIDE a polygon
**
**input variables:
**      xip,yip,zip - vertices of the point in question
**      nv - no. of vertices in the polygon.
**      xyzs - structure containing the vertex coordinates.
**      A, B, C - face unit normal.
**returned value:  INSIDE, ON, or OUTSIDE
** vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
*/

int ptinface(double xip, double yip, double zip, int nv, struct node *xyzs,
              double A, double B, double C)

/*  Takes a polygon with unit normal A,B,C and determines if a   */
/*  point xip,yip,zip is INSIDE, OUTSIDE, or ON the triangle.    */
/*  ON means within EPSILONC distance of the boundary.  Problem  */
/*  is reduced to 2-D by projecting the triangle and point onto  */
/*  a plane XY, YZ, or XZ, that is determined by the largest     */
/*  coordinate of the unit normal.  This plane is the one that   */
/*  the data best projects onto.                                 */

/* input variables:                                              */
/*      xip,yip,zip - vertices of the point in question          */
/*      nv - no. of vertices in the polygon                      */
/*      verts - array of vertices in the polygon.                */
/*      A, B, C - face unit normal.                              */
/* returned value:  INSIDE, ON, or OUTSIDE                       */

{
  short crossed[10000];
  int right, left, dir, inside;
  int i, k, l;
  double x1,y1,z1,  x2,y2,z2;
  double dist2, xtest, fac;

   /*  Determine the 2-D projection plane.  */   
   right = left = dir = 0;
   for (i = 0; i < nv; i++) crossed[i] = 0;
   if (fabs(B) > fabs(A)) dir = 1;
   if ((fabs(C) > fabs(B)) && (fabs(C) > fabs(A))) dir = 2;

   /*  Copy the appropriate coordinates for this plane over to "x-y"  */
   /*  for convenience.  See if the point is on any of the edges.     */
   /*  For this to be so, xp must lie between the endpoints and the   */ 
   /*  distance from the point to the edge must be < EPSILONC;        */
   /*  dist2 (= d^2) must be < EPSILONC^2.  Save some info in         */ 
   /*  crossed[] for possible later use.                              */
   switch (dir) 
     {
      case 0:  /*  Z-Y plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if ((z1 <= zip && zip <= z2) ||
                (z2 <= zip && zip <= z1))
              {
               crossed[i] = 1;
               fac = (zip - z1) * (y2 - y1) -
                     (yip - y1) * (z2 - z1);
               dist2 = (fac * fac) /
                       ((y2 - y1) * (y2 - y1) +
                        (z2 - z1) * (z2 - z1));

               /*  Graphics Gems II, p.10  */
               if (dist2 < EPSILONC) 
                 {
                  if ((y1 <= yip && yip <= y2) ||
                      (y2 <= yip && yip <= y1))
                    {
                     return(ON);
                    } 
                  else if (z1 == z2)
                     return(OUTSIDE);
                 }
              }
           }
         break;

      case 1:  /*  X-Z plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if ((x1 <= xip && xip <= x2) ||
                (x2 <= xip && xip <= x1))
              {
               crossed[i] = 1;
               fac = (xip - x1) * (z2 - z1) -
                     (zip - z1) * (x2 - x1);
               dist2 = (fac * fac ) /
                       ((z2 - z1) * (z2 - z1) +
                        (x2 - x1) * (x2 - x1) );

               /*  Graphics Gems II, p.10  */
               if (dist2 < EPSILONC) 
                 {
                  if ((z1 <= zip && zip <= z2) ||
                      (z2 <= zip && zip <= z1))
                    {
                     return(ON);
                    }
                  else if ( x1 == x2 )
                    return(OUTSIDE);
                 }
              }
           }
         break;

      case 2:  /*  Y-X plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if ((y1 <= yip && yip <= y2) ||
                (y2 <= yip && yip <= y1))
              {
               crossed[i] = 1;
               fac = (yip - y1) * (x2 - x1) -
                     (xip - x1) * (y2 - y1);
               dist2 = (fac * fac)/
                       ((x2 - x1) * (x2 - x1) +
                        (y2 - y1) * (y2 - y1));

               /*  Graphics Gems II, p.10  */
               if (dist2 < EPSILONC) 
                 {
                  if ((x1 <= xip && xip <= x2) ||
                      (x2 <= xip && xip <= x1))
                    {
                     return(ON);
                    } 
                  else if ( y1 == y2 )
                     return(OUTSIDE);
                 }
              }
           }
         break;
     }

   /*  If we get this far, the point is not ON the triangle boundary.     */
   /*  Check for INSIDE or OUTSIDE by finding the x intersection with     */
   /*  each edge for which crossed[] = 1.  Set right or left depending    */
   /*  on whether the x value at the intersection is to the right or      */
   /*  left of xp.  After testing the edges, decide on INSIDE or OUTSIDE  */
   /*  by looking at right and left.                                      */
   switch (dir)
     {

      case 0:  /*  Z-Y plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if (crossed[i] == 1) 
              {
               xtest = y1 + (zip - z1) * (y2 - y1) / (z2 - z1);
               if (xtest > yip ) right = 1;
               else left = 1;
              }
           }
         break;

      case 1:  /*  X-Z plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if (crossed[i] == 1) 
              {
               xtest = z1 + (xip - x1) * (z2 - z1) / (x2 - x1);
               if (xtest > zip ) right = 1;
               else left = 1;
              }
           }
         break;

      case 2:  /*  Y-X plane  */
         for (i = 0; i < nv; i++) 
           {
            k = i;
            if (i < nv - 1)
               l = i + 1;
            else
               l = 0;
            x1 = xyzs[k].x;
            y1 = xyzs[k].y;
            z1 = xyzs[k].z;
            x2 = xyzs[l].x;
            y2 = xyzs[l].y;
            z2 = xyzs[l].z;
            if (crossed[i] == 1) 
              {
               xtest = x1 + (yip - y1) * (x2 - x1) / (y2 - y1);
               if (xtest > xip ) right = 1;
               else left = 1;
              }
           }
         break;
        }


   /*  If there were no intersections to either the right or the  */
   /*  left, the point is OUTSIDE; else the point is INSIDE.      */
   if (right == 0 || left == 0) inside = OUTSIDE;
   else inside = INSIDE;
   return(inside);
}


char *mpnew(int nBytes)
{
  int      fd;
  int      rc;

   /*  Open a new shared memory map file.  */

   membytes = nBytes;

   /*  mmap file exists, remove it.  */
   if (access(MAPFILE,0) == 0)
     {
      rc = remove(MAPFILE);
      if(rc == -1) return (char*)NULL;
     }

   /*  Create a mmap file . */
   fd = open(MAPFILE,O_RDWR|O_CREAT,S_IREAD|S_IWRITE); 
   if (fd < 0)
     {
      perror("Open mmap file:");
      return (char*)NULL;
     }
   chmod(MAPFILE,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

   /*  Set mmap file size.  */
   rc = lseek(fd,membytes-1,SEEK_SET);
   write(fd,"",1);
   if (rc == -1)
     {
      return (char*)NULL;
     }

   /*  Set the buf map space.  */
   mapData = (char *)mmap(0,membytes,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
   if (mapData == 0 )
     {
      perror("*** mmap failed");
      return (char*)NULL;
     }

   return mapData;
}


char *mpold(void)
{
  int fd;
  struct stat statBuf;

   /*  Open an existing shared memory map file.  */

   if (stat(MAPFILE,&statBuf) == -1)
     {
      return (char*)NULL;
     }
   membytes=statBuf.st_size;

   /*  Open the mmap file.  */
   fd = open(MAPFILE,O_RDWR); 
   if (fd < 0)
     {
      return (char*)NULL;
     }

   /*  Set the buf map space.  */
   mapData=(char *)mmap(0,membytes,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
   if (mapData <= (char *) 0) return (char*)NULL;

   return mapData;
}
