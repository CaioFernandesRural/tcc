#include "../include/ImagesTest.hpp"

ImagesTest::ImagesTest(){}
ImagesTest::~ImagesTest(){}

map<string, int> ImagesTest::getNomes(string folderPath){

    // Verifica se o diretório existe
    if (!fs::exists(folderPath)) {
        cerr << "O diretório não existe." << endl;
        throw;
    }

    // Lista para armazenar os nomes dos arquivos
    map<string, int> fileNames;

    // Itera pelos arquivos no diretório
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) { // Verifica se é um arquivo regular
            string filename = entry.path().filename().string();
            fileNames[filename] = 0; // Inicializa com valor 0
        }
    }

    return fileNames;
}

void ImagesTest::carregarConfiguracao(string filename)
{
    try
    {
        json conf = manager.lerConfig(manager.getConfigPath());
        
        inputFileName = manager.getInputImagePath(filename);

        ksize = conf["ksize"];
        threshold = conf["threshold"];
        message = conf["mensagem"];
        
    }
    catch (const runtime_error &e)
    {
        cerr << "Erro ao carregar a configuração: " << e.what() << endl;
        throw;
    }
}

void ImagesTest::run() {

    map<string, int> files = getNomes("./resources/input_images");

    // Exibe os nomes dos arquivos
    //cout << "Arquivos encontrados na pasta:" << endl;
    // for (const auto& fileName : files) {
    //     cout << fileName << endl;
    // }

    for (auto& [fileName, nInicial] : files) {
        try
        {
            cout << "\nTrantando a imagem: " << fileName << endl;

            carregarConfiguracao(fileName);
            processarImagem();
            salvarResultados(fileName);
            compararImagens(fileName);

            // aqui deu pau pq o N muda com cada imag
            //int nInicial = manager.lerConfig(manager.getConfigPath())["Ninicial"];

            //int nInicial;

            //cout << "\n" << "Insira N inicial de: " << fileName << "\n";
            //cin >> nInicial;

            int nInicial = encoder.getNinicial();
            files[fileName] = nInicial;

            string recoveredMessage = decoder.decodeImagem(imgOut, nInicial);
            cout << "Mensagem decodificada: " << recoveredMessage << endl;

        }
        catch (const exception &e)
        {
            cerr << "Erro durante a execução: " << e.what() << endl;
        }
    } 


    return;

}