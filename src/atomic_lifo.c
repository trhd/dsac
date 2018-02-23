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
#include "atomic_lifo.h"

void
atomic_lifo_initialize(struct atomic_lifo * f)
{
	assert(f);

	atomic_init(&f->next, 0);

	debug_flags_initialize(f);
	debug_flags_set(f, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
}

void
atomic_lifo_push(struct atomic_lifo * f, struct atomic_lifo * e)
{
	assert(f);
	assert(e);
	debug_flags_assert(f, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(e, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_lifo * t;

	do
	{
		t = (struct atomic_lifo *)atomic_load(&f->next);
		atomic_store(&e->next, (uintptr_t)t);
	}
	while (!atomic_compare_exchange_weak(&f->next, (uintptr_t*)&t, (uintptr_t)e));
}

struct atomic_lifo *
atomic_lifo_pop(struct atomic_lifo * f)
{
	assert(f);
	debug_flags_assert(f, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_lifo * r, * t;

	do
	{
		r = (struct atomic_lifo *)atomic_load(&f->next);

		if (!r)
			return NULL;

		t = (struct atomic_lifo *)atomic_load(&r->next); 
	}
	while(!atomic_compare_exchange_weak(&f->next, (uintptr_t*)&r, (uintptr_t)t));

	atomic_store(&r->next, 0);

	return r;
}

void
atomic_lifo_set(struct atomic_lifo * e, void * d)
{
	assert(e);

	atomic_init(&e->next, 0);
	e->data = d;

	debug_flags_initialize(e);
	debug_flags_set(e, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
}

void *
atomic_lifo_get(struct atomic_lifo *e)
{
	assert(e);

	return e->data;
}
