#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;


int main(void);

int
main(void)
{
		  char nome[100], nome_out[100];
		  Mat img, img_sobel, imgO;
		  int ksize=0;

		  cout << "Digite o nome da imagem: ";
		  cin >> nome;

		  imgO=imread(nome);
		  img=imread(nome,0);

		  cout << "X =" << img.rows << " Y = " << img.cols << 
					 " Depth = " << img.depth() << " Channels = " << 
					 img.channels() << endl;

		  /*
			* A funcao Sobel, o kernel deve ser um valor impar entre 3 e 31
			*/
		  do
		  {
		  		cout << "Digite o tamanho do kernel do sobel: ";
		  		cin >> ksize;
		  }
		  while ( ksize  < 3 || ksize > 31);

		  Sobel(img, img_sobel, CV_8U, 1, 0, ksize, 1, 1, BORDER_DEFAULT);

		  cout << "Digite o nome da imagem de saida: ";
		  cin >> nome_out;

		  imshow("Original", imgO);
		  imshow("P&B", img);
		  imshow("Sobel", img_sobel);
		  waitKey(0);

		  imwrite(nome_out,img_sobel);

		  return 0;
}
