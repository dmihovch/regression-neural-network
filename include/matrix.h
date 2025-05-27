#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "timing.h"

typedef struct matrix_t {
    int rows;
    int cols;
    double* data; //index a value at i,j => data[i * cols + j]
} matrix_t;


#include "threading.h"


matrix_t* matrix_alloc(int rows, int cols);
void matrix_free(matrix_t* m);
matrix_t* matrix_mult(matrix_t* a, matrix_t* b);
void matrix_print(const matrix_t* m);
double matrix_index(matrix_t* m,int row, int col);
void matrix_set_linear_range(matrix_t* m);
double* matrix_get_row(matrix_t* m, int row);
double* matrix_get_col(matrix_t* m, int col);
void matrix_print_rowcol(double* arr, int size, int flag);//flag 1 = row 0 = col
void matrix_write_to_index(matrix_t* m, double val, int row, int col);
void matrix_mult_thread_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab);
void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_scalar_mult(matrix_t* a, double scalar);
void matrix_hadamard(matrix_t* a, matrix_t*b, matrix_t* out);
void matrix_add_bias(matrix_t* z, const matrix_t* bias);
matrix_t* matrix_sum_rows(matrix_t* x);
void matrix_sub_ip(matrix_t* a, matrix_t* b);
matrix_t* matrix_transpose(matrix_t* m);
void matrix_copy(matrix_t* dest, matrix_t* src);
void matrix_apply_activation_ip(matrix_t* m, double(*p_act_func)(double));
void matrix_apply_activation_new(matrix_t* m, double(*p_act_func)(double), matrix_t* out);
matrix_t* matrix_copy_alloc_new(matrix_t* src);
