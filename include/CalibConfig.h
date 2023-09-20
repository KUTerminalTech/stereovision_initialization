#ifndef __CALIB_CONFIG_H__
#define __CALIB_CONFIG_H__

#include <json/json.h>
#include <string>

class CalibConfig {
private:
    Json::Value config_root;
    Json::Value camera_index;
    Json::Value chessboard;

public:
    CalibConfig() = delete;
    CalibConfig(std::string config_path);
    ~CalibConfig() = default;

    const std::string leftCameraFdIdx() const;
    const std::string rightCameraFdIdx() const;
    const int numHorizontalCorner() const;
    const int numVerticalCorner() const;
    const int chessboardSquareLength() const;

};

#endif