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

#ifndef INC_S_COLOR_DEF_H_
#define INC_S_COLOR_DEF_H_

/******************************************************************************
 * The struct contains the definition of a color, which contains of a red,
 * green, blue value. The values can be: 0-255 or 0-1000.
 *****************************************************************************/

typedef struct {

	short r;
	short g;
	short b;

} s_color_def;

/******************************************************************************
 * Definition of functions and macros.
 *****************************************************************************/

#define s_color_def_set(c, cr, cg, cb) (c)->r = (cr) ; (c)->g = (cg) ; (c)->b = (cb)

void s_color_def_gradient(short *colors, const int num, const char *str_start, const char *str_end);

/******************************************************************************
 * Function definitions that are only used for unit tests.
 *****************************************************************************/

int s_color_def_hex_chr(const char c);

void s_color_def_hex_str(s_color_def *color_def, const char *color);

void s_color_def_hex_dec(s_color_def *color_def);

void s_color_def_gradient_do(const int num, const int i, const s_color_def *start, const s_color_def *end, s_color_def *result);

#endif /* INC_S_COLOR_DEF_H_ */
