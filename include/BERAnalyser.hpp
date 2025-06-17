#ifndef BERANALYSER_H
#define BERANALYSER_H

#include <opencv2/opencv.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <bitset>
#include <algorithm> // para std::min

using namespace cv;
using namespace std;


class BERAnalyser
{
public:
    static double analyse(const string& original, const string& recovered);
};

#endif // BERANALYSER_H