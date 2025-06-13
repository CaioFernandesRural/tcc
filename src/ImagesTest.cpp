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

    MetricsLogger logger;

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

            // === Testes ===
            string originalPath = manager.getInputImagePath(fileName);
            string stegoPath = manager.getOutputImagePath(fileName + "_teste.png");
            string binPath = manager.getOutputImagePath(fileName + "_imagem_binarizada.png");

            cv::Mat original = cv::imread(originalPath);
            cv::Mat stego = cv::imread(stegoPath);
            cv::Mat bin = cv::imread(binPath);

            double psnr = PSNRCalculator::compute(original, stego);
            cout << "PSNR: " << psnr << " dB" << endl;

            double ssim = SSIMCalculator::compute(original, stego);
            cout << "SSIM: " << ssim << endl;

            double fsim = FSIMCalculator::compute(original, stego);
            cout << "FSIM: " << fsim << endl;

            double histSim = HistogramAnalyzer::compare(original, stego);
            cout << "Histogram Similarity (Correlação média): " << histSim << endl;

            // Exportar CSV de histograma
            string csvFile = fileName + "_hist.csv";
            HistogramAnalyzer::exportToCSV(original, stego, csvFile);
            cout << "Histograma exportado para CSV: " << csvFile << endl;

            double chi2 = ChiSquareAnalyzer::analyze(stego);
            cout << "Chi-square médio (detecção LSB): " << chi2 << endl;

            double rsDiff = RSAnalyzer::analyze(stego);
            cout << "RS Analysis (|R - S| / (R + S)): " << rsDiff << endl;
            
            int blockCapacity = CapacityAnalyser::analyze(bin);
            cout << "Max Payload (bytes): " << blockCapacity - 1 << endl;

            // Cálculo de bpp (bits por pixel)
            int totalPixels = bin.rows * bin.cols;
            int totalBits = (blockCapacity - 1) * 8;
            double bpp = static_cast<double>(totalBits) / static_cast<double>(totalPixels);

            cout << "bpp: " << fixed << setprecision(6) << bpp << endl;

            double ber = BERAnalyser::analyse(message, recoveredMessage);
            cout << "BER (Bit Error Ratio): " << std::fixed << std::setprecision(6) << ber << endl;

            // após calcular todas as métricas para fileName:
            MetricsLogger::Row row;
            row["image"] = fileName;
            row["psnr"]  = std::to_string(psnr);
            row["ssim"]  = std::to_string(ssim);
            row["fsim"]  = std::to_string(fsim);
            row["histSim"] = std::to_string(histSim);
            row["chi2"]    = std::to_string(chi2);
            row["rsDiff"]  = std::to_string(rsDiff);
            row["maxBytes"] = std::to_string(blockCapacity - 1);
            row["bpp"] =  std::to_string(bpp);
            row["ber"] = std::to_string(ber);

// adiciona ao logger
logger.addRow(row);
        
        }
        catch (const exception &e)
        {
            cerr << "Erro durante a execução: " << e.what() << endl;
        }
    } 
    logger.exportCSV("metrics.csv");

    return;

}