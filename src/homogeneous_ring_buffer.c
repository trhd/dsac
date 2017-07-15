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

#include "homogeneous_ring_buffer.h"

#ifndef NULL
# define NULL ((void*)0)
#endif

#define HRB_IS_EMPTY ((size_t)-1)

void
homogeneous_ring_buffer_initialize(struct homogeneous_ring_buffer * r,
		void * b, size_t bl, size_t el)
{
	assert(r);
	assert(b);
	assert(bl);
	assert(el);
	assert(el <= bl);
	assert(bl < HRB_IS_EMPTY);

	r->read_cursor = HRB_IS_EMPTY;
	r->write_cursor = 0;
	r->element_bytes = el;
	r->array = b;
	r->array_size = bl / el;

	debug_flags_initialize(r);
}

static bool
is_full(struct homogeneous_ring_buffer const * r)
{
	assert(r);

	return r->read_cursor == r->write_cursor;
}

static bool
is_empty(struct homogeneous_ring_buffer const * r)
{
	assert(r);

	return r->read_cursor == HRB_IS_EMPTY;
}

static void const *
read_data_ptr(struct homogeneous_ring_buffer const * r)
{
	assert(r);

	return (char*)r->array + r->read_cursor * r->element_bytes;
}

static void *
get_write_ptr(struct homogeneous_ring_buffer * r)
{
	assert(r);

	return (char*)r->array + r->write_cursor * r->element_bytes;
}

static void
advance_read_ptr(struct homogeneous_ring_buffer * r)
{
	assert(r);

	r->read_cursor = (r->read_cursor + 1) % r->array_size;

	if (r->read_cursor == r->write_cursor)
		r->read_cursor = HRB_IS_EMPTY;
}

static void
hbr_set_non_empty(struct homogeneous_ring_buffer * r)
{
	assert(r);

	if (is_empty(r))
		r->read_cursor = r->write_cursor;
}

static void
advance_write_ptr(struct homogeneous_ring_buffer * r)
{
	assert(r);

	hbr_set_non_empty(r);
	r->write_cursor = (r->write_cursor + 1) % r->array_size;

}

static void
write_data(struct homogeneous_ring_buffer * r, void const * d)
{
	assert(r);

	memcpy(get_write_ptr(r), d, r->element_bytes);
	advance_write_ptr(r);
}

bool
homogeneous_ring_buffer_write(struct homogeneous_ring_buffer * r, void const * d)
{
	assert(r);
	assert(d);

	if (is_full(r))
		return true;

	write_data(r, d);

	return false;
}

void *
homogeneous_ring_buffer_allocate(struct homogeneous_ring_buffer * r)
{
	assert(r);

	void * rv;

	if (is_full(r))
		return NULL;

	rv = get_write_ptr(r);
	advance_write_ptr(r);

	return rv;
}

void
homogeneous_ring_buffer_overwrite(struct homogeneous_ring_buffer * r, void const * d)
{
	assert(r);
	assert(d);

	if (r->read_cursor == r->write_cursor)
		advance_read_ptr(r);

	write_data(r, d);
}

static void
peek_data(struct homogeneous_ring_buffer const * r, void * d)
{
	assert(r);
	assert(d);
	assert(!is_empty(r));

	memcpy(d, read_data_ptr(r), r->element_bytes);
}

static void
read_data(struct homogeneous_ring_buffer * r, void * d)
{
	assert(r);
	assert(d);

	peek_data(r, d);
	advance_read_ptr(r);
}

bool
homogeneous_ring_buffer_read(struct homogeneous_ring_buffer * r, void * d)
{
	assert(r);
	assert(d);

	if (is_empty(r))
		return true;

	read_data(r, d);

	return false;
}

bool
homogeneous_ring_buffer_peek(struct homogeneous_ring_buffer const * r, void * d)
{
	assert(r);
	assert(d);

	if (is_empty(r))
		return true;

	peek_data(r, d);

	return false;
}

bool
homogeneous_ring_buffer_empty(struct homogeneous_ring_buffer const * r)
{
	assert(r);

	return r->read_cursor == HRB_IS_EMPTY;
}
