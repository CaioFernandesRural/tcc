CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LIBS = `pkg-config --cflags --libs opencv4`
TARGETS = compara decode encode encodeFAT imagem

<<<<<<< HEAD
all: imagem denovo

imagem: imagem.cpp
	clang++ -o imagem imagem.cpp -g $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core

denovo: denovo.cpp
	clang++ -o denovo denovo.cpp -g $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core
=======
# Alvos para cada programa
all: compara decode encode encodeFAT imagem
>>>>>>> 5806a7dd036f253639dce05598b301dddb273c8e

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
<<<<<<< HEAD
	@ rm -rf imagem denovo
	@ rm -rf *.o
	@ rm -rf *.core
=======
	rm -f $(TARGETS)
	find . -type f -name '*.png' ! -name 'cg.png' -delete

.PHONY: all clean
>>>>>>> 5806a7dd036f253639dce05598b301dddb273c8e
