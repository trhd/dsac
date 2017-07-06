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

#include <stdbool.h>
#include <pthread.h>
#include "debug_flags.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)
# define LOCK_INITIALIZER \
	{ \
		.mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP, \
		._debug_flags = { 1 << LOCK_DEBUG_FLAG_INITIALIZED } \
	}
#else
# define LOCK_INITIALIZER \
	{ \
		.mutex = PTHREAD_MUTEX_INITIALIZER \
	}
#endif

DEBUG_FLAGS_ENUM(lock_debug_flag,
		LOCK_DEBUG_FLAG_INITIALIZED,
		_LOCK_DEBUG_FLAG_COUNT)

struct lock
{
	pthread_mutex_t mutex;
	DEBUG_FLAGS(_LOCK_DEBUG_FLAG_COUNT)
};

bool lock_initialize(struct lock * lock);
bool lock_uninitialize(struct lock * lock);

bool lock_acquire(struct lock * lock);
bool lock_acquire_try(struct lock * lock);
bool lock_release(struct lock * lock);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
void lock_assert(const struct lock * lock);
#else
#define lock_assert(...) ((void)0)
#endif
