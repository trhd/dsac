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
atomic_lifo_initialize(struct atomic_lifo * head)
{
	assert(head);

	atomic_init(&head->next, 0);

	debug_flags_initialize(head);
	debug_flags_set(head, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
}

struct atomic_lifo *
acquire_head(struct atomic_lifo * head)
{
	assert(head);

	struct atomic_lifo * rv;

	do
		rv = (struct atomic_lifo *)atomic_exchange(&head->next, (uintptr_t)head);
	while (rv == head);

	return rv;
}

void
atomic_lifo_push(struct atomic_lifo * head, struct atomic_lifo * new)
{
	assert(head);
	assert(new);
	debug_flags_assert(head, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(new, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_lifo * tmp = acquire_head(head);

	atomic_store(&new->next, (uintptr_t)tmp);
	atomic_store(&head->next, (uintptr_t)new);
}

struct atomic_lifo *
atomic_lifo_pop(struct atomic_lifo * head)
{
	assert(head);
	debug_flags_assert(head, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);

	struct atomic_lifo * rv = acquire_head(head);

	if (!rv)
		atomic_store(&head->next, 0);
	else
	{
		atomic_store(&head->next, rv->next);
		atomic_store(&rv->next, 0);
	}

	return rv;
}

void
atomic_lifo_set(struct atomic_lifo * entry, void * data)
{
	assert(entry);

	atomic_init(&entry->next, 0);
	entry->data = data;

	debug_flags_initialize(entry);
	debug_flags_set(entry, ATOMIC_LIFO_DEBUG_FLAG_INITIALIZED);
}

void *
atomic_lifo_get(struct atomic_lifo * entry)
{
	assert(entry);

	return entry->data;
}
