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
// The number of points (triangles) of the backgammon game.
//
#define POINTS_NUM 24

//
// The dimension of a checker (gaming piece)
//
#define CHECKER_ROW 2

#define CHECKER_COL 4

//
// The dimension of the points (triangles) of the backgammon game.
//
#define POINTS_ROW (5 * CHECKER_ROW)

#define POINTS_COL (CHECKER_COL + 2)

//
// The macro returns the reverse index given a total number and an index.
//
#define reverse_idx(t,i) ((t) - 1 - (i))

//
// Empty wchar_t
//
#define EMPTY L' '

//
// The number of players (which is trivial).
//
#define NUM_PLAYER 2

/******************************************************************************
 * The enum defines an owner of a checker which can be black or white or none
 * if there is no checker. The owner can be used as an index for some arrays.
 *****************************************************************************/

typedef enum {

	OWNER_BLACK = 0, OWNER_WHITE = 1, OWNER_NONE = -1

} e_owner;

#define e_owner_has_owner(o) ((o) != -1)

#endif /* INC_BG_DEFS_H_ */