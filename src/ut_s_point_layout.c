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

#include "s_point_layout.h"

/******************************************************************************
 * The function checks the s_point_layout_not_visible() macro.
 *****************************************************************************/

static void test_s_point_layout_not_visible() {
	bool result;

	//
	// 9 - uncompressed => first 9 are visible
	//
	result = s_point_layout_not_visible(s_point_layout_get(9, E_UNCOMP), 0);
	ut_check_bool(result, false, "not_visible-9-0-c");

	result = s_point_layout_not_visible(s_point_layout_get(9, E_UNCOMP), 8);
	ut_check_bool(result, false, "not_visible-9-8-c");

	//
	// 9 - compressed => first 8 are visible
	//
	result = s_point_layout_not_visible(s_point_layout_get(9, E_COMP), 0);
	ut_check_bool(result, false, "not_visible-9-0-u");

	result = s_point_layout_not_visible(s_point_layout_get(9, E_COMP), 7);
	ut_check_bool(result, false, "not_visible-9-7-u");

	result = s_point_layout_not_visible(s_point_layout_get(9, E_COMP), 8);
	ut_check_bool(result, true, "not_visible-9-8-u");

	//
	// 16 - uncompressed => first 9 are visible
	//
	result = s_point_layout_not_visible(s_point_layout_get(16, E_UNCOMP), 0);
	ut_check_bool(result, false, "not_visible-16-0-c");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_UNCOMP), 8);
	ut_check_bool(result, false, "not_visible-16-8-c");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_UNCOMP), 9);
	ut_check_bool(result, true, "not_visible-16-9-c");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_UNCOMP), 15);
	ut_check_bool(result, true, "not_visible-16-15-c");

	//
	// 16 - compressed => first 8 are visible
	//
	result = s_point_layout_not_visible(s_point_layout_get(16, E_COMP), 0);
	ut_check_bool(result, false, "not_visible-16-0-u");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_COMP), 7);
	ut_check_bool(result, false, "not_visible-16-7-u");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_COMP), 8);
	ut_check_bool(result, true, "not_visible-16-8-u");

	result = s_point_layout_not_visible(s_point_layout_get(16, E_COMP), 15);
	ut_check_bool(result, true, "not_visible-16-15-u");
}

/******************************************************************************
 * The function checks the s_point_layout_has_label() macro.
 *****************************************************************************/

static void test_s_point_layout_has_label() {
	bool result;

	//
	// 5 - uncompressed => no label
	//
	result = s_point_layout_has_label(s_point_layout_get(5, E_UNCOMP), 0);
	ut_check_bool(result, false, "has_label-5-0-u");

	result = s_point_layout_has_label(s_point_layout_get(5, E_UNCOMP), 4);
	ut_check_bool(result, false, "has_label-5-4-u");

	//
	// 5 - compressed => no label
	//
	result = s_point_layout_has_label(s_point_layout_get(5, E_COMP), 0);
	ut_check_bool(result, false, "has_label-5-0-c");

	result = s_point_layout_has_label(s_point_layout_get(5, E_COMP), 4);
	ut_check_bool(result, false, "has_label-5-4-c");

	//
	// 6 - uncompressed => 5 label
	//
	result = s_point_layout_has_label(s_point_layout_get(6, E_UNCOMP), 4);
	ut_check_bool(result, false, "has_label-6-4-u");

	result = s_point_layout_has_label(s_point_layout_get(6, E_UNCOMP), 5);
	ut_check_bool(result, true, "has_label-6-5-u");

	//
	// 6 - compressed => 5 label
	//
	result = s_point_layout_has_label(s_point_layout_get(6, E_COMP), 4);
	ut_check_bool(result, false, "has_label-6-4-c");

	result = s_point_layout_has_label(s_point_layout_get(6, E_COMP), 5);
	ut_check_bool(result, true, "has_label-6-5-c");

	//
	// 16 - uncompressed => 9 label
	//
	result = s_point_layout_has_label(s_point_layout_get(16, E_UNCOMP), 7);
	ut_check_bool(result, false, "has_label-16-7-u");

	result = s_point_layout_has_label(s_point_layout_get(16, E_UNCOMP), 8);
	ut_check_bool(result, true, "has_label-16-8-u");

	result = s_point_layout_has_label(s_point_layout_get(16, E_UNCOMP), 9);
	ut_check_bool(result, false, "has_label-16-9-u");

	//
	// 16 - compressed => 8 label
	//
	result = s_point_layout_has_label(s_point_layout_get(16, E_COMP), 6);
	ut_check_bool(result, false, "has_label-16-6-c");

	result = s_point_layout_has_label(s_point_layout_get(16, E_COMP), 7);
	ut_check_bool(result, true, "has_label-16-7-c");

	result = s_point_layout_has_label(s_point_layout_get(16, E_COMP), 8);
	ut_check_bool(result, false, "has_label-16-8-c");
}

/******************************************************************************
 * The function checks the s_point_layout_is_half() macro.
 *****************************************************************************/

static void test_s_point_layout_is_half() {
	bool result;

	//
	// Uncompressed
	//
	result = s_point_layout_is_half(s_point_layout_get(3, E_UNCOMP), 2);
	ut_check_bool(result, false, "is_half-3-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_UNCOMP), 2);
	ut_check_bool(result, true, "is_half-9-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_UNCOMP), 8);
	ut_check_bool(result, false, "is_half-9-8");

	//
	// Compressed is the same
	//
	result = s_point_layout_is_half(s_point_layout_get(3, E_COMP), 2);
	ut_check_bool(result, false, "is_half-3-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_COMP), 2);
	ut_check_bool(result, true, "is_half-9-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_COMP), 8);
	ut_check_bool(result, false, "is_half-9-8");
}

/******************************************************************************
 * The function checks the s_point_layout_color_idx() function.
 *****************************************************************************/

static void test_s_point_layout_color_idx() {
	int idx;

	//
	// 3 total uncompressed
	//
	idx = s_point_layout_color_idx(s_point_layout_get(3, E_UNCOMP), 0);
	ut_check_int(idx, 3, "color_idx-3-0-u");

	idx = s_point_layout_color_idx(s_point_layout_get(3, E_UNCOMP), 1);
	ut_check_int(idx, 2, "color_idx-3-1-u");

	idx = s_point_layout_color_idx(s_point_layout_get(3, E_UNCOMP), 2);
	ut_check_int(idx, 1, "color_idx-3-2-u");

	//
	// 3 total compressed
	//
	idx = s_point_layout_color_idx(s_point_layout_get(3, E_COMP), 0);
	ut_check_int(idx, 3, "color_idx-3-0-c");

	idx = s_point_layout_color_idx(s_point_layout_get(3, E_COMP), 1);
	ut_check_int(idx, 2, "color_idx-3-1-c");

	idx = s_point_layout_color_idx(s_point_layout_get(3, E_COMP), 2);
	ut_check_int(idx, 1, "color_idx-3-2-c");

	//
	// 9 total compressed => all visible
	//
	idx = s_point_layout_color_idx(s_point_layout_get(9, E_UNCOMP), 0);
	ut_check_int(idx, 9, "color_idx-9-0-u");

	idx = s_point_layout_color_idx(s_point_layout_get(9, E_UNCOMP), 8);
	ut_check_int(idx, 1, "color_idx-9-8-u");

	// 9 total uncompressed => 8 visible
	//
	idx = s_point_layout_color_idx(s_point_layout_get(9, E_COMP), 0);
	ut_check_int(idx, 8, "color_idx-9-0-c");

	idx = s_point_layout_color_idx(s_point_layout_get(9, E_COMP), 7);
	ut_check_int(idx, 1, "color_idx-9-7-c");

	//
	// 16 total uncompressed => 9 are visible
	//
	idx = s_point_layout_color_idx(s_point_layout_get(16, E_UNCOMP), 0);
	ut_check_int(idx, 9, "color_idx-16-0-u");

	idx = s_point_layout_color_idx(s_point_layout_get(16, E_UNCOMP), 8);
	ut_check_int(idx, 1, "color_idx-16-8-u");

	//
	// 16 total compressed => 8 are visible
	//
	idx = s_point_layout_color_idx(s_point_layout_get(16, E_COMP), 0);
	ut_check_int(idx, 8, "color_idx-16-0-c");

	idx = s_point_layout_color_idx(s_point_layout_get(16, E_COMP), 7);
	ut_check_int(idx, 1, "color_idx-16-7-c");

}

/******************************************************************************
 * The function checks the s_tmpl_checker_point_area() function. (This checks
 * the assumptions not the implementation).
 *****************************************************************************/

#define AREA_ROW (CHECKER_ROW * (CHECK_DIS_FULL + 1))

static void test_s_point_layout_get_area() {
	s_area area;

	area = s_point_layout_get_area((s_point ) { 1, 1 }, true);

	ut_check_s_point(&area.dim, &(s_point ) { AREA_ROW, CHECKER_COL }, "dim 1/1 upper");
	ut_check_s_point(&area.pos, &(s_point ) { 1, 1 }, "pos 1/1 upper");

	//
	// Example: area-row: 3 pos: 4
	//
	// 0
	// 1 <- pos-row - area-row
	// ---------------------------
	// 2 <- pos-row - area-row + 1
	// 3
	// 4 <- pos-row
	// ---------------------------
	// 5
	//
	area = s_point_layout_get_area((s_point ) { 20, 20 }, false);

	ut_check_s_point(&area.dim, &(s_point ) { AREA_ROW, CHECKER_COL }, "dim 20/20 lower");
	ut_check_s_point(&area.pos, &(s_point ) { 9, 20 }, "pos 20/20 lower");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_point_layout_exec() {

	test_s_point_layout_not_visible();

	test_s_point_layout_has_label();

	test_s_point_layout_is_half();

	test_s_point_layout_color_idx();

	test_s_point_layout_get_area();
}
