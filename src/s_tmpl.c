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

#include "s_tmpl.h"

/******************************************************************************
 * The function creates the array for the structure with the given dimension
 * and stores the dimension.
 *****************************************************************************/

void s_tarr_new(s_tarr *tarr, const int row, const int col) {

	log_debug("Creating s_tmpl: %d/%d", row, col);

	tarr->arr = malloc(row * col * sizeof(s_tchar));
	if (tarr->arr == NULL) {
		log_exit_str("Unable to allocate memory!");
	}

	tarr->dim.row = row;
	tarr->dim.col = col;
}

/******************************************************************************
 * The function frees the array of the structure.
 *****************************************************************************/

void s_tarr_free(s_tarr *tarr) {

#ifdef DEBUG
	if (tarr->arr == NULL) {
		log_exit("Template already freed: %d/%d", tarr->dim.row, tarr->dim.col);
	}
#endif

	log_debug("Freeing s_tmpl: %d/%d", tarr->dim.row, tarr->dim.col);

	if (tarr->arr != NULL) {
		free(tarr->arr);
		tarr->arr = NULL;
	}
}

/******************************************************************************
 * The function initializes the array with a s_tchar.
 *****************************************************************************/

void s_tarr_set(s_tarr *tarr, const s_tchar tchar) {

	for (int row = 0; row < tarr->dim.row; row++) {
		for (int col = 0; col < tarr->dim.col; col++) {

			s_tarr_get(tarr, row, col) = tchar;
		}
	}
}
