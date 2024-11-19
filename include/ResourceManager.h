#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

using namespace std;

// Diretórios base (ajuste conforme sua estrutura de pastas)
const string INPUT_DIR = "resources/input_images/";
const string OUTPUT_DIR = "resources/output_images/";
const string CONFIG_FILE = "resources/config.json";

// Classe ResourceManager com implementação embutida
class ResourceManager {
public:
    // Métodos estáticos para obter os caminhos
    static string getInputImagePath(const string& imageName) {
        return INPUT_DIR + imageName;
    }

    static string getOutputImagePath(const string& imageName) {
        return OUTPUT_DIR + imageName;
    }

    static string getConfigPath() {
        return CONFIG_FILE;
    }
};

#endif // RESOURCE_MANAGER_H
