#ifndef RSANALYZER_HPP
#define RSANALYZER_HPP

#include <opencv2/opencv.hpp>

class RSAnalyzer {
public:
    // Retorna diferença percentual entre grupos R e S (quanto maior, maior a suspeita)
    static double analyze(const cv::Mat& image);
};

#endif // RSANALYZER_HPP