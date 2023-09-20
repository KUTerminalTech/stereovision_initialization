#include <iostream>

#include "StereoCalib.h"
#include "CalibConfig.h"

int main(int argc, char* argv[]){

    CalibConfig config(CONFIG_DIR_PATH "configuration.json");

    std::string left_cam_path = config.leftCameraFdIdx();
    std::string right_cam_path = config.rightCameraFdIdx();
    int horizontal_corner_n = config.numHorizontalCorner();
    int vertical_corner_n = config.numVerticalCorner();
    int square_length = config.chessboardSquareLength();

    StereoCalib s_calib(left_cam_path, right_cam_path, horizontal_corner_n, vertical_corner_n, square_length);
    
    // TODO implement stereo calibration below
    s_calib.start_stereo_calib();

    return 0;
}