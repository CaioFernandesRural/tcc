#include "../include/PSNRCalculator.hpp"
#include <cmath>

double PSNRCalculator::compute(const cv::Mat& original, const cv::Mat& stego) {
    if (original.empty() || stego.empty()) {
        throw std::runtime_error("Uma ou ambas as imagens estão vazias.");
    }

    if (original.size() != stego.size() || original.type() != stego.type()) {
        throw std::runtime_error("As imagens devem ter o mesmo tamanho e tipo.");
    }

    cv::Mat diff;
    cv::absdiff(original, stego, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    double mse = cv::sum(diff)[0] / (double)(original.total() * original.channels());
    if (mse <= 1e-10) return INFINITY;

    double psnr = 10.0 * log10((255 * 255) / mse);
    return psnr;
}
