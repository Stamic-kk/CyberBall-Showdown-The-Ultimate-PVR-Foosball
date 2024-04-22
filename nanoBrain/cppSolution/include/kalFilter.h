#ifndef KAL_FILTER_H_
#define KAL_FILTER_H_
extern "C"{
#include "ukal.h"
}
#include "camera.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
using namespace cv;
using std::vector;
using std::pair;
using std::string;
#define VISUAL_EXAG_FACTOR 20  
#define ACTIVATION_DISTANCE 160
#define MOVE_THRESHOLD 5

extern Filter_t kFilter;
extern int activation[3];
extern Matrix_t y; 
extern Matrix_t x;
extern Matrix_t Phi;
extern Matrix_t gam;
extern Matrix_t Q;
extern Matrix_t P;
extern Matrix_t R;
extern Matrix_t H;
extern Matrix_t fx;
extern Matrix_t hx;
extern const MatrixEntry_t dt;
extern bool attack;
extern const int lines[3];


MatrixError_t setup(Matrix_t y1);
 void get_phi(Matrix_t * const Phi, 
                    const Matrix_t * const x, 
                    const MatrixEntry_t dt);
 void get_fx(Matrix_t * const fx,
                   const Matrix_t * const x,
                   const MatrixEntry_t dt);
 void get_hx(Matrix_t * const hx, 
                   const Matrix_t * const x);
MatrixError_t init_Matrices();
MatrixError_t setup(Matrix_t y1);
MatrixError_t set_filter();
MatrixError_t put_data (Matrix_t *y, float x_coord, float y_coord);
void visualize(Mat &background, bool is_static = false);
void test_filter(std::string path);
bool read_sim_data(string path, vector<pair<float, float>> &data);
bool cal_variance(std::pair<int, int> loc);
void init_kalman(pair<int,int> firstLoc);
void update_filter(pair<int, int> loc);
void kalmanCapture(pair<float, float> loc);
void print_mat(Matrix_t *mat);
void add_lines(cv::Mat background);
void get_intercepts(int *intercepts);
int mapping(int rod_id, int pixel_pos);
void draw_intercepts(cv::Mat background, int rod_id, int y);
#endif
