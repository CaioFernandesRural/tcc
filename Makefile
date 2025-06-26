WHERE := $(shell uname -s)

ifeq ($(WHERE), FreeBSD)
CXX = clang++
else
CXX = g++
endif

CXXFLAGS = -std=c++17 -Wall -g
LIBS += `pkg-config --libs opencv4`
CFLAGS += -g `pkg-config --cflags opencv4`

SRCPATH = src/
BINPATH = bin/
OBJPATH = obj/
ARCHIVEPATH = archive/

# Detectar arquivos .cpp no diretório src/
SRCFILES := $(wildcard $(SRCPATH)*.cpp)
OBJFILES := $(SRCFILES:$(SRCPATH)%.cpp=$(OBJPATH)%.o)

# Criação de diretórios necessários
$(shell mkdir -p $(BINPATH))
$(shell mkdir -p $(OBJPATH))
$(shell mkdir -p resources/output_images)

# Para os histogramas
$(shell mkdir -p resources/output_histograms)


# Alvo principal
bin/main: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Regra para compilar arquivos .cpp em objetos .o
$(OBJPATH)%.o: $(SRCPATH)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CFLAGS)

# Limpar arquivos compilados
clean:
	@rm -rf $(BINPATH)* $(OBJPATH)* resources/output_images/* resources/input_images/* resources/output_histograms/*

analysis_clean:
	@rm -rf analysis/plots/* analysis/metrics.csv

super_clean:
	@rm -rf $(BINPATH)* $(OBJPATH)* resources/output_images/* resources/input_images/* resources/output_histograms/* analysis/plots/* analysis/metrics.csv resources/image_base/*

.PHONY: all clean

# ===== Targets extras para baixar e preparar a base de imagens DIV2K =====

.PHONY: get_imagebase prepare_inputs

get_imagebase:
	@echo "Baixando base de imagens DIV2K (Train HR)..."
	@mkdir -p resources/image_base
	@wget -c https://data.vision.ee.ethz.ch/cvl/DIV2K/DIV2K_train_HR.zip -O resources/image_base/DIV2K_train_HR.zip
	@echo "Extraindo imagens..."
	@unzip -n resources/image_base/DIV2K_train_HR.zip -d resources/image_base/
	@echo "Base de imagens disponível em resources/image_base/DIV2K_train_HR/"

prepare_inputs:
	@echo "Preparando pasta resources/input_images/ com as imagens da base..."
	@mkdir -p resources/input_images
	@cp resources/image_base/DIV2K_train_HR/*.png resources/input_images/
	@echo "Cópia concluída. Input images disponíveis em resources/input_images/"
