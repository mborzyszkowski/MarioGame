#include "main.h"

int main(int argc, char **argv) {
	SDL_Window *okno_glowne;
	SDL_Event zdarzenie, zdarzenie_pomocnicze;
	SDL_Renderer *render_okna;
	SDL_Texture *teskstura_okna;
	SDL_Surface *powierzchnia_okna = NULL, *powierzchnia_tla = NULL, *moneta_grafika = NULL;

	SDL_Surface *elementy_planszy[OKNO_ELEM_NUM];
	for (int i = 0; i < OKNO_ELEM_NUM; i++) elementy_planszy[i] = NULL;

	SDL_Surface *znaki = NULL;

	SDL_Surface *mario_grafiki[MARIO_GRAF_NUM];
	for (int i = 0; i < MARIO_GRAF_NUM; i++) mario_grafiki[i] = NULL;

	SDL_Surface *przeciwnik_grafiki[PRZECIWNIK_GRAF_NUM];
	for (int i = 0; i < PRZECIWNIK_GRAF_NUM; i++) przeciwnik_grafiki[i] = NULL;

	uint8_t kolizja;

	// Mario nie moze opadac szybciej niz 1/2 rozmiaru bloku na klatke
	int predkosc_w_dol_max = ROZMIAR_BLOKU / 2;

	uint32_t kolor_tla;

	bool wyjscie = false, koniec_gry = false;
	int czas_start, czas_aktualny;
	int czas_fps;

	int aktualny_poziom_gry = 1;

	struct ModelGry model_gry;

	// ustalenie liczby etapow i wczytanie pierwszego etapu
	int max_liczba_etapow = liczba_etapow();
	uint16_t kod_bledu_odczytu_etapu = init_model_gry(1, &model_gry);
	model_gry.numer_etapu = 1;

	model_gry.liczbaZyc = POCZATKOWA_LICZBA_ZYC;

	if (inicjalizacja_okna_glownego(&okno_glowne, &render_okna, &powierzchnia_okna, &teskstura_okna) != 0)
		return 1;

	// kolor tla #043D8A #0000FF
	kolor_tla = SDL_MapRGB(powierzchnia_okna->format, 0x04, 0x3D, 0x8A);
	SDL_FillRect(powierzchnia_okna, NULL, kolor_tla);

	// czcionka 
	ladowanie_obrazka(&znaki, "obrazki/literkiCyferki.bmp", SDL_TRUE, 0x000000, false, kolor_tla);
	uint32_t bialy = 0xFFFFFF;
	ladowanie_obrazka(&moneta_grafika, "obrazki/Moneta.bmp", SDL_TRUE, bialy, false, kolor_tla);

	// wczytywanie grafik
	wczytaj_elementy_planszy(elementy_planszy);
	wczytaj_mario(mario_grafiki, kolor_tla);
	wczytaj_przeciwnik(przeciwnik_grafiki, kolor_tla);

	wczytaj_plansze(&powierzchnia_tla, model_gry, elementy_planszy, kolor_tla);

	czas_start = SDL_GetTicks();
	czas_fps = SDL_GetTicks();

	mario_na_poczatek_etapu(&model_gry, 0, &czas_start);
	do {
		SDL_FillRect(powierzchnia_okna, NULL, kolor_tla);

		// czas
		czas_aktualny = SDL_GetTicks();
		model_gry.etapCzasDoKonca = model_gry.etapCzasMax - (int)((czas_aktualny - czas_start) * 0.001);
		model_gry.etapCzasDoKonca = (model_gry.etapCzasDoKonca >= 0 ? model_gry.etapCzasDoKonca : 0);
		
		// Mario traci zycie po uplywie czasu
		if (model_gry.etapCzasDoKonca == 0) {
			mario_na_poczatek_etapu(&model_gry, -1, &czas_start);
		}

		SDL_RenderClear(render_okna);

		// Mario straci³ wszystkie zycia
		if(kod_bledu_odczytu_etapu != 0)
			wypisz_tekst(powierzchnia_okna, 10, 10, "Blad odczytu etapu!!! wczytac raz jeszcze? T/N", znaki);
		else
		if (model_gry.liczbaZyc == 0)
			wypisz_tekst(powierzchnia_okna, 10, 10, "Koniec gry przegrana!!! rozpoczac nowa gre? T/N", znaki);
		else if (koniec_gry)
			wypisz_tekst(powierzchnia_okna, 10, 10, "Koniec gry wygrana!!! rozpoczac nowa gre? T/N", znaki);
		else
			wypisz_stan_gry(powierzchnia_okna, znaki, model_gry, 10, 10);

		// aktualizacja ekranu
		rysuj_tlo(powierzchnia_okna, powierzchnia_tla, model_gry.planszaShift);
		rysuj_przeciwnicy(powierzchnia_okna, przeciwnik_grafiki, model_gry);
		rysuj_spadajace_monety(powierzchnia_okna, moneta_grafika, model_gry);
		rysuj_mario(powierzchnia_okna, mario_grafiki, model_gry);
		
		// zaladowanie i wyswietlenie tekstury
		SDL_UpdateTexture(teskstura_okna, NULL, powierzchnia_okna->pixels, powierzchnia_okna->pitch);
		SDL_RenderCopy(render_okna, teskstura_okna, NULL, NULL);
		SDL_RenderPresent(render_okna);
		
		// zdarzenia
		while (SDL_PollEvent(&zdarzenie)) {
			if (model_gry.liczbaZyc != 0 && !koniec_gry && kod_bledu_odczytu_etapu == 0 ) {
				switch (zdarzenie.type)
				{
				case SDL_KEYDOWN:
					switch (zdarzenie.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						wyjscie = true;
						break;
					case SDLK_n:
						model_gry.liczbaZyc = POCZATKOWA_LICZBA_ZYC;
						model_gry.numer_etapu = 1;
						kod_bledu_odczytu_etapu = init_model_gry(model_gry.numer_etapu, &model_gry);
						wczytaj_plansze(&powierzchnia_tla, model_gry, elementy_planszy, kolor_tla);
						czas_start = SDL_GetTicks();
						mario_na_poczatek_etapu(&model_gry, 0, &czas_start);
						model_gry.monety.liczbaMonetNaPoczatkuEtapu = 0;
						break;
					case SDLK_s:
						zapisz_stan_gry(model_gry);
						break;
					case SDLK_l:
						odczyt_stanu_gry(&model_gry);
						wczytaj_plansze(&powierzchnia_tla, model_gry, elementy_planszy, kolor_tla);
						czas_start = SDL_GetTicks() - (model_gry.etapCzasMax - model_gry.etapCzasDoKonca) * 1000;
						zdarzenie_pomocnicze.type = SDL_KEYUP;
						zdarzenie_pomocnicze.key.keysym.sym = SDLK_RIGHT;
						SDL_PushEvent(&zdarzenie_pomocnicze);
						break;
					case SDLK_RIGHT:
						model_gry.marioStan.predkosc_x = MARIO_PREDKOSC;
						break;
					case SDLK_LEFT:
						model_gry.marioStan.predkosc_x = -MARIO_PREDKOSC;
						break;
					case SDLK_UP:
						if (model_gry.marioStan.predkosc_y == 0 && !model_gry.marioStan.w_trakcie_skoku) {
							model_gry.marioStan.predkosc_y = MARIO_PREDKOSC_W_GORE;
							model_gry.marioStan.w_trakcie_skoku = true;
						}
						break;
					}
					break;
				case SDL_KEYUP:
					switch (zdarzenie.key.keysym.sym)
					{
					case SDLK_RIGHT:
						model_gry.marioStan.predkosc_x = 0;
						break;
					case SDLK_LEFT:
						model_gry.marioStan.predkosc_x = 0;
						break;
					}
					break;
				}
			}
			else if (model_gry.liczbaZyc == 0 || koniec_gry || kod_bledu_odczytu_etapu != 0)
			{ // liczba zyc == 0
				switch (zdarzenie.type)
				{
				case SDL_KEYDOWN:
					switch (zdarzenie.key.keysym.sym)
					{
					case SDLK_t:
						model_gry.liczbaZyc = POCZATKOWA_LICZBA_ZYC;
						model_gry.numer_etapu = 1;
						kod_bledu_odczytu_etapu = init_model_gry(model_gry.numer_etapu, &model_gry);
						czas_start = SDL_GetTicks();
						mario_na_poczatek_etapu(&model_gry, 0, &czas_start);
						model_gry.monety.liczbaMonetNaPoczatkuEtapu = 0;
						if (koniec_gry) koniec_gry = false;
						break;
					case SDLK_n:
						wyjscie = true;
						break;
					case SDLK_ESCAPE:
						wyjscie = true;
						break;
					}
				}
			}
		}
		// grawitacja dziala na mario w trakcie skoku
		if (model_gry.marioStan.w_trakcie_skoku)
			model_gry.marioStan.predkosc_y -= MARIO_PREDKOSC_GRAWITACJA;
		else // dosuniecie do podloza
			if (model_mario_aktualizacja_pozycji(&model_gry, 0, -1, true, &czas_start) == 0) {
				model_gry.marioStan.predkosc_y = -1;
				model_gry.marioStan.w_trakcie_skoku = true;
			}
		// korekta predkosci opadania
		if (model_gry.marioStan.predkosc_y < -1 * predkosc_w_dol_max)
			model_gry.marioStan.predkosc_y = -1 * predkosc_w_dol_max;

		// aktualizacja polozen elementow
		aktualizacja_polozenia_planszy(&model_gry);
		aktualizacja_polozenia_przeciwnikow(&model_gry);
		zmiana_obrazka_przeciwnik(&model_gry);
		zmiana_obrazka_mario(&model_gry);
		aktualizacja_polozenia_monet(&model_gry);

		// badanie kolizji
		kolizja = model_mario_aktualizacja_pozycji(&model_gry,
			model_gry.marioStan.predkosc_x,
			model_gry.marioStan.predkosc_y,
			false, &czas_start);
		if ((kolizja & KOLIZJA_KONIEC_ETAPU) > 0) {
			if (model_gry.numer_etapu < max_liczba_etapow) {
				model_gry.numer_etapu += 1;
				model_gry.monety.liczbaMonetNaPoczatkuEtapu += model_gry.monety.liczbaZebranychMonetNaEtapie;
				kod_bledu_odczytu_etapu = init_model_gry(model_gry.numer_etapu, &model_gry);
				czas_start = SDL_GetTicks();
				mario_na_poczatek_etapu(&model_gry, 0, &czas_start);
			}
			else {
				koniec_gry = true;
			}
		}
		wczytaj_plansze(&powierzchnia_tla, model_gry, elementy_planszy, kolor_tla);

		// wyrownanie klatek na sekunde
		czas_fps = dopasuj_fps(czas_fps);
	} while (!wyjscie);

	// zwalnianie pamiecie i wyjscie z programu
	for (int i = 0; i < OKNO_ELEM_NUM; i++)
	SDL_FreeSurface(elementy_planszy[i]);
	SDL_FreeSurface(powierzchnia_okna);
	SDL_FreeSurface(powierzchnia_tla);
	SDL_FreeSurface(moneta_grafika);
	SDL_FreeSurface(znaki);
	for (int i = 0; i < MARIO_GRAF_NUM; i++) 
		SDL_FreeSurface(mario_grafiki[i]);
	for (int i = 0; i < PRZECIWNIK_GRAF_NUM; i++) 
		SDL_FreeSurface(przeciwnik_grafiki[i]);
	free_model_gry(&model_gry);
	SDL_DestroyTexture(teskstura_okna);
	SDL_DestroyRenderer(render_okna);
	SDL_DestroyWindow(okno_glowne);

	// wyjscie
	SDL_Quit();
	return 0;
};
