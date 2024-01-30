#include "../include/predictor.h"

using namespace std;

int sum_x = 0;
int sum_y = 0;
int sum_x2 = 0;
int sum_y2 = 0;
int sum_xy = 0;

bool updateLoc (int x, int y){
    if(x > MAX_X || y > MAX_Y){
        return false;
    }
    if(x < 0 || y < 0){
        return false;
    }
    if(xs.size() == LOCATION_MEM){
        sum_x -= xs[0];
        sum_y -= ys[0];
        sum_x2 -= xs[0] * xs[0];
        sum_y2 -= ys[0] * ys[0];
        sum_xy -= xs[0] * ys[0];
        xs.erase(xs.begin());
        ys.erase(ys.begin());
    }
    xs.push_back(x);
    ys.push_back(y);
    return true;
}

void clearLoc(){
    xs.clear();
    ys.clear();
    sum_x = 0;
    sum_y = 0;
    sum_x2 = 0;
    sum_y2 = 0;
    sum_xy = 0;
}


pair<float, float> linear_predict(){
    float coef = calcCoef();
    float intercept = calcIntercept();
    return make_pair(coef, intercept);
}

float calcCoef(int n){
    // if you arg n, modify sum_sth.
    float numerator = (n * sum_xy) - (sum_x * sum_y);
    float denominator = n * sum_x2 - sum_x * sum_x;
    return numerator / denominator;
}

float calcIntercept(int n){
    // if you arg n, modify sum_sth.
    float numerator = sum_y - calcCoef(n) * sum_x;
    return numerator / n;
}

pair<float, float> twoPointPredit(int x1, int y1, int x2, int y2){
    std::cout<<"x1: "<<x1<<", y1: "<<y1<<std::endl;
    std::cout<<"x2: "<<x2<<", y2: "<<y2<<std::endl;


    float coef = (y2 - y1) / (x2 - x1);
    float intercept = y1 - coef * x1;
    return make_pair(coef, intercept);
}
array<pair<float, float>,2> predictRange (){
    if(xs.size() < 2){
        return array<pair<float, float>,2>();
    }
    int x_last = xs.back();
    int y_last = ys.back();
    int x_pre_last = xs[xs.size() - 2];
    int y_pre_last = ys[ys.size() - 2];
    std::cout<<"x_pre_last: "<<x_pre_last<<", y_pre_last: "<<y_pre_last<<std::endl;
    std::cout<<"x_last: "<<x_last<<", y_last: "<<y_last<<std::endl;
    pair<float, float> bound1 = twoPointPredit(
        x_last + BALL_RADIUS, y_last - BALL_RADIUS, 
        x_pre_last - BALL_RADIUS, y_pre_last + BALL_RADIUS);
    pair<float, float> bound2 = twoPointPredit(
        x_last - BALL_RADIUS, y_last + BALL_RADIUS, 
        x_pre_last + BALL_RADIUS, y_pre_last - BALL_RADIUS);
    return array<pair<float, float>,2>{bound1, bound2};
}

bool touchTopBottom(int next_x, int next_y){
    pair<float, float> bounds = linear_predict();
    if(next_y < bounds.first * next_x + bounds.second){
        return true;
    }

}

