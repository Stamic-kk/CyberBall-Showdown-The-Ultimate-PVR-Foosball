/* Define scalling macros*/
#ifndef CONST_H 
#define CONST_H

// SET x to length 
// SET y to width

#define TABLE_WIDTH 20 * 1000
#define TABLE_LENGTH 37 * 1000

#define NUM_SENSOR_ROWs 7      // corresponding to width of table
#define NUM_SENSOR_COLS 12   // corresponding to length of table

#define ROW_SCALING_FACTOR TABLE_WIDTH / NUM_SENSOR_ROWs
#define COL_SCALING_FACTOR TABLE_LENGTH / NUM_SENSOR_COLS


/* Define ball macros*/

#define BALL_RADIUS -1      // needs to be replaced.


/* Define array macros*/
#define NUM_LAST_COORDINATE 4 
#endif