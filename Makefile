CXX = clang++
CXXFLAGS = -std=c++11 -Wall -g
LIBS = `pkg-config --cflags --libs opencv4`
TARGETS = compara decode encode encodeFAT imagem

# Alvos para cada programa
all: compara decode encode encodeFAT imagem

compara: compara.cpp
	$(CXX) $(CXXFLAGS) compara.cpp -o compara $(LIBS)

decode: decode.cpp
	$(CXX) $(CXXFLAGS) decode.cpp -o decode $(LIBS)

encode: encode.cpp
	$(CXX) $(CXXFLAGS) encode.cpp -o encode $(LIBS)

encodeFAT: encodeFAT.cpp
	$(CXX) $(CXXFLAGS) encodeFAT.cpp -o encodeFAT $(LIBS)

imagem: imagem.cpp
	$(CXX) $(CXXFLAGS) imagem.cpp -o imagem $(LIBS)

# Limpar arquivos executáveis
clean:
	@ rm -f $(TARGETS)
	@ find . -type f -name '*.png' ! -name 'cg.png' -delete

.PHONY: all clean
