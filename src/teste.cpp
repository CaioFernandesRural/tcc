#include <iostream>

using namespace std;


class Objeto {

private:
		  int id = 0;
		  string __version="1.00";
		  string fn;

		  void countInstances(void)
		  {
					 static int lid =0;
					 id = lid ++;
		  }

public:
		  Objeto(){countInstances();}
		  Objeto(string name){ countInstances(); }

		  void Seila(void);

		  string version(void)
		  {
					 return __version;
		  }

		  void setName(string name)
		  {
					 fn = name;
		  }

		  string getName(void)
		  {
					 return fn;
		  }

		  int getId(void)
		  {
					 return id;
		  }

};

void Objeto::Seila(void)
{
		  cout << "Funcao Seila()" << endl;
}
int main(void)
{
		  Objeto o;
		  Objeto p;

		  cout << o.version() << endl;
		  o.setName("arquivo.txt");
		  cout << "Objeto O.ID = " << o.getId() << endl;
		  cout << "Objeto P.ID = " << p.getId() << endl;
		  o.Seila();
		 return 0; 
}


