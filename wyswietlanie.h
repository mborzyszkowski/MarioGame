#ifndef WYSWIETLANIE_H
#define WYSWIETALNIE_H
#include"./sdl/include/SDL.h"

void rysuj_tlo(SDL_Surface *pow_okna, SDL_Surface *pow_tla, int przesuniecie);
void rysuj_mario(SDL_Surface *pow_okna, SDL_Surface **mario_graf, struct ModelGry modelGry);
void rysuj_przeciwnicy(SDL_Surface *pow_okna, SDL_Surface **przeciwnicy_graf, struct ModelGry modelGry);
void rysuj_spadajace_monety(SDL_Surface *pow_okna, SDL_Surface *moneta_graf, struct ModelGry modelGry);
void wypisz_tekst(SDL_Surface *ekran, int pos_x, int pos_y, const char *napis, SDL_Surface *zestaw_znakow);
void wypisz_stan_gry(SDL_Surface *ekran, SDL_Surface *zestaw_znakow, struct ModelGry mg, int pos_x, int pos_y);

#endif // WYSWIETALNIE_H
