/*
 * parser_private.h -- token parsing
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef JAVK_AS_PARSER_PRIVATE
#define JAVK_AS_PARSER_PRIVATE


#include "parser.h"

#include <stddef.h>

#include "section.h"


#define TOKENSIZ 4


typedef struct keyword_s {
	char  *key;
	size_t len;
	int   (*parser)(section_t *sec, char **tokens);
} keyword_t;

typedef struct register_s {
	char     *key;
	size_t    len;
	unsigned  val;
} register_t;


static int parser_nop(section_t *sec, char **tokens);


#endif /* JAVK_AS_PARSER_PRIVATE */
