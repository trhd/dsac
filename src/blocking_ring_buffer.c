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

#include "blocking_ring_buffer.h"
#include <assert.h>
#include <string.h>
#include <sys/types.h>

bool
blocking_ring_buffer_initialize(struct blocking_ring_buffer * r, void * b,
		size_t l)
{
	assert(r);
	assert(b);
	assert(l);
	assert(l > sizeof(size_t));

	if (ring_buffer_initialize(&r->buffer, b, l)
			|| lock_initialize(&r->lock)
			|| condition_initialize(&r->condition, &r->lock))
		return true;

	debug_flags_set(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
blocking_ring_buffer_uninitialize(struct blocking_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	debug_flags_clear(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	if (condition_uninitialize(&r->condition)
			|| lock_uninitialize(&r->lock))
		return true;

	return false;
}

bool
blocking_ring_buffer_write(struct blocking_ring_buffer * r, void const * d,
		size_t l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);
	assert(l < r->buffer.size - sizeof(size_t));

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = ring_buffer_write(&r->buffer, d, l)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition) || rv;

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_write_try(struct blocking_ring_buffer * r,
		void const * d, size_t l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);
	assert(l < r->buffer.size - sizeof(size_t));

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = ring_buffer_write(&r->buffer, d, l);

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_overwrite(struct blocking_ring_buffer * r, void const * d,
		size_t l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	ring_buffer_overwrite(&r->buffer, d, l);

	rv = condition_signal(&r->condition);

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_read(struct blocking_ring_buffer * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = ring_buffer_read(&r->buffer, d, l)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition);

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_read_try(struct blocking_ring_buffer * r, void * d,
		size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = ring_buffer_read(&r->buffer, d, l);

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_peek(struct blocking_ring_buffer * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = ring_buffer_peek(&r->buffer, d, l)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition);

	lock_release(&r->lock);

	return rv;
}

bool
blocking_ring_buffer_peek_try(struct blocking_ring_buffer * r, void * d,
		size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = ring_buffer_peek(&r->buffer, d, l);

	lock_release(&r->lock);

	return rv;
}

int
blocking_ring_buffer_empty(struct blocking_ring_buffer const * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	int rv;

	if (lock_acquire(&((struct blocking_ring_buffer *)r)->lock))
		return -1;

	rv = ring_buffer_empty(&r->buffer) ? 1 : 0;

	lock_release(&((struct blocking_ring_buffer *)r)->lock);

	return rv;
}

void *
blocking_ring_buffer_write_allocate(struct blocking_ring_buffer * r,
		size_t l)
{
	assert(r);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);
	assert(l < r->buffer.size - sizeof(size_t));

	void * rv;

	if (lock_acquire(&r->lock))
		return NULL;

	while (!(rv = ring_buffer_allocate(&r->buffer, l)))
		if (condition_wait(&r->condition))
			break;

	debug_flags_set(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_DIRTY);

	return rv;
}

void *
blocking_ring_buffer_write_allocate_try(struct blocking_ring_buffer * r,
		size_t l)
{
	assert(r);
	assert(l);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);
	assert(l < r->buffer.size - sizeof(size_t));

	void * rv;

	if (lock_acquire(&r->lock))
		return NULL;

	while (!(rv = ring_buffer_allocate(&r->buffer, l)))
	{
		lock_release(&r->lock);
		return NULL;
	}

	debug_flags_set(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_DIRTY);

	return rv;
}

void
blocking_ring_buffer_write_flush(struct blocking_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	debug_flags_assert(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_DIRTY);
	debug_flags_clear(r, BLOCKING_RING_BUFFER_DEBUG_FLAG_DIRTY);

	lock_release(&r->lock);
}
