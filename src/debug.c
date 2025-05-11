#include "../include/debug.h"

void matrix_print_rowcol(double* arr, int size,int flag){
    int i = 0;
    for(;i<size;++i){
        printf("[%.2f]",arr[i]);
        if(!flag){
            printf("\n");
        }
    }
    printf("\n");
}


void matrix_print(const matrix_t *m){

    const int cols = m->cols;
    const int mat_size = m->rows * cols;
    int i = 0;
    double* d = m->data;
    for(;i<mat_size;++i){
        if(i%cols == 0){
            printf("\n");
        }
        printf("[%0.2f] ",d[i]);
    }
    printf("\n");
}


void matrix_set_linear_range(matrix_t* m){
    int i = 0;
    const int num_elem = m->cols * m->rows;
    double* arr = m->data;
    for(; i<num_elem; ++i){
        arr[i] = (double)i;
    }
}

void matrix_set_rand_val(matrix_t* m){
    int i = 0;
    const int num_elem = m->cols * m->rows;
    double* arr = m->data;
    for(; i<num_elem; ++i){
        arr[i] = randf();
    }
}

void matrix_set_const_val(matrix_t* m, double x){
    int i = 0;
    const int num_elem = m->cols * m->rows;
    double* arr = m->data;
    for(; i<num_elem; ++i){
        arr[i] = x;
    }
}
