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

#pragma once

#include <pthread.h>
#include <stdbool.h>
#include "debug_flags.h"
#include "lock.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)
# define CONDITION_INITIALIZER(lock_arg) \
	{ \
		.lock = lock_arg, \
		.cond = PTHREAD_COND_INITIALIZER, \
		._debug_flags = { 1 << CONDITION_DEBUG_FLAG_INITIALIZED } \
	}
#else
# define CONDITION_INITIALIZER(lock_arg) \
	{ \
		.lock = lock_arg, \
		.cond = PTHREAD_COND_INITIALIZER \
	}
#endif

DEBUG_FLAGS_ENUM(condition_debug_flag,
		CONDITION_DEBUG_FLAG_INITIALIZED,
		_CONDITION_DEBUG_FLAG_COUNT)

struct condition
{
	struct lock *lock;
	pthread_cond_t cond;
	DEBUG_FLAGS(_CONDITION_DEBUG_FLAG_COUNT)
};

static inline bool
condition_initialize(struct condition *condition, struct lock *lock)
{
	assert(condition);
	assert(lock);

	condition->lock = lock;
	pthread_cond_init(&condition->cond, NULL);
	debug_flags_set(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return false;
}

static inline bool
condition_uninitialize(struct condition *condition)
{
	assert(condition);
	debug_flags_assert(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	pthread_cond_destroy(&condition->cond);
	debug_flags_clear(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return false;
}

static inline bool
condition_wait(struct condition *condition)
{
	assert(condition);
	debug_flags_assert(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return pthread_cond_wait(&condition->cond, &condition->lock->mutex);
}

static inline bool
condition_signal(struct condition *condition)
{
	assert(condition);
	debug_flags_assert(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return pthread_cond_signal(&condition->cond);
}

static inline bool
condition_broadcast(struct condition *condition)
{
	assert(condition);
	debug_flags_assert(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return pthread_cond_broadcast(&condition->cond);
}
