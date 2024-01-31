#include "kalFilter.h"
#include <math.h>
#include "camera.h"

using namespace cv;


static void get_phi(Matrix_t * const Phi, 
                    const Matrix_t * const x, 
                    const MatrixEntry_t dt) {
    MatrixEntry_t v = x->entry[2][0];
    MatrixEntry_t theta = x->entry[3][0];

    ulapack_eye(Phi);
    ulapack_edit_entry(Phi, 0, 2, dt*cos(theta));
    ulapack_edit_entry(Phi, 1, 2, dt*sin(theta));

    ulapack_edit_entry(Phi, 0, 3, -1*dt*v*sin(theta));
    ulapack_edit_entry(Phi, 1, 3, dt*v*cos(theta));
}

// propagation
static void get_fx(Matrix_t * const fx,
                   const Matrix_t * const x,
                   const MatrixEntry_t dt) {
    MatrixEntry_t v = x->entry[2][0];
    MatrixEntry_t theta = x->entry[3][0];

    ulapack_edit_entry(fx, 0, 0, x->entry[0][0] + dt * v * cos(theta));
    ulapack_edit_entry(fx, 1, 0, x->entry[1][0] + dt * v * sin(theta));
    ulapack_edit_entry(fx, 2, 0, x->entry[2][0]);
    ulapack_edit_entry(fx, 3, 0, x->entry[3][0]);
}
// Observor
static void get_hx(Matrix_t * const hx, 
                   const Matrix_t * const x) {
    ulapack_edit_entry(hx, 0, 0, x->entry[0][0]);
    ulapack_edit_entry(hx, 1, 0, x->entry[1][0]);
}

MatrixError_t init_Matrices(){
    ulapack_init(&y, n_measurements, 1);
    ulapack_init(&x, n_states, 1);
    ulapack_init(&Phi, n_states, n_states);
    ulapack_init(&gamma, n_states, n_measurements);
    ulapack_init(&Q, gamma.n_cols, gamma.n_cols);
    ulapack_init(&P, n_states, n_states);
    ulapack_init(&R, n_measurements, n_measurements);
    ulapack_init(&H, n_measurements, n_states);
    ulapack_init(&fx, n_states, 1);
    ulapack_init(&hx, n_measurements, 1);
}

MatrixError_t setup(Matrix_t y1){
    ulapack_edit_entry(&x, 0, 0, y1.entry[0][0]); // x0
    ulapack_edit_entry(&x, 1, 0, y1.entry[1][0]); // y0
    ulapack_edit_entry(&x, 2, 0, sqrt(
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) *
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) + 

                                ((y1.entry[1][0] - y.entry[1][0]) / dt) *
                                ((y1.entry[1][0] - y.entry[1][0]) / dt) 
    )); 

    ulapack_edit_entry(&x, 3, 0, M_PI); // theta0

    get_phi(&Phi, &x, dt);

    ulapack_set(&gamma, 0.0);
    ulapack_edit_entry(&gamma, 2, 0, 1.0);
    ulapack_edit_entry(&gamma, 3, 1, 1.0);
    
    ulapack_set(&Q, 0.0);
    ulapack_edit_entry(&Q, 0, 0, 5*5*dt);
    ulapack_edit_entry(&Q, 1, 1, .5*.5*dt);

    ulapack_edit_entry(&P, 0, 0, varx);
    ulapack_edit_entry(&P, 1, 1, vary);
    ulapack_edit_entry(&P, 2, 2, varv);
    ulapack_edit_entry(&P, 3, 3, vartheta);

    ulapack_edit_entry(&P, 0, 2, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 2, 0, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 1, 2, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 2, 1, 2*varx / (dt * dt));

    ulapack_edit_entry(&P, 0, 3, 2*varx);
    ulapack_edit_entry(&P, 3, 0, 2*varx);
    ulapack_edit_entry(&P, 1, 3, 2*varx);
    ulapack_edit_entry(&P, 3, 1, 2*varx);

    ulapack_set(&H, 0.0);
    ulapack_edit_entry(&H, 0, 0, 1.0);
    ulapack_edit_entry(&H, 1, 1, 1.0);

    ulapack_set(&R, 0.0);
    ulapack_edit_entry(&R, 0, 0, (stdx*stdx) / 3);
    ulapack_edit_entry(&R, 1, 1, (stdy*stdy) / 3);

    get_hx(&hx, &x);
    return ulapack_success;
}

MatrixError_t set_filter(){
    ukal_filter_create(&kFilter, ekf,            
                   n_states, n_measurements, 
                   &Phi, &gamma, &x, &Q,     
                   &P,                       
                   &H, &R);
    return ulapack_success;
}

MatrixError_t put_data (Matrix_t *y, float x_coord, float y_coord){
    ulapack_edit_entry(y, 0, 0, x_coord);
    ulapack_edit_entry(y, 1, 0, y_coord);
    return ulapack_success;
    
}

void visualize(Matrix_t x, Mat background){
    float x_coord = x.entry[0][0];
    float y_coord = x.entry[1][0];
    float v = x.entry[2][0];
    float theta = x.entry[3][0];
    cv::Point center(x_coord, y_coord);

}

void test_filter(){
    Mat backgrounhd = Mat::zeros(CAPTURE_WIDTH, CAPTURE_HEIGHT, CV_8UC3);
    Matrix_t y1;
    init_Matrices();
    put_data(&y1, 0, 0);
    put_data(&y, 1, 0);
    ulapack_init(&y1, n_measurements, 1);
    set_filter();

}