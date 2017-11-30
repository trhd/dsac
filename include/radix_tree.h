/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2016-2017 Hemmo Nieminen
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

/**
 * Configuration:
 * - RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH:
 *     Define this to remove a per "node" key lengt variable. Disasbling
 *     this saves memory with large radix trees but introduces overhead
 *     when using it.
 * - RADIX_TREE_CONFIG_ITERATION_PREALLOCATION:
 *     The amount of memory the tree will allocate in the beginning of
 *     radix_tree_iterate(). A large initial buffer can waste memory but
 *     saves the routine from memore reallocations when less memory is
 *     needed. Defaults to 50 bytes.
 */
#pragma once

#include <stdbool.h>

struct radix_tree *
radix_tree_initialize(
		void * (*malloc) (size_t size),
		void (*free) (void * data),
		void * (*realloc) (void * data, size_t size)
		);

void
radix_tree_uninitialize(
		struct radix_tree ** tree
		);


bool
radix_tree_insert(
		struct radix_tree * tree,
		char const * key,
		void * value
		);

void *
radix_tree_remove(
		struct radix_tree * tree,
		char const * key
		);

void *
radix_tree_find(
		struct radix_tree * tree,
		char const * key
		);


size_t
radix_tree_get_key_count(
		struct radix_tree const * tree
		);

int
radix_tree_iterate(
		struct radix_tree * tree,
		int (*callback_function) (char const * key, void * value, void * argument),
		void * argument
		);


#if !defined(NDEBUG) || defined(UNIT_TESTING)

void
radix_tree_dump(
		struct radix_tree const * tree
		);

#endif
