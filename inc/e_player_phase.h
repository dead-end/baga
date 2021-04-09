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
 * The header file provides an enum for the phases of the game. Each player has
 * his own phase, which defines which set of rules is applied (for this pase).
 *****************************************************************************/

#ifndef INC_E_PLAYER_PHASE_H_
#define INC_E_PLAYER_PHASE_H_

/******************************************************************************
 * The enum defines the possible phases of a player. Each player has his own
 * phase,
 *****************************************************************************/

typedef enum {

	//
	// None of the other two phases.
	//
	E_PHASE_NORMAL = 0,

	//
	// At least one of the checkers is on the bar.
	//
	E_PHASE_BAR = 1,

	//
	// All checkers are in the player's home board.
	//
	E_PHASE_BEAR_OFF = 2,

	//
	// All checkers are in the bear off.
	//
	E_PHASE_WIN = 3,

} e_player_phase;

const char* e_player_phase_str(const e_player_phase player_phase);

#endif /* INC_E_PLAYER_PHASE_H_ */
