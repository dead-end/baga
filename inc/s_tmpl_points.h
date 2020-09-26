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

#ifndef INC_S_TMPL_POINTS_H_
#define INC_S_TMPL_POINTS_H_

#include "lib_s_tchar.h"
#include "lib_s_point.h"

#include "bg_defs.h"
#include "s_area.h"

/******************************************************************************
 * The struct contains the character and color definition of the points.
 *****************************************************************************/

typedef struct {

	s_tchar tchar[POINTS_ROW][POINTS_COL];

} s_points_tchar;

/******************************************************************************
 * The struct contains the definition of the points for black and white.
 *****************************************************************************/

typedef struct {

	s_points_tchar points[NUM_PLAYER];

} s_tmpl_points;

/******************************************************************************
 * Declaration of the functions.
 *****************************************************************************/

#define s_tmpl_point_get(t,i) &((t)->points[(i) % 2])

void s_tmpl_points_init(s_tmpl_points *tmpl_point);

void s_tmpl_points_set_pos(s_point *point_pos, const s_area *area_board_outer, const s_area *area_board_inner);

#endif /* INC_S_TMPL_POINTS_H_ */
