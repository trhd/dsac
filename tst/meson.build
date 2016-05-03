# dsac -- Data Structures and Algorithms for C.
# Copyright (C) 2016 Hemmo Nieminen
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

VALGRIND_ARGS = [ '--leak-check=full', '-v', '--show-leak-kinds=all', '--error-exitcode=1' ]

test(
	'avl_tree',
	executable(
		'avl_tree',
		'avl_tree.c',
		link_with : DSAC_LIB_AVL_TREE,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'splay_tree',
	executable(
		'splay_tree',
		'splay_tree.c',
		link_with : DSAC_LIB_SPLAY_TREE,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'bitmap',
	executable(
		'bitmap',
		'bitmap.c',
		link_with : DSAC_LIB_BITMAP,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'linked_list',
	executable(
		'linked_list',
		'linked_list.c',
		link_with : DSAC_LIB_LINKED_LIST,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'doubly_linked_list',
	executable(
		'doubly_linked_list',
		'doubly_linked_list.c',
		link_with : DSAC_LIB_DOUBLY_LINKED_LIST,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'lifo',
	executable(
		'lifo',
		'lifo.c',
		link_with : DSAC_LIB_LIFO,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'fifo',
	executable(
		'fifo',
		'fifo.c',
		link_with : DSAC_LIB_FIFO,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)

test(
	'quicksort',
	executable(
		'quicksort',
		'quicksort.c',
		link_with : DSAC_LIB_QUICKSORT,
		include_directories : DSAC_INCLUDE
	),
	valgrind_args : VALGRIND_ARGS
)
