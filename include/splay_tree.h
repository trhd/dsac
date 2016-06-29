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
#include <stdbool.h>
#include <sys/types.h>

struct splay_node
{
	const void *data;
	struct splay_node *left, *right;
};

struct splay_tree
{
	size_t size;
	int (*compare)(const void *, const void *);
	struct splay_node *root;
};

void splay_initialize(struct splay_tree *tree,
		int (*comparison_function)(const void *, const void *key));

int splay_insert(struct splay_tree *tree, struct splay_node *node);

struct splay_node * splay_remove(struct splay_tree *, const void *);

struct splay_node * splay_remove_any(struct splay_tree *tree);

struct splay_node * splay_remove_min(struct splay_tree *tree);
struct splay_node * splay_remove_max(struct splay_tree *tree);

struct splay_node * splay_remove_next(struct splay_tree *tree, const void *key);
struct splay_node * splay_remove_prev(struct splay_tree *tree, const void *key);

const struct splay_node * splay_find(struct splay_tree *tree, const void *key);

const struct splay_node * splay_find_min(struct splay_tree *tree);
const struct splay_node * splay_find_max(struct splay_tree *tree);

const struct splay_node * splay_find_next(struct splay_tree *tree, const void *key);
const struct splay_node * splay_find_prev(struct splay_tree *tree, const void *key);

int splay_iterate(const struct splay_tree *tree,
		int (*callback_function)(const struct splay_node *, void *),
		void *argument_pointer);

static inline void
splay_set(struct splay_node *node, const void *key)
{
	assert(node);
	/** key could be null/zero if pretending pointers to be number */
	node->data = key;
}

static inline const void *
splay_get(const struct splay_node *node)
{
	assert(node);
	return node->data;
}

static inline size_t
splay_size(const struct splay_tree *tree)
{
	assert(tree);
	return tree->size;
}

static inline bool
splay_is_empty(const struct splay_tree *tree)
{
	assert(tree);
	return splay_size(tree) == 0;
}

#ifndef NDEBUG
int splay_verify(const struct splay_tree *tree);
void debug_print(const struct splay_node *m,
		const struct splay_node *l,
		const struct splay_node *r);
void splay_print(const struct splay_tree *tree);
#endif
