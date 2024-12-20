#ifndef DECODE_H
#define DECODE_H

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept> // Para runtime_error
#include <nlohmann/json.hpp>

using namespace cv;
using namespace std;

using json = nlohmann::json;

class Decode
{
private:
    int extractLSB(const Vec3b pixel);
    char decodeBloco(const Mat image, int &N);
    
public:
    Decode() {};
    ~Decode() {};
    
    string decodeImagem(const Mat image, int inicialN);
};

#endif // DECODE_H