import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from PIL import Image
import os

# === CONFIGURAÇÃO ===
csv_file = 'metrics.csv'
base_output_dir = 'analysis/plots'
input_images_dir = '../resourses/input_images'
output_images_dir = '../resourses/output_images'

# Cria diretório base se não existir
os.makedirs(base_output_dir, exist_ok=True)

# === 1. LER O ARQUIVO CSV ===
try:
    df = pd.read_csv(csv_file)
except FileNotFoundError:
    print(f"ERRO: Arquivo CSV não encontrado em '{csv_file}'")
    exit()

# === 2. TRATAR COLUNAS NUMÉRICAS ===
numeric_cols = df.columns[1:]  # todas menos 'image'
for col in numeric_cols:
    df[col] = pd.to_numeric(df[col].astype(str).str.replace(',', '.'), errors='coerce')
df.dropna(subset=numeric_cols, inplace=True)

print("Métricas encontradas:", list(numeric_cols))

# === 3. GERAR GRÁFICOS INDIVIDUAIS POR MÉTRICA ===
for col in numeric_cols:
    metric_dir = os.path.join(base_output_dir, col)
    os.makedirs(metric_dir, exist_ok=True)

    # Histograma
    plt.figure(figsize=(10,6))
    sns.histplot(df[col], kde=True, bins=30)
    plt.title(f'Distribuição de {col}')
    plt.xlabel('Valor')
    plt.ylabel('Frequência')
    if 'psnr' in col.lower():
        plt.axvline(x=40, color='r', linestyle='--', linewidth=2, label='Qualidade Mínima (40 dB)')
        plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(metric_dir, f'{col}_distribuicao.png'))
    plt.close()

    # Boxplot
    plt.figure(figsize=(8,6))
    sns.boxplot(y=df[col])
    plt.title(f'Boxplot de {col}')
    plt.ylabel('Valores')
    plt.tight_layout()
    plt.savefig(os.path.join(metric_dir, f'{col}_boxplot.png'))
    plt.close()

# === 4. GERAR TABELAS RESUMIDAS ===
summary_stats = df[numeric_cols].agg(['mean','std']).T
summary_stats.rename(columns={'mean':'Média', 'std':'Desvio Padrão'}, inplace=True)
summary_stats.to_csv(os.path.join(base_output_dir, 'tabela_resumo.csv'))
print("📑 Tabela resumo salva em 'plots/tabela_resumo.csv'")

# === 5. FIGURA 1 – Comparação Original x Estego ===
fig_dir = os.path.join(base_output_dir, 'figura1')
os.makedirs(fig_dir, exist_ok=True)

for img_name in df['image']:
    orig_path = os.path.join(input_images_dir, img_name)
    stego_path = os.path.join(output_images_dir, img_name)
    if os.path.exists(orig_path) and os.path.exists(stego_path):
        fig, axs = plt.subplots(1, 2, figsize=(12,6))
        axs[0].imshow(Image.open(orig_path))
        axs[0].set_title('Original')
        axs[0].axis('off')

        axs[1].imshow(Image.open(stego_path))
        axs[1].set_title('Estego')
        axs[1].axis('off')

        plt.tight_layout()
        plt.savefig(os.path.join(fig_dir, f'{img_name}_comparacao.png'))
        plt.close()

# === 6. FIGURA 2 – Histogramas comparativos PSNR, SSIM, FSIM etc. ===
fig_dir2 = os.path.join(base_output_dir, 'figura2')
os.makedirs(fig_dir2, exist_ok=True)

# Exemplo: histograma comparativo PSNR
plt.figure(figsize=(10,6))
sns.histplot(df['psnr'], kde=True, color='blue', label='PSNR', bins=30)
plt.title('Histograma PSNR')
plt.xlabel('Valor')
plt.ylabel('Frequência')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(fig_dir2, 'psnr_comparativo.png'))
plt.close()

# === 7. FIGURA 4 – maxBytes x bpp ===
fig_dir4 = os.path.join(base_output_dir, 'figura4')
os.makedirs(fig_dir4, exist_ok=True)

df_grouped = df.groupby('image')[['maxBytes','bpp']].mean()
df_grouped.plot(kind='bar', figsize=(12,6))
plt.title('Comparação de Capacidade (maxBytes vs bpp)')
plt.ylabel('Valor Médio')
plt.tight_layout()
plt.savefig(os.path.join(fig_dir4, 'maxBytes_bpp.png'))
plt.close()

# === 8. FIGURA 5 – Boxplot BER ===
fig_dir5 = os.path.join(base_output_dir, 'figura5')
os.makedirs(fig_dir5, exist_ok=True)

plt.figure(figsize=(8,6))
sns.boxplot(y=df['ber'])
plt.title('Distribuição do BER (Robustez)')
plt.ylabel('Valores')
plt.tight_layout()
plt.savefig(os.path.join(fig_dir5, 'ber_boxplot.png'))
plt.close()

print("\n✅ Todas as Figuras e Tabelas foram geradas com sucesso!")
