/*
 * parser.c -- token parsing
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

#include "parser.h"
#include "parser_private.h"

#include <stddef.h>

#include "hashtable.h"


static ht_t *keywords_ht;

static keyword_t keywords[] = {
	/* instructions */
	{"NOP", 4, NULL},  // no operation
	{"LDR", 4, NULL},  // load register
	{"LDH", 4, NULL},  // load high nibble
	{"LDL", 4, NULL},  // load low nibble
	{"MOV", 4, NULL},  // move register
	// TODO: branch instructions
	{"ADD", 4, NULL},  // add
	{"SUB", 4, NULL},  // subtract
	{"AND", 4, NULL},  // and
	{"ORR", 4, NULL},  // inclusive or
	{"EOR", 4, NULL},  // exclusive or
	{"LSL", 4, NULL},  // logical shift left
	{"LSR", 4, NULL},  // logical shift right
};

static char tokenbuf[TOKENSIZ];


int parser_init(void)
{
	keywords_ht = htalloc();
	if (!keywords_ht) return -1;

	for (size_t i = 0; i < sizeof(keywords) / sizeof(keyword_t); i++) {
		if (0 > ht_set(
				keywords_ht,
				keywords[i].key,
				keywords[i].len,
				keywords + i
			)
		)
			goto error;
	}

	return 0;

error:
	htfree(keywords_ht, NULL);
	return -1;
}

void parser_rm(void)
{
	htfree(keywords_ht, NULL);
}
