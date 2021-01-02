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

#include "direction.h"

/******************************************************************************
 * The function checks the direction_get() calls.
 *****************************************************************************/

static void test_direction_get() {

	s_point result;

	//
	// UP
	//
	result = direction_to((s_point ) { 8, 10 }, (s_point ) { 3, 10 });
	ut_check_s_point(&result, &dirs[E_DIR_UP], "up");

	//
	// DOWN
	//
	result = direction_to((s_point ) { 4, 3 }, (s_point ) { 5, 3 });
	ut_check_s_point(&result, &dirs[E_DIR_DOWN], "down");

	//
	// LEFT
	//
	result = direction_to((s_point ) { 0, 3 }, (s_point ) { 0, 0 });
	ut_check_s_point(&result, &dirs[E_DIR_LEFT], "left");

	//
	// RIGHT
	//
	result = direction_to((s_point ) { 9, 1 }, (s_point ) { 9, 11 });
	ut_check_s_point(&result, &dirs[E_DIR_RIGHT], "right");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_direction_exec() {

	test_direction_get();
}
