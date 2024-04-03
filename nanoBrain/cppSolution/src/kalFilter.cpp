#include "kalFilter.h"


Filter_t kFilter;

const Index_t n_states = 4;
const Index_t n_measurements = 2;
const MatrixEntry_t dt = SAMPLE_RATE;
// const MatrixEntry_t stdx = 1.87285;      //change this
// const MatrixEntry_t stdy = 2.01378;        //change this

const MatrixEntry_t stdx = 2.3;      //change this
const MatrixEntry_t stdy = 2.3;        //change this
const MatrixEntry_t varx =  (stdx*stdx);
const MatrixEntry_t vary = (stdy * stdy);
const MatrixEntry_t varv = 2*sqrt(2)*(varx / (dt * dt));
// const MatrixEntry_t vartheta = M_PI;
const MatrixEntry_t vartheta = sqrt(11);




vector<int> x_arr;
vector<int> y_arr;
vector<Matrix_t> stateArr; 

Matrix_t y; 
Matrix_t x;
Matrix_t Phi;
Matrix_t gam;
Matrix_t Q;
Matrix_t P;
Matrix_t R;
Matrix_t H;
Matrix_t fx;
Matrix_t hx;

int line_A = 70;
int line_B = 190;
int line_C = CAPTURE_WIDTH - 5;


int counter = 0;



void init_kalman(pair<int, int> firstLoc){
    Matrix_t y1;
    init_Matrices();
    ulapack_init(&y1, n_measurements, 1);
    put_data(&y1, firstLoc.first, firstLoc.second);
    put_data(&y, firstLoc.first, firstLoc.second);
    setup(y1);  
    set_filter();
}
 void get_phi(Matrix_t * const Phi, 
                    const Matrix_t * const x, 
                    const MatrixEntry_t dt) {
    MatrixEntry_t v = x->entry[2][0];
    MatrixEntry_t theta = x->entry[3][0];

    ulapack_eye(Phi);
    ulapack_edit_entry(Phi, 0, 2, dt*cos(theta));
    ulapack_edit_entry(Phi, 1, 2, dt*sin(theta));

    ulapack_edit_entry(Phi, 0, 3, -1*dt*v*sin(theta));
    ulapack_edit_entry(Phi, 1, 3, dt*v*cos(theta));
}

// propagation
 void get_fx(Matrix_t * const fx,
                   const Matrix_t * const x,
                   const MatrixEntry_t dt) {
    MatrixEntry_t v = x->entry[2][0];
    MatrixEntry_t theta = x->entry[3][0];

    ulapack_edit_entry(fx, 0, 0, x->entry[0][0] + dt * v * cos(theta));
    ulapack_edit_entry(fx, 1, 0, x->entry[1][0] + dt * v * sin(theta));
    ulapack_edit_entry(fx, 2, 0, x->entry[2][0]);
    ulapack_edit_entry(fx, 3, 0, x->entry[3][0]);
}
// Observor
 void get_hx(Matrix_t * const hx, 
                   const Matrix_t * const x) {
    ulapack_edit_entry(hx, 0, 0, x->entry[0][0]);
    ulapack_edit_entry(hx, 1, 0, x->entry[1][0]);
}

MatrixError_t init_Matrices(){
    ulapack_init(&y, n_measurements, 1);
    ulapack_init(&x, n_states, 1);
    ulapack_init(&Phi, n_states, n_states);
    ulapack_init(&gam, n_states, n_measurements);
    ulapack_init(&Q, gam.n_cols, gam.n_cols);
    ulapack_init(&P, n_states, n_states);
    ulapack_init(&R, n_measurements, n_measurements);
    ulapack_init(&H, n_measurements, n_states);
    ulapack_init(&fx, n_states, 1);
    ulapack_init(&hx, n_measurements, 1);
}

MatrixError_t setup(Matrix_t y1){
    ulapack_edit_entry(&x, 0, 0, y1.entry[0][0]); // x0
    ulapack_edit_entry(&x, 1, 0, y1.entry[1][0]); // y0
    ulapack_edit_entry(&x, 2, 0, sqrt(
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) *
                                ((y1.entry[0][0] - y.entry[0][0]) / dt) + 

                                ((y1.entry[1][0] - y.entry[1][0]) / dt) *
                                ((y1.entry[1][0] - y.entry[1][0]) / dt) 
    )); 

    ulapack_edit_entry(&x, 3, 0, M_PI); // theta0

    get_phi(&Phi, &x, dt);

    ulapack_set(&gam, 0.0);
    ulapack_edit_entry(&gam, 2, 0, 1.0);
    ulapack_edit_entry(&gam, 3, 1, 1.0);
    
    ulapack_set(&Q, 0.0);
    ulapack_edit_entry(&Q, 0, 0, 5*5*dt);
    ulapack_edit_entry(&Q, 1, 1, .5*.5*dt);

    ulapack_edit_entry(&P, 0, 0, varx);
    ulapack_edit_entry(&P, 1, 1, vary);
    ulapack_edit_entry(&P, 2, 2, varv);
    ulapack_edit_entry(&P, 3, 3, vartheta);

    ulapack_edit_entry(&P, 0, 2, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 2, 0, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 1, 2, 2*varx / (dt * dt));
    ulapack_edit_entry(&P, 2, 1, 2*varx / (dt * dt));

    ulapack_edit_entry(&P, 0, 3, 2*varx);
    ulapack_edit_entry(&P, 3, 0, 2*varx);
    ulapack_edit_entry(&P, 1, 3, 2*varx);
    ulapack_edit_entry(&P, 3, 1, 2*varx);

    ulapack_set(&H, 0.0);
    ulapack_edit_entry(&H, 0, 0, 1.0);
    ulapack_edit_entry(&H, 1, 1, 1.0);

    ulapack_set(&R, 0.0);
    ulapack_edit_entry(&R, 0, 0, (stdx*stdx) / 3);
    ulapack_edit_entry(&R, 1, 1, (stdy*stdy) / 3);

    get_hx(&hx, &x);
    return ulapack_success;
}

MatrixError_t set_filter(){
    std::cout<<"init Kalman filter"<<std::endl;
    std::cout<<"Phi"<<std::endl;
    print_mat(&Phi);
    std::cout<<"gam"<<std::endl;
    print_mat(&gam);
    ukal_filter_create(&kFilter, ekf,            
                   n_states, n_measurements, 
                   &Phi, &gam, &x, &Q,     
                   &P,                       
                   &H, &R);
    return ulapack_success;
}

MatrixError_t put_data (Matrix_t *y, float x_coord, float y_coord){
    ulapack_edit_entry(y, 0, 0, x_coord);
    ulapack_edit_entry(y, 1, 0, y_coord);
    return ulapack_success;
    
}

void visualize(Matrix_t x, Mat &background, bool is_static=false){
    float x_coord = x.entry[0][0];
    float y_coord = x.entry[1][0];
    float v = x.entry[2][0];
    float theta = x.entry[3][0];
    float x_end = x_coord + v * cos(theta) * VISUAL_EXAG_FACTOR;
    float y_end = y_coord + v * sin(theta) * VISUAL_EXAG_FACTOR;
    cv::Point center = cv::Point(y_coord, x_coord);
    cv::Point to_dot = cv::Point(y_end, x_end);
    if(!is_static)
        cv::line(background, center, to_dot, cv::Scalar(0, 255  , 255), 1, 4, 0);
    cv::circle(background, center, 2, cv::Scalar(255, 0, 255), 2, 8, 0);

}

void test_filter(std::string path=""){
    Mat backgrounhd = Mat::zeros(CAPTURE_HEIGHT, CAPTURE_WIDTH * 2, CV_8UC3);
    Matrix_t y1;

    const MatrixEntry_t sample_data[10][2]={ {2.1,2.204},
                                                {2.91,2.84},
                                                {3.1,3.204},
                                                {3.91,3.84},
                                                {4.1,4.204},
                                                {4.91,4.84},
                                                {5.1,5.204},
                                                {5.91,5.84},
                                                {6.1,6.204},
                                                {6.91,6.84},
                                                };

    vector<pair<float, float>> data;
    if(path.length() > 0){
        if(read_sim_data(path, data) == false){
            std::cout<<"Error reading file"<<std::endl;
        }
    }
    else{
        for(int i = 0;i < 10;i++){
            data.push_back(std::make_pair(sample_data[i][0], sample_data[i][1]));
        }
    }
    int n = data.size();
    for_each(data.begin(), data.end(), [](std::pair<float, float> p){
        std::cout<<p.first<<", "<<p.second<<std::endl;
    });
    std::cout<<"Data size: "<<n<<std::endl; 
    

    // init_Matrices();
    // ulapack_init(&y1, n_measurements, 1);
    // put_data(&y1, data[0].first, data[0].second);
    put_data(&y, data[1].first, data[1].second);
    init_kalman(data[0]);
    // setup(y1);  
    // set_filter();

    for(int i = 1;i < data.size();i++){
        // get_fx(&fx, &kFilter.x, dt);
        // ukal_set_fx(&kFilter, &fx);
        // get_phi(&Phi, &kFilter.x, dt);
        // ukal_set_phi(&kFilter, &Phi);
        // ukal_model_predict(&kFilter);
        // put_data(&y, data[i].first, data[i].second);
        // get_hx(&hx, &kFilter.x);
        // ukal_set_hx(&kFilter, &hx);
        // ukal_update(&kFilter, &y);
        kalmanCapture(data[i]);
        visualize(kFilter.x, backgrounhd);
    }
    cv::imshow("Trajectory", backgrounhd);
    while(true){
        if(cv::waitKey(10) == 27) break;
    }
    cv::destroyAllWindows();
}

void kalmanCapture(pair<float, float> loc){
        get_fx(&fx, &kFilter.x, dt);
        ukal_set_fx(&kFilter, &fx);
        get_phi(&Phi, &kFilter.x, dt);
        ukal_set_phi(&kFilter, &Phi);
        ukal_model_predict(&kFilter);
        put_data(&y, loc.first, loc.second);
        get_hx(&hx, &kFilter.x);
        ukal_set_hx(&kFilter, &hx);
        ukal_update(&kFilter, &y);
}

void get_intercepts(int *intercepts){
    //current position
    Matrix_t x = kFilter.x;
    float x_coord = x.entry[0][0];
    float y_coord = x.entry[1][0];
    //velocity
    float v = x.entry[2][0];
    float theta = x.entry[3][0];
    float slope = tan(theta / M_PI * 180);
    int arr[] = {line_A, line_B, line_C};
    for(int i = 0 ;i < 3;i++){
        float defense_x = arr[i];
        float dx = defense_x - x_coord;
    
        // intercepts[i] = max(min((int)(y_coord - slope * dx), CAPTURE_HEIGHT), 0);
        float y_hat = (y_coord - slope * dx);
        intercepts[i] = y_hat <= 0 || y_hat>= CAPTURE_HEIGHT ? -1 : y_hat;
    }

}




bool read_sim_data(string path, vector<pair<float, float>> &data){
    // std::ifstream file(path);
    // std::ifstream file = std::ifstream(path);
    // if(!file.is_open()) return false;

    // file.close();
    // return true;
    
    FILE *file = fopen(path.c_str(), "r");
    if(file == NULL) return false;
    data.clear();
    float x, y;
    while(fscanf(file, " [%f %f]\n", &x, &y) != EOF){
        data.push_back(std::make_pair(x, y));
    }
    fclose(file);
    return true;
}


bool cal_variance(std::pair<int, int> loc){
    if(x_arr.size() >= 10000 ) return false;


    x_arr.push_back(loc.first);
    y_arr.push_back(loc.second);
    if(x_arr.size() < 10000) return false;
    int x_sum = std::accumulate(x_arr.begin(), x_arr.end(), 0);
    int y_sum = std::accumulate(y_arr.begin(), y_arr.end(), 0);
    int x_mean = x_sum / x_arr.size();
    int y_mean = y_sum / y_arr.size();
    float var_x = 0;
    float var_y = 0;
    for(int x: x_arr){
        var_x += (x - x_mean) * (x - x_mean);
    }
    for(int y: y_arr){
        var_y += (y - y_mean) * (y - y_mean);
    }
    var_x /= x_arr.size() - 1;
    var_y /= y_arr.size() - 1;
    std::cout<<"Variance x: "<<var_x<<", Variance y: "<<var_y<<std::endl;
    std::cout<<"Mean x: "<<x_mean<<", Mean y: "<<y_mean<<std::endl;
    std::cout<<"std x"<<sqrt(var_x)<<", std y: "<<sqrt(var_y)<<std::endl;

    return true;

}

void print_mat(Matrix_t *mat){
    for(int i = 0;i < mat->n_rows;i++){
        for(int j = 0;j < mat->n_cols;j++){
            std::cout<<mat->entry[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}


void add_lines(cv::Mat background){

    cv::line(background, cv::Point(line_A, 0), cv::Point(line_A, CAPTURE_HEIGHT), cv::Scalar(255, 255, 255), 1, 4, 0);
    cv::line(background, cv::Point(line_B, 0), cv::Point(line_B, CAPTURE_HEIGHT), cv::Scalar(255, 255, 255), 1, 4, 0);    
    cv::line(background, cv::Point(line_C, 0), cv::Point(line_C, CAPTURE_HEIGHT), cv::Scalar(255, 255, 255), 1, 4, 0);    
}
