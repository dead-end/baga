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

#ifndef INC_DICE_H_
#define INC_DICE_H_

/******************************************************************************
 * The header file provides an interface to the window with the 4 buttons:
 *
 * - 2 dices (which are buttons)
 * - undo button
 * - confirm button
 *****************************************************************************/

#include <ncurses.h>

#include "s_fieldset.h"
#include "s_status.h"
#include "s_game_cfg.h"
#include "lib_s_point.h"

/******************************************************************************
 * Sizes for the dices. They are globally defined to be able to compute the
 * size of the overall layout.
 *
 * TODO: maybe it is better to use a function that computes the total size of
 * the window.
 *****************************************************************************/

#define D_ROWS 4

#define D_COLS 7

#define D_PAD 2

/******************************************************************************
 * The definitions of the functions.
 *****************************************************************************/

void dice_init(const s_game_cfg *game_cfg, WINDOW *win);

void dice_free();

void dice_print(const s_status *status);

bool dice_process_event(s_fieldset *fieldset, s_status *status, const s_point *event);

#endif /* INC_DICE_H_ */
