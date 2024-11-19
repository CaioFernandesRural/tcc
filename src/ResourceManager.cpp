#include "include/ResourceManager.h"

// Diretórios base (ajuste conforme sua estrutura de pastas)
const std::string INPUT_DIR = "resources/input_images/";
const std::string OUTPUT_DIR = "resources/output_images/";
const std::string CONFIG_FILE = "resources/config.json";

// Implementação dos métodos
std::string ResourceManager::getInputImagePath(const std::string& imageName) {
    return INPUT_DIR + imageName;
}

std::string ResourceManager::getOutputImagePath(const std::string& imageName) {
    return OUTPUT_DIR + imageName;
}

std::string ResourceManager::getConfigPath() {
    return CONFIG_FILE;
}
