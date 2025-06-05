#include "../include/FSIMCalculator.hpp"
#include <cmath>
#include <stdexcept>

static double computeFSIMSingleChannel(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat grad1X, grad1Y, grad2X, grad2Y;
    cv::Sobel(img1, grad1X, CV_32F, 1, 0);
    cv::Sobel(img1, grad1Y, CV_32F, 0, 1);
    cv::Sobel(img2, grad2X, CV_32F, 1, 0);
    cv::Sobel(img2, grad2Y, CV_32F, 0, 1);

    cv::Mat mag1, mag2, phase1, phase2;
    cv::magnitude(grad1X, grad1Y, mag1);
    cv::magnitude(grad2X, grad2Y, mag2);
    cv::phase(grad1X, grad1Y, phase1);
    cv::phase(grad2X, grad2Y, phase2);

    double T1 = 0.01, T2 = 0.02, T3 = 0.03;
    double sumFSIM = 0.0, sumWeight = 0.0;

    for (int y = 0; y < mag1.rows; ++y) {
        for (int x = 0; x < mag1.cols; ++x) {
            float G1 = mag1.at<float>(y, x);
            float G2 = mag2.at<float>(y, x);
            float P1 = phase1.at<float>(y, x);
            float P2 = phase2.at<float>(y, x);

            double simGrad = (2 * G1 * G2 + T1) / (G1 * G1 + G2 * G2 + T1);
            double simPhase = (2 * std::cos(P1 - P2) + T2) / (2 + T2);

            double simCombined = simGrad * simPhase;
            double weight = std::max(G1, G2);

            sumFSIM += simCombined * weight;
            sumWeight += weight;
        }
    }

    return (sumWeight == 0.0) ? 1.0 : sumFSIM / sumWeight;
}

double FSIMCalculator::compute(const cv::Mat& original, const cv::Mat& stego) {
    if (original.empty() || stego.empty()) {
        throw std::runtime_error("Uma ou ambas as imagens estão vazias.");
    }

    if (original.size() != stego.size() || original.type() != stego.type()) {
        throw std::runtime_error("As imagens devem ter o mesmo tamanho e tipo.");
    }

    std::vector<cv::Mat> origChannels, stegoChannels;
    cv::split(original, origChannels);
    cv::split(stego, stegoChannels);

    double fsimTotal = 0.0;

    for (int i = 0; i < 3; ++i) {
        fsimTotal += computeFSIMSingleChannel(origChannels[i], stegoChannels[i]);
    }

    return fsimTotal / 3.0;
}
