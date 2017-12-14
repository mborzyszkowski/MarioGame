#include "main.h"

/*
ograniczenie klatek na sekunde do wybranej liczby_FPS (LICZBA_FPS)
*/
int dopasuj_fps(int ostatnie_tykniecie) {
	int biezace_tykniecie = SDL_GetTicks();
	int sekunda = 1000;
	int wymagany_czas = sekunda / LICZBA_FPS;
	int roznica_czasu = biezace_tykniecie - ostatnie_tykniecie;
	if (roznica_czasu < wymagany_czas)
		if ( (wymagany_czas - roznica_czasu) > 0) 
			SDL_Delay(wymagany_czas - roznica_czasu);
	return SDL_GetTicks();
}

/*
ustawienie obiektow na ich poczatkowe pozycje w danym etapie
*/
void mario_na_poczatek_etapu(struct ModelGry *mg, int zmiana_zyc, int* czas_start) {
	int poczatek_planszy_y = WYSOKOSC_OKNA - ((*mg).plansza.rozmiar_y * ROZMIAR_BLOKU);
	// Mario na poczatek
	(*mg).planszaShift = 0;
	(*mg).marioStan.currPos.x = (*mg).marioInit.x * ROZMIAR_BLOKU;
	(*mg).marioStan.currPos.y = poczatek_planszy_y + (*mg).marioInit.y * ROZMIAR_BLOKU;
	(*mg).marioStan.predkosc_x = 0;
	(*mg).marioStan.predkosc_y = 0;
	(*mg).marioStan.indeks_obrazka = MARIO_STOI_IDX;
	(*mg).marioStan.w_trakcie_skoku = false;
	// liczba zyc i czas do konca gry
	(*mg).etapCzasDoKonca = (*mg).etapCzasMax;
	if ((*mg).liczbaZyc + zmiana_zyc >= 0)
		(*mg).liczbaZyc += zmiana_zyc;
	// przeciwnicy na poczatek
	int liczba_przec = (*mg).przeciwnicy.liczbaPrzeciwnikow;
	for (int i = 0; i < liczba_przec; i++) {
		(*mg).przeciwnicy.jednostki[i].cur.x = (*mg).przeciwnicy.jednostki[i].posEnd1.x * ROZMIAR_BLOKU;
		(*mg).przeciwnicy.jednostki[i].cur.y = poczatek_planszy_y + ((*mg).przeciwnicy.jednostki[i].posEnd1.y * ROZMIAR_BLOKU);
		(*mg).przeciwnicy.jednostki[i].indeks_obrazka = PRZECIWNIK_STOI_IDX;
		(*mg).przeciwnicy.jednostki[i].kierunek = 2;
	}
	// inicjalizacja monet
	int liczba_monet = (*mg).monety.liczbaMonet;
	for (int i = 0; i < liczba_monet; i++) {
		(*mg).monety.moneta[i].currPos.x = (*mg).monety.moneta[i].initPos.x * ROZMIAR_BLOKU;
		(*mg).monety.moneta[i].currPos.y = ((*mg).monety.moneta[i].initPos.y * ROZMIAR_BLOKU);
		(*mg).monety.moneta[i].predkosc_y = 0;
		(*mg).monety.moneta[i].dodana_po_zbiciu = false;
		(*mg).monety.moneta[i].zbita = false;
	}
	// aktualizacja czasu startu etapu i zerowanie zebranej liczby monet na etapie
	// przywracanie platform przy stracie zycia
	if (zmiana_zyc < 0) {
		*czas_start = SDL_GetTicks();
		przywroc_platformy(mg);
		(*mg).monety.liczbaZebranychMonetNaEtapie = 0;
	}
	if (zmiana_zyc == 0)
		(*mg).monety.liczbaZebranychMonetNaEtapie = 0;
}

/*
przywracanie straconych platform w danym etapie
*/
void przywroc_platformy(struct ModelGry *mg) {
	for (int i = 0; i < (*mg).plansza.rozmiar_y; i++)
		for (int j = 0; j < (*mg).plansza.rozmiar_x; j++)
			if ((*mg).plansza.kopia_pol[i][j] == PLATFORMA || (*mg).plansza.kopia_pol[i][j] == PLATRORMA_Z_MONETA)
				(*mg).plansza.pola[i][j] = (*mg).plansza.kopia_pol[i][j];
}