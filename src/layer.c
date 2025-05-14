#include "../include/layer.h"
#include <math.h>

//mallocs a layer
layer_t* layer_init(int input_size, int output_size, activation_type act){

    layer_t* l = calloc(1,sizeof(layer_t));
    if(l == NULL){
        return NULL;
    }
    l->weights = matrix_alloc(input_size, output_size);
    if(l->weights == NULL){
        free(l);
        return NULL;
    }
    l->biases = matrix_alloc(1,output_size);
    if(l->biases == NULL){
        free(l->weights); //technically only need to call free here, but I'll just do it for kicks
        free(l);
        return NULL;
    }
    l->act = act;

    init_weights(l->weights, l->act, input_size, output_size);
    init_bias(l->biases, l->act);


    l->dbiases = NULL;
    l->dweights = NULL;
    l->dinputs = NULL;

    return l;
}

void layer_forward(layer_t* layer, matrix_t* input){


    if(layer->output != NULL){
        matrix_free(layer->output);
        layer->output = NULL;
    }


    const int in_rows = input->rows;
    const int in_cols = input->cols;
    matrix_t* input_copy = matrix_alloc(in_rows, in_cols);
    if(input_copy == NULL){
        return;
    }
    matrix_copy(input_copy, input);

    layer->input = input_copy;
    matrix_t* in = layer->input;
    matrix_t* w = layer->weights;
    matrix_t* b = layer->biases;
    activation_type act = layer->act;

    matrix_t* w_in = matrix_mult(in,w);
    if(w_in == NULL){
        layer->output = NULL;
        return;
    }

    matrix_add_bias(w_in, b);
    if(act == A_RELU){
        matrix_apply_activation_ip(w_in, relu);
    }
    else if(act == A_SIGMOID){
        matrix_apply_activation_ip(w_in,sigmoid);
    }

    layer->output = w_in;
}



void init_randf_vals(matrix_t* m){
    const int size = m->rows * m->cols;
    double* arr = m->data;
    int i = 0;
    for(;i<size;++i){
        arr[i] = lcgrandf();
    }
}


void init_bias(matrix_t* m, activation_type act){
    if(act == A_RELU){
        double* arr = m->data;
        const int size = m->rows*m->cols;
        int i = 0;
        for(;i<size;++i){
            arr[i] = 0.01;
        }
    }
}


void init_weights(matrix_t* m, activation_type act, int inputs, int outputs){
    if(act == A_RELU){
        const double sigma = sqrt(2./inputs);
        init_weights_relu(m, 0.,sigma); //mu,sigma for reLu
    }
    if(act == A_SIGMOID){
        init_weights_sigmoid(m, inputs, outputs);
    }
    if(act == A_NONE){
        init_randf_vals(m);
    }
}
void init_weights_relu(matrix_t* m, double mu, double sigma){
    double* arr = m->data;
    const int size = m->rows*m->cols;
    int i = 0;
    for(;i<size;++i){
        arr[i] = box_muller_tran(mu,sigma);
    }
}

double box_muller_tran(double mu, double sigma){
    double u1 = 0.;
    while(u1 == 0.){
        u1 = lcgrandf();
    }
    double u2 = lcgrandf();

    const double mult1 = sqrt(-2*log(u1));
    const double mult2 = cos(2*M_PI*u2);
    const double z = mult1*mult2;
    return mu + sigma * z;
}

void init_weights_sigmoid(matrix_t* m, int inputs, int outputs){
    double* arr = m->data;
    const double min = -sqrt(6./(inputs+outputs));
    const double max = sqrt(6./(inputs+outputs));
    const int size = m->rows*m->cols;
    const double mult = min + (max - min);
    int i = 0;
    for(;i<size;++i){
        arr[i] = mult * lcgrandf();
    }
}

void layer_free(layer_t *layer){

    if(layer == NULL){
        return;
    }
    if(layer->weights != NULL){
        matrix_free(layer->weights);
        layer->weights = NULL;
    }
    if(layer->biases != NULL){
        matrix_free(layer->biases);
        layer->biases = NULL;
    }
    if(layer->input != NULL){
        matrix_free(layer->input);
        layer->input = NULL;
    }
    if(layer->output != NULL){
        matrix_free(layer->output);
        layer->output = NULL;
    }
    if(layer->dweights != NULL){
        matrix_free(layer->dweights);
        layer->dweights = NULL;
    }
    if(layer->dbiases != NULL){
        matrix_free(layer->dbiases);
        layer->dbiases = NULL;
    }
    if(layer->dinputs != NULL){
        matrix_free(layer->dinputs);
        layer->dinputs = NULL;
    }
    free(layer);
    return;
}
