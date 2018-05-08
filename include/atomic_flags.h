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
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define ATOMIC_FLAGS(n)\
	atomic_int_least8_t _atomic_flags[(n + 7) / 8]

#define _atomic_flags_length(a) \
	sizeof(a)/sizeof(atomic_int_least8_t)

#define _atomic_flags_assert_index_in_range(s, i)\
	assert(_atomic_flags_length((s)->_atomic_flags) >= (size_t)i/8 + 1)

#define atomic_flags_initialize(s)\
	(assert(s), _atomic_flags_initialize((s)->_atomic_flags, _atomic_flags_length((s)->_atomic_flags)))

#define atomic_flags_set(s, i)\
	(assert(s), _atomic_flags_assert_index_in_range(s, i), _atomic_flags_set((s)->_atomic_flags, i))

#define atomic_flags_get(s, i)\
	(assert(s), _atomic_flags_assert_index_in_range(s, i), _atomic_flags_get((s)->_atomic_flags, i))

#define atomic_flags_clear(s, i)\
	(assert(s), _atomic_flags_assert_index_in_range(s, i), _atomic_flags_clear((s)->_atomic_flags, i))

#define atomic_flags_copy(d, s)\
	(assert(d), assert(s), _atomic_flags_copy((d)->_atomic_flags, _atomic_flags_length((d)->_atomic_flags), (s)->_atomic_flags, _atomic_flags_length((s)->_atomic_flags)))

#define atomic_flags_compare(a, b)\
	(assert(a), assert(b), _atomic_flags_compare((a)->_atomic_flags, _atomic_flags_length((a)->_atomic_flags), (b)->_atomic_flags, _atomic_flags_length((b)->_atomic_flags)))

static inline void
_atomic_flags_initialize(atomic_int_least8_t * flags, size_t len)
{
	assert(flags);
	assert(len);

	for (size_t i = 0 ; i < len ; i++)
		atomic_init(flags + i, 0);
}

static inline void
_atomic_flags_set(atomic_int_least8_t * flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	atomic_fetch_or(flags + f, 1 << b);
}

static inline bool
_atomic_flags_get(atomic_int_least8_t * flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	return atomic_load(flags + f) & (1 << b);
}

#if defined(NDEBUG) && !defined(UNIT_TESTING)

# define atomic_flags_assert(s, i)
# define atomic_flags_assert_not(s, i)

#else

#define atomic_flags_assert(s, i)\
	(assert((s)), _atomic_flags_assert_index_in_range(s, i), _atomic_flags_assert((s)->_atomic_flags, i))

static inline void
_atomic_flags_assert(atomic_int_least8_t * flags, unsigned int idx)
{
	assert(flags);
	assert(_atomic_flags_get(flags, idx));
}

#define atomic_flags_assert_not(s, i)\
	(assert((s)), _atomic_flags_assert_index_in_range(s, i), _atomic_flags_assert_not((s)->_atomic_flags, i))

static inline void
_atomic_flags_assert_not(atomic_int_least8_t * flags, unsigned int idx)
{
	assert(flags);
	assert(!_atomic_flags_get(flags, idx));
}

#endif

static inline void
_atomic_flags_clear(atomic_int_least8_t * flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	atomic_fetch_and(flags + f, ~(1 << b));
}

static inline void
_atomic_flags_copy(atomic_int_least8_t * dst_flags, size_t dst_len,
		atomic_int_least8_t * src_flags, size_t src_len)
{
	assert(dst_flags);
	assert(dst_len > 0);
	assert(src_flags);
	assert(src_len > 0);
	assert(dst_len >= src_len);

	for (size_t i = 0 ; i < src_len ; i++)
		atomic_init(dst_flags + i, atomic_load(src_flags + i));
}

static inline int
_atomic_flags_compare(atomic_int_least8_t * dst_flags, size_t dst_len,
		atomic_int_least8_t * src_flags, size_t src_len)
{
	assert(dst_flags);
	assert(dst_len > 0);
	assert(src_flags);
	assert(src_len > 0);
	assert(dst_len == src_len);

	for (size_t i = 0 ; i < src_len ; i++)
	{
		int a = atomic_load(src_flags + i),
		    b = atomic_load(dst_flags + i);

		if (a < b)
			return -1;
		else if (a > b)
			return 1;
	}

	return 0;
}
