#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct matrix_t {
    int rows;
    int cols;
    float* data; //index a value at i,j => data[i * cols + j]
} matrix_t;

matrix_t* matrix_alloc(int rows, int cols);
void matrix_free(matrix_t* m);
void matrix_fill(matrix_t* m, float value);
void matrix_copy(matrix_t* dest, matrix_t* src);
matrix_t* matrix_mult(matrix_t* a, matrix_t* b);
void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out);
void matrix_apply(matrix_t* m, float(*func)(float));
void matrix_print(const matrix_t* m);
float matrix_index(matrix_t* m,int r, int c);
void matrix_set_linear_range(matrix_t* m);
float* matrix_get_row(matrix_t* m, int row);
float* matrix_get_col(matrix_t* m, int col);
void matrix_print_rowcol(float* arr, int size, int flag);//flag 1 = row 0 = col
void matrix_write_to_index(matrix_t* m, float val, int row, int col);
int matrix_mult_loop_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab);
