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

#pragma once

#include "debug_flags.h"
#include "ring_buffer.h"

DEBUG_FLAGS_ENUM(homogeneous_ring_buffer_debug_flag,
		HOMOGENEOUS_RING_BUFFER_INITIALIZED,
		_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_COUNT);

struct homogeneous_ring_buffer
{
	size_t read_cursor;
	size_t write_cursor;
	size_t element_bytes;
	void * array;
	size_t array_size;
	DEBUG_FLAGS(_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_COUNT);
};

void
homogeneous_ring_buffer_initialize(struct homogeneous_ring_buffer * ring,
		void * array, size_t array_size, size_t array_element_bytes);

bool
homogeneous_ring_buffer_write(struct homogeneous_ring_buffer * ring,
		void const * data);

void
homogeneous_ring_buffer_overwrite(struct homogeneous_ring_buffer * ring,
		void const * data);

void *
homogeneous_ring_buffer_allocate(struct homogeneous_ring_buffer * ring);

bool
homogeneous_ring_buffer_read(struct homogeneous_ring_buffer * ring,
		void * data);

bool
homogeneous_ring_buffer_peek(struct homogeneous_ring_buffer const * ring,
		void * data);

bool
homogeneous_ring_buffer_empty(struct homogeneous_ring_buffer const * ring);
