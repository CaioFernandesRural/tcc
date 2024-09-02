#include <iostream>
#include <string>
#include <bitset>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(void)
{
    char nome[100], nome_out[100];
    Mat img, img_sobel, img_gray;
    int ksize = 0;

    cout << "Digite o nome da imagem: ";
    cin >> nome;

    // Carrega a imagem original
    Mat imgO = imread(nome);
    if (imgO.empty())
    {
        cout << "Erro ao abrir a imagem: " << nome << " Nao existe" << endl;
        exit(-1);
    }

    // Converte para escala de cinza
    cvtColor(imgO, img_gray, COLOR_BGR2GRAY);

    cout << "X =" << img_gray.rows << " Y = " << img_gray.cols << " Depth = " << img_gray.depth() << " Channels = " << img_gray.channels() << endl;

    // Define o tamanho do kernel do Sobel
    do
    {
        cout << "Digite o tamanho do kernel do Sobel: ";
        cin >> ksize;
    } while (ksize < 3 || ksize > 31);

    // Aplica o filtro Sobel para detectar bordas
    Sobel(img_gray, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    // Salvar a imagem Sobel
    imwrite("imagem_sobel.png", img_sobel);

    cout << "Digite o nome da imagem de saida: ";
    cin >> nome_out;

    // Solicita a mensagem do usuário
    string message;
    cout << "Digite a mensagem para inserir na imagem: ";
    cin.ignore(); // Limpa o buffer do cin
    getline(cin, message); // Lê a mensagem completa, incluindo espaços

    // Converter a mensagem para bits
    vector<int> messageBits;
    for (char c : message)
    {
        bitset<8> bits(c);
        for (int i = 7; i >= 0; --i)
        {
            messageBits.push_back(bits[i]);
        }
    }

    // Inserir a mensagem apenas onde o Sobel detectou bordas
    int bitIndex = 0;
    Mat imgWithMessage = imgO.clone();
    for (int i = 0; i < img_sobel.rows; ++i)
    {
        for (int j = 0; j < img_sobel.cols; ++j)
        {
            uchar sobelValue = img_sobel.at<uchar>(i, j);

            // Se o pixel é uma borda (valor de Sobel maior que um limiar, como 0)
            if (sobelValue > 0 && bitIndex < messageBits.size())
            {
                Vec3b& pixel = imgWithMessage.at<Vec3b>(i, j);
                // Alterar o último bit do primeiro canal (azul)
                pixel[0] = (pixel[0] & ~1) | messageBits[bitIndex];
                bitIndex++;
            }
        }
    }

    // Salvar a imagem com a mensagem inserida
    imwrite(nome_out, imgWithMessage);

    // Comparar a imagem original com a imagem com mensagem para visualizar a diferença
    Mat diffImage = Mat::zeros(imgO.size(), CV_8UC1); // Imagem de comparação inicializada como preta

    for (int i = 0; i < imgO.rows; ++i)
    {
        for (int j = 0; j < imgO.cols; ++j)
        {
            uchar originalBit = imgO.at<Vec3b>(i, j)[0] & 1; // Último bit da imagem original
            uchar messageBit = imgWithMessage.at<Vec3b>(i, j)[0] & 1; // Último bit da imagem com mensagem

            // Se o bit for diferente, coloca um ponto branco; se for igual, ponto preto
            diffImage.at<uchar>(i, j) = (originalBit != messageBit) ? 255 : 0;
        }
    }

    // Salvar e exibir a imagem de comparação
    imwrite("imagem_diferenca.png", diffImage);
    imshow("Diferenca entre imagens", diffImage);
    waitKey(0);

    return 0;
}
