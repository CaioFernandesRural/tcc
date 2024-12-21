#ifndef APP_H
#define APP_H

#include <string>
#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>
#include "../include/ResManager.hpp"
#include "../include/Processador.hpp"
#include "../include/Encode.hpp"
#include "../include/Decode.hpp"

using namespace std;
using namespace cv;
using json = nlohmann::json;

class App
{
private:
    string inputFileName;
    Mat imgIn;
    Mat imgOut;
    Mat imgSobel;
    Mat imgGray;
    Mat imgBinary;

    int ksize;
    int threshold;
    string outputFileName;
    string message;

    ResManager manager;
    Processador processador;
    Encode encoder;
    Decode decoder;

    void carregarConfiguracao();
    void processarImagem();
    void salvarResultados();
    void compararImagens();

public:
    App();
    ~App();

    void run();
};

#endif // APP_H
