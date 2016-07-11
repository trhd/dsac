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

struct linked_list
{
	struct linked_list *next;
	void const *data;
};

struct linked_list_meta
{
	struct linked_list * head;
	unsigned int size;
};

void
linked_list_initialize(struct linked_list_meta *meta);

void
linked_list_set(struct linked_list *entry, const void *data);

const void *
linked_list_get(const struct linked_list *from);

struct linked_list **
linked_list_next(const struct linked_list *from);

int
linked_list_insert_front(struct linked_list_meta *meta, struct linked_list *entry);

int
linked_list_insert_back(struct linked_list_meta *meta, struct linked_list *entry);

struct linked_list *
linked_list_detach(struct linked_list_meta *meta, struct linked_list **entry);

struct linked_list *
linked_list_remove(struct linked_list_meta *meta, const void * p);

struct linked_list *
linked_list_remove_head(struct linked_list_meta *meta);

struct linked_list *
linked_list_remove_tail(struct linked_list_meta *meta);

struct linked_list *
linked_list_remove_match(struct linked_list_meta *meta, const void *match,
		int (*matcher)(const void *, const void *));

struct linked_list **
linked_list_find(struct linked_list_meta *meta, const void *data);

struct linked_list **
linked_list_find_head(const struct linked_list_meta *meta);

struct linked_list **
linked_list_find_match(struct linked_list_meta *meta, const void *match,
		int(*matcher)(const void *, const void *));

int
linked_list_iterate(const struct linked_list_meta *meta,
		int(*cb)(const struct linked_list *node, void *data),
		void *data);
int
linked_list_empty(const struct linked_list_meta *meta);

unsigned int
linked_list_size(const struct linked_list_meta *meta);
