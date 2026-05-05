#pragma once

// Wspolna definicja elementu kolejki priorytetowej.
// Pole nrWstawienia sluzy do rozstrzygania kolejnosci elementow
// o tym samym priorytecie wedlug strategii FIFO - mniejszy numer
// oznacza element wstawiony wczesniej, czyli majacy pierwszenstwo.
struct ElementKolejki {
    int wartosc;
    int priorytet;
    long long nrWstawienia;
};

// Zwraca true gdy element a ma wyzszy priorytet od b.
// Przy rownych priorytetach pierwszenstwo ma element wstawiony wczesniej (FIFO).
inline bool wyzszyPriorytet(const ElementKolejki& a, const ElementKolejki& b) {
    if (a.priorytet != b.priorytet) {
        return a.priorytet > b.priorytet;
    }
    return a.nrWstawienia < b.nrWstawienia;
}
