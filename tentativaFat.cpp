#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>  // Para runtime_error

using namespace cv;
using namespace std;

// O bloco é organizado em 8 bits de carga,
// 4 para marcar o fim da carga
// e 20 para a localização do próximo bloco (y * N + x)

void binarizeImage(Mat &sobelImage, int threshold) {
    for (int i = 0; i < sobelImage.rows; ++i) {
        for (int j = 0; j < sobelImage.cols; ++j) {
            uchar &pixel = sobelImage.at<uchar>(i, j);
            pixel = (pixel >= threshold) ? 255 : 0;
        }
    }
}

// Função para verificar se há 32 pixels contínuos de borda
bool checkContinuousSequence(const Mat &sobelImage, int startY, int startX, int length) {
    int cols = sobelImage.cols;

    for (int i = 0; i < length; ++i) {
        int newX = (startX + i) % cols;
        int newY = startY + (startX + i) / cols;

        if (newY >= sobelImage.rows || sobelImage.at<uchar>(newY, newX) != 255) {
            return false;  // Não encontrou sequência contínua
        }
    }
    return true;
}

// Função auxiliar para definir o LSB de um pixel
void setLSB(Mat &image, int location, int bitValue) {
    int cols = image.cols;
    int y = location / cols; // Calcula a linha (y) do pixel
    int x = location % cols; // Calcula a coluna (x) do pixel

    Vec3b &pixel = image.at<Vec3b>(y, x); // Acessa o pixel na posição (y, x)

    // Limpa o LSB atual do canal azul
    pixel[0] = (pixel[0] & 0xFE); // Zera o LSB (primeiro bit) do canal azul

    // Define o LSB como o valor de bitValue (0 ou 1)
    if (bitValue == 1) {
        pixel[0] |= 0x01; // Se bitValue for 1, define LSB como 1
    }
    // Se bitValue for 0, não faz nada, pois o LSB já está zerado

    // Exibe o valor do pixel após a alteração (opcional)
    // cout << int(pixel[0]) << " ";
    cout << " |" << int(pixel[0]) << "| ";
}


// Função para inserir um bloco de 32 bits nos pixels da imagem
// Função para inserir um bloco de 32 bits nos pixels da imagem
void insertBlock(Mat &image, char carga, int loc1, int loc2, size_t &bitIndex) {

    cout << "\nInserindo bloco para o caractere: '" << carga << "'\n";
    cout << "Localização do bloco atual: " << loc2 << "\n";
    cout << "Localização do bloco anterior: " << loc1 << "\n";

    // Inserir os 8 bits de carga
    cout << "Bits de carga: ";
    for (int i = 0; i < 8; ++i) {
        int bitValue = (carga >> (7 - i)) & 1;
        setLSB(image, loc2 + i, bitValue);
        cout << bitValue << " ";
    }
    cout << "\n";

    // Inserir os 4 bits do marcador de fim de carga
    cout << "Marcador de fim de carga (4 bits): ";
    for (int i = 0; i < 4; ++i) {
        setLSB(image, loc2 + 8 + i, 0);  // Sempre 0 indicando fim da carga
        cout << 0;
    }
    cout << "\n";

    // Inserir os 20 bits de localização do próximo bloco (variável N)
    cout << "Próxima localização (20 bits): ";
    for (int i = 0; i < 20; ++i) {
        int bitValue = (loc1 >> (19 - i)) & 1;
        setLSB(image, loc2 + 12 + i, bitValue);
        cout << bitValue;
    }
    cout << "\n";

    bitIndex += 32;  // Atualizar o índice de bits para o próximo bloco
}


void encodeMessage(Mat &image, const Mat &sobelImage, const string &message) {
    int rows = sobelImage.rows;
    int cols = sobelImage.cols;

    int loc1 = -1;  // Localização do bloco anterior
    int loc2 = 0;   // Localização do bloco atual

    string messageWithDelimiter = message + '\0';  // Adiciona o delimitador
    size_t bitIndex = 0;  // Índice do bit na mensagem
    bool endOfMessage = false;
    size_t blocksInserted = 0;  // Contador de blocos inseridos

    for (size_t i = 0; i < messageWithDelimiter.size() && !endOfMessage; ++i) {
        char currentChar = messageWithDelimiter[i];  // Caractere atual
        bool blockFound = false;  // Controle para verificar se um bloco foi encontrado

        // Percorrer a imagem para encontrar uma sequência contínua de 32 pixels
        for (int y = 0; y < rows && !blockFound; ++y) {
            for (int x = 0; x < cols && !blockFound; ++x) {
                if (sobelImage.at<uchar>(y, x) == 255 &&
                    checkContinuousSequence(sobelImage, y, x, 32)) {

                    // Calcular a localização do bloco atual
                    loc2 = y * cols + x;

                    // Inserir o bloco na imagem
                    insertBlock(image, currentChar, loc1, loc2, bitIndex);

                    // Atualizar loc1 para o próximo bloco
                    loc1 = loc2;

                    // Incrementar o contador de blocos inseridos
                    blocksInserted++;
                    blockFound = true;

                    // Verificar se é o último caractere
                    if (currentChar == '\0') {
                        endOfMessage = true;
                    }
                }
            }
        }

        // Verificar se nenhum bloco foi encontrado para o caractere atual
        if (!blockFound) {
            cerr << "Erro: Não foi possível inserir o caractere '" 
                 << currentChar << "' na imagem.\n";
        }
    }

    // Verificar se a mensagem foi completamente inserida
    if (blocksInserted < messageWithDelimiter.size()) {
        cerr << "Atenção: Mensagem incompleta. Blocos inseridos: " 
             << blocksInserted << ", Esperado: " 
             << messageWithDelimiter.size() << endl;
    } else {
        cout << "Mensagem inserida com sucesso!" << endl;
    }
}


int main() {
    //char nome[100], nome_out[100];
    Mat img, img_sobel, img_gray, imgOri;
    int ksize = 0;

    // cout << "Digite o nome da imagem original: ";
    // cin >> nome;
    string nome = "cg.png";

    // Carregar a imagem original
    imgOri = imread(nome);

    if (imgOri.empty()) {  // Verificar se a imagem foi carregada
        cerr << "Erro: Não foi possível abrir a imagem: " << nome << endl;
        return -1;  // Encerrar o programa com código de erro
    }

    // Converter para escala de cinza
    cvtColor(imgOri, img_gray, COLOR_BGR2GRAY);

    // Solicitar o tamanho do kernel do Sobel
    try {
        // cout << "Digite o tamanho do kernel do Sobel (ímpar entre 3 e 31): ";
        // cin >> ksize;
        ksize = 9;

        if (ksize < 3 || ksize > 31 || (ksize % 2 == 0)) {
            throw runtime_error("Valor inválido! O kernel deve ser ímpar entre 3 e 31.");
        }
    } 
    catch (const runtime_error& e) {
        cerr << "Erro: " << e.what() << endl;
        return -1;  // Encerrar o programa com código de erro
    }

    // Aplicar o filtro Sobel para detectar bordas
    Sobel(img_gray, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    // Salvar a imagem Sobel original antes da binarização
    imwrite("sobel_original.png", img_sobel);

    int threshold;

    // cout << "Digite o valor do threshold (0 a 255): ";
    // cin >> threshold;
    threshold = 100;

    // Recebe endereço da img_sobel
    binarizeImage(img_sobel, threshold);
    // Salvar a imagem Sobel binarizada
    imwrite("imagem_binarizada.png", img_sobel);

    // cin.ignore();
    // cout << "Digite o nome da imagem de saida: ";
    // cin >> nome_out;
    string nome_out = "teste.png";

    string mensagem = "caio";

    // Codificar a mensagem na imagem
    Mat imgWithMessage = imgOri.clone();
    encodeMessage(imgWithMessage, img_sobel, mensagem);

    // Salvar a imagem com a mensagem inserida
    imwrite(nome_out, imgWithMessage);

    // Decodificar a mensagem no final

    // int N;

    // cin.ignore();
    // cout << "Digite o valor de N do primeiro bloco: ";
    // cin >> N;
    // string recoveredMessage = decodeMessage(imgWithMessage, N);

    // // Mostrar a mensagem recuperada
    // cout << "Mensagem decodificada: " << recoveredMessage << endl;

    return 0;
}
