#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/layer.h"
#include "../include/data.h"
#include "../include/model.h"
#include "../include/debug.h"
#include "../include/activation.h"
#include "../include/loss.h"


int main(int argc, char* argv[]){

    srand(time(NULL));
    // Set up an input matrix: 2 samples, 3 features each
        matrix_t* input = matrix_alloc(1000, 1000);
        if (!input) return 1;

        matrix_set_rand_val(input); // fills 0, 1, 2, ..., 5
        //printf("Input:\n");
        //matrix_print(input);

        // Create a single layer: 3 input features -> 2 outputs, using ReLU
        layer_t* l1 = layer_init(1000, 1000, A_RELU);
        if (!l1) {
            matrix_free(input);
            return 1;
        }

        // Forward pass
        layer_forward(l1, input);
        //printf("\nLayer Output:\n");
        if(l1->output == NULL){
            return 1;
        }
        //matrix_print(l1->output);

        //printf("\nLayer weights:\n");
        //matrix_print(l1->weights);
        //printf("\nLayer biases:\n");
        //matrix_print(l1->biases);

        // Clean up
        matrix_free(input);
        matrix_free(l1->output); // forward pass allocates this
        matrix_free(l1->weights);
        matrix_free(l1->biases);
        free(l1);
    return 0;
}


/*

matrix_t* mat = matrix_alloc(3, 4);
if(mat == NULL){
    return 1;
}
matrix_t* mat2 = matrix_alloc(4,3);
if(mat2 == NULL){
    matrix_free(mat);
    return 1;
}
matrix_set_linear_range(mat);
matrix_set_linear_range(mat2);
matrix_print(mat);
matrix_print(mat2);


matrix_t* mat3 = matrix_mult(mat, mat2);
if(mat3 != NULL){
    matrix_print(mat3);
}

matrix_t* mat4 = matrix_transpose(mat);
if(mat4 != NULL){
    matrix_print(mat4);
}
matrix_free(mat4);
matrix_free(mat3);
matrix_free(mat2);
matrix_free(mat);



*/
