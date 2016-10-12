#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>

#include "gmvdata.h"
#include "formwidgets.h"

#include "cdm/include/cdmGenPolyhedralMesh_c.h"
#include "cdm/include/cdmGenPolyhedralMeshField_c.h"

void writesdm(char *filename)
{
  cdmObjectHandle ds, mesh, meshref;
  cdmObjectHandle PtVec3C, PtScalarT, ZoneScalarT, FldHandle;
  cdm_int64 status;

  short flag;
  long i, j, nvect;
  char str2[] = "Error - cannot open file: ";
  XmString string, string1, string2;
  double *temparray, *tempvect1, *tempvect2, *tempvect3;


   /*  Create the dataset that contains the field data. */
   printf("\nStoring Field data in CDM file \"%s\".\n", filename);
   ds = cdmDataset_Create(cdmHDF5, cdmCreate, filename);

   if (ds < 0)
     {
      errorfile = 0;
      string1 = XmStringLtoRCreate(str2,
                                  XmSTRING_DEFAULT_CHARSET);
      string2 = XmStringLtoRCreate(filnam,
                                  XmSTRING_DEFAULT_CHARSET);
      string = XmStringConcat(string1,string2);
      XtVaSetValues(objerrlab,XmNlabelString,string,NULL);
      XmStringFree(string);
      XmStringFree(string1);
      XmStringFree(string2);
      MyManageChild(errorform);
      return;
     }

   mesh = cdmGenPolyhedralMesh_Create(ds, 3, 3, nnodes, "XCMUPoly3DMesh");
   if (mesh < 0)
     {
      printf("Cannot create mesh.\n");
      cdmDataset_Delete(ds);
      return;
     }

   status = cdmGenPolyhedralMesh_Commit(mesh);

   /*  Write velocities.  */
   flag = -1;
   if (nvectin > -1) flag = 1;
   else if (cvectin > -1) flag = 0;
   if (flag > -1)
     {

      /*  Define velocity field template.  */
      PtVec3C = cdmGenPolyhedralMeshFieldTemplate_Create(ds, mesh, 3, 
                    "ThreeCompPtVector");
      if (PtVec3C < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefCellCategory(PtVec3C, 1, 
                    cdm0Cell);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefDataType(PtVec3C, 1, 
                    cdmFloat64);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefCellCategory(PtVec3C, 2, 
                  cdm0Cell);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
        }

      status = cdmGenPolyhedralMeshFieldTemplate_DefDataType(PtVec3C, 2, 
                  cdmFloat64);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefCellCategory(PtVec3C, 3, 
                  cdm0Cell);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefDataType(PtVec3C, 3, 
                  cdmFloat64);
      if (status < 0)
        {
	 printf("Cannot create ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_Commit(PtVec3C);
      if (status < 0)
        {
	 printf("Cannot commit ThreeCompPtVector template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      /*  Fill vector arrays.  */
      if (flag == 1)
        {
         nvect = nnodes;
         tempvect1 = (double *)malloc((nnodes+1)*sizeof(double));
         tempvect2 = (double *)malloc((nnodes+1)*sizeof(double));
         tempvect3 = (double *)malloc((nnodes+1)*sizeof(double));
         fillnodevalue(vectfldx,tempvect1);
         fillnodevalue(vectfldy,tempvect2);
         fillnodevalue(vectfldz,tempvect3);
        }
      else
        {
         nvect = ncells;
         tempvect1 = (double *)malloc((ncells+1)*sizeof(double));
         tempvect2 = (double *)malloc((ncells+1)*sizeof(double));
         tempvect3 = (double *)malloc((ncells+1)*sizeof(double));
         fillcellvalue(cvectfldx,tempvect1+1);
         fillcellvalue(cvectfldy,tempvect2+1);
         fillcellvalue(cvectfldz,tempvect3+1);
        }

      /*  Store vector data.  */
      FldHandle = cdmGenPolyhedralMeshField_Create(ds, PtVec3C, "Velocity");
      if (FldHandle < 0)
	{
	 printf("Cannot create Velocity field.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      printf("\nStoring first velocity component with %ld values.\n",nvect);
      status = cdmGenPolyhedralMeshField_DefFieldVals(FldHandle,1,1,nvect,1, 
                  tempvect1+1);
      if (status < 0)
        {
	 printf("Cannot create Velocity field.\n");
	 cdmGenPolyhedralMeshField_Delete(FldHandle);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      printf("\nStoring second velocity component with %ld values.\n",nvect);
      status = cdmGenPolyhedralMeshField_DefFieldVals(FldHandle,1,1,nvect,1, 
                  tempvect2+1);
      if (status < 0)
        {
	 printf("Cannot create Velocity field.\n");
	 cdmGenPolyhedralMeshField_Delete(FldHandle);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      printf("\nStoring third velocity component with %ld values.\n",nvect);
      status = cdmGenPolyhedralMeshField_DefFieldVals(FldHandle,1,1,nvect,1, 
                  tempvect3+1);
      if (status < 0)
        {
	 printf("Cannot create Velocity field.\n");
	 cdmGenPolyhedralMeshField_Delete(FldHandle);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      free(tempvect1);  free(tempvect2);  free(tempvect3);

      status = cdmGenPolyhedralMeshField_Commit(FldHandle);
      if (status < 0)
        {
	 printf("Cannot create Velocity field.\n");
	 cdmGenPolyhedralMeshField_Delete(FldHandle);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
        }

      cdmGenPolyhedralMeshField_Delete(FldHandle);
      cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
     }


   /*  Define point scalar field template, if field data exists.  */
   flag = 0; 
   for (i = 0; i < MAXFIELDS; i++) flag += nvars_to_write[i];
   if (flag > 0) 
     {
      PtScalarT = cdmGenPolyhedralMeshFieldTemplate_Create(ds, mesh, 1, 
                     "PointScalarT");
      if (PtScalarT < 0)
        {
	 printf("Cannot create PointScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefCellCategory(PtScalarT, 1, 
                  cdm0Cell);
      if (status < 0)
        {
	 printf("Cannot create PointScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefDataType(PtScalarT, 1, 
                  cdmFloat64);
      if (status < 0)
        {
	 printf("Cannot create PointScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_Commit(PtScalarT);
      if (status < 0)
        {
	 printf("Cannot commit PointScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      /*  Loop through node fields and store data.  */
      temparray = (double *)malloc((nnodes+1)*sizeof(double));
      for (i = 0; i < MAXFIELDS; i++)
        {
         if (nvars_to_write[i])
           {
	    FldHandle = cdmGenPolyhedralMeshField_Create(ds, PtScalarT, 
                           fieldname[i]);
	    if (FldHandle < 0)
              {
	       printf("Cannot create point scalar field \"%s\".\n", 
                      fieldname[i]);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }

            fillnodevalue(i,temparray);
            printf("\nStoring point component %s with %ld values.\n", 
                   fieldname[i], nnodes);
	    status = cdmGenPolyhedralMeshField_DefFieldVals(FldHandle, 1, 1, 
                        nnodes, 1, temparray+1);
	    if (status < 0)
              {
	       printf("Cannot create point scalar field \"%s\".\n",
                      fieldname[i]);
	       cdmGenPolyhedralMeshField_Delete(FldHandle);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }

            status = cdmGenPolyhedralMeshField_Commit(FldHandle);
	    if (status < 0)
	      {
	       printf("Cannot create point scalar field \"%s\".\n", 
                      fieldname[i]);
	       cdmGenPolyhedralMeshField_Delete(FldHandle);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }
	    cdmGenPolyhedralMeshField_Delete(FldHandle);
           }
        }
      cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
      free(temparray);
     }

   /*  Define cell scalar field template, if field data exists.  */
   flag = 0; 
   for (i = 0; i < MAXFIELDS; i++) flag += cvars_to_write[i];
   if (flag > 0) 
     {
      ZoneScalarT = cdmGenPolyhedralMeshFieldTemplate_Create(ds, mesh, 1, 
                       "ZoneScalarT");
      if (ZoneScalarT < 0)
        {
	 printf("Cannot create ZoneScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefCellCategory(ZoneScalarT, 1,
                  cdm0Cell);
      if (status < 0)
        {
	 printf("Cannot create ZoneScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_DefDataType(ZoneScalarT, 1, 
                  cdmFloat64);
      if (status < 0)
        {
	 printf("Cannot create ZoneScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      status = cdmGenPolyhedralMeshFieldTemplate_Commit(ZoneScalarT);
      if (status < 0)
        {
	 printf("Cannot commit ZoneScalarT template.\n");
	 cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	 cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	 cdmGenPolyhedralMesh_Delete(mesh);
	 cdmDataset_Delete(ds);
	 return;
	}

      /*  Loop through cell fields and store data.  */
      temparray = (double *)malloc((ncells)*sizeof(double));
      for (i = 0; i < MAXFIELDS; i++)
        {
         if (cvars_to_write[i])
           {
	    FldHandle = cdmGenPolyhedralMeshField_Create(ds, ZoneScalarT, 
                           cfieldname[i]);
	    if (FldHandle < 0)
	      {
	       printf("Cannot create zone scalar field \"%s\".\n", 
                      cfieldname[i]);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }

            fillcellvalue(i,temparray);
	    printf("\nStoring zone component %s with %ld values.\n", 
                   cfieldname[i], ncells);
	    status = cdmGenPolyhedralMeshField_DefFieldVals(FldHandle, 1, 1, 
                     ncells, 1, temparray);
	    if (status < 0)
	      {
	       printf("Cannot create zone scalar field \"%s\".\n", 
                      cfieldname[i]);
	       cdmGenPolyhedralMeshField_Delete(FldHandle);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }

	    status = cdmGenPolyhedralMeshField_Commit(FldHandle);
	    if (status < 0)
	      {
	       printf("Cannot create zone scalar field \"%s\".\n", 
                      cfieldname[i]);
	       cdmGenPolyhedralMeshField_Delete(FldHandle);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtScalarT);
	       cdmGenPolyhedralMeshFieldTemplate_Delete(PtVec3C);
	       cdmGenPolyhedralMesh_Delete(mesh);
	       cdmDataset_Delete(ds);
	       return;
	      }

	    cdmGenPolyhedralMeshField_Delete(FldHandle);
           }
        }
      free(temparray);
      cdmGenPolyhedralMeshFieldTemplate_Delete(ZoneScalarT);
     }

   /*  Cleanup.  */
   cdmGenPolyhedralMesh_Delete(mesh);
   cdmDataset_Delete(ds);
}
