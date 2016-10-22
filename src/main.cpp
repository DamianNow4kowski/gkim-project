#include <iostream>

using namespace std;

int main() 
{
	int liczba;
	cout << "Hello friend!\nPlease take care of me well!" << endl;
	cout << "-------------------------------"  << endl;
	cout << "Wpisz liczbe: "; cin >> liczba;
	cout << "Ta liczba jest " 
		<< ((liczba % 2 == 0) ? "parzysta" : "nieparzysta") 
		<< ".\n\n"; 
	
	
	return 0;
}
