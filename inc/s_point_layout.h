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

#ifndef INC_S_POINT_LAYOUT_H_
#define INC_S_POINT_LAYOUT_H_

#include "bg_defs.h"

/******************************************************************************
 * The definition of the checkers on a point.
 *****************************************************************************/

//
// No more than CHECK_DIS_MAX checkers are displayed on a point.
//
#define CHECK_DIS_MAX 9

//
// The maximum number of checker that are displayed completely on a point.
//
#define CHECK_DIS_FULL 5

/******************************************************************************
 * The struct contains the layout of a checkers on a point.
 *****************************************************************************/

typedef struct {

	//
	// The total number of checkers on the point.
	//
	int total;

	//
	// The number of checkers on the point, which are displayed half.
	//
	int num_half;

	//
	// The index of the checker, which has a label, or -1 if none of the
	// checkers has a label.
	//
	int label_idx;

} s_point_layout;

/******************************************************************************
 *
 *****************************************************************************/

typedef enum {

	E_UNCOMP = 0, E_COMP = 1

} e_compressed;

/******************************************************************************
 * The declaration of the arrays with the layouts.
 *****************************************************************************/

extern s_point_layout _point_layout[CHECKER_NUM + 1][2];

/******************************************************************************
 * The macro to access the layout.
 *****************************************************************************/

#define s_point_layout_get(t,c) _point_layout[t][c]

/******************************************************************************
 * The macro checks if the checker with the given index is not visible.
 *****************************************************************************/

#define s_point_layout_not_visible(cl,i) ((cl).label_idx >= 0) && ((i) > (cl).label_idx)

/******************************************************************************
 * The macro checks if the checker with the given index has a label.
 *****************************************************************************/

#define s_point_layout_has_label(cl,i) ((cl).label_idx == (i))

/******************************************************************************
 * The macro checks if the checker with the given index is displayed half.
 *****************************************************************************/

#define s_point_layout_is_half(cl,i) ((i) < (cl).num_half)

/******************************************************************************
 * The macro returns the color index of a given checker.
 *****************************************************************************/

// todo: unit test
#define s_point_layout_color_idx(t,i) (reverse_idx(min(CHECK_DIS_MAX, (t)), (i)) + 1)

#endif /* INC_S_POINT_LAYOUT_H_ */
