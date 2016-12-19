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

#include "blocking_homogeneous_ring_buffer.h"

bool
blocking_homogeneous_ring_buffer_initialize(struct blocking_homogeneous_ring_buffer * r,
		void * b, size_t bl, size_t el)
{
	assert(r);
	assert(b);
	assert(bl);
	assert(el);

	if (lock_initialize(&r->lock)
			|| condition_initialize(&r->condition, &r->lock))
		return true;

	homogeneous_ring_buffer_initialize(&r->buffer, b, bl, el);

	debug_flags_set(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
blocking_homogeneous_ring_buffer_uninitialize(struct blocking_homogeneous_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	debug_flags_clear(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	if (condition_uninitialize(&r->condition) ||
			lock_uninitialize(&r->lock))
		return true;

	return false;
}

bool
blocking_homogeneous_ring_buffer_write(struct blocking_homogeneous_ring_buffer * r, void const * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = homogeneous_ring_buffer_write(&r->buffer, d)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition) || rv;
	return lock_release(&r->lock) || rv;
}

bool
blocking_homogeneous_ring_buffer_write_try(struct blocking_homogeneous_ring_buffer * r,
		void const * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = homogeneous_ring_buffer_write(&r->buffer, d);

	return lock_release(&r->lock) || rv;
}

bool
blocking_homogeneous_ring_buffer_overwrite(struct blocking_homogeneous_ring_buffer * r,
		void const * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	homogeneous_ring_buffer_overwrite(&r->buffer, d);

	rv = condition_signal(&r->condition);
	return lock_release(&r->lock) || rv;
}

void *
blocking_homogeneous_ring_buffer_write_allocate(struct blocking_homogeneous_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	void * rv;

	if (lock_acquire(&r->lock))
		return NULL;

	while (!(rv = homogeneous_ring_buffer_allocate(&r->buffer)))
		if (condition_wait(&r->condition))
			break;

	debug_flags_set(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_DIRTY);

	return rv;
}

void *
blocking_homogeneous_ring_buffer_write_allocate_try(struct blocking_homogeneous_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	void * rv;
	
	if (lock_acquire(&r->lock))
		return NULL;

	while (!(rv = homogeneous_ring_buffer_allocate(&r->buffer)))
	{
		lock_release(&r->lock);
		return NULL;
	}

	debug_flags_set(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_DIRTY);

	return rv;
}

bool
blocking_homogeneous_ring_buffer_write_flush(struct blocking_homogeneous_ring_buffer * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_DIRTY);
	debug_flags_clear(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_DIRTY);

	return lock_release(&r->lock);;
}

bool
blocking_homogeneous_ring_buffer_read(struct blocking_homogeneous_ring_buffer * r, void * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = homogeneous_ring_buffer_read(&r->buffer, d)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition);
	return lock_release(&r->lock) || rv;
}

bool
blocking_homogeneous_ring_buffer_read_try(struct blocking_homogeneous_ring_buffer * r,
		void * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = homogeneous_ring_buffer_read(&r->buffer, d);

	return lock_release(&r->lock) || rv;
}

bool
blocking_homogeneous_ring_buffer_peek(struct blocking_homogeneous_ring_buffer * r,
		void * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	while ((rv = homogeneous_ring_buffer_peek(&r->buffer, d)))
		if (condition_wait(&r->condition))
			break;

	rv = condition_signal(&r->condition);
	return lock_release(&r->lock) || rv;
}

bool
blocking_homogeneous_ring_buffer_peek_try(struct blocking_homogeneous_ring_buffer * r,
		void * d)
{
	assert(r);
	assert(d);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	bool rv;

	if (lock_acquire(&r->lock))
		return true;

	rv = homogeneous_ring_buffer_peek(&r->buffer, d);

	return lock_release(&r->lock) || rv;
}

int
blocking_homogeneous_ring_buffer_empty(struct blocking_homogeneous_ring_buffer const * r)
{
	assert(r);
	debug_flags_assert(r, BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	int rv;

	if (lock_acquire(&((struct blocking_homogeneous_ring_buffer *)r)->lock))
		return -1;

	rv = homogeneous_ring_buffer_empty(&r->buffer) ? 1 : 0;

	return lock_release(&((struct blocking_homogeneous_ring_buffer *)r)->lock) ? -1 : rv;
}
