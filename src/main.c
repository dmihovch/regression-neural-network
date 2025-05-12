#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/layer.h"
#include "../include/data.h"
#include "../include/model.h"
#include "../include/debug.h"
#include "../include/activation.h"
#include "../include/loss.h"


int main(/*int argc, char* argv[]*/){
    srand(time(NULL));

    matrix_t* init = matrix_alloc(25, 25);
    matrix_set_rand_val(init);

    // Set up an input matrix: 2 samples, 3 features each
    int layer_sizes[5] = {25,75,150,15,1};
    model_t* model = model_init(25, layer_sizes,A_RELU, 5, 0);
    for(int i = 0; i<5;++i){
        if(i == 0){

            layer_forward(model->layers[i], init);
            matrix_free(init);
        }
        else{
            layer_forward(model->layers[i], model->layers[i-1]->output);
        }

    }

    matrix_print(model->layers[4]->output);

    matrix_t* y_true = matrix_alloc(25, 1);
    if(y_true == NULL){
        model_free(model);
        return 1;
    }

    matrix_set_const_val(y_true, 1.0);
    double mse = mean_squared_error_loss(y_true, model->layers[4]->output);

    printf("mse: %0.5f\n",mse);
    matrix_free(y_true);
    model_free(model);
    return 0;
}
