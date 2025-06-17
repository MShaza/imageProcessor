#include "imageProcessor.h"

bool ImageProcessor::processGray(const std::string &inputPath, const std::string& outputPath){
    std::cout<<"[ImageProcessor::process] -Enter function."<<std::endl;
            cv::Mat img = cv::imread(inputPath);
            if(img.empty()){
                std::cerr << "[ImageProcessor::process] Could not read input image: " << inputPath << std::endl;
                return false;
            }
            std::cout<<"[ImageProcessor::process] Process image"<<std::endl;
            cv::Mat gray;
            cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
            //int new_width = 256;
            //int new_height = 256;
            //Resize the image
            //cv::Mat resized_image;
            //cv::resize(gray, resized_image, cv::Size(new_width, new_height));
            std::cout<<"[mageProcessor::process] Write Image."<<std::endl;
            if (!cv::imwrite(outputPath, gray)) {
        std::cerr << "ImageProcessor::process] Could not write output image: " << outputPath << std::endl;
        return false;
    }

    std::cout << "ImageProcessor::process] Image processed and saved to " << outputPath << std::endl;
    return true;
    }

bool ImageProcessor::processResize(const std::string &inputPath, const std::string& outputPath, const int &width, const int &height){
        std::cout<<"[ImageProcessor::processResize] -Enter function."<<std::endl;
            cv::Mat img = cv::imread(inputPath);
            if(img.empty()){
                std::cerr << "[ImageProcessor::processResize] Could not read input image: " << inputPath << std::endl;
                return false;
            }
            std::cout<<"[ImageProcessor::processResize] Process image"<<std::endl;

            //Resize the image
            cv::Mat resized_image;
            cv::resize(img, resized_image, cv::Size(width, height));
            std::cout<<"[ImageProcessor::processResize] Write Image."<<std::endl;
            if (!cv::imwrite(outputPath, resized_image)) {
        std::cerr << "[ImageProcessor::processResize] Could not write output image: " << outputPath << std::endl;
        return false;
    }

    std::cout << "[ImageProcessor::processResize] Image processed and saved to " << outputPath << std::endl;
    return true;
    }

bool ImageProcessor::processBlur(const std::string &inputPath, const std::string& outputPath, const int &width, const int &height){
        std::cout<<"[ImageProcessor::processBlur] -Enter function."<<std::endl;
            cv::Mat img = cv::imread(inputPath);
            if(img.empty()){
                std::cerr << "[processBlur::processResize] Could not read input image: " << inputPath << std::endl;
                return false;
            }
            std::cout<<"[ImageProcessor::processBlur] Process image"<<std::endl;

            //Resize the image
            cv::Mat blur_image;
            cv::blur(img, blur_image, cv::Size(width, height));
            std::cout<<"[ImageProcessor::processBlur] Write Image."<<std::endl;
            if (!cv::imwrite(outputPath, blur_image)) {
        std::cerr << "[ImageProcessor::processBlur] Could not write output image: " << outputPath << std::endl;
        return false;
    }

    std::cout << "[ImageProcessor::processBlur] Image processed and saved to " << outputPath << std::endl;
    return true;
    }