#pragma once
#include "matrix.h"
#include "utils.h"



typedef struct thread_payload_t {
    int shared_dim;
    int i;
    matrix_t* a;
    matrix_t* b;
    matrix_t* c;
} thread_payload_t;

void* thread_matrix_mult(void* payload);
