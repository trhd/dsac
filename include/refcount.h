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

#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>
#include "debug_flags.h"

DEBUG_FLAGS_ENUM(refcount_debug_flag,
		REFCOUNT_DEBUG_FLAG_INITIALIZED,
		_REFCOUNT_DEBUG_FLAG_COUNT
	)

struct refcount
{
	atomic_ushort count;
	DEBUG_FLAGS(_REFCOUNT_DEBUG_FLAG_COUNT)
};

inline static void
refcount_initialize_to(struct refcount * refcount, uint16_t initial_value)
{
	assert(refcount);

	atomic_init(&refcount->count, initial_value);

	debug_flags_initialize(refcount);
	debug_flags_set(refcount, REFCOUNT_DEBUG_FLAG_INITIALIZED);
}

inline static void
refcount_initialize(struct refcount * refcount)
{
	assert(refcount);

	refcount_initialize_to(refcount, 0);
}

inline static uint16_t
refcount_acquire(struct refcount * refcount)
{
	assert(refcount);
	debug_flags_assert(refcount, REFCOUNT_DEBUG_FLAG_INITIALIZED);

	uint16_t r = atomic_fetch_add(&refcount->count, 1);
	assert(r != (uint16_t)-1);

	return r;
}

inline static uint16_t
refcount_release(struct refcount * refcount)
{
	assert(refcount);
	debug_flags_assert(refcount, REFCOUNT_DEBUG_FLAG_INITIALIZED);

	uint16_t r = atomic_fetch_sub(&refcount->count, 1);
	assert(r > 0);

	return r;
}

inline static uint16_t
refcount_check(struct refcount * refcount)
{
	assert(refcount);
	debug_flags_assert(refcount, REFCOUNT_DEBUG_FLAG_INITIALIZED);

	return atomic_load(&refcount->count);
}
