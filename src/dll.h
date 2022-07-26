/*
 * dll.h -- doubly linked list
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

#ifndef JAVK_AS_DLL
#define JAVK_AS_DLL


#include <stddef.h>


typedef struct dll_node_s {
	struct dll_node_s *prev;
	struct dll_node_s *next;
	void              *data;
} dll_node_t;

typedef struct dll_s {
	dll_node_t *head;
	dll_node_t *tail;
	size_t      size;
} dll_t;


dll_t  *dll_alloc(void);
size_t  dll_append(dll_t *dll, void *data);
void    dll_free(dll_t *dll, void (*free_data)(void *ptr));
size_t  dll_prepend(dll_t *dll, void *data);


#endif /* JAVK_AS_DLL */
