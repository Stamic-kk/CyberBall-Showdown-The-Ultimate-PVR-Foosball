#ifndef CAMERA_H_
#define CAMERA_H_
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#if CV_MAJOR_VERSION >= 3
#    include <opencv2/imgcodecs.hpp>
#else
#    include <opencv2/highgui/highgui.hpp>
#endif

#include <vpi/OpenCVInterop.hpp>
#include <opencv2/videoio.hpp>

#define CAPTURE_WIDTH 320   
#define CAPTURE_HEIGHT 240

int camera_test();

#endif