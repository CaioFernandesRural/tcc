#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

// Função para decodificar a mensagem da imagem com delimitador
string decodeMessage(const Mat &image, const Mat &binaryImage) {
    string message;
    int bit = 0;

    for (int i = 0; i < binaryImage.rows; ++i) {
        for (int j = 0; j < binaryImage.cols; ++j) {
            uchar binaryValue = binaryImage.at<uchar>(i, j);

            if (binaryValue > 0) { // Verifica se o pixel faz parte de uma borda (imagem binarizada)
                Vec3b pixel = image.at<Vec3b>(i, j);

                if (bit % 8 == 0) {
                    message += '\0';  // Adiciona um novo caractere à string
                }

                int charIndex = bit / 8;
                int bitIndex = bit % 8;
                bool bitValue = pixel[0] & 1;  // Obtém o último bit do canal azul

                message[charIndex] |= (bitValue << (7 - bitIndex));  // Insere o bit no caractere atual
                ++bit;

                // Verifica o final da mensagem
                if (bit % 8 == 0 && message[charIndex] == '\0') {
                    return message.substr(0, charIndex);  // Retorna a mensagem completa
                }
            }
        }
    }

    return message; // Retorna a mensagem lida (caso não tenha delimitador)
}

// Espaço reservado para função de leitura de metadados
void readMetadataParameters() {
    cout << "Função de leitura de metadados ainda não implementada." << endl;
    // Implementar a leitura de metadados da imagem no futuro
}

int main() {
    char nome[100];
    int escolha, escolhaFiltro, thresholdValue, faixaMin, faixaMax, ksize;

    // Solicitar o nome da imagem com a mensagem
    cout << "Digite o nome da imagem com a mensagem inserida: ";
    cin >> nome;

    // Carregar a imagem com a mensagem inserida
    Mat imgWithMessage = imread(nome);
    if (imgWithMessage.empty()) {
        cout << "Erro ao abrir a imagem: " << nome << " Não existe." << endl;
        return -1;
    }

    // Oferecer escolha ao usuário para definir parâmetros
    cout << "Escolha uma opção para configurar o filtro Sobel:" << endl;
    cout << "1. Ler parâmetros dos metadados da imagem" << endl;
    cout << "2. Inserir parâmetros manualmente" << endl;
    cout << "Digite sua escolha (1 ou 2): ";
    cin >> escolha;

    Mat img_gray, img_sobel, img_binarizada;

    if (escolha == 1) {
        // Ler os metadados (implementação futura)
        readMetadataParameters();

    } else if (escolha == 2) {
        // Converter a imagem para escala de cinza
        cvtColor(imgWithMessage, img_gray, COLOR_BGR2GRAY);

        // Inserir manualmente o tamanho do kernel para o Sobel
        do {
            cout << "Digite o tamanho do kernel para o Sobel (ímpar, entre 3 e 31): ";
            cin >> ksize;
        } while (ksize < 3 || ksize > 31 || ksize % 2 == 0);

        // Aplicar o filtro Sobel
        Sobel(img_gray, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

        // Oferecer ao usuário a escolha entre threshold ou faixa de valores
        cout << "Escolha o método de binarização:" << endl;
        cout << "1. Usar Threshold" << endl;
        cout << "2. Usar Faixa de Valores" << endl;
        cout << "Digite sua escolha (1 ou 2): ";
        cin >> escolhaFiltro;

        if (escolhaFiltro == 1) {
            // Aplicar Threshold
            cout << "Digite o valor do Threshold (0 a 255): ";
            cin >> thresholdValue;
            threshold(img_sobel, img_binarizada, thresholdValue, 255, THRESH_BINARY);

        } else if (escolhaFiltro == 2) {
            // Aplicar Faixa de Valores
            cout << "Digite o valor mínimo da faixa: ";
            cin >> faixaMin;
            cout << "Digite o valor máximo da faixa: ";
            cin >> faixaMax;

            // Criar a máscara binária com base na faixa de valores
            inRange(img_sobel, faixaMin, faixaMax, img_binarizada);

        } else {
            cout << "Escolha de método de binarização inválida!" << endl;
            return -1;
        }

    } else {
        cout << "Escolha inválida!" << endl;
        return -1;
    }

    // Salvar a imagem binarizada (apenas para conferência)
    imwrite("imagem_binarizada_decode.png", img_binarizada);  // Corrigido o nome do arquivo

    // Decodificar a mensagem da imagem gerada e binarizada
    string recoveredMessage = decodeMessage(imgWithMessage, img_binarizada);

    // Mostrar a mensagem decodificada
    cout << "Mensagem decodificada: " << recoveredMessage << endl;

    return 0;
}
