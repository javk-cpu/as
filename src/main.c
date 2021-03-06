/*
 * javk-as
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

#include <stdlib.h>

#include "asm/parser.h"


static void cleanexit(void)
{
	parser_rm();
}

int main(void)
{
	static int ret;

	ret = atexit(cleanexit);
	if (ret < 0) goto error;

	ret = parser_init();
	if (ret < 0) goto error;

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}
