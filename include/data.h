#pragma once
#include "matrix.h"
typedef struct dataset_t {
    matrix_t* x_train;
    matrix_t* y_train;
    matrix_t* x_test;
    matrix_t* y_test;
    int train_size;
    int test_size;
} dataset_t;

dataset_t load_dataset(const char* filepath);
void normalize_dataset(dataset_t* data);
void shuffle_dataset(dataset_t* data);
void dataset_free(dataset_t* data);
