#include <iostream>

#include <tinyxml2.h>
#include <fmt/core.h>
#include <fmt/color.h>

#include "StereoCalib.h"

using namespace tinyxml2;

template<typename ...T>
void xmlThrowError_noElement(std::string element_desc) {
    throw std::runtime_error(
        fmt::format("[{}] there's no \"{}\" element. error code: {}\n",
            fmt::format(fg(fmt::color::red), "CRITIC"),
            element_desc
        )
    );
} 

int main(int argc, char* argv[]){

    // XMLDocument* camera_param_xml = new XMLDocument(); 
    XMLDocument camera_param_xml;
    XMLError xmlret = camera_param_xml.LoadFile(CONFIG_DIR_PATH "cam_param.xml");
    if ((int) xmlret) {
        throw std::runtime_error(
            fmt::format("[{}] xml parse failed error code: {}\n",
                fmt::format(fg(fmt::color::red), "CRITIC"),
                (int) xmlret
            )
        );
    }
    XMLElement* param = camera_param_xml.FirstChildElement("param");
    if (!param) {
        xmlThrowError_noElement("param");
    }

    XMLElement* left = param->FirstChildElement("left");
    if (!left) {
        xmlThrowError_noElement("left");
    }

    XMLElement* right = param->FirstChildElement("right");
    if (!right) {
        xmlThrowError_noElement("right");
    }

    int horizontal_corner_n;
    int vertical_corner_n;
    int square_n;

    if ((int) param->QueryIntAttribute("horizontal_corner", &horizontal_corner_n)) {
        xmlThrowError_noElement("horizontal_corner");
    }

    if ((int) param->QueryIntAttribute("vertical_corner", &vertical_corner_n)) {
        xmlThrowError_noElement("vertical_corner");
    }

    if ((int) param->QueryIntAttribute("square_length", &square_n)) {
        xmlThrowError_noElement("square_length");
    }

    std::string left_cam_path = left->GetText();
    std::string right_cam_path = right->GetText();

    StereoCalib s_calib(left_cam_path, right_cam_path, horizontal_corner_n, vertical_corner_n, square_n);
    
    // TODO implement stereo calibration below



    return 0;
}
