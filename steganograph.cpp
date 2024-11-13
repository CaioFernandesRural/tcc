
class Stegnograph {

		  private:
					 string _fname;	// Nome do arquivo ed imagem
					 string _text;		// Texto que será escrito na Imagem
					 size_t _len;		// Comprimento do texto em bits
					 Mat	  _img		// Imagem para gravar o texto
		  public :

					 Stegnograph();
					 Stegnograph(string fname); 
					 open();
					 open(string fname);
					 close(); 
					 ~Stegnograph(); 
					 getBlock(size_t len); 
					 getBlocks(); 
					 size_t getLenText(string text); 			// O tamanho do texto em bits
					 string cleanText(string text); 			// Remove os espaços do texto
					 uint8_t& convetToBinary(string text); 
					 string& convetToString(uint8_t& text); 
					 uint8_t saveToImage(uint8_t& text); 

};
