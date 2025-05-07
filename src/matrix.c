#include "../include/matrix.h"
#include <time.h>




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
    m->data = (double*)calloc(rows*cols,sizeof(double));

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





double matrix_index(matrix_t* m, int row, int col){
    return m->data[row * m->cols + col];
}

//need to free
double* matrix_get_row(matrix_t* m, int row){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    const int cols = m->cols;
    double* arr = calloc(cols,sizeof(double));
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
double* matrix_get_col(matrix_t* m, int col){
    if(m == NULL || m->data == NULL){
        return NULL;
    }
    const int rows = m->rows;
    double* arr = calloc(rows,sizeof(double));
    if(arr == NULL){
        return NULL;
    }
    int i = 0;
    for(;i<rows;++i){
        arr[i] = matrix_index(m,i,col);
    }
    return arr;
}


void matrix_write_to_index(matrix_t* m,double val, int row, int col){
    //printf("m->data[%d]\n",(row*m->cols+col));
    m->data[row*m->cols+col] = val;
}



/*

matrix math :()

*/


//thjis be an ugly ass function
matrix_t* matrix_mult(matrix_t* a, matrix_t* b){

    clock_t start  = clock();
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

    clock_t end = clock();

    double time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("MATRIX MULT TIME:\n%0.5f seconds\n",time);
    return c;
}


//this is a pure helper function. it can basically be treated as inline code...... I think
void matrix_mult_loop_handler(matrix_t* c, matrix_t* a, matrix_t* b, const int shared_dimension_size_ab){
    double* a_arr = a->data;
    double* b_arr = b->data;
    double* c_arr = c->data;
    const int a_rows = a->rows;
    const int a_cols = a->cols;
    const int b_cols = b->cols;
    const int b_rows = b->rows;
    const int c_cols = c->cols;
    double a_ik;



    for(int i = 0; i<a_rows;++i){
        for(int k = 0; k < shared_dimension_size_ab; ++k){
            a_ik = a_arr[i*a_cols+k];
            for(int j = 0; j<b_cols; ++j){
                c_arr[i*c_cols+j] += a_ik * b_arr[k*b_cols+j];
            }
        }
    }



    return;
}
/*

for(int i = 0; i< a->rows; i++){
    for(int j = 0; j< b->cols; j++){
        double sum = 0.;
        for(int k = 0; k < shared_dimension_size_ab; ++k){
            sum += matrix_index(a,i,k) * matrix_index(b,k,j);
        }
        //printf("c->rows:%d\nc->cols:%d\ni:%d\nj:%d\n",c->rows,c->cols,i,j);
        matrix_write_to_index(c,sum,i,j);
    }
}
*/


void matrix_copy(matrix_t* dest, matrix_t* src);


void matrix_apply_activation_ip(matrix_t* m, double(*p_act_func)(double)){
    const int size = m->rows*m->cols;
    double* arr = m->data;
    int i = 0;
    for(;i<size;++i){
        arr[i] = p_act_func(arr[i]);
    }
}
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
    const int mt_cols = mt->cols;

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

    //doesn't matter which one, they are same size; hmmmmmmmm this could raise some issues potensh
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

    double xi_b;
    int i = 0;
    for(;i< rows; ++i){
        int j = 0;
        for(;j<cols;++j){
            xi_b = matrix_index(z,i,j) + b[i];
            matrix_write_to_index(z, xi_b, i, j);
        }
    }
}
