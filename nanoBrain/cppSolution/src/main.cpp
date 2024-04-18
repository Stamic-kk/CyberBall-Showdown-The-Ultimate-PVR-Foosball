#include "locator.h"
#include "camera.h"
#include "kalFilter.h"
#include "uart.h"
#include <unistd.h>

bool show_image = false;


int main(int argc, char const *argv[]){
    // camera_test();
    if(argc >= 2){
        if(strcmp(argv[1], "graph") == 0){
           show_image = true;
        }
    }
    setUpVpi();
    cv::VideoCapture cap = get_camera();
    if(!cap.isOpened()) throw std::runtime_error("Failed to open camera");
    cv::Mat img;
    cv::Mat copy;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current;
    int runs = 0;
    int diff;
    int *intercepts = new int[3];
    Uart uart;
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
        }
        cv::absdiff(img, cv::Scalar(TARGET_B, TARGET_G, TARGET_R), copy);
        current = std::chrono::steady_clock::now();
        curr_location =  getLocation(copy);
        // Get variance first before setup the filter
        if(cal_variance(curr_location)){
        //    break;
        }
        if(goal()){
            char data = (char) GOAL_MSG;
            uart.send(&data, 1);
        }
        else if(curr_location.first != -1){
            diff = get_different(curr_location, last_location);
            kalmanCapture(curr_location);
            if(!is_static()){
                get_intercepts(intercepts);
                for(int i = 0; i < 3; i++){
                    float intercept = intercepts[i];
                    if(abs(curr_location.second-lines[i]) <= 50 ){
                        char data = packByte(i, mapping(i, curr_location.second));
                        uart.send(&data, 1);   
                    }
                    else if(intercept != -1 && curr_location.first >= lines[i] ){ 
                        int scale = mapping(i, intercept);
                        char data = packByte(i, scale);
                        if(show_image){
                            draw_intercepts(copy, i, intercept);
                            // cv::line(copy, cv::Point(kFilter.x.entry[0][0], kFilter.x.entry[0][1]), cv::Point(lines[i], intercept), cv::Scalar(255, 255  , 255), 1, 4, 0);
                        }
                        uart.send(&data, 1);    
                    }
            
                }
            }
            else{
                for(int i = 0; i< 3; i++){
                    char data = packByte(i, mapping(i, curr_location.second));
                    uart.send(&data, 1);
                }
            }
        }
        if(show_image){
            visualize(kFilter.x, copy, is_static());
            add_lines(copy);
            if(curr_location.first != -1)
            draw_detected(copy, curr_location);
            cv::imshow("camera",copy);
            int keycode = cv::waitKey(1) & 0xff ; 
                if (keycode == 27) break;
        }

        current = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 1000){
            std::cout<<"FPS: "<<runs<<std::endl;   
            runs = 0;
            begin = std::chrono::steady_clock::now();
        }
        runs++;
        last_location.first = curr_location.first;
        last_location.second = curr_location.second;
        if(locations.size() > 20)
            locations.pop_back();
        locations.push_front(last_location);
        
    }
    delete[] intercepts;
    tearDownVpi();
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
