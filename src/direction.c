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

#include "direction.h"

/******************************************************************************
 * The enum defines the 4 directions that are supported for the movements of
 * the traveling checkers. The value is used as an index to an array.
 *****************************************************************************/

typedef enum {

	E_DIR_UP = 0, E_DIR_DOWN = 1, E_DIR_RIGHT = 2, E_DIR_LEFT = 3

} e_dir;

/******************************************************************************
 * The array contains the s_points with the movements. To move from one
 * position in a given direction, add the s_point to the position. The index of
 * the array is the enum e_dir value.
 *****************************************************************************/

s_point dirs[4] = {

//
// Direction: UP
//
		[E_DIR_UP] = { -1, 0 },

		//
		// Direction: DOWN
		//
		[E_DIR_DOWN] = { 1, 0 },

		//
		// Direction: RIGHT
		//
		[E_DIR_RIGHT] = { 0, 1 },

		//
		// Direction: LEFT
		//
		[E_DIR_LEFT] = { 0, -1 },

};

/******************************************************************************
 * The function takes a source and a target position and returns the s_point
 * with the direction. It is assumed that the movement can be only horizontal
 * or vertical. This means the rows or columns have to be equal. It is an error
 * if the source and the target are the same.
 *****************************************************************************/

s_point direction_get(const s_point from, const s_point to) {

	//
	// Direction: UP
	//

	if (from.col == to.col) {

		//
		// DOWN
		//
		if (from.row < to.row) {
			return dirs[E_DIR_DOWN];
		}

		//
		// UP
		//
		if (from.row > to.row) {
			return dirs[E_DIR_UP];
		}

	} else if (from.row == to.row) {

		//
		// RIGHT
		//
		if (from.col < to.col) {
			return dirs[E_DIR_RIGHT];
		}

		//
		// LEFT
		//
		if (from.col > to.col) {
			return dirs[E_DIR_LEFT];
		}

	}

	log_exit("from: %d/%d to: %d/%d", from.row, from.col, to.row, to.col);
}
