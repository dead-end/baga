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

#ifndef INC_S_TMPL_CHECKER_H_
#define INC_S_TMPL_CHECKER_H_

#include <stdbool.h>

#include "bg_defs.h"
#include "s_tarr.h"
#include "s_point_layout.h"
#include "s_game_cfg.h"

/******************************************************************************
 * Declaration of the functions.
 *****************************************************************************/

void s_tmpl_checker_create(const s_game_cfg *game_cfg);

void s_tmpl_checker_free();

const s_tarr* s_tmpl_checker_get_travler(const e_owner owner);

const s_tarr* s_tmpl_checker_get_tmpl(const e_owner owner, const s_point_layout point_layout, const int idx, const bool reverse);

#endif /* INC_S_TMPL_CHECKER_H_ */
