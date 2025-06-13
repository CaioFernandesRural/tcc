#include "../include/MetricsLogger.hpp"
#include <fstream>
#include <iostream>
#include <set>

void MetricsLogger::addRow(const Row& row) {
    rows_.push_back(row);
}

void MetricsLogger::exportCSV(const std::string& filename) const {
    if (rows_.empty()) {
        std::cerr << "MetricsLogger: nenhum dado para exportar.\n";
        return;
    }

    // Ordem personalizada das colunas
    std::vector<std::string> headers = {
        "image", "psnr", "ssim", "fsim", "histSim",
        "chi2", "rsDiff", "maxBytes", "bpp", "ber"
    };

    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        std::cerr << "MetricsLogger: não consegui abrir " << filename << "\n";
        return;
    }

    // Escrever cabeçalho
    for (size_t i = 0; i < headers.size(); ++i) {
        ofs << headers[i];
        if (i < headers.size() - 1) ofs << ",";
    }
    ofs << "\n";

    // Escrever linhas
    for (const auto& row : rows_) {
        for (size_t i = 0; i < headers.size(); ++i) {
            auto it = row.find(headers[i]);
            if (it != row.end()) {
                ofs << it->second;
            }
            // else: célula vazia
            if (i < headers.size() - 1) ofs << ",";
        }
        ofs << "\n";
    }

    ofs.close();
    std::cout << "MetricsLogger: exportado para " << filename << "\n";
}
