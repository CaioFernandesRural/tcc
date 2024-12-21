#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

class ResManager
{

private:
private:
    string inputDir;
    string outputDir;
    string configFile;

public:
    ResManager(const string &inputDir = "resources/input_images/",
               const string &outputDir = "resources/output_images/",
               const string &configFile = "resources/config.json");

    ~ResManager() {}

    // Métodos para obter os caminhos
    string getInputImagePath(const string imageName);
    string getOutputImagePath(const string imageName);
    string getConfigPath();
    json lerConfig(const string caminhoArquivo);
};

#endif // MANAGER_H
