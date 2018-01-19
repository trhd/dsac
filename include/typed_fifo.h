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

#include "fifo.h"

#ifndef TYPED_FIFO_CONFIG_TYPE
# error TYPED_FIFO_CONFIG_TYPE has not been defined.
#endif

#ifndef TYPED_FIFO_CONFIG_PREFIX
# error TYPED_FIFO_CONFIG_PREFIX has not been defined.
#endif

#define _CONCAT(a, b) a##b
#define _PREFIX(a, b) _CONCAT(a, b)
#define PREFIX(identifier) _PREFIX(TYPED_FIFO_CONFIG_PREFIX, identifier)

typedef fifo_meta_t PREFIX(_meta_t);
typedef fifo_t PREFIX(_t);

static inline void
PREFIX(_initialize)(PREFIX(_meta_t) * fifo)
{
	fifo_initialize((fifo_meta_t*)fifo);
}

static inline bool
PREFIX(_is_empty)(PREFIX(_meta_t) const * fifo)
{
	return fifo_empty((fifo_meta_t*)fifo);
}

static inline TYPED_FIFO_CONFIG_TYPE *
PREFIX(_get)(PREFIX(_t) const * object)
{
	return fifo_get((fifo_t*)object);
}

static inline void
PREFIX(_set)(PREFIX(_t) * object, TYPED_FIFO_CONFIG_TYPE * value)
{
	fifo_set((fifo_t*)object, value);
}

static inline PREFIX(_t) const *
PREFIX(_peek)(PREFIX(_meta_t) * fifo)
{
	return (PREFIX(_t) const *)fifo_peek((fifo_meta_t*)fifo);
}

static inline PREFIX(_t) *
PREFIX(_pop)(PREFIX(_meta_t) * fifo)
{
	return (PREFIX(_t)*)fifo_pop((fifo_meta_t*)fifo);
}

static inline void
PREFIX(_push)(PREFIX(_meta_t) * fifo, PREFIX(_t) const * object)
{
	fifo_push((fifo_meta_t*)fifo, (fifo_t*)object);
}

static inline unsigned int
PREFIX(_get_size)(PREFIX(_meta_t) const * fifo)
{
	return fifo_size((fifo_meta_t*)fifo);
}

#undef _CONCAT
#undef _PREFIX
#undef PREFIX

#undef TYPED_FIFO_CONFIG_PREFIX
#undef TYPED_FIFO_CONFIG_TYPE
