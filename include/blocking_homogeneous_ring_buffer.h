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

#pragma once

#include "condition.h"
#include "debug_flags.h"
#include "homogeneous_ring_buffer.h"
#include "lock.h"

DEBUG_FLAGS_ENUM(blocking_homogeneous_ring_buffer_debug_flag,
		BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_INITIALIZED,
		BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_DIRTY,
		_BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_COUNT)

struct blocking_homogeneous_ring_buffer
{
	struct homogeneous_ring_buffer buffer;
	struct lock lock;
	struct condition condition;
	DEBUG_FLAGS(_BLOCKING_HOMOGENEOUS_RING_BUFFER_DEBUG_FLAG_COUNT)
};

bool
blocking_homogeneous_ring_buffer_initialize(struct blocking_homogeneous_ring_buffer * ring,
		void * array, size_t array_size, size_t array_element_bytes);

bool
blocking_homogeneous_ring_buffer_uninitialize(struct blocking_homogeneous_ring_buffer * ring);

bool
blocking_homogeneous_ring_buffer_write(struct blocking_homogeneous_ring_buffer * ring,
		void const * data);

bool
blocking_homogeneous_ring_buffer_write_try(struct blocking_homogeneous_ring_buffer * ring,
		void const * data);

bool
blocking_homogeneous_ring_buffer_overwrite(struct blocking_homogeneous_ring_buffer * ring,
		void const * data);

void *
blocking_homogeneous_ring_buffer_write_allocate(struct blocking_homogeneous_ring_buffer * ring);

void *
blocking_homogeneous_ring_buffer_write_allocate_try(struct blocking_homogeneous_ring_buffer * ring);

void
blocking_homogeneous_ring_buffer_write_flush(struct blocking_homogeneous_ring_buffer * ring);

bool
blocking_homogeneous_ring_buffer_read(struct blocking_homogeneous_ring_buffer * ring,
		void * data);

bool
blocking_homogeneous_ring_buffer_read_try(struct blocking_homogeneous_ring_buffer * ring,
		void * data);

bool
blocking_homogeneous_ring_buffer_peek(struct blocking_homogeneous_ring_buffer * ring,
		void * data);

bool
blocking_homogeneous_ring_buffer_peek_try(struct blocking_homogeneous_ring_buffer * ring,
		void * data);

int
blocking_homogeneous_ring_buffer_empty(struct blocking_homogeneous_ring_buffer const * ring);
