#include "../include/activation.h"

float relu(float x){
    return x > 0 ? x : 0.;
}

float drelu(float x){
    return derivative(relu, x, HSTEP);
}

float sigmoid(float x){
    return 1. / (1. + expf(-x));
}

float dsigmoid(float x){
    return derivative(sigmoid, x, HSTEP);
}
