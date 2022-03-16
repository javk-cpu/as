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
#include <string.h>


void *ht_get(const ht_t *ht, const void *key, size_t len)
{
	if (!key || !len) return NULL;

	size_t i = (size_t) (fnv1a_hash(key, len) & (uint64_t) (ht->cap - 1));

	while (ht->ent[i].key) {
		if (i >= ht->cap) i = 0;

		if (len != ht->ent[i].key_len) {
			++i;
			continue;
		}

		if (!memcmp(key, ht->ent[i].key, len))
			return ht->ent[i].val;

		++i;
	}

	return NULL;
}

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

void htfree(ht_t *ht, void (*free_val)(void *ptr))
{
	if (!ht) return;

	ht_ent_t *ent = ht->ent;
	if (free_val) {
		while (ht->cap) {
			if (ent->key) {
				free(ent->key);
				if (ent->val) free_val(ent->val);
			}
			--ht->cap;
			++ent;
		}
	} else {
		while (ht->cap) {
			if (ent->key) free(ent->key);
			--ht->cap;
			++ent;
		}
	}

	free(ht->ent);
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

static int ht_set_private(ht_t *ht, const void *key, size_t len, void *val)
{
	if (!key || !len) return -1;

	size_t i = (size_t) (fnv1a_hash(key, len) & (uint64_t) (ht->cap - 1));

	while (ht->ent[i].key) {
		if (i >= ht->cap) i = 0;

		if (len != ht->ent[i].key_len) {
			++i;
			continue;
		}

		if (!memcmp(key, ht->ent[i].key, len)) {
			ht->ent[i].val = val;
			++ht->cnt;
			return 0;
		}

		++i;
	}

	ht->ent[i].key     = key;
	ht->ent[i].key_len = len;
	ht->ent[i].val     = val;
	++ht->cnt;

	return 0;
}
