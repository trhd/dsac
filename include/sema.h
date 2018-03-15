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

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include "debug_flags.h"

DEBUG_FLAGS_ENUM(semaphore_debug_flag,
		SEMAPHORE_DEBUG_FLAG_INITIALIZED,
		_SEMAPHORE_DEBUG_FLAG_COUNT)

struct semaphore
{
	sem_t sem;
	DEBUG_FLAGS(_SEMAPHORE_DEBUG_FLAG_COUNT)
};

static inline void
semaphore_initialize(struct semaphore * s, unsigned int i)
{
	assert(s);
	//assert(i < SEM_VALUE_MAX);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert(!sem_init(&s->sem, 0, i));
#else
	sem_init(&s->sem, 0, i);
#endif

	debug_flags_set(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);
}

static inline void
semaphore_uninitialize(struct semaphore * s)
{
	assert(s);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert(!sem_destroy(&s->sem));
#else
	sem_destroy(&s->sem);
#endif

	debug_flags_clear(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);
}

static inline bool
semaphore_acquire_interruptible(struct semaphore * s)
{
	assert(s);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

	bool rv = sem_wait(&s->sem);

	assert(!rv || errno == EINTR);

	return rv;
}

static inline void
semaphore_acquire(struct semaphore * s)
{
	assert(s);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

	bool rv;

	do
		rv = semaphore_acquire_interruptible(s);
	while (rv && errno == EINTR);

	assert(!rv);
}

static inline bool
semaphore_acquire_try(struct semaphore * s)
{
	assert(s);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

	bool rv;

	do
		rv = sem_trywait(&s->sem);
	while (rv && errno == EINTR);

	return rv;
}

static inline bool
semaphore_acquire_timed(struct semaphore * s, struct timespec const * d)
{
	assert(s);
	assert(d);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

	bool rv;
	struct timespec c;

	clock_gettime(CLOCK_REALTIME, &c);

	c.tv_sec += d->tv_sec;
	c.tv_nsec = (c.tv_nsec + d->tv_nsec) % 1000000000;

	if (c.tv_nsec < d->tv_nsec)
		c.tv_sec += 1;

	do
		rv = sem_timedwait(&s->sem, &c);
	while (rv && errno == EINTR);

	assert(!rv || errno == ETIMEDOUT);

	return rv;
}

static inline void
semaphore_release(struct semaphore * s)
{
	assert(s);
	debug_flags_assert(s, SEMAPHORE_DEBUG_FLAG_INITIALIZED);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert(!sem_post(&s->sem));
#else
	sem_post(&s->sem);
#endif
}
