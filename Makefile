WHERE := $(shell uname -s)

ifeq ($(WHERE), FreeBSD)
CXX = clang++
else
CXX = g++
endif

CXXFLAGS = -std=c++11 -Wall -g
LIBS = `pkg-config --cflags --libs opencv4`

SRCPATH = src/
BINPATH = bin/
ARCHIVEPATH = archive/

# Detectar arquivos .cpp no diretório src/ e archive/
SRCFILES := $(wildcard $(SRCPATH)*.cpp)
ARCHIVEFILES := $(wildcard $(ARCHIVEPATH)*.cpp)

# Gerar a lista de binários correspondentes
SRCTARGETS := $(SRCFILES:$(SRCPATH)%.cpp=$(BINPATH)%)
ARCHIVETARGETS := $(ARCHIVEFILES:$(ARCHIVEPATH)%.cpp=$(BINPATH)%)

TARGETS := $(SRCTARGETS) $(ARCHIVETARGETS)

# Criação do diretório de binários, caso não existam
$(shell mkdir -p $(BINPATH))
$(shell mkdir -p resources/output_images)

# Regras para compilar arquivos .cpp
$(BINPATH)%: $(SRCPATH)%.cpp
	-$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

$(BINPATH)%: $(ARCHIVEPATH)%.cpp
	-$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

# Compilar apenas os arquivos de src/
src: $(SRCTARGETS)

# Compilar apenas os arquivos de archive/
archive: $(ARCHIVETARGETS)

# Alvos para cada programa
all: $(TARGETS)

# Limpar arquivos executáveis e imagens
clean:
	@ rm -rf $(BINPATH)*
	@ rm -rf *.o
	@ rm -rf *.core
	@ find . -type f -name '*.png' ! -name 'cg.png' -delete

.PHONY: all clean