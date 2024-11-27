#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

// Diretórios base (ajuste conforme sua estrutura de pastas)
const string INPUT_DIR = "resources/input_images/";
const string OUTPUT_DIR = "resources/output_images/";
const string CONFIG_FILE = "resources/config.json";

namespace ResManager {

    // Métodos para obter os caminhos
    inline string getInputImagePath(const string imageName) {
        return INPUT_DIR + imageName;
    }

    inline string getOutputImagePath(const string imageName) {
        return OUTPUT_DIR + imageName;
    }

    inline string getConfigPath() {
        return CONFIG_FILE;
    }

    inline json lerConfig(const string caminhoArquivo)
    {
        ifstream arquivo(caminhoArquivo);
        if (!arquivo.is_open())
        {
            throw runtime_error("Erro ao abrir o arquivo config.json");
        }
        json config;
        arquivo >> config;
        arquivo.close();
        return config;
    }
};

#endif // RESOURCE_MANAGER_H
