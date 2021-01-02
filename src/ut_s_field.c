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

static void test_s_field_rel_idx() {
	s_status status;
	int idx_rel;

	status.up_2_down = OWNER_BLACK;

	idx_rel = s_field_rel_idx(&status, OWNER_BLACK, 0);
	ut_check_int(idx_rel, 0, "black 0");

	idx_rel = s_field_rel_idx(&status, OWNER_WHITE, 0);
	ut_check_int(idx_rel, 23, "black 0");

	idx_rel = s_field_rel_idx(&status, OWNER_BLACK, 23);
	ut_check_int(idx_rel, 23, "black 23");

	idx_rel = s_field_rel_idx(&status, OWNER_WHITE, 23);
	ut_check_int(idx_rel, 0, "black 23");

}
/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_field_exec() {

	test_s_field_rel_idx();
}
