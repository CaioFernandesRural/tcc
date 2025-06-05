#ifndef SSIMCALCULATOR_HPP
#define SSIMCALCULATOR_HPP

#include <opencv2/opencv.hpp>

class SSIMCalculator {
public:
    // Calcula o SSIM entre duas imagens
    static double compute(const cv::Mat& original, const cv::Mat& stego);
};

#endif // SSIMCALCULATOR_HPP