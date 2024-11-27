#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <nlohmann/json.hpp> // Biblioteca para manipulação de JSON
#include <stdexcept>         // Para runtime_error
#include "../include/ResourceManager.h"
#include "../include/ProcessadorImagem.h"
#include "../include/FatSteg.h"

using namespace cv;
using namespace std;

using json = nlohmann::json;

using namespace ResManager;
using namespace ImgProc;
using namespace FSteg;

class App
{
private:

		  string fn;
		  Mat		imgIn;
		  Mat		imgOut;
		  Mat		imgSobel;
		  Mat		imgGray;
		  Mat		imgBinary;

		  int ksize;

public:
    
		  App(){}
		  App(string arquivo) {}

		  ~App(){}


    void run()
    {
        int ksize = 0, N;
        Mat img, img_sobel, img_gray, imgOri, img_Bin;
        string nome, nome_out, mensagem;

        json conf = lerConfig("resources/config.json");
        nome = getInputImagePath(conf["nome"]);
        ksize = conf["ksize"];

        // cout << "Digite o nome da imagem original: ";
        // cin >> nome;
        // comentei pra não ter que ficar digitando toda hora

        // Carregar a imagem original
        imgOri = imread(nome);

        if (imgOri.empty())
        { // Verificar se a imagem foi carregada
            cerr << "Erro: Não foi possível abrir a imagem: " << nome << endl;
            return; // Encerrar o programa com código de erro
        }

        // Converter para escala de cinza
        img_gray = converteCinza(imgOri);

        // Solicitar o tamanho do kernel do Sobel
        try
        {
            // cout << "Digite o tamanho do kernel do Sobel (ímpar entre 3 e 31): ";
            // cin >> ksize;

            if (ksize < 3 || ksize > 31 || (ksize % 2 == 0))
            {
                throw runtime_error("Valor inválido! O kernel deve ser ímpar entre 3 e 31.");
            }
        }
        catch (const runtime_error &e)
        {
            cerr << "Erro: " << e.what() << endl;
            return; // Encerrar o programa com código de erro
        }

        // Aplicar o filtro Sobel para detectar bordas
        img_sobel = aplicaSobel(img_gray, ksize);

        // Salvar a imagem Sobel original antes da binarização

        imwrite(getOutputImagePath("sobel_original.png"), img_sobel);

        int threshold;

        // cout << "Digite o valor do threshold (0 a 255): ";
        // cin >> threshold;
        threshold = conf["threshold"];

        // Recebe endereço da img_sobel

        img_Bin = binarizaImagem(img_sobel, threshold);
        // Salvar a imagem Sobel binarizada
        imwrite(getOutputImagePath("imagem_binarizada.png"), img_Bin);

        nome_out = conf["nome_out"];
        // cin.ignore();
        // cout << "Digite o nome da imagem de saida: ";
        // cin >> nome_out;

        mensagem = conf["mensagem"];

        // Codificar a mensagem na imagem
        Mat imgWithMessage = imgOri.clone();
        encodeMessage(imgWithMessage, img_Bin, mensagem, conf);

        // Salvar a imagem com a mensagem inserida
        imwrite(nome_out, imgWithMessage);

        // Decodificar a mensagem no final

        N = conf["Ninicial"];

        // cin.ignore();
        // cout << "Digite o valor de N do primeiro bloco: ";
        // cin >> N;
        string recoveredMessage = decodeImagem(imgWithMessage, N);

        // // Mostrar a mensagem recuperada
        cout << "Mensagem decodificada: " << recoveredMessage << endl;

        // Comparar a imagem original com a imagem com mensagem para visualizar a diferença
        Mat diffImage = Mat::zeros(imgOri.size(), CV_8UC1); // Imagem de comparação inicializada como preta

        for (int i = 0; i < imgOri.rows; ++i)
        {
            for (int j = 0; j < imgOri.cols; ++j)
            {
                uchar originalBit = imgOri.at<Vec3b>(i, j)[0] & 0x01;        // Último bit da imagem original
                uchar messageBit = imgWithMessage.at<Vec3b>(i, j)[0] & 0x01; // Último bit da imagem com mensagem

                // Se o bit for diferente, coloca um ponto branco; se for igual, ponto preto
                diffImage.at<uchar>(i, j) = (originalBit != messageBit) ? 255 : 0;
            }
        }
        imwrite(getOutputImagePath("imagem_diferenca.png"), diffImage);
    }
};

int main()
{
    App app;
    app.run();
    return 0;
}
