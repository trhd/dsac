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

#pragma once

#include <stdatomic.h>
#include "debug_flags.h"

DEBUG_FLAGS_ENUM(atomic_fifo_debug_flag,
		ATOMIC_FIFO_DEBUG_FLAG_INITIALIZED,
		_ATOMIC_FIFO_DEBUG_FLAG_COUNT)

struct atomic_fifo
{
	atomic_uintptr_t next;
	void * data;
	DEBUG_FLAGS(_ATOMIC_FIFO_DEBUG_FLAG_COUNT)
};

void
atomic_fifo_initialize(struct atomic_fifo * fifo);

void
atomic_fifo_set(struct atomic_fifo * element, void * user_data);

void *
atomic_fifo_get(struct atomic_fifo * element);

void
atomic_fifo_push(struct atomic_fifo * fifo, struct atomic_fifo * element);

struct atomic_fifo *
atomic_fifo_pop( struct atomic_fifo * fifo);
