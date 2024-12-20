#ifndef ENCODE_H
#define ENCODE_H

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

class Encode
{
private:
    bool checaSequencia(const Mat sobelImage, Point &ptoInicio, int blockLen, Point &ptoTrabalho);
    void setLSB(Mat &image, Point local, int bitValue);
    void insertBlock(Mat &image, char carga, Point ptoInicial, Point ptoSeguinte);
    int extractLSB(const Vec3b pixel);

public:
    Encode();
    ~Encode();

    void encodeMessage(Mat &image, const Mat img_bin, const string message, const json conf);
};

#endif // ENCODE_H