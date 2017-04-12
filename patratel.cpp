#include "patratel.h"

int Patratel::newID = 0;

Patratel::Patratel()
{
    m_Apasat = 0;
    m_AreBomba = 0;
    m_BombeInJur = 0;
    m_Grup = 0;
    //m_ID = ID;
    //ID++;
}

Patratel::Patratel(int x, int y)
{
    m_X = x;
    m_Y = y;
    //m_ID =
    m_AreBomba = 0;
    m_Apasat = 0;
}

void Patratel::setID()
{
    m_ID = newID;
    newID++;
}
