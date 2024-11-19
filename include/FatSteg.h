#ifndef FATSTEG_H
#define FATSTEG_H

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>         // Para runtime_error
#include <nlohmann/json.hpp>

using namespace cv;
using namespace std;

using json = nlohmann::json;

namespace FSteg
{
    inline bool checaSequencia(const Mat sobelImage, int &startY, int &startX, int blockLen,
                               int &initY, int &initX)
    {
        int contaSequencia = 0;

        for (int i = startY; i < sobelImage.rows; i++)
        {
            for (int j = (i == startY ? startX : 0); j < sobelImage.cols; j++)
            {
                if (sobelImage.at<uchar>(i, j) == 255)
                {
                    // Armazena a posição do início da sequência
                    if (contaSequencia == 0)
                    {
                        initY = i;
                        initX = j;
                    }
                    contaSequencia++;
                }
                else
                {
                    // Reseta a contagem se a sequência for interrompida
                    contaSequencia = 0;
                }

                // Verifica se a sequência foi encontrada
                if (contaSequencia == blockLen)
                {
                    startY = i;
                    startX = j;
                    cout << "\n\n"
                         << "Sequência de " << blockLen << " pixels encontrada.\n";
                    return true;
                }
            }
        }
        return false;
    }

    // Função auxiliar para definir o LSB de um pixel
    inline void setLSB(Mat &image, Point local, int bitValue)
    {
        // pixel é alterado diretamente na imagem
        Vec3b &pixel = image.at<Vec3b>(local.y, local.x);
        // Define o valor do LSB de acordo com o valor de bitValue
        if (bitValue == 1)
        {
            pixel[0] |= 1; // Força o LSB para 1
        }
        else
        {
            pixel[0] &= ~1; // Força o LSB para 0
        } // Altera apenas o LSB do canal azul
        cout << "\n"
             << pixel << "\n";
    }
    
    inline void insertBlock(Mat &image, char carga, Point ptoInicial, Point ptoSeguinte)
    {

        vector<int> bitValues(8);

        int i = ptoInicial.x;
        int j = ptoInicial.y;

        cout << "\n"
             << "Inserindo bloco para o caractere: '" << carga << "'\n";
        cout << "Localização inicial do bloco atual: " << ptoInicial << "\n";
        // cout << "Localização do bloco anterior: " << loc1 << "\n";

        cout << "Bits de carga: ";
        for (int k = 7; k >= 0; --k)
        {
            bitValues[7 - k] = (carga >> k) & 1;
            cout << bitValues[7 - k];
        }
        cout << "\n";

        // Loop para inserir os bits na imagem
        int bitIndex = 0;
        while (bitIndex < 8)
        {
            if (i >= image.cols)
            { // Se atingir o fim da coluna, passa para a próxima linha
                i = 0;
                j++;
            }
            if (j >= image.rows)
            { // Se atingir o fim da linha, para de inserir (fora dos limites da imagem)
                cout << "Erro: imagem muito pequena para inserir todos os bits." << endl;
                return;
            }
            setLSB(image, Point(i, j), bitValues[bitIndex]);
            bitIndex++;
            i++; // Avança para o próximo pixel na coluna
        }
        cout << "\n";

        // declara y converte
        int loc = ptoSeguinte.y * image.cols + ptoSeguinte.x;
        vector<int> locBits(20);

        // se chegar no final não precisa inserir o N
        if (carga == '\0')
        {
            return;
        }

        cout << "\nBits de loc (" << loc << "): ";
        for (int k = 19; k >= 0; --k)
        {
            locBits[19 - k] = (loc >> k) & 1;
            cout << locBits[19 - k]; // Exibe cada bit após calcular
        }
        cout << "\n";

        bitIndex = 0;
        while (bitIndex < 20)
        {
            if (i >= image.cols)
            {
                i = 0;
                j++;
            }
            if (j >= image.rows)
            {
                cout << "Erro: imagem muito pequena para inserir todos os bits." << endl;
                return;
            }
            cout << "Inserindo locBit[" << bitIndex << "] = " << locBits[bitIndex]
                 << " na posição (" << i << ", " << j << ")" << endl;

            setLSB(image, Point(i, j), locBits[bitIndex]);
            bitIndex++;
            i++; // Avança para o próximo pixel na coluna
        }
    }

    inline void encodeMessage(Mat &image, const Mat img_bin, const string message, json conf)
    {
        vector<Point> positions;

        int blockLen = conf["blockLen"];

        string delimitada = message + '\0';

        // coordenadas atuais da sequência de busca de blocos
        int startX = 0;
        int startY = 0;

        for (size_t i = 0; i < delimitada.length(); i++)
        {
            int initX;
            int initY;

            if (startX != 0 || startY != 0)
            {
                startX++;
                if (startX > image.cols)
                {
                    startX = 0;
                    startY++;
                }
                if (startY > image.rows)
                {
                    cerr << "deu pau";
                }
            }

            if (checaSequencia(img_bin, startY, startX, blockLen, initY, initX))
            {
                cout << "Início da sequência: (" << initY << ", " << initX << ")\n";
                cout << "Fim da sequência: (" << startY << ", " << startX << ")\n";

                // vetor de posições inicias dos blocos
                positions.push_back(Point(initX, initY));
            }
            else
            {
                cerr << "\nA imagem não tem blocos o suficiente\n";
                abort();
            }
        }

        // printa o N inicial (para testes)
        cout << "\n N inicial: " << positions[0].y * image.cols + positions[0].x << "\n";

        for (size_t i = 0; i < delimitada.length(); i++)
        {
            char caractere = delimitada[i];
            Point pos = positions[i];
            Point posProx = positions[i + 1];

            cout << "\n"
                 << "Bloco em posição inicial: (" << pos.x << ", " << pos.y << ")\n";
            insertBlock(image, caractere, pos, posProx);
        }
    }

    // Função para extrair o LSB de um pixel em uma imagem
    inline int extractLSB(const Vec3b pixel)
    {
        return pixel[0] % 2; // Retorna o LSB do canal azul
    }

    inline char decodeBloco(const Mat image, int &N)
    {
        int y = N / image.cols;
        int x = N % image.cols;

        char caractere = 0;
        int newN = 0;

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

            Vec3b pixel = image.at<Vec3b>(y, x);
            int lsb = extractLSB(pixel);

            caractere = (caractere << 1) | lsb;

            // Debugging: print the values at each step
            cout << "[Caractere] Bit " << i << ": LSB = " << lsb
                 << ", Caractere (parcial) = " << caractere << endl;

            ++x;
        }

        if (caractere == '\0')
        {
            return caractere;
        }

        for (int i = 8; i < 28; ++i)
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

            Vec3b pixel = image.at<Vec3b>(y, x);
            int lsb = extractLSB(pixel);

            newN = (newN << 1) | lsb;

            // Debugging: print the values at each step
            cout << "[newN] Bit " << i << ": LSB = " << lsb
                 << ", newN (parcial) = " << newN << endl;

            ++x;
        }

        if (newN == 0)
        {
            throw runtime_error("newN decodificado como 0, o que causa loop infinito.");
        }

        N = newN;
        return caractere;
    }

    inline string decodeImagem(const Mat image, int inicialN)
    {
        string mensagem;
        int N = inicialN;
        char caractere;

        do
        {
            cout << caractere;
            caractere = decodeBloco(image, N);

            mensagem += caractere;

        } while (caractere != '\0');

        return mensagem;
    }

};

#endif