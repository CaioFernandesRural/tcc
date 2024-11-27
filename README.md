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
