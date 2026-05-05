#include <iostream>
#include <string>
#include "kolejka_kopiec.h"
#include "kolejka_lista.h"
#include "pomocnik.h"

// Obsluguje menu operacji na podanej kolejce. Dziala dla dowolnej implementacji
// kolejki priorytetowej (kopiec lub lista) - wymagane jest jedynie zgodne API.
template <typename Kolejka>
void obslugaMenu(Kolejka& kolejka, const std::string& nazwa) {
    int op;
    do {
        std::cout << "\n--- Menu operacji (" << nazwa << ") ---\n";
        std::cout << "1. insert(e, p)\n";
        std::cout << "2. extractMax\n";
        std::cout << "3. peek\n";
        std::cout << "4. modifyKey(e, nowyPriorytet)\n";
        std::cout << "5. rozmiar\n";
        std::cout << "6. wypisz strukture\n";
        std::cout << "7. wypelnij losowymi danymi\n";
        std::cout << "8. wyczysc kolejke\n";
        std::cout << "0. powrot do wyboru implementacji\n";
        std::cout << "Wybor: ";
        std::cin >> op;

        switch (op) {
            case 1: {
                int e, p;
                std::cout << "Podaj wartosc e: ";
                std::cin >> e;
                std::cout << "Podaj priorytet p: ";
                std::cin >> p;
                kolejka.insert(e, p);
                std::cout << "Wstawiono element.\n";
                break;
            }
            case 2: {
                if (kolejka.pusta()) {
                    std::cout << "Kolejka jest pusta.\n";
                } else {
                    ElementKolejki el = kolejka.extractMax();
                    std::cout << "Pobrano: wart=" << el.wartosc
                              << " p=" << el.priorytet
                              << " nr=" << el.nrWstawienia << "\n";
                }
                break;
            }
            case 3: {
                if (kolejka.pusta()) {
                    std::cout << "Kolejka jest pusta.\n";
                } else {
                    ElementKolejki el = kolejka.peek();
                    std::cout << "Peek: wart=" << el.wartosc
                              << " p=" << el.priorytet
                              << " nr=" << el.nrWstawienia << "\n";
                }
                break;
            }
            case 4: {
                int e, p;
                std::cout << "Podaj wartosc elementu e do modyfikacji: ";
                std::cin >> e;
                std::cout << "Podaj nowy priorytet: ";
                std::cin >> p;
                bool ok = kolejka.modifyKey(e, p);
                if (ok) {
                    std::cout << "Zmodyfikowano priorytet.\n";
                } else {
                    std::cout << "Nie znaleziono elementu o wartosci " << e << ".\n";
                }
                break;
            }
            case 5: {
                std::cout << "Rozmiar kolejki: " << kolejka.rozmiar() << "\n";
                break;
            }
            case 6: {
                kolejka.wypisz();
                break;
            }
            case 7: {
                int n, zakres;
                std::cout << "Liczba elementow do wstawienia: ";
                std::cin >> n;
                std::cout << "Zakres priorytetow (X oznacza zakres [0, X-1]): ";
                std::cin >> zakres;
                if (n <= 0 || zakres <= 0) {
                    std::cout << "Nieprawidlowe parametry.\n";
                    break;
                }
                wypelnijKolejke(kolejka, n, zakres);
                std::cout << "Wstawiono " << n << " losowych elementow.\n";
                break;
            }
            case 8: {
                while (!kolejka.pusta()) {
                    kolejka.extractMax();
                }
                std::cout << "Kolejka wyczyszczona.\n";
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Nieznana opcja.\n";
                break;
        }
    } while (op != 0);
}

int main() {
    while (true) {
        std::cout << "\n===== Kolejka priorytetowa MAX =====\n";
        std::cout << "1. Kopiec binarny\n";
        std::cout << "2. Lista wiazana\n";
        std::cout << "0. Zakoncz program\n";
        std::cout << "Wybierz implementacje: ";
        int wybor;
        std::cin >> wybor;

        if (wybor == 1) {
            KolejkaKopiec kopiec;
            obslugaMenu(kopiec, "kopiec binarny");
        } else if (wybor == 2) {
            KolejkaLista lista;
            obslugaMenu(lista, "lista wiazana");
        } else if (wybor == 0) {
            std::cout << "Koniec programu.\n";
            break;
        } else {
            std::cout << "Nieznana opcja.\n";
        }
    }
    return 0;
}
