#include "../include/App.hpp"
#include <iostream>
#include <stdexcept>

App::App() : ksize(0), threshold(0) {}

App::~App() {}

Encode& App::getEncoder() {
    return encoder;
}

void App::carregarConfiguracao()
{
    try
    {
        json conf = manager.lerConfig(manager.getConfigPath());
        inputFileName = manager.getInputImagePath(conf["nome"]);
        ksize = conf["ksize"];
        threshold = conf["threshold"];
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

    imgBinary = imgSobel.clone();
    imgBinary = processador.binarizaImagem(imgBinary, threshold);

    imgOut = imgIn.clone(); // Cria uma cópia de imgIn para receber a mensagem

    int blockCapacity = CapacityAnalyser::analyze(imgBinary);
    message = string(blockCapacity - 1, 'a');

    encoder.encodeMessage(imgOut, imgBinary, message, manager.lerConfig(manager.getConfigPath()));
}

void App::salvarResultados(string filename)
{
    imwrite(manager.getOutputImagePath(filename + "_sobel_original.png"), imgSobel);
    imwrite(manager.getOutputImagePath(filename + "_imagem_binarizada.png"), imgBinary);
    imwrite(manager.getOutputImagePath(filename + "_teste.png"), imgOut);
}

void App::compararImagens(string filename)
{
    Mat diffImage = Mat::zeros(imgIn.size(), CV_8UC1);

    for (int i = 0; i < imgIn.rows; ++i)
    {
        for (int j = 0; j < imgIn.cols; ++j)
        {
            uchar originalBit = imgIn.at<Vec3b>(i, j)[0] & 0x01;
            uchar encodedBit = imgOut.at<Vec3b>(i, j)[0] & 0x01; // Comparar com a imagem codificada
            diffImage.at<uchar>(i, j) = (originalBit != encodedBit) ? 255 : 0;
        }
    }

    imwrite(manager.getOutputImagePath(filename + "imagem_diferenca.png"), diffImage);
}


void App::run()
{
    try
    {
        carregarConfiguracao();
        processarImagem();
        salvarResultados(manager.lerConfig(manager.getConfigPath())["nome"]);
        compararImagens("comparada");

        int nInicial = manager.lerConfig(manager.getConfigPath())["Ninicial"];
        string recoveredMessage = decoder.decodeImagem(imgOut, nInicial);
        cout << "Mensagem decodificada: " << recoveredMessage << endl;

    }
    catch (const exception &e)
    {
        cerr << "Erro durante a execução: " << e.what() << endl;
    }
}
