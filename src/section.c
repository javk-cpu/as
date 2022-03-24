/*
 * section.c -- section generation
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

#include "section.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


uint8_t *section2bin(const section_t *sec)
{
	uint8_t *bin = malloc(sizeof(uint8_t) * sec->cnt);
	if (!bin) return NULL;

	const instruction_t *instr = sec->instr;
	for (size_t i = 0; i < sec->cnt; i++) {
		bin[i]  = instr->opcode << 4;
		bin[i] |= instr->operand;
		++instr;
	}

	return bin;
}

section_t *sectionalloc(size_t siz)
{
	section_t *tmp = malloc(sizeof(section_t));
	if (!tmp) return NULL;

	tmp->instr = malloc(sizeof(instruction_t) * siz);
	if (!tmp->instr) goto error;

	tmp->cnt = 0;
	tmp->siz = siz;

	return tmp;

error:
	free(tmp);
	return NULL;
}

void sectionfree(section_t *sec)
{
	if (!sec) return;

	free(sec->instr);
	free(sec);
}
