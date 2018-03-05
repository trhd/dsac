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

#include "atomic_fifo.h"

#ifndef TYPED_ATOMIC_FIFO_CONFIG_TYPE
# error TYPED_ATOMIC_FIFO_CONFIG_TYPE has not been defined.
#endif

#ifndef TYPED_ATOMIC_FIFO_CONFIG_PREFIX
# error TYPED_ATOMIC_FIFO_CONFIG_PREFIX has not been defined.
#endif

#define _CONCAT(a, b) a##b
#define _PREFIX(a, b) _CONCAT(a, b)
#define PREFIX(identifier) _PREFIX(TYPED_ATOMIC_FIFO_CONFIG_PREFIX, identifier)

typedef struct atomic_fifo PREFIX(_t);

static inline void
PREFIX(_initialize)(PREFIX(_t) * fifo)
{
	atomic_fifo_initialize((struct atomic_fifo *)fifo);
}

static inline void
PREFIX(_set)(PREFIX(_t) * object, TYPED_ATOMIC_FIFO_CONFIG_TYPE * value)
{
	atomic_fifo_set((struct atomic_fifo *)object, value);
}

static inline TYPED_ATOMIC_FIFO_CONFIG_TYPE *
PREFIX(_get)(PREFIX(_t) * object)
{
	return (TYPED_ATOMIC_FIFO_CONFIG_TYPE*)atomic_fifo_get((struct atomic_fifo *)object);
}

static inline void
PREFIX(_push)(PREFIX(_t) * fifo, PREFIX(_t) * object)
{
	atomic_fifo_push((struct atomic_fifo *)fifo, (struct atomic_fifo *)object);
}

static inline PREFIX(_t) *
PREFIX(_pop)(PREFIX(_t) * fifo)
{
	return (PREFIX(_t)*)atomic_fifo_pop((struct atomic_fifo *)fifo);
}

#undef _CONCAT
#undef _PREFIX
#undef PREFIX

#undef TYPED_ATOMIC_FIFO_CONFIG_PREFIX
#undef TYPED_ATOMIC_FIFO_CONFIG_TYPE
