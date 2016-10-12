/* main.h */

/* This file is the partner to main.c, the driver for GMV/PHYSED */

/*
   IFDEF sets up the naming and version information for the executable
   depending on whether the compilation flag is set to PHYSED or, 
   by default, to GMV.
*/
#ifdef PHYSED
   char *physed_version   = "@(#)RELEASE VERSION 1.0";
   char *appname          = "PHYSED";
   char *appstring        = "pe";
   char *appinitstring    = "PHYSED initialization.";
   char pfxn[5]="PEN", pfxc[5]="PEC",pfxp[5]="PEP", pfxs[5]="PES",
        pfxf[5]="PEF";
#ifdef MESA
   char *appversionstring = "PHYSED V1.0 with Mesa";
#else
   char *appversionstring = "PHYSED V1.0";
#endif

#else
   char *gmv_version_no   = "4.6.2";
   char *gmv_version      = "@(#)RELEASE VERSION 4.6.2";
   char *appname          = "GMV";
   char *appstring        = "gmv";
   char *appinitstring    = "GMV initialization.";
   char pfxn[5]="GMVN", pfxc[5]="GMVC",pfxp[5]="GMVP", pfxs[5]="GMVS",
        pfxf[5]="GMVF";
#ifdef MESA
   char *appversionstring = "GMV  V4.6.2 with Mesa built 2015-05-19";
#else
   char *appversionstring = "GMV  V4.6.2 built 2015-05-19";
#endif
#endif    

