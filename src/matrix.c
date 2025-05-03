#include "../include/matrix.h"




/*

Setup, shutdown, and debug

*/




matrix_t* matrix_alloc(int rows, int cols){

    if(rows <= 0 || cols <= 0){
        printf("rows or cols <= 0");
        return NULL;
    }

    matrix_t* m = (matrix_t*)calloc(1,sizeof(matrix_t));
    if(m == NULL){
        printf("matrix calloc failed\n");
        return NULL;
    }
    m->rows = rows;
    m->cols = cols;
    m->data = (float*)calloc(rows*cols,sizeof(float));

    if(m->data == NULL){
        printf("data calloc failed\n");
        free(m);
        return NULL;
    }

    return m;
}

void matrix_free(matrix_t *m){
    if(m){
        if(m->data){
            free(m->data);
        }
        free(m);
    }
}


/*

all functions below this point rely on the caller
to make sure matrix and matrix data are not null

*/




float matrix_index(matrix_t* m, int row, int col){
    if(row >= m->rows || col >= m->cols){
        return -1.;
    }
    float* mat = (m->data);
    return mat[row * m->cols + col];
}

//need to free
float* matrix_get_row(matrix_t* m, int row){
    const int cols = m->cols;
    float* arr = calloc(cols,sizeof(float));
    if(arr == NULL){
        return NULL;
    }
    int i = 0;
    for(;i<cols;++i){
        arr[i] = matrix_index(m,row,i);
    }
    return arr;
}
//need to free
float* matrix_get_col(matrix_t* m, int col){
    const int cols = m->cols;
    const int rows = m->rows;
    float* arr = calloc(rows,sizeof(float));
    if(arr == NULL){
        return NULL;
    }
    int i = 0;
    for(;i<rows;++i){
        arr[i] = matrix_index(m,i,col);
    }
    return arr;
}


void matrix_write_to_index(matrix_t* m,float val, int row, int col){
    m->data[row*m->cols+col] = val;
}



/*

matrix math :()

*/


//thjis be an ugly ass function
matrix_t* matrix_mult(matrix_t* a, matrix_t* b){

    //gotta break this up at some point

    const int shared_dimension_size_ab = a->cols == b->rows ? a->cols : 0;
    //remove for prod
    if(!shared_dimension_size_ab){
        printf("matrix_dot: a->col: %d != b->row: %d",a->cols,b->rows);
        return NULL;
    }



    matrix_t* c = matrix_alloc(a->rows,b->cols);
    if(c==NULL){
        return NULL;
    }

    int ret = matrix_mult_loop_handler(c,a,b,shared_dimension_size_ab);

    if(ret){
        matrix_free(c);
        return NULL;
    }

    return c;
}

int matrix_mult_loop_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab){
    float* a_arr;
    float* b_arr;

    for(int i = 0; i< a->rows; i++){
        a_arr = matrix_get_row(a,i);

        if(a_arr == NULL){
            matrix_free(c);
            return 1;
        }
        for(int j = 0; j< b->cols; j++){
            b_arr = matrix_get_col(b,j);

            if(b_arr == NULL){
                free(a_arr);
                matrix_free(c);
                return 1;
            }
            float sum = 0.;
            for(int k = 0; k < shared_dimension_size_ab; ++k){
                sum += ((a_arr[k]) * (b_arr[k]));
            }
            matrix_write_to_index(c,sum,i,j);
            free(b_arr);
        }
        free(a_arr);
    }

    return 0;
}
