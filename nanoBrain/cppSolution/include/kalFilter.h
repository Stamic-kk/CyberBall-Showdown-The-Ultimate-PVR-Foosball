#ifndef KAL_FILTER_H_
#define KAL_FILTER_H_
#include "ukal.h"
#include "predictor.h"

/*Change config.hh for Matrix configuration modification*/

Filter_t kFilter;

const Index_t n_states = 4;
const Index_t n_measurements = 2;
const MatrixEntry_t dt = SAMPLE_RATE;
const MatrixEntry_t stdx = 1.3940;      //change this
const MatrixEntry_t stdy = stdx;        //change this

const MatrixEntry_t varx = (stdx*stdx) / 3;
const MatrixEntry_t vary = varx;
const MatrixEntry_t varv = 2*sqrt(2)*(varx / (dt * dt));
const MatrixEntry_t vartheta = sqrt(11);


Matrix_t y;
Matrix_t x;
Matrix_t Phi;
Matrix_t gamma;
Matrix_t Q;
Matrix_t P;
Matrix_t R;
Matrix_t H;
Matrix_t fx;
Matrix_t hx;

#endif