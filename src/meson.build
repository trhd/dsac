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

if get_option('buildtype') == 'release'
	release_flags = [ '-DNDEBUG' ]
else
	release_flags = []
endif

DSAC_LIB_AVL_TREE = static_library(
	'avl_tree',
	'avl_tree.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB_SPLAY_TREE = static_library(
	'splay_tree',
	'splay_tree.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB_BITMAP = static_library(
	'bitmap',
	'bitmap.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB_LINKED_LIST = static_library(
	'linked_list',
	'linked_list.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB_DOUBLY_LINKED_LIST = static_library(
	'doubly_linked_list',
	'doubly_linked_list.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB_LIFO = DSAC_LIB_DOUBLY_LINKED_LIST

DSAC_LIB_FIFO = DSAC_LIB_DOUBLY_LINKED_LIST

DSAC_LIB_QUICKSORT = static_library(
	'quicksort',
	'quicksort.c',
	include_directories : DSAC_INCLUDE,
	c_args : release_flags
)

DSAC_LIB = [
	DSAC_LIB_AVL_TREE,
	DSAC_LIB_SPLAY_TREE,
	DSAC_LIB_BITMAP,
	DSAC_LIB_LINKED_LIST,
	DSAC_LIB_DOUBLY_LINKED_LIST,
	# DSAC_LIB_LIFO == DSAC_LIB_DOUBLY_LINKED_LIST
	# DSAC_LIB_FIFO == DSAC_LIB_DOUBLY_LINKED_LIST
	DSAC_LIB_QUICKSORT
]

DSAC_LIB_SHARED = shared_library(
	'dsac',
	[
		'avl_tree.c',
		'bitmap.c',
		'doubly_linked_list.c',
		'linked_list.c',
		'splay_tree.c',
		'quicksort.c'
	],
	include_directories : DSAC_INCLUDE,
	version : meson.project_version(),
	install : true,
	c_args : release_flags,
	install : true
)
