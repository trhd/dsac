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
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include "atomic_debug_flags.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)

# include <pthread.h>

static unsigned long
get_thread_identifier()
{
	return pthread_self();
}

# define SPINLOCK_INITIALIZER \
	{ \
		.locked = ATOMIC_VAR_INIT(0), \
		._atomic_debug_flags = { 1 << SPINLOCK_DEBUG_FLAG_INITIALIZED } \
	}

#else

# define SPINLOCK_INITIALIZER \
	{ \
		.locked = ATOMIC_FLAG_INIT \
	}

#endif

ATOMIC_DEBUG_FLAGS_ENUM(
	spinlock_debug_flag,
	SPINLOCK_DEBUG_FLAG_INITIALIZED,
	_SPINLOCK_DEBUG_FLAG_COUNT
)

struct spinlock
{
#if !defined(NDEBUG) || defined(UNIT_TESTING)
	atomic_ulong locked;
#else
	atomic_flag locked;
#endif
	ATOMIC_DEBUG_FLAGS(_SPINLOCK_DEBUG_FLAG_COUNT)
};

static inline
void
spinlock_initialize(struct spinlock * l)
{
	assert(l);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	atomic_init(&l->locked, 0);
#else
	memcpy(&l->locked, &(atomic_flag)ATOMIC_FLAG_INIT, sizeof(atomic_flag));
#endif

	atomic_debug_flags_initialize(l);
	atomic_debug_flags_set(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);
}

static inline
void
spinlock_uninitialize(struct spinlock * l)
{
	assert(l);
	atomic_debug_flags_assert(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);

	atomic_debug_flags_clear(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);
}

static inline
void
spinlock_acquire(struct spinlock * l)
{
	assert(l);
	atomic_debug_flags_assert(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	unsigned long const tid = get_thread_identifier();

	for (unsigned long old = 0 ; !atomic_compare_exchange_weak(&l->locked, &old, tid) ; old = 0)
		assert(old != tid);
#else
	while (atomic_flag_test_and_set_explicit(&l->locked, memory_order_acquire));
#endif

}

static inline
bool
spinlock_acquire_try(struct spinlock * l)
{
	assert(l);
	atomic_debug_flags_assert(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	unsigned long const tid = get_thread_identifier();
	unsigned long old = 0;
	bool rv = !atomic_compare_exchange_strong(&l->locked, &old, get_thread_identifier());
	printf("rv=%s, old=%lu, tid=%lu\n", rv ? "true" : "false", old, tid);
	assert(!rv || old != tid);
	return rv;
#else
	return atomic_flag_test_and_set_explicit(&l->locked, memory_order_acquire);
#endif

}

#if defined(NDEBUG) && !defined(UNIT_TESTING)
# define spinlock_assert(ptr)
#else

static inline
void
spinlock_assert(struct spinlock * l)
{
	assert(l);
	atomic_debug_flags_assert(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);

	assert(get_thread_identifier() == atomic_load(&l->locked));
}

#endif

static inline
void
spinlock_release(struct spinlock * l)
{
	assert(l);
	atomic_debug_flags_assert(l, SPINLOCK_DEBUG_FLAG_INITIALIZED);
	spinlock_assert(l);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	atomic_store(&l->locked, 0);
#else
	atomic_flag_clear_explicit(&l->locked, memory_order_release);
#endif
}
