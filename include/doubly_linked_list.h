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

struct doubly_linked_list
{
	struct doubly_linked_list *next;
	struct doubly_linked_list *prev;
	void const *data;
};

struct doubly_linked_list_meta
{
	struct doubly_linked_list *head;
	struct doubly_linked_list *tail;
	unsigned int size;
};

void
doubly_linked_list_initialize(struct doubly_linked_list_meta *meta);

static inline void
doubly_linked_list_set(struct doubly_linked_list *entry, const void *data)
{
	assert(entry);
	entry->data = data;
}

static inline const void *
doubly_linked_list_get(const struct doubly_linked_list *entry)
{
	assert(entry);
	return (void*)entry->data;
}

static inline struct doubly_linked_list *
doubly_linked_list_next(const struct doubly_linked_list *entry)
{
	assert(entry);
	return entry->next;
}

static inline struct doubly_linked_list *
doubly_linked_list_previous(const struct doubly_linked_list *entry)
{
	assert(entry);
	return entry->prev;
}

int
doubly_linked_list_insert_front(struct doubly_linked_list_meta *meta, struct doubly_linked_list *entry);

int
doubly_linked_list_insert_back(struct doubly_linked_list_meta *meta, struct doubly_linked_list *entry);

void
doubly_linked_list_detach(struct doubly_linked_list_meta *meta, struct doubly_linked_list *entry);

struct doubly_linked_list *
doubly_linked_list_remove_front(struct doubly_linked_list_meta *meta, const void * p);

struct doubly_linked_list *
doubly_linked_list_remove_back(struct doubly_linked_list_meta *meta, const void * p);

struct doubly_linked_list *
doubly_linked_list_remove_head(struct doubly_linked_list_meta *meta);

struct doubly_linked_list *
doubly_linked_list_remove_tail(struct doubly_linked_list_meta *meta);

struct doubly_linked_list *
doubly_linked_list_find_front(struct doubly_linked_list *head, const void *data);

struct doubly_linked_list *
doubly_linked_list_find_back(struct doubly_linked_list *tail, const void *data);

static inline struct doubly_linked_list *
doubly_linked_list_find_tail(struct doubly_linked_list_meta *meta)
{
	assert(meta);
	return meta->tail;
}

static inline struct doubly_linked_list *
doubly_linked_list_find_head(struct doubly_linked_list_meta *meta)
{
	assert(meta);
	return meta->head;
}

static inline unsigned int
doubly_linked_list_size(const struct doubly_linked_list_meta *meta)
{
	assert(meta);
	return meta->size;
}

static inline int
doubly_linked_list_empty(const struct doubly_linked_list_meta *meta)
{
	assert(meta);
	return doubly_linked_list_size(meta) == 0;
}
