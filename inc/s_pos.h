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

#ifndef INC_S_POS_H_
#define INC_S_POS_H_

#include <stdbool.h>

#include "lib_s_point.h"
#include "s_area.h"

/******************************************************************************
 * The struct contains the position information of a point, the bar and the
 * bear off area.
 *****************************************************************************/

typedef struct {

	//
	// The upper left / lower left corner of the point area, depending on the
	// is_upper flag.
	//
	s_point pos;

	bool is_upper;

} s_pos;

/******************************************************************************
 * An enumeration for the three types of areas, which can contain checkers.
 *****************************************************************************/

typedef enum {

	E_POS_POINTS = 0, E_POS_BAR = 1, E_POS_BEAR_OFF = 2

} e_pos;

/******************************************************************************
 * The function declarations.
 *****************************************************************************/

s_pos* s_pos_get_points();

s_pos s_pos_checker_get(const s_pos *pos);

s_pos s_pos_get_checker(const e_pos type, const int idx);

void s_pos_set_bear_off(const s_area *area_bear_off);

void s_pos_set_bar(const s_area *area_bar);

void s_pos_set_points(const s_area *area_board_outer, const s_area *area_board_inner);

#endif /* INC_S_POS_H_ */
