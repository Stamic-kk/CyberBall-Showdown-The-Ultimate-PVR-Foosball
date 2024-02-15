#include "../include/locator.h"

// #define SAVE

int tmep_index = 0;
std::string name;

vector<string> getAllFiles(string path){
    vector<string> files;
    for(const auto & entry : fs::directory_iterator(path)){
        if(entry.path().extension() == ".png" || entry.path().extension() == ".jpg")
            files.push_back(entry.path());
    }
    for(string file : files){
        std::cout<<file<<std::endl;
    }
    return files;
}

pair<int, int> getLocation (cv::Mat cvImage){
    time_t start, end;
    VPIImage inputBGR = NULL;
    VPIImage input = NULL;
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
    //std::cout<<"width: "<<width<<", height: "<<height<<std::endl;
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
    #ifdef SAVE
    cv:: Mat outCvImage;
    vpiImageDataExportOpenCVMat(inputImageData, &outCvImage);
    cv::circle(outCvImage, cv::Point(min_j, min_i), 2, cv::Scalar(0, 0, 255), 20);
    // bool succ = cv::imwrite("../outputs/"+ std::to_string(tmep_index++) + ".png", outCvImage);
    bool succ = cv::imwrite("../outputs/"+ name, outCvImage);
    // std::cout<<succ<<std::endl;
    #endif
    vpiArrayUnlock(maxCoords);
    vpiArrayUnlock(minCoords);
    vpiImageUnlock(input);
    vpiStreamDestroy(stream);
    vpiPayloadDestroy(payload);
    vpiImageDestroy(input);
    vpiArrayDestroy(minCoords);
    vpiArrayDestroy(maxCoords);
    return std::make_pair(min_i, min_j);
}

void test_locator(string path){
    vector<string> files = getAllFiles(path);
    for(string file : files){
        cv::Mat cvImage = cv::imread(file);
        name = file.substr(file.find_last_of("/\\") + 1);
        std::cout<<name<<std::endl;
        cv::absdiff(cvImage, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), cvImage);
        pair<int, int> location = getLocation(cvImage);
        std::cout<<"Location: "<<location.first<<", "<<location.second<<std::endl;
    }
}

void draw_detected(cv::Mat &img, std::pair<int, int> location){
    cv::circle(img, cv::Point(location.second, location.first), 2, cv::Scalar(0, 0, 255), 3);
}