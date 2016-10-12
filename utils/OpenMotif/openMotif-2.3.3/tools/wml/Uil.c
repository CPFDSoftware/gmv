/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     UILEOF = 0,
     NAME = 1,
     FONT_NAME = 2,
     COLOR_NAME = 3,
     ARGUMENT_NAME = 4,
     REASON_NAME = 5,
     CHARSET_NAME = 6,
     ENUMVAL_NAME = 7,
     CLASS_NAME = 8,
     CHILD_NAME = 96,
     UNS_FLOAT_LITERAL = 9,
     COMP_STRING = 10,
     CHAR_8_LITERAL = 11,
     UNS_INT_LITERAL = 12,
     LEFT_PAREN = 13,
     RIGHT_PAREN = 14,
     COLON = 15,
     SEMICOLON = 16,
     LEFT_BRACE = 17,
     RIGHT_BRACE = 18,
     COMMA = 19,
     EQUAL_SIGN = 20,
     NOT = 21,
     PLUS = 22,
     MINUS = 23,
     AND = 24,
     OR = 25,
     XOR = 26,
     MULTIPLY = 27,
     DIVIDE = 28,
     LEFT_SHIFT = 29,
     RIGHT_SHIFT = 30,
     LIST = 31,
     IDENTIFIER = 32,
     END = 33,
     MODULE = 34,
     UILTRUE = 35,
     UILFALSE = 36,
     INCLUDE = 37,
     MACRO = 38,
     ON = 39,
     OFF = 40,
     VALUE = 41,
     ARGUMENTS = 42,
     CALLBACKS = 43,
     PROCEDURES = 44,
     CONTROLS = 45,
     PROCEDURE = 46,
     OBJECT = 47,
     OBJECTS = 48,
     WIDGET = 49,
     GADGET = 50,
     FONT = 51,
     ARGUMENT = 52,
     REASON = 53,
     PIXMAP = 54,
     COLOR = 55,
     NAMES = 56,
     CHARACTER_SET = 57,
     CASE_SENSITIVE = 58,
     CASE_INSENSITIVE = 59,
     VERSION = 60,
     MANAGED = 61,
     UNMANAGED = 62,
     PRIVATE = 63,
     IMPORTED = 64,
     EXPORTED = 65,
     UILFILE = 66,
     STRING_TABLE = 67,
     TRANSLATION_TABLE = 68,
     COMPOUND_STRING = 69,
     FONT_TABLE = 70,
     ANY = 71,
     STRING = 72,
     BOOLEAN = 73,
     ICON = 74,
     RIGHT_TO_LEFT = 75,
     BACKGROUND = 76,
     FOREGROUND = 77,
     COLOR_TABLE = 78,
     FLOAT = 79,
     INTEGER = 80,
     CLASS_REC_NAME = 81,
     ASCIZ_TABLE = 82,
     INTEGER_TABLE = 83,
     ASCIZ_STRING_TABLE = 84,
     COMPOUND_STRING_TABLE = 85,
     XBITMAPFILE = 86,
     SEPARATE = 87,
     SIXTEEN_BIT = 88,
     POUND = 89,
     KEYSYM = 90,
     SINGLE_FLOAT = 91,
     RGB = 92,
     WIDE_CHARACTER = 93,
     LOC_STRING = 94,
     FONTSET = 95,
     COMPOUND_STRING_COMPONENT = 97,
     PIX = 115,
     PIXEL = 103,
     PIXELS = 104,
     IN = 98,
     INCH = 105,
     INCHES = 106,
     CM = 99,
     CENTIMETER = 107,
     CENTIMETERS = 108,
     MM = 100,
     MILLIMETER = 109,
     MILLIMETERS = 110,
     PT = 101,
     POINT = 111,
     POINTS = 112,
     FU = 102,
     FONT_UNIT = 113,
     FONT_UNITS = 114
   };
#endif
/* Tokens.  */
#define UILEOF 0
#define NAME 1
#define FONT_NAME 2
#define COLOR_NAME 3
#define ARGUMENT_NAME 4
#define REASON_NAME 5
#define CHARSET_NAME 6
#define ENUMVAL_NAME 7
#define CLASS_NAME 8
#define CHILD_NAME 96
#define UNS_FLOAT_LITERAL 9
#define COMP_STRING 10
#define CHAR_8_LITERAL 11
#define UNS_INT_LITERAL 12
#define LEFT_PAREN 13
#define RIGHT_PAREN 14
#define COLON 15
#define SEMICOLON 16
#define LEFT_BRACE 17
#define RIGHT_BRACE 18
#define COMMA 19
#define EQUAL_SIGN 20
#define NOT 21
#define PLUS 22
#define MINUS 23
#define AND 24
#define OR 25
#define XOR 26
#define MULTIPLY 27
#define DIVIDE 28
#define LEFT_SHIFT 29
#define RIGHT_SHIFT 30
#define LIST 31
#define IDENTIFIER 32
#define END 33
#define MODULE 34
#define UILTRUE 35
#define UILFALSE 36
#define INCLUDE 37
#define MACRO 38
#define ON 39
#define OFF 40
#define VALUE 41
#define ARGUMENTS 42
#define CALLBACKS 43
#define PROCEDURES 44
#define CONTROLS 45
#define PROCEDURE 46
#define OBJECT 47
#define OBJECTS 48
#define WIDGET 49
#define GADGET 50
#define FONT 51
#define ARGUMENT 52
#define REASON 53
#define PIXMAP 54
#define COLOR 55
#define NAMES 56
#define CHARACTER_SET 57
#define CASE_SENSITIVE 58
#define CASE_INSENSITIVE 59
#define VERSION 60
#define MANAGED 61
#define UNMANAGED 62
#define PRIVATE 63
#define IMPORTED 64
#define EXPORTED 65
#define UILFILE 66
#define STRING_TABLE 67
#define TRANSLATION_TABLE 68
#define COMPOUND_STRING 69
#define FONT_TABLE 70
#define ANY 71
#define STRING 72
#define BOOLEAN 73
#define ICON 74
#define RIGHT_TO_LEFT 75
#define BACKGROUND 76
#define FOREGROUND 77
#define COLOR_TABLE 78
#define FLOAT 79
#define INTEGER 80
#define CLASS_REC_NAME 81
#define ASCIZ_TABLE 82
#define INTEGER_TABLE 83
#define ASCIZ_STRING_TABLE 84
#define COMPOUND_STRING_TABLE 85
#define XBITMAPFILE 86
#define SEPARATE 87
#define SIXTEEN_BIT 88
#define POUND 89
#define KEYSYM 90
#define SINGLE_FLOAT 91
#define RGB 92
#define WIDE_CHARACTER 93
#define LOC_STRING 94
#define FONTSET 95
#define COMPOUND_STRING_COMPONENT 97
#define PIX 115
#define PIXEL 103
#define PIXELS 104
#define IN 98
#define INCH 105
#define INCHES 106
#define CM 99
#define CENTIMETER 107
#define CENTIMETERS 108
#define MM 100
#define MILLIMETER 109
#define MILLIMETERS 110
#define PT 101
#define POINT 111
#define POINTS 112
#define FU 102
#define FONT_UNIT 113
#define FONT_UNITS 114




/* Copy the first part of user declarations.  */
#line 55 "Uil.y"



	/* Begin user declarations section */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "UilDefI.h"
#include "UilCompGl.h"

#define		YYSTYPE		yystype

#define		YYDEBUG		1

/*   Declare and initialize stack entry for epsilon productions.    */

YYSTYPE		gz_yynullval = {NULL,0,0,0,0};

/*   Declare token to aid in saving source context	*/

YYSTYPE		prev_yylval;

	/* End user declarations section */



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 368 "Uil.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3848

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  118
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  474
/* YYNRULES -- Number of states.  */
#define YYNSTATES  750

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    11,    99,   103,   106,
     109,   112,   115,   101,   102,   104,   105,   107,   108,   110,
     111,   113,   114,   116,   117,   100,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    11,    12,    13,    19,    20,    22,
      24,    27,    31,    35,    39,    40,    45,    47,    49,    52,
      56,    57,    61,    64,    68,    69,    73,    74,    79,    81,
      83,    84,    87,    89,    91,    93,    95,    97,    99,   100,
     104,   106,   109,   110,   114,   119,   122,   125,   127,   130,
     132,   134,   136,   138,   140,   142,   144,   146,   148,   150,
     152,   154,   156,   158,   160,   162,   164,   166,   168,   170,
     172,   174,   176,   178,   180,   182,   184,   186,   188,   190,
     192,   194,   196,   198,   200,   202,   204,   206,   208,   210,
     212,   214,   216,   218,   220,   222,   224,   226,   228,   229,
     233,   235,   238,   239,   243,   248,   249,   253,   254,   256,
     258,   260,   261,   262,   266,   268,   271,   272,   276,   279,
     284,   285,   289,   290,   294,   297,   301,   302,   306,   307,
     312,   313,   317,   318,   322,   323,   327,   328,   329,   336,
     337,   341,   342,   346,   347,   351,   352,   356,   357,   361,
     362,   363,   369,   370,   376,   378,   380,   381,   387,   389,
     393,   395,   398,   400,   403,   407,   408,   412,   414,   418,
     423,   424,   428,   432,   437,   438,   442,   444,   447,   450,
     453,   456,   457,   461,   462,   463,   467,   470,   474,   475,
     479,   480,   485,   487,   489,   491,   493,   494,   498,   499,
     503,   504,   508,   509,   513,   515,   517,   519,   521,   523,
     526,   530,   531,   535,   537,   540,   544,   545,   549,   551,
     554,   558,   559,   563,   565,   568,   572,   573,   577,   579,
     582,   584,   587,   589,   592,   594,   597,   600,   603,   604,
     608,   611,   614,   615,   619,   622,   625,   626,   630,   633,
     636,   637,   641,   642,   646,   647,   651,   652,   656,   657,
     661,   664,   665,   670,   671,   676,   677,   682,   684,   686,
     690,   695,   699,   703,   704,   708,   710,   712,   714,   718,
     721,   723,   727,   732,   735,   737,   739,   741,   743,   745,
     747,   749,   751,   753,   755,   757,   759,   761,   763,   765,
     767,   769,   771,   773,   775,   777,   779,   781,   783,   785,
     787,   789,   791,   793,   795,   797,   799,   801,   803,   805,
     807,   809,   811,   813,   815,   817,   819,   821,   823,   825,
     827,   829,   831,   833,   835,   837,   839,   841,   846,   848,
     852,   856,   858,   862,   864,   868,   872,   874,   878,   882,
     884,   888,   892,   894,   897,   900,   903,   905,   909,   911,
     914,   917,   919,   921,   924,   929,   934,   939,   944,   949,
     954,   960,   962,   964,   966,   968,   971,   976,   981,   986,
     991,   996,  1001,  1006,  1011,  1016,  1021,  1026,  1031,  1036,
    1041,  1046,  1048,  1049,  1051,  1053,  1057,  1058,  1060,  1062,
    1066,  1067,  1069,  1071,  1075,  1081,  1082,  1084,  1086,  1090,
    1091,  1094,  1097,  1099,  1102,  1104,  1108,  1112,  1116,  1120,
    1123,  1127,  1131,  1135,  1139,  1141,  1147,  1149,  1155,  1157,
    1159,  1163,  1167,  1169,  1172,  1174,  1176,  1181,  1183,  1185,
    1188,  1191,  1195,  1199,  1203,  1205,  1209,  1214,  1219,  1223,
    1225,  1229,  1231,  1233,  1235,  1241,  1243,  1247,  1249,  1251,
    1253,  1255,  1257,  1259,  1261,  1263,  1265,  1267,  1269,  1271,
    1273,  1275,  1277,  1279,  1281
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     119,     0,    -1,    -1,   121,   120,   136,    36,    37,    19,
      -1,    -1,    -1,   122,    37,   260,   123,   124,    -1,    -1,
     125,    -1,   126,    -1,   125,   126,    -1,    63,    23,   263,
      -1,    59,    23,   128,    -1,    60,    23,   295,    -1,    -1,
      51,   127,    23,   129,    -1,    61,    -1,    62,    -1,    20,
      21,    -1,    20,   131,    21,    -1,    -1,     1,   130,    21,
      -1,   133,    19,    -1,   131,   133,    19,    -1,    -1,     1,
     132,    19,    -1,    -1,    10,   134,    23,   135,    -1,    52,
      -1,    53,    -1,    -1,   136,   137,    -1,   138,    -1,   156,
      -1,   148,    -1,   163,    -1,   200,    -1,   161,    -1,    -1,
      44,   139,   140,    -1,   142,    -1,   140,   142,    -1,    -1,
       1,   141,    19,    -1,   260,    18,   143,    19,    -1,    68,
     264,    -1,    66,   265,    -1,   265,    -1,    67,   144,    -1,
      83,    -1,    75,    -1,    57,    -1,    82,    -1,    76,    -1,
      54,    -1,    56,    -1,    55,    -1,    58,    -1,   146,    -1,
     147,    -1,    86,    -1,    71,    -1,    73,    -1,    77,    -1,
      72,    -1,    99,    -1,    84,    -1,    89,    -1,    93,    -1,
      94,    -1,    95,    -1,    96,    -1,    98,    -1,    52,    -1,
      83,    -1,    75,    -1,    57,    -1,    82,    -1,    76,    -1,
      54,    -1,    56,    -1,    58,    -1,   146,    -1,   147,    -1,
      86,    -1,    71,    -1,    73,    -1,    77,    -1,    72,    -1,
      93,    -1,    94,    -1,    96,    -1,    98,    -1,    52,    -1,
      70,    -1,    88,    -1,    85,    -1,    87,    -1,    -1,    49,
     149,   150,    -1,   152,    -1,   150,   152,    -1,    -1,     1,
     151,    19,    -1,   260,   153,   155,    19,    -1,    -1,    16,
     154,    17,    -1,    -1,   144,    -1,    74,    -1,    10,    -1,
      -1,    -1,    35,   157,   158,    -1,   160,    -1,   158,   160,
      -1,    -1,     1,   159,    19,    -1,   260,    19,    -1,    40,
      69,   263,    19,    -1,    -1,     1,   162,    19,    -1,    -1,
      50,   164,   165,    -1,   167,    19,    -1,   165,   167,    19,
      -1,    -1,     1,   166,    19,    -1,    -1,   260,   168,    18,
     169,    -1,    -1,    68,   170,   183,    -1,    -1,    66,   171,
     183,    -1,    -1,   199,   172,   183,    -1,    -1,    -1,    67,
     173,    10,   174,   188,   199,    -1,    -1,    68,   176,   183,
      -1,    -1,    66,   177,   183,    -1,    -1,    64,   178,   183,
      -1,    -1,    65,   179,   183,    -1,    -1,   199,   180,   183,
      -1,    -1,    -1,    67,   181,    10,   182,   199,    -1,    -1,
      10,   184,   188,   185,   190,    -1,   199,    -1,   135,    -1,
      -1,    10,   187,   188,   185,   192,    -1,   199,    -1,    49,
     261,   189,    -1,   199,    -1,    16,    17,    -1,   261,    -1,
      20,    21,    -1,    20,   196,    21,    -1,    -1,     1,   191,
      21,    -1,   261,    -1,    20,    21,   199,    -1,    20,   196,
      21,   199,    -1,    -1,     1,   193,    21,    -1,    20,    21,
     199,    -1,    20,   196,    21,   199,    -1,    -1,     1,   195,
      21,    -1,   197,    -1,   196,   197,    -1,   241,    19,    -1,
     243,    19,    -1,   239,    19,    -1,    -1,     1,   198,    19,
      -1,    -1,    -1,    34,   201,   202,    -1,   204,    19,    -1,
     202,   204,    19,    -1,    -1,     1,   203,    19,    -1,    -1,
     260,   205,    18,   206,    -1,   207,    -1,   209,    -1,   211,
      -1,   213,    -1,    -1,   218,   208,   225,    -1,    -1,   215,
     210,   219,    -1,    -1,   216,   212,   221,    -1,    -1,   217,
     214,   223,    -1,    45,    -1,    46,    -1,    47,    -1,    48,
      -1,   261,    -1,    20,    21,    -1,    20,   227,    21,    -1,
      -1,     1,   220,    21,    -1,   261,    -1,    20,    21,    -1,
      20,   228,    21,    -1,    -1,     1,   222,    21,    -1,   261,
      -1,    20,    21,    -1,    20,   229,    21,    -1,    -1,     1,
     224,    21,    -1,   261,    -1,    20,    21,    -1,    20,   230,
      21,    -1,    -1,     1,   226,    21,    -1,   231,    -1,   227,
     231,    -1,   233,    -1,   228,   233,    -1,   235,    -1,   229,
     235,    -1,   237,    -1,   230,   237,    -1,   241,    19,    -1,
     252,    19,    -1,    -1,     1,   232,    19,    -1,   243,    19,
      -1,   253,    19,    -1,    -1,     1,   234,    19,    -1,   245,
      19,    -1,   258,    19,    -1,    -1,     1,   236,    19,    -1,
     239,    19,    -1,   247,    19,    -1,    -1,     1,   238,    19,
      -1,    -1,   199,   240,   207,    -1,    -1,   199,   242,   209,
      -1,    -1,   199,   244,   211,    -1,    -1,   199,   246,   213,
      -1,   256,   254,    -1,    -1,   260,   248,    18,   175,    -1,
      -1,    11,   251,   249,   194,    -1,    -1,    11,    65,   250,
     194,    -1,   199,    -1,    64,    -1,   267,    23,   267,    -1,
     267,    23,    10,   261,    -1,   267,    23,   257,    -1,   267,
      23,   245,    -1,    -1,   199,   255,   186,    -1,   199,    -1,
      64,    -1,    65,    -1,    49,   261,   259,    -1,   261,   259,
      -1,   199,    -1,    16,   267,    17,    -1,    16,    10,   261,
      17,    -1,    16,    17,    -1,     3,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     9,    -1,    11,    -1,   262,
      -1,   260,    -1,    75,    -1,    76,    -1,    82,    -1,    83,
      -1,    54,    -1,    55,    -1,    56,    -1,    57,    -1,    58,
      -1,    59,    -1,    61,    -1,    62,    -1,    63,    -1,    64,
      -1,    65,    -1,    69,    -1,   146,    -1,   147,    -1,    86,
      -1,    71,    -1,    72,    -1,    99,    -1,    84,    -1,    73,
      -1,    78,    -1,    60,    -1,    81,    -1,    77,    -1,    79,
      -1,    80,    -1,    89,    -1,    90,    -1,    74,    -1,    67,
      -1,    51,    -1,    93,    -1,    94,    -1,    95,    -1,    96,
      -1,    98,    -1,   267,    -1,   267,    -1,   266,    -1,   267,
      -1,    81,    16,   299,    17,    -1,   268,    -1,   267,    28,
     268,    -1,   267,    29,   268,    -1,   269,    -1,   268,    27,
     269,    -1,   270,    -1,   269,    32,   270,    -1,   269,    33,
     270,    -1,   271,    -1,   270,    25,   271,    -1,   270,    26,
     271,    -1,   272,    -1,   271,    30,   272,    -1,   271,    31,
     272,    -1,   273,    -1,    26,   272,    -1,    24,   272,    -1,
      25,   272,    -1,   274,    -1,    16,   267,    17,    -1,    15,
      -1,    15,   305,    -1,   294,    14,    -1,   260,    -1,    12,
      -1,    12,   305,    -1,    57,    16,   263,    17,    -1,    54,
      16,   290,    17,    -1,    98,    16,   291,    17,    -1,    58,
      16,   301,    17,    -1,    56,    16,   263,    17,    -1,    84,
      16,   263,    17,    -1,    55,    16,   263,   284,    17,    -1,
      38,    -1,    39,    -1,    42,    -1,    43,    -1,   294,    13,
      -1,   146,    16,   282,    17,    -1,   147,    16,   277,    17,
      -1,    86,    16,   279,    17,    -1,    71,    16,   275,    17,
      -1,    72,    16,   285,    17,    -1,    99,    16,   286,    17,
      -1,    73,    16,   292,    17,    -1,    77,    16,   303,    17,
      -1,    82,    16,   267,    17,    -1,    83,    16,   267,    17,
      -1,    89,    16,   263,    17,    -1,    93,    16,   263,    17,
      -1,    94,    16,   267,    17,    -1,    95,    16,   281,    17,
      -1,    96,    16,   289,    17,    -1,    97,    -1,    -1,   276,
      -1,   264,    -1,   276,    22,   264,    -1,    -1,   278,    -1,
     264,    -1,   278,    22,   264,    -1,    -1,   280,    -1,   264,
      -1,   280,    22,   264,    -1,   264,    22,   264,    22,   264,
      -1,    -1,   283,    -1,   264,    -1,   283,    22,   264,    -1,
      -1,    22,   145,    -1,    22,    74,    -1,   264,    -1,   264,
     287,    -1,     9,    -1,     9,    22,     9,    -1,     9,    22,
      14,    -1,     9,    22,    97,    -1,     9,    22,     8,    -1,
      22,   288,    -1,   287,    22,   288,    -1,    60,    23,   295,
      -1,    78,    23,   264,    -1,    90,    23,   264,    -1,   264,
      -1,   263,    22,    60,    23,   295,    -1,   263,    -1,   263,
      22,    60,    23,   295,    -1,   263,    -1,   293,    -1,   292,
      22,   293,    -1,   295,    23,   264,    -1,   264,    -1,    92,
     295,    -1,   199,    -1,     8,    -1,    60,    16,   296,    17,
      -1,     3,    -1,   263,    -1,   263,   297,    -1,    22,   298,
      -1,   297,    22,   298,    -1,    78,    23,   264,    -1,    91,
      23,   264,    -1,   300,    -1,   299,    22,   300,    -1,    79,
      58,    23,   263,    -1,    80,    58,    23,   263,    -1,   267,
      23,   263,    -1,   263,    -1,   263,    22,   302,    -1,    79,
      -1,    80,    -1,   304,    -1,    81,    23,   264,    22,   304,
      -1,   264,    -1,   304,    22,   264,    -1,   100,    -1,   101,
      -1,   102,    -1,   103,    -1,   104,    -1,   105,    -1,   106,
      -1,   107,    -1,   108,    -1,   109,    -1,   110,    -1,   111,
      -1,   112,    -1,   113,    -1,   114,    -1,   115,    -1,   116,
      -1,   117,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   258,   258,   257,   263,   264,   263,   268,   270,   274,
     275,   279,   280,   281,   282,   282,   286,   287,   291,   292,
     293,   293,   297,   298,   299,   299,   303,   303,   308,   309,
     312,   314,   318,   319,   320,   321,   322,   323,   331,   331,
     335,   336,   337,   337,   341,   345,   346,   347,   348,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   404,   405,   409,   410,   417,   417,
     421,   422,   423,   423,   427,   431,   432,   436,   437,   438,
     440,   451,   464,   464,   468,   469,   470,   470,   474,   484,
     485,   485,   493,   493,   497,   498,   499,   499,   504,   503,
     512,   511,   515,   514,   518,   517,   521,   523,   520,   538,
     537,   541,   540,   544,   543,   547,   546,   550,   549,   553,
     555,   552,   562,   561,   570,   572,   578,   577,   586,   587,
     593,   594,   603,   605,   606,   607,   607,   611,   613,   616,
     619,   619,   623,   626,   629,   629,   633,   634,   638,   640,
     642,   645,   645,   650,   658,   658,   663,   664,   665,   665,
     670,   669,   677,   678,   679,   680,   685,   684,   692,   691,
     699,   698,   706,   705,   712,   717,   722,   726,   731,   732,
     733,   734,   734,   738,   739,   740,   741,   741,   745,   747,
     748,   749,   749,   753,   754,   755,   756,   756,   760,   761,
     765,   766,   770,   771,   775,   776,   780,   782,   784,   784,
     788,   790,   792,   792,   796,   798,   800,   800,   804,   806,
     808,   808,   813,   812,   820,   819,   827,   826,   834,   833,
     843,   847,   846,   853,   852,   858,   857,   864,   865,   873,
     878,   892,   894,   900,   899,   906,   908,   910,   915,   920,
     925,   926,   928,   936,   945,   946,   947,   948,   949,   950,
     951,   952,   958,   963,   964,   965,   966,   967,   968,   969,
     970,   971,   973,   974,   975,   977,   979,   980,   982,   984,
     985,   986,   987,   988,   989,   990,   991,   992,   993,   994,
     995,   996,   997,   998,   999,  1001,  1003,  1004,  1005,  1006,
    1007,  1008,  1009,  1016,  1033,  1046,  1054,  1064,  1070,  1071,
    1072,  1076,  1077,  1081,  1082,  1083,  1087,  1088,  1089,  1093,
    1094,  1095,  1099,  1100,  1101,  1102,  1106,  1107,  1111,  1113,
    1115,  1119,  1120,  1122,  1124,  1125,  1126,  1127,  1128,  1130,
    1132,  1135,  1137,  1139,  1141,  1143,  1147,  1149,  1151,  1153,
    1155,  1156,  1157,  1158,  1159,  1160,  1162,  1165,  1168,  1170,
    1172,  1173,  1179,  1180,  1185,  1187,  1193,  1194,  1199,  1201,
    1207,  1208,  1213,  1215,  1221,  1231,  1232,  1237,  1239,  1244,
    1245,  1246,  1250,  1251,  1255,  1256,  1257,  1258,  1259,  1263,
    1264,  1268,  1272,  1274,  1279,  1283,  1285,  1289,  1291,  1295,
    1296,  1303,  1304,  1308,  1311,  1316,  1317,  1318,  1324,  1327,
    1333,  1334,  1338,  1342,  1349,  1350,  1354,  1355,  1356,  1360,
    1361,  1365,  1366,  1370,  1371,  1376,  1378,  1383,  1385,  1387,
    1389,  1391,  1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,
    1409,  1411,  1413,  1415,  1417
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "UILEOF", "error", "$undefined", "NAME", "FONT_NAME", "COLOR_NAME",
  "ARGUMENT_NAME", "REASON_NAME", "CHARSET_NAME", "ENUMVAL_NAME",
  "CLASS_NAME", "CHILD_NAME", "UNS_FLOAT_LITERAL", "COMP_STRING",
  "CHAR_8_LITERAL", "UNS_INT_LITERAL", "LEFT_PAREN", "RIGHT_PAREN",
  "COLON", "SEMICOLON", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "EQUAL_SIGN",
  "NOT", "PLUS", "MINUS", "AND", "OR", "XOR", "MULTIPLY", "DIVIDE",
  "LEFT_SHIFT", "RIGHT_SHIFT", "LIST", "IDENTIFIER", "END", "MODULE",
  "UILTRUE", "UILFALSE", "INCLUDE", "MACRO", "ON", "OFF", "VALUE",
  "ARGUMENTS", "CALLBACKS", "PROCEDURES", "CONTROLS", "PROCEDURE",
  "OBJECT", "OBJECTS", "WIDGET", "GADGET", "FONT", "ARGUMENT", "REASON",
  "PIXMAP", "COLOR", "NAMES", "CHARACTER_SET", "CASE_SENSITIVE",
  "CASE_INSENSITIVE", "VERSION", "MANAGED", "UNMANAGED", "PRIVATE",
  "IMPORTED", "EXPORTED", "UILFILE", "STRING_TABLE", "TRANSLATION_TABLE",
  "COMPOUND_STRING", "FONT_TABLE", "ANY", "STRING", "BOOLEAN", "ICON",
  "RIGHT_TO_LEFT", "BACKGROUND", "FOREGROUND", "COLOR_TABLE", "FLOAT",
  "INTEGER", "CLASS_REC_NAME", "ASCIZ_TABLE", "INTEGER_TABLE",
  "ASCIZ_STRING_TABLE", "COMPOUND_STRING_TABLE", "XBITMAPFILE", "SEPARATE",
  "SIXTEEN_BIT", "POUND", "KEYSYM", "SINGLE_FLOAT", "RGB",
  "WIDE_CHARACTER", "LOC_STRING", "FONTSET", "COMPOUND_STRING_COMPONENT",
  "PIX", "PIXEL", "PIXELS", "IN", "INCH", "INCHES", "CM", "CENTIMETER",
  "CENTIMETERS", "MM", "MILLIMETER", "MILLIMETERS", "PT", "POINT",
  "POINTS", "FU", "FONT_UNIT", "FONT_UNITS", "$accept", "module_block",
  "@1", "module_header", "@2", "@3", "opt_module_clauses",
  "module_clauses", "module_clause", "@4", "sensitivity_value",
  "variant_spec", "@5", "variant_list", "@6", "variant_clause", "@7",
  "variant", "module_declaration_list", "module_declaration",
  "value_declaration", "@8", "value_declaration_list", "@9", "value_decl",
  "value_definition", "value_type", "arg_value_type", "string_table_type",
  "asciz_table_type", "procedure_declaration", "@10",
  "procedure_declaration_list", "@11", "procedure_decl",
  "opt_formal_parameters", "formal_parameter_type", "opt_class",
  "identifier_declaration", "@12", "identifier_declaration_list", "@13",
  "identifier_decl", "include_directive", "@14", "object_declaration",
  "@15", "object_decl_list", "@16", "object_decl", "@17",
  "object_definition", "@18", "@19", "@20", "@21", "@22",
  "control_object_definition", "@23", "@24", "@25", "@26", "@27", "@28",
  "@29", "object_specification", "@30", "opt_variant",
  "control_object_specification", "@31", "opt_create_proc_ref",
  "opt_parens", "object_spec", "@32", "control_object_spec", "@33",
  "child_object_spec", "@34", "object_feature_list", "object_feature",
  "@35", "epsilon_production", "list_declaration", "@36", "list_decl_list",
  "@37", "list_decl", "@38", "list_specification",
  "control_list_specification", "@39", "argument_list_specification",
  "@40", "callback_list_specification", "@41",
  "procedure_list_specification", "@42", "argument_list_type",
  "callback_list_type", "procedure_list_type", "control_list_type",
  "argument_list_spec", "@43", "callback_list_spec", "@44",
  "procedure_list_spec", "@45", "control_list_spec", "@46",
  "argument_list_clause_list", "callback_list_clause_list",
  "procedure_list_clause_list", "control_list_clause_list",
  "argument_list_clause", "@47", "callback_list_clause", "@48",
  "procedure_list_clause", "@49", "control_list_clause", "@50",
  "control_list_def", "@51", "argument_list_def", "@52",
  "callback_list_def", "@53", "procedure_list_def", "@54",
  "control_list_item", "@55", "@56", "@57", "opt_child_managed",
  "argument_list_item", "callback_list_item", "control_item", "@58",
  "opt_managed", "procedure_reference", "procedure_list_def_ref",
  "opt_procedure_argument", "id", "id_ref", "keyword", "character_8_value",
  "private_value", "private_value_plus", "non_export_value", "value",
  "value_1", "value_2", "value_3", "value_4", "value_5", "value_6",
  "operand", "string_list", "string_list_2", "asciz_list", "asciz_list_2",
  "integer_list", "integer_list_2", "rgb_list", "comp_str_list",
  "comp_str_list_2", "opt_arg_type", "comp_str_result",
  "comp_str_comp_result", "comp_str_attrs", "comp_str_attr",
  "wchar_str_result", "font_spec", "fontset_spec", "font_list",
  "font_item", "optional_charset", "valid_charset", "charset_info",
  "charset_attrs", "charset_attr", "color_list", "color_item",
  "color_spec", "mono_color_spec", "icon_spec", "icon_rows",
  "units_specification", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,     1,     2,     3,     4,     5,     6,     7,
       8,    96,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    97,
     115,   103,   104,    98,   105,   106,    99,   107,   108,   100,
     109,   110,   101,   111,   112,   102,   113,   114
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   118,   120,   119,   122,   123,   121,   124,   124,   125,
     125,   126,   126,   126,   127,   126,   128,   128,   129,   129,
     130,   129,   131,   131,   132,   131,   134,   133,   135,   135,
     136,   136,   137,   137,   137,   137,   137,   137,   139,   138,
     140,   140,   141,   140,   142,   143,   143,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   146,   146,   147,   147,   149,   148,
     150,   150,   151,   150,   152,   153,   153,   154,   154,   154,
     154,   155,   157,   156,   158,   158,   159,   158,   160,   161,
     162,   161,   164,   163,   165,   165,   166,   165,   168,   167,
     170,   169,   171,   169,   172,   169,   173,   174,   169,   176,
     175,   177,   175,   178,   175,   179,   175,   180,   175,   181,
     182,   175,   184,   183,   185,   185,   187,   186,   188,   188,
     189,   189,   190,   190,   190,   191,   190,   192,   192,   192,
     193,   192,   194,   194,   195,   194,   196,   196,   197,   197,
     197,   198,   197,   199,   201,   200,   202,   202,   203,   202,
     205,   204,   206,   206,   206,   206,   208,   207,   210,   209,
     212,   211,   214,   213,   215,   216,   217,   218,   219,   219,
     219,   220,   219,   221,   221,   221,   222,   221,   223,   223,
     223,   224,   223,   225,   225,   225,   226,   225,   227,   227,
     228,   228,   229,   229,   230,   230,   231,   231,   232,   231,
     233,   233,   234,   233,   235,   235,   236,   235,   237,   237,
     238,   237,   240,   239,   242,   241,   244,   243,   246,   245,
     247,   248,   247,   249,   247,   250,   247,   251,   251,   252,
     252,   253,   253,   255,   254,   256,   256,   256,   257,   258,
     259,   259,   259,   259,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   263,   264,   265,   265,   266,   267,   267,
     267,   268,   268,   269,   269,   269,   270,   270,   270,   271,
     271,   271,   272,   272,   272,   272,   273,   273,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   275,   275,   276,   276,   277,   277,   278,   278,
     279,   279,   280,   280,   281,   282,   282,   283,   283,   284,
     284,   284,   285,   285,   286,   286,   286,   286,   286,   287,
     287,   288,   288,   288,   289,   290,   290,   291,   291,   292,
     292,   293,   293,   294,   294,   295,   295,   295,   296,   296,
     297,   297,   298,   298,   299,   299,   300,   300,   300,   301,
     301,   302,   302,   303,   303,   304,   304,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     0,     0,     5,     0,     1,     1,
       2,     3,     3,     3,     0,     4,     1,     1,     2,     3,
       0,     3,     2,     3,     0,     3,     0,     4,     1,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     0,     3,
       1,     2,     0,     3,     4,     2,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       1,     2,     0,     3,     4,     0,     3,     0,     1,     1,
       1,     0,     0,     3,     1,     2,     0,     3,     2,     4,
       0,     3,     0,     3,     2,     3,     0,     3,     0,     4,
       0,     3,     0,     3,     0,     3,     0,     0,     6,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       0,     5,     0,     5,     1,     1,     0,     5,     1,     3,
       1,     2,     1,     2,     3,     0,     3,     1,     3,     4,
       0,     3,     3,     4,     0,     3,     1,     2,     2,     2,
       2,     0,     3,     0,     0,     3,     2,     3,     0,     3,
       0,     4,     1,     1,     1,     1,     0,     3,     0,     3,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     2,
       3,     0,     3,     1,     2,     3,     0,     3,     1,     2,
       3,     0,     3,     1,     2,     3,     0,     3,     1,     2,
       1,     2,     1,     2,     1,     2,     2,     2,     0,     3,
       2,     2,     0,     3,     2,     2,     0,     3,     2,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       2,     0,     4,     0,     4,     0,     4,     1,     1,     3,
       4,     3,     3,     0,     3,     1,     1,     1,     3,     2,
       1,     3,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     2,     2,     2,     1,     3,     1,     2,
       2,     1,     1,     2,     4,     4,     4,     4,     4,     4,
       5,     1,     1,     1,     1,     2,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     1,     0,     1,     1,     3,     0,     1,     1,     3,
       0,     1,     1,     3,     5,     0,     1,     1,     3,     0,
       2,     2,     1,     2,     1,     3,     3,     3,     3,     2,
       3,     3,     3,     3,     1,     5,     1,     5,     1,     1,
       3,     3,     1,     2,     1,     1,     4,     1,     1,     2,
       2,     3,     3,     3,     1,     3,     4,     4,     3,     1,
       3,     1,     1,     1,     5,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     2,     0,     1,    30,     0,     0,   284,   285,
     286,   287,   288,   289,   290,   327,   297,   298,   299,   300,
     301,   302,   318,   303,   304,   305,   306,   307,   326,   308,
      94,   312,   313,   316,   325,   293,   294,   320,   317,   321,
     322,   319,   295,   296,   315,    96,   311,    97,    95,   323,
     324,   328,   329,   330,   331,   332,   314,   309,   310,     5,
     291,   120,   184,   112,     0,     0,    38,    98,   122,    31,
      32,    34,    33,    37,    35,    36,     7,     0,     0,     0,
       0,   183,     0,     0,     0,    14,     0,     0,     0,     6,
       8,     9,   121,   188,   185,     0,   190,   116,   113,   114,
       0,     3,   362,   358,   183,   183,   183,   183,   371,   372,
     373,   374,   297,   298,   299,   300,   301,   312,   313,   316,
     320,   295,   296,   315,   311,   323,     0,   328,   329,   330,
     331,   391,   332,   314,   309,   310,   434,   361,     0,   333,
     338,   341,   343,   346,   349,   352,   356,     0,    42,    39,
      40,     0,   102,    99,   100,   105,   126,   123,     0,   128,
       0,     0,     0,   183,    10,     0,     0,   186,     0,     0,
     115,   118,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     363,   359,     0,   354,   355,   353,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     437,   435,     0,   433,   183,   183,   183,   183,   183,     0,
     183,   183,   119,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   375,   360,     0,    41,   183,     0,   101,   107,
     111,     0,     0,   124,     0,     0,    16,    17,    12,    13,
      11,   189,   187,     0,   117,   357,   426,     0,   409,     0,
       0,   449,     0,   394,   334,     0,   393,   412,     0,   284,
     318,   432,     0,   429,     0,   319,   455,     0,   453,     0,
       0,     0,   402,     0,   401,     0,   183,     0,     0,     0,
       0,   424,     0,   428,     0,   414,     0,   407,     0,   406,
     398,     0,   397,   339,   340,   342,   344,   345,   347,   348,
     350,   351,    43,   183,   326,   183,   319,     0,    47,   335,
     336,   103,   110,    73,    54,    56,    55,    51,    57,    61,
      64,    62,   109,    50,    53,    63,    52,    49,    66,    60,
      67,    68,    69,    70,    71,    72,    65,   108,    58,    59,
       0,     0,   127,   125,   183,    20,     0,    15,   204,   205,
     206,   207,   191,   192,   193,   194,   195,   198,   200,   202,
     196,     0,   365,     0,     0,   368,   364,     0,   367,   379,
     183,     0,   413,   380,   382,   183,   183,   183,   383,   183,
     384,   385,   369,   378,   183,   386,   438,     0,   387,   388,
     183,   389,   390,     0,   366,     0,   381,   376,   183,   377,
     183,    46,    48,    45,   183,    44,   106,   104,   132,   136,
     130,   129,   134,     0,    24,    26,    18,     0,     0,     0,
       0,     0,     0,     0,    93,    79,    80,    76,    81,    85,
      88,    86,   411,    75,    78,    87,    77,    74,    84,    89,
      90,    91,    92,   410,    82,    83,   370,   451,   452,   450,
     395,     0,     0,     0,   419,     0,   430,   431,     0,   456,
     403,     0,   439,   436,     0,     0,   418,   415,   416,   417,
     408,   399,   321,   322,     0,     0,   444,     0,     0,     0,
       0,    21,     0,     0,    19,     0,    22,   211,     0,   199,
     292,   208,   216,     0,   201,   213,   221,     0,   203,   218,
     226,     0,   197,   223,     0,     0,   183,   183,   420,   183,
       0,     0,   440,     0,   183,     0,     0,     0,   183,   337,
     183,   152,   133,   137,   131,   135,    25,     0,    23,     0,
     238,   209,   434,     0,   228,     0,     0,     0,     0,   242,
     214,   434,     0,   230,     0,     0,     0,     0,   246,   219,
     258,     0,   232,     0,     0,   183,     0,   250,   183,   224,
     276,   277,   252,     0,   234,     0,     0,   183,   261,   425,
     421,   422,   423,   454,   183,   183,   441,   404,   427,   183,
     183,   448,   445,   183,   183,    28,    29,    27,   212,     0,
       0,   210,   229,   236,   237,   183,   217,     0,     0,   215,
     231,   240,   241,   183,   222,     0,     0,   220,   233,   244,
     245,   183,   280,   279,   227,     0,   268,   265,   267,   263,
       0,   225,   235,   248,   249,   273,   260,     0,   442,   443,
     446,   447,     0,   183,   158,   183,   239,   255,     0,   269,
     243,   257,     0,   272,   271,   247,   259,     0,   283,     0,
     251,     0,     0,   253,     0,   183,   183,   155,     0,   154,
     138,   270,   183,     0,   281,   174,     0,   266,   264,   156,
     274,   143,   145,   141,   149,   139,   262,   147,     0,   159,
     160,   165,     0,   153,   162,   278,   282,     0,   181,   183,
       0,   176,   252,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,   161,     0,   163,     0,   175,     0,   172,
     183,   177,   180,   178,   179,   183,   144,   146,   142,   150,
     140,   148,   166,   164,   182,   173,     0,   183,   170,     0,
     157,   167,   151,     0,   183,     0,   171,   168,   183,   169
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     5,     2,     3,    76,    89,    90,    91,   160,
     248,   357,   423,   427,   492,   428,   493,   667,     7,    69,
      70,    82,   149,   234,   150,   317,   347,   453,   134,   135,
      71,    83,   153,   237,   154,   240,   350,   351,    72,    79,
      98,   169,    99,    73,    77,    74,    84,   157,   241,   158,
     244,   421,   489,   487,   490,   488,   594,   686,   711,   709,
     707,   708,   712,   710,   737,   532,   593,   668,   680,   706,
     643,   689,   693,   714,   740,   743,   677,   697,   700,   701,
     718,   136,    75,    78,    94,   165,    95,   168,   362,   363,
     432,   364,   429,   365,   430,   366,   431,   367,   368,   369,
     370,   499,   539,   504,   548,   508,   557,   512,   566,   543,
     552,   561,   573,   544,   599,   553,   607,   562,   615,   574,
     625,   703,   630,   704,   600,   705,   608,   563,   616,   576,
     637,   662,   661,   629,   546,   555,   636,   664,   577,   654,
     564,   623,   137,   565,    60,   138,   271,   318,   319,   264,
     140,   141,   142,   143,   144,   145,   146,   265,   266,   301,
     302,   283,   284,   290,   298,   299,   374,   268,   296,   382,
     464,   292,   257,   294,   272,   273,   147,   274,   397,   472,
     522,   485,   486,   262,   459,   277,   278,   190
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -675
static const yytype_int16 yypact[] =
{
    -675,    51,  -675,    16,  -675,  -675,  3645,    73,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,    44,    29,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,    -2,    70,  1988,  2087,
      72,  3257,  2186,  2285,  2384,  -675,   102,   113,   115,  -675,
      -2,  -675,  -675,  -675,  3645,   108,  -675,  -675,  3645,  -675,
     123,  -675,   217,   217,  3257,  3257,  3257,  3257,  -675,  -675,
    -675,  -675,   139,   141,   170,   178,   186,   200,   211,   215,
     230,   231,   236,   242,   258,   283,    15,   284,   289,   297,
     298,  -675,   319,   320,   321,   322,  -675,  -675,   206,    77,
     312,    86,   120,   118,  -675,  -675,  -675,   152,  -675,  3645,
    -675,   323,  -675,  3645,  -675,   324,  -675,  3645,   325,  -675,
     330,   121,    15,  3257,  -675,   326,   327,  -675,   336,   329,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,    65,  -675,  -675,  -675,  3257,  3257,  3257,  3257,
    3257,  2675,  3257,  2772,  3354,  3257,  3257,  3257,  2869,  3257,
    -675,  -675,   341,  -675,  3257,  3257,  3257,  3257,  3257,   334,
    2966,  3063,  -675,  3257,  3257,  3257,  3257,  3257,  3257,  3257,
    3257,  3257,  -675,  -675,   339,  -675,  2481,   340,  -675,  3701,
    -675,   342,   343,  -675,   345,    26,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,   264,  -675,  -675,   338,   353,   349,   355,
     356,   354,   358,  -675,    77,   361,   357,   359,   363,   360,
     341,  -675,    11,  -675,   362,   365,  -675,   367,   364,   104,
     111,   372,  -675,   373,   371,   377,  3257,   378,   124,   375,
     381,  -675,   382,   379,   383,   384,   386,  -675,   387,   385,
    -675,   391,   388,   312,   312,    86,   120,   120,   118,   118,
    -675,  -675,  -675,  3451,  3749,  3257,   366,   392,  -675,  -675,
      77,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
     396,   395,  -675,  -675,   -36,  -675,   110,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,   369,  -675,   541,   398,  -675,  -675,   117,  -675,  -675,
    3257,   -35,   394,  -675,  -675,  2772,  3257,  3257,  -675,  3257,
    -675,  -675,  -675,  -675,  3257,  -675,   397,   401,  -675,  -675,
    3257,  -675,  -675,   374,  -675,     6,  -675,  -675,  3257,  -675,
    3257,  -675,  -675,  -675,  3548,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,   399,  -675,  -675,  -675,    94,   403,  1394,
    1493,  1592,  1691,   412,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,   413,   416,   417,  -675,   -35,  -675,  -675,   419,  -675,
    -675,   -28,   420,  -675,   421,   422,  -675,  -675,  -675,  -675,
    -675,  -675,   393,   400,    31,   173,  -675,   434,   436,   434,
     434,  -675,   433,   430,  -675,   435,  -675,  -675,   701,  -675,
    -675,  -675,  -675,   800,  -675,  -675,  -675,  1196,  -675,  -675,
    -675,   208,  -675,  -675,    15,    15,  3257,  3257,  -675,  3257,
     432,   437,  -675,   -28,  3257,    15,   439,   441,  3257,  -675,
    3548,  -675,  -675,  -675,  -675,  -675,  -675,   168,  -675,   438,
    -675,  -675,   411,   899,  -675,   446,   448,   151,   447,  -675,
    -675,   423,   998,  -675,   451,   452,   164,   453,  -675,  -675,
    -675,  1295,  -675,   454,   456,   460,   457,  -675,    22,  -675,
     459,   461,   462,  1097,  -675,   463,   464,  -675,  -675,  -675,
    -675,  -675,  -675,   364,  3257,  3257,  -675,  -675,  -675,  3257,
    3257,  -675,  -675,   431,   431,  -675,  -675,  -675,  -675,   465,
     440,  -675,  -675,  -675,  -675,  3160,  -675,   467,   442,  -675,
    -675,  -675,  -675,   443,  -675,   471,   449,  -675,  -675,  -675,
    -675,  2578,  -675,  -675,  -675,   475,  -675,  -675,  -675,  -675,
     450,  -675,  -675,  -675,  -675,  -675,  -675,   477,  -675,  -675,
    -675,  -675,  3645,   168,  -675,  -675,  -675,  -675,  3645,    77,
    -675,  -675,  3645,  -675,  -675,  -675,  -675,  3645,  -675,   156,
    -675,    28,    28,  -675,   487,   177,   483,  -675,  1790,  -675,
    -675,  -675,   460,   485,  -675,  -675,    20,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,   486,  -675,
    -675,  -675,    89,  -675,  -675,  -675,  -675,   479,  -675,  -675,
     143,  -675,   159,   488,   492,   494,   431,   434,   434,   434,
     504,   434,   434,  -675,   496,  -675,   155,  -675,   499,  -675,
    -675,  -675,  -675,  -675,  -675,   168,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  1889,  -675,  -675,   205,
    -675,  -675,  -675,   498,  -675,   209,  -675,  -675,  -675,  -675
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,   414,  -675,
    -675,  -675,  -675,  -675,  -675,    54,  -675,   -15,  -675,  -675,
    -675,  -675,  -675,  -675,   389,  -675,   207,  -675,    -6,     1,
    -675,  -675,  -675,  -675,   370,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,   426,  -675,  -675,  -675,  -675,  -675,  -675,   376,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -473,  -675,  -199,  -675,  -675,
    -592,  -675,  -675,  -675,  -675,  -675,  -135,  -675,  -670,  -674,
    -675,  -156,  -675,  -675,  -675,  -675,   445,  -675,  -675,  -100,
    -675,   -69,  -675,   -76,  -675,   -75,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,     2,  -675,   -12,  -675,   -14,  -675,   -27,
    -675,  -503,  -675,  -479,  -675,  -490,  -675,   -64,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -121,    18,  -420,  -675,  -162,   148,   241,  -675,   -37,
     -16,   332,    -3,    32,  -101,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
      96,  -675,  -675,  -675,  -675,   174,  -675,  -123,  -675,  -675,
      40,  -675,    35,  -675,  -675,  -675,    47,   468
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -438
static const yytype_int16 yytable[] =
{
      57,   250,   645,   213,   193,   194,   195,    58,   575,   501,
     505,   509,   513,   554,   476,   477,   534,   535,   210,   545,
     478,   698,   716,   211,    59,   461,   721,   355,   384,   675,
     418,   419,   420,   385,   256,   258,   259,   260,   261,   249,
    -290,   699,   721,   462,   139,   281,   356,   285,   676,    85,
     520,     4,   287,     6,   528,   463,   293,    86,    87,   223,
     224,    88,   554,   521,   545,  -183,  -183,   192,  -183,   745,
     575,   721,    57,    57,    61,   212,    57,    57,    57,    58,
      58,    80,   255,    58,    58,    58,   626,   627,    57,    92,
     698,   101,    57,   223,   224,    58,    96,   100,    81,    58,
     151,   155,   159,   479,   425,   223,   224,    62,    63,    64,
     715,   424,    96,    65,   725,   494,   100,    66,   226,   227,
     425,   390,    67,    68,   396,   161,   139,   167,   391,   310,
     311,   426,   223,   224,  -183,  -183,   162,  -183,   163,   223,
     224,   399,   171,    57,   698,   228,   229,    57,   230,   231,
      58,    57,   223,   224,    58,   196,   698,   197,    58,   139,
     139,   139,   139,   139,   720,   232,   233,   151,   279,   280,
     139,   155,   139,   674,   605,   159,   733,   139,   288,   223,
     224,   139,   246,   247,   223,   224,   198,   613,  -183,  -183,
     529,  -183,   223,   224,   199,   530,   457,   458,   422,   320,
    -183,  -183,   200,  -183,  -254,  -256,   698,   303,   304,   567,
     698,     8,     9,    10,    11,    12,   201,    13,  -183,   568,
     595,   596,   666,   306,   307,   222,   744,   202,   671,   569,
     748,   203,   672,   348,   726,   727,   728,   673,   730,   731,
     349,   681,   682,   683,   684,   685,   204,   205,   694,   139,
    -183,  -183,   206,  -183,  -183,  -183,  -183,  -183,   207,    15,
     308,   309,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,   570,   571,   208,    28,   320,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   209,
     214,    51,    52,    53,    54,   215,    55,    56,   348,   358,
     359,   360,   361,   216,   217,   349,   741,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   218,   219,   220,   221,   225,
     239,   236,   542,   295,   243,   251,   252,   551,   254,   263,
     267,   560,   276,   245,   253,   572,   282,   286,   312,   321,
     371,   352,   353,   354,   289,   291,   591,   454,   297,   300,
     372,   373,   375,   376,   455,   378,   377,   484,   379,   380,
     383,   381,   414,  -437,   388,   386,   389,   542,   387,   392,
     393,   579,   580,   394,   395,   398,   551,   400,   401,   402,
     404,   403,   588,   406,   407,   560,   405,   408,   409,   622,
     410,   415,   628,   416,   417,   456,   465,   572,   473,   471,
     491,   635,   496,    57,    57,    57,    57,   640,   641,   433,
      58,    58,    58,    58,   475,   514,   515,   644,   644,   516,
     517,   519,   523,   524,   531,   525,   533,   500,   500,   500,
     500,   526,   536,   537,   538,   584,  -254,   560,   527,   598,
     585,   547,   589,   413,   590,   603,   556,   604,   606,  -256,
     611,   612,  -275,   619,   614,   620,   621,  -306,   624,  -307,
     642,   495,   633,   634,   646,   358,   650,   669,   359,   670,
     655,   139,   652,   484,   660,   665,   360,   679,   361,   688,
     717,    57,   696,   713,   164,    57,   547,   722,    58,   687,
     690,   723,    58,   724,   729,   556,   622,   732,   734,   746,
     702,   412,   597,   238,   170,   500,   736,   678,   460,   578,
     663,   647,   651,   242,   467,   468,   702,   469,   235,   166,
     610,   656,   470,   719,   702,   602,   632,   618,   474,   653,
     644,   695,   139,   139,   411,    57,   480,   305,   481,   466,
     702,   518,    58,   586,   735,   592,   583,    57,   649,   669,
       0,   191,     0,     0,    58,     0,     0,     0,     0,   500,
       0,   742,     0,   702,   659,     0,     0,     0,   747,   702,
       0,   578,   749,   434,     0,   435,     0,   436,   437,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,   439,   440,   441,   442,   443,   444,   445,     0,
       0,     0,     0,   446,   447,     0,    45,   448,    47,    48,
       0,     0,     0,     0,   449,   450,    57,   451,     0,   452,
       0,     0,    57,    58,     0,     0,    57,     0,     0,    58,
       0,    57,     0,    58,     0,     0,     0,     0,    58,     0,
     500,     0,    57,     0,   581,   582,   500,   276,     0,    58,
     500,     0,   587,     0,     0,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   500,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   540,     0,     8,     9,    10,    11,    12,     0,
      13,     0,    14,   102,  -183,  -183,   103,   104,     0,     0,
       0,     0,   541,     0,     0,   105,   106,   107,     0,     0,
      57,     0,   638,   639,     0,     0,     0,    58,     0,   108,
     109,     0,     0,   110,   111,     0,  -183,     0,     0,     0,
       0,     0,    15,     0,   500,   112,   113,   114,   115,   116,
      21,    22,    23,    24,    25,    26,    27,     0,    28,     0,
      29,    30,   117,   118,   119,    34,    35,    36,   120,    38,
      39,    40,    41,   121,   122,   123,    45,   124,    47,    48,
     125,    50,     0,   126,   127,   128,   129,   130,   131,   132,
     133,   549,     0,     8,     9,    10,    11,    12,     0,    13,
       0,    14,   102,  -183,  -183,   103,   104,     0,     0,     0,
       0,   550,     0,     0,   105,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   109,
       0,     0,   110,   111,     0,     0,  -183,     0,     0,     0,
       0,    15,     0,     0,   112,   113,   114,   115,   116,    21,
      22,    23,    24,    25,    26,    27,     0,    28,     0,    29,
      30,   117,   118,   119,    34,    35,    36,   120,    38,    39,
      40,    41,   121,   122,   123,    45,   124,    47,    48,   125,
      50,     0,   126,   127,   128,   129,   130,   131,   132,   133,
     540,     0,     8,     9,    10,    11,    12,     0,    13,     0,
      14,   102,  -183,  -183,   103,   104,     0,     0,     0,     0,
     601,     0,     0,   105,   106,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   109,     0,
       0,   110,   111,     0,  -183,     0,     0,     0,     0,     0,
      15,     0,     0,   112,   113,   114,   115,   116,    21,    22,
      23,    24,    25,    26,    27,     0,    28,     0,    29,    30,
     117,   118,   119,    34,    35,    36,   120,    38,    39,    40,
      41,   121,   122,   123,    45,   124,    47,    48,   125,    50,
       0,   126,   127,   128,   129,   130,   131,   132,   133,   549,
       0,     8,     9,    10,    11,    12,     0,    13,     0,    14,
     102,  -183,  -183,   103,   104,     0,     0,     0,     0,   609,
       0,     0,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   109,     0,     0,
     110,   111,     0,     0,  -183,     0,     0,     0,     0,    15,
       0,     0,   112,   113,   114,   115,   116,    21,    22,    23,
      24,    25,    26,    27,     0,    28,     0,    29,    30,   117,
     118,   119,    34,    35,    36,   120,    38,    39,    40,    41,
     121,   122,   123,    45,   124,    47,    48,   125,    50,     0,
     126,   127,   128,   129,   130,   131,   132,   133,   567,     0,
       8,     9,    10,    11,    12,     0,    13,  -183,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   631,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -183,     0,     0,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,   570,   571,     0,    28,     0,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
      51,    52,    53,    54,     0,    55,    56,   558,     0,     8,
       9,    10,    11,    12,     0,    13,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   559,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -183,     0,     0,     0,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     0,     0,    51,
      52,    53,    54,     0,    55,    56,   558,     0,     8,     9,
      10,    11,    12,     0,    13,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   617,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -183,     0,     0,     0,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,    51,    52,
      53,    54,     0,    55,    56,   497,     0,     8,     9,    10,
      11,    12,     0,    13,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,    51,    52,    53,
      54,     0,    55,    56,   502,     0,     8,     9,    10,    11,
      12,     0,    13,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,     0,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,     0,    51,    52,    53,    54,
       0,    55,    56,   506,     0,     8,     9,    10,    11,    12,
       0,    13,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,   507,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
       0,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,     0,     0,    51,    52,    53,    54,     0,
      55,    56,   510,     0,     8,     9,    10,    11,    12,     0,
      13,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,     0,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     0,     0,    51,    52,    53,    54,     0,    55,
      56,   691,     0,     8,     9,    10,    11,    12,     0,    13,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
     692,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,     0,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,     0,     0,    51,    52,    53,    54,     0,    55,    56,
     738,     0,     8,     9,    10,    11,    12,     0,    13,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,   739,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,     0,    51,    52,    53,    54,     0,    55,    56,    93,
       0,     8,     9,    10,    11,    12,     0,    13,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,     0,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,    51,    52,    53,    54,     0,    55,    56,    97,     0,
       8,     9,    10,    11,    12,     0,    13,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,     0,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
      51,    52,    53,    54,     0,    55,    56,   148,     0,     8,
       9,    10,    11,    12,     0,    13,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,     0,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     0,     0,    51,
      52,    53,    54,     0,    55,    56,   152,     0,     8,     9,
      10,    11,    12,     0,    13,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,    51,    52,
      53,    54,     0,    55,    56,   156,     0,     8,     9,    10,
      11,    12,     0,    13,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,    51,    52,    53,
      54,     0,    55,    56,     8,     9,    10,    11,    12,     0,
      13,     0,    14,   102,     0,     0,   103,   104,     0,     0,
       0,     0,     0,     0,     0,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     109,     0,     0,   110,   111,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,   112,   113,   114,   115,   116,
      21,    22,    23,    24,    25,    26,    27,   313,   314,   315,
      29,    30,   117,   118,   119,    34,    35,    36,   120,    38,
      39,    40,   316,   121,   122,   123,    45,   124,    47,    48,
     125,    50,     0,   126,   127,   128,   129,   130,   131,   132,
     133,     8,     9,    10,    11,    12,     0,    13,   657,    14,
     102,     0,     0,   103,   104,   658,     0,     0,     0,     0,
       0,     0,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   109,     0,     0,
     110,   111,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,   112,   113,   114,   115,   116,    21,    22,    23,
      24,    25,    26,    27,     0,    28,     0,    29,    30,   117,
     118,   119,    34,    35,    36,   120,    38,    39,    40,    41,
     121,   122,   123,    45,   124,    47,    48,   125,    50,     0,
     126,   127,   128,   129,   130,   131,   132,   133,     8,     9,
      10,    11,    12,     0,    13,     0,    14,   102,     0,     0,
     103,   104,  -392,     0,     0,     0,     0,     0,     0,   105,
     106,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,   109,     0,     0,   110,   111,     0,
       0,     0,     0,     0,     0,     0,    15,     0,     0,   112,
     113,   114,   115,   116,    21,    22,    23,    24,    25,    26,
      27,     0,    28,     0,    29,    30,   117,   118,   119,    34,
      35,    36,   120,    38,    39,    40,    41,   121,   122,   123,
      45,   124,    47,    48,   125,    50,     0,   126,   127,   128,
     129,   130,   131,   132,   133,   269,     9,    10,    11,    12,
     211,    13,     0,    14,   102,     0,     0,   103,   104,     0,
       0,     0,     0,     0,     0,     0,   105,   106,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     108,   109,     0,     0,   110,   111,     0,     0,     0,     0,
       0,     0,     0,    15,     0,     0,   112,   113,   114,   115,
     116,    21,   270,    23,    24,    25,    26,    27,     0,    28,
       0,    29,    30,   117,   118,   119,    34,    35,    36,   120,
      38,    39,    40,    41,   121,   122,   123,    45,   124,    47,
      48,   125,    50,     0,   126,   127,   128,   129,   130,   131,
     132,   133,     8,     9,    10,    11,    12,     0,    13,     0,
      14,   102,     0,     0,   103,   104,  -400,     0,     0,     0,
       0,     0,     0,   105,   106,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   109,     0,
       0,   110,   111,     0,     0,     0,     0,     0,     0,     0,
      15,     0,     0,   112,   113,   114,   115,   116,    21,    22,
      23,    24,    25,    26,    27,     0,    28,     0,    29,    30,
     117,   118,   119,    34,    35,    36,   120,    38,    39,    40,
      41,   121,   122,   123,    45,   124,    47,    48,   125,    50,
       0,   126,   127,   128,   129,   130,   131,   132,   133,     8,
       9,    10,    11,    12,     0,    13,     0,    14,   102,     0,
       0,   103,   104,  -405,     0,     0,     0,     0,     0,     0,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   108,   109,     0,     0,   110,   111,
       0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
     112,   113,   114,   115,   116,    21,    22,    23,    24,    25,
      26,    27,     0,    28,     0,    29,    30,   117,   118,   119,
      34,    35,    36,   120,    38,    39,    40,    41,   121,   122,
     123,    45,   124,    47,    48,   125,    50,     0,   126,   127,
     128,   129,   130,   131,   132,   133,     8,     9,    10,    11,
      12,     0,    13,     0,    14,   102,     0,     0,   103,   104,
    -396,     0,     0,     0,     0,     0,     0,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   109,     0,     0,   110,   111,     0,     0,     0,
       0,     0,     0,     0,    15,     0,     0,   112,   113,   114,
     115,   116,    21,    22,    23,    24,    25,    26,    27,     0,
      28,     0,    29,    30,   117,   118,   119,    34,    35,    36,
     120,    38,    39,    40,    41,   121,   122,   123,    45,   124,
      47,    48,   125,    50,     0,   126,   127,   128,   129,   130,
     131,   132,   133,     8,     9,    10,    11,    12,     0,    13,
     648,    14,   102,     0,     0,   103,   104,     0,     0,     0,
       0,     0,     0,     0,   105,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   109,
       0,     0,   110,   111,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,   112,   113,   114,   115,   116,    21,
      22,    23,    24,    25,    26,    27,     0,    28,     0,    29,
      30,   117,   118,   119,    34,    35,    36,   120,    38,    39,
      40,    41,   121,   122,   123,    45,   124,    47,    48,   125,
      50,     0,   126,   127,   128,   129,   130,   131,   132,   133,
       8,     9,    10,    11,    12,     0,    13,     0,    14,   102,
       0,     0,   103,   104,     0,     0,     0,     0,     0,     0,
       0,   105,   106,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   108,   109,     0,     0,   110,
     111,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,   112,   113,   114,   115,   116,    21,    22,    23,    24,
      25,    26,    27,     0,    28,     0,    29,    30,   117,   118,
     119,    34,    35,    36,   120,    38,    39,    40,    41,   121,
     122,   123,    45,   124,    47,    48,   125,    50,     0,   126,
     127,   128,   129,   130,   131,   132,   133,     8,     9,    10,
      11,    12,     0,    13,     0,    14,   102,     0,     0,   103,
     104,     0,     0,     0,     0,     0,     0,     0,   105,   106,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   109,     0,     0,   110,   111,     0,     0,
       0,     0,     0,     0,     0,    15,     0,     0,   112,   113,
     114,   115,   116,    21,    22,    23,    24,    25,    26,    27,
       0,    28,     0,    29,    30,   117,   118,   119,    34,    35,
      36,   120,    38,    39,    40,   275,   121,   122,   123,    45,
     124,    47,    48,   125,    50,     0,   126,   127,   128,   129,
     130,   131,   132,   133,     8,     9,    10,    11,    12,     0,
      13,     0,    14,   102,     0,     0,   103,   104,     0,     0,
       0,     0,     0,     0,     0,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     109,     0,     0,   110,   111,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,   112,   113,   114,   115,   116,
      21,    22,    23,    24,    25,    26,    27,     0,    28,     0,
      29,    30,   117,   118,   119,    34,    35,    36,   120,    38,
      39,    40,   316,   121,   122,   123,    45,   124,    47,    48,
     125,    50,     0,   126,   127,   128,   129,   130,   131,   132,
     133,     8,     9,    10,    11,    12,     0,    13,     0,    14,
     102,     0,     0,   103,   104,     0,     0,     0,     0,     0,
       0,     0,   105,   106,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   109,     0,     0,
     110,   111,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,   112,   113,   114,   115,   116,    21,    22,    23,
      24,    25,    26,    27,     0,    28,     0,    29,    30,   117,
     118,   119,    34,    35,    36,   120,    38,   482,   483,    41,
     121,   122,   123,    45,   124,    47,    48,   125,    50,     0,
     126,   127,   128,   129,   130,   131,   132,   133,     8,     9,
      10,    11,    12,     0,    13,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,   322,    28,     0,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,    51,    52,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,   324,   325,   326,   327,   328,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,   329,   330,   331,   332,   333,   334,   335,     0,
       0,     0,     0,   336,   337,   338,    45,   339,    47,    48,
     340,     0,     0,     0,   341,   342,   343,   344,     0,   345,
     346,   323,     0,   324,   325,   326,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
     329,   330,   331,     0,   333,   334,   335,     0,     0,     0,
       0,   336,   337,   338,    45,   339,    47,    48,   340,     0,
       0,     0,   341,   342,   343,   344,     0,   345,   346
};

static const yytype_int16 yycheck[] =
{
       6,   163,   594,   126,   105,   106,   107,     6,   511,   429,
     430,   431,   432,   503,     8,     9,   489,   490,     3,   498,
      14,     1,   692,     8,     6,    60,   700,     1,    17,     1,
      66,    67,    68,    22,   196,   197,   198,   199,   200,   162,
      18,    21,   716,    78,    81,   207,    20,   209,    20,    51,
      78,     0,   214,    37,    23,    90,   218,    59,    60,    28,
      29,    63,   552,    91,   543,    45,    46,   104,    48,   739,
     573,   745,    78,    79,     1,    60,    82,    83,    84,    78,
      79,    37,    17,    82,    83,    84,    64,    65,    94,    19,
       1,    19,    98,    28,    29,    94,    78,    79,    69,    98,
      82,    83,    84,    97,    10,    28,    29,    34,    35,    36,
      21,     1,    94,    40,   706,    21,    98,    44,    32,    33,
      10,    17,    49,    50,   286,    23,   163,    19,    17,   230,
     231,    21,    28,    29,    45,    46,    23,    48,    23,    28,
      29,    17,    19,   149,     1,    25,    26,   153,    30,    31,
     149,   157,    28,    29,   153,    16,     1,    16,   157,   196,
     197,   198,   199,   200,    21,    13,    14,   149,   205,   206,
     207,   153,   209,    17,    23,   157,    21,   214,   215,    28,
      29,   218,    61,    62,    28,    29,    16,    23,    45,    46,
      17,    48,    28,    29,    16,    22,    79,    80,   354,   236,
      45,    46,    16,    48,    45,    46,     1,   223,   224,     1,
       1,     3,     4,     5,     6,     7,    16,     9,    10,    11,
      52,    53,   642,   226,   227,    19,    21,    16,   648,    21,
      21,    16,   652,   239,   707,   708,   709,   657,   711,   712,
     239,    64,    65,    66,    67,    68,    16,    16,   668,   286,
      45,    46,    16,    48,    45,    46,    48,    48,    16,    51,
     228,   229,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    16,    67,   313,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    16,
      16,    93,    94,    95,    96,    16,    98,    99,   314,    45,
      46,    47,    48,    16,    16,   314,   736,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    16,    16,    16,    16,    27,
      16,    18,   498,     9,    19,    19,    19,   503,    19,   201,
     202,   507,   204,    23,    18,   511,   208,    16,    19,    19,
      22,    19,    19,    18,   216,   217,   528,   373,   220,   221,
      17,    22,    17,    17,   373,    17,    22,   414,    17,    22,
      17,    22,    16,    23,    17,    23,    22,   543,    23,    17,
      17,   514,   515,    22,    17,    17,   552,    22,    17,    17,
      17,    22,   525,    17,    17,   561,    22,    22,    17,   565,
      22,    19,   568,    17,    19,    17,    22,   573,    17,    22,
      21,   577,    19,   429,   430,   431,   432,   589,   590,    60,
     429,   430,   431,   432,    60,    23,    23,   593,   594,    23,
      23,    22,    22,    22,    10,    23,    10,   429,   430,   431,
     432,    58,    19,    23,    19,    23,    45,   613,    58,    21,
      23,   498,    23,   315,    23,    19,   503,    19,    21,    46,
      19,    19,    10,    19,    21,    19,    16,    18,    21,    18,
      49,   427,    19,    19,    19,    45,    19,   643,    46,   645,
      19,   528,    49,   530,    19,    18,    47,    10,    48,    16,
      21,   507,    17,    17,    90,   511,   543,    19,   507,   665,
     666,    19,   511,    19,    10,   552,   672,    21,    19,    21,
     676,   314,   537,   153,    98,   507,   725,   662,   380,   511,
     630,   600,   608,   157,   386,   387,   692,   389,   149,    94,
     552,   616,   394,   699,   700,   543,   573,   561,   400,   613,
     706,   672,   589,   590,   313,   561,   408,   225,   410,   385,
     716,   465,   561,   523,   720,   530,   519,   573,   605,   725,
      -1,   103,    -1,    -1,   573,    -1,    -1,    -1,    -1,   561,
      -1,   737,    -1,   739,   621,    -1,    -1,    -1,   744,   745,
      -1,   573,   748,    52,    -1,    54,    -1,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    82,    83,    -1,    85,    86,    87,    88,
      -1,    -1,    -1,    -1,    93,    94,   642,    96,    -1,    98,
      -1,    -1,   648,   642,    -1,    -1,   652,    -1,    -1,   648,
      -1,   657,    -1,   652,    -1,    -1,    -1,    -1,   657,    -1,
     642,    -1,   668,    -1,   516,   517,   648,   519,    -1,   668,
     652,    -1,   524,    -1,    -1,   657,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   668,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,    -1,
       9,    -1,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    24,    25,    26,    -1,    -1,
     736,    -1,   584,   585,    -1,    -1,    -1,   736,    -1,    38,
      39,    -1,    -1,    42,    43,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    51,    -1,   736,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,     1,    -1,     3,     4,     5,     6,     7,    -1,     9,
      -1,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    42,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
       1,    -1,     3,     4,     5,     6,     7,    -1,     9,    -1,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    42,    43,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,     1,
      -1,     3,     4,     5,     6,     7,    -1,     9,    -1,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,     1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    98,    99,     1,    -1,     3,
       4,     5,     6,     7,    -1,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    98,    99,     1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    98,    99,     1,    -1,     3,     4,     5,
       6,     7,    -1,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    98,    99,     1,    -1,     3,     4,     5,     6,
       7,    -1,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    93,    94,    95,    96,
      -1,    98,    99,     1,    -1,     3,     4,     5,     6,     7,
      -1,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      98,    99,     1,    -1,     3,     4,     5,     6,     7,    -1,
       9,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    98,
      99,     1,    -1,     3,     4,     5,     6,     7,    -1,     9,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    98,    99,
       1,    -1,     3,     4,     5,     6,     7,    -1,     9,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    -1,    93,    94,    95,    96,    -1,    98,    99,     1,
      -1,     3,     4,     5,     6,     7,    -1,     9,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    98,    99,     1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    -1,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    -1,
      93,    94,    95,    96,    -1,    98,    99,     1,    -1,     3,
       4,     5,     6,     7,    -1,     9,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    98,    99,     1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    98,    99,     1,    -1,     3,     4,     5,
       6,     7,    -1,     9,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    98,    99,     3,     4,     5,     6,     7,    -1,
       9,    -1,    11,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,     3,     4,     5,     6,     7,    -1,     9,    10,    11,
      12,    -1,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,     3,     4,
       5,     6,     7,    -1,     9,    -1,    11,    12,    -1,    -1,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    95,    96,    97,
      98,    99,     3,     4,     5,     6,     7,    -1,     9,    -1,
      11,    12,    -1,    -1,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,     3,
       4,     5,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      -1,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    95,    96,    97,    98,    99,     3,     4,     5,     6,
       7,    -1,     9,    -1,    11,    12,    -1,    -1,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    95,    96,
      97,    98,    99,     3,     4,     5,     6,     7,    -1,     9,
      10,    11,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    -1,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    95,    96,    97,    98,    99,
       3,     4,     5,     6,     7,    -1,     9,    -1,    11,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    95,    96,    97,    98,    99,     3,     4,     5,
       6,     7,    -1,     9,    -1,    11,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    95,
      96,    97,    98,    99,     3,     4,     5,     6,     7,    -1,
       9,    -1,    11,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    95,    96,    97,    98,
      99,     3,     4,     5,     6,     7,    -1,     9,    -1,    11,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    95,    96,    97,    98,    99,     3,     4,
       5,     6,     7,    -1,     9,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    10,    67,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    98,
      99,    52,    -1,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    98,    99
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   119,   121,   122,     0,   120,    37,   136,     3,     4,
       5,     6,     7,     9,    11,    51,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    67,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    93,    94,    95,    96,    98,    99,   146,   147,   260,
     262,     1,    34,    35,    36,    40,    44,    49,    50,   137,
     138,   148,   156,   161,   163,   200,   123,   162,   201,   157,
      37,    69,   139,   149,   164,    51,    59,    60,    63,   124,
     125,   126,    19,     1,   202,   204,   260,     1,   158,   160,
     260,    19,    12,    15,    16,    24,    25,    26,    38,    39,
      42,    43,    54,    55,    56,    57,    58,    71,    72,    73,
      77,    82,    83,    84,    86,    89,    92,    93,    94,    95,
      96,    97,    98,    99,   146,   147,   199,   260,   263,   267,
     268,   269,   270,   271,   272,   273,   274,   294,     1,   140,
     142,   260,     1,   150,   152,   260,     1,   165,   167,   260,
     127,    23,    23,    23,   126,   203,   204,    19,   205,   159,
     160,    19,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     305,   305,   267,   272,   272,   272,    16,    16,    16,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
       3,     8,    60,   295,    16,    16,    16,    16,    16,    16,
      16,    16,    19,    28,    29,    27,    32,    33,    25,    26,
      30,    31,    13,    14,   141,   142,    18,   151,   152,    16,
     153,   166,   167,    19,   168,    23,    61,    62,   128,   295,
     263,    19,    19,    18,    19,    17,   263,   290,   263,   263,
     263,   263,   301,   264,   267,   275,   276,   264,   285,     3,
      60,   264,   292,   293,   295,    81,   264,   303,   304,   267,
     267,   263,   264,   279,   280,   263,    16,   263,   267,   264,
     281,   264,   289,   263,   291,     9,   286,   264,   282,   283,
     264,   277,   278,   268,   268,   269,   270,   270,   271,   271,
     272,   272,    19,    66,    67,    68,    81,   143,   265,   266,
     267,    19,    10,    52,    54,    55,    56,    57,    58,    71,
      72,    73,    74,    75,    76,    77,    82,    83,    84,    86,
      89,    93,    94,    95,    96,    98,    99,   144,   146,   147,
     154,   155,    19,    19,    18,     1,    20,   129,    45,    46,
      47,    48,   206,   207,   209,   211,   213,   215,   216,   217,
     218,    22,    17,    22,   284,    17,    17,    22,    17,    17,
      22,    22,   287,    17,    17,    22,    23,    23,    17,    22,
      17,    17,    17,    17,    22,    17,   263,   296,    17,    17,
      22,    17,    17,    22,    17,    22,    17,    17,    22,    17,
      22,   265,   144,   264,    16,    19,    17,    19,    66,    67,
      68,   169,   199,   130,     1,    10,    21,   131,   133,   210,
     212,   214,   208,    60,    52,    54,    56,    57,    58,    71,
      72,    73,    74,    75,    76,    77,    82,    83,    86,    93,
      94,    96,    98,   145,   146,   147,    17,    79,    80,   302,
     264,    60,    78,    90,   288,    22,   293,   264,   264,   264,
     264,    22,   297,    17,   264,    60,     8,     9,    14,    97,
     264,   264,    79,    80,   267,   299,   300,   171,   173,   170,
     172,    21,   132,   134,    21,   133,    19,     1,    20,   219,
     260,   261,     1,    20,   221,   261,     1,    20,   223,   261,
       1,    20,   225,   261,    23,    23,    23,    23,   288,    22,
      78,    91,   298,    22,    22,    23,    58,    58,    23,    17,
      22,    10,   183,    10,   183,   183,    19,    23,    19,   220,
       1,    21,   199,   227,   231,   241,   252,   267,   222,     1,
      21,   199,   228,   233,   243,   253,   267,   224,     1,    21,
     199,   229,   235,   245,   258,   261,   226,     1,    11,    21,
      64,    65,   199,   230,   237,   239,   247,   256,   260,   295,
     295,   264,   264,   304,    23,    23,   298,   264,   295,    23,
      23,   263,   300,   184,   174,    52,    53,   135,    21,   232,
     242,    21,   231,    19,    19,    23,    21,   234,   244,    21,
     233,    19,    19,    23,    21,   236,   246,    21,   235,    19,
      19,    16,   199,   259,    21,   238,    64,    65,   199,   251,
     240,    21,   237,    19,    19,   199,   254,   248,   264,   264,
     263,   263,    49,   188,   199,   188,    19,   209,    10,   267,
      19,   211,    49,   245,   257,    19,   213,    10,    17,   267,
      19,   250,   249,   207,   255,    18,   261,   135,   185,   199,
     199,   261,   261,   261,    17,     1,    20,   194,   194,    10,
     186,    64,    65,    66,    67,    68,   175,   199,    16,   189,
     199,     1,    20,   190,   261,   259,    17,   195,     1,    21,
     196,   197,   199,   239,   241,   243,   187,   178,   179,   177,
     181,   176,   180,    17,   191,    21,   196,    21,   198,   199,
      21,   197,    19,    19,    19,   188,   183,   183,   183,    10,
     183,   183,    21,    21,    19,   199,   185,   182,     1,    20,
     192,   261,   199,   193,    21,   196,    21,   199,    21,   199
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 258 "Uil.y"
    { sar_save_module_source (); }
    break;

  case 4:
#line 263 "Uil.y"
    { sar_create_root ( &(yyval) ); }
    break;

  case 5:
#line 264 "Uil.y"
    { sar_create_module( &(yyval), &(yyvsp[(3) - (3)]), &(yyvsp[(2) - (3)]) ); }
    break;

  case 11:
#line 279 "Uil.y"
    { sar_process_module_version(  &(yyvsp[(3) - (3)]) , &(yyvsp[(1) - (3)]) ); }
    break;

  case 12:
#line 280 "Uil.y"
    { sar_process_module_sensitivity( &(yyvsp[(3) - (3)]) , &(yyvsp[(1) - (3)]) ); }
    break;

  case 13:
#line 281 "Uil.y"
    { sar_process_module_charset( &(yyvsp[(3) - (3)]) , &(yyvsp[(1) - (3)]) ); }
    break;

  case 14:
#line 282 "Uil.y"
    { sar_make_def_obj (&(yyvsp[(1) - (1)])); }
    break;

  case 20:
#line 293 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 24:
#line 299 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 26:
#line 303 "Uil.y"
    { sar_make_def_obj (&(yyvsp[(1) - (1)])); }
    break;

  case 27:
#line 304 "Uil.y"
    { sar_process_module_variant (&(yyvsp[(1) - (4)]), &(yyvsp[(4) - (4)])); (yyval) = gz_yynullval; }
    break;

  case 28:
#line 308 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_widget_entry; }
    break;

  case 29:
#line 309 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_gadget_entry; }
    break;

  case 38:
#line 331 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); sar_save_section_source (&(yyvsp[(1) - (1)]), sym_k_value_section); }
    break;

  case 42:
#line 337 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 44:
#line 341 "Uil.y"
    { sar_bind_value_name( &(yyvsp[(1) - (4)]), &(yyvsp[(3) - (4)]), &(yyvsp[(4) - (4)]) ); }
    break;

  case 45:
#line 345 "Uil.y"
    { (yyval) = (yyvsp[(2) - (2)]); (yyval).b_flags |= sym_m_exported; (yyval).b_flags &= ~sym_m_private; }
    break;

  case 46:
#line 346 "Uil.y"
    { (yyval) = (yyvsp[(2) - (2)]); (yyval).b_flags |= sym_m_private; (yyval).b_flags &= ~sym_m_exported; }
    break;

  case 47:
#line 347 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_flags |= sym_m_private; (yyval).b_flags &= ~sym_m_exported; }
    break;

  case 48:
#line 348 "Uil.y"
    { sar_import_value_entry( &(yyval), &(yyvsp[(2) - (2)]) ); }
    break;

  case 49:
#line 352 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_integer_value; }
    break;

  case 50:
#line 353 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_char_8_value; }
    break;

  case 51:
#line 354 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_pixmap_value; }
    break;

  case 52:
#line 355 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_float_value; }
    break;

  case 53:
#line 356 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_bool_value; }
    break;

  case 54:
#line 357 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_font_value; }
    break;

  case 55:
#line 358 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_reason_value; }
    break;

  case 56:
#line 359 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_argument_value; }
    break;

  case 57:
#line 360 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_color_value; }
    break;

  case 58:
#line 361 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_string_table_value; }
    break;

  case 59:
#line 362 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_asciz_table_value; }
    break;

  case 60:
#line 363 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_integer_table_value; }
    break;

  case 61:
#line 364 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_trans_table_value; }
    break;

  case 62:
#line 365 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_font_table_value; }
    break;

  case 63:
#line 366 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_icon_value; }
    break;

  case 64:
#line 367 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_compound_string_value; }
    break;

  case 65:
#line 368 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_compound_string_value; }
    break;

  case 66:
#line 369 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_class_rec_name_value; }
    break;

  case 67:
#line 370 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_xbitmapfile_value; }
    break;

  case 68:
#line 371 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_keysym_value; }
    break;

  case 69:
#line 372 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_single_float_value; }
    break;

  case 70:
#line 373 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_rgb_value; }
    break;

  case 71:
#line 374 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_wchar_string_value; }
    break;

  case 72:
#line 375 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_fontset_value; }
    break;

  case 73:
#line 376 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_widget_ref_value; }
    break;

  case 74:
#line 381 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_integer_value; }
    break;

  case 75:
#line 382 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_char_8_value; }
    break;

  case 76:
#line 383 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_pixmap_value; }
    break;

  case 77:
#line 384 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_float_value; }
    break;

  case 78:
#line 385 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_bool_value; }
    break;

  case 79:
#line 386 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_font_value; }
    break;

  case 80:
#line 387 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_reason_value; }
    break;

  case 81:
#line 388 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_color_value; }
    break;

  case 82:
#line 389 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_string_table_value; }
    break;

  case 83:
#line 390 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_asciz_table_value; }
    break;

  case 84:
#line 391 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_integer_table_value; }
    break;

  case 85:
#line 392 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_trans_table_value; }
    break;

  case 86:
#line 393 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_font_table_value; }
    break;

  case 87:
#line 394 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_icon_value; }
    break;

  case 88:
#line 395 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_compound_string_value; }
    break;

  case 89:
#line 396 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_keysym_value; }
    break;

  case 90:
#line 397 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_single_float_value; }
    break;

  case 91:
#line 398 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_wchar_string_value; }
    break;

  case 92:
#line 399 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_fontset_value; }
    break;

  case 93:
#line 400 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = sym_k_widget_ref_value; }
    break;

  case 98:
#line 417 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); sar_save_section_source (&(yyvsp[(1) - (1)]), sym_k_procedure_section); }
    break;

  case 102:
#line 423 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 104:
#line 427 "Uil.y"
    { sar_create_procedure( &(yyvsp[(1) - (4)]), &(yyvsp[(2) - (4)]), &(yyvsp[(3) - (4)]), &(yyvsp[(4) - (4)]) );}
    break;

  case 105:
#line 431 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 106:
#line 432 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 107:
#line 436 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_no_value; }
    break;

  case 109:
#line 438 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).b_type = 
sym_k_any_value; }
    break;

  case 110:
#line 441 "Uil.y"
    { YYSTYPE temp[2];
	  temp[0] = gz_yynullval;
	  sar_create_object(&temp[0], sym_k_widget_entry);
	  temp[1] = (yyvsp[(1) - (1)]); sar_set_object_class(&temp[1]);
	  sar_verify_object(&temp[1]);
	  (yyval) = temp[0];
	}
    break;

  case 111:
#line 451 "Uil.y"
    { (yyval).b_flags = sym_m_private; }
    break;

  case 112:
#line 464 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); sar_save_section_source (&(yyvsp[(1) - (1)]), sym_k_identifier_section); }
    break;

  case 116:
#line 470 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 118:
#line 474 "Uil.y"
    { sar_create_identifier ( &(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]) ); }
    break;

  case 119:
#line 484 "Uil.y"
    { sar_include_file (&(yyvsp[(3) - (4)]), &(yyvsp[(1) - (4)]), &(yyvsp[(4) - (4)])); }
    break;

  case 120:
#line 485 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 122:
#line 493 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); sar_save_section_source (&(yyvsp[(1) - (1)]), sym_k_object_section); }
    break;

  case 124:
#line 497 "Uil.y"
    { (yyval) = (yyvsp[(1) - (2)]); sar_save_src_semicolon_pos (&(yyvsp[(2) - (2)]) ); }
    break;

  case 125:
#line 498 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); sar_save_src_semicolon_pos (&(yyvsp[(3) - (3)]) ); }
    break;

  case 126:
#line 499 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 128:
#line 504 "Uil.y"
    { sar_create_object (&(yyvsp[(1) - (1)]), sym_k_widget_entry); 
	  sar_link_section (&(yyvsp[(1) - (1)])); (yyval) = gz_yynullval;}
    break;

  case 129:
#line 507 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 130:
#line 512 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_exported); }
    break;

  case 132:
#line 515 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_private); }
    break;

  case 134:
#line 518 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_exported); }
    break;

  case 136:
#line 521 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_imported); }
    break;

  case 137:
#line 523 "Uil.y"
    { sar_set_object_class (&(yyvsp[(3) - (3)])); }
    break;

  case 138:
#line 533 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (6)])); }
    break;

  case 139:
#line 538 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_exported); }
    break;

  case 141:
#line 541 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_private); }
    break;

  case 143:
#line 544 "Uil.y"
    { sar_set_object_flags (& (yyvsp[(1) - (1)]), (sym_m_managed | sym_m_exported)); }
    break;

  case 145:
#line 547 "Uil.y"
    { sar_unset_object_flags (& (yyvsp[(1) - (1)]), sym_m_managed), sar_set_object_flags (& (yyvsp[(1) - (1)]), sym_m_exported); }
    break;

  case 147:
#line 550 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_exported); }
    break;

  case 149:
#line 553 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (1)]), sym_m_imported); }
    break;

  case 150:
#line 555 "Uil.y"
    { sar_set_object_class (&(yyvsp[(3) - (3)])); }
    break;

  case 151:
#line 557 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (5)])); }
    break;

  case 152:
#line 562 "Uil.y"
    { sar_set_object_class (&(yyvsp[(1) - (1)])); }
    break;

  case 153:
#line 566 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (5)])); }
    break;

  case 154:
#line 571 "Uil.y"
    { sar_set_object_variant (&(yyvsp[(1) - (1)])); }
    break;

  case 155:
#line 573 "Uil.y"
    { sar_set_object_variant (&(yyvsp[(1) - (1)])); }
    break;

  case 156:
#line 578 "Uil.y"
    { sar_set_object_class (&(yyvsp[(1) - (1)])); (yyval) = gz_yynullval;}
    break;

  case 157:
#line 582 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (5)])); }
    break;

  case 159:
#line 588 "Uil.y"
    { sar_process_proc_ref (&(yyvsp[(2) - (3)]), &gz_yynullval, sym_k_object_proc);
	  sar_save_user_proc_ref_src ( &(yyvsp[(1) - (3)]), &(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 161:
#line 594 "Uil.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 162:
#line 604 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 165:
#line 607 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 167:
#line 612 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 168:
#line 615 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (3)]), sym_m_exported); }
    break;

  case 169:
#line 618 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (4)]), sym_m_exported); }
    break;

  case 170:
#line 619 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 172:
#line 625 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (3)]), sym_m_private); }
    break;

  case 173:
#line 628 "Uil.y"
    { sar_set_object_flags (&(yyvsp[(1) - (4)]), sym_m_private); }
    break;

  case 174:
#line 629 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 178:
#line 639 "Uil.y"
    { sar_save_list_end ( &(yyvsp[(2) - (2)])); sar_save_feature (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 179:
#line 641 "Uil.y"
    { sar_save_list_end ( &(yyvsp[(2) - (2)])); sar_save_feature (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 180:
#line 643 "Uil.y"
    { sar_save_list_end ( &(yyvsp[(2) - (2)])); sar_save_feature (&(yyvsp[(1) - (2)])); 
sar_update_parent_list (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 181:
#line 645 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 183:
#line 650 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 184:
#line 658 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); sar_save_section_source (&(yyvsp[(1) - (1)]), sym_k_list_section); }
    break;

  case 186:
#line 663 "Uil.y"
    { (yyval) = (yyvsp[(1) - (2)]); sar_save_src_semicolon_pos (&(yyvsp[(2) - (2)]) ); }
    break;

  case 187:
#line 664 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); sar_save_src_semicolon_pos (&(yyvsp[(3) - (3)]) ); }
    break;

  case 188:
#line 665 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 190:
#line 670 "Uil.y"
    { sar_create_object (&(yyvsp[(1) - (1)]), sym_k_list_entry); 
sar_link_section (&(yyvsp[(1) - (1)])); (yyval) = gz_yynullval;}
    break;

  case 191:
#line 673 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 196:
#line 685 "Uil.y"
    { (yyval) = gz_yynullval; sar_set_list_type (&(yyvsp[(1) - (1)])); }
    break;

  case 197:
#line 687 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (3)])); }
    break;

  case 198:
#line 692 "Uil.y"
    { (yyval) = gz_yynullval; sar_set_list_type (&(yyvsp[(1) - (1)])); }
    break;

  case 199:
#line 694 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (3)])); }
    break;

  case 200:
#line 699 "Uil.y"
    { (yyval) = gz_yynullval; sar_set_list_type (&(yyvsp[(1) - (1)])); }
    break;

  case 201:
#line 701 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (3)])); }
    break;

  case 202:
#line 706 "Uil.y"
    { (yyval) = gz_yynullval; sar_set_list_type (&(yyvsp[(1) - (1)])); }
    break;

  case 203:
#line 708 "Uil.y"
    { sar_verify_object (&(yyvsp[(1) - (3)])); }
    break;

  case 204:
#line 713 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_argument_list; }
    break;

  case 205:
#line 718 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_callback_list; }
    break;

  case 206:
#line 723 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_proc_ref_list; }
    break;

  case 207:
#line 727 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_control_list; }
    break;

  case 208:
#line 731 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 211:
#line 734 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 213:
#line 738 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 216:
#line 741 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 218:
#line 746 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 221:
#line 749 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 223:
#line 753 "Uil.y"
    { sar_object_reference (&(yyvsp[(1) - (1)])); }
    break;

  case 226:
#line 756 "Uil.y"
    { lex_issue_error (RIGHT_BRACE); }
    break;

  case 236:
#line 781 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 237:
#line 783 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 238:
#line 784 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 240:
#line 789 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 241:
#line 791 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 242:
#line 792 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 244:
#line 797 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 245:
#line 799 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 246:
#line 800 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 248:
#line 805 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 249:
#line 807 "Uil.y"
    { sar_save_src_entry_end (&(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); sar_add_list_entry (&(yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 250:
#line 808 "Uil.y"
    { lex_issue_error (SEMICOLON); }
    break;

  case 252:
#line 813 "Uil.y"
    { (yyval) = gz_yynullval; sar_create_object (&(yyvsp[(1) - (1)]), sym_k_list_entry); }
    break;

  case 253:
#line 815 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 254:
#line 820 "Uil.y"
    { (yyval) = gz_yynullval; sar_create_object (&(yyvsp[(1) - (1)]), sym_k_list_entry); }
    break;

  case 255:
#line 822 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 256:
#line 827 "Uil.y"
    { (yyval) = gz_yynullval; sar_create_object (&(yyvsp[(1) - (1)]), sym_k_list_entry); }
    break;

  case 257:
#line 829 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 258:
#line 834 "Uil.y"
    { (yyval) = gz_yynullval; sar_create_object (&(yyvsp[(1) - (1)]), sym_k_list_entry); }
    break;

  case 259:
#line 836 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 260:
#line 844 "Uil.y"
    { sar_save_control_item (&(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)])); (yyval) = (yyvsp[(1) - (2)]); }
    break;

  case 261:
#line 847 "Uil.y"
    { sar_create_object (& (yyvsp[(1) - (1)]), sym_k_widget_entry); (yyvsp[(1) - (1)]).b_flags = sym_m_managed; (yyval) = gz_yynullval;}
    break;

  case 262:
#line 850 "Uil.y"
    { sar_save_control_widget (& (yyvsp[(1) - (4)]), & (yyvsp[(2) - (4)])); (yyval) = (yyvsp[(1) - (4)]); }
    break;

  case 263:
#line 853 "Uil.y"
    { sar_create_child (& (yyvsp[(1) - (2)])); (yyvsp[(1) - (2)]).b_flags = sym_m_managed; 
	  (yyval) = gz_yynullval; }
    break;

  case 264:
#line 856 "Uil.y"
    { sar_save_control_widget (& (yyvsp[(1) - (4)]), & (yyvsp[(2) - (4)])); (yyval) = (yyvsp[(1) - (4)]); }
    break;

  case 265:
#line 858 "Uil.y"
    { sar_create_child (& (yyvsp[(1) - (2)])); (yyval) = gz_yynullval; }
    break;

  case 266:
#line 860 "Uil.y"
    { sar_save_control_widget (& (yyvsp[(1) - (4)]), & (yyvsp[(3) - (4)])); (yyval) = (yyvsp[(1) - (4)]); }
    break;

  case 269:
#line 874 "Uil.y"
    { sar_save_argument_pair (&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 270:
#line 879 "Uil.y"
    { YYSTYPE temp [2];
	  temp [0] = gz_yynullval;
	  sar_create_object (&temp [0], sym_k_widget_entry);
	  temp [1] = (yyvsp[(3) - (4)]); sar_set_object_class (&temp [1]);
	  temp [1] = (yyvsp[(4) - (4)]); sar_object_reference (&temp [1]);
	  sar_verify_object (&temp [1]);
	  sar_save_argument_pair (&(yyvsp[(1) - (4)]), &temp [0], &(yyvsp[(2) - (4)])); (yyval) = (yyvsp[(1) - (4)]); }
    break;

  case 271:
#line 893 "Uil.y"
    { sar_save_reason_binding (&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 272:
#line 895 "Uil.y"
    { sar_save_reason_binding (&(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 273:
#line 900 "Uil.y"
    { sar_create_object (&(yyvsp[(1) - (1)]), sym_k_widget_entry); (yyval) = gz_yynullval; }
    break;

  case 274:
#line 902 "Uil.y"
    { (yyval) = (yyvsp[(1) - (3)]); }
    break;

  case 275:
#line 907 "Uil.y"
    { (yyval).b_flags = sym_m_managed; }
    break;

  case 276:
#line 909 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_flags = sym_m_managed; }
    break;

  case 277:
#line 911 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 278:
#line 916 "Uil.y"
    { sar_process_proc_ref (&(yyvsp[(2) - (3)]), &(yyvsp[(3) - (3)]), sym_k_callback_proc); (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 279:
#line 921 "Uil.y"
    { sar_process_proc_ref (&(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]), sym_k_callback_proc); (yyval) = (yyvsp[(1) - (2)]); }
    break;

  case 281:
#line 927 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 282:
#line 929 "Uil.y"
    { YYSTYPE temp [2];
	  temp [0] = gz_yynullval;
	  sar_create_object (&temp [0], sym_k_widget_entry);
	  temp [1] = (yyvsp[(2) - (4)]); sar_set_object_class (&temp [1]);
	  temp [1] = (yyvsp[(3) - (4)]); sar_object_reference (&temp [1]);
	  sar_verify_object (&temp [1]);
	  (yyval) = temp [0]; }
    break;

  case 283:
#line 937 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 291:
#line 952 "Uil.y"
    { sar_map_keyword_to_name( &(yyval), &(yyvsp[(1) - (1)])); }
    break;

  case 292:
#line 959 "Uil.y"
    { sar_process_id_ref (&(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 333:
#line 1017 "Uil.y"
    { 
                 (yyval) = (yyvsp[(1) - (1)]); 
		 if (((yyval).b_type != sym_k_error_value) &&
		     ((yyval).b_type != sym_k_any_value) &&
		    (((yyval).b_flags &sym_m_forward_ref) == 0))
		       {
			if (((yyval).b_type != sym_k_char_8_value) &&
			    ((yyval).b_type != sym_k_localized_string_value))
		  	    sar_value_type_error( &(yyval), sym_k_char_8_value );
			if (((yyval).b_flags &sym_m_private) == 0) 
			    sar_private_error( &(yyval) );
		       }
		}
    break;

  case 334:
#line 1033 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); 
								  if ((yyval).b_type != sym_k_error_value)
								  {
								    if ((((yyval).b_flags &sym_m_private) == 0) &&
									(((yyval).b_flags &sym_m_forward_ref) == 0))
									sar_private_error( &(yyval) );
								  }
 								}
    break;

  case 335:
#line 1046 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); 
 								  if ((yyval).b_type != sym_k_error_value)
 								  {
 								    if ((((yyval).b_flags &sym_m_private) == 0) &&
 									(((yyval).b_flags &sym_m_forward_ref) == 0))
 									sar_private_error( &(yyval) );
 								  }
							}
    break;

  case 336:
#line 1054 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); 
 								  if ((yyval).b_type != sym_k_error_value)
 								  {
 								    if ((((yyval).b_flags &sym_m_private) == 0) &&
 									(((yyval).b_flags &sym_m_forward_ref) == 0))
 									sar_private_error( &(yyval) );
 								  }
 								}
    break;

  case 337:
#line 1064 "Uil.y"
    { sar_make_color_table( &(yyval), &(yyvsp[(3) - (4)]), &(yyvsp[(1) - (4)])); }
    break;

  case 339:
#line 1071 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 340:
#line 1072 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 342:
#line 1077 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 344:
#line 1082 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 345:
#line 1083 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 347:
#line 1088 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 348:
#line 1089 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 350:
#line 1094 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 351:
#line 1095 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); sar_binary_op( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 353:
#line 1100 "Uil.y"
    { (yyval) = (yyvsp[(1) - (2)]); sar_unary_op( &(yyval), &(yyvsp[(2) - (2)]) ); }
    break;

  case 354:
#line 1101 "Uil.y"
    { (yyval) = (yyvsp[(1) - (2)]); sar_unary_op( &(yyval), &(yyvsp[(2) - (2)]) ); }
    break;

  case 355:
#line 1102 "Uil.y"
    { (yyval) = (yyvsp[(1) - (2)]); sar_unary_op( &(yyval), &(yyvsp[(2) - (2)]) ); }
    break;

  case 357:
#line 1107 "Uil.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 358:
#line 1111 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_integer_value, 
								  &(yyvsp[(1) - (1)]), XmPIXELS); }
    break;

  case 359:
#line 1113 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (2)]), sym_k_integer_value, 
								  &(yyvsp[(1) - (2)]), sar_get_units_type(&(yyvsp[(2) - (2)]))); }
    break;

  case 360:
#line 1115 "Uil.y"
    { /* Don't use FALSE because it is a token not a Boolean! */
								  Uil_lex_l_charset_specified = 0; 
								  sar_make_private_value( &(yyval), &(yyvsp[(2) - (2)]), sym_k_char_8_value, 
								  &(yyvsp[(2) - (2)]), 0); }
    break;

  case 361:
#line 1119 "Uil.y"
    { sar_process_id( &(yyval), &(yyvsp[(1) - (1)]) );}
    break;

  case 362:
#line 1120 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_float_value, 
								  &(yyvsp[(1) - (1)]), XmPIXELS); }
    break;

  case 363:
#line 1122 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (2)]), sym_k_float_value, 
								  &(yyvsp[(1) - (2)]), sar_get_units_type(&(yyvsp[(2) - (2)]))); }
    break;

  case 364:
#line 1124 "Uil.y"
    { sar_value_not_implemented( &(yyval), &(yyvsp[(1) - (4)]), "pixmap literal" ); }
    break;

  case 365:
#line 1125 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 366:
#line 1126 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 367:
#line 1127 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 368:
#line 1128 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(3) - (4)]), sym_k_reason_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 369:
#line 1130 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(3) - (4)]), sym_k_class_rec_name_value,
											 &(yyvsp[(1) - (4)]), 0); }
    break;

  case 370:
#line 1133 "Uil.y"
    { sar_make_private_value
								      ( &(yyval), &(yyvsp[(3) - (5)]), sym_k_argument_value, &(yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]).b_type ); }
    break;

  case 371:
#line 1135 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_bool_value, 
								  &(yyvsp[(1) - (1)]), 0); }
    break;

  case 372:
#line 1137 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_bool_value, 
								  &(yyvsp[(1) - (1)]), 0); }
    break;

  case 373:
#line 1139 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_bool_value, 
								  &(yyvsp[(1) - (1)]), 0); }
    break;

  case 374:
#line 1141 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(1) - (1)]), sym_k_bool_value, 
								  &(yyvsp[(1) - (1)]), 0); }
    break;

  case 375:
#line 1143 "Uil.y"
    { /* Don't use FALSE because it is a token not a Boolean! */
								  Uil_lex_l_charset_specified = 0;
								  sar_make_private_value
								      ( &(yyval), &(yyvsp[(2) - (2)]), sym_k_compound_string_value, &(yyvsp[(2) - (2)]), 0); }
    break;

  case 376:
#line 1147 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(3) - (4)]), sym_k_string_table_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 377:
#line 1149 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(3) - (4)]), sym_k_asciz_table_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 378:
#line 1151 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(3) - (4)]), sym_k_integer_table_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 379:
#line 1153 "Uil.y"
    { sar_make_private_value( &(yyval), &(yyvsp[(3) - (4)]), sym_k_trans_table_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 380:
#line 1155 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 381:
#line 1156 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 382:
#line 1157 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 383:
#line 1158 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 384:
#line 1159 "Uil.y"
    { (yyval) = (yyvsp[(1) - (4)]); sar_unary_op( &(yyval), &(yyvsp[(3) - (4)]) ); }
    break;

  case 385:
#line 1160 "Uil.y"
    { (yyval) = (yyvsp[(1) - (4)]); sar_unary_op( &(yyval), &(yyvsp[(3) - (4)]) ); }
    break;

  case 386:
#line 1162 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(3) - (4)]), sym_k_xbitmapfile_value,
											 &(yyvsp[(1) - (4)]), 0); }
    break;

  case 387:
#line 1165 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(3) - (4)]), sym_k_keysym_value,
											 &(yyvsp[(1) - (4)]), 0); }
    break;

  case 388:
#line 1168 "Uil.y"
    { (yyval) = (yyvsp[(1) - (4)]); sar_unary_op( &(yyval), &(yyvsp[(3) - (4)]) ); }
    break;

  case 389:
#line 1170 "Uil.y"
    { sar_make_private_value( & (yyval), & (yyvsp[(3) - (4)]), sym_k_rgb_value, 
								  &(yyvsp[(1) - (4)]), 0); }
    break;

  case 390:
#line 1172 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 391:
#line 1173 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(1) - (1)]), sym_k_localized_string_value,
								  &(yyvsp[(1) - (1)]), 0); }
    break;

  case 392:
#line 1179 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 393:
#line 1181 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 394:
#line 1186 "Uil.y"
    { sar_append_table_value (&(yyvsp[(1) - (1)]), &gz_yynullval, sym_k_trans_table_value, &(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 395:
#line 1188 "Uil.y"
    { sar_append_table_value (&(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), sym_k_trans_table_value, &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 396:
#line 1193 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 397:
#line 1195 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 398:
#line 1200 "Uil.y"
    { sar_append_table_value (&(yyvsp[(1) - (1)]), &gz_yynullval, sym_k_asciz_table_value, &(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 399:
#line 1202 "Uil.y"
    { sar_append_table_value (&(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), sym_k_asciz_table_value, &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 400:
#line 1207 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 401:
#line 1209 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 402:
#line 1214 "Uil.y"
    { sar_append_table_value (&(yyvsp[(1) - (1)]), &gz_yynullval, sym_k_integer_table_value, &(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 403:
#line 1216 "Uil.y"
    { sar_append_table_value (&(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), sym_k_integer_table_value, &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 404:
#line 1222 "Uil.y"
    {sar_append_table_value(&(yyvsp[(1) - (5)]), & gz_yynullval, sym_k_integer_table_value, & (yyvsp[(1) - (5)]));
     sar_append_table_value(& (yyvsp[(3) - (5)]), & (yyvsp[(1) - (5)]), sym_k_integer_table_value, & (yyvsp[(2) - (5)]));
     sar_append_table_value(& (yyvsp[(5) - (5)]), & (yyvsp[(3) - (5)]), sym_k_integer_table_value, & (yyvsp[(4) - (5)]));
     (yyval) = (yyvsp[(5) - (5)]);}
    break;

  case 405:
#line 1231 "Uil.y"
    { (yyval) = gz_yynullval; }
    break;

  case 406:
#line 1233 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 407:
#line 1238 "Uil.y"
    { sar_append_table_value (&(yyvsp[(1) - (1)]), &gz_yynullval, sym_k_string_table_value, &(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 408:
#line 1240 "Uil.y"
    { sar_append_table_value (&(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), sym_k_string_table_value, &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 409:
#line 1244 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_any_value; }
    break;

  case 410:
#line 1245 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = (yyvsp[(2) - (2)]).b_type; }
    break;

  case 411:
#line 1246 "Uil.y"
    { (yyval) = gz_yynullval; (yyval).b_type = sym_k_any_value; }
    break;

  case 412:
#line 1250 "Uil.y"
    { sar_make_comp_str( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)]) ); }
    break;

  case 413:
#line 1251 "Uil.y"
    { sar_make_comp_str( &(yyval), &(yyvsp[(1) - (2)]), &(yyvsp[(2) - (2)]), &(yyvsp[(-1) - (2)]) ); }
    break;

  case 414:
#line 1255 "Uil.y"
    { sar_make_comp_str_comp(&(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)]) ); }
    break;

  case 415:
#line 1256 "Uil.y"
    { sar_make_comp_str_comp( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 416:
#line 1257 "Uil.y"
    { sar_make_comp_str_comp( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 417:
#line 1258 "Uil.y"
    { sar_make_comp_str_comp( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 418:
#line 1259 "Uil.y"
    { sar_make_comp_str_comp( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 419:
#line 1263 "Uil.y"
    { sar_chk_comp_str_attr( &(yyval), &(yyvsp[(2) - (2)]), &gz_yynullval ); }
    break;

  case 420:
#line 1264 "Uil.y"
    { sar_chk_comp_str_attr( &(yyval), &(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]) ); }
    break;

  case 421:
#line 1268 "Uil.y"
    { (yyval).b_type = (yyvsp[(1) - (3)]).b_type; 
							  (yyval).b_tag = (yyvsp[(3) - (3)]).b_tag;
							  (yyval).value.az_keyword_entry = (yyvsp[(3) - (3)]).value.az_keyword_entry;
							  (yyval).value.az_symbol_entry = (yyvsp[(3) - (3)]).value.az_symbol_entry; }
    break;

  case 422:
#line 1272 "Uil.y"
    { (yyval).b_type = (yyvsp[(1) - (3)]).b_type; 
							  (yyval).value.az_symbol_entry = (yyvsp[(3) - (3)]).value.az_symbol_entry; }
    break;

  case 423:
#line 1274 "Uil.y"
    { (yyval).b_type = (yyvsp[(1) - (3)]).b_type; 
							  (yyval).value.az_symbol_entry = (yyvsp[(3) - (3)]).value.az_symbol_entry; }
    break;

  case 424:
#line 1279 "Uil.y"
    { sar_make_wchar_str( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)]) ); }
    break;

  case 425:
#line 1284 "Uil.y"
    { sar_make_font( &(yyval), &(yyvsp[(5) - (5)]), &(yyvsp[(1) - (5)]), &(yyvsp[(-1) - (5)]) ); }
    break;

  case 426:
#line 1285 "Uil.y"
    { sar_make_font( &(yyval), &gz_yynullval, &(yyvsp[(1) - (1)]), &(yyvsp[(-1) - (1)]) ); }
    break;

  case 427:
#line 1290 "Uil.y"
    { sar_make_fontset( &(yyval), &(yyvsp[(5) - (5)]), &(yyvsp[(1) - (5)]), &(yyvsp[(-1) - (5)]) ); }
    break;

  case 428:
#line 1291 "Uil.y"
    { sar_make_fontset( &(yyval), &gz_yynullval, &(yyvsp[(1) - (1)]), &(yyvsp[(-1) - (1)]) ); }
    break;

  case 429:
#line 1295 "Uil.y"
    { sar_make_font_table( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)]) ); }
    break;

  case 430:
#line 1296 "Uil.y"
    { sar_make_font_table( &(yyval), &(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 431:
#line 1303 "Uil.y"
    { sar_make_font_item( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 432:
#line 1304 "Uil.y"
    { sar_make_font_item( &(yyval), &gz_yynullval, &(yyvsp[(1) - (1)]) ); }
    break;

  case 433:
#line 1308 "Uil.y"
    { /* Don't use TRUE because it is a token not a Boolean! */
							  (yyval) = (yyvsp[(2) - (2)]); 
							  Uil_lex_l_charset_specified = 1;}
    break;

  case 435:
#line 1316 "Uil.y"
    { sar_charset_verify (&(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 436:
#line 1317 "Uil.y"
    { (yyval) = (yyvsp[(3) - (4)]); }
    break;

  case 437:
#line 1318 "Uil.y"
    { sar_make_fallback_charset(&(yyvsp[(1) - (1)]));
				  (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 438:
#line 1324 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(1) - (1)]), sym_k_char_8_value, 
							  &(yyvsp[(1) - (1)]), 0);
							  sar_make_charset (&(yyval), &(yyval), &gz_yynullval, &(yyvsp[(1) - (1)])); }
    break;

  case 439:
#line 1327 "Uil.y"
    { sar_make_private_value(&(yyval), &(yyvsp[(1) - (2)]), sym_k_char_8_value, 
							  &(yyvsp[(1) - (2)]), 0);
							  sar_make_charset (&(yyval), &(yyval), &(yyvsp[(2) - (2)]), &(yyvsp[(1) - (2)])); }
    break;

  case 440:
#line 1333 "Uil.y"
    { sar_chk_charset_attr( &(yyval), &(yyvsp[(2) - (2)]), &gz_yynullval ); }
    break;

  case 441:
#line 1334 "Uil.y"
    { sar_chk_charset_attr( &(yyval), &(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]) ); }
    break;

  case 442:
#line 1338 "Uil.y"
    { (yyval).b_type = (yyvsp[(1) - (3)]).b_type; 
							  if ((yyvsp[(3) - (3)]).b_type != sym_k_bool_value) 
							      sar_value_type_error( &(yyvsp[(3) - (3)]), sym_k_bool_value );
							  (yyval).value.az_symbol_entry = (yyvsp[(3) - (3)]).value.az_symbol_entry; }
    break;

  case 443:
#line 1342 "Uil.y"
    { (yyval).b_type = (yyvsp[(1) - (3)]).b_type; 
							  if ((yyvsp[(3) - (3)]).b_type != sym_k_bool_value) 
							      sar_value_type_error( &(yyvsp[(3) - (3)]), sym_k_bool_value );
							  (yyval).value.az_symbol_entry = (yyvsp[(3) - (3)]).value.az_symbol_entry; }
    break;

  case 444:
#line 1349 "Uil.y"
    { sar_append_color_item( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval ); }
    break;

  case 445:
#line 1350 "Uil.y"
    { sar_append_color_item( &(yyval), &(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]) ); }
    break;

  case 446:
#line 1354 "Uil.y"
    { sar_make_color_item( &(yyval), &(yyvsp[(1) - (4)]), &(yyvsp[(4) - (4)]) ); }
    break;

  case 447:
#line 1355 "Uil.y"
    { sar_make_color_item( &(yyval), &(yyvsp[(1) - (4)]), &(yyvsp[(4) - (4)]) ); }
    break;

  case 448:
#line 1356 "Uil.y"
    { sar_make_color_item( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]) ); }
    break;

  case 449:
#line 1360 "Uil.y"
    { sar_make_color( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)]) ); }
    break;

  case 450:
#line 1361 "Uil.y"
    { sar_make_color( &(yyval), &(yyvsp[(1) - (3)]), &(yyvsp[(3) - (3)]), &(yyvsp[(-1) - (3)]) ); }
    break;

  case 453:
#line 1370 "Uil.y"
    { sar_make_icon( &(yyval), &(yyvsp[(1) - (1)]), &gz_yynullval, &(yyvsp[(-1) - (1)])); }
    break;

  case 454:
#line 1372 "Uil.y"
    { sar_make_icon( &(yyval), &(yyvsp[(5) - (5)]), &(yyvsp[(3) - (5)]), &(yyvsp[(-1) - (5)]) ); }
    break;

  case 455:
#line 1377 "Uil.y"
    { sar_append_table_value (&(yyvsp[(1) - (1)]), &gz_yynullval, sym_k_icon_value, &(yyvsp[(1) - (1)])); (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 456:
#line 1379 "Uil.y"
    { sar_append_table_value (&(yyvsp[(3) - (3)]), &(yyvsp[(1) - (3)]), sym_k_icon_value, &(yyvsp[(2) - (3)])); (yyval) = (yyvsp[(3) - (3)]); }
    break;

  case 457:
#line 1384 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 458:
#line 1386 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 459:
#line 1388 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 460:
#line 1390 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 461:
#line 1392 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 462:
#line 1394 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 463:
#line 1396 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 464:
#line 1398 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 465:
#line 1400 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 466:
#line 1402 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 467:
#line 1404 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 468:
#line 1406 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 469:
#line 1408 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 470:
#line 1410 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 471:
#line 1412 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 472:
#line 1414 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 473:
#line 1416 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 474:
#line 1418 "Uil.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;


/* Line 1267 of yacc.c.  */
#line 4680 "Uil.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1421 "Uil.y"


/*    Dummy error routine for the parser.  We will output our own
      error messages.    */

int yyerror (s)
char	* s;
{
	return 0;
}




