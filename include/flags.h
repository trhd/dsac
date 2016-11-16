#pragma once

#include <assert.h>
#include <stdint.h>
#include <string.h>

#define FLAGS(n)\
	uint8_t _flags[(n + 7) / 8]

#define _flags_assert_index_in_range(s, i)\
	assert(sizeof((s)->_flags) >= (size_t)i/8 + 1)

#define flags_initialize(s)\
	(assert(s), _flags_initialize((s)->_flags, sizeof((s)->_flags)))

#define flags_set(s, i)\
	(assert(s), _flags_assert_index_in_range(s, i), _flags_set((s)->_flags, i))

#define flags_get(s, i)\
	(assert(s), _flags_assert_index_in_range(s, i), _flags_get((s)->_flags, i))

#define flags_clear(s, i)\
	(assert(s), _flags_assert_index_in_range(s, i), _flags_clear((s)->_flags, i))

#define flags_copy(d, s)\
	(assert(d), assert(s), _flags_copy((d)->_flags, sizeof((d)->_flags), (s)->_flags, sizeof((s)->_flags)))

#define flags_compare(a, b)\
	(assert(a), assert(b), _flags_compare((a)->_flags, sizeof((a)->_flags), (b)->_flags, sizeof((b)->_flags)))

static inline void
_flags_initialize(uint8_t *flags, size_t len)
{
	assert(flags);
	assert(len);

	memset(flags, 0, len);
}

static inline void
_flags_set(uint8_t *flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	*(flags + f) |= 1 << b;
}

static inline int
_flags_get(const uint8_t *flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	return *(flags + f) & (1 << b) ? 1 : 0;
}

#if !defined(NDEBUG) || defined(UNIT_TESTING)

#define flags_assert(s, i)\
	(assert((s)), _flags_assert_index_in_range(s, i), _flags_assert((s)->_flags, i))

static inline void
_flags_assert(const uint8_t *flags, unsigned int idx)
{
	assert(flags);

	assert(_flags_get(flags, idx));
}

#define flags_assert_not(s, i)\
	(assert((s)), _flags_assert_index_in_range(s, i), _flags_assert_not((s)->_flags, i))

static inline void
_flags_assert_not(const uint8_t *flags, unsigned int idx)
{
	assert(flags);

	assert(!_flags_get(flags, idx));
}

#else
#define flags_assert(s, i)
#define flags_assert_not(s, i)
#endif

static inline void
_flags_clear(uint8_t *flags, unsigned int idx)
{
	assert(flags);

	int b = idx % 8,
	    f = idx / 8;

	*(flags + f) &= ~(1 << b);
}

static inline void
_flags_copy(uint8_t *dst_flags, size_t dst_len,
		const uint8_t *src_flags, size_t src_len)
{
	assert(dst_flags);
	assert(dst_len > 0);
	assert(src_flags);
	assert(src_len > 0);
	assert(dst_len >= src_len);

	memcpy(dst_flags, src_flags, dst_len < src_len ? dst_len : src_len);
}

static inline int
_flags_compare(uint8_t *dst_flags, size_t dst_len,
		const uint8_t *src_flags, size_t src_len)
{
	assert(dst_flags);
	assert(dst_len > 0);
	assert(src_flags);
	assert(src_len > 0);
	assert(dst_len == src_len);

	return memcmp(dst_flags, src_flags, dst_len < src_len ? dst_len : src_len);
}
