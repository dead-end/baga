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
 * The function checks the ls_strs_dim() function.
 *****************************************************************************/

static void test_ls_strs_dim() {
	const char *str[] = { "1", "12", "123", "1234", "1", NULL };

	const s_point dim = ls_strs_dim(str);

	ut_check_s_point(&dim, &(s_point ) { 5, 4 }, "test_strs_dim");
}

/******************************************************************************
 * The function tests the trimming function. Remember that the ls_trim()
 * function changes the parameter, so you cannot use a literal string as an
 * argument.
 *
 * char chr[] = "test"; // OK
 * char *chr  = "test"; // does not work
 *****************************************************************************/

#define STR "hallo"
#define WHITE " \t\n "

static void test_ls_trim() {

	char chr1[] = STR WHITE;
	const char *tr1 = ls_trim(chr1);
	ut_check_char_str(tr1, STR, "trim-end");

	char chr2[] = WHITE STR;
	const char *tr2 = ls_trim(chr2);
	ut_check_char_str(tr2, STR, "trim-start");

	char chr3[] = WHITE STR WHITE;
	const char *tr3 = ls_trim(chr3);
	ut_check_char_str(tr3, STR, "trim-both");
}

/******************************************************************************
 * The function checks the ls_cpy_centered() function, which copies the source
 * string centered to the destination string.
 *****************************************************************************/

#define BUF_SIZE_1 9
#define BUF_SIZE_2 10

static void test_ls_cpy_centered() {
	const char *from_1 = "1234";
	const char *from_2 = "123";

	char to_1[BUF_SIZE_1];
	char to_2[BUF_SIZE_2];

	//
	// strlen(dst) - strlen(src) odd
	//
	ls_cpy_centered(to_1, BUF_SIZE_1, from_1);
	ut_check_char_str(to_1, "  1234  ", "check center 1");

	//
	// strlen(dst) - strlen(src) even
	//
	ls_cpy_centered(to_2, BUF_SIZE_2, from_1);
	ut_check_char_str(to_2, "  1234   ", "check center 2");

	//
	// strlen(dst) - strlen(src) even
	//
	ls_cpy_centered(to_1, BUF_SIZE_1, from_2);
	ut_check_char_str(to_1, "  123   ", "check center 3");

	//
	// strlen(dst) - strlen(src) odd
	//
	ls_cpy_centered(to_2, BUF_SIZE_2, from_2);
	ut_check_char_str(to_2, "   123   ", "check center 4");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_lib_string_exec() {

	test_ls_strs_dim();

	test_ls_trim();

	test_ls_cpy_centered();
}
