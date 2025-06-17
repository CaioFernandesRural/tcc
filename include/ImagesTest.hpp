#ifndef IMAGESTEST_H
#define IMAGESTEST_H

#include "../include/App.hpp"

#include "../include/PSNRCalculator.hpp"
#include "../include/SSIMCalculator.hpp"
#include "../include/FSIMCalculator.hpp"
#include "../include/HistogramAnalyser.hpp"
#include "../include/ChiSquareAnalyser.hpp"
#include "../include/RSAnalyser.hpp"
#include "../include/BERAnalyser.hpp"
#include "../include/MetricsLogger.hpp"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <map>

using namespace std;
namespace fs = filesystem;

class ImagesTest : public App
{
private:
    map<string, int> getNomes(string folderPath);
    void carregarConfiguracao(string filename);

    // Variáveis de métricas (visíveis para outros métodos, se quiser)
    double psnr;
    double ssim;
    double fsim;
    double histSim;
    double chi2;
    double rsDiff;
    int blockCapacity;
    double bpp;
    double ber;
    string recoveredMessage;

public:
    ImagesTest();
    ~ImagesTest();

    void run();
};

#endif // IMAGESTEST_H
