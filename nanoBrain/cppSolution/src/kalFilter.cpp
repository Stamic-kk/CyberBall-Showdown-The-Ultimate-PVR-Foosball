#include "kalFilter.h"
#include <math.h>


static void get_phi(Matrix_t * const Phi, 
                    const Matrix_t * const x, 
                    const MatrixEntry_t dt) {
    // grab the velocity and angle states from the state vector
    MatrixEntry_t v = x->entry[2][0];
    MatrixEntry_t theta = x->entry[3][0];

    // since there are ones on the diagonal of Phi, just set it to
    // the identity initially.
    ulapack_eye(Phi);

    // fill in the values of Phi_k as calculated via the Jacobian
    ulapack_edit_entry(Phi, 0, 2, dt*cos(theta));
    ulapack_edit_entry(Phi, 1, 2, dt*sin(theta));

    ulapack_edit_entry(Phi, 0, 3, -1*dt*v*sin(theta));
    ulapack_edit_entry(Phi, 1, 3, dt*v*cos(theta));
}


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

static void get_hx(Matrix_t * const hx, 
                   const Matrix_t * const x) {
    ulapack_edit_entry(hx, 0, 0, x->entry[0][0]);
    ulapack_edit_entry(hx, 1, 0, x->entry[1][0]);
}


MatrixError_t init_Matrices(Matrix_t y1){
    ulapack_init(&y, n_measurements, 1);

    ulapack_init(&x, n_states, 1);
    ulapack_edit_entry(&x, 0, 0, y1.entry[0][0]); // x0
    ulapack_edit_entry(&x, 1, 0, y1.entry[1][0]); // y0
    ulapack_edit_entry(&x, 2, 0, sqrt(
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) *
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) + 

                                ((y1.entry[1][0] - y.entry[1][0]) / dt) *
                                ((y1.entry[1][0] - y.entry[1][0]) / dt) 
    )); 

    ulapack_edit_entry(&x, 3, 0, M_PI); // theta0
    ulapack_init(&Phi, n_states, n_states);
    get_phi(&Phi, &x, dt);
}