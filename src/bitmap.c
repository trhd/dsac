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

#include <assert.h>
#include <errno.h>
#include "bitmap.h"

static unsigned long
word_offset(unsigned long i)
{
	return i / 8;
}

static unsigned long
bit_offset(unsigned long i)
{
	return i % 8;
}

static void
memzero(void *b, unsigned long l)
{
	assert(b);
	assert(l);

	for ( ; l ; l--, b = (unsigned char*)b + 1)
		*(unsigned char*)b = 0;
}

void
bitmap_initialize(void *b, unsigned long l)
{
	assert(b);
	assert(l);

	memzero(b, l);
}

int
bitmap_set(void *b, unsigned long i)
{
	assert(b);

	unsigned char *w = &((unsigned char*)b)[word_offset(i)],
		t = *w;
	*w |= (unsigned char)1 << bit_offset(i);

	return *w == t ? 1 : 0;
}

int
bitmap_get(const void *b, unsigned long i)
{
	assert(b);

	return ((unsigned char*)b)[word_offset(i)] & ((unsigned char)1 << bit_offset(i)) ? 1 : 0;
}

int
bitmap_clear(void *b, unsigned long i)
{
	assert(b);

	unsigned char *w = &((unsigned char*)b)[word_offset(i)],
		t = *w;
	*w &= ~((unsigned char)1 << bit_offset(i));

	return *w != t ? 1 : 0;
}

void
bitmap_guard_initialize(struct bitmap_guard *g, void *b, unsigned long l)
{
	assert(g);
	assert(b);
	assert(l);

	g->bitmap = b;
	g->bitmap_length = l * 8;

	bitmap_initialize(g->bitmap, l);
}

static int
index_is_valid(const struct bitmap_guard *g, unsigned long i)
{
	assert(g);

	return i < g->bitmap_length;
}

int
bitmap_guard_set(struct bitmap_guard *g, unsigned long i)
{
	assert(g);

	if (!index_is_valid(g, i))
		return -EBADR;

	return bitmap_set(g->bitmap, i);
}

int
bitmap_guard_get(const struct bitmap_guard *g, unsigned long i)
{
	assert(g);

	if (!index_is_valid(g, i))
		return -EBADR;

	return bitmap_get(g->bitmap, i);
}

int
bitmap_guard_clear(struct bitmap_guard *g, unsigned long i)
{
	assert(g);

	if (!index_is_valid(g, i))
		return -EBADR;

	return bitmap_clear(g->bitmap, i);
}
