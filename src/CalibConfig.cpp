#include "CalibConfig.h"

#include <fmt/core.h>
#include <fmt/color.h>

using namespace tinyxml2;

void CalibConfig::xmlThrowRuntimeError_noElement(std::string element_desc) {
    throw std::runtime_error(
        fmt::format("[{}] there's no \"{}\" element.\n",
            fmt::format(fg(fmt::color::red), "CRITIC"),
            element_desc.c_str()
        )
    );
}

CalibConfig::CalibConfig(std::string config_path) {
    XMLError ret = xml_file.LoadFile(config_path.c_str());

    if ((int) ret) {
        throw std::runtime_error(
            fmt::format("[{}] xml parse failed error code: {}\n",
                fmt::format(fg(fmt::color::red), "CRITIC"),
                (int) ret
            )
        );
    }

    rootxml = xml_file.FirstChildElement("param");
    if (!rootxml) {
        xmlThrowRuntimeError_noElement("param");
    }
}

const std::string CalibConfig::leftCameraFdPath() {
    XMLElement* content = rootxml->FirstChildElement("left");
    if(!content) {
        xmlThrowRuntimeError_noElement("left");
    }

    return content->GetText();
}

const std::string CalibConfig::rightCameraFdPath() {
    XMLElement* content = rootxml->FirstChildElement("right");
    if (!content) {
        xmlThrowRuntimeError_noElement("right");
    }

    return content->GetText();
}

const int CalibConfig::numHorizontalCorner() {
    int ret;
    XMLElement* content = rootxml->FirstChildElement("horizontal_corner");
    if (!content || (int) content->QueryIntText(&ret)) {
        xmlThrowRuntimeError_noElement("horizontal_corner");
    }

    return ret;
}

const int CalibConfig::numVerticalCorner() {
    int ret;
    XMLElement* content = rootxml->FirstChildElement("vertical_corner");
    if (!content || (int) content->QueryIntText(&ret)) {
        xmlThrowRuntimeError_noElement("vertical_corner");
    }

    return ret;
}

const int CalibConfig::chessboardSquareLength() {
    int ret;
    XMLElement* content = rootxml->FirstChildElement("square_length");
    if (!content || (int) content->QueryIntText(&ret)) {
        xmlThrowRuntimeError_noElement("square_length");
    }

    return ret;
}