#include "../include/HistogramAnalyser.hpp"
#include <stdexcept>

double HistogramAnalyzer::compare(const cv::Mat& original, const cv::Mat& stego) {
    if (original.empty() || stego.empty()) {
        throw std::runtime_error("Uma ou ambas as imagens estão vazias.");
    }

    std::vector<cv::Mat> origChannels, stegoChannels;
    cv::split(original, origChannels);
    cv::split(stego, stegoChannels);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    double totalSimilarity = 0.0;

    for (int i = 0; i < 3; ++i) {
        cv::Mat histOrig, histStego;

        cv::calcHist(&origChannels[i], 1, 0, cv::Mat(), histOrig, 1, &histSize, &histRange);
        cv::calcHist(&stegoChannels[i], 1, 0, cv::Mat(), histStego, 1, &histSize, &histRange);

        cv::normalize(histOrig, histOrig, 0, 1, cv::NORM_MINMAX);
        cv::normalize(histStego, histStego, 0, 1, cv::NORM_MINMAX);

        double similarity = cv::compareHist(histOrig, histStego, cv::HISTCMP_CORREL);
        totalSimilarity += similarity;
    }

    return totalSimilarity / 3.0;
}

#include <fstream>

void HistogramAnalyzer::exportToCSV(const cv::Mat& original, const cv::Mat& stego, const std::string& filename) {
    std::vector<cv::Mat> origChannels, stegoChannels;
    cv::split(original, origChannels);
    cv::split(stego, stegoChannels);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    cv::Mat hOrig[3], hStego[3];

    for (int i = 0; i < 3; ++i) {
        cv::calcHist(&origChannels[i], 1, 0, cv::Mat(), hOrig[i], 1, &histSize, &histRange);
        cv::calcHist(&stegoChannels[i], 1, 0, cv::Mat(), hStego[i], 1, &histSize, &histRange);
        cv::normalize(hOrig[i], hOrig[i], 0, 1, cv::NORM_MINMAX);
        cv::normalize(hStego[i], hStego[i], 0, 1, cv::NORM_MINMAX);
    }

    std::ofstream file("resources/output_histograms/" + filename);
    if (!file.is_open()) throw std::runtime_error("Erro ao criar arquivo CSV de histograma.");

    file << "Bin,Original_B,Stego_B,Original_G,Stego_G,Original_R,Stego_R\n";

    for (int i = 0; i < histSize; ++i) {
        file << i;
        for (int c = 0; c < 3; ++c) {
            file << "," << hOrig[c].at<float>(i) << "," << hStego[c].at<float>(i);
        }
        file << "\n";
    }

    file.close();
}
