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
# CONFIGURAÇÃO DAS MÉTRICAS
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

# BER não entra na correlação pois é constante
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
# CONFIGURAÇÃO DAS IMAGENS
# ============================================================
#
# Informe aqui as imagens que deseja utilizar nas Figuras 1,
# 2 e 3.
#
# Exemplo:
#
# IMAGEM_ORIGINAL_1 = "testes/original_baboon.png"
# IMAGEM_ESTego_1   = "testes/estego_baboon.png"
#
# Faça o mesmo para uma segunda imagem.
#

IMAGEM_ORIGINAL_1 = "original1.png"
IMAGEM_ESTego_1 = "estego1.png"

IMAGEM_ORIGINAL_2 = "original2.png"
IMAGEM_ESTego_2 = "estego2.png"


# ============================================================
# FUNÇÕES AUXILIARES
# ============================================================

def salvar_figura(nome):
    """
    Salva a figura em PNG com qualidade adequada para o TCC.
    """
    caminho = PASTA_SAIDA / nome

    plt.savefig(
        caminho,
        dpi=300,
        bbox_inches="tight"
    )

    print(f"[OK] Figura salva: {caminho}")


def carregar_imagem(caminho):
    """
    Carrega uma imagem utilizando OpenCV.
    """
    import cv2

    imagem = cv2.imread(str(caminho))

    if imagem is None:
        raise FileNotFoundError(
            f"Não foi possível carregar a imagem: {caminho}"
        )

    # OpenCV carrega em BGR.
    # Converte para RGB para exibição correta no matplotlib.
    return cv2.cvtColor(imagem, cv2.COLOR_BGR2RGB)


def encontrar_coluna_imagem(df):
    """
    Tenta descobrir automaticamente qual coluna identifica
    o nome da imagem.
    """

    candidatos = [
        "image",
        "imagem",
        "filename",
        "file",
        "name",
        "nome",
        "imageName",
        "image_name"
    ]

    for coluna in candidatos:
        if coluna in df.columns:
            return coluna

    return None


# ============================================================
# LEITURA DOS DADOS
# ============================================================

print("=" * 60)
print("ANÁLISE DOS RESULTADOS")
print("=" * 60)

df = pd.read_csv(ARQUIVO_CSV)

print(f"\nQuantidade de registros: {len(df)}")
print("\nColunas encontradas:")
print(list(df.columns))


# ============================================================
# VERIFICAÇÃO DAS MÉTRICAS
# ============================================================

for metrica in METRICAS:

    if metrica not in df.columns:
        print(
            f"[AVISO] A coluna '{metrica}' não foi encontrada."
        )
    else:
        df[metrica] = pd.to_numeric(
            df[metrica],
            errors="coerce"
        )


# ============================================================
# ESTATÍSTICAS DESCRITIVAS
# ============================================================

print("\n")
print("=" * 60)
print("ESTATÍSTICAS")
print("=" * 60)

estatisticas = df[METRICAS].describe().T

estatisticas["desvio_padrao"] = df[METRICAS].std()

print(estatisticas)

estatisticas.to_csv(
    PASTA_SAIDA / "estatisticas_descritivas.csv"
)


# ============================================================
# FIGURA 1
# ORIGINAL × ESTÉGO
# ============================================================

print("\nGerando Figura 1...")

try:

    original1 = carregar_imagem(IMAGEM_ORIGINAL_1)
    estego1 = carregar_imagem(IMAGEM_ESTego_1)

    fig, ax = plt.subplots(
        1,
        2,
        figsize=(12, 6)
    )

    ax[0].imshow(original1)
    ax[0].set_title("Imagem original")
    ax[0].axis("off")

    ax[1].imshow(estego1)
    ax[1].set_title("Imagem estego")
    ax[1].axis("off")

    plt.tight_layout()

    salvar_figura("figura_1_original_vs_estego.png")

    plt.close()

except FileNotFoundError as erro:

    print(f"[AVISO] Figura 1 não gerada: {erro}")


# ============================================================
# FUNÇÃO PARA HISTOGRAMA RGB
# ============================================================

def plotar_histograma_rgb(imagem, ax, titulo):

    canais = [
        ("R", 0),
        ("G", 1),
        ("B", 2)
    ]

    for nome, canal in canais:

        valores = imagem[:, :, canal].ravel()

        ax.hist(
            valores,
            bins=256,
            alpha=0.35,
            density=True,
            label=nome
        )

    ax.set_title(titulo)
    ax.set_xlabel("Intensidade")
    ax.set_ylabel("Frequência normalizada")
    ax.set_xlim(0, 255)
    ax.legend()


# ============================================================
# FIGURA 2
# HISTOGRAMAS DE DUAS IMAGENS
# ============================================================

print("\nGerando Figura 2...")

try:

    original1 = carregar_imagem(IMAGEM_ORIGINAL_1)
    estego1 = carregar_imagem(IMAGEM_ESTego_1)

    original2 = carregar_imagem(IMAGEM_ORIGINAL_2)
    estego2 = carregar_imagem(IMAGEM_ESTego_2)

    fig, ax = plt.subplots(
        2,
        2,
        figsize=(12, 8)
    )

    plotar_histograma_rgb(
        original1,
        ax[0, 0],
        "Imagem 1 — Original"
    )

    plotar_histograma_rgb(
        estego1,
        ax[0, 1],
        "Imagem 1 — Estego"
    )

    plotar_histograma_rgb(
        original2,
        ax[1, 0],
        "Imagem 2 — Original"
    )

    plotar_histograma_rgb(
        estego2,
        ax[1, 1],
        "Imagem 2 — Estego"
    )

    plt.tight_layout()

    salvar_figura("figura_2_histogramas.png")

    plt.close()

except FileNotFoundError as erro:

    print(f"[AVISO] Figura 2 não gerada: {erro}")


# ============================================================
# FIGURA 3
# HISTOGRAMAS SOBREPOSTOS
# ============================================================

print("\nGerando Figura 3...")

try:

    original1 = carregar_imagem(IMAGEM_ORIGINAL_1)
    estego1 = carregar_imagem(IMAGEM_ESTego_1)

    original2 = carregar_imagem(IMAGEM_ORIGINAL_2)
    estego2 = carregar_imagem(IMAGEM_ESTego_2)

    fig, ax = plt.subplots(
        1,
        2,
        figsize=(13, 5)
    )

    def hist_comparativo(original, estego, eixo, titulo):

        for canal, nome in [
            (0, "R"),
            (1, "G"),
            (2, "B")
        ]:

            eixo.hist(
                original[:, :, canal].ravel(),
                bins=256,
                alpha=0.25,
                density=True,
                label=f"Original {nome}"
            )

            eixo.hist(
                estego[:, :, canal].ravel(),
                bins=256,
                alpha=0.25,
                density=True,
                histtype="step",
                linewidth=1.5,
                label=f"Estego {nome}"
            )

        eixo.set_title(titulo)
        eixo.set_xlabel("Intensidade")
        eixo.set_ylabel("Frequência normalizada")
        eixo.set_xlim(0, 255)
        eixo.legend(fontsize=8)

    hist_comparativo(
        original1,
        estego1,
        ax[0],
        "Imagem 1"
    )

    hist_comparativo(
        original2,
        estego2,
        ax[1],
        "Imagem 2"
    )

    plt.tight_layout()

    salvar_figura("figura_3_histogramas_comparativos.png")

    plt.close()

except FileNotFoundError as erro:

    print(f"[AVISO] Figura 3 não gerada: {erro}")


# ============================================================
# FIGURA 4
# CAPACIDADE
# ============================================================

print("\nGerando Figura 4...")

if "maxBytes" in df.columns and "bpp" in df.columns:

    coluna_imagem = encontrar_coluna_imagem(df)

    # --------------------------------------------------------
    # Seleção das imagens
    # --------------------------------------------------------

    if coluna_imagem is not None:

        dados = df.copy()

        # Ordena pelas imagens com maior capacidade.
        dados = dados.sort_values(
            "maxBytes",
            ascending=False
        )

        # Seleciona algumas imagens representativas.
        dados = dados.head(10)

        nomes = dados[coluna_imagem].astype(str)

    else:

        # Caso não exista coluna de nome da imagem,
        # utiliza o índice dos registros.
        dados = df.sort_values(
            "maxBytes",
            ascending=False
        ).head(10)

        nomes = [
            f"Imagem {i + 1}"
            for i in range(len(dados))
        ]

    # --------------------------------------------------------
    # Gráfico
    # --------------------------------------------------------

    fig, ax1 = plt.subplots(
        figsize=(12, 6)
    )

    x = np.arange(len(dados))
    largura = 0.35

    barras = ax1.bar(
        x - largura / 2,
        dados["maxBytes"],
        largura,
        label="maxBytes"
    )

    ax1.set_xlabel("Imagem")
    ax1.set_ylabel("Capacidade máxima (bytes)")
    ax1.set_xticks(x)
    ax1.set_xticklabels(
        nomes,
        rotation=45,
        ha="right"
    )

    ax2 = ax1.twinx()

    ax2.bar(
        x + largura / 2,
        dados["bpp"],
        largura,
        alpha=0.6,
        label="bpp"
    )

    ax2.set_ylabel("Bits por pixel (bpp)")

    ax1.set_title(
        "Capacidade de inserção nas imagens selecionadas"
    )

    plt.tight_layout()

    salvar_figura("figura_4_capacidade.png")

    plt.close()

else:

    print(
        "[AVISO] Figura 4 não gerada: "
        "colunas maxBytes e/ou bpp ausentes."
    )


# ============================================================
# FIGURA 5
# BER
# ============================================================

print("\nGerando Figura 5...")

if "ber" in df.columns:

    ber = df["ber"].dropna()

    fig, ax = plt.subplots(
        figsize=(8, 6)
    )

    # Boxplot
    ax.boxplot(
        ber,
        labels=["BER"]
    )

    # Pontos individuais
    x_jitter = np.random.normal(
        1,
        0.04,
        size=len(ber)
    )

    ax.scatter(
        x_jitter,
        ber,
        alpha=0.45,
        s=20
    )

    ax.set_ylabel("Bit Error Rate (BER)")
    ax.set_title(
        "Distribuição do Bit Error Rate"
    )

    # Média
    media_ber = ber.mean()

    ax.axhline(
        media_ber,
        linestyle="--",
        linewidth=1,
        label=f"Média = {media_ber:.6f}"
    )

    ax.legend()

    plt.tight_layout()

    salvar_figura("figura_5_ber.png")

    plt.close()

    print(f"Média BER: {media_ber:.10f}")

else:

    print(
        "[AVISO] Figura 5 não gerada: "
        "coluna ber ausente."
    )


# ============================================================
# MATRIZ DE CORRELAÇÃO
# ============================================================

print("\nGerando matriz de correlação...")

metricas_disponiveis = [
    m for m in METRICAS_CORRELACAO
    if m in df.columns
]

if len(metricas_disponiveis) >= 2:

    correlacao = df[
        metricas_disponiveis
    ].corr()

    fig, ax = plt.subplots(
        figsize=(10, 8)
    )

    imagem = ax.imshow(
        correlacao,
        interpolation="nearest",
        aspect="auto"
    )

    ax.set_xticks(
        np.arange(len(metricas_disponiveis))
    )

    ax.set_yticks(
        np.arange(len(metricas_disponiveis))
    )

    ax.set_xticklabels(
        metricas_disponiveis,
        rotation=45,
        ha="right"
    )

    ax.set_yticklabels(
        metricas_disponiveis
    )

    # Valores dentro da matriz
    for i in range(len(metricas_disponiveis)):

        for j in range(len(metricas_disponiveis)):

            valor = correlacao.iloc[i, j]

            ax.text(
                j,
                i,
                f"{valor:.2f}",
                ha="center",
                va="center"
            )

    ax.set_title(
        "Correlação entre as métricas"
    )

    plt.colorbar(
        imagem,
        ax=ax,
        label="Correlação de Pearson"
    )

    plt.tight_layout()

    salvar_figura("matriz_correlacao.png")

    plt.close()


# ============================================================
# RESUMO FINAL
# ============================================================

print("\n")
print("=" * 60)
print("ANÁLISE CONCLUÍDA")
print("=" * 60)

print(f"\nArquivos gerados em:")
print( PASTA_SAIDA.resolve() )

print("\nConteúdo esperado:")

for arquivo in PASTA_SAIDA.iterdir():

    print(
        f" - {arquivo.name}"
    )