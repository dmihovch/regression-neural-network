#pragma once
#include "matrix.h"
#include <string.h>

#define TRAINING 1
#define TESTING 0

typedef struct dataset_t {
    matrix_t* input;
    matrix_t* output;
    int samples;
    int features;
} dataset_t;

dataset_t* dataset_alloc(int samples, int features);
void normalize_dataset(dataset_t* data);
void shuffle_dataset(dataset_t* data);
void dataset_free(dataset_t* data);
dataset_t* input_parser(char* filepath, int mode);
int count_features(FILE* data_file);
int count_samples(FILE* data_file);
int count_max_line_chars(FILE* data_file);
dataset_t* parse_training(FILE* data_file);
dataset_t* parse_testing(FILE* data_file);
void dataset_free(dataset_t* d);
