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
#include "s_area.h"

/******************************************************************************
 * The function checks the s_area_is_inside() function. Area:
 *
 * 012345678
 * 1
 * 2  xxxxx
 * 3  xxxxx
 * 4  xxxxx
 * 5  xxxxx
 * 6
 *****************************************************************************/

static void test_s_area_is_inside() {
	const s_area area = { .pos.row = 2, .pos.col = 3, .dim.row = 4, .dim.col = 5 };

	bool result;

	result = s_area_is_inside(&area, (s_point ) { 1, 2 });
	ut_check_bool(result, false, "1");

	result = s_area_is_inside(&area, (s_point ) { 1, 3 });
	ut_check_bool(result, false, "2");

	result = s_area_is_inside(&area, (s_point ) { 2, 1 });
	ut_check_bool(result, false, "3");

	result = s_area_is_inside(&area, (s_point ) { 2, 3 });
	ut_check_bool(result, true, "4");

	result = s_area_is_inside(&area, (s_point ) { 5, 3 });
	ut_check_bool(result, true, "5");

	result = s_area_is_inside(&area, (s_point ) { 6, 3 });
	ut_check_bool(result, false, "6");

	result = s_area_is_inside(&area, (s_point ) { 2, 7 });
	ut_check_bool(result, true, "7");

	result = s_area_is_inside(&area, (s_point ) { 2, 8 });
	ut_check_bool(result, false, "8");

	result = s_area_is_inside(&area, (s_point ) { 5, 7 });
	ut_check_bool(result, true, "9");

	result = s_area_is_inside(&area, (s_point ) { 6, 8 });
	ut_check_bool(result, false, "0");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_area_exec() {

	test_s_area_is_inside();
}
