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

#pragma once

#include "avl_tree.h"

#define SEARCH_STRUCTURE_ELEMENT struct avl_node
#define SEARCH_STRUCTURE_ELEMENT_INITIALIZE(...)

#define SEARCH_STRUCTURE_ELEMENT_DEFINE avl_set
#define SEARCH_STRUCTURE_ELEMENT_VALUE avl_get

#define SEARCH_STRUCTURE struct avl_tree
#define SEARCH_STRUCTURE_INITIALIZE avl_initialize

#define SEARCH_STRUCTURE_INSERT avl_insert
#define SEARCH_STRUCTURE_REMOVE avl_remove
#define SEARCH_STRUCTURE_FIND avl_find
