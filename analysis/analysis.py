import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# === CONFIGURAÇÃO ===
csv_file = 'analysis/metrics.csv'
base_output_dir = 'analysis/plots' # Diretório principal para todos os gráficos

# Não é mais necessário criar o diretório base aqui, 
# pois o script criará as subpastas conforme necessário.

print(f"Lendo dados de: {csv_file}")

# === 1. LER O ARQUIVO CSV ===
try:
    df = pd.read_csv(csv_file)
except FileNotFoundError:
    print(f"ERRO: Arquivo CSV não encontrado em '{csv_file}'. Verifique o caminho.")
    exit()

# === 2. TRATAR AS COLUNAS NUMÉRICAS ===
# Seleciona todas as colunas, exceto a primeira ('image')
numeric_cols = df.columns[1:] 

print("Métricas encontradas:", list(numeric_cols))

# Garante que os dados sejam numéricos (float), tratando vírgulas como decimais
for col in numeric_cols:
    # Converte para string, troca vírgula por ponto, e depois converte para numérico
    df[col] = pd.to_numeric(df[col].astype(str).str.replace(',', '.'), errors='coerce')
    
# Remove linhas onde qualquer valor numérico não pôde ser convertido
df.dropna(subset=numeric_cols, inplace=True)


# === 3. GERAR GRÁFICOS INDIVIDUAIS PARA CADA MÉTRICA ===

# Itera sobre cada coluna de métrica para criar sua pasta e gráficos
for col in numeric_cols:
    print(f"-> Gerando gráficos para '{col}'...")
    
    # --- NOVO: Criar uma pasta dedicada para cada métrica ---
    metric_dir = os.path.join(base_output_dir, col)
    os.makedirs(metric_dir, exist_ok=True)

    # --- Histograma Individual (salvando na nova pasta) ---
    plt.figure(figsize=(10, 6))
    sns.histplot(data=df, x=col, kde=True, bins=30)
    plt.title(f'Distribuição de {col}')
    plt.xlabel('Valor')
    plt.ylabel('Frequência')

    # Adiciona linha de referência específica para PSNR
    if 'psnr' in col.lower():
        plt.axvline(x=40, color='r', linestyle='--', linewidth=2, label='Qualidade Mínima (40 dB)')
        plt.legend()

    plt.tight_layout()
    # Salva o gráfico na pasta da métrica
    plt.savefig(os.path.join(metric_dir, f'{col}_distribuicao.png'))
    plt.close()

    # --- NOVO: Boxplot Individual (salvando na nova pasta) ---
    plt.figure(figsize=(8, 6))
    sns.boxplot(y=df[col]) # Usar 'y=' cria um boxplot vertical para uma única métrica
    plt.title(f'Boxplot de {col}')
    plt.ylabel('Valores')
    
    plt.tight_layout()
    # Salva o gráfico na pasta da métrica
    plt.savefig(os.path.join(metric_dir, f'{col}_boxplot.png'))
    plt.close()


# O boxplot geral foi removido, conforme solicitado.

print(f"\n✅ Gráficos exportados com sucesso!")
print(f"Verifique as subpastas dentro de: {base_output_dir}/")