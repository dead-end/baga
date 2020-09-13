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
#include "s_color_def.h"

/******************************************************************************
 * A utility function that checks a color definition. The expected value is not
 * a reference.
 *****************************************************************************/

static void check_color_def(const s_color_def *current, const s_color_def expected, const char *msg) {

	ut_check_int(current->r, expected.r, msg);
	ut_check_int(current->g, expected.g, msg);
	ut_check_int(current->b, expected.b, msg);
}

/******************************************************************************
 * The function checks the s_color_def_hex_chr() function.
 *****************************************************************************/

static void test_s_color_def_hex_chr() {
	int result;

	result = s_color_def_hex_chr('0');
	ut_check_int(result, 0, "Test: 0");

	result = s_color_def_hex_chr('5');
	ut_check_int(result, 5, "Test: 5");

	result = s_color_def_hex_chr('9');
	ut_check_int(result, 9, "Test: 9");

	result = s_color_def_hex_chr('a');
	ut_check_int(result, 10, "Test: a");

	result = s_color_def_hex_chr('A');
	ut_check_int(result, 10, "Test: A");

	result = s_color_def_hex_chr('c');
	ut_check_int(result, 12, "Test: c");

	result = s_color_def_hex_chr('C');
	ut_check_int(result, 12, "Test: C");

	result = s_color_def_hex_chr('f');
	ut_check_int(result, 15, "Test: f");

	result = s_color_def_hex_chr('F');
	ut_check_int(result, 15, "Test: F");
}

/******************************************************************************
 * The function checks the s_color_def_hex_str() function.
 *****************************************************************************/

static void test_s_color_def_hex_str() {
	s_color_def result;

	s_color_def_hex_str(&result, "#000000");
	check_color_def(&result, (s_color_def ) { 0, 0, 0 }, "#000000");

	s_color_def_hex_str(&result, "#ffffff");
	check_color_def(&result, (s_color_def ) { 255, 255, 255 }, "#ffffff");

	s_color_def_hex_str(&result, "#3d1749");
	check_color_def(&result, (s_color_def ) { 61, 23, 73 }, "#3d1749");
}

/******************************************************************************
 * The function checks the s_color_def_hex_dec() function.
 *****************************************************************************/

static void test_s_color_def_hex_dec() {
	s_color_def current;

	s_color_def_set(&current, 0, 0, 0);
	s_color_def_hex_dec(&current);
	check_color_def(&current, (s_color_def ) { 0, 0, 0 }, "#000000");

	s_color_def_set(&current, 0xff, 0xff, 0xff);
	s_color_def_hex_dec(&current);
	check_color_def(&current, (s_color_def ) { 1000, 1000, 1000 }, "#ffffff");

	s_color_def_set(&current, 63, 127, 191);
	s_color_def_hex_dec(&current);
	check_color_def(&current, (s_color_def ) { 247, 498, 749 }, "lower");

	s_color_def_set(&current, 64, 128, 192);
	s_color_def_hex_dec(&current);
	check_color_def(&current, (s_color_def ) { 251, 502, 753 }, "upper");
}

/******************************************************************************
 * The function checks the s_color_def_gradient_do() function.
 *****************************************************************************/

static void test_s_color_def_gradient_do() {

	s_color_def result;

	const s_color_def start = { .r = 100, .g = 200, .b = 300 };
	const s_color_def end = { .r = 200, .g = 400, .b = 600 };

	const int num = 5;

	s_color_def_gradient_do(num, 0, &start, &end, &result);
	check_color_def(&result, (s_color_def ) { 100, 200, 300 }, "step: 0");

	s_color_def_gradient_do(num, 1, &start, &end, &result);
	check_color_def(&result, (s_color_def ) { 125, 250, 375 }, "step: 1");

	s_color_def_gradient_do(num, 2, &start, &end, &result);
	check_color_def(&result, (s_color_def ) { 150, 300, 450 }, "step: 2");

	s_color_def_gradient_do(num, 3, &start, &end, &result);
	check_color_def(&result, (s_color_def ) { 175, 350, 525 }, "step: 3");

	s_color_def_gradient_do(num, 4, &start, &end, &result);
	check_color_def(&result, (s_color_def ) { 200, 400, 600 }, "step: 4");
}

/******************************************************************************
 * The function is the a wrapper, that triggers the internal unit tests.
 *****************************************************************************/

void ut_s_color_def_exec() {

	test_s_color_def_hex_chr();

	test_s_color_def_hex_str();

	test_s_color_def_hex_dec();

	test_s_color_def_gradient_do();
}
