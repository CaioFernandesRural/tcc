#include "../include/App.hpp"
#include <iostream>
#include <stdexcept>

App::App() : ksize(0), threshold(0) {}

App::~App() {}

void App::carregarConfiguracao()
{
    try
    {
        json conf = manager.lerConfig(manager.getConfigPath());
        inputFileName = manager.getInputImagePath(conf["nome"]);
        ksize = conf["ksize"];
        threshold = conf["threshold"];
        outputFileName = manager.getOutputImagePath(conf["nome_out"]);
        message = conf["mensagem"];
    }
    catch (const runtime_error &e)
    {
        cerr << "Erro ao carregar a configuração: " << e.what() << endl;
        throw;
    }
}

void App::processarImagem()
{
    imgIn = imread(inputFileName);
    if (imgIn.empty())
    {
        throw runtime_error("Erro: Não foi possível abrir a imagem: " + inputFileName);
    }

    imgGray = processador.converteCinza(imgIn);

    if (ksize < 3 || ksize > 31 || ksize % 2 == 0)
    {
        throw runtime_error("Valor inválido! O kernel deve ser ímpar entre 3 e 31.");
    }

    imgSobel = processador.aplicaSobel(imgGray, ksize);

    imgBinary = processador.binarizaImagem(imgSobel, threshold);

    encoder.encodeMessage(imgIn, imgBinary, message, manager.lerConfig(manager.getConfigPath()));
}

void App::salvarResultados()
{
    imwrite(manager.getOutputImagePath("sobel_original.png"), imgSobel);
    imwrite(manager.getOutputImagePath("imagem_binarizada.png"), imgBinary);
    imwrite(outputFileName, imgIn);
}

void App::compararImagens()
{
    Mat diffImage = Mat::zeros(imgIn.size(), CV_8UC1);

    for (int i = 0; i < imgIn.rows; ++i)
    {
        for (int j = 0; j < imgIn.cols; ++j)
        {
            cout << "LSB do pixel [0, 0] da original: " << (imgIn.at<Vec3b>(0, 0)[0] & 0x01) << endl;
            cout << "LSB do pixel [0, 0] da modificada: " << (imgOut.at<Vec3b>(0, 0)[0] & 0x01) << endl;

        
            uchar originalBit = imgIn.at<Vec3b>(i, j)[0] & 0x01;
            uchar encodedBit = imgIn.at<Vec3b>(i, j)[0] & 0x01;
            diffImage.at<uchar>(i, j) = (originalBit != encodedBit) ? 255 : 0;
        }
    }

    imwrite(manager.getOutputImagePath("imagem_diferenca.png"), diffImage);
}

void App::run()
{
    try
    {
        carregarConfiguracao();
        processarImagem();
        salvarResultados();
        compararImagens();

        int nInicial = manager.lerConfig(manager.getConfigPath())["Ninicial"];
        string recoveredMessage = decoder.decodeImagem(imgIn, nInicial);
        cout << "Mensagem decodificada: " << recoveredMessage << endl;

    }
    catch (const exception &e)
    {
        cerr << "Erro durante a execução: " << e.what() << endl;
    }
}
