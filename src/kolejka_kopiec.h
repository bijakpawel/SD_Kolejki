#pragma once
#include <iostream>
#include "kolejka.h"

// Kolejka priorytetowa typu MAX zaimplementowana jako kopiec binarny.
// Kopiec przechowywany jest w dynamicznie alokowanej tablicy.
// Indeksowanie 0-bazowe: rodzic(i) = (i-1)/2, lewy(i) = 2i+1, prawy(i) = 2i+2.
class KolejkaKopiec {
private:
    ElementKolejki* tablica;
    int rozmiarObecny;
    int pojemnosc;
    long long licznikWstawien;

    // Powieksza tablice dwukrotnie gdy zabraknie miejsca.
    void zwiekszPojemnosc() {
        int nowaPojemnosc = pojemnosc * 2;
        ElementKolejki* nowa = new ElementKolejki[nowaPojemnosc];
        for (int i = 0; i < rozmiarObecny; i++) {
            nowa[i] = tablica[i];
        }
        delete[] tablica;
        tablica = nowa;
        pojemnosc = nowaPojemnosc;
    }

    // Przywraca wlasnosc kopca idac w gore od podanego indeksu.
    void kopcujWGore(int i) {
        while (i > 0) {
            int rodzic = (i - 1) / 2;
            if (wyzszyPriorytet(tablica[i], tablica[rodzic])) {
                ElementKolejki tmp = tablica[i];
                tablica[i] = tablica[rodzic];
                tablica[rodzic] = tmp;
                i = rodzic;
            } else {
                break;
            }
        }
    }

    // Przywraca wlasnosc kopca idac w dol od podanego indeksu.
    void kopcujWDol(int i) {
        while (true) {
            int lewy = 2 * i + 1;
            int prawy = 2 * i + 2;
            int najwyzszy = i;
            if (lewy < rozmiarObecny && wyzszyPriorytet(tablica[lewy], tablica[najwyzszy])) {
                najwyzszy = lewy;
            }
            if (prawy < rozmiarObecny && wyzszyPriorytet(tablica[prawy], tablica[najwyzszy])) {
                najwyzszy = prawy;
            }
            if (najwyzszy != i) {
                ElementKolejki tmp = tablica[i];
                tablica[i] = tablica[najwyzszy];
                tablica[najwyzszy] = tmp;
                i = najwyzszy;
            } else {
                break;
            }
        }
    }

public:
    KolejkaKopiec(int poczatkowaPojemnosc = 16) {
        if (poczatkowaPojemnosc < 1) poczatkowaPojemnosc = 1;
        pojemnosc = poczatkowaPojemnosc;
        tablica = new ElementKolejki[pojemnosc];
        rozmiarObecny = 0;
        licznikWstawien = 0;
    }

    ~KolejkaKopiec() {
        delete[] tablica;
    }

    // Wstawia element o wartosci e i priorytecie p. Zlozonosc O(log n).
    void insert(int e, int p) {
        if (rozmiarObecny == pojemnosc) {
            zwiekszPojemnosc();
        }
        ElementKolejki nowy;
        nowy.wartosc = e;
        nowy.priorytet = p;
        nowy.nrWstawienia = licznikWstawien++;
        tablica[rozmiarObecny] = nowy;
        kopcujWGore(rozmiarObecny);
        rozmiarObecny++;
    }

    // Usuwa i zwraca element o najwyzszym priorytecie. Zlozonosc O(log n).
    ElementKolejki extractMax() {
        if (rozmiarObecny == 0) {
            std::cout << "Kolejka jest pusta!" << std::endl;
            return ElementKolejki{0, 0, 0};
        }
        ElementKolejki najwyzszy = tablica[0];
        rozmiarObecny--;
        if (rozmiarObecny > 0) {
            tablica[0] = tablica[rozmiarObecny];
            kopcujWDol(0);
        }
        return najwyzszy;
    }

    // Zwraca element o najwyzszym priorytecie bez usuwania. Zlozonosc O(1).
    ElementKolejki peek() const {
        if (rozmiarObecny == 0) {
            std::cout << "Kolejka jest pusta!" << std::endl;
            return ElementKolejki{0, 0, 0};
        }
        return tablica[0];
    }

    // Zwraca liczbe elementow w kolejce. Zlozonosc O(1).
    int rozmiar() const {
        return rozmiarObecny;
    }

    bool pusta() const {
        return rozmiarObecny == 0;
    }

    // Zmienia priorytet pierwszego napotkanego elementu o wartosci e na nowyPriorytet.
    // Zwraca true jesli element zostal znaleziony, false w przeciwnym razie.
    // Zlozonosc O(n) - liniowe wyszukiwanie elementu plus O(log n) na naprawe kopca.
    bool modifyKey(int e, int nowyPriorytet) {
        int indeks = -1;
        for (int i = 0; i < rozmiarObecny; i++) {
            if (tablica[i].wartosc == e) {
                indeks = i;
                break;
            }
        }
        if (indeks == -1) {
            return false;
        }
        int staryPriorytet = tablica[indeks].priorytet;
        tablica[indeks].priorytet = nowyPriorytet;
        if (nowyPriorytet > staryPriorytet) {
            kopcujWGore(indeks);
        } else if (nowyPriorytet < staryPriorytet) {
            kopcujWDol(indeks);
        }
        return true;
    }
};
