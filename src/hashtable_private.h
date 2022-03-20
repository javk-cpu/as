/*
 * hashtable_private.h -- hash table implementation
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

#ifndef JAVK_AS_HASHTABLE_PRIVATE
#define JAVK_AS_HASHTABLE_PRIVATE


#include "hashtable.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define FNV_OFFSET_BASIS 0xcbf29ce484222325UL
#define FNV_PRIME        0x100000001b3UL


static uint64_t fnv1a_hash(const void *key, size_t len);
static int      ht_set_private(ht_t *ht, void *key, size_t len, void *val, bool cpy);
static int      rehash(ht_t *ht);


#endif /* JAVK_AS_HASHTABLE_PRIVATE */