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

#include "homogeneous_ring_buffer.h"

#ifndef TYPED_RING_BUFFER_CONFIG_TYPE
# error TYPED_RING_BUFFER_CONFIG_TYPE has not been defined.
#endif

#ifndef TYPED_RING_BUFFER_CONFIG_PREFIX
# error TYPED_RING_BUFFER_CONFIG_PREFIX has not been defined.
#endif

#define _CONCAT(a, b) a##b
#define _PREFIX(a, b) _CONCAT(a, b)
#define PREFIX(identifier) _PREFIX(TYPED_RING_BUFFER_CONFIG_PREFIX, identifier)

typedef struct homogeneous_ring_buffer PREFIX(_t);

static inline void
PREFIX(_initialize)(PREFIX(_t) * ring, TYPED_RING_BUFFER_CONFIG_TYPE * buffer, size_t count)
{
	homogeneous_ring_buffer_initialize((struct homogeneous_ring_buffer *)ring, buffer,
			sizeof(TYPED_RING_BUFFER_CONFIG_TYPE) * count, sizeof(TYPED_RING_BUFFER_CONFIG_TYPE));
}

static inline bool
PREFIX(_write)(PREFIX(_t) * ring, TYPED_RING_BUFFER_CONFIG_TYPE const * data)
{
	return homogeneous_ring_buffer_write((struct homogeneous_ring_buffer *)ring, data);
}

static inline void
PREFIX(_overwrite)(PREFIX(_t) * ring, TYPED_RING_BUFFER_CONFIG_TYPE const * data)
{
	homogeneous_ring_buffer_overwrite((struct homogeneous_ring_buffer *)ring, data);
}

static inline TYPED_RING_BUFFER_CONFIG_TYPE *
PREFIX(_allocate)(PREFIX(_t) * ring)
{
	return homogeneous_ring_buffer_allocate((struct homogeneous_ring_buffer *)ring);
}

static inline bool
PREFIX(_read)(PREFIX(_t) * ring, TYPED_RING_BUFFER_CONFIG_TYPE * data)
{
	return homogeneous_ring_buffer_read((struct homogeneous_ring_buffer *)ring, data);
}

static inline bool
PREFIX(_peek)(PREFIX(_t) const * ring, TYPED_RING_BUFFER_CONFIG_TYPE * data)
{
	return homogeneous_ring_buffer_peek((struct homogeneous_ring_buffer const *)ring, data);
}

static inline bool
PREFIX(_empty)(PREFIX(_t) const * ring)
{
	return homogeneous_ring_buffer_empty((struct homogeneous_ring_buffer const *)ring);
}

#undef _CONCAT
#undef _PREFIX
#undef PREFIX

#undef TYPED_RING_BUFFER_CONFIG_PREFIX
#undef TYPED_RING_BUFFER_CONFIG_TYPE
