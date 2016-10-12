AC_DEFUN([AC_FIND_XFT],
[
AH_TEMPLATE([HAVE_FREETYPE], [We have the FreeType library])
AH_TEMPLATE([HAVE_FONTCONFIG], [We have the fontconfig library])
AH_TEMPLATE([HAVE_XRENDER], [We have the fontconfig library])
AH_TEMPLATE([FC_DEFAULT_FONTS], [We have the fontconfig library])
AH_TEMPLATE([CONFDIR], [We have the fontconfig library])
AH_TEMPLATE([USE_XFT], [Use XFT])

AC_ARG_ENABLE(xft,             [  --enable-xft            Enable XFT support (font anti aliasing)],,[enable_xft=yes])
AC_ARG_WITH(freetype_includes, [  --with-freetype-includes=DIR  Use FreeType includes in DIR], freetype_includes=$withval, freetype_includes=yes)
AC_ARG_WITH(freetype_lib,      [  --with-freetype-lib=DIR       Use FreeType library in DIR], freetype_lib=$withval, freetype_lib=yes)
AC_ARG_WITH(freetype_config,   [  --with-freetype-config=PROG   Use FreeType configuration program PROG], freetype_config=$withval, freetype_config=yes)
AC_ARG_WITH(default_fonts,     [  --with-default-fonts=DIR      Use fonts from DIR when config is busted], defaultfonts="$withval", default_fonts=yes)
AC_ARG_WITH(fontconfig_includes, [  --with-fontconfig-includes=DIR  Use Fontconfig includes in DIR], fontconfig_includes=$withval, fontconfig_includes=yes)
AC_ARG_WITH(fontconfig_lib,      [  --with-fontconfig-lib=DIR       Use Fontconfig library in DIR], fontconfig_lib=$withval, fontconfig_lib=yes)
AC_ARG_WITH(fontconfig_config,   [  --with-fontconfig-config=PROG  Use Fontconfig configuration program PROG], fontconfig_config=$withval, fontconfig_config=yes)

if test "$enable_xft" = "yes"
then

#
# Check freetype configuration
#
case "$freetype_config" in
no)
	;;
yes)
	AC_CHECK_PROG(ft_config,freetype-config,freetype-config,no)
	;;
*)
	ft_config="$freetype_config"
	;;
esac

case "$freetype_includes" in
no)
	FREETYPE_CFLAGS=""
	;;
yes)
	case "$ft_config" in
	no)
		FREETYPE_CFLAGS=""
		;;
	*)
		FREETYPE_CFLAGS="`$ft_config --cflags`"
		;;
	esac
	;;
*)
	FREETYPE_CFLAGS="-I$freetype_includes"
	;;
esac

case "$freetype_lib" in
no)
	freetype_lib=""
	;;
yes)
	case "$ft_config" in
	no)
		freetype_lib=""
		;;
	*)
		freetype_lib="`$ft_config --libs`"
		;;
	esac
	;;
*)
	freetype_lib="-L$freetype_lib -lfreetype"
	;;
esac

saved_LIBS="$LIBS"
LIBS="$LIBS $freetype_lib"
saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $FREETYPE_CFLAGS"
AC_CHECK_HEADERS(freetype/freetype.h)

FINDXFT_HAVE_FREETYPE="no"
case "$ac_cv_header_freetype_freetype_h" in
no)
	CPPFLAGS="$saved_CPPFLAGS"
	LIBS="$saved_LIBS"
	;;
yes)
	AC_CHECK_FUNCS(FT_Init_FreeType)
	case "$ac_cv_func_FT_Init_FreeType" in
	no)
		CPPFLAGS="$saved_CPPFLAGS"
		LIBS="$saved_LIBS"
		;;
	yes)
		FINDXFT_HAVE_FREETYPE="yes"
		AC_DEFINE(HAVE_FREETYPE, 1, Means we have discovered the FreeType library)
		AC_SUBST(FREETYPE_CFLAGS)
		;;
	esac
	;;
esac

case "$default_fonts" in
yes)
	FC_DEFAULT_FONTS="/usr/share/fonts"
	AC_DEFINE_UNQUOTED(FC_DEFAULT_FONTS, "/usr/share/fonts")
	;;
*)
	FC_DEFAULT_FONTS="$default_fonts"
	AC_DEFINE_UNQUOTED(FC_DEFAULT_FONTS, "$default_fonts")
	;;
esac

AC_SUBST(FC_DEFAULT_FONTS)

#
# Set CONFDIR and FONTCONFIG_PATH
#

case "$confdir" in
no|yes)
	confdir=/etc/fonts
	;;
*)
	;;
esac
AC_SUBST(confdir)
CONFDIR='${confdir}'
AC_DEFINE_UNQUOTED(CONFDIR, "$CONFDIR")
AC_SUBST(CONFDIR)

#
# Check X configuration
#
FINDXFT_HAVE_XRENDER="no"
case "$have_x" in
yes)
	XRENDER_CFLAGS="-I$x_includes"
	XRENDER_LIBS="-L$x_libraries -lXft -lXrender"

	saved_LIBS="$LIBS"
	LIBS="$LIBS $XRENDER_LIBS"
	saved_CPPFLAGS="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS $XRENDER_CFLAGS"
	AC_CHECK_HEADERS(X11/extensions/Xrender.h)

	case "$ac_cv_header_X11_extensions_Xrender_h" in
	no)
		CPPFLAGS="$saved_CPPFLAGS"
		LIBS="$saved_LIBS"
		;;
	yes)
		AC_CHECK_FUNCS(XRenderParseColor)
		case "$ac_cv_func_XRenderParseColor" in
		no)
			CPPFLAGS="$saved_CPPFLAGS"
			LIBS="$saved_LIBS"
			;;
		yes)
			FINDXFT_HAVE_XRENDER="yes"
			AC_DEFINE(HAVE_XRENDER, 1, Means we have discovered the Xrender library)
			AC_SUBST(XRENDER_CFLAGS)
			AC_SUBST(XRENDER_LIBS)
			;;
		esac
		;;
	esac

	;;
esac

#
# Check fontconfig configuration
#
case "$fontconfig_config" in
no)
	;;
yes)
	AC_CHECK_PROG(fc_config,fontconfig-config,fontconfig-config,no)
	;;
*)
	fc_config="$fontconfig_config"
	;;
esac

case "$fontconfig_includes" in
no)
	FONTCONFIG_CFLAGS=""
	;;
yes)
	case "$fc_config" in
	no)
		FONTCONFIG_CFLAGS=""
		;;
	*)
		FONTCONFIG_CFLAGS="`$fc_config --cflags`"
		;;
	esac
	;;
*)
	FONTCONFIG_CFLAGS="-I$fontconfig_includes"
	;;
esac

case "$fontconfig_lib" in
no)
	fontconfig_lib=""
	;;
yes)
	case "$fc_config" in
	no)
		fontconfig_lib=""
		;;
	*)
		FONTCONFIG_LIBS="`$fc_config --libs`"
		;;
	esac
	;;
*)
	FONTCONFIG_LIBS="-L$fontconfig_lib -lfontconfig"
	;;
esac

saved_LIBS="$LIBS"
LIBS="$LIBS $FONTCONFIG_LIBS"
saved_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $FONTCONFIG_CFLAGS"
AC_CHECK_HEADERS(fontconfig/fontconfig.h)

case "$ac_cv_header_fontconfig_fontconfig_h" in
no)
	CPPFLAGS="$saved_CPPFLAGS"
	LIBS="$saved_LIBS"
	;;
yes)
	AC_CHECK_FUNCS(FcInit)
	case "$ac_cv_func_FcInit" in
	no)
		CPPFLAGS="$saved_CPPFLAGS"
		LIBS="$saved_LIBS"
		;;
	yes)
		AC_DEFINE(HAVE_FONTCONFIG, 1, This is defined if we find the FontConfig library)
		AC_SUBST(FONTCONFIG_CFLAGS)
		AC_SUBST(FONTCONFIG_LIBS)
		;;
	esac
	;;
esac

dnl
dnl Should this be conditionally defined ?
dnl

dnl if test "$FINDXFT_HAVE_XRENDER" = "yes" -a "$FINDXFT_HAVE_FREETYPE" = "yes"
dnl then
 	AC_DEFINE(USE_XFT, 1, This means we will be using the Xft library)
CFLAGS="$CFLAGS `pkg-config xft --cflags`"
AC_SUBST(CFLAGS)
LIBS="$LIBS `pkg-config xft --libs`"
AC_SUBST(LIBS)

dnl fi

fi
])
