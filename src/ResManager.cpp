#include "../include/ResManager.hpp"

ResManager::ResManager(const string &inputDir, const string &outputDir, const string &configFile)
    : inputDir(inputDir), outputDir(outputDir), configFile(configFile) {};


string ResManager::getInputImagePath(const string imageName)
{
    return inputDir + imageName;
}

string ResManager::getOutputImagePath(const string imageName)
{
    return outputDir + imageName;
}

string ResManager::getConfigPath()
{
    return configFile;
}

json ResManager::lerConfig(const string caminhoArquivo)
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