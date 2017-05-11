#ifndef TABLA_H
#define TABLA_H

#include "patratel.h"

#include <list>
#include <vector>

class Tabla
{
    int m_Inaltime, m_Latime, m_PatrateVizibile, m_NrBombe, m_GrupNou, m_BombeRamase;
    bool m_Initializat;
    std::vector<std::list<int> > m_Grupuri;

    void updateNumber(int x, int y, int adun);
    void spawnMines(int nr);
    void grupeazaPatratele();
    void floodFill(int x, int y);
    Patratel* getSquare(int ID);

public:
    const bool validCoords(int x, int y);
    void flagMines();
    void revealMines();
    void initializeaza(int x, int y);
    bool m_Over, m_Won;
    const int getBombeRamase();
    std::vector<std::vector<Patratel> > m_Tabla;

    void schimbaSemn(int x, int y); //cicleaza intre steag, semn de intrebare si nimic

    Tabla();
    Tabla(int inaltime, int latime, int nr_bombe);

    void reset(int inaltime, int latime, int nr_bombe);

    const int getInaltime();
    const int getLatime();
    const bool eInitializat();

    void click(int x, int y);
    void clickMijloc(int x, int y);
};

#endif // TABLA_H
