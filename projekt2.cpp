#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define M_PI 3.14159265358979323846

using namespace std;

class Punkt {
private:
	double x, y, z;
public:
	Punkt():x(0), y(0), z(0) {}
	Punkt(double x_wczytywane, double y_wczytywane, double z_wczytywane): x(x_wczytywane), y(y_wczytywane), z(z_wczytywane){}
	void drukuj_wspolrzedne() {
		cout << "[" << x << ", " << y << ", " << z << "]";
	}
	double get_x() { return x; }
	double get_y() { return y; }
	double get_z() { return z; }
	bool operator!=(Punkt p2) {
		if (x == p2.x && y == p2.y && z == p2.z) {
			return false;
		}
		else {
			return true;
		}
	}
};

class Figura {
private:
	vector <Punkt> wierzcholki_figury;
public:
	Figura() {}
	int rozmiar(){
		return wierzcholki_figury.size();
	}

	Punkt operator[](int i) {//geter dla jednego punktu figury
		return wierzcholki_figury[i];
	}
	bool operator==(const Figura f2) {
		for (int i = 0; i < wierzcholki_figury.size(); i++) {
			if (wierzcholki_figury[i] != f2.wierzcholki_figury[i]) {
				return false;
			}
		}
		return true;
	}
	void dodaj_punkt(Punkt punkt) {
		wierzcholki_figury.push_back(punkt);
	}
	void drukuj_figure() {
		for (int i = 0; i < wierzcholki_figury.size(); i++) {
			wierzcholki_figury[i].drukuj_wspolrzedne();
		}
		cout << endl;
	}
};

class Przeksztalcenie {
public:
	virtual Punkt transformuj_punkt (Punkt p) = 0;//za kazdeym razem jest inne(raz macierz, raz wektor) wiec wirtualne, abstrakcyjne bo na poczatku jest puste
	Figura transformuj_figure(Figura f) {//transformujemy przechodzac po kazdym punkcie
		Punkt punkt_po_transf;
		Figura nowa_figura;
		for (int i = 0; i < f.rozmiar(); i++) {//rozmiar dla figury
			punkt_po_transf = transformuj_punkt(f[i]);//wywolanie dla kazdego punktu transformacji (operator[] z klasy Figura'pobranie punktu
			nowa_figura.dodaj_punkt(punkt_po_transf);
		}
		return nowa_figura;
	};
};

class Przeksztalcenie_wektorem_translacja : public Przeksztalcenie {//translacja
private:
	Punkt wektor;//klasa wektor jest identyczna z punktem
public:
	Przeksztalcenie_wektorem_translacja(Punkt wek): wektor(wek){}//konstruktor pobiera wektor
	Punkt transformuj_punkt(Punkt p) {//tworze translacje 
		double nowy_x, nowy_y, nowy_z;
		nowy_x = p.get_x() + wektor.get_x();
		nowy_y = p.get_y() + wektor.get_y();
		nowy_z = p.get_z() + wektor.get_z();
		Punkt nowy_punkt(nowy_x, nowy_y, nowy_z);
		return nowy_punkt;
	}
};

class Przeksztalcenie_macierza : public Przeksztalcenie {
protected: // bo uzywane w podklasach
	static const int  N = 3;
	double macierz[N][N];
public:
	Punkt transformuj_punkt(Punkt p) {
		//mnozenie macierzy*
		double tab_punktu[N];
		double tab_wyniku[N];//wynik
		tab_punktu[0] = p.get_x();
		tab_punktu[1] = p.get_y();
		tab_punktu[2] = p.get_z();
		for (int w = 0; w < N; w++) {
			tab_wyniku[w] = 0;
			for (int k = 0; k < N; k++) {
				tab_wyniku[w] += macierz[w][k] * tab_punktu[k];
			}
		}
		Punkt przetransformowany_punkt(tab_wyniku[0], tab_wyniku[1], tab_wyniku[2]) ;
		return przetransformowany_punkt;
	}
};

class Wyjatek : exception {
private:
	std::string napis;
public:
	Wyjatek( std::string nap):napis(nap) {}
	const char* what() const noexcept 
	{ 
		return napis.c_str(); 
	}
};

class Skalowanie: public Przeksztalcenie_macierza{
public:
	Skalowanie(double skala){//konstruktor ze skala jednakowa
		if (skala == 0) {
			Wyjatek ex("skala nie moze byc 0");
			throw ex;
		}
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
			for (int j = 0; j < Przeksztalcenie_macierza::N; j++) {
				macierz[i][j] = 0;
			}
		}
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
				macierz[i][i] = skala;
		}
	}

	Skalowanie(double skala_x, double skala_y, double skala_z){//konstruktor skalujacy róznie
		if (skala_x == 0 || skala_y == 0 || skala_z == 0) {
			Wyjatek ex("skala nie moze byc 0");
			throw ex;
		}
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
			for (int j = 0; j < Przeksztalcenie_macierza::N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[0][0] = skala_x;
		macierz[1][1] = skala_y;
		macierz[2][2] = skala_z;
	}
};

class Obrot : public Przeksztalcenie_macierza {
protected: 
	double kat;
};

class Obrot_XY :public Obrot {
public:
	Obrot_XY(double alfa) {
		kat = alfa;
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
			for (int j = 0; j < Przeksztalcenie_macierza::N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[0][0] = cos(alfa);
		macierz[0][1] = -sin(alfa);
		macierz[1][0] = sin(alfa);
		macierz[1][1] = cos(alfa);
		macierz[2][2] = 1;
	}
};

class Obrot_YZ :public Obrot {
public:
	Obrot_YZ(double alfa) {
		kat = alfa;
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
			for (int j = 0; j < Przeksztalcenie_macierza::N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[0][0] = 1;
		macierz[1][2] = -sin(alfa);
		macierz[2][1] = sin(alfa);
		macierz[1][1] = cos(alfa);
		macierz[2][2] = cos(alfa);
	}
};

class Obrot_XZ :public Obrot {
public:
	Obrot_XZ(double alfa) {
		kat = alfa;
		for (int i = 0; i < Przeksztalcenie_macierza::N; i++) {
			for (int j = 0; j < Przeksztalcenie_macierza::N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[0][0] = cos(alfa);
		macierz[0][2] = sin(alfa);
		macierz[1][1] = 1;
		macierz[2][0] = -sin(alfa);
		macierz[2][2] = cos(alfa);
	}
};

class Rzut_prostopadly :public Przeksztalcenie_macierza {//bez rzutu ogolnego
};

class Rzut_XY : public Rzut_prostopadly {
public:
	Rzut_XY() { 
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[0][0] = 1;
		macierz[1][1] = 1;
	}
};

class Rzut_YZ : public Rzut_prostopadly {
public:
	Rzut_YZ() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[2][2] = 1;
		macierz[1][1] = 1;
	}
};

class Rzut_XZ : public Rzut_prostopadly {
public:
	Rzut_XZ() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				macierz[i][j] = 0;
			}
		}
		macierz[2][2] = 1;
		macierz[0][0] = 1;
	}
};

struct sortuj_malejaco {//musi byc struktura
	bool operator()(Figura a, Figura b) const
	{//czy wieksze czy mniejsze
		return a.rozmiar() > b.rozmiar();//domyslnie zwraca wieksze
	}
};

class N_katy {
private:
	int n;
public:
	N_katy(int liczba) : n(liczba) {
		if (liczba <= 0) {
			Wyjatek ex("skala nie moze byc 0");
			throw ex;
		}
	}
	bool operator()(Figura a) const {//zlicz liczy jesli spelnia jakis waruenk
		if (a.rozmiar() == n) {
			return true;
		}
		else {
			return false;
		}
	}
};

class Obsluga_uzytkownika {
private:
	vector <Figura> aktualne_figury;
	vector <Figura> nowe_figury;

	bool bledny_int(std::string napis) {
		int licznik_kropek = 0;
		for (int i = 0; i < napis.size(); i++) {
			if (!('0' <= napis[i] && napis[i] <= '9')) {
				return true;
			}
		}
		return false;
	}

	int wczytujInt() {
		std::string napis;
		int k;
		do {
			cin >> napis;
			k = atoi(napis.c_str());
		} while (k<=0||bledny_int(napis));
		return k;
	}

	bool bledny_double(std::string napis) {
		int licznik_kropek = 0;
		int licznik_minusow = 0;
		for (int i = 0; i < napis.size(); i++) {
			if (!('0' <= napis[i] && napis[i] <= '9'||napis[i]=='.' || napis[0] == '-')) {
				return true;
			}
			else if(napis[i] == '.'){
				licznik_kropek++;
			}
			else if (napis[i] == '-') {
				licznik_minusow++;
			}
		}
		if (licznik_kropek > 1|| licznik_minusow>1) {
			return true;
		}
		return false;
	}

	double wczytujDouble() {
		std::string napis;
		double d;
		do {
			cin >> napis;
			d = atof(napis.c_str());
		} while (bledny_double(napis));
		return d;
	}

	void utworz_figure() { 
		Figura fig; 
		int rozmiar_figury;
		std::string kom;
		cout << "podaj rozmiar figury" << endl;
		rozmiar_figury = wczytujInt();
		cout << "jesli chcesz wylosowac wspolrzedne wpisz 'lw' (przedzial -100 do 100) lub cokolwiek"<<endl;
		cin >> kom;
		if (kom == "lw") {
			srand(time(0));
			for (int i = 0; i < rozmiar_figury; i++) {
				double x, y, z;
				x = -100 + (double)rand() / RAND_MAX * 200;
				y = -100 + (double)rand() / RAND_MAX * 200;
				z = -100 + (double)rand() / RAND_MAX * 200;
				Punkt p(x, y, z);
				fig.dodaj_punkt(p);
			}
		}
		else {
			for (int i = 0; i < rozmiar_figury; i++) {
				cout << "podaj punkt (3 liczby) " << endl;
				double x, y, z;
				x = wczytujDouble();
				y = wczytujDouble();
				z = wczytujDouble();
				Punkt p(x, y, z);
				fig.dodaj_punkt(p);
			}
		}
		auto it = find(aktualne_figury.begin(), aktualne_figury.end(), fig);
		if (it != aktualne_figury.end()) {
			Wyjatek ex("istnieje juz taka figura ");
			throw ex;
		}
		aktualne_figury.push_back(fig);
	}

	void stworz_nowe_figury(Przeksztalcenie* p) {
		if (aktualne_figury.size() == 0) {
			Wyjatek ex("brak figur do przeksztalcenia ");
			throw ex;
		}
		for (auto iterator = aktualne_figury.begin(); iterator != aktualne_figury.end(); iterator++) {
			Figura nowa_figura;
			nowa_figura = p->transformuj_figure(*iterator);
			nowe_figury.push_back(nowa_figura);
		}
	}

	void drukuj_figury() {
		cout << endl << "stare figury" << endl;
		for (auto iterator = aktualne_figury.begin(); iterator != aktualne_figury.end(); iterator++) {
			(*iterator).drukuj_figure();
		}
		cout << endl<<"nowe figury" << endl;
		for (auto iterator = nowe_figury.begin(); iterator != nowe_figury.end(); iterator++) {
			(*iterator).drukuj_figure();
		}
	}
	
	void przenies_figury() {
		aktualne_figury = nowe_figury;
		nowe_figury.clear();
	}

	void skaluj_figury() {
		cout << "czy chcesz skalowac wszystkie wspolrzedne jednakowo(wpisz 'tak' lub cokolwiek)?" << endl;
		std::string ktora_skala;
		cin >> ktora_skala;
		if(ktora_skala == "tak"){
			double skala;
			cout << "podaj jedna skale ";
			skala = wczytujDouble();
			Skalowanie* skalowanie = new Skalowanie(skala);
			stworz_nowe_figury(skalowanie);
			delete skalowanie;
		}
		else {
			double skala_x, skala_y, skala_z;
			cout << "podaj jedna skale dla x, dla y, dla z" << endl;
			cout << "dla x: ";
			skala_x = wczytujDouble(); 
			cout << endl<<"dla y: ";
			skala_y = wczytujDouble(); 
			cout <<endl<< "dla z: ";
			skala_z = wczytujDouble();;
			Skalowanie* skalowanie3 = new Skalowanie(skala_x,skala_y,skala_z);
			stworz_nowe_figury(skalowanie3);
			delete skalowanie3;
		}
	}

	void obroc_figure() {
		cout << "podaj kat w stopniach" << endl;
		double kat;
		kat = wczytujDouble();;
		kat *= (M_PI / 180);
		cout << "podaj os obrotu" << endl;
		char os;
		cin >> os;
		cout << "podaj os obrotu" << endl;
		if (os == 'z') {
			Obrot* obiekt_obrotu = new Obrot_XY(kat);
			stworz_nowe_figury(obiekt_obrotu);
			delete obiekt_obrotu;
		}
		else if (os == 'y') {
			Obrot* obiekt_obrotu = new Obrot_XZ(kat);
			stworz_nowe_figury(obiekt_obrotu);
			delete obiekt_obrotu;
		}
		else if (os == 'x') {
			Obrot* obiekt_obrotu = new Obrot_YZ(kat);
			stworz_nowe_figury(obiekt_obrotu);
			delete obiekt_obrotu;
		}
		else {
			cout << "nie ma takiej osi obrotu" << endl;
		}
	}

	void przesun_figure(){
		cout << "podaj wektor przesuniecia" << endl;
		double x, y, z;
		x = wczytujDouble();
		y = wczytujDouble();
		z = wczytujDouble();
		Punkt wektor(x, y, z);
		Przeksztalcenie_wektorem_translacja* obiekt_przesuniecia = new Przeksztalcenie_wektorem_translacja (wektor);
		stworz_nowe_figury(obiekt_przesuniecia);
		delete obiekt_przesuniecia;
	}

	void rzutuj_figure(){
		std::cout << "podaj plaszczyzne rzutowania XY/YZ/XZ: " << std::endl;
		string plaszczyzna;
		cin >> plaszczyzna;
		if (plaszczyzna == "XY") {
			Rzut_XY* obiekt_rzutowaniaXY = new Rzut_XY();
			stworz_nowe_figury(obiekt_rzutowaniaXY);
			delete obiekt_rzutowaniaXY;
		}
		else if (plaszczyzna == "XZ") {
			Rzut_XZ* obiekt_rzutowaniaXZ = new Rzut_XZ();
			stworz_nowe_figury(obiekt_rzutowaniaXZ);
			delete obiekt_rzutowaniaXZ;
		}
		else if (plaszczyzna == "YZ") {
			Rzut_YZ* obiekt_rzutowaniaYZ = new Rzut_YZ();
			stworz_nowe_figury(obiekt_rzutowaniaYZ);
			delete obiekt_rzutowaniaYZ;
		}
		else {
			cout << "nie ma takiej plaszczyzny" <<endl;
		}

	}

	void sortuj_figury() {
		sortuj_malejaco porownaj;
		cout << "domyslnie sortuje figury malejaco" << std::endl;
		std:: sort(aktualne_figury.begin(), aktualne_figury.end(), porownaj);
		std::sort(nowe_figury.begin(), nowe_figury.end(), porownaj);
	}

	void szukaj_wielokata() {
		std::cout << "podaj szukana ilosc wierzcholkow" << std::endl;
		int n;
		n=wczytujInt();
		N_katy wielokat(n);
		int ile_n_katow_starych = count_if(aktualne_figury.begin(), aktualne_figury.end(), wielokat);
		std::cout << "jest " << ile_n_katow_starych << " starych figur o " << n << " wierzcholkach " << endl;
		int ile_n_katow_nowych = count_if(nowe_figury.begin(), nowe_figury.end(), wielokat);
		std::cout << "jest " << ile_n_katow_nowych << " nowych figur o " << n << " wierzcholkach" << endl;
	}

	void odwroc_figury() {
		reverse(aktualne_figury.begin(), aktualne_figury.end());
		reverse(nowe_figury.begin(), nowe_figury.end());
	}

public:
	bool krok(){
			string komenda;
			cout << "podaj komende: " << endl;
			cin >> komenda;
			if (komenda == "uf") {//utworz fu=igure
				utworz_figure();
			}
			else if (komenda == "df") {//drukuj figure
				drukuj_figury();
			}
			else if (komenda == "sf") {//skaluj figury 
				skaluj_figury();
			}

			else if (komenda == "of") {//obroc figury
				obroc_figure();
			}
			else if (komenda == "rzf") {//rzutuj figury
				rzutuj_figure();
			}
			else if (komenda == "pf") {//przesun figury
				przesun_figure();
			}
			else if (komenda == "przenies") {//przenies figury
				przenies_figury();
			}
			else if (komenda == "sortuj") {
				sortuj_figury();
			}
			else if (komenda == "szukaj") {
				szukaj_wielokata();

			}
			else if (komenda == "odwroc") {
				odwroc_figury();
			}
			else if (komenda == "koniec") {//koniec progrmu
				return true;
			}
			else {
				cout << "nie ma takiej komendy" << endl;

			}
			return false;
		}

	void uruchom(){
		bool koniec = false;
		do {
			try {
				koniec = krok();
			}
			catch(Wyjatek e){
				cout << e.what()<<std::endl;
			}
		} while (!koniec);
	}

	void menu(){
		std::cout << "Program punkt trojwymiarowy" << std::endl;
		std::cout << "Lista dostepnych polecen:" << std::endl;
		std::cout << "Operacje na figurach wykonywane są dla wszystkich figur!" << std::endl;
		std::cout << "- uf - utworz figure" << std::endl;
		std::cout << "- df - drukuj figure" << std::endl;
		std::cout << "- sf - skaluj figure" << std::endl;
		std::cout << "- of - obroc figure" << std::endl;
		std::cout << "- rzf - rzutuj figure" << std::endl;
		std::cout << "- pf - przesun figure" << std::endl;
		std::cout << "- przenies - przenies figure - nadpisuje w miejsce pierwotnych figur figury po przeksztalceniach" << std::endl;
		std::cout << "- sortuj - sortuje figury malejaco" << std::endl;
		std::cout << "- szukaj - szuka figury o podanym rozmiarze wierzcholkow" << std::endl;
		std::cout << "- odwroc - odwraca kolejnosc figur" << std::endl;
		std::cout << "- koniec - koniec programu" << std::endl;
	}
};

int main()
{
 	Obsluga_uzytkownika nowa_gra;
	nowa_gra.menu();
	nowa_gra.uruchom();
}
