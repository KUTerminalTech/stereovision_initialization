cmake_minimum_required(VERSION 2.7.0...3.27.0)
project(DepthEstimation VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(yaml_cpp REQUIRED IMPORTED_TARGET yaml-cpp)
pkg_check_modules(tinyxml REQUIRED IMPORTED_TARGET tinyxml2)
pkg_check_modules(fmt REQUIRED IMPORTED_TARGET fmt)

include_directories(include)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp

    src/StereoCalib.cpp
    src/CalibConfig.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${OpenCV_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    ${OpenCV_LIBS}
    PkgConfig::yaml_cpp
    PkgConfig::tinyxml
    PkgConfig::fmt
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
    "CONFIG_DIR_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/config/\""
)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
