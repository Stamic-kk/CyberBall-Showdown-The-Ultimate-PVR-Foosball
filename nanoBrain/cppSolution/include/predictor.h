#ifndef PRED_H
#define PRED_H
#include <vector>
#include <math.h>
#include <array>
#include <iostream>

#define MAX_X_REAL   40 
#define MAX_Y_REAL   20
#define MAX_X 900
#define MAX_Y 900
#define SCALE MAX_X_REAL / MAX_X
#define LOCATION_MEM 10
#define BALL_RADIUS_REAL 1.5
// #define BALL_RADIUS BALL_RADIUS_REAL / SCALE
#define BALL_RADIUS 5
#define SAMPLE_RATE 0.020

static std::vector<int> xs;
static std::vector<int> ys;
bool updateLoc (int x, int y);
void clearLoc();
std::pair<float, float> linear_predict();
float calcCoef(int n = xs.size());
float calcIntercept(int n = xs.size());
float calcDistance(int x1, int y1, int x2, int y2);
std::pair<float, float> twoPointPredit(int x1, int y1, int x2, int y2);
std::pair<float, float> twoPointPredit(std::array<int, 2> p1, std::array<int, 2> p2);
std::array<std::pair<float, float>,2> predictRange ();

#endif