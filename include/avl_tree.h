/**
 * dsac -- Data Structures and Algorithms for C.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <assert.h>

#define AVL_TREE_INITIALIZER(comparison_function) { 0, comparison_function, NULL }

struct avl_node
{
	struct avl_node * left;
	struct avl_node * right;
	unsigned short height;
	const void * data;
};

struct avl_tree
{
	unsigned int size;
	int (*compare)(const void *, const void *);
	struct avl_node * root;
};

void avl_initialize(struct avl_tree *, int (*)(const void *, const void *));

int avl_insert(struct avl_tree *, struct avl_node *);

struct avl_node * avl_remove(struct avl_tree *, const void *);
struct avl_node * avl_remove_min(struct avl_tree *);
struct avl_node * avl_remove_max(struct avl_tree *);

const struct avl_node * avl_find(const struct avl_tree *, const void *);
const struct avl_node * avl_find_min(const struct avl_tree *);
const struct avl_node * avl_find_max(const struct avl_tree *);

int avl_iterate(const struct avl_tree *, int(*)(const struct avl_node *, void *), void *);

static inline void
avl_set(struct avl_node *n, const void *d)
{
	n->data = d;
}

static inline const void *
avl_get(const struct avl_node *n)
{
	return n->data;
}

static inline unsigned int
avl_size(const struct avl_tree *meta)
{
	assert(meta);
	return meta->size;
}

static inline int
avl_empty(const struct avl_tree *meta)
{
	assert(meta);
	return avl_size(meta) == 0;
}

#ifndef NDEBUG
int avl_verify(const struct avl_tree *);
void avl_print(const struct avl_tree *);
#endif
