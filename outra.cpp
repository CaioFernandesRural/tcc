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

bool checaSequencia(const Mat &sobelImage, int &startY, int &startX, int length, 
                    int &initY, int &initX) {
    int contaSequencia = 0;

    for (int i = startY; i < sobelImage.rows; i++) {
        for (int j = (i == startY ? startX : 0); j < sobelImage.cols; j++) {
            if (sobelImage.at<uchar>(i, j) == 255) {
                // Armazena a posição do início da sequência
                if (contaSequencia == 0) {
                    initY = i;
                    initX = j;
                }
                contaSequencia++;
            } else {
                // Reseta a contagem se a sequência for interrompida
                contaSequencia = 0;
            }

            // Verifica se a sequência foi encontrada
            if (contaSequencia == length) {
                startY = i;
                startX = j;
                cout << "\n\n" << "Sequência de " << length << " pixels encontrada.\n";
                return true;
            }
        }
    }
    // Se chegou ao fim da imagem sem encontrar a sequência
    return false;
}


// Função auxiliar para definir o LSB de um pixel
void setLSB(Mat &image, Point local, int bitValue) {
    Vec3b &pixel = image.at<Vec3b>(local.y, local.x);
    // Define o valor do LSB de acordo com o valor de bitValue
    if (bitValue == 1) {
        pixel[0] |= 1;  // Força o LSB para 1
    } else {
        pixel[0] &= ~1; // Força o LSB para 0
    }  // Altera apenas o LSB do canal azul
    cout << "\n" << pixel << "\n";
}

// Função para inserir um bloco de 8 + 20 bits nos pixels da imagem
void insertBlock(Mat &image, char carga, Point ptoInicial, Point ptoSeguinte) {

    vector<int> bitValues(8);

    int i = ptoInicial.x;
    int j = ptoInicial.y;

    cout << "\n" << "Inserindo bloco para o caractere: '" << carga << "'\n";
    cout << "Localização inicial do bloco atual: " << ptoInicial << "\n";
    // cout << "Localização do bloco anterior: " << loc1 << "\n";

    cout << "Bits de carga: ";
    for (int k = 7; k >= 0; --k) {
        bitValues[7 - k] = (carga >> k) & 1;
        cout << bitValues[7 - k];
    }
    cout << "\n";

    // Loop para inserir os bits na imagem
    int bitIndex = 0;
    while (bitIndex < 8) {
        if (i >= image.cols) {  // Se atingir o fim da coluna, passa para a próxima linha
            i = 0;
            j++;
        }
        if (j >= image.rows) {  // Se atingir o fim da linha, para de inserir (fora dos limites da imagem)
            cout << "Erro: imagem muito pequena para inserir todos os bits." << endl;
            return;
        }
        setLSB(image, Point(i, j), bitValues[bitIndex]);
        bitIndex++;
        i++;  // Avança para o próximo pixel na coluna
    }
    cout << "\n";

    //declara y converte
    int loc = ptoSeguinte.y * image.cols + ptoSeguinte.x;
    vector<int> locBits(20);

    cout << "\nBits de loc (" << loc << "): ";
    for (int k = 19; k >= 0; --k) {
        locBits[19 - k] = (loc >> k) & 1;
        cout << locBits[19 - k]; // Exibe cada bit após calcular
    }
    cout << "\n";

    bitIndex = 0;
    while (bitIndex < 20) {
        if (i >= image.cols) {
            i = 0;
            j++;
        }
        if (j >= image.rows) {
            cout << "Erro: imagem muito pequena para inserir todos os bits." << endl;
            return;
        }
        cout << "Inserindo locBit[" << bitIndex << "] = " << locBits[bitIndex]
            << " na posição (" << i << ", " << j << ")" << endl;

        setLSB(image, Point(i, j), locBits[bitIndex]);
        bitIndex++;
        i++;  // Avança para o próximo pixel na coluna
    }
}


void encodeMessage(Mat &image, const Mat &sobelImage, const string &message) {

    vector<Point> positions;

    string delimitada = message + '\0';

    int blocksInserted = 0;

    //coordenadas atuais da sequência de busca de blocos
    int x = 0;
    int y = 0;
    int startX = 0;
    int startY = 0;

    for(int i = 0; i < delimitada.length(); i++){
        
        int initY, initX;

        if (checaSequencia(sobelImage, startY, startX, 5, initY, initX)) {
            cout << "Início da sequência: (" << initY << ", " << initX << ")\n";
            cout << "Fim da sequência: (" << startY << ", " << startX << ")\n";
				startX ++; // proximo elemento após fim de bloco - Rizzo

            //vetor de posições inicias dos blocos
            positions.push_back(Point(initX, initY));

            blocksInserted++;

        }
    }

    for (int i = 0; i < delimitada.length(); i++){
        char caractere = delimitada[i];
        Point pos = positions[i];
        Point posProx = positions[i+1];
        
        cout << "\n" << "Bloco em posição inicial: (" << pos.x << ", " << pos.y << ")\n";
        insertBlock(image, caractere, pos, posProx);
    }  

    bool endOfMessage = false;
}

// Função para extrair o LSB de um pixel em uma imagem
int extractLSB(const Vec3b &pixel) {
    return pixel[0] % 2;  // Retorna o LSB do canal azul
}

char decodeBloco(const Mat image, int &N) {
    int y = N / image.cols;
    int x = N % image.cols;

    char caractere = 0;
    int newN = 0;

    for (int i = 0 ; i < 8; ++i) {
        if (x >= image.cols) {
            x = 0;
            ++y;
        }
        if (y >= image.rows) {
            throw runtime_error("Fim da imagem atingido durante a decodificação.");
        }

        Vec3b pixel = image.at<Vec3b>(y, x);
        int lsb = extractLSB(pixel);

        caractere = (caractere << 1) | lsb;

        // Debugging: print the values at each step
        cout << "[Caractere] Bit " << i << ": LSB = " << lsb 
        << ", Caractere (parcial) = " << caractere << endl;

        ++x;
    }

    if(caractere == '\0'){
        return caractere;
    }

    for (int i = 8; i < 28; ++i) {
        if (x >= image.cols) {
            x = 0;
            ++y;
        }
        if (y >= image.rows) {
            throw runtime_error("Fim da imagem atingido durante a decodificação.");
        }

        Vec3b pixel = image.at<Vec3b>(y, x);
        int lsb = extractLSB(pixel);

        newN = (newN << 1) | lsb;

        // Debugging: print the values at each step
        cout << "[newN] Bit " << i << ": LSB = " << lsb 
        << ", newN (parcial) = " << newN << endl;

        ++x;
    }

    if (newN == 0) {
    throw runtime_error("newN decodificado como 0, o que causa loop infinito.");
    }

    N = newN;
    return caractere;
}

string decodeImagem(const Mat &image, int inicialN) {
    string mensagem;
    int N = inicialN;
    char caractere = '-';

    do {

        cout << caractere;
        caractere = decodeBloco(image, N);

        if(N == inicialN){return "erro menor";}

        mensagem += caractere;

    } while (caractere != '\0');

    return mensagem;
}


int main() {
    //char nome[100], nome_out[100];
    int ksize = 0;
    Mat img, img_sobel, img_gray, imgOri;

    // cout << "Digite o nome da imagem original: ";
    // cin >> nome;
    //comentei pra não ter que ficar digitando toda hora
    //string nome = "cg.png";
    string nome = "img_teste.png";

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

    int N = 0;

    // cin.ignore();
    // cout << "Digite o valor de N do primeiro bloco: ";
    // cin >> N;
    string recoveredMessage = decodeImagem(imgWithMessage, N);

    // // Mostrar a mensagem recuperada
    cout << "Mensagem decodificada: " << recoveredMessage << endl;

    // Comparar a imagem original com a imagem com mensagem para visualizar a diferença
    Mat diffImage = Mat::zeros(imgOri.size(), CV_8UC1); // Imagem de comparação inicializada como preta

    for (int i = 0; i < imgOri.rows; ++i) {
        for (int j = 0; j < imgOri.cols; ++j) {
            uchar originalBit = imgOri.at<Vec3b>(i, j)[0] & 0x01; // Último bit da imagem original
            uchar messageBit = imgWithMessage.at<Vec3b>(i, j)[0] & 0x01; // Último bit da imagem com mensagem

            // Se o bit for diferente, coloca um ponto branco; se for igual, ponto preto
            diffImage.at<uchar>(i, j) = (originalBit != messageBit) ? 255 : 0;
        }
    }
    imwrite("imagem_diferenca.png", diffImage);

    return 0;
}
