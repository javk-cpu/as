/*
 * section.h -- section generation
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

#ifndef JAVK_AS_SECTION
#define JAVK_AS_SECTION


#include <stddef.h>
#include <stdint.h>


enum opcodes {
	NOP,  // no operation
	LDR,  // load register
	LDH,  // load high nibble
	LDL,  // load low nibble
	MOV,  // move register
	// TODO: branch instructions
	ADD,  // add
	SUB,  // subtract
	AND,  // and
	ORR,  // inclusive or
	EOR,  // exclusive or
	LSL,  // logical shift left
	LSR,  // logical shift right
};

typedef struct instruction_s {
	unsigned opcode  : 4;
	unsigned operand : 4;
} instruction_t;

typedef struct section_s {
	instruction_t *instruction;
	size_t         section_siz;
} section_t;


uint8_t   *section2bin(const section_t *section);
section_t *sectionalloc(size_t instructions);
void       sectionfree(section_t *section);


#endif /* JAVK_AS_SECTION */