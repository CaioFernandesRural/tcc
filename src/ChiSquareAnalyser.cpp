#include "../include/ChiSquareAnalyser.hpp"
#include <cmath>
#include <stdexcept>

static double chiSquareChannel(const cv::Mat& channel) {
    const int bins = 128; // Agrupa pares (0 e 1, 2 e 3, ..., 254 e 255)

    std::vector<int> freq(bins, 0);

    // Conta as ocorrências de cada intensidade
    for (int y = 0; y < channel.rows; ++y) {
        for (int x = 0; x < channel.cols; ++x) {
            int val = channel.at<uchar>(y, x);
            freq[val / 2]++;
        }
    }

    double chi2 = 0.0;

    for (int i = 0; i < bins; ++i) {
        int even = freq[i];       // freq de par (2*i ou 2*i+1)
        int odd = (i + 1 < bins) ? freq[i + 1] : 0;
        double expected = (even + odd) / 2.0;

        if (expected > 0) {
            chi2 += ((even - expected) * (even - expected)) / expected;
            chi2 += ((odd - expected) * (odd - expected)) / expected;
        }
    }

    return chi2;
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

    return totalChi2 / 3.0;
}
