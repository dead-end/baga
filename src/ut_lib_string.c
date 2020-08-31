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

#include "ut_utils.h"
#include "lib_string.h"

#include <string.h>
#include <stdio.h>

/******************************************************************************
 * The function checks the strs_dim() function.
 *****************************************************************************/

static void test_strs_dim() {
	const char *str[] = { "1", "12", "123", "1234", "1", NULL };

	const s_point dim = strs_dim(str);

	ut_check_s_point(&dim, &(s_point ) { 5, 4 }, "test_strs_dim");
}

/******************************************************************************
 * The function tests the trimming function. Remember that the trim function
 * changes the parameter, so you cannot use a literal string as an argument.
 *
 * char chr[] = "test"; // OK
 * char *chr  = "test"; // does not work
 *****************************************************************************/

#define STR "hallo"
#define WHITE " \t\n "

static void test_trim() {

	char chr1[] = STR WHITE;
	const char *tr1 = trim(chr1);
	ut_check_char_str(tr1, STR, "trim-end");

	char chr2[] = WHITE STR;
	const char *tr2 = trim(chr2);
	ut_check_char_str(tr2, STR, "trim-start");

	char chr3[] = WHITE STR WHITE;
	const char *tr3 = trim(chr3);
	ut_check_char_str(tr3, STR, "trim-both");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_lib_string_exec() {

	test_strs_dim();

	test_trim();
}
