WHERE := $(shell uname -s)

ifeq ($(WHERE), FreeBSD)
CXX = clang++
else
CXX = g++
endif

CXXFLAGS = -std=c++11 -Wall -g
LIBS = `pkg-config --cflags --libs opencv4`
TARGETS = compara objetos decode encode fatAttempt outra imagem tentativaFat

SRCPATH = src/
BINPATH = bin/
ARCHIVEPATH = archive/

# Alvos para cada programa
all: $(TARGETS)

objetos: $(SRCPATH)objetos.cpp
	$(CXX) $(CXXFLAGS) $(SRCPATH)objetos.cpp -o $(BINPATH)objetos $(LIBS) && ./$(BINPATH)objetos

compara: $(ARCHIVEPATH)compara.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)compara.cpp -o $(BINPATH)compara $(LIBS)

decode: $(ARCHIVEPATH)decode.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)decode.cpp -o $(BINPATH)decode $(LIBS)

encode: $(ARCHIVEPATH)encode.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)encode.cpp -o $(BINPATH)encode $(LIBS)

tentativaFat: $(ARCHIVEPATH)tentativaFat.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)tentativaFat.cpp -o $(BINPATH)tentativaFat $(LIBS)

fatAttempt: $(ARCHIVEPATH)fatAttempt.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)fatAttempt.cpp -o $(BINPATH)fatAttempt $(LIBS)

outra: $(ARCHIVEPATH)outra.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)outra.cpp -o $(BINPATH)outra $(LIBS) -g

imagem: $(ARCHIVEPATH)imagem.cpp
	$(CXX) $(CXXFLAGS) $(ARCHIVEPATH)imagem.cpp -o $(BINPATH)imagem $(LIBS)

# Limpar arquivos executáveis
clean:
	@ rm -rf $(BINPATH)*
	@ rm -rf *.o
	@ rm -rf *.core
	@ find . -type f -name '*.png' ! -name 'cg.png' -delete

.PHONY: all clean