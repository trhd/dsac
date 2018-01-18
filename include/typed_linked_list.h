/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2016-2018 Hemmo Nieminen
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linked_list.h"

#ifndef TYPED_LINKED_LIST_CONFIG_TYPE
# error TYPED_LINKED_LIST_CONFIG_TYPE has not been defined.
#endif

#ifndef TYPED_LINKED_LIST_CONFIG_PREFIX
# error TYPED_LINKED_LIST_CONFIG_PREFIX has not been defined.
#endif

#define _CONCAT(a, b) a##b
#define _PREFIX(a, b) _CONCAT(a, b)
#define PREFIX(identifier) _PREFIX(TYPED_LINKED_LIST_CONFIG_PREFIX, identifier)

typedef struct linked_list_meta PREFIX(_meta_t);
typedef struct linked_list PREFIX(_t);

static inline void
PREFIX(_initialize)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	linked_list_initialize(meta);
}

static inline void
PREFIX(_set)(PREFIX(_t) * entry, TYPED_LINKED_LIST_CONFIG_TYPE const * data)
{
	assert(entry);

	linked_list_set(entry, data);
}

static inline TYPED_LINKED_LIST_CONFIG_TYPE const *
PREFIX(_get)(PREFIX(_t) const * from)
{
	assert(from);

	return (TYPED_LINKED_LIST_CONFIG_TYPE const *)linked_list_get(from);
}

static inline PREFIX(_t) **
PREFIX(_next)(PREFIX(_t) const * from)
{
	assert(from);

	return linked_list_next(from);
}

static inline void
PREFIX(_insert_front)(PREFIX(_meta_t) * meta, PREFIX(_t) * entry)
{
	assert(meta);
	assert(entry);

	linked_list_insert_front(meta, entry);
}

static inline void
PREFIX(_insert_back)(PREFIX(_meta_t) * meta, PREFIX(_t) * entry)
{
	assert(meta);
	assert(entry);

	linked_list_insert_back(meta, entry);
}

static inline PREFIX(_t) *
PREFIX(_detach)(PREFIX(_meta_t) * meta, PREFIX(_t) ** entry)
{
	assert(meta);
	assert(entry);
	assert(*entry);

	return linked_list_detach(meta, entry);
}

static inline PREFIX(_t) *
PREFIX(_remove)(PREFIX(_meta_t) * meta, TYPED_LINKED_LIST_CONFIG_TYPE const * p)
{
	assert(meta);

	return linked_list_remove(meta, p);
}

static inline PREFIX(_t) *
PREFIX(_remove_head)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_remove_head(meta);
}

static inline PREFIX(_t) *
PREFIX(_remove_tail)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_remove_tail(meta);
}

static inline PREFIX(_t) *
PREFIX(_remove_match)(PREFIX(_meta_t) * meta,
		TYPED_LINKED_LIST_CONFIG_TYPE const * match,
		int (*matcher)(TYPED_LINKED_LIST_CONFIG_TYPE const *, TYPED_LINKED_LIST_CONFIG_TYPE const *))
{
	assert(meta);
	assert(matcher);

	return linked_list_remove_match(meta, match, (int(*)(void const *, void const *))matcher);
}

static inline PREFIX(_t) **
PREFIX(_find)(PREFIX(_meta_t) * meta, TYPED_LINKED_LIST_CONFIG_TYPE const * data)
{
	assert(meta);

	return linked_list_find(meta, data);
}

static inline PREFIX(_t) **
PREFIX(_find_head)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_find_head(meta);
}

static inline PREFIX(_t) **
PREFIX(_find_tail)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_find_tail(meta);
}

static inline PREFIX(_t) **
PREFIX(_find_match)(PREFIX(_meta_t) * meta, TYPED_LINKED_LIST_CONFIG_TYPE const * match,
		int(*matcher)(TYPED_LINKED_LIST_CONFIG_TYPE const *, TYPED_LINKED_LIST_CONFIG_TYPE const *))
{
	assert(meta);
	assert(matcher);

	return linked_list_find_match(meta, match, (int(*)(void const *, void const *))matcher);
}

static inline int
PREFIX(_iterate)(PREFIX(_meta_t) const * meta,
		int (*cb)(PREFIX(_t) const * node, void * argument),
		void *data)
{
	assert(meta);
	assert(cb);

	return linked_list_iterate(meta, (int(*)(struct linked_list const * t, void *))cb, data);
}

static inline int
PREFIX(_empty)(const PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_empty(meta);
}

static inline unsigned int
PREFIX(_size)(const PREFIX(_meta_t) * meta)
{
	assert(meta);

	return linked_list_size(meta);
}

#undef _CONCAT
#undef _PREFIX
#undef PREFIX

#undef TYPED_LINKED_LIST_CONFIG_PREFIX
#undef TYPED_LINKED_LIST_CONFIG_TYPE
