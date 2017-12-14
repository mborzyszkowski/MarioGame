#ifndef KOLIZJA_H
#define KOLIZJA_H

// kodowanie kolizji
#define KOLIZJA_BRAK 0             // brak kolizji
#define KOLIZJA_BLOK_00 1          // kolizja z blokiem gornym lewym
#define KOLIZJA_BLOK_10 2          // kolizja z blokiem gornym prawym
#define KOLIZJA_BLOK_01 4          // kolizja z blokiem dolnym lewym
#define KOLIZJA_BLOK_11 8          // kolizja z blokiem dolnym prawym
#define KOLIZJA_PLANSZA 16         // kolizja z plansza (wyjscie poza plansze)
#define KOLIZJA_PRZECIWNIK 32      // kolizja z przeciwnikiem
#define KOLIZJA_KONIEC_ETAPU 64    // kolizja z blokiem przenoszacym do nastepnego etapu
#define KOLIZJA_Z_PLATFORMA 128    // kolizja z platforma przy zbijaniu platformy przez Mario glowa z dolu

// kodowanie punktow kolizji prostokatow
#define PUNKT_KOLIZJI_0 1          // bok gorny z lewej strony
#define PUNKT_KOLIZJI_1 2          // bok gorny z prawej strony
#define PUNKT_KOLIZJI_2 4          // bok prawy z gory
#define PUNKT_KOLIZJI_3 8          // bok prawy z dolu
#define PUNKT_KOLIZJI_4 16         // bok dolny z prawej
#define PUNKT_KOLIZJI_5 32         // bok dolny z lewej
#define PUNKT_KOLIZJI_6 64         // bok lewy z dolu
#define PUNKT_KOLIZJI_7 128        // bok lewy z gory

uint8_t model_mario_aktualizacja_pozycji(struct ModelGry *mg, int przesuniecieX, int przesuniecieY, bool czy_test_kolizji, int *czas_start);
void aktualizacja_polozenia_planszy(struct ModelGry *mg);
void aktualizacja_polozenia_przeciwnikow(struct ModelGry *mg);
void aktualizacja_polozenia_monet(struct ModelGry *mg);
void zmiana_obrazka_mario(struct ModelGry *mg);
void zmiana_obrazka_przeciwnik(struct ModelGry *mg);

#endif // KOLIZJA_H
