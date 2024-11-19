#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <random> // Para geração de caracteres aleatórios

using namespace cv;
using namespace std;

// Função para codificar a mensagem na imagem
int encodeMessage(Mat &image, const Mat &sobelImage, const string &message) {
    int bit = 0;
    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar sobelValue = sobelImage.at<uchar>(i, j);

            if (sobelValue > 0 && bit < message.size() * 8) { // Verifica se é uma borda e há bits restantes
                Vec3b &pixel = image.at<Vec3b>(i, j);

                int charIndex = bit / 8;
                int bitIndex = bit % 8;
                char currentChar = message[charIndex];
                bool bitValue = (currentChar >> (7 - bitIndex)) & 1;

                pixel[0] = (pixel[0] & 0xFE) | bitValue;
                ++bit;
            }
        }
    }
	 if ( bit == 0 || bit != message.size() * 8 )
				return bit? bit: -1;
	 else
				return 0;
}

// Função para decodificar a mensagem da imagem
string decodeMessage(const Mat &image, const Mat &sobelImage, int messageLength) {
    string message(messageLength, '\0');
    int bit = 0;

    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar sobelValue = sobelImage.at<uchar>(i, j);

            if (sobelValue > 0 && bit < messageLength * 8) { // Verifica se é uma borda e há bits restantes
                Vec3b pixel = image.at<Vec3b>(i, j);

                int charIndex = bit / 8;
                int bitIndex = bit % 8;
                bool bitValue = pixel[0] & 1;

                message[charIndex] |= (bitValue << (7 - bitIndex));
                ++bit;
            }
        }
    }

    return message;
}

// Função para gerar uma string aleatória de um tamanho específico
string generateRandomString(size_t length) {
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distrib(0, characters.size() - 1);

    string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += characters[distrib(generator)];
    }
    return randomString;
}

int main()
{
    char nome[100], nome_out[100];
    Mat img, img_sobel, img_gray;
    int ksize = 0;
	 int merror;

    cout << "Digite o nome da imagem: ";
    cin >> nome;

    // Carregar a imagem original
    Mat imgO = imread(nome);
    if (imgO.empty())
    {
        cout << "Erro ao abrir a imagem: " << nome << " Nao existe" << endl;
        return -1;
    }

    // Converter para escala de cinza
    cvtColor(imgO, img_gray, COLOR_BGR2GRAY);

    cout << "X =" << img_gray.rows << " Y = " << img_gray.cols << " Depth = " << img_gray.depth() << " Channels = " << img_gray.channels() << endl;

    // Definir o tamanho do kernel do Sobel
    do
    {
        cout << "Digite o tamanho do kernel do Sobel: ";
        cin >> ksize;
    } while (ksize < 3 || ksize > 31);

    // Aplicar o filtro Sobel para detectar bordas
    Sobel(img_gray, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    // Salvar a imagem Sobel
    imwrite("imagem_sobel.png", img_sobel);

    cout << "Digite o nome da imagem de saida: ";
    cin >> nome_out;

    // Solicitar a escolha do usuário
    int choice;
    cout << "Escolha uma opção:" << endl;
    cout << "1. Inserir uma mensagem personalizada" << endl;
    cout << "2. Usar uma mensagem predefinida" << endl;
    cout << "Digite sua escolha (1 ou 2): ";
    cin >> choice;

    // Mensagem a ser codificada
    string message;

    if (choice == 1) {
        // Limpar o buffer do cin
        cin.ignore();
        cout << "Digite a mensagem para inserir na imagem: ";
        getline(cin, message); // Ler a mensagem completa, incluindo espaços
    } else {
        // Usar uma mensagem predefinida de 10KB
        const size_t messageSize = 10240; // 10KB em bytes
        message = generateRandomString(messageSize);
    }
    // Codificar a mensagem na imagem
    Mat imgWithMessage = imgO.clone();
    if ((merror =  encodeMessage(imgWithMessage, img_sobel, message)) != 0)
	 {
				printf("Erro %d\n", merror);
				exit (-1);
	 }

    // Salvar a imagem com a mensagem inserida
    imwrite(nome_out, imgWithMessage);

    // Comparar a imagem original com a imagem com mensagem para visualizar a diferença
    Mat diffImage = Mat::zeros(imgO.size(), CV_8UC1); // Imagem de comparação inicializada como preta

    for (int i = 0; i < imgO.rows; ++i)
    {
        for (int j = 0; j < imgO.cols; ++j)
        {
            uchar originalBit = imgO.at<Vec3b>(i, j)[0] & 0x01; // Último bit da imagem original
            uchar messageBit = imgWithMessage.at<Vec3b>(i, j)[0] & 0x01; // Último bit da imagem com mensagem

            // Se o bit for diferente, coloca um ponto branco; se for igual, ponto preto
            diffImage.at<uchar>(i, j) = (originalBit != messageBit) ? 255 : 0;
        }
    }

    // Salvar e exibir a imagem de comparação
    imwrite("imagem_diferenca.png", diffImage);
    imshow("Diferenca entre imagens", diffImage);
    waitKey(0);

    // Recuperar a mensagem da imagem modificada
    int messageLength = message.size();
    string recoveredMessage = decodeMessage(imgWithMessage, img_sobel, messageLength);

    // Mostrar a mensagem recuperada
    cout << "Mensagem recuperada: " << recoveredMessage << endl;

    return 0;
}
