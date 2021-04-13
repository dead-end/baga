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
 * The header file provides an interface to access the different areas of the
 * board. We have an outer and inner board with points top and bottom. We have
 * a bar and a bear off area.
 *
 * +---------------+-----+---------------+------+
 * | top points    |     | top points    |      |
 * |               |     |               | bear |
 * | outer board   | bar | inner board   | off  |
 * |               |     |               |      |
 * | bottom points |     | bottom points |      |
 * +---------------+-----+---------------+------+
 *****************************************************************************/

#ifndef INC_S_BOARD_AREAS_H_
#define INC_S_BOARD_AREAS_H_

#include "lib_s_point.h"
#include "s_field_id.h"
#include "s_area.h"

/******************************************************************************
 * Definitions of the various areas of the board. An area has a position and a
 * dimension. The areas a listed above.
 *****************************************************************************/

typedef struct {

	s_area board_outer;

	s_area bar_inner;

	s_area board_inner;

	s_area bear_off;

	s_point board_dim;

} s_board_areas;

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
 * The function declarations.
 *****************************************************************************/

const s_board_areas* s_board_areas_init();

const s_pos* s_board_areas_get_points();

s_pos s_board_areas_get_checker(const s_field_id field_id);

void s_board_areas_mouse_target(const s_point mouse, s_field_id *field_id);

#endif /* INC_S_BOARD_AREAS_H_ */
