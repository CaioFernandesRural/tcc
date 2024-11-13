// Funçoes apenas relacionadas aos eventos
// de abertura, fechamento, leitura e escrita 
// da imagem
#include <iostream>
#include <string>
#include <opencv/core.hpp>

File *Steganopragh::open(void)
{
		  this->fp = fopen(_fname);
		  return this->fp;
}

