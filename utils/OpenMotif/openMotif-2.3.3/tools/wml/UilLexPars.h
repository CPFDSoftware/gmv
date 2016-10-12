/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

