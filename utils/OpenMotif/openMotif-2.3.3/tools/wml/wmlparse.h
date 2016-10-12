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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

