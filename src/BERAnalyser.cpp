#include "../include/BERAnalyser.hpp"

double BERAnalyser::analyse(const string& original, const string& recovered) {
    string binOriginal, binRecovered;

    // Converte cada caractere para representação binária
    for (char c : original) {
        bitset<8> bits(c);
        binOriginal += bits.to_string();
    }

    for (char c : recovered) {
        bitset<8> bits(c);
        binRecovered += bits.to_string();
    }

    // Garante que ambas tenham o mesmo comprimento para comparação
    size_t minSize = min(binOriginal.size(), binRecovered.size());
    if (minSize == 0) return 1.0; // Evita divisão por zero

    int bitErrors = 0;
    for (size_t i = 0; i < minSize; i++) {
        if (binOriginal[i] != binRecovered[i]) {
            bitErrors++;
        }
    }

    return static_cast<double>(bitErrors) / static_cast<double>(minSize);
}
