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

    // Set up an input matrix: 2 samples, 3 features each
        matrix_t* input = matrix_alloc(1000, 10000);
        if (!input) return 1;

        matrix_set_rand_val(input);
        layer_t* l1 = layer_init(10000, 1000, A_RELU);
        if (!l1) {
            matrix_free(input);
            return 1;
        }
        layer_forward(l1, input);
        if(l1->output == NULL){
            matrix_free(input);
            matrix_free(l1->weights);
            matrix_free(l1->biases);
            free(l1);
            return 1;
        }
        matrix_free(input);
        matrix_free(l1->output); // forward pass allocates this
        matrix_free(l1->weights);
        matrix_free(l1->biases);
        free(l1);
    return 0;
}
