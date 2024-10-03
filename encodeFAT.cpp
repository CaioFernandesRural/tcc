#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

// Função para aplicar o filtro de Sobel e binarizar a imagem
Mat applySobelBinarization(const Mat& image) {
    Mat gray, sobel, sobelBin;
    cvtColor(image, gray, COLOR_BGR2GRAY); // Converter para escala de cinza
    Sobel(gray, sobel, CV_8U, 1, 0, 3);   // Aplicar Sobel
    threshold(sobel, sobelBin, 128, 255, THRESH_BINARY); // Binarização
    return sobelBin;
}

// Função para converter um caractere para uma sequência de bits (string)
string charToBits(char c) {
    return bitset<8>(c).to_string();
}

// Função para encontrar todas as bordas (pixels onde Sobel == 255)
vector<Point> findEdges(const Mat& sobelMap) {
    vector<Point> edges;
    for (int row = 0; row < sobelMap.rows; row++) {
        for (int col = 0; col < sobelMap.cols; col++) {
            if (sobelMap.at<uchar>(row, col) == 255) {
                edges.push_back(Point(col, row));
            }
        }
    }
    return edges;
}

// Função para codificar a mensagem usando FAT (referências em Red/Green)
void encodeMessage(Mat& image, const vector<Point>& edges, const string& message) {
    int bit = 0;
    string messageWithDelimiter = message + "|"; // Delimitador para marcar o fim da mensagem
    int messageBitLength = messageWithDelimiter.size() * 8;
    int progressStep = edges.size() / 10; // Define progresso a cada 10%

    // Percorrer as bordas e codificar a mensagem
    for (size_t i = 0; i < edges.size(); i++) {
        if (bit >= messageBitLength) break; // Se todos os bits da mensagem foram codificados

        // Codificar o bit no canal azul (LSB)
        Point currentEdge = edges[i];
        Vec3b& pixel = image.at<Vec3b>(currentEdge);
        char currentBit = charToBits(messageWithDelimiter[bit / 8])[bit % 8]; // Obter bit da mensagem
        pixel[0] = (pixel[0] & 0xFE) | (currentBit - '0'); // Altera o bit menos significativo do canal azul
        
        // Definir o próximo bloco nos canais Red/Green
        if (i + 1 < edges.size()) {
            Point nextEdge = edges[i + 1];
            pixel[2] = nextEdge.x; // Canal Red armazena a coordenada x do próximo bloco
            pixel[1] = nextEdge.y; // Canal Green armazena a coordenada y do próximo bloco
        } else {
            // Se não houver próximo bloco, marcamos o fim
            pixel[2] = 0; // Canal Red = 0
            pixel[1] = 0; // Canal Green = 0
        }

        // Acompanhar progresso a cada 10%
        if (i % progressStep == 0 && i != 0) {
            cout << "Progresso: " << (i / progressStep) * 10 << "% concluído." << endl;
        }
        
        bit++;
    }

    if (bit < messageBitLength) {
        cerr << "Erro: A imagem não tem bordas suficientes para codificar a mensagem inteira." << endl;
    }
}

// Função para decodificar a mensagem usando FAT
string decodeMessage(const Mat& image, const vector<Point>& edges) {
    string message;
    char currentChar = 0;
    int bit = 0;

    // Começar da primeira borda
    Point currentEdge = edges[0];

    while (true) {
        Vec3b pixel = image.at<Vec3b>(currentEdge);

        // Recuperar o bit do canal azul (LSB)
        currentChar = (currentChar << 1) | (pixel[0] & 1); // Obter o bit LSB
        bit++;

        if (bit % 8 == 0) { // Se completamos um caractere
            if (currentChar == '|') { // Verifica se é o delimitador
                return message; // Retornar a mensagem decodificada
            }
            message += currentChar;
            currentChar = 0; // Resetar para o próximo caractere
        }

        // Verificar o próximo bloco de dados (coordenada x e y)
        int nextX = pixel[2]; // Canal Red (x)
        int nextY = pixel[1]; // Canal Green (y)

        // Verificar se as coordenadas são válidas (dentro dos limites da imagem)
        if (nextX <= 0 && nextY <= 0) {
            break; // Fim da mensagem (último bloco)
        }

        if (nextX < 0 || nextX >= image.cols || nextY < 0 || nextY >= image.rows) {
            cerr << "Erro: Coordenadas fora dos limites da imagem (" << nextX << ", " << nextY << ")." << endl;
            break;
        }

        // Continuar no próximo bloco
        currentEdge = Point(nextX, nextY);
    }

    return message;
}


int main() {
    // Carregar a imagem
    Mat image = imread("cg.png");
    if (image.empty()) {
        cerr << "Erro ao carregar a imagem!" << endl;
        return -1;
    }

    // Aplicar o filtro de Sobel e binarizar
    Mat sobelBin = applySobelBinarization(image);

    // Encontrar todas as bordas (áreas para armazenar dados)
    vector<Point> edges = findEdges(sobelBin);

    // Mensagem a ser escondida
    string mensagem = "Segredo escondido usando Sobel, LSB e FAT";

    // Codificar a mensagem na imagem
    encodeMessage(image, edges, mensagem);
    imwrite("encoded_image.png", image); // Salvar a imagem codificada

    // Decodificar a mensagem da imagem
    Mat encodedImage = imread("encoded_image.png");
    string decodedMessage = decodeMessage(encodedImage, edges);

    cout << "Mensagem decodificada: " << decodedMessage << endl;

    return 0;
}
