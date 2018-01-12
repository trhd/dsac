# dsac -- Data Structures and Alorithms for C
# Copyright (C) 2016-2018 Hemmo Nieminen
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

DSAC_LIB_SOURCES = files(
		'avl_tree.c',
		'bitmap.c',
		'blocking_ring_buffer.c',
		'blocking_homogeneous_ring_buffer.c',
		'doubly_linked_list.c',
		'homogeneous_ring_buffer.c',
		'linked_list.c',
		'lock.c',
		'mergesort.c',
		'ring_buffer.c',
		'radix_tree.c',
		'quicksort.c',
		'splay_tree.c'
	)

THREADS = dependency('threads')

DSAC_LIB = library(
	'dsac',
	DSAC_LIB_SOURCES,
	include_directories : DSAC_INCLUDE,
	version : meson.project_version(),
	dependencies : THREADS,
	c_args : '-D_GNU_SOURCE',
	install : true
)
