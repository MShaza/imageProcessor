#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <limits.h>
class ImageProcessor{
    public:
       static bool processGray(const std::string& inputPath, const std::string& outputPath);
       static bool processResize(const std::string& inputPath, const std::string& outputPath, const int &width, const int &height);
       static bool processBlur(const std::string& inputPath, const std::string& outputPath, const int &width, const int &height);
};