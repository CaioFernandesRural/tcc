WHERE!=uname -m
.if $(WHERE) == "amd64"
INCLUDE+=-I/usr/local/include/opencv4
LIBINCLUDE+=-L/usr/local/lib
.else
INCLUDE+=
LIBINCLUDE+=
.endif

iamge: imagem.cpp
	clang++ -o image imagem.cpp $(INCLUDE)  $(LIBINCLUDE) -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core


clean:
	@ rm -rf image
	@ rm -rf *.o
	@ rm -rf *.core
