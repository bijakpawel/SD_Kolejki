#pragma once
#include <iostream>
#include "kolejka.h"

// Kolejka priorytetowa typu MAX zaimplementowana jako jednokierunkowa
// lista wiazana posortowana malejaco po priorytecie. Element o najwyzszym
// priorytecie znajduje sie na poczatku listy (glowa).
// Dla rownych priorytetow zachowywana jest kolejnosc FIFO - nowy element
// trafia ZA wszystkie istniejace o tym samym priorytecie.
class KolejkaLista {
private:
    struct Wezel {
        ElementKolejki dane;
        Wezel* nastepny;
    };

    Wezel* glowa;
    int rozmiarObecny;
    long long licznikWstawien;

public:
    KolejkaLista() {
        glowa = nullptr;
        rozmiarObecny = 0;
        licznikWstawien = 0;
    }

    ~KolejkaLista() {
        Wezel* aktualny = glowa;
        while (aktualny != nullptr) {
            Wezel* dalej = aktualny->nastepny;
            delete aktualny;
            aktualny = dalej;
        }
    }

    // Wstawia element o wartosci e i priorytecie p w odpowiednie miejsce
    // w liscie posortowanej malejaco. Zlozonosc O(n).
    void insert(int e, int p) {
        Wezel* nowy = new Wezel;
        nowy->dane.wartosc = e;
        nowy->dane.priorytet = p;
        nowy->dane.nrWstawienia = licznikWstawien++;
        nowy->nastepny = nullptr;

        // Lista pusta lub nowy element ma scisle wyzszy priorytet od glowy.
        if (glowa == nullptr || p > glowa->dane.priorytet) {
            nowy->nastepny = glowa;
            glowa = nowy;
            rozmiarObecny++;
            return;
        }

        // Szukamy ostatniego wezla o priorytecie >= p (nowy idzie ZA niego, czyli FIFO).
        Wezel* aktualny = glowa;
        while (aktualny->nastepny != nullptr && aktualny->nastepny->dane.priorytet >= p) {
            aktualny = aktualny->nastepny;
        }
        nowy->nastepny = aktualny->nastepny;
        aktualny->nastepny = nowy;
        rozmiarObecny++;
    }

    // Usuwa i zwraca element o najwyzszym priorytecie (z glowy listy). Zlozonosc O(1).
    ElementKolejki extractMax() {
        if (glowa == nullptr) {
            std::cout << "Kolejka jest pusta!" << std::endl;
            return ElementKolejki{0, 0, 0};
        }
        ElementKolejki wynik = glowa->dane;
        Wezel* doUsuniecia = glowa;
        glowa = glowa->nastepny;
        delete doUsuniecia;
        rozmiarObecny--;
        return wynik;
    }

    // Zwraca element o najwyzszym priorytecie bez usuwania. Zlozonosc O(1).
    ElementKolejki peek() const {
        if (glowa == nullptr) {
            std::cout << "Kolejka jest pusta!" << std::endl;
            return ElementKolejki{0, 0, 0};
        }
        return glowa->dane;
    }

    int rozmiar() const {
        return rozmiarObecny;
    }

    bool pusta() const {
        return glowa == nullptr;
    }
};
