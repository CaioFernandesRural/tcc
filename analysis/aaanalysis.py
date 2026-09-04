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

# BER não entra na correlação pois é constante (todos os valores são 0)
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
# FUNÇÕES AUXILIARES
# ============================================================

def salvar_figura(nome):

    plt.tight_layout()

    plt.savefig(
        PASTA_SAIDA / nome,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close()


def gerar_histograma(df, coluna, titulo, xlabel, bins=30):

    dados = df[coluna].dropna()

    plt.figure(figsize=(10, 6))

    plt.hist(
        dados,
        bins=bins,
        edgecolor="black"
    )

    media = dados.mean()
    mediana = dados.median()

    plt.axvline(
        media,
        linestyle="--",
        linewidth=2,
        label=f"Média: {media:.6f}"
    )

    plt.axvline(
        mediana,
        linestyle=":",
        linewidth=2,
        label=f"Mediana: {mediana:.6f}"
    )

    plt.title(titulo)

    plt.xlabel(xlabel)

    plt.ylabel("Quantidade de imagens")

    plt.legend()

    plt.grid(
        axis="y",
        alpha=0.3
    )

    salvar_figura(
        f"distribuicao_{coluna}.png"
    )


def gerar_boxplot(dados, labels, titulo, ylabel, arquivo):

    plt.figure(figsize=(10, 6))

    # labels diretamente no boxplot
    # Não utilizamos plt.xticks()
    plt.boxplot(
        dados,
        labels=labels
    )

    plt.title(titulo)

    plt.ylabel(ylabel)

    plt.grid(
        axis="y",
        alpha=0.3
    )

    salvar_figura(arquivo)


# ============================================================
# CARREGAMENTO
# ============================================================

print("Carregando CSV...")

df = pd.read_csv(ARQUIVO_CSV)

print(f"Total de imagens analisadas: {len(df)}")

print("\nColunas encontradas:")

print(df.columns.tolist())


# ============================================================
# VALIDAÇÃO
# ============================================================

colunas_faltando = [

    coluna

    for coluna in METRICAS

    if coluna not in df.columns

]


if colunas_faltando:

    print("\nERRO: Colunas ausentes:")

    print(colunas_faltando)

    raise SystemExit(1)


# ============================================================
# CONVERSÃO PARA NUMÉRICO
# ============================================================

for coluna in METRICAS:

    df[coluna] = pd.to_numeric(

        df[coluna],

        errors="coerce"

    )


# ============================================================
# ESTATÍSTICAS DESCRITIVAS
# ============================================================

print("\nCalculando estatísticas descritivas...")


estatisticas = pd.DataFrame({

    "Métrica": METRICAS,

    "Média": [
        df[m].mean()
        for m in METRICAS
    ],

    "Mediana": [
        df[m].median()
        for m in METRICAS
    ],

    "Desvio Padrão": [
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
    ],

    "Q1": [
        df[m].quantile(0.25)
        for m in METRICAS
    ],

    "Q3": [
        df[m].quantile(0.75)
        for m in METRICAS
    ]

})


estatisticas.to_csv(

    PASTA_SAIDA / "estatisticas_descritivas.csv",

    index=False,

    encoding="utf-8-sig"

)


print("\nESTATÍSTICAS DESCRITIVAS")

print(

    estatisticas.to_string(

        index=False

    )

)


# ============================================================
# TABELAS PARA O TCC
# ============================================================

print("\nGerando tabelas específicas...")


# Imperceptibilidade

metricas_imperceptibilidade = [

    "psnr",

    "ssim",

    "fsim"

]


estatisticas[

    estatisticas["Métrica"].isin(

        metricas_imperceptibilidade

    )

].to_csv(

    PASTA_SAIDA / "tabela_imperceptibilidade.csv",

    index=False,

    encoding="utf-8-sig"

)


# Segurança

metricas_seguranca = [

    "histSim",

    "chi2",

    "rsDiff"

]


estatisticas[

    estatisticas["Métrica"].isin(

        metricas_seguranca

    )

].to_csv(

    PASTA_SAIDA / "tabela_seguranca.csv",

    index=False,

    encoding="utf-8-sig"

)


# Capacidade

metricas_capacidade = [

    "maxBytes",

    "bpp"

]


estatisticas[

    estatisticas["Métrica"].isin(

        metricas_capacidade

    )

].to_csv(

    PASTA_SAIDA / "tabela_capacidade.csv",

    index=False,

    encoding="utf-8-sig"

)


# ============================================================
# RECUPERABILIDADE / BER
# ============================================================

print("\nAnalisando recuperabilidade (BER)...")


total_imagens = len(df)


ber_zero = (

    df["ber"] == 0

).sum()


ber_com_erro = (

    df["ber"] > 0

).sum()


percentual_ber_zero = (

    ber_zero / total_imagens

) * 100


percentual_erro = (

    ber_com_erro / total_imagens

) * 100


recuperabilidade = pd.DataFrame({

    "Indicador": [

        "Total de imagens",

        "BER médio",

        "BER mediano",

        "BER mínimo",

        "BER máximo",

        "Imagens com BER = 0",

        "Imagens com BER > 0",

        "% recuperação perfeita",

        "% imagens com erro"

    ],

    "Valor": [

        total_imagens,

        df["ber"].mean(),

        df["ber"].median(),

        df["ber"].min(),

        df["ber"].max(),

        ber_zero,

        ber_com_erro,

        percentual_ber_zero,

        percentual_erro

    ]

})


recuperabilidade.to_csv(

    PASTA_SAIDA / "recuperabilidade.csv",

    index=False,

    encoding="utf-8-sig"

)


print()

print(

    recuperabilidade.to_string(

        index=False

    )

)


# ============================================================
# DETECÇÃO DE OUTLIERS - IQR
# ============================================================

print("\nDetectando outliers...")


lista_outliers = []


for metrica in METRICAS:

    dados = df[metrica].dropna()


    q1 = dados.quantile(0.25)

    q3 = dados.quantile(0.75)


    iqr = q3 - q1


    limite_inferior = q1 - 1.5 * iqr

    limite_superior = q3 + 1.5 * iqr


    mascara = (

        (dados < limite_inferior)

        |

        (dados > limite_superior)

    )


    for indice, valor in dados[mascara].items():

        lista_outliers.append({

            "image": df.loc[indice, "image"],

            "metrica": metrica,

            "valor": valor,

            "limite_inferior": limite_inferior,

            "limite_superior": limite_superior

        })


df_outliers = pd.DataFrame(lista_outliers)


df_outliers.to_csv(

    PASTA_SAIDA / "outliers.csv",

    index=False,

    encoding="utf-8-sig"

)


print(

    f"Total de outliers encontrados: "

    f"{len(df_outliers)}"

)


if not df_outliers.empty:

    resumo_outliers = (

        df_outliers

        .groupby("metrica")

        .size()

        .reset_index(

            name="quantidade"

        )

    )


    resumo_outliers.to_csv(

        PASTA_SAIDA / "resumo_outliers.csv",

        index=False,

        encoding="utf-8-sig"

    )


# ============================================================
# CORRELAÇÃO
# ============================================================

print("\nCalculando matriz de correlação...")


correlacao = df[

    METRICAS_CORRELACAO

].corr()


correlacao.to_csv(

    PASTA_SAIDA / "correlacao.csv",

    encoding="utf-8-sig"

)


print()

print(correlacao)


# ============================================================
# HISTOGRAMAS
# ============================================================

print("\nGerando histogramas...")


gerar_histograma(

    df,

    "psnr",

    "Distribuição dos valores de PSNR",

    "PSNR (dB)"

)


gerar_histograma(

    df,

    "ssim",

    "Distribuição dos valores de SSIM",

    "SSIM"

)


gerar_histograma(

    df,

    "fsim",

    "Distribuição dos valores de FSIM",

    "FSIM"

)


gerar_histograma(

    df,

    "histSim",

    "Distribuição da Similaridade de Histogramas",

    "Similaridade"

)


gerar_histograma(

    df,

    "chi2",

    "Distribuição dos valores de Chi-Square",

    "Chi-Square",

    bins=50

)


gerar_histograma(

    df,

    "rsDiff",

    "Distribuição dos valores de RS-Diff",

    "RS-Diff"

)


gerar_histograma(

    df,

    "maxBytes",

    "Distribuição da Capacidade Máxima",

    "Bytes"

)


gerar_histograma(

    df,

    "bpp",

    "Distribuição de Bits por Pixel",

    "BPP"

)


# ============================================================
# BOXPLOTS
# ============================================================

print("Gerando boxplots...")


# PSNR

gerar_boxplot(

    [

        df["psnr"].dropna()

    ],

    [

        "PSNR"

    ],

    "Distribuição dos valores de PSNR",

    "PSNR (dB)",

    "boxplot_psnr.png"

)


# Similaridade

gerar_boxplot(

    [

        df["ssim"].dropna(),

        df["fsim"].dropna()

    ],

    [

        "SSIM",

        "FSIM"

    ],

    "Distribuição das métricas de similaridade",

    "Valor",

    "boxplot_similaridade.png"

)


# Segurança

gerar_boxplot(

    [

        df["histSim"].dropna(),

        df["rsDiff"].dropna()

    ],

    [

        "HistSim",

        "RS-Diff"

    ],

    "Distribuição das métricas de segurança",

    "Valor",

    "boxplot_seguranca.png"

)


# Capacidade

gerar_boxplot(

    [

        df["maxBytes"].dropna()

    ],

    [

        "MaxBytes"

    ],

    "Distribuição da capacidade máxima",

    "Bytes",

    "boxplot_capacidade.png"

)


# BPP

gerar_boxplot(

    [

        df["bpp"].dropna()

    ],

    [

        "BPP"

    ],

    "Distribuição de bits por pixel",

    "Bits por pixel",

    "boxplot_bpp.png"

)


# ============================================================
# HEATMAP DE CORRELAÇÃO
# ============================================================

print("Gerando heatmap de correlação...")


plt.figure(

    figsize=(12, 10)

)


plt.imshow(

    correlacao.values,

    aspect="auto",

    vmin=-1,

    vmax=1

)


plt.colorbar(

    label="Correlação"

)


plt.xticks(

    range(

        len(METRICAS_CORRELACAO)

    ),

    METRICAS_CORRELACAO,

    rotation=45,

    ha="right"

)


plt.yticks(

    range(

        len(METRICAS_CORRELACAO)

    ),

    METRICAS_CORRELACAO

)


for i in range(

    len(METRICAS_CORRELACAO)

):

    for j in range(

        len(METRICAS_CORRELACAO)

    ):

        valor = correlacao.iloc[i, j]


        plt.text(

            j,

            i,

            f"{valor:.2f}",

            ha="center",

            va="center",

            fontsize=8

        )


plt.title(

    "Matriz de Correlação entre Métricas"

)


salvar_figura(

    "correlacao.png"

)


# ============================================================
# CAPACIDADE X QUALIDADE
# ============================================================

print("Gerando gráficos comparativos...")


# BPP x SSIM

plt.figure(figsize=(10, 6))


plt.scatter(

    df["bpp"],

    df["ssim"],

    alpha=0.6

)


plt.xlabel(

    "Bits por Pixel (BPP)"

)


plt.ylabel(

    "SSIM"

)


plt.title(

    "Capacidade de Ocultação vs Similaridade"

)


plt.grid(alpha=0.3)


salvar_figura(

    "capacidade_vs_ssim.png"

)


# BPP x PSNR

plt.figure(figsize=(10, 6))


plt.scatter(

    df["bpp"],

    df["psnr"],

    alpha=0.6

)


plt.xlabel(

    "Bits por Pixel (BPP)"

)


plt.ylabel(

    "PSNR (dB)"

)


plt.title(

    "Capacidade de Ocultação vs PSNR"

)


plt.grid(alpha=0.3)


salvar_figura(

    "capacidade_vs_psnr.png"

)


# ============================================================
# RELATÓRIO TEXTUAL
# ============================================================

print("Gerando resumo textual...")


with open(

    PASTA_SAIDA / "resumo_geral.txt",

    "w",

    encoding="utf-8"

) as arquivo:


    arquivo.write("=" * 60 + "\n")

    arquivo.write("ANÁLISE DOS RESULTADOS EXPERIMENTAIS\n")

    arquivo.write("=" * 60 + "\n\n")


    arquivo.write(

        f"Total de imagens analisadas: "

        f"{total_imagens}\n\n"

    )


    arquivo.write("IMPERCEPTIBILIDADE\n")

    arquivo.write("-" * 40 + "\n")

    arquivo.write(

        f"PSNR médio: "

        f"{df['psnr'].mean():.6f} dB\n"

    )

    arquivo.write(

        f"PSNR mediano: "

        f"{df['psnr'].median():.6f} dB\n"

    )

    arquivo.write(

        f"PSNR mínimo: "

        f"{df['psnr'].min():.6f} dB\n"

    )

    arquivo.write(

        f"PSNR máximo: "

        f"{df['psnr'].max():.6f} dB\n"

    )

    arquivo.write(

        f"SSIM médio: "

        f"{df['ssim'].mean():.8f}\n"

    )

    arquivo.write(

        f"FSIM médio: "

        f"{df['fsim'].mean():.8f}\n\n"

    )


    arquivo.write("SEGURANÇA\n")

    arquivo.write("-" * 40 + "\n")

    arquivo.write(

        f"HistSim médio: "

        f"{df['histSim'].mean():.8f}\n"

    )

    arquivo.write(

        f"Chi-Square médio: "

        f"{df['chi2'].mean():.6f}\n"

    )

    arquivo.write(

        f"RS-Diff médio: "

        f"{df['rsDiff'].mean():.6f}\n\n"

    )


    arquivo.write("CAPACIDADE\n")

    arquivo.write("-" * 40 + "\n")

    arquivo.write(

        f"MaxBytes médio: "

        f"{df['maxBytes'].mean():.2f}\n"

    )

    arquivo.write(

        f"MaxBytes máximo: "

        f"{df['maxBytes'].max():.0f}\n"

    )

    arquivo.write(

        f"BPP médio: "

        f"{df['bpp'].mean():.8f}\n\n"

    )


    arquivo.write("RECUPERABILIDADE\n")

    arquivo.write("-" * 40 + "\n")

    arquivo.write(

        f"BER médio: "

        f"{df['ber'].mean():.8f}\n"

    )

    arquivo.write(

        f"BER máximo: "

        f"{df['ber'].max():.8f}\n"

    )

    arquivo.write(

        f"Recuperação perfeita: "

        f"{percentual_ber_zero:.2f}%\n\n"

    )


    arquivo.write("OUTLIERS\n")

    arquivo.write("-" * 40 + "\n")

    arquivo.write(

        f"Total de outliers detectados: "

        f"{len(df_outliers)}\n"

    )


# ============================================================
# FINALIZAÇÃO
# ============================================================

print()

print("=" * 60)

print("ANÁLISE FINALIZADA COM SUCESSO!")

print(

    f"Resultados salvos em: "

    f"{PASTA_SAIDA.resolve()}"

)

print("=" * 60)