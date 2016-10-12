! -*- mode: fortran -*-
!
! Copyright 1994 Sandia Corporation. Under the terms of Contract
! DE-AC04-94AL85000 with Sandia Corporation, the U.S. Governement
! retains certain rights in this software.
!
! See the COPYRIGHT file for copying and redistribution conditions.
!
!***************************************************************
!
! Name: exodusII.inc - Exodus II Application include file 
!
!       Use this include file to define general Exodus II parameters
!
! author - Victor R. Yarberry, Sandia National Laboratories
!
! environment - UNIX F77
!
! revision history - 
!
! History:
!  02/25/93     V.R. Yarberry, Original derived from exodus.inc
!
!  $Id: exodusII.inc,v 1.4 2006/11/28 14:02:19 gdsjaar Exp $
!
!*****************************************************************************
!
!       Define Exodus functions that return values
!
        integer excre,exopen,exlgmd
        external excre, exopen, exlgmd
!
!       Define access modes
!
        integer EXNOCL,EXCLOB,EXREAD,EXWRIT,EXWRT,EXNORM,EXLARG
        integer EXNET4, EXNOSH, EXSHAR
!       EXNOCL - File create with no overwrite (no clobber), default
        parameter (EXNOCL = 0)
!       EXCLOB - File create with overwrite (clobber)
        parameter (EXCLOB = 1)
!       EXNORM - Disable large model modifications
        parameter (EXNORM = 2)
!       EXLARG - Large model modifications
        parameter (EXLARG = 4)
!       EXNET4 - use the hdf5-based netcdf4 output
        parameter (EXNET4 = 8)
!       EXNOSH - Do not open netcdf file in "share" mode
        parameter (EXNOSH = 16)
!       EXSHAR - Do open netcdf file in "share" mode
        parameter (EXSHAR = 32)
!       EXREAD - File open for read
        parameter (EXREAD = 0)
!       EXWRIT, EXWRT  - File open for write
        parameter (EXWRIT = 1, EXWRT = 1)
!
!  --INQUIRE FLAGS
!  --
!  --   EXTYPE - EXODUS II file type is desired.
!  --   EXVERS - EXODUS API version number (in file) is desired
!  --   EXDBVR - EXODUS Database version number is desired
!  --   EXLBVR - EXODUS API Library version number is desired
!  --   EXTITL - Netcdf file title is desired
!  --   EXDIM  - Dimensionality (the number of coordinates per node)
!  --            is desired.
!  --   EXNODE - The number of nodal points is desired.
!  --   EXELEM - The number of elements is desired.
!  --   EXELBL - The number of element blocks is desired.
!  --   EXNODS - The number of node sets is desired.
!  --   EXSSNL - The length of the concatenated node list for all the
!  --            node sets.
!  --   EXSIDS - The number of side sets is desired.
!  --   EXSSEL - The length of the concatenated element list for all
!  --            the side sets.
!  --   EXNSNL - The length of the concatenated node list for all the
!  --            side sets.
!  --   EXQA   - The number of QA records is desired.
!  --   EXINFO - The number of information records is desired.
!  --   EXTIMS - The number of time steps is desired.
!  --   EXNEBP - The number of element block properties
!  --   EXNNSP - The number of node set properties
!  --   EXNSSP - The number of side set properties
!  --   EXNSDF - The length of the node set dist factors array
!  --   EXSSDF - The length of the side set dist factors array
!  --   EXNEMP - The number of element map properties
!  --   EXNNMP - The number of node map properties
!  --   EXNEM  - The number of element maps
!  --   EXNNM  - The number of node maps
!  --
      integer EXTYPE, EXVERS, EXDBVR, EXTITL, EXDIM, EXNODE,            &
     &        EXELEM, EXELBL, EXNODS, EXSIDS, EXQA,  EXINFO,            &
     &        EXTIMS, EXSSNL, EXSSEL, EXNSNL, EXNEBP, EXNNSP,           &
     &        EXNSSP, EXNSDF, EXSSDF, EXLBVR, EXNEMP, EXNNMP,           &
     &        EXNEM,  EXNNM
!
      parameter (EXTYPE = 1)
      parameter (EXVERS = 2)
      parameter (EXDBVR = 3)
      parameter (EXTITL = 4)
      parameter (EXDIM  = 5)
      parameter (EXNODE = 6)
      parameter (EXELEM = 7)
      parameter (EXELBL = 8)
      parameter (EXNODS = 9)
      parameter (EXNSNL = 10)
      parameter (EXSIDS = 11)
      parameter (EXSSNL = 12)
      parameter (EXSSEL = 13)
      parameter (EXQA   = 14)
      parameter (EXINFO = 15)
      parameter (EXTIMS = 16)
      parameter (EXNEBP = 17)
      parameter (EXNNSP = 18)
      parameter (EXNSSP = 19)
      parameter (EXNSDF = 20)
      parameter (EXSSDF = 21)
      parameter (EXLBVR = 22)
      parameter (EXNEMP = 23)
      parameter (EXNNMP = 24)
      parameter (EXNEM  = 25)
      parameter (EXNNM  = 26)
!
!         Define limits
!
        integer MXSTLN, MXLNLN
!       MXSTLN - Maximum string length: used for store strings
        parameter (MXSTLN = 32)
!       MXLNLN - Maximum line string length
        parameter (MXLNLN = 80)
!
!       properties
!
        integer EXEBLK, EXNSET, EXSSET, EXEMAP, EXNMAP
!       EXEBLK - element block property code
        parameter (EXEBLK = 1)
!       EXNSPR - node set property code
        parameter (EXNSET = 2)
!       EXSSPR - side set property code
        parameter (EXSSET = 3)
!       EXEMAP - element map property code
        parameter (EXEMAP = 4)
!       EXNMAP - node map property code
        parameter (EXNMAP = 5)
!
!       Define options codes
!
        integer EXVRBS, EXDEBG, EXABRT
!       EXVRBS - verbose mode message flag
        parameter (EXVRBS = 1)
!       EXDEBG - debug mode message flag
        parameter (EXDEBG = 2)
!       EXABRT - abort mode flag
        parameter (EXABRT = 4)
!
!       Define error return codes
!
        integer EXFATL, EXOK, EXWARN
!       EXFATL - fatal error flag def
        parameter (EXFATL = -1)
!       EXOK - no err (success) flag def
        parameter (EXOK = 0)
!       EXWARN - warning flag def
        parameter (EXWARN = 1)
!
!       Define error return codes and flags
!
        integer EXMEMF, EXBFMD, EXBFID, EXBTID, EXBPRM, EXNULL,         &
     &          EXPMSG, EXLMSG
!       EXMEMF - memory allocation failure flag def
        parameter (EXMEMF = 1000)
!       EXBFMD - wrong file mode
        parameter (EXBFMD = 1001)
!       EXBFID - bad file id def
        parameter (EXBFID = 1002)
!       EXBTID - id table lookup failed
        parameter (EXBTID = 1004)
!       EXBPRM - bad parameter passed
        parameter (EXBPRM  =  1005)
!       EXNULL - null entity found
        parameter (EXNULL = -1006)
!       EXPMSG - message print code - no error implied
        parameter (EXPMSG = -1000)
!       EXLMSG - print last error message msg code
        parameter (EXLMSG = -1001)
