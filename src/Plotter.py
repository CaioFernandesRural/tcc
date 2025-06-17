import os
import pandas as pd
import matplotlib.pyplot as plt

# Caminho para os CSVs gerados
hist_dir = "resources/output_histograms"

# Itera por todos os arquivos .csv
for filename in os.listdir(hist_dir):
    if filename.endswith(".csv"):
        filepath = os.path.join(hist_dir, filename)
        df = pd.read_csv(filepath)

        # Extrai nome base sem extensão
        base_name = filename.replace("_hist.csv", "")

        plt.figure(figsize=(10, 6))
        plt.title(f"Histograma - {base_name}")
        plt.xlabel("Intensidade de Pixel (0–255)")
        plt.ylabel("Frequência Normalizada")

        # Plot por canal
        plt.plot(df["Bin"], df["Original_R"], label="Original R", color='r')
        plt.plot(df["Bin"], df["Stego_R"], label="Stego R", color='r', linestyle='--')

        plt.plot(df["Bin"], df["Original_G"], label="Original G", color='g')
        plt.plot(df["Bin"], df["Stego_G"], label="Stego G", color='g', linestyle='--')

        plt.plot(df["Bin"], df["Original_B"], label="Original B", color='b')
        plt.plot(df["Bin"], df["Stego_B"], label="Stego B", color='b', linestyle='--')

        plt.legend()
        plt.grid(True)

        # Exibir
        plt.tight_layout()
        plt.show()

        # (Opcional) Salvar como imagem:
        # plt.savefig(os.path.join(hist_dir, f"{base_name}_plot.png"))
        # plt.close()
