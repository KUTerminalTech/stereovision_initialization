#ifndef __STEREO_CALIB_H__
#define __STEREO_CALIB_H__

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

#define FPS 30; // 30 fps

using namespace cv;
using namespace std;

class StereoCalib {

public:
    // StereoCalib(cv::Mat& actualOne, cv::Mat& actualTwo);
    StereoCalib(
        std::string left_cam_path,
        std::string right_cam_path,
        int chessboard_horizontal_corner_num,
        int chessboard_vertical_corner_num,
        int chessboard_square_size
    );
    ~StereoCalib();
    
    void start_stereo_calib();
    
private:

    std::string left_cam_path;
    std::string right_cam_path;

    int hor_corner_n;  //Horizontal corners
    int ver_corner_n; //Vertical corners
    float square_size;


    // calibration parameter both intrinsic and extrinsic
    vector<vector<Point3f>> object_points; //Represents the 3D corners actual location
    vector<vector<Point2f>> img_points_l, img_points_r; //Represent the location of corners detected in 3D
    vector<Point2f> corner_pts_l, corner_pts_r;
    // vector<Point3f> obj;
    
    
    /**
     * @brief calibration parameter both intrinsic and extrinsic
     * 
     */
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
    
    /**
     * @brief rectification parameter
     * 
     */
    // R1 - 3x3 Rectification Transformation (Rotation Matrix) for the first Camera
    // R2 - 3x3 Rectification Transformation (Rotation Matrix) for the second Camera
    // P1 - Projection matrix 3x4 in the new and rectified coordinate system of the first camera
    // P2 - Projection matrix 3x4 in the new and rectified coordinate system of the second camera
    // Q - Disparity matrix by depth 4x4
    Mat R1, R2, P1, P2, Q; 
    
};

#endif /* __STEREO_CALIB_H */
