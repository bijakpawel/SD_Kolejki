#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include "pomocnik.h"

// Konfiguracja pomiarow.
// FAKTOR_ZAKRESU - mnoznik dla zakresu losowanych priorytetow wzgledem rozmiaru kolejki.
// Zgodnie z trescia projektu zakres priorytetow ma byc kilkukrotnie wiekszy niz rozmiar struktury.
constexpr int FAKTOR_ZAKRESU = 5;

// Liczba powtorzen calego eksperymentu dla danego rozmiaru. Wyniki sa usredniane.
constexpr int LICZBA_POWTORZEN = 5;

// Liczba operacji wykonywanych w jednej serii pomiarowej (peek, modify, extract).
// Pojedyncze wywolanie jest za szybkie aby zmierzyc je dokladnie zegarem,
// dlatego mierzymy laczny czas K wywolan i dzielimy.
constexpr int LICZBA_OPERACJI = 1000;

// Liczba wywolan w pomiarze rozmiar() - operacja jest na tyle szybka,
// ze potrzeba duzej liczby powtorzen.
constexpr int LICZBA_OPERACJI_ROZMIAR = 100000;

// Rozmiary kolejki uzywane w pomiarach.
inline std::vector<int> domyslneRozmiary() {
    return { 1000, 2000, 5000, 10000, 20000, 50000, 100000 };
}

// Wykonuje pomiary wszystkich operacji dla podanej struktury i zapisuje wyniki do CSV.
// Format pliku: naglowek "rozmiar,insert,extract,peek,modify,rozmiarOp", wartosci w nanosekundach.
template <typename Kolejka>
void zmierzKolejke(const std::vector<int>& rozmiary, const std::string& nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        std::cout << "Nie mozna otworzyc pliku do zapisu: " << nazwaPliku << std::endl;
        return;
    }
    plik << "rozmiar,insert,extract,peek,modify,rozmiarOp\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distWart(-1000, 1000);

    for (int n : rozmiary) {
        double sumaInsert = 0.0, sumaExtract = 0.0, sumaPeek = 0.0;
        double sumaModify = 0.0, sumaRozmiar = 0.0;

        int zakresPrio = n * FAKTOR_ZAKRESU;
        std::uniform_int_distribution<> distPrio(0, zakresPrio - 1);

        for (int r = 0; r < LICZBA_POWTORZEN; r++) {
            Kolejka kolejka;

            // --- Pomiar insert: czas zbudowania kolejki o n elementach ---
            // Pre-generujemy wszystkie pary aby pomiar nie obejmowal kosztu losowania.
            std::vector<int> wart(n), prio(n);
            for (int i = 0; i < n; i++) {
                wart[i] = distWart(gen);
                prio[i] = distPrio(gen);
            }

            Zegar zegar;
            zegar.start();
            for (int i = 0; i < n; i++) {
                kolejka.insert(wart[i], prio[i]);
            }
            sumaInsert += static_cast<double>(zegar.stop()) / n;

            // --- Pomiar peek: K powtorzen na pelnej kolejce (rozmiar = n) ---
            int Kp = LICZBA_OPERACJI;
            volatile int blokadaOptymalizacji = 0;
            zegar.start();
            for (int i = 0; i < Kp; i++) {
                blokadaOptymalizacji += kolejka.peek().wartosc;
            }
            sumaPeek += static_cast<double>(zegar.stop()) / Kp;

            // --- Pomiar rozmiar: bardzo szybka operacja, wiele powtorzen ---
            int Kr = LICZBA_OPERACJI_ROZMIAR;
            zegar.start();
            for (int i = 0; i < Kr; i++) {
                blokadaOptymalizacji += kolejka.rozmiar();
            }
            sumaRozmiar += static_cast<double>(zegar.stop()) / Kr;

            // --- Pomiar modify: K modyfikacji na pelnej kolejce ---
            int Km = std::min(LICZBA_OPERACJI, n);
            std::vector<int> wartModify(Km), prioModify(Km);
            for (int i = 0; i < Km; i++) {
                wartModify[i] = distWart(gen);
                prioModify[i] = distPrio(gen);
            }
            zegar.start();
            for (int i = 0; i < Km; i++) {
                kolejka.modifyKey(wartModify[i], prioModify[i]);
            }
            sumaModify += static_cast<double>(zegar.stop()) / Km;

            // --- Pomiar extract: K wyciagniec (rozmiar maleje, ale K << n) ---
            int Kx = std::min(LICZBA_OPERACJI, n);
            zegar.start();
            for (int i = 0; i < Kx; i++) {
                kolejka.extractMax();
            }
            sumaExtract += static_cast<double>(zegar.stop()) / Kx;

            (void)blokadaOptymalizacji;
        }

        double sredniInsert = sumaInsert / LICZBA_POWTORZEN;
        double sredniExtract = sumaExtract / LICZBA_POWTORZEN;
        double sredniPeek = sumaPeek / LICZBA_POWTORZEN;
        double sredniModify = sumaModify / LICZBA_POWTORZEN;
        double sredniRozmiar = sumaRozmiar / LICZBA_POWTORZEN;

        plik << n << ","
             << sredniInsert << ","
             << sredniExtract << ","
             << sredniPeek << ","
             << sredniModify << ","
             << sredniRozmiar << "\n";
        plik.flush();

        std::cout << "  n=" << n
                  << "  insert=" << sredniInsert << " ns"
                  << "  extract=" << sredniExtract << " ns"
                  << "  peek=" << sredniPeek << " ns"
                  << "  modify=" << sredniModify << " ns"
                  << std::endl;
    }

    plik.close();
}
