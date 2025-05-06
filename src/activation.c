#include "../include/activation.h"

double relu(double x){
    return x > 0 ? x : 0.;
}

double drelu(double x){
    return derivative(relu, x, HSTEP);
}

double sigmoid(double x){
    return 1. / (1. + exp(-x));
}

double dsigmoid(double x){
    return derivative(sigmoid, x, HSTEP);
}
