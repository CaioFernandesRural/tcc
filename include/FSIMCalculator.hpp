#ifndef FSIMCALCULATOR_HPP
#define FSIMCALCULATOR_HPP

#include <opencv2/opencv.hpp>

class FSIMCalculator {
public:
    static double compute(const cv::Mat& original, const cv::Mat& stego);
};

#endif // FSIMCALCULATOR_HPP