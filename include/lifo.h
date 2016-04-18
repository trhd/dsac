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

#include "doubly_linked_list.h"

typedef struct doubly_linked_list_meta lifo_meta_t;
typedef struct doubly_linked_list lifo_t;

static inline void *
lifo_get(lifo_t * lifo)
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

static inline int
lifo_push(lifo_meta_t *meta, lifo_t *buffer)
{
	return doubly_linked_list_insert_front(meta, buffer);
}

static inline lifo_t *
lifo_pop(lifo_meta_t *meta)
{
	return doubly_linked_list_remove_head(meta);
}

static inline lifo_t *
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
