"""
Generuje wykresy porownawcze dla obu implementacji kolejki priorytetowej
na podstawie plikow CSV wygenerowanych przez program.

Uzycie:
    python wykresy.py

Wymagane biblioteki: matplotlib

Wykresy zapisywane sa do katalogu sprawozdanie/wykresy/.
"""

import csv
import os
import matplotlib.pyplot as plt

KATALOG_WYNIKOW = os.path.dirname(os.path.abspath(__file__))
KATALOG_DOCELOWY = os.path.normpath(os.path.join(KATALOG_WYNIKOW, "..", "sprawozdanie", "wykresy"))

NAZWA_KOPIEC = "pomiary_kopiec.csv"
NAZWA_LISTA = "pomiary_lista.csv"

# Nazwa kolumny w CSV -> tytul wykresu i etykieta
OPERACJE = {
    "insert":     "Operacja insert(e, p)",
    "extract":    "Operacja extractMax()",
    "peek":       "Operacja peek()",
    "modify":     "Operacja modifyKey(e, p)",
    "rozmiarOp":  "Operacja rozmiar()",
}


def wczytaj(plik):
    rozmiary = []
    kolumny = {kolumna: [] for kolumna in OPERACJE}
    with open(plik, newline='') as f:
        reader = csv.DictReader(f)
        for wiersz in reader:
            rozmiary.append(int(wiersz["rozmiar"]))
            for kolumna in OPERACJE:
                kolumny[kolumna].append(float(wiersz[kolumna]))
    return rozmiary, kolumny


def rysuj(operacja, tytul, n_kopiec, czasy_kopiec, n_lista, czasy_lista, sciezka):
    fig, ax = plt.subplots(figsize=(7.5, 5))
    ax.plot(n_kopiec, czasy_kopiec, marker='o', label='Kopiec binarny')
    ax.plot(n_lista, czasy_lista, marker='s', label='Lista wiazana')
    ax.set_xlabel("Rozmiar kolejki n")
    ax.set_ylabel("Sredni czas operacji [ns]")
    ax.set_title(tytul)
    ax.set_yscale('log')
    ax.set_xscale('log')
    ax.grid(True, which='both', linestyle='--', alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(sciezka)
    plt.close(fig)
    print(f"  zapisano: {sciezka}")


def main():
    plik_kopiec = os.path.join(KATALOG_WYNIKOW, NAZWA_KOPIEC)
    plik_lista = os.path.join(KATALOG_WYNIKOW, NAZWA_LISTA)

    if not os.path.exists(plik_kopiec) or not os.path.exists(plik_lista):
        print("Brak plikow CSV. Najpierw uruchom program i wybierz tryb pomiarow.")
        return

    n_kopiec, kol_kopiec = wczytaj(plik_kopiec)
    n_lista, kol_lista = wczytaj(plik_lista)

    os.makedirs(KATALOG_DOCELOWY, exist_ok=True)

    for kolumna, tytul in OPERACJE.items():
        sciezka = os.path.join(KATALOG_DOCELOWY, f"wykres_{kolumna}.pdf")
        rysuj(kolumna, tytul, n_kopiec, kol_kopiec[kolumna],
              n_lista, kol_lista[kolumna], sciezka)

    print("Gotowe.")


if __name__ == "__main__":
    main()
