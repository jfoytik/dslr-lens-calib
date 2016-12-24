#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

// TODO:
//  - Use QR code like processing to detect focus target.
//    - This will allow to automatically set the ROI to do sharpness check.
//    - This will allow a check for camera alignment with target.
//  - Make script to process images. 

using namespace std;
using namespace cv;

struct Input_Params
{
    string img_filename;
    string roi_filename;
};

Input_Params get_input_params(int argc, char* argv[]);
double calc_sharp_score_sobel(const Mat &img);
double calc_sharp_score_laplace(const Mat &img);

Input_Params get_input_params(int argc, char* argv[])
{
    if(argc != 2 && argc != 3)
    {
        cerr << "Usage : ./get_sharpness <image path>" << endl
            << "    eg : ./get_sharpness /home/jfoytik/my_pic.jpg" << endl;
        exit(1);
    }

    Input_Params params;
    params.img_filename = argv[1];
    params.roi_filename = argc == 3 ? argv[2] : "";

    return params;
}

double calc_sharp_score_sobel(const Mat &img)
{
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    // Gradient X
    Sobel(img, grad_x, CV_16S, 1, 0);
    convertScaleAbs(grad_x, abs_grad_x);

    // Gradient Y
    Sobel(img, grad_y, CV_16S, 0, 1);
    convertScaleAbs(grad_y, abs_grad_y);

    CvScalar grad_x_sum = sum(abs_grad_x);
    CvScalar grad_y_sum = sum(abs_grad_y);

    return static_cast<double>(grad_x_sum.val[0] + grad_y_sum.val[0]);
}

double calc_sharp_score_laplace(const Mat &img)
{
    Mat lap_out;

    Laplacian(img, lap_out, CV_16S, 3);
    
    Scalar mean, stddev;
    meanStdDev(lap_out, mean, stddev);

    return static_cast<double>(stddev[0]*stddev[0]);
}

int main(int argc, char* argv[])
{
    Input_Params params = get_input_params(argc, argv);

    cerr << "Load Image - " << params.img_filename << endl;
    Mat img = imread(params.img_filename); 

    Mat img_gray;
    cvtColor(img, img_gray, CV_BGR2GRAY);

    Mat img_roi;
    if(params.roi_filename != "")
    {
        ifstream roi_file(params.roi_filename.c_str());
        if(roi_file.is_open() == false)
        {
            cerr << "Failed to open ROI file : " << params.roi_filename << endl;
            exit(1);
        }

        int vals[4];
        int count = 0;
        string line;
        while(std::getline(roi_file, line) && count < 4)
        {
            vals[count] = std::stoi(line); 
            count++;
        }

        if(count != 4)
        {
            cerr << "Error parsing ROI file" << endl;
            exit(1);
        } 
        roi_file.close(); 

        img_roi = Mat(img_gray, Rect(vals[0], vals[1], vals[2], vals[3])); 
    }
    else
    {
        img_roi = Mat(img_gray);
    }
    
    imwrite("debug.jpg", img_roi);
    //namedWindow("Window", WINDOW_AUTOSIZE);
    //imshow("Window", img_roi);
    //waitKey(25);

    //double calc_score = calc_sharp_score_sobel(img_roi);
    double calc_score = calc_sharp_score_laplace(img_roi);
    cout<<calc_score<<endl;

    return 0;
}
