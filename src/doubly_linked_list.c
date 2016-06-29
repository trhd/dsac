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

#include <errno.h>
#include <string.h>
#include "doubly_linked_list.h"

void
doubly_linked_list_initialize(struct doubly_linked_list_meta *m)
{
	assert(m);

	memset(m, 0, sizeof(*m));
}

int
doubly_linked_list_insert_front(struct doubly_linked_list_meta *m, struct doubly_linked_list *e)
{
	assert(m);
	assert(e);

	if (doubly_linked_list_find_front(m->head, e->data))
		return EEXIST;

	e->next = m->head;
	e->prev = 0;
	m->head = e;

	if (++m->size == 1)
		m->tail = e;
	else
		e->next->prev = e;

	assert(e != e->next);

	return 0;
}

int
doubly_linked_list_insert_back(struct doubly_linked_list_meta *m, struct doubly_linked_list *e)
{
	assert(m);
	assert(e);

	if (doubly_linked_list_find_back(m->tail, e->data))
		return EEXIST;

	e->prev = m->tail;
	e->next = 0;
	m->tail = e;

	if (++m->size == 1)
		m->head = e;
	else
		e->prev->next = e;

	assert(e != e->next);

	return 0;
}

void
doubly_linked_list_detach(struct doubly_linked_list_meta *m, struct doubly_linked_list *e)
{
	assert(m);
	assert(e);

	if (e == m->head)
		m->head = e->next;
	else if (e->prev)
		e->prev->next = e->next;

	if (e == m->tail)
		m->tail = e->prev;
	else if (e->next)
		e->next->prev = e->prev;

	--m->size;
}

struct doubly_linked_list *
doubly_linked_list_remove_front(struct doubly_linked_list_meta *m, const void *p)
{
	assert(m);
	assert(p);

	struct doubly_linked_list *r = doubly_linked_list_find_front(m->head, p);

	if (r)
		doubly_linked_list_detach(m, r);

	return r;
}

struct doubly_linked_list *
doubly_linked_list_remove_back(struct doubly_linked_list_meta *m, const void *p)
{
	assert(m);
	assert(p);

	struct doubly_linked_list *r = doubly_linked_list_find_back(m->tail, p);

	if (r)
		doubly_linked_list_detach(m, r);

	return r;
}

struct doubly_linked_list *
doubly_linked_list_remove_head(struct doubly_linked_list_meta *m)
{
	assert(m);

	struct doubly_linked_list *r = m->head;

	if (r)
		doubly_linked_list_detach(m, r);

	return r;
}

struct doubly_linked_list *
doubly_linked_list_remove_tail(struct doubly_linked_list_meta *m)
{
	assert(m);

	struct doubly_linked_list *r = m->tail;

	if (r)
		doubly_linked_list_detach(m, r);

	return r;
}

struct doubly_linked_list *
doubly_linked_list_find_front(struct doubly_linked_list *h, const void *d)
{
	while (h && doubly_linked_list_get(h) != d)
		h = doubly_linked_list_next(h);

	return h;
}

struct doubly_linked_list *
doubly_linked_list_find_back(struct doubly_linked_list *t, const void *d)
{
	while (t && doubly_linked_list_get(t) != d)
		t = doubly_linked_list_previous(t);

	return t;
}

