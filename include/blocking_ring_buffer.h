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

#include "condition.h"
#include "debug_flags.h"
#include "lock.h"
#include "ring_buffer.h"
#include <stdbool.h>

DEBUG_FLAGS_ENUM(blocking_ring_buffer_debug_flag,
		BLOCKING_RING_BUFFER_DEBUG_FLAG_INITIALIZED,
		BLOCKING_RING_BUFFER_DEBUG_FLAG_DIRTY,
		_BLOCKING_RING_BUFFER_DEBUG_FLAG_COUNT);

struct blocking_ring_buffer
{
	struct ring_buffer buffer;
	struct condition condition;
	struct lock lock;
	DEBUG_FLAGS(_BLOCKING_RING_BUFFER_DEBUG_FLAG_COUNT);
};

bool
blocking_ring_buffer_initialize(struct blocking_ring_buffer * ring,
		void * buffer, size_t length);

bool
blocking_ring_buffer_uninitialize(struct blocking_ring_buffer * ring);


bool
blocking_ring_buffer_write(struct blocking_ring_buffer * ring,
		void const * data, size_t data_length);

bool
blocking_ring_buffer_write_try(struct blocking_ring_buffer * ring,
		void const * data, size_t data_length);

bool
blocking_ring_buffer_overwrite(struct blocking_ring_buffer * ring,
		void const * data, size_t data_length);


bool
blocking_ring_buffer_read(struct blocking_ring_buffer * ring, void * data,
		size_t * data_length);

bool
blocking_ring_buffer_read_try(struct blocking_ring_buffer * ring, void * data,
		size_t * data_length);


bool
blocking_ring_buffer_peek(struct blocking_ring_buffer * ring, void * data,
		size_t * data_length);

bool
blocking_ring_buffer_peek_try(struct blocking_ring_buffer * ring, void * data,
		size_t * data_length);


void *
blocking_ring_buffer_write_allocate(struct blocking_ring_buffer * ring,
		size_t data_length);

void *
blocking_ring_buffer_write_allocate_try(struct blocking_ring_buffer * ring,
		size_t data_length);


bool
blocking_ring_buffer_write_flush(struct blocking_ring_buffer * ring);


int
blocking_ring_buffer_empty(struct blocking_ring_buffer const * ring);
