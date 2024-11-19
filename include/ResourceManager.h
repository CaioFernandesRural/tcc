#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

class ResourceManager {
public:
    // Métodos para obter os caminhos
    static std::string getInputImagePath(const std::string& imageName);
    static std::string getOutputImagePath(const std::string& imageName);
    static std::string getConfigPath();
};

#endif // RESOURCE_MANAGER_H
