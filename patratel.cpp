#include "patratel.h"

int Patratel::newID = 0;

Patratel::Patratel()
{
    m_Apasat = 0;
    m_AreBomba = 0;
    m_BombeInJur = 0;
    m_BombaRosie = 0;
    m_BombaX = 0;
    m_Steag = 0;
    m_Intrebare = 0;
    m_Grup = 0;
}

Patratel::Patratel(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_AreBomba = 0;
    m_BombaRosie = 0;
    m_BombaX = 0;
    m_Steag = 0;
    m_Intrebare = 0;
    m_Apasat = 0;
}

void Patratel::setID()
{
    m_ID = newID;
    newID++;
}

void Patratel::resetID()
{
    newID = 0;
}
