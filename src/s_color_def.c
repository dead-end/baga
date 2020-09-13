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

#include <math.h>
#include <string.h>

#include "s_color_def.h"
#include "lib_logging.h"
#include "lib_color.h"

/******************************************************************************
 * The function is called with a hex character and returns the decimal value.
 *
 * (Unit tested)
 *****************************************************************************/

int s_color_def_hex_chr(const char c) {

	if (c >= '0' && c <= '9') {
		return c - '0';
	}

	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}

	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}

	log_exit("Invalid character: %c", c);
}

/******************************************************************************
 * The function is called with a string that is a color definition. The string
 * starts with a '#' and has 6 hex characters. Two hex characters form a rgb
 * value. Example: "#ffffff"
 *
 * The function stores the rgb values in a color definition struct.
 *
 * (Unit tested)
 *****************************************************************************/

void s_color_def_hex_str(s_color_def *color_def, const char *color) {

	//
	// Ensure the format.
	//
	if (strlen(color) != 7 || color[0] != '#') {
		log_exit("Invalid color: %s (valid example: #01ff9a)", color);
	}

	color_def->r = s_color_def_hex_chr(color[1]) * 16 + s_color_def_hex_chr(color[2]);
	color_def->g = s_color_def_hex_chr(color[3]) * 16 + s_color_def_hex_chr(color[4]);
	color_def->b = s_color_def_hex_chr(color[5]) * 16 + s_color_def_hex_chr(color[6]);
}

/******************************************************************************
 * The most common definition of a rgb color has the form: "#ffffff", where
 * each color part (red, green, blue) has a value between 0-255.
 *
 * Curses used color definitions, where each color part has a value between
 * 0-1000.
 *
 * The function converts the first to the curses format.
 *
 * (Unit tested)
 *****************************************************************************/

void s_color_def_hex_dec(s_color_def *color_def) {

#ifdef DEBUG

	//
	// Ensure that the color definition has valid ranges. If the color was
	// constructed by a hex string (example: "#ffffff") this is implicitly
	// guaranteed, so we test it only in debug mode.
	//
	if (color_def->r < 0 || color_def->r > 0xff) {
		log_exit("Invalid color definition - red:   %d (min: 0 max: %d)", color_def->r, 0xff);
	}

	if (color_def->g < 0 || color_def->g > 0xff) {
		log_exit("Invalid color definition - green: %d (min: 0 max: %d)", color_def->g, 0xff);
	}

	if (color_def->b < 0 || color_def->b > 0xff) {
		log_exit("Invalid color definition - blue:  %d (min: 0 max: %d)", color_def->b, 0xff);
	}
#endif

	color_def->r = round((double) color_def->r * 1000.0 / 0xff);
	color_def->g = round((double) color_def->g * 1000.0 / 0xff);
	color_def->b = round((double) color_def->b * 1000.0 / 0xff);
}

/******************************************************************************
 * The function computes the rgb values for the gradient step i.
 *
 * (Unit tested)
 *****************************************************************************/

void s_color_def_gradient_do(const int num, const int i, const s_color_def *start, const s_color_def *end, s_color_def *result) {

#ifdef DEBUG

	//
	// Sanity check.
	//
	if (i >= num) {
		log_exit("Invalid i: %d num: %d", i, num);
	}
#endif

	//
	// Ration of start and end color. The value should be between 0 and 1.
	//
	const double x = (double) i / (num - 1);

	result->r = end->r * x + start->r * (1 - x);
	result->g = end->g * x + start->g * (1 - x);
	result->b = end->b * x + start->b * (1 - x);
}

/******************************************************************************
 * The function is called with an array of colors with num elements, which
 * should be filled with the gradient from start color to end color.
 *****************************************************************************/

void s_color_def_gradient(short *colors, const int num, const char *str_start, const char *str_end) {

	log_debug("num: %d start: %s end: %s", num, str_start, str_end);

	s_color_def start, end, result;

	//
	// Convert the start color from a string to hex values to ncurses values (0-1000)
	//
	s_color_def_hex_str(&start, str_start);
	s_color_def_hex_dec(&start);

	//
	// Convert the end color from a string to hex values to ncurses values (0-1000)
	//
	s_color_def_hex_str(&end, str_end);
	s_color_def_hex_dec(&end);

	for (int i = 0; i < num; i++) {

		//
		// Create the color for step i.
		//
		s_color_def_gradient_do(num, i, &start, &end, &result);

		//
		// Create the color with the ncurses rgb values (0-1000).
		//
		colors[i] = col_color_create(result.r, result.g, result.b);
	}
}
