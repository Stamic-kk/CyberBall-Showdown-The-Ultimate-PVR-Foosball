/*
 * predictor.c
 *
 *  Created on: Jan 14, 2024
 *      Author: Linus
 */

#include "const.h"
#include "stdbool.h"

// SET x to length 
// SET y to width

float last_xs [NUM_LAST_COORDINATE];
float last_ys [NUM_LAST_COORDINATE];
float time_interval [NUM_LAST_COORDINATE];
int pt = 0;

static float slope = 0;
static float intercept = 0;


void update_coord(float x, float y, float interval){
    // push in the new coordinate
    last_xs[pt] = x;
    last_ys[pt] = y;
    pt = (pt + 1) % NUM_LAST_COORDINATE;
}



void update_coord(float x, float y, float interval ){
    update_coord(x, y);
    time_interval[pt] = interval;
}


float* predict (int num_points){


}
 


