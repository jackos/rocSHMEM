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

#include "rocshmem/rocshmem_c_api.h"
#include "rocshmem/rocshmem.hpp"

extern "C" {

/* Initialization and finalization */
void rocshmem_init(void) {
    rocshmem::rocshmem_init();
}

int rocshmem_init_thread(int requested, int *provided) {
    return rocshmem::rocshmem_init_thread(requested, provided);
}

void rocshmem_finalize(void) {
    rocshmem::rocshmem_finalize();
}

void rocshmem_query_thread(int *provided) {
    rocshmem::rocshmem_query_thread(provided);
}

/* Memory management */
void* rocshmem_malloc(size_t size) {
    return rocshmem::rocshmem_malloc(size);
}

void rocshmem_free(void *ptr) {
    rocshmem::rocshmem_free(ptr);
}

/* PE queries */
int rocshmem_n_pes(void) {
    return rocshmem::rocshmem_n_pes();
}

int rocshmem_my_pe(void) {
    return rocshmem::rocshmem_my_pe();
}

/* Context management */
int rocshmem_ctx_create(int64_t options, rocshmem_ctx_t_c *ctx) {
    return rocshmem::rocshmem_ctx_create(options,
        reinterpret_cast<rocshmem::rocshmem_ctx_t*>(ctx));
}

void rocshmem_ctx_destroy(rocshmem_ctx_t_c ctx) {
    rocshmem::rocshmem_ctx_destroy(*reinterpret_cast<rocshmem::rocshmem_ctx_t*>(&ctx));
}

/* Synchronization */
void rocshmem_barrier_all(void) {
    rocshmem::rocshmem_barrier_all();
}

void rocshmem_sync_all(void) {
    rocshmem::rocshmem_sync_all();
}

void rocshmem_fence(void) {
    rocshmem::rocshmem_fence();
}

void rocshmem_quiet(void) {
    rocshmem::rocshmem_quiet();
}

void rocshmem_ctx_fence(rocshmem_ctx_t_c ctx) {
    rocshmem::rocshmem_ctx_fence(*reinterpret_cast<rocshmem::rocshmem_ctx_t*>(&ctx));
}

void rocshmem_ctx_quiet(rocshmem_ctx_t_c ctx) {
    rocshmem::rocshmem_ctx_quiet(*reinterpret_cast<rocshmem::rocshmem_ctx_t*>(&ctx));
}

/* Global exit */
void rocshmem_global_exit(int status) {
    rocshmem::rocshmem_global_exit(status);
}

/* Statistics */
void rocshmem_dump_stats(void) {
    rocshmem::rocshmem_dump_stats();
}

void rocshmem_reset_stats(void) {
    rocshmem::rocshmem_reset_stats();
}

}  // extern "C"
