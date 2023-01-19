/**************************************************************************************
 * Copyright (c) 2020 ["Peking University", "Peng Cheng Laboratory"]
 *
 * All rights reserved.
 * The software is released under the Open-Intelligence Open Source License V2.0.
 * you may not use this file except in compliance with the License.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
**************************************************************************************/

#ifndef BROADCAST_H
#define BROADCAST_H

#include "src/core/tensor.h"

/**
 * @brief Do broadcast and save result into dims_out.
 *
 * @details Compares every pair of elements of dims_in1 and dims_in2 in
 * 			reverse order.
 * 			Suppose that k and m are the i_th last elements of dims_in1 and
 * 			dims_in2. (1) k == m, then dim_out[i_th last] = k; (2) k == 1 or m ==1, then
 * 			dim_out[i_th last] = max(k, m); (3) k != m and k != 1 and m !=1, mismatched error.
 * 			For example: dims_in1 = {3, 2, 3, 1, 5}, dims_in2 = {1, 3, 4, 5}, then
 * 			dims_out = {3, 2, 3, 4, 5}
 *
 * @note This function does not allocated any memory inside, so the memory of
 * dims_out is assumed to be managed by user.
 *
 * @param dims_in1 The first dims.
 * @param ndim_in1 The size of dims_in1.
 * @param dims_in2 The second dims.
 * @param ndim_in2 The size of dims_in2.
 * @param dims_out The output dims.
 * @param ndim_out The size of dims_out.
 *
 * @return Status.
 * @retval STATUS_SUCCESS Success.
 * @retval STATUS_DIMENSIONS_MISMATCH The dims of input are mismatched and can not be broadcasted.
 */
AITISA_API_PUBLIC Status
aitisa_broadcast_array(int64_t* dims_in1, int64_t ndim_in1, int64_t* dims_in2,
                       int64_t ndim_in2, int64_t* dims_out, int64_t ndim_out);

#endif
