
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
namespace fs = std::experimental::filesystem;
using std::string;
using std::vector;

vector<string> getAllFiles(string path){
    vector<string> files;
    for(const auto & entry : fs::directory_iterator(path)){
        files.push_back(entry.path());
    }
    for(string file : files){
        std::cout<<file<<std::endl;
    }
    return files;
}

int main(int argc, char**argv){
    std::cout<<CV_MAJOR_VERSION<<std::endl;
    vector<string> files_path = getAllFiles("inputs/");
    time_t start, end;
    for(string file_name : files_path){
    cv::Mat cvImage = cv::imread(file_name);
    VPIImage inputBGR = NULL;
    VPIImage input = NULL;
    cv::Mat outCvImage;
    VPIStream stream;
    vpiStreamCreate(0, &stream);
    start = clock();
    vpiImageCreateOpenCVMatWrapper(cvImage, 0, &inputBGR);
    vpiImageCreate(cvImage.cols, cvImage.rows, VPI_IMAGE_FORMAT_U8, 0, &input);
    vpiSubmitConvertImageFormat(stream, VPI_BACKEND_CUDA, inputBGR, input, NULL);

    VPIImageFormat format;
    vpiImageGetFormat(input, &format);
 
    int width, height;
    vpiImageGetSize(input, &width, &height);
    VPIPayload payload;
    vpiCreateMinMaxLoc(VPI_BACKEND_CPU, width, height, format, &payload);
    VPIArray minCoords;
    vpiArrayCreate(10000, VPI_ARRAY_TYPE_KEYPOINT, 0, &minCoords);
 
    VPIArray maxCoords;
    vpiArrayCreate(10000, VPI_ARRAY_TYPE_KEYPOINT, 0, &maxCoords);

    vpiSubmitMinMaxLoc(stream, VPI_BACKEND_CPU, payload, input, minCoords, maxCoords);
    vpiStreamSync(stream);
    VPIImageData inputImageData;
    vpiImageLock(input, VPI_LOCK_READ, &inputImageData);
    VPIArrayData minCoordsData, maxCoordsData;
    vpiArrayLock(minCoords, VPI_LOCK_READ, &minCoordsData);
    vpiArrayLock(maxCoords, VPI_LOCK_READ, &maxCoordsData);
    VPIKeypoint *min_coords = (VPIKeypoint *)minCoordsData.data;
    VPIKeypoint *max_coords = (VPIKeypoint *)maxCoordsData.data; 
 
    int min_i = min_coords[0].y;
    int min_j = min_coords[0].x;
 
    int max_i = max_coords[0].y;
    int max_j = max_coords[0].x;

    end = clock();
    std::cout << "Time taken by program is : " << double(end - start) / double(CLOCKS_PER_SEC) << " seconds" << std::endl;
    // Assuming that the input image is grayscale (only one plane).
        assert(inputImageData.numPlanes == 1);
 
    void *imgData     = inputImageData.planes[0].data;
    int imgPitchBytes = inputImageData.planes[0].pitchBytes;
 
    // Assuming that the plane has 8-bit unsigned int type.
    assert(inputImageData.planes[0].pixelType == VPI_PIXEL_TYPE_U8);
    typedef unsigned char Pixel;
    typedef unsigned char Byte;
 
    const Pixel *min_row = (const Pixel *)((const Byte *)imgData + min_i * imgPitchBytes);
    const Pixel *max_row = (const Pixel *)((const Byte *)imgData + max_i * imgPitchBytes);
 
    unsigned char min_value = min_row[min_j];
    unsigned char max_value = max_row[max_j];
    std::cout << "min: " << (int)min_value << " at (" << min_i << ", " << min_j << ")" << std::endl;
    vpiImageDataExportOpenCVMat(inputImageData, &outCvImage);
    cv::circle(outCvImage, cv::Point(min_j, min_i), 100, cv::Scalar(0, 0, 255), 20);
    cv::imwrite("outputs/"+file_name.substr(7), outCvImage);
    vpiArrayUnlock(maxCoords);
    vpiArrayUnlock(minCoords);
    vpiImageUnlock(input);
    vpiStreamDestroy(stream);
    vpiPayloadDestroy(payload);
    vpiImageDestroy(input);
    vpiArrayDestroy(minCoords);
    vpiArrayDestroy(maxCoords);
       
    } 


    return 0;

}