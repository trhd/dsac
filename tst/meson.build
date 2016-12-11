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

CMOCKA = meson.get_compiler('c').find_library('cmocka')
TEST_TIMEOUT = 900

test_deps = [ CMOCKA, THREADS ]
test_cflags = [
	'-DUNIT_TESTING',
	'-D_GNU_SOURCE',
	'-isystem', meson.current_source_dir() + '/system'
]

if get_option('buildtype') == 'release'
	test_cflags += [ '-DNDEBUG' ]
endif

VALGRIND_ARGS = [
	'--leak-check=full',
	'--track-origins=yes',
	'-v',
	'--show-leak-kinds=all',
	'--error-exitcode=1',
	'--max-threads=5000'
]

TEST_LIB = library(
	'dsactest',
	DSAC_LIB_SOURCES + [ 'cmocka-wrapper.c' ],
	include_directories : DSAC_INCLUDE,
	c_args : test_cflags,
	dependencies : test_deps
)

test('search_structure-avl',
	executable('search_structure-avl',
		'search_structure.c',
		link_with : TEST_LIB,
		dependencies : test_deps,
		include_directories : DSAC_INCLUDE,
		c_args : [
			'-DSEARCH_STRUCTURE_AVL',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	),
	timeout : TEST_TIMEOUT,
	valgrind_args : VALGRIND_ARGS
)

test('search_structure-splay',
	executable('search_structure-splay',
		'search_structure.c',
		link_with : TEST_LIB,
		dependencies : test_deps,
		include_directories : DSAC_INCLUDE,
		c_args : [
			'-DSEARCH_STRUCTURE_SPLAY',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	),
	timeout : TEST_TIMEOUT,
	valgrind_args : VALGRIND_ARGS
)

foreach t : [
		'avl_tree',
		'bitmap',
		'debug_flags',
		'blocking_ring_buffer',
		'blocking_homogeneous_ring_buffer',
		'doubly_linked_list',
		'condition',
		'fifo',
		'flags',
		'homogeneous_ring_buffer',
		'lifo',
		'linked_list',
		'lock',
		'quicksort',
		'ring_buffer',
		'splay_tree'
		]
	test(t,
		executable(t,
			'@0@.c'.format(t),
			link_with : TEST_LIB,
			c_args : test_cflags,
			dependencies : test_deps,
			include_directories : DSAC_INCLUDE,
		),
		timeout : TEST_TIMEOUT,
		valgrind_args : VALGRIND_ARGS
	)
endforeach
