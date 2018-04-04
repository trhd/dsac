/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2018 Hemmo Nieminen
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

#include <assert.h>
#include "splay_tree.h"

#ifndef TYPED_SPLAY_TREE_CONFIG_TYPE
# error TYPED_SPLAY_TREE_CONFIG_TYPE has not been defined.
#endif

#ifndef TYPED_SPLAY_TREE_CONFIG_PREFIX
# error TYPED_SPLAY_TREE_CONFIG_PREFIX has not been defined.
#endif

#define _CONCAT(a, b) a##b
#define _PREFIX(a, b) _CONCAT(a, b)
#define PREFIX(identifier) _PREFIX(TYPED_SPLAY_TREE_CONFIG_PREFIX, identifier)

typedef struct splay_tree PREFIX(_meta_t);
typedef struct splay_node PREFIX(_t);

static inline void
PREFIX(_initialize)(PREFIX(_meta_t) * meta,
		int (*comparison_function) (TYPED_SPLAY_TREE_CONFIG_TYPE const * a, TYPED_SPLAY_TREE_CONFIG_TYPE const * b))
{
	assert(meta);
	assert(comparison_function);

	splay_initialize(meta, (int(*)(void const *, void const *))comparison_function);
}

static inline void
PREFIX(_set)(PREFIX(_t) * node, TYPED_SPLAY_TREE_CONFIG_TYPE * value)
{
	assert(node);

	splay_set(node, value);
}

static inline TYPED_SPLAY_TREE_CONFIG_TYPE const *
PREFIX(_get)(PREFIX(_t) * node)
{
	assert(node);

	return splay_get(node);
}

static inline bool
PREFIX(_insert)(PREFIX(_meta_t) * meta, PREFIX(_t) * node)
{
	assert(meta);
	assert(node);

	return splay_insert(meta, node);
}

static inline PREFIX(_t) *
PREFIX(_remove)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_remove(meta, key);
}

static inline PREFIX(_t) *
PREFIX(_remove_any)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_remove_any(meta);
}

static inline PREFIX(_t) *
PREFIX(_remove_min)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_remove_min(meta);
}

static inline PREFIX(_t) *
PREFIX(_remove_max)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_remove_max(meta);
}

static inline PREFIX(_t) *
PREFIX(_remove_next)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_remove_next(meta, key);
}

static inline PREFIX(_t) *
PREFIX(_remove_prev)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_remove_prev(meta, key);
}

static inline PREFIX(_t) const *
PREFIX(_find)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_find(meta, key);
}

static inline PREFIX(_t) const *
PREFIX(_find_any)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_find_any(meta);
}

static inline PREFIX(_t) const *
PREFIX(_find_min)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_find_min(meta);
}

static inline PREFIX(_t) const *
PREFIX(_find_max)(PREFIX(_meta_t) * meta)
{
	assert(meta);

	return splay_find_max(meta);
}

static inline PREFIX(_t) const *
PREFIX(_find_next)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_find_next(meta, key);
}

static inline PREFIX(_t) const *
PREFIX(_find_prev)(PREFIX(_meta_t) * meta, TYPED_SPLAY_TREE_CONFIG_TYPE const * key)
{
	assert(meta);
	assert(key);

	return splay_find_prev(meta, key);
}

static inline size_t
PREFIX(_size)(PREFIX(_meta_t) const * meta)
{
	assert(meta);

	return splay_size(meta);
}

static inline bool
PREFIX(_is_empty)(PREFIX(_meta_t) const * meta)
{
	assert(meta);

	return splay_is_empty(meta);
}

static inline int
PREFIX(_iterate)(PREFIX(_meta_t) const * meta, int (*callback_function)(PREFIX(_t) const *, void *), void * user_data)
{
	assert(meta);
	assert(callback_function);

	return splay_iterate(meta, callback_function, user_data);
}

#undef _CONCAT
#undef _PREFIX
#undef PREFIX

#undef TYPED_SPLAY_TREE_CONFIG_PREFIX
#undef TYPED_SPLAY_TREE_CONFIG_TYPE
