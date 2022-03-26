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

#include <ctype.h>
#include <stddef.h>

#include "hashtable.h"


static ht_t *keywords_ht;
static ht_t *registers_ht;

static keyword_t keywords[] = {
	/* instructions */
	{"NOP", 4, parser_nop},  // no operation
	{"LDR", 4, NULL},        // load register
	{"LDH", 4, NULL},        // load high nibble
	{"LDL", 4, NULL},        // load low nibble
	{"MOV", 4, NULL},        // move register
	// TODO: branch instructions
	{"ADD", 4, parser_add},  // add
	{"SUB", 4, parser_sub},  // subtract
	{"AND", 4, parser_and},  // and
	{"ORR", 4, NULL},        // inclusive or
	{"EOR", 4, NULL},        // exclusive or
	{"LSL", 4, NULL},        // logical shift left
	{"LSR", 4, NULL},        // logical shift right
};

static register_t registers[] = {
	{"A",  2, A},   // accumulator
	{"B",  2, B},   // b register
	{"C",  2, C},   // c register
	// TODO: finalize registers
	{"SP", 3, SP},  // stack pointer
	{"PC", 3, PC},  // program counter
};

static char tokenbuf[TOKENSIZ];


int parser_init(void)
{
	keywords_ht = htalloc();
	if (!keywords_ht) return -1;
	registers_ht = htalloc();
	if (!registers_ht) goto error;

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

	for (size_t i = 0; i < sizeof(registers) / sizeof(register_t); i++) {
		if (0 > ht_set(
				registers_ht,
				registers[i].key,
				registers[i].len,
				registers + i
			)
		)
			goto error;
	}

	return 0;

error:
	htfree(keywords_ht,  NULL);
	htfree(registers_ht, NULL);
	return -1;
}

void parser_rm(void)
{
	htfree(keywords_ht,  NULL);
	htfree(registers_ht, NULL);
}


static int parser_nop(section_t *sec, char **tokens)
{
	(void) tokens;

	if (sec->cnt + 1 > sec->siz)
		if (sectionrealloc(sec, sec->siz * 2) < 0)
			return -1;

	sec->instr[sec->cnt].opcode  = NOP;
	sec->instr[sec->cnt].operand = 0;

	++sec->cnt;

	return 0;
}

static int parser_add(section_t *sec, char **tokens)
{
	return parser_arithmetic(sec, tokens, ADD);
}

static int parser_sub(section_t *sec, char **tokens)
{
	return parser_arithmetic(sec, tokens, SUB);
}

static int parser_and(section_t *sec, char **tokens)
{
	return parser_arithmetic(sec, tokens, AND);
}


static int parser_arithmetic(section_t *sec, char **tokens, unsigned opcode)
{
	char   *tmp = tokens[1];
	size_t  i   = 0;

	do {
		tokenbuf[i] = toupper(tmp[i]);
		++i;
	} while (tmp[i - 1]);

	register_t *reg = ht_get(registers_ht, tokenbuf, i);
	if (!reg) return -1;

	// only 8-bit registers can be used
	register_t *bad = registers + (sizeof(registers) / sizeof(register_t));
	if (reg == bad - 1) return -1;
	if (reg == bad - 2) return -1;

	if (sec->cnt + 1 > sec->siz)
		if (sectionrealloc(sec, sec->siz * 2) < 0)
			return -1;

	sec->instr[sec->cnt].opcode  = opcode;
	sec->instr[sec->cnt].operand = reg->val;

	++sec->cnt;

	return 0;
}
