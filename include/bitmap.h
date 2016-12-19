/**
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This file is part of dsac (Data Structures and Alorithms for C).
 *
 * dsac is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * dsac is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsac. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

struct bitmap_guard
{
	void *bitmap;
	unsigned long bitmap_length;
};

void
bitmap_initialize(void * buffer, unsigned long buffer_length);

int
bitmap_set(void * buffer, unsigned long index);

int
bitmap_get(void const * buffer, unsigned long index);

int
bitmap_clear(void * buffer, unsigned long index);


void
bitmap_guard_initialize(struct bitmap_guard * guard, void * buffer,
		unsigned long buffer_length);

int
bitmap_guard_set(struct bitmap_guard * guard, unsigned long index);

int
bitmap_guard_get(struct bitmap_guard const * guard, unsigned long index);

int
bitmap_guard_clear(struct bitmap_guard * guard, unsigned long index);
