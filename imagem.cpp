#include <iostream>
#include <string>
#include <bitset> // Necessário para a conversão de bits
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(void) {
    char nome[100], nome_out[100];
    Mat img, img_sobel, imgO;
    int ksize = 0;

    cout << "Digite o nome da imagem: ";
    cin >> nome;

    imgO = imread(nome);
    if (imgO.empty()) {
        cout << "Erro abertura de imagem: " << nome << " Nao existe" << endl;
        exit(-1);
    }
    img = imread(nome, 0);
    if (img.empty()) {
        cout << "Erro abertura de imagem: " << nome << " Nao existe" << endl;
        exit(-1);
    }

    cout << "X =" << img.rows << " Y = " << img.cols << " Depth = " << img.depth() << " Channels = " << img.channels() << endl;

    /*
     * A funcao Sobel, o kernel deve ser um valor impar entre 3 e 31
     */
    do {
        cout << "Digite o tamanho do kernel do sobel: ";
        cin >> ksize;
    } while (ksize < 3 || ksize > 31);

    Sobel(img, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    cout << "Digite o nome da imagem de saida: ";
    cin >> nome_out;

    cout << "Pixel = " << imgO.at<Vec3b>(0, 0) << endl;
    imshow("Original", imgO);
    imshow("P&B", img);
    imshow("Sobel", img_sobel);
    waitKey(0);

    imwrite(nome_out, img_sobel);

    // Armazenar a matriz de bits da imagem Sobel
    vector<vector<int>> bitsMap;

    cout << "Matriz de bits da imagem Sobel:" << endl;
    for (int i = 0; i < img_sobel.rows; ++i) {
        vector<int> rowBits;
        for (int j = 0; j < img_sobel.cols; ++j) {
            uchar pixelValue = img_sobel.at<uchar>(i, j);
            for (int k = 7; k >= 0; --k) { // Itera sobre cada bit do byte
                int bit = (pixelValue >> k) & 1;
                rowBits.push_back(bit); // Armazena o bit na linha atual
            }
        }
        bitsMap.push_back(rowBits); // Adiciona a linha de bits ao mapa
    }

    // Conversão de bits de volta para imagem
    /*
    Mat img_bits = Mat::zeros(img_sobel.size(), CV_8U); // Imagem para recriação a partir dos bits

    for (int i = 0; i < img_sobel.rows; ++i) {
        for (int j = 0; j < img_sobel.cols; ++j) {
            uchar pixelValue = img_sobel.at<uchar>(i, j);
            bitset<8> bits(pixelValue); // Converte o valor do pixel para bits

            // Reconstruir o valor do pixel a partir dos bits
            uchar reconstructedPixelValue = 0;
            for (int k = 0; k < 8; ++k) {
                if (bits[k] == 1) {
                    reconstructedPixelValue |= (1 << k);
                }
            }
            img_bits.at<uchar>(i, j) = reconstructedPixelValue;
        }
    }

    // Exibir a imagem reconstruída a partir dos bits
    imshow("Imagem Reconstruída a partir dos Bits", img_bits);
    */

    return 0;
}
