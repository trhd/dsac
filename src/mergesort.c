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
#include "mergesort.h"

static void
ptrcpy(void const ** to, void const ** from, unsigned int count)
{
	while (count --> 0)
		*to++ = *from++;
}

void
mergesort(void const ** data, unsigned int const size, int (* const cmp) (void const *, void const *))
{
	assert(data);
	assert(size);
	assert(cmp);

	unsigned int const split = size / 2;
	void const * tmp[split];

	if (size == 1)
		return;

	if (size > 2)
	{
		mergesort(data, split, cmp);
		mergesort(data + split, size - split, cmp);
	}

	ptrcpy(tmp, data, split);

	for (unsigned int i = 0, j = split, k = 0; i < split ; )
	{
		if (j == size)
		{
			while (i < split)
				data[k++] = tmp[i++];
			break;
		}

		if (cmp(tmp[i], data[j]) < 0)
			data[k++] = tmp[i++];
		else
			data[k++] = data[j++];
	}
}
