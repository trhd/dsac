/**
 * dsac -- Data Structures and Algorithms for C.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include "quicksort.h"

static void
swap(const void **a, const void **b)
{
	const void *t = *a;
	*a = *b;
	*b = t;
}

static size_t
partition(const void **d, size_t z, int (*cmp)(const void *, const void *))
{
	assert(d);
	assert(cmp);

	const void *p = d[0];
	size_t l = -1, h = z;

	for (;;)
	{
		while (cmp(p, d[--h]) < 0);
		while (cmp(p, d[++l]) > 0);

		if (l < h)
			swap(&d[h], &d[l]);
		else
			return h + 1;
	}
}

void
quicksort(const void **d, size_t z, int (*cmp)(const void *, const void *))
{
	assert(d);
	assert(cmp);

	size_t p;

	if (z > 1)
	{
		p = partition(d, z, cmp);
		quicksort(d, p, cmp);
		quicksort(d + p, z - p, cmp);
	}
}
