/*
 * hashtable.c -- hash table implementation
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

#include "hashtable.h"
#include "hashtable_private.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


ht_t *htalloc(void)
{
	ht_t *tmp = malloc(sizeof(ht_t));
	if (!tmp) return NULL;

	tmp->ent = calloc(HT_DEFAULT_CAP, sizeof(ht_ent_t));
	if (!tmp->ent) goto error;

	tmp->cap = HT_DEFAULT_CAP;
	tmp->cnt = 0;

	return tmp;

error:
	free(tmp);
	return NULL;
}

void htfree(ht_t *ht)
{
	if (!ht) return;

	ht_ent_t *ent = ht->ent;
	while (ht->cap) {
		if (ent->key) free(ent->key);
		--ht->cap;
		++ent;
	}

	free(ht);
}


static uint64_t fnv1a_hash(const void *key, size_t len)
{
	uint64_t hash = FNV_OFFSET_BASIS;

	const unsigned char *byte = key;
	for (size_t i = 0; i < len; i++, byte++) {
		hash ^= *byte;
		hash *= FNV_PRIME;
	}

	return hash;
}
