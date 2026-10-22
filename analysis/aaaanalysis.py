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

# ============================================================
# LEITURA DO CSV
# ============================================================
#
# metrics.csv:
#
# image,psnr,ssim,...
# 0001.png,77.889218,0.999978,...
#
# ,  = separador de coluna
# .  = separador decimal
#
# ============================================================

df = pd.read_csv(
    ARQUIVO_CSV,
    sep=",",
    decimal="."
)

# ============================================================
# CONVERSÃO DAS MÉTRICAS
# ============================================================

for metrica in METRICAS:
    if metrica in df.columns:
        df[metrica] = pd.to_numeric(
            df[metrica],
            errors="coerce"
        )

print("\n==============================================")
print("DADOS CARREGADOS")
print("==============================================")

print(f"Quantidade de imagens: {len(df)}")
print("\nColunas:")
print(df.columns.tolist())

print("\nPrimeiras linhas:")
print(df.head())


# ============================================================
# FUNÇÃO PARA SALVAR FIGURAS
# ============================================================

def salvar_figura(nome):
    caminho = PASTA_SAIDA / nome

    plt.tight_layout()

    plt.savefig(
        caminho,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close()

    print(f"[OK] {caminho}")


# ============================================================
# TABELA 1
#
# Valores médios e desvio padrão de
# PSNR, SSIM e FSIM
# ============================================================

tabela1 = pd.DataFrame({
    "Métrica": [
        "PSNR",
        "SSIM",
        "FSIM"
    ],

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

    # Padrão brasileiro no arquivo de saída
    sep=";",
    decimal=","
)

print("\n==============================================")
print("TABELA 1")
print("==============================================")

print(
    tabela1.to_string(
        index=False,
        float_format=lambda x: f"{x:.6f}".replace(".", ",")
    )
)


# ============================================================
# FIGURA 1
#
# Exemplo comparativo entre imagem original
# e imagem estego
#
# OBS:
# O CSV não possui o caminho das imagens.
# Portanto, esta figura será gerada posteriormente
# quando forem informadas as pastas das imagens.
# ============================================================

print("\n[INFO] Figura 1 depende das pastas das imagens original/estego.")


# ============================================================
# TABELA 2
#
# Valores médios de χ² e RS-Diff
# ============================================================

tabela2 = pd.DataFrame({
    "Métrica": [
        "χ²",
        "RS-Diff"
    ],

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
    sep=";",
    decimal=","
)

print("\n==============================================")
print("TABELA 2")
print("==============================================")

print(
    tabela2.to_string(
        index=False,
        float_format=lambda x: f"{x:.6f}".replace(".", ",")
    )
)


# ============================================================
# FIGURA 4
#
# Capacidade máxima por imagem
# ============================================================

plt.figure(figsize=(12, 6))

plt.bar(
    df["image"].astype(str),
    df["maxBytes"]
)

plt.title(
    "Capacidade máxima de inserção por imagem"
)

plt.xlabel("Imagem")
plt.ylabel("Capacidade (bytes)")

plt.xticks(
    rotation=45,
    ha="right"
)

salvar_figura(
    "figura_4_capacidade.png"
)


# ============================================================
# TABELA 4
#
# Valores médios de BER
# ============================================================

tabela4 = pd.DataFrame({
    "Métrica": [
        "BER"
    ],

    "Média": [
        df["ber"].mean()
    ],

    "Desvio padrão": [
        df["ber"].std()
    ],

    "Mínimo": [
        df["ber"].min()
    ],

    "Máximo": [
        df["ber"].max()
    ]
})

tabela4.to_csv(
    PASTA_SAIDA / "tabela_4_ber.csv",
    index=False,
    sep=";",
    decimal=","
)

print("\n==============================================")
print("TABELA 4")
print("==============================================")

print(
    tabela4.to_string(
        index=False,
        float_format=lambda x: f"{x:.6f}".replace(".", ",")
    )
)


# ============================================================
# FIGURA 5
#
# Variação do BER entre as imagens
# ============================================================

plt.figure(figsize=(12, 6))

plt.bar(
    df["image"].astype(str),
    df["ber"]
)

plt.title(
    "Variação do BER entre as imagens"
)

plt.xlabel("Imagem")
plt.ylabel("BER")

plt.xticks(
    rotation=45,
    ha="right"
)

salvar_figura(
    "figura_5_ber.png"
)


# ============================================================
# RESUMO GERAL
# ============================================================

resumo = pd.DataFrame({
    "Métrica": METRICAS,

    "Média": [
        df[m].mean()
        for m in METRICAS
    ],

    "Desvio padrão": [
        df[m].std()
        for m in METRICAS
    ],

    "Mínimo": [
        df[m].min()
        for m in METRICAS
    ],

    "Máximo": [
        df[m].max()
        for m in METRICAS
    ]
})

resumo.to_csv(
    PASTA_SAIDA / "resumo_metricas.csv",
    index=False,
    sep=";",
    decimal=","
)


# ============================================================
# CORRELAÇÃO
# ============================================================

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

metricas_existentes = [
    m
    for m in METRICAS_CORRELACAO
    if m in df.columns
]

correlacao = df[
    metricas_existentes
].corr()

correlacao.to_csv(
    PASTA_SAIDA / "correlacao_metricas.csv",
    sep=";",
    decimal=","
)


# ============================================================
# RELATÓRIO FINAL NO TERMINAL
# ============================================================

print("\n==============================================")
print("RESUMO DAS MÉTRICAS")
print("==============================================")

print(
    resumo.to_string(
        index=False,
        float_format=lambda x: f"{x:.6f}".replace(".", ",")
    )
)

print("\n==============================================")
print("ANÁLISE CONCLUÍDA")
print("==============================================")

print(
    f"Arquivos salvos em:\n"
    f"{PASTA_SAIDA.resolve()}"
)

print("\nArquivos:")

for arquivo in sorted(PASTA_SAIDA.iterdir()):
    print(f" - {arquivo.name}")