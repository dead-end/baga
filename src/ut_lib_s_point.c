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

#include "lib_s_point.h"

/******************************************************************************
 * The function checks the s_point_smaller_dist() calls. The function stops on
 * a larger distance, so there is no negative test possible.
 *****************************************************************************/

static void test_dist() {
	int d;

	d = s_point_dist((s_point ) { .row = 1, .col = 1 }, (s_point ) { .row = 5, .col = 10 });

	d = s_point_smaller_dist((s_point ) { .row = 1, .col = 1 }, (s_point ) { .row = 10, .col = 4 }, d);
	d = s_point_smaller_dist((s_point ) { .row = 1, .col = 1 }, (s_point ) { .row = 4, .col = 9 }, d);
	d = s_point_smaller_dist((s_point ) { .row = 1, .col = 1 }, (s_point ) { .row = 3, .col = 9 }, d);
	d = s_point_smaller_dist((s_point ) { .row = 2, .col = 1 }, (s_point ) { .row = 3, .col = 9 }, d);
	d = s_point_smaller_dist((s_point ) { .row = 2, .col = 2 }, (s_point ) { .row = 3, .col = 9 }, d);
}

/******************************************************************************
 * The function checks some basic macros.
 *****************************************************************************/

static void test_simple() {
	s_point p1, p2;

	//
	// Check set macros
	//
	s_point_set(p1, 1, 2);

	s_point_set_ptr(&p2, 2, 1);

	ut_check_bool(s_point_same(&p1, &p2), false, "wrong set");

	//
	// Check copy macros
	//
	s_point_copy(&p1, &p2);

	ut_check_bool(s_point_same(&p1, &p2), true, "ok copy");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_lib_s_point_exec() {

	test_dist();

	test_simple();
}
