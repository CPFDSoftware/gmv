INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE( malloc.h EX_HAVE_MALLOC_H )

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/exodusII_cfg.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/exodusII_cfg.h
  @ONLY IMMEDIATE)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../netcdf/netcdf-3.6.1/src/libsrc"
  "${CMAKE_CURRENT_BINARY_DIR}/../../netcdf/netcdf-3.6.1/src/libsrc"
)

SET(cbind_SRCS
  src/exclos.c
  src/excn2s.c
  src/ex_conv.c
  src/excopy.c
  src/excre.c
  src/exerr.c
  src/exgatm.c
  src/exgcns.c
  src/exgcon.c
  src/exgcor.c
  src/exgcset.c
  src/exgcss.c
  src/exgean.c
  src/exgatn.c
  src/exgeat.c
  src/exgatt.c
  src/exgebi.c
  src/exgelb.c
  src/exgblk.c
  src/exgelc.c
  src/exgconn.c
  src/exgem.c
  src/exgenm.c
  src/exgev.c
  src/exgvar.c
  src/exgevid.c
  src/exgevt.c
  src/exgvart.c
  src/exgfrm.c
  src/exggv.c
  src/exggvt.c
  src/exgids.c
  src/exginf.c
  src/exgini.c
  src/exginix.c
  src/exgmap.c
  src/exgmp.c
  src/exgnam.c
  src/exgnams.c
  src/exgnm.c
  src/exgnnm.c
  src/exgnp.c
  src/exgns.c
  src/exgnsd.c
  src/exgnsi.c
  src/exgnstt.c
  src/exgnsv.c
  src/exgnsvid.c
  src/exgnv.c
  src/exgnvid.c
  src/exgnvt.c
  src/exgnvv.c
  src/exgoea.c
  src/exgoatt.c
  src/exgotv.c
  src/exgpa.c
  src/exgp.c
  src/exgpem.c
  src/exgpn.c
  src/exgqa.c
  src/exgsnl.c
  src/exgsetp.c
  src/exgsp.c
  src/exgset.c
  src/exgss.c
  src/exgssc.c
  src/exgsetd.c
  src/exgssd.c
  src/exgssi.c
  src/exgssn.c
  src/exgsstt.c
  src/exgssv.c
  src/exgssvid.c
  src/exgtim.c
  src/exgtt.c
  src/exgvan.c
  src/exgvid.c
  src/exgvnm.c
  src/exgvp.c
  src/exgvtt.c
  src/exgvv.c
  src/exinq.c
  src/exopen.c
  src/exopts.c
  src/expatn.c
  src/expatt.c
  src/expcab.c
  src/expclb.c
  src/expcns.c
  src/expcon.c
  src/expconn.c
  src/expcor.c
  src/expcset.c
  src/expcss.c
  src/expean.c
  src/expeat.c
  src/expelb.c
  src/expblk.c
  src/expelc.c
  src/expem.c
  src/expenm.c
  src/expev.c
  src/expfrm.c
  src/expgv.c
  src/expinf.c
  src/expini.c
  src/expinix.c
  src/expmap.c
  src/expmp.c
  src/expnam.c
  src/expnams.c
  src/expnm.c
  src/expnmap.c
  src/exgnmap.c
  src/expnnm.c
  src/expnp.c
  src/expns.c
  src/expnsd.c
  src/expnstt.c
  src/expnsv.c
  src/expnv.c
  src/expnvv.c
  src/expoea.c
  src/expoatt.c
  src/exppa.c
  src/expp.c
  src/exppem.c
  src/exppn.c
  src/expqa.c
  src/expsetp.c
  src/expset.c
  src/expsetd.c
  src/expsp.c
  src/expss.c
  src/expssd.c
  src/expsstt.c
  src/expssv.c
  src/exptim.c
  src/exptt.c
  src/expvan.c
  src/expvar.c
  src/expvnm.c
  src/expvpa.c
  src/expvpax.c
  src/expvp.c
  src/expvpc.c
  src/expvtt.c
  src/expvv.c
  src/exupda.c
  src/ex_utils.c
)

IF(APPLE)
  SET_SOURCE_FILES_PROPERTIES(
    src/ex_utils.c
    PROPERTIES
    COMPILE_FLAGS -fno-common)
ENDIF(APPLE)

ADD_LIBRARY(exoIIc ${cbind_SRCS})
TARGET_LINK_LIBRARIES(exoIIc NetCDF)

IF(VTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(exoIIc PROPERTIES ${VTK_LIBRARY_PROPERTIES})
ENDIF(VTK_LIBRARY_PROPERTIES)

IF(NOT VTK_INSTALL_NO_LIBRARIES)
  IF(VTK_INSTALL_HAS_CMAKE_24)
    INSTALL(TARGETS exoIIc
      RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT Runtime
      LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT Runtime
      ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib COMPONENT Development)
  ELSE(VTK_INSTALL_HAS_CMAKE_24)
    INSTALL_TARGETS(${CMAKE_INSTALL_PREFIX}/lib exoIIc)
  ENDIF(VTK_INSTALL_HAS_CMAKE_24)
ENDIF(NOT VTK_INSTALL_NO_LIBRARIES)

IF(NOT VTK_INSTALL_NO_DEVELOPMENT)
#  INSTALL_FILES(${VTK_INSTALL_INCLUDE_DIR}/vtkexodus2 .h
ENDIF(NOT VTK_INSTALL_NO_DEVELOPMENT)

INCLUDE (${CMAKE_ROOT}/Modules/Dart.cmake)

IF (BUILD_TESTING)
  ENABLE_TESTING()
  SUBDIRS(test)
ENDIF (BUILD_TESTING)

