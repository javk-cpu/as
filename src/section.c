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


uint8_t *section2bin(const section_t *section)
{
	uint8_t *bin = malloc(sizeof(uint8_t) * section->section_siz);
	if (!bin) return NULL;

	const instruction_t *instruction = section->instruction;
	for (size_t i = 0; i < section->section_siz; i++) {
		bin[i]  = instruction->opcode << 4;
		bin[i] |= instruction->operand;
		++instruction;
	}

	return bin;
}

section_t *sectionalloc(size_t instructions)
{
	section_t *tmp = malloc(sizeof(section_t));
	if (!tmp) return NULL;

	tmp->instruction = malloc(sizeof(instruction_t) * instructions);
	if (!tmp->instruction) goto error;

	tmp->section_siz = instructions;

	return tmp;

error:
	free(tmp);
	return NULL;
}
