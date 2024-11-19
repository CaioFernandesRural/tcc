#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // Para usar a função de conversão de cores

using namespace cv;
using namespace std;

// Função para comparar se duas imagens são iguais
bool compareImages(const Mat &img1, const Mat &img2) {
    if (img1.empty() || img2.empty()) {
        cout << "Erro ao carregar as imagens." << endl;
        return false;
    }

    // Verificar se as dimensões das imagens são iguais
    if (img1.size() != img2.size() || img1.type() != img2.type()) {
        cout << "As imagens têm tamanhos ou tipos diferentes." << endl;
        return false;
    }

    // Converter as imagens para escala de cinza, caso sejam coloridas
    Mat img1_gray, img2_gray;
    if (img1.channels() > 1) {
        cvtColor(img1, img1_gray, COLOR_BGR2GRAY);
    } else {
        img1_gray = img1;
    }

    if (img2.channels() > 1) {
        cvtColor(img2, img2_gray, COLOR_BGR2GRAY);
    } else {
        img2_gray = img2;
    }

    // Comparar pixel por pixel
    Mat diff;
    compare(img1_gray, img2_gray, diff, CMP_NE);

    int nonZeroPixels = countNonZero(diff);  // Contar os pixels diferentes

    if (nonZeroPixels == 0) {
        return true;  // As imagens são idênticas
    } else {
        return false; // As imagens são diferentes
    }
}

int main() {
    char nome1[100], nome2[100];

    // Solicitar o nome das duas imagens
    cout << "Digite o nome da primeira imagem (gerada pelo denovo.cpp): ";
    cin >> nome1;

    cout << "Digite o nome da segunda imagem (gerada pelo decode.cpp): ";
    cin >> nome2;

    // Carregar as duas imagens
    Mat img1 = imread(nome1);
    Mat img2 = imread(nome2);

    // Comparar as imagens
    bool iguais = compareImages(img1, img2);

    if (iguais) {
        cout << "As imagens são idênticas!" << endl;
    } else {
        cout << "As imagens são diferentes!" << endl;
    }

    return 0;
}
