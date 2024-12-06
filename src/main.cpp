#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <nlohmann/json.hpp> // Biblioteca para manipulação de JSON
#include <stdexcept>         // Para runtime_error

using namespace cv;
using namespace std;
using json = nlohmann::json;


bool par(size_t x)
{
		  return !( x % 2 );
}

class Stegnograph
{
private:

		  string fileconfig;
		  Mat		imgIn;
		  Mat		imgOut;
		  Mat		imgSobel;
		  Mat		imgGray;
		  Mat		imgBinary;

		  size_t		_blocklen;
		  size_t		_ksize;
		  size_t		_threshold;
		  string		_msg;
		  string		_fnameinput;
		  string		_fnameoutput;
		  string		_imgoutputformat;
		  string		_configname;


		  string		_types="jpgjpegpnggiftiff";


public:
    
		  enum TypeInt	{KSIZE, THRESHOLD, BLOCKLEN};
		  enum TypeStr {INPUTNAME, OUTPUTNAME, MSG, IMGFORMAT, CONFIGNAME};

		  Stegnograph(){}
		  Stegnograph(string arquivo){}
		  ~Stegnograph(){}

		  bool readConfig(void);
		  bool readConfig(string str);
		  bool setConfig(size_t val, TypeInt t);
		  bool setConfig(string val, TypeStr t);
		  size_t getConfig( TypeInt t);
		  string getConfig( TypeStr t);
		  bool openConfig(void);
		  bool openConfig( string str);
		  void printConfig(void);
};


bool Stegnograph::readConfig(void){}
bool Stegnograph::readConfig(string str){}

bool Stegnograph::openConfig(void)
{
		  return this->openConfig(this->getConfig(CONFIGNAME));
}

bool Stegnograph::openConfig(string str)
{
		  ifstream file;

		  file.open(str, std::ios::in);
		  return (file.is_open());
}


bool Stegnograph::setConfig(size_t val, TypeInt t)
{
		  bool r;

		  r = true;
		  switch ( t )
		  {
					 case	KSIZE:
								if (( val < 3 || val > 31 ) && !par(val) )
										  cerr << "Ksize must be odd and must be between 3 and 31" << endl;
								else
								{
										  _ksize = val;
										  r = false;
								}
								break;
					case THRESHOLD:
								if ( val < 0 || val > 200 )
										  cerr << "Threshold must be between 1 and 200" << endl;
								else
								{
										  r = false;
										  _threshold = val;
								}
					case BLOCKLEN:
								if ( val  < 28 )
										  cerr << "Block len must be equal or greater than28" << endl;
								else
								{
									  r = false;
						 				_blocklen = val;
		  						}		
		  }
		  return 0;
}
bool Stegnograph::setConfig(string val, TypeStr t)
{
		  bool r;

		  r = true;
		  switch ( t )
		  {
					 case	INPUTNAME:
								if ( val.size() < 3  )
										  cerr << "Ksize must be odd and must be between 3 and 31" << endl;
								else
								{
										  r = false;
								}
								break;
					 case OUTPUTNAME:
								if ( val.size() < 3200 )
										  cerr << "Threshold must be between 1 and 200" << endl;
								else
								{
										  r = false;
								}
								break;
					case MSG:
								if ( val.size() < 3)
										  cerr << "Block len must be equal or greater than28" << endl;
								else
								{
									  r = false;
		  						}
								break;
					case IMGFORMAT:
								size_t pos;
								if ( (val.size() < 3) || _types.find(val, pos) != string::npos )
										  cerr << "Invalid string " << val << endl;
								else
								{
										  r = false;
										  _imgoutputformat = val;
								}
								break;
					case CONFIGNAME:
								if ( val.size() < 3 )
										  cerr << "File name of config Invalid " << val << endl;
								else
								{
										  r =false;
										  _configname = val;
								}		
								break;
		  }
		  return 0;
}

string Stegnograph::getConfig(TypeStr t)
{
		  string str;

		  switch ( t )
		  {
					 case	INPUTNAME:
								break;
					 case OUTPUTNAME:
								break;
					case MSG:
								break;
					case IMGFORMAT:
								break;
					case CONFIGNAME:
								str = _configname;
								break;
		  }
		  return str;
}
size_t Stegnograph::getConfig(TypeInt t){}


void Stegnograph::printConfig(void)
{
		  cout << "Config Configuration" << endl;
		  cout << "====================" << endl;
		  cout << "Input image file name .......: " << _fnameinput << endl;
		  cout << "Output image file name ......: " << _fnameoutput << endl;
		  cout << "Config file .................: " << this->getConfig(CONFIGNAME) << endl;
		  cout << "Output image format .........: " << _imgoutputformat << endl;
		  cout << "Block length ................: " << _blocklen << endl;
		  cout << "Threshold ...................: " << _threshold << endl;
		  cout << "Sobel kernel size ...........: " << _ksize << endl; 
		  cout << "Menssage ....................: " << _msg << endl;
}


int main()
{
    Stegnograph app;

	 app.printConfig();
	 app.setConfig(10, Stegnograph::KSIZE);
	 app.setConfig("PNG", Stegnograph::IMGFORMAT);
	 app.setConfig("config.json", Stegnograph::CONFIGNAME);
	 app.setConfig(200, Stegnograph::THRESHOLD);
	 app.setConfig(29, Stegnograph::BLOCKLEN);
	 app.printConfig();

    return 0;
}
