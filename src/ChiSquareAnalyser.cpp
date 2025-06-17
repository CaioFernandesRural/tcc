#include "../include/ChiSquareAnalyser.hpp"
#include <cmath>
#include <stdexcept>

static double chiSquareChannel(const cv::Mat& channel) {
    const int bins = 128; // Agrupa pares (0 e 1, 2 e 3, ..., 254 e 255)

    std::vector<int> freq(256, 0);  // frequências de todos os valores 0–255

    // Conta as ocorrências de cada intensidade
    for (int y = 0; y < channel.rows; ++y) {
        for (int x = 0; x < channel.cols; ++x) {
            int val = channel.at<uchar>(y, x);
            freq[val]++;
        }
    }

    double chi2 = 0.0;
    int validBins = 0;

    for (int i = 0; i < 256; i += 2) {
        int even = freq[i];
        int odd = (i + 1 < 256) ? freq[i + 1] : 0;
        double expected = (even + odd) / 2.0;

        if (expected > 0) {
            chi2 += ((even - expected) * (even - expected)) / expected;
            chi2 += ((odd - expected) * (odd - expected)) / expected;
            validBins++;
        }
    }

    // Normaliza pelo número de pares válidos
    return (validBins > 0) ? (chi2 / validBins) : 0.0;
}

double ChiSquareAnalyzer::analyze(const cv::Mat& image) {
    if (image.empty()) {
        throw std::runtime_error("Imagem vazia.");
    }

    if (image.channels() != 3) {
        throw std::runtime_error("Imagem precisa ter 3 canais.");
    }

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    double totalChi2 = 0.0;
    for (const auto& ch : channels) {
        totalChi2 += chiSquareChannel(ch);
    }

    return totalChi2 / 3.0; // média entre os canais B, G e R
}
