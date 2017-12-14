#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include "modelGry.h"


/*
	inicjalizacja modelu gry
*/
uint16_t init_model_gry(int nr_etapu, struct ModelGry* gra) {
	char sciezka[20], pom[10];
	uint16_t blad_odczytu_planszy = 0;

	strcpy(sciezka, "./etap");
	itoa(nr_etapu, pom, 10);
	polacz_napisy(sciezka, pom);
	polacz_napisy(sciezka, ".txt");

	if ( ( blad_odczytu_planszy = read_model_gry(sciezka, gra) ) != 0)
		return blad_odczytu_planszy;
	return 0;
}

/*
	inicjalizacja pol planszy oraz pol kopi planszy (alloc pamieci)
*/
void init_plansza(struct Plansza* p, int rozmX, int rozmY) {
	p->rozmiar_x = rozmX;
	p->rozmiar_y = rozmY;
	p->pola = (char **)malloc(rozmY * sizeof(char*));
	for (int i = 0; i < rozmY; i++)
		p->pola[i] = (char *)malloc(rozmX * sizeof(char));
	for (int i = 0; i < rozmX; i++)
		for (int j = 0; j < rozmY; j++) {
			p->pola[j][i] = PUSTE;
		}
	p->kopia_pol = (char **)malloc(rozmY * sizeof(char*));
	for (int i = 0; i < rozmY; i++)
		p->kopia_pol[i] = (char *)malloc(rozmX * sizeof(char));
	for (int i = 0; i < rozmX; i++)
		for (int j = 0; j < rozmY; j++) {
			p->kopia_pol[j][i] = PUSTE;
		}
}

/*
	zwalnianie pamieci przeznaczonej na pola planszy oraz pola kopi planszy
*/
void free_plansza(struct Plansza* p) {
	if (p->pola) {
		for (int i = 0; i < p->rozmiar_y; i++)
			free(p->pola[i]);
		free(p->pola);
		p->pola = NULL;
	}
	if (p->kopia_pol) {
		for (int i = 0; i < p->rozmiar_y; i++)
			free(p->kopia_pol[i]);
		free(p->kopia_pol);
		p->kopia_pol = NULL;
	}

}

/*
	inicjalizacja przeciwnikow (alloc pamieci)
*/
void init_przeciwnicy(struct Przeciwnicy* przec, int rozm) {
	przec->jednostki = (struct Przeciwnik *)malloc(rozm * sizeof(struct Przeciwnik));
	for (int i = 0; i < rozm; i++) {
		przec->jednostki[i].posEnd1.x = 0;
		przec->jednostki[i].posEnd1.y = 0;
		przec->jednostki[i].posEnd2.x = 0;
		przec->jednostki[i].posEnd2.y = 0;
		przec->jednostki[i].cur.x = 0;
		przec->jednostki[i].cur.y = 0;
		przec->jednostki[i].indeks_obrazka = 0;
		przec->jednostki[i].kierunek = 2;
	}
}

/*
	zwalaninie pamieci przeznaczonej na przeciwnikow
*/
void free_przeciwnicy(struct Przeciwnicy* przec) {
	if (przec->jednostki) {
		free(przec->jednostki);
		przec->jednostki = NULL;
	}
}

/*
	inicjalizacja monet (alloc pamieci)
*/
void init_monety(struct Monety* monety, int rozm) {
	monety->moneta = (struct Moneta *)malloc(rozm * sizeof(struct Moneta));
	for (int i = 0; i < rozm; i++) {
		monety->moneta[i].currPos.x = 0;
		monety->moneta[i].currPos.y = 0;
		monety->moneta[i].predkosc_y = 0;
		monety->moneta[i].zbita = false;
		monety->moneta[i].dodana_po_zbiciu = false;
	}
}

/*
	zwalaninie pamieci przeznaczonej na monety
*/
void free_monety(struct Monety* monety) {
	if (monety->moneta) {
		free(monety->moneta);
		monety->moneta = NULL;
	}
}

/*
	zwalaninie pamieci przeznaczonej na model gry
*/
void free_model_gry(struct ModelGry *mg) {
	free_plansza(&(*mg).plansza);
	free_przeciwnicy(&(*mg).przeciwnicy);
	free_monety(&(*mg).monety);
}

/*
	czytanie naglowka do znaku konca lini
*/
bool czytaj_naglowek(FILE *f, char *nagl) {
	char line[256];

	if (fscanf(f, "%s\n", &line) == 1)
		if (strcmp(line, nagl) == 0)
			return true;
		else return false;
	else return false;
}

/*
	sprawdzanie znakow planszy
*/
bool sprawdz_znak_plansza(char c) {
	if (c == PUSTE || c == PODSTAWA || c == PRZESZKODA || c == PLATFORMA || c == PLATRORMA_Z_MONETA)
		return true;
	return false;
}

/*
	zczytywanie wybranego etapu gry
*/
uint16_t read_model_gry(char *sciezka, struct ModelGry* gra) {
	FILE*  f;
	char line[256], c;
	int licznik_monet = 0;
	uint16_t kod_bledu = 0;

	f = fopen(sciezka, "r");
	if (f != NULL) {
		if (czytaj_naglowek(f, HAED_F)) {
			if (fscanf(f, "%d %d\n%d %d %d %d\n%d\n", &(gra->plansza.rozmiar_x), &(gra->plansza.rozmiar_y),
				&(gra->marioInit.x), &(gra->marioInit.y), &(gra->marioEnd.x), &(gra->marioEnd.y),
				&(gra->etapCzasMax)) == 7) {
				gra->etapCzasDoKonca = gra->etapCzasMax;
				if (czytaj_naglowek(f, PRZECIWNICY_F)) {
					if (fscanf(f, "%d\n", &(gra->przeciwnicy.liczbaPrzeciwnikow)) == 1) {
						free_przeciwnicy(&(gra->przeciwnicy));
						init_przeciwnicy(&(gra->przeciwnicy), gra->przeciwnicy.liczbaPrzeciwnikow);
						for (int i = 0; i < gra->przeciwnicy.liczbaPrzeciwnikow; i++)
							if (fscanf(f, "%d %d %d %d\n",
								&(gra->przeciwnicy.jednostki[i].posEnd1.x),
								&(gra->przeciwnicy.jednostki[i].posEnd1.y),
								&(gra->przeciwnicy.jednostki[i].posEnd2.x),
								&(gra->przeciwnicy.jednostki[i].posEnd2.y)
							) != 4)
								kod_bledu = kod_bledu | READ_ETAP_ERR_PRZECIWNICY_POZYCJA;

						if (czytaj_naglowek(f, PLANSZA_F)) {
							free_plansza(&(gra->plansza));
							init_plansza(&(gra->plansza), gra->plansza.rozmiar_x, gra->plansza.rozmiar_y);
							for (int y = 0; y < gra->plansza.rozmiar_y; y++) {
								for (int x = 0; x < gra->plansza.rozmiar_x; x++)
									if (sprawdz_znak_plansza(c = fgetc(f))) {
										gra->plansza.pola[y][x] = c;
										gra->plansza.kopia_pol[y][x] = c;
										if (c == PLATRORMA_Z_MONETA)
											licznik_monet++;
									}
									else
										kod_bledu = kod_bledu | READ_ETAP_ERR_PLANASZA_NIEPOPRAWNY_ZNAK;
								if ((c = fgetc(f)) != '\n')
									kod_bledu = kod_bledu | READ_ETAP_ERR_PLANASZA_BRAK_NOWEJ_LINII;
							}

							gra->monety.liczbaMonet = licznik_monet;
							free_monety(&gra->monety);
							init_monety(&gra->monety, gra->monety.liczbaMonet);
							licznik_monet = 0;
							for (int y = 0; y < gra->plansza.rozmiar_y; y++)
								for (int x = 0; x < gra->plansza.rozmiar_x; x++)
									if (gra->plansza.pola[y][x] == PLATRORMA_Z_MONETA) {
										gra->monety.moneta[licznik_monet].initPos.x = x;
										gra->monety.moneta[licznik_monet].initPos.y = y;
										licznik_monet++;
									}
						}
						else
							kod_bledu = kod_bledu | READ_ETAP_ERR_BRAK_PLANASZA;
					}
					else
						kod_bledu = kod_bledu | READ_ETAP_ERR_ODCZYT_PRZECIWNICY_LICZBA;
				}
				else
					kod_bledu = kod_bledu | READ_ETAP_ERR_BRAK_PRZECIWNICY;
			}
			else
				kod_bledu = kod_bledu | READ_ETAP_ERR_ODCZYT_HEAD;
		}
		else
			kod_bledu = kod_bledu | READ_ETAP_ERR_BRAK_HEAD;
		fclose(f);
	}
	else
		kod_bledu = kod_bledu | READ_ETAP_ERR_OPEN;

	// czy Mario na planszy
	if (!(gra->marioInit.x >= 0 && gra->marioInit.x < gra->plansza.rozmiar_x &&
		gra->marioInit.y >= 0 && gra->marioInit.y < gra->plansza.rozmiar_y &&
		gra->marioEnd.x >= 0 && gra->marioEnd.x < gra->plansza.rozmiar_x &&
		gra->marioEnd.y >= 0 && gra->marioEnd.y < gra->plansza.rozmiar_y)
		)
		kod_bledu = kod_bledu | READ_ETAP_ERR_MARIO_ZLA_POZYCJA;
	for (int i = 0; i < gra->przeciwnicy.liczbaPrzeciwnikow; i++)
		if (!(gra->przeciwnicy.jednostki[i].posEnd1.x >= 0 && gra->przeciwnicy.jednostki[i].posEnd1.x < gra->plansza.rozmiar_x &&
			gra->przeciwnicy.jednostki[i].posEnd1.y >= 0 && gra->przeciwnicy.jednostki[i].posEnd1.y < gra->plansza.rozmiar_y &&
			gra->przeciwnicy.jednostki[i].posEnd2.x >= 0 && gra->przeciwnicy.jednostki[i].posEnd2.x < gra->plansza.rozmiar_x &&
			gra->przeciwnicy.jednostki[i].posEnd2.y >= 0 && gra->przeciwnicy.jednostki[i].posEnd2.y < gra->plansza.rozmiar_y)
			)
			kod_bledu = kod_bledu | READ_ETAP_ERR_PRZECIWNIK_ZLA_POZYCJA;

	return kod_bledu;
}

/*
	laczenie dwoch podanych napisow
*/
void polacz_napisy(char * pierwszy_napis, char * drugi_napis) {
	while (*pierwszy_napis)
		pierwszy_napis++;
	while (*drugi_napis) {
		*pierwszy_napis = *drugi_napis;
		pierwszy_napis++;
		drugi_napis++;
	}
	*pierwszy_napis = '\0';
}

/*
	sprawdzanie ilosci plikow dostepnych etapow od etap1.txt do etapN.txt
*/
int liczba_etapow() {
	int liczba_etapow = 0;
	char nazwa_pliku[20];
	char pom[10];
	FILE *plik_etap;
	bool kontynuuj = true;
	do {
		strcpy(nazwa_pliku, "./etap");
		itoa(++liczba_etapow, pom, 10);
		polacz_napisy(nazwa_pliku, pom);
		polacz_napisy(nazwa_pliku, ".txt");
		plik_etap = fopen(nazwa_pliku, "r");
		if (plik_etap != NULL)
			fclose(plik_etap);
		else
			kontynuuj = false;
	} while (kontynuuj);

	return liczba_etapow - 1;
}

/*
	zapisanie modelu do pilku (bitowo)
*/
void zapisz_stan_gry(struct ModelGry mg) {
	FILE *plik_zapis;

	plik_zapis = fopen(PLIK_SAVE, "wb");
	if (plik_zapis != NULL) {
		// plansza
		fwrite(&mg.plansza.rozmiar_x, sizeof(int), 1, plik_zapis);
		fwrite(&mg.plansza.rozmiar_y, sizeof(int), 1, plik_zapis);
		for (int i = 0; i < mg.plansza.rozmiar_y; i++)
			for (int j = 0; j < mg.plansza.rozmiar_x; j++)
				fwrite(&mg.plansza.pola[i][j], sizeof(char), 1, plik_zapis);
		for (int i = 0; i < mg.plansza.rozmiar_y; i++)
			for (int j = 0; j < mg.plansza.rozmiar_x; j++)
				fwrite(&mg.plansza.kopia_pol[i][j], sizeof(char), 1, plik_zapis);
		// marioInit
		fwrite(&mg.marioInit, sizeof(struct Pozycja), 1, plik_zapis);
		//marioStan
		fwrite(&mg.marioStan, sizeof(struct StanMario), 1, plik_zapis);
		// marioEnd
		fwrite(&mg.marioEnd, sizeof(struct Pozycja), 1, plik_zapis);

		//przeciwnicy
		fwrite(&mg.przeciwnicy.liczbaPrzeciwnikow, sizeof(int), 1, plik_zapis);
		for (int i = 0; i < mg.przeciwnicy.liczbaPrzeciwnikow; i++) {
			fwrite(&(mg.przeciwnicy.jednostki[i]), sizeof(struct Przeciwnik), 1, plik_zapis);
		}

		//monety
		fwrite(&mg.monety.liczbaMonet, sizeof(int), 1, plik_zapis);
		fwrite(&mg.monety.liczbaMonetNaPoczatkuEtapu, sizeof(int), 1, plik_zapis);
		fwrite(&mg.monety.liczbaZebranychMonetNaEtapie, sizeof(int), 1, plik_zapis);
		for (int i = 0; i < mg.monety.liczbaMonet; i++) {
			fwrite(&mg.monety.moneta[i], sizeof(struct Moneta), 1, plik_zapis);
		}
		fwrite(&mg.liczbaZyc, sizeof(int), 1, plik_zapis);
		fwrite(&mg.etapCzasMax, sizeof(int), 1, plik_zapis);
		fwrite(&mg.etapCzasDoKonca, sizeof(int), 1, plik_zapis);
		fwrite(&mg.numer_etapu, sizeof(int), 1, plik_zapis);
		fwrite(&mg.planszaShift, sizeof(int), 1, plik_zapis);

		fclose(plik_zapis);
	}
}

/*
	odczytanie modelu z pilku (bitowo)
*/
void odczyt_stanu_gry(struct ModelGry *mg) {
	FILE *plik_odczyt;

	plik_odczyt = fopen(PLIK_SAVE, "rb");
	if (plik_odczyt != NULL) {
		// plansza
		fread(&(*mg).plansza.rozmiar_x, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).plansza.rozmiar_y, sizeof(int), 1, plik_odczyt);
		free_plansza(&(*mg).plansza);
		init_plansza(&(*mg).plansza, (*mg).plansza.rozmiar_x, (*mg).plansza.rozmiar_y);

		for (int i = 0; i < (*mg).plansza.rozmiar_y; i++)
			for (int j = 0; j < (*mg).plansza.rozmiar_x; j++)
				fread(&(*mg).plansza.pola[i][j], sizeof(char), 1, plik_odczyt);
		for (int i = 0; i < (*mg).plansza.rozmiar_y; i++)
			for (int j = 0; j < (*mg).plansza.rozmiar_x; j++)
				fread(&(*mg).plansza.kopia_pol[i][j], sizeof(char), 1, plik_odczyt);
		// marioInit
		fread(&(*mg).marioInit, sizeof(struct Pozycja), 1, plik_odczyt);
		//marioStan
		fread(&(*mg).marioStan, sizeof(struct StanMario), 1, plik_odczyt);
		// marioEnd
		fread(&(*mg).marioEnd, sizeof(struct Pozycja), 1, plik_odczyt);
		//przeciwnicy
		fread(&(*mg).przeciwnicy.liczbaPrzeciwnikow, sizeof(int), 1, plik_odczyt);
		free_przeciwnicy(&(*mg).przeciwnicy);
		init_przeciwnicy(&(*mg).przeciwnicy, (*mg).przeciwnicy.liczbaPrzeciwnikow);
		for (int i = 0; i < (*mg).przeciwnicy.liczbaPrzeciwnikow; i++) {
			fread(&((*mg).przeciwnicy.jednostki[i]), sizeof(struct Przeciwnik), 1, plik_odczyt);

		}		//monety
		fread(&(*mg).monety.liczbaMonet, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).monety.liczbaMonetNaPoczatkuEtapu, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).monety.liczbaZebranychMonetNaEtapie, sizeof(int), 1, plik_odczyt);
		free_monety(&(*mg).monety);
		init_monety(&(*mg).monety, (*mg).monety.liczbaMonet);
		for (int i = 0; i < (*mg).monety.liczbaMonet; i++) {
			fread(&(*mg).monety.moneta[i], sizeof(struct Moneta), 1, plik_odczyt);
		}
		fread(&(*mg).liczbaZyc, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).etapCzasMax, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).etapCzasDoKonca, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).numer_etapu, sizeof(int), 1, plik_odczyt);
		fread(&(*mg).planszaShift, sizeof(int), 1, plik_odczyt);

		fclose(plik_odczyt);
	}
}
