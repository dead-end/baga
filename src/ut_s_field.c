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
 *
 * After refactoring the macro is simple.
 *****************************************************************************/

static void test_s_field_idx_rel() {
	int idx_rel;

	idx_rel = s_field_idx_rel(E_OWNER_TOP, 0);
	ut_check_int(idx_rel, 0, "top 0");

	idx_rel = s_field_idx_rel(E_OWNER_BOT, 0);
	ut_check_int(idx_rel, 23, "bottom 0");

	idx_rel = s_field_idx_rel(E_OWNER_TOP, 23);
	ut_check_int(idx_rel, 23, "top 23");

	idx_rel = s_field_idx_rel(E_OWNER_BOT, 23);
	ut_check_int(idx_rel, 0, "bottom 23");

	//
	// rel(rel) = abs BOTTOM
	//
	idx_rel = s_field_idx_rel(E_OWNER_BOT, 0);
	idx_rel = s_field_idx_rel(E_OWNER_BOT, idx_rel);
	ut_check_int(idx_rel, 0, "bottom 0 - id");

	idx_rel = s_field_idx_rel(E_OWNER_BOT, 23);
	idx_rel = s_field_idx_rel(E_OWNER_BOT, idx_rel);
	ut_check_int(idx_rel, 23, "bottom 23 - id");

	//
	// rel(rel) = abs TOP
	//
	idx_rel = s_field_idx_rel(E_OWNER_TOP, 0);
	idx_rel = s_field_idx_rel(E_OWNER_TOP, idx_rel);
	ut_check_int(idx_rel, 0, "top 0 - id");

	idx_rel = s_field_idx_rel(E_OWNER_TOP, 23);
	idx_rel = s_field_idx_rel(E_OWNER_TOP, idx_rel);
	ut_check_int(idx_rel, 23, "top 23 - id");
}

/******************************************************************************
 * The function checks the test_s_field_idx_add_abs() calls.
 *****************************************************************************/
// TODO: not used
static void test_s_field_idx_add_abs() {
	int idx1, idx2;
	int idx;

	idx = s_field_idx_add_abs(E_OWNER_TOP, 0, 5);
	ut_check_int(idx, 5, "top 0 - 5");

	idx = s_field_idx_add_abs(E_OWNER_BOT, 23, 5);
	ut_check_int(idx, 18, "bottom 23 - 5");

	//
	// from rel => abs
	//
	idx = s_field_idx_rel(E_OWNER_BOT, 0);
	idx = s_field_idx_add_abs(E_OWNER_BOT, idx, 5);
	ut_check_int(idx, 18, "bottom 0 - 5");

	//
	// Using the macro should be the same as:
	//
	//   abs => rel
	//   rel + 5
	//   rel => abs
	//
	idx1 = s_field_idx_rel(E_OWNER_BOT, 23);
	idx1 += 5;
	idx1 = s_field_idx_rel(E_OWNER_BOT, idx1);

	idx2 = s_field_idx_add_abs(E_OWNER_BOT, 23, 5);
	ut_check_int(idx1, idx2, "bottom 0 - 5");

	//
	// Same with top
	//
	idx1 = s_field_idx_rel(E_OWNER_TOP, 23);
	idx1 += 5;
	idx1 = s_field_idx_rel(E_OWNER_TOP, idx1);

	idx2 = s_field_idx_add_abs(E_OWNER_TOP, 23, 5);
	ut_check_int(idx1, idx2, "bottom 0 - 5");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_field_exec() {

	test_s_field_idx_rel();

	test_s_field_idx_add_abs();
}
