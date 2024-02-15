#include "camera.h"
#include <opencv2/highgui.hpp>


const char* gst = "v4l2src device=/dev/video0 ! video/x-raw, format=YUY2, width=640, height=480, framerate=30/1 ! videoconvert ! video/x-raw, format=BGR ! appsink"
                        ;
// const char* gst_HW = "v4l2src device=/dev/video0 ! video/x-raw, format=YUY2, width=320, height=480, framerate=60/1 ! nvvidconv ! video/x-raw(memory:NVMM) ! nvvidconv ! video/x-raw, format=BGRx !  videoconvert ! video/x-raw, format=BGR ! appsink"
const std::string gst_HW = "v4l2src device=/dev/video0 ! video/x-raw, format=YUY2, width="+ std::to_string(CAPTURE_WIDTH) + ", height="+ std::to_string(CAPTURE_HEIGHT) + ", framerate=60/1 ! nvvidconv ! video/x-raw(memory:NVMM) ! nvvidconv ! video/x-raw, format=BGRx !  videoconvert ! video/x-raw, format=BGR ! appsink"
            ;

int camera_test(){
    std::cout << "Using pipeline: \n\t" << gst_HW << "\n";
 
    cv::VideoCapture cap(gst_HW, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
	std::cout<<"Failed to open camera."<<std::endl;
	return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;
    std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
    	if (!cap.read(img)) {
		std::cout<<"Capture read error"<<std::endl;
		break;
	    }
	
        cv::imshow("CSI Camera",img);
        int keycode = cv::waitKey(10) & 0xff ; 
            if (keycode == 27) break ;
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;
}

cv::VideoCapture get_camera(){
    cv::VideoCapture cap(gst_HW, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        throw std::runtime_error("Failed to open camera");
    }
    return cap;
}

bool get_image(cv::VideoCapture cap, cv::Mat &img){
    if (!cap.read(img)) {
        std::cout<<"Capture read error"<<std::endl;
        return false;
    }
    return true;
}