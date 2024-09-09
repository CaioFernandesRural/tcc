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

// Função para calcular o tamanho máximo da mensagem em bits e bytes
int calculateMaxMessageSize(const Mat &sobelImage) {
    int bitCount = 0;

    // Percorre a imagem Sobel para contar os pixels que fazem parte de bordas
    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            if (sobelImage.at<uchar>(i, j) == 255) {
                ++bitCount; // Cada pixel pode armazenar 1 bit no canal azul
            }
        }
    }

    // Retorna o número de bits disponíveis para armazenar a mensagem
    return bitCount / 8; // Retorna o valor em bytes
}

// Função para codificar a mensagem na imagem
void encodeMessage(Mat &image, const Mat &sobelImage, const string &message) {
    int bit = 0;
    string messageWithDelimiter = message + '\0';  // Adiciona o delimitador ao final da mensagem

    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar sobelValue = sobelImage.at<uchar>(i, j);

            if (sobelValue == 255 && bit < messageWithDelimiter.size() * 8) { // Verifica se é uma borda e há bits restantes
                Vec3b &pixel = image.at<Vec3b>(i, j);

                int charIndex = bit / 8;
                int bitIndex = bit % 8;
                char currentChar = messageWithDelimiter[charIndex];
                bool bitValue = (currentChar >> (7 - bitIndex)) & 1;

                pixel[0] = (pixel[0] & 0xFE) | bitValue;  // Insere o bit no canal azul
                ++bit;
            }
        }
    }
}

// Função para decodificar a mensagem da imagem
string decodeMessage(const Mat &image, const Mat &sobelImage) {
    string message;
    int bit = 0;

    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar sobelValue = sobelImage.at<uchar>(i, j);

            if (sobelValue == 255) { // Verifica se o pixel faz parte de borda
                Vec3b pixel = image.at<Vec3b>(i, j);

                if (bit % 8 == 0) {
                    message += '\0'; // Adiciona um novo caractere à string
                }

                int charIndex = bit / 8;
                int bitIndex = bit % 8;
                bool bitValue = pixel[0] & 1;

                message[charIndex] |= (bitValue << (7 - bitIndex));
                ++bit;

                // Verifica o final da mensagem
                if (bit % 8 == 0 && message[charIndex] == '\0') {
                    return message.substr(0, charIndex);
                }
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

// Função para binarizar a imagem Sobel com base no threshold ou em uma faixa de valores
void binarizeImage(Mat &sobelImage, int threshold, int lowerBound = -1, int upperBound = -1) {
    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar &pixel = sobelImage.at<uchar>(i, j);
            if (lowerBound != -1 && upperBound != -1) {
                pixel = (pixel >= lowerBound && pixel <= upperBound) ? 255 : 0;
            } else {
                pixel = (pixel >= threshold) ? 255 : 0;
            }
        }
    }
}

// Função para calcular a diferença entre duas imagens
Mat calculateDifference(const Mat &original, const Mat &modified) {
    // Cria uma imagem em preto e branco (1 canal) para armazenar a diferença
    Mat diff = Mat::zeros(original.size(), CV_8UC1);

    for (int i = 0; i < original.rows; ++i) {
        for (int j = 0; j < original.cols; ++j) {
            Vec3b pixelOriginal = original.at<Vec3b>(i, j);
            Vec3b pixelModified = modified.at<Vec3b>(i, j);

            // Inicializa um flag para verificar se há diferença entre os canais
            bool isDifferent = false;

            // Comparar bit a bit os três canais (BGR) dos pixels
            for (int k = 0; k < 3; ++k) {
                if (pixelOriginal[k] != pixelModified[k]) {
                    isDifferent = true;
                    break;
                }
            }

            // Se houver diferença, marca o pixel em branco (255); caso contrário, preto (0)
            diff.at<uchar>(i, j) = isDifferent ? 255 : 0;
        }
    }

    return diff;
}


int main()
{
    char nome[100], nome_out[100];
    Mat img, img_sobel, img_gray;
    int ksize = 0;

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

    cout << "X = " << img_gray.rows << " Y = " << img_gray.cols << " Depth = " << img_gray.depth() << " Channels = " << img_gray.channels() << endl;

    // Definir o tamanho do kernel do Sobel
    do
    {
        cout << "Digite o tamanho do kernel do Sobel: ";
        cin >> ksize;
    } while (ksize < 3 || ksize > 31);

    // Aplicar o filtro Sobel para detectar bordas
    Sobel(img_gray, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    // Salvar a imagem Sobel original antes da binarização
    imwrite("sobel_original.png", img_sobel);

    // Opção de binarização
    int option;
    cout << "Escolha a opção de binarização:\n1. Usar threshold\n2. Usar faixa de valores\nEscolha: ";
    cin >> option;

    if (option == 1) {
        int threshold;
        do {
            cout << "Digite o valor do threshold (0 a 255): ";
            cin >> threshold;
        } while (threshold < 0 || threshold > 255);
        binarizeImage(img_sobel, threshold);
    } else if (option == 2) {
        int lowerBound, upperBound;
        do {
            cout << "Digite o valor mínimo da faixa (0 a 255): ";
            cin >> lowerBound;
            cout << "Digite o valor máximo da faixa (0 a 255): ";
            cin >> upperBound;
        } while (lowerBound < 0 || upperBound > 255 || lowerBound > upperBound);
        binarizeImage(img_sobel, 0, lowerBound, upperBound);
    }

    // Salvar a imagem Sobel binarizada
    imwrite("imagem_binarizada.png", img_sobel);

    // Calcular e mostrar o tamanho máximo da mensagem
    int maxMessageSize = calculateMaxMessageSize(img_sobel);
    cout << "Tamanho máximo da mensagem: " << maxMessageSize << " bytes." << endl;

    cout << "Digite o nome da imagem de saida: ";
    cin >> nome_out;

    // Solicitar a escolha do usuário
    int choice;
    cout << "Escolha uma opção:\n1. Inserir uma mensagem personalizada\n2. Usar a mensagem predefinida com o tamanho máximo (" << maxMessageSize << " bytes)\nEscolha (1 ou 2): ";
    cin >> choice;

    // Mensagem a ser codificada
    string message;

    if (choice == 1) {
        // Limpar o buffer do cin
        cin.ignore();
        cout << "Digite a mensagem para inserir na imagem: ";
        getline(cin, message);

        // Verificar o tamanho da mensagem
        if (message.size() > maxMessageSize) {
            cout << "Erro: A mensagem excede o tamanho máximo permitido de " << maxMessageSize << " bytes." << endl;
            return -1;
        }
    } else if (choice == 2) {
        // Gerar uma string aleatória de tamanho máximo
        message = generateRandomString(maxMessageSize);
        cout << "Mensagem gerada: " << message << endl;
    } else {
        cout << "Escolha inválida." << endl;
        return -1;
    }

    // Codificar a mensagem na imagem
    Mat imgWithMessage = imgO.clone();
    encodeMessage(imgWithMessage, img_sobel, message);

    // Salvar a imagem com a mensagem inserida
    imwrite(nome_out, imgWithMessage);

    // Calcular e salvar a diferença entre a imagem original e a imagem com a mensagem
    Mat imgDiff = calculateDifference(imgO, imgWithMessage);
    imwrite("imagem_diferenca.png", imgDiff);

    // Decodificar a mensagem no final
    string recoveredMessage = decodeMessage(imgWithMessage, img_sobel);

    // Mostrar a mensagem recuperada
    cout << "Mensagem decodificada: " << recoveredMessage << endl;

    return 0;
}
