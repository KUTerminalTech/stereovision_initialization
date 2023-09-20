#include "StereoCalib.h"

#include <opencv2/calib3d/calib3d_c.h>

#include <fmt/core.h>
#include <fmt/color.h>

#include <chrono>
#include <thread>

using namespace std::chrono;

// StereoCalib::StereoCalib(Mat& actualOne, Mat& actualTwo)
//     : m_imageOne(actualOne), m_imageTwo(actualTwo)
// {  }
StereoCalib::StereoCalib(
#if __linux__
    std::string left_cam_path,
    std::string right_cam_path,
#elif __APPLE__
    int left_cam_param,
    int right_cam_param,
#endif
    int chessboard_horizontal_corner_num,
    int chessboard_vertical_corner_num,
    int chessboard_square_size
) :
#if __linux__
left_cam_path(left_cam_path),
right_cam_path(right_cam_path),
#elif __APPLE__
left_cam_param(left_cam_param),
right_cam_param(right_cam_param),
#endif
hor_corner_n(chessboard_horizontal_corner_num),
ver_corner_n(chessboard_vertical_corner_num),
square_size(chessboard_square_size) 
{  }

StereoCalib::~StereoCalib() {
    camera_mat_left.release();
    camera_mat_right.release();
    dist_coeff_left.release();
    dist_coeff_right.release();
    rotation_mat.release();
    translation_mat.release();
    essential_mat.release();
    fundamental_mat.release();
}

void StereoCalib::start_stereo_calib() {

#if __linux__
    auto stream_left = VideoCapture(left_cam_path, CAP_V4L2);
    auto stream_right = VideoCapture(right_cam_path, CAP_V4L2);
#elif __APPLE__
    // TODO make macOS version
    auto stream_left = VideoCapture(left_cam_param, CAP_AVFOUNDATION);
    auto stream_right = VideoCapture(right_cam_param, CAP_AVFOUNDATION);
#elif __WINDOWS__
    // TODO make windows version
#endif

    // stream_left.set(CAP_PROP_FRAME_WIDTH, 660);
    // stream_left.set(CAP_PROP_FRAME_HEIGHT, 660);
    // stream_right.set(CAP_PROP_FRAME_WIDTH, 660);
    // stream_right.set(CAP_PROP_FRAME_HEIGHT, 660);
    // stream_left.set(CAP_PROP_FPS, (double) FPS);
    // stream_right.set(CAP_PROP_FPS, (double) FPS);

    if (!stream_left.isOpened() || !stream_right.isOpened()) {
        throw std::runtime_error(
            fmt::format("[{}] camera is not opened.\n",
                fmt::format(fg(fmt::color::red), "CRITIC")
            )
        );
    }

    double desired_sleep_time = 1.0 / (double) FPS;

    vector<Point3f> objp;
    for(size_t i = 0; i < ver_corner_n; i++) {
        for(size_t j = 0; j < hor_corner_n; j++) {
            objp.push_back(Point3f(j * square_size, i * square_size, 0.0f));
        }
    }

    Mat left_image, right_image;
    while (true) {
        // capture left and right image synchronously
        static bool first_calib = false;
        static int capture_cnt = 0;

        auto start_time = high_resolution_clock::now();

        int ret = stream_left.read(left_image); // capture image left
        ret = stream_right.read(right_image);   // capture iamge right

        if (!ret) {
            throw std::runtime_error(
                fmt::format("[{}] Cannot grab camera image from camera stream.\n",
                    fmt::format(fg(fmt::color::red), "CRITIC")
                )
            );
        }

        /* ---------- while running as 30 fps START ---------- */
        /**
         * @brief calculate calib in this section
         * 
         */

        Mat left_image_gray, right_image_gray; 
        if (first_calib) {
            undistort(left_image, left_image, camera_mat_left, dist_coeff_left);
            undistort(right_image, right_image, camera_mat_right, dist_coeff_right);
        }

#if (CV_VERSION_MAJOR >= 4)
        cvtColor(left_image, left_image_gray, COLOR_RGB2GRAY);
        cvtColor(right_image, right_image_gray, COLOR_RGB2GRAY);
#else
        cvtColor(left_image, left_image_gray, CV_RGB2GRAY);
        cvtColor(right_image, right_image_gray, CV_RGB2GRAY);
#endif 

        bool success_l = findChessboardCorners(
            left_image,
            Size(ver_corner_n, hor_corner_n),
            corner_pts_l,
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE
            // CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS 
        );
        
        bool success_r = findChessboardCorners(
            right_image,
            Size(ver_corner_n, hor_corner_n),
            corner_pts_r,
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE
            // CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS 
        );

        if (success_l && success_r) {
            TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);

            cornerSubPix(left_image_gray, corner_pts_l, Size(11, 11), Size(-1, -1), criteria);
            cornerSubPix(right_image_gray, corner_pts_r, Size(11, 11), Size(-1, -1), criteria);

            drawChessboardCorners(left_image_gray, Size(hor_corner_n, ver_corner_n), corner_pts_l, success_l);
            drawChessboardCorners(right_image_gray, Size(hor_corner_n, ver_corner_n), corner_pts_r, success_r);
            

            // if (capture_cnt > 30) { // 
            //     object_points.push_back(objp);
            //     img_points_l.push_back(corner_pts_l);
            //     img_points_r.push_back(corner_pts_r);

            //     capture_cnt = 0;

            //     stereoCalibrate(
            //         object_points,
            //         img_points_l, img_points_r,
            //         camera_mat_left, dist_coeff_left,
            //         camera_mat_right, dist_coeff_right,
            //         Size(hor_corner_n, ver_corner_n),
            //         rotation_mat,
            //         translation_mat,
            //         essential_mat,
            //         fundamental_mat,
            //         CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST,
            //         cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5)
            //     );

            //     first_calib = true;
            // }

        }
        imshow("image_left", left_image_gray);
        imshow("image_right", right_image_gray);

        /* ------------ while running as 30 fps END ----------- */

        auto end_time = high_resolution_clock::now();
        duration<double> elapse =  end_time - start_time;

        double sleep_time = desired_sleep_time - elapse.count();
        if (sleep_time > 0) {
            std::this_thread::sleep_for(microseconds(static_cast<int>(sleep_time * 1e6)));
        }

        if (waitKey(1) == 27) {
            break;
        }
    }

    // Size board_sz = Size(board_w, board_h);
    // int board_n = board_w*board_h;
    
    // for (int y = 0; y < board_h; y++) {
    //     for (int x = 0; x < board_w; x++) {
    //         obj.push_back(Point3f(y * squareSize, x * squareSize, 0));
    //     }
    // }
        
    // int success = 0;
    // int k = 0;
    // char key = 0;
    // bool found1 = false;
    // bool found2 = false;
    
    // //Saves images for calibration
    // while(success < numBoards){
    //     img1 = m_imageOne.clone();
    //     img2 = m_imageTwo.clone();
        
    //     cvtColor(img1, gray1, CV_BGR2GRAY);
    //     cvtColor(img2, gray2, CV_BGR2GRAY);
        
    //     found1 = findChessboardCorners(img1, board_sz, corners1, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    //     found2 = findChessboardCorners(img2, board_sz, corners2, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        
    //     if(found1){
    //         cornerSubPix(gray1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    //         drawChessboardCorners(gray1, board_sz, corners1, found1);
    //     }
        
    //     if(found2){
    //         cornerSubPix(gray2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    //         drawChessboardCorners(gray2, board_sz, corners2, found2);
    //     }
        
    //     imshow("image1", gray1);
    //     imshow("image2", gray2);
        
    //     k = waitKey(10);
    //     key = (char) waitKey(10);
        
    //     if(found1 && found2){
    //         k = waitKey(0);
    //         key = (char) waitKey(0);
    //     }
    //     if(key == 27){
    //         break;
    //     }
    //     cout << "images amount: ";
    //     cout << success << endl;
    //     if(key == ' ' && found1 != 0 && found2 != 0){ // if(k == ' ' && found1 != 0 && found2 != 0)
    //         imagePoints1.push_back(corners1);
    //         imagePoints2.push_back(corners2);
    //         object_points.push_back(obj);
    //         cout << "Stored corners " << endl;
    //         success++;
            
    //         if(success >= numBoards){
    //             break;
    //         }
    //     }
    // }
    
    // destroyAllWindows();
    
    // //Calibrates and saves calibration data
    // cout << "Starting Calibration..." << endl;
    
    // CM1 = Mat(3, 3, CV_64FC1);
    // CM2 = Mat(3, 3, CV_64FC1);
        
    // stereoCalibrate(object_points, imagePoints1, imagePoints2, CM1, D1, CM2, 
    //         D2, img1.size(), R, T, E, F,
    //         CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST,
    //         cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5));
    
    // FileStorage fs1("mystereocalib.yml", FileStorage::WRITE);
    // fs1 << "CM1" << CM1;
    // fs1 << "CM2" << CM2;
    // fs1 << "D1" << D1;
    // fs1 << "D2" << D2;
    // fs1 << "R" << R;
    // fs1 << "T" << T;
    // fs1 << "E" << E;
    // fs1 << "F" << F;
    
    // cout << "Calibration completed" << endl;
    
    // //Rectify and save the rectification data
    // cout << "Starting Rectification..." << endl;
    
    // stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);
    // fs1 << "R1" << R1;
    // fs1 << "R2" << R2;
    // fs1 << "P1" << P1;
    // fs1 << "P2" << P2;
    // fs1 << "Q" << Q;
    
    // cout << Q << endl;
    
    // fs1.release();
    // cout << "Rectification completed" << endl;
    
    // cout << "Applying Undistort..." << endl;
    
    // Mat map1x, map1y, map2x, map2y;
    // Mat imgU1, imgU2, imgRectify, img1Teste, img2Teste;
    
    // initUndistortRectifyMap(CM1, D1, R1, P1, img1.size(), CV_32FC1, map1x, map1y);
    // initUndistortRectifyMap(CM2, D2, R2, P2, img2.size(), CV_32FC1, map2x, map2y);
    
    // cout << "Undistort completed" << endl;
    
    // while(1){
    //     img1Teste = m_imageOne.clone();
    //     img2Teste = m_imageTwo.clone(); 
        
    //     cvtColor(img1Teste, img1Teste, cv::COLOR_BGR2RGB);
    //     cvtColor(img2Teste, img2Teste, cv::COLOR_BGR2RGB);
       
    //     remap(img1Teste, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    //     remap(img2Teste, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
        
    //     //imshow("image1", img1Teste);
    //     //imshow("image2", img2Teste);
        
    //     //To display the rectified images
    //     imgRectify = Mat::zeros(imgU1.rows, imgU1.cols*2+10, imgU1.type());

    //     imgU1.copyTo(imgRectify(Range::all(), Range(0, imgU2.cols)));
    //     imgU2.copyTo(imgRectify(Range::all(), Range(imgU2.cols+10, imgU2.cols*2+10)));

    //     //If it is too large to fit on the screen, scale down by 2, it should fit.
    //     if(imgRectify.cols > 1920){
    //         resize(imgRectify, imgRectify, Size(imgRectify.cols/2, imgRectify.rows/2));
    //     }
        
    //     //To draw the lines in the rectified image
    //     for(int j = 0; j < imgRectify.rows; j += 16){
    //         Point p1 = Point(0,j);
    //         Point p2 = Point(imgRectify.cols*2,j);
    //         line(imgRectify, p1, p2, CV_RGB(255,0,0));
    //     }

    //     imshow("Rectified", imgRectify);
        
    //     k = waitKey(5);
    //     key = (char) waitKey(5);
        
    //     if(key==27){
    //         break;
    //     }
    // }
}
