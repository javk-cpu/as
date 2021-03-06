/*
 * section.h -- section generation
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2022  Ani Vardanyan <ani.var.2003@gmail.com>
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

#ifndef JAVK_AS_ASM_SECTION
#define JAVK_AS_ASM_SECTION


#include <stddef.h>
#include <stdint.h>


enum opcodes {
	ADD,  // add
	SUB,  // subtract
	NEG,  // negate
	AND,  // and
	ORR,  // inclusive or
	EOR,  // exclusive or
	LSL,  // logical shift left
	LSR,  // logical shift right
	MVA,  // move 'a' register
	MVB,  // move 16-bit register
	LNL,  // load nibble low
	LNH,  // load nibble high
	LDB,  // load byte
	STB,  // store byte
	JMP,  // jump
	JPL,  // jump (with link)
};

enum registers_8bit {
	A,   // accumulator
	B,   // b register
	C,   // c register
	D,   // d register
	E,   // e register
	F,   // f register
	G,   // g register
	H,   // h register
	I,   // i register
	J,   // j register
	K,   // k register
	L,   // l register
	M,   // m register
	N,   // n register
	O,   // o register
	Z,   // zero register
};

enum registers_16bit {
	PC,  // program counter
	SP,  // stack pointer
	IJ,  // intended jump
	KL,  // kl register
};

typedef struct instruction_s {
	unsigned opcode  : 4;
	unsigned operand : 4;
} instruction_t;

typedef struct section_s {
	instruction_t *instr;
	size_t         cnt;
	size_t         siz;
} section_t;


section_t *section_alloc(size_t siz);
void       section_free(section_t *sec);
int        section_realloc(section_t *sec, size_t siz);
uint8_t   *section_to_bin(const section_t *sec);


#endif /* JAVK_AS_ASM_SECTION */
