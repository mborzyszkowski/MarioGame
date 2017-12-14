#include "main.h"

/*
zaladowanie obrazka do surface i nadanie jemu odpowiedniej przezroczystosci
*/
int ladowanie_obrazka(SDL_Surface **obiekt, char *src, int przezroczytosc, uint32_t kolor_przezroczystosci, bool symOY, uint32_t kolor_tla) {
	if (*obiekt != NULL) {
		SDL_FreeSurface(*obiekt);
	}
	*obiekt = SDL_LoadBMP(src);
	if (*obiekt == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Nie moge zaladowac obrazka: %s, SDL: %s", src, SDL_GetError());
		return 1;
	}
	if (przezroczytosc == SDL_TRUE) {
		SDL_SetColorKey(*obiekt, przezroczytosc, kolor_przezroczystosci);
	}
	if (symOY)
		symOY_Surface(*obiekt);
	//dodanie niebieskiego tla do postaci
	/*SDL_Surface *pom_surf = SDL_CreateRGBSurface(0, (*obiekt)->w, (*obiekt)->h, 32, 0, 0, 0, 0);
	SDL_FillRect(pom_surf, NULL, kolor_tla);
	SDL_BlitSurface(*obiekt, NULL, pom_surf, NULL);
	SDL_FreeSurface(*obiekt);
	*obiekt = pom_surf;*/
	return 0;
}

/*
obraca obrazek wzgledem osi OY
*/
void symOY_Surface(SDL_Surface *obraz) {
	int dlugosc_wiersza = obraz->w;
	int liczba_wierszy = obraz->h;
	int liczba_pixeli_w_wierszu = obraz->pitch;
	uint8_t *pixele = (uint8_t *)obraz->pixels;
	uint8_t tmp_pixel[3];
	for (int y = 0; y < liczba_wierszy; y++)
		for (int x = 0; x < dlugosc_wiersza / 2; x++) {
			for (int pixele_w_rgb_bez_przezroczystosci = 0; pixele_w_rgb_bez_przezroczystosci < 3; pixele_w_rgb_bez_przezroczystosci++) {
				if (3 * ((dlugosc_wiersza * y) + x) + pixele_w_rgb_bez_przezroczystosci <= liczba_pixeli_w_wierszu * (y+1)) {
					tmp_pixel[pixele_w_rgb_bez_przezroczystosci] = pixele[3 * ((dlugosc_wiersza * y) + x) + pixele_w_rgb_bez_przezroczystosci];
					pixele[3 * ((dlugosc_wiersza * y) + x) + pixele_w_rgb_bez_przezroczystosci] = pixele[3 * ((dlugosc_wiersza * y) + (dlugosc_wiersza - x - 1)) + pixele_w_rgb_bez_przezroczystosci];
					pixele[3 * ((dlugosc_wiersza * y) + (dlugosc_wiersza - x - 1)) + pixele_w_rgb_bez_przezroczystosci] = tmp_pixel[pixele_w_rgb_bez_przezroczystosci];
				}
			}
		}
}

/*
wczytanie grafik elementow planszy poprzez funkcje: ladowanie obrazka
*/
void wczytaj_elementy_planszy(SDL_Surface **elementy_planszy) {
	ladowanie_obrazka(elementy_planszy + PODSTAWA_IDX, "obrazki/Podstawa.bmp", SDL_FALSE, 0, false, 0);
	ladowanie_obrazka(elementy_planszy + PRZESZKODA_IDX, "obrazki/Przeszkoda.bmp", SDL_FALSE, 0, false, 0);
	ladowanie_obrazka(elementy_planszy + PLATFORMA_IDX, "obrazki/Platforma.bmp", SDL_FALSE, 0, false, 0);
	ladowanie_obrazka(elementy_planszy + PLATFORMA_Z_MONETA_IDX, "obrazki/MonetaCegla.bmp", SDL_FALSE, 0, false, 0);
}

/*
wczytanie wszystkich blokow planszy do jednego surface tworzac plansze
*/
void wczytaj_plansze(SDL_Surface **pow_tla, struct ModelGry modelGry, SDL_Surface **elementy_planszy, uint32_t kolor_tla) {
	int max_x_planszy = modelGry.plansza.rozmiar_x * ROZMIAR_BLOKU;
	int max_y_planszy = modelGry.plansza.rozmiar_y * ROZMIAR_BLOKU;

	SDL_Rect cel = { 0, 0, 0, 0 };

	SDL_Surface *elem_pl = NULL;

	if (*pow_tla != NULL)
		SDL_FreeSurface(*pow_tla);
	*pow_tla = SDL_CreateRGBSurface(0, max_x_planszy, max_y_planszy, 32, 0, 0, 0, 0);
	SDL_FillRect(*pow_tla, NULL, kolor_tla);

	cel.x = 0;
	cel.y = 0;
	cel.w = ROZMIAR_BLOKU;
	cel.h = ROZMIAR_BLOKU;
	for (int y = 0; y < modelGry.plansza.rozmiar_y; y++) {
		for (int x = 0; x < modelGry.plansza.rozmiar_x; x++) {
			switch (modelGry.plansza.pola[y][x])
			{
			case PODSTAWA:
				elem_pl = elementy_planszy[PODSTAWA_IDX];
				break;
			case PRZESZKODA:
				elem_pl = elementy_planszy[PRZESZKODA_IDX];
				break;
			case PLATFORMA:
				elem_pl = elementy_planszy[PLATFORMA_IDX];
				break;
			case PLATRORMA_Z_MONETA:
				elem_pl = elementy_planszy[PLATFORMA_Z_MONETA_IDX];
				break;
			default:
				elem_pl = NULL;
				break;
			}
			if (elem_pl)
				SDL_BlitScaled(elem_pl, NULL, *pow_tla, &cel);
			cel.x += ROZMIAR_BLOKU;

		}
		cel.y += ROZMIAR_BLOKU;
		cel.x = 0;
	}
}

/*
wczytanie grafik Mario planszy poprzez funkcje: ladowanie obrazka
*/
void wczytaj_mario(SDL_Surface **mario_grafiki, uint32_t kolor_tla) {
	uint32_t bialy = 0xFFFFFF;
	ladowanie_obrazka(mario_grafiki + MARIO_STOI_IDX, "obrazki/MarioStojacy.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_SKOK_PRAWO_IDX, "obrazki/MarioJump.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_SKOK_LEWO_IDX, "obrazki/MarioJump.bmp", SDL_TRUE, bialy, true, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_LEWO_1_IDX, "obrazki/MarioChodzi1.bmp", SDL_TRUE, bialy, true, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_LEWO_2_IDX, "obrazki/MarioChodzi2.bmp", SDL_TRUE, bialy, true, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_LEWO_3_IDX, "obrazki/MarioChodzi3.bmp", SDL_TRUE, bialy, true, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_PRAWO_1_IDX, "obrazki/MarioChodzi1.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_PRAWO_2_IDX, "obrazki/MarioChodzi2.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(mario_grafiki + MARIO_IDZIE_PRAWO_3_IDX, "obrazki/MarioChodzi3.bmp", SDL_TRUE, bialy, false, kolor_tla);
}

/*
wczytanie grafik Przeciwnika planszy poprzez funkcje: ladowanie obrazka
*/
void wczytaj_przeciwnik(SDL_Surface **przeciwnik_grafiki, uint32_t kolor_tla) {
	uint32_t bialy = 0xFFFFFF;
	ladowanie_obrazka(przeciwnik_grafiki + PRZECIWNIK_STOI_IDX, "obrazki/wrogStojacy.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(przeciwnik_grafiki + PRZECIWNIK_IDZIE_1_IDX, "obrazki/wrogChodzi1.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(przeciwnik_grafiki + PRZECIWNIK_IDZIE_2_IDX, "obrazki/wrogChodzi2.bmp", SDL_TRUE, bialy, false, kolor_tla);
	ladowanie_obrazka(przeciwnik_grafiki + PRZECIWNIK_IDZIE_3_IDX, "obrazki/wrogChodzi3.bmp", SDL_TRUE, bialy, false, kolor_tla);

}

/*
inicjalizacja okna glownego, renderu okna i tekstury
*/
int inicjalizacja_okna_glownego(SDL_Window **okno, SDL_Renderer ** wizualizator, SDL_Surface **powierzchnia, SDL_Texture **teskstura) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Nie moge zainicjalizowac SDL: %s", SDL_GetError());
		return 10;
	}
	if (SDL_CreateWindowAndRenderer(SZEROKOSC_OKNA, WYSOKOSC_OKNA, 0, okno, wizualizator) != 0) {
		SDL_Quit();
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Nie moge utworzyc okna SDL: %s", SDL_GetError());
		return 20;
	}
	else {
		*powierzchnia = SDL_GetWindowSurface(*okno);
		*teskstura = SDL_CreateTexture(*wizualizator, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SZEROKOSC_OKNA, WYSOKOSC_OKNA);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetWindowTitle(*okno, "Mario - Maciej Borzyszkowski: 165407");
	}
	return 0;
}