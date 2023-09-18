#ifndef CALIBRACAOCAMERA_H
#define CALIBRACAOCAMERA_H

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

#define NUM_HORIZON_CORNER  10
#define NUM_VERTICAL_CORNER 7
#define SQUIRE_LEN 25 // 25mm
#define FPS 30; // 30 fps

using namespace cv;
using namespace std;

class StereoCalib {

public:
    // StereoCalib(cv::Mat& actualOne, cv::Mat& actualTwo);
    StereoCalib(std::string left_cam_path, std::string right_cam_path);
    ~StereoCalib();
    
    void start_stereo_calib();
    
private:

    std::string left_cam_path;
    std::string right_cam_path;

    // * Chessboard setting
    // ! We will use video for calibration and rectification 

    //Chessboard Settings
    // int numBoards = 13; //Number of images for the calibration
    int hor_corner_n = NUM_HORIZON_CORNER;  //Horizontal corners
    int ver_corner_n = NUM_VERTICAL_CORNER; //Vertical corners
    // float squareSize = 2.5f; //Standard = 1. Small chessboard = 2,5. Large chessboard = 4,4
    float square_size = SQUIRE_LEN;

    vector<vector<Point3f>> object_points; //Represents the 3D corners actual location
    vector<vector<Point2f>> img_points_l, img_points_r; //Represent the location of corners detected in 3D
    vector<Point2f> corner_pts_l, conrner_pts_r;
    // vector<Point3f> obj;
    
    // Mat img1, img2, gray1, gray2;
    // Mat m_imageOne, m_imageTwo;
    
    // camera_mat's are 3x3 floating point arrays of each camera
    // dist_coeff's are distortion coefficients vectors of each camera
    // dist_coeff's Matrix of distortion coefficient of camera left and right
    Mat camera_mat_left, camera_mat_right;
    Mat dist_coeff_left, dist_coeff_right;
    
    // rotation_mat     - Rotation Matrix between the first and second camera coordinate systems
    // translation_mat  - Translation vector between the cameras coordinate systems
    // essential_mat    - Essential Matrix
    // fundamental_mat  - Fundamental matrix
    Mat rotation_mat;
    Mat translation_mat;
    Mat essential_mat;
    Mat fundamental_mat;
    
    // R1 - 3x3 Rectification Transformation (Rotation Matrix) for the first Camera
    // R2 - 3x3 Rectification Transformation (Rotation Matrix) for the second Camera
    // P1 - Projection matrix 3x4 in the new and rectified coordinate system of the first camera
    // P2 - Projection matrix 3x4 in the new and rectified coordinate system of the second camera
    // Q - Disparity matrix by depth 4x4
    Mat R1, R2, P1, P2, Q; 
    
};

#endif /* CALIBRACAOCAMERA_H */
