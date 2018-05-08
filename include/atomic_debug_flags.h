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
#include <stdint.h>
#include <string.h>
#include "atomic_flags.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)

# define ATOMIC_DEBUG_FLAGS(n)\
	atomic_int_least8_t _atomic_debug_flags[(n + 7) / 8];

# define ATOMIC_DEBUG_FLAGS_ENUM(name, ...)\
	enum name \
	{\
		__VA_ARGS__\
	};

#define _atomic_debug_flags_length(a) \
	sizeof(a)/sizeof(atomic_int_least8_t)

# define _atomic_debug_flags_assert_index_in_range(s, i)\
	assert(_atomic_debug_flags_length((s)->_atomic_debug_flags) >= (size_t)i/8 + 1)

# define atomic_debug_flags_initialize(s)\
	(assert(s), _atomic_flags_initialize((s)->_atomic_debug_flags,_atomic_debug_flags_length((s)->_atomic_debug_flags)))

# define atomic_debug_flags_assert(s, i)\
	(assert(s), _atomic_debug_flags_assert_index_in_range(s, i), _atomic_flags_assert((s)->_atomic_debug_flags, i))

# define atomic_debug_flags_assert_not(s, i)\
	(assert(s), _atomic_debug_flags_assert_index_in_range(s, i), _atomic_flags_assert_not((s)->_atomic_debug_flags, i))

# define atomic_debug_flags_set(s, i)\
	(assert(s), _atomic_debug_flags_assert_index_in_range(s, i), _atomic_flags_set((s)->_atomic_debug_flags, i))

# define atomic_debug_flags_get(s, i)\
	(assert(s), _atomic_debug_flags_assert_index_in_range(s, i), _atomic_flags_get((s)->_atomic_debug_flags, i))

# define atomic_debug_flags_clear(s, i)\
	(assert(s), _atomic_debug_flags_assert_index_in_range(s, i), _atomic_flags_clear((s)->_atomic_debug_flags, i))

# define atomic_debug_flags_copy(d, s)\
	(assert(d), assert(s), _atomic_flags_copy((d)->_atomic_debug_flags,_atomic_debug_flags_length((d)->_atomic_debug_flags), (s)->_atomic_debug_flags,_atomic_debug_flags_length((s)->_atomic_debug_flags)))

# define atomic_debug_flags_compare(d, s)\
	(assert(d), assert(s), _atomic_flags_compare((d)->_atomic_debug_flags,_atomic_debug_flags_length((d)->_atomic_debug_flags), (s)->_atomic_debug_flags,_atomic_debug_flags_length((s)->_atomic_debug_flags)))

#else

# define ATOMIC_DEBUG_FLAGS(...)
# define ATOMIC_DEBUG_FLAGS_ENUM(...)

# define atomic_debug_flags_initialize(s)      do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_assert(s, ...)     do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_assert_not(s, ...) do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_set(s, ...)        do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_get(s, ...)        do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_clear(s, ...)      do { (void)sizeof(s); } while (0)
# define atomic_debug_flags_copy(a, b)         do { (void)sizeof(a); (void)sizeof(b); } while (0)
# define atomic_debug_flags_compare(a, b)      do { (void)sizeof(a); (void)sizeof(b); } while (0)

#endif
