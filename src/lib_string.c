/*
 * MIT License
 *
 * Copyright (c) 2020 dead-end
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>
#include <ctype.h>

#include "lib_utils.h"
#include "lib_logging.h"
#include "lib_s_point.h"

/******************************************************************************
 * The function is called with an array of strings, which is NULL terminated.
 * It computes the maximal string length and the number of rows.
 *
 * (Unit tested)
 *****************************************************************************/

s_point ls_strs_dim(const char *strs[]) {
	s_point dim = { .row = 0, .col = -1 };

	int col;

	for (int i = 0; strs[i] != NULL; i++) {
		col = strlen(strs[i]);
		if (col > dim.col) {
			dim.col = col;
		}
		dim.row++;
	}

	return dim;
}

/******************************************************************************
 * The function counts the items in a null terminated array of string.
 *****************************************************************************/

int ls_strs_count(const char *strs[]) {
	int num_items = 0;

	for (const char **ptr = strs; *ptr != NULL; ptr++, num_items++) {
		log_debug("Item: '%s'", *ptr);
	}

	return num_items;
}

/******************************************************************************
 * The function removes leading and tailing spaces. The process changes the
 * argument string. So do not call the function with a literal string.
 *
 * char chr[] = "test"; // OK
 * char *chr  = "test"; // does not work
 *
 * (Unit tested)
 *****************************************************************************/

char* ls_trim(char *str) {
	char *ptr;

	//
	// skip leading white spaces
	//
	for (ptr = str; isspace(*ptr); ptr++)
		;

	//
	// skip tailing white spaces by overwriting them with '\0'
	//
	const size_t len = strlen(ptr);
	for (int i = len - 1; i >= 0 && isspace(ptr[i]); i--) {
		ptr[i] = '\0';
	}

	return ptr;
}

/******************************************************************************
 * The function copies the source string centered to the target string.
 *
 * Example:
 *
 * - source string: 123
 * - target size: 9 (with \0)
 *
 * 012345678
 *   1234  \0
 *
 * (Unit tested)
 *****************************************************************************/

char* ls_cpy_centered(char *to, const int size, const char *from) {

	const int src_len = strlen(from);

#ifdef DEBUG

	//
	// Ensure that the string fits in the target.
	//
	if (src_len > size) {
		log_exit("String too long: %s", from);
	}
#endif

	const int start = lu_center(size - 1, src_len);
	const int end = start + src_len;

	//
	// Copy the source to the destination with the padding.
	//
	for (int i = 0; i < size - 1; i++) {

		if (i < start || i >= end) {
			to[i] = ' ';

		} else {
			to[i] = from[i - start];
		}
	}

	//
	// Add terminating \0 to the target
	//
	to[size - 1] = '\0';

	log_debug("source: '%s' target: '%s' start-idx: %d end-idx: %d source-len: %d size: %d", from, to, start, end, src_len, size);

	return to;
}
