#include "../include/ResourceManager.h"
#include <iostream>

int main() {
    // Exemplo de como usar os métodos da classe ResourceManager
    std::string inputPath = ResourceManager::getInputImagePath("image1.jpg");
    std::string outputPath = ResourceManager::getOutputImagePath("image1_processed.jpg");
    std::string configPath = ResourceManager::getConfigPath();

    // Agora você pode usar essas variáveis conforme necessário
    // Exemplo:
    std::cout << "Input Path: " << inputPath << std::endl;
    std::cout << "Output Path: " << outputPath << std::endl;
    std::cout << "Config Path: " << configPath << std::endl;

    return 0;
}

