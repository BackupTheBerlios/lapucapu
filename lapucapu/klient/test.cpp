#include <fstream>
#include <iostream>

using namespace std;

int ddd()
{
	char * data[512];
	ifstream in("nowy_plik.txt");
	in.get(data[0], 512);
	
	return 0;	
}
