#pragma once
#include <fstream>
#include <random>
#include <chrono>
#include <iostream>

// Generuje plik z n losowymi liczbami calkowitymi z zakresu [-1000, 1000]
void generujLiczby(int n, const char* nazwaPliku = "liczby.txt") {
    std::ofstream plik(nazwaPliku);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1000, 1000);

    for (int i = 0; i < n; i++) {
        plik << dist(gen) << "\n";
    }

    plik.close();
}

// Klasa do mierzenia czasu wykonania operacji
class Zegar {
    std::chrono::steady_clock::time_point moment_start;

public:
    void start() {
        moment_start = std::chrono::steady_clock::now();
    }

    std::chrono::nanoseconds::rep stop() {
        auto moment_stop = std::chrono::steady_clock::now();
        auto czas = std::chrono::duration_cast<std::chrono::nanoseconds>(moment_stop - moment_start);
        return czas.count();
    }
};

// Mierzy czas wykonania przekazanej operacji (w nanosekundach)
template <typename Funkcja>
double zmierzCzas(Funkcja operacja) {
    Zegar zegar;
    zegar.start();
    operacja();
    return static_cast<double>(zegar.stop());
}

// Wypelnia kolejke priorytetowa n losowymi elementami.
// Wartosci losowane sa z zakresu [-1000, 1000], priorytety z zakresu [0, zakresPriorytetow-1].
// Zaleca sie aby zakresPriorytetow byl kilkukrotnie wiekszy od n (zgodnie z trescia projektu).
template <typename Kolejka>
void wypelnijKolejke(Kolejka& kolejka, int n, int zakresPriorytetow) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distWart(-1000, 1000);
    std::uniform_int_distribution<> distPrio(0, zakresPriorytetow - 1);

    for (int i = 0; i < n; i++) {
        kolejka.insert(distWart(gen), distPrio(gen));
    }
}
