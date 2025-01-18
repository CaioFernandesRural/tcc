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
    Mat imgIn;
    Mat imgSobel;
    Mat imgGray;
    Mat imgBinary;

    string outputFileName;

    Processador processador;
    Encode encoder;

protected:
    string inputFileName;
    int ksize;
    int threshold;
    string message;
    Mat imgOut;
    ResManager manager;
    Decode decoder;

    void carregarConfiguracao();
    void processarImagem();
    void salvarResultados(string filename);
    void compararImagens(string filename);

public:
    App();
    ~App();

    void run();
};

#endif // APP_H
