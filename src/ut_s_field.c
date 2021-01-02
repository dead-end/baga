/*
 * MIT License
 *
 * Copyright (c) 2021 dead-end
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

#include "s_field.h"

/******************************************************************************
 * The function checks the s_field_rel_idx() calls.
 *****************************************************************************/

static void test_s_field_idx_rel() {
	s_status status;
	int idx_rel;

	status.up_2_down = OWNER_BLACK;

	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, 0);
	ut_check_int(idx_rel, 0, "black 0");

	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, 0);
	ut_check_int(idx_rel, 23, "white 0");

	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, 23);
	ut_check_int(idx_rel, 23, "black 23");

	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, 23);
	ut_check_int(idx_rel, 0, "white 23");

	//
	// rel(rel) = abs WHITE
	//
	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, 0);
	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, idx_rel);
	ut_check_int(idx_rel, 0, "white 0 - id");

	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, 23);
	idx_rel = s_field_idx_rel(&status, OWNER_WHITE, idx_rel);
	ut_check_int(idx_rel, 23, "white 23 - id");

	//
	// rel(rel) = abs BLACK
	//
	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, 0);
	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, idx_rel);
	ut_check_int(idx_rel, 0, "white 0 - id");

	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, 23);
	idx_rel = s_field_idx_rel(&status, OWNER_BLACK, idx_rel);
	ut_check_int(idx_rel, 23, "white 23 - id");
}

/******************************************************************************
 * The function checks the s_field_idx_is_out() calls.
 *****************************************************************************/

static void test_s_field_idx_is_out() {
	s_status status;
	bool result;

	status.up_2_down = OWNER_BLACK;

	result = s_field_idx_is_out(&status, OWNER_BLACK, 0);
	ut_check_bool(result, false, "black 0");

	result = s_field_idx_is_out(&status, OWNER_BLACK, 23);
	ut_check_bool(result, false, "black 23");

	result = s_field_idx_is_out(&status, OWNER_BLACK, 24);
	ut_check_bool(result, true, "black 24");

	result = s_field_idx_is_out(&status, OWNER_WHITE, 23);
	ut_check_bool(result, false, "white 23");

	result = s_field_idx_is_out(&status, OWNER_WHITE, 0);
	ut_check_bool(result, false, "white 0");

	result = s_field_idx_is_out(&status, OWNER_WHITE, -1);
	ut_check_bool(result, true, "white -1");

}

/******************************************************************************
 * The function checks the test_s_field_idx_add_abs() calls.
 *****************************************************************************/

static void test_s_field_idx_add_abs() {
	s_status status;
	int idx;

	status.up_2_down = OWNER_BLACK;

	idx = s_field_idx_add_abs(&status, OWNER_BLACK, 0, 5);
	ut_check_int(idx, 5, "black 0 - 5");

	idx = s_field_idx_add_abs(&status, OWNER_WHITE, 23, 5);
	ut_check_int(idx, 18, "white 23 - 5");

	//
	// from rel => abs
	//
	idx = s_field_idx_rel(&status, OWNER_WHITE, 0);
	idx = s_field_idx_add_abs(&status, OWNER_WHITE, idx, 5);
	ut_check_int(idx, 18, "white 0 - 5");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_field_exec() {

	test_s_field_idx_rel();

	test_s_field_idx_is_out();

	test_s_field_idx_add_abs();
}
