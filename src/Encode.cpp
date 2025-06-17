#include "../include/Encode.hpp"

Encode::Encode() {}

Encode::~Encode() {}

int Encode::getNinicial() const {
    return nInicial;
}

bool Encode::checaSequencia(const Mat sobelImage, Point &ptoInicio, int blockLen, Point &ptoTrabalho)

{
    int contaSequencia = 0;

    for (int i = ptoInicio.y; i < sobelImage.rows; i++)
    {
        for (int j = (i == ptoInicio.y ? ptoInicio.x : 0); j < sobelImage.cols; j++)
        {
            if (sobelImage.at<uchar>(i, j) == 255)
            {
                // Armazena a posição do início da sequência
                if (contaSequencia == 0)
                {
                    ptoTrabalho.y = i;
                    ptoTrabalho.x = j;
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
                ptoInicio.x = j;
                ptoInicio.y = i;
                // Debugging:
                //cout << "\nSequência de " << blockLen << " pixels encontrada.\n";
                return true;
            }
        }
    }
    return false;
}

// Função auxiliar para definir o LSB de um pixel
void Encode::setLSB(Mat &image, Point local, int bitValue)
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
    // Debugging:
    //cout << "\n"
    //     << pixel << "\n";
}

void Encode::insertBlock(Mat &image, char carga, Point ptoInicial, Point ptoSeguinte)
{

    vector<int> bitValues(8);

    int i = ptoInicial.x;
    int j = ptoInicial.y;

    // Debugging:
    //cout << "\n"
    //     << "Inserindo bloco para o caractere: '" << carga << "'\n";
    //cout << "Localização inicial do bloco atual: " << ptoInicial << "\n";
    //cout << "Localização do bloco anterior: " << loc1 << "\n";

    //cout << "Bits de carga: ";

    for (int k = 7; k >= 0; --k)
    {
        bitValues[7 - k] = (carga >> k) & 1;
        // Debugging:
        //cout << bitValues[7 - k];
    }
    //cout << "\n";

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
    //cout << "\n";

    // Calcula loc linearizado
    int loc = ptoSeguinte.y * image.cols + ptoSeguinte.x;

    // Calcula quantos bits são necessários para representar loc
    int total_pixels = image.rows * image.cols;
    int bits_loc = std::floor(std::log2(total_pixels - 1)) + 1;

    vector<int> locBits(bits_loc);

    // se chegar no final não precisa inserir o N
    if (carga == '\0')
    {
        return;
    }
    // Debugging:
    //cout << "\nBits de loc (" << loc << "): ";

    for (int k = bits_loc - 1; k >= 0; --k)
    {
        locBits[bits_loc - 1 - k] = (loc >> k) & 1;
        // Debugging:
        //cout << locBits[bits_loc - 1 - k]; // Exibe cada bit após calcular
    }
    //cout << "\n";

    bitIndex = 0;
    while (bitIndex < bits_loc)
    {
        if (i >= image.cols)
        {
            i = 0;
            j++;
        }
        if (j >= image.rows)
        {
            //cout << "Erro: imagem muito pequena para inserir todos os bits." << endl;
            return;
        }
        else
        {
            // Debugging:
            //cout << "Inserindo locBit[" << bitIndex << "] = " << locBits[bitIndex]
            //     << " na posição (" << i << ", " << j << ")" << endl;

            setLSB(image, Point(i, j), locBits[bitIndex]);
            bitIndex++;
            i++; // Avança para o próximo pixel na coluna
        }
    }
}

void Encode::encodeMessage(Mat &image, const Mat img_bin, const string message, json conf)
{
    vector<Point> positions;

    //int blockLen = conf["blockLen"];

    //calcular o maior N necessário
    int total_pixels = image.rows * image.cols;
    int bits_N = std::floor(std::log2(total_pixels - 1)) + 1;
    int blockLen = bits_N + 8;


    string delimitada = message + '\0';

    // coordenadas atuais da sequência de busca de blocos
    Point ptoInicio;
    ptoInicio.x = 0;
    ptoInicio.y = 0;

    for (size_t i = 0; i < delimitada.length(); i++)
    {
        Point ptoTrabalho;
        ptoTrabalho.x = 0;
        ptoTrabalho.y = 0;

        if (ptoInicio.x != 0 || ptoInicio.y != 0)
        {
            ptoInicio.x++;
            if (ptoInicio.x > image.cols)
            {
                ptoInicio.x = 0;
                ptoInicio.y++;
            }
            if (ptoInicio.y > image.rows)
            {
                cerr << "deu pau";
            }
        }

        if (checaSequencia(img_bin, ptoInicio, blockLen, ptoTrabalho))
        {
            // Debugging:
            //cout << "Início da sequência: (" << ptoTrabalho.y << ", " << ptoTrabalho.x << ") Fim da sequência: (" << ptoInicio.y << ", " << ptoInicio.x << ")\n";

            // vetor de posições inicias dos blocos
            positions.push_back(ptoTrabalho);
        }
        else
        {
            throw std::runtime_error("Imagem não tem blocos o suficiente.");
        }
    }
    // printa o N inicial (para testes)
    //cout << "\n N inicial: " << positions[0].y * image.cols + positions[0].x << "\n";
    nInicial = positions[0].y * image.cols + positions[0].x;

    for (size_t i = 0; i < delimitada.length(); i++)
    {
        char caractere = delimitada[i];
        Point pos = positions[i];
        Point posProx;

        if (i < positions.size() - 1)
            posProx = positions[i + 1];
        else
            posProx = Point(-1, -1);
        // Debugging:
        //cout << "\n"
        //     << "Bloco em posição inicial: (" << pos.x << ", " << pos.y << ")\n";
        insertBlock(image, caractere, pos, posProx);
    }
}