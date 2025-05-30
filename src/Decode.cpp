#include "../include/Decode.hpp"

// Decode::Decode() {}

// Decode::~Decode() {}

// Função para extrair o LSB de um pixel em uma imagem
int Decode::extractLSB(const Vec3b pixel)
{
    return pixel[0] % 2; // Retorna o LSB do canal azul
}

char Decode::decodeBloco(const Mat image, int &N)
{
    if (image.empty()) {
        throw runtime_error("Imagem não carregada corretamente.");
    }

    int y = N / image.cols;
    int x = N % image.cols;

    char caractere = 0;
    int newN = 0;

    // Primeiro: calcula dinamicamente o blockLen = bits_N + 8
    int total_pixels = image.rows * image.cols;
    int bits_N = std::floor(std::log2(total_pixels - 1)) + 1;
    int blockLen = bits_N + 8;

    for (int i = 0; i < 8; ++i)
    {
        if (x >= image.cols)
        {
            x = 0;
            ++y;
        }
        if (y >= image.rows)
        {
            throw runtime_error("Fim da imagem atingido durante a decodificação.");
        }
        // Debugging:
        //cout << "x: " << x << ", y: " << y << ", N: " << N << endl;
        Vec3b pixel = image.at<Vec3b>(y, x);
        int lsb = extractLSB(pixel);

        caractere = (caractere << 1) | lsb;

        // Debugging: print the values at each step
        //cout << "[Caractere] Bit " << i << ": LSB = " << lsb
        //     << ", Caractere (parcial) = " << caractere << endl;

        ++x;
    }

    if (caractere == '\0')
    {
        return caractere;
    }

    for (int i = 8; i < blockLen; ++i)
    {
        if (x >= image.cols)
        {
            x = 0;
            ++y;
        }
        if (y >= image.rows)
        {
            throw runtime_error("Fim da imagem atingido durante a decodificação.");
        }
        // Debugging:
        //cout << "x: " << x << ", y: " << y << ", N: " << N << endl;
        Vec3b pixel = image.at<Vec3b>(y, x);
        int lsb = extractLSB(pixel);

        newN = (newN << 1) | lsb;

        // Debugging:
        //cout << "[newN] Bit " << i << ": LSB = " << lsb
        //     << ", newN (parcial) = " << newN << endl;

        ++x;
    }

    if (newN == 0)
    {
        throw runtime_error("newN decodificado como 0, o que causa loop infinito.");
    }

    N = newN;
    return caractere;
}

string Decode::decodeImagem(const Mat image, int inicialN)
{
    string mensagem;
    int N = inicialN;
    char caractere;

    do
    {
        caractere = decodeBloco(image, N);
        // Debugging:
        //cout << caractere;

        mensagem += caractere;

    } while (caractere != '\0');

    return mensagem;
}
