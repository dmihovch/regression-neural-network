#include "../include/threading.h"


void* thread_matrix_mult(void* payload){

    if(payload == NULL){
        printf("NULL PAYLOAD IN THREAD\n");
        return NULL;
    }
    thread_payload_t p = *(thread_payload_t*)payload;
    free(payload);
    double* a_arr = p.a->data;
    double* b_arr = p.b->data;
    double* c_arr = p.c->data;
    const int shared_dim = p.shared_dim;
    const int i = p.i;
    const int a_cols = p.a->cols;
    const int c_cols = p.c->cols;
    const int b_cols = p.b->cols;

    double* a_i = a_arr+i*a_cols;
    double a_ik;

    double* b_k;

    double* c_i = c_arr+i*c_cols;
    for(int k = 0; k < shared_dim; ++k){
        a_ik = a_i[k];
        b_k = b_arr+k*b_cols;
        for(int j = 0; j<b_cols; ++j){
            c_i[j] += a_ik * b_k[j];
        }
    }



    return NULL;
}
