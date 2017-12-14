#include "main.h"

/*
dodaje tlo do planszy
*/
void rysuj_tlo(SDL_Surface *pow_okna, SDL_Surface *pow_tla, int przesuniecie) {
	SDL_Rect zrodlo = { przesuniecie, 0, pow_okna->w, pow_tla->h };
	SDL_Rect cel = { 0, pow_okna->h - pow_tla->h, pow_okna->w, pow_tla->h };
	SDL_BlitScaled(pow_tla, &zrodlo, pow_okna, &cel);
}

/*
wyswietlenie Mario na oknie
*/
void rysuj_mario(SDL_Surface *pow_okna, SDL_Surface **mario_graf, struct ModelGry modelGry) {
	SDL_Rect r = { modelGry.marioStan.currPos.x, modelGry.marioStan.currPos.y, ROZMIAR_BLOKU, ROZMIAR_BLOKU };
	SDL_BlitScaled(mario_graf[modelGry.marioStan.indeks_obrazka], NULL, pow_okna, &r);
}

/*
wyswietlenie Przeciwnikow na oknie
*/
void rysuj_przeciwnicy(SDL_Surface *pow_okna, SDL_Surface **przeciwnicy_graf, struct ModelGry modelGry) {
	int liczba_przec = modelGry.przeciwnicy.liczbaPrzeciwnikow;
	SDL_Rect r;
	for (int i = 0; i < liczba_przec; i++) {
		r = { modelGry.przeciwnicy.jednostki[i].cur.x, modelGry.przeciwnicy.jednostki[i].cur.y, ROZMIAR_BLOKU, ROZMIAR_BLOKU };
		SDL_BlitScaled(przeciwnicy_graf[modelGry.przeciwnicy.jednostki[i].indeks_obrazka], NULL, pow_okna, &r);
	}
}

/*
wyswietlenie Monet na oknie
*/
void rysuj_spadajace_monety(SDL_Surface *pow_okna, SDL_Surface *moneta_graf, struct ModelGry modelGry) {
	int liczba_monet = modelGry.monety.liczbaMonet;
	int poczatek_planszy_y = WYSOKOSC_OKNA - (modelGry.plansza.rozmiar_y * ROZMIAR_BLOKU);
	SDL_Rect r;
	for (int i = 0; i < liczba_monet; i++) {
		if (modelGry.monety.moneta[i].zbita && modelGry.monety.moneta[i].currPos.y <= (modelGry.plansza.rozmiar_y * ROZMIAR_BLOKU) &&
			(modelGry.monety.moneta[i].currPos.x - modelGry.planszaShift) > 0 &&
			(modelGry.planszaShift + SZEROKOSC_OKNA) > modelGry.monety.moneta[i].currPos.x) {
			r = { modelGry.monety.moneta[i].currPos.x - modelGry.planszaShift,
				modelGry.monety.moneta[i].currPos.y + poczatek_planszy_y,
				ROZMIAR_BLOKU, ROZMIAR_BLOKU };
			SDL_BlitScaled(moneta_graf, NULL, pow_okna, &r);
		}
	}
}

/*
wypisuje tekst na ekran uzywajac danego zestawu znakow
*/
void wypisz_tekst(SDL_Surface *ekran, int pos_x, int pos_y, const char *napis, SDL_Surface *zestaw_znakow) {
	int znak;
	int outSize = 16;
	int rozmiar_tablicy_znakow = 16;
	int liczba_pixeli_na_znak = 16;
	SDL_Rect zrodlo = { 0, 0, liczba_pixeli_na_znak, liczba_pixeli_na_znak }, cel = { pos_x, pos_y, outSize, outSize }; // {x, y, w ,h}
	while (*napis) {
		znak = *napis & 0xff;
		if (znak > 0) {
			zrodlo = { (znak % rozmiar_tablicy_znakow) * liczba_pixeli_na_znak,
				(znak / rozmiar_tablicy_znakow) * liczba_pixeli_na_znak, zrodlo.w, zrodlo.h };
			SDL_BlitScaled(zestaw_znakow, &zrodlo, ekran, &cel);
			cel.x += outSize;
		}
		napis++;
	};
}

/*
wypisuje informacje o pozostalym czasie, liczbie zyc, obecnym poziomie oraz liczbie zdobytych monet poprzez funkcje wypisz_tekst
*/
void wypisz_stan_gry(SDL_Surface *ekran, SDL_Surface *zestaw_znakow, struct ModelGry mg, int pos_x, int pos_y) {
	char czas_str[100];
	char stan_gry_str[110];

	sprintf(stan_gry_str, "Pozostaly czas:%d Zycia:%d Poziom:%d Monety:%d",
		mg.etapCzasDoKonca, mg.liczbaZyc, mg.numer_etapu, mg.monety.liczbaMonetNaPoczatkuEtapu + mg.monety.liczbaZebranychMonetNaEtapie);

	wypisz_tekst(ekran, pos_x, pos_y, stan_gry_str, zestaw_znakow);
}