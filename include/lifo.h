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

#include "doubly_linked_list.h"

typedef struct doubly_linked_list_meta lifo_meta_t;
typedef struct doubly_linked_list lifo_t;

static inline void *
lifo_get(const lifo_t * lifo)
{
	return doubly_linked_list_get(lifo);
}

static inline void
lifo_set(lifo_t * lifo, void * arg)
{
	doubly_linked_list_set(lifo, arg);
}

static inline void
lifo_initialize(lifo_meta_t * meta)
{
	doubly_linked_list_initialize(meta);
}

static inline void
lifo_push(lifo_meta_t *meta, lifo_t *buffer)
{
	doubly_linked_list_insert_front(meta, buffer);
}

static inline lifo_t *
lifo_pop(lifo_meta_t *meta)
{
	return doubly_linked_list_remove_head(meta);
}

static inline const lifo_t *
lifo_peek(lifo_meta_t *meta)
{
	return doubly_linked_list_find_head(meta);
}

static inline unsigned int
lifo_size(const lifo_meta_t *meta)
{
	return doubly_linked_list_size(meta);
}

static inline int
lifo_empty(const lifo_meta_t *meta)
{
	return doubly_linked_list_empty(meta);
}
