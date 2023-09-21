#! /usr/bin/env python3

import numpy as np
import cv2

if __name__ == '__main__':
    # cap = cv2.VideoCapture("/dev/video2")
    # cap2 = cv2.VideoCapture("/dev/video4")

    # cap.set(cv2.CAP_PROP_FPS, 30)
    # cap.set

    cap = cv2.VideoCapture("/dev/video2", cv2.CAP_V4L2)
    # cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080.0)

    print("video width: {}".format(cap.get(cv2.CAP_PROP_FRAME_WIDTH)))

    while True:
        retval, image = cap.read()
        # retval2, image2 = cap2.read()

        if not retval:
            break

        cv2.imshow("image", image)
        # cv2.imshow("image2", image2)

        key = cv2.waitKey(30)

        if key == 27:
            break
