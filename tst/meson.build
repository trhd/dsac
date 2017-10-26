# dsac -- Data Structures and Alorithms for C
# Copyright (C) 2016-2017 Hemmo Nieminen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

add_test_setup(
	'valgrind',
	exe_wrapper : [
		'valgrind',
			'--leak-check=full',
			'--track-origins=yes',
			'-v',
			'--show-leak-kinds=all',
			'--error-exitcode=1'
	],
	env : [ 'VALGRIND=1' ],
	timeout_multiplier : 20
)

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
			'-DSEARCH_STRUCTURE_BACKEND=avl',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	)
)

test('search_structure-splay',
	executable('search_structure-splay',
		'search_structure.c',
		link_with : TEST_LIB,
		dependencies : test_deps,
		include_directories : DSAC_INCLUDE,
		c_args : [
			'-DSEARCH_STRUCTURE_BACKEND=splay',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	)
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
		)
	)
endforeach
