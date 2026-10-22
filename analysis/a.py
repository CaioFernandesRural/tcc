import csv

ARQUIVO_ENTRADA = "metrics.csv"
ARQUIVO_SAIDA = "metrics_br.csv"

def converter_csv():
    with open(ARQUIVO_ENTRADA, "r", encoding="utf-8", newline="") as entrada:
        leitor = csv.reader(entrada)

        with open(ARQUIVO_SAIDA, "w", encoding="utf-8-sig", newline="") as saida:
            escritor = csv.writer(
                saida,
                delimiter=";",
                quoting=csv.QUOTE_MINIMAL
            )

            for linha in leitor:
                nova_linha = []

                for valor in linha:
                    # Tenta identificar valores numéricos
                    try:
                        float(valor)

                        # Troca ponto decimal por vírgula
                        valor = valor.replace(".", ",")

                    except ValueError:
                        # Texto permanece inalterado
                        pass

                    nova_linha.append(valor)

                escritor.writerow(nova_linha)

    print(f"CSV convertido com sucesso!")
    print(f"Entrada: {ARQUIVO_ENTRADA}")
    print(f"Saída:   {ARQUIVO_SAIDA}")


if __name__ == "__main__":
    converter_csv()