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

#include "lib_logging.h"
#include "ut_utils.h"
#include "s_tarr.h"

/******************************************************************************
 * Definition of array sizes. The "from" array is smaller than the "to" array.
 *****************************************************************************/

#define UT_ROWS_TO 3
#define UT_COLS_TO 3

#define UT_ROWS_FROM 2
#define UT_COLS_FROM 2

/******************************************************************************
 * Definition of various constants that are used in the following tests.
 *****************************************************************************/

static const s_tchar C_TO = { .chr = L'T', .fg = 1, .bg = 2 };

static const s_tchar C_FROM = { .chr = L'F', .fg = 3, .bg = 4 };

static const s_point POINT_0_0 = { .row = 0, .col = 0 };

static const s_point POINT_1_1 = { .row = 1, .col = 1 };

static const s_point DIM_FROM = { .row = UT_ROWS_FROM, .col = UT_COLS_FROM };

/******************************************************************************
 * The function processes and checks the freeing of the s_tarr.
 *****************************************************************************/

static void ut_check_free(s_tarr *tarr, const char *msg) {

	s_tarr_free(&tarr);

	ut_check_bool(tarr == NULL, true, msg);
}

/******************************************************************************
 * The function checks a s_tchar structure.
 *****************************************************************************/

static void ut_check_s_tchar(const s_tchar *current, const s_tchar *expected, const char *msg) {

	log_debug("Check: %s", msg);

	ut_check_wchar_t(current->chr, expected->chr, "chr");
	ut_check_int(current->fg, expected->fg, "fg");
	ut_check_int(current->bg, expected->bg, "bg");
}

/******************************************************************************
 * Test function that checks the s_tarr values with a simple array.
 *****************************************************************************/

static void ut_check_arrays(s_tarr *tarr, s_tchar arr[UT_ROWS_TO][UT_COLS_TO], const char *msg) {

	for (int row = 0; row < UT_ROWS_TO; row++) {
		for (int col = 0; col < UT_COLS_TO; col++) {

			ut_check_s_tchar(&s_tarr_get(tarr, row, col), &arr[row][col], msg);
		}
	}
}

/******************************************************************************
 * The function checks the s_tarr_del() functions.
 *****************************************************************************/

static void test_s_tarr_del() {

	//
	// Initialize s_tarr
	//
	s_tarr *tarr = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);
	s_tarr_set(tarr, C_TO);

	//
	// Call: s_tarr_del()
	//
	s_tarr_del(tarr, DIM_FROM, POINT_1_1);

	//
	// Define the array for the checks
	//
	s_tchar arr[UT_ROWS_TO][UT_COLS_TO] = {

	{ C_TO, C_TO, C_TO },

	{ C_TO, S_TCHAR_UNUSED, S_TCHAR_UNUSED },

	{ C_TO, S_TCHAR_UNUSED, S_TCHAR_UNUSED },

	};

	//
	// Do the check
	//
	ut_check_arrays(tarr, arr, "test_set_del");

	//
	// Free
	//
	ut_check_free(tarr, "test_set_del");
}

/******************************************************************************
 * The function checks the s_tarr_set_gradient function.
 *****************************************************************************/

static void test_s_tarr_set_gradient() {

	s_tarr *tarr = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);

	//
	// Call: s_tarr_set_gradient()
	//
	const short bg_colors[UT_ROWS_TO] = { 1, 2, 3 };

	s_tarr_set_gradient(tarr, L'A', 1, bg_colors);

	//
	// Define the array for the checks
	//
	s_tchar arr[UT_ROWS_TO][UT_COLS_TO];

	arr[0][0] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 1 };
	arr[0][1] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 1 };
	arr[0][2] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 1 };

	arr[1][0] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 2 };
	arr[1][1] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 2 };
	arr[1][2] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 2 };

	arr[2][0] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 3 };
	arr[2][1] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 3 };
	arr[2][2] = (s_tchar ) { .chr = L'A', .fg = 1, .bg = 3 };

	//
	// Do the check
	//
	ut_check_arrays(tarr, arr, "test_s_tarr_set_gradient");

	//
	// Free
	//
	ut_check_free(tarr, "test_s_tarr_set_gradient");
}

/******************************************************************************
 * The function checks the s_tarr_cp function.
 *****************************************************************************/

static void test_s_tarr_cp() {

	//
	// Initialize the two s_tarr's
	//
	s_tarr *to = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);
	s_tarr_set(to, C_TO);

	s_tarr *from = s_tarr_new(UT_ROWS_FROM, UT_COLS_FROM);
	s_tarr_set(from, C_FROM);

	//
	// Call: s_tarr_cp()
	//
	s_tarr_cp(to, from, POINT_1_1);

	//
	// Define the array for the checks
	//
	s_tchar arr[UT_ROWS_TO][UT_COLS_TO] = {

	{ C_TO, C_TO, C_TO },

	{ C_TO, C_FROM, C_FROM },

	{ C_TO, C_FROM, C_FROM },

	};

	//
	// Do the check
	//
	ut_check_arrays(to, arr, "test_s_tarr_cp");

	//
	// Free
	//
	ut_check_free(to, "test_s_tarr_cp");
	ut_check_free(from, "test_s_tarr_cp");
}

/******************************************************************************
 * The function checks the s_tarr_set_bg() function;
 *****************************************************************************/

static void test_s_tarr_set_bg() {

	//
	// Initialize the s_tarr
	//
	s_tarr *tarr = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);
	s_tarr_set(tarr, C_TO);

	const short bg_colors[UT_ROWS_FROM] = { 3, 4 };

	//
	// Call: s_tarr_set_bg() - normal
	//
	s_tarr_set_bg(tarr, POINT_1_1, DIM_FROM, bg_colors, false);

	//
	// Define the array for the checks
	//
	s_tchar arr[UT_ROWS_TO][UT_COLS_TO];

	arr[0][0] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 2 };
	arr[0][1] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 2 };
	arr[0][2] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 2 };

	arr[1][0] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 2 };
	arr[1][1] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 3 };
	arr[1][2] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 3 };

	arr[2][0] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 2 };
	arr[2][1] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 4 };
	arr[2][2] = (s_tchar ) { .chr = L'T', .fg = 1, .bg = 4 };

	//
	// Do the check
	//
	ut_check_arrays(tarr, arr, "test_s_tarr_set_bg");

	//
	// Call: s_tarr_set_bg() - reverse
	//
	s_tarr_set_bg(tarr, POINT_0_0, DIM_FROM, bg_colors, true);

	arr[0][0].bg = 4;
	arr[0][1].bg = 4;
	arr[1][0].bg = 3;
	arr[1][1].bg = 3;

	//
	// Do the check
	//
	ut_check_arrays(tarr, arr, "test_s_tarr_set_bg");

	//
	// Free
	//
	ut_check_free(tarr, "test_s_tarr_set_bg");
}

/******************************************************************************
 * The function checks the s_tarr_cp_fg() function.
 *****************************************************************************/

static void test_s_tarr_cp_fg() {

	//
	// Initialize the tarr's
	//
	s_tarr *to = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);
	s_tarr_set(to, C_TO);

	s_tarr *from = s_tarr_new(UT_ROWS_FROM, UT_COLS_FROM);
	s_tarr_set(from, C_FROM);

	//
	// Call: s_tarr_cp_fg()
	//
	s_tarr_cp_fg(to, from, POINT_1_1);

	//
	// Define the array for the checks
	//
	const s_tchar c_from = { .chr = C_FROM.chr, .fg = C_FROM.fg, .bg = C_TO.bg };

	s_tchar arr[UT_ROWS_TO][UT_COLS_TO] = {

	{ C_TO, C_TO, C_TO },

	{ C_TO, c_from, c_from },

	{ C_TO, c_from, c_from },

	};

	//
	// Do the check
	//
	ut_check_arrays(to, arr, "test_s_tarr_cp_fg");

	//
	// Free
	//
	ut_check_free(to, "test_s_tarr_cp_fg");
	ut_check_free(from, "test_s_tarr_cp_fg");
}

/******************************************************************************
 * The function checks the s_tarr_cp_pos() function.
 *
 *   012
 * 0 xx-
 * 1 xx-
 * 2 o--  <= next
 *
 *   012
 * 0 -o-  <= next
 * 1 -xx
 * 2 -xx
 *****************************************************************************/

static void test_s_tarr_cp_pos() {
	s_point pos;

	//
	// Initialize arrays
	//
	s_tarr *to = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);
	s_tarr_set(to, C_TO);

	s_tarr *from = s_tarr_new(UT_ROWS_FROM, UT_COLS_FROM);
	s_tarr_set(from, C_FROM);

	//
	// Call: s_tarr_cp_pos() - reverse: false
	//
	pos = s_tarr_cp_pos(to, from, POINT_0_0, false);
	ut_check_s_point(&pos, &(s_point ) { 2, 0 }, "test_s_tarr_cp_pos: point false");

	//
	// Define the array for the checks
	//
	s_tchar arr1[UT_ROWS_TO][UT_COLS_TO] = {

	{ C_FROM, C_FROM, C_TO },

	{ C_FROM, C_FROM, C_TO },

	{ C_TO, C_TO, C_TO },

	};

	//
	// Do the check
	//
	ut_check_arrays(to, arr1, "test_s_tarr_cp_pos: array false");

	//
	// Re-initialize the array
	//
	s_tarr_set(to, C_TO);

	//
	// Call: s_tarr_cp_pos() - reverse: true
	//
	pos = s_tarr_cp_pos(to, from, (s_point ) { 2, 1 }, true);
	ut_check_s_point(&pos, &(s_point ) { 0, 1 }, "test_s_tarr_cp_pos: point true");

	//
	// Define the array for the checks
	//
	s_tchar arr2[UT_ROWS_TO][UT_COLS_TO] = {

	{ C_TO, C_TO, C_TO },

	{ C_TO, C_FROM, C_FROM },

	{ C_TO, C_FROM, C_FROM },

	};

	//
	// Do the check
	//
	ut_check_arrays(to, arr2, "test_s_tarr_cp_pos: array true");

	//
	// Free
	//
	ut_check_free(to, "test_s_tarr_cp_pos");
	ut_check_free(from, "test_s_tarr_cp_pos");
}

/******************************************************************************
 * The function checks the s_tarr_ul_pos_get() function.
 *
 * 0
 * 1
 * 2
 * 3   1
 * 4   2
 * 5 1 3 <-
 * 6 2
 * 7 3
 * 8
 * 9
 *****************************************************************************/

static void test_s_tarr_ul_pos_get() {
	s_point result;

	s_tarr *tarr = s_tarr_new(UT_ROWS_TO, UT_COLS_TO);

	//
	// Reverse: false
	//
	result = s_tarr_ul_pos_get(tarr, (s_point ) { 5, 5 }, false);
	ut_check_s_point(&result, &(s_point ) { 5, 5 }, "test_s_tarr_ul_pos_get: reverse = false");

	//
	// Reverse: true
	//
	result = s_tarr_ul_pos_get(tarr, (s_point ) { 5, 5 }, true);
	ut_check_s_point(&result, &(s_point ) { 3, 5 }, "test_s_tarr_ul_pos_get: reverse = true");

	//
	// Free
	//
	ut_check_free(tarr, "test_s_tarr_ul_pos_get");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_tarr_exec() {

	test_s_tarr_del();

	test_s_tarr_set_gradient();

	test_s_tarr_cp();

	test_s_tarr_set_bg();

	test_s_tarr_cp_fg();

	test_s_tarr_cp_pos();

	test_s_tarr_ul_pos_get();
}
