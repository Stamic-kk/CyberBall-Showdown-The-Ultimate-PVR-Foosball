#ifndef LOC_H
#define LOC_H
#include <vector>
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#if CV_MAJOR_VERSION >= 3
#    include <opencv2/imgcodecs.hpp>
#else
#    include <opencv2/highgui/highgui.hpp>
#endif

#include <vpi/OpenCVInterop.hpp>

#include <vpi/Image.h>
#include <vpi/Status.h>
#include <vpi/Stream.h>
#include <vpi/algo/ConvertImageFormat.h>
#include <vpi/algo/MinMaxLoc.h>
#include <vpi/Array.h>

#include <cstring> // for memset
#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>
#include <experimental/filesystem>

#define TARGET_B 30
#define TARGET_G 255
#define TARGET_R 255
#define TARGET_GREY (TARGET_B + TARGET_G + TARGET_R) / 3    
typedef unsigned char Pixel;
typedef unsigned char Byte;

namespace fs = std::experimental::filesystem;
using std::string;
using std::vector;
using std::pair;

vector<string> getAllFiles(string path);
pair<int, int> getLocation (cv::Mat cvImage);
void test_locator(string path);
void draw_detected(cv::Mat &img, std::pair<int, int> location);
void setUpVpi();
void tearDownVpi();
#endif