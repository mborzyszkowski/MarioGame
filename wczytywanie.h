#ifndef WCZYTYWANIE_H
#define WCZYTYWANIE_H
#include"./sdl/include/SDL.h"

int ladowanie_obrazka(SDL_Surface **obiekt, char *src, int przezroczytosc, uint32_t kolor_przezroczystosci, bool symOY, uint32_t kolor_tla);
void symOY_Surface(SDL_Surface *obraz);
void wczytaj_elementy_planszy(SDL_Surface **elementy_planszy);
void wczytaj_plansze(SDL_Surface **pow_tla, struct ModelGry modelGry, SDL_Surface **elementy_planszy, uint32_t kolor_tla);
void wczytaj_mario(SDL_Surface **mario_grafiki, uint32_t kolor_tla);
void wczytaj_przeciwnik(SDL_Surface **przeciwnik_grafiki, uint32_t kolor_tla);
int inicjalizacja_okna_glownego(SDL_Window **okno, SDL_Renderer ** wizualizator, SDL_Surface **powierzchnia, SDL_Texture **teskstura);

#endif // WCZYTYWANIE_H
