WHERE := $(shell uname -s)

ifeq ($(WHERE), FreeBSD)
CXX = clang++
else
CXX = g++
endif

CXXFLAGS = -std=c++11 -Wall -g
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

# Alvo principal
bin/main: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Regra para compilar arquivos .cpp em objetos .o
$(OBJPATH)%.o: $(SRCPATH)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CFLAGS)

# Limpar arquivos compilados
clean:
	@rm -rf $(BINPATH)* $(OBJPATH)* resources/output_images/*

.PHONY: all clean
