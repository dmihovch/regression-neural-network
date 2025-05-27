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
        free(l->weights);
        free(l);
        return NULL;
    }
    l->act = act;

    init_weights(l->weights, l->act, input_size, output_size);
    init_bias(l->biases, l->act);


    l->dbiases = NULL;
    l->dweights = NULL;
    l->dinputs = NULL;
    l->pre_act_z = NULL;
    l->input = NULL;

    return l;
}

void layer_forward(layer_t* layer, matrix_t* input){


    if(layer->output != NULL){
        matrix_free(layer->output);
        layer->output = NULL;
    }

    matrix_t* input_copy = matrix_copy_alloc_new(input);
    if(input_copy == NULL){
        return;
    }
    if(layer->input != NULL){
        matrix_free(layer->input);
    }
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


    //layer->pre_act_z malloc'd here
    if(save_pre_activation_z(layer, w_in)){
        return;
    }

    if(act == A_RELU){
        matrix_apply_activation_ip(w_in, relu);
    }
    else if(act == A_SIGMOID){
        matrix_apply_activation_ip(w_in,sigmoid);
    }

    layer->output = w_in;
}

int save_pre_activation_z(layer_t* layer, matrix_t* z){

    matrix_t* pre_act_z_mat = matrix_alloc(z->rows, z->cols);
    if(pre_act_z_mat == NULL){
        return 1;
    }

    matrix_copy(pre_act_z_mat, z);
    if(layer->pre_act_z != NULL){
        matrix_free(layer->pre_act_z);
    }
    layer->pre_act_z = pre_act_z_mat;
    return 0;
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
    if(layer->pre_act_z != NULL){
        matrix_free(layer->pre_act_z);
        layer->pre_act_z = NULL;
    }
    free(layer);
    return;
}


void layer_backwards(layer_t *l, matrix_t *dA, double learning_rate){
    matrix_t* dZ = activation_backwards(l->pre_act_z, dA, l->act); //modifies l->pre_act_z
    if(dZ == NULL){
        return;
    }
    matrix_t* t_input = matrix_transpose(l->input);
    if(t_input == NULL){
        matrix_free(dZ);
        return;
    }
    matrix_t* dW = matrix_mult(t_input, dZ);
    if(dW == NULL){
        matrix_free(t_input);
        matrix_free(dZ);
        return;
    }
    matrix_t* db = matrix_sum_rows(dZ);
    if(db == NULL){
        matrix_free(dZ);
        matrix_free(t_input);
        matrix_free(dW);
        return;
    }
    matrix_t* t_weights = matrix_transpose(l->weights);
    if(t_weights == NULL){
        matrix_free(dZ);
        matrix_free(t_input);
        matrix_free(dW);
        matrix_free(db);
        return;
    }
    matrix_t* dinputs = matrix_mult(dZ,t_weights);
    if(dinputs == NULL){
        matrix_free(dZ);
        matrix_free(t_input);
        matrix_free(dW);
        matrix_free(db);
        matrix_free(t_weights);
        return;
    }
    if(l->dinputs != NULL){
        matrix_free(l->dinputs);
    }
    l->dinputs = dinputs;

    matrix_scalar_mult(dW, learning_rate);
    matrix_sub_ip(l->weights, dW);

    matrix_scalar_mult(db, learning_rate);
    matrix_sub_ip(l->biases,db);

    matrix_free(dZ);
    matrix_free(t_input);
    matrix_free(dW);
    matrix_free(db);
    matrix_free(t_weights);
}
