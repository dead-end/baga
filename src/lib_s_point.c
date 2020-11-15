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
#include "lib_s_point.h"

/******************************************************************************
 * The function returns a simple distance of two s_point structs.
 *****************************************************************************/

int s_point_dist(const s_point from, const s_point to) {

	const int result = abs(from.row - to.row) + abs(from.col - to.col);

	log_debug("from: %d/%d to: %d/%d result: %d", from.row, from.col, to.row, to.col, result);

	return result;
}

/******************************************************************************
 * The function ensures that the next distance of two (moving) points is
 * smaller than the last. It is used to ensure that a point is moving to a
 * target.
 *****************************************************************************/

// TODO: unit test
int s_point_smaller_dist(const s_point from, const s_point to, const int dist_cur) {

	int dist_next = s_point_dist(from, to);

	if (dist_next >= dist_cur) {
		log_exit("from: %d/%d to: %d/%d dist-cur: %d dist-next: %d", from.row, from.col, to.row, to.col, dist_cur, dist_next);
	}

	return dist_next;
}
