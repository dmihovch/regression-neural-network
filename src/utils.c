#include "../include/utils.h"

uint32_t lcgstate = 0;

double derivative(double (*p_act_func)(double), double x, double h_step){
    return (p_act_func(x+h_step)-p_act_func(x-h_step)) / (2*h_step);
}

double lcgrandf(){
    uint32_t nstate = (A * lcgstate + C) & 0x7fffffff; //ensured no overflow issues
    lcgstate = nstate;
    return (double)nstate/2147483647.0; //[0,1)
}

void seedlcgrandf(uint32_t seed){
    lcgstate = seed;
}
