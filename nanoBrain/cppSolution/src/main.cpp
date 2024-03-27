#include "predictor.h"
#include "locator.h"
#include "camera.h"
#include "kalFilter.h"
#include "uart.h"
#include <unistd.h>
#define SHOW_IMAGE

int main(){
    // camera_test();
    // return 0;
    // Uart uart;
    // usleep(1000);
    // char pack = packByte(0, 18);
    // char c = (char)84;
    // std::cout<<(int)c<<std::endl;
    //  while(true){
    //     uart.send(&c,1);
    //     usleep(100000);
    //  }
    // return 0;


    // test_filter("../inputs/traj.txt");
    // return 0;
    setUpVpi();
    cv::VideoCapture cap = get_camera();
    if(!cap.isOpened()) throw std::runtime_error("Failed to open camera");
    cv::Mat img;
    cv::Mat copy;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current;
    int runs = 0;
    init_Matrices();
    if(get_image(cap, img) == false){
        std::cout<<"Capture read error"<<std::endl;
        return EXIT_FAILURE;
    }
    cv::absdiff(img, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), copy);
    pair<int, int> y1_loc = getLocation(copy);
    Matrix_t y1;
    put_data(&y1, y1_loc.first, y1_loc.second);
        if(get_image(cap, img) == false){
        std::cout<<"Capture read error"<<std::endl;
        return EXIT_FAILURE;
    }
    cv::absdiff(img, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), copy);
    pair<int, int> y_loc = getLocation(copy);

    std::cout<<"y1 "<<y1_loc.first<<", "<<y1_loc.second<<std::endl;
    std::cout<<"y "<<y_loc.first<<", "<<y_loc.second<<std::endl;
    put_data(&y, y_loc.first, y_loc.second);
    last_location.first = y_loc.first;
    last_location.second = y_loc.second;
    setup(y1);
    set_filter();

    get_fx(&fx, &kFilter.x, dt);
    ukal_set_fx(&kFilter, &fx);
    get_phi(&Phi, &kFilter.x, dt);
    ukal_set_phi(&kFilter, &Phi);
    ukal_model_predict(&kFilter);
    get_hx(&hx, &kFilter.x);
    print_mat(&kFilter.x);

    while (/* condition */true)
    {
        /* code */
        if(get_image(cap, img) == false){
            std::cout<<"Capture read error"<<std::endl;
           break;
        }else{
            
            //std::cout<<"Time to get image: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - current).count()<<std::endl;
    
        }// cv::copyTo(img, copy, cv::noArray());
        cv::absdiff(img, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), copy);
        current = std::chrono::steady_clock::now();
        curr_location =  getLocation(copy);
        //std::pair<int, int> loc = getLocation(copy);
        // std::cout<<"Time to get location: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - current).count()<<std::endl;
        int diff = get_different(curr_location, last_location);
        if(diff > 10)
            std::cout<<"Diff: "<<diff<<std::endl;
        //std::cout<<"Location: "<<loc.first<<", "<<loc.second<<std::endl;
        // draw_detected(copy, getLocation(copy));
        // cv::imshow("camera",copy);
        kalmanCapture(curr_location);
        visualize(kFilter.x, copy);
        add_lines(copy);
        if(cal_variance(curr_location)){
        //    break;
        }
        #ifdef SHOW_IMAGE
        draw_detected(copy, curr_location);
        cv::imshow("camera",copy);
        int keycode = cv::waitKey(1) & 0xff ; 
             if (keycode == 27) break;
        #endif



        current = std::chrono::steady_clock::now();
        //std::cout<<"Time esplised "<<std::chrono::duration_cast<std::chrono::milliseconds>(current-begin).count()<<std::endl;
        if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 1000){
            std::cout<<"FPS: "<<runs<<std::endl;   
            runs = 0;
            begin = std::chrono::steady_clock::now();
        }
        runs++;
        last_location.first = curr_location.first;
        last_location.second = curr_location.second;
    }
    tearDownVpi();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
