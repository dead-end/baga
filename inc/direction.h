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

/******************************************************************************
 * The header file defines an enum for the 4 directions (up, down, left,
 * right) and provides an interface with utility functions. The directions are
 * used for the movement of the checker as part of an animation.
 *****************************************************************************/

#ifndef INC_DIRECTION_H_
#define INC_DIRECTION_H_

#include "lib_s_point.h"

/******************************************************************************
 * The enum defines the 4 directions that are supported for the movements of
 * the traveling checkers. The value is used as an index to an array.
 *****************************************************************************/

typedef enum {

	E_DIR_UP = 0, E_DIR_DOWN = 1, E_DIR_RIGHT = 2, E_DIR_LEFT = 3

} e_dir;

extern s_point dirs[4];

/******************************************************************************
 * The definitions of the functions.
 *****************************************************************************/

s_point direction_to(const s_point from, const s_point to);

#define direction_mov_to(p,d) (p)->row += (d).row ; (p)->col += (d).col

#define direction_get(i) dirs[i]

#endif /* INC_DIRECTION_H_ */
