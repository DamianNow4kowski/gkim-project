1. Struktura katalogów projektu

	* /bin  - wszystkie wykonywalne skompilowane w ramach projektu
	* /build - folder dla kompilatora
	* /include - folder na pliki naglowkowe zewnetrznych pakietow (tzw. 3-rd party)
	* /lib - folder na biblioteki kompilowane w projekcie
	* /src - caly kod ktory tworzymy laduje w plikach w tym folderze
	* /test - kod testow jednostkowych (unit test) [prawdopodobnie niepotrzebny - jak starczy czasu to moze, moze]
	* /doc - dokumentacja

2. Makefile - czyli latwa kompilacja

	Na razie dostepna jest tylko komenda podstawowa czyli podstawowa kompilacja plikow, czyli wpisanie w głównym katalogu komendy: `make`, a po prawidlowej kompilacji wpisujac `bin/app` powinien sie nam uruchomic program w konsoli.

3. Do ustalenia

	Wersja SDL: 2.0.x | 1.2.x
	Komentarze: PL | EN
	Styl: 2 Spacje, 3 Spacje, 1 Tab
	Klamerki: 

	* w tym samym wierszu; przykład:
	```
	void func() {
		return;
	}
	```
	* w nastepnym wierszu; przykład:
	```
	void func()
	{
		return;
	}
	```
	Opis funkcji:
	
	* jakiś standard; przykład:
	```
	/**
	 * @global (funkcja globalna)
	 * @param int a czynnik
	 * @param int b czynnik
	 * @return int wynik mnozenia
	 */
	int mul(int a, int b) { return a*b; }
	```

	* brak standardu;
	* inne:;
		
	

