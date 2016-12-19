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

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "lock.h"

bool
lock_initialize(struct lock * l)
{
	assert(l);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	pthread_mutexattr_t t;

	if ((errno = pthread_mutexattr_init(&t))
			|| (errno = pthread_mutexattr_settype(&t, PTHREAD_MUTEX_ERRORCHECK))
			|| (errno = pthread_mutex_init(&l->mutex, &t)))
#else
	if ((errno = pthread_mutex_init(&l->mutex, NULL)))
#endif
		return true;

	debug_flags_initialize(l);
	debug_flags_set(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
lock_uninitialize(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_destroy(&l->mutex);

	if (errno)
		return true;

	debug_flags_clear(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
lock_acquire(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_lock(&l->mutex);

	assert(errno != EDEADLK);

	return errno;
}

bool
lock_acquire_try(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_trylock(&l->mutex);

	assert(errno != EDEADLK);

	return errno;
}

bool
lock_release(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return (errno = pthread_mutex_unlock(&l->mutex));
}


#if !defined(NDEBUG) || defined(UNIT_TESTING)
void
lock_assert(const struct lock * l __attribute__((unused)))
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	int e = pthread_mutex_lock(&((struct lock *)l)->mutex);

	if (e != EDEADLK)
	{
		if (!e)
			lock_release((struct lock *)l);
		abort();
	}
}
#endif
