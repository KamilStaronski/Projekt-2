#include <iostream>
#include <vector>
#include <cmath>

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
//private:

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
	static const int N = 3;
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

class Skalowanie: public Przeksztalcenie_macierza{
public:
	Skalowanie(double skala){//konstruktor ze skala jednakowa
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
public:

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

//class Rzutowanie_ogolne:public Przeksztalcenie_macierza {
//public:

//private:
//};

class Rzut_prostopadly :public Przeksztalcenie_macierza {//bez rzutu ogolnego
private:

public:

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

class Obsluga_uzytkownika {
private:
	vector <Figura> aktualne_figury;
	vector <Figura> nowe_figury;
	void utworz_figure() { 
		Figura fig; 
		int rozmiar_figury;
		cout << "podaj rozmiar figury" << endl;
		cin >> rozmiar_figury;
		for (int i = 0; i < rozmiar_figury; i++) {
			cout << "podaj punkt (3 liczby) " << endl;
			double x, y, z;
			cin >> x >> y >> z;
			Punkt p(x, y, z);
			fig.dodaj_punkt(p);
		}
		aktualne_figury.push_back(fig);
	}

	void stworz_nowe_figury(Przeksztalcenie* p) {
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
		cout << "czy chcesz skalowac wszystkie wspolrzedne jednakowo" << endl;
		bool ktora_skala;
		cin >> ktora_skala;
		if(ktora_skala==true){
			double skala;
			cout << "podaj jedna skale ";
			cin >> skala;
			Skalowanie* skalowanie = new Skalowanie(skala);
			stworz_nowe_figury(skalowanie);
		}
		else {
			double skala_x, skala_y, skala_z;
			cout << "podaj jedna skale dla x, dla y, dla z";
			cin >> skala_x >> skala_y >> skala_z;
			Skalowanie* skalowanie3 = new Skalowanie(skala_x,skala_y,skala_z);
			stworz_nowe_figury(skalowanie3);
		}
	}

	void obroc_figure() {
		cout << "podaj kat w radianach" << endl;
		double kat;
		cin >> kat;
		kat *= (M_PI / 180);
		cout << "podaj os obrotu" << endl;
		char os;
		cin >> os;
		cout << "podaj os obrotu" << endl;
		if (os == 'z') {
			Obrot* obiekt_obrotu = new Obrot_XY(kat);
			stworz_nowe_figury(obiekt_obrotu);
		}
		else if (os == 'y') {
			Obrot* obiekt_obrotu = new Obrot_XZ(kat);
			stworz_nowe_figury(obiekt_obrotu);
		}
		else if (os == 'x') {
			Obrot* obiekt_obrotu = new Obrot_YZ(kat);
			stworz_nowe_figury(obiekt_obrotu);
		}
		else {
			cout << "error" << endl;
		}

		
	}
public:
	bool krok(){
	
		//stworz_figure(){  podaj rozmiar figury };
			string komenda;
			cout << "podaj komende: " << endl;
			cin >> komenda;//zrobic na switchach
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

			}
			else if (komenda == "pf") {//przesun figury

			}
			else if (komenda == "przenies") {//przenies figury
				przenies_figury();
			}
			else if (komenda == "koniec") {//koniec progrmu
				return true;
			}
			return false;
		}

	void uruchom(){
		bool koniec = false;
		do {
			koniec = krok();
		} while (!koniec);
	}

	void menu(){}//dopidac menu uzytkownika
	//dopiac ze domyslnie dla kazdej figury wykonywane sa operacje
};

int main()
{
 	Obsluga_uzytkownika nowa_gra;
	nowa_gra.uruchom();
	nowa_gra.menu();
}
