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

#define _header(X) #X
# define header(X) _header(search_structure-X.h)
#  include header(SEARCH_STRUCTURE_BACKEND)
# undef header
#undef _header

#ifndef SEARCH_STRUCTURE_ELEMENT
# error "Macro SEARCH_STRUCTURE_ELEMENT has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_ELEMENT_INITIALIZE
# error "Macro SEARCH_STRUCTURE_ELEMENT_INITIALIZE has not been defined."
#endif


#ifndef SEARCH_STRUCTURE_ELEMENT_DEFINE
# error "Macro SEARCH_STRUCTURE_ELEMENT_DEFINE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_ELEMENT_VALUE
# error "Macro SEARCH_STRUCTURE_ELEMENT_VALUE has not been defined."
#endif


#ifndef SEARCH_STRUCTURE
# error "Macro SEARCH_STRUCTURE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_INITIALIZE
# error "Macro SEARCH_STRUCTURE_INITIALIZE has not been defined."
#endif


#ifndef SEARCH_STRUCTURE_INSERT
# error "Macro SEARCH_STRUCTURE_INSERT has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_REMOVE
# error "Macro SEARCH_STRUCTURE_REMOVE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_REMOVE_ANY
# error "Macro SEARCH_STRUCTURE_REMOVE_ANY has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_FIND
# error "Macro SEARCH_STRUCTURE_FIND has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_FIND_ANY
# error "Macro SEARCH_STRUCTURE_FIND_ANY has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_GET_ENTRY_COUNT
# error "Macro SEARCH_STRUCTURE_GET_ENTRY_COUNT has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_ITERATE
# error "Macro SEARCH_STRUCTURE_ITERATE has not been defined."
#endif

#ifndef SEARCH_STRUCTURE_IS_EMPTY
# error "Macro SEARCH_STRUCTURE_IS_EMPTY has not been defined."
#endif
