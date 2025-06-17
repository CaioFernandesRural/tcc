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
    Mat imgSobel;
    Mat imgGray;

    string outputFileName;

    Processador processador;

protected:
    
    Mat imgBinary;
    Mat imgIn;
    Mat imgOut;
    string inputFileName;
    int ksize;
    int threshold;
    string message;
    ResManager manager;
    Decode decoder;
    Encode encoder;

    void carregarConfiguracao();
    void processarImagem();
    void salvarResultados(string filename);
    void compararImagens(string filename);

public:
    App();
    ~App();

    void run();
    Encode& getEncoder();
};

#endif // APP_H
