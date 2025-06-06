# caio


### FreeBSD

## Estrutura do Projeto

1. `archive` códigos-fonte "legados" que foram criados ao longo do projeto e podem conter código útil a ser reciclado.

2. `bin` arquivos binários de código c++ compilado.

3. `docs` documentos do TCC relacionado a este projeto

4. `resources` recursos utilizados pelo código, como imagens de entrada, de saída e arquivos de configuração

5. `src` código fonte principal do projeto

- `Makefile` responsável por compilar e limpar o projeto

## Makefile (compilação)

Através do Makefile o código é compilado com todas as dependências e flags necessárias automaticamente.

Temos dois diretórios de compiláveis `archive/` e `src/` podemos então compilar cada um deles separadamente com `make archive` e `make src`, respectivamente, ou apenas `make` para compilar tudo.

Os arquivos compilados serão colocados em `bin/` e as imagens em `resources/output_imagens` para limpálos usamos `make clean`.

* Ao usar qualquer comando ``make`` automaticamente `bin/` e `resources/output_imagens` serão criados caso não existam

## fatApp.cpp

Implementação de Esteganografia LSB com utilização de filtro de sobel para detecção de bordas e "tabela de endereçamento" para blocos de pixel.

---
# ✅ Testes a Implementar – Validação Esteganográfica (Principais Métricas)

Este checklist reúne **somente os testes mais importantes e amplamente utilizados** para validar um método de esteganografia de imagens, com base no artigo:

**"Digital Image Steganography Survey and Investigation – Signal Processing, 2023"**

---

## 0. Refatorção

- ~~Processamento independente~~
- ~~Só passar o nome da imagem, sem depender do config~~ (vai lendo o config iterativo)
- ~~Calcular o tamanho dos blocos de acordo com o tamanho da imagem~~
- ~~salvar os nomes das imagens de saída com seus Ns iniciais num json~~

## 🧪 1. Testes de Imperceptibilidade

> Avaliam a diferença entre a imagem original e a esteganografada.

- [x] **PSNR (Peak Signal-to-Noise Ratio)**  
  Medida objetiva de distorção (ideal ≥ 40 dB).

- [x] **SSIM (Structural Similarity Index)**  
  Compara percepção visual baseada em estrutura (ideal ≥ 0.9).

- [x] **FSIM (Feature Similarity Index)**  
  Mede similaridade com base em características visuais relevantes.

---

## 🔐 2. Testes de Segurança (Esteganálise)

> Avaliam se a imagem esteganografada pode ser detectada.

- [x] **Histogram Analysis**  
  Verifica visualmente e estatisticamente alterações na distribuição dos pixels.

- [x] **Chi-square Analysis**  
  Detecta padrões estatísticos típicos de métodos baseados em LSB.

- [x] **RS Analysis (Regular/Singular)**  
  Teste clássico para avaliar segurança de métodos baseados em bits.

- [-] **SPAM (Subtractive Pixel Adjacency Matrix)**  
  Análise universal com SVM baseada em padrões de vizinhança de pixels.

---

## 📦 3. Teste de Capacidade

> Avalia quanto de informação é possível esconder sem perder qualidade.

- [x] **Payload Capacity (em bits)**
- [x] **BPP (Bits per Pixel)**

---

## ⏱️ 4. Teste de Robustez (se aplicável)

> Avalia a integridade da mensagem após ataques ou compressões.

- [x] **BER (Bit Error Ratio)**  
  Compara bit a bit a mensagem original e a extraída.

---

## 📌 Recomendação para TCC

Se o seu método **não é robusto**, o mínimo recomendado é:

- ✅ PSNR  
- ✅ SSIM  
- ✅ Payload (bits e BPP)  
- ✅ Histogram + Chi-square ou RS  
- ✅ SPAM (opcional, mas recomendável)

Essas métricas são suficientes para um TCC bem fundamentado e comparável com o estado da arte.

---

