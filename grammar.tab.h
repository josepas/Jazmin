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
    NL = 261,
    BOOL = 262,
    BORN = 263,
    BREAK = 264,
    CHAR = 265,
    CONFEDERATION = 266,
    ELIF = 267,
    ELSE = 268,
    JFALSE = 269,
    FLOAT = 270,
    FOR = 271,
    FUNC = 272,
    HOLLOW = 273,
    IF = 274,
    INT = 275,
    NEXT = 276,
    JNULL = 277,
    PROC = 278,
    PUFF = 279,
    READ = 280,
    REF = 281,
    RETURN = 282,
    STEP = 283,
    STRUCT = 284,
    TO = 285,
    JTRUE = 286,
    WHILE = 287,
    WRITE = 288,
    LTOE = 289,
    GTOE = 290,
    EQUAL = 291,
    UNEQUAL = 292,
    PLUS_ASSIGN = 293,
    MINUS_ASSIGN = 294,
    MULT_ASSIGN = 295,
    DIV_ASSIGN = 296,
    AND = 297,
    OR = 298,
    ARROW = 299
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

#line 105 "grammar.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
