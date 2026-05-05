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

    // Wypisuje zawartosc listy od glowy do konca. Elementy sa juz posortowane
    // malejaco po priorytecie, wiec to jest jednoczesnie kolejnosc wyciagania.
    void wypisz() const {
        if (glowa == nullptr) {
            std::cout << "(lista pusta)" << std::endl;
            return;
        }
        Wezel* aktualny = glowa;
        int i = 0;
        while (aktualny != nullptr) {
            std::cout << "[" << i << "] wart=" << aktualny->dane.wartosc
                      << " p=" << aktualny->dane.priorytet
                      << " nr=" << aktualny->dane.nrWstawienia << std::endl;
            aktualny = aktualny->nastepny;
            i++;
        }
    }

    // Zmienia priorytet pierwszego napotkanego elementu o wartosci e na nowyPriorytet.
    // Zwraca true jesli element zostal znaleziony, false w przeciwnym razie.
    // Element zachowuje swoj oryginalny nrWstawienia, dzieki czemu jego pozycja
    // wzgledem innych elementow o tym samym nowym priorytecie jest zgodna z FIFO.
    // Zlozonosc O(n) - liniowe wyszukiwanie i ponowne wstawienie.
    bool modifyKey(int e, int nowyPriorytet) {
        // Szukamy wezla o wartosci e oraz jego poprzednika.
        Wezel* poprzedni = nullptr;
        Wezel* aktualny = glowa;
        while (aktualny != nullptr && aktualny->dane.wartosc != e) {
            poprzedni = aktualny;
            aktualny = aktualny->nastepny;
        }
        if (aktualny == nullptr) {
            return false;
        }

        // Wypinamy wezel z listy.
        if (poprzedni == nullptr) {
            glowa = aktualny->nastepny;
        } else {
            poprzedni->nastepny = aktualny->nastepny;
        }

        // Aktualizujemy priorytet zachowujac numer wstawienia.
        aktualny->dane.priorytet = nowyPriorytet;
        aktualny->nastepny = nullptr;

        // Wstawiamy ponownie w odpowiednie miejsce wedlug regul FIFO.
        if (glowa == nullptr || wyzszyPriorytet(aktualny->dane, glowa->dane)) {
            aktualny->nastepny = glowa;
            glowa = aktualny;
            return true;
        }
        Wezel* iter = glowa;
        while (iter->nastepny != nullptr && wyzszyPriorytet(iter->nastepny->dane, aktualny->dane)) {
            iter = iter->nastepny;
        }
        aktualny->nastepny = iter->nastepny;
        iter->nastepny = aktualny;
        return true;
    }
};
