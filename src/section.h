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
	JPC,  // jump (conditional)
	JPU,  // jump (unconditional)
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


uint8_t   *section2bin(const section_t *sec);
section_t *sectionalloc(size_t siz);
void       sectionfree(section_t *sec);
int        sectionrealloc(section_t *sec, size_t siz);


#endif /* JAVK_AS_SECTION */
