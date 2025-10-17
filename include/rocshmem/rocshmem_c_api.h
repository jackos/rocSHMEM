/******************************************************************************
 * Copyright (c) Advanced Micro Devices, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/

#ifndef LIBRARY_INCLUDE_ROCSHMEM_C_API_H
#define LIBRARY_INCLUDE_ROCSHMEM_C_API_H

#include <stddef.h>
#include <stdint.h>
#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file rocshmem_c_api.h
 * @brief C API wrapper for rocSHMEM - provides unmangled symbol names
 *
 * This header provides a C-compatible interface to rocSHMEM with unmangled
 * symbols that can be used with dlsym().
 */

/* Opaque types */
typedef void* rocshmem_ctx_t_c;
typedef void* rocshmem_team_t_c;

/* Initialization and finalization */
void rocshmem_init(void);
int rocshmem_init_thread(int requested, int *provided);
void rocshmem_finalize(void);
void rocshmem_query_thread(int *provided);

/* Memory management */
void* rocshmem_malloc(size_t size);
void rocshmem_free(void *ptr);

/* PE queries */
int rocshmem_n_pes(void);
int rocshmem_my_pe(void);

/* Context management */
int rocshmem_ctx_create(int64_t options, rocshmem_ctx_t_c *ctx);
void rocshmem_ctx_destroy(rocshmem_ctx_t_c ctx);

/* Synchronization */
void rocshmem_barrier_all(void);
void rocshmem_sync_all(void);
void rocshmem_fence(void);
void rocshmem_quiet(void);
void rocshmem_ctx_fence(rocshmem_ctx_t_c ctx);
void rocshmem_ctx_quiet(rocshmem_ctx_t_c ctx);

/* Global exit */
void rocshmem_global_exit(int status);

/* Statistics */
void rocshmem_dump_stats(void);
void rocshmem_reset_stats(void);

#ifdef __cplusplus
}
#endif

#endif  // LIBRARY_INCLUDE_ROCSHMEM_C_API_H
