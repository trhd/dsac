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
		void * argument);
