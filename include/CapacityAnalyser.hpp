#ifndef CAPACITYANALISER_HPP
#define CAPACITYANALISER_HPP

#include <opencv2/opencv.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class CapacityAnalyser {

private:

    static bool checaSequencia(const Mat& sobelImage, Point& ptoInicio, int blockLen, Point& ptoTrabalho);

public:
    // Retorna diferença percentual entre grupos R e S (quanto maior, maior a suspeita)
    static int analyze(const Mat& image);
};

#endif // CAPACITYANALISER_HPP