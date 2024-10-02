CXX = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = `pkg-config --cflags --libs opencv4`

# Alvos para cada programa
all: compara decode denovo imagem

compara: compara.cpp
	$(CXX) $(CXXFLAGS) compara.cpp -o compara $(LIBS)

decode: decode.cpp
	$(CXX) $(CXXFLAGS) decode.cpp -o decode $(LIBS)

denovo: denovo.cpp
	$(CXX) $(CXXFLAGS) denovo.cpp -o denovo $(LIBS)

imagem: imagem.cpp
	$(CXX) $(CXXFLAGS) imagem.cpp -o imagem $(LIBS)

# Limpar arquivos executáveis
clean:
	rm -f compara decode denovo imagem
