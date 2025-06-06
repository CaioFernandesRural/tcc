#include "../include/CapacityAnalyser.hpp"

bool CapacityAnalyser::checaSequencia(const Mat& sobelImage, Point& ptoInicio, int blockLen, Point& ptoTrabalho) {
    int contaSequencia = 0;

    for (int i = ptoInicio.y; i < sobelImage.rows; i++) {
        for (int j = (i == ptoInicio.y ? ptoInicio.x : 0); j < sobelImage.cols; j++) {
            if (sobelImage.at<uchar>(i, j) == 255) {
                if (contaSequencia == 0) {
                    ptoTrabalho.y = i;
                    ptoTrabalho.x = j;
                }
                contaSequencia++;
            } else {
                contaSequencia = 0;
            }

            if (contaSequencia == blockLen) {
                ptoInicio.x = j + 1;
                ptoInicio.y = i;

                if (ptoInicio.x >= sobelImage.cols) {
                    ptoInicio.x = 0;
                    ptoInicio.y++;
                }

                return true;
            }
        }
    }
    return false;
    
};

int CapacityAnalyser::analyze(const cv::Mat& image) {
    if (image.empty()) {
        throw std::runtime_error("Imagem vazia.");
    }

    int totalPixels = image.rows * image.cols;
    int bits_N = std::floor(std::log2(totalPixels - 1)) + 1;
    int blockLen = bits_N + 8;

    vector<Point> allBlocks;
    Point ptoInicio(0, 0);

    while (true) {
        Point ptoTrabalho(0, 0);

        if (checaSequencia(image, ptoInicio, blockLen, ptoTrabalho)) {
            allBlocks.push_back(ptoTrabalho);
            // Nada de ptoInicio.x++ aqui!
        } else {
            break; // não há mais blocos
        }

        if (ptoInicio.y >= image.rows) {
            break; // segurança extra: fim da imagem
        }
    }

    return static_cast<int>(allBlocks.size());
}
