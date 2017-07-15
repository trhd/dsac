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

#if defined(SEARCH_STRUCTURE_AVL)
#include "search_structure-avl.h"
#elif defined(SEARCH_STRUCTURE_SPLAY)
#include "search_structure-splay.h"
#endif

#ifndef SEARCH_STRUCTURE_ELEMENT
#error "Macro SEARCH_STRUCTURE_ELEMENT has not been defined."
#endif
#ifndef SEARCH_STRUCTURE_ELEMENT_INITIALIZE
#error "Macro SEARCH_STRUCTURE_ELEMENT_INITIALIZE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_ELEMENT_DEFINE
#error "Macro SEARCH_STRUCTURE_ELEMENT_DEFINE has not been defined."
#endif
#ifndef SEARCH_STRUCTURE_ELEMENT_VALUE
#error "Macro SEARCH_STRUCTURE_ELEMENT_VALUE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE
#error "Macro SEARCH_STRUCTURE_META has not been defined."
#endif
#ifndef SEARCH_STRUCTURE_INITIALIZE
#error "Macro SEARCH_STRUCTURE_META_INITIALIZE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_INSERT
#error "Macro SEARCH_STRUCTURE_INSERT has not been defined."
#endif
#ifndef SEARCH_STRUCTURE_REMOVE
#error "Macro SEARCH_STRUCTURE_REMOVE has not been defined."
#endif
#ifndef SEARCH_STRUCTURE_FIND
#error "Macro SEARCH_STRUCTURE_FIND has not been defined."
#endif
