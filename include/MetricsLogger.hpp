#ifndef METRICSLOGGER_HPP
#define METRICSLOGGER_HPP

#include <string>
#include <vector>
#include <map>

class MetricsLogger {
public:
    // Chave -> valor (tudo em string para facilitar o CSV)
    using Row = std::map<std::string, std::string>;

    // Adiciona uma nova linha de métricas
    void addRow(const Row& row);

    // Exporta todas as linhas para um CSV com esse filename (cabeçalho + linhas)
    void exportCSV(const std::string& filename) const;

private:
    std::vector<Row> rows_;
};

#endif // METRICSLOGGER_HPP