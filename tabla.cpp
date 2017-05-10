#include "tabla.h"
#include <stdlib.h>
#include <time.h>
#include <iostream> //doar pt debugging

//int Tabla::grup = 1;

Tabla::Tabla()
{
    m_Over = 0;
    m_PatrateVizibile = 0;
    m_Inaltime = 0;
    m_Latime = 0;
}

Tabla::Tabla(int inaltime, int latime, int nr_bombe)
{
    Patratel::resetID();
    m_Over = 0;
    m_GrupNou = 1;
    m_PatrateVizibile = 0;
    m_Tabla.resize(inaltime);

    for (int i = 0; i < inaltime; ++i) {
        m_Tabla[i].resize(latime);

        for (int j = 0; j < inaltime; ++j) {
            m_Tabla[i][j].setID();
        }
    }

    m_Inaltime = inaltime;
    m_Latime = latime;

    m_Grupuri.resize(1);

    for (int i = 0; i < inaltime; ++i) {
        for (int j = 0; j < latime; ++j) {
            m_Tabla[i][j].m_X = j;
            m_Tabla[i][j].m_Y = i;
        }
    }

    spawnMines(nr_bombe);
    grupeazaPatratele();
}

void Tabla::reset(int inaltime, int latime, int nr_bombe)
{
    m_Over = 0;
    m_GrupNou = 1;
    m_PatrateVizibile = 0;
    m_Tabla.clear();
    m_Tabla.resize(inaltime);
    Patratel::resetID();

    for (int i = 0; i < inaltime; ++i) {
        m_Tabla[i].resize(latime);

        for (int j = 0; j < inaltime; ++j) {
            m_Tabla[i][j].setID();
        }
    }

    m_Inaltime = inaltime;
    m_Latime = latime;

    m_Grupuri.clear();
    m_Grupuri.resize(1);

    for (int i = 0; i < inaltime; ++i) {
        for (int j = 0; j < latime; ++j) {
            m_Tabla[i][j].m_X = j;
            m_Tabla[i][j].m_Y = i;
        }
    }

    spawnMines(nr_bombe);

    grupeazaPatratele();
/*
    for (int i = 0; i < m_GrupNou; ++i) {
        //std::cout << "i=" << i;
        std::vector<std::vector<char> > matrice(9, std::vector<char>(9,'_'));
        //std::cout << "Grup " << i << ": ";

        for (std::list<int>::iterator j = m_Grupuri[i].begin(); j != m_Grupuri[i].end(); ++j) {
            //std::cout << "(" << *j - (*j / m_Inaltime * m_Inaltime) << "," << *j / m_Inaltime << ") ";
            matrice[*j / m_Inaltime][*j - (*j / m_Inaltime * m_Inaltime)] = '0' + i;
            //std::cout << "x=" << *j - (*j / m_Inaltime * m_Inaltime) << " y=" << *j / m_Inaltime << "\n";
        }

        for (int j = 0; j < 9; ++j) {
            //std::cout << "j=" << j << "\n";
            for (int k = 0; k < 9; ++k) {
                //std::cout << "k=" << k << "\n";
                std::cout << matrice[j][k] << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }*/
}

bool Tabla::validCoords(int x, int y)
{
    if (x >= 0 && y >= 0 && x < m_Latime && y < m_Inaltime) {
        return 1;
    }

    return 0;
}

const int Tabla::getInaltime()
{
    return m_Inaltime;
}

const int Tabla::getLatime()
{
    return m_Latime;
}

void Tabla::updateNumber(int x, int y)
{
    for (int ydif = -1; ydif <= 1; ++ydif) {
        for (int xdif = -1; xdif <= 1; ++xdif) {
            if ((xdif || ydif) && validCoords(x + xdif, y + ydif)) {
                m_Tabla[y + ydif][x + xdif].m_BombeInJur++;
            }
        }
    }
}

void Tabla::spawnMines(int nr)
{
    m_NrBombe = nr;
    srand(time(NULL));

    for (int i = 0; i < nr; ++i) {
        int pos = rand() % (m_Latime * m_Inaltime);
        int y = pos / m_Latime;
        int x = pos - y * m_Latime;

        if (m_Tabla[y][x].m_AreBomba) {
            i--;
            continue;
        } else {
            m_Tabla[y][x].m_AreBomba = 1;

            updateNumber(x, y);
        }
    }
}

void Tabla::floodFill(int x, int y)
{
    m_Tabla[y][x].m_Grup = m_GrupNou;
    m_Grupuri[m_GrupNou].push_back(m_Tabla[y][x].m_ID);
    //std::cout << "am bagat " << x << " " << y << " in grupul " << m_GrupNou << "\n";

    int x_dif[8] = {1, 1, 0, -1, -1, -1, 0, 1}, y_dif[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    for (int i = 0; i < 8; ++i) {
        if (validCoords(x + x_dif[i], y + y_dif[i])) {
            if (!m_Tabla[y + y_dif[i]][x + x_dif[i]].m_Grup && !m_Tabla[y + y_dif[i]][x + x_dif[i]].m_BombeInJur) {
                floodFill(x + x_dif[i], y + y_dif[i]);
            } else if (m_Tabla[y + y_dif[i]][x + x_dif[i]].m_BombeInJur && m_Tabla[y + y_dif[i]][x + x_dif[i]].m_Grup != m_GrupNou){
                //std::cout << "grup vechi " << m_Tabla[y + y_dif[i]][x + x_dif[i]].m_Grup << " grup nou " << m_GrupNou \
                 << " pt " << x + x_dif[i] << " " << y + y_dif[i] << "\n";
                //un patratel cu bombe in jur poate fi in mai multe grupuri
                m_Tabla[y + y_dif[i]][x + x_dif[i]].m_Grup = m_GrupNou;
                m_Grupuri[m_GrupNou].push_back(m_Tabla[y + y_dif[i]][x + x_dif[i]].m_ID);
                //std::cout << "am bagat " << x + x_dif[i] << " " << y + y_dif[i] << " in grupul " << m_GrupNou << "\n";
            }
        }
    }
}

void Tabla::grupeazaPatratele()
{
    for (int i = 0; i < m_Inaltime; ++i) {
        for (int j = 0; j < m_Latime; ++j) {
            if (!m_Tabla[i][j].m_Grup && !m_Tabla[i][j].m_AreBomba && !m_Tabla[i][j].m_BombeInJur) {
                m_Grupuri.resize(m_Grupuri.size() + 1);
                floodFill(j, i);
                m_GrupNou++;
            }
        }
    }
}

void Tabla::click(int x, int y)
{
    //std::cout << "click pe " << x << " " << y << "\n";
    if (m_Tabla[y][x].m_Apasat || m_Tabla[y][x].m_Steag || m_Tabla[y][x].m_Intrebare) {
        //std::cout << "Ai apasat deja acolo boss\n";
    } else if (m_Tabla[y][x].m_AreBomba) {
        m_Tabla[y][x].m_Apasat = 1;
        m_Tabla[y][x].m_BombaRosie = 1;
        m_Over = 1;
        m_Won = 0;
        //std::cout << "Game over\n";
    } else if (m_Tabla[y][x].m_BombeInJur) {
        m_Tabla[y][x].m_Apasat = 1;
        m_PatrateVizibile++;
    } else {
        //nu merge asa pt ca un patrat cu numar poate fi in mai multe grupuri
        //m_PatrateVizibile = m_PatrateVizibile + m_Grupuri[m_Tabla[y][x].m_Grup].size();

        for (std::list<int>::iterator i = m_Grupuri[m_Tabla[y][x].m_Grup].begin(); i != m_Grupuri[m_Tabla[y][x].m_Grup].end(); ++i) {

            if (!getSquare(*i)->m_Apasat && !getSquare(*i)->m_Steag && !getSquare(*i)->m_Intrebare) {
                m_PatrateVizibile++;
                getSquare(*i)->m_Apasat = 1;
            }
        }
    }

    //std::cout << m_PatrateVizibile << " vizibile\n";

    if (m_PatrateVizibile == m_Latime * m_Inaltime - m_NrBombe) {
        m_Over = 1;
        m_Won = 1;
        //std::cout << "ai castigat\n";
    }
}
/*
void Tabla::show()
{
    for (int i = 0; i < m_Inaltime; ++i) {
        for (int j = 0; j < m_Latime; ++j) {
            if (!m_Tabla[i][j].m_Apasat) {
                std::cout << "# ";
            } else if (m_Tabla[i][j].m_AreBomba) {
                std::cout << "* ";
            } else if (m_Tabla[i][j].m_BombeInJur) {
                std::cout << m_Tabla[i][j].m_BombeInJur << " ";
            } else {
                std::cout << "_ ";
            }
        }

        std::cout << std::endl;
    }
}*/

Patratel* Tabla::getSquare(int ID)
{
    return &(m_Tabla[ID / m_Inaltime][ID - (ID / m_Inaltime * m_Inaltime)]);
}

void Tabla::flagMines()
{
    for (int i = 0; i < m_Inaltime; ++i) {
        for (int j = 0; j < m_Latime; ++j) {
            if (m_Tabla[i][j].m_AreBomba) {
                m_Tabla[i][j].m_Steag = 1;
            }
        }
    }
}

void Tabla::revealMines()
{
    for (int i = 0; i < m_Inaltime; ++i) {
        for (int j = 0; j < m_Latime; ++j) {
            if (m_Tabla[i][j].m_AreBomba && !m_Tabla[i][j].m_Steag) {
                m_Tabla[i][j].m_Apasat = 1;
            } else if (!m_Tabla[i][j].m_AreBomba && m_Tabla[i][j].m_Steag) {
                m_Tabla[i][j].m_Apasat = 1;
                m_Tabla[i][j].m_BombaX = 1;
            }
        }
    }
}

void Tabla::schimbaSemn(int x, int y)
{
    if (m_Tabla[y][x].m_Steag) {
        m_Tabla[y][x].m_Steag = 0;
        m_Tabla[y][x].m_Intrebare = 1;
    } else if (m_Tabla[y][x].m_Intrebare) {
        m_Tabla[y][x].m_Intrebare = 0;
    } else {
        m_Tabla[y][x].m_Steag = 1;
    }
}

void Tabla::clickMijloc(int x, int y)
{
    if (m_Tabla[y][x].m_Apasat && m_Tabla[y][x].m_BombeInJur) {
        int nr_bombe = 0;

        for (int xdif = -1; xdif <= 1; ++xdif) {
            for (int ydif = -1; ydif <= 1; ++ydif) {
                if (validCoords(x + xdif, y + ydif) && !m_Tabla[y + ydif][x + xdif].m_Apasat) {
                    if (m_Tabla[y + ydif][x + xdif].m_Intrebare) {
                        return;
                    }

                    if (m_Tabla[y + ydif][x + xdif].m_Steag) {
                        nr_bombe++;
                    }
                }
            }
        }

        if (nr_bombe != m_Tabla[y][x].m_BombeInJur) {
            return;
        }

        for (int xdif = -1; xdif <= 1; ++xdif) {
            for (int ydif = -1; ydif <= 1; ++ydif) {
                if (validCoords(x + xdif, y + ydif) && !m_Tabla[y + ydif][x + xdif].m_Apasat) {
                    click(x + xdif, y + ydif);
                }
            }
        }
    }
}
