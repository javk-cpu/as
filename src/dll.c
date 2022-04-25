/*
 * dll.c -- doubly linked list
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

#include "dll.h"

#include <stddef.h>
#include <stdlib.h>


size_t dll_append(dll_t *dll, void *data)
{
	dll_node_t *tmp = calloc(1, sizeof(dll_node_t));
	if (!tmp) return 0;

	tmp->data = data;

	if (dll->tail) {
		dll->tail->next = tmp;
		tmp->prev = dll->tail;
		dll->tail = tmp;
	} else {
		dll->tail = dll->head = tmp;
	}

	return ++dll->size;
}

size_t dll_prepend(dll_t *dll, void *data)
{
	dll_node_t *tmp = calloc(1, sizeof(dll_node_t));
	if (!tmp) return 0;

	tmp->data = data;

	if (dll->head) {
		dll->head->prev = tmp;
		tmp->next = dll->head;
		dll->head = tmp;
	} else {
		dll->head = dll->tail = tmp;
	}

	return ++dll->size;
}

dll_t *dllalloc(void)
{
	dll_t *tmp = calloc(1, sizeof(dll_t));
	if (!tmp) return NULL;

	return tmp;
}

void dllfree(dll_t *dll, void (*free_data)(void *ptr))
{
	if (!dll) return;

	dll_node_t *tmp;
	dll_node_t *head = dll->head;
	if (free_data) {
		while (head) {
			tmp  = head;
			head = head->next;

			if (tmp->data) free_data(tmp->data);
			free(tmp);
		}
	} else {
		while (head) {
			tmp  = head;
			head = head->next;

			free(tmp);
		}
	}

	free(dll);
}
