cmake_minimum_required(VERSION 3.10)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})
# Find Boost Libraries (Fix Policy Warning)
# Include directories

find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
cmake_policy(SET CMP0167 NEW)

add_executable(image_service
    main.cpp
    src/server.cpp
    src/session.cpp
    src/ImageProcessor
    src/generateId
)

target_link_libraries(image_service ${OpenCV_LIBS} ${Boost_INCLUDE_DIRS})
