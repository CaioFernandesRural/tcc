#include "../include/SSIMCalculator.hpp"
#include <opencv2/quality.hpp>
#include <stdexcept>

double SSIMCalculator::compute(const cv::Mat& original, const cv::Mat& stego) {
    if (original.empty() || stego.empty()) {
        throw std::runtime_error("Uma ou ambas as imagens estão vazias.");
    }

    if (original.size() != stego.size() || original.type() != stego.type()) {
        throw std::runtime_error("As imagens devem ter o mesmo tamanho e tipo.");
    }

    // A função compute retorna um cv::Scalar (um valor por canal)
    cv::Scalar ssim = cv::quality::QualitySSIM::compute(original, stego, cv::noArray());
    
    // Para imagens em tons de cinza, basta retornar o primeiro valor.
    return ssim[0];
}
