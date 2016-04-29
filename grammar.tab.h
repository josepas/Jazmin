/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    REAL = 259,
    ID = 260,
    STRING = 261,
    CHARACTER = 262,
    NL = 263,
    BOOL = 264,
    CHAR = 265,
    FLOAT = 266,
    INT = 267,
    HOLLOW = 268,
    CONFEDERATION = 269,
    STRUCT = 270,
    IF = 271,
    ELIF = 272,
    ELSE = 273,
    JFALSE = 274,
    JTRUE = 275,
    JNULL = 276,
    FOR = 277,
    WHILE = 278,
    TO = 279,
    STEP = 280,
    BREAK = 281,
    NEXT = 282,
    FUNC = 283,
    PROC = 284,
    REF = 285,
    RETURN = 286,
    BORN = 287,
    PUFF = 288,
    READ = 289,
    WRITE = 290,
    LTOE = 291,
    GTOE = 292,
    EQUAL = 293,
    UNEQUAL = 294,
    AND = 295,
    OR = 296,
    PLUS_ASSIGN = 297,
    MINUS_ASSIGN = 298,
    MULT_ASSIGN = 299,
    DIV_ASSIGN = 300,
    ARROW = 301
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 8 "grammar.y" /* yacc.c:1909  */

	int ival;
	float fval;
	char *idval;
	char *str;
	char c[2];

#line 109 "grammar.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
