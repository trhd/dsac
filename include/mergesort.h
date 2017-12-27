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

/**
 * Sort an array of (data) pointers with the merge sort algorithm.
 *
 * Arguments:
 *
 *  data:     The array of pointers to be sorted.
 *  size:     The number of pointer in data.
 *  compare:  The comparison function used for sorting the pointers from the
 *            data argument. Consider providing an inlinable function here to
 *            improve the algorithm's performance.
 */
void mergesort(void const ** data, unsigned int size,
		int (*compare)(void const *, void const *));
