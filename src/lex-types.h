/* lex-types.h -- this file is part of C Ratfor.
 * Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LEX_TYPES_H
#  define LEX_TYPES_H 1

/*
 *  L E X E R  S Y M B O L S
 *
 */

/*
 * WARNING: the values of all these macros must be smaller than the
 * numerical value of any ascii character that might be used in the
 * input.
 */
#define LEXBREAK        10
#define LEXDO           11
#define LEXELSE         12
#define LEXFOR          13
#define LEXIF           14
#define LEXNEXT         15
#define LEXREPEAT       16
#define LEXUNTIL        17
#define LEXWHILE        18
#define LEXRETURN       19
#define LEXEND          20
#define LEXSTOP         21
#define LEXSWITCH       22
#define LEXCASE         23
#define LEXDEFAULT      24
#define LEXVERBATIM     25

#endif
/* vim: set ft=c ts=4 sw=4 et : */
