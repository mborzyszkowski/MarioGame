#include "main.h"
#include "indeksy_grafik.h"

/*
funkcja wewnetrzna: model_mario_aktualizacja_pozycji
kolizja punktu z prostokatem
*/
bool kolizja_punkt_prostokat(int punktX, int punktY, SDL_Rect prostokat) {
	if (punktX > prostokat.x && punktX < prostokat.x + prostokat.w)
		if (punktY > prostokat.y && punktY < prostokat.y + prostokat.h)
			return true;
		else
			return false;
	else
		return false;
}

/*
funkcja wewnetrzna: model_mario_aktualizacja_pozycji
sprawdzanie kolizji dwoch kwaratow
*/
uint8_t sprawdz_kolizje(SDL_Rect rx, SDL_Rect ry) {
	uint8_t kod_punktu_kolizji = 0;
	if (kolizja_punkt_prostokat(rx.x + 1, rx.y, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_0;
	if (kolizja_punkt_prostokat(rx.x + rx.w - 1, rx.y, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_1;
	if (kolizja_punkt_prostokat(rx.x + rx.w, rx.y + 1, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_2;
	if (kolizja_punkt_prostokat(rx.x + rx.w, rx.y + rx.h - 1, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_3;
	if (kolizja_punkt_prostokat(rx.x + rx.w - 1, rx.y + rx.h, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_4;
	if (kolizja_punkt_prostokat(rx.x + 1, rx.y + rx.h, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_5;
	if (kolizja_punkt_prostokat(rx.x, rx.y + rx.h - 1, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_6;
	if (kolizja_punkt_prostokat(rx.x, rx.y + 1, ry)) kod_punktu_kolizji += PUNKT_KOLIZJI_7;
	return kod_punktu_kolizji;
}

/*
glowna funkcja sprawdzajaca kolizje Mario z otaczajacymi go obiektami
 - czy_test_kolizji: true = testowanie czy na nowej pozycji by³aby kolizja, bez przesuwania Mario
					 false = testowanie kolizji i przesuniecie Mario jezeli nie by³o kolizji
*/
uint8_t model_mario_aktualizacja_pozycji(struct ModelGry *mg, int przesuniecieX, int przesuniecieY, bool czy_test_kolizji, int *czas_start) {
	SDL_Rect mario_na_planszy_nowa_pozycja = { (*mg).marioStan.currPos.x + (*mg).planszaShift + przesuniecieX,
		(*mg).marioStan.currPos.y - (WYSOKOSC_OKNA - ROZMIAR_BLOKU * (*mg).plansza.rozmiar_y) - przesuniecieY,
		ROZMIAR_BLOKU, ROZMIAR_BLOKU };
	uint8_t kolizja = KOLIZJA_BRAK;
	uint8_t punkt_kolizji_pom = 0, punkt_kolizji = 0;

	// mario poza plansza
	if (mario_na_planszy_nowa_pozycja.x < 0) {
		// lewo
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_6;
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_7;
		kolizja = kolizja | KOLIZJA_PLANSZA;
	}
	else if (mario_na_planszy_nowa_pozycja.x + mario_na_planszy_nowa_pozycja.w >= (*mg).plansza.rozmiar_x * ROZMIAR_BLOKU - 1) {
		// prawo
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_2;
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_3;
		kolizja = kolizja | KOLIZJA_PLANSZA;
	}
	else if (mario_na_planszy_nowa_pozycja.y < 0) {
		// gora
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_0;
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_1;
		kolizja = kolizja | KOLIZJA_PLANSZA;
	}
	else if (mario_na_planszy_nowa_pozycja.y + mario_na_planszy_nowa_pozycja.h >= (*mg).plansza.rozmiar_y * ROZMIAR_BLOKU - 1) {
		// dol
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_4;
		punkt_kolizji = punkt_kolizji | PUNKT_KOLIZJI_5;
		kolizja = kolizja | KOLIZJA_PLANSZA;
	}

	// bloki do badania kolizji
	int x_0 = mario_na_planszy_nowa_pozycja.x / ROZMIAR_BLOKU;
	int y_0 = mario_na_planszy_nowa_pozycja.y / ROZMIAR_BLOKU;

	SDL_Rect blok_00 = { x_0 * ROZMIAR_BLOKU, y_0 * ROZMIAR_BLOKU ,ROZMIAR_BLOKU , ROZMIAR_BLOKU };            // gora lewo
	SDL_Rect blok_10 = { (x_0 + 1) * ROZMIAR_BLOKU, y_0 * ROZMIAR_BLOKU ,ROZMIAR_BLOKU , ROZMIAR_BLOKU };      // gora prawo
	SDL_Rect blok_01 = { x_0 * ROZMIAR_BLOKU, (y_0 + 1) * ROZMIAR_BLOKU ,ROZMIAR_BLOKU , ROZMIAR_BLOKU };      // dol lewo
	SDL_Rect blok_11 = { (x_0 + 1) * ROZMIAR_BLOKU, (y_0 + 1) * ROZMIAR_BLOKU ,ROZMIAR_BLOKU , ROZMIAR_BLOKU };// dol prawo

	// kolizja z blokami
	if ((kolizja & KOLIZJA_PLANSZA) == 0) {
		if ((*mg).plansza.pola[y_0][x_0] != PUSTE) {
			punkt_kolizji_pom = sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_00);
			if (punkt_kolizji_pom > 0) {
				kolizja = kolizja | KOLIZJA_BLOK_00;
				punkt_kolizji = punkt_kolizji | punkt_kolizji_pom;
			}
		}
		if ((*mg).plansza.pola[y_0][x_0 + 1] != PUSTE) {
			punkt_kolizji_pom = sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_10);
			if (punkt_kolizji_pom > 0) {
				kolizja = kolizja | KOLIZJA_BLOK_10;
				punkt_kolizji = punkt_kolizji | punkt_kolizji_pom;
			}
		}
		if ((*mg).plansza.pola[y_0 + 1][x_0] != PUSTE) {
			punkt_kolizji_pom = sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_01);
			if (punkt_kolizji_pom > 0) {
				kolizja = kolizja | KOLIZJA_BLOK_01;
				punkt_kolizji = punkt_kolizji | punkt_kolizji_pom;
			}
		}
		if ((*mg).plansza.pola[y_0 + 1][x_0 + 1] != PUSTE) {
			punkt_kolizji_pom = sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_11);
			if (punkt_kolizji_pom > 0) {
				kolizja = kolizja | KOLIZJA_BLOK_11;
				punkt_kolizji = punkt_kolizji | punkt_kolizji_pom;
			}
		}
	}
	// kolizja z przeciwnikiem
	int liczba_przeciwnikow = (*mg).przeciwnicy.liczbaPrzeciwnikow;
	SDL_Rect blok_przeciwnik = { 0 , 0 ,ROZMIAR_BLOKU , ROZMIAR_BLOKU };
	for (int i = 0; i < liczba_przeciwnikow; i++) {
		blok_przeciwnik.x = (*mg).przeciwnicy.jednostki[i].cur.x + (*mg).planszaShift;
		blok_przeciwnik.y = (*mg).przeciwnicy.jednostki[i].cur.y - (WYSOKOSC_OKNA - ROZMIAR_BLOKU * (*mg).plansza.rozmiar_y);
		if (sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_przeciwnik) > 0) {
			kolizja = kolizja | KOLIZJA_PRZECIWNIK;
			break;
		}
	}
	// kolizja koniec etapu
	SDL_Rect blok_koniec = { (*mg).marioEnd.x * ROZMIAR_BLOKU, (*mg).marioEnd.y * ROZMIAR_BLOKU, ROZMIAR_BLOKU, ROZMIAR_BLOKU };
	if (sprawdz_kolizje(mario_na_planszy_nowa_pozycja, blok_koniec) > 0) {
		kolizja = kolizja | KOLIZJA_KONIEC_ETAPU;
	}

	// zmina predkosci
	if (!czy_test_kolizji) {
		if (kolizja == KOLIZJA_BRAK) {
			(*mg).marioStan.currPos.x += przesuniecieX;
			(*mg).marioStan.currPos.y -= przesuniecieY;
		}
		else { // zerowanie predkosci po kolizji
			if ((punkt_kolizji & PUNKT_KOLIZJI_2) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_3) > 0 ||
				(punkt_kolizji & PUNKT_KOLIZJI_6) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_7) > 0)
				(*mg).marioStan.predkosc_x = 0;
			if ((punkt_kolizji & PUNKT_KOLIZJI_0) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_1) > 0 ||
				(punkt_kolizji & PUNKT_KOLIZJI_4) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_5) > 0)
				(*mg).marioStan.predkosc_y = 0;
			if ((punkt_kolizji & PUNKT_KOLIZJI_4) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_5) > 0)
				(*mg).marioStan.w_trakcie_skoku = false;
			// zbijanie platformy
			if ((punkt_kolizji & PUNKT_KOLIZJI_0) > 0 || (punkt_kolizji & PUNKT_KOLIZJI_1) > 0)
				if ((kolizja & KOLIZJA_BLOK_00) > 0 || (kolizja & KOLIZJA_BLOK_10) > 0) {
					if ((mario_na_planszy_nowa_pozycja.x - blok_00.x) < ROZMIAR_BLOKU / 2) {// kolizja z blokiem 00 z dolu
						if ((*mg).plansza.pola[y_0][x_0] == PLATFORMA)
							(*mg).plansza.pola[y_0][x_0] = PUSTE;
						else if ((*mg).plansza.pola[y_0][x_0] == PLATRORMA_Z_MONETA) {
							for (int i = 0; i < (*mg).monety.liczbaMonet; i++) {
								if (!(*mg).monety.moneta[i].zbita &&
									((*mg).monety.moneta[i].initPos.x == x_0) &&
									((*mg).monety.moneta[i].initPos.y == y_0)) {
									(*mg).monety.moneta[i].zbita = true;
									(*mg).monety.moneta[i].predkosc_y = MONETA_PREDKOSC_W_GORE;
								}
							}
							(*mg).plansza.pola[y_0][x_0] = PUSTE;
						}
					}
					else // kolizja z blokiem 10 z dolu
						if ((*mg).plansza.pola[y_0][x_0 + 1] == PLATFORMA)
							(*mg).plansza.pola[y_0][x_0 + 1] = PUSTE;
						else if ((*mg).plansza.pola[y_0][x_0 + 1] == PLATRORMA_Z_MONETA) {
							for (int i = 0; i < (*mg).monety.liczbaMonet; i++) {
								if (!(*mg).monety.moneta[i].zbita &&
									((*mg).monety.moneta[i].initPos.x == (x_0 + 1)) &&
									((*mg).monety.moneta[i].initPos.y == y_0)) {
									(*mg).monety.moneta[i].zbita = true;
									(*mg).monety.moneta[i].predkosc_y = MONETA_PREDKOSC_W_GORE;
								}
							}
							(*mg).plansza.pola[y_0][x_0 + 1] = PUSTE;
						}
						kolizja = kolizja | KOLIZJA_Z_PLATFORMA;
				}
			// dosuwanie do bloku
			if ((kolizja & KOLIZJA_PLANSZA) == 0 && (kolizja & KOLIZJA_PRZECIWNIK) == 0) {
				// dosuwanie po kolizji z blokiem (prawo / lewo ) 
				if ((punkt_kolizji & PUNKT_KOLIZJI_2) > 0 && (punkt_kolizji & PUNKT_KOLIZJI_3) > 0)
					(*mg).marioStan.currPos.x += ((blok_10.x - (*mg).planszaShift) - ((*mg).marioStan.currPos.x + ROZMIAR_BLOKU));
				if ((punkt_kolizji & PUNKT_KOLIZJI_6) > 0 && (punkt_kolizji & PUNKT_KOLIZJI_7) > 0)
					(*mg).marioStan.currPos.x -= ((*mg).marioStan.currPos.x - (blok_00.x - (*mg).planszaShift) - ROZMIAR_BLOKU);
			}
			else if ((kolizja & KOLIZJA_PLANSZA) > 0) {
				// upadek
				if ((punkt_kolizji & PUNKT_KOLIZJI_4) > 0 && (punkt_kolizji & PUNKT_KOLIZJI_5) > 0)
					mario_na_poczatek_etapu(mg, -1, czas_start);
			}
			else // kolizja z przeciwnikiem
				mario_na_poczatek_etapu(mg, -1, czas_start);
		}
	}
	return kolizja;
}

/*
przesuniecie okna oraz wybranych obiektow o odpowiednie przesuniecie zalezne od Mario
*/
void aktualizacja_polozenia_planszy(struct ModelGry *mg) {
	int liczba_przec = mg->przeciwnicy.liczbaPrzeciwnikow;
	int mario_pozycja_na_oknie_x = (*mg).marioStan.currPos.x;
	int margines = 2 * ROZMIAR_BLOKU;
	int rozmiar_planszy = (*mg).plansza.rozmiar_x * ROZMIAR_BLOKU;
	int przesuniecie = OKNO_PREDKOSC_PRZESUWANIA;
	if ((mario_pozycja_na_oknie_x / SZEROKOSC_OKNA) > 1) {
		przesuniecie = (mario_pozycja_na_oknie_x / SZEROKOSC_OKNA - 1) * SZEROKOSC_OKNA;
		(*mg).planszaShift += przesuniecie;
		(*mg).marioStan.currPos.x -= przesuniecie;
		for (int i = 0; i < liczba_przec; i++) {
			mg->przeciwnicy.jednostki[i].cur.x -= przesuniecie;
		}
	}
	else
		if (mario_pozycja_na_oknie_x > SZEROKOSC_OKNA && ((*mg).planszaShift + SZEROKOSC_OKNA + SZEROKOSC_OKNA / 2) <= rozmiar_planszy) {
			przesuniecie = SZEROKOSC_OKNA / 2;
			(*mg).planszaShift += przesuniecie;
			(*mg).marioStan.currPos.x -= przesuniecie;
			for (int i = 0; i < liczba_przec; i++) {
				mg->przeciwnicy.jednostki[i].cur.x -= przesuniecie;
			}
		}
		else
			if (mario_pozycja_na_oknie_x < SZEROKOSC_OKNA / 2 - margines) {
				if ((*mg).planszaShift - przesuniecie >= 0) {
					(*mg).planszaShift -= przesuniecie;
					(*mg).marioStan.currPos.x += przesuniecie;
					for (int i = 0; i < liczba_przec; i++) {
						mg->przeciwnicy.jednostki[i].cur.x += przesuniecie;
					}
				}
			}
			else
				if (mario_pozycja_na_oknie_x > SZEROKOSC_OKNA / 2 + margines)
					if ((*mg).planszaShift + przesuniecie + SZEROKOSC_OKNA < rozmiar_planszy) {
						(*mg).planszaShift += przesuniecie;
						(*mg).marioStan.currPos.x -= przesuniecie;
						for (int i = 0; i < liczba_przec; i++) {
							mg->przeciwnicy.jednostki[i].cur.x -= przesuniecie;
						}
					}
}

/*
funkcja wewnetrzna: aktualizacja_polozenia_przeciwnikow, zmiana_obrazka_przeciwnik
odleglosc dwoch punktow od siebie w pixelach
*/
float odleglosc_punktow(float x1, float y1, float x2, float y2) {
	float roznica_x = (x2 - x1);
	float roznica_y = (y2 - y1);
	return sqrt(roznica_x * roznica_x + roznica_y * roznica_y);
}

/*
aktualizacja pozycji, na oknie, oraz zwrotu przeciwnikow
*/
void aktualizacja_polozenia_przeciwnikow(struct ModelGry *mg) {
	int wekt_kier_x, wekt_kier_y, dx, dy;
	float wekt_kier_dlugosc;
	float proporcja;
	int przesuniecie_x, przesuniecie_y;
	int liczba_przec = mg->przeciwnicy.liczbaPrzeciwnikow;
	struct Pozycja end_pocz;
	struct Pozycja end_koniec;
	int poczatek_planszy_y = WYSOKOSC_OKNA - ((*mg).plansza.rozmiar_y * ROZMIAR_BLOKU);
	float pom_x = 0, pom_y = 0;
	for (int i = 0; i < liczba_przec; i++) {
		// zmiana kierunku
		if (mg->przeciwnicy.jednostki[i].kierunek == 2) {
			if (odleglosc_punktow(mg->przeciwnicy.jednostki[i].cur.x + mg->planszaShift,
				mg->przeciwnicy.jednostki[i].cur.y - poczatek_planszy_y,
				mg->przeciwnicy.jednostki[i].posEnd2.x * ROZMIAR_BLOKU,
				mg->przeciwnicy.jednostki[i].posEnd2.y * ROZMIAR_BLOKU) < (PRZECIWNIK_PREDKOSC + 1))
				mg->przeciwnicy.jednostki[i].kierunek = 1;
		}
		else // kier == 1
		{
			if (odleglosc_punktow(mg->przeciwnicy.jednostki[i].cur.x + mg->planszaShift,
				mg->przeciwnicy.jednostki[i].cur.y - poczatek_planszy_y,
				mg->przeciwnicy.jednostki[i].posEnd1.x * ROZMIAR_BLOKU,
				mg->przeciwnicy.jednostki[i].posEnd1.y * ROZMIAR_BLOKU) < (PRZECIWNIK_PREDKOSC + 1))
				mg->przeciwnicy.jednostki[i].kierunek = 2;
		}
		// ustawienie poczatku i konca
		if (mg->przeciwnicy.jednostki[i].kierunek == 2) {
			end_pocz = mg->przeciwnicy.jednostki[i].posEnd1;
			end_koniec = mg->przeciwnicy.jednostki[i].posEnd2;
		}
		else { // kier == 1
			end_pocz = mg->przeciwnicy.jednostki[i].posEnd2;
			end_koniec = mg->przeciwnicy.jednostki[i].posEnd1;
		}
		// wylicz przesuniecie, gdy przeciwnik chodzi po skosie
		dx = end_koniec.x - end_pocz.x;
		dy = end_koniec.y - end_pocz.y;
		wekt_kier_x = dx * ROZMIAR_BLOKU;
		wekt_kier_y = dy * ROZMIAR_BLOKU;
		wekt_kier_dlugosc = odleglosc_punktow(0, 0, wekt_kier_x, wekt_kier_y);
		proporcja = PRZECIWNIK_PREDKOSC / wekt_kier_dlugosc;
		przesuniecie_x = (int)(wekt_kier_x * proporcja);
		przesuniecie_y = (int)(wekt_kier_y * proporcja);
		// aktualizacja pozycji
		mg->przeciwnicy.jednostki[i].cur.x += przesuniecie_x;
		mg->przeciwnicy.jednostki[i].cur.y += przesuniecie_y;
		// korekta wspolrzednej y przez wyliczenie pom_y dla wyliczonego x calkowitego
		if (dx != 0 && dy != 0) { // chodzi po skosie
			float a_x = end_pocz.x * ROZMIAR_BLOKU;
			float a_y = end_pocz.y * ROZMIAR_BLOKU;
			float b_x = end_koniec.x * ROZMIAR_BLOKU;
			float b_y = end_koniec.y * ROZMIAR_BLOKU;
			float p_x = mg->przeciwnicy.jednostki[i].cur.x + mg->planszaShift;
			float p_y = mg->przeciwnicy.jednostki[i].cur.y - poczatek_planszy_y;

			float a = ((1.0) * dy) / dx;
			float b = a_y - a * a_x;
			pom_y = a * p_x + b;
			mg->przeciwnicy.jednostki[i].cur.y = (int)pom_y + poczatek_planszy_y;
		}
	}
}

/*
aktualizacja pozycji monet, na oknie, oraz zmiana ich przyspieszenia
*/
void aktualizacja_polozenia_monet(struct ModelGry *mg) {
	for (int i = 0; i < (*mg).monety.liczbaMonet; i++) {
		if ((*mg).monety.moneta[i].zbita && !(*mg).monety.moneta[i].dodana_po_zbiciu &&
			(*mg).monety.moneta[i].currPos.y <= ((*mg).plansza.rozmiar_y * ROZMIAR_BLOKU)) {
			(*mg).monety.moneta[i].currPos.y -= (*mg).monety.moneta[i].predkosc_y;
			(*mg).monety.moneta[i].predkosc_y -= MARIO_PREDKOSC_GRAWITACJA;
		}
		if ((*mg).monety.moneta[i].zbita && !(*mg).monety.moneta[i].dodana_po_zbiciu &&
			(*mg).monety.moneta[i].currPos.y > ((*mg).plansza.rozmiar_y * ROZMIAR_BLOKU)) {
			(*mg).monety.moneta[i].dodana_po_zbiciu = true;
			(*mg).monety.liczbaZebranychMonetNaEtapie++;
		}
	}
}

/*
animowanie Mario poprzez odpowiednia zmiane indeksu jego grafiki
*/
void zmiana_obrazka_mario(struct ModelGry *mg) {
	int vx = (*mg).marioStan.predkosc_x;
	int vy = (*mg).marioStan.predkosc_y;
	int new_obrazek_idx;
	int cur_obrazek_idx = (*mg).marioStan.indeks_obrazka;

	if (vx == 0 && vy == 0)
		new_obrazek_idx = MARIO_STOI_IDX;
	else { // vx != 0 || vy != 0
		if (vx > 0) {
			if (vy == 0) {
				switch (cur_obrazek_idx)
				{
				case MARIO_STOI_IDX:
				case MARIO_IDZIE_PRAWO_3_IDX:
					new_obrazek_idx = MARIO_IDZIE_PRAWO_1_IDX;
					break;
				case MARIO_IDZIE_PRAWO_1_IDX:
					new_obrazek_idx = MARIO_IDZIE_PRAWO_2_IDX;
					break;
				case MARIO_IDZIE_PRAWO_2_IDX:
					new_obrazek_idx = MARIO_IDZIE_PRAWO_3_IDX;
					break;
				default:
					new_obrazek_idx = MARIO_IDZIE_PRAWO_1_IDX;
					break;
				}
			}
			else
				new_obrazek_idx = MARIO_SKOK_PRAWO_IDX;
		}
		else {
			if (vx < 0) {
				if (vy == 0) {
					switch (cur_obrazek_idx)
					{
					case MARIO_STOI_IDX:
					case MARIO_IDZIE_LEWO_3_IDX:
						new_obrazek_idx = MARIO_IDZIE_LEWO_1_IDX;
						break;
					case MARIO_IDZIE_LEWO_1_IDX:
						new_obrazek_idx = MARIO_IDZIE_LEWO_2_IDX;
						break;
					case MARIO_IDZIE_LEWO_2_IDX:
						new_obrazek_idx = MARIO_IDZIE_LEWO_3_IDX;
						break;
					default:
						new_obrazek_idx = MARIO_IDZIE_LEWO_1_IDX;
						break;
					}
				}
				else
					new_obrazek_idx = MARIO_SKOK_LEWO_IDX;
			}
			else // vx == 0
				switch (cur_obrazek_idx)
				{
				case MARIO_STOI_IDX:
				case MARIO_IDZIE_PRAWO_1_IDX:
				case MARIO_IDZIE_PRAWO_2_IDX:
				case MARIO_IDZIE_PRAWO_3_IDX:
				case MARIO_SKOK_PRAWO_IDX:
					new_obrazek_idx = MARIO_SKOK_PRAWO_IDX;
					break;
				default:
					new_obrazek_idx = MARIO_SKOK_LEWO_IDX;
					break;
				}
		}
	}
	(*mg).marioStan.indeks_obrazka = new_obrazek_idx;
}

/*
animowanie Przeciwnika poprzez odpowiednia zmiane indeksu jego grafiki
*/
void zmiana_obrazka_przeciwnik(struct ModelGry *mg) {
	int liczba_przec = mg->przeciwnicy.liczbaPrzeciwnikow;
	int poczatek_planszy_y = WYSOKOSC_OKNA - ((*mg).plansza.rozmiar_y * ROZMIAR_BLOKU);

	for (int i = 0; i < liczba_przec; i++) { // przeciwnik osiagnal koniec drogi
		if (odleglosc_punktow(mg->przeciwnicy.jednostki[i].cur.x,
			mg->przeciwnicy.jednostki[i].cur.y,
			mg->przeciwnicy.jednostki[i].posEnd2.x * ROZMIAR_BLOKU,
			mg->przeciwnicy.jednostki[i].posEnd2.y * ROZMIAR_BLOKU + poczatek_planszy_y) < PRZECIWNIK_PREDKOSC ||
			odleglosc_punktow(mg->przeciwnicy.jednostki[i].cur.x,
				mg->przeciwnicy.jednostki[i].cur.y,
				mg->przeciwnicy.jednostki[i].posEnd1.x * ROZMIAR_BLOKU,
				mg->przeciwnicy.jednostki[i].posEnd1.y * ROZMIAR_BLOKU + poczatek_planszy_y) < PRZECIWNIK_PREDKOSC)
			mg->przeciwnicy.jednostki[i].indeks_obrazka = PRZECIWNIK_STOI_IDX;
		else
		{
			switch (mg->przeciwnicy.jednostki[i].indeks_obrazka)
			{
			case PRZECIWNIK_IDZIE_1_IDX:
				mg->przeciwnicy.jednostki[i].indeks_obrazka = PRZECIWNIK_IDZIE_2_IDX;
				break;
			case PRZECIWNIK_IDZIE_2_IDX:
				mg->przeciwnicy.jednostki[i].indeks_obrazka = PRZECIWNIK_IDZIE_3_IDX;
				break;
			case PRZECIWNIK_IDZIE_3_IDX:
			case PRZECIWNIK_STOI_IDX:
				mg->przeciwnicy.jednostki[i].indeks_obrazka = PRZECIWNIK_IDZIE_1_IDX;
				break;
			}
		}
	}
}
