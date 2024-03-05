#include "predictor.h"
#include "locator.h"
#include "camera.h"
#include "kalFilter.h"
#include "uart.h"
#include <unistd.h>
int main(){
    // camera_test();
    // return 0;
    // Uart uart;
    // usleep(1000);
    // char pack = packByte(0, 18);
    // char c = (char)84;
    // while(true){
    //     uart.send(&c ,1);
    //     usleep(1000);
    // }
    // uart.send(&pack ,1);
    // return 0;
    setUpVpi();
    cv::VideoCapture cap = get_camera();
    if(!cap.isOpened()) throw std::runtime_error("Failed to open camera");
    cv::Mat img;
    cv::Mat copy;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current;
    int runs = 0;
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
       // std::pair<int, int> loc = getLocation(copy);
        // std::cout<<"Time to get location: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - current).count()<<std::endl;
        
        //std::cout<<"Location: "<<loc.first<<", "<<loc.second<<std::endl;
        draw_detected(copy, getLocation(copy));
        cv::imshow("camera",copy);
        int keycode = cv::waitKey(2) & 0xff ; 
             if (keycode == 27) break;
            
         current = std::chrono::steady_clock::now();
         std::cout<<"Time esplised "<<std::chrono::duration_cast<std::chrono::milliseconds>(current-begin).count()<<std::endl;
        while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 1000){
            std::cout<<"FPS: "<<runs<<std::endl;   
            runs = 0;
            begin = std::chrono::steady_clock::now();
        }
        runs++;
    }
    tearDownVpi();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
