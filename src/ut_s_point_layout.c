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
 *
 *****************************************************************************/

void test_s_point_layout_is_half() {
	bool result;

	//
	// Uncompressed
	//
	result = s_point_layout_is_half(s_point_layout_get(3, E_UNCOMP), 2);
	ut_check_bool(result, false, "3-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_UNCOMP), 2);
	ut_check_bool(result, true, "9-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_UNCOMP), 8);
	ut_check_bool(result, false, "9-8");

	//
	// Compressed is the same
	//
	result = s_point_layout_is_half(s_point_layout_get(3, E_COMP), 2);
	ut_check_bool(result, false, "3-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_COMP), 2);
	ut_check_bool(result, true, "9-2");

	result = s_point_layout_is_half(s_point_layout_get(9, E_COMP), 8);
	ut_check_bool(result, false, "9-8");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_point_layout_exec() {

	test_s_point_layout_is_half();
}
