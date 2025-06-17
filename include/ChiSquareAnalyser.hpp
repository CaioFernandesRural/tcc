#ifndef CHISQUAREANALYZER_HPP
#define CHISQUAREANALYZER_HPP

#include <opencv2/opencv.hpp>
#include <string>

class ChiSquareAnalyzer {
public:
    // Aplica a análise de chi-quadrado em cada canal de cor
    // Retorna o valor médio de chi² (quanto maior, maior a evidência de alteração)
    static double analyze(const cv::Mat& image);
};

#endif // CHISQUAREANALYZER_HPP
