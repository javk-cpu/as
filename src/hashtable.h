/*
 * hashtable.h -- hash table implementation
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

#ifndef JAVK_AS_HASHTABLE
#define JAVK_AS_HASHTABLE


#include <stddef.h>


#define HT_DEFAULT_CAP 512


typedef struct ht_ent_s {
	const void *key;
	size_t      key_len;
	void       *val;
} ht_ent_t;

typedef struct ht_s {
	ht_ent_t *ent;
	size_t    cap;
	size_t    len;
} ht_t;


#endif /* JAVK_AS_HASHTABLE */
