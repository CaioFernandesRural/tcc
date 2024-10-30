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
// Função para verificar se há 32 pixels contínuos de borda
bool checaSequencia(const Mat &sobelImage, int &startY, int &startX, int length, 
                    int &initY, int &initX) {
    int contaSequencia = 0;
    bool encontrouPrimeiro = false;

    for (int i = startY; i < sobelImage.rows; i++) {
        for (int j = (encontrouPrimeiro ? 0 : startX); j < sobelImage.cols; j++) {
            if (sobelImage.at<uchar>(i, j) == 255) {
                // Armazena a posição do início da sequência
                if (!encontrouPrimeiro) {
                    initY = i;
                    initX = j;
                    encontrouPrimeiro = true;
                }
                contaSequencia++;
            } else {
                // Reseta a contagem se a sequência for interrompida
                contaSequencia = 0;
                encontrouPrimeiro = false;
            }

            // Verifica se a sequência foi encontrada
            if (contaSequencia == length) {
                startY = i;
                startX = j;
                cout << "Sequência de " << length << " pixels encontrada.\n";
                return true;
            }
        }
    }
    // Se chegou ao fim da imagem sem encontrar a sequência
    return false;
}


// Função auxiliar para definir o LSB de um pixel
void setLSB(Mat &image, int location, int bitValue) {
    
}

// Função para inserir um bloco de 32 bits nos pixels da imagem
void insertBlock(Mat &image, char carga, int loc1, int loc2, size_t &bitIndex) {
    static bool firstBlock = true;

    cout << "\nInserindo bloco para o caractere: '" << carga << "'\n";
    cout << "Localização do bloco atual: " << loc2 << "\n";
    cout << "Localização do bloco anterior: " << loc1 << "\n";

    
}


void encodeMessage(Mat &image, const Mat &sobelImage, const string &message) {

    int locAnt = -1;
    int locAtu = 0;

    string delimitada = message + '\0';

    int blocksInserted = 0;

    //coordenadas atuais da sequência de busca de blocos
    int x = 0;
    int y = 0;
    int startX = 0;
    int startY = 0;

    for(int i = 0; i < delimitada.length(); i++){
        char caractere = delimitada[i];
        
        int initY, initX;

        if (checaSequencia(sobelImage, startY, startX, 32, initY, initX)) {
            cout << "Início da sequência: (" << initY << ", " << initX << ")\n";
            cout << "Fim da sequência: (" << startY << ", " << startX << ")\n";

            //calcula loc de inserção (Atual)

            //insere bloco, passa locAtual

            //atualiza locAnt para = locAtual

            blocksInserted++;

        }
    }

    bool endOfMessage = false;
}


int main() {
    //char nome[100], nome_out[100];
    int ksize = 0;
    Mat img, img_sobel, img_gray, imgOri;

    // cout << "Digite o nome da imagem original: ";
    // cin >> nome;
    //comentei pra não ter que ficar digitando toda hora
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
        //cout << "Digite o tamanho do kernel do Sobel (ímpar entre 3 e 31): ";
        //cin >> ksize;
        ksize = 3;

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
    threshold = 0;

    // Recebe endereço da img_sobel
    binarizeImage(img_sobel, threshold);
    // Salvar a imagem Sobel binarizada
    imwrite("imagem_binarizada.png", img_sobel);

    string nome_out = "teste.png";
    // cin.ignore();
    // cout << "Digite o nome da imagem de saida: ";
    // cin >> nome_out;

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
