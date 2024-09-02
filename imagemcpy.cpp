#include <iostream>
#include <string>
#include <bitset>  // Necessário para a conversão de bits
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(void);

int main(void)
{
    char nome[100], nome_out[100];
    Mat img, img_sobel, imgO;
    int ksize = 0;

    cout << "Digite o nome da imagem: ";
    cin >> nome;

    imgO = imread(nome);
    if (imgO.empty())
    {
        cout << "Erro abertura de imagem: " << nome << " Nao existe" << endl;
        exit(-1);
    }
    img = imread(nome, 0);
    if (img.empty())
    {
        cout << "Erro abertura de imagem: " << nome << " Nao existe" << endl;
        exit(-1);
    }

    cout << "X =" << img.rows << " Y = " << img.cols << " Depth = " << img.depth() << " Channels = " << img.channels() << endl;

    /*
     * A funcao Sobel, o kernel deve ser um valor impar entre 3 e 31
     */
    do
    {
        cout << "Digite o tamanho do kernel do sobel: ";
        cin >> ksize;
    } while (ksize < 3 || ksize > 31);

    Sobel(img, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

    cout << "Digite o nome da imagem de saida: ";
    cin >> nome_out;

    cout << "Pixel = " << imgO.at<Vec3b>(0, 0) << endl;
    imshow("Original", imgO);
    imshow("P&B", img);
    imshow("Sobel", img_sobel);
    waitKey(0);

    imwrite(nome_out, img_sobel);

    // Solicitar a mensagem do usuário
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

    // Inserir a mensagem na imagem original
    int bitIndex = 0;
    for (int i = 0; i < imgO.rows; ++i)
    {
        for (int j = 0; j < imgO.cols; ++j)
        {
            if (bitIndex < messageBits.size())
            {
                Vec3b& pixel = imgO.at<Vec3b>(i, j);
                // Alterar o último bit do primeiro canal (azul)
                pixel[0] = (pixel[0] & ~1) | messageBits[bitIndex];
                bitIndex++;
            }
        }
    }

    // Salvar a imagem com a mensagem inserida
    imwrite("imagem_com_mensagem.png", imgO);

    // Verificar a mensagem inserida na imagem
    string recoveredMessage;
    bitIndex = 0;
    char currentChar = 0;

    for (int i = 0; i < imgO.rows; ++i)
    {
        for (int j = 0; j < imgO.cols; ++j)
        {
            if (bitIndex < messageBits.size())
            {
                Vec3b pixel = imgO.at<Vec3b>(i, j);
                int bit = pixel[0] & 1;  // Recupera o último bit
                currentChar |= (bit << (7 - (bitIndex % 8)));
                bitIndex++;

                if (bitIndex % 8 == 0)
                {
                    recoveredMessage += currentChar;
                    currentChar = 0;
                }
            }
        }
    }

    cout << "Mensagem recuperada: " << recoveredMessage << endl;

    return 0;
}
