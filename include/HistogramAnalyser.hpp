#ifndef HISTOGRAMANALYZER_HPP
#define HISTOGRAMANALYZER_HPP

#include <opencv2/opencv.hpp>
#include <string>

class HistogramAnalyzer {
public:
    static double compare(const cv::Mat& original, const cv::Mat& stego);
    static void exportToCSV(const cv::Mat& original, const cv::Mat& stego, const std::string& filename);
};

#endif // HISTOGRAMANALYZER_HPP