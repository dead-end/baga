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
#include "lib_string.h"
#include "lib_s_point.h"

#include <stdbool.h>
#include <string.h>

/******************************************************************************
 * The function checks whether an int parameter has the expected value or not.
 *****************************************************************************/

void ut_check_int(const int current, const int expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: %d expected: %d", msg, current, expected);
	}

	log_debug("[%s] OK current: %d", msg, current);
}

/******************************************************************************
 * The function checks whether an short parameter has the expected value or not.
 *****************************************************************************/

void ut_check_short(const short current, const short expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: %d expected: %d", msg, current, expected);
	}

	log_debug("[%s] OK current: %d", msg, current);
}

/******************************************************************************
 * The function checks whether a bool parameter has the expected value or not.
 *****************************************************************************/

void ut_check_bool(const bool current, const bool expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: %s expected: %s", msg, ls_bool_str(current), ls_bool_str(expected));
	}

	log_debug("[%s] OK current: %s", msg, ls_bool_str(current));
}

/******************************************************************************
 * The function checks whether a s_point parameter has the expected value or
 * not.
 *****************************************************************************/

void ut_check_s_point(const s_point *current, const s_point *expected, const char *msg) {

	if (current->row != expected->row || current->col != expected->col) {
		log_exit("[%s] current: %d/%d expected: %d/%d", msg, current->row, current->col, expected->row, expected->col);
	}

	log_debug("[%s] OK current: %d/%d", msg, current->row, current->col);
}

/******************************************************************************
 * The function checks whether a string parameter has the expected value or
 * not.
 *****************************************************************************/

void ut_check_char_str(const char *current, const char *expected, const char *msg) {

	if (strcmp(current, expected) != 0) {
		log_exit("[%s] current: '%s' expected: '%s'", msg, current, expected);
	}

	log_debug("[%s] OK - Strings are equal: '%s'", msg, current);
}

/******************************************************************************
 * The function checks whether a wchar_t parameter has the expected value or
 * not.
 *****************************************************************************/

void ut_check_wchar_t(const wchar_t current, const wchar_t expected, const char *msg) {

	if (current != expected) {
		log_exit("[%s] current: '%lc' expected: '%lc'", msg, current, expected);
	}

	log_debug("[%s] OK - Strings are equal: '%lc'", msg, current);
}
