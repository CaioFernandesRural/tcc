WHERE := $(shell uname -s)

ifeq ($(WHERE), FreeBSD)
CXX = clang++
else
CXX = g++
endif

CXXFLAGS = -std=c++11 -Wall -g
LIBS = `pkg-config --cflags --libs opencv4`
TARGETS = compara objetos decode encode fatAttempt outra imagem tentativaFat

# Alvos para cada programa
all: $(TARGETS)

objetos: objetos.cpp
	$(CXX) $(CXXFLAGS) objetos.cpp -o objetos $(LIBS)

compara: compara.cpp
	$(CXX) $(CXXFLAGS) compara.cpp -o compara $(LIBS)

decode: decode.cpp
	$(CXX) $(CXXFLAGS) decode.cpp -o decode $(LIBS)

encode: encode.cpp
	$(CXX) $(CXXFLAGS) encode.cpp -o encode $(LIBS)

tentativaFat: tentativaFat.cpp
	$(CXX) $(CXXFLAGS) tentativaFat.cpp -o tentativaFat $(LIBS)

fatAttempt: fatAttempt.cpp
	$(CXX) $(CXXFLAGS) fatAttempt.cpp -o fatAttempt $(LIBS)

outra: outra.cpp
	$(CXX) $(CXXFLAGS) outra.cpp -o outra $(LIBS) -g

imagem: imagem.cpp
	$(CXX) $(CXXFLAGS) imagem.cpp -o imagem $(LIBS)

# Limpar arquivos executáveis
clean:
	@ rm -rf $(TARGETS)
	@ rm -rf *.o
	@ rm -rf *.core
	@ find . -type f -name '*.png' ! -name 'cg.png' -delete

.PHONY: all clean
