#ifndef IMAGESTEST_H
#define IMAGESTEST_H

#include "../include/App.hpp"

#include "../include/PSNRCalculator.hpp"
#include "../include/SSIMCalculator.hpp"
#include "../include/FSIMCalculator.hpp"
#include "../include/HistogramAnalyser.hpp"
#include "../include/ChiSquareAnalyser.hpp"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = filesystem;


class ImagesTest : public App
{
private:
    map<string, int> getNomes(string folderPath);
    void carregarConfiguracao(string filename);

public:
    ImagesTest();
    ~ImagesTest();

    void run();
};

#endif // IMAGESTEST_H
