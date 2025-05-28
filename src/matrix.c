#include "../include/matrix.h"
#include <pthread.h>
#include <time.h>


matrix_t* matrix_alloc(int rows, int cols){

    if(rows <= 0 || cols <= 0){
        printf("rows or cols <= 0\n");
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



    matrix_t* prod = matrix_alloc(a->rows,b->cols);
    if(prod==NULL){
        return NULL;
    }


    matrix_mult_thread_handler(prod,a,b,shared_dimension_size_ab);
    return prod;
}


//this is a pure helper function. it can basically be treated as inline code...... I think
inline void matrix_mult_thread_handler(matrix_t* prod, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab){

    //double start  = get_time_sec();

    //way too many threads being spawned. need to limit this to just a few.

    const int a_rows = a->rows;
    pthread_t threads[a_rows];
    for(int i = 0; i<a_rows;++i){
        thread_payload_t* p = calloc(1,sizeof(thread_payload_t));
        if(p == NULL){
            return;
        }
        *p = (thread_payload_t){shared_dimension_size_ab,i,a,b,prod};
        pthread_create(threads+i, NULL, thread_matrix_mult, p);
    }
    for(int i = 0; i<a_rows;++i){
        pthread_join(threads[i],NULL);
    }


    //double end = get_time_sec();
    //printf("MATRIX MULT TIME: %0.5f seconds\n",end-start);
}

//could use threading

//subtracts b from a, in place
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
matrix_t* matrix_apply_activation_new(matrix_t* m, double(*p_act_func)(double)){

    const int rows = m->rows;
    const int cols = m->cols;

    matrix_t* new = matrix_alloc(rows,cols);
    if(new == NULL){
        return NULL;
    }

    const int size = rows*cols;
    double* m_arr = m->data;
    double* new_arr = new->data;

    for(int i = 0;i<size;++i){
        new_arr[i] = p_act_func(m_arr[i]);
    }


    return new;
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


    for(int i = 0; i<m_rows; ++i){
        for(int j = 0; j<m_cols; ++j){
            mt_arr[j * m_rows + i] = m_arr[i*m_cols+j];
        }
    }

    return mt;
}

void matrix_add(matrix_t* a, matrix_t* b, matrix_t* out){
    const int size = a->rows*a->cols;

    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(int i = 0;i<size;++i){
        out_arr[i] = a_arr[i] + b_arr[i];
    }
}

//subtracts b from a, stored in out
void matrix_sub(matrix_t* a, matrix_t* b, matrix_t* out){

    const int size = a->rows*a->cols;

    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(int i = 0;i<size;++i){
        out_arr[i] = a_arr[i] - b_arr[i];
    }
}

void matrix_scalar_mult(matrix_t* m, double scalar){
    const int size = m->rows*m->cols;

    double* m_arr = m->data;
    for(int i = 0;i<size;++i){
        m_arr[i] *= scalar;
    }
}
void matrix_hadamard(matrix_t* a, matrix_t*b, matrix_t* out){
    const int size = a->rows*a->cols;

    double* a_arr = a->data;
    double* b_arr = b->data;
    double* out_arr = out->data;
    for(int i = 0;i<size; ++i){
        out_arr[i] = a_arr[i] * b_arr[i];
    }
}

void matrix_add_bias(matrix_t* weighted_input, const matrix_t* bias){

    double* bias_arr = bias->data;
    const int rows = weighted_input->rows;
    const int cols = weighted_input->cols;
    double* weighted_input_arr = weighted_input->data;

    for(int i = 0;i< rows; ++i){

        for(int j = 0;j<cols;++j){

            weighted_input_arr[i*cols+j] = weighted_input_arr[i*cols+j] + bias_arr[j];
        }
    }
}

matrix_t* matrix_sum_rows(matrix_t* m){
    matrix_t* m_rows_summed = matrix_alloc(1,m->cols);
    if(m_rows_summed == NULL){
        return NULL;
    }

    double* sum_arr = m_rows_summed->data;
    double* mat_arr = m->data;

    const int rows = m->rows;
    const int cols = m->cols;

    double sum;
    for(int j = 0;j<cols;++j){
        sum = 0.;

        for(int i = 0;i<rows;++i){
            sum+=mat_arr[i*cols+j];
        }
        sum_arr[j] = sum;
    }
    return m_rows_summed;
}

matrix_t* matrix_copy_alloc_new(matrix_t* src){
    matrix_t* dest = matrix_alloc(src->rows, src->cols);
    if(dest == NULL){
        return NULL;
    }
    matrix_copy(dest,src);
    return dest;
}
