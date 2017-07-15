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

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "flags.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)

# define DEBUG_FLAGS(n)\
	uint8_t _debug_flags[(n + 7) / 8];

# define DEBUG_FLAGS_ENUM(name, ...)\
	enum name \
	{\
		__VA_ARGS__\
	};

# define _debug_flags_assert_index_in_range(s, i)\
	assert(sizeof((s)->_debug_flags) >= (size_t)i/8 + 1)

# define debug_flags_initialize(s)\
	(assert(s), _flags_initialize((s)->_debug_flags, sizeof((s)->_debug_flags)))

# define debug_flags_assert(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_assert((s)->_debug_flags, i))

# define debug_flags_assert_not(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_assert_not((s)->_debug_flags, i))

# define debug_flags_set(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_set((s)->_debug_flags, i))

# define debug_flags_get(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_get((s)->_debug_flags, i))

# define debug_flags_clear(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_clear((s)->_debug_flags, i))

# define debug_flags_copy(d, s)\
	(assert(d), assert(s), _flags_copy((d)->_debug_flags, sizeof((d)->_debug_flags), (s)->_debug_flags, sizeof((s)->_debug_flags)))

# define debug_flags_compare(d, s)\
	(assert(d), assert(s), _flags_compare((d)->_debug_flags, sizeof((d)->_debug_flags), (s)->_debug_flags, sizeof((s)->_debug_flags)))

#else

# define DEBUG_FLAGS(...)
# define DEBUG_FLAGS_ENUM(...)

# define debug_flags_initialize(s)      do { s = s; } while (0)
# define debug_flags_assert(s, ...)     do { s = s; } while (0)
# define debug_flags_assert_not(s, ...) do { s = s; } while (0)
# define debug_flags_set(s, ...)        do { s = s; } while (0)
# define debug_flags_get(s, ...)        do { s = s; } while (0)
# define debug_flags_clear(s, ...)      do { s = s; } while (0)
# define debug_flags_copy(a, b)         do { a = a; b = b; } while (0)
# define debug_flags_compare(a, b)      do { a = a; b = b; } while (0)

#endif
