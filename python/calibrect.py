#! /usr/bin/env python3

import cv2 as cv
import json

class Configuration:

    def __init__(self, config_path: str) -> None:
        json_file = open(config_path, mode='r')
        content: dict = json.load(json_file)
        json_file.close()

        if 'camera_index' not in content:
            raise RuntimeError("camera_index is not a member of \"configuration.json\".")
        if 'chessboard' not in content:
            raise RuntimeError("chessboard is not a member of \"configuration.json\".")

        camera_index = content['camera_index']
        chessboard = content['chessboard']

        if 'left' not in camera_index:
            raise RuntimeError("left is not a member of \"camera_index\".")
        if 'right' not in camera_index:
            raise RuntimeError("right is not a member of \"camera_index\".")
        if 'num_hor_corner' not in chessboard:
            raise RuntimeError("num_hor_corner is not a member of \"chessboard\"")
        if 'num_ver_corner' not in chessboard:
            raise RuntimeError("num_ver_corner is not a member of \"chessboard\"")
        if 'square_length_mm' not in chessboard:
            raise RuntimeError("square_length_mm is not a member of \"chessboard\"")
        
        self.left_cam_path = camera_index['left']
        self.right_cam_path = camera_index['right']
        self.num_horizontal_corner = chessboard['num_hor_corner']
        self.num_vertical_corner = chessboard['num_ver_corner']
        self.chessboard_square_len = chessboard['square_length_mm']

class StereoCalibNRect:

    def __init__(self,
                 left_cam_path,
                 right_cam_path,
                 chessboard_horizontal_corner_num: int,
                 chessboard_vertical_corner_num: int,
                 chessboard_square_size: int) -> None:
        self.left_cam_path = left_cam_path
        self.right_cam_pth = right_cam_path
        self.hor_corner_n = chessboard_horizontal_corner_num
        self.ver_corner_n = chessboard_vertical_corner_num
        self.square_size = chessboard_square_size

    
    def startStereoCalib(self) -> None:
        
        stream_left = cv.VideoCapture(self.left_cam_path) 
        stream_right = cv.VideoCapture(self.right_cam_pth)

        # TODO make below section
       
        
        



if __name__ == '__main__':

    config = Configuration('../config/configuration.json')

    calib_n_rect = StereoCalibNRect(config.left_cam_path,
                                    config.right_cam_path,
                                    config.num_horizontal_corner,
                                    config.num_vertical_corner,
                                    config.chessboard_square_len)

    calib_n_rect.startStereoCalib()

