#include "../include/RSAnalyser.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>

static int discriminationFunction(const std::vector<int>& group) {
    int sum = 0;
    for (size_t i = 1; i < group.size(); ++i) {
        sum += std::abs(group[i] - group[i - 1]);
    }
    return sum;
}

static void invertLSB(std::vector<int>& group) {
    for (int& val : group) {
        val ^= 1; // inverte o último bit
    }
}

static double analyzeChannel(const cv::Mat& channel) {
    int R = 0, S = 0;
    int groupSize = 8;

    for (int y = 0; y < channel.rows; ++y) {
        for (int x = 0; x <= channel.cols - groupSize; x += groupSize) {
            std::vector<int> group;

            for (int i = 0; i < groupSize; ++i) {
                group.push_back(channel.at<uchar>(y, x + i));
            }

            int fOriginal = discriminationFunction(group);

            std::vector<int> flippedGroup = group;
            invertLSB(flippedGroup);

            int fFlipped = discriminationFunction(flippedGroup);

            if (fFlipped > fOriginal) R++;
            else if (fFlipped < fOriginal) S++;
            // Se for igual, é grupo neutro, ignoramos
        }
    }

    if ((R + S) == 0) return 0.0; // evita divisão por zero
    return std::abs(R - S) / static_cast<double>(R + S);
}

double RSAnalyzer::analyze(const cv::Mat& image) {
    if (image.empty()) {
        throw std::runtime_error("Imagem vazia.");
    }

    if (image.channels() != 3) {
        throw std::runtime_error("Imagem precisa ter 3 canais.");
    }

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    double totalScore = 0.0;
    for (const auto& ch : channels) {
        totalScore += analyzeChannel(ch);
    }

    return totalScore / 3.0;
}
