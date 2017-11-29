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

#include <assert.h>
#include <errno.h>
#include "linked_list.h"

static void
assert_unique_pointer(struct linked_list_meta * m, struct linked_list * n)
{
	assert(m);
	assert(n);

	struct linked_list * nn = m->head;

	while (nn)
	{
		assert(nn != n);
		nn = nn->next;
	}
}

void
linked_list_initialize(struct linked_list_meta *m)
{
	assert(m);

#ifdef LINKED_LIST_CONFIG_SIZE_CACHE
	m->size = 0;
#endif
	m->head = 0;
#ifdef LINKED_LIST_CONFIG_TAIL_POINTER
	m->tail = 0;
#endif
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

void
linked_list_insert_front(struct linked_list_meta *m, struct linked_list *n)
{
	assert(m);
	assert(n);
	assert_unique_pointer(m, n);

	n->next = m->head;
	m->head = n;

#ifdef LINKED_LIST_CONFIG_SIZE_CACHE
	++m->size;
#endif

#ifdef LINKED_LIST_CONFIG_TAIL_POINTER
	if (!n->next)
		m->tail = n;
#endif

	assert(n != n->next);
}

void
linked_list_insert_back(struct linked_list_meta *m, struct linked_list *n)
{
	assert(m);
	assert(n);

	struct linked_list **h;

#if LINKED_LIST_CONFIG_TAIL_POINTER
	h = &m->tail;
#else
	h = &m->head;

	while (*h)
		h = &(*h)->next;
#endif

	*h = n;
	n->next = 0;
#if LINKED_LIST_CONFIG_SIZE_CACHE
	++m->size;
#endif

	assert(n != n->next);
}

/**
 * FIXME: Would be better if the unused argument wouldn't need to be
 * passed in the first place.
 */
struct linked_list *
#if LINKED_LIST_CONFIG_SIZE_CACHE
linked_list_detach(struct linked_list_meta *m, struct linked_list **e)
#else
linked_list_detach(struct linked_list_meta *m __attribute__((unused)), struct linked_list **e)
#endif
{
	assert(e);
	assert(*e);

	struct linked_list *t = *e;

	*e = t->next;
	t->next = 0;
#if LINKED_LIST_CONFIG_SIZE_CACHE
	--m->size;
#endif

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

	return m->head ? 0 : 1;
}

unsigned int
linked_list_size(const struct linked_list_meta *m)
{
	assert(m);

#if LINKED_LIST_CONFIG_SIZE_CACHE
	return m->size;
#else
	unsigned int rv = 0;

	for (struct linked_list * n = m->head ; n ; n = n->next)
		rv++;

	return rv;
#endif
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
