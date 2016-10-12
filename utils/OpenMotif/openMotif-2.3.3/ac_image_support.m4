AC_DEFUN([AC_IMAGE_SUPPORT],
[
AH_TEMPLATE([JPEG_SUPPORTED], [Use libjpeg library])
AH_TEMPLATE([PNG_SUPPORTED], [Use libpng library])

AC_ARG_ENABLE(jpeg,           [  --enable-jpeg           Enable JPEG support],,[enable_jpeg=yes])
AC_ARG_WITH(libjpeg_includes, [  --with-libjpeg-includes=DIR   Use libjpeg includes in DIR], libjpeg_includes=$withval)
AC_ARG_WITH(libjpeg_lib,      [  --with-libjpeg-lib=DIR        Use libjpeg library in DIR], libjpeg_lib=$withval)
AC_ARG_ENABLE(png,            [  --enable-png            Enable PNG support],,[enable_png=yes])
AC_ARG_WITH(libpng_includes,  [  --with-libpng-includes=DIR    Use libpng includes in DIR], libpng_includes=$withval)
AC_ARG_WITH(libpng_lib,       [  --with-libpng-lib=DIR         Use libpng library in DIR], libpng_lib=$withval)

#
# Check libjpeg configuration
#

if test "$enable_jpeg" = "yes"
then
    if test "$libjpeg_includes"
    then
	    LIBJPEG_CFLAGS="-I$libjpeg_includes"
    fi

    if test "$libjpeg_lib"
    then
    	LIBJPEG_LIBS="-L$libjpeg_lib -ljpeg"
    else
    	LIBJPEG_LIBS="-ljpeg"
    fi

    saved_LIBS="$LIBS"
    LIBS="$LIBS $LIBJPEG_LIBS"
    saved_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $LIBJPEG_CFLAGS"
    AC_CHECK_HEADERS(jpeglib.h, libjpeg_headers_present="yes")
    AC_CHECK_FUNCS(jpeg_start_decompress, libjpeg_present="yes")

    if test "$libjpeg_present" = "yes" -a "$libjpeg_headers_present" = "yes"
    then
        AC_SUBST(LIBS)	
	use_libjpeg=yes
        AC_DEFINE(JPEG_SUPPORTED, 1, We have discovered the libjpeg library)
    else
        CFLAGS="$saved_CFLAGS"
        LIBS="$saved_LIBS"
    fi
fi
AM_CONDITIONAL(OM_LIBJPEG, test "$use_libjpeg" = "yes")

#
# Check libpng configuration
#

if test "$enable_png" = "yes"
then
    if test "$libpng_includes"
    then
	    LIBPNG_CFLAGS="-I$libpng_includes"
    fi

    if test "$libpng_lib"
    then
    	LIBPNG_LIBS="-L$libpng_lib -lpng"
    else
    	LIBPNG_LIBS="-lpng"
    fi

    saved_LIBS="$LIBS"
    LIBS="$LIBS $LIBPNG_LIBS"
    saved_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $LIBPNG_CFLAGS"
    AC_CHECK_HEADERS(png.h, libpng_headers_present="yes")
    AC_CHECK_FUNCS(png_create_read_struct, libpng_present="yes")

    if test "$libpng_present" = "yes" -a "$libpng_headers_present" = "yes"
    then
        AC_SUBST(LIBS)	
	use_libpng=yes
        AC_DEFINE(PNG_SUPPORTED, 1, We have discovered the libpng library)
    else
	CFLAGS="$saved_CFLAGS"
	LIBS="$saved_LIBS"
    fi
fi
AM_CONDITIONAL(OM_LIBPNG, test "$use_libpng" = "yes")

])
