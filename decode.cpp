#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

// Função para decodificar a mensagem da imagem com delimitador
string decodeMessage(const Mat &image, const Mat &binaryImage) {
    string message;
    char currentChar = 0;
    int bitCount = 0;
    
    for (int i = 0; i < binaryImage.rows; ++i) {
        for (int j = 0; j < binaryImage.cols; ++j) {
            uchar binaryValue = binaryImage.at<uchar>(i, j);

            if (binaryValue > 0) { // Verifica se o pixel faz parte de uma borda (imagem binarizada)
                Vec3b pixel = image.at<Vec3b>(i, j);
                bool bitValue = pixel[0] & 1; // Obtém o último bit do canal azul

                currentChar |= (bitValue << (7 - (bitCount % 8))); // Insere o bit no caractere atual
                ++bitCount;

                // Se completou um caractere (8 bits), adiciona ao string
                if (bitCount % 8 == 0) {
                    // Se o caractere é o delimitador, termina a leitura
                    if (currentChar == '\0') {
                        return message;
                    }
                    message += currentChar;
                    currentChar = 0; // Reseta o caractere para o próximo
                }
            }
        }
    }

    return message; // Retorna a mensagem lida (caso não tenha delimitador)
}

int main() {
    char nome[100];

    // Solicitar o nome da imagem com a mensagem
    cout << "Digite o nome da imagem com a mensagem inserida: ";
    cin >> nome;

    // Carregar a imagem com a mensagem inserida
    Mat imgWithMessage = imread(nome);
    if (imgWithMessage.empty()) {
        cout << "Erro ao abrir a imagem: " << nome << " Nao existe" << endl;
        return -1;
    }

    // Carregar a imagem binarizada (substituindo a Sobel)
    Mat binaryImage = imread("imagem_binarizada.png", IMREAD_GRAYSCALE); // Certifique-se que a imagem binarizada foi salva com esse nome
    if (binaryImage.empty()) {
        cout << "Erro ao abrir a imagem binarizada." << endl;
        return -1;
    }

    // Decodificar a mensagem da imagem
    string recoveredMessage = decodeMessage(imgWithMessage, binaryImage);

    // Mostrar a mensagem decodificada
    cout << "Mensagem decodificada: " << recoveredMessage << endl;

    return 0;
}
