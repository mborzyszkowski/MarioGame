#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include "wczytywanie.h"
#include "indeksy_grafik.h"
#include "wyswietlanie.h"
#include "modelGry.h"
#include "aktualizacje.h"
#include "odswiezanie_i_zdarzenie_zmiany_zycia.h"
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"

#define ROZMIAR_BLOKU 32  // rozmiar jednego bloku w pikselach

#define SZEROKOSC_OKNA (25 * ROZMIAR_BLOKU)   // szerokosc okna w pikselach (25 blokow)
#define WYSOKOSC_OKNA	600	                  // wysokosc okna w pikselach
#define OKNO_PREDKOSC_PRZESUWANIA 3           // predkosc przeuwania okna przy ruchach Mario

#define LICZBA_FPS 30                // liczba klatek na sekunde
#define POCZATKOWA_LICZBA_ZYC  3     // poczatkowa liczba zyc Mario w grze

// predkosc poruszania sie w czasie jednej klatki
#define MARIO_PREDKOSC 3                                // postawowa predkosc Mario w poziomie
#define MARIO_PREDKOSC_W_GORE 15                        // poczatkowa predkosc Mario podczas skoku w gore
#define MARIO_PREDKOSC_GRAWITACJA 1                     // zmiana predkosci wynikajaca z grawitacji
#define PRZECIWNIK_PREDKOSC MARIO_PREDKOSC              // predkosc poruszania sie przeciwnikow
#define MONETA_PREDKOSC_W_GORE MARIO_PREDKOSC_W_GORE    // poczatkowa predkosc poruszania sie monet w gore

#endif // MAIN_H

