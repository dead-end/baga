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
 * The source file provides utility functions for the enum for the phases of
 * the player.
 *****************************************************************************/

#include "e_player_phase.h"
#include "lib_logging.h"

/******************************************************************************
 * The function returns a string representation of the game phase. It is used
 * for debugging.
 *****************************************************************************/

const char* e_player_phase_str(const e_player_phase player_phase) {

	switch (player_phase) {

	case E_PHASE_NORMAL:
		return "E_PHASE_NORMAL";

	case E_PHASE_BAR:
		return "E_PHASE_BAR";

	case E_PHASE_BEAR_OFF:
		return "E_PHASE_BEAR_OFF";

	case E_PHASE_WIN:
		return "E_PHASE_WIN";

	default:
		log_exit("Unknown phase: %d", player_phase)
		;
	}
}
