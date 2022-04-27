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
#include <stdlib.h>
#include <string.h>

#include "dll.h"
#include "hashtable.h"
#include "section.h"


static dll_t *labels_dll;

static ht_t *keywords_ht;
static ht_t *labels_ht;
static ht_t *registers_ht;

static keyword_t keywords[] = {
	/* instructions */
	{"ADD", 4, parser_add},  // add
	{"SUB", 4, parser_sub},  // subtract
	{"NEG", 4, parser_neg},  // negate
	{"AND", 4, parser_and},  // and
	{"ORR", 4, parser_orr},  // inclusive or
	{"EOR", 4, parser_eor},  // exclusive or
	{"LSL", 4, parser_lsl},  // logical shift left
	{"LSR", 4, parser_lsr},  // logical shift right
	// TODO: branch instructions
	{"LDR", 4, NULL},        // load register
	{"LDN", 4, NULL},        // load nibble
	{"MOV", 4, NULL},        // move register

	/* mnemonics */
	{"LDA", 4, parser_lda},  // load accumulator
	{"NOP", 4, parser_nop},  // no operation
};

static register_t registers[] = {
	{"A",  2, A},   // accumulator
	{"B",  2, B},   // b register
	{"C",  2, C},   // c register
	{"ZR", 3, ZR},  // zero register
	// TODO: finalize registers
	{"SP", 3, SP},  // stack pointer
	{"PC", 3, PC},  // program counter
};

static char tokenbuf[TOKENSIZ];


int parse_section(const char *name, const char ***keys)
{
	int ret;

	if (!keys) return -1;

	label_t *label = labelalloc(name);
	if (!label) return -1;

	ret = dll_append(labels_dll, label);
	if (ret < 0) goto dll_append_error;

	ret = ht_set(labels_ht, label->key, label->len, label);
	if (ret < 0) goto error;

	keyword_t  *keyword;
	const char *key;
	size_t      len;
	while (*keys) {
		key = **keys;

		len = strlen(key) + 1;
		if (len > TOKENSIZ) goto error;

		int i = 0;
		do {
			tokenbuf[i] = toupper(key[i]);
			++i;
		} while (key[i - 1]);
		key = tokenbuf;

		keyword = ht_get(keywords_ht, key, len);
		if (!keyword) goto error;

		ret = keyword->parser(label->sec, *keys);
		if (ret < 0) goto error;

		++keys;
	}

	return 0;

dll_append_error:
	// NOTE: if label was successfully added to labels_dll,
	// we risk a double free condition, parser_rm() will
	// cleanup after us if we fail after dll_append()
	labelfree(label);

error:
	return -1;
}

int parser_init(void)
{
	labels_dll = dllalloc();
	if (!labels_dll) return -1;

	keywords_ht = htalloc();
	if (!keywords_ht) goto error;
	labels_ht = htalloc();
	if (!labels_ht) goto error;
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
	dllfree(labels_dll, NULL);

	htfree(keywords_ht,  NULL);
	htfree(labels_ht,    NULL);
	htfree(registers_ht, NULL);

	return -1;
}

void parser_rm(void)
{
	dllfree(labels_dll, labelfree);

	htfree(keywords_ht,  NULL);
	htfree(labels_ht,    NULL);
	htfree(registers_ht, NULL);
}


static label_t *labelalloc(const char *key)
{
	label_t *tmp = calloc(1, sizeof(label_t));
	if (!tmp) return NULL;

	tmp->len = strlen(key) + 1;

	tmp->key = malloc(tmp->len);
	if (!tmp->key) goto error;
	strncpy(tmp->key, key, tmp->len);

	tmp->sec = sectionalloc(SECSIZ);
	if (!tmp->sec) goto error;

	return tmp;

error:
	labelfree(tmp);
	return NULL;
}

static void labelfree(void *label)
{
	label_t *tmp = label;

	if (!tmp) return;

	if (tmp->key) free(tmp->key);
	if (tmp->sec) sectionfree(tmp->sec);

	free(label);
}


static int parser_add(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, ADD);
}

static int parser_sub(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, SUB);
}

static int parser_neg(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, NEG);
}

static int parser_and(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, AND);
}

static int parser_orr(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, ORR);
}

static int parser_eor(section_t *sec, const char **tokens)
{
	return parser_arithmetic(sec, tokens, EOR);
}

static int parser_lsl(section_t *sec, const char **tokens)
{
	return parser_shift(sec, tokens, LSL);
}

static int parser_lsr(section_t *sec, const char **tokens)
{
	return parser_shift(sec, tokens, LSR);
}


static int parser_arithmetic(section_t *sec, const char **tokens, unsigned opcode)
{
	const char *tmp = tokens[1];
	size_t      i   = 0;

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

static int parser_shift(section_t *sec, const char **tokens, unsigned opcode)
{
	unsigned long shamt = strtoul(tokens[1], NULL, 0);

	if (sec->cnt + 1 > sec->siz)
		if (sectionrealloc(sec, sec->siz * 2) < 0)
			return -1;

	sec->instr[sec->cnt].opcode  = opcode;
	sec->instr[sec->cnt].operand = shamt;

	++sec->cnt;

	return 0;
}


static int parser_lda(section_t *sec, const char **tokens)
{
	int         ret;
	const char *zr_clr_tokens[2];

	// clear the accumulator first
	zr_clr_tokens[0] = tokens[0];
	zr_clr_tokens[0] = "ZR";
	ret = parser_and(sec, zr_clr_tokens);
	if (ret < 0) return -1;

	ret = parser_orr(sec, tokens);
	if (ret < 0) goto error;

	return 0;

error:
	--sec->cnt;
	return -1;
}

static int parser_nop(section_t *sec, const char **tokens)
{
	const char *zr_orr_tokens[2];

	zr_orr_tokens[0] = tokens[0];
	zr_orr_tokens[1] = "ZR";

	// do nothing by preserving the accumulator
	return parser_orr(sec, zr_orr_tokens);
}
