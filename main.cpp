#include <iostream>

#include <tinyxml2.h>
#include <fmt/core.h>
#include <fmt/color.h>

#include "StereoCalib.h"

using namespace tinyxml2;

void xmlThrowError_noElement(std::string element_desc);

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

    XMLElement* horizontal_corner = param->FirstChildElement("horizontal_corner");
    if (!horizontal_corner || (int) horizontal_corner->QueryIntText(&horizontal_corner_n)) {
        xmlThrowError_noElement("horizontal_corner");
    }

    XMLElement* vertical_corner = param->FirstChildElement("vertical_corner");
    if (!vertical_corner || (int) vertical_corner->QueryIntText(&vertical_corner_n)) {
        xmlThrowError_noElement("vertical_corner");
    }

    XMLElement* square_length= param->FirstChildElement("square_length");
    if (!square_length || (int) square_length->QueryIntText(&square_n)) {
        xmlThrowError_noElement("square_length");
    }

    std::string left_cam_path = left->GetText();
    std::string right_cam_path = right->GetText();

    std::cout << "Left Camera Path:\t" << left_cam_path << std::endl;
    std::cout << "Right Camera Path:\t" << right_cam_path << std::endl;

    StereoCalib s_calib(left_cam_path, right_cam_path, horizontal_corner_n, vertical_corner_n, square_n);
    
    // TODO implement stereo calibration below
    s_calib.start_stereo_calib();

    return 0;
}

void xmlThrowError_noElement(std::string element_desc) {
    throw std::runtime_error(
        fmt::format("[{}] there's no \"{}\" element.\n",
            fmt::format(fg(fmt::color::red), "CRITIC"),
            element_desc.c_str()
        )
    );
} 