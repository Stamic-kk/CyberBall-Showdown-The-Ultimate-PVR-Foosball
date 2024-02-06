#ifndef KAL_FILTER_H_
#define KAL_FILTER_H_
extern "C"{
#include "ukal.h"
}
#include "predictor.h"
#include "camera.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <algorithm>
using namespace cv;
using std::vector;
using std::pair;
using std::string;

MatrixError_t setup(Matrix_t y1);
static void get_phi(Matrix_t * const Phi, 
                    const Matrix_t * const x, 
                    const MatrixEntry_t dt);
static void get_fx(Matrix_t * const fx,
                   const Matrix_t * const x,
                   const MatrixEntry_t dt);
static void get_hx(Matrix_t * const hx, 
                   const Matrix_t * const x);
MatrixError_t init_Matrices();
MatrixError_t setup(Matrix_t y1);
MatrixError_t set_filter();
MatrixError_t put_data (Matrix_t *y, float x_coord, float y_coord);
void visualize(Matrix_t x, Mat &background);
void test_filter(std::string path);
bool read_sim_data(string path, vector<pair<float, float>> &data);
#endif