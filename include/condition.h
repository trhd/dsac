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
#include <errno.h>
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

	pthread_condattr_t attr;

	if (pthread_condattr_init(&attr))
		return true;

	if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
		return true;

	condition->lock = lock;
	if (pthread_cond_init(&condition->cond, &attr))
	{
		pthread_condattr_destroy(&attr);
		return true;
	}
	debug_flags_set(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	return pthread_condattr_destroy(&attr);
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
condition_timedwait(struct condition * condition, struct timespec const * delta)
{
	assert(condition);
	assert(delta);
	debug_flags_assert(condition, CONDITION_DEBUG_FLAG_INITIALIZED);

	int e;
	struct timespec abs;

	e = clock_gettime(CLOCK_MONOTONIC, &abs);
	assert(!e);

	abs.tv_sec += delta->tv_sec;

	assert(abs.tv_nsec < 1000 * 1000 * 1000);
	assert(delta->tv_nsec < 1000 * 1000 * 1000);
	abs.tv_nsec = (abs.tv_nsec + delta->tv_nsec) % (1000 * 1000 * 1000);
	if (abs.tv_nsec < delta->tv_nsec)
		abs.tv_sec++;

	assert(abs.tv_sec >= delta->tv_sec);

	e = pthread_cond_timedwait(&condition->cond, &condition->lock->mutex, &abs);

	assert(e != EINVAL);
	assert(e != EOWNERDEAD);
	assert(e != EPERM);
	assert(e != ENOTRECOVERABLE);
	assert(!e || e == ETIMEDOUT);

	return e;
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
