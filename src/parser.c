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


static char *opcode_str[] = {
	"NOP",  // no operation
	"LDR",  // load register
	"LDH",  // load high nibble
	"LDL",  // load low nibble
	"MOV",  // move register
	// TODO: branch instructions
	"ADD",  // add
	"SUB",  // subtract
	"AND",  // and
	"ORR",  // inclusive or
	"EOR",  // exclusive or
	"LSL",  // logical shift left
	"LSR",  // logical shift right
};
