WHERE!=uname -m
.if $(WHERE) == "amd64"
INCLUDE+=-I/usr/local/include/opencv4
LIBINCLUDE+=-L/usr/local/lib
.else
INCLUDE+=
LIBINCLUDE+=
.endif

all: imagem denovo

imagem: imagem.cpp
	clang++ -o imagem imagem.cpp -g $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core

denovo: denovo.cpp
	clang++ -o denovo denovo.cpp -g $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core


clean:
	@ rm -rf imagem denovo
	@ rm -rf *.o
	@ rm -rf *.core
