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

#ifndef INC_BG_BOARD_H_
#define INC_BG_BOARD_H_

#include "bg_defs.h"

/******************************************************************************
 * The struct contains the number of checkers on a point and the owner of the
 * checkers (black / white) or none if there are no checkers on the point.
 *****************************************************************************/

typedef struct {

	e_owner owner;

	int num;

} s_bg_point;

#define s_bg_point_set(p, o,n) (p)->owner = (o); (p)->num = (n)

/******************************************************************************
 * The struct contains the positions of each checker on the board..
 *****************************************************************************/

typedef struct {

	//
	// The number of checkers that are on a point for black and white.
	//
	s_bg_point bg_point[POINTS_NUM];

	//
	// The number of checkers that are in the bear off area for black and
	// white. We use e_owner as an index.
	//
	int bear_off[NUM_PLAYER];

	//
	// The number of checkers that have to reenter for black and white. We use
	// e_owner as an index.
	//
	int reenter[NUM_PLAYER];

} s_bg_board;

/******************************************************************************
 * Function declarations.
 *****************************************************************************/

void bg_board_init(s_bg_board *bg_board);

void bg_board_new_game(s_bg_board *bg_board);

void nc_board_test();

#endif /* INC_BG_BOARD_H_ */
