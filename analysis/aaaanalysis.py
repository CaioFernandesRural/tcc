import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# ============================================================
# CONFIGURAÇÃO
# ============================================================

ARQUIVO_CSV = "metrics.csv"
PASTA_SAIDA = Path("analise_resultados")
PASTA_SAIDA.mkdir(exist_ok=True)

# ============================================================
# MÉTRICAS
# ============================================================

METRICAS = [
    "psnr",
    "ssim",
    "fsim",
    "histSim",
    "chi2",
    "rsDiff",
    "maxBytes",
    "bpp",
    "ber"
]

METRICAS_CORRELACAO = [
    "psnr",
    "ssim",
    "fsim",
    "histSim",
    "chi2",
    "rsDiff",
    "maxBytes",
    "bpp"
]

# ============================================================
# LEITURA
# ============================================================

df = pd.read_csv(ARQUIVO_CSV)

print("\nColunas encontradas:")
print(df.columns.tolist())

# Converte métricas para numérico
for metrica in METRICAS:
    if metrica in df.columns:
        df[metrica] = pd.to_numeric(df[metrica], errors="coerce")

# ============================================================
# FUNÇÕES AUXILIARES
# ============================================================

def salvar_figura(nome):
    caminho = PASTA_SAIDA / nome
    plt.tight_layout()
    plt.savefig(caminho, dpi=300, bbox_inches="tight")
    plt.close()
    print(f"[OK] Figura gerada: {caminho}")


def encontrar_coluna(possiveis):
    """
    Procura uma coluna no CSV ignorando maiúsculas/minúsculas.
    """
    mapa = {c.lower(): c for c in df.columns}

    for nome in possiveis:
        if nome.lower() in mapa:
            return mapa[nome.lower()]

    return None


# ============================================================
# TABELA 1
# PSNR, SSIM E FSIM
# Média + desvio padrão
# ============================================================

metricas_tabela1 = ["psnr", "ssim", "fsim"]

tabela1 = pd.DataFrame({
    "Métrica": ["PSNR", "SSIM", "FSIM"],
    "Média": [
        df["psnr"].mean(),
        df["ssim"].mean(),
        df["fsim"].mean()
    ],
    "Desvio padrão": [
        df["psnr"].std(),
        df["ssim"].std(),
        df["fsim"].std()
    ]
})

tabela1.to_csv(
    PASTA_SAIDA / "tabela_1_psnr_ssim_fsim.csv",
    index=False,
    sep=";"
)

print("\nTabela 1:")
print(tabela1.to_string(index=False))

# ============================================================
# FIGURA 1
# ORIGINAL X ESTEGANOGRAFADA
# ============================================================

col_original = encontrar_coluna([
    "original",
    "imagem_original",
    "original_path",
    "originalPath",
    "input"
])

col_stego = encontrar_coluna([
    "stego",
    "imagem_estego",
    "stego_path",
    "stegoPath",
    "output",
    "encoded"
])

if col_original and col_stego:

    original_path = Path(str(df.iloc[0][col_original]))
    stego_path = Path(str(df.iloc[0][col_stego]))

    if original_path.exists() and stego_path.exists():

        import cv2

        original = cv2.imread(str(original_path))
        stego = cv2.imread(str(stego_path))

        original = cv2.cvtColor(original, cv2.COLOR_BGR2RGB)
        stego = cv2.cvtColor(stego, cv2.COLOR_BGR2RGB)

        plt.figure(figsize=(12, 5))

        plt.subplot(1, 2, 1)
        plt.imshow(original)
        plt.title("Imagem original")
        plt.axis("off")

        plt.subplot(1, 2, 2)
        plt.imshow(stego)
        plt.title("Imagem estego")
        plt.axis("off")

        salvar_figura("figura_1_original_vs_estego.png")

    else:
        print("[AVISO] Caminhos das imagens não foram encontrados.")

else:
    print("[AVISO] Colunas original/stego não encontradas.")
    print("        Figura 1 não foi gerada.")


# ============================================================
# FIGURA 2
# HISTOGRAMAS COMPARATIVOS
# Duas imagens representativas
# ============================================================

if col_original and col_stego:

    import cv2

    quantidade = min(2, len(df))

    for i in range(quantidade):

        original_path = Path(str(df.iloc[i][col_original]))
        stego_path = Path(str(df.iloc[i][col_stego]))

        if not original_path.exists() or not stego_path.exists():
            continue

        original = cv2.imread(str(original_path))
        stego = cv2.imread(str(stego_path))

        plt.figure(figsize=(12, 5))

        # Original
        plt.subplot(1, 2, 1)

        cores = ("b", "g", "r")

        for canal, cor in enumerate(cores):
            hist = cv2.calcHist(
                [original],
                [canal],
                None,
                [256],
                [0, 256]
            )

            plt.plot(hist, color=cor)

        plt.title(f"Histograma - Original {i + 1}")
        plt.xlim([0, 256])
        plt.xlabel("Intensidade")
        plt.ylabel("Frequência")

        # Estego
        plt.subplot(1, 2, 2)

        for canal, cor in enumerate(cores):
            hist = cv2.calcHist(
                [stego],
                [canal],
                None,
                [256],
                [0, 256]
            )

            plt.plot(hist, color=cor)

        plt.title(f"Histograma - Estego {i + 1}")
        plt.xlim([0, 256])
        plt.xlabel("Intensidade")
        plt.ylabel("Frequência")

        salvar_figura(
            f"figura_2_histograma_comparativo_{i + 1}.png"
        )

else:
    print("[AVISO] Figura 2 não foi gerada.")


# ============================================================
# FIGURA 3
# HISTOGRAMAS SOBREPOSTOS
# Original x Estego
# ============================================================

if col_original and col_stego:

    import cv2

    for i in range(min(2, len(df))):

        original_path = Path(str(df.iloc[i][col_original]))
        stego_path = Path(str(df.iloc[i][col_stego]))

        if not original_path.exists() or not stego_path.exists():
            continue

        original = cv2.imread(str(original_path))
        stego = cv2.imread(str(stego_path))

        plt.figure(figsize=(10, 6))

        cores = ("b", "g", "r")

        for canal, cor in enumerate(cores):

            hist_original = cv2.calcHist(
                [original],
                [canal],
                None,
                [256],
                [0, 256]
            )

            hist_stego = cv2.calcHist(
                [stego],
                [canal],
                None,
                [256],
                [0, 256]
            )

            plt.plot(
                hist_original,
                color=cor,
                linestyle="-",
                alpha=0.8
            )

            plt.plot(
                hist_stego,
                color=cor,
                linestyle="--",
                alpha=0.8
            )

        plt.title(
            f"Comparação dos histogramas - Imagem {i + 1}"
        )

        plt.xlabel("Intensidade")
        plt.ylabel("Frequência")
        plt.xlim([0, 256])

        salvar_figura(
            f"figura_3_histogramas_sobrepostos_{i + 1}.png"
        )


# ============================================================
# TABELA 2
# CHI² E RS-DIFF
# ============================================================

tabela2 = pd.DataFrame({
    "Métrica": ["χ²", "RS-Diff"],
    "Média": [
        df["chi2"].mean(),
        df["rsDiff"].mean()
    ],
    "Desvio padrão": [
        df["chi2"].std(),
        df["rsDiff"].std()
    ]
})

tabela2.to_csv(
    PASTA_SAIDA / "tabela_2_chi2_rsdiff.csv",
    index=False,
    sep=";"
)

print("\nTabela 2:")
print(tabela2.to_string(index=False))


# ============================================================
# FIGURA 4
# CAPACIDADE DAS IMAGENS
# ============================================================

col_nome = encontrar_coluna([
    "image",
    "imagem",
    "filename",
    "file",
    "nome",
    "name"
])

plt.figure(figsize=(12, 6))

if col_nome:
    nomes = df[col_nome].astype(str)
else:
    nomes = [f"Imagem {i + 1}" for i in range(len(df))]

plt.bar(
    nomes,
    df["maxBytes"]
)

plt.title("Capacidade máxima de inserção por imagem")
plt.xlabel("Imagem")
plt.ylabel("Capacidade (bytes)")
plt.xticks(rotation=45, ha="right")

salvar_figura("figura_4_capacidade.png")


# ============================================================
# TABELA 4
# BER
# ============================================================

tabela4 = pd.DataFrame({
    "Métrica": ["BER"],
    "Média": [df["ber"].mean()],
    "Desvio padrão": [df["ber"].std()],
    "Mínimo": [df["ber"].min()],
    "Máximo": [df["ber"].max()]
})

tabela4.to_csv(
    PASTA_SAIDA / "tabela_4_ber.csv",
    index=False,
    sep=";"
)

print("\nTabela 4:")
print(tabela4.to_string(index=False))


# ============================================================
# FIGURA 5
# VARIAÇÃO DO BER
# ============================================================

plt.figure(figsize=(12, 6))

plt.bar(
    nomes,
    df["ber"]
)

plt.title("Variação do BER entre as imagens")
plt.xlabel("Imagem")
plt.ylabel("BER")
plt.xticks(rotation=45, ha="right")

salvar_figura("figura_5_ber.png")


# ============================================================
# RELATÓRIO RESUMIDO
# ============================================================

resumo = pd.DataFrame({
    "Métrica": METRICAS,
    "Média": [
        df[m].mean() if m in df.columns else np.nan
        for m in METRICAS
    ],
    "Desvio padrão": [
        df[m].std() if m in df.columns else np.nan
        for m in METRICAS
    ],
    "Mínimo": [
        df[m].min() if m in df.columns else np.nan
        for m in METRICAS
    ],
    "Máximo": [
        df[m].max() if m in df.columns else np.nan
        for m in METRICAS
    ]
})

resumo.to_csv(
    PASTA_SAIDA / "resumo_metricas.csv",
    index=False,
    sep=";"
)


# ============================================================
# CORRELAÇÃO
# ============================================================

metricas_existentes = [
    m for m in METRICAS_CORRELACAO
    if m in df.columns
]

if len(metricas_existentes) >= 2:

    correlacao = df[metricas_existentes].corr()

    correlacao.to_csv(
        PASTA_SAIDA / "correlacao_metricas.csv",
        sep=";"
    )


# ============================================================
# FINAL
# ============================================================

print("\n==============================================")
print("ANÁLISE CONCLUÍDA")
print("==============================================")
print(f"Arquivos salvos em: {PASTA_SAIDA.resolve()}")
print()
print("Arquivos principais:")

for arquivo in sorted(PASTA_SAIDA.iterdir()):
    print(f" - {arquivo.name}")
