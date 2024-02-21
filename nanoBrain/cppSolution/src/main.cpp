#include "predictor.h"
#include "locator.h"
#include "camera.h"
#include "kalFilter.h"
#include <unistd.h>
int main(){
    // char * first_loc_image = argv[1];
    // char * second_loc_image = argv[2];
    // cv::Mat imTosolve = cv::imread(first_loc_image, cv::IMREAD_GRAYSCALE);
    // std::cout<<"Image size: "<<imTosolve.cols<<", "<<imTosolve.rows<<std::endl;
    // cv::Mat imTosolve2 = cv::imread(second_loc_image, cv::IMREAD_GRAYSCALE);
    // pair<int, int> location = getLocation(imTosolve);
    // pair<int, int> location2 = getLocation(imTosolve2);
    // std::cout<<"Location of first image: "<<location.first<<", "<<location.second<<std::endl;
    // std::cout<<"Location of second image: "<<location2.first<<", "<<location2.second<<std::endl;
    
    // clearLoc();
    // updateLoc(location.first, location.second);
    // updateLoc(location2.first, location2.second);
    // std::array<pair<float, float>,2> lines = predictRange();
    // std::pair<float, float> point1 = lines[0];
    // std::cout<<lines[0].first<<", "<<lines[0].second<<std::endl;
    // std::cout<<lines[1].first<<", "<<lines[1].second<<std::endl;
    // cv::Point pt1 = cv::Point(0, point1.second);
    // cv::Point pt2 = cv::Point(300, point1.first * 300 + point1.second);
    // cv::line(imTosolve, pt1, pt2, cv::Scalar(0, 0, 255), 20);
    // std::pair<float, float> point2 = lines[1];
    // cv::Point pt3 = cv::Point(0, point2.second);
    // cv::Point pt4 = cv::Point(300, point2.first * 300 + point2.second);
    // cv::line(imTosolve, pt3, pt4, cv::Scalar(0, 0, 255), 20);
    // cv::imwrite("../outputs/traj.jpg", imTosolve);
    // return 0;

    // camera_test();
    // std::string path = "../inputs/traj.txt";

    // test_filter(path);  
    // std::vector<std::pair<float, float>> data;
    // if(read_sim_data(path, data) == false){
    //     std::cout<<"Error reading file"<<std::endl;
    // }
    //verify
    // for_each(data.begin(), data.end(), [](std::pair<float, float> p){
    //     std::cout<<p.first<<", "<<p.second<<std::endl;
    // });
    // test_locator("../inputs");  
    // cv::namedWindow("Table Capture", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap = get_camera();
    if(!cap.isOpened()) throw std::runtime_error("Failed to open camera");
    cv::Mat img;
    cv::Mat copy;
    while (/* condition */true)
    {
        /* code */
        // if (!cap.read(img)) {
		// std::cout<<"Capture read error"<<std::endl;
		// break;
	    // }
        if(get_image(cap, img) == false){
            std::cout<<"Capture read error"<<std::endl;
            break;
        }
        // cv::copyTo(img, copy, cv::noArray());
        cv::absdiff(img, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), copy);
        draw_detected(copy, getLocation(copy));
        //cv::imshow("camera",copy);
        int keycode = cv::waitKey(10) & 0xff ; 
            if (keycode == 27) break;
    }
    
    // cv::Mat img;
    // while (true)
    // {
    //     /* code */
    //     // get_image(cap, img);
    //     if (!cap.read(img)) {
	// 	std::cout<<"Capture read error"<<std::endl;
	// 	break;
	//     }
    //     // std::pair<int, int> location = getLocation(img);
    //     // draw_detected(img, location);
    //     cv::imshow("Table Capture",img);
    //     // sleep(1);
    // }
    cap.release();
    cv::destroyAllWindows();


    //  sleep(10);
    // if(cap.read(img)){
       
    //     std::cout<<"Read image"<<std::endl;
    //     cv::namedWindow("TRY Camera", cv::WINDOW_AUTOSIZE);
    //     while(true)
    //         cv::imshow("USB Camera", img);
    // }
    // else{
    //     std::cout<<"Failed to read image"<<std::endl;
    // }
    // cap.release();
    // cv::destroyAllWindows();
    return 0;
}