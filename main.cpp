#include <iostream>
#include <SDL.h>
#include "tabla.h"

using namespace std;

void AfiseazaGrup(Tabla tabla)
{
    cout << "Grupul din care face parte fiecare patrat\n";
    for (int i = 0; i < tabla.getInaltime(); ++i) {
        for (int j = 0; j < tabla.getLatime(); ++j) {
            if (!tabla.m_Tabla[i][j].m_AreBomba) {
                cout << tabla.m_Tabla[i][j].m_Grup << " ";
            } else cout << "* ";
        }

        cout << endl;
    }
}

void Afiseaza(Tabla tabla)
{
    //cout << "\n";
    for (int i = 0; i < tabla.getInaltime(); ++i) {
        for (int j = 0; j < tabla.getLatime(); ++j) {
            if (tabla.m_Tabla[i][j].m_AreBomba) {
                cout << "* ";
            } else if (tabla.m_Tabla[i][j].m_BombeInJur) {
                cout << tabla.m_Tabla[i][j].m_BombeInJur << " ";
            } else {
                cout << "_ ";
            }
        }

        cout << endl;
    }
}

void AfiseazaID(Tabla tabla)
{
    cout << "ID-ul fiecarui patrat\n";
    for (int i = 0; i < tabla.getInaltime(); ++i) {
        for (int j = 0; j < tabla.getLatime(); ++j) {
            cout << tabla.m_Tabla[i][j].m_ID << " ";
        }

        cout << endl;
    }
}

int main(int argc, char* argv[])
{
    Tabla t(9, 9, 10);
    int x, y;

    Afiseaza(t);
    cout << endl;
    AfiseazaGrup(t);
    cout << endl;
    //AfiseazaID(t);
    cout << "X, apoi Y\nCoordonatele incep de la 1 din coltul din stanga sus\n\n";

    t.show();

    while (1) {
        cin >> x >> y;
        t.click(x - 1, y - 1);
    }

    return 0;
}
