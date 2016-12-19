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
#include <string.h>
#include <sys/types.h>
#include "ring_buffer.h"

#define RB_IS_EMPTY ((size_t)-1)

static void
wrap_read_if_unusable(struct ring_buffer *r)
{
	assert(r);

	if (r->size - r->read <= sizeof(size_t))
		r->read = 0;
}

static void
wrap_write_if_unusable(struct ring_buffer * r)
{
	assert(r);

	if (r->size - r->write <= sizeof(size_t))
		r->write = 0;
}

static void
mark_empty_if_empty(struct ring_buffer * r)
{
	assert(r);

	if (r->read == r->write)
		r->read = RB_IS_EMPTY;
}

bool
ring_buffer_empty(struct ring_buffer const * r)
{
	assert(r);
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return r->read == RB_IS_EMPTY;
}

bool
ring_buffer_initialize(struct ring_buffer * r, void * b, size_t l)
{
	assert(r);
	assert(b);
	assert(l);
	assert(l > sizeof(size_t));

	r->data = b;
	*(size_t*)&r->size = l;
	r->write = 0;
	r->read = RB_IS_EMPTY;

	debug_flags_set(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return false;
}

static void
mark_nonempty(struct ring_buffer * r)
{
	assert(r);

	if (ring_buffer_empty(r))
		r->read = r->write;
}

static void *
mark_reservation(struct ring_buffer * r, size_t l)
{
	assert(r);
	assert(l);

	void * rv = (char*)r->data + r->write + sizeof(size_t);
	*(size_t*)((char*)r->data + r->write) = l;
	mark_nonempty(r);
	r->write += l;

	assert(r->write <= r->size);

	return rv;
}

static void *
reserve_from_the_end(struct ring_buffer * r, size_t l)
{
	assert(r);
	assert(l);

	void * rv;

	if (r->size - r->write < l)
		return NULL;

	rv = mark_reservation(r, l);

	wrap_write_if_unusable(r);

	return rv;
}

static size_t
void_end_of_buffer(struct ring_buffer * r)
{
	assert(r);

	if (r->size - r->write > sizeof(size_t))
	{
		size_t rv = r->write;
		*(size_t*)((char*)r->data + r->write) = 0;
		r->write = 0;
		return rv;
	}

	r->write = 0;
	return 0;
}

static void
unvoid_end_of_buffer(struct ring_buffer * r, size_t w)
{
	r->write = w;
}

static void *
reserve_from_between(struct ring_buffer * r, size_t l)
{
	assert(r);
	assert(l);
	
	if (r->read - r->write < l)
		return NULL;

	return mark_reservation(r, l);
}

static void *
reserve(struct ring_buffer * r, size_t l)
{
	assert(r);
	assert(l);

	size_t t = 0;
	void * rv;
	l += sizeof(size_t);

	if (r->read == r->write)
		return NULL;

	if (ring_buffer_empty(r) || r->write >= r->read)
	{
		rv = reserve_from_the_end(r, l);

		if (rv)
			return rv;

		t = void_end_of_buffer(r);
	}

	rv = reserve_from_between(r, l);

	if (!rv && t)
		unvoid_end_of_buffer(r, t);

	return rv;
}

bool
ring_buffer_write(struct ring_buffer * r, void const * d, size_t l)
{
	assert(r);
	assert(d);
	assert(l);
	assert(l <= r->size - sizeof(size_t));
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	void * p = reserve(r, l);

	if (!p)
		return true;

	memcpy(p, d, l);

	return false;
}

static size_t
peek_read_length(struct ring_buffer const * r)
{
	assert(r);
	assert(r->read != r->write || !ring_buffer_empty(r));

	size_t t = *(size_t*)((char*)r->data + r->read);

	if (!t)
		return 0;
	assert(t > sizeof(size_t));
	return t - sizeof(size_t);
}

static void
drop(struct ring_buffer * r)
{
	assert(r);
	assert(!ring_buffer_empty(r));

	size_t l = peek_read_length(r);

	r->read += l + sizeof(size_t);
	wrap_read_if_unusable(r);
}

void
ring_buffer_overwrite(struct ring_buffer * r, void const * d, size_t l)
{
	assert(r);
	assert(d);
	assert(l);
	assert(l <= r->size - sizeof(size_t));
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	void * p = reserve(r, l);

	while (!p)
	{
		drop(r);
		p = reserve(r, l);
	}

	memcpy(p, d, l);
}

static void
peek_from_buffer(struct ring_buffer const * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	assert(*l);
	assert(*l >= peek_read_length(r));

	*l = peek_read_length(r);
	memcpy(d, (char*)r->data + r->read + sizeof(size_t), *l);
}

static void
read_from_buffer(struct ring_buffer * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	assert(*l);
	assert(*l >= peek_read_length(r));

	*l = peek_read_length(r);
	r->read += sizeof(size_t);
	memcpy(d, (char*)r->data + r->read, *l);
	r->read += *l;

	wrap_read_if_unusable(r);
	mark_empty_if_empty(r);
}

bool
ring_buffer_read(struct ring_buffer * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	size_t t;

again:
	if (ring_buffer_empty(r))
		return true;

	t = peek_read_length(r);

	if (!t)
	{
		assert(r->read);
		r->read = 0;
		goto again;
	}

	if (t > *l)
		return true;

	read_from_buffer(r, d, l);

	return false;
}

bool
ring_buffer_peek(struct ring_buffer * r, void * d, size_t * l)
{
	assert(r);
	assert(d);
	assert(l);
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	size_t t;

again:
	if (ring_buffer_empty(r))
		return true;

	t = peek_read_length(r);

	if (!t)
	{
		assert(r->read);
		r->read = 0;
		goto again;
	}

	if (t > *l)
		return true;

	peek_from_buffer(r, d, l);

	return false;
}

void *
ring_buffer_allocate(struct ring_buffer * r, size_t l)
{
	assert(r);
	assert(l);
	assert(l <= r->size);
	debug_flags_assert(r, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return reserve(r, l);
}
