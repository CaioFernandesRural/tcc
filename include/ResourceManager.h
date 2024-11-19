#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

// Diretórios base (ajuste conforme sua estrutura de pastas)
const std::string INPUT_DIR = "resources/input_images/";
const std::string OUTPUT_DIR = "resources/output_images/";
const std::string CONFIG_FILE = "resources/config.json";

// Classe ResourceManager com implementação embutida
class ResourceManager {
public:
    // Métodos estáticos para obter os caminhos
    static std::string getInputImagePath(const std::string& imageName) {
        return INPUT_DIR + imageName;
    }

    static std::string getOutputImagePath(const std::string& imageName) {
        return OUTPUT_DIR + imageName;
    }

    static std::string getConfigPath() {
        return CONFIG_FILE;
    }
};

#endif // RESOURCE_MANAGER_H
