#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

class Processador
{
private:

public:
    Processador();
    ~Processador();

    Mat converteCinza(const Mat image);
    Mat aplicaSobel(const Mat img_cinza, int ksize);
    Mat binarizaImagem(Mat img_sobel, int threshold);

};

#endif // PROCESSADOR
