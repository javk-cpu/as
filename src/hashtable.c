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

#include <stdbool.h>
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

int ht_set(ht_t *ht, const void *key, size_t len, void *val)
{
	if (ht->cnt >= (ht->cap / 2) && !rehash(ht)) return -1;

	if (ht_set_private(ht, (void*) key, len, val, true)) return -1;

	return 0;
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

static int ht_set_private(ht_t *ht, void *key, size_t len, void *val, bool cpy)
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

	if (cpy) {
		void *tmp = malloc(len);
		if (!tmp) return -1;

		memcpy(tmp, key, len);
		key = tmp;
	}

	ht->ent[i].key     = key;
	ht->ent[i].key_len = len;
	ht->ent[i].val     = val;
	++ht->cnt;

	return 0;
}

static int rehash(ht_t *ht)
{
	ht_ent_t *oldent = ht->ent;
	size_t    oldcap = ht->cap;

	ht->cap *= 2;
	if (oldcap > ht->cap) goto error;

	ht->ent = calloc(ht->cap, sizeof(ht_ent_t));
	if (!ht->ent) goto error;

	ht->cnt = 0;

	ht_ent_t *ent = oldent;
	for (size_t i = 0; i < oldcap; i++) {
		if (ent->key)
			ht_set_private(
				ht,
				ent->key,
				ent->key_len,
				ent->val,
				false
			);

		++ent;
	}

	free(oldent);

	return 0;

error:
	ht->ent = oldent;
	ht->cap = oldcap;

	return -1;
}
