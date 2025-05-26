#include "../include/matrix.h"
#include <pthread.h>
#include <time.h>


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
    m->data = (double*)calloc(rows*cols,sizeof(double));

    if(m->data == NULL){
        printf("data calloc failed\n");
        free(m);
        return NULL;
    }

    return m;
}

void matrix_free(matrix_t *m){
    if(m != NULL){
        if(m->data != NULL){
            free(m->data);
            m->data = NULL;
        }
        free(m);
    }
}

matrix_t* matrix_mult(matrix_t* a, matrix_t* b){
    if(a == NULL || b == NULL || a->data == NULL || b->data == NULL){
        return NULL;
    }

    const int shared_dimension_size_ab = a->cols == b->rows ? a->cols : 0;


    if(!shared_dimension_size_ab){
        printf("matrix_dot: a->col: %d != b->row: %d",a->cols,b->rows);
        return NULL;
    }



    matrix_t* c = matrix_alloc(a->rows,b->cols);
    if(c==NULL){
        return NULL;
    }


    matrix_mult_thread_handler(c,a,b,shared_dimension_size_ab);
    return c;
}


//this is a pure helper function. it can basically be treated as inline code...... I think
inline void matrix_mult_thread_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab){

    //double start  = get_time_sec();

    //way too many threads being spawned. need to limit this to just a few.

    const int a_rows = a->rows;
    pthread_t threads[a_rows];
    for(int i = 0; i<a_rows;++i){
        thread_payload_t* p = calloc(1,sizeof(thread_payload_t));
        if(p == NULL){
            return;
        }
        *p = (thread_payload_t){shared_dimension_size_ab,i,a,b,c};
        pthread_create(threads+i, NULL, thread_matrix_mult, p);
    }
    for(int i = 0; i<a_rows;++i){
        pthread_join(threads[i],NULL);
    }


    //double end = get_time_sec();
    //printf("MATRIX MULT TIME: %0.5f seconds\n",end-start);
}

//could use threading
void matrix_sub_ip(matrix_t* a, matrix_t* b){
    const int rows = a->rows;
    const int cols = a->cols;
    const int size = rows*cols;
    double* a_arr = a->data;
    double* b_arr = b->data;
    int i = 0;
    for(;i<size;++i){
        a_arr[i] = a_arr[i] - b_arr[i];
    }
}


void matrix_copy(matrix_t* dest, matrix_t* src){
    if(dest == NULL || dest->data == NULL || src == NULL || src->data == NULL){
        return;
    }
    double* d_arr = dest->data;
    double* s_arr = src->data;
    const int rows = src->rows;
    const int cols = src->cols;
    const int size = rows*cols;

    if(rows != dest->rows || cols != dest->cols){
        return;
    }
    int i = 0;
    for(;i<size;++i){
        d_arr[i] = s_arr[i];
    }
}

//could use threading
void matrix_apply_activation_ip(matrix_t* m, double(*p_act_func)(double)){
    const int size = m->rows*m->cols;
    double* arr = m->data;
    int i = 0;
    for(;i<size;++i){
        arr[i] = p_act_func(arr[i]);
    }
}

//could use threading
void matrix_apply_activation_new(matrix_t* m, double(*p_act_func)(double), matrix_t* out){
    const int size = out->rows * out->cols;
    double* m_arr = m->data;
    double* out_arr = out->data;
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
    double* m_arr = m->data;
    double* mt_arr = mt->data;

    const int m_rows = m->rows;
    const int m_cols = m->cols;

    int i  = 0;
    for(; i<m_rows; ++i){
        int j = 0;
        for(; j<m_cols; ++j){
            mt_arr[j * m_rows + i] = m_arr[i*m_cols+j];
        }
    }

    return mt;
}

void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out){
    const int size = a->rows*a->cols;
    int i = 0;
    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(;i<size;++i){
        out_arr[i] = a_arr[i] + b_arr[i];
    }
}
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out){

    const int size = a->rows*a->cols;
    int i = 0;
    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(;i<size;++i){
        out_arr[i] = a_arr[i] - b_arr[i];
    }
}

void matrix_scalar_mult(matrix_t* a, double scalar){
    const int size = a->rows*a->cols;
    int i = 0;
    double* arr = a->data;
    for(;i<size;++i){
        arr[i] *= scalar;
    }
}
void matrix_hadamard(matrix_t* a, matrix_t*b, matrix_t* out){
    const int size = a->rows*a->cols;
    int i = 0;
    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(;i<size; ++i){
        out_arr[i] = a_arr[i] * b_arr[i];
    }
}

void matrix_add_bias(matrix_t* z, const matrix_t* bias){

    double* b = bias->data;
    const int rows = z->rows;
    const int cols = z->cols;
    double* z_arr = z->data;
    int i = 0;
    for(;i< rows; ++i){
        int j = 0;
        for(;j<cols;++j){

            z_arr[i*cols+j] = z_arr[i*cols+j] + b[j];
        }
    }
}

matrix_t* matrix_sum_rows(matrix_t* x){
    matrix_t* x_rows_summed = matrix_alloc(1,x->cols);
    if(x_rows_summed == NULL){
        return NULL;
    }

    double* s_arr = x_rows_summed->data;
    double* x_arr = x->data;

    const int rows = x->rows;
    const int cols = x->cols;
    int j = 0;
    double sum;
    for(;j<cols;++j){
        sum = 0.;
        int i = 0;
        for(;i<rows;++i){
            sum+=x_arr[i*cols+j];
        }
        s_arr[j] = sum;
    }
    return x_rows_summed;
}
