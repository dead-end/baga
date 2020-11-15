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

#ifndef INC_LIB_S_POINT_H_
#define INC_LIB_S_POINT_H_

/******************************************************************************
 * The s_point struct represents an element that has a row and a column. This
 * can be a pixel (terminal character), an array dimension, a block size...
 *****************************************************************************/

typedef struct s_point {

	int row;

	int col;

} s_point;

/******************************************************************************
 * Definition of the macros.
 *****************************************************************************/

#define s_point_set_ptr(p,r,c) (p)->row = (r);(p)->col = (c)

#define s_point_set(p,r,c) (p).row = (r);(p).col = (c)

#define s_point_copy(f,t) (t)->row = (f)->row; (t)->col = (f)->col

#define s_point_same(p,q) (((p)->row == (q)->row) && ((p)->col == (q)->col))

/******************************************************************************
 * The definition of the functions.
 *****************************************************************************/

int s_point_dist(const s_point from, const s_point to);

int s_point_smaller_dist(const s_point from, const s_point to, const int dist_cur);

#endif /* INC_LIB_S_POINT_H_ */
