/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2018 Hemmo Nieminen
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
#include "atomic_fifo.h"

void
atomic_fifo_initialize(struct atomic_fifo * f)
{
	assert(f);

	atomic_init(&f->next, 0);

	debug_flags_initialize(f);
	debug_flags_set(f, ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED);
}

void
atomic_fifo_push(struct atomic_fifo * f, struct atomic_fifo * e)
{
	assert(f);
	assert(e);
	debug_flags_assert(f, ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(e, ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_fifo * t;

	do
	{
		do
			t = (struct atomic_fifo *)atomic_load(&f->next);
		while (t == f);

		atomic_store(&e->next, (uintptr_t)t);
	}
	while (!atomic_compare_exchange_weak(&f->next, (uintptr_t*)&t, (uintptr_t)e));
}

static void
atomic_restore_to(atomic_uintptr_t * d, uintptr_t e, uintptr_t v)
{
	assert(d);
	assert(e);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert(atomic_compare_exchange_strong(d, &e, v));
#else
	atomic_compare_exchange_strong(d, &e, v);
#endif
}

static uintptr_t
atomic_set_to(atomic_uintptr_t * d, uintptr_t v)
{
	assert(d);

	uintptr_t old = 0;

	while (!atomic_compare_exchange_weak(d, &old, v));

	return old;
}

struct atomic_fifo *
reserve_next(struct atomic_fifo * f)
{
	assert(f);

	struct atomic_fifo * n;

	do
		n = (struct atomic_fifo *)atomic_set_to(&f->next, (uintptr_t)f);
	while (n == f);

	if (!n)
		atomic_restore_to(&f->next, (uintptr_t)f, 0);

	return n;
}

struct atomic_fifo *
atomic_fifo_pop(struct atomic_fifo * f)
{
	assert(f);
	debug_flags_assert(f, ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_fifo * prev = f, * current;

	current = reserve_next(f);

	if (!current)
		return NULL;

	for (;;)
	{
		struct atomic_fifo * next = reserve_next(current);

		if (next)
			atomic_restore_to(&prev->next, (uintptr_t)prev, (uintptr_t)current);
		else
		{
			atomic_restore_to(&prev->next, (uintptr_t)prev, 0);
			break;
		}

		prev = current;
		current = next;
	}

	return (struct atomic_fifo *)current;
}

void
atomic_fifo_set(struct atomic_fifo * e, void * d)
{
	assert(e);

	atomic_init(&e->next, 0);
	e->data = d;

	debug_flags_initialize(e);
	debug_flags_set(e, ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED);
}

void *
atomic_fifo_get(struct atomic_fifo *e)
{
	assert(e);

	return e->data;
}
