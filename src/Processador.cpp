#include "../include/Processador.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

Processador::Processador() {}

Processador::~Processador() {}

Mat Processador::converteCinza(const Mat image)
{
    Mat img_cinza;
    cvtColor(image, img_cinza, COLOR_BGR2GRAY);
    return img_cinza;
}

Mat Processador::aplicaSobel(const Mat img_cinza, int ksize)
{
    Mat img_sobel;
    Sobel(img_cinza, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);
    return img_sobel;
}

Mat Processador::binarizaImagem(Mat img_sobel, int threshold)
{
    for (int i = 0; i < img_sobel.rows; ++i)
    {
        for (int j = 0; j < img_sobel.cols; ++j)
        {
            uchar &pixel = img_sobel.at<uchar>(i, j);
            pixel = (pixel >= threshold) ? 255 : 0;
        }
    }
    return img_sobel;
}