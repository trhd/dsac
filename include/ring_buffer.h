/**
 * Copyright (C) 2016-2017 Hemmo Nieminen
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

#pragma once

#include "debug_flags.h"
#include <stdbool.h>

DEBUG_FLAGS_ENUM(
	ring_buffer_debug_flag,
	RING_BUFFER_DEBUG_FLAG_INITIALIZED,
	_RING_BUFFER_DEBUG_FLAG_COUNT)

struct ring_buffer
{
	void * data;
	const size_t size;
	size_t read;
	size_t write;
	DEBUG_FLAGS(_RING_BUFFER_DEBUG_FLAG_COUNT)
};

bool
ring_buffer_initialize(struct ring_buffer * ring, void * buffer, size_t length);

static inline bool
ring_buffer_uninitialize(struct ring_buffer * ring)
{
	debug_flags_assert(ring, RING_BUFFER_DEBUG_FLAG_INITIALIZED);
	debug_flags_clear(ring, RING_BUFFER_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
ring_buffer_write(struct ring_buffer * ring, void const * data,
		size_t data_length);

void
ring_buffer_overwrite(struct ring_buffer * ring, void const * data,
		size_t data_length);

void *
ring_buffer_allocate(struct ring_buffer * ring, size_t data_length);

bool
ring_buffer_read(struct ring_buffer * ring, void * data, size_t * data_length);

bool
ring_buffer_peek(struct ring_buffer * ring, void * data, size_t * data_length);

bool
ring_buffer_empty(struct ring_buffer const * ring);
