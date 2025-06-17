#ifndef PSNRCALCULATOR_HPP
#define PSNRCALCULATOR_HPP

#include <opencv2/opencv.hpp>

class PSNRCalculator {
public:
    // Calcula o PSNR entre duas imagens
    static double compute(const cv::Mat& original, const cv::Mat& stego);
};

#endif // PSNRCALCULATOR_HPP
