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

#ifndef INC_NC_BOARD_H_
#define INC_NC_BOARD_H_

#include "bg_defs.h"
#include "lib_s_point.h"
#include "lib_s_tchar.h"

#include "s_area.h"

/******************************************************************************
 * Definitions of various sizes.
 *****************************************************************************/

#define BORDER_ROW 1

#define BORDER_COL 2

#define BOARD_HALF_ROW (2 * POINTS_ROW + CHECKER_ROW + 2 * BORDER_ROW)

#define BOARD_HALF_COL (6 * POINTS_COL)

//
// Total rows and columns
//
#define BOARD_ROW (2 * BORDER_ROW + BOARD_HALF_ROW)

#define BOARD_COL (5 * BORDER_COL + 2 * BOARD_HALF_COL  + 2 * CHECKER_COL)

/******************************************************************************
 * The struct contains the definition of the board.
 *****************************************************************************/

typedef struct {

	s_tchar arr[BOARD_ROW][BOARD_COL];

} s_nc_board;

/******************************************************************************
 * Function definitions that are only used for unit tests.
 *****************************************************************************/

void nc_board_set_tchar(s_nc_board *board, const s_tchar tchar);

void nc_board_init_bg(s_nc_board *board_bg, const s_area *area_board_outer, const s_area *area_board_inner);

void nc_board_print(const s_nc_board *board_fg, const s_nc_board *board_bg);

#endif /* INC_NC_BOARD_H_ */
