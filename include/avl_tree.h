/**
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This file is part of dsac (Data Structures and Alorithms for C).
 *
 * dsac is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * dsac is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsac. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <assert.h>
#include "debug_flags.h"

#if defined(NDEBUG) && !defined(UNIT_TESTING)
# define AVL_TREE_INITIALIZER(comparison_function) \
	{ 0, comparison_function, NULL }
#else
# define AVL_TREE_INITIALIZER(comparison_function) \
	{ 0, comparison_function, NULL, {1 << AVL_TREE_DEBUG_FLAG_INITIALIZED} }
#endif

DEBUG_FLAGS_ENUM(avl_node_debug_flag,
		AVL_NODE_DEBUG_FLAG_SET,
		_AVL_NODE_DEBUG_FLAG_COUNT);

struct avl_node
{
	struct avl_node * left;
	struct avl_node * right;
	unsigned short height;
	void const * data;
	DEBUG_FLAGS(_AVL_NODE_DEBUG_FLAG_COUNT);
};

DEBUG_FLAGS_ENUM(avl_tree_debug_flag,
		AVL_TREE_DEBUG_FLAG_INITIALIZED,
		_AVL_TREE_DEBUG_FLAG_COUNT);

struct avl_tree
{
	unsigned int size;
	int (*compare)(void const *, void const *);
	struct avl_node * root;
	DEBUG_FLAGS(_AVL_TREE_DEBUG_FLAG_COUNT);
};

void
avl_initialize(struct avl_tree *, int (*)(void const *, void const *));

int
avl_insert(struct avl_tree *, struct avl_node *);

struct avl_node *
avl_remove(struct avl_tree *, void const *);

struct avl_node *
avl_remove_min(struct avl_tree *);

struct avl_node *
avl_remove_max(struct avl_tree *);

struct avl_node const *
avl_find(struct avl_tree const *, void const *);

struct avl_node const *
avl_find_min(struct avl_tree const *);

struct avl_node const *
avl_find_max(struct avl_tree const *);

int
avl_iterate(struct avl_tree const *, int (*)(struct avl_node const *, void *), void *);

static inline void
avl_set(struct avl_node * n, void const * d)
{
	assert(n);

	debug_flags_initialize(n);
	debug_flags_set(n, AVL_NODE_DEBUG_FLAG_SET);
	n->data = d;
}

static inline void const *
avl_get(struct avl_node const * n)
{
	assert(n);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	return n->data;
}

static inline unsigned int
avl_size(struct avl_tree const * meta)
{
	assert(meta);
	debug_flags_assert(meta, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	return meta->size;
}

static inline int
avl_empty(struct avl_tree const * meta)
{
	assert(meta);
	debug_flags_assert(meta, AVL_TREE_DEBUG_FLAG_INITIALIZED);
	return avl_size(meta) == 0;
}

#if !defined(NDEBUG) || defined(UNIT_TESTING)

int
avl_verify(struct avl_tree const *);

void
avl_print(struct avl_tree const *);

#endif
