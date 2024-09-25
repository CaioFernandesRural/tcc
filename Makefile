WHERE!=uname -m
.if $(WHERE) == "amd64"
INCLUDE+=-I/usr/local/include/opencv4
LIBINCLUDE+=-L/usr/local/lib
.else
INCLUDE+=
LIBINCLUDE+=
.endif

BINS= 	image 	\
			decode	\
		  	denovo

all: $(BINS)

image: imagem.cpp
	clang++ -o image imagem.cpp $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core

decode: decode.cpp
	clang++ -o decode decode.cpp $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core

denovo: denovo.cpp
	clang++ -o denovo denovo.cpp $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core

clean:
	@ rm -rf $(BINS)
	@ rm -rf *.o
	@ rm -rf *.core

