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

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>

struct splay_node
{
	void const * data;
	struct splay_node *left, *right;
};

struct splay_tree
{
	size_t size;
	int (*compare)(void const *, void const *);
	struct splay_node * root;
};

void
splay_initialize(struct splay_tree * tree,
		int (*comparison_function)(void const * first, void const * second));


int
splay_insert(struct splay_tree * tree, struct splay_node * node);


struct splay_node *
splay_remove(struct splay_tree *, void const *);

struct splay_node *
splay_remove_any(struct splay_tree * tree);

struct splay_node *
splay_remove_min(struct splay_tree * tree);

struct splay_node *
splay_remove_max(struct splay_tree * tree);

struct splay_node *
splay_remove_next(struct splay_tree * tree, void const * key);

struct splay_node *
splay_remove_prev(struct splay_tree * tree, void const * key);


struct splay_node const *
splay_find(struct splay_tree * tree, void const * key);

struct splay_node const *
splay_find_min(struct splay_tree * tree);

struct splay_node const *
splay_find_max(struct splay_tree * tree);

struct splay_node const *
splay_find_next(struct splay_tree * tree, void const * key);

struct splay_node const *
splay_find_prev(struct splay_tree * tree, void const * key);

static inline struct splay_node const *
splay_find_any(struct splay_tree * tree)
{
	assert(tree);
	return tree->root;
}


int
splay_iterate(struct splay_tree const * tree,
		int (*callback_function)(struct splay_node const *, void *),
		void * argument_pointer);


static inline void
splay_set(struct splay_node * node, void const * key)
{
	assert(node);
	/** key could be null/zero if pretending pointers to be number */
	node->data = key;
}

static inline void const *
splay_get(struct splay_node const * node)
{
	assert(node);
	return node->data;
}


static inline size_t
splay_size(struct splay_tree const * tree)
{
	assert(tree);
	return tree->size;
}


static inline bool
splay_is_empty(struct splay_tree const * tree)
{
	assert(tree);
	return splay_size(tree) == 0;
}

#if !defined(NDEBUG) || defined(UNIT_TESTING)

int
splay_verify(struct splay_tree const * tree);

void
debug_print(struct splay_node const * m, struct splay_node const * l,
            struct splay_node const * r);
void
splay_print(struct splay_tree const * tree);

#endif
