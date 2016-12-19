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

#include <assert.h>
#include <errno.h>
#include "linked_list.h"

void
linked_list_initialize(struct linked_list_meta *m)
{
	assert(m);

	m->size = 0;
	m->head = 0;
}

void
linked_list_set(struct linked_list *e, const void *d)
{
	assert(e);

	e->next = 0;
	e->data = d;
}

const void *
linked_list_get(const struct linked_list *f)
{
	assert(f);

	return (void*)f->data;
}

struct linked_list **
linked_list_next(const struct linked_list *f)
{
	assert(f);

	return (struct linked_list **)&f->next;
}

int
linked_list_insert_front(struct linked_list_meta *m, struct linked_list *n)
{
	assert(m);
	assert(n);

	if (linked_list_find(m, linked_list_get(n)))
		return EEXIST;

	n->next = m->head;
	m->head = n;
	++m->size;

	assert(n != n->next);

	return 0;
}

int
linked_list_insert_back(struct linked_list_meta *m, struct linked_list *n)
{
	assert(m);
	assert(n);

	struct linked_list **h = &m->head;

	while (*h)
	{
		if (*h == n)
			return EEXIST;

		h = &(*h)->next;
	}

	*h = n;
	n->next = 0;
	++m->size;

	assert(n != n->next);

	return 0;
}

struct linked_list *
linked_list_detach(struct linked_list_meta *m, struct linked_list **e)
{
	assert(e);
	assert(*e);

	struct linked_list *t = *e;

	*e = t->next;
	t->next = 0;
	--m->size;

	return t;
}

struct linked_list *
linked_list_remove(struct linked_list_meta *m, const void *p)
{
	assert(m);
	assert(p);

	struct linked_list **h = &m->head;

	while (*h)
	{
		if (linked_list_get(*h) == p)
			return linked_list_detach(m, h);

		h = &(*h)->next;
	}

	return 0;
}

struct linked_list *
linked_list_remove_head(struct linked_list_meta *m)
{
	assert(m);

	if (!m->head)
		return 0;

	return linked_list_detach(m, &m->head);
}

struct linked_list *
linked_list_remove_tail(struct linked_list_meta *m)
{
	assert(m);

	struct linked_list **h = &m->head;

	if (!*h)
		return 0;

	while ((*h)->next)
		h = &(*h)->next;

	return linked_list_detach(m, h);
}

struct linked_list *
linked_list_remove_match(struct linked_list_meta *m, const void *match,
		int (cmp)(const void *, const void *))
{
	assert(m);
	assert(match);
	assert(cmp);

	struct linked_list **t = linked_list_find_match(m, match, cmp);

	if (!t)
		return 0;
	else
		return linked_list_detach(m, t);
}

struct linked_list **
linked_list_find(struct linked_list_meta *m, const void *p)
{
	assert(m);
	assert(p);

	struct linked_list **h = &m->head;

	while (*h)
	{
		if (linked_list_get(*h) == p)
			return h;

		h = &(*h)->next;
	}

	return 0;
}

struct linked_list **
linked_list_find_head(struct linked_list_meta *m)
{
	assert(m);

	return &m->head;
}

struct linked_list **
linked_list_find_tail(struct linked_list_meta *m)
{
	assert(m);

	struct linked_list **h = &m->head;

	if (!*h)
		return 0;

	while ((*h)->next)
		h = &(*h)->next;

	return h;
}

int
linked_list_empty(const struct linked_list_meta *m)
{
	assert(m);

	return linked_list_size(m) == 0;
}

unsigned int
linked_list_size(const struct linked_list_meta *m)
{
	assert(m);

	return m->size;
}

struct linked_list **
linked_list_find_match(struct linked_list_meta *m, const void *t,
		int(*f)(const void *, const void *))
{
	assert(m);
	assert(f);

	struct linked_list **h = &m->head;

	while (*h)
	{
		if (!f(t, linked_list_get(*h)))
			return h;

		h = &(*h)->next;
	}

	return 0;
}

int
linked_list_iterate(const struct linked_list_meta *m,
		int(*cb)(const struct linked_list *node, void *data),
		void *d)
{
	assert(m);
	assert(cb);

	int rv;
	struct linked_list **r = linked_list_find_head((struct linked_list_meta *)m);

	while (*r)
	{
		if ((rv = cb(*r, d)))
			return rv;

		r = linked_list_next(*r);
	}

	return 0;
}
