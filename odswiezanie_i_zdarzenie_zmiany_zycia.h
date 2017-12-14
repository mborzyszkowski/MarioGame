#ifndef ODSWIEZANIE_I_ZDARZENIE_ZMIANY_ZYCIA_H
#define ODSWIEZANIE_I_ZDARZENIE_ZMIANY_ZYCIA_H

// odswiezanie_i_zdarzenie_zmiany_zycia.cpp
int dopasuj_fps(int ostatnie_tykniecie);
void mario_na_poczatek_etapu(struct ModelGry *mg, int zmiana_zyc, int* czas_start);
void przywroc_platformy(struct ModelGry *mg);

#endif // ODSWIEZANIE_I_ZDARZENIE_ZMIANY_ZYCIA_H