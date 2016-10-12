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
     CLASS = 1,
     RESOURCE = 2,
     DATATYPE = 3,
     CONTROLLIST = 4,
     ENUMERATIONSET = 5,
     ENUMERATIONVALUE = 6,
     CHARACTERSET = 7,
     CHILD = 50,
     DOCNAME = 8,
     CONVFUNC = 9,
     ALIAS = 10,
     TYPE = 11,
     RESOURCELITERAL = 12,
     RELATED = 13,
     INTERNALLITERAL = 14,
     XRMRESOURCE = 15,
     EXCLUDE = 16,
     RESOURCES = 17,
     SUPERCLASS = 18,
     CONTROLS = 19,
     WIDGETCLASS = 20,
     DIALOGCLASS = 21,
     DEFAULT = 22,
     ENUMLITERAL = 23,
     XMSTRINGCHARSETNAME = 24,
     DIRECTION = 25,
     PARSEDIRECTION = 26,
     CHARACTERSIZE = 27,
     CTRLMAPSRESOURCE = 28,
     CHILDREN = 51,
     PARENTCLASS = 52,
     METACLASS = 29,
     WIDGET = 30,
     GADGET = 31,
     ARGUMENT = 32,
     REASON = 33,
     CONSTRAINT = 34,
     SUBRESOURCE = 35,
     ATTRTRUE = 36,
     ATTRFALSE = 37,
     LEFTTORIGHT = 38,
     RIGHTTOLEFT = 39,
     ONEBYTE = 40,
     TWOBYTE = 41,
     MIXED1_2BYTE = 42,
     COLON = 43,
     SEMICOLON = 44,
     EQUALS = 45,
     LBRACE = 46,
     RBRACE = 47,
     STRING = 48,
     ERRORTOKEN = 49
   };
#endif
/* Tokens.  */
#define CLASS 1
#define RESOURCE 2
#define DATATYPE 3
#define CONTROLLIST 4
#define ENUMERATIONSET 5
#define ENUMERATIONVALUE 6
#define CHARACTERSET 7
#define CHILD 50
#define DOCNAME 8
#define CONVFUNC 9
#define ALIAS 10
#define TYPE 11
#define RESOURCELITERAL 12
#define RELATED 13
#define INTERNALLITERAL 14
#define XRMRESOURCE 15
#define EXCLUDE 16
#define RESOURCES 17
#define SUPERCLASS 18
#define CONTROLS 19
#define WIDGETCLASS 20
#define DIALOGCLASS 21
#define DEFAULT 22
#define ENUMLITERAL 23
#define XMSTRINGCHARSETNAME 24
#define DIRECTION 25
#define PARSEDIRECTION 26
#define CHARACTERSIZE 27
#define CTRLMAPSRESOURCE 28
#define CHILDREN 51
#define PARENTCLASS 52
#define METACLASS 29
#define WIDGET 30
#define GADGET 31
#define ARGUMENT 32
#define REASON 33
#define CONSTRAINT 34
#define SUBRESOURCE 35
#define ATTRTRUE 36
#define ATTRFALSE 37
#define LEFTTORIGHT 38
#define RIGHTTOLEFT 39
#define ONEBYTE 40
#define TWOBYTE 41
#define MIXED1_2BYTE 42
#define COLON 43
#define SEMICOLON 44
#define EQUALS 45
#define LBRACE 46
#define RBRACE 47
#define STRING 48
#define ERRORTOKEN 49




/* Copy the first part of user declarations.  */
#line 47 "wmlparse.y"


#include "wml.h"

#if defined(__STDC__)
#include <string.h>		/* for strcpy() */
#endif

#ifndef XmConst
#if defined(__STDC__) || !defined( NO_CONST )
#define XmConst const
#else
#define XmConst
#endif /* __STDC__ */
#endif /* XmConst */

#if !defined(__STDC__)
/*
 * Undefine NULL, since it is defined in stdio
 */
#undef NULL
#endif



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
#line 239 "wmlparse.c"

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   339

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  136
/* YYNRULES -- Number of rules.  */
#define YYNRULES  258
/* YYNRULES -- Number of states.  */
#define YYNSTATES  387

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      10,    32,    33,     2,     2,     2,     2,     2,     2,     2,
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
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    24,    28,    31,    34,    38,    39,    43,
      46,    49,    53,    54,    58,    61,    64,    68,    69,    73,
      76,    79,    83,    84,    88,    91,    94,    98,    99,   103,
     106,   109,   113,   114,   118,   121,   124,   128,   129,   133,
     136,   139,   143,   144,   148,   149,   155,   157,   159,   161,
     162,   165,   169,   170,   174,   176,   179,   180,   186,   187,
     193,   196,   199,   202,   203,   207,   209,   211,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   232,   233,   236,
     240,   241,   245,   247,   250,   251,   256,   257,   261,   262,
     265,   269,   270,   274,   276,   279,   280,   286,   287,   293,
     294,   298,   300,   302,   304,   307,   308,   311,   315,   316,
     320,   322,   325,   326,   331,   332,   336,   337,   340,   341,
     345,   348,   349,   352,   356,   357,   361,   363,   366,   367,
     371,   372,   376,   377,   383,   385,   387,   389,   391,   392,
     395,   399,   400,   404,   406,   409,   410,   416,   417,   423,
     424,   428,   430,   432,   434,   436,   438,   440,   442,   444,
     446,   450,   451,   455,   456,   459,   463,   464,   468,   470,
     473,   474,   480,   481,   485,   487,   489,   491,   492,   496,
     497,   500,   504,   505,   509,   511,   514,   515,   520,   521,
     525,   526,   529,   530,   534,   535,   541,   543,   544,   547,
     551,   552,   556,   558,   561,   562,   566,   567,   571,   572,
     577,   578,   581,   585,   586,   590,   592,   595,   596,   602,
     603,   607,   609,   610,   614,   615,   618,   622,   623,   627,
     629,   632,   633,   639,   640,   646,   647,   651,   653,   655,
     657,   659,   661,   663,   665,   667,   669,   671,   673
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      56,     0,    -1,    -1,    56,    57,    -1,    59,    -1,    62,
      -1,    65,    -1,    68,    -1,    71,    -1,    74,    -1,    77,
      -1,    80,    -1,    -1,     1,    58,    49,    -1,     3,    60,
      -1,    83,    49,    -1,    60,    83,    49,    -1,    -1,     1,
      61,    49,    -1,     4,    63,    -1,   128,    49,    -1,    63,
     128,    49,    -1,    -1,     1,    64,    49,    -1,    10,    66,
      -1,   140,    49,    -1,    66,   140,    49,    -1,    -1,     1,
      67,    49,    -1,     5,    69,    -1,   141,    49,    -1,    69,
     141,    49,    -1,    -1,     1,    70,    49,    -1,     6,    72,
      -1,   150,    49,    -1,    72,   150,    49,    -1,    -1,     1,
      73,    49,    -1,     7,    75,    -1,   160,    49,    -1,    75,
     160,    49,    -1,    -1,     1,    76,    49,    -1,     8,    78,
      -1,   169,    49,    -1,    78,   169,    49,    -1,    -1,     1,
      79,    49,    -1,     9,    81,    -1,   178,    49,    -1,    81,
     178,    49,    -1,    -1,     1,    82,    49,    -1,    -1,   190,
      48,    85,    84,    86,    -1,    34,    -1,    35,    -1,    36,
      -1,    -1,    51,    52,    -1,    51,    88,    52,    -1,    -1,
       1,    87,    52,    -1,    89,    -1,    88,    89,    -1,    -1,
      93,    50,    53,    90,    49,    -1,    -1,    94,    50,    95,
      91,    49,    -1,    96,    49,    -1,   112,    49,    -1,   121,
      49,    -1,    -1,     1,    92,    49,    -1,    21,    -1,    33,
      -1,    17,    -1,    11,    -1,    12,    -1,    23,    -1,    31,
      -1,    13,    -1,    24,    -1,    41,    -1,    42,    -1,    20,
      97,    -1,    -1,    51,    52,    -1,    51,    99,    52,    -1,
      -1,     1,    98,    52,    -1,   100,    -1,    99,   100,    -1,
      -1,   190,   101,   103,    49,    -1,    -1,     1,   102,    49,
      -1,    -1,    51,    52,    -1,    51,   105,    52,    -1,    -1,
       1,   104,    52,    -1,   106,    -1,   105,   106,    -1,    -1,
     110,    50,    53,   107,    49,    -1,    -1,   111,    50,    95,
     108,    49,    -1,    -1,     1,   109,    49,    -1,    14,    -1,
      25,    -1,    19,    -1,    22,   113,    -1,    -1,    51,    52,
      -1,    51,   115,    52,    -1,    -1,     1,   114,    52,    -1,
     116,    -1,   115,   116,    -1,    -1,   190,   117,   119,    49,
      -1,    -1,     1,   118,    49,    -1,    -1,    51,    52,    -1,
      -1,     1,   120,    52,    -1,    32,   122,    -1,    -1,    51,
      52,    -1,    51,   124,    52,    -1,    -1,     1,   123,    52,
      -1,   125,    -1,   124,   125,    -1,    -1,   190,   126,    49,
      -1,    -1,     1,   127,    49,    -1,    -1,   190,    48,   130,
     129,   131,    -1,    37,    -1,    38,    -1,    39,    -1,    40,
      -1,    -1,    51,    52,    -1,    51,   133,    52,    -1,    -1,
       1,   132,    52,    -1,   134,    -1,   133,   134,    -1,    -1,
     138,    50,    53,   135,    49,    -1,    -1,   139,    50,    95,
     136,    49,    -1,    -1,     1,   137,    49,    -1,    14,    -1,
      15,    -1,     7,    -1,    17,    -1,    16,    -1,    11,    -1,
      25,    -1,    13,    -1,    18,    -1,   190,    48,    53,    -1,
      -1,   190,   142,   143,    -1,    -1,    51,    52,    -1,    51,
     145,    52,    -1,    -1,     1,   144,    52,    -1,   146,    -1,
     145,   146,    -1,    -1,   149,    50,    53,   147,    49,    -1,
      -1,     1,   148,    49,    -1,    17,    -1,    11,    -1,    18,
      -1,    -1,   190,   151,   152,    -1,    -1,    51,    52,    -1,
      51,   154,    52,    -1,    -1,     1,   153,    52,    -1,   155,
      -1,   154,   155,    -1,    -1,   190,   156,   158,    49,    -1,
      -1,     1,   157,    49,    -1,    -1,    51,    52,    -1,    -1,
       1,   159,    52,    -1,    -1,   190,    48,   162,   161,   163,
      -1,    53,    -1,    -1,    51,    52,    -1,    51,   165,    52,
      -1,    -1,     1,   164,    52,    -1,   166,    -1,   165,   166,
      -1,    -1,   190,   167,    49,    -1,    -1,     1,   168,    49,
      -1,    -1,   190,    48,   170,   171,    -1,    -1,    51,    52,
      -1,    51,   173,    52,    -1,    -1,     1,   172,    52,    -1,
     174,    -1,   173,   174,    -1,    -1,   177,    50,    53,   175,
      49,    -1,    -1,     1,   176,    49,    -1,    26,    -1,    -1,
     190,   179,   180,    -1,    -1,    51,    52,    -1,    51,   182,
      52,    -1,    -1,     1,   181,    52,    -1,   183,    -1,   182,
     183,    -1,    -1,   187,    50,    53,   184,    49,    -1,    -1,
     188,    50,   189,   185,    49,    -1,    -1,     1,   186,    49,
      -1,    17,    -1,    13,    -1,    27,    -1,    28,    -1,    29,
      -1,    30,    -1,    43,    -1,    44,    -1,    45,    -1,    46,
      -1,    47,    -1,    53,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   173,   173,   175,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   187,   196,   201,   202,   203,   203,   207,
     212,   213,   214,   214,   218,   223,   224,   225,   225,   229,
     234,   235,   236,   236,   240,   245,   246,   247,   247,   251,
     256,   257,   258,   258,   262,   267,   268,   269,   269,   273,
     278,   279,   280,   280,   292,   289,   297,   298,   299,   302,
     304,   305,   306,   306,   310,   311,   318,   315,   323,   320,
     325,   326,   327,   328,   328,   332,   333,   334,   335,   336,
     337,   338,   339,   343,   347,   348,   352,   355,   357,   358,
     359,   359,   363,   364,   369,   368,   372,   372,   375,   377,
     378,   379,   379,   383,   384,   391,   388,   396,   393,   398,
     398,   402,   403,   407,   411,   414,   416,   417,   418,   418,
     422,   423,   428,   427,   431,   431,   434,   436,   437,   437,
     441,   444,   446,   447,   448,   448,   452,   453,   458,   457,
     460,   460,   472,   469,   477,   478,   479,   480,   483,   485,
     486,   487,   487,   491,   492,   499,   496,   504,   501,   506,
     506,   510,   511,   512,   513,   514,   515,   516,   517,   521,
     529,   539,   538,   543,   545,   546,   547,   547,   551,   552,
     559,   556,   561,   561,   565,   566,   567,   577,   576,   581,
     583,   584,   585,   585,   589,   590,   595,   594,   598,   598,
     601,   603,   604,   604,   616,   613,   622,   625,   627,   628,
     629,   629,   633,   634,   639,   638,   641,   641,   652,   650,
     656,   658,   659,   660,   660,   664,   665,   672,   669,   674,
     674,   678,   688,   687,   692,   694,   695,   696,   696,   700,
     701,   708,   705,   713,   710,   715,   715,   719,   720,   721,
     725,   726,   727,   731,   732,   733,   734,   735,   743
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CLASS", "RESOURCE", "DATATYPE",
  "CONTROLLIST", "ENUMERATIONSET", "ENUMERATIONVALUE", "CHARACTERSET",
  "CHILD", "DOCNAME", "CONVFUNC", "ALIAS", "TYPE", "RESOURCELITERAL",
  "RELATED", "INTERNALLITERAL", "XRMRESOURCE", "EXCLUDE", "RESOURCES",
  "SUPERCLASS", "CONTROLS", "WIDGETCLASS", "DIALOGCLASS", "DEFAULT",
  "ENUMLITERAL", "XMSTRINGCHARSETNAME", "DIRECTION", "PARSEDIRECTION",
  "CHARACTERSIZE", "CTRLMAPSRESOURCE", "CHILDREN", "PARENTCLASS",
  "METACLASS", "WIDGET", "GADGET", "ARGUMENT", "REASON", "CONSTRAINT",
  "SUBRESOURCE", "ATTRTRUE", "ATTRFALSE", "LEFTTORIGHT", "RIGHTTOLEFT",
  "ONEBYTE", "TWOBYTE", "MIXED1_2BYTE", "COLON", "SEMICOLON", "EQUALS",
  "LBRACE", "RBRACE", "STRING", "ERRORTOKEN", "$accept",
  "statement_block_list", "statement_block", "@1", "class_statement_block",
  "class_statement_list", "@2", "resource_statement_block",
  "resource_statement_list", "@3", "child_statement_block",
  "child_statement_list", "@4", "datatype_statement_block",
  "datatype_statement_list", "@5", "ctrlist_statement_block",
  "ctrlist_statement_list", "@6", "enumset_statement_block",
  "enumset_statement_list", "@7", "enumval_statement_block",
  "enumval_statement_list", "@8", "charset_statement_block",
  "charset_statement_list", "@9", "class_statement", "@10", "class_type",
  "class_definition", "@11", "class_attribute_list", "class_attribute",
  "@12", "@13", "@14", "class_attribute_name",
  "boolean_class_attribute_name", "boolean_attribute_value",
  "class_resources", "class_resources_block", "@15", "class_resource_list",
  "class_resource_element", "@16", "@17", "class_resource_attributes",
  "@18", "class_resource_attribute_list",
  "class_resource_attribute_element", "@19", "@20", "@21",
  "class_resource_attribute_name", "boolean_class_resource_attribute_name",
  "class_controls", "class_controls_block", "@22", "class_controls_list",
  "class_controls_element", "@23", "@24", "class_controls_attributes",
  "@25", "class_children", "class_children_block", "@26",
  "class_children_list", "class_children_element", "@27", "@28",
  "resource_statement", "@29", "resource_type", "resource_definition",
  "@30", "resource_attribute_list", "resource_attribute", "@31", "@32",
  "@33", "resource_attribute_name",
  "boolean_resource_resource_attribute_name", "child_statement",
  "datatype_statement", "@34", "datatype_definition", "@35",
  "datatype_attribute_list", "datatype_attribute", "@36", "@37",
  "datatype_attribute_name", "ctrlist_statement", "@38",
  "ctrlist_definition", "@39", "ctrlist_controls_list", "ctrlist_control",
  "@40", "@41", "ctrlist_controls_attributes", "@42", "enumset_statement",
  "@43", "enumset_data_type", "enumset_definition", "@44",
  "enumset_value_list", "enumset_value", "@45", "@46", "enumval_statement",
  "@47", "enumvalue_definition", "@48", "enumvalue_attributes_list",
  "enumvalue_attribute", "@49", "@50", "enumvalue_attribute_name",
  "charset_statement", "@51", "charset_definition", "@52",
  "charset_attribute_list", "charset_attribute", "@53", "@54", "@55",
  "charset_attribute_name", "predefined_charset_attribute_name",
  "predefined_charset_attribute_value", "name", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,     1,     2,     3,     4,     5,     6,     7,
      50,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    51,    52,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    58,    57,    59,    60,    60,    61,    60,    62,
      63,    63,    64,    63,    65,    66,    66,    67,    66,    68,
      69,    69,    70,    69,    71,    72,    72,    73,    72,    74,
      75,    75,    76,    75,    77,    78,    78,    79,    78,    80,
      81,    81,    82,    81,    84,    83,    85,    85,    85,    86,
      86,    86,    87,    86,    88,    88,    90,    89,    91,    89,
      89,    89,    89,    92,    89,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    95,    95,    96,    97,    97,    97,
      98,    97,    99,    99,   101,   100,   102,   100,   103,   103,
     103,   104,   103,   105,   105,   107,   106,   108,   106,   109,
     106,   110,   110,   111,   112,   113,   113,   113,   114,   113,
     115,   115,   117,   116,   118,   116,   119,   119,   120,   119,
     121,   122,   122,   122,   123,   122,   124,   124,   126,   125,
     127,   125,   129,   128,   130,   130,   130,   130,   131,   131,
     131,   132,   131,   133,   133,   135,   134,   136,   134,   137,
     134,   138,   138,   138,   138,   138,   138,   138,   138,   139,
     140,   142,   141,   143,   143,   143,   144,   143,   145,   145,
     147,   146,   148,   146,   149,   149,   149,   151,   150,   152,
     152,   152,   153,   152,   154,   154,   156,   155,   157,   155,
     158,   158,   159,   158,   161,   160,   162,   163,   163,   163,
     164,   163,   165,   165,   167,   166,   168,   166,   170,   169,
     171,   171,   171,   172,   171,   173,   173,   175,   174,   176,
     174,   177,   179,   178,   180,   180,   180,   181,   180,   182,
     182,   184,   183,   185,   183,   186,   183,   187,   187,   187,
     188,   188,   188,   189,   189,   189,   189,   189,   190
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     2,     2,     3,     0,     3,     2,
       2,     3,     0,     3,     2,     2,     3,     0,     3,     2,
       2,     3,     0,     3,     2,     2,     3,     0,     3,     2,
       2,     3,     0,     3,     2,     2,     3,     0,     3,     2,
       2,     3,     0,     3,     0,     5,     1,     1,     1,     0,
       2,     3,     0,     3,     1,     2,     0,     5,     0,     5,
       2,     2,     2,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     2,     3,
       0,     3,     1,     2,     0,     4,     0,     3,     0,     2,
       3,     0,     3,     1,     2,     0,     5,     0,     5,     0,
       3,     1,     1,     1,     2,     0,     2,     3,     0,     3,
       1,     2,     0,     4,     0,     3,     0,     2,     0,     3,
       2,     0,     2,     3,     0,     3,     1,     2,     0,     3,
       0,     3,     0,     5,     1,     1,     1,     1,     0,     2,
       3,     0,     3,     1,     2,     0,     5,     0,     5,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     3,     0,     2,     3,     0,     3,     1,     2,
       0,     5,     0,     3,     1,     1,     1,     0,     3,     0,
       2,     3,     0,     3,     1,     2,     0,     4,     0,     3,
       0,     2,     0,     3,     0,     5,     1,     0,     2,     3,
       0,     3,     1,     2,     0,     3,     0,     3,     0,     4,
       0,     2,     3,     0,     3,     1,     2,     0,     5,     0,
       3,     1,     0,     3,     0,     2,     3,     0,     3,     1,
       2,     0,     5,     0,     5,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,    17,   258,    14,     0,     0,    22,    19,     0,
       0,    32,    29,     0,   171,    37,    34,     0,   187,    42,
      39,     0,     0,    47,    44,     0,     0,    52,    49,     0,
     232,    27,    24,     0,     0,    13,     0,     0,    15,     0,
       0,     0,    20,     0,     0,     0,    30,     0,     0,     0,
      35,     0,     0,     0,    40,     0,     0,     0,    45,   218,
       0,     0,    50,     0,     0,     0,    25,     0,    18,    16,
      56,    57,    58,    54,    23,    21,   144,   145,   146,   147,
     142,    33,    31,   176,     0,   172,    38,    36,   192,     0,
     188,    43,    41,   206,   204,    48,    46,     0,    53,    51,
     237,     0,   233,    28,    26,   170,     0,     0,     0,   182,
     185,   184,   186,   174,     0,   178,     0,     0,   198,   190,
       0,   194,   196,     0,   223,     0,   219,     0,   245,   248,
     247,   249,   250,   251,   252,   235,     0,   239,     0,     0,
      62,     0,    55,   151,     0,   143,   177,     0,   175,   179,
       0,   193,     0,   191,   195,     0,   210,     0,   205,     0,
     229,   231,   221,     0,   225,     0,   238,     0,   236,   240,
       0,     0,     0,    73,    78,    79,    82,    77,     0,    75,
       0,    80,    83,    81,     0,    76,    60,     0,    64,     0,
       0,     0,     0,     0,     0,   159,   163,   166,   168,   161,
     162,   165,   164,   169,   167,   149,     0,   153,     0,     0,
     183,   180,   199,   202,     0,     0,     0,   216,   208,     0,
     212,   214,   224,     0,   222,   226,     0,   246,   241,   253,
     254,   255,   256,   257,   243,    63,     0,    90,     0,    86,
     118,     0,   114,   134,     0,   130,    61,    65,     0,     0,
      70,    71,    72,   152,     0,   150,   154,     0,     0,     0,
       0,   201,   197,   211,     0,   209,   213,     0,   230,   227,
       0,     0,    74,     0,    96,    88,     0,    92,    94,     0,
     124,   116,     0,   120,   122,     0,   140,   132,     0,   136,
     138,    66,    84,    85,    68,   160,   155,   157,   181,   203,
     217,   215,     0,   242,   244,    91,     0,    89,    93,     0,
     119,     0,   117,   121,     0,   135,     0,   133,   137,     0,
       0,     0,     0,     0,   228,    97,   101,     0,     0,   125,
     128,     0,     0,   141,   139,    67,    69,   156,   158,     0,
     109,   111,   113,   112,    99,     0,   103,     0,     0,    95,
       0,   127,   123,   102,     0,   100,   104,     0,     0,   129,
     110,   105,   107,     0,     0,   106,   108
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    12,    21,    13,    24,    56,    14,    28,    60,
      15,    52,    84,    16,    32,    64,    17,    36,    68,    18,
      40,    72,    19,    44,    76,    20,    48,    80,    25,   126,
      93,   162,   192,   207,   208,   340,   341,   256,   209,   210,
     314,   211,   259,   293,   296,   297,   329,   326,   348,   359,
     365,   366,   383,   384,   374,   367,   368,   212,   262,   299,
     302,   303,   334,   331,   352,   370,   213,   265,   305,   308,
     309,   339,   336,    29,   127,   100,   165,   214,   226,   227,
     342,   343,   274,   228,   229,    53,    33,    67,   105,   128,
     134,   135,   279,   167,   136,    37,    71,   110,   137,   140,
     141,   175,   172,   235,   280,    41,   143,   114,   178,   236,
     239,   240,   287,   284,    45,   117,   146,   179,   183,   184,
     322,   243,   185,    49,    83,   122,   147,   156,   157,   290,
     291,   187,   158,   159,   254,    26
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -272
static const yytype_int16 yypact[] =
{
    -272,   264,  -272,  -272,    27,    28,    29,    31,    32,    33,
      35,    36,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,   -40,  -272,  -272,   -42,   -36,   -33,  -272,   -42,   -32,
     -29,  -272,   -42,   -28,  -272,  -272,   -42,   -24,  -272,  -272,
     -42,   -11,    -2,  -272,   -42,     2,     4,  -272,   -42,     6,
    -272,  -272,   -42,    38,     5,  -272,    54,    66,  -272,   150,
      67,    69,  -272,   238,    70,    74,  -272,    43,    77,    83,
    -272,    44,    93,    97,  -272,    99,   101,   105,  -272,  -272,
     107,   114,  -272,    47,   117,   118,  -272,   155,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,    39,  -272,  -272,  -272,  -272,     7,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,    48,  -272,  -272,
    -272,   170,  -272,  -272,  -272,  -272,   124,   142,   153,  -272,
    -272,  -272,  -272,  -272,   113,  -272,     8,   157,  -272,  -272,
       9,  -272,  -272,   143,  -272,    53,  -272,   158,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,   189,  -272,   162,   164,
    -272,    89,  -272,  -272,   144,  -272,  -272,   172,  -272,  -272,
     173,  -272,   174,  -272,  -272,   152,  -272,    11,  -272,   175,
    -272,  -272,  -272,    82,  -272,   178,  -272,   184,  -272,  -272,
     183,   204,   186,  -272,  -272,  -272,  -272,  -272,   181,  -272,
     188,  -272,  -272,  -272,   194,  -272,  -272,   116,  -272,   190,
     192,   195,   197,   208,   214,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,   163,  -272,   205,   229,
    -272,  -272,  -272,  -272,   228,   211,   230,  -272,  -272,    13,
    -272,  -272,  -272,   232,  -272,  -272,   231,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,   234,  -272,    15,  -272,
    -272,    17,  -272,  -272,    19,  -272,  -272,  -272,   233,     0,
    -272,  -272,  -272,  -272,   236,  -272,  -272,   235,     0,   240,
     241,  -272,  -272,  -272,   243,  -272,  -272,   245,  -272,  -272,
     246,   247,  -272,   248,  -272,  -272,    21,  -272,  -272,   249,
    -272,  -272,    23,  -272,  -272,   250,  -272,  -272,    25,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,   255,  -272,  -272,  -272,   256,  -272,  -272,   203,
    -272,   257,  -272,  -272,   212,  -272,   258,  -272,  -272,   259,
     260,   261,   262,   263,  -272,  -272,  -272,   206,   265,  -272,
    -272,   266,   267,  -272,  -272,  -272,  -272,  -272,  -272,   268,
    -272,  -272,  -272,  -272,  -272,   210,  -272,   237,   269,  -272,
     270,  -272,  -272,  -272,   272,  -272,  -272,   271,     0,  -272,
    -272,  -272,  -272,   274,   276,  -272,  -272
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,   275,  -272,
    -272,  -272,  -272,  -272,    91,  -272,  -272,  -272,  -272,  -272,
    -271,  -272,  -272,  -272,  -272,    -6,  -272,  -272,  -272,  -272,
    -272,   -52,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,    24,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
      20,  -272,  -272,   287,  -272,  -272,  -272,  -272,  -272,   103,
    -272,  -272,  -272,  -272,  -272,   278,   285,  -272,  -272,  -272,
    -272,   193,  -272,  -272,  -272,   295,  -272,  -272,  -272,  -272,
     196,  -272,  -272,  -272,  -272,   292,  -272,  -272,  -272,  -272,
    -272,    94,  -272,  -272,   290,  -272,  -272,  -272,  -272,   154,
    -272,  -272,  -272,   291,  -272,  -272,  -272,  -272,   179,  -272,
    -272,  -272,  -272,  -272,  -272,    -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -235
static const yytype_int16 yytable[] =
{
      30,    34,    38,    42,    46,    50,    54,   317,   138,    55,
     138,    23,   237,    58,   237,    59,   294,    62,   300,    63,
     306,    66,   294,    30,   300,    70,   306,    34,    22,    27,
      31,    38,    35,    39,    43,    42,    47,    51,    74,    46,
     129,   312,   313,    50,   103,   108,    75,    54,   120,   144,
     130,    78,    79,    87,   180,    82,   131,   132,   170,   139,
      23,   173,    23,   238,    23,   285,    23,   295,    23,   301,
      23,   307,    23,   327,    23,   332,    23,   337,    23,   181,
      23,    23,    23,   180,    23,    23,    23,    86,    23,    23,
     193,   133,  -173,  -189,   104,   109,  -234,  -220,   121,   145,
     194,   195,   196,    88,   142,   182,   197,   382,   181,   198,
     199,   200,   201,   202,   129,    89,    94,   193,    95,   101,
     203,   204,   205,   102,   130,   160,   106,   194,   195,   196,
     131,   132,   107,   197,   244,   142,   198,   199,   200,   201,
     202,   206,   111,   163,   176,   215,   112,   203,   204,   205,
     115,   216,   113,   233,   116,   217,   118,   218,   219,   220,
     221,   222,   223,   119,   215,   168,   123,   124,   266,   224,
     216,   148,   241,   -59,   217,   161,   218,   219,   220,   221,
     222,   223,   257,   149,    90,    91,    92,   150,   224,   260,
     148,  -148,  -207,   164,   177,   263,   225,   151,   152,   153,
     154,  -200,   149,   234,   346,   166,   150,   360,   125,   171,
     186,   360,   190,   350,   191,   275,   151,   152,   153,   154,
     361,   230,   155,   232,   361,   362,   231,   242,   246,   362,
     -87,   363,   258,   247,   241,   363,   248,  -115,   255,   261,
     268,   188,   269,  -131,   270,   264,   271,   249,   250,   251,
     252,   253,   -98,   298,   347,   277,   304,   272,   364,   310,
     282,  -126,   375,   351,     2,     3,   273,     4,     5,     6,
       7,     8,     9,    10,    11,    96,    97,    98,    99,   278,
     281,   288,   283,   292,   289,   315,   311,   377,   316,   318,
     328,   298,   320,   319,   321,   323,   324,   304,   267,    57,
     325,   330,   335,   310,   344,   345,   349,   353,   354,   355,
     356,   357,   358,   376,   369,    61,   372,    65,   371,   378,
     373,   380,   379,   385,   381,   386,   333,   169,   338,   276,
      85,    69,    73,   286,    77,   189,   174,   245,     0,    81
};

static const yytype_int16 yycheck[] =
{
       5,     6,     7,     8,     9,    10,    11,   278,     1,    49,
       1,    53,     1,    49,     1,    48,     1,    49,     1,    48,
       1,    49,     1,    28,     1,    49,     1,    32,     1,     1,
       1,    36,     1,     1,     1,    40,     1,     1,    49,    44,
       1,    41,    42,    48,     1,     1,    48,    52,     1,     1,
      11,    49,    48,    48,     1,    49,    17,    18,    50,    52,
      53,    52,    53,    52,    53,    52,    53,    52,    53,    52,
      53,    52,    53,    52,    53,    52,    53,    52,    53,    26,
      53,    53,    53,     1,    53,    53,    53,    49,    53,    53,
       1,    52,    49,    49,    51,    51,    49,    49,    51,    51,
      11,    12,    13,    49,   109,    52,    17,   378,    26,    20,
      21,    22,    23,    24,     1,    49,    49,     1,    49,    49,
      31,    32,    33,    49,    11,     1,    49,    11,    12,    13,
      17,    18,    49,    17,    52,   140,    20,    21,    22,    23,
      24,    52,    49,     1,     1,     1,    49,    31,    32,    33,
      49,     7,    53,     1,    49,    11,    49,    13,    14,    15,
      16,    17,    18,    49,     1,    52,    49,    49,    52,    25,
       7,     1,   177,    49,    11,    51,    13,    14,    15,    16,
      17,    18,     1,    13,    34,    35,    36,    17,    25,     1,
       1,    49,    49,    51,    51,     1,    52,    27,    28,    29,
      30,    49,    13,    51,     1,    52,    17,     1,    53,    52,
      52,     1,    50,     1,    50,    52,    27,    28,    29,    30,
      14,    49,    52,    49,    14,    19,    53,    52,    50,    19,
      49,    25,    51,    49,   239,    25,    53,    49,    52,    51,
      50,    52,    50,    49,    49,    51,    49,    43,    44,    45,
      46,    47,    49,   258,    51,    50,   261,    49,    52,   264,
      49,    49,    52,    51,     0,     1,    52,     3,     4,     5,
       6,     7,     8,     9,    10,    37,    38,    39,    40,    50,
      52,    49,    52,    49,    53,    49,    53,    50,    53,    49,
     296,   296,    49,    52,    49,    49,    49,   302,   207,    24,
      52,    52,    52,   308,    49,    49,    49,    49,    49,    49,
      49,    49,    49,   365,    49,    28,    49,    32,    52,    50,
      52,    49,    52,    49,    53,    49,   302,   134,   308,   226,
      52,    36,    40,   239,    44,   156,   140,   183,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,     0,     1,     3,     4,     5,     6,     7,     8,
       9,    10,    57,    59,    62,    65,    68,    71,    74,    77,
      80,    58,     1,    53,    60,    83,   190,     1,    63,   128,
     190,     1,    69,   141,   190,     1,    72,   150,   190,     1,
      75,   160,   190,     1,    78,   169,   190,     1,    81,   178,
     190,     1,    66,   140,   190,    49,    61,    83,    49,    48,
      64,   128,    49,    48,    70,   141,    49,   142,    73,   150,
      49,   151,    76,   160,    49,    48,    79,   169,    49,    48,
      82,   178,    49,   179,    67,   140,    49,    48,    49,    49,
      34,    35,    36,    85,    49,    49,    37,    38,    39,    40,
     130,    49,    49,     1,    51,   143,    49,    49,     1,    51,
     152,    49,    49,    53,   162,    49,    49,   170,    49,    49,
       1,    51,   180,    49,    49,    53,    84,   129,   144,     1,
      11,    17,    18,    52,   145,   146,   149,   153,     1,    52,
     154,   155,   190,   161,     1,    51,   171,   181,     1,    13,
      17,    27,    28,    29,    30,    52,   182,   183,   187,   188,
       1,    51,    86,     1,    51,   131,    52,   148,    52,   146,
      50,    52,   157,    52,   155,   156,     1,    51,   163,   172,
       1,    26,    52,   173,   174,   177,    52,   186,    52,   183,
      50,    50,    87,     1,    11,    12,    13,    17,    20,    21,
      22,    23,    24,    31,    32,    33,    52,    88,    89,    93,
      94,    96,   112,   121,   132,     1,     7,    11,    13,    14,
      15,    16,    17,    18,    25,    52,   133,   134,   138,   139,
      49,    53,    49,     1,    51,   158,   164,     1,    52,   165,
     166,   190,    52,   176,    52,   174,    50,    49,    53,    43,
      44,    45,    46,    47,   189,    52,    92,     1,    51,    97,
       1,    51,   113,     1,    51,   122,    52,    89,    50,    50,
      49,    49,    49,    52,   137,    52,   134,    50,    50,   147,
     159,    52,    49,    52,   168,    52,   166,   167,    49,    53,
     184,   185,    49,    98,     1,    52,    99,   100,   190,   114,
       1,    52,   115,   116,   190,   123,     1,    52,   124,   125,
     190,    53,    41,    42,    95,    49,    53,    95,    49,    52,
      49,    49,   175,    49,    49,    52,   102,    52,   100,   101,
      52,   118,    52,   116,   117,    52,   127,    52,   125,   126,
      90,    91,   135,   136,    49,    49,     1,    51,   103,    49,
       1,    51,   119,    49,    49,    49,    49,    49,    49,   104,
       1,    14,    19,    25,    52,   105,   106,   110,   111,    49,
     120,    52,    49,    52,   109,    52,   106,    50,    50,    52,
      49,    53,    95,   107,   108,    49,    49
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
        case 12:
#line 187 "wmlparse.y"
    { LexIssueError(0); }
    break;

  case 17:
#line 203 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 22:
#line 214 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 27:
#line 225 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 32:
#line 236 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 37:
#line 247 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 42:
#line 258 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 47:
#line 269 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 52:
#line 280 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 54:
#line 292 "wmlparse.y"
    { wmlCreateClass (yynameval, yytknval1); }
    break;

  case 56:
#line 297 "wmlparse.y"
    { yytknval1 = METACLASS; }
    break;

  case 57:
#line 298 "wmlparse.y"
    { yytknval1 = WIDGET; }
    break;

  case 58:
#line 299 "wmlparse.y"
    { yytknval1 = GADGET; }
    break;

  case 62:
#line 306 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 66:
#line 318 "wmlparse.y"
    { wmlAddClassAttribute (yytknval1, yystringval); }
    break;

  case 68:
#line 323 "wmlparse.y"
    { wmlAddClassAttribute (yytknval1, yytknval2); }
    break;

  case 73:
#line 328 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 75:
#line 332 "wmlparse.y"
    { yytknval1 = SUPERCLASS; }
    break;

  case 76:
#line 333 "wmlparse.y"
    { yytknval1 = PARENTCLASS; }
    break;

  case 77:
#line 334 "wmlparse.y"
    { yytknval1 = INTERNALLITERAL; }
    break;

  case 78:
#line 335 "wmlparse.y"
    { yytknval1 = DOCNAME; }
    break;

  case 79:
#line 336 "wmlparse.y"
    { yytknval1 = CONVFUNC; }
    break;

  case 80:
#line 337 "wmlparse.y"
    { yytknval1 = WIDGETCLASS; }
    break;

  case 81:
#line 338 "wmlparse.y"
    { yytknval1 = CTRLMAPSRESOURCE; }
    break;

  case 82:
#line 339 "wmlparse.y"
    { yytknval1 = ALIAS; }
    break;

  case 83:
#line 343 "wmlparse.y"
    { yytknval1 = DIALOGCLASS; }
    break;

  case 84:
#line 347 "wmlparse.y"
    { yytknval2 = ATTRTRUE; }
    break;

  case 85:
#line 348 "wmlparse.y"
    { yytknval2 = ATTRFALSE; }
    break;

  case 90:
#line 359 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 94:
#line 369 "wmlparse.y"
    { wmlAddClassResource (yynameval); }
    break;

  case 96:
#line 372 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 101:
#line 379 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 105:
#line 391 "wmlparse.y"
    { wmlAddClassResourceAttribute (yytknval1, yystringval); }
    break;

  case 107:
#line 396 "wmlparse.y"
    { wmlAddClassResourceAttribute (yytknval1, yytknval2); }
    break;

  case 109:
#line 398 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 111:
#line 402 "wmlparse.y"
    { yytknval1 = TYPE; }
    break;

  case 112:
#line 403 "wmlparse.y"
    { yytknval1 = DEFAULT; }
    break;

  case 113:
#line 407 "wmlparse.y"
    { yytknval1 = EXCLUDE; }
    break;

  case 118:
#line 418 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 122:
#line 428 "wmlparse.y"
    { wmlAddClassControl (yynameval); }
    break;

  case 124:
#line 431 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 128:
#line 437 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 134:
#line 448 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 138:
#line 458 "wmlparse.y"
    { wmlAddClassChild (yynameval); }
    break;

  case 140:
#line 460 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 142:
#line 472 "wmlparse.y"
    { wmlCreateResource (yynameval, yytknval1); }
    break;

  case 144:
#line 477 "wmlparse.y"
    { yytknval1 = ARGUMENT; }
    break;

  case 145:
#line 478 "wmlparse.y"
    { yytknval1 = REASON; }
    break;

  case 146:
#line 479 "wmlparse.y"
    { yytknval1 = CONSTRAINT; }
    break;

  case 147:
#line 480 "wmlparse.y"
    { yytknval1 = SUBRESOURCE; }
    break;

  case 151:
#line 487 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 155:
#line 499 "wmlparse.y"
    { wmlAddResourceAttribute (yytknval1, yystringval); }
    break;

  case 157:
#line 504 "wmlparse.y"
    { wmlAddResourceAttribute (yytknval1, yytknval2); }
    break;

  case 159:
#line 506 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 161:
#line 510 "wmlparse.y"
    { yytknval1 = TYPE; }
    break;

  case 162:
#line 511 "wmlparse.y"
    { yytknval1 = RESOURCELITERAL; }
    break;

  case 163:
#line 512 "wmlparse.y"
    { yytknval1 = ENUMERATIONSET; }
    break;

  case 164:
#line 513 "wmlparse.y"
    { yytknval1 = INTERNALLITERAL; }
    break;

  case 165:
#line 514 "wmlparse.y"
    { yytknval1 = RELATED; }
    break;

  case 166:
#line 515 "wmlparse.y"
    { yytknval1 = DOCNAME; }
    break;

  case 167:
#line 516 "wmlparse.y"
    { yytknval1 = DEFAULT; }
    break;

  case 168:
#line 517 "wmlparse.y"
    { yytknval1 = ALIAS; }
    break;

  case 169:
#line 521 "wmlparse.y"
    { yytknval1 = XRMRESOURCE; }
    break;

  case 170:
#line 530 "wmlparse.y"
    { wmlCreateChild(yynameval, yystringval); }
    break;

  case 171:
#line 539 "wmlparse.y"
    { wmlCreateDatatype (yystringval); }
    break;

  case 176:
#line 547 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 180:
#line 559 "wmlparse.y"
    { wmlAddDatatypeAttribute (yytknval1, yystringval); }
    break;

  case 182:
#line 561 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 184:
#line 565 "wmlparse.y"
    { yytknval1 = INTERNALLITERAL; }
    break;

  case 185:
#line 566 "wmlparse.y"
    { yytknval1 = DOCNAME; }
    break;

  case 186:
#line 567 "wmlparse.y"
    { yytknval1 = XRMRESOURCE; }
    break;

  case 187:
#line 577 "wmlparse.y"
    { wmlCreateOrAppendCtrlList (yynameval); }
    break;

  case 192:
#line 585 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 196:
#line 595 "wmlparse.y"
    { wmlAddCtrlListControl (yynameval); }
    break;

  case 198:
#line 598 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 202:
#line 604 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 204:
#line 616 "wmlparse.y"
    { wmlCreateEnumSet (yynameval, yystringval); }
    break;

  case 210:
#line 629 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 214:
#line 639 "wmlparse.y"
    { wmlAddEnumSetValue (yynameval); }
    break;

  case 216:
#line 641 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 218:
#line 652 "wmlparse.y"
    { wmlCreateEnumValue (yynameval); }
    break;

  case 223:
#line 660 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 227:
#line 672 "wmlparse.y"
    { wmlAddEnumValueAttribute (yytknval1, yystringval); }
    break;

  case 229:
#line 674 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 231:
#line 678 "wmlparse.y"
    { yytknval1 = ENUMLITERAL; }
    break;

  case 232:
#line 688 "wmlparse.y"
    { wmlCreateCharset (yystringval); }
    break;

  case 237:
#line 696 "wmlparse.y"
    { LexIssueError(RBRACE); }
    break;

  case 241:
#line 708 "wmlparse.y"
    { wmlAddCharsetAttribute (yytknval1, yystringval); }
    break;

  case 243:
#line 713 "wmlparse.y"
    { wmlAddCharsetAttribute (yytknval1, yytknval2); }
    break;

  case 245:
#line 715 "wmlparse.y"
    { LexIssueError(SEMICOLON); }
    break;

  case 247:
#line 719 "wmlparse.y"
    { yytknval1 = INTERNALLITERAL; }
    break;

  case 248:
#line 720 "wmlparse.y"
    { yytknval1 = ALIAS; }
    break;

  case 249:
#line 721 "wmlparse.y"
    { yytknval1 = XMSTRINGCHARSETNAME; }
    break;

  case 250:
#line 725 "wmlparse.y"
    { yytknval1 = DIRECTION; }
    break;

  case 251:
#line 726 "wmlparse.y"
    { yytknval1 = PARSEDIRECTION; }
    break;

  case 252:
#line 727 "wmlparse.y"
    { yytknval1 = CHARACTERSIZE; }
    break;

  case 253:
#line 731 "wmlparse.y"
    { yytknval2 = LEFTTORIGHT; }
    break;

  case 254:
#line 732 "wmlparse.y"
    { yytknval2 = RIGHTTOLEFT; }
    break;

  case 255:
#line 733 "wmlparse.y"
    { yytknval2 = ONEBYTE; }
    break;

  case 256:
#line 734 "wmlparse.y"
    { yytknval2 = TWOBYTE; }
    break;

  case 257:
#line 735 "wmlparse.y"
    { yytknval2 = MIXED1_2BYTE; }
    break;

  case 258:
#line 744 "wmlparse.y"
    { strcpy (yynameval, (XmConst char *)yystringval); }
    break;


/* Line 1267 of yacc.c.  */
#line 2353 "wmlparse.c"
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


#line 747 "wmlparse.y"


/*
 * Dummmy error routine, since we output our own.
 */
int yyerror (s)
char *s;
{
	return 0;
}


#include "wmllex.c"


