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

#ifndef INC_BG_DEFS_H_
#define INC_BG_DEFS_H_

/******************************************************************************
 * Definition of constants, which are widely used. It is not easy to define
 * where they should be defined without having circular dependencies.
 *****************************************************************************/

//
// The number of checker for each player.
//
#define CHECKER_NUM 15

//
// The number of points (triangles) of the backgammon game.
//
#define POINTS_NUM 24

#define POINTS_HALF 12

#define POINTS_QUARTER 6

//
// The dimension of a checker (gaming piece)
//
#define CHECKER_ROW 2

#define CHECKER_COL 4

//
// Checker is smaller than the points.
//
#define CHECKER_OFFSET_ROW 0

#define CHECKER_OFFSET_COL 1

//
// The dimension of the points (triangles) of the backgammon game.
//
#define POINTS_ROW (5 * CHECKER_ROW)

#define POINTS_COL (CHECKER_COL + 2)

//
// Empty wchar_t
//
#define EMPTY L' '

//
// Full block
//
#define FULL L'\u2588'

//
// The number of players (which is trivial).
//
#define NUM_PLAYER 2

/******************************************************************************
 * The enum defines an owner of a checker. The owner can be the player from top
 * to bottom or from bottom to top or none at all. The owner can be used as an
 * index for some arrays.
 *****************************************************************************/

typedef enum {

	//
	// Player from top to bottom
	//
	OWNER_TOP = 0,

	//
	// Player from bottom to top
	//
	OWNER_BOT = 1,

	//
	// Not used
	//
	OWNER_NONE = -1

} e_owner;

#define e_owner_has_owner(o) ((o) != -1)

#define e_owner_other(o) (((o) + 1) % 2)

#endif /* INC_BG_DEFS_H_ */
