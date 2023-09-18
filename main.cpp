#include <iostream>

#include <tinyxml2.h>
#include <fmt/core.h>
#include <fmt/color.h>

#include "StereoCalib.h"

using namespace tinyxml2;

int main(int argc, char* argv[]){

    // XMLDocument* camera_path_xml = new XMLDocument(); 
    XMLDocument camera_path_xml;
    XMLError xmlret = camera_path_xml.LoadFile(CONFIG_DIR_PATH "camera_path.xml");
    if ((int) xmlret) {
        throw std::runtime_error(
            fmt::format("[{}] xml parse failed error code: {}\n",
                fmt::format(fg(fmt::color::red), "CRITIC"),
                (int) xmlret
            )
        );
    }
    XMLElement* root = camera_path_xml.FirstChildElement("camera_path");
    if (!root) {
        throw std::runtime_error(
            fmt::format("[{}] there's no \"camera_path\" element.\n",
                fmt::format(fg(fmt::color::red), "CRITIC")
            )
        );
    }

    XMLElement* root_left = root->FirstChildElement("left");
    if (!root) {
        throw std::runtime_error(
            fmt::format("[{}] there's no \"left\" element.\n",
                fmt::format(fg(fmt::color::red), "CRITIC")
            )
        );
    }

    XMLElement* root_right = root->FirstChildElement("right");
    if (!root) {
        throw std::runtime_error(
            fmt::format("[{}] there's no \"right\" element.\n",
                fmt::format(fg(fmt::color::red), "CRITIC")
            )
        );
    }

    std::string left_cam_path = root_left->GetText();
    std::string right_cam_path = root_right->GetText();

    StereoCalib s_calib(left_cam_path, right_cam_path);
    
    // TODO implement stereo calibration below



    return 0;
}
