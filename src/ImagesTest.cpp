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

    for (auto& [fileName, nInicial] : files) {
        try {
            cout << "\n=== Processando imagem: " << fileName << " ===" << endl;

            carregarConfiguracao(fileName);
            processarImagem();
            salvarResultados(fileName);
            compararImagens(fileName);

            // Atualiza N inicial
            nInicial = encoder.getNinicial();
            files[fileName] = nInicial;

            // Decodificação
            recoveredMessage = decoder.decodeImagem(imgOut, nInicial);
            cout << "Mensagem decodificada: " << recoveredMessage << endl;

            // === Cálculo de Métricas ===
            psnr = PSNRCalculator::compute(imgIn, imgOut);
            ssim = SSIMCalculator::compute(imgIn, imgOut);
            fsim = FSIMCalculator::compute(imgIn, imgOut);
            histSim = HistogramAnalyzer::compare(imgIn, imgOut);
            chi2 = ChiSquareAnalyzer::analyze(imgOut);
            rsDiff = RSAnalyzer::analyze(imgOut);
            blockCapacity = CapacityAnalyser::analyze(imgBinary);
            
            // bpp (bits por pixel)
            int totalPixels = imgBinary.rows * imgBinary.cols;
            int totalBits = (blockCapacity - 1) * 8;
            bpp = static_cast<double>(totalBits) / static_cast<double>(totalPixels);

            // BER
            ber = BERAnalyser::analyse(message, recoveredMessage);

            // === Exibição das métricas ===
            cout << fixed << setprecision(6);
            cout << "PSNR: " << psnr << " dB" << endl;
            cout << "SSIM: " << ssim << endl;
            cout << "FSIM: " << fsim << endl;
            cout << "Histogram Similarity: " << histSim << endl;
            cout << "Chi-square médio: " << chi2 << endl;
            cout << "RS Analysis (|R - S| / (R + S)): " << rsDiff << endl;
            cout << "Max Payload (bytes): " << blockCapacity - 1 << endl;
            cout << "bpp: " << bpp << endl;
            cout << "BER: " << ber << endl;

            // Exporta CSV de histograma
            string csvFile = fileName + "_hist.csv";
            HistogramAnalyzer::exportToCSV(imgIn, imgOut, csvFile);
            cout << "Histograma exportado para: " << csvFile << endl;

            // === Gravação no Logger ===
            MetricsLogger::Row row;
            row["image"]    = fileName;
            row["psnr"]     = to_string(psnr);
            row["ssim"]     = to_string(ssim);
            row["fsim"]     = to_string(fsim);
            row["histSim"]  = to_string(histSim);
            row["chi2"]     = to_string(chi2);
            row["rsDiff"]   = to_string(rsDiff);
            row["maxBytes"] = to_string(blockCapacity - 1);
            row["bpp"]      = to_string(bpp);
            row["ber"]      = to_string(ber);

            logger.addRow(row);
        }
        catch (const exception& e) {
            cerr << "[ERRO] Durante processamento da imagem '" << fileName << "': " << e.what() << endl;
        }
    }

    logger.exportCSV("metrics.csv");
    cout << "\n=== Exportação de métricas finalizada ===" << endl;
}