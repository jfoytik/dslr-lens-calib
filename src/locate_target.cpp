#include <iostream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string get_filename_from_input(int argc, char* argv[]);
Rect find_target(Mat &gray_img);
Rect calculate_roi(Point2f A, Point2f B, Point2f C);

int main(int argc, char* argv[])
{
    string filename = get_filename_from_input(argc, argv);

    // Load image and convert to grayscale.
    cerr << "Load Image - " << filename << endl;
    Mat img = imread(filename); 
    Mat img_gray;
    cvtColor(img, img_gray, CV_BGR2GRAY);

    Rect img_roi = find_target(img_gray);

    cout << img_roi.x << endl
        << img_roi.y << endl
        << img_roi.width << endl
        << img_roi.height << endl;

    return 0;
}

//! @brief Process cmd line input and extract file path.
//! @note Exits on incorrect usage.
string get_filename_from_input(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "Usage : ./locate_target <image path>" << endl
            << "    eg : ./locate_target /home/jfoytik/my_pic.jpg" << endl;
        exit(1);
    }

    return string(argv[1]);
}

//! @brief Find the QR target in the grayscale image. This assumes the image only contains 
//!        one QR code (3 high contrast corners).
//! @param gray_img grayscale image.
//! @returns Rect containing the target ROI.
Rect find_target(Mat &gray_img)
{
    Mat canny_edges(gray_img.size(), CV_MAKETYPE(gray_img.depth(), 1));
    Canny(gray_img, canny_edges, 100, 200, 3);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    cerr<<contours.size()<< " contours found" <<endl;

    int mark = 0;

    // Contour indices that corrospond to the detected markers.
    int A,B,C;

    // Get Moments for all Contours and the mass centers
    vector<Moments> mu(contours.size());
    vector<Point2f> mc(contours.size());

    for(int i = 0; i<contours.size(); i++)
    {
        mu[i] = moments(contours[i], false); 
        mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
    }

    for(int i = 0; i < contours.size(); i++)
    {
        int k=i;
        int c=0;

        while(hierarchy[k][2] != -1)
        {
            k = hierarchy[k][2];
            c = c + 1;
        }

        if(hierarchy[k][2] != -1)
            c = c+1;

        if (c >= 5)
        {    
            if(mark == 0)
                A = i;
            else if(mark == 1)
                B = i;        // i.e., A is already found, assign current contour to B
            else if(mark == 2)
                C = i;        // i.e., A and B are already found, assign current contour to C
            mark = mark + 1 ;
        }
    } 
    
    cerr << "Detected " << mark << " marker positions" <<endl;

    if(mark != 3)
    {
        cerr << "Incorrect number of markers detected." << endl;
        exit(1);
    }

    cerr << "Point A : " << mc[A].x << ", " << mc[A].y << endl;
    cerr << "Point B : " << mc[B].x << ", " << mc[B].y << endl;
    cerr << "Point C : " << mc[C].x << ", " << mc[C].y << endl;

    cerr << "Calculate ROI" << endl;
    return calculate_roi(mc[A], mc[B], mc[C]);

/*
    CvScalar scal;
    scal.val[0] = 128.0;

    circle(gray_img, mc[A], 2, scal);
    circle(gray_img, mc[B], 2, scal);
    circle(gray_img, mc[C], 2, scal);

    putText(gray_img, "A", mc[A], FONT_HERSHEY_SIMPLEX, 1.0, scal);
    putText(gray_img, "B", mc[B], FONT_HERSHEY_SIMPLEX, 1.0, scal);
    putText(gray_img, "C", mc[C], FONT_HERSHEY_SIMPLEX, 1.0, scal);
    
    rectangle(gray_img, roi_rect, scal);
*/

//    imwrite("debug.jpg", gray_img);

/*
    namedWindow("test", WINDOW_AUTOSIZE);
    imshow("test", gray_img);
    waitKey(0);
*/
    
}

//! @brief Calculates a rectangle ROI based on the three target corners.
Rect calculate_roi(Point2f A, Point2f B, Point2f C)
{
    Point2f top_left, top_right, bottom_left;

    float ab = sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
    float ac = sqrt(pow((A.x - C.x), 2) + pow((A.y - C.y), 2));
    float bc = sqrt(pow((B.x - C.x), 2) + pow((B.y - C.y), 2));

    if(ab > ac && ab > bc)
    {
        // C is the top-left marker.    
        top_left = C;
        if(B.y > A.y)
        {
            top_right = B;
            bottom_left = A;
        }
        else
        {
            top_right = A;
            bottom_left = B;
        }
    }
    else if(ac > ab && ac > bc)
    {
        // B is the top-left marker.
        top_left = B;
        if(C.y > A.y)
        {
            top_right = C;
            bottom_left = A;
        }
        else
        {
            top_right = A;
            bottom_left = C;
        }
    }
    else
    {
        // A is the top-left marker.
        top_left = A;
        if(C.y > B.y)
        {
            top_right = C;
            bottom_left = B;
        }
        else
        {
            top_right = B;
            bottom_left = C;
        }
    }

    cerr << "ROI : " 
        << top_left.x << ", " 
        << top_left.y << ", " 
        << top_right.y - top_left.y << ", "
        << bottom_left.x - top_left.x << endl;
    return Rect(top_left.x, top_left.y, top_right.y - top_left.y, bottom_left.x - top_left.x);
}

