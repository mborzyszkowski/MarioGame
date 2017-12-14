#ifndef MODELGRY_H
#define MODELGRY_H

// pola w modelu definicji planszy
#define PUSTE '-'     
#define PODSTAWA '='
#define PRZESZKODA '+'
#define PLATFORMA '*'
#define PLATRORMA_Z_MONETA 'O'

// nazwy sekcji w pliku definicji etapu
#define HAED_F "Head"
#define PRZECIWNICY_F "Przeciwnicy"
#define PLANSZA_F "Plansza"

// nazwa pliku z zapisem gry
#define PLIK_SAVE "saveMario.sav"

// flagi bitowe bledow odczytu pliku z etapem
#define READ_ETAP_ERR_OPEN 1                          // blad odczytu pliku z etapem
#define READ_ETAP_ERR_BRAK_HEAD 2                     // brak naglowka HEAD_F
#define READ_ETAP_ERR_ODCZYT_HEAD 4                   // blad odczytu wartosci w head
#define READ_ETAP_ERR_BRAK_PRZECIWNICY 8              // brak naglowka PRZECIWNICY_F
#define READ_ETAP_ERR_ODCZYT_PRZECIWNICY_LICZBA 16    // blad odczytu liczby przeciwnikow
#define READ_ETAP_ERR_PRZECIWNICY_POZYCJA 32          // blad odczytu lub wartosci pozycji przeciwnika
#define READ_ETAP_ERR_BRAK_PLANASZA 64                // brak naglowka PLANSZA_F
#define READ_ETAP_ERR_PLANASZA_BRAK_NOWEJ_LINII 128   // linia definiujaca plansze nie jest zakonczona nowa linia
#define READ_ETAP_ERR_PLANASZA_NIEPOPRAWNY_ZNAK 256   // niepoprawny znak w linii definiujacej plansze
#define READ_ETAP_ERR_MARIO_ZLA_POZYCJA  512          // nipoprawnie okreskona pozycja Mario
#define READ_ETAP_ERR_PRZECIWNIK_ZLA_POZYCJA  1024    // nipoprawnie okreskona pozycja przeciwnika

struct Pozycja {
	int x, y;     // wspolrzedne punktow
};

struct Plansza {
	int rozmiar_x;           // rozmiar planzy x
	int rozmiar_y;           // rozmiar planzy y
	char **pola = NULL;      // rozmieszczenie elementoww planszy
	char **kopia_pol = NULL; // kopia rozmieszczenia pol potrzebna do odtwarzania przy utracie zycia
};

struct StanMario {
	struct Pozycja currPos = { 0, 0 }; // pozycja mario na oknie w punktach
	int predkosc_x = 0;                // przesuniecie poziome na klatke
	int predkosc_y = 0;                // przesuniecie pionowe na klatke
	int indeks_obrazka = 0;            // indeks kolejnej grafiki animacji
	bool w_trakcie_skoku = false;      // czy mario skacze
};

struct Przeciwnik {
	struct Pozycja posEnd1;        // poczatkowa pozycja przeciwnika w jednostkach planszy 
	struct Pozycja posEnd2;        // koncowa pozycja przeciwnika w jednostkach planszy 
	int kierunek = 2;              // =2 przeciwnik idzie w kier posEnd2 / =1 przeciwnik idzie w kier posEnd1
	struct Pozycja cur;            // na oknie w punktach
	int indeks_obrazka = 0;        // indeks kolejnej grafiki animacji
};

struct Przeciwnicy {
	int liczbaPrzeciwnikow = 0;          // liczba przeciwnikow na danym etapie
	struct Przeciwnik *jednostki = NULL; // informacje o poszczegolnych przeciwnikach na danym etapie
};

struct Moneta {
	struct Pozycja currPos;        // pozycja monety na planszy w punktach
	struct Pozycja initPos;        // pozycja poczatkowa monety na planszy
	int predkosc_y = 0;            // przesuniecie pionowe na klatke
	bool zbita = false;            // czy moneta jest zbita przez Mario
	bool dodana_po_zbiciu = false; // czy dodana po zbiciu do licznika monet
};

struct Monety {
	int liczbaMonet = 0;                   // ca³kowita liczba monet na danym etapie
	int liczbaMonetNaPoczatkuEtapu = 0;    // liczba zebranych monet na poczatku etapu
	int liczbaZebranychMonetNaEtapie = 0;  // liczba zebranych monet podczas etapu
	struct Moneta *moneta = NULL;          // informacje o poszczegolnych monetach na danym etapie
};

struct ModelGry {
	struct Plansza plansza;            // definicja planzszy
	struct Pozycja marioInit;          // poczatkowa pozycja Mario
	struct StanMario marioStan;        // biezacy stan Mario
	struct Pozycja marioEnd;           // koncowa pozycja Mario
	struct Przeciwnicy przeciwnicy;    // informacje o przeciwnikach 
	struct Monety monety;              // informacje o monetach
	int liczbaZyc;                     // pozostala liczba zyc
	int etapCzasMax;                   // maksymalny czas trwania etapu
	int etapCzasDoKonca;               // czas pozostaly do konca etapu
	int numer_etapu;                   // numer biezacego etapu gry 
	int planszaShift = 0;              // liczba pikseli przesuniecia planszy wzgledem okna wyswietlania
};

void init_plansza(struct Plansza* p, int rozmX, int rozmY);
uint16_t read_model_gry(char *sciezka, struct ModelGry* gra);
void polacz_napisy(char * pierwszy, char * drugi);
uint16_t init_model_gry(int nr_etapu, struct ModelGry* gra);
int liczba_etapow();
void zapisz_stan_gry(struct ModelGry mg);
void odczyt_stanu_gry(struct ModelGry *mg);
void free_model_gry(struct ModelGry *mg);
#endif // MODELGRY_H