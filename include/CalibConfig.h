#ifndef __CALIB_CONFIG_H__
#define __CALIB_CONFIG_H__

#include <tinyxml2.h>
#include <string>

class CalibConfig {
private:
    tinyxml2::XMLDocument xml_file;
    tinyxml2::XMLElement* rootxml;

    void xmlThrowRuntimeError_noElement(std::string element_desc);

public:
    CalibConfig() = delete;
    CalibConfig(std::string config_path);
    ~CalibConfig() = default;

    const std::string leftCameraFdPath();
    const std::string rightCameraFdPath();
    const int numHorizontalCorner();
    const int numVerticalCorner();
    const int chessboardSquareLength();

};

#endif