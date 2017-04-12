#ifndef TABLA_H
#define TABLA_H

#include "patratel.h"

#include <list>
#include <vector>

class Tabla
{
    int m_Inaltime, m_Latime, m_PatrateVizibile, m_NrBombe;
    std::vector<std::list<int> > m_Grupuri;

    bool validCoords(int x, int y);
    void updateNumber(int x, int y);
    void spawnMines(int nr);
    void grupeazaPatratele();
    void floodFill(int x, int y, int grup);
    Patratel* getSquare(int ID);

public:
    std::vector<std::vector<Patratel> > m_Tabla;
    //Patratel **m_Tabla;

    static int grup;
    void show();

    Tabla(int inaltime, int latime, int nr_bombe);

    const int getInaltime();
    const int getLatime();

    void click(int x, int y);
};

#endif // TABLA_H
