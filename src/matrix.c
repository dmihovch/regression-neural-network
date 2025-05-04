#include "../include/matrix.h"




/*

Setup, shutdown, and debug

*/


/*

exposed functions

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

//you can chuck whatever the fuck you'd like in here as long as the compiler thinks its a pointer to a matrix,  and we gucci
void matrix_free(matrix_t *m){
    if(m != NULL){
        if(m->data != NULL){
            free(m->data);
        }
        free(m);
    }
}


/*

all functions below this point rely on the caller
to make sure matrix and matrix data are not null

*/





float* matrix_index(matrix_t* m, int row, int col){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    if(row < 0 || col < 0 || row >= m->rows || col >= m->cols){
        return NULL;
    }
    return &m->data[row * m->cols + col];
}

//need to free
float* matrix_get_row(matrix_t* m, int row){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    const int cols = m->cols;
    float* arr = calloc(cols,sizeof(float));
    if(arr == NULL){
        return NULL;
    }
    int i = 0;
    float* xi;
    for(;i<cols;++i){
        if((xi=matrix_index(m,row,i))==NULL){
            free(arr);
            return NULL;
        }
        arr[i] = *xi;
    }
    return arr;
}
//need to free
float* matrix_get_col(matrix_t* m, int col){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    const int rows = m->rows;
    float* arr = calloc(rows,sizeof(float));
    if(arr == NULL){
        return NULL;
    }
    int i = 0;
    float* xi;
    for(;i<rows;++i){
        if((xi=matrix_index(m,i,col))==NULL){
            free(arr);
            return NULL;
        }
        arr[i] = *xi;
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

    if(a == NULL || b == NULL || a->data == NULL || b->data == NULL){
        return NULL;
    }

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

    matrix_mult_loop_handler(c,a,b,shared_dimension_size_ab);
    return c;
}


//this is a pure helper function. it can basically be treated as inline code...... I think
void matrix_mult_loop_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab){
    float* a_arr;
    float* b_arr;

    for(int i = 0; i< a->rows; i++){
        a_arr = matrix_get_row(a,i);

        if(a_arr == NULL){
            matrix_free(c);
            c = NULL;
            return;
        }
        for(int j = 0; j< b->cols; j++){
            b_arr = matrix_get_col(b,j);

            if(b_arr == NULL){
                free(a_arr);
                matrix_free(c);
                c = NULL;
                return;
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

    return;
}


void matrix_copy(matrix_t* dest, matrix_t* src);


void matrix_apply_activation_ip(matrix_t* m, float(*p_act_func)(float)){
    const int size = m->rows*m->cols;
    float* arr = m->data;
    int i = 0;
    for(;i<size;++i){
        arr[i] = p_act_func(arr[i]);
    }
}
void matrix_apply_activation_new(matrix_t* m, float(*p_act_func)(float), matrix_t* out){
    const int size = out->rows * out->cols;
    float* m_arr = m->data;
    float* out_arr = out->data;
    int i = 0;
    for(;i<size;++i){
        out_arr[i] = p_act_func(m_arr[i]);
    }
}



matrix_t* matrix_transpose(matrix_t* m){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    matrix_t* mt = matrix_alloc(m->cols, m->rows);
    if(mt == NULL){
        return NULL;
    }
    float* m_arr = m->data;
    float* mt_arr = mt->data;

    const int m_rows = m->rows;
    const int m_cols = m->cols;
    const int mt_cols = mt->cols;

    int i  = 0;
    float* xi;
    for(; i<m_rows; ++i){
        int j = 0;
        for(; j<m_cols; ++j){
            if((xi=matrix_index(m,i,j))==NULL){
                return NULL;
            }
            mt_arr[j * mt_cols + i] = *xi;
        }
    }

    return mt;
}

void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out){
    const int size = a->rows*a->cols;
    int i = 0;
    float* a_arr = a->data;
    float* b_arr = b->data;
    float* out_arr = out->data;
    for(;i<size;++i){
        out_arr[i] = a_arr[i] + b_arr[i];
    }
}
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out){

    const int size = a->rows*a->cols;
    int i = 0;
    float* a_arr = a->data;
    float* b_arr = b->data;
    float* out_arr = out->data;
    for(;i<size;++i){
        out_arr[i] = a_arr[i] - b_arr[i];
    }
}

void matrix_scalar_mult(matrix_t* a, float scalar){
    const int size = a->rows*a->cols;
    int i = 0;
    float* arr = a->data;
    for(;i<size;++i){
        arr[i] *= scalar;
    }
}
void matrix_hadamard(matrix_t* a, matrix_t*b, matrix_t* out){

    //doesn't matter which one, they are same size; hmmmmmmmm this could raise some issues potensh
    const int size = a->rows*a->cols;
    int i = 0;
    float* a_arr = a->data;
    float* b_arr = b->data;
    float* out_arr = out->data;
    for(;i<size; ++i){
        out_arr[i] = a_arr[i] * b_arr[i];
    }
}
