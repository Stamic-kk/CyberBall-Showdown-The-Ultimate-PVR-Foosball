
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

int main(int argc, char**argv){
    cv::Mat cvImage = cv::imread(argv[1]);
    VPIImage inputBGR = NULL;
    VPIImage input = NULL;

    VPIStream stream;
    vpiStreamCreate(0, &stream);

    vpiImageCreateWrapperOpenCVMat(cvImage, 0, &inputBGR);
    vpiImageCreate(cvImage.cols, cvImage.rows, VPI_IMAGE_FORMAT_U8, 0, &input);
    vpiSubmitConvertImageFormat(stream, VPI_BACKEND_CUDA, inputBGR, input, NULL);

    VPIImageFormat format;
    vpiImageGetFormat(input, &format);
 
    int width, height;
    vpiImageGetSize(input, &width, &height);
 
    VPIPayload payload;
    vpiCreateMinMaxLoc(VPI_BACKEND_CPU, width, height, format, &payload);
    VPIArray minCoords;
    vpiArrayCreate(10000, VPI_ARRAY_TYPE_KEYPOINT_F32, 0, &minCoords);
 
    VPIArray maxCoords;
    vpiArrayCreate(10000, VPI_ARRAY_TYPE_KEYPOINT_F32, 0, &maxCoords);

    vpiSubmitMinMaxLoc(stream, VPI_BACKEND_CPU, payload, input, minCoords, maxCoords);
    vpiStreamSync(stream);
    VPIImageData inputImageData;
    vpiImageLockData(input, VPI_LOCK_READ, VPI_IMAGE_BUFFER_HOST_PITCH_LINEAR, &inputImageData);
 
    VPIArrayData minCoordsData, maxCoordsData;
    vpiArrayLockData(minCoords, VPI_LOCK_READ, VPI_ARRAY_BUFFER_HOST_AOS, &minCoordsData);
    vpiArrayLockData(maxCoords, VPI_LOCK_READ, VPI_ARRAY_BUFFER_HOST_AOS, &maxCoordsData);
    VPIKeypointF32 *min_coords = (VPIKeypointF32 *)minCoordsData.buffer.aos.data;
    VPIKeypointF32 *max_coords = (VPIKeypointF32 *)maxCoordsData.buffer.aos.data;
 
    int min_i = min_coords[0].y;
    int min_j = min_coords[0].x;
 
    int max_i = max_coords[0].y;
    int max_j = max_coords[0].x;

    // Assuming that the input image is grayscale (only one plane).
    assert(inputImageData.buffer.pitch.numPlanes == 1);
 
    void *imgData     = inputImageData.buffer.pitch.planes[0].data;
    int imgPitchBytes = inputImageData.buffer.pitch.planes[0].pitchBytes;
 
    // Assuming that the plane has 8-bit unsigned int type.
    assert(inputImageData.buffer.pitch.planes[0].pixelType == VPI_PIXEL_TYPE_U8);
    typedef unsigned char Pixel;
    typedef unsigned char Byte;
 
    const Pixel *min_row = (const Pixel *)((const Byte *)imgData + min_i * imgPitchBytes);
    const Pixel *max_row = (const Pixel *)((const Byte *)imgData + max_i * imgPitchBytes);
 
    unsigned char min_value = min_row[min_j];
    unsigned char max_value = max_row[max_j];
    std::cout << "min: " << (int)min_value << " at (" << min_i << ", " << min_j << ")" << std::endl;
    vpiArrayUnlock(maxCoords);
    vpiArrayUnlock(minCoords);
    vpiImageUnlock(input);

    vpiStreamDestroy(stream);
    vpiPayloadDestroy(payload);
    vpiImageDestroy(input);
    vpiArrayDestroy(minCoords);
    vpiArrayDestroy(maxCoords);

    return 0;

}
