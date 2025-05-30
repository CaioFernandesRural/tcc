#ifndef IMAGESTEST_H
#define IMAGESTEST_H

#include "../include/App.hpp"

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
