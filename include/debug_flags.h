#pragma once

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "flags.h"

#ifdef NDEBUG

#define DEBUG_FLAGS(n)
#define DEBUG_FLAGS_ENUM(...)

#define debug_flags_initialize(...) ((void)0)
#define debug_flags_assert(...)     ((void)0)
#define debug_flags_assert_not(...) ((void)0)
#define debug_flags_set(...)        ((void)0)
#define debug_flags_get(...)        ((void)0)
#define debug_flags_clear(...)      ((void)0)
#define debug_flags_copy(...)       ((void)0)
#define debug_flags_compare(...)    ((void)0)

#else /** NDEBUG */

#define DEBUG_FLAGS(n)\
	uint8_t _debug_flags[(n + 7) / 8]

#define DEBUG_FLAGS_ENUM(name, ...)\
	enum name \
	{\
		__VA_ARGS__\
	}

#define _debug_flags_assert_index_in_range(s, i)\
	assert(sizeof((s)->_debug_flags) >= (size_t)i/8 + 1)

#define debug_flags_initialize(s)\
	(assert(s), _flags_initialize((s)->_debug_flags, sizeof((s)->_debug_flags)))

#define debug_flags_assert(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_assert((s)->_debug_flags, i))

#define debug_flags_assert_not(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_assert_not((s)->_debug_flags, i))

#define debug_flags_set(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_set((s)->_debug_flags, i))

#define debug_flags_get(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_get((s)->_debug_flags, i))

#define debug_flags_clear(s, i)\
	(assert(s), _debug_flags_assert_index_in_range(s, i), _flags_clear((s)->_debug_flags, i))

#define debug_flags_copy(d, s)\
	(assert(d), assert(s), _flags_copy((d)->_debug_flags, sizeof((d)->_debug_flags), (s)->_debug_flags, sizeof((s)->_debug_flags)))

#define debug_flags_compare(d, s)\
	(assert(d), assert(s), _flags_compare((d)->_debug_flags, sizeof((d)->_debug_flags), (s)->_debug_flags, sizeof((s)->_debug_flags)))

#endif /** NDEBUG */
