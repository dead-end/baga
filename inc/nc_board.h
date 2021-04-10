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

#include <ncurses.h>

#include "bg_defs.h"
#include "s_fieldset.h"
#include "s_point_layout.h"
#include "s_game_cfg.h"
#include "s_status.h"

/******************************************************************************
 * Function definitions that are only used for unit tests.
 *****************************************************************************/

void nc_board_init(WINDOW *win, const s_game_cfg *game_cfg, const s_board_areas *board_areas);

void nc_board_free();

void nc_board_print();

void s_board_print_game(const s_fieldset *fieldset);

void nc_board_process(s_fieldset *fieldset, s_status *status, const s_field_id field);

#endif /* INC_NC_BOARD_H_ */
